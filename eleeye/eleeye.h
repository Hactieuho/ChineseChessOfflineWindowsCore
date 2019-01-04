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
#if defined(WIN32) || defined(_WIN32)
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif
extern char        g_bookPath[1024];

// Hung 22/1/2018
extern "C" {
	struct NuocCo
	{
		int iSrcCol, iSrcRow, iDstCol, iDstRow;
	};
	DLL_EXPORT NuocCo letComputerThinkForHint(int Board[10][9], int iDepth, long lngLimitTime);
	DLL_EXPORT NuocCo letComputerThink(int Board[10][9], int iDepth, long lngLimitTime);
	DLL_EXPORT bool isLegalMove(int Board[10][9], int iSrcCol, int iSrcRow, int iDstCol, int iDstRow);
	DLL_EXPORT bool isGameFinished(int Board[10][9], bool *isBlackWin);
	DLL_EXPORT bool isBoardValid(int board[10][9], bool bCompFirst);
	DLL_EXPORT int getAllLegalMoves(int Board[10][9], int iTestCol, int iTestRow, int *iLegalCol, int *iLegalRow);
	DLL_EXPORT bool quitGame();

	// QuyBKA
	DLL_EXPORT bool findGeneralPiece(int Board[10][9], bool isRedTurn, int* retX, int* retY);
	DLL_EXPORT bool isRedPiece(int piece);
	DLL_EXPORT bool isCheck(int Board[10][9], bool isRedTurn);

	DLL_EXPORT uint32_t genMoves(PositionStruct *lppos, uint32_t* lpmv);
}
