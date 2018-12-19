/*
  t-exportSVG.cpp

  Firing Squad Synhronization Problem:
    test/example to export into svg format file the space-time diagram

    Create Automata
    Export the solution into 6 files of svg format


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
#include <base/automata.h>

int main(int argc, char ** argv) {

    int maxSize = 30;

    Automata ca(maxSize);

    // number of states
    unsigned nbStates = 5;

    // create an empty solution
    Solution x(nbStates);

    // read from file
    std::fstream filein("../solution/solution_5.dat", std::ios::in);

    x.readFrom(filein);

    std::cout << x << std::endl;

    filein.close();

    // export space-time diagram into the svg file
    char name[256];
    for(unsigned i = 2; i <= 6; i++) {
        sprintf(name, "s_%d.svg", i);
        ca.exportSVG(x, i, name);
    }
}