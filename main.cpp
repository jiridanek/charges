#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include <openbabel/obconversion.h>
#include <openbabel/mol.h>

#include <Eigen/Dense>

#include "eemparams.hpp"
#include "eemsolver.hpp"

using std::cout;
using std::endl;
using std::string;

namespace po = boost::program_options;

using OpenBabel::OBConversion;
using OpenBabel::OBMol;
using OpenBabel::OBAtom;

using OpenBabel::OBMolAtomIter;

int main(int argc, char **argv) {
  po::options_description generic("Generic options");
	generic.add_options()
		("version", "print version string")
		("help", "produce help message")    
    ;
	
	po::options_description config("Configuration");
	config.add_options()
		("eem", po::value<string>(), "path to eem params file")
	;
	
	po::options_description hidden("Hidden options");
	hidden.add_options()
		("input-file", po::value<string>(), "path to a mol file")
	;
	
	po::options_description cmdline_options;
	cmdline_options.add(generic).add(config).add(hidden);
	
	po::options_description visible("Allowed options");
	visible.add(generic).add(config);

	po::positional_options_description p;
	p.add("input-file", -1);

	po::variables_map vm;
	try {
		po::store(po::command_line_parser(argc, argv).
          options(cmdline_options).positional(p).run(), vm);
	} catch (po::error e) {
		cout << "Wrong command line option:" << std::endl
			 << e.what() << std::endl;
		return 1;
	}
 	po::notify(vm);    

	if (vm.count("help")) {
		cout << visible << "\n";
		return 1;
	}

	if (!vm.count("eem")) {
		cout << "eem path was not set.\n";
		return 1;
	}
	
	if (!vm.count("input-file")) {
		cout << "input path was not set.\n";
		return 1;
	}
  
  OBConversion conv;
  conv.SetInFormat("MOL");
  OBMol mol;
  
  conv.ReadFile(&mol, vm["input-file"].as<string>());
  
  cout << mol.GetFormula() << endl;
  
  EEMparams par(vm["eem"].as<string>());
  
  EEMsolver sol(mol, par);
  VectorXd q = sol.solve();
  cout << q << endl;
  
  return 0;
}
