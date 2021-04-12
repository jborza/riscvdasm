#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "disassembler.h"
#include "encoding.h"
#include "instruction.h"
#include "decode.h"
#include "csr.h"
#include "state.h"
#include "register.h"
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

#define INS_MATCH(MASK,MATCH,HANDLER) else if ((*instruction & MASK) == MATCH) { HANDLER(state, instruction);	}

void print_debug(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}


int get_rs1(word* instruction) {
	return (*instruction >> 15) & 0x1f;
}

int get_rs2(word* instruction) {
	return (*instruction >> 20) & 0x1f;
}

void add(State* state, word* instruction) {
	PRINT_DEBUG("add %s,%s,%s\n", register_name[GET_RD(*instruction)], register_name[GET_RS1(*instruction)], register_name[GET_RS2(*instruction)]);
}

void addi(State* state, word* instruction) {
	sword imm = get_i_imm(*instruction);
	if(GET_RS1(*instruction) == 0)
		PRINT_DEBUG("li %s,%d\n", register_name[GET_RD(*instruction)], imm);	
	else
		PRINT_DEBUG("addi %s,%s,%d\n", register_name[GET_RD(*instruction)], register_name[GET_RS1(*instruction)], imm);
}

//bitwise and between rs1 and rs2
void /**/and (State* state, word* instruction) {
	PRINT_DEBUG("and %s,%s,%s\n", register_name[GET_RD(*instruction)], register_name[GET_RS1(*instruction)], register_name[GET_RS2(*instruction)]);
}

//bitwise and on rs1 and sign-extended 12-bit immediate
void andi(State* state, word* instruction) {
	sword imm = get_i_imm(*instruction);
	PRINT_DEBUG("andi %s,%s,%d\n", register_name[GET_RD(*instruction)], register_name[GET_RS1(*instruction)], imm);
}

void auipc(State* state, word* instruction) {
	InstructionU* in = instruction;
	PRINT_DEBUG("auipc %s,0x%08x\n", register_name[GET_RD(*instruction)], in->data << 12);
}

void beq(State* state, word* instruction) {
	PRINT_DEBUG("beq %s,%s,0x%x\n", register_name[GET_RS1(*instruction)], register_name[GET_RS2(*instruction)], get_b_imm(*instruction));
}

//branch on greater or equal
void bge(State* state, word* instruction) {
	if(GET_RS2(*instruction)==0)
		PRINT_DEBUG("bgez %s,0x%x\n", register_name[GET_RS1(*instruction)], get_b_imm(*instruction));
	else
		PRINT_DEBUG("bge %s,%s,0x%x\n", register_name[GET_RS1(*instruction)], register_name[GET_RS2(*instruction)], get_b_imm(*instruction));
}

//branch on greater or equal unsigned
void bgeu(State* state, word* instruction) {
	PRINT_DEBUG("bgeu %s,%s,0x%x\n", register_name[GET_RS1(*instruction)], register_name[GET_RS2(*instruction)], get_b_imm(*instruction));
}

//branch on less than
void blt(State* state, word* instruction) {
	PRINT_DEBUG("blt %s,%s,0x%x\n", register_name[GET_RS1(*instruction)], register_name[GET_RS2(*instruction)], get_b_imm(*instruction));
}

//branch on less than unsigned
void bltu(State* state, word* instruction) {
	PRINT_DEBUG("bltu %s,%s,0x%x\n", register_name[GET_RS1(*instruction)], register_name[GET_RS2(*instruction)], get_b_imm(*instruction));
}

void bne(State* state, word* instruction) {
	//branch if src1 and src2 not equal
	PRINT_DEBUG("bne %s,%s,0x%x\n", register_name[GET_RS1(*instruction)], register_name[GET_RS2(*instruction)], get_b_imm(*instruction));
}

void fence(State* state, word* instruction) {
	PRINT_DEBUG("fence\n");
}
void fencei(State* state, word* instruction) {
	PRINT_DEBUG("fence.i\n");
}

//jump and link, J-immediate, 
void jal(State* state, word* instruction) {
	//stores the address of the instruction following the jump (pc+4) into rd
	//J-immediate encoding

	//signed offset in multiples of 2 bytes
	sword offset = get_j_imm(*instruction);
	int rd = GET_RD(*instruction);
	if(rd == 0)
		PRINT_DEBUG("j 0x%X\n", offset);
	else
		PRINT_DEBUG("jal %s, 0x%X\n", register_name[rd], offset);
}

//jump and link register
void jalr(State* state, word* instruction) {
	sword offset = get_i_imm(*instruction);
	PRINT_DEBUG("jalr %s,%s,0x%08x\n", register_name[GET_RD(*instruction)], register_name[GET_RS1(*instruction)], offset);
}

//load 8-bit sign-extended value from memory into rd
void lb(State* state, word* instruction) {
	sword offset = get_i_imm(*instruction);
	PRINT_DEBUG("lb %s,%d(%s)\n", register_name[GET_RD(*instruction)], offset, register_name[GET_RS1(*instruction)]);
}

//load 8-bit zero-extended value from memory into rd
void lbu(State* state, word* instruction) {
	sword offset = get_i_imm(*instruction);
	PRINT_DEBUG("lbu %s,%d(%s)\n", register_name[GET_RD(*instruction)], offset, register_name[GET_RS1(*instruction)]);
}

//load 16-bit sign-extended value from memory into rd
void lh(State* state, word* instruction) {
	sword offset = get_i_imm(*instruction);
	PRINT_DEBUG("lh %s,%d(%s)\n", register_name[GET_RD(*instruction)], offset, register_name[GET_RS1(*instruction)]);
}

//load 16-bit zero-extended value from memory into rd
void lhu(State* state, word* instruction) {
	sword offset = get_i_imm(*instruction);
	PRINT_DEBUG("lhu %s,%d(%s)\n", register_name[GET_RD(*instruction)], offset, register_name[GET_RS1(*instruction)]);
}

void lui(State* state, word* instruction) {
	InstructionU* in = instruction;
	PRINT_DEBUG("lui %s,0x%08x\n", register_name[GET_RD(*instruction)], in->data << 12);
}

//load 32-bit value from memory into rd
void lw(State* state, word* instruction) {
	sword offset = get_i_imm(*instruction);
	PRINT_DEBUG("lw %s,%d(%s)\n", register_name[GET_RD(*instruction)], offset, register_name[GET_RS1(*instruction)]);
}

void or (State * state, word * instruction) {
	PRINT_DEBUG("or %s,%s,%s\n", register_name[GET_RD(*instruction)], register_name[GET_RS1(*instruction)], register_name[GET_RS2(*instruction)]);
}

//bitwise or on rs1 and sign-extended 12-bit immediate
void ori(State* state, word* instruction) {
	InstructionI* in = instruction;
	PRINT_DEBUG("ori %s,%s,%d\n", register_name[GET_RD(*instruction)], register_name[GET_RS1(*instruction)], in->imm);
}

//store 8-bit value from the low bits of rs2 to the memory
void sb(State* state, word* instruction) {
	sword offset = get_s_imm(*instruction);
	PRINT_DEBUG("sb %s,%d(%s)\n", register_name[GET_RS2(*instruction)], offset, register_name[GET_RS1(*instruction)]);
}

//store 16-bit value from the low bits of rs2 to the memory
void sh(State* state, word* instruction) {
	sword offset = get_s_imm(*instruction);
	PRINT_DEBUG("sh %s,%d(%s)\n", register_name[GET_RS2(*instruction)], offset, register_name[GET_RS1(*instruction)]);
}

void sll(State* state, word* instruction) {
	PRINT_DEBUG("sll %s,%s,%s\n", register_name[GET_RD(*instruction)], register_name[GET_RS1(*instruction)], register_name[GET_RS2(*instruction)]);
}

void slli(State* state, word* instruction) {
	InstructionIShift* in = instruction;
	PRINT_DEBUG("slli %s,0x%x\n", register_name[GET_RD(*instruction)], register_name[GET_RS1(*instruction)], in->shamt & 0x1F);
}

void slt(State* state, word* instruction) {
	//signed comparison, if rs1 < rs2 then rd=1 else rd=0
	PRINT_DEBUG("slt %s,%s,%s\n", register_name[GET_RD(*instruction)], register_name[GET_RS1(*instruction)], register_name[GET_RS2(*instruction)]);
}

void sltu(State* state, word* instruction) {
	//unsigned comparison, if rs1 < rs2 then rd=1 else rd=0
	PRINT_DEBUG("sltu %s,%s,%s\n", register_name[GET_RD(*instruction)], register_name[GET_RS1(*instruction)], register_name[GET_RS2(*instruction)]);
}

//set less than immediate
void slti(State* state, word* instruction) {
	//places the value 1 in register rd if register rs1 is less than the sign - extended immediate when both are treated as signed numbers, else 0 is written to rd
	InstructionI* in = instruction;
	PRINT_DEBUG("slti %s,%s,0x%08x\n", register_name[GET_RD(*instruction)], register_name[GET_RS1(*instruction)], in->imm);
}

//set less than immediate unsigned
void sltiu(State* state, word* instruction) {
	//places the value 1 in register rd if register rs1 is less than the sign - extended immediate when both are treated as signed numbers, else 0 is written to rd
	InstructionI* in = instruction;
	PRINT_DEBUG("sltiu %s,%s,0x%08x\n", register_name[GET_RD(*instruction)], register_name[GET_RS1(*instruction)], in->imm);
}

//shift right arithmetic
void sra(State* state, word* instruction) {
	PRINT_DEBUG("sra %s,%s,%s\n", register_name[GET_RD(*instruction)], register_name[GET_RS1(*instruction)], register_name[GET_RS2(*instruction)]);
}

void srai(State* state, word* instruction) {
	InstructionIShift* in = instruction;
	PRINT_DEBUG("srai %s,%s,0x%x\n", register_name[GET_RD(*instruction)], register_name[GET_RS1(*instruction)], in->shamt & 0x1F);
}

//shift right logical
void srl(State* state, word* instruction) {
	PRINT_DEBUG("srl %s,%s,%s\n", register_name[GET_RD(*instruction)], register_name[GET_RS1(*instruction)], register_name[GET_RS2(*instruction)]);
}

void srli(State* state, word* instruction) {
	InstructionIShift* in = instruction;
	PRINT_DEBUG("srli %s,%s,0x%x\n", register_name[GET_RD(*instruction)], register_name[GET_RS1(*instruction)], in->shamt & 0x1F);
}

void sub(State* state, word* instruction) {
	PRINT_DEBUG("sub %s,%s,%s\n", register_name[GET_RD(*instruction)], register_name[GET_RS1(*instruction)], register_name[GET_RS2(*instruction)]);
}

//store 32-bit value from rs to memory
void sw(State* state, word* instruction) {
	sword offset = get_s_imm(*instruction);
	PRINT_DEBUG("sw %s,%d(%s)\n", register_name[GET_RS2(*instruction)], offset, register_name[GET_RS1(*instruction)]);
}

void /**/xor (State* state, word* instruction) {
	PRINT_DEBUG("xor %s,%s,%s\n", register_name[GET_RD(*instruction)], register_name[GET_RS1(*instruction)], register_name[GET_RS2(*instruction)]);
}

//bitwise xor on rs1 and sign-extended 12-bit immediate
void xori(State* state, word* instruction) {
	InstructionI* in = instruction;
	PRINT_DEBUG("xori %s,%s,%d\n", register_name[GET_RD(*instruction)], register_name[GET_RS1(*instruction)], in->imm);
}

void mret(State* state, word* instruction) {
	PRINT_DEBUG("mret\n");
}

void sret(State* state, word* instruction) {
	PRINT_DEBUG("sret\n");
}

void uret(State* state, word* instruction) {
	PRINT_DEBUG("uret\n");
}

void sfence_vma(State* state, word* instruction) {
	PRINT_DEBUG("sfence.vma\r\n");
}

void wfi(State* state, word* instruction) {
	//wait for interrupt
	PRINT_DEBUG("wfi\n");
}

/// M Extension


#define M_OP(NAME, OP) { \
	PRINT_DEBUG(NAME" %s,%s,%s\n", register_name[GET_RD(*instruction)], register_name[GET_RS1(*instruction)], register_name[GET_RS2(*instruction)]); \
}

void mul(State* state, word* instruction) {
	M_OP("mul", mul32);
}

void mulh(State* state, word* instruction) {
	M_OP("mulh", mulh32);
}
void mulhu(State* state, word* instruction) {
	M_OP("mulhu", mulhu32);
}

void mulhsu(State* state, word* instruction) {
	M_OP("mulhsu", mulhsu32);
}
void rem(State* state, word* instruction) {
	M_OP("rem", rem32);
}
void remu(State* state, word* instruction) {
	M_OP("remu", remu32);
}
void op_div(State* state, word* instruction) {
	M_OP("div", div32);
}
void divu(State* state, word* instruction) {
	M_OP("divu", divu32);
}

// ZICSR

//atomic read & set bits in CSR
void csrrs(State* state, word* instruction) {
	word csr = get_i_imm_unsigned(*instruction);
	PRINT_DEBUG("csrrs %s,%s,%s\n", register_name[GET_RD(*instruction)], csr_name(csr), register_name[GET_RS1(*instruction)]);
}

//atomic read/write CSR
void csrrw(State* state, word* instruction) {
	word csr = get_i_imm_unsigned(*instruction);
	PRINT_DEBUG("csrrw %s,%s,%s\n", register_name[GET_RD(*instruction)], csr_name(csr), register_name[GET_RS1(*instruction)]);
}

//atomic read & clear bits in CSR
void csrrc(State* state, word* instruction) {
	word csr = get_i_imm_unsigned(*instruction);
	PRINT_DEBUG("csrrc %s,%s,%s\n", register_name[GET_RD(*instruction)], csr_name(csr), register_name[GET_RS1(*instruction)]);
}

//instead of rs1 use a zero-extended 5-bit unsigned immediate
void csrrwi(State* state, word* instruction) {
	word csr = get_i_imm_unsigned(*instruction);
	PRINT_DEBUG("csrrwi %s,%s,%d\n", register_name[GET_RD(*instruction)], csr_name(csr), GET_CSR_IMM(*instruction));
}

void csrrsi(State* state, word* instruction) {
	word csr = get_i_imm_unsigned(*instruction);
	PRINT_DEBUG("csrrsi %s,%s,%d\n", register_name[GET_RD(*instruction)], csr_name(csr), GET_CSR_IMM(*instruction));
}

void csrrci(State* state, word* instruction) {
	word csr = get_i_imm_unsigned(*instruction);
	PRINT_DEBUG("csrrci %s,%s,%d\n", register_name[GET_RD(*instruction)], csr_name(csr), GET_CSR_IMM(*instruction));
}

void ebreak(State* state, word* instruction) {
	PRINT_DEBUG("ebreak\n");
}

void ecall(State* state, word* instruction) {
	PRINT_DEBUG("ecall\n");
}

// A extension

#define AMO_OP_W(NAME, OP) { \
PRINT_DEBUG(NAME" %s,%s,(%s)\n", register_name[GET_RD(*instruction)], register_name[GET_RS2(*instruction)], register_name[GET_RS1(*instruction)]); \
}

void amoadd_w(State* state, word* instruction) {
	AMO_OP_W("amoadd.w", +)
}

void amoand_w(State* state, word* instruction) {
	AMO_OP_W("amoand.w", &)
}

void amoor_w(State* state, word* instruction) {
	AMO_OP_W("amoor.w", | )
}

void amoxor_w(State* state, word* instruction) {
	AMO_OP_W("amoxor.w", ^);
}

void amoswap_w(State* state, word* instruction) {
	AMO_OP_W("amoswap.w", +);
}

void lr(State* state, word* instruction) {
	PRINT_DEBUG("lr %s, (%s)\n", register_name[GET_RD(*instruction)], register_name[GET_RS1(*instruction)]);
}

void sc(State* state, word* instruction)
{
	// SC.W writes zero to rd on success or a nonzero code on failure.
	//see https://riscv.org/wp-content/uploads/2019/06/riscv-spec.pdf
	PRINT_DEBUG("sc %s,%s,(%s)\n", register_name[GET_RD(*instruction)], register_name[GET_RS2(*instruction)], register_name[GET_RS1(*instruction)]);
}


void disassemble_op(word *instruction){
	State _state;
	State *state = &_state;
//dummy condition as opcode handlers are 'else if'
	if (0 == 1) {
		;
	}
		INS_MATCH(MASK_ADD, MATCH_ADD, add)
		INS_MATCH(MASK_ADDI, MATCH_ADDI, addi)
		INS_MATCH(MASK_AND, MATCH_AND, and)
		INS_MATCH(MASK_ANDI, MATCH_ANDI, andi)
		INS_MATCH(MASK_AUIPC, MATCH_AUIPC, auipc)
		INS_MATCH(MASK_BEQ, MATCH_BEQ, beq)
		INS_MATCH(MASK_BGE, MATCH_BGE, bge)
		INS_MATCH(MASK_BGEU, MATCH_BGEU, bgeu)
		INS_MATCH(MASK_BLT, MATCH_BLT, blt)
		INS_MATCH(MASK_BLTU, MATCH_BLTU, bltu)
		INS_MATCH(MASK_BNE, MATCH_BNE, bne)
		INS_MATCH(MASK_EBREAK, MATCH_EBREAK, ebreak)
		INS_MATCH(MASK_ECALL, MATCH_ECALL, ecall)
		INS_MATCH(MASK_FENCE, MATCH_FENCE, fence)
		INS_MATCH(MASK_FENCE_I, MATCH_FENCE_I, fencei)
		INS_MATCH(MASK_JAL, MATCH_JAL, jal)
		INS_MATCH(MASK_JALR, MATCH_JALR, jalr)
		INS_MATCH(MASK_LB, MATCH_LB, lb)
		INS_MATCH(MASK_LBU, MATCH_LBU, lbu)
		INS_MATCH(MASK_LH, MATCH_LH, lh)
		INS_MATCH(MASK_LHU, MATCH_LHU, lhu)
		INS_MATCH(MASK_LUI, MATCH_LUI, lui)
		INS_MATCH(MASK_LW, MATCH_LW, lw)
		INS_MATCH(MASK_OR, MATCH_OR, or )
		INS_MATCH(MASK_ORI, MATCH_ORI, ori)
		INS_MATCH(MASK_SB, MATCH_SB, sb)
		INS_MATCH(MASK_SH, MATCH_SH, sh)
		INS_MATCH(MASK_SLL, MATCH_SLL, sll)
		INS_MATCH(MASK_SLLI, MATCH_SLLI, slli)
		INS_MATCH(MASK_SLT, MATCH_SLT, slt)
		INS_MATCH(MASK_SLTI, MATCH_SLTI, slti)
		INS_MATCH(MASK_SLTIU, MATCH_SLTIU, sltiu)
		INS_MATCH(MASK_SLTU, MATCH_SLTU, sltu)
		INS_MATCH(MASK_SRA, MATCH_SRA, sra)
		INS_MATCH(MASK_SRAI, MATCH_SRAI, srai)
		INS_MATCH(MASK_SRL, MATCH_SRL, srl)
		INS_MATCH(MASK_SRLI, MATCH_SRLI, srli)
		INS_MATCH(MASK_SUB, MATCH_SUB, sub)
		INS_MATCH(MASK_SW, MATCH_SW, sw)
		INS_MATCH(MASK_XOR, MATCH_XOR, xor)
		INS_MATCH(MASK_XORI, MATCH_XORI, xori)

		INS_MATCH(MASK_WFI, MATCH_WFI, wfi)
		INS_MATCH(MASK_MRET, MATCH_MRET, mret)
		INS_MATCH(MASK_SRET, MATCH_SRET, sret)
		INS_MATCH(MASK_URET, MATCH_URET, uret)

#ifdef EXTENSION_ZICSR
		INS_MATCH(MASK_CSRRS, MATCH_CSRRS, csrrs)
		INS_MATCH(MASK_CSRRW, MATCH_CSRRW, csrrw)
		INS_MATCH(MASK_CSRRC, MATCH_CSRRC, csrrc)
		INS_MATCH(MASK_CSRRSI, MATCH_CSRRSI, csrrsi)
		INS_MATCH(MASK_CSRRWI, MATCH_CSRRWI, csrrwi)
		INS_MATCH(MASK_CSRRCI, MATCH_CSRRCI, csrrci)
#endif

#ifdef EXTENSION_A
		INS_MATCH(MASK_AMOADD_W, MATCH_AMOADD_W, amoadd_w)
		INS_MATCH(MASK_AMOOR_W, MATCH_AMOOR_W, amoor_w)
		INS_MATCH(MASK_AMOAND_W, MATCH_AMOAND_W, amoand_w)
		INS_MATCH(MASK_AMOXOR_W, MATCH_AMOXOR_W, amoxor_w)
		INS_MATCH(MASK_AMOSWAP_W, MATCH_AMOSWAP_W, amoswap_w)
		INS_MATCH(MASK_LR_W, MATCH_LR_W, lr)
		INS_MATCH(MASK_SC_W, MATCH_SC_W, sc)
#endif
		INS_MATCH(MASK_SFENCE_VMA, MATCH_SFENCE_VMA, sfence_vma)
#ifdef EXTENSION_M
		INS_MATCH(MASK_MUL, MATCH_MUL, mul)
		INS_MATCH(MASK_MULH, MATCH_MULH, mulh)
		INS_MATCH(MASK_MULHSU, MATCH_MULHSU, mulhsu)
		INS_MATCH(MASK_MULHU, MATCH_MULHU, mulhu)
		INS_MATCH(MASK_DIV, MATCH_DIV, op_div)
		INS_MATCH(MASK_DIVU, MATCH_DIVU, divu)
		INS_MATCH(MASK_REM, MATCH_REM, rem)
		INS_MATCH(MASK_REMU, MATCH_REMU, remu)
#endif
else {
		printf("Unknown instruction: %8X \n", *instruction);
}
}