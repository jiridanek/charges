/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  jirka <email>

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


#ifndef EEMPARAMS_HPP
#define EEMPARAMS_HPP
#include <istream>
#include <map>
#include <utility>

#include <Eigen/Dense>

using std::istream;

using Eigen::VectorXd;

class EEMparams
{

  std::map< std::pair<int, int>, std::pair<double, double> > ab;
  double kappa;
  friend istream& operator>> (istream &in, EEMparams &params);
  
public:
    EEMparams();
    EEMparams(std::string const& s);
//     EEMparams(char const * str);
    double B(int num, int valence) const;
    double A(int num, int valence) const;
    std::pair<double, double> AB(int num, int valence) const;
    double Kappa() const;
    virtual ~EEMparams();
};

#endif // EEMPARAMS_HPP
