#pragma once
#include <stdio.h>
#include "../base/base2.h"
#include "../base/parse.h"
#include "co_up_search.h"
#include "../eleeye/eleeye.h"
extern "C" {
	// AI core
	NuocCo co_up_letComputerThink(int Board[10][9], int iDepth, long lngLimitTime);
	NuocCo co_up_letComputerThinkForHint(int Board[10][9], int iDepth, long lngLimitTime);

	int co_up_getAllLegalMoves(int Board[10][9], int iTestCol, int iTestRow,	int *iLegalCol, int *iLegalRow);
	bool co_up_isGameFinished(int Board[10][9], bool isRedTurn, bool* isBlackWin);
	bool co_up_isLegalMove(int Board[10][9], int iSrcX, int iSrcY, int iDstX, int iDstY,
		bool isNeedCheck);
	bool co_up_isRedPiece(int piece);
	void co_up_getBoard(int Board[10][9], int srcX, int srcY, int dstX, int dstY,
		int nextBoard[10][9]);
	bool co_up_isCheck(int Board[10][9], bool isRedTurn);
	bool co_up_findGeneralPiece(int Board[10][9], bool isRedTurn, int* retX, int* retY);
	int co_up_generateAllLegalMoves(int Board[10][9], bool isRedTurn, int *iLegalSrcX, int *iLegalSrcY, int *iLegalDstX, int *iLegalDstY);
}