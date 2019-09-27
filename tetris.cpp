#include <iostream>

using namespace std;

Typedef struct {	/* a 2-element struct to indicate
					a certain position in a matrix */
	int row;
	int col;
} PosInMatrix;

class tetris {
public:
	tetris(char *tetrisType, int initCol, int NumOfGameRow);
private:
	int cornerCol;				/* column (horizontal position)
	of the lower left corner in the gameMatrix	*/
	int cornerRow;				/* current row (vertical position) 
	of the lower left corner in the gamaMatrix */
	PosInMatrix geoshape[4];	/* indicate the 4 positions 
	that has a square cell */
};

int main()
{
	bool **gameMatrix;	
	int gameMatrixRow, gameMatrixCol;

	// reading size from test case

	// allocate memory for gameMatrix
	gameMatrix = new *bool[gameMatrixRow];
	for (int i = 0; i < gameMatrixRow)
		gameMatrix = new bool[gameMatrixCol];

	return void;
}