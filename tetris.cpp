#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

class GameMat {
	friend class tetris;
public:
	GameMat() {}	// default constructot	
	void init(string); /* use the first line
		in the file to initialize this object*/
	void elimination();	/* check if any row 
			need to be eliminated */
	bool ggCheck(); /* check if any tetris cell
		is outside the playing scnene */
	void print(ofstream&);
private:
	bool **matrix;
	int rowSize;
	int colSize;
};

class tetris {
public:
	tetris(char *tetrisType, int Col);
	~tetris() {delete [] cellRow; delete [] cellCol;} 
	int rowTran(int); /* given a index to an cell,
	transform its row in tetris into row in gameMat*/
	int colTran(int); // similar to the above one
	void Fall(GameMat&); /* when a new tetris is 
	created, let it fall until hit something*/
private:
	int cornerCol;	/* column (horizontal position)
	of the lower left corner in the gameMatrix */
	int cornerRow;	/* current row (vertical position) 
	of the lower left corner in the gamaMatrix */
	int *cellRow;	
	int *cellCol;	/* indicate the 4 positions that
	has a square cell by its row and col in tetris */
};

void splitLine(string, char [], int &, bool &);  /* split 
a line from file into 2 parts: tetrisType and tetrisCol */

int main()
{ 
	tetris *Tetr;
	char tetrT[3];	// for tetris constructor
	int tetrCol;
	ofstream finalOP;	// I file	
	ifstream data;		// O file
	string line; 	// to store a line read from file
	int i, j;	// looping index
	bool EoG = false;	// check if game is over
	GameMat PlayMat;

	// reading in size from test case
	data.open("tetris.data.txt", ios::in);
	if (!data) {
		cout << "Cannot open tetris.data.txt!\n";
		return 1;
	} 
	getline(data, line);
	PlayMat.init(line); 
	// 4 extra rows for tetris outside the game scene
	// Game start. Read in tetris in sequence
	while ((!EoG) && (!data.eof())) {
		getline(data, line);
		splitLine(line, tetrT, tetrCol, EoG);
		if (!EoG) {
			cout << "Type: ";
			for (i = 0; i < 3; i++) cout << tetrT[i];
			cout << "  Col: "<< tetrCol << endl;
		} else cout << "GG!";
		
		if (!EoG) {
			Tetr = new tetris(tetrT, tetrCol);
			Tetr->Fall(PlayMat);	
			PlayMat.elimination();
			EoG = PlayMat.ggCheck();	
			delete Tetr;				
		}	
	}
	data.close();
	// OutPut the final result of gameMatrix
	PlayMat.print(finalOP);
	PlayMat.~GameMat();
	return 0;
}
tetris::tetris(char *tetrisType, int Col)
: cornerCol(Col), cornerRow(3) {	/* initialize 
lower-left corner at row 0 and given Col in the gameMat */
	// then, read tetrisType to determine its type
	if (tetrisType[0] == 'T') {
		if (tetrisType[1] == '1') {
			cellRow = new int[4]{2, 2, 2, 3};
			cellCol = new int[4]{0, 1, 2, 1};
		} else if (tetrisType[1] == '2') {
			cellRow = new int[4]{1, 2, 2, 3};
			cellCol = new int[4]{1, 0, 1, 1};
		} else if (tetrisType[1] == '3') {
			cellRow = new int[4]{2, 3, 3, 3};
			cellCol = new int[4]{1, 0, 1, 2};
		} else if (tetrisType[1] == '4') {
			cellRow = new int[4]{1, 2, 2, 3};
			cellCol = new int[4]{0, 0, 1, 0};
		} else throw "No Such Type!";
	} else if (tetrisType[0] == 'L') {
		if (tetrisType[1] == '1') {
			cellRow = new int[4]{1, 2, 3, 3};
			cellCol = new int[4]{0, 0, 0, 1};
		} else if (tetrisType[1] == '2') {
			cellRow = new int[4]{2, 2, 2, 3};
			cellCol = new int[4]{0, 1, 2, 0};
		} else if (tetrisType[1] == '3') {
			cellRow = new int[4]{1, 1, 2, 3};
			cellCol = new int[4]{0, 1, 1, 1};
		} else if (tetrisType[1] == '4') {
			cellRow = new int[4]{2, 3, 3, 3};
			cellCol = new int[4]{2, 0, 1, 2};
		} else throw "No Such Type!";
	} else if (tetrisType[0] == 'J') {
		if (tetrisType[1] == '1') {
			cellRow = new int[4]{1, 2, 3, 3};
			cellCol = new int[4]{1, 1, 0, 1};
		} else if (tetrisType[1] == '2') {
			cellRow = new int[4]{2, 3, 3, 3};
			cellCol = new int[4]{0, 0, 1, 2};
		} else if (tetrisType[1] == '3') {
			cellRow = new int[4]{1, 1, 2, 3};
			cellCol = new int[4]{0, 1, 0, 0};
		} else if (tetrisType[1] == '4') {
			cellRow = new int[4]{2, 2, 2, 3};
			cellCol = new int[4]{0, 1, 2, 2};	
		} else throw "No Such Type!"; 
	} else if (tetrisType[0] == 'S') {
		if (tetrisType[1] == '1') {
			cellRow = new int[4]{2, 2, 3, 3};
			cellCol = new int[4]{1, 2, 0, 1};
		} else if (tetrisType[1] == '2') {
			cellRow = new int[4]{1, 2, 2, 3};
			cellCol = new int[4]{0, 0, 1, 1};
		} else throw "No Such Type!";	
	} else if (tetrisType[0] == 'Z') { 
		if (tetrisType[1] == '1') {
			cellRow = new int[4]{2, 2, 3, 3};
			cellCol = new int[4]{0, 1, 1, 2};
		} else if (tetrisType[1] == '2') {
			cellRow = new int[4]{1, 2, 2, 3};
			cellCol = new int[4]{1, 0, 1, 0};
		} else throw "No Such Type!";	
	} else if (tetrisType[0] == 'I') {
		if (tetrisType[1] == '1') {
			cellRow = new int[4]{0, 1, 2, 3};
			cellCol = new int[4]{0, 0, 0, 0};
		} else if (tetrisType[1] == '2') {
			cellRow = new int[4]{3, 3, 3, 3};
			cellCol = new int[4]{0, 1, 2, 3};
		} else throw "No Such Type!";	
	} else if (tetrisType[0] == 'O') {
		if (tetrisType[1] == '\0') {
			cellRow = new int[4]{2, 2, 3, 3};
			cellCol = new int[4]{0, 1, 0, 1};
		} else throw "No Such Type!";
	} else throw "No Such Type!"; 		
}
int tetris::rowTran(int index)
{
	if (index < 4 && index >= 0)
		return (cornerRow + (3 - cellRow[index]));
	throw 404;
}
int tetris::colTran(int index)
{	
	if (index < 4 && index >= 0)
		return (cornerCol + cellCol[index]);
	throw 404;
}	
void tetris::Fall(GameMat &Mat) { 
// NumRowGame is the number of rows in gameMat
	bool flag = true;    
	// check if it can continue moving downward
	int i;			// looping index
	
	while (flag) {
		for (i = 0; i < 4; i++) {
			if (rowTran(i) >= Mat.rowSize || colTran(i) >= Mat.colSize)
				throw 'E';
			if (rowTran(i) + 1 == Mat.rowSize) 
				flag = false;  // stop falling
			else if (Mat.matrix[rowTran(i) + 1][colTran(i)] == true)
				flag = false;
		}	
		if (flag) cornerRow++;		
	}
	for (i = 0; i < 4; i++)
		Mat.matrix[rowTran(i)][colTran(i)] = true;
	// finally, change the value inside gameMat
}
void GameMat::print(ofstream &finalOP)
{
	finalOP.open("tetris.final.txt", ios::out);
	if (!finalOP) {
		cout << "Cannnot Open tetris.final!";
		return;
	}
	for (int i = 4; i < rowSize; i++) {
		for (int j = 0; j < colSize; j++)
			finalOP << matrix[i][j];
		finalOP << endl;
	}	
	finalOP.close();
}
void GameMat::elimination()
{
	bool flag;	// true for need to be eliminated
	int i, j, k;	// looping index

	for (i = 0; i < rowSize; i++) {	
	// check each row
		flag = true;
		for (j = 0; j < colSize; j++) 
		// check each element
			if (matrix[i][j] == false) /* any
		element is false -> can't be eliminated */
				flag = false;	
		if (flag) {	// to eliminate a row
			for (j = i; j > 0; j--) 	
			// all rows above it move downward
				for (int k = 0; k < colSize; k++) 
					matrix[j][k] = matrix[j - 1][k];
			for (j = 0; j < colSize; j++)
			// fill the first row with false
				matrix[0][j] = false;	
		}
	}
}
bool GameMat::ggCheck()	// returning true means "game over" 
{	// check all elements outside the playing matrix
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < colSize; j++)
			if (matrix[i][j]) return true;
	return false;
}
void GameMat::init(string lin)
{ 
	int i, j;	// looping index 
	char row[3];	// to store 1st number in input
	char col[3];	// to store 2nd number
	
	for (i = 0; lin[i] != ' '; i++); /* i become the
			index of the first whitespace*/  
	for (j = 0; j < i && j < 2; j++)	
		row[j] = lin[j];
	for (; lin[i] == ' '; i++);	/* i become 
		the first char of the second number*/
	for (j = 0; lin[i] != '\0' && j < 3; i++) // copy 2nd number 
		col[j++] = lin[i];
	rowSize = atoi(row) + 4;  	/* +4 for 
		rows outside playing scene */
	colSize = atoi(col);
	cout << "rowSize: " << rowSize << " colSize: " << colSize << endl;
	matrix = new bool*[rowSize];	// memory allocate
	for (i = 0; i < rowSize; i++)
		matrix[i] = new bool[colSize];
	// init the matrix with all elements to be false
	for (i = 0; i < rowSize; i++)  
		for (j = 0; j < colSize; j++)
			matrix[i][j] = false;
}
void splitLine(string lin,char type[], int &Col, bool &End)
{	
	int i, j, k; // index
	char col[3];
			
	if (lin[0] == 'E' && lin[1] == 'n' && lin[2] == 'd') {
		End = true;
		return;
	}
	for (i = 0; lin[i] != ' '; i++); /* i become 
		the index of the first whitespace */ 
	for (j = 0; j < i && j < 2; j++)	
		type[j] = lin[j];
	type[j] = '\0'; // store the first part into type
	for (; lin[i] == ' '; i++);	/* i become 
			the start of the second part */
	// store the second part into col
	for (k = 0; lin[i] != '\0' && k < 3; i++) 
		col[k++] = lin[i];
	Col = atoi(col) - 1; /* transfer col to integer
	, and decre it before assign 
	(for array index,1 of common sense,
	means [0] in programming language) */
}
