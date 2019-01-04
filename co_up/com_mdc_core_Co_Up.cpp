#include "com_mdc_core_Co_Up.h"
extern "C" {
	//////////////////////////////////////////////////////////////////////////
	//Check whether the game is finished,                                    //
	//In this case the function returns TRUE and                            //
	//                1. isBlackWin = TRUE if the computer has won the game        //
	//                2. isBlackWin = FALSE if the player has won the game        //
	//////////////////////////////////////////////////////////////////////////
	DLL_EXPORT bool co_up_isGameFinished(int Board[10][9], bool isRedTurn, bool* isBlackWin) {
		int legalX[20];
		int legalY[20];
		int *iLegalX = &legalX[0];
		int *iLegalY = &legalY[0];
		for (int x = 0; x < 9; x++)
			for (int y = 0; y < 10; y++) {
				if (Board[y][x] != 0 && co_up_isRedPiece(Board[y][x]) == isRedTurn) {
					if (co_up_getAllLegalMoves(Board, x, y, iLegalX, iLegalY) > 0) {
						return false;
					}
				}
			}
		*isBlackWin = !isRedTurn;
		return true;
	}

	DLL_EXPORT NuocCo co_up_letComputerThink(int board[10][9], int iDepth, long lngLimitTime) {
		int *pSrcX = new int();
		int *pSrcY = new int();
		int *pDstX = new int();
		int *pDstY = new int();
		search (board, iDepth, true, lngLimitTime, pSrcX, pSrcY, pDstX, pDstY);
		NuocCo nuocCo;
		nuocCo.iSrcCol = *pSrcX;
		nuocCo.iSrcRow = *pSrcY;
		nuocCo.iDstCol = *pDstX;
		nuocCo.iDstRow = *pDstY;
		return nuocCo;
	}

	DLL_EXPORT NuocCo co_up_letComputerThinkForHint(int board[10][9], int iDepth, long lngLimitTime) {
		int *pSrcX = new int();
		int *pSrcY = new int();
		int *pDstX = new int();
		int *pDstY = new int();
		search (board, iDepth, false, lngLimitTime, pSrcX, pSrcY, pDstX, pDstY);
		NuocCo nuocCo;
		nuocCo.iSrcCol = *pSrcX;
		nuocCo.iSrcRow = *pSrcY;
		nuocCo.iDstCol = *pDstX;
		nuocCo.iDstRow = *pDstY;
		return nuocCo;
	}

	DLL_EXPORT bool isLegalMoveWithoutCheck(int board[10][9], int iSrcCol, int iSrcRow,
		int iDstCol, int iDstRow) {
		return true;
	}

	DLL_EXPORT bool co_up_isRedPiece(int piece) {
		int type = abs(piece);
		if (type > 0 && type < 8)
			return true;
		else
			return false;
	}

	DLL_EXPORT void convertToNormalBoard(int Board[10][9], int board[10][9]) {
		int chess_desk[10][9] = { 
			{ 4, 6, 3, 2, 1, 2, 3, 6, 4 },
			{ 0, 0, 0, 0, 0, 0,	0, 0, 0 },
			{ 0, 5, 0, 0, 0, 0, 0, 5, 0 },
			{ 7, 0, 7, 0, 7, 0, 7, 0, 7 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 14, 0, 14, 0, 14, 0, 14, 0, 14 },
			{ 0, 12, 0, 0, 0, 0, 0, 12, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 11, 13, 10, 9, 8, 9, 10, 13, 11 } };

		for (int x = 0; x < 9; x++) {
			for (int y = 0; y < 10; y++) {
				if (Board[y][x] < 0)
					board[y][x] = chess_desk[y][x];
				else
					board[y][x] = Board[y][x];
			}
		}
	}

	DLL_EXPORT int co_up_getAllLegalMoves(int Board[10][9], int X, int Y, int *iLegalX,
		int *iLegalY) {
		//	for (int i = 0; i < 9; i++)
		//		for (int j = 0; j < 10; j++) {
		//			LOGI("Origin Board x:%d,y:%d : %d", i, j, Board[i][j]);
		//		}
		int board[10][9];
		convertToNormalBoard(Board, board);

		int nLegalCount = 0;
		//	for (int i = 0; i < 9; i++)
		//		for (int j = 0; j < 10; j++) {
		//			LOGI("Normal Board x:%d,y:%d : %d", i, j, board[i][j]);
		//		}

		switch (board[Y][X]) {
		case 1:
			//vua do
			//duoi
			if (Y - 1 >= 0
				&& (board[Y - 1][X] == 0 || !co_up_isRedPiece(board[Y - 1][X]))) {
				//Kiem tra 2 tuong doi mat
				bool check = true;
				for (int i = Y + 2; i <= 9; i++) {
					if (board[i][X] == 0) {
						continue;
					}
					else if (board[i][X] != 8) {
						break;
					}
					else if (board[i][X] == 8) {
						check = false;
					}
				}
				//Ghi ket qua
				if (check) {
					iLegalX[nLegalCount] = X;
					iLegalY[nLegalCount] = Y - 1;
					nLegalCount++;
				}
			}
			//tren
			if (Y + 1 <= 2
				&& (board[Y + 1][X] == 0 || !co_up_isRedPiece(board[Y + 1][X]))) {
				//Kiem tra 2 tuong doi mat
				bool check = true;
				for (int y = Y + 2; y <= 9; y++) {
					if (board[y][X] == 0) {
						continue;
					}
					else if (board[y][X] != 8) {
						break;
					}
					else if (board[y][X] == 8) {
						check = false;
					}
				}
				//Ghi ket qua
				if (check) {
					iLegalX[nLegalCount] = X;
					iLegalY[nLegalCount] = Y + 1;
					nLegalCount++;
				}
			}
			//trai
			if (X - 1 >= 3
				&& (board[Y][X - 1] == 0 || !co_up_isRedPiece(board[Y][X - 1]))) {
				//Kiem tra 2 tuong doi mat
				bool check = true;
				for (int y = Y + 1; y <= 9; y++) {
					if (board[y][X - 1] == 0) {
						continue;
					}
					else if (board[y][X - 1] != 8) {
						break;
					}
					else if (board[y][X - 1] == 8) {
						check = false;
					}
				}
				//Ghi ket qua
				if (check) {
					iLegalX[nLegalCount] = X - 1;
					iLegalY[nLegalCount] = Y;
					nLegalCount++;
				}
			}
			//phai
			if (X + 1 <= 5
				&& (board[Y][X + 1] == 0 || !co_up_isRedPiece(board[Y][X + 1]))) {
				//Kiem tra 2 tuong doi mat
				bool check = true;
				for (int y = Y + 1; y <= 9; y++) {
					if (board[y][X + 1] == 0) {
						continue;
					}
					else if (board[y][X + 1] != 8) {
						break;
					}
					else if (board[y][X + 1] == 8) {
						check = false;
					}
				}
				//Ghi ket qua
				if (check) {
					iLegalX[nLegalCount] = X + 1;
					iLegalY[nLegalCount] = Y;
					nLegalCount++;
				}
			}
			//Kiem tra co nuoc tuong doi mat voi tuong
			for (int y = Y + 1; y <= 9; y++) {
				if (board[y][X] == 0) {
					//LOGI("Kiem tra tuong doi mat tuong continue %d %d", X, i);
					continue;
				}
				else if (board[y][X] != 8) {
					//LOGI("Kiem tra tuong doi mat tuong break %d %d", X, i);
					break;
				}
				else if (board[y][X] == 8) {
					iLegalX[nLegalCount] = X;
					iLegalY[nLegalCount] = y;
					nLegalCount++;
					//LOGI("Kiem tra tuong doi mat tuong add %d %d", X, i);
					break;
				}
			}
			break;
		case 8:
			//vua den
			//duoi
			if (Y - 1 >= 7
				&& (board[Y - 1][X] == 0 || co_up_isRedPiece(board[Y - 1][X]))) {
				//Check 2 tuong doi dien nhau
				bool check = true;
				for (int y = Y - 2; y >= 0; y--) {
					if (board[y][X] == 0) {
						continue;
					}
					else if (board[y][X] != 1) {
						break;
					}
					else if (board[y][X] == 1) {
						check = false;
					}
				}
				//Ghi ket qua
				if (check) {
					iLegalX[nLegalCount] = X;
					iLegalY[nLegalCount] = Y - 1;
					nLegalCount++;
				}
			}
			//tren
			if (Y + 1 <= 9
				&& (board[Y + 1][X] == 0 || co_up_isRedPiece(board[Y + 1][X]))) {
				//Check 2 tuong doi dien nhau
				bool check = true;
				for (int y = Y; y >= 0; y--) {
					if (board[y][X] == 0) {
						continue;
					}
					else if (board[y][X] != 1) {
						break;
					}
					else if (board[y][X] == 1) {
						check = false;
					}
				}
				//Ghi ket qua
				iLegalX[nLegalCount] = X;
				iLegalY[nLegalCount] = Y + 1;
				nLegalCount++;
			}
			//trai
			if (X - 1 >= 3
				&& (board[Y][X - 1] == 0 || co_up_isRedPiece(board[Y][X - 1]))) {
				//Check 2 tuong doi dien nhau
				bool check = true;
				for (int y = Y - 1; y >= 0; y--) {
					if (board[y][X - 1] == 0) {
						continue;
					}
					else if (board[y][X - 1] != 1) {
						break;
					}
					else if (board[y][X - 1] == 1) {
						check = false;
					}
				}
				//Ghi ket qua
				if (check) {
					iLegalX[nLegalCount] = X - 1;
					iLegalY[nLegalCount] = Y;
					nLegalCount++;
				}
			}
			//phai
			if (X + 1 <= 5
				&& (board[Y][X + 1] == 0 || co_up_isRedPiece(board[Y][X + 1]))) {
				//Check 2 tuong doi dien nhau
				bool check = true;
				for (int y = Y - 1; y >= 0; y--) {
					if (board[y][X + 1] == 0) {
						continue;
					}
					else if (board[y][X + 1] != 1) {
						break;
					}
					else if (board[y][X + 1] == 1) {
						check = false;
					}
				}
				//Ghi ket qua
				iLegalX[nLegalCount] = X + 1;
				iLegalY[nLegalCount] = Y;
				nLegalCount++;
			}
			//Kiem tra co nuoc tuong doi mat voi tuong
			for (int y = Y - 1; y >= 0; y--) {
				if (board[y][X] == 0) {
					continue;
				}
				else if (board[y][X] != 1) {
					break;
				}
				else if (board[y][X] == 1) {
					iLegalX[nLegalCount] = X;
					iLegalY[nLegalCount] = y;
					nLegalCount++;
					break;
				}
			}
			break;
		case 2:
			//sy do
		case 9:
			//Sy den
			//Quan con dang up, chi co 1 nuoc di cheo
			if (Board[Y][X] < 0) {
				if ((X == 3 && Y == 0) || (X == 5 && Y == 0)) {
					if (board[1][4] == 0
						|| co_up_isRedPiece(board[Y][X]) != co_up_isRedPiece(board[1][4])) {
						iLegalX[nLegalCount] = 4;
						iLegalY[nLegalCount] = 1;
						nLegalCount++;
					}
				}
				else if ((X == 3 && Y == 9) || (X == 5 && Y == 9)) {
					if (board[8][4] == 0
						|| co_up_isRedPiece(board[Y][X]) != co_up_isRedPiece(board[8][4])) {
						iLegalX[nLegalCount] = 4;
						iLegalY[nLegalCount] = 8;
						nLegalCount++;
					}
				}
			}
			else {
				//Quan da lat, di 4 goc xung quang
				//ben trai, duoi
				if (X - 1 >= 0 && Y - 1 >= 0
					&& (board[Y - 1][X - 1] == 0
						|| co_up_isRedPiece(board[Y][X])
						!= co_up_isRedPiece(board[Y - 1][X - 1]))) {
					iLegalX[nLegalCount] = X - 1;
					iLegalY[nLegalCount] = Y - 1;
					nLegalCount++;
				}
				//ben trai, tren
				if (X - 1 >= 0 && Y + 1 <= 9
					&& (board[Y + 1][X - 1] == 0
						|| co_up_isRedPiece(board[Y][X])
						!= co_up_isRedPiece(board[Y + 1][X - 1]))) {
					iLegalX[nLegalCount] = X - 1;
					iLegalY[nLegalCount] = Y + 1;
					nLegalCount++;
				}
				//ben phai, duoi
				if (X + 1 <= 8 && Y - 1 >= 0
					&& (board[Y - 1][X + 1] == 0
						|| co_up_isRedPiece(board[Y][X])
						!= co_up_isRedPiece(board[Y - 1][X + 1]))) {
					iLegalX[nLegalCount] = X + 1;
					iLegalY[nLegalCount] = Y - 1;
					nLegalCount++;
				}
				//ben phai, tren
				if (X + 1 <= 8 && Y + 1 <= 9
					&& (board[Y + 1][X + 1] == 0
						|| co_up_isRedPiece(board[Y][X])
						!= co_up_isRedPiece(board[Y + 1][X + 1]))) {
					iLegalX[nLegalCount] = X + 1;
					iLegalY[nLegalCount] = Y + 1;
					nLegalCount++;
				}
			}
			break;
		case 3:
			//tuong do
		case 10:
			//tuong den
			//ben trai, duoi
			if (X - 2 >= 0 && Y - 2 >= 0
				&& (board[Y - 2][X - 2] == 0
					|| co_up_isRedPiece(board[Y][X])
					!= co_up_isRedPiece(board[Y - 2][X - 2]))) {
				//Ko bi chiem
				if (board[Y - 1][X - 1] == 0) {
					//Ko bi can
					iLegalX[nLegalCount] = X - 2;
					iLegalY[nLegalCount] = Y - 2;
					nLegalCount++;
				}
			}
			//ben trai, tren
			if (X - 2 >= 0 && Y + 2 <= 9
				&& (board[Y + 2][X - 2] == 0
					|| co_up_isRedPiece(board[Y][X])
					!= co_up_isRedPiece(board[Y + 2][X - 2]))) {
				//Ko bi chiem
				if (board[Y + 1][X - 1] == 0) {
					//Ko bi can
					iLegalX[nLegalCount] = X - 2;
					iLegalY[nLegalCount] = Y + 2;
					nLegalCount++;
				}
			}
			//ben phai, duoi
			if (X + 2 <= 8 && Y - 2 >= 0
				&& (board[Y - 2][X + 2] == 0
					|| co_up_isRedPiece(board[Y][X])
					!= co_up_isRedPiece(board[Y - 2][X + 2]))) {
				//Ko bi chiem
				if (board[Y - 1][X + 1] == 0) {
					//Ko bi can
					iLegalX[nLegalCount] = X + 2;
					iLegalY[nLegalCount] = Y - 2;
					nLegalCount++;
				}
			}
			//ben phai, tren
			if (X + 2 <= 8 && Y + 2 <= 9
				&& (board[Y + 2][X + 2] == 0
					|| co_up_isRedPiece(board[Y][X])
					!= co_up_isRedPiece(board[Y + 2][X + 2]))) {
				//Ko bi chiem
				if (board[Y + 1][X + 1] == 0) {
					//Ko bi can
					iLegalX[nLegalCount] = X + 2;
					iLegalY[nLegalCount] = Y + 2;
					nLegalCount++;
				}
			}
			break;
		case 4:
			//xe do
		case 11:
			//xe den
			//ngang ben trai
			for (int x = X - 1; x >= 0; x--) {
				if (board[Y][x] == 0) {
					iLegalX[nLegalCount] = x;
					iLegalY[nLegalCount] = Y;
					nLegalCount++;
				}
				else if (co_up_isRedPiece(board[Y][X]) != co_up_isRedPiece(board[Y][x])) {
					iLegalX[nLegalCount] = x;
					iLegalY[nLegalCount] = Y;
					nLegalCount++;
					break;
				}
				else {
					break;
				}
			}
			//ngang ben phai
			for (int x = X + 1; x < 9; x++) {
				if (board[Y][x] == 0) {
					iLegalX[nLegalCount] = x;
					iLegalY[nLegalCount] = Y;
					nLegalCount++;
				}
				else if (co_up_isRedPiece(board[Y][X]) != co_up_isRedPiece(board[Y][x])) {
					iLegalX[nLegalCount] = x;
					iLegalY[nLegalCount] = Y;
					nLegalCount++;
					break;
				}
				else {
					break;
				}
			}
			//xuong duoi
			for (int y = Y - 1; y >= 0; y--) {
				if (board[y][X] == 0) {
					iLegalX[nLegalCount] = X;
					iLegalY[nLegalCount] = y;
					nLegalCount++;
				}
				else if (co_up_isRedPiece(board[Y][X]) != co_up_isRedPiece(board[y][X])) {
					iLegalX[nLegalCount] = X;
					iLegalY[nLegalCount] = y;
					nLegalCount++;
					break;
				}
				else {
					break;
				}
			}
			//len tren
			for (int y = Y + 1; y <= 9; y++) {
				if (board[y][X] == 0) {
					iLegalX[nLegalCount] = X;
					iLegalY[nLegalCount] = y;
					nLegalCount++;
				}
				else if (co_up_isRedPiece(board[Y][X]) != co_up_isRedPiece(board[y][X])) {
					iLegalX[nLegalCount] = X;
					iLegalY[nLegalCount] = y;
					nLegalCount++;
					break;
				}
				else {
					break;
				}
			}
			break;
		case 5:
			//phao do
		case 12:
			//phao den
			//hang ngang ben trai
			for (int x = X - 1; x >= 0; x--) {
				if (board[Y][x] == 0) {
					iLegalX[nLegalCount] = x;
					iLegalY[nLegalCount] = Y;
					nLegalCount++;
				}
				else {
					//vuot bat
					for (int j = x - 1; j >= 0; j--) {
						if (board[Y][j] != 0) {
							if (co_up_isRedPiece(board[Y][X])
								!= co_up_isRedPiece(board[Y][j])) {
								iLegalX[nLegalCount] = j;
								iLegalY[nLegalCount] = Y;
								nLegalCount++;
							}
							break;
						}
					}
					break;
				}
			}
			//hang ngang ben phai
			for (int x = X + 1; x <= 8; x++) {
				if (board[Y][x] == 0) {
					iLegalX[nLegalCount] = x;
					iLegalY[nLegalCount] = Y;
					nLegalCount++;
				}
				else {
					//vuot bat
					for (int j = x + 1; j <= 8; j++) {
						if (board[Y][j] != 0) {
							if (co_up_isRedPiece(board[Y][X])
								!= co_up_isRedPiece(board[Y][j])) {
								iLegalX[nLegalCount] = j;
								iLegalY[nLegalCount] = Y;
								nLegalCount++;
							}
							break;
						}
					}
					break;
				}
			}
			//hang doc phia duoi
			for (int y = Y - 1; y >= 0; y--) {
				if (board[y][X] == 0) {
					iLegalX[nLegalCount] = X;
					iLegalY[nLegalCount] = y;
					nLegalCount++;
				}
				else {
					//vuot bat
					for (int j = y - 1; j >= 0; j--) {
						if (board[j][X] != 0) {
							if (co_up_isRedPiece(board[Y][X])
								!= co_up_isRedPiece(board[j][X])) {
								iLegalX[nLegalCount] = X;
								iLegalY[nLegalCount] = j;
								nLegalCount++;
							}
							break;
						}
					}
					break;
				}
			}
			//hang doc phia tren
			for (int y = Y + 1; y <= 9; y++) {
				if (board[y][X] == 0) {
					iLegalX[nLegalCount] = X;
					iLegalY[nLegalCount] = y;
					nLegalCount++;
				}
				else {
					//vuot bat
					for (int j = y + 1; j <= 9; j++) {
						if (board[j][X] != 0) {
							if (co_up_isRedPiece(board[Y][X])
								!= co_up_isRedPiece(board[j][X])) {
								iLegalX[nLegalCount] = X;
								iLegalY[nLegalCount] = j;
								nLegalCount++;
							}
							break;
						}
					}
					break;
				}
			}
			break;
		case 6:
			//ma do
		case 13:
			//ma den
			if (X + 1 <= 8 && Y + 2 <= 9 && board[Y + 1][X] == 0
				&& (board[Y + 2][X + 1] == 0
					|| co_up_isRedPiece(board[Y][X])
					!= co_up_isRedPiece(board[Y + 2][X + 1]))) {
				iLegalX[nLegalCount] = X + 1;
				iLegalY[nLegalCount] = Y + 2;
				nLegalCount++;
			}
			if (X + 1 <= 8 && Y - 2 >= 0 && board[Y - 1][X] == 0
				&& (board[Y - 2][X + 1] == 0
					|| co_up_isRedPiece(board[Y][X])
					!= co_up_isRedPiece(board[Y - 2][X + 1]))) {
				iLegalX[nLegalCount] = X + 1;
				iLegalY[nLegalCount] = Y - 2;
				nLegalCount++;
			}
			if (X - 1 >= 0 && Y + 2 <= 9 && board[Y + 1][X] == 0
				&& (board[Y + 2][X - 1] == 0
					|| co_up_isRedPiece(board[Y][X])
					!= co_up_isRedPiece(board[Y + 2][X - 1]))) {
				iLegalX[nLegalCount] = X - 1;
				iLegalY[nLegalCount] = Y + 2;
				nLegalCount++;
			}
			if (X - 1 >= 0 && Y - 2 >= 0 && board[Y - 1][X] == 0
				&& (board[Y - 2][X - 1] == 0
					|| co_up_isRedPiece(board[Y][X])
					!= co_up_isRedPiece(board[Y - 2][X - 1]))) {
				iLegalX[nLegalCount] = X - 1;
				iLegalY[nLegalCount] = Y - 2;
				nLegalCount++;
			}
			if (X + 2 <= 8 && Y + 1 <= 9 && board[Y][X + 1] == 0
				&& (board[Y + 1][X + 2] == 0
					|| co_up_isRedPiece(board[Y][X])
					!= co_up_isRedPiece(board[Y + 1][X + 2]))) {
				iLegalX[nLegalCount] = X + 2;
				iLegalY[nLegalCount] = Y + 1;
				nLegalCount++;
			}
			if (X + 2 <= 8 && Y - 1 >= 0 && board[Y][X + 1] == 0
				&& (board[Y - 1][X + 2] == 0
					|| co_up_isRedPiece(board[Y][X])
					!= co_up_isRedPiece(board[Y - 1][X + 2]))) {
				iLegalX[nLegalCount] = X + 2;
				iLegalY[nLegalCount] = Y - 1;
				nLegalCount++;
			}
			if (X - 2 >= 0 && Y + 1 <= 9 && board[Y][X - 1] == 0
				&& (board[Y + 1][X - 2] == 0
					|| co_up_isRedPiece(board[Y][X])
					!= co_up_isRedPiece(board[Y + 1][X - 2]))) {
				iLegalX[nLegalCount] = X - 2;
				iLegalY[nLegalCount] = Y + 1;
				nLegalCount++;
			}
			if (X - 2 >= 0 && Y - 1 >= 0 && board[Y][X - 1] == 0
				&& (board[Y - 1][X - 2] == 0
					|| co_up_isRedPiece(board[Y][X])
					!= co_up_isRedPiece(board[Y - 1][X - 2]))) {
				iLegalX[nLegalCount] = X - 2;
				iLegalY[nLegalCount] = Y - 1;
				nLegalCount++;
			}
			break;
		case 7:
			//tot do
			if (Y + 1 <= 9
				&& (board[Y + 1][X] == 0
					|| co_up_isRedPiece(board[Y][X])
					!= co_up_isRedPiece(board[Y + 1][X]))) {
				iLegalX[nLegalCount] = X;
				iLegalY[nLegalCount] = Y + 1;
				nLegalCount++;
			}
			if (Y >= 5) {
				//tot sang song
				if (X - 1 >= 0
					&& (board[Y][X - 1] == 0
						|| co_up_isRedPiece(board[Y][X])
						!= co_up_isRedPiece(board[Y][X - 1]))) {
					//trai
					iLegalX[nLegalCount] = X - 1;
					iLegalY[nLegalCount] = Y;
					nLegalCount++;
				}
				if (X + 1 <= 8
					&& (board[Y][X + 1] == 0
						|| co_up_isRedPiece(board[Y][X])
						!= co_up_isRedPiece(board[Y][X + 1]))) {
					//phai
					iLegalX[nLegalCount] = X + 1;
					iLegalY[nLegalCount] = Y;
					nLegalCount++;
				}
			}
			break;
		case 14:
			//tot den
			if (Y - 1 >= 0
				&& (board[Y - 1][X] == 0
					|| co_up_isRedPiece(board[Y][X])
					!= co_up_isRedPiece(board[Y - 1][X]))) {
				iLegalX[nLegalCount] = X;
				iLegalY[nLegalCount] = Y - 1;
				nLegalCount++;
			}
			if (Y <= 4) {
				//tot sang song
				if (X - 1 >= 0
					&& (board[Y][X - 1] == 0
						|| co_up_isRedPiece(board[Y][X])
						!= co_up_isRedPiece(board[Y][X - 1]))) {
					//trai
					iLegalX[nLegalCount] = X - 1;
					iLegalY[nLegalCount] = Y;
					nLegalCount++;
				}
				if (X + 1 <= 8
					&& (board[Y][X + 1] == 0
						|| co_up_isRedPiece(board[Y][X])
						!= co_up_isRedPiece(board[Y][X + 1]))) {
					//phai
					iLegalX[nLegalCount] = X + 1;
					iLegalY[nLegalCount] = Y;
					nLegalCount++;
				}
			}
			break;
		}
		//Log Result
		for (int i = 0; i < nLegalCount; i++) {
		}
		//Loai bo nhung nuoc di lam chieu tuong
		for (int i = 0; i < nLegalCount; i++) {
			int nextBoard[10][9];
			co_up_getBoard(Board, X, Y, iLegalX[i], iLegalY[i], nextBoard);
			if (co_up_isCheck(nextBoard, !co_up_isRedPiece(Board[Y][X]))) {
				//LOGI("Remove check move: %d %d %d %d %d", X, Y, iLegalX[i],
				//		iLegalY[i], nextBoard[iLegalY[i]][iLegalX[i]]);
				//Remove check move
				for (int j = i; j < nLegalCount - 1; j++) {
					iLegalX[j] = iLegalX[j + 1];
					iLegalY[j] = iLegalY[j + 1];
				}
				nLegalCount--;
				i--;
			}
		}
		//LOGE("Get all legal move from: %d %d,  return count: %d!", X, Y,
		//		nLegalCount);
		return nLegalCount;
	}

	DLL_EXPORT int co_up_generateAllLegalMoves(int Board[10][9], bool isRedTurn, int *retLegalSrcX,
		int *retLegalSrcY, int *retLegalDstX, int *retLegalDstY) {
		printf("Co Up generate all legal moves!");

		/*for (int y = 0; y < 10; y++)
		for (int x = 0; x < 9; x++) {
		LOGE("Origin Board x:%d,y:%d : %d", x, y, Board[y][x]);
		}*/

		int nLegalCount = 0;

		for (int y = 0; y < 10; y++)
			for (int x = 0; x < 9; x++) {
				if (Board[y][x] != 0 && co_up_isRedPiece(Board[y][x]) == isRedTurn) {
					//LOGE("Get Legal Move: %d, %d , %d!", x, y, Board[y][x]);
					int legalX[200];
					int legalY[200];
					int *iLegalX = &legalX[0];
					int *iLegalY = &legalY[0];
					int count = co_up_getAllLegalMoves(Board, x, y, iLegalX, iLegalY);
					//LOGE("Legal Move Count: %d!", count);
					for (int i = 0; i < count; i++) {
						//LOGE("Find Legal Move %d %d %d %d!", x, y, legalX[i],
						//legalY[i]);
						retLegalSrcX[nLegalCount + i] = x;
						retLegalSrcY[nLegalCount + i] = y;
						retLegalDstX[nLegalCount + i] = legalX[i];
						retLegalDstY[nLegalCount + i] = legalY[i];
					}
					nLegalCount += count;
				}
			}
		//LOGE("Generate all legal move: moves found count: %d!", nLegalCount);
		return nLegalCount;
	}

	DLL_EXPORT bool co_up_isLegalMove(int Board[10][9], int iSrcX, int iSrcY, int iDstX, int iDstY,
		bool isNeedCheck) {
		bool ret = false;
		if (iSrcX < 0 || iSrcX > 8 || iSrcY < 0 || iSrcY > 9 || iDstX < 0
			|| iDstX > 8 || iDstY < 0 || iDstY > 9
			|| Board[iSrcY][iSrcX] == 0) {
			//Ngoai ban co
			return false;
		}
		if (iSrcX == iDstX && iSrcY == iDstY) {
			//Diem xuat phat va toi trung nhau
			return false;
		}

		int board[10][9];
		convertToNormalBoard(Board, board);
		switch (board[iSrcY][iSrcX]) {
		case 1:
			//vua do
			//ngoai cung dien
			if (iSrcX < 3 || iSrcX > 5 || iSrcY < 0 || iSrcY > 2 || iDstX < 3
				|| iDstX > 5 || iDstY < 0 || (iDstY > 2 && iDstY < 7)
				|| iDstY > 9)
				return false;
			//O trong hoac quan an duoc
			if (board[iDstY][iDstX] == 0 || !co_up_isRedPiece(board[iDstY][iDstX])) {
				//LOGI("Vua do %d %d %d %d %d", iSrcX, iSrcY, iDstX, iDstY, board[iDstX][iDstY]);
				//nam o lan can
				if ((abs(iDstX - iSrcX) == 1 && (iDstY == iSrcY))
					|| (abs(iDstY - iSrcY) == 1 && (iDstX == iSrcX))) {
					//Kiem tra 2 tuong doi mat
					for (int y = iDstY + 1; y <= 9; y++) {
						if (board[y][iDstX] == 0) {
							continue;
						}
						else if (board[y][iDstX] != 8) {
							ret = true;
							break;
						}
						else if (board[y][iDstX] == 8) {
							return false;
						}
					}
					ret = true;
				}
				else if (board[iDstY][iDstX] == 8 && iSrcX == iDstX) {
					//Tuong do an Tuong den
					bool check = true;
					for (int i = iSrcY + 1; i < iDstY; i++) {
						if (board[i][iSrcX] != 0) {
							return false;
						}
					}
					if (check)
						return true;
				}
			}
			break;
		case 8:
			//vua den
			//ngoai cung dien
			if (iSrcX < 3 || iSrcX > 5 || iSrcY < 7 || iSrcY > 9 || iDstX < 3
				|| iDstX > 5 || iDstY < 0 || (iDstY > 2 && iDstY < 7)
				|| iDstY > 9)
				return false;
			//O trong hoac quan an duoc
			if (board[iDstY][iDstX] == 0 || co_up_isRedPiece(board[iDstY][iDstX])) {
				//nam o lan can
				if ((abs(iDstX - iSrcX) == 1 && (iDstY == iSrcY))
					|| (abs(iDstY - iSrcY) == 1 && (iDstX == iSrcX))) {
					//Kiem tra 2 tuong doi mat
					for (int y = iDstY - 1; y >= 0; y--) {
						if (board[y][iDstX] == 0) {
							continue;
						}
						else if (board[y][iDstX] != 1) {
							ret = true;
							break;
						}
						else if (board[y][iDstX] == 1) {
							return false;
						}
					}
					ret = true;
				}
				else if (board[iDstY][iDstX] == 1 && iSrcX == iDstX) {
					//Tuong do an Tuong den
					bool check = true;
					for (int i = iSrcY - 1; i > iDstY; i--) {
						if (board[i][iSrcX] != 0)
							return false;
					}
					if (check)
						return true;
				}
			}
			break;
		case 2:
			//Sy do
		case 9:
			//sy den
			if (Board[iSrcY][iSrcX] < 0) {
				//Quan Co Chua Lat
				if ((iSrcX == 3 && iSrcY == 0 && iDstX == 4 && iDstY == 1)
					|| (iSrcX == 5 && iSrcY == 0 && iDstX == 4 && iDstY == 1)
					|| (iSrcX == 3 && iSrcY == 9 && iDstX == 4 && iDstY == 8)
					|| (iSrcX == 5 && iSrcY == 9 && iDstX == 4 && iDstY == 8))
					ret = true;
			}
			else if (abs(iDstX - iSrcX) == 1 && abs(iDstY - iSrcY) == 1	&&
				(board[iDstY][iDstX] == 0 ||
					co_up_isRedPiece(board[iSrcY][iSrcX]) != co_up_isRedPiece(board[iDstY][iDstX]))) {
				//Quan Co da lat
				ret = true;
			}
			else
				return false;
			break;
		case 3:
			//tuong do
		case 10:
			//tuong den
			//Cach 2 o
			if (abs(iDstX - iSrcX) == 2 && abs(iDstY - iSrcY) == 2) {
				//O toi trong
				if (board[iDstY][iDstX] == 0
					|| co_up_isRedPiece(board[iSrcY][iSrcX])
					!= co_up_isRedPiece(board[iDstY][iDstX])) {
					//Khong co quan can o giua
					if (board[(iSrcY + iDstY) / 2][(iSrcX + iDstX) / 2] == 0) {
						ret = true;
					}
					else {
						return false;
					}
				}
			}
			break;
		case 4:
			//xe do
		case 11:
			//xe den
			if (board[iDstY][iDstX] == 0
				|| co_up_isRedPiece(board[iSrcY][iSrcX])
				!= co_up_isRedPiece(board[iDstY][iDstX])) {
				//Diem toi con trong hoac quan khac mau
				if (iSrcX == iDstX) {
					//Cung hang doc
					if (iSrcY > iDstY) {
						for (int y = iDstY + 1; y < iSrcY; y++) {
							if (board[y][iSrcX] != 0)
								return false;
						}
					}
					else if (iSrcY < iDstY) {
						for (int y = iSrcY + 1; y < iDstY; y++) {
							if (board[y][iSrcX] != 0)
								return false;
						}
					}
					ret = true;
				}
				else if (iSrcY == iDstY) {
					//Cung hang ngang
					if (iSrcX > iDstX) {
						for (int x = iDstX + 1; x < iSrcX; x++) {
							if (board[iSrcY][x] != 0)
								return false;
						}
					}
					else if (iSrcX < iDstX) {
						for (int x = iSrcX + 1; x < iDstX; x++) {
							if (board[iSrcY][x] != 0)
								return false;
						}
					}
					ret = true;
				}
			}
			break;
		case 5:
			//phao do
		case 12:
			//phao den
			if (iSrcX != iDstX && iSrcY != iDstY) {
				//Khong cung hang hoac cot
				return false;
			}
			if (board[iDstY][iDstX] == 0) {
				//Diem toi trong
				if (iSrcX == iDstX) {
					//Cung cot
					if (iSrcY > iDstY) {
						for (int y = iDstY + 1; y < iSrcY; y++) {
							if (board[y][iSrcX] != 0)
								return false;
						}
					}
					else if (iSrcY < iDstY) {
						for (int y = iSrcY + 1; y < iDstY; y++) {
							if (board[y][iSrcX] != 0)
								return false;
						}
					}
					ret = true;
				}
				else if (iSrcY == iDstY) {
					//Cung hang
					if (iSrcX > iDstX) {
						for (int x = iDstX + 1; x < iSrcX; x++) {
							if (board[iSrcY][x] != 0)
								return false;
						}
					}
					else if (iSrcX < iDstX) {
						for (int x = iSrcX + 1; x < iDstX; x++) {
							if (board[iSrcY][x] != 0)
								return false;
						}
					}
					ret = true;
				}
				else {
					return false;
				}
			}
			else if (co_up_isRedPiece(board[iSrcY][iSrcX])
				!= co_up_isRedPiece(board[iDstY][iDstX])) {
				if (iSrcX == iDstX) {
					//Cung cot
					int count = 0;
					if (iSrcY > iDstY) {
						for (int y = iDstY + 1; y < iSrcY; y++) {
							if (board[y][iSrcX] != 0)
								count++;
							if (count > 1)
								return false;
						}
					}
					else if (iSrcY < iDstY) {
						for (int y = iSrcY + 1; y < iDstY; y++) {
							if (board[y][iSrcX] != 0)
								count++;
							if (count > 1)
								return false;
						}
					}
					if (count == 1)
						ret = true;
				}
				else if (iSrcY == iDstY) {
					//Cung hang
					int count = 0;
					if (iSrcX > iDstX) {
						for (int x = iDstX + 1; x < iSrcX; x++) {
							if (board[iSrcY][x] != 0)
								count++;
							if (count > 1)
								return false;
						}
					}
					else if (iSrcX < iDstX) {
						for (int x = iSrcX + 1; x < iDstX; x++) {
							if (board[iSrcY][x] != 0)
								count++;
							if (count > 1)
								return false;
						}
					}
					if (count == 1)
						ret = true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
			break;
		case 6:
			//ma do
		case 13:
			//ma den
			if (board[iDstY][iDstX] == 0
				|| co_up_isRedPiece(board[iSrcY][iSrcX])
				!= co_up_isRedPiece(board[iDstY][iDstX])) {
				if (abs(iDstX - iSrcX) == 1) {
					if (iDstY - iSrcY == 2 && board[iSrcY + 1][iSrcX] == 0) {
						ret = true;
					}
					else if (iDstY - iSrcY == -2
						&& board[iSrcY - 1][iSrcX] == 0) {
						ret = true;
					}
				}
				else if (abs(iDstY - iSrcY) == 1) {
					if (iDstX - iSrcX == 2 && board[iSrcY][iSrcX + 1] == 0) {
						ret = true;
					}
					else if (iDstX - iSrcX == -2
						&& board[iSrcY][iSrcX - 1] == 0) {
						ret = true;
					}
				}
			}
			break;
		case 7:
			//tot do
			if (board[iDstY][iDstX] == 0
				|| co_up_isRedPiece(board[iSrcY][iSrcX])
				!= co_up_isRedPiece(board[iDstY][iDstX])) {
				//Diem toi trong hoac co quan khac mau
				if (iDstY - iSrcY == 1 && iSrcX == iDstX) {
					//Di len phia truoc 1 o
					ret = true;
				}
				else {
					if (iSrcY >= 5) {
						//Tot sang song
						if (iSrcY == iDstY) {
							if (abs(iDstX - iSrcX) == 1)
								ret = true;
						}
					}
				}
			}
			break;
		case 14:
			//tot den
			if (board[iDstY][iDstX] == 0
				|| co_up_isRedPiece(board[iSrcY][iSrcX])
				!= co_up_isRedPiece(board[iDstY][iDstX])) {
				//Diem toi trong hoac co quan khac mau
				if (iDstY - iSrcY == -1 && iSrcX == iDstX) {
					//Di len phia truoc 1 o
					ret = true;
				}
				else {
					if (iSrcY <= 4) {
						//Tot sang song
						if (iSrcY == iDstY) {
							if (abs(iDstX - iSrcX) == 1)
								ret = true;
						}
					}
				}
			}
			break;
		}
		if (ret && isNeedCheck) {
			//Neu hop le, can phai kiem tra xem nuoc di do co gay chieu tuong ko
			int nextBoard[10][9];
			co_up_getBoard(Board, iSrcX, iSrcY, iDstX, iDstY, nextBoard);
			ret = !co_up_isCheck(nextBoard, !co_up_isRedPiece(Board[iSrcY][iSrcX]));
		}
		return ret;
	}

	DLL_EXPORT bool co_up_findGeneralPiece(int Board[10][9], bool isRedTurn, int* retX, int* retY) {
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

	DLL_EXPORT bool co_up_isCheck(int Board[10][9], bool isRedTurn) {
		int X = 0;
		int Y = 0;
		if (!co_up_findGeneralPiece(Board, isRedTurn, &X, &Y)) {
			//Khong tim thay quan tuong, error
			return false;
		}
		for (int x = 0; x < 9; x++)
			for (int y = 0; y < 10; y++) {
				if (Board[y][x] != 0 && co_up_isRedPiece(Board[y][x]) == isRedTurn) {
					if (co_up_isLegalMove(Board, x, y, X, Y, false)) {
						return true;
					}
				}
			}
		return false;
	}

	DLL_EXPORT void co_up_getBoard(int Board[10][9], int srcX, int srcY, int dstX, int dstY,
		int nextBoard[10][9]) {
		for (int x = 0; x < 9; x++)
			for (int y = 0; y < 10; y++)
				nextBoard[y][x] = Board[y][x];
		nextBoard[dstY][dstX] = abs(Board[srcY][srcX]);
		nextBoard[srcY][srcX] = 0;
	}
}