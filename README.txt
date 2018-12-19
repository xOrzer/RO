See AUTHORS and COPYRIGHT for the list of contributors.

********************************************************************************************************
Firing Squad Synchronization Problem:
     Design for 5 states, but could also be used for 6 states.

See for reference:
    Manuel Clergue, Sébastien Verel, Enrico Formenti, 
    An Iterated Local Search to find many solutions of the 6-states Firing Squad Synchronization Problem, 
    Applied Soft Computing, Volume 66, May 2018, Pages 449-461.
    http://www-lisic.univ-littoral.fr/~verel/RESEARCH/firing-squad-synchronization-problem/index.html

********************************************************************************************************
Compile:

git clone https://gitlab.com/verel/firing-squad-synchronization-problem.git

cd firing-squad-synchronization-problem
mkdir build
cd build
cmake ../test
make

********************************************************************************************************
Execute some tests:

./t-solution
./t-eval
./t-exportSVG

