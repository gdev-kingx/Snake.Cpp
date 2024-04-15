#include <cstdlib>
#include <ctime>
#include <ncurses.h>

bool gameOver;
bool gamePaused;
const int width = 20, height = 20;
int x, y, FruitX, FruitY, score;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

int TailX[100], TailY[100];
int nTail = 0;

int shieldX, shieldY;
bool shieldActive = false;

int enemyX, enemyY;
bool enemyActive = false;

int health = 3;

int difficulty = 1; // Default difficulty level

void Setup();
void Draw();
void Input();
void Logic();
void MainMenu();
void GameOverScreen();
void PauseScreen();
void PlaceShield();
void PlaceEnemy();
void CheckCollisionWithShield();
void CheckCollisionWithEnemy();
void ChooseDifficulty();

int main() {
  srand(time(NULL)); // Seed for randomization
  Setup();
  MainMenu();
  ChooseDifficulty();

  while (!gameOver) {
    if (!gamePaused) {
      Draw();
      Input();
      Logic();
      if (health <= 0) {
        gameOver = true;
      }
    } else {
      PauseScreen();
      Input();
    }
  }

  endwin();
  return 0;
}

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
  gamePaused = false;
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

  for (int i = 0; i < height + 2; i++) {
    for (int j = 0; j < width + 2; j++) {
      if (i == 0 || i == 21) {
        attron(COLOR_PAIR(2)); // Green color for Horizontal border
        mvprintw(i, j, "#");
        attroff(COLOR_PAIR(2));
      } else if (j == 0 || j == 21) {
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
      } else if (shieldActive && i == shieldY && j == shieldX) {
        attron(COLOR_PAIR(3)); // Yellow color for shield
        mvprintw(i, j, "S");
        attroff(COLOR_PAIR(3));
      } else if (enemyActive && i == enemyY && j == enemyX) {
        attron(COLOR_PAIR(2)); // Green color for enemy
        mvprintw(i, j, "E");
        attroff(COLOR_PAIR(2));
      } else {
        for (int k = 0; k < nTail; k++) {
          if (TailX[k] == j && TailY[k] == i) {
            attron(COLOR_PAIR(3)); // Yellow color for snake's body
            mvprintw(i, j, "o");
            attroff(COLOR_PAIR(3));
          }
        }
      }
    }
  }

  mvprintw(23, 0, "Score: %d", score);
  mvprintw(24, 0, "Health: %d", health);
  mvprintw(25, 0, "Difficulty: %d", difficulty);

  refresh();
}

void Input() {
  keypad(stdscr, TRUE);
  halfdelay(1);

  int c = getch();

  switch (c) {
    case KEY_LEFT:
      if (dir != RIGHT)
        dir = LEFT;
      break;
    case KEY_RIGHT:
      if (dir != LEFT)
        dir = RIGHT;
      break;
    case KEY_UP:
      if (dir != DOWN)
        dir = UP;
      break;
    case KEY_DOWN:
      if (dir != UP)
        dir = DOWN;
      break;
    case 'p':
    case 'P':
      gamePaused = !gamePaused;
      break;
    case 'q':
    case 'Q':
      gameOver = true;
      break;
  }
}

void Logic() {
  int prevX = TailX[0];
  int prevY = TailY[0];
  int prev2X, prev2Y;

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

  if (x > width || x < 1 || y > height || y < 1) {
    health--; // Snake hits wall, lose health
    x = width / 2;
    y = height / 2;
    return;
  }

  if (x == FruitX && y == FruitY) {
    score++;
    FruitX = (rand() % width) + 1;
    FruitY = (rand() % height) + 1;
    nTail++;
  }

  if (shieldActive && x == shieldX && y == shieldY) {
    shieldActive = false;
  }

  if (enemyActive && x == enemyX && y == enemyY) {
    health--; // Snake hits enemy, lose health
    enemyActive = false;
  }

  if (nTail > 0) {
    for (int i = 0; i < nTail; i++) {
      if (TailX[i] == x && TailY[i] == y) {
        health--; // Snake hits itself, lose health
        return;
      }
    }
  }

  if (score % 5 == 0 && score != 0) {
    PlaceShield(); // Place shield every 5 fruits eaten
  }

  if (score % 10 == 0 && score != 0) {
    PlaceEnemy(); // Place enemy every 10 fruits eaten
  }

  switch (difficulty) {
    case 1:
      halfdelay(1); // Normal speed
      break;
    case 2:
      halfdelay(2); // Easy speed (slower)
      break;
    case 3:
      halfdelay(1); // Hard speed (faster)
      break;
  }
}

void MainMenu() {
  clear();
  mvprintw(10, 10, "Snake Game");
  mvprintw(12, 10, "Press any key to start");
  getch();
}

void PauseScreen() {
  mvprintw(10, 10, "Paused");
  mvprintw(12, 10, "Press 'P' to resume");
  refresh();
}

void GameOverScreen() {
  clear();
  mvprintw(10, 10, "Game Over");
  mvprintw(12, 10, "Press any key to exit");
  getch();
}

void PlaceShield() {
  shieldX = (rand() % width) + 1;
  shieldY = (rand() % height) + 1;
  shieldActive = true;
}

void PlaceEnemy() {
  enemyX = (rand() % width) + 1;
  enemyY = (rand() % height) + 1;
  enemyActive = true;
}

void CheckCollisionWithShield() {
  if (shieldActive && x == shieldX && y == shieldY) {
    shieldActive = false;
  }
}

void CheckCollisionWithEnemy() {
  if (enemyActive && x == enemyX && y == enemyY) {
    health--;
    enemyActive = false;
  }
}

void ChooseDifficulty() {
  clear();
  mvprintw(10, 10, "Choose Difficulty:");
  mvprintw(12, 10, "1. Normal");
  mvprintw(13, 10, "2. Easy");
  mvprintw(14, 10, "3. Hard");
  int choice = getch();
  switch (choice) {
    case '1':
      difficulty = 1;
      break;
    case '2':
      difficulty = 2;
      break;
    case '3':
      difficulty = 3;
      break;
    default:
      difficulty = 1; // Default to normal difficulty
      break;
  }
}
