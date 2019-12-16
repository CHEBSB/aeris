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


void algorithm_B(Board board, Player player, int index[]) {
	class cell {
	public:
		cell() {}
		int cap;
		int size;
		int col;
	};
	class plat {
	public:
		plat() {}
		cell B[5][6];
		int countEnemy(const int myColor) {
			int count = 0;
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 6; j++)
					if (B[i][j].col != myColor)
						count += B[i][j].size;
			return count;
		}
		void checkExp(int i, int j) {
			if (B[i][j].cap == B[i][j].size) {
				B[i][j].size = 0;
				if (i - 1 >= 0) {
					B[i - 1][j].col = B[i][j].col;
					B[i - 1][j].size++;
				}
				if (i + 1 < 5) {
					B[i + 1][j].col = B[i][j].col;
					B[i + 1][j].size++;
				}
				if (j - 1 >= 0) {
					B[i][j - 1].col = B[i][j].col;
					B[i][j - 1].size++;
				}
				if (j + 1 < 6) {
					B[i][j + 1].col = B[i][j].col;
					B[i][j + 1].size++;
				}
				if (i - 1 >= 0) checkExp(i - 1, j);
				if (i + 1 < 5) checkExp(i + 1, j);
				if (j - 1 >= 0) checkExp(i, j - 1);
				if (j + 1 < 6) checkExp(i, j + 1);
			}
		}
	};
	srand(time(NULL));
	int row, col;
	register int i, j;
	int color = player.get_color();
	int candidate[5][6] = { 0 };
	for (i = 0; i < 5; i++)
		for (j = 0; j < 6; j++)
			if (board.get_cell_color(i, j) == color || board.get_orbs_num(i, j) == 0)
				candidate[i][j] = 1;
	for (i = 0; i < 5; i++)		// remove those cell slower than opponent
		for (j = 0; j < 6; j++) {
			if (i - 1 >= 0) {				// check up
				if (board.get_orbs_num(i - 1, j) != 0
					&& board.get_cell_color(i - 1, j) != color
					&& Need(i - 1, j) < Need(i, j)) candidate[i][j] = 0;
			}
			if (i + 1 < 5) {				// check down
				if (board.get_orbs_num(i + 1, j) != 0
					&& board.get_cell_color(i + 1, j) != color
					&& Need(i + 1, j) < Need(i, j)) candidate[i][j] = 0;
			}
			if (j - 1 >= 0) { 				// check left
				if (board.get_orbs_num(i, j - 1) != 0
					&& board.get_cell_color(i, j - 1) != color
					&& Need(i, j - 1) < Need(i, j)) candidate[i][j] = 0;
			}
			if (j - 1 >= 0) {				// check right
				if (board.get_orbs_num(i, j + 1) != 0
					&& board.get_cell_color(i, j + 1) != color
					&& Need(i, j + 1) < Need(i, j)) candidate[i][j] = 0;
			}
		}
	plat Plate;
	plat P[5][6];
	for (i = 0; i < 5; i++)
		for (j = 0; j < 6; j++) {
			Plate.B[i][j].cap = board.get_capacity(i, j);
			Plate.B[i][j].size = board.get_orbs_num(i, j);
			Plate.B[i][j].col = board.get_cell_color(i, j);
		}
	for (int m = 0; m < 5; m++)
		for (int n = 0; n < 6; n++)
			for (i = 0; i < 5; i++)
				for (j = 0; j < 6; j++) {
					P[m][n].B[i][j].cap = board.get_capacity(i, j);
					P[m][n].B[i][j].size = board.get_orbs_num(i, j);
					P[m][n].B[i][j].col = board.get_cell_color(i, j);
				}


	for (i = 0; i < 5; i++)
		for (j = 0; j < 6; j++)
			if (candidate[i][j]) {
				P[i][j].B[i][j].size++;
				P[i][j].checkExp(i, j);
			}
	int leastNum = Plate.countEnemy(color);
	for (i = 0; i < 5; i++)
		for (j = 0; j < 6; j++)
			if (candidate[i][j] == 1) {
				if (P[i][j].countEnemy(color) <= leastNum) {
					if (P[i][j].countEnemy(color) < leastNum) {
						leastNum = P[i][j].countEnemy(color);
						for (int i = 0; i < 5; i++)	// clear candidate array
							for (int j = 0; j < 6; j++)
								candidate[i][j] = 0;
					}
					candidate[i][j] = 1;
				}
			}

	int count = 0;
	cout << endl << "possible position: ";
	for (i = 0; i < 5; i++)
		for (j = 0; j < 6; j++)
			if (candidate[i][j] == 1) {
				cout << '(' << i << ", " << j << ") ";
				count++;
			}
	cout << endl;

	if (count != 0) {
		while (1) {
			row = rand() % 5;
			col = rand() % 6;
			if (candidate[row][col] == 1) break;
		}
	}
	else {
		while (1) {
			row = rand() % 5;
			col = rand() % 6;
			if (board.get_cell_color(row, col) == color || board.get_orbs_num(row, col) == 0) break;
		}
	}
	//////////

	index[0] = row;
	index[1] = col;
}