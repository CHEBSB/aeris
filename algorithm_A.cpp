#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"
using namespace std;

class stack {
private:
	pair<int, int> Array[20];
	int size;
public:
	stack() : size(0) {}
	void push(const int r, const int c) {
		// search if already in the stack. If so, don't push.
		for (int i = 0; i < size; i++)
			if (Array[i].first == r && Array[i].second == c) 
				return;
		Array[size++] = pair<int, int>(r, c);
	}
	void pop() { Array[--size].~pair<int, int>(); }
	bool Empty() { return (size == 0); }
	pair<int, int> Top() { return Array[size - 1]; }
};

class cell {	// a cell on the board
public:
	int cap;	// how many orbs at most it can contain
	int size;	// how many orbs are in that cell
	int col;	// 1 for mine, 0 for oppenent's, -1 for empty
	cell& operator=(const cell& another)
	{
		cap = another.cap;
		size = another.size;
		col = another.col;
		return (*this);
	}
};

class node {	// one stage of the board
private:
	stack ToExp;		// cell in this stack are going to explode
	stack ToCheck;		// cell in this stack need to be check
	bool whoseTurn;		// 1 for my turn, 0 for my oppenent's
	void check(const int r, const int c) {
		if (B[r][c].size >= B[r][c].cap)
			ToExp.push(r, c);
	}
	void Explode(const int r, const int c) {
		B[r][c].size -= B[r][c].cap;
		if (r - 1 >= 0) {
			B[r - 1][c].col = B[r][c].col;
			B[r - 1][c].size++;
			ToCheck.push(r - 1, c);
		}
		if (r + 1 < 5) {
			B[r + 1][c].col = B[r][c].col;
			B[r + 1][c].size++;
			ToCheck.push(r + 1, c);
		}
		if (c - 1 >= 0) {
			B[r][c - 1].col = B[r][c].col;
			B[r][c - 1].size++;
			ToCheck.push(r, c - 1);
		}
		if (c + 1 < 6) {
			B[r][c + 1].col = B[r][c].col;
			B[r][c + 1].size++;
			ToCheck.push(r, c + 1);
		}
	}
	void AfterInsert(const int r, const int c) {
		check(r, c);
		while (!(ToExp.Empty() && ToCheck.Empty())) {
			while (!ToExp.Empty()) {
				Explode(ToExp.Top().first, ToExp.Top().second);
				ToExp.pop();
			}
			while (!ToCheck.Empty()) {
				check(ToCheck.Top().first, ToCheck.Top().second);
				ToCheck.pop();
			}
		}
	}
public:
	node() :whoseTurn(1) {}
	cell B[5][6];	// the board
	node* child[5][6];	// 1-step-after possibilities
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
			for (int j = 0; j < 6; j++)
				if (B[i][j].col == whoseTurn || B[i][j].size == 0)
					child[i][j] = new node(this, i, j);
				else
					child[i][j] = NULL;
	}
	int countColor(int color)
	{	// count # of orb of that color on the board
		int count = 0;
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 6; j++)
				if (B[i][j].col == color)	count += B[i][j].size;
		return count;
	}
};

void algorithm_A(Board board, Player player, int index[]) {
	int i, j;			// looping index
	int color = player.get_color();
	bool candidate[5][6] = { 0 };	// cell suitable to place orb
	node* Current = new node;	// current board

	for (i = 0; i < 5; i++)		// init Current
		for (j = 0; j < 6; j++) {
			Current->B[i][j].cap = board.get_capacity(i, j);
			Current->B[i][j].size = board.get_orbs_num(i, j);
			if (board.get_orbs_num(i, j) == 0)
				Current->B[i][j].col = -1;
			else if (board.get_cell_color(i, j) == color)
				Current->B[i][j].col = 1;
			else
				Current->B[i][j].col = 0;
		}
	Current->initChild();	// init 1st generation child
	for (i = 0; i < 5; i++)	// if any step can win, take it
		for (j = 0; j < 6; j++)
			if (Current->child[i][j] != NULL)
				if (Current->child[i][j]->countColor(0) == 0) {
					index[0] = i;
					index[1] = j;
					return;			// win
				}
	for (i = 0; i < 5; i++)		// init 2nd generation child
		for (j = 0; j < 6; j++)
			if (Current->child[i][j] != NULL)
				Current->child[i][j]->initChild();
	int worstCase[5][6];	/* worst case of number of my orbs
	of each 1st-generation child, counting their derivative 2nd-generation child */
	int BWC = 0;	// Best of the worst cases
	for (i = 0; i < 5; i++)	// init worstCase
		for (j = 0; j < 6; j++)
			worstCase[i][j] = 220;
	for (i = 0; i < 5; i++)	// count worst Case
		for (j = 0; j < 6; j++)
			if (Current->child[i][j] != NULL) {
				for (int m = 0; m < 5; m++)
					for (int n = 0; n < 6; n++)
						if (Current->child[i][j]->child[m][n] != NULL)
							if (Current->child[i][j]->child[m][n]->countColor(1) < worstCase[i][j])
								worstCase[i][j] = Current->child[i][j]->child[m][n]->countColor(1);
			}
	for (i = 0; i < 5; i++)	// choose best of worst case
		for (j = 0; j < 6; j++)
			if (worstCase[i][j] >= BWC && worstCase[i][j] < 200) {
				if (worstCase[i][j] > BWC) {
					for (int m = 0; m < 5; m++)	// clear candidate array
						for (int n = 0; n < 6; n++)
							candidate[m][n] = 0;
					BWC = worstCase[i][j];
				}
				candidate[i][j] = 1;
			}
	srand(time(NULL));
	while (1) {
		i = rand() % 5;
		j = rand() % 6;
		if (candidate[i][j] == 1) break;
	}
	index[0] = i;
	index[1] = j;
}