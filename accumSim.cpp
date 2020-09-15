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

// define data type for .data
typedef struct data { 
    int32 addr;
    std::string operand;
    int32 content;
} Data;

// define data type for .text
typedef struct text { 
    int32 addr;
    int32 instruction; 
    std::string operand;
} Text;

// initialize arrays
Text text_segment[50];
Data data_segment[50];
Data stack[100];
char data_mem[20];
char text_mem[20];

// declare string variables
std::string line;
std::string cmp;
std::string type; 

// set starting addresses
int32 data_addr = 0x00000000;
int32 text_addr = 0x00000010;

// initialze indexes
int32 data_index = 0;
int32 text_index = 0;

// declare write address
int write_addr;

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
    if (instr.compare("LOAD") == 0) {
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
    else if (instr.compare("STO") != 0) {
        return 5;
    } 
    else { 
        return 6;
    }
}

// read and store text file 
void storeAccum() { 
    fstream accumCode;
    int index;
    //open accumCode.txt 
    accumCode.open("accumCode.txt", ios::in);
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
                        if (cmp.compare("E") != 0) {
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

// print results
void printResults() { 
    std::ofstream ofs;
    ofs.open ("accumOutput.txt", std::ofstream::out | std::ofstream::trunc); // line referenced from source
    for (int i = 0; i < 6; i++) {
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
    int accum;
    cout << "\nREADING accumCode.txt" << "\nFILE CONTENT:\n";
    storeAccum();
    cout << "\n\nSUCCESSFUL READ";
    // initialize program counter
    int PC = 0;
    bool user_mode = true;
    text_addr = 0x00000010;
	Data currentData;
    cout << "\n\nSTARTING ACCUM SIMULATOR\n" ;
    while(user_mode) { 
        Text currentText = loadMem(PC + text_addr);
        switch ((int) currentText.instruction) { 
            // LOAD
            case 1: 
                // find data associated with text operand
				currentData = assignData(currentText.operand);
				accum = currentData.content - 48;
                cout << "\nLOAD " << accum;
                // print results
				break;
            // ADD 
            case 2:
                // initialize sum
				int32 sum;
				sum = 0;
                // find data associated with text operand
                currentData = assignData(currentText.operand);
                sum = sum + currentData.content - 48 + accum;
                accum = sum;
                cout << "\nADD " << accum;
                break;
            // MULT
            case 3: 
                // initialize product
				int32 product; 
				product = 0;
                currentData = assignData(currentText.operand);
                product = (currentData.content - 48) * accum;
                accum = product;
                cout << "\nMULT " << accum;
                break;
            // END
            case 4: 
                // exit loop
                cout << "\nEND";
                user_mode = false;
                break;
            // INVALID
            case 5: 
                cout << "\n\nERROR: INVALID COMMAND. \n\nEXITING PROGRAM";
                exit (EXIT_FAILURE);
                user_mode = false;
                break;
            // STO
            default: 
                // find data associated with text operand
				currentData = assignData(currentText.operand);
                currentData.content = accum;
                // write content to data_segment
                data_segment[write_addr].content = currentData.content + 48;
                cout << "\nSTO " << accum;
                user_mode = true; 
        }
        PC++;

    }

    cout << "\n\nSIMULATION COMPLETE\n\nRESULT:\n" << accum << "\n";
    printResults();
    cout << "\n\nEXITING PROGRAM\n";
    return 0;
}