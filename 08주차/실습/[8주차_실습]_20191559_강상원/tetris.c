#include "tetris.h"
////////////8주차 실습 추가->탐색 노드 깊이 설정
#define depth_of_travel 3
// 현재 노드, 다음 노드, 그 다음 노드 보기로 설정
//!!주의 5 이상으로 하면 서버 터진다고 조교님이 당부!!
static struct sigaction act, oact;
HEAD head_node;

int main() {
    int exit = 0;
    ////
    createRankList();
    ////
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
            case MENU_RANK:
                rank();
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
    writeRankFile();
    return 0;
}

void InitTetris() {

    for (int j = 0; j < HEIGHT; j++)
        for (int i = 0; i < WIDTH; i++)
            field[j][i] = 0;

    for (int i = 0; i < 5; i++)
        nextBlock[i] = rand() % 7;

    blockRotate = 0;
    blockY = -1;
    blockX = WIDTH / 2 - 2;

    score = 0;
    gameOver = 0;
    timed_out = 0;

    DrawOutline();
    DrawField();
    recStart();
    DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);

    ///////////


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
    DrawBox(9, WIDTH + 10, 4, 8);
    /* score를 보여주는 공간의 태두리를 그린다.*/
    //move(9,WIDTH+10);
    move(15, WIDTH + 10);
    printw("SCORE");
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
    int game_over = 1;
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
            game_over = 0;
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
    if (game_over)
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
                if (f[blockY + i][blockX + j] == 1) return 0;
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
    int Ylength;
    DrawField();
    //DrawHoldBlock();
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

    DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
}


void BlockDown(int sig) {
    // user code
    //끝까지 내리기
    if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)) {
        blockY++;
        DrawChange(field, -100, nextBlock[0], blockRotate, blockY, blockX);
    } else {
        if (!CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX)) {
            timed_out = 0;
            gameOver = 1;
            return;
        }
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++)
                if (block[nextBlock[0]][blockRotate][i][j] && blockY + i < 0) {
                    timed_out = 0;
                    gameOver = 1;
                    return;
                }
        }
        //점수 계산
        score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
        score += DeleteLine(field);

        //한 칸씩 밀기
        for (int i = 0; i < 2; i++)
            nextBlock[i] = nextBlock[i + 1];

        nextBlock[2] = rand() % 7;

        DrawNextBlock(nextBlock);

        blockX = WIDTH / 2 - 2;
        blockY = -1;
        blockRotate = 0;

        recStart();
        PrintScore(score);
        DrawField();
    }
    timed_out = 0;
}


int AddBlockToField(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
    int cnt = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block[currentBlock][blockRotate][i][j] == 1) {
                f[blockY + i][blockX + j] = 1;
                if (blockY + i >= HEIGHT - 1 || f[blockY + i + 1][blockX + j] == 1)
                    cnt++;
            }
        }
    }
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


int returnY(char field[HEIGHT][WIDTH], int Ypos, int Xpos, int blockID, int blockRotate) {
    int i, j;
    for (; Ypos < HEIGHT; Ypos++) {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if(block[blockID][blockRotate][i][j] && (Ypos+i>= 0) && field[Ypos + i][Xpos + j]) break;
                if (block[blockID][blockRotate][i][j] && (Ypos + i >= HEIGHT)) break;
            }
            if (j < 4) break;
        }
        if (i < 4) break;
    }
    return Ypos - 1;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID, int blockRotate) {

    //////////////////
    // user code
    //int Ylength = FinalShapeDrop(y, x, blockID, blockRotate);
    //DrawBlock(Ylength, x, blockID, blockRotate, '/');


    shadowblockY = FinalShapeDrop(y, x, blockID, blockRotate);
    DrawBlock(shadowblockY, x, blockID, blockRotate, '/');//그림자 블럭 그리기
}


void createRankList() {
    // user code
    FILE *fp = fopen("rank.txt", "r");
    head_node.HEAD = NULL;
    head_node.n = 0; // 처음엔 0명으로 초기화
    char inputname[NAMELEN];
    fscanf(fp, "%d", &(head_node.n)); //괄호...

    int inputscore;

    for (int i = 0; i < head_node.n; i++) {
        fscanf(fp, "%s%d", inputname, &inputscore);
        new_node(inputname, inputscore);
        //new_node 구현할 것!
    }
    fclose(fp);

}


void rank() {
    // user code
    char name[NAMELEN];
    clear();
    char opt;
    int inputdelete = 0;
    do {
        clear();
        printw("1. list ranks from X to Y\n");
        printw("2. list ranks by a specific name\n");
        printw("3. delete a specific rank\n");
        opt = wgetch(stdscr);
    } while (opt < '1' || opt > '3');

    echo(); //...
    int X, Y = -1; //초기화를 음수

    switch (opt) {
        case '1':
            printw("X: ");
            scanw("%d", &X);
            printw("Y: ");
            scanw("%d", &Y);
            if (X < 0) X = 1;
            if (Y < 0) Y = head_node.n;
            noecho(); //...
            //////////////////
            //printf("Hello\n\n");
            int index = 1;
            Node *idx = head_node.HEAD;
            printw("        name        |    score\n");
            printw("---------------------------------------\n");
            //예외 처리를 할 것인가? ->조교님께 질문
            if (X > Y) {
                printw("search failure: the rank not in the list\n");
                break;
            }

            while (1) { //X부터 Y까지 반복 출력
                if (idx == NULL) break;
                if (X <= index && index <= Y) printw(" %-19s| %-12d\n", idx->name, idx->score);
                index++;
                idx = idx->link;
            }
            //////////////////
            break;
        case '2':
            echo();
            printw("input the name: ");
            scanw("%s", name);
            noecho();
            searchname(name);
            break;
        case '3':
            echo();
            printw("Input the rank: ");
            scanw("%d", &inputdelete);
            noecho();
            rank_delete(inputdelete);
            break;
    }
    noecho();
    getch();
}


void writeRankFile() {
    // user code
    FILE *fp = fopen("rank.txt", "w");
    Node *insertnode = head_node.HEAD;

    fprintf(fp, "%d\n", head_node.n);
    while (1) { //NULL 만날 때까지 반복
        fprintf(fp, "%s %d\n", insertnode->name, insertnode->score);
        if (insertnode->link == NULL) break;
        insertnode = insertnode->link;
    }
    fclose(fp);
}

void newRank(int score) {
    // user code
    clear(); //뺴먹지 말자...
    echo();
    char inputname[NAMELEN];
    printw("your name: ");
    scanw("%s", inputname);
    noecho();
    insert_node(inputname, score);

}

void new_node(char *name, int score) {
    Node *BACK = head_node.HEAD;
    Node *NEW_NODE;
    NEW_NODE = (Node *) malloc(sizeof(Node));
    NEW_NODE->name[0] = '\0'; // 없을 경우를 대비해 미리 초기화
    NEW_NODE->score = score;
    strcpy(NEW_NODE->name, name);
    NEW_NODE->link = NULL;

    if (head_node.HEAD == NULL) head_node.HEAD = NEW_NODE;
    else {
        while (1) {
            if (BACK->link == NULL) break;
            BACK = BACK->link;
        }

        BACK->link = NEW_NODE;
    }

}


void DrawBlockWithFeatures(int blockY, int blockX, int currentBlock, int blockRotate) {
    DrawRecommend(Y_rec, X_rec, currentBlock, R_rec);
    DrawShadow(blockY, blockX, currentBlock, blockRotate);

    DrawBlock(blockY, blockX, currentBlock, blockRotate, ' ');
    move(19, WIDTH + 11);
}


void insert_node(char *name, int score) {
    //
    Node *first = head_node.HEAD;
    //
    Node *front = head_node.HEAD;
    Node *idxx = head_node.HEAD;
    Node *new;
    new = (Node *) malloc(sizeof(Node));
    new->name[0] = '\0';
    new->score = score;
    new->link = NULL;
    strcpy(new->name, name);
    head_node.n++;

    if (head_node.HEAD == NULL) head_node.HEAD = new; //No Node
    else if (head_node.HEAD->link == NULL) { //1 Node
        if ((new->score) > (head_node.HEAD->score)) {
            new->link = head_node.HEAD;
            head_node.HEAD = new;
        } else head_node.HEAD->link = new;
    } else { //2 or more Nodes
        ////인데, 첫번째보다 더 큰 경우->예외
        if ((new->score) > (head_node.HEAD->score)) {
            new->link = head_node.HEAD;
            head_node.HEAD = new;
            return;
        }
        ////
        while (1) {
            if ((new->score) > (idxx->score)) break;
            if (idxx->link == NULL) break;
            front = idxx;
            idxx = idxx->link;
        }
        if (front->score > new->score && new->score > idxx->score) {
            front->link = new;
            new->link = idxx;
        } else idxx->link = new;

    }

}

//----------------------------7주차 숙제 시작-------------------------------
void searchname(char *name) {
    bool success = false;
    Node *idx = head_node.HEAD;
    printw("        name        |    score\n");
    printw("---------------------------------------\n");
    while (1) {
        if (!strcmp(idx->name, name)) { // 비교
            success = true;
            printw(" %-19s| %-12d\n", idx->name, idx->score);
        }
        if (idx->link == NULL) break;
        idx = idx->link; // 순차 탐색
    }
    if (!success) printw("search failure: no rank in the list\n");
}

void rank_delete(int deletenum) {
    Node *deletenode = head_node.HEAD;
    Node *front;
    int deletenode_num = 1;

    if (0 < deletenum && deletenum <= head_node.n) {
        if (deletenum != 1) {
            while (deletenode_num < deletenum) {
                front = deletenode;
                deletenode = deletenode->link;
                deletenode_num++;
            }
            front->link = deletenode->link;
            head_node.n--;
            free(deletenode);
            printw("result: the rank deleted\n");
        } else {
            head_node.HEAD = head_node.HEAD->link;
            head_node.n--;
            free(deletenode);
            printw("result: the rank deleted\n");
        }


    } else printw("search failure: no rank in the list\n");
}
//----------------------------7주차 숙제 ----------------------------------

//----------------------------8주차 실습 시작-------------------------------
void DrawRecommend(int y, int x, int blockID, int blockRotate) {
    DrawBlock(y, x, blockID, blockRotate, 'R');
}

int recommend(NodePTR root) {
    RecNode node;
    NodePTR child = &node;
    int Xpos, Ypos, rot;
    int accumulatedScore; //최대 점수
    int result = -1; //결괏값 음수: 오류 방지



    for (rot = 0; rot < 4; rot++) {
        for (Xpos = -2; Xpos < 13; Xpos++) {
            if (!CheckToMove(root->recField, nextBlock[root->lv], rot, 0, Xpos)) continue;
            for(int i=0; i<HEIGHT; i++)
                for(int j=0; j<WIDTH; j++)
                    child->recField[i][j]=root->recField[i][j];

            child->lv = root->lv + 1;
            Ypos = 0; //seg fault 방지
            Ypos = returnY(child->recField, Ypos, Xpos, nextBlock[root->lv], rot);

            //Ypos=FinalShapeDrop()

            //점수 계산
            accumulatedScore = AddBlockToField(child->recField, nextBlock[root->lv], rot, Ypos, Xpos);
            accumulatedScore += DeleteLine(child->recField);
            //재귀
            if (child->lv < depth_of_travel) accumulatedScore += recommend(child);
            //최댓값 찾기
            if (result <= accumulatedScore) {
                if (root->lv == 0) {
                    if (Y_rec < Ypos || result < accumulatedScore) {
                        X_rec = Xpos;
                        Y_rec = Ypos;
                        R_rec = rot;
                    }
                }
                result = accumulatedScore;
            }

        }
    }
    return result;
}

void recStart() {
    RecNode node;
    NodePTR root = &node;
    for(int i=0; i<HEIGHT; i++)
        for(int j=0; j<WIDTH; j++)
            root->recField[i][j]=field[i][j];

    root->lv = 0;
    X_rec = 0;
    Y_rec = 0;
    R_rec = 0; //초기화하고 시작!
    recommend(root);

    return;
}


//----------------------------8주차 실습-------------------------------------

int FinalShapeDrop(int y, int x, int blockID, int blockRotate) {
    int Ylength = y;
    while (CheckToMove(field, nextBlock[0], blockRotate, Ylength + 1, x))
        Ylength++;
    return Ylength;
}