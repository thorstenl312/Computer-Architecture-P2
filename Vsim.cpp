 //On my honor, I have neither given nor received any unauthorized aid on this assignment.
#include <iostream>
#include <climits>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <cstring>

using namespace std;

vector<string> instructionVec;
vector<long long> dataVec;

vector<string> split(string str){
    vector<string> v;
    int start = 0, end = 0;
    while ((start = str.find_first_not_of(" ", end)) != string::npos) {
        end = str.find(" ", start);
        v.push_back(str.substr(start, end - start));
    }
    return v;
}
void printFunctionalUnits(ofstream& sim, string* functionalUnits){
    sim << "IF Unit:" << endl;
    sim << "\tWaiting: " << functionalUnits[0] << endl;
    sim << "\tExecuted: " << functionalUnits[1] << endl;
    sim << "Pre-Issue Queue:" << endl;
    sim << "\tEntry 0: " << functionalUnits[2] << endl;
    sim << "\tEntry 1: " << functionalUnits[3] << endl;
    sim << "\tEntry 2: " << functionalUnits[4] << endl;
    sim << "\tEntry 3: " << functionalUnits[5] << endl;
    sim << "Pre-ALU1 Queue:" << endl;
    sim << "\tEntry 0: " << functionalUnits[6] << endl;
    sim << "\tEntry 1: " << functionalUnits[7] << endl;
    sim << "Pre-MEM Queue: " << functionalUnits[8] << endl;
    sim << "Post-MEM Queue: " << functionalUnits[9] << endl;
    sim << "Pre-ALU2 Queue: " << functionalUnits[10] << endl;
    sim << "Post-ALU2 Queue: " << functionalUnits[11] << endl;
    sim << "Pre-ALU3 Queue: " << functionalUnits[12] << endl;
    sim << "Post-ALU3 Queue: " << functionalUnits[13] << endl << endl;
}
int main(int argc, char *argv[]) {
    ifstream sample(argv[1]);
    ofstream disassembly("disassembly.txt");
    ofstream simulation("simulation.txt");
    string line;
    string address = "256";
    string breakAddress = "256";
    while (getline(sample, line)) {
        string category = line.substr(30, 2);
        string opcode = line.substr(25, 5);
        string instruction;
        string imm;
        string rd;
        string rs1;
        string rs2;
        string func7;
        disassembly << line << "\t" << address << "\t";
        if(category == "00"){
            rd = line.substr(20, 5);
            rs1 = line.substr(12, 5);
            rs2 = line.substr(7, 5);
            func7 = line.substr(0, 7);
            bitset<5> reg1(rs1);
            bitset<5> reg2(rs2);
            bitset<5> dest(rd);
            bitset<12> func(func7 + rd);
            if(opcode == "00000"){
                instruction = "beq";
            }
            if(opcode == "00001"){
                instruction = "bne";
            }
            if(opcode == "00010"){
                instruction = "blt";
            }
            if(opcode == "00011"){
                instruction = "sw";
            }
            if(instruction != "sw"){
                instructionVec.push_back(instruction + " x" + to_string(reg1.to_ulong()) + ", x" + to_string(reg2.to_ulong()) + ", #" + to_string(dest.to_ulong()));
                disassembly << instructionVec.back() << endl;
            }
            else{
                instructionVec.push_back(instruction + " x" + to_string(reg1.to_ulong()) + ", " + to_string(func.to_ulong()) + "(x" + to_string(reg2.to_ulong()) + ")");
                disassembly << instructionVec.back() << endl;
            }
        }
        else if(category == "01"){
            rd = line.substr(20, 5);
            rs1 = line.substr(12, 5);
            rs2 = line.substr(7, 5);
            bitset<5> reg1(rs1);
            bitset<5> reg2(rs2);
            bitset<5> dest(rd);
            if(opcode == "00000"){
                instruction = "add";
            }
            if(opcode == "00001"){
                instruction = "sub";
            }
            if(opcode == "00010"){
                instruction = "and";
            }
            if(opcode == "00011"){
                instruction = "or";
            }
            instructionVec.push_back(instruction + " x" + to_string(dest.to_ulong()) + ", x" + to_string(reg1.to_ulong()) + ", x" + to_string(reg2.to_ulong()));
            disassembly << instructionVec.back() << endl;
        }
        else if(category == "10"){
            rd = line.substr(20, 5);
            rs1 = line.substr(12, 5);
            imm = line.substr(0, 12);
            bitset<5> reg1(rs1);
            bitset<5> dest(rd);
            bitset<12> immediate(imm);
            long decimalImmediate = INT_MAX;
            if(immediate[11] == 1){
                immediate = ~immediate;
                decimalImmediate = -(immediate.to_ulong() + 1);
            }
            if(opcode == "00000"){
                instruction = "addi";
            }
            if(opcode == "00001"){
                instruction = "andi";
            }
            if(opcode == "00010"){
                instruction = "ori";
            }
            if(opcode == "00011"){
                instruction = "sll";
            }
            if(opcode == "00100"){
                instruction = "sra";
            }
            if(opcode == "00101"){
                instruction = "lw";
            }
            if(instruction != "lw"){
                instructionVec.push_back(instruction + " x" + to_string(dest.to_ulong()) + ", x" + to_string(reg1.to_ulong()) + ", #" + to_string(min((long)immediate.to_ulong(), decimalImmediate)));
                disassembly << instructionVec.back() << endl;
            }
            else{
                instructionVec.push_back(instruction + " x" + to_string(dest.to_ulong()) + ", " + to_string(min((long)immediate.to_ulong(), decimalImmediate)) + "(x" + to_string(reg1.to_ulong()) + ")");
                disassembly << instructionVec.back() << endl;
            }
        }
        else if(category == "11"){
            if(opcode == "00000"){
                bool invert = line.at(0) - '0';
                bitset<20> value(line.substr(0, 20));
                bitset<5> reg(line.substr(20, 5));
                string jal = "jal x" + to_string(reg.to_ulong()) + ", #";
                if(invert){
                    jal += "-" + to_string(1048576 - value.to_ulong());
                }
                else{
                    jal += to_string(value.to_ulong());
                }
                instructionVec.push_back(jal);
                disassembly << instructionVec.back() << endl;
            }
            else if(opcode == "11111"){
                instructionVec.push_back("break");
                breakAddress = address;
                disassembly << instructionVec.back() << endl;
                break;
            }
        }
        address = to_string(stoi(address) + 4);
    }
    //After break, print out 2's complement of values
    while (getline(sample, line)) {
        address = to_string(stoi(address) + 4);
        bool invert = line.at(0) - '0';
        bitset<32> value(line);
        disassembly << line << "\t" << address << "\t";
        if(invert){
            dataVec.push_back(value.to_ulong() - 4294967296);
            disassembly << dataVec.back() << endl;
        }
        else{
            dataVec.push_back(value.to_ulong());
            disassembly << dataVec.back() << endl;
        }
    }
    // Close the file
    sample.close();

    int registers[4][8];
    memset(registers, 0, sizeof(registers));

    string functionalUnits[14];
    int registerWrite[31];
    memset(registerWrite, 0, sizeof(registerWrite));
    int cycle = 0;
    int i = 0;
    int prefecth = 2;
    int totalFetch = 0;
    bool executing = false;
    int waitUntilALU3 = 0;
    bool clearExectuedNext = false;
    address = "256";
    while(cycle < 70){
        simulation << "--------------------" << endl;
        simulation << "Cycle " << ++cycle << endl << endl;
        int initial = i;
        
        if(clearExectuedNext == true){
            functionalUnits[1] = "";
            clearExectuedNext = false;
        }
        if(functionalUnits[8] != "" && functionalUnits[8].substr(1,2) == "sw"){
                vector<string> instr = split(functionalUnits[8]);
                int reg1 = stoi(instr[1].substr(1, instr[1].size() - 2));
                int dataAddress = stoi(instr[2].substr(0, instr[2].find("(")));
                int offsetReg = stoi(instr[2].substr(instr[2].find("(") + 2, instr[2].find(")") - instr[2].find("(") - 2));
                int dataStartingAddress = stoi(breakAddress) + 4;
                dataVec.at((dataAddress + registers[offsetReg/8][offsetReg%8] - dataStartingAddress)/4) = registers[reg1/8][reg1%8];
                registerWrite[reg1] = 0;
                functionalUnits[8] = "";
        }
        if(functionalUnits[0] != "" && functionalUnits[2] == "" && functionalUnits[8] == "" && functionalUnits[9] == "" && functionalUnits[10] == "" && functionalUnits[11] == "" && functionalUnits[12] == ""  && functionalUnits[13] == ""){
            functionalUnits[1] = functionalUnits[0];
            functionalUnits[0] = "";
        }
        else if((functionalUnits[1] != "" && functionalUnits[2] == "" && functionalUnits[8] == "" && functionalUnits[9] == "" && functionalUnits[10] == "" && functionalUnits[11] == "" && functionalUnits[12] == "" &&  functionalUnits[13] == "")){
            vector<string> instr = split(functionalUnits[1]);
            if(instr[0] == "[beq"){
                int reg1 = stoi(instr[1].substr(1, instr[1].size() - 2));
                int reg2 = stoi(instr[2].substr(1, instr[2].size() - 2));
                if(registers[reg1/8][reg1%8] == registers[reg2/8][reg2%8]){
                    int change = 2 * stoi(instr[3].substr(1, instr[3].length() - 2)) - 4 ;
                    i += change/4;
                }
                registerWrite[reg1] = 0;
            }
            else if(instr[0] == "[bne"){
                int reg1 = stoi(instr[1].substr(1, instr[1].size() - 2));
                int reg2 = stoi(instr[2].substr(1, instr[2].size() - 2));
                if(registers[reg1/8][reg1%8] != registers[reg2/8][reg2%8]){
                    int change = 2 * stoi(instr[3].substr(1, instr[3].length() - 2)) - 4;
                    i += change/4;
                }
                registerWrite[reg1] = 0;
            }
            else if(instr[0] == "[blt"){
                int reg1 = stoi(instr[1].substr(1, instr[1].size() - 2));
                int reg2 = stoi(instr[2].substr(1, instr[2].size() - 2));
                if(registers[reg1/8][reg1%8] < registers[reg2/8][reg2%8]){
                    int change = 2 * stoi(instr[3].substr(1, instr[3].length() - 2)) - 4;
                    address = to_string(stoi(address) + change);
                    i += change/4;
                }
                registerWrite[reg1] = 0;
            }
            else if(instr[0] == "[jal"){
                int reg1 = stoi(instr[1].substr(1, instr[1].size() - 2));
                int reg2 = stoi(instr[2].substr(1, instr[2].size() - 2));
                if(registers[reg1/8][reg1%8] < registers[reg2/8][reg2%8]){
                    int change = 2 * stoi(instr[3].substr(1, instr[3].length() - 2)) - 4;
                    address = to_string(stoi(address) + change);
                    i += change/4;
                }
                registerWrite[reg1] = 0;
            }
            functionalUnits[1] = "";
        }
        else if(functionalUnits[9] != ""){
            int reg1;
            if(functionalUnits[9] != "" && functionalUnits[9].substr(1,2) == "lw"){
                vector<string> instr = split(functionalUnits[9]);
                reg1 = stoi(instr[1].substr(1, instr[1].size() - 2));
                int dataAddress = stoi(instr[2].substr(0, instr[2].find("(")));
                int offsetReg = stoi(instr[2].substr(instr[2].find("(") + 2, instr[2].find(")") - instr[2].find("(") - 2));
                int dataStartingAddress = stoi(breakAddress) + 4;
                registers[reg1/8][reg1%8] = dataVec.at((dataAddress + registers[offsetReg/8][offsetReg%8] - dataStartingAddress)/4);
                registerWrite[reg1] = 0;
                vector<string> nextInstruction = split(functionalUnits[2]);
                int dest = reg1;
                if(functionalUnits[2] != "" && ((nextInstruction[0].length() == 3 && stoi(nextInstruction[2].substr(nextInstruction[2].find("(") + 2, nextInstruction[2].find(")") - nextInstruction[2].find("(") - 2)) == dest) || 
                ((nextInstruction[0].length() == 5 && stoi(nextInstruction[2].substr(1, nextInstruction[2].size() - 2)) == dest)) || 
                ((nextInstruction[0].length() == 4 && nextInstruction[3][0] == '#' && stoi(nextInstruction[2].substr(1, nextInstruction[2].size() - 2)) == dest)) || 
                ((nextInstruction[0].length() == 4 && nextInstruction[3][0] != '#' && (stoi(nextInstruction[2].substr(1, nextInstruction[2].size() - 2)) == dest || stoi(nextInstruction[3].substr(1, nextInstruction[3].size() - 2)) == dest))))){
                    waitUntilALU3 = 1;
                }
            }
            if(functionalUnits[8] != "" && functionalUnits[8].substr(1,2) == "lw"){
                functionalUnits[9] = functionalUnits[8];
                functionalUnits[8] = "";
            }
            else{
                functionalUnits[9] = "";
            }
        }
        else if(functionalUnits[11] != ""){
            vector<string> instr = split(functionalUnits[11]);
            if(functionalUnits[11].substr(1,4) == "addi"){
                int dest = stoi(instr[1].substr(1, instr[1].size() - 2));
                int reg1 = stoi(instr[2].substr(1, instr[2].size() - 2));
                int imm = stoi(instr[3].substr(1, instr[3].size() - 2));    
                registers[dest/8][dest%8] = registers[reg1/8][reg1%8] + imm;
                registerWrite[dest] = 0;
            }
            else if(functionalUnits[11].substr(1,3) == "sub"){
                int dest = stoi(instr[1].substr(1, instr[1].size() - 2));
                int reg1 = stoi(instr[2].substr(1, instr[2].size() - 2));
                int reg2 = stoi(instr[3].substr(1, instr[3].size() - 2));
                registers[dest/8][dest%8] = registers[reg1/8][reg1%8] - registers[reg2/8][reg2%8];
                registerWrite[dest] = 0;
            }
            else if(functionalUnits[11].substr(1,3) == "add"){
                int dest = stoi(instr[1].substr(1, instr[1].size() - 2));
                int reg1 = stoi(instr[2].substr(1, instr[2].size() - 2));
                int reg2 = stoi(instr[3].substr(1, instr[3].size() - 2));
                registers[dest/8][dest%8] = registers[reg1/8][reg1%8] + registers[reg2/8][reg2%8];
                registerWrite[dest] = 0;
            }
            vector<string> nextInstruction = split(functionalUnits[2]);
            int dest = stoi(instr[1].substr(1, instr[1].size() - 2));
            if(functionalUnits[2] != "" && ((nextInstruction[0].length() == 3 && stoi(nextInstruction[2].substr(nextInstruction[2].find("(") + 2, nextInstruction[2].find(")") - nextInstruction[2].find("(") - 2)) == dest) || 
            ((nextInstruction[0].length() == 5 && stoi(nextInstruction[2].substr(1, nextInstruction[2].size() - 2)) == dest)) || 
            ((nextInstruction[0].length() == 4 && nextInstruction[3][0] == '#' && stoi(nextInstruction[2].substr(1, nextInstruction[2].size() - 2)) == dest)) || 
            ((nextInstruction[0].length() == 4 && nextInstruction[3][0] != '#' && (stoi(nextInstruction[2].substr(1, nextInstruction[2].size() - 2)) == dest || stoi(nextInstruction[3].substr(1, nextInstruction[3].size() - 2)) == dest))))){
                waitUntilALU3 = 1;
            }
            functionalUnits[11] = "";
        }
        else if(functionalUnits[13] != ""){
            vector<string> instr = split(functionalUnits[13]);
            if(instr[0] == "[and"){
                int dest = stoi(instr[1].substr(1, instr[1].size() - 2));
                int reg1 = stoi(instr[2].substr(1, instr[2].size() - 2));
                int reg2 = stoi(instr[3].substr(1, instr[3].size() - 2));
                registers[dest/8][dest%8] = registers[reg1/8][reg1%8] & registers[reg2/8][reg2%8];
                registerWrite[dest] = 0;
            }
            else if(instr[0] == "[or"){
                int dest = stoi(instr[1].substr(1, instr[1].size() - 2));
                int reg1 = stoi(instr[2].substr(1, instr[2].size() - 2));
                int reg2 = stoi(instr[3].substr(1, instr[3].size() - 2));
                registers[dest/8][dest%8] = registers[reg1/8][reg1%8] | registers[reg2/8][reg2%8];
                registerWrite[dest] = 0;
            }
            else if(instr[0] == "[andi"){
                int dest = stoi(instr[1].substr(1, instr[1].size() - 2));
                int reg1 = stoi(instr[2].substr(1, instr[2].size() - 2));
                int imm = stoi(instr[3].substr(1, instr[3].size() - 2));
                registers[dest/8][dest%8] = registers[reg1/8][reg1%8] & imm;
                registerWrite[dest] = 0;
            }
            else if(instr[0] == "[ori"){
                int dest = stoi(instr[1].substr(1, instr[1].size() - 2));
                int reg1 = stoi(instr[2].substr(1, instr[2].size() - 2));
                int imm = stoi(instr[3].substr(1, instr[3].size() - 2));
                registers[dest/8][dest%8] = registers[reg1/8][reg1%8] | imm;
                registerWrite[dest] = 0;
            }
            else if(instr[0] == "[sll"){
                int dest = stoi(instr[1].substr(1, instr[1].size() - 2));
                int reg1 = stoi(instr[2].substr(1, instr[2].size() - 2));
                int imm = stoi(instr[3].substr(1, instr[3].size() - 2));
                registers[dest/8][dest%8] = registers[reg1/8][reg1%8] << imm;
                registerWrite[dest] = 0;
            }
            else if(instr[0] == "[sra"){
                int dest = stoi(instr[1].substr(1, instr[1].size() - 2));
                int reg1 = stoi(instr[2].substr(1, instr[2].size() - 2));
                int imm = stoi(instr[3].substr(1, instr[3].size() - 2));
                registers[dest/8][dest%8] = registers[reg1/8][reg1%8] >> imm;
                registerWrite[dest] = 0;
            }
            vector<string> nextInstruction = split(functionalUnits[2]);
            int dest = stoi(instr[1].substr(1, instr[1].size() - 2));
            if(functionalUnits[2] != "" && ((nextInstruction[0].length() == 3 && stoi(nextInstruction[2].substr(nextInstruction[2].find("(") + 2, nextInstruction[2].find(")") - nextInstruction[2].find("(") - 2)) == dest) || 
            ((nextInstruction[0].length() == 5 && stoi(nextInstruction[2].substr(1, instr[2].size() - 2)) == dest)) || 
            ((nextInstruction[0].length() == 4 && nextInstruction[3][0] == '#' && stoi(nextInstruction[2].substr(1, nextInstruction[2].size() - 2)) == dest)) || 
            ((nextInstruction[0].length() == 4 && nextInstruction[3][0] != '#' && (stoi(nextInstruction[2].substr(1, nextInstruction[2].size() - 2)) == dest || stoi(nextInstruction[3].substr(1, nextInstruction[3].size() - 2)) == dest))))){
                waitUntilALU3 = 1;
            }
            functionalUnits[13] = "";
        }
        else if(functionalUnits[8] != "" && functionalUnits[8].substr(1,2) == "lw"){
            functionalUnits[9] = functionalUnits[8];
            functionalUnits[8] = functionalUnits[6];
            functionalUnits[6] = functionalUnits[7];
            functionalUnits[6] = "";
            executing = true;
        }
        else if(functionalUnits[10] != ""){
            functionalUnits[11] = functionalUnits[10];
            functionalUnits[10] = "";
            executing = true;
        }
        else if(functionalUnits[12] != ""){
            functionalUnits[13] = functionalUnits[12];
            functionalUnits[12] = "";
            executing = true;
        }
        if(functionalUnits[9] != ""){
            executing = true;
        }
        if(functionalUnits[6] != ""){
            functionalUnits[8] = functionalUnits[6];
            functionalUnits[6] = functionalUnits[7];
            functionalUnits[7] = "";
            if(functionalUnits[2][2] == 'w'){
                functionalUnits[6] = functionalUnits[2];
                
                functionalUnits[2] = functionalUnits[3];
                functionalUnits[3] = functionalUnits[4];
                functionalUnits[4] = functionalUnits[5];
                functionalUnits[5] = "";
            }
            prefecth--;
            executing = true;
        }
        if(functionalUnits[2] != "" && !executing && waitUntilALU3 <= 0){
            if(functionalUnits[2].substr(1,2) == "sw"){
                if(functionalUnits[6] == ""){
                    functionalUnits[6] = functionalUnits[2];

                    functionalUnits[2] = functionalUnits[3];
                    functionalUnits[3] = functionalUnits[4];
                    functionalUnits[4] = functionalUnits[5];
                    functionalUnits[5] = "";
                }
                else if(functionalUnits[7] == ""){
                    functionalUnits[7] = functionalUnits[2];
                    
                    functionalUnits[2] = functionalUnits[3];
                    functionalUnits[3] = functionalUnits[4];
                    functionalUnits[4] = functionalUnits[5];
                    functionalUnits[5] = "";
                }
                prefecth--;
            }
            if(functionalUnits[2] != "" && functionalUnits[2].substr(1,2) == "lw"){
                if(functionalUnits[6] == ""){
                    functionalUnits[6] = functionalUnits[2];

                    functionalUnits[2] = functionalUnits[3];
                    functionalUnits[3] = functionalUnits[4];
                    functionalUnits[4] = functionalUnits[5];
                    functionalUnits[5] = "";
                }
                else if(functionalUnits[7] == ""){
                    functionalUnits[7] = functionalUnits[2];
                    
                    functionalUnits[2] = functionalUnits[3];
                    functionalUnits[3] = functionalUnits[4];
                    functionalUnits[4] = functionalUnits[5];
                    functionalUnits[5] = "";
                }
                prefecth--;
            }
            else if(functionalUnits[2] != "" && (functionalUnits[2][2] == 'u' || functionalUnits[2][2] == 'd') && functionalUnits[10] == "" && functionalUnits[11] == ""){
                functionalUnits[10] = functionalUnits[2];

                functionalUnits[2] = functionalUnits[3];
                functionalUnits[3] = functionalUnits[4];
                functionalUnits[4] = functionalUnits[5];
                functionalUnits[5] = "";
                prefecth--;
            }
            else if(functionalUnits[2] != "" && (functionalUnits[2][2] == 'n' || functionalUnits[2][2] == 'r' || functionalUnits[2][2] == 'l') && functionalUnits[12] == "" && functionalUnits[13] == ""){
                functionalUnits[12] = functionalUnits[2];

                functionalUnits[2] = functionalUnits[3];
                functionalUnits[3] = functionalUnits[4];
                functionalUnits[4] = functionalUnits[5];
                functionalUnits[5] = "";
                prefecth--;
            }
        }
        if(functionalUnits[0] == "" && functionalUnits[1] == ""){
            totalFetch = 0;
            int allowedFetches = 0;
            for(int m = 2; m < 6; m++){
                if(functionalUnits[m] != ""){
                    allowedFetches++;
                }
            }
            if(allowedFetches == 4){
                allowedFetches = 0;
            }
            else if(allowedFetches == 3){
                allowedFetches = 1;
            }
            else{
                allowedFetches = 2;
            }
            for(int j = initial; j < initial + allowedFetches; j++){
                vector<string> instruction = split(instructionVec[i]);
                if(instructionVec[i].substr(0,3) == "beq" || instructionVec[i].substr(0,3) == "bne" || instructionVec[i].substr(0,3) == "blt"){
                    int reg1 = stoi(instruction[1].substr(1, instruction[1].size() - 2));
                    int reg2 = stoi(instruction[2].substr(1, instruction[2].size() - 2));
                    if(registerWrite[reg1] != 0 || registerWrite[reg2] != 0){
                        functionalUnits[0] = "[" + instructionVec[i] + "]";
                        i++;
                        break;
                    }
                    else{
                        functionalUnits[1] = "[" + instructionVec[i] + "]";
                        vector<string> instr = split(functionalUnits[1]);
                        if(instr[0] == "[beq"){
                            int reg1 = stoi(instr[1].substr(1, instr[1].size() - 2));
                            int reg2 = stoi(instr[2].substr(1, instr[2].size() - 2));
                            if(registers[reg1/8][reg1%8] == registers[reg2/8][reg2%8]){
                                int change = 2 * stoi(instr[3].substr(1, instr[3].length() - 2)) - 4 ;
                                address = to_string(stoi(address) + change);
                                i += change/4;
                            }
                        }
                        else if(instr[0] == "[bne"){
                            int reg1 = stoi(instr[1].substr(1, instr[1].size() - 2));
                            int reg2 = stoi(instr[2].substr(1, instr[2].size() - 2));
                            if(registers[reg1/8][reg1%8] != registers[reg2/8][reg2%8]){
                                int change = 2 * stoi(instr[3].substr(1, instr[3].length() - 2)) - 4;
                                address = to_string(stoi(address) + change);
                                i += change/4;
                            }
                        }
                        else if(instr[0] == "[blt"){
                            int reg1 = stoi(instr[1].substr(1, instr[1].size() - 2));
                            int reg2 = stoi(instr[2].substr(1, instr[2].size() - 2));
                            if(registers[reg1/8][reg1%8] < registers[reg2/8][reg2%8]){
                                int change = 2 * stoi(instr[3].substr(1, instr[3].length() - 2)) - 4;
                                address = to_string(stoi(address) + change);
                                i += change/4;
                            }
                        }
                        clearExectuedNext = true;
                    }
                    i++;
                }
                else if(instructionVec[i].substr(0,3) == "jal"){
                    int dest = stoi(instruction[1].substr(1, instruction[1].size() - 2));
                    address = to_string(stoi("256") + (i) * 4);
                    if(registerWrite[dest] != 0){
                        functionalUnits[0] = "[" + instructionVec[i] + "]";
                        i++;
                        break;
                    }
                    else{
                        functionalUnits[1] = "[" + instructionVec[i] + "]";
                        int dest = stoi(instruction[1].substr(1, instruction[1].size() - 2));
                        int offset = stoi(instruction[2].substr(1, instruction[2].size() - 1));
                        registers[dest/8][dest%8] = stoi(address) + 4;
                        int change = offset * 2 - 4;
                        address = to_string(stoi(address) + change);
                        i += (change)/4;
                        clearExectuedNext = true;
                        i++;
                        break;
                    }
                }
                else if(instructionVec[i].substr(0,5) == "break"){
                    functionalUnits[1] = "[" + instructionVec[i] + "]";
                    printFunctionalUnits(simulation, functionalUnits);
                    for(int j = 0; j < 4; j++){
                        simulation << "x" << setw(2) << setfill('0') << j * 8 << ":";
                        for(int k = 0; k < 8; k++){
                            simulation << "\t" << registers[j][k];
                        }
                        simulation << endl;
                    }
                    
                    simulation << "Data";
                    for(int j = 0; j < dataVec.size(); j++){
                        if(j%8 == 0){
                            simulation << endl;
                            simulation << (stoi(breakAddress) + 4) + (32 * j/8) << ":";
                        }
                        simulation << "\t" << dataVec.at(j);
                    }
                    return 0;
                }
                else{
                    int dest = stoi(instruction[1].substr(1, instruction[1].size() - 2));
                    registerWrite[dest]++;
                    functionalUnits[prefecth] = "[" + instructionVec[i] + "]";
                    prefecth++;
                    i++;
                }
                totalFetch++;
            }
        }

        printFunctionalUnits(simulation, functionalUnits);

        simulation << "Registers" << endl;
        for(int j = 0; j < 4; j++){
            simulation << "x" << setw(2) << setfill('0') << j * 8 << ":";
            for(int k = 0; k < 8; k++){
                simulation << "\t" << registers[j][k];
            }
            simulation << endl;
        }
        
        simulation << "Data";
        for(int j = 0; j < dataVec.size(); j++){
            if(j%8 == 0){
                simulation << endl;
                simulation << (stoi(breakAddress) + 4) + (32 * j/8) << ":";
            }
            simulation << "\t" << dataVec.at(j);
        }

        simulation << endl;
        
        executing = false;
        waitUntilALU3--;
    }
    
    return 0;
}