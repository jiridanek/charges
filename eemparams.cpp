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

#include <cstdio>
#include <fstream>

#include <openbabel/mol.h>
#include <iterator>
#include <vector>
#include <iostream>
#include "eemparams.hpp"
#include "convert.hpp"
#include "invalidparamline.hpp"
std::vector<std::string> tokenize(std::string line);

EEMparams::EEMparams() {}
EEMparams::EEMparams(const std::string& s) {
  std::ifstream file(s.c_str());
  file >> *this;
  file.close();
}


double EEMparams::A(int num, int valence) const {
  return this->ab.at(std::pair<int, int>(num, valence)).first;
}

double EEMparams::B(int num, int valence) const {
  return this->ab.at(std::pair<int, int>(num, valence)).second;
}

std::pair< double, double > EEMparams::AB(int num, int valence) const {
  return this->ab.at(std::pair<int, int>(num, valence));
}


double EEMparams::Kappa() const {
    return this->kappa;
}

istream& operator>>(istream& in, EEMparams& params) {
    int lineno = 1;
    std::string line;
    std::getline(in, line);
    std::vector<std::string> tokens = tokenize(line);
    if (tokens.size() != 2) {
      throw InvalidParamLine("Wrong numer of parameters on line " + lineno);
    }
    try {
      params.kappa = convertTo<double>(tokens.at(1));
      lineno++;
      while(in.good()) {
	std::getline(in, line);
	tokens = tokenize(line);

	if(tokens.size() != 4) {
	  throw InvalidParamLine("Wrong numer of parameters on line " + lineno);
	}
	
	std::string symbol = tokens.at(0);
	int maxmultiplicity = convertTo<double>(tokens.at(1));
	double a = convertTo<double>(tokens.at(2));
	double b = convertTo<double>(tokens.at(3));

	
	int iso;
	int num = OpenBabel::etab.GetAtomicNum(symbol, iso);
	if (0 == num) {
	  throw InvalidParamLine("Wrong element symbol " + symbol + " on line " + stringify<int>(lineno));
	}
	params.ab.insert(std::pair< std::pair<int, int>, std::pair<double,double> >(std::pair<int, int>(num, maxmultiplicity), std::pair<double, double>(a, b)));

	lineno++;
      }
    } catch(BadConversion &e) {
      throw InvalidParamLine("Parameter in a wrong format on line " + lineno);
    }
    
    return in;
}


EEMparams::~EEMparams() {

}

/* http://stackoverflow.com/questions/236129/how-to-split-a-string-in-c */
std::vector<std::string> tokenize(std::string line) {
    std::stringstream ss(line);    
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(ss)
    , std::istream_iterator<std::string>()
    , std::back_inserter<std::vector<std::string> >(tokens));
    return tokens;
}