#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>

using namespace std;

//COLORS LIST
//1: Blue
//2: Green
//3: Cyan
//4: Red
//5: Purple
//6: Yellow (Dark)
//7: Default white
//8: Gray/Grey
//9: Bright blue
//10: Brigth green
//11: Bright cyan
//12: Bright red
//13: Pink/Magenta
//14: Yellow
//15: Bright white
//Numbers after 15 include background colors

//	  background colors
//    0 = Black       8 = Gray
//    1 = Blue        9 = Light Blue
//    2 = Green       A = Light Green
//    3 = Aqua        B = Light Aqua
//    4 = Red         C = Light Red
//    5 = Purple      D = Light Purple
//    6 = Yellow      E = Light Yellow
//    7 = White       F = Bright White

class checker
{
	public:
		//color if each item
		char color;
		//an array of game to show to the gamer
		string board[8][8];
		//an integer to indicate whether the cell is free or not
		int free;
		//declaring constructor
		checker();
};

class game
{
	public:
		void show(checker [8][8]);	
		void gotoxy(int, int);
		void hide_cursor();
};

class king : public checker
{
	private:
		int position_x;
		int position_y;
	public:
		//check if the destination is in range of move
		bool check_1(checker, int, int);
		//check if there is an obstacle in the middle
		bool check_2(checker, int, int);	
		//check if the friendly king gets checked
		bool check_3(checker, int, int);
		//moving to new place
		void move(checker, int, int);
};

class pawn : public checker
{
	private:
		int position_x;
		int position_y;
	public:
		//check if the destination is in range of move
		bool check_1(checker, int, int);
		//check if there is an obstacle in the middle
		bool check_2(checker, int, int);	
		//check if the friendly king gets checked
		bool check_3(checker, int, int);		
		//moving to new place
		void move(checker, int, int);
};

class knight : public checker
{
	private:
		int position_x;
		int position_y;
	public:
		//check if the destination is in range of move
		bool check_1(checker, int, int);
		//check if there is an obstacle in the middle
		bool check_2(checker, int, int);	
		//check if the friendly king gets checked
		bool check_3(checker, int, int);		
		//moving to new place
		void move(checker, int, int);
};

class queen : public checker
{
	private:
		int position_x;
		int position_y;
	public:
		//check if the destination is in range of move
		bool check_1(checker, int, int);
		//check if there is an obstacle in the middle
		bool check_2(checker, int, int);	
		//check if the friendly king gets checked
		bool check_3(checker, int, int);	
		//moving to new place
		void move(checker, int, int);
};

class bishop : public checker
{
	private:
		int position_x;
		int position_y;
	public:
		//check if the destination is in range of move
		bool check_1(checker, int, int);
		//check if there is an obstacle in the middle
		bool check_2(checker, int, int);	
		//check if the friendly king gets checked
		bool check_3(checker, int, int);
		//moving to new place
		void move(checker, int, int);
};

class rook : public checker
{
	private:
		int position_x;
		int position_y;
	public:
		//check if the destination is in range of move
		bool check_1(checker, int, int);
		//check if there is an obstacle in the middle
		bool check_2(checker, int, int);	
		//check if the friendly king gets checked
		bool check_3(checker, int, int);
		//moving to new place
		void move(checker, int, int);
};

//show the game board
void game::show(checker game_board[8][8])
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	int i, j;
	for(i = 0; i < 8; i++)
	{
		cout << "\t(" << char(i + 65) << ")";
	}
	cout << endl;
	for(i = 0; i < 8; i++)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		cout << endl << endl << "(" << i + 1 << ")";
		for(j = 0; j < 8; j++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), game_board[i][j].color == 'B' ? 3 : 7);
			game_board[i][j].free = game_board[i][j].board[i][j] == "" ? true : false;
			cout << "\t" << game_board[i][j].board[i][j][1];
		}
		cout << endl;
	}
}

//change cursor position
void game::gotoxy(int x, int y)
{
    COORD p = { x, y };
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), p );
}

//checker constructor
//initialize every cell of the board array to free = true
checker::checker()
{
	int i, j;
	//initializing white positions
	//'Z' stands for king, 'K' stands for knight
	board[0][0] = "WR";
	board[0][7] = "WR";
	board[0][1] = "WK";
	board[0][6] = "WK";
	board[0][2] = "WB";
	board[0][5] = "WB";
	board[0][3] = "WQ";
	board[0][4] = "WZ";
	for(i = 0; i < 8; i++)
	{
		board[1][i] = "WP";
	}
	//initializing black positions
	//'Z' stands for king, 'K' stands for knight
	board[7][0] = "BR";
	board[7][7] = "BR";
	board[7][1] = "BK";
	board[7][6] = "BK";
	board[7][2] = "BB";
	board[7][5] = "BB";
	board[7][3] = "BQ";
	board[7][4] = "BZ";
	for(i = 0; i < 8; i++)
	{
		board[6][i] = "BP";
	}
	//setting other sells to empty string
	for(i = 2; i < 6; i++)
	{
		for(j = 0; j < 8; j++)
		{
			board[i][j] = "";
		}
	}
}

//hide mouse cursor
void game::hide_cursor()
{
	HANDLE hOut;
	CONSOLE_CURSOR_INFO ConCurInf;

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	ConCurInf.dwSize = 10;
	ConCurInf.bVisible = FALSE;

	SetConsoleCursorInfo(hOut, &ConCurInf);
}

int main()
{
	//declare game object
	game GAME;
	//declare an 8 * 8 = 64 array object of class checker to control the game 
	checker game_board[8][8];
	int i, j;
	//initialize the amounts of game_board[i][j].color
	for(i = 0; i < 8; i++)
	{
		game_board[0][i].color = 'W';
		game_board[1][i].color = 'W';
		game_board[6][i].color = 'B';
		game_board[7][i].color = 'B';
		for(j = 2; j < 6; j++)
		{
			game_board[j][i].color = ' ';
		}
	}
	//hide mouse cursor
	GAME.hide_cursor();
	bool end_game = false;
	while(!end_game)
	{
		GAME.show(game_board);
		//system("CLS");
		GAME.gotoxy(0, 0);
	}
	getch();
	return 0;
}
