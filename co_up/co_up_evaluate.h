/** evaluate the advantage in the board */
int evaluate(int board[10][9], bool isRed);

int calculateMaterialAdvantage(int board[10][9], bool isRed);

int calculateMobilityAdvantage(int board[10][9]);

int calculateThreat(int board[10][9]);

int calculateProtected(int board[10][9]);

bool isRedColor(int piece);
