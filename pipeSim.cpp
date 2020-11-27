/* Referenced http://www.cplusplus.com/reference/fstream/ofstream/open/ 
   for instructions on how to clear text file. 
   Reflected in the main() method and commented accordingly. */

#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<cstdlib>
#include<cstdio>
#include<sstream>
#include<cstddef>
#include<algorithm>
#include<stdio.h>
#include <clocale>

using namespace std;

// type define mem_addr
typedef unsigned int int32;  
typedef int32 mem_addr;

// declare user mode bool
bool user_mode;

// declare string and char variables
string operand;
string placeholder;
string result;
char palindrome[256];

// initialize size variables
#define SEGMENT_SIZE 50
#define BUFFER_SIZE 50
#define DATA_BASE_ADDR 0x00000000
#define TEXT_BASE_ADDR 0x00000010
#define REGISTER_NUM 32
#define UNIQUE_STRING 3

// define data type for .data
typedef struct data { 
    mem_addr addr;
    mem_addr content;
    std::string operand;
} Data;

// define data type for .text
typedef struct text { 
    mem_addr addr;
    mem_addr instruction; 
    std::string operand;
} Text;

// define latch structs 
typedef struct if_id { 
	int ir;
  std::string op;
} if_id;

typedef struct id_exe { 
	int op_code;
	int rs;
  int rt;
  int rd;
	int op1;
  int op2;
  std::string op;
  int offset;
} id_exe;

typedef struct mem_exe { 
	int op_code;
	int alu_out;
	int op2;
	int rd;
} mem_exe;

typedef struct mem_wb { 
	int op_code;
	int mdr;
	int op2;
	int alu_out;
	int rd;
} mem_wb;

// declare latches
if_id if_id_prev;
if_id if_id_next;
id_exe id_exe_prev;
id_exe id_exe_next;
mem_exe mem_exe_prev;
mem_exe mem_exe_next;
mem_wb mem_wb_prev;
mem_wb mem_wb_next;

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
std::string filename;

// set starting addresses
mem_addr data_addr = DATA_BASE_ADDR;
mem_addr text_addr = TEXT_BASE_ADDR;

// initialze indexes
mem_addr data_index = 0;
mem_addr text_index = 0;

// intialize/declare int variables
int line_count = 0;
int fileType;

// declare write address
mem_addr write_addr;

// intialize size variables
std::size_t msg_1;
std::size_t msg_2;
std::size_t msg_3;
std::size_t msg_4;

// determine line type
void typeCheck(string line) { 
    if (line.compare(".data") == 0) {
        type = "data";
    }
    else if (line.compare(".text") == 0) {
        type = "text";
    }
}

// assign file type
void assignType(string line) {
  if (line.compare("lab3a.s") == 0) {
    fileType = 1;
  }
  else if (line.compare("lab3b.s") == 0) {
    fileType = 2;
    cout << "\nTYPE PALINDROME TEST STRING: ";
    cin >> palindrome;
  }
  else if (line.compare("lab3c.s") == 0) {
    fileType = 3;
  }
  else {
    fileType = 4;
  }
}

// assign code for varying instructions
int assignInstruction(string instr) {
    if (instr.compare("add") == 0) {
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
    else if (instr.compare("addi") == 0) {
        return 11;
    }
    else if (instr.compare("nop") == 0) {
        return 12;
    }
    else if (instr.compare("end") == 0) {
        return 13;
    }
    else { 
        return 14;
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
    fstream accumCode;
    int index;
    int offset = 0;
    cout << "\nTYPE FILE NAME: ";
    cin >> filename;
    assignType(filename);
    accumCode.open(filename, ios::in);
    // read accumCode.txt
    if (accumCode.is_open()) { 
      cout << "\nFILE CONTENT: " << "\n";
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
                    msg_1 = line.find("string_space");
                    msg_2 = line.find("is_palin_msg");
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
                    && line.compare("exit:") != 0
                    && line.compare("loop:") != 0) {
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
                      if (filename.compare("lab3b.s") != 0) {
                        if (line.length() == 7) {
                          while (index < 5) {
                            placeholder = placeholder + text_mem[3 + index];
                            index++;
                          }
                        }
                        else if (line.length() == 8) {
                          while (index < 6) {
                            placeholder = placeholder + text_mem[3 + index];
                            index++;
                          }
                        }
                        else if (line.length() == 9) {
                          while (index < 7) {
                            placeholder = placeholder + text_mem[3 + index];
                            index++;
                          }
                        }
                        else if (line.length() == 10) {
                          while (index < 8) {
                            placeholder = placeholder + text_mem[3 + index];
                            index++;
                          }
                        }
                      }
                      else {
                        msg_1 = line.find("1024");
                        msg_2 = line.find("10");
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
                      }
                      text_segment[text_index].operand = placeholder;
                    }
                    else if (cmp.compare("s") == 0) {
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
                    else if (cmp.compare("a") == 0) {
                       cmp = text_mem[3];
                       if (cmp.compare("i") != 0){
                        while (index < 3) {
                            placeholder = placeholder + text_mem[index];
                            index++;
                          }
                          text_segment[text_index].instruction = assignInstruction(placeholder);
                          index = 0;
                          placeholder = "";
                          while (index < 8) {
                              placeholder = placeholder + text_mem[4 + index];
                            index++;
                          }
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
                       }
                        text_segment[text_index].operand = placeholder;
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
                            while (index < 8) {
                                placeholder = placeholder + text_mem[index + 4];
                                index++;
                            }
                            text_segment[text_index].operand = placeholder;
                        }
                    }
                    else if (cmp.compare("n") == 0) {
                      placeholder = "nop";
                      text_segment[text_index].instruction = assignInstruction(placeholder);
                    }
                    else {
                      placeholder = "end";
                      text_segment[text_index].instruction = assignInstruction(placeholder);
                    }
                }
                    text_index++;
                    offset++;
                }
            }
            // print line
            cout<<"\n" << line;
        }
        cout << "\n\nSUCCESSFUL READ";
    }
    
    else { 
      cout << "\nINVALID FILE NAME";
      cout << "\n\nEXITING PROGRAM\n";
      exit (EXIT_FAILURE);
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
void printResults() { 
    cout << "\n\nREGISTER CONTENTS\n0: " << register_mem[0] << " 1: " 
    << register_mem[1] << " 2: " 
    << register_mem[2] << " 3: " 
    << register_mem[3] << " 4: " << 
    register_mem[4] << " 5: " <<
    register_mem[5];
}

int main() {
    storeAccum();
    user_mode = true;
    text_addr = TEXT_BASE_ADDR;
    cout << "\n\nSTARTING SIMULATOR\n" ;
    
    // initialize int variables
    int IC = 0;
    int C = 0;;
    int test = 0;
    int syscount = 0;
    int nopcount = 0; 
    int PC = 0;
    int count = 0;

    // initialize mem_addr variables
    mem_addr label = 0;
    mem_addr dest = 0;
    mem_addr src1 = 0;
    mem_addr src2 = 0;
    mem_addr offset = 0;
    mem_addr imm = 0;

    // initialize register 0
    register_mem[0] = 0;
  
    while(user_mode) { 
        Text currentText = loadMem(PC + text_addr); 
        // uncomment the following line to view register contents
        //printResults();
        if_id_prev = if_id_next;
        if_id fetch;
        fetch.ir = currentText.instruction;
        fetch.op = currentText.operand;
        if_id_next = fetch;
        id_exe_prev = id_exe_next;
        id_exe output;
        output.op_code = currentText.instruction;
        output.op = currentText.operand;
        trim(output.op);
        switch (output.op_code) {
          // decode add
          case 1: 
            cout << "\nadd " << output.op << " decoded";
            operand = output.op;
            operand = output.op[1];
            dest = std::stoi(operand);
            operand = output.op[4];
            src1 = std::stoi(operand);
            operand = output.op[7];
            src2 = std::stoi(operand);
            register_mem[dest] = register_mem[src1] + register_mem[src2];
            output.op1 = register_mem[src1];
            output.op2 = register_mem[src2];
            break;
          // decode b
          case 2: 
            cout << "\nb " << output.op << " decoded";
            cmp = output.op[0];
            if (cmp.compare("-") == 0) {
                operand = output.op[1];
                test = std::stoi(operand);
                test = test * -1;
            }
            else {
                operand = output.op[0];
                test = std::stoi(operand);
            }
            PC += test;
            C += 4;
            IC++;
            break;
          // decode beqz
          case 3: 
            cout << "\nbeqz " << output.op << " decoded";
            operand = output.op[1];
            src1 = std::stoi(operand);
            operand = output.op[3];
            test = std::stoi(operand);
            if (register_mem[src1] == 0) {
                PC += test;
            }
            C+= 5;
            IC++;
            output.rs = src1;
            output.op1 = register_mem[src1];
           break;
          // decode bge
          case 4: 
            cout << "\nbge " << output.op; 
            operand = output.op[1];
            src1 = std::stoi(operand);
            operand = output.op[4];
            src2 = std::stoi(operand);
            cmp = output.op[6];
            if (cmp.compare("-") == 0){
              operand = output.op[7];
              test = std::stoi(operand);
              test = test * -1 - 1;
            }
            else {
              operand = output.op[6];
              test = std::stoi(operand);
            }
            if (register_mem[src1] >= register_mem[src2]) {
                PC += test;
            }
            C += 5;
            IC++;
            output.rs = src1;
            output.rt = src2;
            output.op1 = register_mem[src1];
            output.op2 = register_mem[src2];
            break;
          // decode bne
          case 5: 
            cout << "\nbne " << output.op << " decoded";
            operand = output.op[1];
            src1 = std::stoi(operand);
            operand = output.op[4];
            src2 = std::stoi(operand);
            operand = output.op[6];
            test = std::stoi(operand);
            if (register_mem[src1] != register_mem[src2]) {
              PC += test;
            }
            C += 5;
            IC++;
            output.rs = src1;
            output.rt = src2;
            output.op1 = register_mem[src1];
            output.op2 = register_mem[src2];
            break;
          // decode la
          case 6: 
            cout << "\nla " << output.op << " decoded";
            operand = output.op;
            operand = operand.substr(operand.find("$") + 1);
            operand = operand.substr(0, operand.find(",")), 0, 0;
            dest = std::stoi(operand);
            operand = output.op[4];
            label = std::stoi(operand);
            register_mem[dest] = label;
            C += 5;
            IC++;
            output.rd = dest;
            output.rs = label;
            break;
          // decode lb
          case 7: 
            cout << "\nlb " << output.op << " decoded";
            operand = output.op;
            operand = operand.substr(operand.find("$") + 1);
            operand = operand.substr(0, operand.find(",")), 0, 0;
            dest = std::stoi(operand);
            operand = operand.substr(operand.find("$") + 1);
            operand = operand.substr(0, operand.find(",")), 0, 0;
            offset = std::stoi(operand);
            register_mem[dest] = stringArr[0][register_mem[offset]];
            C += 6;
            IC++;
            output.rd = dest;
            output.offset = offset;
            break;
          // decode li
          case 8: 
            cout << "\nli " << output.op << " decoded";
            operand = output.op;
            msg_1 = operand.find("1024");
            if (msg_1 != std::string::npos) {
              break;
            }
            operand = currentText.operand;
            operand = operand.substr(operand.find("$") + 1);
            operand = operand.substr(0, operand.find(",")), 0, 0;
            dest = std::stoi(operand);
            cmp = output.op[3];
            msg_1 = operand.find("1000");
            msg_2 = operand.find("100");
            msg_3 = operand.find("10");
            msg_4 = operand.find("32");
            if (msg_1 != std::string::npos) {
                placeholder = "1000";
                imm = std::stoi(placeholder);
            }
            else if (msg_2 != std::string::npos) {
                placeholder = "100";
                imm = std::stoi(placeholder);
            }
            else if (msg_3 != std::string::npos) {
                placeholder = "10";
                imm = std::stoi(placeholder);
            }
            else if (msg_4 != std::string::npos) {
              placeholder = "32";
              imm = std::stoi(placeholder);
            }
            else if (cmp.compare(" ") == 0) {
                imm = output.op[4] - 48;
            }
            else {
                imm = output.op[3] - 48;
            }
            register_mem[dest] = imm;
            output.rd = dest;
            output.op1 = imm;
            C += 3;
            IC++;
            break;
          // decode subi
          case 9: 
            cout << "\nsubi " << output.op << " decoded";
            operand = output.op;
            operand = operand.substr(operand.find("$") + 1);
            operand = operand.substr(0, operand.find(",")), 0, 0;
            dest = std::stoi(operand);
            operand = operand.substr(operand.find("$") + 1);
            operand = operand.substr(0, operand.find(",")), 0, 0;
            src1 = std::stoi(operand);
            operand = output.op[6];
            imm = std::stoi(operand);
            register_mem[dest] = register_mem[src1] - imm;
            output.rd = dest;
            output.rs = src1;
            output.op1 = register_mem[src1];
            output.op2 = imm;
            C += 6;
            IC++;
            break;
          // decode syscall
          case 10: 
            cout << "\nsyscall decoded";
            float speedup;
            offset = register_mem[29];
            C += 8;
            IC++;
            if (filename.compare("lab3b.s") == 0){
              offset = register_mem[29];
                if (offset == 1) {
                    result = stringArr[register_mem[31]];
                }
                if (offset == 10) {
                    cout << "\nend decoded\nsyscall executed\nend executed";
                    cout << "\n\nSIMULATION COMPLETE\n\nC: " << C << "\nIC: " << IC;
                    cout << "\nNOPS: " << nopcount << "\n\nRESULT: " << result;
                    std::ofstream ofs;
                    ofs.open ("result.txt", std::ofstream::out | std::ofstream::trunc); // line referenced from source
                    ofs << stringArr[0];
                    ofs << "\n-----------------";
                    ofs << "\nC: " << C << "\nIC: " << IC;
                    ofs << "\nNOPS: " << nopcount;
                    ofs << "\n\n" << result;
                    ofs.close();
                    cout << "\n\nEXITING PROGRAM\n";
                    exit (EXIT_SUCCESS);
                }
            }
              else {
                offset = register_mem[2];
                if (offset == 10) {
                  cout << "\nend decoded\nsyscall executed\nend executed";
                  cout << "\n\nSIMULATION COMPLETE";
                  if (filename.compare("lab3c.s") == 0){
                    cout << "\n\nRESULT: " << register_mem[4];
                  }
                  cout << "\n\nC: " << C << "\nIC: " << IC;
                  cout << "\nNOPS: " << nopcount;
                  std::ofstream ofs;
                  ofs.open ("result.txt", std::ofstream::out | std::ofstream::trunc); // line referenced from source
                  if (filename.compare("lab3c.s") == 0){
                  ofs << "RESULT: " << register_mem[4];
                  }
                  ofs << "\n-----------------";
                  ofs << "\nC: " << C << "\nIC: " << IC;
                  ofs << "\nNOPS: " << nopcount;
                  ofs << "\n\n" << result;
                  ofs.close();
                  cout << "\n\nEXITING PROGRAM\n";
                  exit (EXIT_SUCCESS);
                }
              }
            break;
          // decode addi
          case 11: 
            cout << "\naddi " << output.op << " decoded";
            cout << "\naddi " << output.op;
            operand = output.op;
            operand = operand.substr(operand.find("$") + 1);
            operand = operand.substr(0, operand.find(",")), 0, 0;
            dest = std::stoi(operand);
            operand = operand.substr(operand.find("$") + 1);
            operand = operand.substr(0, operand.find(",")), 0, 0;
            src1 = std::stoi(operand);
            operand = output.op[6];
            imm = std::stoi(operand);
            register_mem[dest] = register_mem[src1] + imm;
            output.rd = dest;
            output.rs = src1;
            output.op1 = register_mem[src1];
            output.op2 = imm;
            C += 6;
            IC++;
            break;
          // decode nop
          case 12: 
            cout << "\nnop decoded";
            nopcount++;
            break;
          // decode end
          case 13: 
            cout << "\nend decoded";
            break;
        }
    id_exe_next = output;
		mem_exe_prev = mem_exe_next;
    mem_exe_prev.alu_out = 0; 
		mem_wb_prev = mem_wb_next;
		mem_exe val;
    // apply forwarding techniques
    if (id_exe_prev.rs == mem_exe_prev.rd) {
      id_exe_prev.op1 = mem_exe_prev.alu_out; 
    }
    if (id_exe_prev.rt == mem_exe_prev.rd) {
      id_exe_prev.op2 = mem_exe_prev.alu_out;
    }
    if (mem_wb_next.rd == id_exe_prev.rs) { 
      id_exe_prev.op1 = mem_wb_next.alu_out; 
    }
    if (mem_wb_next.rd == id_exe_prev.rt) {
      id_exe_prev.op2 = mem_wb_next.alu_out;
    }
    switch (id_exe_prev.op_code) {
      // execute add
      case 1: 
        cout << "\nadd executed";
        val.alu_out = id_exe_prev.op1 + id_exe_prev.op2; 
        break;
      // execute b 
      case 2: 
        cout << "\nb executed";
        break;
      // execute beqz
      case 3: 
        cout << "\nbeqz executed";
        break;
      // execute bge
      case 4: 
        cout << "\nbge executed";
        break;
      // execute bne
      case 5: 
        cout << "\nbne executed";
        break;
      // execute la
      case 6: 
        cout << "\nla executed";
        val.alu_out = id_exe_prev.op1; 
        break;
      // execute lb
      case 7: 
        cout << "\nlb executed";
        val.alu_out = id_exe_prev.op1; 
        break;
      // execute li
      case 8: 
        cout << "\nli executed";
        val.alu_out = id_exe_prev.op1; 
        break;
      // execute subi
      case 9: 
        cout << "\nsubi executed";
        val.alu_out = id_exe_prev.op1 - id_exe_prev.op2; 
        break;
      // execute syscall
      case 10 :
        cout << "\nsyscall executed";
        break;
      // execute addi
      case 11: 
        cout << "\naddi executed";
        val.alu_out = id_exe_prev.op1 + id_exe_prev.op2; 
        break;
      // execute nop
      case 12: 
        cout << "\nnop executed";
        break;
      // execute end
      case 13: 
        cout << "\nend executed";
        user_mode = false;
      default: 
        break;
    }
    val.op_code = id_exe_prev.op_code;
    val.op2 = id_exe_prev.op2;
    val.rd = id_exe_prev.rd;
    mem_exe_next = val;
    PC++;
    }  
}