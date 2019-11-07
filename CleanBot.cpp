#include <fstream>
#include <iostream>
#include <stdlib.h>
using namespace std;

class stack {
public:
	void push(const int);
	void pop() { ~Arr[--size]; }
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
	cell* Cur(const int, const int);
	cell* Cur() { return Cur(rowPos, colPos); }
	cell* Up(const int, const int);
	cell* Up() { return Up(rowPos, colPos); }
	cell* Down(const int, const int);
	cell* Down() { return Down(rowPos, colPos); }
	cell* Left(const int, const int);
	cell* Left() { return Left(rowPos, colPos); }
	cell* Right(const int, const int);
	cell* Right() { return Right(rowPos, colPos); }
	void afterMove();	// after robot move a step, call this function
	int FindWayBack(const int, const int);
	void Traverse(const int);		// first step of traversal
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
			switch (c) {
			case '0': 	// need to clean
				map[i][j] = cell(false, 1000000);
				map[i][j].wayHome = new bool[4];
				break;
			case '1': 	// obstacle
				map[i][j] = cell(true, -1);
				break;
			case 'R': 
				map[i][j] = cell(true, 0);
				rowPos = Rrow = i;
				colPos = Rcol = j;
				break;
			default:
				throw "Error";
			}
		}
	data.close();
}

cell* robot::Cur(const int r, const int c) {
	return &(map[r][c]);
}
cell* robot::Up(const int r, const int c) {
	if (r - 1 >= 0) return &(map[r - 1][c]);
	return NULL;
}
cell* robot::Down(const int r, const int c) {
	if (r + 1 < RowSize) return &(map[r + 1][c]);
	return NULL;
}
cell* robot::Left(const int r, const int c) {
	if (c - 1 >= 0) return &(map[r][c - 1]);
	return NULL;
}
cell* robot::Right(const int r, const int c) {
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
	Traverse(-1);
	for (int i = 0; i < RowSize; i++)
		for (int j = 0; j < ColSize; j++)
			if (!map[i][j].done)
				PointWiseTraverse(i, j);
}

int robot::FindWayBack(const int r, const int c)
{
	int bestOption = -1, secChoice = -1;

	if (Cur(r, c)->wayHome[0]) {
		if (!Up(r, c)->done) bestOption = 0;
		else secChoice = 0;
	} // up
	if (Cur(r, c)->wayHome[1] && bestOption == -1) {
		if (!Down(r, c)->done)	bestOption = 1;
		else if (secChoice == -1)
			secChoice = 1;
	} // down
	if (Cur(r, c)->wayHome[2] && bestOption == -1) {
		if (!Left(r, c)->done)	bestOption = 2;
		else if (secChoice == -1)
			secChoice = 2;
	} // left
	if (Cur(r, c)->wayHome[3] && bestOption == -1) {
		if (!Right(r, c)->done) bestOption = 3;
		else if (secChoice == -1)
			secChoice = 3;
	} // right

	if (bestOption != -1) return bestOption;
	return secChoice;
}

void robot::Traverse(const int from)
{
	switch (from) {
	case 0: rowPos--; 
		break;
	case 1: rowPos++; 
		break;
	case 2: colPos--; 
		break;
	case 3: colPos++; 
	}
	afterMove();
	if (!WAYHOME) { // still going deeper
		if (!Up()->done) 
			Traverse(0);	// go up	
		else if (!Down()->done) 
			Traverse(1);	// go down
		else if (!Left()->done) 
			Traverse(2);	// go left	
		else if (!Right()->done) 
			Traverse(3);	// go right
		else {
			if (rowPos == Rrow && colPos == Rcol) 
				return;
			else {
				if (Cur()->wayHome[0]) 
					Traverse(0);	// go up
				else if (Cur()->wayHome[1]) 
					Traverse(1);	// go down
				else if (Cur()->wayHome[2]) 
					Traverse(2);	// go left
				else if (Cur()->wayHome[3]) 
					Traverse(3);	// go right
				else throw 404;
			}
		}
	}
	else {		// on its way home		
		switch (FindWayBack(rowPos, colPos)) {
		case 0: // go up
			Traverse(0);
			break;
		case 1: // go down
			Traverse(1);
			break;
		case 2: // go left
			Traverse(2);
			break;
		case 3: // go right
			Traverse(3);
		}
	}
}

void robot::PointWiseTraverse(int r, int c)
{
	stack S;

	// find the shortest path from R to this point
	while (r != Rrow || c != Rcol) {
		switch (FindWayBack(r, c)) {
		case 0: // go up
			r--;
			S.push(1);	/* when Bot is heading from R
			to this point, it has to go down */
			break;
		case 1: // go down
			r++;
			S.push(0);
			break;
		case 2: // go left
			c--;
			S.push(3);
			break;
		case 3: // go right
			c++;
			S.push(2);
		}
	}
	// at this moment, Bot should be at R
	for (; !S.Empty(); afterMove(), S.pop()) {
		switch (S.top()) {
		case 0:
			rowPos--;
			break;
		case 1:
			rowPos++;
			break;
		case 2:
			colPos--;
			break;
		case 3:
			colPos++;
		}
	}
	// Now the bot should be at the initial given position 

}
