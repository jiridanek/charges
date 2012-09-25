/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  Jirka Daněk <dnk@mail.muni.cz>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <string>
#include <openbabel/atom.h>

#include <Eigen/Dense>
#include "eemsolver.hpp"
#include "convert.hpp"

using std::string;

using OpenBabel::OBAtom;

using Eigen::MatrixXd;
using Eigen::VectorXd;

/**
 * 
 * FIXME: OpenBabel will eventualy change the indexing of atoms in a molecule
 *  (from 1 based, to 0 based). This method will have to be changed then.
 * 
 */
void EEMsolver::fillEEMparameters() {
    const int offset = 1;
    const unsigned int natoms = this->mol.NumAtoms();
    
    this->mat = MatrixXd(natoms + 1, natoms + 1);
    this->a = VectorXd(natoms + 1);
    
    for (unsigned int i=0; i < natoms; ++i) {
      OBAtom const* fst = this->mol.GetAtom(i + offset);
      const int num = fst->GetAtomicNum();
      const int multiplicity = fst->GetSpinMultiplicity() + 1;
      
      for (unsigned int j=0; j < natoms; ++j) {
	if (i == j) { /* diagonal */
	  try {
	    const double bi = this->params.B(num, multiplicity);
	    const double ai = this->params.A(num, multiplicity);
	    this->mat(i, j) = bi;
	    this->a(i) = -ai;
	  } catch (std::out_of_range const& e) {
	    throw UndefinedEEMparameter("Parametrization for element " 
	      + std::string(OpenBabel::etab.GetSymbol(num)) + " " + stringify<int>(multiplicity) + " missing.");
	  }
	  continue;
	} else {
	  const double dist = ((OBAtom*)fst)->GetDistance(this->mol.GetAtom(j + offset));
	  this->mat(i, j) = this->params.Kappa() / dist;
	}
      }
    }
    
    for (unsigned int i=0; i < natoms; ++i) {
      // last row
      this->mat(natoms, i) = 1;
      
      //last column
      this->mat(i, natoms) = -1;
    }
    
    // bottom-right corner
    this->mat(natoms, natoms) = 0;
}

VectorXd EEMsolver::solve() {
    this->fillEEMparameters();
    VectorXd q = this->mat.colPivHouseholderQr().solve(this->a);
    return q;
}
