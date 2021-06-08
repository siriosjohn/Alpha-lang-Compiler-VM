#include "execution_enviroment.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long magic_number = -1;

void read_binary(char* filepath) {
    FILE* in = fopen(filepath, "rb");
    char* string;
    size_t size;
    int i;

    fread(&magic_number, sizeof(long long), 1, in);
    if (magic_number != 384838503862) {
        printf("Not an alpha binary file\n");
        exit(1);
    }
     
    /* stringConsts array */
    fread(&totalStringConsts, sizeof(unsigned), 1, in);
    stringConsts = (char**)malloc(totalStringConsts*sizeof(char*));
    for(i = 0; i < totalStringConsts; i++){
        fread(&size, sizeof(size_t), 1, in);
        stringConsts[i] = (char*)malloc(size*sizeof(char));
        fread(stringConsts[i], sizeof(char), size, in);
    }

    /* numConst array */
    fread(&totalNumConsts, sizeof(unsigned), 1, in);
    numConst = (double*)malloc(totalNumConsts*sizeof(double));
    for(i = 0; i < totalNumConsts; i++){
        fread(&numConst[i], sizeof(double), 1, in);
    }

    /* userFuncs array */
    fread(&totalUserFuncs, sizeof(unsigned), 1, in);
    userFuncs = (userfunc*)malloc(totalUserFuncs*sizeof(userfunc));
    for(i = 0; i < totalUserFuncs; i++){
        fread(&(userFuncs[i].address), sizeof(unsigned), 1, in);
        fread(&(userFuncs[i].localSize), sizeof(unsigned), 1, in);
        fread(&size, sizeof(size_t), 1, in);
        userFuncs[i].id = (char*)malloc(size*sizeof(char));
        fread(userFuncs[i].id, sizeof(char), size, in);
    }

    /* lib funcs */
    fread(&totalnamedLibFuncs, sizeof(unsigned), 1, in);
    namedLibfuncs = (char**)malloc(totalnamedLibFuncs*sizeof(char*));
    for(i = 0; i < totalnamedLibFuncs; i++){
        fread(&size, sizeof(size_t), 1, in);
        namedLibfuncs[i] = (char*)malloc(size*sizeof(char));
        fread(namedLibfuncs[i], sizeof(char), size, in);
    }    

    /* instructions */
    fread(&ins_total, sizeof(unsigned), 1, in);
    instructions = (instruction*)malloc(ins_total*sizeof(instruction));
    for (i = 0; i < ins_total; i++) {
        /* opcode */
        fread(&(instructions[i].opcode), sizeof(unsigned int), 1, in);
        /* res */
        fread(&(instructions[i].result.type), sizeof(unsigned int), 1, in);
        fread(&(instructions[i].result.val), sizeof(unsigned), 1, in);
        /* arg1 */
        fread(&(instructions[i].arg1.type), sizeof(unsigned int), 1, in);
        fread(&(instructions[i].arg1.val), sizeof(unsigned), 1, in);
        /* arg2 */
        fread(&(instructions[i].arg2.type), sizeof(unsigned int), 1, in);
        fread(&(instructions[i].arg2.val), sizeof(unsigned), 1, in);

    }

    fclose(in);
}

int count_globals(){
    int max_offset;
    int i;
    for(i = 0; i < ins_total; i++){
        if(instructions[i].result.type == global_a){
            if((int)instructions[i].result.val > max_offset)
                max_offset = (int)instructions[i].result.val;
        }
        if(instructions[i].arg1.type == global_a){
            if((int)instructions[i].arg1.val > max_offset)
                max_offset = (int)instructions[i].arg1.val;
        }
        if(instructions[i].arg2.type == global_a){
            if((int)instructions[i].arg2.val > max_offset)
                max_offset = (int)instructions[i].arg2.val;
        }            
    }

    return max_offset;
}