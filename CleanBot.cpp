#include <fstream>
#include <iostream>
#include <stdlib.h>
#define Cur 	map[rowPos][colPos]
#define Up 	map[rowPos-1][colPos]
#define Down	map[rowPos+1][colPos]
#define Left 	map[rowPos][colPos-1]
#define Right 	map[rowPos][colPos+1]
using namespace std;

class stack {
public:
	void push(const int);
	void pop() { ~Arr[--size];  }
	int top() { return Arr[size - 1]; }
	bool Empty() { return size ? 0 : 1; }
	stack() : size(0), capa(10) { Arr = new int[10]; }
private:
	int* Arr;
	int size;
	int capa;
};

class robot;
class cell {
	friend class robot;
public:
	cell(bool don, int dist) : done(don), 
	distan(dist), pseudTraver(0) {}
	cell() {} // default constructor
private:
	bool done;	// has it been clean?
	int distan;	// minimum distance(step) to R
	bool* wayHome;	/* if Bot want to go back to R 
	with shortest path, which direction may it take? 
	0, 1, 2, 3 for up, down, left, right */
	bool pseudTraver;
	// to be Added...
}; 

class robot {
public:
	robot();
	void findDistan() { findDistan(Rrow, Rcol); } /* before 
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
	void findDistan(const int, const int);	// work horse
	void Traverse();		// first step of traversal
	void PointWiseTraverse(int, int);	/* find the
	optimized way to clean a particular point */
};

int main()
{
	ofstream output;		
	robot Bot;

	Bot.findDistan();
	Bot.Traversal();

	return 0;
}

void stack::push(const int val)
{
	if (size == capa) {
		Arr = (int*)realloc(Arr, sizeof(int) * 2 * capa);
		capa = capa * 2;
	}
	Arr[size++] = val;
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
	if (Cur.done == 0)	
		Cur.done = 1;	// clean this part
	if (rowPos == Rrow && colPos == Rcol) {
		WAYHOME = 0;
		bCurrent = bLife;	// recharge
	}
}

void robot::findDistan(const int r, const int c)
{	// starting at position given by row and col
	cell& cur = map[r][c];	// current position
	if (cur.distan > bLife) throw ("illegal floor!");
	cur.pseudTraver = 1;
	
	if (r - 1 >= 0) {	// check up
		cell& up = map[r - 1][c];
		if (up.distan > (cur.distan + 1)) {
			up.distan = cur.distan + 1;		// rewrite distan
			for (int i = 0; i < 4; i++) 
				up.wayHome[i] = false;	// reset wayBckHome
			up.wayHome[0] = true;		// rewrite wayBackHome
			if (!up.pseudTraver)	// unvisited
				findDistan(r - 1, c);
		} else if (up.distan == (cur.distan + 1)) {
			up.wayHome[0] = true;	// add option into wayBackHome
		}
	}
	if (r + 1 < RowSize) {	// check down
		cell& down = map[r + 1][c];
		if (down.distan > (cur.distan + 1)) {
			down.distan = cur.distan + 1;		// rewrite distan
			for (int i = 0; i < 4; i++)
				down.wayHome[i] = false;	// reset wayBckHome
			down.wayHome[1] = true;		// rewrite wayBackHome
			if (!down.pseudTraver)	// unvisited
				findDistan(r + 1, c);
		} else if (down.distan == (cur.distan + 1)) {
			down.wayHome[1] = true;	// add option into wayBackHome
		}
	}
	if (c - 1 >= 0) {	// check left
		cell& left = map[r][c - 1];
		if (left.distan > (cur.distan + 1)) {
			left.distan = cur.distan + 1;		// rewrite distan
			for (int i = 0; i < 4; i++)
				left.wayHome[i] = false;	// reset wayBckHome
			left.wayHome[2] = true;		// rewrite wayBackHome
			if (!left.pseudTraver)	// unvisited
				findDistan(r, c - 1);
		} else if (left.distan == (cur.distan + 1)) {
			left.wayHome[2] = true;	// add option into wayBackHome
		}
	}	
	if (c + 1 < ColSize) {	// check right
		cell& right = map[r][c + 1];
		if (right.distan > (cur.distan + 1)) {
			right.distan = cur.distan + 1;		// rewrite distan
			for (int i = 0; i < 4; i++)
				right.wayHome[i] = false;	// reset wayBckHome
			right.wayHome[3] = true;		// rewrite wayBackHome
			if (!right.pseudTraver) // unvisited
				findDistan(r, c + 1);
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
		if (!Up.done) {
			rowPos--;	// go up
			Traverse();
		}
		else if (!Down.done) {
			rowPos++;	// go down
			Traverse();
		}
		else if (!Left.done) {
			colPos--;	// go left
			Traverse();
		}
		else if (!Right.done) {
			colPos++;	// go right
			Traverse();
		}
		else {
			if (rowPos == Rrow && colPos == Rcol) 
				return;
			else {
				if (Cur.wayHome[0]) {
					rowPos--; // up
					Traverse();
				} else if (Cur.wayHome[1]) {
					rowPos++; // down
					Traverse();
				} else if (Cur.wayHome[2]) {
					colPos--; // left
					Traverse();
				} else if (Cur.wayHome[3]) {
					colPos++; // right
					Traverse();
				} else throw 404;
			}
		}
	}
	else {		// on its way home
		int bestOption = -1, secChoice = -1;

		// up
		if (Cur.wayHome[0]) {
			if (!Up.done) bestOption = 0;
			else secChoice = 0;
		}
		// down
		if (Cur.wayHome[1] && bestOption == -1) {
			if (!Down.done)	bestOption = 1;
			else if (secChoice == -1)
				secChoice = 1;
		} 
		// left
		if (Cur.wayHome[2] && bestOption == -1) {
			if (!Left.done)	bestOption = 2;
			else if (secChoice == -1)
				secChoice = 2;
		}
		// right
		if (Cur.wayHome[3] && bestOption == -1) {
			if (!Right.done) bestOption = 3;
			else if (secChoice == -1)
				secChoice = 3;
		}
		
		
		switch (bestOption) {
		case 0: // go up
			rowPos--;
			Traverse();
			break;
		case 1: // go down
			rowPos++;
			Traverse();
			break;
		case 2: // go left
			colPos--;
			Traverse();
			break;
		case 3: // go right
			colPos++;
			Traverse();
			break;
		default: 
			switch (secChoice) {
				case 0:	// go up
					rowPos--;
					Traverse();
					break;
				case 1:	// go down
					rowPos++;
					Traverse();
					break;
				case 2:	// go left
					colPos--;
					Traverse();
					break;
				case 3:	// go right
					colPos++; 
					Traverse();		
			}
		}
	}
}

void robot::PointWiseTraverse(int r, int c)
{
	stack S;

	      		
}
