#pragma once

#include "base.h"

inline unsigned int Bsf(int32_t Operand) {
	uint32_t dw = Operand & -Operand;
	return
		((dw & 0xFFFF0000) != 0 ? 16 : 0) +
		((dw & 0xFF00FF00) != 0 ? 8 : 0) +
		((dw & 0xF0F0F0F0) != 0 ? 4 : 0) +
		((dw & 0xCCCCCCCC) != 0 ? 2 : 0) +
		((dw & 0xAAAAAAAA) != 0 ? 1 : 0);
}

inline int Bsr(int32_t Operand) {
	uint32_t dw = Operand;
	int n = 0;
	if ((dw & 0xFFFF0000) != 0) {
		dw &= 0xFFFF0000;
		n += 16;
	}
	if ((dw & 0xFF00FF00) != 0) {
		dw &= 0xFF00FF00;
		n += 8;
	}
	if ((dw & 0xF0F0F0F0) != 0) {
		dw &= 0xF0F0F0F0;
		n += 4;
	}
	if ((dw & 0xCCCCCCCC) != 0) {
		dw &= 0xCCCCCCCC;
		n += 2;
	}
	if ((dw & 0xAAAAAAAA) != 0) {
		dw &= 0xAAAAAAAA;
		n++;
	}
	return n;
}