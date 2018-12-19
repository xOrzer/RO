/*
  solution.h

  Firing Squad Synhronization Problem:
    solution representation

See for reference:
    Manuel Clergue, Sébastien Verel, Enrico Formenti, 
    An Iterated Local Search to find many solutions of the 6-states Firing Squad Synchronization Problem, 
    Applied Soft Computing, Volume 66, May 2018, Pages 449-461.
    http://www-lisic.univ-littoral.fr/~verel/RESEARCH/firing-squad-synchronization-problem/index.html

 Author: 
  Sebastien Verel, 
  Univ. du Littoral Côte d'Opale, France.
  
*/

#ifndef _solution_h
#define _solution_h

#include <string>

#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <vector>
#include <time.h>
class Solution {
public:
  // rules vector
  int * rules;
  // size of the rules vector
  unsigned nbRules ;
  
  std::vector<int> indices;
  
  Solution(unsigned nbStates) {
    unsigned nbDigits = nbStates + 1; // nbStates + BORDER
    
    nbRules = nbDigits * nbDigits * nbDigits ;
    rules = new int[nbRules];

    int FIRE = nbStates - 1;
    int BORD = nbStates;

    int g, c, d, j;
    for(int i = 0; i < nbRules; i++) {
      d = i % nbDigits;
      j = i / nbDigits;
      c = j % nbDigits;
      g = j / nbDigits;
      
      if ( ! ( (g < FIRE && c < FIRE && d < FIRE) ||
              (g == BORD && c < FIRE && d < FIRE) ||
              (g < FIRE && c < FIRE && d == BORD) )
          )
        rules[ i ] = 9; // impossible rules
      else
        rules[ i ] = 0;
    }
    
    fitnessValue = 0;
    invalidValue = true;
    indices = getUsefullRulesIndexList();
  }
  
  /*

    Copy constructor

  */
  Solution(const Solution & _solution) {
    nbRules = _solution.nbRules;
    rules = new int[nbRules];
    for(int i = 0; i < nbRules; i++)
      rules[i] = _solution.rules[i];
    
    invalidValue = _solution.invalid();

    if (invalidValue)
      fitnessValue = 0;
    else
      fitnessValue = _solution.fitness();
  }
  
  /*
    Equal
  */
  Solution& operator=(const Solution & _solution) {
    nbRules = _solution.nbRules;
    for(int i = 0; i < nbRules; i++)
      rules[i] = _solution.rules[i];
    
    invalidValue = _solution.invalid();
    
    if (invalidValue)
      fitnessValue = 0;
    else
      fitnessValue = _solution.fitness();

    return *this;
  }
  
  ~Solution() {
    delete [] rules;
  }
  static int indexVal(int x,int y,int z){
		return (36*x+6*y+z);
		
	}
	
  static std::vector<int> getUsefullRulesIndexList(){
		std::vector<int> res;
		
		for (int g = 0; g<6 ; g++){
			for (int c = 0; c< 6; c++){
				for (int d = 0; d< 6; d++){
					if (g == 4 || c == 4 || d == 4) continue; //break ? // si feu pas de suite
					if (c == 5 || (g == 5 && d == 5)) continue; //regles BORDS
					if (g == 0 && c == 0 && d == 0) continue;
					if (g == 1 && c == 1 && d == 1) continue;
					if (g == 5 && c == 0 && d == 0) continue;
					if (g == 0 && c == 0 && d == 5) continue;
					if (g == 5 && c == 1 && d == 1) continue;
					if (g == 1 && c == 1 && d == 0) continue;
					if (g == 5 && c == 1 && d == 0) continue;
					if (g == 1 && c == 0 && d == 5) continue;
					//std::cout << "|||" << indexVal(g,c,d) << "|||" ;
					res.push_back(indexVal(g,c,d));					
				}
			}
		}
			
			//std::cout<<"blyat" << res.size() << "fin de blyat";
			

		
		return res;
	}
		void setRandomRules(){
			
			for (int i = 0 ; i <  216 ; i++){
				rules[i] = rand() % 4;
			} 
			
			
			setFixedRules();
		}
		
		void setFixedRules(){
			//000 => 0
			indices[indexVal(0,0,0)] = 0 ;
			//111 => 4
			indices[indexVal(1,1,1)] = 4 ;
			//B00 => 0
			indices[indexVal(5,0,0)] = 0;
			//00B => 0
			indices[indexVal(0,0,5)] = 0;
			//B11 => 4
			indices[indexVal(5,1,1)] = 4;
			//11B => 4
			indices[indexVal(1,1,0)] = 4;
			//B10 => 1
			indices[indexVal(5,1,0)] = 1;
			//10B => 1
			indices[indexVal(1,0,5)] = 1;
		}
  
  
  int fitness() const {
    return fitnessValue;
  }
  
  void fitness(int _val) {
    fitnessValue = _val;
    invalidValue = false;
  }
  
  void invalidate() {
    invalidValue = true;
  }
  
  bool invalid() const {
    return invalidValue;
  }
  
  void printOn(std::ostream& _os) const {
    if (invalid()) {
      _os << "I ";
    }
    else
    {
      _os << fitnessValue << ' ';
    }

    // becareful, work only for 5 or 6 states, sorry...
    int nbStates;
    if (nbRules == 216)
      nbStates = 5;
    else
      nbStates = 6;

    unsigned nbDigits = nbStates + 1; // nbStates + BORDER
    int FIRE   = nbStates - 1;
    int BORD   = nbStates;
    int UNUSED = 8;

    int g, c, d, j;
    for(int i = 0; i < nbRules; i++) {
      d = i % nbDigits;
      j = i / nbDigits;
      c = j % nbDigits;
      g = j / nbDigits;
      
      if ( (g < FIRE && c < FIRE && d < FIRE) ||
              (g == BORD && c < FIRE && d < FIRE) ||
              (g < FIRE && c < FIRE && d == BORD) 
          )
        if (rules[i] != UNUSED)
          _os << rules[i] ;
    }
  }

/*********************
 *
 * Load rules from a in stream
 *
 **********************/
void readFrom(std::istream & _in) {
  // fitness value
  std::string f;
  _in >> f ;

  if (f.compare("I") == 0) {
    invalidate();
  } else {
    //fitness(std::stringstream ss((f));
  }

  // vector of rules
  std::string v;
  _in >> v;

  // BECAREFULL only work for 5 or 6 states !!!!
  int nbStates;
  if (v.size() == 96) { // 96 = 4^3 + 2 * 4^2
    nbStates = 5; 
  } else {
    if (v.size() == 175) // 175 = 5^3 + 2 * 5^2
      nbStates = 6;
    else
      std::cerr << "readFrom: unknown number of states (should be 5 or 6)." << std::endl;
  }

  if (rules == NULL)
    rules = new int[nbRules];

  unsigned nbDigits = nbStates + 1; // nbStates + BORDER
  int FIRE = nbStates - 1;
  int BORD = nbStates;

  int value;
  int g, c, d, j, k;
  k = 0;
  for(unsigned i = 0; i < nbRules; i++) {
    d = i % nbDigits;
    j = i / nbDigits;
    c = j % nbDigits;
    g = j / nbDigits;
    
    if ( (g < FIRE && c < FIRE && d < FIRE) ||
        (g == BORD && c < FIRE && d < FIRE) ||
        (g < FIRE && c < FIRE && d == BORD) ) {
      rules[i] = v[k] - '0' ;
      //cout << v[k] << "," << rules[i] << " " ;
      k++;
    } else
      rules[i] = 9 ; //IMPOSSIBLE;
    //      cout << rules[i] << " " ;
  }

}

  /*
    The output format is the basic one. One each, one rule with this format:
    leftState centerState rightState newCenterState
  */
  void printOnFull(std::ostream& _os) const {
    // becareful, work only for 5 or 6 states, sorry...
    int nbStates;
    if (nbRules == 216)
      nbStates = 5;
    else
      nbStates = 6;

    unsigned nbDigits = nbStates + 1; // nbStates + BORDER
    int FIRE = nbStates - 1;
    int BORD = nbStates;
    int UNUSED = 8;
    
    int g, c, d, j;
    for(int i = 0; i < nbRules; i++) {
      d = i % nbDigits;
      j = i / nbDigits;
      c = j % nbDigits;
      g = j / nbDigits;
      
      if ( (g < FIRE && c < FIRE && d < FIRE) ||
              (g == BORD && c < FIRE && d < FIRE) ||
              (g < FIRE && c < FIRE && d == BORD) 
          )
        if (rules[i] != UNUSED)
          _os << g << " " << c << " " << d << " " << rules[i] << std::endl;
    }
  }

protected:
  int fitnessValue;
  bool invalidValue;

};

std::ostream & operator<<(std::ostream& _os, const Solution & _solution) {
  _solution.printOn(_os);
  return _os;
}

#endif
