# COMP4300- Project1

Maggie Blanton, mab0146

Project Overview: 
  Program reads ASCII file consisting of .data and .text. Parses accordingly and stores in memory. 
  The stack-based simulator uses PUSH, POP, ADD, MULT, and END commands to replicate quadratic evaluator functionality. 
  The accumulator-based simulator uses LOAD, STO, ADD, MULT, and END commnds to replicate quadratic evaluator functionality.

Instructions to compile and run on Linux machine:

  1. Download project1.tar.gz.
  2. Decompress downloaded tarball. 
  3. Change directory to downloaded folder. 
  4. Compile stackSim: g++ -std=c++11 stackSim.cpp -o stackSim
  5. Compile accumSim, run: g++ -std=c++11 accumSim.cpp -o accumSim
  6. Run stackSim: ./stackSim
  7. Run accumSim: ./accumSim
  
 Problems during development: 
 
  Compiling with 'g++ stackSim.cop' or 'g++ accumSim.cpp' results in a "'to-string' is not a member of 'std' error. 
  Fixed by specifying C++ version of 11 with command 'g++ -std=c++11...'. 

