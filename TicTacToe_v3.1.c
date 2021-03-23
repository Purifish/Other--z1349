#define _CRT_SECURE_NO_WARNINGS
/*
	Simple single player Tic-Tac-Toe program by Muqaffa Al-Afham bin Kamaruzaman.
	The AI looks the maximum number of moves ahead, and thus cannot lose.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 8-bit ASCII codes for box edges
#define E1 201
#define E2 187
#define E3 200
#define E4 188
#define E5 205
#define E6 186

// 8-bit ASCII codes for box intersections
#define I1 203
#define I2 204
#define I3 185
#define I4 202
#define I5 206

// 8-bit ASCII codes for Loading Bars
#define BAR 219
#define DBAR 176

#define BARCOUNT 25

int Maxi(int, int, int, int, char*);
int Mini(int, int, int, int, char*);
int GameOverCheck(char*);
int Eval(char*);
void Display(int, int, char*);
void play(void);

void ChangeFont(char);
void ClearScreen(void);
void delay(int);

int main()
{
	srand(time(NULL));
	play();
	return 0;
}

void ClearScreen(void)
{
#ifdef _WIN32 // Windows
	system("cls");
#elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)) // Mac OS
	system("clear");
	//add some other OSes here if needed
#else
#error "OS not supported."
#endif
}

void ChangeFont(char c)
{
	if (c == 'r')
		printf("\033[1;31m"); // Red font
	else if (c == 'b')
		printf("\033[1;34m"); // Blue font
	else if (c == 'g')
		printf("\033[1;32m"); // Green font
	else
		printf("\033[1;0m"); // White (default) font
}

void delay(int ms)
{
	long pause;
	clock_t start;

	pause = ms * (CLOCKS_PER_SEC / 1000);
	start = clock();
	while ((clock() - start) < pause);
}

void play(void)
{
	int i, turn, result, place, Depth = 0;
	char board[9] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' }; // Actual board
	char temp[9] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' }; // board representative (for AI use)

	printf("Enter 1 to go first, any other key to go second:\n"); 
	turn = (getchar() == '1') ? 1 : 2;
	ClearScreen();
	printf("Initializing ...\n\n");
	ChangeFont('r');
	for (i = 0; i < BARCOUNT; i++)
		putchar(DBAR);
	putchar('\r');
	ChangeFont('g');
	for (i = 0; i < BARCOUNT; i++)
	{
		delay(30);
		putchar(BAR);
	}
	delay(750);
	ChangeFont('w');
	ClearScreen();
	Display(3, 3, board);

	for (i = 0; i < 9; i++)
	{
		Depth = 9 - i; // There aren't too many different positions, thus AI can in fact solve the game by looking ahead the max number of moves left
		if (i % 2 == 0 && turn == 1 || i % 2 == 1 && turn == 2) // user's turn
		{
			printf("Choose slot (1 to 9)\n");
			do
			{
				place = getchar() - '0';
			} while (place < 1 || place > 9 || board[place - 1] > '9'); // invalid slot
			// Board update
			temp[place - 1] = board[place - 1] = (turn == 1) ? 'X' : 'O';
			ClearScreen();
			Display(3, 3, board); // display updated board
		}
		else
		{
			printf("Computer's turn\n");
			for (int j = 0; j < BARCOUNT; j++)
				putchar(DBAR);
			putchar('\r');
			if (turn == 1)
				place = Mini(-1000000, 1000000, Depth, Depth, temp);
			else
				place = Maxi(-1000000, 1000000, Depth, Depth, temp);

			for (int j = 0; j < BARCOUNT; j++)
			{
				delay(20);
				putchar(BAR);
			}	
			temp[place - 1] = board[place - 1] = turn == 1 ? 'O' : 'X';
			ClearScreen();
			Display(3, 3, board);
		}
		// check if game has ended
		result = GameOverCheck(board);
		switch (result)
		{
		case 1: printf("X wins.\n");
			break;
		case 2: printf("O wins. \n");
			break;
		case 3: printf("It's a tie.\n");
			break;
		}
		if (result > 0)
			break;
	}
}

int Maxi(int alpha, int beta, int depth, int maxDepth, char* board) // AI uses minimax algorithm with alpha-beta pruning
{
	int i, score, over, temp, scores[9];
	if (depth == 0)
		return Eval(board);
	over = GameOverCheck(board);
	if (over != 0) // if game has ended, stop looking deeper and return evaluation
		return Eval(board);
	i = rand() % 9; // randomize which move AI looks at first
	for (int count = 0; count < 9; count++, i = (i + 1) % 9)
	{
		if (board[i] < 79) // empty slot
		{
			temp = board[i];
			board[i] = 'X';
			score = Mini(alpha, beta, depth - 1, maxDepth, board);
			board[i] = temp; // reset slot back to empty
		}
		else // occupied slot.
			score = -1000000;
		if (score >= beta)
			return beta;
		if (score > alpha)
			alpha = score;
		if (depth == maxDepth)
			scores[i] = score;
	}

	if (depth == maxDepth)
	{
		for (int count = 0; count < 9; count++, i = (i + 1) % 9) // find best move
		{
			if (scores[i] == alpha)
				return i + 1;
		}
	}
	return alpha;
}

int Mini(int alpha, int beta, int depth, int maxDepth, char* board)
{
	int i, score, over, temp, scores[9];
	if (depth == 0)
		return Eval(board);
	over = GameOverCheck(board);
	if (over != 0)
		return Eval(board);

	i = rand() % 9;
	for (int count = 0; count < 9; count++, i = (i + 1) % 9)
	{
		if (board[i] < 79)
		{
			temp = board[i];
			board[i] = 'O';
			score = Maxi(alpha, beta, depth - 1, maxDepth, board);
			board[i] = temp;
		}
		else
			score = 1000000;
		if (score <= alpha)
			return alpha;
		if (score < beta)
			beta = score;
		if (depth == maxDepth)
			scores[i] = score;
	}
	if (depth == maxDepth)
	{
		for (int count = 0; count < 9; count++, i = (i + 1) % 9)
		{
			if (scores[i] == beta)
				return i + 1;
		}
	}
	return beta;
}

int Eval(char* board)
{
	int pieces = 0;

	for (int i = 0; i < 9; pieces += board[i++] > '9'); // piece count

	switch (GameOverCheck(board))
	{
	case 1: return 1000 - pieces; // pieces is added/subtracted so that AI wins as quickly as possible
	case 2: return -1000 + pieces;
	}
	return 0;
}

int GameOverCheck(char* board)
{
	int i;

	for (i = 0; i < 7; i += 3) // rows
	{
		if (board[i] + board[i + 1] + board[i + 2] == 264)
			return 1;
		if (board[i] + board[i + 1] + board[i + 2] == 237)
			return 2;
	}
	for (i = 0; i < 3; i++) // columns
	{
		if (board[i] + board[i + 3] + board[i + 6] == 264)
			return 1;
		if (board[i] + board[i + 3] + board[i + 6] == 237)
			return 2;
	}

	// diagonals
	if (board[0] + board[4] + board[8] == 264 || board[6] + board[4] + board[2] == 264)
		return 1;
	if (board[0] + board[4] + board[8] == 237 || board[6] + board[4] + board[2] == 237)
		return 2;

	for (i = 0; i < 9; i++) // tie test
	{
		if (board[i] < 'O') // test failed(not tie)
			return 0;
	}
	return 3; // test passed(tie)
}

void Display(int ROW, int COLUMN, char* board)
{
	int row, column, i;
	printf("\n\n");
	for (row = 0; row < ROW; row++)
	{
		if (row != 0)
			printf("        %c", I2);
		else
			printf("        %c", E1);

		for (i = 0; i < COLUMN - 1; ++i)
		{
			if (row != 0)
				printf("%c%c%c%c", E5, E5, E5, I5);
			else
				printf("%c%c%c%c", E5, E5, E5, I1);
		}
		if (row != 0)
			printf("%c%c%c%c\n", E5, E5, E5, I3);
		else
			printf("%c%c%c%c\n", E5, E5, E5, E2);
		printf("        ");
		for (column = 0; column < COLUMN; column++)
		{
			printf("%c", E6);
			if (board[3 * row + column] == 'X')
				ChangeFont('r');
			else if (board[3 * row + column] == 'O')
				ChangeFont('b');
			printf(" %c ", board[3 * row + column]);
			ChangeFont('w');
			if (column == COLUMN - 1)
				printf("%c", E6);
		}
		printf("\n");
	}
	printf("        %c", E3);
	for (i = 1; i <= COLUMN - 1; ++i)
	{
		printf("%c%c%c%c", E5, E5, E5, I4);
	}
	printf("%c%c%c%c", E5, E5, E5, E4);
	printf("\n\n");
}
