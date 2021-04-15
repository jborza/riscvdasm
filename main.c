#include <stdio.h>
#include <unistd.h>
#include "disassembler.h"

void process_word(uint32_t offset, FILE *fd)
{
    printf("%8x:", offset);
    uint32_t op = 0u;

    op |= getc(fd);
    op |= getc(fd) << 8;
    op |= getc(fd) << 16;
    op |= getc(fd) << 24;

    printf("\t%08x\t\t", op);
    disassemble_op(&op, offset);
}

void process_stdin()
{
    //read input binary from stdin, output text to stdout
    uint32_t offset = 0;
    while (1)
    {
        process_word(offset, stdin);
        offset += 4;

        int dummy = getc(stdin);
        if (dummy == EOF)
            break;
        ungetc(dummy, stdin);
    }
}

int_fast64_t disassemble_file(char *filename)
{
    FILE *fd = fopen(filename, "r");
    if (!fd)
    {
        printf("Error opening input file.\n");
        return 1;
    }
    fseek(fd, 0, SEEK_END);
    size_t size = ftell(fd);
    rewind(fd);
    uint32_t offset = 0;
    while (offset < size)
    {
        process_word(offset, fd);
        offset += 4;
    }
    fclose(fd);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        //read from standard input, output to standard output
        process_stdin();
        return 0;
    }
    else
    {
        //use input file name
        return disassemble_file(argv[1]);
    }
}