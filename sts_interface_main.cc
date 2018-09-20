// Created by Jay Jaewon Yoo

#include "bdd_synthesis.h"
#include "automaton.h"
#include <fstream>
#include <sstream>
#include "timer.h"
#include <bdd.h>
#include <cstring>
#include <string.h>
#include "sts_interface.h"

/********************************************************************/
int main(int argc, const char* argv[]) {
	if( argc != 3 ) {
		std::cout <<" Usage: nbc plant.sts specification\n";
    		exit(1);
  	}
	ifstream ifile(argv[1]);
	if( !ifile ) open_error("plant.sts");

        ifstream ins(argv[2]);
	if( !ins ) open_error("spec");

	sts agv(ifile, ins);
	bdd_synthesis syn(agv);
	
	for(int i = 0; i < 100; i++) cout << endl;
	std::cout << "Beginning computation." << std::endl;
	
	timer t;

	syn.compute();

	std::cout << "\nComputation time is: " << t.diff_time() << " seconds." << std::endl;

	sts_interface stsInterface(agv, &syn);
	stsInterface.beginUI();

	
	return 0;
}
