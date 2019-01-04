/*
*  eleeye.h
*  CoolChix
*
*  Created by minhbkait on 8/22/08.
*  Copyright 2008 bm. All rights reserved.
*
*/
#pragma once
#include "position.h"
#include <string>
#include <cstdint>

// Hung 22/1/2018
extern "C" {
	struct NuocCo
	{
		int iSrcCol, iSrcRow, iDstCol, iDstRow;
	};
	NuocCo letComputerThinkForHint(int Board[10][9], int iDepth, long lngLimitTime);
	NuocCo letComputerThink(int Board[10][9], int iDepth, long lngLimitTime);
	bool isLegalMove(int Board[10][9], int iSrcCol, int iSrcRow, int iDstCol, int iDstRow);
	bool isGameFinished(int Board[10][9], bool *isBlackWin);
	bool isBoardValid(int board[10][9], bool bCompFirst);
	int getAllLegalMoves(int Board[10][9], int iTestCol, int iTestRow, int *iLegalCol, int *iLegalRow);
	bool quitGame();

	// QuyBKA
	bool findGeneralPiece(int Board[10][9], bool isRedTurn, int* retX, int* retY);
	bool isRedPiece(int piece);
	bool isCheck(int Board[10][9], bool isRedTurn);

	uint32_t genMoves(PositionStruct *lppos, uint32_t* lpmv);
}