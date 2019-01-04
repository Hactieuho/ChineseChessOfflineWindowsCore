#include "co_up_evaluate.h"
#include <stdio.h>

const int EMPTY = 0;

const int R_VUA = 1;
const int R_SI = 2;
const int R_TUONG = 3;
const int R_XE = 4;
const int R_PHAO = 5;
const int R_MA = 6;
const int R_TOT = 7;

const int B_VUA = 8;
const int B_SI = 9;
const int B_TUONG = 10;
const int B_XE = 11;
const int B_PHAO = 12;
const int B_MA = 13;
const int B_TOT = 14;

const int piece_value[] = { 0, 10, 10, 100, 50, 40, 20, 0, 10, 10, 100, 50, 40,
		20 };

int evaluate(int board[10][9], bool isRed) {
	int ret = calculateMaterialAdvantage(board, isRed);
	return ret;

	//return 0;
}

int calculateMaterialAdvantage(int board[10][9], bool isRed) {
	/*for (int x = 0; x < 9; x++)
	 for (int y = 0; y < 10; y++) {
	 LOGE("Origin Board, calculate material advantage: x:%d,y:%d : %d", x, y, board[y][x]);
	 }*/

	int ret = 0;
	for (int x = 0; x < 9; x++)
		for (int y = 0; y < 10; y++) {
			int piece = board[y][x] > 0 ? board[y][x] : -board[y][x];
			if (piece >= 1 && piece <= 14) {
				if (isRed == isRedColor(piece)) {
					// cung mau
					ret += piece_value[piece - 1];
				} else {
					// khac mau
					ret -= piece_value[piece - 1];
				}
				//LOGE("Material Advantage +: %d, %d!", board[y][x], piece_value[piece - 1]);
			}
		}
	//LOGE("Material Advantage: %d!", ret);
	return ret;
}

bool isRedColor(int piece) {
	int type = piece > 0 ? piece : -piece;
	if (type > 0 && type < 8)
		return true;
	else
		return false;
}

int calculateMobilityAdvantage(int board[10][9]) {

	return 0;
}

int calculateThreat(int board[10][9]) {
	return 0;
}

int calculateProtected(int board[10][9]) {
	return 0;
}
