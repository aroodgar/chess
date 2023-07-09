
#include <iostream>
#include <string>
#include <math.h>
#include <conio.h>
#include <windows.h>
#include <SFML/Network.hpp>

using namespace std;
using namespace sf;
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
//10: Bright green
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

typedef struct{
    int x;
    int y;
} place;

place emperor_black;
place emperor_white;
int aiturn = 0;

typedef struct{
    place p_mate;
    int flag_mate;
} pl_fl; //place_flag

typedef struct{
    place from;
    place to;
}AImove;

typedef struct{
    place from;
    place to;
    int score;
}AIreturn;

class checker{
public:
    //color of each item
    char color;
    //an integer to indicate whether the cell is free or not
    int free;
    //id
    char id;
    // rook and emperor's hasMoved variable. false means they haven't moved yet and true means they have moved
    bool hasMoved;

};

class AI{
    public:
        AImove AIsearch(checker[8][8]);
        AIreturn AIpawn(checker[8][8], place);
        AIreturn AIrook(checker[8][8], place);
        AIreturn AIknight(checker[8][8], place);
        AIreturn AIbishop(checker[8][8], place);
        AIreturn AIqueen(checker[8][8], place);
        AIreturn AIemperor(checker[8][8], place);
        void AIthink(checker[8][8], place, place, AIreturn &);
        void AI_check_check(checker[8][8], place, place, int &);
        int AIswitchScore(checker[8][8], place, place);
        int AIswitchScoreNegative(checker[8][8], place);
    };

class game{
public:
    void show(checker[8][8], place);
    void gotoxy(int, int);
    void hide_cursor();
    //set main board
    void start(checker[8][8]);
    //moving to new place
    void move(checker[8][8], place, place);
    place get_key(place, checker[8][8]);
    place go_left(place, checker[8][8]);
    place go_up(place, checker[8][8]);
    place go_down(place, checker[8][8]);
    place go_right(place, checker[8][8]);
    bool if_invalid(checker[8][8], place, place, place);
    int searchCount(checker[8][8], char);
};

class emperor : public checker{
private:
    place position;

public:
    //check if the destination is in range of move
    bool check_1(checker[8][8], place, place);
    //check if there is an obstacle in the middle
    bool check_2(checker[8][8], place, place);
    //check final
    bool check(checker[8][8], place, place);
    //check for being checked
    pl_fl check_check(checker[8][8], place, bool);
    //check for check mate
    bool check_mate(checker[8][8], place, int);
    // Castling
    bool castle(checker[8][8], place, place);
    //check for invalid move
    bool invalid(checker[8][8], place, place);
};

class pawn : public checker{
private:
    place position;

public:
    //check if the destination is in range of move
    int check_1(checker[8][8], place, place);
    //check if there is an obstacle in the middle
    bool check_2(checker[8][8], place, place, int);
    //check final
    bool check(checker[8][8], place, place);
    //check for invalid move
    bool invalid(checker[8][8], place, place);
};

class knight : public checker{
private:
    place position;

public:
    //check if the destination is in range of move
    bool check_1(checker[8][8], place, place);
    //check if there is an obstacle in the middle
    bool check_2(checker[8][8], place, place);
    //check final
    bool check(checker[8][8], place, place);
    //check for invalid move
    bool invalid(checker[8][8], place, place);
};

class queen : public checker{
private:
    place position;

public:
    //check if the destination is in range of move
    bool check_1(checker[8][8], place, place);
    //check if there is an obstacle in the middle
    bool check_2(checker[8][8], place, place);
    //check final
    bool check(checker[8][8], place, place);
    //check for invalid move
    bool invalid(checker[8][8], place, place);
};

class bishop : public checker{
private:
    place position;

public:
    //check if the destination is in range of move
    bool check_1(checker[8][8], place, place);
    //check if there is an obstacle in the middle
    bool check_2(checker[8][8], place, place);
    //check final
    bool check(checker[8][8], place, place);
    //check for invalid move
    bool invalid(checker[8][8], place, place);
};

class rook : public checker{
private:
    place position;

public:
    //check if the destination is in range of move
    bool check_1(checker[8][8], place, place);
    //check if there is an obstacle in the middle
    bool check_2(checker[8][8], place, place);
    //check final
    bool check(checker[8][8], place, place);
    //check for invalid move
    bool invalid(checker[8][8], place, place);
};

//hide mouse cursor
void game::hide_cursor(){
    HANDLE hOut;
    CONSOLE_CURSOR_INFO ConCurInf;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    ConCurInf.dwSize = 20;
    ConCurInf.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &ConCurInf);
}

//show the game board
void game::show(checker game_board[8][8], place star_position){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
    int i, j;
    for (i = 0; i < 8; i++){
        cout << "\t(" << char(i + 65) << ")";
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    cout << endl
         << endl;
    cout << "      ";

    for (i = 0; i < 64; i++){
        cout << "#";
    }
    cout << endl
         << "      |";
    for (i = 0; i < 8; i++){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
        for (j = 0; j < 8; j++)
            cout << "\t     |";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
        cout << endl
             << "(" << i + 1 << ")";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
        cout << "   |";
        for (j = 0; j < 8; j++){
            if (i == star_position.y && j == star_position.x){

                game_board[i][j].free = game_board[i][j].id == ' ' ? true : false;
                cout << "\t";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), game_board[i][j].color == 'B' ? 3 : 7);
                game_board[i][j].free = game_board[i][j].id == ' ' ? true : false;
                cout << game_board[i][j].id;


                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
                cout << " *";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
                cout << "  |";
            }
            else{
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), game_board[i][j].color == 'B' ? 3 : 7);
                game_board[i][j].free = game_board[i][j].id == ' ' ? true : false;
                cout << "\t " << game_board[i][j].id;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
                cout << "   |";
            }
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
        cout << endl;
        cout << "      |";
        for (j = 0; j < 8; j++)
            cout << "\t     |";
        cout << endl;
        cout << "      ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
        for (j = 0; j < 64; j++){
            cout << "#";
        }
        if (i != 7)
            cout << endl
                 << "      |";
    }
    game GAME;
    GAME.hide_cursor();
}

//change cursor position
void game::gotoxy(int x, int y){
    COORD p = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}

void game::start(checker game_board[8][8]){
    rook r[4];
    bishop b[4];
    knight k[4];
    queen q[2];
    emperor e[2];
    pawn p[16];
    int i;
    for (i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            game_board[i][j].hasMoved = false;
        }
    }
    //set empty spot
    for (i = 2; i < 6; i++){
        for (int j = 0; j < 8; j++){
            game_board[i][j].free = 1;
            game_board[i][j].id = ' ';
            game_board[i][j].color = ' ';
        }
    }
    //rook
    for (i = 0; i < 2; i++){
        r[i].color = 'W';
        r[i].free = 0;
        r[i].id = 'R';
    }
    for (i = 2; i < 4; i++){
        r[i].color = 'B';
        r[i].free = 0;
        r[i].id = 'R';
    }
    //bishop
    for (i = 0; i < 2; i++){
        b[i].color = 'W';
        b[i].free = 0;
        b[i].id = 'B';
    }
    for (i = 2; i < 4; i++)
    {
        b[i].color = 'B';
        b[i].free = 0;
        b[i].id = 'B';
    }
    //knight
    for (i = 0; i < 2; i++){
        k[i].color = 'W';
        k[i].free = 0;
        k[i].id = 'K';
    }
    for (i = 2; i < 4; i++){
        k[i].color = 'B';
        k[i].free = 0;
        k[i].id = 'K';
    }
    //queen
    for (i = 0; i < 1; i++){
        q[i].color = 'W';
        q[i].free = 0;
        q[i].id = 'Q';
    }
    for (i = 1; i < 2; i++){
        q[i].color = 'B';
        q[i].free = 0;
        q[i].id = 'Q';
    }
    //emperor
    for (i = 0; i < 1; i++){
        e[i].color = 'W';
        e[i].free = 0;
        e[i].id = 'E';
    }
    for (i = 1; i < 2; i++){
        e[i].color = 'B';
        e[i].free = 0;
        e[i].id = 'E';
    }
    //pawn
    for (i = 0; i < 8; i++){
        p[i].color = 'W';
        p[i].free = 0;
        p[i].id = 'P';
    }
    for (i = 8; i < 16; i++){
        p[i].color = 'B';
        p[i].free = 0;
        p[i].id = 'P';
    }
    //set pawn
    for (i = 0; i < 8; i++){
        game_board[1][i] = p[i];
    }
    for (i = 0; i < 8; i++){
        game_board[6][i] = p[i + 8];
    }
    //set rook
    game_board[0][0] = r[0];
    game_board[0][7] = r[1];
    game_board[7][0] = r[2];
    game_board[7][7] = r[3];
    //set knight
    game_board[0][1] = k[0];
    game_board[0][6] = k[1];
    game_board[7][1] = k[2];
    game_board[7][6] = k[3];
    //set bishop
    game_board[0][2] = b[0];
    game_board[0][5] = b[1];
    game_board[7][2] = b[2];
    game_board[7][5] = b[3];
    //set emperor
    game_board[0][4] = e[0];
    game_board[7][4] = e[1];
    //set queen
    game_board[0][3] = q[0];
    game_board[7][3] = q[1];
}

bool rook::check_1(checker game_board[8][8], place from, place to){
    //if horizontal
    if (abs(to.y - from.y) == 0 && abs(to.x - from.x) > 0){
        return 1;
    }
    //if vertical
    else if (abs(to.y - from.y) > 0 && abs(to.x - from.x) == 0){
        return 1;
    }
    //if invalid move
    else{
        return 0;
    }
}

bool rook::check_2(checker game_board[8][8], place from, place to){
    //if horizontal
    if (to.y - from.y == 0){
        if (to.x == from.x){
            return 0;
        }
        else if (to.x > from.x){
            //is there any obstacle
            for (int i = from.x + 1; i < to.x; i++){
                if (game_board[from.y][i].free == 0){
                    return 0;
                }
            }
            //is the destination free, if not is it friendly or not
            if (game_board[from.y][to.x].free == 0 && game_board[from.y][to.x].color == game_board[from.y][from.x].color){
                return 0;
            }
            else{
                return 1;
            }
        }
        else{
            //is there any obstacle
            for (int i = from.x - 1; i > to.x; i--){
                if (game_board[from.y][i].free == 0){
                    return 0;
                }
            }
            //is the destination free, if not is it friendly or not
            if (game_board[from.y][to.x].free == 0 && game_board[from.y][to.x].color == game_board[from.y][from.x].color){
                return 0;
            }
            else{
                return 1;
            }
        }
    }
    //if vertical
    else{
        if (to.y == from.y){
            return 0;
        }
        else if (to.y > from.y){
            //is there any obstacle
            for (int i = from.y + 1; i < to.y; i++){
                if (game_board[i][from.x].free == 0){
                    return 0;
                }
            }
            //is the destination free, if not is it friendly or not
            if (game_board[to.y][from.x].free == 0 && game_board[to.y][from.x].color == game_board[from.y][from.x].color){
                return 0;
            }
            else{
                return 1;
            }
        }
        else{
            //is there any obstacle
            for (int i = from.y - 1; i > to.y; i--){
                if (game_board[i][from.x].free == 0){
                    return 0;
                }
            }
            //is the destination free, if not is it friendly or not
            if (game_board[to.y][from.x].free == 0 && game_board[to.y][from.x].color == game_board[from.y][from.x].color){
                return 0;
            }
            else{
                return 1;
            }
        }
    }
}

bool bishop::check_1(checker game_board[8][8], place from, place to){
    if (abs(to.y - from.y) == abs(to.x - from.x)){
        return 1;
    }
    else{
        return 0;
    }
}

bool bishop::check_2(checker game_board[8][8], place from, place to){
    //if there is any obstacle
    if (to.x > from.x && to.y > from.y){
        for (int i = from.y + 1, j = from.x + 1; i < to.y && j < to.x; i++, j++){
            if (game_board[i][j].free == 0){
                return 0;
            }
        }
    }
    else if (to.x < from.x && to.y > from.y){
        for (int i = from.y + 1, j = from.x - 1; i < to.y && j > to.x; i++, j--){
            if (game_board[i][j].free == 0){
                return 0;
            }
        }
    }
    else if (to.x > from.x && to.y < from.y){
        for (int i = from.y - 1, j = from.x + 1; i > to.y && j < to.x; i--, j++){
            if (game_board[i][j].free == 0){
                return 0;
            }
        }
    }
    else{
        for (int i = from.y - 1, j = from.x - 1; i < to.y && j < to.x; i--, j--){
            if (game_board[i][j].free == 0){
                return 0;
            }
        }
    }
    //is the destination free, if yes is it friendly or not
    if (game_board[to.y][to.x].color == game_board[from.y][from.x].color && game_board[to.y][to.x].free == 0){
        return 0;
    }
    else{
        return 1;
    }
}

int pawn::check_1(checker game_board[8][8], place from, place to){
    if (game_board[from.y][from.x].color == 'W'){
        if (abs(to.y - from.y) > 2 || (to.y - from.y) < 0 || abs(to.x - from.x) > 1){
            return 0;
        }
        if ((to.y - from.y) == 2 && abs(to.x - from.x) == 0){
            if (from.y == 1){
                //if the player wants to move 2 vertically and has the permission
                return 3;
            }
            else{
                return 0;
            }
        }
        else if ((to.y - from.y) == 1 && (to.x - from.x) == 0){
            return 1;
        }
        else if (to.y - from.y == 1 && abs(to.x - from.x) == 1){
            return 2;
        }
    }
    //Black
    else
    {
        if (abs(to.y - from.y) > 2 || (to.y - from.y) > 0 || abs(to.x - from.x) > 1){
            return 0;
        }
        if ((to.y - from.y) == -2 && abs(to.x - from.x) == 0){
            if (from.y == 6)
                return 3;
            else{
                return 0;
            }
        }
        else if ((to.y - from.y) == -1 && (to.x - from.x) == 0){
            return 1;
        }
        else if (to.y - from.y == -1 && abs(to.x - from.x) == 1){
            return 2;
        }
    }
    return 0;
}

bool pawn::check_2(checker game_board[8][8], place from, place to, int flag){
    //player wants to move 2 vertically
    if (flag == 3){
        //obstacles in the way
        if (game_board[to.y - 1][to.x].free == 1 && game_board[to.y][to.x].free == 1 && game_board[from.y][from.x].color == 'W'){
            return 1;
        }
        else if (game_board[to.y + 1][to.x].free == 1 && game_board[to.y][to.x].free == 1 && game_board[from.y][from.x].color == 'B'){
            return 1;
        }
        else{
            return 0;
        }
    }
    //move 1 vertically and 1 horizontally
    else if (flag == 2){
        if (game_board[to.y][to.x].free == 0 && game_board[to.y][to.x].color != game_board[from.y][from.x].color){
            return 1;
        }
        else{
            return 0;
        }
    }
    //move 1 vertically
    else if (flag == 1){
        if (game_board[to.y][to.x].free == 1){
            return 1;
        }
        else{
            return 0;
        }
    }
}

void game::move(checker game_board[8][8], place from, place to){
    if (game_board[to.y][to.x].free == 0){
        game_board[to.y][to.x] = game_board[from.y][from.x];
        game_board[from.y][from.x].id = ' ';
        game_board[from.y][from.x].color = ' ';
        game_board[from.y][from.x].free = 1;
    }
    else{
        checker temp;
        temp = game_board[to.y][to.x];
        game_board[to.y][to.x] = game_board[from.y][from.x];
        game_board[from.y][from.x] = temp;
    }
}

place game::get_key(place star_position, checker game_board[8][8]){
    char key = getch();
    while (key != 13)
    {
        switch (key)
        {
        case 'a':
            star_position = go_left(star_position, game_board);
            break;
        case 'w':
            star_position = go_up(star_position, game_board);
            break;
        case 'd':
            star_position = go_right(star_position, game_board);
            break;
        case 's':
            star_position = go_down(star_position, game_board);
            break;
        }
        key = getch();
    }
    return star_position;
}

place game::go_left(place star_position, checker game_board[8][8]){
    game GAME;
    if (star_position.x == 0){
        star_position.x = 7;
    }
    else{
        star_position.x--;
    }
    GAME.gotoxy(0, 0);
    GAME.show(game_board, star_position);
    return star_position;
}

place game::go_right(place star_position, checker game_board[8][8]){
    game GAME;
    if (star_position.x == 7){
        star_position.x = 0;
    }
    else{
        star_position.x++;
    }
    GAME.gotoxy(0, 0);
    GAME.show(game_board, star_position);
    return star_position;
}

place game::go_up(place star_position, checker game_board[8][8]){
    game GAME;
    if (star_position.y == 0){
        star_position.y = 7;
    }
    else{
        star_position.y--;
    }
    GAME.gotoxy(0, 0);
    GAME.show(game_board, star_position);
    return star_position;
}

place game::go_down(place star_position, checker game_board[8][8]){
    game GAME;
    if (star_position.y == 7){
        star_position.y = 0;
    }
    else{
        star_position.y++;
    }
    GAME.gotoxy(0, 0);
    GAME.show(game_board, star_position);
    return star_position;
}

bool knight::check_1(checker game_board[8][8], place from, place to){
    if (abs(to.y - from.y) == 2 && abs(to.x - from.x) == 1){
        return 1;
    }
    if (abs(to.x - from.x) == 2 && abs(to.y - from.y) == 1){
        return 1;
    }
    else{
        return 0;
    }
}

bool knight::check_2(checker game_board[8][8], place from, place to){
    //checking for obstacle in destination
    if (game_board[to.y][to.x].free == 0 && game_board[to.y][to.x].color == game_board[from.y][from.x].color){
        return 0;
    }
    else{
        return 1;
    }
}

bool emperor::check_1(checker game_board[8][8], place from, place to){
    //if invalid
    if (abs(to.y - from.y) > 1 || abs(to.x - from.x) > 1){
        return 0;
    }
    return 1;
}

bool emperor::check_2(checker game_board[8][8], place from, place to){
    //if friendly or not
    if (game_board[to.y][to.x].color == game_board[from.y][from.x].color){
        return 0;
    }
    //if the destination is adjacent to enemy emperor
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (game_board[i][j].id == 'E' && game_board[i][j].color != game_board[from.y][from.x].color && abs(to.y - i) <= 1 && abs(to.x - j) <= 1){
                return 0;
            }
        }
    }
    return 1;
}

pl_fl emperor::check_check(checker game_board[8][8], place from, bool pawn_situation = 1){
    //returns 1 when it is not check and 0 when it is
    int i, j = from.x;
    int flag = 0;
    place p;
    pl_fl place_flag;
    place_flag.flag_mate = 0;
    //checking down vertical
    if ( from.y != 7 ){
        for (i = from.y + 1; (game_board[i][from.x].id == ' ' || (game_board[from.y][from.x].color == game_board[i][from.x].color && game_board[i][from.x].id == 'E')) && i < 7; i++);
        if (game_board[i][from.x].color != game_board[from.y][from.x].color){
            if (game_board[i][from.x].id == 'R' || game_board[i][from.x].id == 'Q'){
                p.y = i;
                p.x = from.x;
                flag++;
                place_flag.flag_mate = flag;
                place_flag.p_mate = p;
            }
        }
    }
    //checking up vertical
    if ( from.y != 0 ){
        for (i = from.y - 1; (game_board[i][from.x].id == ' ' || (game_board[from.y][from.x].color == game_board[i][from.x].color && game_board[i][from.x].id == 'E')) && i > 0; i--);
        if (game_board[i][from.x].color != game_board[from.y][from.x].color){
            if (game_board[i][from.x].id == 'R' || game_board[i][from.x].id == 'Q'){
                p.y = i;
                p.x = from.x;
                flag++;
                place_flag.flag_mate = flag;
                place_flag.p_mate = p;
            }
        }
    }
    //checking right horizontal
    if ( from.x != 7 ) {
    for (i = from.x + 1; (game_board[from.y][i].id == ' ' || (game_board[from.y][from.x].color == game_board[from.y][i].color && game_board[from.y][i].id == 'E')) && i < 7; i++);
    if (game_board[from.y][i].color != game_board[from.y][from.x].color){
        if (game_board[from.y][i].id == 'R' || game_board[from.y][i].id == 'Q'){
            p.y = from.y;
            p.x = i;
            flag++;
            place_flag.flag_mate = flag;
            place_flag.p_mate = p;
        }
    }
    }
    //checking left horizontal
    if ( from.x != 0 ){
    for (i = from.x - 1; (game_board[from.y][i].id == ' ' || (game_board[from.y][from.x].color == game_board[from.y][i].color && game_board[from.y][i].id == 'E')) && i > 0; i--);
    if (game_board[from.y][i].color != game_board[from.y][from.x].color){
        if (game_board[from.y][i].id == 'R' || game_board[from.y][i].id == 'Q'){
            p.y = from.y;
            p.x = i;
            flag++;
            place_flag.flag_mate = flag;
            place_flag.p_mate = p;
        }
    }
    }
    //checking for pawns
    //1 -> diagonal
    //0 -> straight
    if (pawn_situation == 1){
        if(game_board[from.y][from.x].color == 'W'){
        if (game_board[from.y + 1][from.x + 1].id == 'P' && game_board[from.y + 1][from.x + 1].color != game_board[from.y][from.x].color){
            p.y = i;
            p.x = j;
            flag++;
            place_flag.flag_mate = flag;
            place_flag.p_mate = p;
        }
        if (game_board[from.y + 1][from.x - 1].id == 'P' && game_board[from.y + 1][from.x - 1].color != game_board[from.y][from.x].color){
            p.y = i;
            p.x = j;
            flag++;
            place_flag.flag_mate = flag;
            place_flag.p_mate = p;
        }
        }
        else if(game_board[from.y][from.x].color == 'B'){
        if (game_board[from.y - 1][from.x - 1].id == 'P' && game_board[from.y - 1][from.x - 1].color != game_board[from.y][from.x].color){
            p.y = i;
            p.x = j;
            flag++;
            place_flag.flag_mate = flag;
            place_flag.p_mate = p;
        }
        if (game_board[from.y - 1][from.x + 1].id == 'P' && game_board[from.y - 1][from.x + 1].color != game_board[from.y][from.x].color){
            p.y = i;
            p.x = j;
            flag++;
            place_flag.flag_mate = flag;
            place_flag.p_mate = p;
        }
        }
    }
    else{
        if(game_board[from.y][from.x].color == 'W'){
        if (game_board[from.y + 1][from.x].id == 'P' && game_board[from.y + 1][from.x].color != game_board[from.y][from.x].color){
            p.y = i;
            p.x = j;
            flag++;
            place_flag.flag_mate = flag;
            place_flag.p_mate = p;
        }
        if (game_board[from.y + 1][from.x].id == 'P' && game_board[from.y + 1][from.x].color != game_board[from.y][from.x].color){
            p.y = i;
            p.x = j;
            flag++;
            place_flag.flag_mate = flag;
            place_flag.p_mate = p;
        }
        }
        else if(game_board[from.y][from.x].color == 'B'){
        if (game_board[from.y - 1][from.x].id == 'P' && game_board[from.y - 1][from.x].color != game_board[from.y][from.x].color){
            p.y = i;
            p.x = j;
            flag++;
            place_flag.flag_mate = flag;
            place_flag.p_mate = p;
        }
        if (game_board[from.y - 1][from.x].id == 'P' && game_board[from.y - 1][from.x].color != game_board[from.y][from.x].color){
            p.y = i;
            p.x = j;
            flag++;
            place_flag.flag_mate = flag;
            place_flag.p_mate = p;
        }
        }
    }
    //checking for knights
    if (game_board[from.y + 2][from.x + 1].id == 'K' && game_board[from.y + 2][from.x + 1].color != game_board[from.y][from.x].color){
        p.y = i;
        p.x = j;
        flag++;
        place_flag.flag_mate = flag;
        place_flag.p_mate = p;
    }
    if (game_board[from.y + 2][from.x - 1].id == 'K' && game_board[from.y + 2][from.x - 1].color != game_board[from.y][from.x].color){
        p.y = i;
        p.x = j;
        flag++;
        place_flag.flag_mate = flag;
        place_flag.p_mate = p;
    }
    if (game_board[from.y - 2][from.x + 1].id == 'K' && game_board[from.y - 2][from.x + 1].color != game_board[from.y][from.x].color){
        p.y = i;
        p.x = j;
        flag++;
        place_flag.flag_mate = flag;
        place_flag.p_mate = p;
    }
    if (game_board[from.y - 2][from.x - 1].id == 'K' && game_board[from.y - 2][from.x - 1].color != game_board[from.y][from.x].color){

        p.y = i;
        p.x = j;
        flag++;
        place_flag.flag_mate = flag;
        place_flag.p_mate = p;
    }

    if (game_board[from.y + 1][from.x + 2].id == 'K' && game_board[from.y + 1][from.x + 2].color != game_board[from.y][from.x].color){
        p.y = i;
        p.x = j;
        flag++;
        place_flag.flag_mate = flag;
        place_flag.p_mate = p;
    }
    if (game_board[from.y + 1][from.x - 2].id == 'K' && game_board[from.y + 1][from.x - 2].color != game_board[from.y][from.x].color){
        p.y = i;
        p.x = j;
        flag++;
        place_flag.flag_mate = flag;
        place_flag.p_mate = p;
    }

    if (game_board[from.y - 1][from.x + 2].id == 'K' && game_board[from.y - 1][from.x + 2].color != game_board[from.y][from.x].color){
        p.y = i;
        p.x = j;
        flag++;
        place_flag.flag_mate = flag;
        place_flag.p_mate = p;
    }

    if (game_board[from.y - 1][from.x - 2].id == 'K' && game_board[from.y - 1][from.x - 2].color != game_board[from.y][from.x].color){
        p.y = i;
        p.x = j;
        flag++;
        place_flag.flag_mate = flag;
        place_flag.p_mate = p;
    }
    //CHECKING for bishop and queen
    //down-right
    if ( from.x != 7 && from.y != 7 ){
    for (i = from.x + 1, j = from.y + 1; (game_board[j][i].id == ' ' || game_board[from.y][from.x].color == game_board[j][i].color && game_board[j][i].id == 'E') && (i < 7 && j < 7); i++, j++);
    if (game_board[j][i].color != game_board[from.y][from.x].color){
        if (game_board[j][i].id == 'B' || game_board[j][i].id == 'Q'){
            p.y = i;
            p.x = j;
            flag++;
            place_flag.flag_mate = flag;
            place_flag.p_mate = p;
        }
    }
    }
    //down-left
    if ( from.x != 0 && from.y != 7 ){
    for (i = from.x - 1, j = from.y + 1; (game_board[j][i].id == ' ' || game_board[from.y][from.x].color == game_board[j][i].color && game_board[j][i].id == 'E') && (i > 0 && j < 7); i--, j++);
    if (game_board[j][i].color != game_board[from.y][from.x].color){
        if (game_board[j][i].id == 'B' || game_board[j][i].id == 'Q'){
            p.y = i;
            p.x = j;
            flag++;
            place_flag.flag_mate = flag;
            place_flag.p_mate = p;
        }
    }
    }
    //up-right
    if ( from.x != 7 && from.y != 0 ){
    for (i = from.x + 1, j = from.y - 1; (game_board[j][i].id == ' ' || game_board[from.y][from.x].color == game_board[j][i].color && game_board[j][i].id == 'E') && (i < 7 && j > 0); i++, j--);
    if (game_board[j][i].color != game_board[from.y][from.x].color){
        if (game_board[j][i].id == 'B' || game_board[j][i].id == 'Q'){
            p.y = i;
            p.x = j;
            flag++;
            place_flag.flag_mate = flag;
            place_flag.p_mate = p;
        }
    }
    }
    //up-left
    if ( from.x != 0 && from.y != 0 ){
    for (i = from.x - 1, j = from.y - 1; (game_board[j][i].id == ' ' || game_board[from.y][from.x].color == game_board[j][i].color && game_board[j][i].id == 'E') && (i > 0 && j > 0); i--, j--);
    if (game_board[j][i].color != game_board[from.y][from.x].color){
        if (game_board[j][i].id == 'B' || game_board[j][i].id == 'Q'){
            p.y = i;
            p.x = j;
            flag++;
            place_flag.flag_mate = flag;
            place_flag.p_mate = p;
        }
    }
    }
    if (flag > 1)
        place_flag.p_mate.x = -1;
    return place_flag;
}

bool queen::check_1(checker game_board[8][8], place from, place to){
    //if horizontal
    if (abs(to.y - from.y) == 0 && abs(to.x - from.x) > 0){
        return 1;
    }
    //if vertical
    if (abs(to.y - from.y) > 0 && abs(to.x - from.x) == 0){
        return 1;
    }
    //if diagonal
    if (abs(to.y - from.y) == abs(to.x - from.x)){
        return 1;
    }
    //if invalid
    return 0;
}

bool queen::check_2(checker game_board[8][8], place from, place to){
    //if horizontal
    if (to.y - from.y == 0){
        if (to.x == from.x){
            return 0;
        }
        else if (to.x > from.x){
            //is there any obstacle
            for (int i = from.x + 1; i < to.x; i++){
                if (game_board[from.y][i].free == 0){
                    return 0;
                }
            }
            //is the destination free, if not is it friendly or not
            if (game_board[from.y][to.x].free == 0 && game_board[from.y][to.x].color == game_board[from.y][from.x].color){
                return 0;
            }
            else{
                return 1;
            }
        }
        else{
            //is there any obstacle
            for (int i = from.x - 1; i > to.x; i--){
                if (game_board[from.y][i].free == 0){
                    return 0;
                }
            }
            //is the destination free, if not is it friendly or not
            if (game_board[from.y][to.x].free == 0 && game_board[from.y][to.x].color == game_board[from.y][from.x].color){
                return 0;
            }
            else{
                return 1;
            }
        }
    }
    //if vertical
    else if (to.x - from.x == 0){
        if (to.y == from.y){
            return 0;
        }
        else if (to.y > from.y){
            //is there any obstacle
            for (int i = from.y + 1; i < to.y; i++){
                if (game_board[i][from.x].free == 0){
                    return 0;
                }
            }
            //is the destination free, if not is it friendly or not
            if (game_board[to.y][from.x].free == 0 && game_board[to.y][from.x].color == game_board[from.y][from.x].color){;
                return 0;
            }
            else{
                return 1;
            }
        }
        else{
            //is there any obstacle
            for (int i = from.y - 1; i > to.y; i--){
                if (game_board[i][from.x].free == 0){
                    return 0;
                }
            }
            //is the destination free, if not is it friendly or not
            if (game_board[to.y][from.x].free == 0 && game_board[to.y][from.x].color == game_board[from.y][from.x].color){
                    return 0;
            }
            else{
                return 1;
            }
        }
    }
    //diagonal move
    else{
        //if there is any obstacle
        if (to.x > from.x){    //main game loop && to.y > from.y){
            for (int i = from.y + 1, j = from.x + 1; i < to.y && j < to.x; i++, j++){
                if (game_board[i][j].free == 0){
                    return 0;
                }
            }
        }
        else if (to.x < from.x && to.y > from.y){
            for (int i = from.y + 1, j = from.x - 1; i < to.y && j > to.x; i++, j--){
                if (game_board[i][j].free == 0){
                    return 0;
                }
            }
        }
        else if (to.x > from.x && to.y < from.y){
            for (int i = from.y - 1, j = from.x + 1; i > to.y && j < to.x; i--, j++){
                if (game_board[i][j].free == 0){
                    return 0;
                }
            }
        }
        else{
            for (int i = from.y - 1, j = from.x - 1; i < to.y && j < to.x; i--, j--){
                if (game_board[i][j].free == 0){
                    return 0;
                }
            }
        }
        //is the destination free, if yes is it friendly or not
        if (game_board[to.y][to.x].color == game_board[from.y][from.x].color && game_board[to.y][to.x].free == 0){
            return 0;
        }
        else{
            return 1;
        }
    }
}

bool rook::check(checker game_board[8][8], place from, place to){
    rook ROOK;
    if (ROOK.check_1(game_board, from, to)){
        return ROOK.check_2(game_board, from, to);
    }
    return 0;
}

bool bishop::check(checker game_board[8][8], place from, place to){
    bishop BISHOP;
    if (BISHOP.check_1(game_board, from, to)){
        return BISHOP.check_2(game_board, from, to);
    }
    return 0;
}

bool queen::check(checker game_board[8][8], place from, place to){
    queen QUEEN;
    if (QUEEN.check_1(game_board, from, to)){
        return QUEEN.check_2(game_board, from, to);
    }
    return 0;
}

bool knight::check(checker game_board[8][8], place from, place to){
    knight KNIGHT;
    if (KNIGHT.check_1(game_board, from, to)){
        return KNIGHT.check_2(game_board, from, to);
    }
    return 0;
}

bool pawn::check(checker game_board[8][8], place from, place to){
    pawn PAWN;
    int flag = PAWN.check_1(game_board, from, to);
    if (flag){
        return PAWN.check_2(game_board, from, to, flag);
    }
    return 0;
}

bool emperor::check(checker game_board[8][8], place from, place to){
    emperor EMPEROR;
    if (EMPEROR.check_1(game_board, from, to)){
        return EMPEROR.check_2(game_board, from, to);
    }
    return 0;
}

bool game::if_invalid(checker game_board[8][8], place from, place to, place emperor_place){
    checker BACKUP;
    game GAME;
    emperor EMPEROR;
    BACKUP = game_board[to.y][to.x];
    GAME.move(game_board, from, to);
    if(EMPEROR.check_check(game_board, emperor_place).flag_mate == 0){
        GAME.move(game_board, to, from);
        game_board[to.y][to.x] = BACKUP;
        return 1;
    }
    GAME.move(game_board, to, from);
    game_board[to.y][to.x] = BACKUP;
    return 0;
}

bool pawn::invalid(checker game_board[8][8], place from, place emperor_place){
    //returns 1 if invalid and 0 if not
    game GAME;
    int cnt = 0;
    pawn PAWN;
    place to;
    //white
    //1 down vertical
    to.y = from.y + 1;
    to.x = from.x;
    bool logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if(PAWN.check(game_board, from, to) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    //1 right down
    to.x += 1;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if(PAWN.check(game_board, from, to) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    //1 left down
    to.x-=2;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if(PAWN.check(game_board, from, to) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    //2 down
    to.y += 1;
    to.x += 1;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if(PAWN.check(game_board, from, to) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    //black
    //1 up
    to.y = from.y - 1;
    to.x = from.x;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if(PAWN.check(game_board, from, to) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    //1 right up
    to.x += 1;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if(PAWN.check(game_board, from, to) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    //1 left up
    to.x-=2;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if(PAWN.check(game_board, from, to) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    //2 up
    to.y -= 1;
    to.x += 1;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if(PAWN.check(game_board, from, to) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    return cnt;
}

bool knight::invalid(checker game_board[8][8], place from, place emperor_place){
    int cnt = 0;
    game GAME;
    place to;
    to.y = from.y + 1;
    to.x = from.x + 2;
    bool logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if((game_board[to.y][to.x].id == ' ' || game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    to.y = from.y - 1;
    to.x = from.x + 2;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if((game_board[to.y][to.x].id == ' ' || game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    to.y = from.y + 1;
    to.x = from.x - 2;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if((game_board[to.y][to.x].id == ' ' || game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    to.y = from.y - 1;
    to.x = from.x - 2;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if((game_board[to.y][to.x].id == ' ' || game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    to.y = from.y + 2;
    to.x = from.x + 1;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if((game_board[to.y][to.x].id == ' ' || game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    to.y = from.y + 2;
    to.x = from.x - 1;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if((game_board[to.y][to.x].id == ' ' || game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    to.y = from.y - 2;
    to.x = from.x + 1;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if((game_board[to.y][to.x].id == ' ' || game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    to.y = from.y - 2;
    to.x = from.x - 1;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if((game_board[to.y][to.x].id == ' ' || game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    return cnt;
}

bool queen::invalid(checker game_board[8][8], place from, place emperor_place){
    int cnt = 0;
    place to;
    game GAME;
    //down vertical
    int i;
    if(from.y != 7){
        for(i = from.y + 1; game_board[i][from.x].id == ' ' && i < 7; i++){
        to.y = i;
        to.x = from.x;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
        }
        to.y = i;
        to.x = from.x;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
    }
    //up vertical
    if(from.y != 0){
        for(i = from.y - 1; game_board[i][from.x].id == ' ' && i > 0; i--){
        to.y = i;
        to.x = from.x;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
        }
        to.y = i;
        to.x = from.x;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
    }
    //left horizontal
    if(from.x != 0){
        for(i = from.x - 1; game_board[from.y][i].id == ' ' && i > 0; i--){
        to.y = from.y;
        to.x = i;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
        }
        to.y = from.y;
        to.x = i;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
    }
    //right horizontal
    if(from.x != 7){
        for(i = from.x + 1; game_board[from.y][i].id == ' ' && i < 7; i++){
        to.y = from.y;
        to.x = i;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
        }
        to.y = from.y;
        to.x = i;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
    }
    //down right
    int j;
    if ( from.x != 7 && from.y != 7 ){
        for (i = from.x + 1, j = from.y + 1; game_board[j][i].id == ' ' && (i < 7 && j < 7); i++, j++){
        to.x = i;
        to.y = j;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
        }
        to.x = i;
        to.y = j;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
    }
    //down left
    if ( from.x != 0 && from.y != 7 ){
        for (i = from.x - 1, j = from.y + 1; game_board[j][i].id == ' ' && (i > 0 && j < 7); i--, j++){
        to.x = i;
        to.y = j;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
        }
        to.x = i;
        to.y = j;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
    }
    //up right
    if ( from.x != 7 && from.y != 0 ) {
        for (i = from.x + 1, j = from.y - 1; game_board[j][i].id == ' ' && (i < 7 && j > 0); i++, j--){
        to.x = i;
        to.y = j;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
        }
        to.x = i;
        to.y = j;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
    }
    //up left
    if ( from.x != 0 && from.y != 0 ) {
        for (i = from.x - 1, j = from.y - 1; game_board[j][i].id == ' ' && (i > 0 && j > 0); i--, j--){
        to.x = i;
        to.y = j;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
        }
        to.x = i;
        to.y = j;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
    }
    return cnt;
}

bool bishop::invalid(checker game_board[8][8], place from, place emperor_place){
    game GAME;
    place to;
    //down right
    int i, j;
    int cnt = 0;
    if ( from.x != 7 && from.y != 7 ){
        for (i = from.x + 1, j = from.y + 1; game_board[j][i].id == ' ' && (i < 7 && j < 7); i++, j++){
        to.x = i;
        to.y = j;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
        }
        to.x = i;
        to.y = j;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
    }
    //down left
    if ( from.x != 0 && from.y != 7 ) {
        for (i = from.x - 1, j = from.y + 1; game_board[j][i].id == ' ' && (i > 0 && j < 7); i--, j++){
        to.x = i;
        to.y = j;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
        }
        to.x = i;
        to.y = j;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
    }
    //up right
    if ( from.x != 7 && from.y != 0 ) {
        for (i = from.x + 1, j = from.y - 1; game_board[j][i].id == ' ' && (i < 7 && j > 0); i++, j--){
        to.x = i;
        to.y = j;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
        }
        to.x = i;
        to.y = j;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
    }
    //up left
    if ( from.x != 0 && from.y != 0 ){
        for (i = from.x - 1, j = from.y - 1; game_board[j][i].id == ' ' && (i > 0 && j > 0); i--, j--){
        to.x = i;
        to.y = j;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
        }
        to.x = i;
        to.y = j;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
    }
    return cnt;
}

bool rook::invalid(checker game_board[8][8], place from, place emperor_place){
    place to;
    //down vertical
    int i;
    int cnt = 0;
    game GAME;
    if(from.y != 7){
        for(i = from.y + 1; game_board[i][from.x].id == ' ' && i < 7; i++){
        to.y = i;
        to.x = from.x;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
        }
        to.y = i;
        to.x = from.x;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
    }
    //up vertical
    if(from.y != 0){
        for(i = from.y - 1; game_board[i][from.x].id == ' ' && i > 0; i--){
        to.y = i;
        to.x = from.x;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
        }
        to.y = i;
        to.x = from.x;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
    }
    //left horizontal
    if(from.x != 0){
        for(i = from.x - 1; game_board[from.y][i].id == ' ' && i > 0; i--){
        to.y = from.y;
        to.x = i;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
        }
        to.y = from.y;
        to.x = i;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
    }
    //right horizontal
    if(from.x != 7){
        for(i = from.x + 1; game_board[from.y][i].id == ' ' && i < 7; i++){
        to.y = from.y;
        to.x = i;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
        }
        to.y = from.y;
        to.x = i;
        if(game_board[to.y][to.x].color != game_board[emperor_place.y][emperor_place.x].color){
            cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
        }
    }
    return cnt;
}

bool emperor::invalid(checker game_board[8][8], place from, place emperor_place){
    place backup = emperor_place;
    place to;
    game GAME;
    int cnt = 0;
    emperor EMPEROR;
    to.y = from.y + 1;
    to.x = from.x;
    emperor_place = to;
    bool logic = (to.y > -1 && to.y < 8 && to.x > -1 && to.x < 8);
    if(EMPEROR.check(game_board, from, to) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    to.y = from.y - 1;
    to.x = from.x;
    emperor_place = to;
    logic = (to.y > -1 && to.y < 8 && to.x > -1 && to.x < 8);
    if(EMPEROR.check(game_board, from, to) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    to.y = from.y;
    to.x = from.x - 1;
    emperor_place = to;
    logic = (to.y > -1 && to.y < 8 && to.x > -1 && to.x < 8);
    if(EMPEROR.check(game_board, from, to) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    to.y = from.y;
    to.x = from.x + 1;
    emperor_place = to;
    logic = (to.y > -1 && to.y < 8 && to.x > -1 && to.x < 8);
    if(EMPEROR.check(game_board, from, to) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    to.y = from.y + 1;
    to.x = from.x + 1;
    emperor_place = to;
    logic = (to.y > -1 && to.y < 8 && to.x > -1 && to.x < 8);
    if(EMPEROR.check(game_board, from, to) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    to.y = from.y - 1;
    to.x = from.x + 1;
    emperor_place = to;
    logic = (to.y > -1 && to.y < 8 && to.x > -1 && to.x < 8);
    if(EMPEROR.check(game_board, from, to) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    to.y = from.y + 1;
    to.x = from.x - 1;
    emperor_place = to;
    logic = (to.y > -1 && to.y < 8 && to.x > -1 && to.x < 8);
    if(EMPEROR.check(game_board, from, to) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    to.y = from.y - 1;
    to.x = from.x - 1;
    emperor_place = to;
    logic = (to.y > -1 && to.y < 8 && to.x > -1 && to.x < 8);
    if(EMPEROR.check(game_board, from, to) && logic){
        cnt+=GAME.if_invalid(game_board, from, to, emperor_place);
    }
    emperor_place = backup;
    return cnt;
}

bool emperor::check_mate(checker game_board[8][8], place emperor_place, int count_friendly){
    //return 0 if mate and 1 if not
    int i, j;
    int counter = 0;
    pawn PAWN;
    knight KNIGHT;
    bishop BISHOP;
    queen QUEEN;
    rook ROOK;
    emperor EMPEROR;
    place from;
    for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++){
            from.y = i;
            from.x = j;
            switch(game_board[i][j].id){
            case 'P':
                if(game_board[i][j].color == game_board[emperor_place.y][emperor_place.x].color)
                    counter = counter + (PAWN.invalid(game_board, from, emperor_place));
                break;
            case 'K':
                if(game_board[i][j].color == game_board[emperor_place.y][emperor_place.x].color)
                    counter = counter + (KNIGHT.invalid(game_board, from, emperor_place));
                break;
            case 'B':
                if(game_board[i][j].color == game_board[emperor_place.y][emperor_place.x].color)
                    counter = counter + (BISHOP.invalid(game_board, from, emperor_place));
                break;
            case 'R':
                if(game_board[i][j].color == game_board[emperor_place.y][emperor_place.x].color)
                    counter = counter + (ROOK.invalid(game_board, from, emperor_place));
                break;
            case 'Q':
                if(game_board[i][j].color == game_board[emperor_place.y][emperor_place.x].color)
                    counter = counter + (QUEEN.invalid(game_board, from, emperor_place));
                break;
            case 'E':
                if(game_board[i][j].color == game_board[emperor_place.y][emperor_place.x].color)
                    counter = counter + (EMPEROR.invalid(game_board, from, emperor_place));
                break;
            }

        }
    }
    if(counter == 0){
        return 0;
    }
    return 1;
}

//returns 1 if the move is a castling
bool emperor::castle(checker game_board[8][8], place from, place to){
    //without checking that king is going through checks or not
    if (from.y == to.y && abs(from.x - to.x) == 2){
        if (!game_board[from.y][from.x].hasMoved){
            if (game_board[from.y][from.x].color == 'W'){
                if (to.x > from.x){
                    //white kingside
                    if (game_board[0][5].free == 1 && game_board[0][6].free == 1){
                        if (game_board[0][7].id == 'R' && game_board[0][7].color == 'W' && game_board[0][7].hasMoved == false){
                            return 1;
                        }
                    }
                }
                else{
                    //white queenside
                    if (game_board[0][1].free == 1 && game_board[0][2].free == 1 && game_board[0][3].free == 1){
                        if (game_board[0][0].id == 'R' && game_board[0][0].color == 'W' && !game_board[0][0].hasMoved){
                            return 1;
                        }
                    }
                }
            }
            else{
                if (to.x > from.x){
                    //black kingside
                    if (game_board[7][5].free == 1 && game_board[7][6].free == 1){
                        if (game_board[7][7].id == 'R' && game_board[7][7].color == 'B' && game_board[7][7].hasMoved == false){
                            return 1;
                        }
                    }
                }
                else{
                    //black queenside
                    if (game_board[7][1].free == 1 && game_board[7][2].free == 1 && game_board[7][3].free == 1){
                        if (game_board[7][0].id == 'R' && game_board[7][0].color == 'B' && game_board[7][0].hasMoved == false){
                            return 1;
                        }
                    }
                }
            }
        }
    }
    else
    return 0;
}

AImove AI::AIsearch(checker game_board[8][8]){
    //counts to break if 16 whites found
    int counterPlayer = 0;
    int i, j;
    AIreturn score_place[16];
    AI aiobject;
    for (i=0 ; i<16 ; i++){
        score_place[i].from.x = 0;
        score_place[i].from.y = 0;
        score_place[i].to.x = 0;
        score_place[i].to.y = 0;
        score_place[i].score = -9999;
    }
    int cnt = 0;
    for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++){
            if(game_board[i][j].color == 'W' && game_board[i][j].id != ' '){
                counterPlayer++;
                place source;
                source.y = i;
                source.x = j;
                switch(game_board[i][j].id){
                case 'P':
                        score_place[cnt] = aiobject.AIpawn(game_board, source);
                        cnt++;
                    break;
                case 'B':
                        score_place[cnt] = aiobject.AIbishop(game_board, source);
                        cnt++;
                    break;
                case 'Q':
                        score_place[cnt] = aiobject.AIqueen(game_board, source);
                        cnt++;
                    break;
                case 'R':
                        score_place[cnt] = aiobject.AIrook(game_board, source);
                        cnt++;
                    break;
                case 'K':
                        score_place[cnt] = aiobject.AIknight(game_board, source);
                        cnt++;
                    break;
                case 'E':
                        score_place[cnt] = aiobject.AIemperor(game_board, source);
                        cnt++;
                    break;
                }
            }
        }
        if(counterPlayer == 16){
            break;
        }
    }
    int maximum = -9999;
    AImove finalmove;
    if(aiturn %4 == 0){
    for(i=0 ; i<16 ; i++){
        if(score_place[i].score >= maximum){
            maximum = score_place[i].score;
            finalmove.from = score_place[i].from;
            finalmove.to = score_place[i].to;
        }
    }
    }
    else{
        for(i=0 ; i <= 3 ; i++){
        if(score_place[i].score >= maximum){
            maximum = score_place[i].score;
            finalmove.from = score_place[i].from;
            finalmove.to = score_place[i].to;
        }
        }
        for(i=10 ; i < 16 ; i++){
        if(score_place[i].score >= maximum){
            maximum = score_place[i].score;
            finalmove.from = score_place[i].from;
            finalmove.to = score_place[i].to;
        }
        }
        for(i=4 ; i <10 ; i++){
        if(score_place[i].score >= maximum){
            maximum = score_place[i].score;
            finalmove.from = score_place[i].from;
            finalmove.to = score_place[i].to;
        }
        }
    }
    return finalmove;
}

void AI::AI_check_check(checker game_board[8][8], place from, place to, int &score){
    emperor EMPEROR;
    checker BACKUP;
    game GAME;
    place emperor_white_backup = emperor_white;
    if(EMPEROR.check_check(game_board , emperor_white).flag_mate){
        BACKUP = game_board[to.y][to.x];
        GAME.move(game_board, from , to);
        if(game_board[to.y][to.x].id == 'E')
            emperor_white = to;
        if(EMPEROR.check_check(game_board, emperor_white).flag_mate == 0){
            score = 1000;
        }
        GAME.move(game_board, to, from);
        game_board[to.y][to.x] = BACKUP;
        emperor_white = emperor_white_backup;
    }
    else{
        BACKUP = game_board[to.y][to.x];
        GAME.move(game_board, from, to);
        if(game_board[to.y][to.x].id == 'E')
            emperor_white = to;
        if(EMPEROR.check_check(game_board, emperor_black).flag_mate && EMPEROR.check_check(game_board, emperor_white).flag_mate == 0){
            score = 45;
        }
        GAME.move(game_board, to, from);
        game_board[to.y][to.x] = BACKUP;
        emperor_white = emperor_white_backup;
    }
}

int AI::AIswitchScore(checker game_board[8][8] , place from, place to){
    int score = 0;
    switch(game_board[to.y][to.x].id){
    case 'P':
        AI_check_check(game_board, from, to, score);
       score += 10;
        break;
    case 'K':
        AI_check_check(game_board, from, to, score);
        score += 25;
        break;
    case 'B':
        AI_check_check(game_board, from, to, score);
        score += 30;
        break;
    case 'R':
        AI_check_check(game_board, from, to, score);
        score += 50;
        break;
    case 'Q':
        AI_check_check(game_board, from, to, score);
        score += 90;
        break;
    case ' ':
        AI_check_check(game_board, from, to, score);
        if(game_board[from.y][from.x].id == 'E'){
            score--;
        }
        break;
    case 'E':
        AI_check_check(game_board, from, to, score);
        score += -1;
        break;
    }
    return score;
}

int AI::AIswitchScoreNegative(checker game_board[8][8] , place to){
    int score = 0;
    switch(game_board[to.y][to.x].id){
    case 'P':
        score = 10;
        break;
    case 'K':
        score = 25;
        break;
    case 'B':
        score = 30;
        break;
    case 'R':
        score = 50;
        break;
    case 'Q':
        score = 90;
        break;
    case ' ':
        score = 0;
        break;
    case 'E':
        score = -1;
        break;
    }
    return score;
}

void AI::AIthink(checker game_board[8][8], place from, place to, AIreturn &aireturn){
        char backup_color;
        AI aiobject;
        checker BACKUP;
        game GAME;
        emperor EMPEROR;
        place emperor_white_backup = emperor_white;
        BACKUP = game_board[to.y][to.x];
        GAME.move(game_board , from , to);
        if(game_board[to.y][to.x].id == 'E')
            emperor_white = to;
        if(EMPEROR.check_check(game_board, emperor_white).flag_mate){
            GAME.move(game_board, to, from);
            game_board[to.y][to.x] = BACKUP;
            emperor_white = emperor_white_backup;
        }
        else{
            GAME.move(game_board, to, from);
            game_board[to.y][to.x] = BACKUP;
            emperor_white = emperor_white_backup;
            int score_flag = aiobject.AIswitchScore(game_board , from , to);
            if(aireturn.score < score_flag){
                aireturn.score = score_flag;
                aireturn.to = to;
            }
            backup_color = game_board[to.y][to.x].color;
            game_board[to.y][to.x].color = 'W';
            BACKUP = game_board[to.y][to.x];
            GAME.move(game_board , from , to);
            if(game_board[to.y][to.x].id == 'E')
                emperor_white = to;
            if(EMPEROR.check_check(game_board, to).flag_mate != 0){
                aireturn.score -= aiobject.AIswitchScoreNegative(game_board ,to);
            }
            GAME.move(game_board, to, from);
            game_board[to.y][to.x] = BACKUP;
            game_board[to.y][to.x].color = backup_color;
            emperor_white = emperor_white_backup;
        }
}

AIreturn AI::AIpawn(checker game_board[8][8], place from){
    place to;
    pawn PAWN;
    AIreturn aireturn;
    aireturn.from = from;
    aireturn.to = from;
    aireturn.score = -9999;
    AI aiobject;
    //1 down vertical
    to.y = from.y + 1;
    to.x = from.x;
    bool logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if(PAWN.check(game_board, from, to) && logic){
        aiobject.AIthink(game_board, from, to, aireturn);
    }
    //1 right down
    to.x += 1;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if(PAWN.check(game_board, from, to) && logic){
        aiobject.AIthink(game_board, from, to, aireturn);
    }
    //1 left down
    to.x-=2;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if(PAWN.check(game_board, from, to) && logic){
        aiobject.AIthink(game_board, from, to, aireturn);
    }
    //2 down
    to.y += 1;
    to.x += 1;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if(PAWN.check(game_board, from, to) && logic){
        aiobject.AIthink(game_board, from, to, aireturn);
    }
    return aireturn;
}

AIreturn AI::AIbishop(checker game_board[8][8], place from){
    AI aiobject;
    place to;
    checker BACKUP;
    bishop BISHOP;
    AIreturn aireturn;
    aireturn.from = from;
    aireturn.to = from;
    aireturn.score = -9999;
    //down right
    int i, j;
    if ( from.x != 7 && from.y != 7 ){
        for (i = from.x + 1, j = from.y + 1; game_board[j][i].id == ' ' && (i < 7 && j < 7); i++, j++);
        to.x = i;
        to.y = j;
        if(game_board[j][i].color == 'B'){
            aiobject.AIthink(game_board, from, to, aireturn);
        }
    }
    //down left
    if ( from.x != 0 && from.y != 7 ) {
        for (i = from.x - 1, j = from.y + 1; game_board[j][i].id == ' ' && (i > 0 && j < 7); i--, j++);
        to.x = i;
        to.y = j;
        if(game_board[j][i].color == 'B'){
            aiobject.AIthink(game_board, from, to, aireturn);
        }
    }
    //up right
    if ( from.x != 7 && from.y != 0 ) {
        for (i = from.x + 1, j = from.y - 1; game_board[j][i].id == ' ' && (i < 7 && j > 0); i++, j--);
        to.x = i;
        to.y = j;
        if(game_board[j][i].color == 'B'){
            aiobject.AIthink(game_board, from, to, aireturn);
        }
    }
    //up left
    if ( from.x != 0 && from.y != 0 ){
        for (i = from.x - 1, j = from.y - 1; game_board[j][i].id == ' ' && (i > 0 && j > 0); i--, j--);
        to.x = i;
        to.y = j;
        if(game_board[j][i].color == 'B'){
            aiobject.AIthink(game_board, from, to, aireturn);
        }
    }
    return aireturn;
}

AIreturn AI::AIknight(checker game_board[8][8], place from){
    AI aiobject;
    knight KNIGHT;
    checker BACKUP;
    game GAME;
    AIreturn aireturn;
    aireturn.from = from;
    aireturn.to = from;
    aireturn.score = -9999;
    place to;
    to.y = from.y + 1;
    to.x = from.x + 2;
    bool logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if(game_board[to.y][to.x].id != ' ' && game_board[to.y][to.x].color == 'B' && logic){
            aiobject.AIthink(game_board, from, to, aireturn);
    }
    to.y = from.y - 1;
    to.x = from.x + 2;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if(game_board[to.y][to.x].id != ' ' && game_board[to.y][to.x].color == 'B' && logic){
            aiobject.AIthink(game_board, from, to, aireturn);
    }
    to.y = from.y + 1;
    to.x = from.x - 2;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if(game_board[to.y][to.x].id != ' ' && game_board[to.y][to.x].color == 'B' && logic){
            aiobject.AIthink(game_board, from, to, aireturn);
    }
    to.y = from.y - 1;
    to.x = from.x - 2;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if(game_board[to.y][to.x].id != ' ' && game_board[to.y][to.x].color == 'B' && logic){
            aiobject.AIthink(game_board, from, to, aireturn);
    }
    to.y = from.y + 2;
    to.x = from.x + 1;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if(game_board[to.y][to.x].id != ' ' && game_board[to.y][to.x].color == 'B' && logic){
            aiobject.AIthink(game_board, from, to, aireturn);
    }
    to.y = from.y + 2;
    to.x = from.x - 1;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if(game_board[to.y][to.x].id != ' ' && game_board[to.y][to.x].color == 'B' && logic){
            aiobject.AIthink(game_board, from, to, aireturn);
    }
    to.y = from.y - 2;
    to.x = from.x + 1;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if(game_board[to.y][to.x].id != ' ' && game_board[to.y][to.x].color == 'B' && logic){
            aiobject.AIthink(game_board, from, to, aireturn);
    }
    to.y = from.y - 2;
    to.x = from.x - 1;
    logic = (to.y < 8 && to.y > -1 && to.x < 8 && to.x > -1);
    if(game_board[to.y][to.x].id != ' ' && game_board[to.y][to.x].color == 'B' && logic){
            aiobject.AIthink(game_board, from, to, aireturn);
    }
    return aireturn;
}

AIreturn AI::AIrook(checker game_board[8][8], place from){
    AI aiobject;
    rook ROOK;
    checker BACKUP;
    game GAME;
    AIreturn aireturn;
    aireturn.from = from;
    aireturn.to = from;
    aireturn.score = -9999;
    place to;
    //down vertical
    int i;
    if(from.y != 7){
        for(i = from.y + 1; game_board[i][from.x].id == ' ' && i < 7; i++);
        to.y = i;
        to.x = from.x;
        if(game_board[to.y][to.x].color == 'B'){
            aiobject.AIthink(game_board, from, to, aireturn);
        }
    }
    //up vertical
    if(from.y != 0){
        for(i = from.y - 1; game_board[i][from.x].id == ' ' && i > 0; i--);
        to.y = i;
        to.x = from.x;
        if(game_board[to.y][to.x].color == 'B'){
            aiobject.AIthink(game_board, from, to, aireturn);
        }
    }
    //left horizontal
    if(from.x != 0){
        for(i = from.x - 1; game_board[from.y][i].id == ' ' && i > 0; i--);
        to.y = from.y;
        to.x = i;
        if(game_board[to.y][to.x].color == 'B'){
            aiobject.AIthink(game_board, from, to, aireturn);
        }
    }
    //right horizontal
    if(from.x != 7){
        for(i = from.x + 1; game_board[from.y][i].id == ' ' && i < 7; i++);
        to.y = from.y;
        to.x = i;
        if(game_board[to.y][to.x].color == 'B'){
            aiobject.AIthink(game_board, from, to, aireturn);
        }
    }
    return aireturn;
}

AIreturn AI::AIqueen(checker game_board[8][8], place from){
    AI aiobject;
    queen QUEEN;
    checker BACKUP;
    game GAME;
    AIreturn aireturn;
    aireturn.from = from;
    aireturn.to = from;
    aireturn.score = -9999;
    place to;
    //down vertical
    int i;
    if(from.y != 7){
        for(i = from.y + 1; game_board[i][from.x].id == ' ' && i < 7; i++);
        to.y = i;
        to.x = from.x;
        if(game_board[to.y][to.x].color == 'B'){
            aiobject.AIthink(game_board, from, to, aireturn);
        }
    }
    //up vertical
    if(from.y != 0){
        for(i = from.y - 1; game_board[i][from.x].id == ' ' && i > 0; i--);
        to.y = i;
        to.x = from.x;
        if(game_board[to.y][to.x].color == 'B'){
            aiobject.AIthink(game_board, from, to, aireturn);
        }
    }
    //left horizontal
    if(from.x != 0){
        for(i = from.x - 1; game_board[from.y][i].id == ' ' && i > 0; i--);
        to.y = from.y;
        to.x = i;
        if(game_board[to.y][to.x].color == 'B'){
            aiobject.AIthink(game_board, from, to, aireturn);
        }
    }
    //right horizontal
    if(from.x != 7){
        for(i = from.x + 1; game_board[from.y][i].id == ' ' && i < 7; i++);
        to.y = from.y;
        to.x = i;
        if(game_board[to.y][to.x].color == 'B'){
            aiobject.AIthink(game_board, from, to, aireturn);
        }
    }
    //down right
    int j;
    if ( from.x != 7 && from.y != 7 ){
        for (i = from.x + 1, j = from.y + 1; game_board[j][i].id == ' ' && (i < 7 && j < 7); i++, j++);
        to.x = i;
        to.y = j;
        if(game_board[j][i].color == 'B'){
            aiobject.AIthink(game_board, from, to, aireturn);
        }
    }
    //down left
    if ( from.x != 0 && from.y != 7 ){
        for (i = from.x - 1, j = from.y + 1; game_board[j][i].id == ' ' && (i > 0 && j < 7); i--, j++);
        to.x = i;
        to.y = j;
        if(game_board[j][i].color == 'B'){
            aiobject.AIthink(game_board, from, to, aireturn);
        }
    }
    //up right
    if ( from.x != 7 && from.y != 0 ) {
        for (i = from.x + 1, j = from.y - 1; game_board[j][i].id == ' ' && (i < 7 && j > 0); i++, j--);
        to.x = i;
        to.y = j;
        if(game_board[j][i].color == 'B'){
            aiobject.AIthink(game_board, from, to, aireturn);
        }
    }
    //up left
    if ( from.x != 0 && from.y != 0 ) {
        for (i = from.x - 1, j = from.y - 1; game_board[j][i].id == ' ' && (i > 0 && j > 0); i--, j--);
        to.x = i;
        to.y = j;
        if(game_board[j][i].color == 'B'){
            aiobject.AIthink(game_board, from, to, aireturn);
        }
    }
    return aireturn;
}

AIreturn AI::AIemperor(checker game_board[8][8], place from){
    AIreturn aireturn;
    aireturn.from = from;
    aireturn.to = from;
    aireturn.score = -9999;
    place to;
    emperor EMPEROR;
    to.y = from.y + 1;
    to.x = from.x;
    bool logic = (to.y > -1 && to.y < 8 && to.x > -1 && to.x < 8);
    if(EMPEROR.check(game_board, from, to) && logic){
        AIthink(game_board, from, to, aireturn);
    }
    to.y = from.y - 1;
    to.x = from.x;
    logic = (to.y > -1 && to.y < 8 && to.x > -1 && to.x < 8);
    if(EMPEROR.check(game_board, from, to) && logic){
        AIthink(game_board, from, to, aireturn);
    }
    to.y = from.y;
    to.x = from.x - 1;
    logic = (to.y > -1 && to.y < 8 && to.x > -1 && to.x < 8);
    if(EMPEROR.check(game_board, from, to) && logic){
        AIthink(game_board, from, to, aireturn);
    }
    to.y = from.y;
    to.x = from.x + 1;
    logic = (to.y > -1 && to.y < 8 && to.x > -1 && to.x < 8);
    if(EMPEROR.check(game_board, from, to) && logic){
        AIthink(game_board, from, to, aireturn);
    }
    to.y = from.y + 1;
    to.x = from.x + 1;
    logic = (to.y > -1 && to.y < 8 && to.x > -1 && to.x < 8);
    if(EMPEROR.check(game_board, from, to) && logic){
        AIthink(game_board, from, to, aireturn);
    }
    to.y = from.y - 1;
    to.x = from.x + 1;
    logic = (to.y > -1 && to.y < 8 && to.x > -1 && to.x < 8);
    if(EMPEROR.check(game_board, from, to) && logic){
        AIthink(game_board, from, to, aireturn);
    }
    to.y = from.y + 1;
    to.x = from.x - 1;
    logic = (to.y > -1 && to.y < 8 && to.x > -1 && to.x < 8);
    if(EMPEROR.check(game_board, from, to) && logic){
        AIthink(game_board, from, to, aireturn);
    }
    to.y = from.y - 1;
    to.x = from.x - 1;
    logic = (to.y > -1 && to.y < 8 && to.x > -1 && to.x < 8);
    if(EMPEROR.check(game_board, from, to) && logic){
        AIthink(game_board, from, to, aireturn);
    }
    return aireturn;
}

int game::searchCount(checker game_board[8][8], char color){
    int cnt = 0;
    int i, j;
    for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++){
            if(game_board[i][j].color == color){
                cnt++;
            }
        }
    }
    return cnt;
}

int main(){
    IpAddress ip = IpAddress::getLocalAddress();
    TcpSocket socket;
    char connectionType;
    char buffer[100];
    size_t received;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
    cout << "\n           CCCCCCCCCCCCC  HHHHHHHHH     HHHHHHHHH  EEEEEEEEEEEEEEEEEEEEEE     SSSSSSSSSSSSSSS      SSSSSSSSSSSSSSS \n        CCC::::::::::::C  H:::::::H     H:::::::H  E::::::::::::::::::::E   SS:::::::::::::::S   SS:::::::::::::::S\n      CC:::::::::::::::C  H:::::::H     H:::::::H  E::::::::::::::::::::E  S:::::SSSSSS::::::S  S:::::SSSSSS::::::S\n     C:::::CCCCCCCC::::C  HH::::::H     H::::::HH  EE::::::EEEEEEEEE::::E  S:::::S     SSSSSSS  S:::::S     SSSSSSS\n    C:::::C       CCCCCC    H:::::H     H:::::H      E:::::E       EEEEEE  S:::::S              S:::::S            \n   C:::::C                  H:::::H     H:::::H      E:::::E               S:::::S              S:::::S            \n   C:::::C                  H::::::HHHHH::::::H      E::::::EEEEEEEEEE      S::::SSSS            S::::SSSS         \n   C:::::C                  H:::::::::::::::::H      E:::::::::::::::E       SS::::::SSSSS        SS::::::SSSSS    \n   C:::::C                  H:::::::::::::::::H      E:::::::::::::::E         SSS::::::::SS        SSS::::::::SS  \n   C:::::C                  H::::::HHHHH::::::H      E::::::EEEEEEEEEE            SSSSSS::::S          SSSSSS::::S \n   C:::::C                  H:::::H     H:::::H      E:::::E                          S:::::S              S:::::S\n    C:::::C       CCCCCC    H:::::H     H:::::H      E:::::E       EEEEEE              S:::::S              S:::::S\n     C:::::CCCCCCCC::::C  HH::::::H     H::::::HH  EE::::::EEEEEEEE:::::E  SSSSSSS     S:::::S  SSSSSSS     S:::::S\n      CC:::::::::::::::C  H:::::::H     H:::::::H  E::::::::::::::::::::E  S::::::SSSSSS:::::S  S::::::SSSSSS:::::S\n        CCC::::::::::::C  H:::::::H     H:::::::H  E::::::::::::::::::::E  S:::::::::::::::SS   S:::::::::::::::SS \n           CCCCCCCCCCCCC  HHHHHHHHH     HHHHHHHHH  EEEEEEEEEEEEEEEEEEEEEE   SSSSSSSSSSSSSSS      SSSSSSSSSSSSSSS\n\n\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
    cout << "                                         For MultiPlayer on 1 system press (a)\n\n                                         For MultiPlayer on 2 systems press (b)\n\n                                              For SinglePlayer press (c)";
    cout << "                                                                                                           ";
    char game_type;
    cin >> game_type;
    string server_ip;
    if(game_type == 'b'){
    cout << "                                         For white press (w), ";
    cout << "For black press (b)\n";
    cout << "                                                          ";
    cin >> connectionType;
    if(connectionType == 'w'){
        cout << "                                            Your local IP is: " << ip << endl;
    }
    else{
        cout << "                                          Enter the server's IP : ";
        cin >> server_ip;
    }


    if (connectionType == 'w'){
        TcpListener listener;
        listener.listen(126);
        listener.accept(socket);
    }
    else{
        socket.connect(server_ip, 126);
    }
    }
    string text = "";
    place from, to;
    //declare game object
    game GAME;
    //declare an 8 * 8 = 64 array object of class checker to control the game
    checker game_board[8][8];
    GAME.start(game_board);
    int i, j, temp_turn;
    //initialize the amounts of game_board[i][j].color
    for (i = 0; i < 8; i++){
        game_board[0][i].color = 'W';
        game_board[1][i].color = 'W';
        game_board[6][i].color = 'B';
        game_board[7][i].color = 'B';
        for (j = 2; j < 6; j++){
            game_board[j][i].color = ' ';
        }
    }
    //hide mouse cursor
    bool end_game = false;
    //creating objects
    pawn PAWN;
    rook ROOK;
    bishop BISHOP;
    queen QUEEN;
    knight KNIGHT;
    emperor EMPEROR;
    //emperors position
    emperor_black.y = 7;
    emperor_black.x = 4;
    emperor_white.y = 0;
    emperor_white.x = 4;
    //turns
    int turn = 0;
    place star_position;
    star_position.y = 0;
    star_position.x = 0;
    checker BACKUP;
    GAME.hide_cursor();
    GAME.show(game_board, star_position);
    system("CLS");
    GAME.gotoxy(0, 0);
    GAME.show(game_board, star_position);
    for (i=0 ; i<8 ; i++){
        for (j=0;j<8;j++){
            game_board[i][j].hasMoved=false;
        }
    }
    i=0;j=0;
    int white_count = 16;
    int black_count = 16;
    //main game loop
    while (!end_game){
        if(game_type == 'b'){
        if (connectionType == 'w'){
            if (turn % 2 == 0){
                from = GAME.get_key(star_position, game_board);
                star_position.y = 0;
                star_position.x = 0;
                to = GAME.get_key(star_position, game_board);
                text = char(from.y + 48);
                text += char(from.x + 48);
                text += char(to.y + 48);
                text += char(to.x + 48);
                socket.send(text.c_str(), text.length() + 1);
            }
            else{
                socket.receive(buffer, sizeof(buffer), received);
                from.y = int(buffer[0]) - 48;
                from.x = int(buffer[1]) - 48;
                to.y = int(buffer[2]) - 48;
                to.x = int(buffer[3]) - 48;
            }
        }
        else if (connectionType == 'b'){
            if (turn % 2 == 1){
                from = GAME.get_key(star_position, game_board);
                star_position.y = 0;
                star_position.x = 0;
                to = GAME.get_key(star_position, game_board);
                text = char(from.y + 48);
                text += char(from.x + 48);
                text += char(to.y + 48);
                text += char(to.x + 48);
                socket.send(text.c_str(), text.length() + 1);
            }
            else{
                socket.receive(buffer, sizeof(buffer), received);
                from.y = int(buffer[0]) - 48;
                from.x = int(buffer[1]) - 48;
                to.y = int(buffer[2]) - 48;
                to.x = int(buffer[3]) - 48;
            }
        }
        }
        else if(game_type == 'c'){
                if(turn %2 == 0){
                    AI aiobject;
                    AImove AIfromto;
                    AIfromto = aiobject.AIsearch(game_board);
                    from = AIfromto.from;
                    to = AIfromto.to;
                    }
                    else{
                        from = GAME.get_key(star_position, game_board);
                        star_position.y = 0;
                        star_position.x = 0;
                        to = GAME.get_key(star_position, game_board);
                    }
        }
        else{
            from = GAME.get_key(star_position, game_board);
            star_position.y = 0;
            star_position.x = 0;
            to = GAME.get_key(star_position, game_board);
        }

        if ((game_board[from.y][from.x].color == 'W' && turn % 2 == 0) || (game_board[from.y][from.x].color == 'B' && turn % 2 == 1)){
            if (game_board[to.y][to.x].id != 'E'){
                switch (game_board[from.y][from.x].id){
                case 'P':
                    if (PAWN.check(game_board, from, to)){
                        BACKUP = game_board[to.y][to.x];
                        GAME.move(game_board, from, to);
                        turn++;
                    }
                    if (game_board[to.y][to.x].color == 'W' ? EMPEROR.check_check(game_board, emperor_white).flag_mate : EMPEROR.check_check(game_board, emperor_black).flag_mate){
                        GAME.move(game_board, to, from);
                        game_board[to.y][to.x] = BACKUP;
                        turn--;
                    }
                    break;
                case 'R':
                    temp_turn = turn;
                    if (ROOK.check(game_board, from, to)){
                        BACKUP = game_board[to.y][to.x];
                        GAME.move(game_board, from, to);
                        turn++;
                    }
                    if (game_board[to.y][to.x].color == 'W' ? EMPEROR.check_check(game_board, emperor_white).flag_mate : EMPEROR.check_check(game_board, emperor_black).flag_mate){
                        GAME.move(game_board, to, from);
                        turn--;
                        game_board[to.y][to.x] = BACKUP;
                    }
                    if (temp_turn != turn){
                        game_board[to.y][to.x].hasMoved = true;
                    }
                    break;
                case 'B':
                    if (BISHOP.check(game_board, from, to)){
                        BACKUP = game_board[to.y][to.x];
                        GAME.move(game_board, from, to);
                        turn++;
                    }
                    if (game_board[to.y][to.x].color == 'W' ? EMPEROR.check_check(game_board, emperor_white).flag_mate : EMPEROR.check_check(game_board, emperor_black).flag_mate)
                    {
                        GAME.move(game_board, to, from);
                        turn--;
                        game_board[to.y][to.x] = BACKUP;
                    }
                    break;
                case 'Q':
                    if (QUEEN.check(game_board, from, to)){
                        BACKUP = game_board[to.y][to.x];
                        GAME.move(game_board, from, to);
                        turn++;
                    }
                    if (game_board[to.y][to.x].color == 'W' ? EMPEROR.check_check(game_board, emperor_white).flag_mate : EMPEROR.check_check(game_board, emperor_black).flag_mate){
                        GAME.move(game_board, to, from);
                        turn--;
                        game_board[to.y][to.x] = BACKUP;
                    }
                    break;
                case 'K':
                    if (KNIGHT.check(game_board, from, to)){
                        BACKUP = game_board[to.y][to.x];
                        GAME.move(game_board, from, to);
                        turn++;
                    }
                    if (game_board[to.y][to.x].color == 'W' ? EMPEROR.check_check(game_board, emperor_white).flag_mate : EMPEROR.check_check(game_board, emperor_black).flag_mate){
                        GAME.move(game_board, to, from);
                        turn--;
                        game_board[to.y][to.x] = BACKUP;
                    }
                    break;
                case 'E':
                    // castling  EMPEROR.check_check(game_board,from).flag_mate == 0 &&
                    if (EMPEROR.castle(game_board, from, to)){
                        place temporary = from;
                        // now we also need to check if the king is
                        // going to move through check or not
                        if (1){
                            if (to.x - from.x > 0){ //  king side //
                                temporary.x++;
                                GAME.move(game_board, from, temporary);
                                if (EMPEROR.check_check(game_board, temporary).flag_mate == 0){
                                    place temporary2 = temporary;
                                    temporary2.x++;
                                    GAME.move(game_board, temporary, temporary2);
                                    if (EMPEROR.check_check(game_board, temporary2).flag_mate == 0){
                                        turn++;
                                        if (game_board[temporary2.y][temporary2.x].color == 'W')
                                            emperor_white = temporary2;
                                        else
                                            emperor_black = temporary2;
                                        place rooks = temporary2;
                                        rooks.x++;
                                        GAME.move(game_board, rooks, temporary);
                                        game_board[from.y][from.x + 1].hasMoved = true;
                                        game_board[from.y][from.x + 2].hasMoved = true;
                                        break;
                                    }
                                    else{
                                        GAME.move(game_board, temporary2, from);
                                    }
                                }
                                else{
                                    GAME.move(game_board, from, temporary);
                                }
                            }
                            else{//queen side
                                temporary.x--;
                                GAME.move(game_board, from, temporary);
                                if (EMPEROR.check_check(game_board, temporary).flag_mate == 0){
                                    place temporary2 = temporary;
                                    temporary2.x--;
                                    GAME.move(game_board, temporary, temporary2);
                                    if (EMPEROR.check_check(game_board, temporary2).flag_mate == 0){
                                        turn++;
                                        if (game_board[temporary2.y][temporary2.x].color == 'W')
                                            emperor_white = temporary2;
                                        else
                                            emperor_black = temporary2;
                                        place rooks = temporary2;
                                        rooks.x -= 2;
                                        GAME.move(game_board, rooks, temporary);
                                        game_board[from.y][from.x - 1].hasMoved = true;
                                        game_board[from.y][from.x - 2].hasMoved = true;
                                        break;
                                    }
                                    else{
                                        GAME.move(game_board, temporary2, from);
                                    }
                                }
                                else{
                                    GAME.move(game_board, from, temporary);
                                }
                            }
                        }
                    }
                    temp_turn = turn;
                    if (EMPEROR.check(game_board, from, to)){
                        BACKUP = game_board[to.y][to.x];
                        turn++;
                        GAME.move(game_board, from, to);
                        if (game_board[to.y][to.x].color == 'W'){
                            emperor_white = to;
                        }
                        else{
                            emperor_black = to;
                        }
                    }
                    if (game_board[to.y][to.x].color == 'W' ? EMPEROR.check_check(game_board, emperor_white).flag_mate : EMPEROR.check_check(game_board, emperor_black).flag_mate){
                        GAME.move(game_board, to, from);
                        turn--;
                        if (game_board[from.y][from.x].color == 'W'){
                            emperor_white = from;
                        }
                        else{
                            emperor_black = from;
                        }
                        game_board[to.y][to.x] = BACKUP;
                    }
                    if (temp_turn != turn){
                        game_board[to.y][to.x].hasMoved = true;
                    }
                    break;
                }
                int k;
                for(k = 0; k < 8; k++){
                    if(game_board[0][k].id == 'P'){
                        game_board[0][k].id = 'Q';
                    }
                    if(game_board[7][k].id == 'P'){
                        game_board[7][k].id = 'Q';
                    }
                }
                aiturn = turn;
                white_count = GAME.searchCount(game_board, 'W');
                black_count = GAME.searchCount(game_board, 'B');
                GAME.gotoxy(0, 0);
                GAME.show(game_board, star_position);
                if (turn % 2 == 1){
                    if (EMPEROR.check_check(game_board, emperor_black).flag_mate){
                        if (!EMPEROR.check_mate(game_board, emperor_black, black_count)){
                            end_game = true;
                            MessageBoxW(NULL, L"White Army Won!", L"Game Over!", MB_ICONEXCLAMATION | MB_OK);
                        }
                    }
                }
                else{
                    if (EMPEROR.check_check(game_board, emperor_white).flag_mate){
                        if (!EMPEROR.check_mate(game_board, emperor_white, white_count)){
                            end_game = true;
                            MessageBoxW(NULL, L"Black Army Won!", L"Game Over!", MB_ICONEXCLAMATION | MB_OK);
                        }
                    }
                }
            }
            else{
                Beep(523, 500);
            }
        }
        else if (game_board[from.y][from.x].id != ' '){
            MessageBoxW(NULL, L"Not Your Turn!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        }
        else{
            MessageBoxW(NULL, L"There is nothing to move", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        }
    }
    system("pause");
    return 0;
}
