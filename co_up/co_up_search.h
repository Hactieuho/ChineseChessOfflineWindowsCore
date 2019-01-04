#pragma once
#include "../eleeye/position.h"

typedef struct _MoveNode {
	_MoveNode* parent;
	int piece;
	int srcX;
	int srcY;
	int dstX;
	int dstY;
	_MoveNode* children[MAX_GEN_MOVES];
	int childrenCount;
	_MoveNode* bestMoves[MAX_GEN_MOVES];
	int bestMovesCount;
	int bestScore;
} MoveNode;

void search(int board[10][9], int iDepth, bool isRed, long lngLimitTime,
		int* pSrcX, int* pSrcY, int* pDstX, int* pDstY);

int maxi(int board[10][9], bool isRed, int depth, MoveNode* parent);

int mini(int board[10][9], bool isRed, int depth, MoveNode* parent);

int genMoves(int board[10][9], bool isRed, MoveNode* ret[]);

int makeMove(int board[10][9], MoveNode *move);

int undoMove(int board[10][9], MoveNode *move);
