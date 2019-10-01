#include <iostream>

using namespace std;

typedef struct {	/* a 2-element struct to indicate
			a certain position in a matrix */
	int row;
	int col;
} MatPos;

class tetris {
public:
	tetris(char *tetrisType, int Col);
	*MatPos getPos();	// locate 4 cells in the gameMatrix
private:
	int cornerCol;		/* column (horizontal position)
	of the lower left corner in the gameMatrix */
	int cornerRow;		/* current row (vertical position) 
	of the lower left corner in the gamaMatrix */
	MatPos geoshape[4];	/* indicate the 4 positions 
	that has a square cell */
};

void Fall(tetris& newCome, int gameMatRow);	/* whenever there
is a new tetris coming from above, let it fall until hit something */

int main()
{
	bool **gameMat;	// bool type: 1 for occupied space
	int gameMatRow, gameMatCol;
	char *tetrisT;	// for tetris constructor
	int tetrisCol;

	// reading in size from test case

	// allocate memory for gameMatrix
	gameMat = new *bool[gameMatRow + 4];
	for (int i = 0; i < gameMatRow + 4; i++)
		gameMat[i] = new bool[gameMatCol];
	// 4 extra rows for tetris outside the game scene

	// Game start. Read in tetris in sequence

	// OutPut the final result of gameMatrix
	for (int i = 4; i < gameMatRow + 4; i++) {
		for (int j = 0; j < gameMatCol; j++)
			cout << gameMat[i][j] << ' ';
		cout << endl;
	}

	// finally, delete memory
	for (int i = 0; i < gameMatRow + 4; i++)
		delete [] gameMat[i];
	delete gameMat;
	return 0;
}

tetris::tetris(char *tetrisType, int Col)
: cornerCol(Col), cornerRow(3) {	
	/* initially, lower -left corner is at 
	   the row 0 and the given Col in the gameMatrix */
	if (tetrisType == "T1") {
		geoshape[0] = MatPos{2, 0};
		geoshape[1] = MatPos{2, 1};
		geoshape[2] = MatPos{2, 2};
		geoshape[3] = MatPos{3, 1};
	} else if (tetrisType == "T2") {
		geoshape[0] = MatPos{1, 1};
		geoshape[1] = MatPos{2, 0};
		geoshape[2] = MatPos{2, 1};
		geoshape[3] = MatPos{3, 1};
	} else if (tetrisType == "T3") {
		geoshape[0] = MatPos{2, 1};
		geoshape[1] = MatPos{3, 0};
		geoshape[2] = MatPos{3, 1};
		geoshape[3] = MatPos{3, 2};
	} else if (tetrisType == "T4") {
		geoshape[0] = MatPos{1, 0};
		geoshape[1] = MatPos{2, 0};
		geoshape[2] = MatPos{2, 1};
		geoshape[3] = MatPos{3, 0};
	} else if (tetrisType == "L1") {
		geoshape[0] = MatPos{1, 0};
		geoshape[1] = MatPos{2, 0};
		geoshape[2] = MatPos{3, 0};
		geoshape[3] = MatPos{3, 1};
	} else if (tetrisType == "L2") {
		geoshape[0] = MatPos{2, 0};
		geoshape[1] = MatPos{2, 1};
		geoshape[2] = MatPos{2, 2};
		geoshape[3] = MatPos{3, 0};
	} else if (tetrisType == "L3") {
		geoshape[0] = MatPos{1, 0};
		geoshape[1] = MatPos{1, 1};
		geoshape[2] = MatPos{2, 1};
		geoshape[3] = MatPos{3, 1};
	} else if (tetrisType == "L4") {
		geoshape[0] = MatPos{2, 2};
		geoshape[1] = MatPos{3, 0};
		geoshape[2] = MatPos{3, 1};
		geoshape[3] = MatPos{3, 2};
	} else if (tetrisType == "J1") {
		geoshape[0] = MatPos{1, 1};
		geoshape[1] = MatPos{2, 1};
		geoshape[2] = MatPos{3, 0};
		geoshape[3] = MatPos{3, 1};
	} else if (tetrisType == "J2") {
		geoshape[0] = MatPos{2, 0};
		geoshape[1] = MatPos{3, 0};
		geoshape[2] = MatPos{3, 1};
		geoshape[3] = MatPos{3, 2};
	} else if (tetrisType == "J3") {
		geoshape[0] = MatPos{1, 0};
		geoshape[1] = MatPos{1, 1};
		geoshape[2] = MatPos{2, 0};
		geoshape[3] = MatPos{3, 0};
	} else if (tetrisType == "J4") {
		geoshape[0] = MatPos{2, 0};
		geoshape[1] = MatPos{2, 1};
		geoshape[2] = MatPos{2, 2};
		geoshape[3] = MatPos{3, 2};
	} else if (tetrisType == "S1") {
		geoshape[0] = MatPos{2, 1};
		geoshape[1] = MatPos{2, 2};
		geoshape[2] = MatPos{3, 0};
		geoshape[3] = MatPos{3, 1};
	} else if (tetrisType == "S2") {
		geoshape[0] = MatPos{1, 0};
		geoshape[1] = MatPos{2, 0};
		geoshape[2] = MatPos{2, 1};
		geoshape[3] = MatPos{3, 1};
	} else if (tetrisType == "Z1") {
		geoshape[0] = MatPos{2, 0};
		geoshape[1] = MatPos{2, 1};
		geoshape[2] = MatPos{3, 1};
		geoshape[3] = MatPos{3, 2};
	} else if (tetrisType == "Z2") {
		geoshape[0] = MatPos{1, 1};
		geoshape[1] = MatPos{2, 0};
		geoshape[2] = MatPos{2, 1};
		geoshape[3] = MatPos{3, 0};
	} else if (tetrisType == "I1") {
		geoshape[0] = MatPos{0, 0};
		geoshape[1] = MatPos{1, 0};
		geoshape[2] = MatPos{2, 0};
		geoshape[3] = MatPos{3, 0};
	} else if (tetrisType == "I2") {
		geoshape[0] = MatPos{3, 0};
		geoshape[1] = MatPos{3, 1};
		geoshape[2] = MatPos{3, 2};
		geoshape[3] = MatPos{3, 3};
	} else if (tetrisType == "O") {
		geoshape[0] = MatPos{2, 0};
		geoshape[1] = MatPos{2, 1};
		geoshape[2] = MatPos{3, 0};
		geoshape[3] = MatPos{3, 1};
	} else throw "Error! No such tetris type exists!"; 		
}

*MatPos tetris::getPos() 
{
	MatPos PosInGame[4];

	for (int i = 0; i < 4; i++) {
		PosInGame[i].row = cornerRow - (3 - geoshape[i].row);
		PosInGame[i].col = geoshape[i].col + cornerCo1;
	}	
	return PosInGame;
}

void Fall(tetris& newCome, int gameMatRow) {
	
}
