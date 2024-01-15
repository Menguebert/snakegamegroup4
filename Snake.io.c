#include <stdio.h>
#include <stdlib.h>
#include <conio.h>                        //_kbhit: It returns a non-zero value if a key was pressed, otherwise it returns 0.
#include <windows.h>                      //for function Sleep , paused for a certain number of milliseconds.

#define WIDTH 20
#define HEIGHT 10
#define SPEED 120

int gameOver = 0;
int score = 0;

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Coordinate {
    int x;
    int y;
};

struct Snake {
    struct Coordinate head;  // Kopf der Schlange
    int length;              // Länge der Schlange
    struct Coordinate body[WIDTH * HEIGHT];  // Körper der Schlange
    enum Direction direction;  // Richtung der Schlange
};

struct Apple {
    struct Coordinate position;  // Position des Apfels
};

// Initialisiert das Spiel
void initializeGame(struct Snake *snake, struct Apple *apple) {
    snake->head.x = WIDTH / 2;
    snake->head.y = HEIGHT / 2;
    snake->length = 1;
    snake->direction = RIGHT;

    apple->position.x = rand() % WIDTH;
    apple->position.y = rand() % HEIGHT;
}

// Zeichnet das Spielfeld
void drawGame(struct Snake *snake, struct Apple *apple) {
    system("cls");

    for (int i = 0; i < HEIGHT + 2; i++) {
        for (int j = 0; j < WIDTH + 2; j++) {
            if (i == 0 || i == HEIGHT + 1) {
                printf("#");  // Zeichnet obere und untere Grenzen
            } else if (j == 0 || j == WIDTH + 1) {
                printf("#");  // Zeichnet linke und rechte Grenzen
            } else {
                int isBodyPart = 0;
                if (i - 1 == snake->head.y && j - 1 == snake->head.x) {
                    printf("H");  // Zeichnet Kopf der Schlange
                } else if (i - 1 == apple->position.y && j - 1 == apple->position.x) {
                    printf("A");  // Zeichnet Apfel
                } else {
                    for (int k = 0; k < snake->length; k++) {
                        if (i - 1 == snake->body[k].y && j - 1 == snake->body[k].x) {
                            printf("B");  // Zeichnet Körper der Schlange
                            isBodyPart = 1;
                            break;
                        }
                    }
                    if (!isBodyPart) {
                        printf(" ");
                    }
                }
            }
        }
        printf("\n");
    }

    printf("Score: %d\n", score);
}

// Bewegt die Schlange
void moveSnake(struct Snake *snake) {
    // Speichert aktuelle Position im Körper der Schlange
    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }
    snake->body[0] = snake->head;

    // Bewegt Kopf der Schlange
    switch (snake->direction) {
        case UP:
            snake->head.y--;
            break;
        case DOWN:
            snake->head.y++;
            break;
        case LEFT:
            snake->head.x--;
            break;
        case RIGHT:
            snake->head.x++;
            break;
    }
}

// Aktualisiert den Spielzustand
void updateGame(struct Snake *snake, struct Apple *apple) {
    moveSnake(snake);

    // Überprüft, ob die Schlange den Apfel gegessen hat
    if (snake->head.x == apple->position.x && snake->head.y == apple->position.y) {
        snake->length++;
        apple->position.x = rand() % WIDTH;
        apple->position.y = rand() % HEIGHT;
        score++;
    }

    // Überprüft, ob die Schlange die Grenzen berührt hat
    if (snake->head.x <= 0 || snake->head.x >= WIDTH + 1 || snake->head.y <= 0 || snake->head.y >= HEIGHT + 1) {
        gameOver = 1;
    }

    // Überprüft, ob die Schlange sich selbst berührt hat
    for (int i = 0; i < snake->length; i++) {
        if (snake->head.x == snake->body[i].x && snake->head.y == snake->body[i].y) {
            gameOver = 1;
        }
    }
}

int main() {
    struct Snake snake;
    struct Apple apple;

    srand(time(NULL));

    initializeGame(&snake, &apple);

    while (!gameOver) {
        if (_kbhit()) {
            char key = _getch();
            switch (key) {
                case 'w':
                    snake.direction = UP;
                    break;
                case 's':
                    snake.direction = DOWN;
                    break;
                case 'a':
                    snake.direction = LEFT;
                    break;
                case 'd':
                    snake.direction = RIGHT;
                    break;
                case 'q':
                    gameOver = 1;
                    break;
            }
        }

        updateGame(&snake, &apple);
        drawGame(&snake, &apple);

        Sleep(SPEED);                       //wartet die bestimmte Zeit
    }

    printf("Spiel vorbei! Punktzahl: %d\n", score);


    return 0;
}
