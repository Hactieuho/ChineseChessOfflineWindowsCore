#include "co_up_evaluate.h"
#include "co_up_search.h"
#include "com_mdc_core_Co_Up.h"

void search(int board[10][9], int iDepth, bool isRed, long lngLimitTime,
		int* pSrcX, int* pSrcY, int* pDstX, int* pDstY) {
	*pSrcX = -1;
	*pSrcY = -1;
	*pDstX = -1;
	*pDstY = -1;
	MoveNode* parent = new MoveNode;
	// init
	{
		parent->parent = NULL;
		parent->piece = 0;
		parent->srcX = -1;
		parent->srcY = -1;
		parent->dstX = -1;
		parent->dstY = -1;
		parent->childrenCount = 0;
		parent->bestMovesCount = 0;
		parent->bestScore = 0;
	}
	if (isRed) {
		int value = mini(board, isRed, iDepth, parent);
		*pSrcX = parent->bestMoves[0]->srcX;
		*pSrcY = parent->bestMoves[0]->srcY;
		*pDstX = parent->bestMoves[0]->dstX;
		*pDstY = parent->bestMoves[0]->dstY;
	} else {
		int value = mini(board, isRed, iDepth, parent);
		*pSrcX = parent->bestMoves[0]->srcX;
		*pSrcY = parent->bestMoves[0]->srcY;
		*pDstX = parent->bestMoves[0]->dstX;
		*pDstY = parent->bestMoves[0]->dstY;
	}

	// in move list
	char txt[8192] = "";
	for(int i=0; i<parent->childrenCount; i++){
		sprintf(txt, "%s %d", txt, parent->children[i]->bestScore);
	}
}

int maxi(int board[10][9], bool isRed, int depth, MoveNode* parent) {
	// the last
	if (depth == 0){
		parent->bestScore = evaluate(board, isRed);
		return parent->bestScore;
	}
	int max = -9999999; // am vo cung
	parent->bestScore = max;
	// generate moves
	MoveNode* moves[MAX_GEN_MOVES];
	int moveCount = genMoves(board, isRed, moves);
	// check deeper
	for (int i = 0; i < moveCount; i++) {
		MoveNode* move = moves[i];
		// Add this move to children nod of the list
		{
			move->parent = parent;
			parent->children[parent->childrenCount] = move;
			parent->childrenCount++;
		}
		// do the move to change board
		makeMove(board, move);
		int score = mini(board, !isRed, depth - 1, move);
		// add to best move list
		if (score > max) {
			max = score;
			parent->bestMoves[0] = move;
			parent->bestMovesCount = 1;
		} else if (score == max) {
			parent->bestMoves[parent->bestMovesCount] = move;
			parent->bestMovesCount++;
		}
		// undo the move to change board
		undoMove(board, move);
	}
	parent->bestScore = max;
	return max;
}

int mini(int board[10][9], bool isRed, int depth, MoveNode* parent) {
	if (depth == 0){
		parent->bestScore = evaluate(board, isRed);
		return parent->bestScore;
	}
	int min = +999999;	// duong vo cung
	parent->bestScore = min;
	// generate moves
	MoveNode* moves[MAX_GEN_MOVES];
	int moveCount = genMoves(board, isRed, moves);
	// check deeper
	for (int i = 0; i < moveCount; i++) {
		MoveNode* move = moves[i];
		// Add this move to children nod of the list
		{
			move->parent = parent;
			parent->children[parent->childrenCount] = move;
			parent->childrenCount++;
		}
		// do the move to change board
		makeMove(board, move);
		int score = maxi(board, !isRed, depth - 1, move);
		if (score < min) {
			min = score;
			parent->bestMoves[0] = move;
			parent->bestMovesCount = 1;
		} else if (score == min) {
			// add move
			parent->bestMoves[parent->bestMovesCount] = move;
			parent->bestMovesCount++;
		}
		// undo the move to change board
		undoMove(board, move);
	}
	parent->bestScore = min;
	return min;
}

int genMoves(int board[10][9], bool isRed, MoveNode* ret[]) {
	int legalSrcX[200];
	int legalSrcY[200];
	int *iLegalSrcX = &legalSrcX[0];
	int *iLegalSrcY = &legalSrcY[0];

	int legalDstX[200];
	int legalDstY[200];
	int *iLegalDstX = &legalDstX[0];
	int *iLegalDstY = &legalDstY[0];

	int count = co_up_generateAllLegalMoves(board, isRed, iLegalSrcX, iLegalSrcY,
			iLegalDstX, iLegalDstY);

	for (int i = 0; i < count; i++) {
		ret[i] = new MoveNode;
		ret[i]->srcX = legalSrcX[i];
		ret[i]->srcY = legalSrcY[i];
		ret[i]->dstX = legalDstX[i];
		ret[i]->dstY = legalDstY[i];
		ret[i]->piece = board[ret[i]->srcY][ret[i]->srcX];
		ret[i]->childrenCount = 0;
		ret[i]->bestMovesCount = 0;
	}
	return count;
}

int makeMove(int board[10][9], MoveNode *move) {
	board[move->dstY][move->dstX] = abs(board[move->srcY][move->srcX]);
	board[move->srcY][move->srcX] = 0;
	return 0;
}

int undoMove(int board[10][9], MoveNode *move) {
	board[move->srcY][move->srcX] = move->piece;
	board[move->dstY][move->dstX] = 0;
	return 0;
}
