#include <fstream>
#include <iostream>
using namespace std;

class robot;
class cell {
	friend class robot;
public:
	cell(bool don, int dist) : done(don), 
	distan(dist), pseudoTraversed(0) {}
	cell() {} // default constructor
private:
	bool done;	// has it been clean?
	int distan;	// minimum distance(step) to R
	bool* wayHome;	/* if Bot want to go back to R 
	with shortest path, whichdirection may it take? 
	0, 1, 2, 3 for up, down, left, right */
	bool pseudoTraversed;
	// to be Added...
}; 

class robot {
public:
	robot();
	void pseudoTraverse() { pseudoTraverse(Rrow, Rcol); } /* before 
	Bot start traverse the floor, gather information of floor */
	void Traversal();	// actual Traversal of the floor
private:
	cell** map;		// the informataion of area to traverse
	int bLife;		// battery lifetime
	int bCurrent;	// current energy in battery
	bool WAYHOME;	/* whether the bot is on 
		its way home, or to clean more area */
	int rowPos, colPos;	// current position of bot
	int RowSize, ColSize;	// array size
	int Rrow, Rcol;		// position of R
	void afterMove();	// after robot move a step, call this function
	void pseudoTraverse(const int, const int);	// work horse
	void Traverse();		// first step of traversal
	void PointWiseTraverse(int, int);	/* find the
	optimized way to clean a particular point */
};

int main()
{
	ofstream output;		
	robot Bot;

	Bot.pseudoTraverse();
	Bot.Traversal();

	return 0;
}

robot::robot() {
	ifstream data;
	int i, j;	// looping index
	char c;		// 1 char from file

	data.open("floor.data", ios::in);
	data >> RowSize >> ColSize >> bLife;	// read first line
	bCurrent = bLife;	// fully charge battery
	WAYHOME = 0;	// default state
	map = new cell*[RowSize];	// memory allocate
	for (i = 0; i < RowSize; i++)
		map[i] = new cell[ColSize];

	/* read the following lines to
	get information of the area */
	for (i = 0; i < RowSize; i++)	
		for (j = 0; j < ColSize; j++) {
			data >> c;
			if (c == '0') {	// need to clean
				map[i][j] = cell(false, 1000000);
				map[i][j].wayHome = new bool[4];
			} else if (c == '1') {	// obstacle
				map[i][j] = cell(true, -1);
			} else if (c == 'R') {
				map[i][j] = cell(true, 0);
				rowPos = Rrow = i;
				colPos = Rcol = j;
			}	else throw "Error";
		}
	data.close();
}
void robot::afterMove()
{
	bCurrent--;		// consume energy
	if (map[rowPos][colPos].done == 0)	
		map[rowPos][colPos].done = 1;	// clean this part
	if (rowPos == Rrow && colPos == Rcol) {
		WAYHOME = 0;
		bCurrent = bLife;	// recharge
	}
}

void robot::pseudoTraverse(const int r, const int c)
{	// starting at position given by row and col
	cell& cur = map[r][c];	// current position
	if (cur.distan > bLife) throw ("illegal floor!");
	cur.pseudoTraversed = 1;
	// check upward
	if (r - 1 >= 0) {
		cell& up = map[r - 1][c];
		if (up.distan > (cur.distan + 1)) {
			up.distan = cur.distan + 1;		// rewrite distan
			for (int i = 0; i < 4; i++) 
				up.wayHome[i] = false;	// reset wayBckHome
			up.wayHome[0] = true;		// rewrite wayBackHome
			if (!up.pseudoTraversed)	// unvisited
				pseudoTraverse(r - 1, c);
		} else if (up.distan == (cur.distan + 1)) {
			up.wayHome[0] = true;	// add option into wayBackHome
		}
	}
	// check downward
	if (r + 1 < RowSize) {
		cell& down = map[r + 1][c];
		if (down.distan > (cur.distan + 1)) {
			down.distan = cur.distan + 1;		// rewrite distan
			for (int i = 0; i < 4; i++)
				down.wayHome[i] = false;	// reset wayBckHome
			down.wayHome[1] = true;		// rewrite wayBackHome
			if (!down.pseudoTraversed)	// unvisited
				pseudoTraverse(r + 1, c);
		} else if (down.distan == (cur.distan + 1)) {
			down.wayHome[1] = true;	// add option into wayBackHome
		}
	}
	// check leftward
	if (c - 1 >= 0) {
		cell& left = map[r][c - 1];
		if (left.distan > (cur.distan + 1)) {
			left.distan = cur.distan + 1;		// rewrite distan
			for (int i = 0; i < 4; i++)
				left.wayHome[i] = false;	// reset wayBckHome
			left.wayHome[2] = true;		// rewrite wayBackHome
			if (!left.pseudoTraversed)	// unvisited
				pseudoTraverse(r, c - 1);
		} else if (left.distan == (cur.distan + 1)) {
			left.wayHome[2] = true;	// add option into wayBackHome
		}
	}	
	// check rightward
	if (c + 1 < ColSize) {
		cell& right = map[r][c + 1];
		if (right.distan > (cur.distan + 1)) {
			right.distan = cur.distan + 1;		// rewrite distan
			for (int i = 0; i < 4; i++)
				right.wayHome[i] = false;	// reset wayBckHome
			right.wayHome[3] = true;		// rewrite wayBackHome
			if (!right.pseudoTraversed) // unvisited
				pseudoTraverse(r, c + 1);
		} else if (right.distan == (cur.distan + 1)) {
			right.wayHome[3] = true;	// add option into wayBackHome
		}
	}
}

void robot::Traversal()
{
	Traverse();
	for (int i = 0; i < RowSize; i++)
		for (int j = 0; j < ColSize; j++)
			if (!map[i][j].done)
				PointWiseTraverse(i, j);
}

void robot::Traverse()
{
	afterMove();
	if (!WAYHOME) { // still going deeper
		// up
		if (!map[rowPos - 1][colPos].done) {
			rowPos--;
			Traverse();
		}
		// down
		else if (!map[rowPos + 1][colPos].done) {
			rowPos++;
			Traverse();
		}
		// left
		else if (!map[rowPos][colPos - 1].done) {
			colPos--;
			Traverse();
		}
		// right
		else if (!map[rowPos][colPos + 1].done) {
			colPos++;
			Traverse();
		}
		else {
			if (rowPos == Rrow && colPos == Rcol) {
				return;
			} else {
				if (map[rowPos][colPos].wayHome[0]) {
					// go up
					rowPos++;
				} else if (map[rowPos][colPos].wayHome[1]) {
					// go down
					rowPos--;
				} else if (map[rowPos][colPos].wayHome[2]) {
					// go left
					colPos--;
				} else if (map[rowPos][colPos].wayHome[3]) {
					// go right
					colPos++;
				} else throw 404;
			}
		}
	}
	else {		// on its way home
		int bestOption = -1, secChoice = -1;

		// up
		if (map[rowPos][colPos].wayHome[0]) {
			if (!map[rowPos - 1][colPos].done) 
				bestOption = 0;
			else secChoice = 0;
		}
		// down
		if (map[rowPos][colPos].wayHome[1] && bestOption == -1) {
			if (!map[rowPos + 1][colPos].done)
				bestOption = 1;
			else if (secChoice == -1)
				secChoice = 1;
		} 
		// left
		if (map[rowPos][colPos].wayHome[2] && bestOption == -1) {
			if (!map[rowPos][colPos - 1].done)
				bestOption = 2;
			else if (secChoice == -1)
				secChoice = 2;
		}
		// right
		if (map[rowPos][colPos].wayHome[3] && bestOption == -1) {
			if (!map[rowPos][colPos + 1].done)
				bestOption = 3;
			else if (secChoice == -1)
				secChoice = 3;
		}
		
		
		switch (bestOption) {
		case 0: 
		// go up
			break;
		case 1: 
		// go down
			break;
		case 2: 
		// go left
			break;
		case 3: 
		// go right
			break;
		default: 
			switch (secChoice) {
				case 0:
					// go up
					break;
				case 1:
					// go down
					break;
				case 2:
					// go left
					break;
				case 3:
					// go right
					break;			
			}
		}
	}
}
