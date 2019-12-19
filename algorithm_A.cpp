#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"
#define Need(i, j) (board.get_capacity(i, j) - board.get_orbs_num(i, j))
using namespace std;

/******************************************************
 * In your algorithm, you can just use the the funcitons
 * listed by TA to get the board information.(functions
 * 1. ~ 4. are listed in next block)
 *
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 *
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/


void algorithm_A(Board board, Player player, int index[]) {
	// cout << board.get_capacity(0, 0) << endl;
	// cout << board.get_orbs_num(0, 0) << endl;
	// cout << board.get_cell_color(0, 0) << endl;
	// board.print_current_board(0, 0, 0);

	class stack {
	private:
		pair<int, int>* Array;
		int size;
		int capa;
	public:
		stack() : capa(20), size(0)
		{
			Array = new pair<int, int>[20];
		}
		void push(const int r, const int c) {
			// search if already in the queue. If so, don't push.
			for (int i = 0; i < size; i++)
				if (Array[i].first == r && Array[i].second == c)
					return;
			Array[size++] = pair<int, int>(r, c);
		}
		void pop() {
			if (Empty()) throw "Can't pop an empty stack!";
			Array[--size].~pair<int, int>();
		}
		pair<int, int> Top() { return Array[size - 1]; }
		bool Empty() { return (size == 0); }
	};
	//////////// Random Algorithm ////////////
	// Here is the random algorithm for your reference, you can delete or comment it.
	srand(time(NULL));
	class cell {
	public:
		cell() {}
		int cap;	// how many orbs at most it can contain
		int size;	// how many orbs are in that cell
		int col;	// 1 for mine, 0 for oppenent's, -1 for blank
		cell& operator=(const cell& another)
		{
			cap = another.cap;
			size = another.size;
			col = another.col;
			return (*this);
		}
	};

	class node {
	public:
		node() :whoseTurn(1) {}
		stack ToBeExp;	// cell in this queue are going to explode
		stack ToBeCheck;	// cell in this queue are to be check
		bool whoseTurn;	// 1 for my turn, 0 for my oppenent's
		cell B[5][6];	// the board
		node* child[5][6];	// 1-step-after possibilities
		void AfterInsert(const int r, const int c) {
			int i, j;
			check(r, c);
			while (!(ToBeExp.Empty() && ToBeCheck.Empty())) {
				while (!ToBeExp.Empty()) {
					i = ToBeExp.Top().first;
					j = ToBeExp.Top().second;
					Explode(i, j);
					if (i - 1 >= 0) ToBeCheck.push(i - 1, j);
					if (i + 1 < 5) ToBeCheck.push(i + 1, j);
					if (j - 1 >= 0) ToBeCheck.push(i, j - 1);
					if (j + 1 < 6) ToBeCheck.push(i, j + 1);
					ToBeExp.pop();
				}
				while (!ToBeCheck.Empty()) {
					check(ToBeCheck.Top().first, ToBeCheck.Top().second);
					ToBeCheck.pop();
				}
			}
		}
		void check(const int r, const int c) {
			if (B[r][c].size >= B[r][c].cap)
				ToBeExp.push(r, c);
		}
		void Explode(const int r, const int c) {
			B[r][c].size -= B[r][c].cap;
			if (r - 1 >= 0) {
				B[r - 1][c].col = B[r][c].col;
				B[r - 1][c].size++;
			}
			if (r + 1 < 5) {
				B[r + 1][c].col = B[r][c].col;
				B[r + 1][c].size++;
			}
			if (c - 1 >= 0) {
				B[r][c - 1].col = B[r][c].col;
				B[r][c - 1].size++;
			}
			if (c + 1 < 6) {
				B[r][c + 1].col = B[r][c].col;
				B[r][c + 1].size++;
			}
		}
		node(node* Parent, int r, int c)
		{	// constructor using its parent
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 6; j++)
					B[i][j] = Parent->B[i][j];
			whoseTurn = !(Parent->whoseTurn);
			B[r][c].size++;
			B[r][c].col = Parent->whoseTurn;
			AfterInsert(r, c);
		}
		void initChild()
		{	// initialize all its children
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 6; j++) {
					if (B[i][j].col == whoseTurn || B[i][j].size == 0)
						child[i][j] = new node(this, i, j);
					else
						child[i][j] = NULL;
				}
		}
		int countColor(int color)
		{
			int count = 0;
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 6; j++)
					if (B[i][j].col == color)
						count += B[i][j].size;
			return count;
		}
	};

	srand(time(NULL));
	int i, j;
	int color = player.get_color();
	bool candidate[5][6] = { 0 };

	node* Current = new node;
	for (i = 0; i < 5; i++)		// init Current
		for (j = 0; j < 6; j++) {
			Current->B[i][j].cap = board.get_capacity(i, j);
			Current->B[i][j].size = board.get_orbs_num(i, j);
			if (board.get_orbs_num(i, j) != 0 && board.get_cell_color(i, j) == color)
				Current->B[i][j].col = 1;
			else if (board.get_orbs_num(i, j) != 0 && board.get_cell_color(i, j) != color)
				Current->B[i][j].col = 0;
			else
				Current->B[i][j].col = -1;
		}
	Current->initChild();	// init 1st generation child
	for (i = 0; i < 5; i++)	// if any step can win, take it
		for (j = 0; j < 6; j++)
			if (Current->child[i][j] != NULL)
				if (Current->child[i][j]->countColor(0) == 0) {
					index[0] = i;
					index[1] = j;
					cout << "Get!\n";
					return;
				}
	for (i = 0; i < 5; i++)		// init 2nd generation child
		for (j = 0; j < 6; j++)
			if (Current->child[i][j] != NULL)
				Current->child[i][j]->initChild();

	int worstCase[5][6];
	for (i = 0; i < 5; i++)	// initially fill worstCase array
		for (j = 0; j < 6; j++)
			worstCase[i][j] = 220;
	for (i = 0; i < 5; i++)	// count worst Case of each 1-generation child
		for (j = 0; j < 6; j++)
			if (Current->child[i][j] != NULL) {
				for (int m = 0; m < 5; m++)
					for (int n = 0; n < 6; n++)
						if (Current->child[i][j]->child[m][n] != NULL)
							if (Current->child[i][j]->child[m][n]->countColor(1) < worstCase[i][j])
								worstCase[i][j] = Current->child[i][j]->child[m][n]->countColor(1);
			}
	int BWC = 0;	// Best of the worst case
	for (i = 0; i < 5; i++)
		for (j = 0; j < 6; j++) {
			if (worstCase[i][j] >= BWC && worstCase[i][j] < 200) {
				if (worstCase[i][j] > BWC) {
					for (int i = 0; i < 5; i++)	// clear candidate array
						for (int j = 0; j < 6; j++)
							candidate[i][j] = 0;
					BWC = worstCase[i][j];
				}
				candidate[i][j] = 1;
			}
		}

	int NumCandidate = 0;
	for (i = 0; i < 5; i++)
		for (j = 0; j < 6; j++)
			if (candidate[i][j] == 1) NumCandidate++;

	for (i = 0; i < 5; i++)		// remove those cell slower than opponent
		for (j = 0; j < 6 && NumCandidate > 1; j++) {
			if (candidate[i][j] == 1) {
				if (i - 1 >= 0) {				// check up
					if (board.get_orbs_num(i - 1, j) != 0
						&& board.get_cell_color(i - 1, j) != color
						&& Need(i - 1, j) < Need(i, j)) {
						candidate[i][j] = 0;
						NumCandidate--;
					}
				}
				if (i + 1 < 5) {				// check down
					if (board.get_orbs_num(i + 1, j) != 0
						&& board.get_cell_color(i + 1, j) != color
						&& Need(i + 1, j) < Need(i, j)) {
						candidate[i][j] = 0;
						NumCandidate--;
					}
				}
				if (j - 1 >= 0) { 				// check left
					if (board.get_orbs_num(i, j - 1) != 0
						&& board.get_cell_color(i, j - 1) != color
						&& Need(i, j - 1) < Need(i, j)) {
						candidate[i][j] = 0;
						NumCandidate--;
					}
				}
				if (j - 1 >= 0) {				// check right
					if (board.get_orbs_num(i, j + 1) != 0
						&& board.get_cell_color(i, j + 1) != color
						&& Need(i, j + 1) < Need(i, j)) {
						candidate[i][j] = 0;
						NumCandidate--;
					}
				}
			}
		}

	// choose the one with most nearby blocks //
	int MaxCap = 1;
	for (i = 0; i < 5; i++)
		for (j = 0; j < 6; j++) {
			if (candidate[i][j] == 1) {
				if (board.get_capacity(i, j) > MaxCap) {
					MaxCap = board.get_capacity(i, j);
				}
				else if (board.get_capacity(i, j) < MaxCap) {
					candidate[i][j] = 0;
				}
			}
		}
	cout << endl << "possible position: ";
	for (i = 0; i < 5; i++)
		for (j = 0; j < 6; j++)
			if (candidate[i][j] == 1)
				cout << '(' << i << ", " << j << ") ";
	cout << endl;

	if (NumCandidate != 0) {
		while (1) {
			i = rand() % 5;
			j = rand() % 6;
			if (candidate[i][j] == 1) break;
		}
	}
	else {
		while (1) {
			i = rand() % 5;
			j = rand() % 6;
			if (board.get_cell_color(i, j) == color || board.get_orbs_num(i, j) == 0) break;
		}
	}
	index[0] = i;
	index[1] = j;
}