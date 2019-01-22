#include "Constants.h"
#include <math.h>
#include <fstream>
/*
 * Instructions:
 * ADD ADC SUB INV NND RGT CMP CPV
 * IFO IFV IFI PUT MOV JPC PSE HLT
 * 
 * Flags: 
 * Carry out, negative, zero, <, >, =
 * 
 * Interfaces:
 * CNT Line
 * Out Reg
 * Out Dec
 * Input
 * PRGM write line reg
 * PRGM write data
 */

void showReg(int mem[MEM_SIZE], int prgm[ ][4], bool flg[6]){
    string text = "";
    int mem_width = 0, prgm_width[4] = {0,0,0,0};
    for(int i = 0; i < MEM_SIZE; i++){
        
        if(((int)log10(mem[i]!=0?mem[i]:1)+1)>mem_width){
            mem_width = (int)log10(mem[i]!=0?mem[i]:1) + 1;
        }
        for(int n = 0; n < 4; n++){
            
            if(((int)log10(prgm[i][n]!=0?prgm[i][n]:1)+1)>prgm_width[n]){
                prgm_width[n] = (int)log10(prgm[i][n]!=0?prgm[i][n]:1) + 1;
            }
        }
    }
    
    
        text = text + "     FLG\n+-+-+-+-+-+-+\n";
    for(int i = 0; i < 6; i++){
        
        text = text + "|" + to_string(flg[i]?1:0);
    }
    text = text + "|\n+-+-+-+-+-+-+\n\nMEM";
    for(int i = 0; i < mem_width; i++){ text = text + " ";}
    text = text + "PRGM\n";
    
    //+---+
    text = text + "+";
    for(int i = 0; i < mem_width; i++){ text = text + "-";}
    text = text + "+ ";
    for(int i = 0; i < 4; i++){
        
        text = text + "+";
        for(int n = 0; n < prgm_width[i]; n++){
            text = text + "-";
        }
    }
    text = text + "+\n";
    //end line
    
    for(int x = 0; x < MEM_SIZE; x++){
        
        text = text + "|" + to_string(mem[x]);
        for(int i = 0; i < mem_width - (log10(mem[x]!=0?mem[x]:1)+1); i++){
            
            text = text + " ";
        }
        text = text + "| ";
        for(int n = 0; n < 4; n++){
            
            text = text + "|" + to_string(prgm[x][n]);
            for(int i = 0; i < prgm_width[n] - ((int)log10(prgm[x][n]!=0?prgm[x][n]:1) + 1); i++){
                
                text = text + " ";
            }
        }
        text = text + "| " + to_string(x) + "\n";
        //+---+
        text = text + "+";
        for(int i = 0; i < mem_width; i++){ text = text + "-";}
        text = text + "+ ";
        for(int i = 0; i < 4; i++){

            text = text + "+";
            for(int n = 0; n < prgm_width[i]; n++){
                text = text + "-";
            }
        }
        text = text + "+\n";
        //end line
    }
    cout << text;
    

    /*  
     *      FLG
     * +-+-+-+-+-+-+
     * |0|1|1|0|1|1|
     * +-+-+-+-+-+-+
     *
     * MEM PRGM
     * +-+ +-+-+-+-+
     * |0| |0|0|0|0|
     * +-+ +-+-+-+-+
     * |0| |0|0|0|0|
     * +-+ +-+-+-+-+
     */
}

int main(int argc, char** argv) {
    int MEM[MEM_SIZE], PRGM[MEM_SIZE][INSTR_SIZE], ifs[IFS_SIZE];
    bool FLG[FLG_SIZE];
    ifs[0] = 0;
    
    for(int i = 0; i < MEM_SIZE; i++){
        MEM[i] = 0;
        for(int n = 0; n < 4; n++){
            PRGM[i][n] = 0;
        }
    }
        
    string str;
    int op[4];
    bool halt = false;
    
    /*
    int x, y, value;
    while(true){
        cout << "Set value? (Y/N)";
        cin >> str;
        if(str == "y"){
            cout << "Set value in PRGM (Row, cell, value): ";
            cin >> x >> y >> value;
            PRGM[x][y] = value;
        } else {
            break;
        }
    }
    cin.get();
    */
    
    ifstream file("test.txt");
    
    string temp = "";
    int cnt = 0;
    if(file.is_open()){
        while(file){
            for(int i = 0; i != 4; i++){
                file >> PRGM[cnt][i];
            }
            cnt++;
        }
    }
    
    cout << "Execute: ";
    cin.get();
    while(!halt){
        showReg(MEM, PRGM, FLG);
        cout << "Line: " << ifs[0] << "\n"; 
        cout << "Step: ";
        
        cin.get();
        for(int i = 0; i < 4; i++){
            op[i] = PRGM[ifs[0]][i];
        }
        switch(op[0]){
            case 0: //ADD
                MEM[op[3]] = MEM[op[1]] + MEM[op[2]];
                break;
            case 1: //ADC
                MEM[op[3]] = MEM[op[1]] + MEM[op[2]] + FLG[0];
                break;
            case 2: //SUB
                MEM[op[3]] = MEM[op[1]] - MEM[op[2]];
                break;
            case 3: //INV
                // Unknown bit length? How to implement?
                break;
            case 4: //NND
                // Unknown bit length? How to implement?
                break;
            case 5: //RGT
                MEM[op[2]] = MEM[op[1]] / 2;
                break;
            case 6: //CMP
                FLG[3] = op[1]<op[2];
                FLG[4] = op[1]>op[2];
                FLG[5] = op[1]==op[2];
                break;
            case 7: //CPV
                // Unknown bit length? How to implement?
                // op[2] + op[3] should be 8 bits
                break;
            case 8: //IFO
                ifs[op[1]] = MEM[op[2]];
                break;
            case 9: //IFV
                ifs[op[1]] = op[2];
                break;
            case 10: //IFI
                MEM[op[2]] = ifs[op[1]];
                break;
            case 11: //PUT
                MEM[op[2]] = op[1];
                break;
            case 12: //MOV
                MEM[op[2]] = MEM[op[1]];
                break;
            case 13: //JPC
                ifs[0] = FLG[op[1]]?op[2]:ifs[0];
                break;
            case 14: //PSE
                cout << "Press enter to continue";
                cin.get();
                break;
            case 15: //HLT
                halt = true;
                break;
        }
        ifs[0]++;
        if(ifs[0]>= MEM_SIZE){ifs[0]=0;}
    }
    
    return 0;
}

