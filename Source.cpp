#include <iostream>
#include <string>
#include <cmath>
#include <windows.h>
using namespace std;

//Confidentials
void enableANSI()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(hOut, &mode);
	SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
const int rows = 8;
const int cols = 8;

//Enum For colours
enum Color
{
	WHITE, BLACK
};

//Forward Declaration
class Board;

//Piece Class
class Piece
{
	Color color;
	char symbol;

public:
	Piece(Color c, char s) :color(c), symbol(s) {}
	~Piece() {}

	char getSymbol() const
	{
		return symbol;
	}
	Color getColor() const
	{
		return color;
	}

	//Virtual Validaton Function
	virtual bool valid(int s_row, int s_col, int d_row, int d_col, Board* board) = 0;
};

//Pawn Class - Inheritance
class Pawn :public Piece
{
public:
	Pawn(Color c) :Piece(c, c == WHITE ? 'P' : 'p') {}
	bool valid(int s_row, int s_col, int d_row, int d_col, Board* board);
};
//Rook Class - Inheritance
class Rook :public Piece
{
public:
	Rook(Color c) :Piece(c, c == WHITE ? 'R' : 'r') {}
	bool valid(int s_row, int s_col, int d_row, int d_col, Board* board);
};
//Knight Class - Inheritance
class Knight :public Piece
{
public:
	Knight(Color c) :Piece(c, c == WHITE ? 'N' : 'n') {}
	bool valid(int s_row, int s_col, int d_row, int d_col, Board* board);
};
//Bishop Class - Inheritance
class Bishop :public Piece
{
public:
	Bishop(Color c) :Piece(c, c == WHITE ? 'B' : 'b') {}
	bool valid(int s_row, int s_col, int d_row, int d_col, Board* board);
};
//Queen Class - Inheritance
class Queen :public Piece
{
public:
	Queen(Color c) :Piece(c, c == WHITE ? 'Q' : 'q') {}
	bool valid(int s_row, int s_col, int d_row, int d_col, Board* board);
};
//King Class - Inheritance
class King :public Piece
{
public:
	King(Color c) :Piece(c, c == WHITE ? 'K' : 'k') {}
	bool valid(int s_row, int s_col, int d_row, int d_col, Board* board);
};

//Board Class
class Board
{
	//Composition with Piece Class
	Piece* grid[rows][cols];
	char whiteCaptured[32];
	char blackCaptured[32];
	int whiteCapturedCount = 0;
	int blackCapturedCount = 0;

public:
	Board()
	{
		initializeEmpty();
		setupPiece();
	}
	~Board()
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				if (grid[i][j] != nullptr)
				{
					delete grid[i][j];
					grid[i][j] = nullptr;
				}
			}
		}
	}

	//Blank Board Initializing
	void initializeEmpty()
	{
		for (int i = 0;i < rows;i++)
		{
			for (int j = 0;j < cols;j++)
			{
				grid[i][j] = nullptr;
			}
		}
	}

	//Setting up Pieces
	void setupPiece()
	{
		for (int j = 0; j < cols; j++)
		{
			grid[1][j] = new Pawn(Color::BLACK);
			grid[6][j] = new Pawn(Color::WHITE);
		}

		grid[0][0] = new Rook(Color::BLACK);
		grid[0][7] = new Rook(Color::BLACK);
		grid[7][0] = new Rook(Color::WHITE);
		grid[7][7] = new Rook(Color::WHITE);

		grid[0][1] = new Knight(Color::BLACK);
		grid[0][6] = new Knight(Color::BLACK);
		grid[7][1] = new Knight(Color::WHITE);
		grid[7][6] = new Knight(Color::WHITE);

		grid[0][2] = new Bishop(Color::BLACK);
		grid[0][5] = new Bishop(Color::BLACK);
		grid[7][2] = new Bishop(Color::WHITE);
		grid[7][5] = new Bishop(Color::WHITE);

		grid[0][3] = new Queen(Color::BLACK);
		grid[7][3] = new Queen(Color::WHITE);

		grid[0][4] = new King(Color::BLACK);
		grid[7][4] = new King(Color::WHITE);
	}

	//Get Piece
	Piece* getPiece(int r, int c) const
	{
		return grid[r][c];
	}

	//Set Piece
	void setPiece(int r, int c, Piece* p)
	{
		grid[r][c] = p;
	}

	//Checking boundries of Board
	bool validMove(string s)
	{
		s[0] = tolower(s[0]);
		return (s.length() == 2 && s[0] >= 'a' && s[0] <= 'h' && s[1] >= '1' && s[1] <= '8');
	}

	//Get opponent colour
	Color getOpponent(Color c)
	{
		return (c == WHITE) ? BLACK : WHITE;
	}

	//Move Safety
	bool isMoveSafe(int s_row, int s_col, int d_row, int d_col, Piece* p)
	{
		Piece* captured = getPiece(d_row, d_col);
		setPiece(d_row, d_col, p);
		setPiece(s_row, s_col, nullptr);
		int kRow, kCol;
		findKing(p->getColor(), kRow, kCol);
		bool safe = !isUnderAttack(kRow, kCol, getOpponent(p->getColor()));
		setPiece(s_row, s_col, p);
		setPiece(d_row, d_col, captured);

		return safe;
	}

	//Finding King
	void findKing(Color c, int& kRow, int& kCol)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				if (grid[i][j] != nullptr && grid[i][j]->getColor() == c)
				{
					char s = grid[i][j]->getSymbol();

					if ((c == WHITE && s == 'K') || (c == BLACK && s == 'k'))
					{
						kRow = i;
						kCol = j;
						return;
					}
				}
			}
		}
	}

	//Is King Under Attack???
	bool isUnderAttack(int row, int col, Color attacker)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				if (grid[i][j] != nullptr && grid[i][j]->getColor() == attacker)
				{
					if (grid[i][j]->valid(i, j, row, col, this))
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	//Valid move check for Checkmate
	bool hasValidMoves(Color player)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				if (grid[i][j] && grid[i][j]->getColor() == player)
				{
					for (int r = 0; r < rows; r++)
					{
						for (int c = 0; c < cols; c++)
						{
							if (grid[i][j]->valid(i, j, r, c, this))
							{
								if (grid[r][c] && grid[r][c]->getColor() == player)
								{
									continue;
								}

								if (isMoveSafe(i, j, r, c, grid[i][j]))
								{
									return true;
								}
							}
						}
					}
				}
			}
		}
		return false;
	}

	//Display whole Board
	void display(int dangerRow = -1, int dangerCol = -1)
	{
		const char* pieces[2][6] =
		{
			{ u8"♔", u8"♕", u8"♖", u8"♗", u8"♘", u8"♙" },
			{ u8"♚", u8"♛", u8"♜", u8"♝", u8"♞",u8"♙" }
		};
		cout << "\t\t\t  ";
		for (char c = 'A'; c <= 'H'; c++)
		{
			cout << "    " << c;
		}

		cout << "\n";

		cout << "\t\t\t    " << u8"┌";
		for (int i = 0; i < 7; i++)
		{
			cout << u8"────┬";
		}
		cout << u8"────┐\n";

		for (int i = 0; i < rows; i++)
		{
			cout << "\t\t\t  " << 8 - i << " " << u8"│";

			for (int j = 0; j < cols; j++)
			{
				bool light = ((i + j) % 2 == 0);

				if (i == dangerRow && j == dangerCol)
				{
					cout << "\033[1;48;5;196m";
				}
				else if (light)
				{
					cout << "\033[1;48;5;247m";
				}
				else
				{
					cout << "\033[1;48;5;239m";
				}

				if (grid[i][j] == nullptr)
				{
					cout << "    ";
				}
				else
				{
					Piece* p = grid[i][j];

					int pieceIndex = 0;

					switch (tolower(p->getSymbol()))
					{
					case 'k': pieceIndex = 0; break;
					case 'q': pieceIndex = 1; break;
					case 'r': pieceIndex = 2; break;
					case 'b': pieceIndex = 3; break;
					case 'n': pieceIndex = 4; break;
					case 'p': pieceIndex = 5; break;
					}

					if (p->getColor() == WHITE)
					{
						cout << "\033[1;38;5;15m";
						cout << " " << pieces[0][pieceIndex] << "  ";
					}
					else
					{
						cout << "\033[1;38;5;232m";;
						cout << " " << pieces[1][pieceIndex] << "  ";
					}
				}

				cout << "\033[0m" << u8"│";
			}

			cout << " " << 8 - i;
			cout << "\n";

			if (i != 7)
			{
				cout << "\t\t\t    " << u8"├";
				for (int j = 0; j < 7; j++)
				{
					cout << u8"────┼";
				}
				cout << u8"────┤\n";
			}
		}

		cout << "\t\t\t    " << u8"└";
		for (int i = 0; i < 7; i++)
		{
			cout << u8"────┴";
		}
		cout << u8"────┘\n";

		cout << "\t\t\t  ";
		for (char c = 'A'; c <= 'H'; c++)
		{
			cout << "    " << c;
		}

		cout << "\n";
		displayCaptured();
	}
	//Starting Game
	void start()
	{
		//Checking Turns (White By Default)
		bool isWhite = true;
		int start = 0;
		//Game Loop
		while (true)
		{
			if (start != 0)
			Sleep(1000);

			start = 1;
			system("cls");

			Color currentPlayer = isWhite ? WHITE : BLACK;
			Color opponent = getOpponent(currentPlayer);
			int kRow, kCol;
			findKing(currentPlayer, kRow, kCol);
			bool inCheck = isUnderAttack(kRow, kCol, opponent);

			if (inCheck)
			{
				display(kRow, kCol);
				cout << endl << (opponent == WHITE ? "Black " : "White ") << "is in CHECK\n";
			}
			else
			{
				display();
			}

			string from, to;
			if (isWhite)
			{
				cout << "\nWhite Turns.";
			}
			else
			{
				cout << "\nBlack Turns.";
			}

			cout << "\nEnter move: ";
			cin >> from >> to;

			//Boundries Validation
			if (!validMove(from) || !validMove(to))
			{
				cout << "Error: Out of Bound!\n";
				continue;
			}

			//String -> Int (conversion)
			int s_row = 8 - (from[1] - '0');
			int s_col = tolower(from[0]) - 'a';
			int d_row = 8 - (to[1] - '0');
			int d_col = tolower(to[0]) - 'a';

			//Getting source Piece
			Piece* p = getPiece(s_row, s_col);
			if (p == nullptr)
			{
				cout << "Error: No piece at source!\n";
				continue;
			}

			//Crosschecking source Piece with turn
			if ((isWhite && p->getColor() == WHITE) || (!isWhite && p->getColor() == BLACK))
			{
				//Checking Piece movement 
				if (!p->valid(s_row, s_col, d_row, d_col, this))
				{
					cout << "Error: Invalid move!\n";
					continue;
				}

				//Capturing piece
				Piece* destination = getPiece(d_row, d_col);
				if (destination != nullptr && destination->getColor() == p->getColor())
				{
					cout << "Error: Cannot capture own piece!\n";
					continue;
				}

				//Move Safety
				if (!isMoveSafe(s_row, s_col, d_row, d_col, p))
				{
					cout << "Error: King is in Check!\n";
					continue;
				}

				setPiece(d_row, d_col, p);
				setPiece(s_row, s_col, nullptr);
				if (destination != nullptr)
				{
					if (destination->getColor() == WHITE)
					{
						whiteCaptured[whiteCapturedCount++] = destination->getSymbol();
					}
					else
					{
						blackCaptured[blackCapturedCount++] = destination->getSymbol();
					}
				}
				delete destination;

				//Checkmate Logic
				findKing(currentPlayer, kRow, kCol);
				inCheck = isUnderAttack(kRow, kCol, opponent);

				if (!hasValidMoves(currentPlayer))
				{
					system("cls");
					if (inCheck)
					{
						display(kRow, kCol);
						cout << "CHECKMATE!\n";
						cout << (opponent == BLACK ? "Black " : "White ") << "won the match\n";
					}
					else
					{
						display();
						cout << "STALEMATE!\n";
					}
					break;
				}

			}
			else
			{
				cout << "Error: source piece isn't your piece!\n";
				continue;
			}

			//Changing Turn
			isWhite = (!isWhite);
		}
	}

	const char* getUnicodePiece(char symbol)
	{
		switch (tolower(symbol))
		{
		case 'k': return u8"♚";
		case 'q': return u8"♛";
		case 'r': return u8"♜";
		case 'b': return u8"♝";
		case 'n': return u8"♞";
		case 'p': return u8"♟";
		}

		return " ";
	}

	void displayCaptured()
	{

		cout << "Captured By White: ";

		for (int i = 0; i < blackCapturedCount; i++)
		{
			cout << getUnicodePiece(blackCaptured[i]) << " ";
		}

		cout << "\nCaptured By Black: ";

		for (int i = 0; i < whiteCapturedCount; i++)
		{
			cout << getUnicodePiece(whiteCaptured[i]) << " ";
		}

		cout << "\n";
	}
	void startScreen()
	{
		cout << "\033[38;5;247m";
		cout << R"(

	 	    ______   __   __   _______   _______   _______
		   /  ____| |  | |  | |   ____| |  _____| |  _____|
		  |  |      |  | |  | |  |___   | |_____  | |_____ 
		  |  |      |  ---  | |   ___|   \_____ \  \_____ \
		  |  |____  |  ___  | |  |____   _____| |  _____| |
		   \______| |__| |__| |_______| |_______/ |_______/

)";
		cout << "\033[0m";
		cout << "\n\t\t\t\t\033[1;33mTEXT BASED CHESS GAME\033[0m\n";
		cout << "\n\t\t\t\033[38;5;240m----------------------------------------\033[0m\n";
		cout << "\n\t\t\t\t\033[1;32mInstructions:\033[0m\n";
		cout << "\t\t\t\t- Enter moves like: e2 e4\n";
		cout << "\t\t\t\t- White plays first\n";
		cout << "\t\t\t\t- Checkmate to win\n";
		cout << "\n\t\t\t\t\033[1;36mPress ENTER to start...\033[0m\n";

		cin.ignore();
		cin.get();

		system("cls");
	}

};

//Pawn::valid Definition
bool Pawn::valid(int s_row, int s_col, int d_row, int d_col, Board* board)
{
	int direction = (getColor() == WHITE) ? -1 : 1;
	Piece* destination = board->getPiece(d_row, d_col);

	//Forward
	if (d_col == s_col && d_row == s_row + direction && destination == nullptr)
	{
		return true;
	}

	//First Move
	if (d_col == s_col && ((getColor() == WHITE && s_row == 6) ||
		(getColor() == BLACK && s_row == 1)) && d_row == s_row + 2 * direction)
	{
		if (board->getPiece(s_row + direction, s_col) == nullptr && destination == nullptr)
			return true;
	}

	//Capture 
	if (abs(d_col - s_col) == 1 && d_row == s_row + direction &&
		destination != nullptr && destination->getColor() != getColor())
	{
		return true;
	}
	return false;
}
//Rook::valid Definition
bool Rook::valid(int s_row, int s_col, int d_row, int d_col, Board* board)
{
	int c_row = abs(d_row - s_row);
	int c_col = abs(d_col - s_col);

	//2D movement
	if (!((c_row >= 1 && c_col == 0) || (c_row == 0 && c_col >= 1)))
	{
		return false;
	}

	//Validating Jump
	if (c_col == 0)
	{
		if (s_row > d_row)
		{
			swap(s_row, d_row);
		}
		for (int i = s_row + 1; i < d_row; i++)
		{
			if (board->getPiece(i, s_col) != nullptr)
			{
				return false;
			}
		}
	}
	if (c_row == 0)
	{
		if (s_col > d_col)
		{
			swap(s_col, d_col);
		}
		for (int i = s_col + 1; i < d_col; i++)
		{
			if (board->getPiece(s_row, i) != nullptr)
			{
				return false;
			}
		}
	}
	return true;
}
//Knight::valid Definition
bool Knight::valid(int s_row, int s_col, int d_row, int d_col, Board* board)
{
	int c_row = abs(d_row - s_row);
	int c_col = abs(d_col - s_col);

	return ((c_row == 2 && c_col == 1) || (c_row == 1 && c_col == 2));
}
//Bishop::valid Definition
bool Bishop::valid(int s_row, int s_col, int d_row, int d_col, Board* board)
{
	int c_row = abs(d_row - s_row);
	int c_col = abs(d_col - s_col);

	//Diagonal
	if (c_row != c_col)
	{
		return false;
	}

	//Validating Jump
	int rowStep = (d_row > s_row) ? 1 : -1;
	int colStep = (d_col > s_col) ? 1 : -1;
	int r = s_row + rowStep;
	int c = s_col + colStep;
	while (r != d_row && c != d_col)
	{
		if (board->getPiece(r, c) != nullptr)
		{
			return false;
		}
		r += rowStep;
		c += colStep;
	}
	return true;
}
//Queen::valid Definition
bool Queen::valid(int s_row, int s_col, int d_row, int d_col, Board* board)
{
	int c_row = abs(d_row - s_row);
	int c_col = abs(d_col - s_col);

	//2D Movement
	if (c_col == 0)
	{
		if (s_row > d_row)
			swap(s_row, d_row);

		for (int i = s_row + 1; i < d_row; i++)
		{
			if (board->getPiece(i, s_col) != nullptr)
				return false;
		}
		return true;
	}
	if (c_row == 0)
	{
		if (s_col > d_col)
			swap(s_col, d_col);

		for (int i = s_col + 1; i < d_col; i++)
		{
			if (board->getPiece(s_row, i) != nullptr)
				return false;
		}
		return true;
	}

	//Diagonal
	if (c_row == c_col)
	{
		int rowStep = (d_row > s_row) ? 1 : -1;
		int colStep = (d_col > s_col) ? 1 : -1;

		int r = s_row + rowStep;
		int c = s_col + colStep;

		while (r != d_row && c != d_col)
		{
			if (board->getPiece(r, c) != nullptr)
				return false;

			r += rowStep;
			c += colStep;
		}

		return true;
	}

	return false;
}
//King::valid Definition
bool King::valid(int s_row, int s_col, int d_row, int d_col, Board* board)
{
	int c_row = abs(d_row - s_row);
	int c_col = abs(d_col - s_col);

	if (c_row == 0 && c_col == 0)
	{
		return false;
	}
	return (c_row <= 1 && c_col <= 1);
}

int main()
{
	enableANSI();

	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	system("chcp 65001 > nul");

	ios::sync_with_stdio(false);

	Board b;
	b.startScreen();
	b.start();

	return 0;
}