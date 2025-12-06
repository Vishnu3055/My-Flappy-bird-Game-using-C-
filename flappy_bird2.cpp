#include<iostream>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include<time.h>

using namespace std;

#define WIN_WIDTH 70
#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define GAP_SIZE 7

HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int pipePos[2];
int gapPos[2];
int pipeFlag[2];

char bird[2][6] = {
    {'/', '-', '-', 'o', '\\', ' '},
    {'|', '-', '-', ' ', '>', ' '}
};

int birdPos = 6;
int score = 0;

void gotoxy(int x, int y);
void setcursor(bool visible, DWORD size);
void drawBorder();
void genPipe(int ind);
void updateScore();
void drawBird();
void eraseBird();
void drawPipe(int ind);
void erasePipe(int ind);
int collision();
void gameover();
void play();
void instruction();

int main() {
    setcursor(0, 0);
    srand((unsigned)time(NULL));

    do {
        system("cls");
        gotoxy(10,5); cout << "---------------------";
        gotoxy(10,6); cout << "|--- FLAPPY BIRD ---|";
        gotoxy(10,7); cout << "---------------------";

        gotoxy(10,9); cout << "1. Start Game";
        gotoxy(10,10); cout << "2. Instructions";
        gotoxy(10,11); cout << "3. Quit";

        gotoxy(10,13); cout << "Select Option: ";
        char op = getche();

        if(op == '1') play();
        else if(op == '2') instruction();
        else if(op == '3') exit(0);

    } while(true);
}

void play() {
    birdPos = 6;
    score = 0;
    pipeFlag[0] = 1;
    pipeFlag[1] = 0;
    pipePos[0] = 4;

    system("cls");
    drawBorder();
    genPipe(0);
    updateScore();

    gotoxy(WIN_WIDTH + 5, 2); cout << "FLAPPY BIRD";
    gotoxy(WIN_WIDTH + 6, 6); cout << "Spacebar = Jump";

    while(1) {
        if(kbhit()) {
            char ch = getch();
            if(ch == 32 && birdPos > 3) birdPos -= 3;
            if(ch == 27) break;
        }

        drawBird();
        drawPipe(0);
        drawPipe(1);

        if(collision() == 1){
            gameover();
            return;
        }

        Sleep(100);
        eraseBird();
        erasePipe(0);
        erasePipe(1);

        birdPos += 1;

        if(birdPos > SCREEN_HEIGHT - 2) {
            gameover();
            return;
        }

        if(pipeFlag[0] == 1) pipePos[0] += 2;
        if(pipeFlag[1] == 1) pipePos[1] += 2;

        if(pipePos[0] >= 40 && pipePos[0] < 42) {
            pipeFlag[1] = 1;
            pipePos[1] = 4;
            genPipe(1);
        }

        if(pipePos[0] > 68) {
            score++;
            updateScore();
            pipeFlag[1] = 0;
            pipePos[0] = pipePos[1];
            gapPos[0] = gapPos[1];
        }
    }
}

void drawBorder(){
    for(int i=0; i<=SCREEN_WIDTH; i++){
        gotoxy(i,0); cout << "-";
        gotoxy(i,SCREEN_HEIGHT); cout << "-";
    }

    for(int i=0; i<=SCREEN_HEIGHT; i++){
        gotoxy(0,i); cout << "|";
        gotoxy(WIN_WIDTH,i); cout << "|";
    }
}

void genPipe(int ind){
    gapPos[ind] = 3 + rand()%14;
}

void updateScore(){
    gotoxy(WIN_WIDTH + 7, 5); cout << "Score: " << score;
}

void drawBird(){
    for(int i=0;i<2;i++){
        for(int j=0;j<6;j++){
            gotoxy(j+2, i + birdPos);
            cout << bird[i][j];
        }
    }
}

void eraseBird(){
    for(int i=0;i<2;i++){
        for(int j=0;j<6;j++){
            gotoxy(j+2, i + birdPos);
            cout << " ";
        }
    }
}

void drawPipe(int ind){
    if(pipeFlag[ind] == 1){
        for(int i=0;i<gapPos[ind];i++){
            gotoxy(WIN_WIDTH - pipePos[ind], i+1);
            cout << "***";
        }
        for(int i=gapPos[ind] + GAP_SIZE; i<SCREEN_HEIGHT - 1; i++){
            gotoxy(WIN_WIDTH - pipePos[ind], i+1);
            cout << "***";
        }
    }
}

void erasePipe(int ind){
    if(pipeFlag[ind] == 1){
        for(int i=0;i<gapPos[ind];i++){
            gotoxy(WIN_WIDTH - pipePos[ind], i+1);
            cout << "   ";
        }
        for(int i=gapPos[ind]+ GAP_SIZE; i<SCREEN_HEIGHT - 1; i++){
            gotoxy(WIN_WIDTH - pipePos[ind], i+1);
            cout << "   ";
        }
    }
}

int collision(){
    if(pipePos[0] >= 61 && pipePos[0] <= 63){
        if(birdPos < gapPos[0] || birdPos > gapPos[0] + GAP_SIZE)
            return 1;
    }
    return 0;
}

void gameover(){
    system("cls");
    cout << "\n\n\t\t*** GAME OVER ***\n";
    cout << "\t\tScore: " << score << "\n\n";
    cout << "\tPress any key to return...";
    getch();
}

void instruction(){
    system("cls");
    cout << "\n\tInstructions:";
    cout << "\n\t» Press SPACE to make bird fly";
    cout << "\n\t» Avoid hitting the pipes";
    cout << "\n\t» ESC to exit game";
    cout << "\n\n\tPress any key to go back";
    getch();
}

void gotoxy(int x, int y){
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(consoleHandle, CursorPosition);
}

void setcursor(bool visible, DWORD size){
    if(size == 0) size = 20;
    CONSOLE_CURSOR_INFO cursor = {size, visible};
    SetConsoleCursorInfo(consoleHandle, &cursor);
}
