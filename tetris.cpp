#include <iostream>

using namespace std;

Typedef struct {	/* a 2-element struct to indicate
			a certain position in a matrix */
	int row;
	int col;
} MatrixPos;

class tetris {
public:
	tetris(char *tetrisType, int initCol, int NumOfGameRow);
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

	// reading size from test case

	// allocate memory for gameMatrix
	gameMatrix = new *bool[gameMatrixRow];
	for (int i = 0; i < gameMatrixRow)
		gameMatrix[i] = new bool[gameMatrixCol];
	

	// finally, delete memory
	for (int i = 0; i < gameMatrixRow)
		delete [] gameMatrix[i];
	delete gameMatrix;
	return void;
}
