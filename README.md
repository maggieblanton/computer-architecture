# COMP4300- Project3

Maggie Blanton, mab0146

Project Overview: 
  
  The gprSim file developed in Project 2 was extended into a a simulator for a 
  scalar pipelined architecture.
  
  This program takes an input of 'lab3a.s', 'lab3b.s', or 'lab3c.s' and outputs 
  results accordingly. Please note, if your test file name is 'lab3b.s', you will
  also be promoted to input a palindrome test string. 
  
Instructions to compile and run on Linux machine:

    1. Download project3.tar.gz.
    2. Type 'tar -xvf project3.tar' to extract the file to the current directory.
    3. Change directory to downloaded folder. 
    4. Compile pipeSim: g++ -std=c++11 pipeSim.cpp -o pipeSim
    5. Run pipeSim: ./pipeSim
    6. If necessary, type in a string for testing when prompted (ie. anna).
  
  Instructions to view output: 
  
    1. Open the result.txt file created.
 
    Note: Output is also printed in the terminal. 
  
 Problems during development: 
  
    (Resolved)
    Upon completion, gprSim.cpp both ran without errors in VS Code, but threw a 'Segmentation fault
    (core dumped)' error while running on the tux machines. To fix this, the program was ended after the final 'syscall' 
    instruction.
 
  

