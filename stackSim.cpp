#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<cstdlib>
#include<cstdio>
#include<sstream>

using namespace std;

typedef unsigned int int32;  

// define data type for .data
typedef struct data { 
    int32 addr;
    string operand;
    int32 content;
} Data;

// define data type for .text
typedef struct text { 
    int32 addr;
    int32 instruction; 
    string operand;
} Text;

// initialize arrays
Text text_segment[50];
Data data_segment[50];
Data stack[50];
char data_mem[20];
char text_mem[20];

// declare string variables
string line;
string cmp;
string type; 

// set starting addresses
int32 data_addr = 0x00000000;
int32 text_addr = 0x00000010;

// initialze indexes
int32 data_index = 0;
int32 text_index = 0;

// declare write address
int32 write_addr;

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
    else if (instr.compare("POP") == 0) {
        return 2; 
    }
    else if (instr.compare("ADD") == 0) {
        return 3;
    }
    else if (instr.compare("MULT") == 0) {
        return 4;
    }
    else if (instr.compare("END") == 0) {
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
    for (int i = 0; i < 5; i++) {
        cout << "\n" << data_segment[i].operand << " " << (int) data_segment[i].content - 48;
    }
}

int main() { 
    cout << "\nREADING stackCode.txt" << "\nFILE CONTENT:";
    storeStck();
    cout << "\n\nSUCCESSFUL READ";
    // initialize program counter
    int PC = 0;
    bool user_mode = true;
    // reset text_addr
    text_addr = 0x00000010;
    // declare currentData
	Data currentData;
    // set stack index to -1
	int top = -1; 
    // set stack contents to 0
	for (int i = 0; i < 49; i++) { 
		stack[i].content = 48;
	}

    cout << "\n\nSTARTING STACK SIMULATOR\nSTACK CONTENT:\n\nBOTTOM <--> TOP"; ;
    while(user_mode) { 
        Text currentText = loadMem(PC + text_addr);
        PC++;
        switch (currentText.instruction) { 
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
            // POP
            case 2: 
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
                break;
            // ADD 
            case 3:
                // initialize sum
				int sum;
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
            case 4: // MULT
				int product; // initialize product
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
            case 5: 
                // exit loop
                user_mode = false;
                break;
            default: 
                user_mode = false; 

        }

    }

    cout << "\n\nSIMULATION COMPLETE\n\nRESULTS:\n";
    printResults();
    cout << "\n\nEXITING PROGRAM\n";

    return 0;
}