  
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

string strings[UNIQUE_STRING];

// initialize arrays
Text text_segment[SEGMENT_SIZE];
Data data_segment[SEGMENT_SIZE];
Data stack[STACK_SIZE];
Instruction instr_segment[INSTR_NUM];
char data_mem[BUFFER_SIZE];
char text_mem[BUFFER_SIZE];
int register_mem[REGISTER_NUM];

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
    fstream accumCode;
    int index;
    int offset = 0;
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
                    //cout << "\nEntered";
                    index = 0;
                    string placeholder = "";
                    data_segment[data_index].addr = data_addr;
                    data_addr++;
                    std::copy(line.begin(), line.end(), data_mem);
                    cmp = data_mem[0];
                    if (cmp.compare("i") == 0) {
                        while (index < 12) {
                            placeholder = placeholder + data_mem[index];
                            index++;
                        }
                        data_segment[data_index].operand = placeholder;
                        index = 0;
                        placeholder = "";
                        while (index < 27) { 
                            placeholder = placeholder + data_mem[index + 23];
                            index++;
                        }
                        data_segment[data_index].content = placeholder;
                    }
                    else if (cmp.compare("n") == 0) {
                        while (index < 13) {
                            placeholder = placeholder + data_mem[index];
                            index++;
                        }
                        data_segment[data_index].operand = placeholder;
                        index = 0;
                        placeholder = "";
                        while (index < 31) { 
                            placeholder = placeholder + data_mem[index + 24];
                            index++;
                        }
                        data_segment[data_index].content = placeholder; 
                    }
                    else { 
                         while (index < 12) {
                            placeholder = placeholder + data_mem[index];
                            index++;
                        }
                        data_segment[data_index].operand = placeholder;
                    }
                   
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
                    if (cmp.compare("l") == 0) {
                        while (index < 2) { 
                        placeholder = placeholder + text_mem[index];
                        index++;
                        }
                        text_segment[text_index].instruction = assignInstruction(placeholder);
                        index = 0;
                        placeholder = "";
                        cmp = text_mem[1];
                        if (cmp.compare("b") == 0) {
                            while (index < 8) {
                                    placeholder = placeholder + text_mem[3 + index];
                                    index++;
                                }
                        }
                        else { 
                            std::size_t msg_1 = line.find("string_space");
                            std::size_t msg_2 = line.find("is_palin_msg");
                            std::size_t msg_3 = line.find("not_palin_msg");
                            std::size_t msg_4 = line.find("(");
                            if (msg_1 != std::string::npos || msg_2 != std::string::npos) {
                                cmp = text_mem[6];
                                if (cmp.compare(",") == 0) 
                                    while (index < 17) {
                                        placeholder = placeholder + text_mem[3 + index];
                                        index++;
                                    }
                                else { 
                                    while (index < 16) {
                                        placeholder = placeholder + text_mem[3 + index];
                                        index++;
                                    }
                                }
                            }
                            else if (msg_3 != std::string::npos) {
                                 cmp = text_mem[6];
                                if (cmp.compare(",") == 0) 
                                    while (index < 18) {
                                        placeholder = placeholder + text_mem[3 + index];
                                        index++;
                                    }
                                else { 
                                    while (index < 17) {
                                        placeholder = placeholder + text_mem[3 + index];
                                        index++;
                                    }
                                }
                                
                            }
                            else if (msg_4 != std::string::npos) {
                                while (index < 10) {
                                    placeholder = placeholder + text_mem[3 + index];
                                    index++;
                                }
                            }
                            else { 
                                while (index < 9) {
                                    placeholder = placeholder + text_mem[3 + index];
                                    index++;
                                }
                            }
                        }
                        trim(placeholder);
                        text_segment[text_index].operand = placeholder;
                        //cout << "\nINSTRUCTION IS " << text_segment[text_index].instruction;
                        //cout << "\nOPERAND IS " << text_segment[text_index].operand;
                    }
                    else if (cmp.compare("s") == 0 || cmp.compare("a") == 0) {
                        cmp = text_mem[1];
                        if (cmp.compare("y") == 0) {
                            while (index < 7) { 
                                placeholder = placeholder + text_mem[index];
                                index++;
                            }
                            text_segment[text_index].instruction = assignInstruction(placeholder);
                            //cout << "\nINSTRUCTION IS " << text_segment[text_index].instruction;
                        }
                        else { 
                            while (index < 4) {
                                 placeholder = placeholder + text_mem[index];
                                 index++;
                            }
                            text_segment[text_index].instruction = assignInstruction(placeholder);
                            //cout << "\nINSTRUCTION IS " << text_segment[text_index].instruction;
                            index = 0;
                            placeholder = "";
                            while (index < 11) {
                                placeholder = placeholder + text_mem[5 + index];
                                index++;
                             }
                            trim(placeholder);
                            text_segment[text_index].operand = placeholder;
                            //cout << "\nOPERAND IS " << text_segment[text_index].operand;
                            //cout << "\nADDRESS IS " << text_segment[text_index].addr;
                        }
                    }
                    else if (cmp.compare("b") == 0) {
                        cmp = text_mem[1];
                        if (cmp.compare(" ") == 0) {
                            placeholder = text_mem[0];
                            text_segment[text_index].instruction = assignInstruction(placeholder);
                            //cout << "\nINSTRUCTION IS " << text_segment[text_index].instruction;
                            std::size_t msg_1 = line.find("length_loop");
                            std::size_t msg_2 = line.find("test_loop");
                            //std::size_t msg_3 = line.find("exit");
                            if (msg_1 != std::string::npos) {
                                placeholder = "length_loop";
                            }
                            else if (msg_2 != std::string::npos) {
                                placeholder = "test_loop";
                            }
                            else {
                                placeholder = "exit";
                            }
                            text_segment[text_index].operand = placeholder;
                            //cout << "\nOPERAND IS " << text_segment[text_index].operand;
                           
                        }
                        else if (cmp.compare("e") == 0) {
                            while (index < 4) {
                                 placeholder = placeholder + text_mem[index];
                                 index++;
                            }
                            text_segment[text_index].instruction = assignInstruction(placeholder);
                            index = 0;
                            placeholder = "";
                            while (index < 13) {
                                placeholder = placeholder + text_mem[5 + index];
                                index++;
                             }
                            trim(placeholder);
                            text_segment[text_index].operand = placeholder;
                            //cout << "\nOPERAND IS " << text_segment[text_index].operand;
                        }
                        else {
                            placeholder = "";
                             while (index < 3) {
                                 placeholder = placeholder + text_mem[index];
                                 index++;
                            }
                            text_segment[text_index].instruction = assignInstruction(placeholder);
                            //cout << "\nINSTRUCTION IS " << text_segment[text_index].instruction;
                            std::size_t msg_1 = line.find("is_palin");
                            //std::size_t msg_2 = line.find("not_palin");
                            index = 0;
                            placeholder = "";
                            while (index < 9) {
                                placeholder = placeholder + text_mem[3 + index];
                                index++;
                            }
                            if (msg_1 != std::string::npos) {
                                placeholder = placeholder + "is_palin";
                            }
                            else { 
                                placeholder = placeholder + "not_palin";
                            }
                            
                            trim(placeholder);
                            text_segment[text_index].operand = placeholder;
                            //cout << "\nOPERAND IS " << text_segment[text_index].operand;
                        }

                    }
                    //cout << "\n\n" << text_segment[text_index].instruction;
                    //cout << "\n" << text_segment[text_index].operand;
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
    string palindrome;
    cout << "\nREADING palindrome.s" << "\nFILE CONTENT:\n";
    storeAccum();
    cout << "\n\nSUCCESSFUL READ";
    // initialize program counter
    string operand;
    Instruction currentInstruction;
    string jump;
    std::size_t msg_1;
    std::size_t msg_2;
    std::size_t msg_3;
    Data currentData;
    int32 service_num;
    int32 arg1;
    int32 arg2;
    int laVal = 0;
    int PC = 0;
    int size = 0;
    int IC = 0;
    int C = 0;
    int dest = 0;
    int src = 0;
    int src2 = 0;
    int imm = 0;
    int offset = 0;
    bool user_mode = true;
    text_addr = TEXT_BASE_ADDR;
    cout << "\n\nSTARTING SIMULATOR\n" ;

    cout << "TYPE TEST STRING: ";
    cin >> palindrome;
    strings[0] = palindrome;
    strings[1] = "This is a palindrome";
    strings[2] = "This is not a palindrome";


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
                //operand.erase(std::remove(operand.begin(), operand.end(), 't'), operand.end());
                dest = std::stoi(operand);
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                //operand.erase(std::remove(operand.begin(), operand.end(), 't'), operand.end());
                imm = currentText.operand[8] - 48;
                src = std::stoi(operand);
                register_mem[dest] = register_mem[src] + imm;
                //cout << " dest " << dest << " src " << src << " imm " << imm;
                C += 6;
                IC++;
                break;
            // b
            case 2: 
                cout << "\nb " << currentText.operand;
                currentInstruction = loadInstr(currentText.operand);
                offset = currentInstruction.offset;
                cout << " OFFSET " << offset; 
                PC = offset - 1;
                C += 4;
                IC++;
                break;
            case 3: 
                cout << "\nbeqz " << currentText.operand;
                operand = currentText.operand;
                //cout << "OPERAND " << currentText.operand;
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(" ")), 0, 0;
                //operand.erase(std::remove(operand.begin(), operand.end(), 't'), operand.end());
                src = std::stoi(operand);
                operand = currentText.operand;
                operand = operand.substr(operand.find(" ") + 1);
                jump = operand.substr(0, operand.find("\r"));
                currentInstruction = loadInstr(jump);
                offset = currentInstruction.offset;
                cout << " " << src << " JUMP " << offset;
                //cout << "OFFSET " << offset; 
                dest = register_mem[src];
                if (dest == 0) {
                   PC = offset - 1;
                }
            
                break;
            case 4: 
                cout << "\nbge " << currentText.operand;
                operand = currentText.operand;
                msg_1 = operand.find("is_palin");
                if (msg_1 != std::string::npos) {
                    jump = "is_palin";
                }
                else {
                    jump = "not_palin";
                }
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                //operand.erase(std::remove(operand.begin(), operand.end(), 't'), operand.end());
                src = std::stoi(operand);
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                //operand.erase(std::remove(operand.begin(), operand.end(), 't'), operand.end());
                src2 = std::stoi(operand);
                //register_mem[dest] = register_mem[src] + imm;
                //operand = currentText.operand;
                //cout << "src " << src << "src2 " << src2 << "offset " << offset;
                currentInstruction = loadInstr(jump);
                offset = currentInstruction.offset;
                //cout << "jump " << offset;
                dest = register_mem[src];
                imm = register_mem[src2];
                if (dest >= imm) {
                    PC = offset - 1;
                }
                C += 5;
                IC++;
                break;
            case 5: 
                cout << "\nbne " << currentText.operand;
                operand = currentText.operand;
                msg_1 = operand.find("is_palin");
                if (msg_1 != std::string::npos) {
                    jump = "is_palin";
                }
                else {
                    jump = "not_palin";
                }
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                //operand.erase(std::remove(operand.begin(), operand.end(), 't'), operand.end());
                src = std::stoi(operand);
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                //operand.erase(std::remove(operand.begin(), operand.end(), 't'), operand.end());
                src2 = std::stoi(operand);
                //register_mem[dest] = register_mem[src] + imm;
                //operand = currentText.operand;
                //cout << "src " << src << "src2 " << src2 << "offset " << offset;
                currentInstruction = loadInstr(jump);
                offset = currentInstruction.offset;
                //cout << "jump " << offset;
                dest = register_mem[src];
                imm = register_mem[src2];
                if (dest != imm) {
                    PC = offset - 1;
                }
                C += 5;
                IC++;
                break;
            case 6: 
                operand = currentText.operand;
                cout << "\nla " << currentText.operand;
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                //operand.erase(std::remove(operand.begin(), operand.end(), 't'), operand.end());
                //cout << " dest " << operand;
                dest = std::stoi(operand);
                operand = currentText.operand;
                msg_1 = operand.find("is_palin_msg");
                msg_2 = operand.find("not_palin_msg");
                msg_3 = operand.find("string_space");
                if (msg_1 != std::string::npos) {
                    operand = "is_palin_msg";
                    //currentData = loadData(operand);
                    register_mem[dest] = 1;
                    //register_mem[dest] = currentData.addr;
                }
                else if (msg_2 != std::string::npos) {
                    operand = "not_palin_msg";
                    register_mem[dest] = 2;
                    //register_mem[dest] = currentData.addr;
                }
                else if (msg_3 != std::string::npos) {
                    operand = "string_space";
                    register_mem[dest] = laVal;
                    //register_mem[dest] = 0;
                    //register_mem[dest] = currentData.addr;
                }
                
                C += 5;
                IC++;
                break;
            case 7:
                cout << "\nlb " << currentText.operand;
                operand = currentText.operand;
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                //operand.erase(std::remove(operand.begin(), operand.end(), 't'), operand.end());
                dest = std::stoi(operand);
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(")")), 0, 0;
                //operand.erase(std::remove(operand.begin(), operand.end(), 't'), operand.end());
                src = std::stoi(operand);
                register_mem[dest] = strings[0][register_mem[src]];
                C += 6;
                IC++;
                break;
            case 8: 
                cout << "\nli " << currentText.operand;
                operand = currentText.operand;
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                //operand.erase(std::remove(operand.begin(), operand.end(), 't'), operand.end());
                dest = std::stoi(operand);
                operand = currentText.operand;
                operand = operand.substr(operand.find(" ") + 1);
                operand = operand.substr(0, operand.find("\r")), 0, 0;
                imm = std::stoi(operand);
                //cout << " dest " << dest << " imm " << imm;
                register_mem[dest] = imm;
                C += 3;
                IC++;
                break;
            case 9: 
                cout << "\nsubi " << currentText.operand;
                operand = currentText.operand;
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                //operand.erase(std::remove(operand.begin(), operand.end(), 't'), operand.end());
                dest = std::stoi(operand);
                operand = operand.substr(operand.find("$") + 1);
                operand = operand.substr(0, operand.find(",")), 0, 0;
                //operand.erase(std::remove(operand.begin(), operand.end(), 't'), operand.end());
                imm = currentText.operand[8] - 48;
                src = std::stoi(operand);
                register_mem[dest] = register_mem[src] - imm;
                //cout << " " << dest << " " << src << " " << imm;
                C += 6;
                IC++;
                break;
            case 10: 
                cout << "\nsyscall";
                service_num = register_mem[29];
                arg1 = register_mem[30];
                arg2 = register_mem[31];

                if (service_num == 1) {
                    cout << "%s\n", strings[register_mem[31]];
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