#include <fstream>
#include <iostream>
#include <stdlib.h>
using namespace std;

class stack {
public:
	void push(const int);
	void pop();
	int top() { return Arr[size - 1]; }
	bool Empty() { return size ? 0 : 1; }
	stack() : size(0), capa(10) { Arr = new int[10]; }
private:
	int* Arr;
	int size;
	int capa;
};

class queue {
public:
	void push(const int);
	void pop();
	int Front() { return Arr[front]; }
	int Rear() { return Arr[rear]; }
	bool Empty() { return size ? 0 : 1; }
	queue() : size(0), capa(10), front(0), rear(0) { Arr = new int[10]; }
private:
	int* Arr;
	int size;
	int capa;
	int front;
	int rear;
};

class robot;
class cell {
	friend class robot;
public:
	cell(bool d, int dist) : done(d), 
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
	robot(ifstream&);
	void Dijkstra();
	void Traversal();	// actual Traversal of the floor
	void print(ofstream&);
	void ForDebug();	/**/
private:
	cell** map;		// the informataion of area to traverse
	int bLife;		// battery lifetime
	int bCurrent;	// current energy in battery
	bool WAYHOME;	/* whether the bot is on 
		its way home, or to clean more area */
	int rowPos, colPos;	// current position of bot
	int RowSize, ColSize;	// array size
	int Rrow, Rcol;		// position of R
	int step;		// total step to traverse the floor
	queue output1;	// 2 queue to store robot' s position
	queue output2;	// for final output
	// current position of the robot
	cell* Cur() { return &(map[rowPos][colPos]); } 
	cell* Up();
	cell* Down();
	cell* Left();
	cell* Right();
	void afterMove();	// after robot move a step, call this function
	int FindWayBack(const int, const int);
	int FindWayOut(const int, const int);
	void Traverse(const int);		// first step of traversal
	void PointWiseTraverse(int, int);	/* find the
	optimized way to clean a particular point */
};

int main()
{
	ofstream output;
	ifstream data;

	data.open("floor.data", ios::in);
	if (!data) {
		cout << "Failed to open Input file";
		return 1;
	}
	robot Bot = robot(data);
	data.close();
	
	Bot.Dijkstra();
	Bot.ForDebug();	/**/
	Bot.Traversal();
	Bot.print(output);
	return 0;
}
void robot::ForDebug() {	/**/
	cout << "Position of R: (" << Rrow << ", " << Rcol << ")\n";
	cout << "Distance of the floor:\n";
	for (int i = 0; i < RowSize; i++) {
		cout << '\t';
		for (int j = 0; j < ColSize; j++) {
			if (map[i][j].done && map[i][j].distan) 
				cout << '#';
			else cout << map[i][j].distan;
		}
		cout << endl;
	}
}
void stack::push(const int val)
{
	if (size == capa) {
		Arr = (int*)realloc(Arr, sizeof(int) * 2 * capa);
		capa = capa * 2;
	}
	Arr[size++] = val;
}
void stack::pop() 
{ 
	if (Empty()) throw "you can't pop an empty stack"; 
	~Arr[--size]; 
}

void queue::push(const int val)
{
	if ((front == rear) && rear)	rear = front = 0;
	if (rear == capa) {
		Arr = (int*)realloc(Arr, sizeof(int) * 2 * capa);
		capa = capa * 2;
	}
	Arr[rear++] = val;
	size++;
}
void queue::pop()
{
	if (Empty())	throw "you can't pop an empty queue";
	~Arr[front++];
	size--;
}

robot::robot(ifstream& data) {
	int i, j;	// looping index
	char c;		// 1 char from file

	data >> RowSize >> ColSize >> bLife;	// read first line
	bCurrent = bLife;	// fully charge battery
	WAYHOME = 0;	// default state
	step = 0;
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
}

cell* robot::Up() {
	if (rowPos - 1 >= 0) return &(map[rowPos - 1][colPos]);
	return NULL;
}
cell* robot::Down() {
	if (rowPos + 1 < RowSize) return &(map[rowPos + 1][colPos]);
	return NULL;
}
cell* robot::Left() {
	if (colPos - 1 >= 0) return &(map[rowPos][colPos - 1]);
	return NULL;
}
cell* robot::Right() {
	if (colPos + 1 < ColSize) return &(map[rowPos][colPos + 1]);
	return NULL;
}

void robot::Dijkstra()
{
	bool s[RowSize][ColSize];

	for (int i = 0; i < RowSize; i++)
		for (int j = 0; j < ColSize; j++) 
			s[i][j] = false;
	if (Rrow - 1 >= 0)
		if (!map[Rrow - 1][Rcol].done) {
			map[Rrow - 1][Rcol].distan = 1;
			map[Rrow - 1][Rcol].wayHome[1] = true;
		}
	if (Rrow + 1 < RowSize)
		if (!map[Rrow + 1][Rcol].done) {
			map[Rrow + 1][Rcol].distan = 1;
			map[Rrow + 1][Rcol].wayHome[0] = true;
		}
	if (Rcol - 1 >= 0)
		if (!map[Rrow][Rcol - 1].done) {
			map[Rrow][Rcol - 1].distan = 1;
			map[Rrow][Rcol - 1].wayHome[3] = true;
		}
	if (Rcol + 1 < ColSize)
		if (!map[Rrow][Rcol + 1].done) {
			map[Rrow][Rcol + 1].distan = 1;
			map[Rrow][Rcol + 1].wayHome[2] = true;
		}
	s[Rrow][Rcol] = true;

	for (int i = 0; i < RowSize; i++)
		for (int j = 0; j < ColSize; j++) {
			if (!map[i][j].done) {	
				int ur, uc, D = 100000;
				// choose(ur, uc)
				for (int m = 0; m < RowSize; m++)
					for (int n = 0; n < ColSize; n++) 
						if (!map[m][n].done && !s[m][n])
							if (map[m][n].distan < D) {
								ur = m; uc = n; 
								D = map[m][n].distan;
							}
				s[ur][uc] = true;

				// w = ur - 1; x = uc;
				if (ur - 1 >= 0) {
					if ((!map[ur - 1][uc].done) && (!s[ur - 1][uc])) {
						if (map[ur][uc].distan + 1 < map[ur - 1][uc].distan) {
							map[ur - 1][uc].distan = map[ur][uc].distan + 1;
							for (int i = 0; i < 4; i++) // reset wayHome
								map[ur - 1][uc].wayHome[i] = false;
							map[ur - 1][uc].wayHome[1] = true;
						}
						else if (map[ur][uc].distan + 1 == map[ur - 1][uc].distan)
							map[ur - 1][uc].wayHome[1] = true;
					}
				}
				// w = ur + 1; x = uc;
				if (ur + 1 < RowSize) {
					if ((!map[ur + 1][uc].done) && (!s[ur + 1][uc])) {
						if (map[ur][uc].distan + 1 < map[ur + 1][uc].distan) {
							map[ur + 1][uc].distan = map[ur][uc].distan + 1;
							for (int i = 0; i < 4; i++) // reset wayHome
								map[ur + 1][uc].wayHome[i] = false;
							map[ur + 1][uc].wayHome[0] = true;
						}
						else if (map[ur][uc].distan + 1 == map[ur + 1][uc].distan)
							map[ur + 1][uc].wayHome[0] = true;
					}
				}
				// w = ur; x = uc - 1;
				if (uc - 1 >= 0) {
					if ((!map[ur][uc - 1].done) && (!s[ur][uc - 1])) {
						if (map[ur][uc].distan + 1 < map[ur][uc - 1].distan) {
							map[ur][uc - 1].distan = map[ur][uc].distan + 1;
							for (int i = 0; i < 4; i++) // reset wayHome
								map[ur][uc - 1].wayHome[i] = false;
							map[ur][uc - 1].wayHome[3] = true;
						}
						else if (map[ur][uc].distan + 1 == map[ur][uc - 1].distan)
							map[ur][uc - 1].wayHome[3] = true;
					}
				}
				// w = ur; x = uc + 1
				if (uc + 1 < ColSize) {
					if ((!map[ur][uc + 1].done) && (!s[ur][uc + 1])) {
						if (map[ur][uc].distan + 1 < map[ur][uc + 1].distan) {
							map[ur][uc + 1].distan = map[ur][uc].distan + 1;
							for (int i = 0; i < 4; i++) // reset wayHome
								map[ur][uc + 1].wayHome[i] = false;
							map[ur][uc + 1].wayHome[2] = true;
						}
						else if (map[ur][uc].distan + 1 == map[ur][uc + 1].distan)
							map[ur][uc + 1].wayHome[2] = true;
					}
				}
			}		
		}
}	

void robot::afterMove()
{
	bCurrent--;		// consume energy
	if (Cur()->done == 0)	
		Cur()->done = 1;	// clean this part
	if (bCurrent <= Cur()->distan + 1)	/* enrgy left
	is not enough for going deeper	*/
		WAYHOME = 1;
	if (rowPos == Rrow && colPos == Rcol) {
		bCurrent = bLife;	// recharge
		WAYHOME = 0;
	}
	output1.push(rowPos);	// store position
	output2.push(colPos);	// for final output
	step++;
}

void robot::Traversal()
{	
	int Direction = FindWayOut(Rrow, Rcol);
	if (Direction != -1) Traverse(Direction);
	else return;	// no area can be clean, End.

	cout << "The world wonders.\n";	/**/
	for (int i = 0; i < RowSize; i++)
		for (int j = 0; j < ColSize; j++)
			if (!map[i][j].done)
				PointWiseTraverse(i, j);
}

void robot::print(ofstream& output)
{
	output.open("final.path", ios::out);
	if (!output) {
		cout << "Cannot open final.path!";
		throw "error";
	}
	output << step << endl;
	cout << "Total steps needed: " << step << endl;	/**/
	output << Rrow << ' ' << Rcol << endl;
	cout << "Position of R: (" << Rrow << ", " << Rcol << ")\n"; /**/
	while (!output1.Empty()) {
		output << output1.Front() << ' ';
		cout << output1.Front() << ' '; /**/
		output << output2.Front() << endl;
		cout << output2.Front() << endl; /**/
		output1.pop();
		output2.pop();
	}
	output.close();
}

int robot::FindWayBack(const int r, const int c)
{
	int secChoice = -1;

	if (map[r][c].wayHome[0]) {
		if (!map[r - 1][c].done) return 0;
		else secChoice = 0;
	} // up
	if (map[r][c].wayHome[1]) {
		if (!map[r + 1][c].done) return 1;
		else if (secChoice == -1)	secChoice = 1;
	} // down
	if (map[r][c].wayHome[2]) {
		if (!map[r][c - 1].done) return 2;
		else if (secChoice == -1)	secChoice = 2;
	} // left
	if (map[r][c].wayHome[3]) {
		if (!map[r][c + 1].done) return 3;
		else if (secChoice == -1)	secChoice = 3;
	} // right

	return secChoice;
}
int robot::FindWayOut(const int r, const int c) 
{
	if (r - 1 >= 0) {
		if (!map[r - 1][c].done) 
			return 0;
	}
	if (r + 1 < RowSize) {
		if (!map[r + 1][c].done) 
			return 1;
	}
	if (c - 1 >= 0) {
		if (!map[r][c - 1].done)
			return 2;
	}
	if (c + 1 < ColSize) {
		if (!map[r][c + 1].done)
			return 3;
	}
	return -1;	// all neighbour cleaned
}
void robot::Traverse(const int from)
{
	switch (from) {
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
		break;
	default: 
		throw 404;
		break;
	}
	afterMove();
	cout << "Current Position: (" << rowPos << ", "<< colPos << ")\n";	/**/
	if (!WAYHOME) { // enough battery, move deeper
		int direction = FindWayOut(rowPos, colPos);
		if (direction != -1) {
			Traverse(direction);
		}
		else {
			if (rowPos == Rrow && colPos == Rcol)
				return;	// turn to stage 2: PointWiseTraverse
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
			break;
		default:
			throw 404;
			break;
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
			break;
		default:
			throw 404;
			break;
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
			break;
		default:
			throw 404;
			break;
		}
	}
	/* Now the bot should be at the initially given position,
	with bCurrent = bLife - (distan of that point) */
	for (; rowPos != Rrow || colPos != Rcol; afterMove()) {
		if (WAYHOME) {	// time to go home
			switch (FindWayBack(rowPos, colPos)) {
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
				break;
			default: 
				throw 404;
				break;
			}
		}
		else {	// keep working
			if (!Up()->done)
				rowPos--;	// go up	
			else if (!Down()->done)
				rowPos++;	// go down
			else if (!Left()->done)
				colPos--;	// go left	
			else if (!Right()->done)
				colPos++;	// go right
			else {
				if (Cur()->wayHome[0])
					rowPos--; // go up
				else if (Cur()->wayHome[1])
					rowPos++; // go down
				else if (Cur()->wayHome[2])
					colPos--; // go left
				else if (Cur()->wayHome[3])
					colPos++; // go right
				else throw 404;				
			}
		}
	}
}
