#include <fstream>
#include <iostream>
using namespace std;

class robot;
class cell {
	friend class robot;
public:
	cell(bool don, int dist) : done(don), 
	distance(dist), pseudoTraversed(0) {}
private:
	bool done;	// has it been clean?
	int distan;	// minimum distance(step) to R
	bool* wayBackHome;	/* if Bot want to go back to R 
	with shortest path, whichdirection may it take? 
	0, 1, 2, 3 for up, down, left, right */
	bool pseudoTraversed;
	// to be Added...
} 

class robot {
public:
	robot();
	void pseudoTraverse() { pseudoTraverse(Rrow, Rcol); }/* before 
	Bot start traverse the floor, gather information of floor */
private:
	cell** map;		// the informataion of area to traverse
	int bLife;		// battery lifetime
	int bCurrent;	// current energy in battery
	enum {wayHome, working} state;	/* whether the bot is on 
						its way home, or to clean more area */
	int row, col;		// current position of bot
	int m, n;			// array size
	int Rrow, Rcol;		// position of R
	void afterMove();	// after robot move a step, call this function
	void pseudoTraverse(const int, const int);	// work horse
};

int main()
{
	ofstream output;		
	robot Bot;
	Bot.pseudoTraverse();


	delete Bot;
	return 0;
}

robot::robot() {
	ifstream data;
	int i, j;	// looping index
	char c;		// 1 char from file

	data.open("floor.data", ios::in);
	data >> m >> n >> bLife;	// read first line
	bCurrent = bLife;	// fully charge battery
	state = working;	// default state
	map = new cell*[m];	// memory allocate
	for (i = 0; i < m; i++)
		map[i] = new cell[n];

	/* read the following lines to
	get information of the area */
	for (i = 0; i < m; i++)	
		for (j = 0; j < n; j++) {
			data >> c;
			if (c == '0') {	// need to clean
				map[i][j] = cell(false, 1000000);
				map[i][j].wayBackHome = new bool[4];
			} else if (c == '1') {	// obstacle
				map[i][j] = cell(true, -1);
			} else if (c == 'R') {
				map[i][j] = cell(true, 0);
				row = Rrow = i;			// initialize bot' s position
				col = Rcol = j;
			}	else throw "Error";
		}
	data.close();
}
void robot::afterMove()
{
	bCurrent--;		// consume energy
	if (map[row][col].done == 0)	
		map[row][col].done = 1;	// clean this part
	if (row == Rrow && col == Rcol)	bCurrent = bLife;
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
				up.wayBackHome[i] = false;	// reset wayBckHome
			up.wayBackHome[0] = true;		// rewrite wayBackHome
			if (!up.pseudoTraversed)	// unvisited
				pseudoTraverse(r - 1, c);
		} else if (up.distan == (cur.distan + 1)) {
			up.wayBackHome[0] = true;	// add option into wayBackHome
		}
	}
	// check downward
	if (r + 1 < m) {
		cell& down = map[r + 1][c];
		if (down.distan > (cur.distan + 1)) {
			down.distan = cur.distan + 1;		// rewrite distan
			for (int i = 0; i < 4; i++)
				down.wayBackHome[i] = false;	// reset wayBckHome
			down.wayBackHome[1] = true;		// rewrite wayBackHome
			if (!down.pseudoTraversed)	// unvisited
				pseudoTraverse(r + 1, c);
		} else if (down.distan == (cur.distan + 1)) {
			down.wayBackHome[1] = true;	// add option into wayBackHome
		}
	}
	// check leftward
	if (c - 1 >= 0) {
		cell& left = map[r][c - 1];
		if (left.distan > (cur.distan + 1)) {
			left.distan = cur.distan + 1;		// rewrite distan
			for (int i = 0; i < 4; i++)
				left.wayBackHome[i] = false;	// reset wayBckHome
			left.wayBackHome[2] = true;		// rewrite wayBackHome
			if (!left.pseudoTraversed)	// unvisited
				pseudoTraverse(r, c - 1);
		} else if (left.distan == (cur.distan + 1)) {
			left.wayBackHome[2] = true;	// add option into wayBackHome
		}
	// check rightward
	if (c + 1 < n) {
		cell& right = map[r][c + 1];
		if (right.distan > (cur.distan + 1)) {
			right.distan = cur.distan + 1;		// rewrite distan
			for (int i = 0; i < 4; i++)
				right.wayBackHome[i] = false;	// reset wayBckHome
			right.wayBackHome[3] = true;		// rewrite wayBackHome
			if (!right.pseudoTraversed) // unvisited
				pseudoTraverse(r, c + 1);
		} else if (right.distan == (cur.distan + 1)) {
			right.wayBackHome[3] = true;	// add option into wayBackHome
		}
	}
	}
}