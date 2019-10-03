#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

typedef struct {	/* a 2-element struct to indicate
			a certain position in a matrix */
	int row;
	int col;
} MatPos;

class GameMat {
	friend class tetris;
public:
	GameMat(int, int);
	~GameMat();
	void elimination();
	bool ggCheck();
	bool MatVal(int, int);
private:
	bool **matrix;
	int rowSize;
	int colSize;
};

class tetris {
public:
	tetris(char *tetrisType, int Col);
	~tetris() {}
	int rowTran(int index); /* given a index to an cell,
	transform its row in tetris into row in gameMat*/
	int colTran(int index); // similar as the above one
	void Fall(GameMat&); /* when a new tetris is 
		created, let it fall until hit something*/
private:
	int cornerCol;		/* column (horizontal position)
	of the lower left corner in the gameMatrix */
	int cornerRow;		/* current row (vertical position) 
	of the lower left corner in the gamaMatrix */
	MatPos geoshape[4];	/* indicate the 4 positions 
	that has a square cell */
};

void splitFirstLine(string, int &, int &); /* split 
the first line from file into 2 integer*/
void splitLine(string, char [], int &, bool &);  /* split a line 
read from the file into 2 parts: tetrisType and tetrisCol*/

int main()
{
	int Row, Col;	// number of rows and cols for gameMat 
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
	splitFirstLine(line, Row, Col);
	
	// construct PlayMat
	GameMat PlayMat(Row + 4, Col); 
	// 4 extra rows for tetris outside the game scene

	// Game start. Read in tetris in sequence
	while (!EoG) {
		getline(data, line);
		cout << "Line: " << line << endl;	// used for debug
		splitLine(line, tetrisT, tetrisCol, EoG);	
		if (!EoG) {
			Tetr = new tetris(tetrisT, tetrisCol);
			Tetr->Fall(PlayMat);		
			PlayMat.elimination();	
			EoG = PlayMat.ggCheck();		
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
	for (i = 4; i < Row + 4; i++) {
		for (j = 0; j < Col; j++)
			finalOP << PlayMat.MatVal(i, j);
		finalOP << endl;
	}
	finalOP.close();

	PlayMat.~GameMat();
	return 0;
}

tetris::tetris(char *tetrisType, int Col)
: cornerCol(Col), cornerRow(3) {	
	/* initially, lower -left corner is at 
	   the row 0 and the given Col in the gameMatrix */
	if (tetrisType[0] == 'T') {
		if (tetrisType[1] == '1') {
			geoshape[0] = MatPos{2, 0};
			geoshape[1] = MatPos{2, 1};
			geoshape[2] = MatPos{2, 2};
			geoshape[3] = MatPos{3, 1};
		} else if (tetrisType[1] == '2') {
			geoshape[0] = MatPos{1, 1};
			geoshape[1] = MatPos{2, 0};
			geoshape[2] = MatPos{2, 1};
			geoshape[3] = MatPos{3, 1};
		} else if (tetrisType[1] == '3') {
			geoshape[0] = MatPos{2, 1};
			geoshape[1] = MatPos{3, 0};
			geoshape[2] = MatPos{3, 1};
			geoshape[3] = MatPos{3, 2};
		} else if (tetrisType[1] == '4') {
			geoshape[0] = MatPos{1, 0};
			geoshape[1] = MatPos{2, 0};
			geoshape[2] = MatPos{2, 1};
			geoshape[3] = MatPos{3, 0};
		} else throw "No Such Type!";
	} else if (tetrisType[0] == 'L') {
		if (tetrisType[1] == '1') {
			geoshape[0] = MatPos{1, 0};
			geoshape[1] = MatPos{2, 0};
			geoshape[2] = MatPos{3, 0};
			geoshape[3] = MatPos{3, 1};
		} else if (tetrisType[1] == '2') {
			geoshape[0] = MatPos{2, 0};
			geoshape[1] = MatPos{2, 1};
			geoshape[2] = MatPos{2, 2};
			geoshape[3] = MatPos{3, 0};
		} else if (tetrisType[1] == '3') {
			geoshape[0] = MatPos{1, 0};
			geoshape[1] = MatPos{1, 1};
			geoshape[2] = MatPos{2, 1};
			geoshape[3] = MatPos{3, 1};
		} else if (tetrisType[1] == '4') {
			geoshape[0] = MatPos{2, 2};
			geoshape[1] = MatPos{3, 0};
			geoshape[2] = MatPos{3, 1};
			geoshape[3] = MatPos{3, 2};
		} else throw "No Such Type!";
	} else if (tetrisType[0] == 'J') {
		if (tetrisType[1] == '1') {
			geoshape[0] = MatPos{1, 1};
			geoshape[1] = MatPos{2, 1};
			geoshape[2] = MatPos{3, 0};
			geoshape[3] = MatPos{3, 1};
		} else if (tetrisType[1] == '2') {
			geoshape[0] = MatPos{2, 0};
			geoshape[1] = MatPos{3, 0};
			geoshape[2] = MatPos{3, 1};
			geoshape[3] = MatPos{3, 2};
		} else if (tetrisType[1] == '3') {
			geoshape[0] = MatPos{1, 0};
			geoshape[1] = MatPos{1, 1};
			geoshape[2] = MatPos{2, 0};
			geoshape[3] = MatPos{3, 0};
		} else if (tetrisType[1] == '4') {
			geoshape[0] = MatPos{2, 0};
			geoshape[1] = MatPos{2, 1};
			geoshape[2] = MatPos{2, 2};
			geoshape[3] = MatPos{3, 2};	
		} else throw "No Such Type!"; 
	} else if (tetrisType[0] == 'S') {
		if (tetrisType[1] == '1') {
			geoshape[0] = MatPos{2, 1};
			geoshape[1] = MatPos{2, 2};
			geoshape[2] = MatPos{3, 0};
			geoshape[3] = MatPos{3, 1};
		} else if (tetrisType[1] == '2') {
			geoshape[0] = MatPos{1, 0};
			geoshape[1] = MatPos{2, 0};
			geoshape[2] = MatPos{2, 1};
			geoshape[3] = MatPos{3, 1};
		} else throw "No Such Type!";	
	} else if (tetrisType[0] == 'Z') { 
		if (tetrisType[1] == '1') {
			geoshape[0] = MatPos{2, 0};
			geoshape[1] = MatPos{2, 1};
			geoshape[2] = MatPos{3, 1};
			geoshape[3] = MatPos{3, 2};
		} else if (tetrisType[1] == '2') {
			geoshape[0] = MatPos{1, 1};
			geoshape[1] = MatPos{2, 0};
			geoshape[2] = MatPos{2, 1};
			geoshape[3] = MatPos{3, 0};
		} else throw "No Such Type!";	
	} else if (tetrisType[0] == 'I') {
		if (tetrisType[1] == '1') {
			geoshape[0] = MatPos{0, 0};
			geoshape[1] = MatPos{1, 0};
			geoshape[2] = MatPos{2, 0};
			geoshape[3] = MatPos{3, 0};
		} else if (tetrisType[1] == '2') {
			geoshape[0] = MatPos{3, 0};
			geoshape[1] = MatPos{3, 1};
			geoshape[2] = MatPos{3, 2};
			geoshape[3] = MatPos{3, 3};
		} else throw "No Such Type!";	
	} else if (tetrisType[0] == 'O') {
		if (tetrisType[1] == '\0') {
			geoshape[0] = MatPos{2, 0};
			geoshape[1] = MatPos{2, 1};
			geoshape[2] = MatPos{3, 0};
			geoshape[3] = MatPos{3, 1};
		} else throw "No Such Type!";
	} else throw "No Such Type!"; 		
}

int tetris::rowTran(int index)
{
	return (cornerRow - (3 - geoshape[index].row));
}
int tetris::colTran(int index)
{
	return (geoshape[index].col + cornerCol);
}	

void tetris::Fall(GameMat &Mat) { 
// NumRowGame is the number of rows in gameMat
	bool flag = true;    // check if it can move downward
	int i;			// looping index

	while (flag) {
		cout << "loop enter\n";
		for (i = 0; i < 4; i++) {
			if (Mat.matrix[rowTran(i) + 1][(colTran(i))] == true || rowTran(i) + 1 == Mat.rowSize) 
			// if the downstair is occupied, or it already hit the ground, 
				flag = false;  // stop falling
		}
		if (flag) cornerRow--;	
	}

	for (i = 0; i < 4; i++)
		Mat.matrix[rowTran(i)][colTran(i)] = true;
	// finally, change the value inside gameMat
}

GameMat::GameMat(int R, int C): 
rowSize(R), colSize(C) {
	matrix = new bool*[rowSize];
	for (int i = 0; i < rowSize; i++)
		matrix[i] = new bool[colSize];
	for (int i = 0; i < rowSize; i++)
		for (int j = 0; j < colSize; j++)
			matrix[i][j] = false;
}

GameMat::~GameMat()
{
	for (int i = 0; i < rowSize; i++)
		delete [] matrix[i];
	delete matrix;
}

bool GameMat::MatVal(int rowIndex, int colIndex)
{
	return matrix[rowIndex][colIndex];
}

void GameMat::elimination()
{
	bool flag;	// true: need to be eliminated
	int i, j, k;	// looping index

	for (i = 0; i < rowSize; i++) {	
	// check each row
		flag = true;
		for (j = 0; j < colSize; j++) 
		// check each element
			if (matrix[i][j] == false)	
			// if any element is false -> can't be eliminated
				flag = false;	
		if (flag) {	// to eliminate a row
			for (j = i; j > 0; j--) 	
			// all rows above it move downward
				// matrix[k] = matrix[k - 1]
				for (int k = 0; k < colSize; k++) 
					matrix[j][k] = matrix[j - 1][k];
			for (j = 0; j < colSize; j++)
			// fill the first row with false
				matrix[0][j] = false;	
		}
	}
}

bool GameMat::ggCheck()	// returning true means "game over" 
{
	// check all elements outside the playing matrix
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < colSize; j++)
			if (matrix[i][j]) return true;
	return false;
}

void splitFirstLine(string lin, int &RowPt, int &ColPt)
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
	RowPt = atoi(row);
	ColPt = atoi(col);
}

void splitLine(string lin,char type[], int &Col, bool &End)
{	
	cout << "enter function";	// used for debug
	int i, j, k; // index
	char col[3];
			
	if (lin[0] == 'E' && lin[1] == 'n' && lin[2] == 'd') {
		End = true;
		return;
	}

	for (i = 0; lin[i] != ' '; i++);
	// after this, i is the positon of first whitespace

	for (j = 0; j < i; j++) 
		type[j] = lin[j];
	type[j] = '\0';
	// store the first part into type

	for (; lin[i] == ' '; i++);
	// i become the start of the second part
	
	for (k = 0; lin[i] != '\0'; i++) 
		col[k++] = lin[i];
	// store th second part into col

	Col = atoi(col) - 1; /* transfer col to integer
	, and decre it before assign 
	(because for array index,
	when we say 1 of common sense,
	we actually means [0] in programming language) */
}
