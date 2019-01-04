# ChineseChessOfflineWindowsCore

g++ -shared -fpic ChineseChessOfflineWindowsCore/base/* ChineseChessOfflineWindowsCore/co_up/* ChineseChessOfflineWindowsCore/eleeye/* ChineseChessOfflineWindowsCore/main.cpp -w -o libChineseChess.so
<p>
	<b>- Co tuong:</b>
  struct NuocCo<br/>
	{
		int iSrcCol, iSrcRow, iDstCol, iDstRow;
	};
  // Nghi nuoc di quan do
  // lngLimitTime (ms)
	NuocCo letComputerThinkForHint(int Board[10][9], int iDepth, long lngLimitTime);
  // Nghi nuoc di quan den
	NuocCo letComputerThink(int Board[10][9], int iDepth, long lngLimitTime);
  // Kiem tra neu nuoc di hop le
	bool isLegalMove(int Board[10][9], int iSrcCol, int iSrcRow, int iDstCol, int iDstRow);
  // Kiem tra het co
	bool isGameFinished(int Board[10][9], bool *isBlackWin);
  // Kiem tra ban co hop le
  // bCompFirst: may di truoc khong
	bool isBoardValid(int board[10][9], bool bCompFirst);
  // Lay danh sach nuoc di hop le
  // Return: so luong nuoc di hop le
  // iLegalCol: mang luu nhung nuoc di hop le
	int getAllLegalMoves(int Board[10][9], int iTestCol, int iTestRow, int *iLegalCol, int *iLegalRow);
  // Ngung nghi
	bool quitGame();
	// QuyBKA
  // Tim vi tri quan vua
	bool findGeneralPiece(int Board[10][9], bool isRedTurn, int* retX, int* retY);
  // Kiem tra xem co phai quan do khong
	bool isRedPiece(int piece);
  // Kiem tra chieu tuong
	bool isCheck(int Board[10][9], bool isRedTurn);
</p>  
  
- Co up:
  // Nghi nuoc di quan den
  NuocCo co_up_letComputerThink(int Board[10][9], int iDepth, long lngLimitTime);
  // Nghi nuoc di quan do
	NuocCo co_up_letComputerThinkForHint(int Board[10][9], int iDepth, long lngLimitTime);
  // Lay danh sach nuoc di hople
	int co_up_getAllLegalMoves(int Board[10][9], int iTestCol, int iTestRow,	int *iLegalCol, int *iLegalRow);
  // Kiem tra het co
	bool co_up_isGameFinished(int Board[10][9], bool isRedTurn, bool* isBlackWin);
  // Kiem tra nuoc di hop le
	bool co_up_isLegalMove(int Board[10][9], int iSrcX, int iSrcY, int iDstX, int iDstY, bool isNeedCheck);
  // Kiem tra neu la quan do
	bool co_up_isRedPiece(int piece);
  // Tinh toan ban co sau khi di
	void co_up_getBoard(int Board[10][9], int srcX, int srcY, int dstX, int dstY, int nextBoard[10][9]);
	bool co_up_isCheck(int Board[10][9], bool isRedTurn);
	bool co_up_findGeneralPiece(int Board[10][9], bool isRedTurn, int* retX, int* retY);
	int co_up_generateAllLegalMoves(int Board[10][9], bool isRedTurn, int *iLegalSrcX, int *iLegalSrcY, int *iLegalDstX, int *iLegalDstY);
