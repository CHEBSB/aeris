#include <iostream>
#include <fstream>
using namespace std;

typedef struct {	/* a 2-element struct to indicate
			a certain position in a matrix */
	int row;
	int col;
} MatPos;

class tetris {
public:
	tetris(char *tetrisType, int Col);
	int rowTran(int index); /* given a index to an cell,
	transform its row in tetris into row in gameMat*/
	int colTran(int index); // similar as the above one
	void Fall(int gameMatRow); /* when a new tetris is 
		created, let it fall until hit something*/
private:
	int cornerCol;		/* column (horizontal position)
	of the lower left corner in the gameMatrix */
	int cornerRow;		/* current row (vertical position) 
	of the lower left corner in the gamaMatrix */
	MatPos geoshape[4];	/* indicate the 4 positions 
	that has a square cell */
};

bool **gameMat; /* the playing matrix;
	value "true" for occupied space */

void elimination(int, int);	/* check if any row in gameMat 
	are full of cells and can be eliminated */

bool gg(int NumColGame);	/*after elimination, check 
if any cell outside playing matrix -> game over!*/ 

int main()
{
	int gameMatRow, gameMatCol;	/* number of 
			rows and cols for gameMat */
	char tetrisT[3];	// for tetris constructor
	int tetrisCol;

	// I/O files
	ofstream finalOP;	
	ifstream data;

	int i, j;	// looping index

	// reading in size from test case
	data.open("tetris.data.txt", ios::in);
	

	// allocate memory for gameMatrix
	gameMat = new *bool[gameMatRow + 4];
	for (i = 0; i < gameMatRow + 4; i++)
		gameMat[i] = new bool[gameMatCol];
	// 4 extra rows for tetris outside the game scene

	// init gameMat with all false
	for (i = 0; i < gameMatRow + 4; i++)
		for (j = 0; j < gameMatCol; j++)
			gameMat[i][j] = false;

	// Game start. Read in tetris in sequence

	// OutPut the final result of gameMatrix
	for (i = 4; i < gameMatRow + 4; i++) {
		for (j = 0; j < gameMatCol; j++)
			cout << gameMat[i][j];
		cout << endl;
	}

	// finally, delete memory
	for (i = 0; i < gameMatRow + 4; i++)
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

int tetris::rowTran(int index)
{
	return (cornerRow - (3 - geoshape[index].row));
}
int tetris::colTran(int index)
{
	return (geoshape[index].col + cornerCol);
}	

void tetris::Fall(int NumRowGame) { 
// NumRowGame is the number of rows in gameMat
	bool flag = true;    // check if it can move downward
	while(flag) {
		for (int i = 0; i < 4; i++) {
			if (gameMat[rowTran(i) + 1][(rowTran(i))] 
			|| rowTran(i) + 1 == NumRowGame) 
			/* if the downstair is occupied, 
			 * or it already hit the ground, */
				flag = false;  
				// stop falling
		}
		if (flag) cornerRow--;	
	}
}

void elimination(int NumRowGame, int NumColGame)
{
	bool flag;	// true: need to be eliminated
	int i, j, k;	// looping index

	for (i = 0; i < NumRowGame; i++) {	
	// check each row
		flag = true;
		for (j = 0; j < NumColGame; j++) 
		// check each element
			if (gameMat[i][j] == false)	
			// if any element is false -> can't be eliminated
				flag = false;	
		if (flag) {	// to eliminate a row
			for (j = i; j > 0; j--) 	
			// all rows above it move downward
				// gameMat[k] = gameMat[k - 1]
				for (int k = 0; k < NumColGame; k++) 
					gameMat[j][k] = gameMat[j - 1][k];
			for (j = 0; j < NumColGame; j++)
			// fill the first row with false
				game[0][j] = false;	
		}
	}
}

bool gg(int NumColGame)	// returning true means "game over" 
{
	// check all elements outside the playing matrix
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < NumGameCol; j++)
			if (gameMat[i][j]) return true;
	return false;
}
