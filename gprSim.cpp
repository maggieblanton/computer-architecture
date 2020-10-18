  
/* Referenced http://www.cplusplus.com/reference/fstream/ofstream/open/ 
   for instructions on how to clear text file. 
   Reflected in the printResults() method and commented accordingly. */

#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<cstdlib>
#include<cstdio>
#include<sstream>

using namespace std;

typedef unsigned int int32;  
typedef int32 mem_addr;

// initialize size variables
#define SEGMENT_SIZE 50
#define STACK_SIZE 50
#define BUFFER_SIZE 20
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

// initialize arrays
Text text_segment[SEGMENT_SIZE];
Data data_segment[SEGMENT_SIZE];
Data stack[STACK_SIZE];
char data_mem[BUFFER_SIZE];
char text_mem[BUFFER_SIZE];

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
    fstream accumCode;
    int index;
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
                if (line.compare(".text") != 0 && !line.empty()) { 
                    clearMem();
                    index = 0;
                    string placeholder = "";
                    text_segment[text_index].addr = text_addr;
                    text_addr++;
                    //cout << "\nADDRESS: " << text_segment[text_index].addr;
                    std::copy(line.begin(), line.end(), text_mem); 
                    cmp = text_mem[0];
                    if (cmp.compare("l") == 0) { 
                        while (index < 2) { 
                        placeholder = placeholder + text_mem[index];
                        index++;
                        }
                        text_segment[text_index].instruction = assignInstruction(placeholder);
                        //cout << "\nINSTRUCTION IS " << text_segment[text_index].instruction;
                        index = 0;
                        placeholder = "";
                        while (index < 5) {
                            placeholder = placeholder + text_mem[3 + index];
                            index++;
                        }
                        trim(placeholder);
                        text_segment[text_index].operand = placeholder;
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
                            while (index < 7) {
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
                            //placeholder = text_mem[0];
                            text_segment[text_index].instruction = assignInstruction(placeholder);
                             while (index < 5) {
                                placeholder = placeholder + text_mem[2 + index];
                                index++;
                             }
                            trim(placeholder);
                            text_segment[text_index].operand = placeholder;
                           
                        }
                        else if (cmp.compare("e") == 0) {
                            while (index < 4) {
                                 placeholder = placeholder + text_mem[index];
                                 index++;
                            }
                            text_segment[text_index].instruction = assignInstruction(placeholder);
                            index = 0;
                            placeholder = "";
                            while (index < 6) {
                                placeholder = placeholder + text_mem[5 + index];
                                index++;
                             }
                            trim(placeholder);
                            text_segment[text_index].operand = placeholder;
                        }
                        else { 
                             while (index < 3) {
                                 placeholder = placeholder + text_mem[index];
                                 index++;
                            }
                            text_segment[text_index].instruction = assignInstruction(placeholder);
                            //cout << "\nINSTRUCTION IS " << text_segment[text_index].instruction;
                            index = 0;
                            placeholder = "";
                            while (index < 7) {
                                placeholder = placeholder + text_mem[4 + index];
                                index++;
                             }
                            trim(placeholder);
                            text_segment[text_index].operand = placeholder;
                            //cout << "\nOPERAND IS " << text_segment[text_index].operand;
                        }

                    }
                    text_index++;
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

// find data associated with text operand
Data assignData(string address) { 
	for(int i = 0; i < sizeof(data_segment) + 1; i++) {
		if ((data_segment[i].operand).compare(address) == 0) {
            write_addr = (int32) i;
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
            ofs << "\n" << data_segment[i].operand << " " << (int) data_segment[i].content - 48;
        }
        else { 
            ofs << data_segment[i].operand << " " << (int) data_segment[i].content - 48;
        }
        cout << "\n" << data_segment[i].operand << " " << (int) data_segment[i].content - 48;  
    }
    ofs.close(); // line referenced from source
    cout << "\n\nRESULTS WRITTEN TO accumOutput.txt";
}

int main() { 
    cout << "\nREADING palindrome.s" << "\nFILE CONTENT:\n";
    storeAccum();
    cout << "\n\nSUCCESSFUL READ";
    // initialize program counter
    int PC = 0;
    int IC = 0;
    int C = 0;
    bool user_mode = true;
    text_addr = TEXT_BASE_ADDR;
	Data currentData;
    cout << "\n\nSTARTING SIMULATOR\n" ;
    while(user_mode) { 
        Text currentText = loadMem(PC + text_addr);
        cout << "\nINSTRUCTION " << text_segment[PC].operand;
        switch ((int) currentText.instruction) { 
            // addi
            case 1: 
                cout << "\naddi";
            case 2: 
                cout << "\nb";
            case 3: 
                cout << "\nbeqz";
            case 4: 
                cout << "\nbge";
            case 5: 
                cout << "\nbne";
            case 6: 
                cout << "\nla";
            case 7:
                cout << "\nlb";
            case 8: 
                cout << "\nli";
            case 9: 
                cout << "\nsubi";
            case 10: 
                cout << "\nsyscall";
            default: 
                // find data associated with text operand
				//currentData = assignData(currentText.operand);
                //currentData.content = accum;
                // write content to data_segment
                //data_segment[write_addr].content = currentData.content + 48;
                //cout << "\nSTO " << accum;
                user_mode = false; 
        }
        PC++;
    }

    

    cout << "\n\nSIMULATION COMPLETE\n\nRESULT:\n" << "add result here" << "\n";
    //printResults();
    cout << "\n\nEXITING PROGRAM\n";
    return 0;
}