#include <fstream>
#include <iostream>
#include <stdlib.h>
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
	distan(dist) {}
	cell() {} // default constructor
private:
	bool done;	// has it been clean?
	int distan;	// minimum distance(step) to R
	bool* wayHome;	/* if Bot want to go back to R 
	with shortest path, which direction may it take? 
	0, 1, 2, 3 for up, down, left, right */
	// to be Added...
}; 

class robot {
public:
	robot();
	void Dijkstra();
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
	cell* Cur(int, int);
	cell* Cur() { return Cur(rowPos, colPos); }
	cell* Up(int, int);
	cell* Up() { return Up(rowPos, colPos); }
	cell* Down(int, int);
	cell* Down() { return Down(rowPos, colPos); }
	cell* Left(int, int);
	cell* Left() { return Left(rowPos, colPos); }
	cell* Right(int, int);
	cell* Right() { return Right(rowPos, colPos); }
	void afterMove();	// after robot move a step, call this function
	void Traverse();		// first step of traversal
	void PointWiseTraverse(int, int);	/* find the
	optimized way to clean a particular point */
};

int main()
{
	ofstream output;		
	robot Bot;

	Bot.Dijkstra();
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

cell* robot::Cur(int r, int c) {
	return &(map[r][c]);
}
cell* robot::Up(int r, int c) {
	if (r - 1 >= 0) return &(map[r - 1][c]);
	return NULL;
}
cell* robot::Down(int r, int c) {
	if (r + 1 < RowSize) return &(map[r + 1][c]);
	return NULL;
}
cell* robot::Left(int r, int c) {
	if (c - 1 >= 0) return &(map[r][c - 1]);
	return NULL;
}
cell* robot::Right(int r, int c) {
	if (c + 1 < ColSize) return &(map[r][c + 1]);
	return NULL;
}

void robot::Dijkstra()
{
	bool s[RowSize][ColSize];

	for (int i = 0; i < RowSize; i++)
		for (int j = 0; j < ColSize; j++) 
			s[i][j] = false;
	if (Up(Rrow, Rcol))
		if (!Up(Rrow, Rcol)->done)
			Up(Rrow, Rcol)->distan = 1;
	if (Down(Rrow, Rcol))
		if (!Down(Rrow, Rcol)->done)
			Down(Rrow, Rcol)->distan = 1;
	if (Left(Rrow, Rcol))
		if (!Left(Rrow, Rcol)->done)
			Left(Rrow, Rcol)->distan = 1;
	if (Right(Rrow, Rcol))
		if (!Right(Rrow, Rcol)->done)
			Right(Rrow, Rcol)->distan = 1;
	s[Rrow][Rcol] = true;

	for (int i = 0; i < RowSize; i++)
		for (int j = 0; j < ColSize; j++) {
			if (!map[i][j].done) {	
				int ur, uc, D = 100000;
				// choose(ur, uc)
				for (int m = 0; m < RowSize; m++)
					for (int n = 0; n < ColSize; n++) 
						if (!map[m][n].done && !s[m][m])
							if (map[m][n].distan < D) {
								ur = m; uc = n; 
								D = map[m][n].distan;
							}
				s[ur][uc] = true;

				for (int w = 0; w < RowSize; w++)
					for (int x = 0; x < ColSize; x++) 
						if ((!map[w][x].done) && (!s[w][x])
						&& ((w == ur - 1 && x == uc) 
						|| (w == ur + 1 && x == uc)
						|| (w == ur && x == uc - 1) 
						|| (w == ur && x == uc + 1)))
							map[w][x].distan = map[ur][uc].distan + 1;
			}
			
		}
}	

void robot::afterMove()
{
	bCurrent--;		// consume energy
	if (Cur()->done == 0)	
		Cur()->done = 1;	// clean this part
	if (rowPos == Rrow && colPos == Rcol) {
		WAYHOME = 0;
		bCurrent = bLife;	// recharge
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
		if (!Up()->done) {
			rowPos--;	// go up
			Traverse();
		}
		else if (!Down()->done) {
			rowPos++;	// go down
			Traverse();
		}
		else if (!Left()->done) {
			colPos--;	// go left
			Traverse();
		}
		else if (!Right()->done) {
			colPos++;	// go right
			Traverse();
		}
		else {
			if (rowPos == Rrow && colPos == Rcol) 
				return;
			else {
				if (Cur()->wayHome[0]) {
					rowPos--; // up
					Traverse();
				} else if (Cur()->wayHome[1]) {
					rowPos++; // down
					Traverse();
				} else if (Cur()->wayHome[2]) {
					colPos--; // left
					Traverse();
				} else if (Cur()->wayHome[3]) {
					colPos++; // right
					Traverse();
				} else throw 404;
			}
		}
	}
	else {		// on its way home
		int bestOption = -1, secChoice = -1;

		// up
		if (Cur()->wayHome[0]) {
			if (!Up()->done) bestOption = 0;
			else secChoice = 0;
		}
		// down
		if (Cur()->wayHome[1] && bestOption == -1) {
			if (!Down()->done)	bestOption = 1;
			else if (secChoice == -1)
				secChoice = 1;
		} 
		// left
		if (Cur()->wayHome[2] && bestOption == -1) {
			if (!Left()->done)	bestOption = 2;
			else if (secChoice == -1)
				secChoice = 2;
		}
		// right
		if (Cur()->wayHome[3] && bestOption == -1) {
			if (!Right()->done) bestOption = 3;
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
