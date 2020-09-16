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

// initialize variables
#define SEGMENT_SIZE 50
#define STACK_SIZE 50
#define BUFFER_SIZE 20
#define DATA_BASE_ADDR 0x00000000
#define TEXT_BASE_ADDR 0x00000010

// define data type for .data
typedef struct data { 
    mem_addr addr;
    std::string operand;
    mem_addr content;
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

// declare write address
int write_addr;

// initialize line count
int line_count = 0;

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
    if (instr.compare("PUSH") == 0) {
        return 1; 
    }
    else if (instr.compare("ADD") == 0) {
        return 2;
    }
    else if (instr.compare("MULT") == 0) {
        return 3;
    }
    else if (instr.compare("END") == 0) {
        return 4;
    }
    else if (instr.compare("POP") != 0) {
        return 5;
    } 
    else { 
        return 6;
    }
}

// read and store text file 
void storeStck() { 
    fstream stackCode;
    int index;
    //open stackCode.txt 
    stackCode.open("stackCode.txt", ios::in);
    // read stackCode.txt
    if (stackCode.is_open()) { 
        // parse ASCII file by line
        while(getline(stackCode, line)) { 
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
                    text_mem[3] = ' ';
                    index = 0;
                    string placeholder = "";
                    text_segment[text_index].addr = text_addr;
                    text_addr++;
                    std::copy(line.begin(), line.end(), text_mem); 
                    cmp = text_mem[3];
                    if (cmp.compare(" ") == 0) { 
                        while (index < 3) { 
                        placeholder = placeholder + text_mem[index];
                        index++;
                        }
                        text_segment[text_index].instruction = assignInstruction(placeholder);
                        cmp = text_mem[0];
                        if (cmp.compare("P") == 0) {
                            placeholder = "";
                            index = 0;
                                while (index < 10) { 
                                placeholder = placeholder + text_mem[4 + index];
                                index++;
                                }
                            text_segment[text_index].operand = placeholder;
                        }
                    }
                    else { 
                        while (index < 4) { 
                            placeholder = placeholder + text_mem[index];
                            index++;
                        }
                        text_segment[text_index].instruction = assignInstruction(placeholder);
                        placeholder = "";
                        index = 0;
                        while (index < 10) { 
                            placeholder = placeholder + text_mem[5 + index];
                            index++;
                        }
                        text_segment[text_index].operand = placeholder;
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

// print current state of stack
void printStck() { 
	cout << "\n";
	for (int i = 0; i < 7; i++) {
		cout << " " << (int) stack[i].content - 48;
	}
}

// print results
void printResults() { 
    std::ofstream ofs;
    ofs.open ("stackOutput.txt", std::ofstream::out | std::ofstream::trunc); // line referenced from source
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
    cout << "\n\nRESULTS WRITTEN TO stackOutput.txt";
}

int main() { 
    
    cout << "\nREADING stackCode.txt" << "\nFILE CONTENT:\n";
    storeStck();
    cout << "\n\nSUCCESSFUL READ";
    // initialize program counter
    int PC = 0;
    bool user_mode = true;
    // reset text_addr
    text_addr = TEXT_BASE_ADDR;
    // declare currentData
	Data currentData;
    // set stack index to -1
	int top = -1; 
    // set stack contents to 0
	for (int i = 0; i < 99; i++) { 
		stack[i].content = 48;
	}

    cout << "\n\nSTARTING STACK SIMULATOR\nSTACK CONTENT:\n\nBOTTOM <--> TOP"; ;
    while(user_mode) { 
        Text currentText = loadMem(PC + text_addr);
        switch ((int) currentText.instruction) { 
            // PUSH
            case 1: 
                // find data associated with text operand
				currentData = assignData(currentText.operand);
				if (top > 49) { 
					exit (EXIT_FAILURE);
				}
                // add data to stack
				else {
					stack[top + 1] = currentData;
                    // increment top
					top++;
				}
                // print results
				printStck(); 
				break;
            // ADD 
            case 2:
                // initialize sum
				int32 sum;
				sum = 0;
                // sum top two stack contents
				sum = sum + (int) stack[top - 1].content + (int) stack[top].content - 96;
                // set top content to 0
				stack[top].content = (int32) 48;
                // decrement top
				top--;
                // set top content to sum
				stack[top].content = sum + 48;
                // print results
				printStck();
                break;
            // MULT
            case 3: 
				int32 product; // initialize product
				product = 0;
                // mult top two stack contents
				product = ((int) stack[top - 1].content - 48) * ((int) stack[top].content - 48);
                // set top content to 0
				stack[top].content = (int32) 48;
                // decrement top
				top--;
                // set top content to product
				stack[top].content = product + 48;
                // print results
				printStck();
                break;
            // END
            case 4: 
                // exit loop
                user_mode = false;
                break;
            // INVALID
            case 5: 
                cout << "\n\nERROR: INVALID COMMAND. \n\nEXITING PROGRAM";
                exit (EXIT_FAILURE);
                user_mode = false;
                break;
            // POP
            default: 
                // find data associated with text operand
				currentData = assignData(currentText.operand);
				currentData.content = (int) stack[top].content - 48;
                // write content to data_segment
                data_segment[write_addr].content = currentData.content + 48;
                // set top to 0
				stack[top].content = (int32) 48;
                // decrement top
				top--;
                // print results
				printStck(); 
                user_mode = false; 
        }
        PC++;

    }

    cout << "\n\nSIMULATION COMPLETE\n\nRESULTS:\n";
    printResults();
    cout << "\n\nEXITING PROGRAM\n";
    return 0;
}