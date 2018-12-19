/*
  automata.h

  Firing Squad Synhronization Problem:
     Cellular automata computation with objective function, and export into svg.
     Design for 5 states, but could also be used with 6 states.

See for reference:
    Manuel Clergue, Sébastien Verel, Enrico Formenti, 
    An Iterated Local Search to find many solutions of the 6-states Firing Squad Synchronization Problem, 
    Applied Soft Computing, Volume 66, May 2018, Pages 449-461.
    http://www-lisic.univ-littoral.fr/~verel/RESEARCH/firing-squad-synchronization-problem/index.html

 Author: 
  Sebastien Verel, 
  Univ. du Littoral Côte d'Opale, France.
  
*/

#ifndef __automata__h
#define __automata__h

#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
// type of a solution
#include <base/solution.h>

// Glocal constants, but could local somewhere for a better design...

// number of states
const int nbStates  = 5;

// number of digit (including border)
const int nbDigits  = nbStates + 1;
const int nbDigits2 = nbDigits * nbDigits;

// dimension of the tabular of the solution (of type Solution)
const int nbRules   = nbDigits * nbDigits * nbDigits; // = 6^3 = 216

// Notation for states
const int REPOS = 0;
const int GEN = 1;
const int NONGEN = 2;
const int NONGENBIS = 3;
const int FIRE = nbStates - 1;
const int BORD = nbStates;

const int UNUSED = 8;
const int IMPOSSIBLE = 9;


using namespace std;

class Automata {
public:
  /*********************************************
   * constructor
   *
   * input : N : maximum size of the automata
   *
   *********************************************/
  Automata(int _maxSize) : maxSize(_maxSize), maxIteration(2 * _maxSize - 2) {
    int i;
    
    configs = new int*[maxIteration + 1];
    for(i = 0; i <= maxIteration; i++)
      configs[i] = new int[maxSize];
    
    initialConfigurations();
    
    // trace
    trace = NULL;
    
    // for drawing
    width  = 10;
    height = 10;
    //indexList = getUsefullRulesIndexList();
    
  }
  
  /*********************************************
   *
   * destructor
   *
   *********************************************/
  ~Automata() {
    if (trace != NULL)
      delete [] trace;
    
    for(int i = 0; i <= maxIteration; i++)
      delete [] configs[i];

    delete [] configs;
  }

  /*********************************************
   * compute objective function
   *
   * input : x : Solution with the rules of the automata
   *         nMin : minimum size of the firing squad
   *         nMax : maximum size of the firing squad
   *
   * output : the maximum size solved
   *
   *********************************************/
  void eval(Solution & x, int nMax) {
    int nMin = 2; // in this version.

    // nous supposons que la regle est faite pour que la longueur n = nMin fonctionne
    int nbFireTot = nMin;
    
    int k = nMin;
    
    if (trace == NULL) {
      createTrace(x.rules, nMin);
    } else {
      // load from the trace
      for(int i = 0; i < nMin - 1; i++) {
        configs[2 * nMin - 3 - i][i] = trace[i];
      }
    }

    while (nbFireTot == k && k <= nMax) {
      k++;
      nbFireTot = evolQuick(x.rules, k);
      //if (k % 100 == 0 && k == nbFireTot) std::cout << " " << k << std::endl;
    }
    
    if (k == nMin)
      return x.fitness(0);
    else
      return x.fitness(k - 1);
  }
  

  
  void marcheAleatoire(Solution & x, int nMax, int maxIter){ 
	  eval(x, nMax);
	  int best = x.fitness();
	  vector<int> v(x.indices);
	  Solution temp(x); 
	  eval(temp, maxSize);
	  int randomIndice = 0, randomValue = 0;
	  for(int i = 0; i < maxIter; i++){
		randomIndice = rand() % v.size();
		randomValue = rand() % FIRE;
		x.rules[v.at(randomIndice)] = randomValue;
		eval(x, nMax);
	
		if(x.fitness() > temp.fitness()){
			temp = x; }
		}
	 x = temp; 
	 }
	

	
	
	void hillClimberFirst(Solution & solution,int nMax,int maxIter){
		srand(time(NULL));	
		eval(solution,nMax);
			
		//Solution s (5);
		Solution s = solution;
		//srand(time(NULL));	
		//int * tmpRule = x.rules;
		//eval(solution, nMax);
		int fitness ;//= solution.fitness();
		int fitNN = 0;
		int nextIndice = 0, nextVal = 0;		
		for(int i = 0; i < maxIter; i++){
			s= solution;
			//std::vector< std::pair< int,int > > worse;
			nextIndice = rand() % solution.indices.size();
			nextVal = /*rand() % 4;*/( s.rules[solution.indices.at(nextIndice)] + (rand() % (3-1) +1))% 4;
			/*do{
				 nextVal = rand()%FIRE ;
				 
			 }while(nextVal == s.rules[solution.indices.at(nextIndice)]);
		*/
			s.rules[solution.indices.at(nextIndice)] = nextVal;
			//std::cout<<" oui : " << nextVal << std::endl;
			eval(s,nMax);
			//cout << "S:" << s << endl;
			//cout << "X:" << solution << endl;
			//fitNN = s.fitness();
			if(s.fitness()>= solution.fitness()){ //first improve
				//cout << "S:" << s << endl;
				//cout << fitNN;
				solution= s;
				if(s.fitness()> solution.fitness()){cout << solution ;}
				//solution.rules[solution.indices[nextIndice]] = nextVal;
				//x.rules = s.rules;
				//std ::cout << "fitnessNN " << fitNN ;
				//x.fitness(s.fitness());
				//x = s ;
			}	
		}
	}
	
	
	void iteratedLocalSearch(Solution & solution,int nMax,int maxIter){
		srand(time(NULL));
		int randIndice, randVal;
		//Solution s = solution;
		for(int i = 0; i < maxIter; i++){
			hillClimberFirst(solution,nMax,10000);
						eval(solution,nMax);

			for(int i = 0; i < 10; i++){
				randIndice = rand() % solution.indices.size();
				randVal = /*rand() % 4;*/( solution.rules[solution.indices.at(randIndice)] + (rand() % (3-1) +1))% 4;
				solution.rules[randIndice] = randVal;
			}
			
			hillClimberFirst(solution,nMax,10000);
						eval(solution,nMax);

			
		}
		
	}
	

		
			
	

  
  
  /*********************************************
   * export in svg format
   *
   * input :
   *.   x : solution
   *.   N : size of the firing squad (number of cells)
   *.   all : true: from 2 to N; false: only N
   *
   *********************************************/
  void exportSVG(Solution & x, int N, const char * fileName, bool all = false) {
    print(x.rules, N, fileName, all);
  }

private:
  // space-time diagram
  int ** configs;
  
  // maximum size of the cellular automata
  int maxSize;
  
  // maximum number of evaluations
  int maxIteration;
  
  // trace : left diagonal used in evolQuick
  int * trace;
  
  // for svg output
  int width ;
  int height;
  
  
  /*********************************************
   * configurations initiale de l'automate
   *
   *
   *********************************************/
  void initialConfigurations() {
    // repos
    for(int i = 0; i < maxSize - 1; i++)
      for(int j = 1 + i; j < maxSize; j++)
        configs[i][j] = REPOS;
    
    // general
    configs[0][0] = GEN;
  }
  
  
  /*********************************************
   * evolution of the automate from initial configuration
   * to the first time of firing
   *
   * input : regles : rules of the automata
   *         N : size of the automata
   *         nbIter : maximum number of iterations
   *
   * output : number of the firing after 2N-2 iteration
   *          0 else
   *********************************************/
  int evol(int * regles, int N) {
    int nbIter = 2 * N - 2;
    
    if (nbIter > maxIteration)
      nbIter = maxIteration;
    
    // initialise l'automate
    //    initialConfiguration(N);
    
    // nombre d'etats feu
    int nbFire = 0;
    
    // valeur de la regle locale
    int r;
    
    int i, t;
    
    t = 1;
    
    // les premieres iterations en utilisant la propriete de quiescence 000 -> 0
    while (t < N - 1 && nbFire == 0) {
      // premiere cellule a gauche
      r = regles[ BORD * nbDigits2 + configs[t-1][0] * nbDigits + configs[t-1][1] ];
      if (r == FIRE)
        nbFire++;
      configs[t][0] = r;
      
      // cellules du centre
      for(i = 1; i < t; i++) {
        r = regles[ configs[t-1][i-1] * nbDigits2 + configs[t-1][i] * nbDigits + configs[t-1][i+1] ];
        if (r == FIRE)
          nbFire++;
        configs[t][i] = r;
      }
      
      // cellule a droite avant REPOS
      r = regles[ configs[t-1][t - 1] * nbDigits2 + configs[t-1][t] * nbDigits + REPOS ];
      if (r == FIRE)
        nbFire++;
      configs[t][t] = r;
      
      t++;
    }
    
    // les iterations suivantes
    while (t <= nbIter && nbFire == 0) {
      // premiere cellule a gauche
      r = regles[ BORD * nbDigits2 + configs[t-1][0] * nbDigits + configs[t-1][1] ];
      if (r == FIRE)
        nbFire++;
      configs[t][0] = r;
      
      // cellules du centre
      for(i = 1; i < N - 1; i++) {
        r = regles[ configs[t-1][i-1] * nbDigits2 + configs[t-1][i] * nbDigits + configs[t-1][i+1] ];
        if (r == FIRE)
          nbFire++;
        configs[t][i] = r;
      }
      
      // cellule a droite
      r = regles[ configs[t-1][N - 2] * nbDigits2 + configs[t-1][N - 1] * nbDigits + BORD ];
      if (r == FIRE)
        nbFire++;
      configs[t][N - 1] = r;
      
      t++;
    }
    
    // nombre de fusiliers apres 2N-2 iterations
    if (t == 2 * N - 2 + 1)
      return nbFire;
    else
      return 0;
  }
  
  /*********************************************
   * evolution of the automate from initial configuration
   * to the first time of firing.
   * "Fast" computation in n(n+3)/2 for size n.
   *
   *   IMPORTANT:
   *   we suppose that the size N-1 (i.e. configs) 
   *   has been computed (and synchronized the firing squad)
   *
   * input : regles : rules of the automata
   *         N : size of the automata
   *
   * output : number of the firing after 2N-2 iteration
   *          0 else
   *********************************************/
  int evolQuick(int * regles, int N) {
    int nbIter = 2 * N - 2;
    
    if (nbIter > maxIteration)
      nbIter = maxIteration;
    
    // nombre d'etats feu
    int nbFire = 0;
    
    // valeur de la regle locale
    int r;
    
    int i, t;
    
    // les premieres iterations en utilisant la propriete de quiescence 000 -> 0
    // t = N - 2;
    i = N - 2;
    r = regles[ configs[N-3][i-1] * nbDigits2 + REPOS * nbDigits + REPOS ];
    if (r == FIRE)
      return 0;
    configs[N-2][i] = r;
    configs[N-2][N-1] = 0;

    // les iterations suivantes jusqu'au bord gauche
    t = N - 1;
    while (t < 2 * N - 4 && nbFire == 0) {
      // cellules du centre
      for(i = 2 * N - 4 - t; i < N - 1; i++) {
        r = regles[ configs[t-1][i-1] * nbDigits2 + configs[t-1][i] * nbDigits + configs[t-1][i+1] ];
        if (r == FIRE)
          return 0; //nbFire++;
        configs[t][i] = r;
      }
      
      // cellule a droite
      r = regles[ configs[t-1][N - 2] * nbDigits2 + configs[t-1][N - 1] * nbDigits + BORD ];
      if (r == FIRE)
        return 0; //nbFire++;
      configs[t][N - 1] = r;
      
      t++;
    }

    // les 3 dernieres iterations
    while (t <= nbIter && nbFire == 0) {
      // premiere cellule a gauche
      r = regles[ BORD * nbDigits2 + configs[t-1][0] * nbDigits + configs[t-1][1] ];
      if (r == FIRE)
        nbFire++;
      configs[t][0] = r;

      // cellules du centre
      for(i = 1; i < N - 1; i++) {
        r = regles[ configs[t-1][i-1] * nbDigits2 + configs[t-1][i] * nbDigits + configs[t-1][i+1] ];
        if (r == FIRE)
          nbFire++;
        configs[t][i] = r;
      }
      
      // cellule a droite
      r = regles[ configs[t-1][N - 2] * nbDigits2 + configs[t-1][N - 1] * nbDigits + BORD ];
      if (r == FIRE)
        nbFire++;
      configs[t][N - 1] = r;
      
      t++;
    }
    
    // nombre de fusiliers apres 2N-2 iterations
    if (t == 2 * N - 2 + 1)
      return nbFire;
    else
      return 0;
  }
  
  /*********************************************
   * compute objective function
   *
   * input : regles : rules of the automata
   *         n : maximum of automata size
   *         nbIter : maximum number of iterations
   *
   * output : the maximum size solved
   *
   *********************************************/
  int evalSlow(int * regles, int n) {
    //int nbFireTot = evol(regles, 2);
    // nous supposons que la regle est faite pour que la longueur n=2 fonctionne
    int nbFireTot = 2;
    
    int k = 2;
    
    while (nbFireTot == k && k <= n) {
      k++;
      nbFireTot = evol(regles, k);
    }
    
    if (k == 2)
      return 0;
    else
      return k - 1;
  }
  
  /*********************************************
   * create the trace for evolQuick
   *
   * input : regles : rules of the automata
   *         nMin : minimum size of the firing squad
   *
   * output : create the trace which is the "left" diagonal
   *          of the time-space diagram
   *
   *********************************************/
  void createTrace(int * regles, int nMin) {
    // size 2
    if (nbStates == 5) {
      configs[1][0] = 1;
      configs[1][1] = 1;
    } else { // nbStates = 6 for example
      configs[1][0] = 2;
      configs[1][1] = 2;
    }
 
    int k = 3;
    
    int nbFireTot;
    while (k <= nMin) {
      nbFireTot = evalSlow(regles, k);
      k++;
    }
    
    trace = new int[nMin-1];
    for(int i = 0; i < nMin - 1; i++) {
      trace[i] = configs[2 * nMin - 3 - i][i];
    }
  }
  
  /*********************************************
   * evolution of the automate from initial configuration
   * to the first time of firing
   * This function is used in svg export because some "unused" rule can be handled
   *
   * input : regles : rules of the automata
   *         N : size of the automata
   *         nbIter : maximum number of iterations
   *
   * output : number of the firing after 2N-2 iteration
   *          0 else
   *********************************************/
  int evolWithUnused(int * regles, int N) {
    int nbIter = 2 * N - 2;
    
    if (nbIter > maxIteration)
      nbIter = maxIteration;
    
    // initialise l'automate
    //    initialConfiguration(N);
    
    // nombre d'etats feu
    int nbFire = 0;
    
    // valeur de la regle locale
    int r;
    
    int i, t;
    int g, c, d;
    int valueByDefault = 0; // when unused
    
    t = 1;
    
    // les iterations suivantes
    while (t <= nbIter && nbFire == 0) {
      // premiere cellule a gauche
      c = configs[t-1][0];
      if (c == UNUSED)
        c = valueByDefault;
      d = configs[t-1][1];
      if (d == UNUSED)
        d = valueByDefault;
      
      r = regles[ BORD * nbDigits2 + c * nbDigits + d ];
      if (r == FIRE)
        nbFire++;
      configs[t][0] = r;
      
      // cellules du centre
      for(i = 1; i < N - 1; i++) {
        g = configs[t-1][i-1];
        if (g == UNUSED)
          g = valueByDefault;
        c = configs[t-1][i];
        if (c == UNUSED)
          c = valueByDefault;
        d = configs[t-1][i+1];
        if (d == UNUSED)
          d = valueByDefault;
        
        r = regles[ g * nbDigits2 + c * nbDigits + d ];
        if (r == FIRE)
          nbFire++;
        configs[t][i] = r;
      }
      
      // cellule a droite
      g = configs[t-1][N - 2];
      if (g == UNUSED)
        g = valueByDefault;
      c = configs[t-1][N - 1];
      if (c == UNUSED)
        c = valueByDefault;
      r = regles[ g * nbDigits2 + c * nbDigits + BORD ];
      if (r == FIRE)
        nbFire++;
      configs[t][N - 1] = r;
      
      t++;
    }
    
    // nombre de fusiliers apres 2N-2 iterations
    if (t == 2 * N - 2 + 1)
      return nbFire;
    else
      return 0;
  }
  
	

  
  
  /*********************************************
   * export in svg format
   *
   * input :
   *.   regles : solution
   *.   N : size of the firing squad (number of cells)
   *.   all : true: from 2 to N; false: only N
   *
   *********************************************/
  void print(int * regles, int N, const char * fileName, bool all = false) {
    fstream file(fileName, ios::out);
    
    if (file) {
      
      int nFire ;
      int nbIter ;
      int nInit;
      
      if (all) {
        nInit = 2;
        headSVG(file, (N * (N + 1) / 2 + (N - 1) * 2) * width, (2 * N) * height);
      } else {
        nInit = N;
        headSVG(file, (N + 1) * width, (2 * N) * height);
      }
      
      // head of the file
      int x = 0;
      int y = 0;
      
      for(int n = nInit; n <= N; n++) {
        nFire = evolWithUnused(regles, n);
        
        nbIter= 2 * n - 2;
        
        if (nbIter > maxIteration)
          nbIter = maxIteration;
        
        // drawing
        bool fire = false;
        int i, j;
        for(i = 0; i <= nbIter && !fire; i++) {
          for(j = 0; j < n; j++) {
            cellSVG(file, x + j * width, y + i * height, configs[i][j]);
            
            fire = fire || (configs[i][j] == FIRE);
          }
        }
        
        x += (n+2) * width;
      }
      
      // bottom of file
      file << "</g> </svg>" << endl;
      
      file.close();
    } else
      cerr << "print: impossible to open file " << fileName << endl;
  }
  
  void headSVG(fstream & file, int w, int h) {
    file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    << endl
    << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\" \"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">"
    << endl << endl
    << "<svg" << endl
    << "xmlns=\"http://www.w3.org/2000/svg\"" << endl
    << "xmlns:xlink=\"http://www.w3.org/1999/xlink\"" << endl
    << "xmlns:ev=\"http://www.w3.org/2001/xml-events\"" << endl
    << "version=\"1.1\"" << endl
    << "baseProfile=\"full\"" << endl
    << "width=\"" << w <<"\" height=\"" << h << "\">" << endl // viewBox="0 0 800 400">
    << "<g stroke-width=\"1px\" stroke=\"black\" fill=\"white\">" << endl;
  }
  
  void cellSVG(fstream & file, int x, int y, int state) {
    file << "<rect "
    << "width=\""  << width   << "\" "
    << "height=\"" << height  << "\" "
    << "x=\"" << (x + width)  << "\" "
    << "y=\"" << (y + height) << "\" "
    << "fill=\"";
    
    if (state > FIRE) // == UNUSED
      file << "grey" ; 
    else if (state == REPOS)
      file << "white" ; 
    else if (state == FIRE)
      file << "red"  ; 
    else if (state == GEN)
      file << "blue" ; 
    else if (state == NONGEN)
      file << "yellow" ; 
    else if (state == NONGENBIS)
      file << "green" ; 
    else if (state == NONGENBIS + 1)
      file << "violet" ; 
    else if (state == NONGENBIS + 2)
      file << "orange" ; 
    else
      file << "grey" ; 

    file << "\"/>" << endl;
  }
  
  void cellSVG_BW(fstream & file, int x, int y, int state) {
    file << "<rect " << "width=\""  << width   << "\" "<< "height=\"" << height  << "\" " << "x=\"" << (x + width)  << "\" " << "y=\"" << (y + height) << "\" "
    << "fill=\"";
    
    if (state > FIRE) // == UNUSED
      file << "grey" ; 
    else if (state == REPOS)
      file << "white" ; 
    else if (state == FIRE)
      file << "black"  ; 
    else if (state == GEN)
      file << "blue" ; 
    else if (state == NONGEN)
      file << "yellow" ; 
    else if (state == NONGENBIS)
      file << "green" ; 
    else if (state == NONGENBIS + 1)
      file << "violet" ; 
    else if (state == NONGENBIS + 2)
      file << "orange" ; 
    else
      file << "grey" ; 

    file << "\"/>" << endl;
  }
  
  /*********************
   *
   * Print the cells in text mode
   *
   **********************/
  void print(int n) {
    int maxIter = 2 * n - 1;
    for(int i = 0; i < maxIter; i++) {
      for(int j = 0; j < n; j++) {
        if (configs[i][j] == REPOS)
          cout << "0" ;
        else
          cout << configs[i][j] ;
      }
      cout << endl ;
    }
  }

};
#endif
