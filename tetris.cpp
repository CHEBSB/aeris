#include <iostream>
#include <fstream>
#include <string.h>
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

bool ggCheck(int NumColGame);	/*after elimination, check 
if any cell outside playing matrix -> game over!*/ 

void splitFirstLine(string, int *, int *); /* split 
	the first line from file into 2 integer*/

void splitLine(string, char *, int *, bool *); /* split a line 
read from the file into 2 parts: tetrisType and tetrisCol*/

int main()
{
	int gameMatRow, gameMatCol;	/* number of 
			rows and cols for gameMat */
	tetris *Tetr;
	char tetrisT[3];	// for tetris constructor
	int tetrisCol;

	// I/O files
	ofstream finalOP;	
	ifstream data;

	string line; 	// to store a line read from file
	int i, j;	// looping index
	bool EoG = false;	// check if game is over

	// reading in size from test case
	data.open("tetris.data.txt", ios::in);
	if (!data) {
		cout << "Cannot open tetris.data.txt!\n";
		return 1;
	}
	getline(data, line);
	splitFirstLine(line, &gameMatRow, &gameMatCol);

	// allocate memory for gameMatrix
	gameMat = new bool*[gameMatRow + 4];
	for (i = 0; i < gameMatRow + 4; i++)
		gameMat[i] = new bool[gameMatCol];
	// 4 extra rows for tetris outside the game scene

	// init gameMat with all false
	for (i = 0; i < gameMatRow + 4; i++)
		for (j = 0; j < gameMatCol; j++)
			gameMat[i][j] = false;

	// Game start. Read in tetris in sequence
	while (!EoG) {
		getline(data, line);
		splitLine(line, tetrisT, &tetrisCol, &EoG);
		if (!EoG) {
			Tetr = new tetris(tetrisT, tetrisCol);
			Tetr->Fall(gameMatRow);
			elimination(gameMatRow, gameMatCol);
			EoG = ggCheck(gameMatCol);
			delete Tetr;
		}	
	}
	data.close();

	// OutPut the final result of gameMatrix
	finalOP.open("tetris.final.txt", ios::out);
	if (!finalOP) {
		cout << "Cannot open tetris.final.txt!\n";
		return 1;
	}
	for (i = 4; i < gameMatRow + 4; i++) {
		for (j = 0; j < gameMatCol; j++)
			finalOP << gameMat[i][j];
		finalOP << endl;
	}
	finalOP.close();

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
			if (gameMat[rowTran(i) + 1][(colTran(i))] 
			|| rowTran(i) + 1 == NumRowGame) 
			/* if the downstair is occupied, 
			 * or it already hit the ground, */
				flag = false;  
				// stop falling
		}
		if (flag) cornerRow--;	
	}

	for (int i = 0; i < 4; i++)
		gameMat[rowTran(i)][colTran(i)] = true;
	// finally, change the value inside gameMat
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
				gameMat[0][j] = false;	
		}
	}
}

bool ggCheck(int NumColGame)	// returning true means "game over" 
{
	// check all elements outside the playing matrix
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < NumColGame; j++)
			if (gameMat[i][j]) return true;
	return false;
}

void splitFirstLine(string lin, int *RowPt, int*ColPt)
{ // similar to splitline (I come up with splitline first)
	int i, j, k; 
	char row[3];
	char col[3];
	
	for (i = 0; lin[i] != ' '; i++);
	for (j = 0; j < i; j++)
		row[j] = lin[j];
	for (; lin[i] == ' '; i++);
	for (k = 0; lin[i] != '\0'; i++)
		col[k++] = lin[i];
	*RowPt = atoi(row);
	*ColPt = atoi(col);
}

void splitLine(string lin, char *type, int *ColPt, bool *End)
{
	int i, j, k; // index
	char col[3];

	if (lin[0] == 'E' && lin[1] == 'n' && lin [2] == 'd') {
		*End = true;
		return;
	}	

	for (i = 0; lin[i] != ' '; i++);
	// after this, i is the positon of first whitespace
	
	for (j = 0; j < i; j++) 
		type[j] = lin[j];
	type[j] = '\0';
	// store the first part into type

	for (; lin[i] == ' '; i++);
	// i become teh==he start of the second part
	
	for (k = 0; lin[i] != '\0'; i++) 
		col[k++] = lin[i];
	// store th second part into col

	*ColPt = atoi(col) - 1; /* transfer col to integer
	, and decre it before assign 
	(because for array index,
	when we say 1 of common sense,
	we actually means [0] in programming language) */
}
