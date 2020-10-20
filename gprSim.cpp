/* Referenced http://www.cplusplus.com/reference/fstream/ofstream/open/ 
   for instructions on how to clear text file. 
   Reflected in the printResults() method and commented accordingly. */

// https://github.com/cahuila/CS572-Micro-Architecture/blob/master/Lab2/gprSim.c

#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<cstdlib>
#include<cstdio>
#include<sstream>
#include <cstddef>
#include<algorithm>
#include<stdio.h>

using namespace std;

typedef unsigned int int32;  
typedef int32 mem_addr;

// initialize size variables
#define SEGMENT_SIZE 50
#define STACK_SIZE 50
#define BUFFER_SIZE 50
#define DATA_BASE_ADDR 0x00000000
#define TEXT_BASE_ADDR 0x00000010
#define REGISTER_NUM 32
#define UNIQUE_STRING 3
#define INSTR_NUM 7

// define data type for .data
typedef struct data { 
    mem_addr addr;
    std::string content;
    std::string operand;
} Data;

// define data type for .text
typedef struct text { 
    mem_addr addr;
    mem_addr instruction; 
    std::string operand;
} Text;

// define data type for .text
typedef struct instr { 
    int offset;
    std::string name;
} Instruction;


// initialize arrays
Text text_segment[SEGMENT_SIZE];
Data data_segment[SEGMENT_SIZE];
Data stack[STACK_SIZE];
Instruction instr_segment[INSTR_NUM];
char data_mem[BUFFER_SIZE];
char text_mem[BUFFER_SIZE];
int register_mem[REGISTER_NUM];
string stringArr[UNIQUE_STRING];

// declare string variables
std::string line;
std::string cmp;
std::string type; 

// set starting addresses
mem_addr data_addr = DATA_BASE_ADDR;
mem_addr text_addr = TEXT_BASE_ADDR;

// initialze indexes
mem_addr data_index = 0;
mem_addr text_index = 0;
mem_addr instr_index = 0;

// initialize line count
int line_count = 0;

// declare write address
mem_addr write_addr;



// determine line type
void typeCheck(string line) { 
    if (line.compare(".data") == 0) {
        type = "data";
    }
    else if (line.compare(".text") == 0) {
        type = "text";
    }
}

// assign code for varying instructions
int assignInstruction(string instr) {
    if (instr.compare("addi") == 0) {
        return 1;
    }
    else if (instr.compare("b") == 0) {
        return 2;
    }
    else if (instr.compare("beqz") == 0) {
        return 3;
    }
    else if (instr.compare("bge") == 0) {
        return 4;
    }
    else if (instr.compare("bne") == 0) {
        return 5;
    }
    else if (instr.compare("la") == 0) {
        return 6;
    }
    else if (instr.compare("lb") == 0) {
        return 7;
    }
    else if (instr.compare("li") == 0) {
        return 8;
    }
    else if (instr.compare("subi") == 0) {
        return 9;
    }
    else if (instr.compare("syscall") == 0) {
        return 10;
    }
    else { 
        return 11;
    }
}

// remove leading/training whitespace 
void trim(string& s) { 
    size_t p = s.find_first_not_of(" \t");
    s.erase(0, p);
    p = s.find_last_not_of(" \t");
    if (string::npos !=p) {
        s.erase(p+1);
    }
}

// clear string list
void clearMem() {
    for (int i = 1; i < 12; i++) {
        text_mem[i] = ' ';
    }
}

// read and store text file 
void storeAccum() { 
    string palindrome;
    fstream accumCode;
    int index;
    int offset = 0;
    //string s1 = "racecar";
    //char str1[] = "racecar";
    cout << "\nTYPE PALINDROME TEST STRING: ";
    cin >> palindrome;
    stringArr[0] = palindrome;
    stringArr[1] = "This is a palindrome";
    stringArr[2] = "This is not a palindrome";

    //open accumCode.txt 
    accumCode.open("palindrome.s", ios::in);
    // read accumCode.txt
    if (accumCode.is_open()) { 
        // parse ASCII file by line
        while(getline(accumCode, line)) { 
            // determine line type
            typeCheck(line);
            
            // add line to data_segment
             if (type.compare("data") == 0) { 
                if (line.compare(".data") != 0 && !line.empty()) { 
                    index = 0;
                    string placeholder = "";
                    data_segment[data_index].addr = data_addr;
                    data_addr++;
                    std::copy(line.begin(), line.end(), data_mem);
                    while (index < 10) { 
                        placeholder = placeholder + data_mem[index];
                        index++;
                    }
                    data_segment[data_index].operand = placeholder;
                    data_segment[data_index].content = data_mem[11];
                    data_index++;
                    line_count++;
                }
            }
         
            // add line to text_segment
            else if (type.compare("text") == 0) { 
                if (line.compare("main:") == 0 || line.compare("length_loop:") == 0 || line.compare("end_length_loop:") == 0 || line.compare("test_loop:") == 0 || line.compare("is_palin:") == 0 || line.compare("not_palin:") == 0 || line.compare("exit:") == 0) {
                    //cout << "offset: " << offset;
                    instr_segment[instr_index].offset = offset;
                    string removeColon = line;
                    removeColon.erase(std::remove(removeColon.begin(), removeColon.end(), ':'), removeColon.end());
                    instr_segment[instr_index].name = removeColon;
                    //offset++;
                    instr_index++;
                    //cout<<"\n" << line;
                    continue;
                }
                else if (line.compare(".text") != 0 && !line.empty()) { 
                    clearMem();
                    index = 0;
                    string placeholder = "";
                    text_segment[text_index].addr = text_addr;
                    text_addr++;
                    std::copy(line.begin(), line.end(), text_mem); 
                    cmp = text_mem[0];
                    //cout << " CMP: " << cmp;
                    if (cmp.compare("l") == 0) {
                        while (index < 2) {
                            placeholder = placeholder + text_mem[index];
                            index++;
                        }
                        text_segment[text_index].instruction = assignInstruction(placeholder);
                        //cout << " INSTR: " << text_segment[text_index].instruction;
                        index = 0;
                        placeholder = "";
                        if (line.length() == 8) {
                            while (index < 5) {
                                placeholder = placeholder + text_mem[3 + index];
                                index++;
                            }
                        }
                        else { 
                            while (index < 4) {
                                placeholder = placeholder + text_mem[3 + index];
                                index++;
                            }
                        }
                        text_segment[text_index].operand = placeholder;
                        //cout << " OP: " << placeholder;
                    }
                    else if (cmp.compare("s") == 0 || cmp.compare("a") == 0 ) {
                        cmp = text_mem[1];
                        if (cmp.compare("y") == 0) {
                            placeholder = "syscall";
                            text_segment[text_index].instruction = assignInstruction(placeholder);
                            //cout << " INSTR: " << text_segment[text_index].instruction;
                        }
                        else { 
                            while (index < 4) {
                                placeholder = placeholder + text_mem[index];
                                index++;
                            }
                            text_segment[text_index].instruction = assignInstruction(placeholder);
                            //cout << " INSTR: " << text_segment[text_index].instruction;
                            index = 0;
                            placeholder = "";
                            while (index < 7) {
                                placeholder = placeholder + text_mem[5 + index];
                                index++;
                            }
                            text_segment[text_index].operand = placeholder;
                            cout << " OP: " << placeholder;
                        
                        //text_segment[text_index].instruction = assignInstruction(placeholder);
                        }
                    }
                    else if (cmp.compare("b") == 0) {
                        cmp = text_mem[1];
                        if (cmp.compare(" ") == 0) {
                            placeholder = "b";
                            text_segment[text_index].instruction = assignInstruction(placeholder);
                            //cout << " INSTR: " << text_segment[text_index].instruction;
                            cmp = text_mem[2];
                            placeholder = "";
                            if (cmp.compare("-") == 0) {
                                index = 0;
                                while (index < 2) {
                                    placeholder = placeholder + text_mem[index + 2];
                                    index++;
                                }
                            }
                            else {
                               placeholder = text_mem[2]; 
                            }
                            text_segment[text_index].operand = placeholder;
                            //cout << " PLACEHOLDER " << placeholder;
                        }
                        else if (cmp.compare("e") == 0) {
                            placeholder = "beqz";
                            text_segment[text_index].instruction = assignInstruction(placeholder);
                            //cout << " INSTR: " << text_segment[text_index].instruction;
                            placeholder = "";
                            while (index < 4) {
                                placeholder = placeholder + text_mem[index + 5];
                                index++;
                            }
                            text_segment[text_index].operand = placeholder;
                            //cout << " PLACEHOLDER " << placeholder;
                        }
                         else if (cmp.compare("n") == 0 || cmp.compare("g") == 0) {
                            if (cmp.compare("n") == 0) { 
                                placeholder = "bne";
                            }
                            else { 
                                placeholder = "bge";
                            }
                            text_segment[text_index].instruction = assignInstruction(placeholder);
                            cout << " INSTR: " << text_segment[text_index].instruction;
                            placeholder = "";
                            while (index < 7) {
                                placeholder = placeholder + text_mem[index + 4];
                                index++;
                            }
                            text_segment[text_index].operand = placeholder;
                            cout << " PLACEHOLDER " << placeholder; 
                        }
                        

                    }
                    text_index++;
                    offset++;
                }
            }
            // print line
            cout<<"\n" << line;
        }
    }

}

// load currentText 
Text loadMem(int32 address) {
    for(int i = 0; i < sizeof(text_segment) + 1; i++) {
		if (std::to_string(text_segment[i].addr).compare(std::to_string(address)) == 0) {
    		return text_segment[i];
        }
    }
    exit (EXIT_FAILURE);
}


Instruction loadInstr(string operand) {
    //trim(operand);
    //cout << operand;
    for(int i = 0; i < 7; i++) {
        if (instr_segment[i].name.compare(operand) == 0) {
    		return instr_segment[i];
        }
    }
    exit (EXIT_FAILURE);
}

Data loadData(string operand) {
    //trim(operand);
    //cout << operand;
    for(int i = 0; i < 3; i++) {
        //cout << "OP " << data_segment[i].operand;
        if (data_segment[i].operand.compare(operand) == 0) {
    		return data_segment[i];
        }
    }
    exit (EXIT_FAILURE);
}

// print results
void printResults() { 
    std::ofstream ofs;
    ofs.open ("accumOutput.txt", std::ofstream::out | std::ofstream::trunc); // line referenced from source
    for (int i = 0; i < line_count; i++) {
        if (i != 0) {
            ofs << "\n" << data_segment[i].operand << " " << data_segment[i].content;
        }
        else { 
            ofs << data_segment[i].operand << " " << data_segment[i].content;
        }
        cout << "\n" << data_segment[i].operand << " " << data_segment[i].content;  
    }
    ofs.close(); // line referenced from source
    cout << "\n\nRESULTS WRITTEN TO accumOutput.txt";
}

int main() { 
    cout << "\nREADING palindrome.s" << "\nFILE CONTENT:\n";
    storeAccum();
    cout << "\n\nSUCCESSFUL READ";
    // initialize program counter
    string operand;
    Instruction currentInstruction;
    Data currentData;
    string placeholder;
    int32 service_num;
    int32 arg1;
    int32 arg2;
    int PC = 0;
    int IC = 0;
    int C = 0;
    int test = 0;
    int32 label = 0;
    int32 rdest = 0;
    int32 rsrc1 = 0;
    int32 rsrc2 = 0;
    int32 offset = 0;
    int32 imm = 0;
    bool user_mode = true;
    text_addr = TEXT_BASE_ADDR;
    cout << "\n\nSTARTING SIMULATOR\n" ;
    while(user_mode) { 
        Text currentText = loadMem(PC + text_addr);
        //cout << "\nADDRESS " << currentText.addr;
        switch ((int) currentText.instruction) { 
            // addi
            case 1: 
                cout << "\naddi " << currentText.operand;
                operand = currentText.operand;
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                rdest = std::stoi(operand);
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                rsrc1 = std::stoi(operand);
                operand = currentText.operand[6];
                imm = std::stoi(operand);
                //cout << " rdest " << rdest << " rsrc1 " << rsrc1 << " imm " << imm;
                register_mem[rdest] = register_mem[rsrc1] + imm;
                C += 6;
                IC++;
                break;
            // b
            case 2: 
                cout << "\nb " << currentText.operand;

                //cout << " curr text " << currentText.operand;
                //label = (int32) currentText.operand;
                cmp = currentText.operand[0];
                //cout << " cmp " << cmp;
                if (cmp.compare("-") == 0) {
                    operand = currentText.operand[1];
                    test = std::stoi(operand);
                    test = test * -1;
                    //label = test;
                    cout << " op " << test;
                }
                else {
                    operand = currentText.operand[0];
                    test = std::stoi(operand);
                    cout << " op " << test;
                }
                PC += test;
                C += 4;
                IC++;
                break;
            case 3: 
                cout << "\nbeqz " << currentText.operand;
                operand = currentText.operand[1];
                rsrc1 = std::stoi(operand);
                operand = currentText.operand[3];
                test = std::stoi(operand);
                //cout << " rsrc1 " << rsrc1 << " label " << test;
                if (register_mem[rsrc1] == 0) {
                    PC += test;
                }

                C += 5;
                IC++;
                break;
            case 4: 
                cout << "\nbge " << currentText.operand; 
                operand = currentText.operand[1];
                rsrc1 = std::stoi(operand);
                operand = currentText.operand[4];
                rsrc2 = std::stoi(operand);
                operand = currentText.operand[6];
                test = std::stoi(operand);
                //cout << " rsrc1 " << rsrc1 << " rsrc2 " << rsrc2 << " label " << test;
                if (register_mem[rsrc1] >= register_mem[rsrc2]) {
                    PC += test;
                }
                C += 5;
                IC++;
                break;
            case 5: 
                cout << "\nbne " << currentText.operand;
                operand = currentText.operand[1];
                rsrc1 = std::stoi(operand);
                operand = currentText.operand[4];
                rsrc2 = std::stoi(operand);
                operand = currentText.operand[6];
                test = std::stoi(operand);
                //cout << " rsrc1 " << rsrc1 << " rsrc2 " << rsrc2 << " label " << test;
                if (register_mem[rsrc1] != register_mem[rsrc2]) {
                    PC += test;
                }
                C += 5;
                IC++;
                break;
            case 6: 
                cout << "\nla " << currentText.operand;
                operand = currentText.operand;
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                rdest = std::stoi(operand);
                //cout << " place holder " << rdest;
                operand = currentText.operand[4];
                label = std::stoi(operand);
                //cout << " label " << label; 
                register_mem[rdest] = label;
                C += 5;
                IC++;
                break;
            case 7:
                cout << "\nlb " << currentText.operand;
                operand = currentText.operand;
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                rdest = std::stoi(operand);
                //cout << " place holder " << rdest;
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                offset = std::stoi(operand);
                //cout << " off " << offset;
                register_mem[rdest] = stringArr[0][register_mem[offset]];
                //register_mem[rdest] = string
                C += 6;
                IC++;
                break;
            case 8: 
                cout << "\nli " << currentText.operand;
                operand = currentText.operand;
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                rdest = std::stoi(operand);
                //cout << " place holder " << rdest;
                cmp = currentText.operand[3];
                if (cmp.compare(" ") == 0) {
                    operand = currentText.operand[4];
                }
                else {
                    operand = currentText.operand[3];
                }
                imm = std::stoi(operand);
                //cout << " imm " << imm;
                register_mem[rdest] = imm;
                C += 3;
                IC++;
                break;
            case 9: 
                cout << "\nsubi " << currentText.operand;
                operand = currentText.operand;
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                rdest = std::stoi(operand);
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                rsrc1 = std::stoi(operand);
                operand = currentText.operand[6];
                imm = std::stoi(operand);
                //cout << " rdest " << rdest << " rsrc1 " << rsrc1 << " imm " << imm;
                register_mem[rdest] = register_mem[rsrc1] - imm;
                C += 6;
                IC++;
                break;
            case 10: 
                cout << "\nsyscall";
                service_num = register_mem[29];
                cout << " service num " << service_num;
                if (service_num == 1) {
                    cout << "\n" << stringArr[register_mem[31]];
                }
                
                C += 8;
                IC++;
                break;
            default: 
              
                user_mode = false; 
        }
        PC++;
    }

    

    cout << "\n\nSIMULATION COMPLETE\n\nRESULT:\n" << "add result here" << "\n";
    //printResults();
    cout << "\n\nEXITING PROGRAM\n";
    return 0;
}