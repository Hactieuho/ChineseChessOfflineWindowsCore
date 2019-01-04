/*
eleeye.cpp - Source Code for ElephantEye, Part IX

ElephantEye - a Chinese Chess Program (UCCI Engine)
Designed by Morning Yellow, Version: 3.3, Last Modified: Mar. 2012
Copyright (C) 2004-2012 www.xqbase.com

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <stdio.h>
#include "../base/base2.h"
#include "../base/parse.h"
#include "ucci.h"
#include "pregen.h"
#include "position.h"
#include "hash.h"
#include "search.h"

#include "eleeye.h"
//#include "common_dstring.h"

#ifdef _WIN32
#include <windows.h>
const char *const cszLibEvalFile = "EVALUATE.DLL";
const char *WINAPI GetEngineNameEVA(void);
int WINAPI EvaluateEVA(const PositionStruct *lppos, int vlAlpha, int vlBeta);
void WINAPI PreEvaluateEVA(PositionStruct *lppos, PreEvalStruct *lpPreEval);
#else
#include <dlfcn.h>
#define WINAPI
const char *const cszLibEvalFile = "libeval.so";
const char *WINAPI GetEngineNameEVA(void);
int WINAPI EvaluateEVA(const PositionStruct *lppos, int vlAlpha, int vlBeta);
void WINAPI PreEvaluateEVA(PositionStruct *lppos, PreEvalStruct *lpPreEval);

#endif


const int INTERRUPT_COUNT = 4096; // 搜索若干结点后调用中断

static const char *WINAPI GetEngineName(void) {
	return NULL;
}

static void WINAPI PreEvaluate(PositionStruct *lppos, PreEvalStruct *lpPreEval) {
	// 缺省的局面预评价过程，什么都不做
}

static int WINAPI Evaluate(const PositionStruct *lppos, int vlAlpha, int vlBeta) {
	// 缺省的局面评价过程，只返回子力价值
	return lppos->Material();
}

#ifdef _WIN32

inline HMODULE LoadEvalApi(const char *szLibEvalFile) {
	HMODULE hModule;
	hModule = LoadLibrary(szLibEvalFile);
	if (hModule == NULL) {
		Search.GetEngineName = GetEngineNameEVA;
		Search.PreEvaluate = PreEvaluateEVA;
		Search.Evaluate = EvaluateEVA;
	}
	else {
		Search.GetEngineName = (const char *(WINAPI *)(void)) GetProcAddress(hModule, "_GetEngineName@0");
		Search.PreEvaluate = (void (WINAPI *)(PositionStruct *, PreEvalStruct *)) GetProcAddress(hModule, "_PreEvaluate@8");
		Search.Evaluate = (int (WINAPI *)(const PositionStruct *, int, int)) GetProcAddress(hModule, "_Evaluate@12");
	}
	return hModule;
}

inline void FreeEvalApi(HMODULE hModule) {
	if (hModule != NULL) {
		FreeLibrary(hModule);
	}
}

#else

inline void *LoadEvalApi(const char *szLibEvalFile) {
	Search.GetEngineName = GetEngineNameEVA;
	Search.PreEvaluate = PreEvaluateEVA;
	Search.Evaluate = EvaluateEVA;
	return 0;
}

inline void FreeEvalApi(void *hModule) {
	if (hModule != NULL) {
		dlclose(hModule);
	}
}

#endif

extern uint32_t g_dwMoveStr;
extern uint32_t g_dwSecondStr;
extern uint32_t g_dwThirdStr;

extern "C" {
	NuocCo letComputerThink(int Board[10][9], int iDepth, long lngLimitTime)
	{
		NuocCo nuocCo;
		int i, j;
		int pcWhite[7];
		int pcBlack[7];
		char fen[1024];
		pcWhite[0] = SIDE_TAG(0) + KING_FROM;
		pcWhite[1] = SIDE_TAG(0) + ADVISOR_FROM;
		pcWhite[2] = SIDE_TAG(0) + BISHOP_FROM;
		pcWhite[3] = SIDE_TAG(0) + ROOK_FROM;
		pcWhite[4] = SIDE_TAG(0) + CANNON_FROM;
		pcWhite[5] = SIDE_TAG(0) + KNIGHT_FROM;
		pcWhite[6] = SIDE_TAG(0) + PAWN_FROM;

		for (i = 0; i < 7; i++)
		{
			pcBlack[i] = pcWhite[i] + 16;
		};
		LoadEvalApi("EVALUATE");
		PreGenInit();
		NewHash(24); // 24=16MB, 25=32MB, 26=64MB, ...
		Search.pos.ClearBoard();

		for (i = 0; i < 10; i++)
		{
			for (j = 0; j < 9; j++)
			{
				if ((Board[i][j] > 0) && (Board[i][j] < 8)) //Computer's pieces
					Search.pos.AddPiece(COORD_XY(j + FILE_LEFT, 9 - i + RANK_TOP), pcWhite[Board[i][j] - 1]);
				else if (Board[i][j] > 0) //Human's pieces
					Search.pos.AddPiece(COORD_XY(j + FILE_LEFT, 9 - i + RANK_TOP), pcBlack[Board[i][j] - 8]);
			};
		};
		memset(fen, 0, 1024);
		Search.pos.ToFen(fen);
		Search.pos.FromFen(fen);
		Search.pos.nDistance = 0;
		Search.PreEvaluate(&Search.pos, &PreEval);
		Search.nBanMoves = 0;
		Search.bIdle = Search.bPonder = Search.bQuit = Search.bBatch = Search.bDebug = Search.bAlwaysCheck = false;
		Search.bUseHash = Search.bUseBook = Search.bNullMove = Search.bKnowledge = true;
		Search.nCountMask = INTERRUPT_COUNT - 1;
		Search.nRandomMask = 0;
		Search.rc4Random.InitRand();
		if (lngLimitTime == 0)
			Search.nGoMode = GO_MODE_INFINITY;
		else
		{
			Search.nGoMode = GO_MODE_TIMER;
			Search.nMaxTimer = lngLimitTime;
			Search.nProperTimer = lngLimitTime;
		};
		if (iDepth == -1)
			SearchMain(1);
		else
			SearchMain(iDepth);
		if (g_dwMoveStr == 0)
		{
			nuocCo.iSrcCol = -1;
			nuocCo.iSrcRow = -1;
			nuocCo.iDstCol = -1;
			nuocCo.iDstRow = -1;
		}
		else
		{
			if (iDepth == -1)
			{
				char* pMoveStr;
				if (g_dwThirdStr)
				{
					pMoveStr = (char*)&g_dwSecondStr;
					printf("thirt move.\n");
				}
				else
				{
					pMoveStr = (char*)&g_dwMoveStr;
					printf("normal move.\n");
				}
				nuocCo.iSrcCol = pMoveStr[0] - 'a';
				nuocCo.iSrcRow = pMoveStr[1] - 48;
				nuocCo.iDstCol = pMoveStr[2] - 'a';
				nuocCo.iDstRow = pMoveStr[3] - 48;
				if (!g_dwSecondStr)
					printf("No second move\r\n");
			}
			else
			{
				char* pMoveStr = (char*)(&g_dwMoveStr);
				nuocCo.iSrcCol = pMoveStr[0] - 'a';
				nuocCo.iSrcRow = pMoveStr[1] - 48;
				nuocCo.iDstCol = pMoveStr[2] - 'a';
				nuocCo.iDstRow = pMoveStr[3] - 48;
				if (g_dwSecondStr)
				{
					pMoveStr = (char*)(&g_dwSecondStr);
					//            printf("Second move %.4s\r\n",(const char*)&g_dwSecondStr);
				}
				else
					printf("No second move\r\n");
			}
		};
		DelHash();
		return nuocCo;
	};

	bool quitGame()
	{
		return quit();
	}

	NuocCo letComputerThinkForHint(int Board[10][9], int iDepth, long lngLimitTime)
	{
		NuocCo nuocCo;
		int i, j;
		int pcWhite[7];
		int pcBlack[7];
		char fen[1024];
		pcWhite[0] = SIDE_TAG(0) + KING_FROM;
		pcWhite[1] = SIDE_TAG(0) + ADVISOR_FROM;
		pcWhite[2] = SIDE_TAG(0) + BISHOP_FROM;
		pcWhite[3] = SIDE_TAG(0) + ROOK_FROM;
		pcWhite[4] = SIDE_TAG(0) + CANNON_FROM;
		pcWhite[5] = SIDE_TAG(0) + KNIGHT_FROM;
		pcWhite[6] = SIDE_TAG(0) + PAWN_FROM;

		for (i = 0; i < 7; i++)
		{
			pcBlack[i] = pcWhite[i] + 16;
		};
		LoadEvalApi("EVALUATE");
		PreGenInit();
		NewHash(24); // 24=16MB, 25=32MB, 26=64MB, ...
		Search.pos.ClearBoard();

		for (i = 0; i < 10; i++)
		{
			for (j = 0; j < 9; j++)
			{
				if ((Board[i][j] > 0) && (Board[i][j] < 8)) //Computer's pieces
					Search.pos.AddPiece(COORD_XY(j + FILE_LEFT, 9 - i + RANK_TOP), pcWhite[Board[i][j] - 1]);
				else if (Board[i][j] > 0) //Human's pieces
					Search.pos.AddPiece(COORD_XY(j + FILE_LEFT, 9 - i + RANK_TOP), pcBlack[Board[i][j] - 8]);
			};
		};
		memset(fen, 0, 1024);
		Search.pos.ToFen(fen);
		Search.pos.FromFen(fen);
		Search.pos.nDistance = 0;
		Search.PreEvaluate(&Search.pos, &PreEval);
		Search.nBanMoves = 0;
		Search.bIdle = Search.bPonder = Search.bQuit = Search.bBatch = Search.bDebug = Search.bAlwaysCheck = false;
		Search.bUseHash = Search.bUseBook = Search.bNullMove = Search.bKnowledge = true;
		Search.nCountMask = INTERRUPT_COUNT - 1;
		Search.nRandomMask = 0;
		Search.rc4Random.InitRand();
		if (lngLimitTime == 0)
			Search.nGoMode = GO_MODE_INFINITY;
		else
		{
			Search.nGoMode = GO_MODE_TIMER;
			Search.nMaxTimer = lngLimitTime;
			Search.nProperTimer = lngLimitTime;
		};
		Search.pos.ChangeSide();
		SearchMain(iDepth);
		if (g_dwMoveStr == 0)
		{
			nuocCo.iSrcCol = -1;
			nuocCo.iSrcRow = -1;
			nuocCo.iDstCol = -1;
			nuocCo.iDstRow = -1;
		}
		else
		{
			char* pMoveStr = (char*)(&g_dwMoveStr);
			nuocCo.iSrcCol = pMoveStr[0] - 'a';
			nuocCo.iSrcRow = pMoveStr[1] - 48;
			nuocCo.iDstCol = pMoveStr[2] - 'a';
			nuocCo.iDstRow = pMoveStr[3] - 48;
			if (g_dwSecondStr)
			{
				pMoveStr = (char*)(&g_dwSecondStr);
				//            printf("Second move %.4s\r\n",(const char*)&g_dwSecondStr);
			}
			else
				printf("No second move\r\n");
		};

		DelHash();
		return nuocCo;
	};

	bool isLegalMove(int Board[10][9], int iSrcCol, int iSrcRow, int iDstCol, int iDstRow)
	{
		int i, j;
		int pcWhite[7];
		int pcBlack[7];
		char fen[1024];
		union
		{
			char strMove[5];
			uint32_t dwMove;
		} move;

		pcWhite[0] = SIDE_TAG(0) + KING_FROM;
		pcWhite[1] = SIDE_TAG(0) + ADVISOR_FROM;
		pcWhite[2] = SIDE_TAG(0) + BISHOP_FROM;
		pcWhite[3] = SIDE_TAG(0) + ROOK_FROM;
		pcWhite[4] = SIDE_TAG(0) + CANNON_FROM;
		pcWhite[5] = SIDE_TAG(0) + KNIGHT_FROM;
		pcWhite[6] = SIDE_TAG(0) + PAWN_FROM;

		for (i = 0; i < 7; i++)
		{
			pcBlack[i] = pcWhite[i] + 16;
		};
		LoadEvalApi("EVALUATE");
		PreGenInit();
		Search.pos.ClearBoard();
		for (i = 0; i < 10; i++)
		{
			for (j = 0; j < 9; j++)
			{
				if ((Board[i][j] > 0) && (Board[i][j] < 8)) //Computer's pieces
					Search.pos.AddPiece(COORD_XY(j + FILE_LEFT, 9 - i + RANK_TOP), pcWhite[Board[i][j] - 1]);
				else if (Board[i][j] > 0) //Human's pieces
					Search.pos.AddPiece(COORD_XY(j + FILE_LEFT, 9 - i + RANK_TOP), pcBlack[Board[i][j] - 8]);
			};
		};

		memset(fen, 0, 1024);
		Search.pos.ToFen(fen);
		Search.pos.FromFen(fen);
		Search.pos.nDistance = 0;
		Search.PreEvaluate(&Search.pos, &PreEval);
		Search.nBanMoves = 0;
		Search.bIdle = Search.bPonder = Search.bQuit = Search.bBatch = Search.bDebug = Search.bAlwaysCheck = false;
		Search.bUseHash = false;
		Search.bUseBook = Search.bNullMove = Search.bKnowledge = true;
		Search.nCountMask = INTERRUPT_COUNT - 1;
		Search.nRandomMask = 0;
		Search.rc4Random.InitRand();
		memset(fen, 0, 1024);
		memset(move.strMove, 0, 5);

		move.strMove[0] = iSrcCol + 'a';
		move.strMove[1] = iSrcRow + '0';
		move.strMove[2] = iDstCol + 'a';
		move.strMove[3] = iDstRow + '0';

		if (Board[iSrcRow][iSrcCol] > 7)
		{
			Search.pos.ChangeSide();
		}

		bool ret = Search.pos.LegalMove(COORD_MOVE(move.dwMove));
		if (ret)
		{
			Search.pos.MovePiece(COORD_MOVE(move.dwMove));
			ret = ret && (!Search.pos.CheckedBy());
		};
		return ret;
	};

	//////////////////////////////////////////////////////////////////////////
	//Check whether the game is finished,                                    //
	//In this case the function returns TRUE and                            //
	//                1. bCompWon = TRUE if the computer has won the game        //
	//                2. bCompWon = FALSE if the player has won the game        //
	//////////////////////////////////////////////////////////////////////////

	bool isGameFinished(int Board[10][9], bool* isBlackWin)
	{
		g_dwMoveStr = 0;
		int i, j;
		int pcWhite[7];
		int pcBlack[7];
		char fen[1024];
		pcWhite[0] = SIDE_TAG(0) + KING_FROM;
		pcWhite[1] = SIDE_TAG(0) + ADVISOR_FROM;
		pcWhite[2] = SIDE_TAG(0) + BISHOP_FROM;
		pcWhite[3] = SIDE_TAG(0) + ROOK_FROM;
		pcWhite[4] = SIDE_TAG(0) + CANNON_FROM;
		pcWhite[5] = SIDE_TAG(0) + KNIGHT_FROM;
		pcWhite[6] = SIDE_TAG(0) + PAWN_FROM;

		for (i = 0; i < 7; i++)
		{
			pcBlack[i] = pcWhite[i] + 16;
		};
		LoadEvalApi("EVALUATE");
		PreGenInit();
		NewHash(24); // 24=16MB, 25=32MB, 26=64MB, ...
		Search.pos.ClearBoard();

		for (i = 0; i < 10; i++)
		{
			for (j = 0; j < 9; j++)
			{
				if ((Board[i][j] > 0) && (Board[i][j] < 8)) //Computer's pieces
					Search.pos.AddPiece(COORD_XY(j + FILE_LEFT, 9 - i + RANK_TOP), pcWhite[Board[i][j] - 1]);
				else if (Board[i][j] > 0) //Human's pieces
					Search.pos.AddPiece(COORD_XY(j + FILE_LEFT, 9 - i + RANK_TOP), pcBlack[Board[i][j] - 8]);
			};
		};
		memset(fen, 0, 1024);
		Search.pos.ToFen(fen);
		Search.pos.FromFen(fen);
		Search.pos.nDistance = 0;
		Search.PreEvaluate(&Search.pos, &PreEval);
		Search.nBanMoves = 0;
		Search.bIdle = Search.bPonder = Search.bQuit = Search.bBatch = Search.bDebug = Search.bAlwaysCheck = false;
		Search.bUseHash = true;
		Search.bUseBook = Search.bNullMove = Search.bKnowledge = true;
		Search.nCountMask = INTERRUPT_COUNT - 1;
		Search.nRandomMask = 0;
		Search.rc4Random.InitRand();
		Search.nGoMode = GO_MODE_INFINITY;
		SearchMain(1);
		if (g_dwMoveStr == 0)
		{
			*isBlackWin = false;
			DelHash();
			return true;
		}
		else
		{
			Search.pos.ChangeSide();
			SearchMain(1);
			DelHash();
			if (g_dwMoveStr == 0)
			{
				*isBlackWin = true;
				return true;
			};
		};
		return false;
	};

	//////////////////////////////////////////////////////////////////////////
	//Check whether the board is valid                                        //
	//In case of invalid board there're some reasons as following:            //
	//    1. Invalid piece position, such as the KING is not in the palace    //
	//    2. Invalid number some pieces (such as there're more than 2 rooks    //
	//    3. Invalid game logic such as:                                        //
	//        a. The chasing side move first                                    //
	//        b. The game is finished                                            //
	//Note that you have to indicate which side is going to move first        //
	//        1. bCompFirst = TRUE - The computer (piece < 8) first            //
	//        2. bCompFirst = FALSE - The player (piece > 7) first            //
	//////////////////////////////////////////////////////////////////////////

	bool isBoardValid(int board[10][9], bool bCompFirst)
	{
		int Count[14] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
		int PieceCount[14] = { 1,2,2,2,2,2,5,1,2,2,2,2,2,5 };
		int LegalCount[14] = { 9,5,7,0,0,0,10,9,5,7,0,0,0,10 };
		int LegalPos[14][10] = { { 3,4,5,12,13,14,21,22,23 },
		{ 3,5,13,21,23 },
		{ 2,6,18,22,26,38,42 },
		{ 0 },{ 0 },{ 0 },
		{ 27,29,31,33,35,36,38,40,42,44 },
		{ 84,85,86,75,76,77,66,67,68 },
		{ 84,86,76,66,68 },
		{ 83,87,63,67,71,47,51 },
		{ 0 },{ 0 },{ 0 },
		{ 45,47,49,51,53,54,56,58,60,62 }
		};
		bool ret = true;
		bool legalpos = false;
		bool legalcount = true;
		int i, j, k;
		for (i = 0; i < 10; i++)
		{
			for (j = 0; j < 9; j++)
			{
				if (board[i][j] != -1 && board[i][j] != 0)
				{
					Count[board[i][j] - 1] += 1;
					legalpos = LegalCount[board[i][j] - 1] == 0;
					for (k = 0; k < LegalCount[board[i][j] - 1]; k++)
					{
						if (i * 9 + j == LegalPos[board[i][j] - 1][k])
						{
							legalpos = true;
							break;
						};
					};
					if ((board[i][j] == 7) && (i >= 5))
						legalpos = true;
					if ((board[i][j] == 14) && (i <= 4))
						legalpos = true;
					ret = ret && legalpos;
				};
			};
		};
		for (i = 0; i < 14; i++)
		{
			if (Count[i] > PieceCount[i])
			{
				legalcount = false;
				break;
			};
		};
		ret = ret && legalcount;
		if (ret)
		{
			int pcWhite[7];
			int pcBlack[7];
			char fen[1024];
			bool bCompWon = false;
			pcWhite[0] = SIDE_TAG(0) + KING_FROM;
			pcWhite[1] = SIDE_TAG(0) + ADVISOR_FROM;
			pcWhite[2] = SIDE_TAG(0) + BISHOP_FROM;
			pcWhite[3] = SIDE_TAG(0) + ROOK_FROM;
			pcWhite[4] = SIDE_TAG(0) + CANNON_FROM;
			pcWhite[5] = SIDE_TAG(0) + KNIGHT_FROM;
			pcWhite[6] = SIDE_TAG(0) + PAWN_FROM;

			for (i = 0; i < 7; i++)
			{
				pcBlack[i] = pcWhite[i] + 16;
			};
			LoadEvalApi("EVALUATE");
			PreGenInit();
			Search.pos.ClearBoard();

			for (i = 0; i < 10; i++)
			{
				for (j = 0; j < 9; j++)
				{
					if ((board[i][j] > 0) && (board[i][j] < 8)) //Computer's pieces
						Search.pos.AddPiece(COORD_XY(j + FILE_LEFT, 9 - i + RANK_TOP), pcWhite[board[i][j] - 1]);
					else if (board[i][j] > 0) //Human's pieces
						Search.pos.AddPiece(COORD_XY(j + FILE_LEFT, 9 - i + RANK_TOP), pcBlack[board[i][j] - 8]);
				};
			};
			memset(fen, 0, 1024);
			Search.pos.ToFen(fen);
			Search.pos.FromFen(fen);
			Search.pos.nDistance = 0;
			Search.PreEvaluate(&Search.pos, &PreEval);
			Search.nBanMoves = 0;
			Search.bIdle = Search.bPonder = Search.bQuit = Search.bBatch = Search.bDebug = Search.bAlwaysCheck = false;
			Search.bUseHash = Search.bUseBook = Search.bNullMove = Search.bKnowledge = true;
			Search.nCountMask = INTERRUPT_COUNT - 1;
			Search.nRandomMask = 0;
			Search.rc4Random.InitRand();
			if (bCompFirst)
				Search.pos.ChangeSide();
			ret = ret && (Search.pos.CheckedBy() == 0) && !isGameFinished(board, &bCompWon);
		};
		return ret;
	};

	int getAllLegalMoves(int Board[10][9], int iTestCol, int iTestRow, int *iLegalCol, int *iLegalRow)
	{
		int i, j;
		int pcWhite[7];
		int pcBlack[7];
		char fen[1024];

		pcWhite[0] = SIDE_TAG(0) + KING_FROM;
		pcWhite[1] = SIDE_TAG(0) + ADVISOR_FROM;
		pcWhite[2] = SIDE_TAG(0) + BISHOP_FROM;
		pcWhite[3] = SIDE_TAG(0) + ROOK_FROM;
		pcWhite[4] = SIDE_TAG(0) + CANNON_FROM;
		pcWhite[5] = SIDE_TAG(0) + KNIGHT_FROM;
		pcWhite[6] = SIDE_TAG(0) + PAWN_FROM;

		for (i = 0; i < 7; i++)
		{
			pcBlack[i] = pcWhite[i] + 16;
		};

		SearchStruct localSearch;
		PreGenInit();
		localSearch.pos.ClearBoard();
		for (i = 0; i < 10; i++)
		{
			for (j = 0; j < 9; j++)
			{
				if ((Board[i][j] > 0) && (Board[i][j] < 8)) //Computer's pieces
					localSearch.pos.AddPiece(COORD_XY(j + FILE_LEFT, 9 - i + RANK_TOP), pcWhite[Board[i][j] - 1]);
				else if (Board[i][j] > 0) //Human's pieces
					localSearch.pos.AddPiece(COORD_XY(j + FILE_LEFT, 9 - i + RANK_TOP), pcBlack[Board[i][j] - 8]);
			};
		};

		memset(fen, 0, 1024);
		localSearch.pos.ToFen(fen);
		localSearch.pos.FromFen(fen);
		localSearch.pos.nDistance = 0;
		localSearch.nBanMoves = 0;
		localSearch.bIdle = localSearch.bPonder = localSearch.bQuit = localSearch.bBatch = localSearch.bDebug = localSearch.bAlwaysCheck = false;
		localSearch.bUseHash = false;
		localSearch.bUseBook = localSearch.bNullMove = localSearch.bKnowledge = false;
		localSearch.nCountMask = INTERRUPT_COUNT - 1;
		localSearch.nRandomMask = 0;
		localSearch.rc4Random.InitRand();

		if (Board[iTestRow][iTestCol] > 7)
		{
			localSearch.pos.ChangeSide();
		}

		uint32_t lpmv[MAX_GEN_MOVES];
		uint32_t nCount = genMoves(&localSearch.pos, lpmv);

		uint32_t dwMoveStr;
		int nLegalCount = 0;
		for (uint32_t i = 0; i<nCount; i++)
		{
			dwMoveStr = MOVE_COORD(lpmv[i]);
			char* pMoveStr = (char*)(&dwMoveStr);
			if (pMoveStr != NULL && strlen(pMoveStr)>0)
			{
				int iSrcCol = pMoveStr[0] - 'a';
				int iSrcRow = pMoveStr[1] - 48;
				int iDesCol = pMoveStr[2] - 'a';
				int iDesRow = pMoveStr[3] - 48;

				if (iTestCol == iSrcCol && iTestRow == iSrcRow)
				{
					iLegalCol[nLegalCount] = iDesCol;
					iLegalRow[nLegalCount] = iDesRow;
					nLegalCount++;
				}
			}
		}
		return nLegalCount;
	}

	uint32_t genMoves(PositionStruct *lppos, uint32_t* lpmv)
	{
		int i, nTotal, nLegal;
		MoveStruct mvs[MAX_GEN_MOVES];
		nTotal = lppos->GenAllMoves(mvs);
		nLegal = 0;
		for (i = 0; i < nTotal; i++)
		{
			if (lppos->MakeMove(mvs[i].wmv))
			{
				lppos->UndoMakeMove();
				lpmv[nLegal] = mvs[i].wmv;
				nLegal++;
			}
		}
		return nLegal;
	}

	bool findGeneralPiece(int Board[10][9], bool isRedTurn, int* retX, int* retY) {
		if (isRedTurn) {
			//Tim vua den
			for (int x = 3; x <= 5; x++)
				for (int y = 7; y <= 9; y++) {
					if (Board[y][x] == 8) {
						*retX = x;
						*retY = y;
						return true;
					}
				}
		}
		else {
			//Tim vua do
			for (int x = 3; x <= 5; x++)
				for (int y = 0; y <= 2; y++) {
					if (Board[y][x] == 1) {
						*retX = x;
						*retY = y;
						return true;
					}
				}
		}
		return false;
	}

	bool isRedPiece(int piece) {
		int type = abs(piece);
		if (type > 0 && type < 8)
			return true;
		else
			return false;
	}

	bool isCheck(int Board[10][9], bool isRedTurn) {
		int X = 0;
		int Y = 0;
		if (!findGeneralPiece(Board, isRedTurn, &X, &Y)) {
			//Khong tim thay quan tuong, error
			printf("Cannot find the general!");
			return false;
		}
		printf("Find General Piece: X %d Y %d", X, Y);
		for (int x = 0; x < 9; x++)
			for (int y = 0; y < 10; y++) {
				if (Board[y][x] != 0 && isRedPiece(Board[y][x]) == isRedTurn) {
					if (isLegalMove(Board, x, y, X, Y)) {
						printf("Check Move: X %d Y %d Piece %d", x, y, Board[y][x]);
						return true;
					}
				}
			}
		return false;
	}
}
