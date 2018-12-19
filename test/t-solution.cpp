/*
  t-solution.h

  Firing Squad Synhronization Problem:
	test/example for Solution

	Create empty solution
	read a solution from file
	save a solution into file

	file of solution format:
	fitness listOfRules

See for reference:
    Manuel Clergue, Sébastien Verel, Enrico Formenti, 
    An Iterated Local Search to find many solutions of the 6-states Firing Squad Synchronization Problem, 
    Applied Soft Computing, Volume 66, May 2018, Pages 449-461.
    http://www-lisic.univ-littoral.fr/~verel/RESEARCH/firing-squad-synchronization-problem/index.html

 Author: 
  Sebastien Verel, 
  Univ. du Littoral Côte d'Opale, France.
  
*/

#include <iostream>
#include <fstream>

#include <base/solution.h>

int main(int argc, char ** argv) {
	// number of states
	unsigned nbStates = 5;

	// create an empty solution
	Solution x(nbStates);

	// print fitness and vector of rules (compact format)
	std::cout << x << std::endl;

	// read from file
	std::fstream filein("../solution/solution_5.dat", std::ios::in);

	x.readFrom(filein);

	filein.close();

	// print fitness and vector of rules (compact format)
	std::cout << x << std::endl;

	// print all rules (expanded format)
	x.printOnFull(std::cout);

	// save into file
	std::fstream fileout("out.dat", std::ios::out);

	fileout << x << std::endl;

	fileout.close();

}