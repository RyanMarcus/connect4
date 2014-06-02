#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define OFF_BOARD -2
#define EMPTY -1

typedef struct {
	int width;
	int height;
	int* board;
	int last_move;
} GameState;

GameState* newGameState(int width, int height) {
	GameState* toR = (GameState*) malloc(sizeof(GameState));
	toR->width = width;
	toR->height = height;
	toR->board = (int*) malloc(sizeof(int) * width * height);

	for (int i = 0; i < width * height; i++) {
		toR->board[i] = EMPTY;
	}

	return toR;
}


int at(GameState* gs, int x, int y) {
	if (x < 0 || y < 0)
		return OFF_BOARD;

	if (x >= gs->width || y >= gs->height)
		return OFF_BOARD;

	return gs->board[x * gs->width + y];
}

void drop(GameState* gs, int column, int player) {
	for (int i = 0; i < gs->height; i++) {
		if (at(gs, column, i) == EMPTY) {
			gs->board[column * gs->width + i] = player;
			gs->last_move = column;
			return;
		}
	}


}

int checkAt(GameState* gs, int x, int y) {
	// check across
	int found = 1;
	int curr = at(gs, x, y);
	for (int i = 1; i < 4; i++) {
		if (at(gs, x + i, y) != curr) {
			found = 0;
			break;
		}
	}

	if (found && (curr != EMPTY && curr != OFF_BOARD))
		return curr;

	// check down
	found = 1;
	for (int i = 1; i < 4; i++) {
		if (at(gs, x, y + i) != curr) {
			found = 0;
			break;
		}
	}

	if (found && (curr != EMPTY && curr != OFF_BOARD))
		return curr;

	// check diag +/+
	found = 1;
	for (int i = 1; i < 4; i++) {
		if (at(gs, x + i, y + i) != curr) {
			found = 0;
			break;
		}
	}

	if (found && (curr != EMPTY && curr != OFF_BOARD))
		return curr;

	// check diag -/+
	found = 1;
	for (int i = 1; i < 4; i++) {
		if (at(gs, x - i, y + i) != curr) {
			found = 0;
			break;
		}
	}

	if (found && (curr != EMPTY && curr != OFF_BOARD))
		return curr;

	return 0;
}

int countAt(GameState* gs, int x, int y, int player) {

	// check across
	int found = 0;
	int inc = 1;
	for (int i = 1; i < 4; i++) {
		int curr = at(gs, x + i, y);
		if (curr != player && curr != EMPTY) {
			inc = 0;
			break;
		}
	}
	found += inc;

	// check down
	inc = 1;
	for (int i = 1; i < 4; i++) {
		int curr = at(gs, x, y + i);
		if (curr != player && curr != EMPTY) {
			inc = 0;
			break;
		}
	}
	found += inc;

	// check diag +/+
	inc = 1;
	for (int i = 1; i < 4; i++) {
		int curr = at(gs, x + i, y + i);
		if (curr != player && curr != EMPTY) {
			inc = 0;
			break;
		}
	}
	found += inc;

	// check diag -/+
	inc = 1;
	for (int i = 1; i < 4; i++) {
		int curr = at(gs, x - i, y + i);
		if (curr != player && curr != EMPTY) {
			inc = 0;
			break;
		}
	}
	found += inc;

	return found;
}

int getWinner(GameState* gs) {
	for (int x = 0; x < gs->width; x++) {
		for (int y = 0; y < gs->height; y++) {
			int res = checkAt(gs, x, y);
			if (res)
				return res;
		}
	}

	return 0;
}

int isDraw(GameState* gs) {
	for (int x = 0; x < gs->width; x++) {
		for (int y = 0; y < gs->height; y++) {
			if (at(gs, x, y) == EMPTY)
				return 0;
		}
	}

	return 1;
}

int getHeuristic(GameState* gs, int player, int other_player) {
	int toR = 0;
	for (int x = 0; x < gs->width; x++) {
		for (int y = 0; y < gs->height; y++) {
			toR += countAt(gs, x, y, player);
			toR -= countAt(gs, x, y, other_player);
		}
	}
	
	return toR;
}

int canMove(GameState* gs, int column) {
	for (int y = 0; y < gs->height; y++) {
		if (at(gs, column, y) == EMPTY)
			return 1;
	}

	return 0;
}

GameState* stateForMove(GameState* orig, int column, int player) {
	GameState* toR = newGameState(orig->width, orig->height);
	memcpy(toR->board, orig->board, sizeof(int) * orig->width * orig->height);
	drop(toR, column, player);
	return toR;
}

void printGameState(GameState* gs) {
	for (int i = 0; i < gs->width; i++) {
		printf("%d ", i);
	}

	printf("\n");
	
	for (int y = gs->height - 1; y >= 0 ; y--) {
		for (int x = 0; x < gs->width; x++) {
			int toP = at(gs, x, y);
			if (toP == EMPTY) {
				printf("  ");
			} else {
				printf("%d ", toP);
			}
		}
		printf("\n");
	}

	for (int i = 0; i < gs->width; i++) {
		printf("%d ", i);
	}

	printf("\n");
}

long hashGameState(GameState* gs) {
	unsigned long hash = 14695981039346656037Lu;
	for (int i = 0; i < gs->width * gs->height; i++) {
		hash ^= gs->board[i];
		hash *= 1099511628211Lu;
	}
	return hash;
}


typedef struct {
	GameState* gs;
	int player;
	int other_player;
	int turn;
	
	int alpha;
	int beta;

	int best_move;
} GameTreeNode;

GameTreeNode* newGameTreeNode(GameState* gs, int player, int other, int turn, int alpha, int beta) {
	GameTreeNode* toR = (GameTreeNode*) malloc(sizeof(GameTreeNode));
	toR->gs = gs;
	toR->player = player;
	toR->other_player = other;
	toR->turn = turn;
	toR->alpha = alpha;
	toR->beta = beta;
	toR->best_move = -1;
	return toR;
}

int heuristicForState(GameState* gs, int player, int other) {
	if (isDraw(gs))
		return 0;

	int term_stat = getWinner(gs);
	if (term_stat == player)
		return 1000;

	if (term_stat)
		return -1000;

	
	return getHeuristic(gs, player, other);
		     
}


int ascComp(void* thunk, const void* a, const void* b) {
	GameTreeNode* node = (GameTreeNode*) thunk;
	return heuristicForState(*(GameState**) a, node->player, node->other_player) -
		heuristicForState(*(GameState**) b, node->player, node->other_player);
	
}

int desComp(void* thunk, const void* a, const void* b) {
	GameTreeNode* node = (GameTreeNode*) thunk;
	return heuristicForState(*(GameState**) b, node->player, node->other_player) -
		heuristicForState(*(GameState**) a, node->player, node->other_player);
	
}

int getWeight(GameTreeNode* node, int movesLeft) {
	if (getWinner(node->gs) == EMPTY || isDraw(node->gs) || movesLeft == 0)
		return heuristicForState(node->gs, node->player, node->other_player);

	GameState** possibleMoves = (GameState**) malloc(sizeof(GameState*) * node->gs->width);
	int validMoves = 0;
	for (int possibleMove = 0; possibleMove < node->gs->width; possibleMove++) {
		if (!canMove(node->gs, possibleMove)) {
			continue;
		}
		
		possibleMoves[validMoves] = stateForMove(node->gs, possibleMove, (node->turn ? node->player : node->other_player));
		validMoves++;
	}

	// order possibleMoves by the heuristic
	if (node->turn) {
		qsort_r(possibleMoves, validMoves, sizeof(GameState*), node, ascComp); 
	} else {
		qsort_r(possibleMoves, validMoves, sizeof(GameState*), node, desComp); 
	}

	int best_weight = (node->turn ? INT_MIN : INT_MAX);

	for (int move = 0; move < validMoves; move++) {
		// TODO: hash table
		GameTreeNode* child = newGameTreeNode(possibleMoves[move], node->player, node->other_player, !(node->turn),
						      node->alpha, node->beta);
		int child_weight = getWeight(child, movesLeft - 1);

		// alpha-beta pruning
		if (!node->turn) {
			// min node
			if (child_weight <= node->alpha) {
				// MAX ensures we will never go here
				return child_weight;
			}
			node->beta = (node->beta < child_weight ? node->beta : child_weight);
		} else {
			// max node
			if (child_weight >= node->beta) {
				// MIN ensures we will never go here
				return child_weight;
			}
			node->alpha = (node->alpha > child_weight ? node->alpha : child_weight);
		}

		if (!(node->turn)) {
			// min node
			if (best_weight > child_weight) {
				best_weight = child_weight;
				node->best_move = move;
			}
		} else {
			// max node
			if (best_weight < child_weight) {
				best_weight = child_weight;
				node->best_move = move;
			}
		}

		free(possibleMoves[move]);
		free(child);
		
	}

	free(possibleMoves);
	return best_weight;
}

int getBestMove(GameTreeNode* node, int movesLeft) {
	getWeight(node, movesLeft);
	return node->best_move;
}


// END OF API

void checkWin(GameState* gs) {
	int win = getWinner(gs);
	
	if (win) {
		printf("Game over! %d wins!\n", win);
		printGameState(gs);
		exit(0);
	}

	if (isDraw(gs)) {
		printf("Game over! Draw!\n");
		printGameState(gs);
		exit(0);
	}
	     
}

int main(int argc, char** argv) {
	GameState* gs = newGameState(4, 4);
	while (1) {
		GameTreeNode* n = newGameTreeNode(gs, 1, 2, 1, INT_MIN, INT_MAX);
		int move = getBestMove(n, 5);
		drop(gs, move, 1);
		free(n);

		printGameState(gs);

		checkWin(gs);

		n = newGameTreeNode(gs, 2, 1, 1, INT_MIN, INT_MAX);
		move = getBestMove(n, 5);
		drop(gs, move, 2);
		free(n);

		printGameState(gs);

		checkWin(gs);
		
	}

	return 0;
}

