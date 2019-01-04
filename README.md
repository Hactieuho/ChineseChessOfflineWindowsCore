# ChineseChessOfflineWindowsCore

g++ -shared -fpic ChineseChessOfflineWindowsCore/base/* ChineseChessOfflineWindowsCore/co_up/* ChineseChessOfflineWindowsCore/eleeye/* ChineseChessOfflineWindowsCore/main.cpp -w -o libChineseChess.so
<p>
	<b>- Co tuong:</b><br/>
  struct NuocCo<br/>
	{<br/>
		int iSrcCol, iSrcRow, iDstCol, iDstRow;<br/>
	};<br/>
  // Nghi nuoc di quan do<br/>
  // lngLimitTime (ms)<br/>
	NuocCo letComputerThinkForHint(int Board[10][9], int iDepth, long lngLimitTime);<br/>
  // Nghi nuoc di quan den<br/>
	NuocCo letComputerThink(int Board[10][9], int iDepth, long lngLimitTime);<br/>
  // Kiem tra neu nuoc di hop le<br/>
	bool isLegalMove(int Board[10][9], int iSrcCol, int iSrcRow, int iDstCol, int iDstRow);<br/>
  // Kiem tra het co<br/>
	bool isGameFinished(int Board[10][9], bool *isBlackWin);<br/>
  // Kiem tra ban co hop le<br/>
  // bCompFirst: may di truoc khong<br/>
	bool isBoardValid(int board[10][9], bool bCompFirst);<br/>
  // Lay danh sach nuoc di hop le<br/>
  // Return: so luong nuoc di hop le<br/>
  // iLegalCol: mang luu nhung nuoc di hop le<br/>
	int getAllLegalMoves(int Board[10][9], int iTestCol, int iTestRow, int *iLegalCol, int *iLegalRow);<br/>
  // Ngung nghi<br/>
	bool quitGame();<br/>
	// QuyBKA<br/>
  // Tim vi tri quan vua<br/>
	bool findGeneralPiece(int Board[10][9], bool isRedTurn, int* retX, int* retY);<br/>
  // Kiem tra xem co phai quan do khong<br/>
	bool isRedPiece(int piece);<br/>
  // Kiem tra chieu tuong<br/>
	bool isCheck(int Board[10][9], bool isRedTurn);<br/>
</p>  <br/>
  <p>
**- Co up:**<br/>
  // Nghi nuoc di quan den<br/>
  NuocCo co_up_letComputerThink(int Board[10][9], int iDepth, long lngLimitTime);<br/>
  // Nghi nuoc di quan do<br/>
	NuocCo co_up_letComputerThinkForHint(int Board[10][9], int iDepth, long lngLimitTime);<br/>
  // Lay danh sach nuoc di hople<br/>
	int co_up_getAllLegalMoves(int Board[10][9], int iTestCol, int iTestRow,	int *iLegalCol, int *iLegalRow);<br/>
  // Kiem tra het co<br/>
	bool co_up_isGameFinished(int Board[10][9], bool isRedTurn, bool* isBlackWin);<br/>
  // Kiem tra nuoc di hop le<br/>
	bool co_up_isLegalMove(int Board[10][9], int iSrcX, int iSrcY, int iDstX, int iDstY, bool isNeedCheck);<br/>
  // Kiem tra neu la quan do<br/>
	bool co_up_isRedPiece(int piece);<br/>
  // Tinh toan ban co sau khi di<br/>
	void co_up_getBoard(int Board[10][9], int srcX, int srcY, int dstX, int dstY, int nextBoard[10][9]);<br/>
	bool co_up_isCheck(int Board[10][9], bool isRedTurn);<br/>
	bool co_up_findGeneralPiece(int Board[10][9], bool isRedTurn, int* retX, int* retY);<br/>
	int co_up_generateAllLegalMoves(int Board[10][9], bool isRedTurn, int *iLegalSrcX, int *iLegalSrcY, int *iLegalDstX, int *iLegalDstY);<br/>
</p>
