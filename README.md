# COMP4300- Project2

Maggie Blanton, mab0146

Project Overview: 
  
  The accumSim file developed in Project 1 was extended into a GPR machine. It runs
  different instructions in different numbers of cycles. 
  
  This program determines whether or not an inputted string is a palindrome. 
  
Instructions to compile and run on Linux machine:

    1. Download project2.tar.gz.
    2. Type 'tar -xvf project2.tar' to extract the file to the current directory.
    3. Change directory to downloaded folder. 
    4. Compile gprSim: g++ -std=c++11 gprSim.cpp -o gprSim
    5. Run gprSim: ./gprSim
    6. Type in a string for testing when prompted (ie. anna).
  
  Instructions to view output: 
  
    1. Open the result.txt file created.
 
    Note: Output is also printed in the terminal. 
  
 Problems during development: 
 
    (Resolved)
    Compiling with 'g++ stackSim.cop' or 'g++ accumSim.cpp' results in a "'to-string' is not a member of 'std' error. 
    Fixed by specifying C++ version of 11 with command 'g++ -std=c++11...'. 
  
    (Resolved)
    Upon completion, stackSim.cpp and accumSim.cpp both ran without errors in VS Code, but threw a 'Segmentation fault
    (core dumped)' error while running on the tux machines. To fix this, the program was ended after the final 'syscall' 
    instruction.
 
  

