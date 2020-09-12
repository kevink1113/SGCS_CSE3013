#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
using namespace std;

void line_feed();
void first_rand();
void ver_write();
void hor_write();
void vertical_meet();
void meet_rand();
void meet_rand_last();

int WIDTH, HEIGHT;

vector<int> horizontal_wall; //가로 벽 그리는 배열->포인터
vector<int> vertical_wall; //세로 벽 그리는 배열->포인터
vector<int> maze; //전체 배열

int Id = 0; //방에 적힌 숫자 의미
int before_wall = -1;
FILE *fp;

int main() {
    fp = fopen("mazeRes.maz", "wt");

    //미로의 너비, 높이 입력받기
    cin >> WIDTH >> HEIGHT;

    horizontal_wall.resize(WIDTH);
    vertical_wall.resize(WIDTH-1);
    maze.resize(WIDTH, 0);

    //첫 line
    line_feed();
    first_rand();
    ver_write();

    for (int k = 1; k < HEIGHT - 1; k++) { //반복
        vertical_meet();
        hor_write();
        meet_rand();
        ver_write();
    }

    //마지막 line
    vertical_meet();
    hor_write();

    meet_rand_last();
    ver_write();
    line_feed();

    return 0;
}


void line_feed() { //한 줄 쭉 출력 (+-+-+-+)
    for (int i = 0; i < WIDTH; i++)
        fprintf(fp, "+-");
    fprintf(fp, "+\n");
}

void first_rand() {
    srand(time(NULL));
    for (int i = 0; i < WIDTH - 1; i++) {
        vertical_wall[i] = rand() % 2; //0 아니면 1
        if (vertical_wall[i] == 1) { //1일 경우, 막혀 있다는 의미이므로 그 이전 칸까지 모두 뚫는 처리.
            for (int j = (before_wall + 1); j <= i; j++)
                maze[j] = Id; //방에 적힌 숫자 통일

            before_wall = i; // 이전 벽 위치
            Id++; // 이제 다음 집합에 적힌 숫자는, 1 증가.
        }
    }
    for (int i = before_wall + 1; i < WIDTH; i++)
        maze[i] = Id; //마무리 처리->나머지 곳도 숫자 맞추기.
    Id++;
}

void ver_write() {
    fprintf(fp, "|");
    for (int i = 0; i < WIDTH - 1; i++) {
        fprintf(fp, " ");
        if (vertical_wall[i] == 1) fprintf(fp, "|");
        else fprintf(fp, " ");
    }
    fprintf(fp, " |\n");
}

void hor_write() {
    fprintf(fp, "+");
    for (int i = 0; i < WIDTH; i++) {
        if (horizontal_wall[i] == 1) fprintf(fp, "-+");
        else fprintf(fp, " +");
    }
    fprintf(fp, "\n");

}

void vertical_meet() {
    int before_num;
    int ver_meet = 0;

    before_num = maze[0];
    for (int i = 0; i < WIDTH; i++) {
        horizontal_wall[i] = rand() % 2;
        if (horizontal_wall[i] == 0)
            ver_meet = 1;

        if (i < WIDTH - 1) {
            if (before_num != maze[i + 1]) { //적어도 한 번 만나야...
                if (ver_meet == 0) horizontal_wall[i] = 0;
                else ver_meet = 0;
                before_num = maze[i + 1];
            }
        }
        if ((i == WIDTH - 1) && (ver_meet == 0))
            horizontal_wall[i] = 0;

        if (horizontal_wall[i]) {
            maze[i] = Id;
            Id++;
        }
    }
}

void meet_rand() {
    int shall_we_meet = 0;
    int before_num;

    for (int i = 0; i < WIDTH - 1; i++) {
        if (maze[i] != maze[i + 1]) {
            shall_we_meet = rand() % 2; //만날지 여부 결정
            if (shall_we_meet) {
                before_num = maze[i + 1];
                maze[i + 1] = maze[i];
                for (int j = 0; j < WIDTH; j++)
                    if (maze[j] == before_num) maze[j] = maze[i];
                vertical_wall[i] = 0;
            } else vertical_wall[i] = 1;
        } else vertical_wall[i] = 1;
    }
}

void meet_rand_last() {
    int before_num;
    for (int i = 0; i < WIDTH - 1; i++) {
        if (maze[i] != maze[i + 1]) {
            vertical_wall[i] = 0;
            before_num = maze[i + 1];
            maze[i + 1] = maze[i];
            for (int j = 0; j < WIDTH; j++) {
                if (maze[j] == before_num) maze[j] = maze[i];
            }
        } else vertical_wall[i] = 1;
    }
}