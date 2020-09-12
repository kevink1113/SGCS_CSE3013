#include "tetris.h"

static struct sigaction act, oact;

int main() {
    int exit = 0;

    initscr();
    noecho();
    keypad(stdscr, TRUE);

    srand((unsigned int) time(NULL));

    while (!exit) {
        clear();
        switch (menu()) {
            case MENU_PLAY:
                play();
                break;
            case MENU_EXIT:
                exit = 1;
                break;
            default:
                break;
        }
    }

    endwin();
    system("clear");
    return 0;
}

void InitTetris() {
    int i, j;

    for (j = 0; j < HEIGHT; j++)
        for (i = 0; i < WIDTH; i++)
            field[j][i] = 0;

    nextBlock[0] = rand() % 7;
    nextBlock[1] = rand() % 7;
    blockRotate = 0;
    blockY = -1;
    blockX = WIDTH / 2 - 2;
    score = 0;
    gameOver = 0;
    timed_out = 0;

    DrawOutline();
    DrawField();

    DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);

    //DrawBlock(blockY, blockX, nextBlock[0], blockRotate, ' ');
    //
    //DrawShadow(blockY, blockX, nextBlock[0], blockRotate);

    //
    DrawNextBlock(nextBlock);
    PrintScore(score);
}

void DrawOutline() {
    int i, j;
    /* 블럭이 떨어지는 공간의 태두리를 그린다.*/
    DrawBox(0, 0, HEIGHT, WIDTH);

    /* next block을 보여주는 공간의 태두리를 그린다.*/
    move(2, WIDTH + 10);
    printw("NEXT BLOCK");
    DrawBox(3, WIDTH + 10, 4, 8);
    //
    DrawBox(9, WIDTH + 10, 4, 8);
    //
    /* score를 보여주는 공간의 태두리를 그린다.*/
    //move(9,WIDTH+10);
    move(15, WIDTH + 10);

    printw("SCORE");
    //DrawBox(10,WIDTH+10,1,8);
    DrawBox(16, WIDTH + 10, 1, 8);
}

int GetCommand() {
    int command;
    command = wgetch(stdscr);
    switch (command) {
        case KEY_UP:
            break;
        case KEY_DOWN:
            break;
        case KEY_LEFT:
            break;
        case KEY_RIGHT:
            break;
        case ' ':    /* space key*/
            /*fall block*/
            break;
        case 'q':
        case 'Q':
            command = QUIT;
            break;
        default:
            command = NOTHING;
            break;
    }
    return command;
}

int ProcessCommand(int command) {
    int ret = 1;
    int drawFlag = 0;
    switch (command) {
        case QUIT:
            ret = QUIT;
            break;
        case KEY_UP:
            if ((drawFlag = CheckToMove(field, nextBlock[0], (blockRotate + 1) % 4, blockY, blockX)))
                blockRotate = (blockRotate + 1) % 4;
            break;
        case KEY_DOWN:
            if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)))
                blockY++;
            break;
        case KEY_RIGHT:
            if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX + 1)))
                blockX++;
            break;
        case KEY_LEFT:
            if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX - 1)))
                blockX--;
            break;
        default:
            break;
    }
    if (drawFlag) DrawChange(field, command, nextBlock[0], blockRotate, blockY, blockX);
    return ret;
}

void DrawField() {
    int i, j;
    for (j = 0; j < HEIGHT; j++) {
        move(j + 1, 1);
        for (i = 0; i < WIDTH; i++) {
            if (field[j][i] == 1) {
                attron(A_REVERSE);
                printw(" ");
                attroff(A_REVERSE);
            } else printw(".");
        }
    }
}


void PrintScore(int score) {
    //move(11,WIDTH+11);
    move(17, WIDTH + 11);
    printw("%8d", score);
}

void DrawNextBlock(int *nextBlock) {
    int i, j;

    for (i = 0; i < 4; i++) {
        move(4 + i, WIDTH + 13);
        for (j = 0; j < 4; j++) {
            if (block[nextBlock[1]][0][i][j] == 1) {
                attron(A_REVERSE);
                printw(" ");
                attroff(A_REVERSE);
            } else printw(" ");
        }
    }

    for (i = 0; i < 4; i++) {
        move(10 + i, WIDTH + 13);
        for (j = 0; j < 4; j++) {
            if (block[nextBlock[2]][0][i][j] == 1) {
                attron(A_REVERSE);
                printw(" ");
                attroff(A_REVERSE);
            } else printw(" ");
        }
    }

}

void DrawBlock(int y, int x, int blockID, int blockRotate, char tile) {
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++) {
            if (block[blockID][blockRotate][i][j] == 1 && i + y >= 0) {
                move(i + y + 1, j + x + 1);
                attron(A_REVERSE);
                printw("%c", tile);
                attroff(A_REVERSE);
            }
        }

    move(HEIGHT, WIDTH + 10);
}

void DrawBox(int y, int x, int height, int width) {
    int i, j;
    move(y, x);
    addch(ACS_ULCORNER);
    for (i = 0; i < width; i++)
        addch(ACS_HLINE);
    addch(ACS_URCORNER);
    for (j = 0; j < height; j++) {
        move(y + j + 1, x);
        addch(ACS_VLINE);
        move(y + j + 1, x + width + 1);
        addch(ACS_VLINE);
    }
    move(y + j + 1, x);
    addch(ACS_LLCORNER);
    for (i = 0; i < width; i++)
        addch(ACS_HLINE);
    addch(ACS_LRCORNER);
}

void play() {
    int command;
    clear();
    act.sa_handler = BlockDown;
    sigaction(SIGALRM, &act, &oact);
    InitTetris();
    do {
        if (timed_out == 0) {
            alarm(1);
            timed_out = 1;
        }

        command = GetCommand();
        if (ProcessCommand(command) == QUIT) {
            alarm(0);
            DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
            move(HEIGHT / 2, WIDTH / 2 - 4);
            printw("Good-bye!!");
            refresh();
            getch();

            return;
        }
    } while (!gameOver);

    alarm(0);
    getch();
    DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
    move(HEIGHT / 2, WIDTH / 2 - 4);
    printw("GameOver!!");
    refresh();
    getch();
    newRank(score);
}

char menu() {
    printw("1. play\n");
    printw("2. rank\n");
    printw("3. recommended play\n");
    printw("4. exit\n");
    return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
    // user code
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block[currentBlock][blockRotate][i][j]) {
                int xpos = blockX + j;
                int ypos = blockY + i;

                if (f[ypos][xpos] || (xpos >= WIDTH) || (xpos < 0) || (ypos >= HEIGHT) || (ypos < 0))
                    return 0;
            }
        }
    }
    return 1;
}
/*
void Erase_Func(int ypos, int xpos, int currentBlock, int blockRotate){
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			if(block[currentBlock][blockRotate][i][j] && (i+blockY)){
				move(blockY+i+1, blockX+j+1);
				printw(".");
			}
		}
	}
	move(HEIGHT, WIDTH+10);
}
*/
///////////////////////////
/*
void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	switch(command){
	case KEY_UP:
		blockRotate=(blockRotate)%4; break;
	case KEY_DOWN:
		blockY--; break;
	case KEY_LEFT:
		blockX++; break;
	case KEY_RIGHT:
		blockX--; break;
	default:
		break;
	}

	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	int i, j;
	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			if(block[currentBlock][blockRotate][i][j]==1 && (i+blockY>=0)){
		//	if(1){
				move(blockY+i+1, blockX+j+1);
				printw(".");

			}
		}
	}

	//3. 새로운 블록 정보를 그린다.
	//move(HEIGHT, WIDTH+10);
	DrawBlock(blockY, blockX, currentBlock, blockRotate, ' ');
	move(HEIGHT, WIDTH+10);
}
*/
////////////////

void Delete_Block(int ypos, int xpos, int currentBlock, int blockRotate) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            if (block[currentBlock][blockRotate][i][j] && (i + ypos >= 0)) {
                move(i + ypos + 1, j + xpos + 1);
                printw(".");
            }
        }

    move(HEIGHT, WIDTH + 10);
}

void DrawChange(char f[HEIGHT][WIDTH], int command, int currentBlock, int blockRotate, int blockY, int blockX) {
    //user code
    int Ylength;
    //1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
    //2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.

    switch (command) {
        case KEY_UP:
            Delete_Block(blockY, blockX, currentBlock, (blockRotate + 3) % 4);
            Ylength = FinalShapeDrop(blockY, blockX, currentBlock, (blockRotate + 3) % 4);
            Delete_Block(Ylength, blockX, currentBlock, (blockRotate + 3) % 4);
            break;
        case KEY_DOWN:
            Delete_Block(blockY - 1, blockX, currentBlock, blockRotate);
            break;
        case KEY_LEFT:
            Delete_Block(blockY, blockX + 1, currentBlock, blockRotate);
            Ylength = FinalShapeDrop(blockY, blockX + 1, currentBlock, blockRotate);
            Delete_Block(Ylength, blockX + 1, currentBlock, blockRotate);
            break;
        case KEY_RIGHT:
            Delete_Block(blockY, blockX - 1, currentBlock, blockRotate);
            Ylength = FinalShapeDrop(blockY, blockX - 1, currentBlock, blockRotate);
            Delete_Block(Ylength, blockX - 1, currentBlock, blockRotate);
            break;
        default:
            break;

    }
    /*
    switch(command){
        case KEY_UP:
            blockRotate=(blockRotate+3)%4; break;
        case KEY_DOWN:
            blockY--; break;
        case KEY_LEFT:
            blockX++; break;
        case KEY_RIGHT:
            blockX--; break;
    }

    Delete_Block(blockY, blockX, currentBlock, blockRotate);
    */

    //3. 새로운 블록 정보를 그린다.
    DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
    //DrawShadow(blockY, blockX, currentBlock, blockRotate);
    //DrawBlock(blockY, blockX, currentBlock, blockRotate, ' ');
    //DrawShadow(blockY, blockX, currentBlock, blockRotate);
}


////
void BlockDown(int sig) {
    // user code
    if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)) {
        blockY++;
        DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
    } else {//
        score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
        //
        if (blockY == -1) gameOver = 1;
        else {
            score += DeleteLine(field);
            for (int i = 0; i < 2; i++)
                nextBlock[i] = nextBlock[i + 1];
            nextBlock[2] = rand() % 7;
            DrawNextBlock(nextBlock);

            blockY = -1;
            blockX = WIDTH / 2 - 2;
            blockRotate = 0;
            PrintScore(score);
        }
        DrawField();
    }
    //강의자료 p26-27의 플로우차트를 참고한다.
    timed_out = 0;
}

int AddBlockToField(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
    int cnt = 0;
    // user code
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block[currentBlock][blockRotate][i][j]) {
                if (0 <= i + blockY && i + blockY < HEIGHT && 0 <= j + blockX && j + blockX < WIDTH) {
                    f[i + blockY][j + blockX] = 1;

                    if (HEIGHT - 1 == i + blockY) cnt++;
                }
            }

        }
    }
    //Block이 추가된 영역의 필드값을 바꾼다.
    return cnt * 10;
}

int DeleteLine(char f[HEIGHT][WIDTH]) {
    // user code

    //1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
    int er_line_cnt = 0;
    bool all_fit = 0;

    for (int i = 0; i < HEIGHT; i++) {
        all_fit = 1;
        for (int j = 0; j < WIDTH; j++) {
            if (!f[i][j]) all_fit = 0;
        }
        if (all_fit) {
            er_line_cnt++;
            for (int ypos = i; ypos >= 1; ypos--) {
                for (int xpos = 0; xpos < WIDTH; xpos++)
                    f[ypos][xpos] = f[ypos - 1][xpos];
            }
            for (int xpos = 0; xpos < WIDTH; xpos++) f[0][xpos] = 0;
            i--;
        }
    }
    //2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.

    return er_line_cnt * er_line_cnt * 100;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID, int blockRotate) {
    // user code
    int Ylength = FinalShapeDrop(y, x, blockID, blockRotate);
    DrawBlock(Ylength, x, blockID, blockRotate, '/');
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {
    DrawShadow(y, x, blockID, blockRotate);
    DrawBlock(y, x, blockID, blockRotate, ' ');
}

void createRankList() {
    // user code
}

void rank() {
    // user code
}

void writeRankFile() {
    // user code
}

void newRank(int score) {
    // user code
}

void DrawRecommend(int y, int x, int blockID, int blockRotate) {
    // user code
}

int recommend(RecNode *root) {
    int max = 0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

    // user code

    return max;
}

void recommendedPlay() {
    // user code
}

int FinalShapeDrop(int y, int x, int blockID, int blockRotate) {
    int Ylength = y;
    while (CheckToMove(field, nextBlock[0], blockRotate, Ylength + 1, x))
        Ylength++;
    return Ylength;
}

