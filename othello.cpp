#include "stdafx.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <utility>
#include <time.h>

//returns a pair <direction, coordinate> where direction is North, Northeast, etc (clockwise)
//and coordinate is in the form x*8 + y
std::vector< std::pair<int, int> > get_directions(int board[100][100], int x, int y, int color, int n) {
	std::vector< std::pair<int, int> > directions;
	if (board[x][y]) {
		return directions;
	}
	//north
	if ((y < n-2) && (board[x][y + 1] == -color)) {
		for (int i = y + 2; i < n; i++) {
			if (!board[x][i]) {
				break;
			}
			else if (board[x][i] == color) {
				directions.push_back(std::make_pair<int, int>(0, x * n + i));
				break;
			}
		}
	}
	//northeast
	if ((y < n-2) && (x < n-2) && (board[x + 1][y + 1] == -color)) {
		for (int i = 0; (x + i + 2 < n) && (y + i + 2 < n); i++) {
			if (!board[x + i + 2][y + i + 2]) {
				break;
			}
			else if (board[x + i + 2][y + i + 2] == color) {
				directions.push_back(std::make_pair<int, int>(1, (x + i + 2) * n + (y + i + 2)));
				break;
			}
		}
	}
	//east
	if ((x < n-2) && (board[x + 1][y] == -color)) {
		for (int i = x + 2; i < n; i++) {
			if (!board[i][y]) {
				break;
			}
			else if (board[i][y] == color) {
				directions.push_back(std::make_pair<int, int>(2, i * n + y));
				break;
			}
		}
	}
	//southeast
	if ((y > 1) && (x < n-2) && (board[x + 1][y - 1] == -color)) {
		for (int i = 0; (x + i + 2 < n) && (y - i - 2 >= 0); i++) {
			if (!board[x + i + 2][y - i - 2]) {
				break;
			}
			else if (board[x + i + 2][y - i - 2] == color) {
				directions.push_back(std::make_pair<int, int>(3, (x + i + 2) * n + (y - i - 2)));
				break;
			}
		}
	}
	//south
	if ((y > 1) && (board[x][y - 1] == -color)) {
		for (int i = y - 2; i >= 0; i--) {
			if (!board[x][i]) {
				break;
			}
			else if (board[x][i] == color) {
				directions.push_back(std::make_pair<int, int>(4, x * n + i));
				break;
			}
		}
	}
	//southwest
	if ((y > 1) && (x > 1) && (board[x - 1][y - 1] == -color)) {
		for (int i = 0; (x - i - 2 >= 0) && (y - i - 2 >= 0); i++) {
			if (!board[x - i - 2][y - i - 2]) {
				break;
			}
			else if (board[x - i - 2][y - i - 2] == color) {
				directions.push_back(std::make_pair<int, int>(5, (x - i - 2) * n + (y - i - 2)));
				break;
			}
		}
	}
	//west
	if ((x > 1) && (board[x - 1][y] == -color)) {
		for (int i = x - 2; i >= 0; i--) {
			if (!board[i][y]) {
				break;
			}
			else if (board[i][y] == color) {
				directions.push_back(std::make_pair<int, int>(6, i * n + y));
				break;
			}
		}
	}
	//northwest
	if ((y < 6) && (x > 1) && (board[x - 1][y + 1] == -color)) {
		for (int i = 0; (x - i - 2 >= 0) && (y + i + 2 < n); i++) {
			if (!board[x - i - 2][y + i + 2]) {
				break;
			}
			else if (board[x - i - 2][y + i + 2] == color) {
				directions.push_back(std::make_pair<int, int>(7, (x - i - 2) * n + (y + i + 2)));
				break;
			}
		}
	}
	return directions;
}

//returns all moves for a players on the current board. Each pair has a coordinate in the form
//x*8+y, and a vector of pairs, each pair contains a direction and an endpoint in the form x*8+y
std::vector< std::pair<int, std::vector< std::pair<int, int> > > > get_moves(int board[100][100], int color,int n) {
	std::vector< std::pair<int, std::vector< std::pair<int, int> > > > moves;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			moves.push_back(std::make_pair<int, std::vector< std::pair<int, int> > >(i * n + j, get_directions(board, i, j, color,n)));
			if (!moves.back().second.size()) {
				moves.pop_back();
			}
		}
	}
	return moves;
}

//makes a move and flips the appropriate pieces
void make_move(int board[100][100], int x, int y, int color, std::vector< std::pair<int, int> > directions, int n) {
	for (auto it = directions.begin(); it != directions.end(); it++) {
		int i = x;
		int j = y;
		while ((i != ((*it).second / n)) || (j != ((*it).second & n-1))) {
			board[i][j] = color;
			if (i < ((*it).second / n)) {
				i++;
			}
			else if ((i >(*it).second / n)) {
				i--;
			}
			if (j < ((*it).second & n-1)) {
				j++;
			}
			else if (j >((*it).second & n-1)) {
				j--;
			}
		}
	}
}

//undoes a move (needs directions data so it can unflip stuff)
void undo_move(int board[100][100], int x, int y, int color, std::vector< std::pair<int, int> > directions, int n) {
	for (auto it = directions.begin(); it != directions.end(); it++) {
		int i = x;
		int j = y;
		while ((i != ((*it).second / n)) || (j != ((*it).second & n-1))) {
			board[i][j] = -color;
			if (i < ((*it).second / n)) {
				i++;
			}
			else if ((i >(*it).second / n)) {
				i--;
			}
			if (j < ((*it).second & n-1)) {
				j++;
			}
			else if (j >((*it).second & n-1)) {
				j--;
			}
		}
		board[i][j] = color;
	}
	board[x][y] = 0;
}

void print(int board[100][100], int n) {
	for (int i = n-1; i >= 0; i--) {
		printf("%d ", i);
		for (int j = 0; j < n; j++) {
			if (!board[j][i]) {
				printf("-");
			}
			else if (board[j][i] == 1) {
				printf("O");
			}
			else {
				printf("X");
			}
		}
		printf("\n");
	}
	printf("  ");
	for (int i = 0; i < n; i++) {
		printf("%d", i);
	}
	printf("\n\n");
}

int score(int board[100][100], int color, int n) {
	int sum = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			sum += board[i][j];
		}
	}
	return sum * color;
}

//Largely the same pseudocode from the negamax wikipedia article, but adapted for the rules of othello
int negamax_aux(int board[100][100], int color, int depth, int alpha, int beta, int n) {
	if (depth == 0) {
		return score(board, color,n);
	}
	std::vector< std::pair<int, std::vector< std::pair<int, int> > > > moves = get_moves(board, color,n);
	if (moves.size() == 0) {
		if (get_moves(board, -color,n).size() == 0) {
			return score(board, color,n);
		}
		int val = -negamax_aux(board, -color, depth - 1, -beta, -alpha, n);
		if (val >= beta) {
			return val;
		}
		if (val > alpha) {
			alpha = val;
		}
	}
	else {
		for (auto it = moves.begin(); it != moves.end(); it++) {
			make_move(board, (*it).first / 8, (*it).first & 7, color, (*it).second,n);
			int val = -negamax_aux(board, -color, depth - 1, -beta, -alpha,n);
			undo_move(board, (*it).first / 8, (*it).first & 7, color, (*it).second,n);
			if (val >= beta) {
				return val;
			}
			if (val > alpha) {
				alpha = val;
			}
		}
	}
	return alpha;
}

//parent function to negamax_aux, this function actually maintains the current best move
int negamax(int board[100][100], int color, int depth, int n) {
	int alpha = -65;
	int beta = 65;
	std::vector< std::pair<int, std::vector< std::pair<int, int> > > > moves = get_moves(board, color,n);
	int move = moves[0].first;
	for (auto it = moves.begin(); it != moves.end(); it++) {
		make_move(board, (*it).first / n, (*it).first & n-1, color, (*it).second,n);
		int val = -negamax_aux(board, -color, depth - 1, -beta, -alpha, n);
		undo_move(board, (*it).first / n, (*it).first & n-1, color, (*it).second,n);
		if (val >= beta) {
			return (*it).first;
		}
		if (val > alpha) {
			alpha = val;
			move = (*it).first;
		}
	}
	return move;
}

//run with ./reversi [int] where int is the number of ply the AI searches deep. Default is 3 ply.
int main(int argc, char **argv) {
	int depth = 3;
	if (argc > 1) {
		depth = atol(argv[1]);
	}
	depth *= 2;

	int n;
	printf("Enter  number \'N\', \'N\' indicating size of the game board: \n");
	scanf("%d", &n);
	printf("\n\n");

	int board[100][100];
	memset(board, 0, sizeof(board));
	board[n/2-1][n/2-1] = board[n/2][n/2] = -1;
	board[n/2-1][n/2] = board[n/2][n/2-1] = 1;
	int turn = -1;

	while (true) {
		std::vector< std::pair<int, std::vector< std::pair<int, int> > > > moves = get_moves(board, turn,n);
		if (moves.size() == 0) {
			turn = -turn;
			moves = get_moves(board, turn,n);
			if (moves.size() == 0) {
				int b = score(board, -1, n);
				printf("final score: %d\n", b);
				if (b < 0)
					printf("\nO won the game\n\n");
				else if(b > 0)
					printf("\nX won the game\n\n");
				else
					printf("\nit is a tie\n\n");
				return 0;
			}
		}
		else {
			int x, y;
			if (turn == -1) {
			x = negamax(board, turn, depth, n);
				for (auto it = moves.begin(); it != moves.end(); it++) {
					if (x == ((*it).first)) {
						make_move(board, x / n, x % n, turn, (*it).second,n);
						turn = -turn;
						break;
					}
				}
			}
			else {
				x = negamax(board, turn, depth, n);
				for (auto it = moves.begin(); it != moves.end(); it++) {
					if (x == ((*it).first)) {
						make_move(board, x / n, x % n, turn, (*it).second,n);
						turn = -turn;
						break;
					}
				}
			}
		}
	}
	
	return 0;
	
}