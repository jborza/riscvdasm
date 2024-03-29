#include "decode.h"
#include "instruction.h"

uint32_t bextr(uint32_t src, uint32_t start, uint32_t len) {
	return (src >> start) & ((1 << len) - 1);
}

int32_t shamt(word value) {
	return bextr(value, 20, 6);
}

uint32_t imm_sign(word value) {
	int sign = bextr(value, 31, 1);
	return sign == 1 ? (uint32_t)-1 : 0;
}

int32_t get_b_imm(word value) {
	return (bextr(value, 8, 4) << 1) + (bextr(value, 25, 6) << 5) + (bextr(value, 7, 1) << 11) + (imm_sign(value) << 12);
}

uint32_t get_i_imm_unsigned(word value) {
	return value >> 20;
}

int32_t get_i_imm(word value) {
	int val = bextr(value, 20, 12);
	int sign = bextr(value, 31, 1) == 1 ? -1 : 1;
	//sign extend if negative
	if (sign == -1)
		val |= (0xfffff000);
	return val;
}

int32_t get_s_imm(word value) { 
	int sign = bextr(value, 31, 1) == 1 ? -1 : 1;
	int val = bextr(value, 7, 5) + (bextr(value, 25, 7) << 5);
	//sign extend if negative
	if (sign == -1)
		val |= (0xfffff000);
	return val; 
}

int32_t get_j_imm(word value) {
	int sign = bextr(value, 31, 1) == 1 ? -1 : 1;
	int val = (bextr(value, 21, 10) << 1) + (bextr(value, 20, 1) << 11) + (bextr(value, 12, 8) << 12) + (bextr(value, 31, 1) << 20);
	//sign extend if negative by 12 bits
	if (sign == -1)
		val |= (0xfff00000);
	return val;
}