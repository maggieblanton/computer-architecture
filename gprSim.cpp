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

// initialize arrays
Text text_segment[SEGMENT_SIZE];
Data data_segment[SEGMENT_SIZE];
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
    int val = 0;
    char palindrome[256];
    fstream accumCode;
    int index;
    int offset = 0;
    cout << "\nTYPE PALINDROME TEST STRING: ";
    std::cin.getline(palindrome, 256);
    cout << "\nREADING palindrome.s";
    cout << "\nFILE CONTENT: " << "\n";
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
                    std::size_t msg_1 = line.find("string_space");
                    std::size_t msg_2 = line.find("is_palin_msg");
                    if (msg_1 != std::string::npos) {
                        data_segment[data_index].content = val;
                        stringArr[0] = palindrome;
                    }
                    else if (msg_2 != std::string::npos) {
                        data_segment[data_index].content = val + 1;
                        while (index < 28) {
                            placeholder = placeholder + data_mem[index + 23];
                            index++;
                        }
                        stringArr[1] = placeholder;
                    }
                    else { 
                        data_segment[data_index].content = val + 2;
                        while (index < 32) {
                            placeholder = placeholder + data_mem[index + 24];
                            index++;
                        }
                        stringArr[2] = placeholder;
                    }
                    data_index++;
                    line_count++;
                }
            }
         
            // add line to text_segment
            else if (type.compare("text") == 0) { 
                if (line.compare(".text") != 0 && !line.empty()) { 
                    if (line.compare("main:") != 0 && line.compare("length_loop:") != 0 && line.compare("end_length_loop:") != 0
                    && line.compare("test_loop:") != 0 && line.compare("is_palin:") != 0 && line.compare("not_palin:") != 0
                    && line.compare("exit:") != 0) {
                    clearMem();
                    index = 0;
                    string placeholder = "";
                    text_segment[text_index].addr = text_addr;
                    text_addr++;
                    std::copy(line.begin(), line.end(), text_mem); 
                    cmp = text_mem[0];
                    if (cmp.compare("l") == 0) {
                        while (index < 2) {
                            placeholder = placeholder + text_mem[index];
                            index++;
                        }
                        text_segment[text_index].instruction = assignInstruction(placeholder);
                        index = 0;
                        placeholder = "";
                        std::size_t msg_1 = line.find("1024");
                        std::size_t msg_2 = line.find("10");
                        if (msg_1 != std::string::npos) {
                            placeholder = "$31 1024";
                        }
                        else if (msg_2 != std::string::npos) {
                            placeholder = "$29 10";
                        }
                        else if (line.length() == 8) {
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
                    }
                    else if (cmp.compare("s") == 0 || cmp.compare("a") == 0 ) {
                        cmp = text_mem[1];
                        if (cmp.compare("y") == 0) {
                            placeholder = "syscall";
                            text_segment[text_index].instruction = assignInstruction(placeholder);
                        }
                        else { 
                            while (index < 4) {
                                placeholder = placeholder + text_mem[index];
                                index++;
                            }
                            text_segment[text_index].instruction = assignInstruction(placeholder);
                            index = 0;
                            placeholder = "";
                            while (index < 7) {
                                placeholder = placeholder + text_mem[5 + index];
                                index++;
                            }
                            text_segment[text_index].operand = placeholder;
                        }
                    }
                    else if (cmp.compare("b") == 0) {
                        cmp = text_mem[1];
                        if (cmp.compare(" ") == 0) {
                            placeholder = "b";
                            text_segment[text_index].instruction = assignInstruction(placeholder);
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
                        }
                        else if (cmp.compare("e") == 0) {
                            placeholder = "beqz";
                            text_segment[text_index].instruction = assignInstruction(placeholder);
                            placeholder = "";
                            while (index < 4) {
                                placeholder = placeholder + text_mem[index + 5];
                                index++;
                            }
                            text_segment[text_index].operand = placeholder;
                        }
                         else if (cmp.compare("n") == 0 || cmp.compare("g") == 0) {
                            if (cmp.compare("n") == 0) { 
                                placeholder = "bne";
                            }
                            else { 
                                placeholder = "bge";
                            }
                            text_segment[text_index].instruction = assignInstruction(placeholder);
                            placeholder = "";
                            while (index < 7) {
                                placeholder = placeholder + text_mem[index + 4];
                                index++;
                            }
                            text_segment[text_index].operand = placeholder;
                        }
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

// print results
void printResults(int ) { 
    std::ofstream ofs;
    ofs.open ("result.txt", std::ofstream::out | std::ofstream::trunc); // line referenced from source
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
    cout << "\n\nRESULT WRITTEN TO result.txt";
}

int main() {
    storeAccum();
    cout << "\n\nSUCCESSFUL READ";
    std::size_t msg_1;
    // initialize string variables
    string operand;
    string placeholder;
    string result;
    // intialize int variables
    int PC = 0;
    int IC = 0;
    int C = 0;
    int test = 0;
    // initialize speedup
    float speedup;
    // initialize int32 variables
    mem_addr label = 0;
    mem_addr dest = 0;
    mem_addr src1 = 0;
    mem_addr src2 = 0;
    mem_addr offset = 0;
    mem_addr imm = 0;
    // initialize boolean to enter while loop
    bool user_mode = true;
    // set base addr
    text_addr = TEXT_BASE_ADDR;
    cout << "\n\nSTARTING SIMULATOR\n" ;
    while(user_mode) { 
        Text currentText = loadMem(PC + text_addr);
        switch ((int) currentText.instruction) { 
            // addi
            case 1: 
                cout << "\naddi " << currentText.operand;
                operand = currentText.operand;
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                dest = std::stoi(operand);
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                src1 = std::stoi(operand);
                operand = currentText.operand[6];
                imm = std::stoi(operand);
                register_mem[dest] = register_mem[src1] + imm;
                C += 6;
                IC++;
                break;
            // b
            case 2: 
                cout << "\nb " << currentText.operand;
                cmp = currentText.operand[0];
                if (cmp.compare("-") == 0) {
                    operand = currentText.operand[1];
                    test = std::stoi(operand);
                    test = test * -1;
                }
                else {
                    operand = currentText.operand[0];
                    test = std::stoi(operand);
                }
                PC += test;
                C += 4;
                IC++;
                break;
            // beqz
            case 3: 
                cout << "\nbeqz " << currentText.operand;
                operand = currentText.operand[1];
                src1 = std::stoi(operand);
                operand = currentText.operand[3];
                test = std::stoi(operand);
                if (register_mem[src1] == 0) {
                    PC += test;
                }
                C += 5;
                IC++;
                break;
            // bge
            case 4: 
                cout << "\nbge " << currentText.operand; 
                operand = currentText.operand[1];
                src1 = std::stoi(operand);
                operand = currentText.operand[4];
                src2 = std::stoi(operand);
                operand = currentText.operand[6];
                test = std::stoi(operand);
                if (register_mem[src1] >= register_mem[src2]) {
                    PC += test;
                }
                C += 5;
                IC++;
                break;
            // bne
            case 5: 
                cout << "\nbne " << currentText.operand;
                operand = currentText.operand[1];
                src1 = std::stoi(operand);
                operand = currentText.operand[4];
                src2 = std::stoi(operand);
                operand = currentText.operand[6];
                test = std::stoi(operand);
                if (register_mem[src1] != register_mem[src2]) {
                    PC += test;
                }
                C += 5;
                IC++;
                break;
            // la
            case 6: 
                cout << "\nla " << currentText.operand;
                operand = currentText.operand;
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                dest = std::stoi(operand);
                operand = currentText.operand[4];
                label = std::stoi(operand);
                register_mem[dest] = label;
                C += 5;
                IC++;
                break;
            // lb
            case 7:
                cout << "\nlb " << currentText.operand;
                operand = currentText.operand;
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                dest = std::stoi(operand);
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                offset = std::stoi(operand);
                register_mem[dest] = stringArr[0][register_mem[offset]];
                C += 6;
                IC++;
                break;
            // li
            case 8: 
                cout << "\nli " << currentText.operand;
                operand = currentText.operand;
                msg_1 = operand.find("1024");
                if (msg_1 != std::string::npos) {
                    break;
                }
                operand = currentText.operand;
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                dest = std::stoi(operand);
                cmp = currentText.operand[3];
                msg_1 = operand.find("10");
                if (msg_1 != std::string::npos) {
                    placeholder = "10";
                    imm = std::stoi(placeholder);
                }
                else if (cmp.compare(" ") == 0) {
                    imm = currentText.operand[4] - 48;
                }
                else {
                    imm = currentText.operand[3] - 48;
                }
                register_mem[dest] = imm;
                C += 3;
                IC++;
                break;
            // subi
            case 9: 
                cout << "\nsubi " << currentText.operand;
                operand = currentText.operand;
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                dest = std::stoi(operand);
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                src1 = std::stoi(operand);
                operand = currentText.operand[6];
                imm = std::stoi(operand);
                register_mem[dest] = register_mem[src1] - imm;
                C += 6;
                IC++;
                break;
            // syscall
            case 10: 
                cout << "\nsyscall";
                offset = register_mem[29];
                if (offset == 1) {
                    result = stringArr[register_mem[31]];
                }
                if (offset == 10) {
                    cout << "\n\nSIMULATION COMPLETE\n\nC: " << C << "\nIC: " << IC;
                    speedup = 8 * (float) IC / (float) C;
                    cout << "\nSPEEDUP: " << speedup << "\n\nRESULT: " << result;
                    std::ofstream ofs;
                    ofs.open ("result.txt", std::ofstream::out | std::ofstream::trunc);
                    ofs << stringArr[0];
                    ofs << "\n-----------------";
                    ofs << "\nC: " << C << "\nIC: " << IC;
                    ofs << "\nSpeedup: " << speedup;
                    ofs << "\n\n" << result;
                    ofs.close();
                    cout << "\n\nEXITING PROGRAM\n";
                    exit (EXIT_FAILURE);
                }
                C += 8;
                IC++;
                break;
            default: 
                //cout << "\nSIMULATION COMPLETE";
                user_mode = false; 
        }
        PC++;
    }
    return 0;
}