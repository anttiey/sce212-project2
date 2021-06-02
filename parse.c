/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   SCE212 Ajou University                                    */
/*   parse.c                                                   */
/*   Adapted from CS311@KAIST                                  */
/*                                                             */
/***************************************************************/

#include <stdio.h>

#include "util.h"
#include "parse.h"
#include "run.h"

int text_size;
int data_size;

char* strcpy_part(char* copy, const char* origin, int start, int size) {

    if(strlen(origin) > start) {

        char *src = origin + start;
        char *des = copy;

        while(size > 0 && *src != '\0') {

            *(des) = *(src);

            des++;
            src++;
            size--;

        }

        *des = '\0';
    }

    return copy;
}

int toBinary(const char* origin, int start, int size)
{
    char temp[33];
    strcpy_part(temp, origin, start, size);
    return fromBinary(temp);
}

instruction parsing_instr(const char *buffer, const int index)
{

    /** Implement this function */
    
    instruction instr_s;
    instruction *instr = &instr_s;

    char opcode[7];
    strcpy_part(opcode, buffer, 0, 6);

    uint32_t text = fromBinary(buffer);
    mem_write_32((MEM_TEXT_START + index), text);
    instr->value = text;

    SET_OPCODE(instr, toBinary(buffer, 0, 6));

    if(!strcmp(opcode, "000000")) {

        SET_RS(instr, toBinary(buffer, 6, 5));
        SET_RT(instr, toBinary(buffer, 11, 5));
        SET_RD(instr, toBinary(buffer, 16, 5));
        SET_SHAMT(instr, toBinary(buffer, 21, 5));
        SET_FUNC(instr, toBinary(buffer, 26, 6));

    } else if((!strcmp(opcode, "000010")) || (!strcmp(opcode, "000011"))) {

        SET_TARGET(instr, toBinary(buffer, 6, 26));

    } else {

        SET_RS(instr, toBinary(buffer, 6, 5));
        SET_RT(instr, toBinary(buffer, 11, 5));
        SET_IMM(instr, toBinary(buffer, 16, 16));

    }

    return instr_s;
}

void parsing_data(const char *buffer, const int index)
{
	/** Implement this function */

    uint32_t data = fromBinary(buffer);
    mem_write_32((MEM_DATA_START + index), data);
    
}

void print_parse_result()
{
    int i;
    printf("Instruction Information\n");

    for(i = 0; i < text_size/4; i++)
    {
        printf("INST_INFO[%d].value : %x\n",i, INST_INFO[i].value);
        printf("INST_INFO[%d].opcode : %d\n",i, INST_INFO[i].opcode);

	    switch(INST_INFO[i].opcode)
        {
            //Type I
            case 0x9:		//(0x001001)ADDIU
            case 0xc:		//(0x001100)ANDI
            case 0xf:		//(0x001111)LUI	
            case 0xd:		//(0x001101)ORI
            case 0xb:		//(0x001011)SLTIU
            case 0x23:		//(0x100011)LW
            case 0x2b:		//(0x101011)SW
            case 0x4:		//(0x000100)BEQ
            case 0x5:		//(0x000101)BNE
                printf("INST_INFO[%d].rs : %d\n",i, INST_INFO[i].r_t.r_i.rs);
                printf("INST_INFO[%d].rt : %d\n",i, INST_INFO[i].r_t.r_i.rt);
                printf("INST_INFO[%d].imm : %d\n",i, INST_INFO[i].r_t.r_i.r_i.imm);
                break;

            //TYPE R
            case 0x0:		//(0x000000)ADDU, AND, NOR, OR, SLTU, SLL, SRL, SUBU  if JR
                printf("INST_INFO[%d].func_code : %d\n",i, INST_INFO[i].func_code);
                printf("INST_INFO[%d].rs : %d\n",i, INST_INFO[i].r_t.r_i.rs);
                printf("INST_INFO[%d].rt : %d\n",i, INST_INFO[i].r_t.r_i.rt);
                printf("INST_INFO[%d].rd : %d\n",i, INST_INFO[i].r_t.r_i.r_i.r.rd);
                printf("INST_INFO[%d].shamt : %d\n",i, INST_INFO[i].r_t.r_i.r_i.r.shamt);
                break;

            //TYPE J
            case 0x2:		//(0x000010)J
            case 0x3:		//(0x000011)JAL
                printf("INST_INFO[%d].target : %d\n",i, INST_INFO[i].r_t.target);
                break;

            default:
                printf("Not available instruction\n");
                assert(0);
        }
    }

    printf("Memory Dump - Text Segment\n");
    for(i = 0; i < text_size; i+=4)
        printf("text_seg[%d] : %x\n", i, mem_read_32(MEM_TEXT_START + i));
    for(i = 0; i < data_size; i+=4)
        printf("data_seg[%d] : %x\n", i, mem_read_32(MEM_DATA_START + i));
    printf("Current PC: %x\n", CURRENT_STATE.PC);
}
