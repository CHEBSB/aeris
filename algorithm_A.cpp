#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"
using namespace std;

void algorithm_A(Board board, Player player, int index[]) {
	cout << "enter my Algorithm\n";
	class stack {
	private:
		pair<int, int> Array[20];
		int size;
		int capa;
	public:
		stack() : capa(20), size(0) {}
		void push(const int r, const int c) {
			// search if already in the queue. If so, don't push.
			for (int i = 0; i < size; i++)
				if (Array[i].first == r && Array[i].second == c)
					return;
			Array[size++] = pair<int, int>(r, c);
		}
		void pop() {
			if (size == 0) throw "Can't pop an empty stack!";
			Array[--size].~pair<int, int>();
		}
		bool Empty() { return (size == 0); }
		pair<int, int> Top() { return Array[size - 1]; }
	};

	class cell {	// a cell on the board
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

	class node {	// one stage of the board
	private:
		stack ToBeExp;	// cell in this queue are going to explode
		stack ToBeCheck;	// cell in this queue are to be check
		bool whoseTurn;	// 1 for my turn, 0 for my oppenent's
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
	int i, j;			// looping index
	int color = player.get_color();
	bool candidate[5][6] = { 0 };	// cell suitable to place orb
	node* Current = new node;	// current board

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
					return;			// win
				}
	for (i = 0; i < 5; i++)		// init 2nd generation child
		for (j = 0; j < 6; j++)
			if (Current->child[i][j] != NULL)
				Current->child[i][j]->initChild();
	int worstCase[5][6];	/* worst case of number of my orbs
	of each 1st-generation child, counting their derivative 2nd-generatnio child */
	int BWC = 0;	// Best of the worst cases
	for (i = 0; i < 5; i++)	// initially fill worstCase array
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
		if (board.get_cell_color(i, j) == color
			|| board.get_orbs_num(i, j) == 0) break;
	}
	index[0] = i;
	index[1] = j;
}