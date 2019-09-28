#include <iostream>

using namespace std;

typedef struct {	/* a 2-element struct to indicate
			a certain position in a matrix */
	int row;
	int col;
} MatrixPos;

class tetris {
public:
	tetris(char *tetrisType, int Col, int GameMatrixeRow);
	*MatrixPos getPos();	// locate 4 cells in the gameMatrix
private:
	int cornerCol;		/* column (horizontal position)
	of the lower left corner in the gameMatrix */
	int cornerRow;		/* current row (vertical position) 
	of the lower left corner in the gamaMatrix */
	MatrixPos geoshape[4];	/* indicate the 4 positions 
	that has a square cell */
};

void Fall(tetris& newCome);	/* whenever there is a new
tetris coming from above, let it fall until hit something */

int main()
{
	bool **gameMatrix;	// bool type: 1 for occupied space
	int gameMatrixRow, gameMatrixCol;

	// reading in size from test case

	// allocate memory for gameMatrix
	gameMatrix = new *bool[gameMatrixRow];
	for (int i = 0; i < gameMatrixRow; i++)
		gameMatrix[i] = new bool[gameMatrixCol];

	// Game start. Read in tetris in sequence

	// OutPut the final result of gameMatrix
	for (int i = 0; i < gameMatrixRow; i++) {
		for (int j = 0; j < gameMatrixCol; j++)
			cout << gameMatrix[i][j] << ' ';
		cout << endl;
	}

	// finally, delete memory
	for (int i = 0; i < gameMatrixRow; i++)
		delete [] gameMatrix[i];
	delete gameMatrix;
	return 0;
}

tetris::tetris(char *tetrisType, int Col, int GameMatrixRow)
: cornerCol(Col), cornerRow(GameMatrixRow - 1) {
	if (tetrisType == "T1") {
		geoshape[0] = MatrixPos{2, 0};
		geoshape[1] = MatrixPos{2, 1};
		geoshape[2] = MatrixPos{2, 2};
		geoshape[3] = MatrixPos{3, 1};
	} else if (tetrisType == "T2") {
		geoshape[0] = MatrixPos{1, 1};
		geoshape[1] = MatrixPos{2, 0};
		geoshape[2] = MatrixPos{2, 1};
		geoshape[3] = MatrixPos{3, 1};
	} else if (tetrisType == "T3") {
		geoshape[0] = MatrixPos{2, 1};
		geoshape[1] = MatrixPos{3, 0};
		geoshape[2] = MatrixPos{3, 1};
		geoshape[3] = MatrixPos{3, 2};
	} else if (tetrisType == "T4") {
		geoshape[0] = MatrixPos{1, 0};
		geoshape[1] = MatrixPos{2, 0};
		geoshape[2] = MatrixPos{2, 1};
		geoshape[3] = MatrixPos{3, 0};
	} else if (tetrisType == "L1") {
		geoshape[0] = MatrixPos{1, 0};
		geoshape[1] = MatrixPos{2, 0};
		geoshape[2] = MatrixPos{3, 0};
		geoshape[3] = MatrixPos{3, 1};
	} else if (tetrisType == "L2") {
		geoshape[0] = MatrixPos{2, 0};
		geoshape[1] = MatrixPos{2, 1};
		geoshape[2] = MatrixPos{2, 2};
		geoshape[3] = MatrixPos{3, 0};
	} else if (tetrisType == "L3") {
		geoshape[0] = MatrixPos{1, 0};
		geoshape[1] = MatrixPos{1, 1};
		geoshape[2] = MatrixPos{2, 1};
		geoshape[3] = MatrixPos{3, 1};
	} else if (tetrisType == "L4") {
		geoshape[0] = MatrixPos{2, 2};
		geoshape[1] = MatrixPos{3, 0};
		geoshape[2] = MatrixPos{3, 1};
		geoshape[3] = MatrixPos{3, 2};
	} else if (tetrisType == "J1") {
		geoshape[0] = MatrixPos{1, 1};
		geoshape[1] = MatrixPos{2, 1};
		geoshape[2] = MatrixPos{3, 0};
		geoshape[3] = MatrixPos{3, 1};
	} else if (tetrisType == "J2") {
		geoshape[0] = MatrixPos{2, 0};
		geoshape[1] = MatrixPos{3, 0};
		geoshape[2] = MatrixPos{3, 1};
		geoshape[3] = MatrixPos{3, 2};
	} else if (tetrisType == "J3") {
		geoshape[0] = MatrixPos{1, 0};
		geoshape[1] = MatrixPos{1, 1};
		geoshape[2] = MatrixPos{2, 0};
		geoshape[3] = MatrisPos{3, 0};
	} else if (tetrisType == "J4") {
		geoshape[0] = MatrixPos{2, 0};
		geoshape[1] = MatrixPos{2, 1};
		geoshape[2] = MatrixPos{2, 2};
		geoshape[3] = MatrixPos{3, 2};
	} else if (tetrisType == "S1") {
		geoshape[0] = MatrixPos{2, 1};
		geoshape[1] = MatrixPos{2, 2};
		geoshape[2] = MatrixPos{3, 0};
		geoshape[3] = MatrixPos{3, 1};
	} else if (tetrisType == "S2") {
		geoshape[0] = MatrixPos{1, 0};
		geoshape[1] = MatrixPos{2, 0};
		geoshape[2] = MatrixPos{2, 1};
		geoshape[3] = MatrixPos{3, 1};
	} else if (tetrisType == "Z1") {
		geoshape[0] = MatrixPos{2, 0};
		geoshape[1] = MatrixPos{2, 1};
		geoshape[2] = MatrixPos{3, 1};
		geoshape[3] = MatrixPos{3, 2};
	} else if (tetrisType == "Z2") {
		geoshape[0] = MatrixPos{1, 1};
		geoshape[1] = MatrixPos{2, 0};
		geoshape[2] = MatrixPos{2, 1};
		geoshape[3] = MatrixPos{3, 0};
	} else if (tetrisType == "I1") {
		geoshape[0] = MatrixPos{0, 0};
		geoshape[1] = MatrixPos{1, 0};
		geoshape[2] = MatrixPos{2, 0};
		geoshape[3] = MatrixPos{3, 0};
	} else if (tetrisType == "I2") {
		geoshape[0] = MatrixPos{3, 0};
		geoshape[1] = MatrixPos{3, 1};
		geoshape[2] = MatrisPos{3, 2};
		geoshape[3] = MatrixPos{3, 3};
	} else if (tetrisType == "O") {
		geoshape[0] = MatrixPos{2, 0};
		geoshape[1] = MatrixPos{2, 1};
		geoshape[2] = MatrixPos{3, 0};
		geoshape[3] = MatrixPos{3, 1};
	} 		
}

*MatrixPos tetris::getPos() 
{
	MatrixPos PosInGame[4];

	for (int i = 0; i < 4; i++) {
		PosInGame[i].row = (3 - geoshape[i].row) + cornerRow;
		PosInGame[i].col = geoshape[i].col + cornerCol;
	}	
	return PosInGame;
}
