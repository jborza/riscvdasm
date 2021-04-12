#include <stdio.h>
#include <unistd.h>
#include "disassembler.h"

int main(int argc, char* argv[]){
    //read input binary from stdin, output text to stdout
    uint32_t offset = 0;
    while(1){
        printf("%8X:", offset);
        uint32_t op = 0u;

        op |= getc(stdin);            
        op |= getc(stdin) << 8;       
        op |= getc(stdin) << 16;     
        op |= getc(stdin) << 24;      
        
        printf("\t%08X\t", op);
        disassemble_op(&op);
        offset += 4;

        int dummy = getc(stdin);
        if(dummy == EOF)
            break;
        ungetc(dummy, stdin);
    }
    return 0;
}