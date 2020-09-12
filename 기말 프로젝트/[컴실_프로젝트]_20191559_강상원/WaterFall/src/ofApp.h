#pragma once

#include "ofMain.h"
#include <vector>


struct WaterLine{       // 물받침판의 정보 저장하는 구조체
    bool active=false;  // 물받침판의 선택 여부
    double startX;      // 시작 x좌표
    double startY;      // 시작 y좌표
    double toX;         // 끝 x좌표
    double toY;         // 끝 y좌표
    
    float slope;        // 물받침판의 기울기
};

struct WaterPoint{      // 물 시작 점의 정보 저장하는 구조체
    bool active;        // 물 시작 점의 선택 여부
    int X;              // x좌표
    int Y;              // y좌표
};

struct WaterDrop{       // 물방울의 정보 저장하는 구조체
    float xpos;         // x좌표
    float ypos;         // y좌표
};

struct Destination{     // 목적지의 정보 저장하는 구조체
    int x1;             // 좌표값
    int reached = 0;    // 도달 성공 유무
};


class ofApp : public ofBaseApp{
    private:
        int tmpScore;                       // 임시 점수 저장
        int draw_flag;                      // 그리기 유무_flag
        int flow_flag;                      // 물 흐름 유무_flag
        bool SUCCESS = false;               // 성공_여부 flag
        bool FAIL = false;                  // 실패 여부_flag
        bool start_page=true;               // 시작 페이지 보이기 유무_flag
        bool menu_page=false;               // 메뉴 페이지 보이기 유무_flag
    
        int activeIdx=0;                    // 활성화된 점 위치
        int activeLine=0;                   // 활성화된 물받침대 위치
        int successNum=0;                   // 도달한 목적지들의 개수
        int current_level=0;                // 현재 플레이 중인 레벨
        
        float time;                         // 지난 시간
        float time_left;                    // 남은 시간
        short int LEVELS[12]={0, };         // 레벨들 각각의 정보 (성공 여부, 별점)
    
        vector<pair<int, int>> obstacles;   // 장애물 정보 저장 (first: x좌표, second: y좌표)
    
        vector<WaterLine> lines;            // 물받침대들의 정보 저장 vector
        vector<WaterPoint> points;          // 물 시작 점들의 정보 저장 vector
        vector<WaterDrop> drops;            // 물방울들의 정보 저장 vector
        vector<Destination> dests;          // 목적지들의 정보 저장 vector
        
    public:
        void setup();                                   // setup 함수
        void update();                                  // update 함수
        void draw();                                    // 그리기 함수

        void keyPressed(int key);                       // 키 눌림 함수
        void keyReleased(int key);                      // 키 놓음 함수
        void mouseMoved(int x, int y );                 // 마우스 움직임 함수
        void mouseDragged(int x, int y, int button);    // 마우스 드래그 함수
        void mousePressed(int x, int y, int button);    // 마우스 누름 함수
        void mouseReleased(int x, int y, int button);   // 마우스 놓음 함수
        void mouseEntered(int x, int y);                // 마우스 입력 함수
        void mouseExited(int x, int y);                 // 마우스 벗어남 함수
        void windowResized(int w, int h);               // 창 크기 조정 함수
        void dragEvent(ofDragInfo dragInfo);            // 드래그 사건 함수
        void gotMessage(ofMessage msg);                 // 메시지 받기 함수
        
        
        void UserLine(WaterLine line);                                  // 물받침대 그리기 함수
        void UserCircle(WaterPoint point);                              // 물 시작 점 그리기 함수
        void UserDest(Destination dest);                                // 목적지 그리기 함수
        void drawDrop(WaterDrop water);                                 // 물방울 그리기 함수
        void moveCal(WaterPoint activePoint, vector<WaterLine> lines);  // 물방울 움직임 위치 계산 함수
    
        void OPEN(int level);                                           // 파일 입력 함수
        void save();                                                    // 세이브 함수
        void drawObstacles(pair<int, int>);                             // 장애물 그리기 함수
        void drawStar(int x, int y, int point, float size);             // 별 그리기 함수
    
        ofTrueTypeFont verdana14;                           // 크기 14인 서체 정보 저장
        ofTrueTypeFont verdana30;                           // 크기 30인 서체 정보 저장
        
        ofSoundPlayer success_sound;                        // 성공 사운드
        ofSoundPlayer water_sound;                          // 물 소리
        ofSoundPlayer selection_sound;                      // 메뉴 선택 소리
        ofSoundPlayer click_sound;                          // 딸깍 소리 (물받침대 회전 시)
        
        ofImage opening;                                    // 시작 화면 타이틀 이미지
    
};
