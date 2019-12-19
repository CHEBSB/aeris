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

	//////////// Random Algorithm ////////////
	// Here is the random algorithm for your reference, you can delete or comment it.
	srand(time(NULL));
	class cell {
	public:
		cell() {}
		int cap;
		int size;
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
		node(node* Parent, int r, int c)
		{
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 6; j++)
					B[i][j] = Parent->B[i][j];
			whoseTurn = !(Parent->whoseTurn);
			B[r][c].size++;
			B[r][c].col = Parent->whoseTurn;
			checkExp(r, c);
		}
		bool whoseTurn;	// 1 for my turn, 0 for my oppenent's
		cell B[5][6];
		node* child[5][6];
		void initChild()
		{
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
	int row, col;
	int i, j;
	int color = player.get_color();
	int candidate[5][6] = { 0 };

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
			row = rand() % 5;
			col = rand() % 6;
			if (candidate[row][col] == 1) break;
		}
	}
	else {
		while (1) {
			row = rand() % 5;
			col = rand() % 6;
			if (board.get_cell_color(row, col) == color && board.get_orbs_num(row, col) != 0) break;
		}
	}
	//////////

	index[0] = row;
	index[1] = col;
}
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