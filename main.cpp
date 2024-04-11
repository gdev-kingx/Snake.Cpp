#include <cstdlib>
#include <ncurses.h>

bool gameOver;
const int width = 20, height = 20;
int x, y, FruitX, FruitY, score;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

int TailX[100], TailY[100];
int nTail = 0;

void Setup() {

    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);
    start_color(); // Start color mode

    // Define color pairs
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);

    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    FruitX = (rand() % width) + 1;
    FruitY = (rand() % height) + 1;
    score = 0;

}

void Draw() {

    clear();

    for (int i = 0; i < width + 2; i++)
        mvprintw(0, i, "+");

    for (int i = 0; i < height + 2; i++)
    {
        for (int j = 0; j < width + 2; j++)
        {
            if (i == 0 || i == 21) {
                attron(COLOR_PAIR(2)); // Green color for Horizontal border
                mvprintw(i, j, "#");
                attroff(COLOR_PAIR(2));
            } else if (j == 0 || j == 21){
                attron(COLOR_PAIR(2)); // Green color for Vertical border
                mvprintw(i, j, "+");
                attroff(COLOR_PAIR(2));
            } else if (i == y && j == x) {
                attron(COLOR_PAIR(1)); // Red color for snake's head
                mvprintw(i, j, "H");
                attroff(COLOR_PAIR(1));
            } else if (i == FruitY && j == FruitX) {
                attron(COLOR_PAIR(4)); // Blue color for fruit
                mvprintw(i, j, "0");
                attroff(COLOR_PAIR(4));
            } else
                for (int k = 0; k < nTail; k++) {
                    if (TailX[k] == j && TailY[k] == i) {
                        attron(COLOR_PAIR(3)); // Yellow color for snake's body
                        mvprintw(i, j, "o");
                        attroff(COLOR_PAIR(3));
                    }
                }
        }
    }

    mvprintw(23, 0, "Score %d", score);

    refresh();


}

void Input() {

    keypad(stdscr, TRUE);
    halfdelay(1);

    int c = getch();

    switch (c) {
        case KEY_LEFT:
            dir = LEFT;
            break;
        case KEY_RIGHT:
            dir = RIGHT;
            break;
        case KEY_UP:
            dir = UP;
            break;
        case KEY_DOWN:
            dir = DOWN;
            break;
        case 113:
            gameOver = true;
            break;
    }

}

void Logic() {

    int prevX = TailX[0];
    int prevY = TailY[0];
    int prev2X, prev2Y;

    //THIS MOVES 1ST TAIL PIECE TO THE HEAD - MWAHAHAHA
    TailX[0] = x;
    TailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = TailX[i];
        prev2Y = TailY[i];
        TailX[i] = prevX;
        TailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }

    if (x > width || x < 1 || y > height) {
        gameOver = true;
    }
    if (x == FruitX && y == FruitY) {
        score++;
        FruitX = (rand() % width) + 1;
        FruitY = (rand() % height) + 1;
        nTail++;
    }

    for (int i = 0; i < nTail; i++) {
        if (TailX[i] == x && TailY[i] == y) {
            gameOver = true;
        }
    }
}

int main() {

    Setup();

    while (!gameOver) {
        Draw();
        Input();
        Logic();
    }

    getch();
    endwin();

    return 0;
}
