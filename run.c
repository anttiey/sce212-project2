/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   SCE212 Ajou University                                    */
/*   run.c                                                     */
/*   Adapted from CS311@KAIST                                  */
/*                                                             */
/***************************************************************/

#include <stdio.h>

#include "util.h"
#include "run.h"

/***************************************************************/
/*                                                             */
/* Procedure: get_inst_info                                    */
/*                                                             */
/* Purpose: Read insturction information                       */
/*                                                             */
/***************************************************************/
instruction* get_inst_info(uint32_t pc)
{
    return &INST_INFO[(pc - MEM_TEXT_START) >> 2];
}

/***************************************************************/
/*                                                             */
/* Procedure: process_instruction                              */
/*                                                             */
/* Purpose: Process one instrction                             */
/*                                                             */
/***************************************************************/


void process_instruction()
{
	/** Implement this function */

    instruction* cur_inst = get_inst_info(CURRENT_STATE.PC);
    CURRENT_STATE.PC += 4;
    
    short opcode = OPCODE(cur_inst);

    // R
    if(opcode == 0x0) {

        uint32_t cur_rt = CURRENT_STATE.REGS[RT(cur_inst)];
        uint32_t cur_rs = CURRENT_STATE.REGS[RS(cur_inst)];
        unsigned char cur_shamt = SHAMT(cur_inst);
        short cur_func = FUNC(cur_inst);

        if(cur_func == 0x00) { // SLL
            CURRENT_STATE.REGS[RD(cur_inst)] = cur_rt << cur_shamt;
        }   
        else if(cur_func== 0x02) { // SRL
            CURRENT_STATE.REGS[RD(cur_inst)] = cur_rt >> cur_shamt;
        }
        else if(cur_func == 0x08) { // JR
            CURRENT_STATE.PC = cur_rs;
        }
        else if(cur_func == 0x21) { // ADDU
            CURRENT_STATE.REGS[RD(cur_inst)] = cur_rs + cur_rt;
        }
        else if(cur_func == 0x23) { // SUBU
            CURRENT_STATE.REGS[RD(cur_inst)] = cur_rs - cur_rt;
        }
        else if(cur_func == 0x24) { // AND
            CURRENT_STATE.REGS[RD(cur_inst)] = cur_rs & cur_rt;
        }
        else if(cur_func == 0x25) { // OR
            CURRENT_STATE.REGS[RD(cur_inst)] = (cur_rs | cur_rt);
        }
        else if(cur_func == 0x27) { // NOR
            CURRENT_STATE.REGS[RD(cur_inst)] = ~(cur_rs | cur_rt);
        }
        else if(cur_func == 0x2b) { // SLTU
            CURRENT_STATE.REGS[RD(cur_inst)] = (cur_rs < cur_rt) ? 1 : 0;
        }
    } 
    // J
    else if(opcode == 0x2 || opcode == 0x3) {

        if(opcode == 0x3) { // JAL
            CURRENT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
        }

        JUMP_INST(TARGET(cur_inst) << 2);

    }
    // I
    else {

        uint32_t cur_rt = CURRENT_STATE.REGS[RT(cur_inst)];
        uint32_t cur_rs = CURRENT_STATE.REGS[RS(cur_inst)];
        short cur_imm = IMM(cur_inst);

        switch(opcode) {

            case 0x4:		// BEQ
                if(cur_rs == cur_rt) {
                    CURRENT_STATE.PC += cur_imm << 2;
                }
            break;

            case 0x5:		//BNE
                if(cur_rs != cur_rt) {
                    CURRENT_STATE.PC += cur_imm << 2;
                }
            break;

            case 0x9:		// ADDIU
            CURRENT_STATE.REGS[RT(cur_inst)] = cur_rs + SIGN_EX(cur_imm);
            break;

            case 0xc:		// ANDI
            CURRENT_STATE.REGS[RT(cur_inst)] = cur_rs & cur_imm;
            break;

            case 0xf:		// LUI	
            CURRENT_STATE.REGS[RT(cur_inst)] = cur_imm << 16;
            break;

            case 0xd:		// ORI
            CURRENT_STATE.REGS[RT(cur_inst)] = cur_rs | cur_imm;
            break;

            case 0xb:		// SLTIU
            CURRENT_STATE.REGS[RT(cur_inst)] = (cur_rs < SIGN_EX(cur_imm)) ? 1 : 0;
            break;

            case 0x23:		// LW
            CURRENT_STATE.REGS[RT(cur_inst)] = mem_read_32(SIGN_EX(cur_imm) + cur_rs);
            break;

            case 0x2b:		// SW
            mem_write_32(SIGN_EX(cur_imm) + cur_rs , CURRENT_STATE.REGS[RT(cur_inst)]);
            break;

            default: break;

        }

    }

   if(CURRENT_STATE.PC >= MEM_TEXT_START + (NUM_INST << 2)) {
        RUN_BIT = FALSE;
        return;
    }

}
