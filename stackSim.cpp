
#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<cstdlib>
#include<cstdio>
#include<sstream>

using namespace std;
string type; 

typedef unsigned int int32;  
//int32 addr;
string instr;

typedef struct data { 
    int32 addr;
    string operand;
    int32 content;
} Data;

typedef struct text { 
    int32 addr;
    //string instruction; 
    int32 instruction; 
    string operand;
} Text;


Text text_segment[50];
Data data_segment[50];
Data stack[50];

string line;
string cmp;
//string popFix;
char data_mem[20];
char text_mem[20];
int32 data_addr = 0x00000000;
int32 text_addr = 0x00000010;
int32 data_index = 0;
int32 text_index = 0;
int32 write_addr;

void typeCheck(string line) { 
    if (line.compare(".data") == 0) {
        type = "data";
    }
    else if (line.compare(".text") == 0) {
        type = "text";
    }
}

int assignInstruction(string instr) {
    //cout << "\nINSTR: " << instr;
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

void storeStck() { 
    fstream stackCode;
    int index;
    stackCode.open("stackCode.txt", ios::in);
    if (stackCode.is_open()) { 

        while(getline(stackCode, line)) { 
            typeCheck(line);
            //if (type.compare("data") == 0) { 
            if (type.compare("data") == 0) { 

                if (line.compare(".data") != 0 && !line.empty()) { 
                    //add to data
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

                    //cout<< "\n ADDR: " << data_segment[data_index].addr;
                    //cout<< "\n PLACEHOLDER: " << data_segment[data_index].operand;
                    //cout<< "\n PLACEHOLDER: " << data_segment[data_index].content;


                    data_index++;
                    //cout<<"\nData: " << line;
                }

            }
            else if (type.compare("text") == 0) { 

                if (line.compare(".text") != 0 && !line.empty()) { 
                    //add to text
                    //memset(text_mem, 0, sizeof(text_mem));
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
                        //text_segment[text_index].instruction = placeholder;

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
                        //text_segment[text_index].instruction = placeholder;
                        //cout<<"\nTEXT TO COMPARE: " << placeholder;
                        text_segment[text_index].instruction = assignInstruction(placeholder);
                        placeholder = "";
                        index = 0;
                        while (index < 10) { 
                            placeholder = placeholder + text_mem[5 + index];
                            index++;
                        }
                        text_segment[text_index].operand = placeholder;
                    }
                    //cout<< "\n ADDR: " << text_segment[text_index].addr;
                    //cout<< "\n PLACEHOLDER: " << text_segment[text_index].instruction;
                    //cout<< "\n PLACEHOLDER: " << text_segment[text_index].operand;

                    text_index++;
                    //cout<<"\nText: " << line;
                }
            }

            cout<<"\n" << line;
        }
    }

}

Text loadMem(int32 address) {
    for(int i = 0; i < sizeof(text_segment) + 1; i++) {
		if (std::to_string(text_segment[i].addr).compare(std::to_string(address)) == 0) {
        //if ((text_segment[i].addr).compare((int32) address) == 0) {
    		return text_segment[i];
        }
    }
    exit (EXIT_FAILURE);
}

Data assignData(string address) { 
	for(int i = 0; i < sizeof(data_segment) + 1; i++) {
		if ((data_segment[i].operand).compare(address) == 0) {
            write_addr = (int32) i;
    		return data_segment[i];
        }
    }
    exit (EXIT_FAILURE);
}

void printStck() { 

	cout << "\n";
	for (int i = 0; i < 7; i++) {
		cout << " " << (int) stack[i].content - 48;
	}
}

void printResults() { 
    cout << "\n\n"; 
    for (int i = 0; i < 5; i++) {
        cout << "\n" << data_segment[i].operand << " " << (int) data_segment[i].content - 48;
    }
}


int main() { 
    storeStck();

    int PC = 0;
    bool user_mode = true;
    text_addr = 0x00000010;
    //data_addr = 0x00000000;
	Data currentData;
	cout << "\n";


	//initialize stack variables
	int top = -1; 

	//Data emptyData;
	//emptyData.content = 0;

	for (int i = 0; i < 49; i++) { 
		stack[i].content = 48;
	}





    while(user_mode) { 
        //int32 value = 0x00000000;

        Text currentText = loadMem(PC + text_addr);

        PC++;

        switch (currentText.instruction) { 
		//switch(text_segment[PC].instruction) {

            case 1: //PUSH
				currentData = assignData(currentText.operand);
				if (top > 49) { 
					exit (EXIT_FAILURE);
				}
				else {
					stack[top + 1] = currentData;
					top++;
				}

				printStck(); 

				break;
            case 2: //POP
				currentData = assignData(currentText.operand);
				currentData.content = (int) stack[top].content - 48;
				//cout << " POP: " << currentData.operand << " " << currentData.content;
                data_segment[write_addr].content = currentData.content + 48;
				stack[top].content = (int32) 48;
				top--;
				printStck();
                break;
            case 3: //ADD
				int sum;
				sum = 0;
				sum = sum + (int) stack[top - 1].content + (int) stack[top].content - 96;
				//sum = sum + (int) stack[top].content - 48;
				//cout << " SUM: " << sum;
				stack[top].content = (int32) 48;
				top--;
				stack[top].content = sum + 48;
				printStck();
                //cout << "\nADD: " << PC;
                break;
            case 4: //MULT
				int product;
				product = 0;
				product = ((int) stack[top - 1].content - 48) * ((int) stack[top].content - 48);
				stack[top].content = (int32) 48;
				top--;
				stack[top].content = product + 48;
				printStck();

                //cout << "\nMULT: " << PC;
                break;
            case 5: //END 
                user_mode = false;
				//printStck();
                break;
            default: 
                user_mode = false; 

        }
		//PC++;

    }

    printResults();

    return 0;
}