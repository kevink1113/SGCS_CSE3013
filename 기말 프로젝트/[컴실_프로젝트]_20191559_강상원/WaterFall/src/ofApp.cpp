#include "ofApp.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

int idx=0;      // 물방울 그리기 끝점의 index

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLineWidth(5);                          // 선 굵기 기본: 5
    ofSetBackgroundColor(ofColor::lavender);    // 배경 색: 라벤더 색
    this->draw_flag=0;                          // 기본 : 그리기 flag 끔
    
    success_sound.load("success_sound.wav");    // 성공 사운드 로드
    water_sound.load("water_sound.wav");        // 물 소리 로드
    selection_sound.load("selection.wav");      // 선택 사운드 로드
    click_sound.load("click.wav");              // 클릭 사운드 로드
    
    opening.load("WaterFall_opening.jpg");      // 시작 타이틀 이미지
    
    
    ofTrueTypeFont::setGlobalDpi(72);           // 서체 DPI 설정

    verdana14.load("verdana.ttf", 14, true, true);  // verdana14에 대한 모양 설정
    verdana14.setLineHeight(18.0f);
    verdana14.setLetterSpacing(1.037);

    verdana30.load("verdana.ttf", 30, true, true);  // verdana30에 대한 모양 설정
    verdana30.setLineHeight(34.0f);
    verdana30.setLetterSpacing(1.035);

    this->LEVELS[0]= 1;                             // 첫 레벨은 처음 플레이시에도 활성화되어 있어야 한다.
}

//--------------------------------------------------------------
void ofApp::update(){
    if(this->flow_flag==1)
        moveCal(this->points[this->activeIdx], this->lines);    // flow_flag가 켜지면 물방울의 움직임 위치 계산 함수 수행
    
    if(this->flow_flag){
        successNum= this->dests[0].reached;
        for(int i=0; i<this->dests.size(); i++)                 // 목적지 하나씩 방문 여부 체크
            successNum *= dests[i].reached;                     // 만약 하나라도 도착 안했다면,
        
        if(successNum == 1){                                    // 만약 성공(레벨 클리어)하면,
            cout << "Real Success!" << endl;
            save();                                             // 세이브 함수 수행
            water_sound.stop();                                 // 물 소리를 멈춘다.
            
            this->SUCCESS = true;                               // SUCCESS flag 설정
            LEVELS[current_level]= 1;                           // 처음엔, 점수 1
            success_sound.play();                               // 성공 사운드 재생
            flow_flag=0;                                        // 물 흐름 flag 끄기
            cout << "Time left : " << time_left << "s" << endl; // 남은 시간 콘솔 출력
            
            tmpScore = (int)(time_left);                        // 아래의 기준에 따라 점수 배정
            
            if(time_left >17) tmpScore = 30;        // 30점
            else if (time_left>14) tmpScore = 25;   // 25점
            else if (time_left>11) tmpScore = 20;   // 20점
            else if (time_left>8) tmpScore = 15;    // 15점
            else if (time_left>5) tmpScore = 10;    // 10점
            else if (time_left>2) tmpScore = 5;     // 5점
            
            if(LEVELS[current_level] < tmpScore) LEVELS[current_level]=tmpScore;    // 점수가 더 크다면, 갱신
            save();     // 세이브
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(this->start_page){           // 시작 페이지 그리기
        
        ofSetColor(ofColor::white); // 배경 색: 흰색
        opening.draw(230, 100);     // 타이틀 이미지 띄우기
        
        
        // "Press Enter key to Start"를 sin 함수를 이용해 크기 조정, 움직이게끔 띄우기
        ofPushMatrix(); // 현재 좌표계 push
            string scaleAA = "Press Enter key to Start";
            ofRectangle bounds = verdana14.getStringBoundingBox(scaleAA, 0, 0); // 텍스트 바운드 설정

            ofTranslate(400 + bounds.width/2, 584 + bounds.height / 2, 0);
            ofScale(2.0 + sin(ofGetElapsedTimef()), 2.0 + sin(ofGetElapsedTimef()), 1.0);   // 크기 배율 (sin 이용), 지난 시간에 따른 sin 값으로 배율.
            
            ofSetColor(245, 58, 135); //색깔 붉은색
            verdana14.drawString(scaleAA, -bounds.width/2, bounds.height/2 );       // 최종 그리기
        ofPopMatrix();  // 좌표계 pop
    }
    
    if(this->menu_page){
        ofFill();
        for(int i=0; i<10; i++){            // 레벨 10까지
            string levelN=to_string(i+1);
            string levelName = "Lv. ";
            levelName = levelName + levelN; // Lv. 1 형식으로 입력 파일이름 설정
            if(LEVELS[i]>0) ofSetColor(ofColor::lightSkyBlue);  // 활성화된 레벨 하늘색 색깔 설정
            else ofSetColor(ofColor::gray);                     // 비활성화된 레벨 회색 색깔 설정
            
            ofDrawRectangle(20+100*i, 150, 90, 125);            // 각 레벨에 해당하는 버튼 그리기
            ofSetColor(ofColor::white);                         // 글씨 색: 흰색
            verdana30.drawString(levelName, 20+100*i, 250);     // 버튼에 레벨 이름 쓰기
        }
        
        for(int i=0; i<10; i++)
            drawStar(50+100*i, 180, LEVELS[i] ,0.15);           // 각 레벨의 저장된 점수에 따라 별점 그리기
    }
    
    if(flow_flag==1){
        this->time=ofGetElapsedTimef();                         // 지난 시간 불러오기
        this->time_left=30.0-time;                              // 남은 시간 : 30 - 지난 시간
        
        ofSetColor(ofColor::navy);      // 색깔: 네이비
        verdana14.drawString("time left - ", 30, 195-160);
        verdana30.drawString(to_string(time_left), 30, 229-160);    // 남은 시간 표시하기
        
        if(time_left <= 0){
            FAIL = true;                                        // 미리 정한 시간 기준(30초)을 초과하였을 때, 실패!
        }
    }
    
    if(this->draw_flag==1){
        // 게임 플레이 팁들 보여주기
        ofSetColor(ofColor::navy);
        verdana14.drawString("press 's' to start water flow.", 690, 30);
        verdana14.drawString("press 'left, right' key to select line.", 690, 45);
        verdana14.drawString("press 'up, down' key to rotate selected line.", 690, 60);
        verdana14.drawString("avoid red circles.", 690, 75);
        verdana14.drawString("try to reach purple destinations in time.", 690, 90);
        verdana14.drawString("Tip: The steeper the slope, the faster the water flows.", 250, 30);
        
        //verdana30.drawString(to_string(time_left), 30, 229-160);
        
        // 목적지들 그리기
        for(int i=0; i<this->dests.size(); i++)
            UserDest(this->dests[i]);
        
        // 방해물들 그리기
        for(int i=0; i<this->obstacles.size(); i++)
            drawObstacles(this->obstacles[i]);
        
        // 물받침대들 그리기
        ofSetColor(143, 0, 25);
        for(int i=0; i<this->lines.size(); i++)
            UserLine(this->lines[i]);
        
        // 물 시작 점 그리기
        ofSetColor(0, 0, 0);
        for(int i=0; i<this->points.size(); i++)
            UserCircle(this->points[i]);
    }
    
    // 물방울들 그리기
    if(this->flow_flag==1 && this->draw_flag==1){
        for(int i=0; i<idx; i++){
            drawDrop(this->drops[i]);   // idx번째까지 물방울 그리기
        }
        idx++;      // 그리기 끝점의 인덱스 1 증가
    }
    
    if(this->SUCCESS){      // 성공(레벨 클리어)하면,
        ofFill();
        ofSetColor(ofColor::ivory);             //아이보리 색으로
        ofDrawRectangle(170, 100, 650, 525);    // 팝업 창 띄우고
        
        ofPushMatrix();
            string scaleAA = "SUCCESS!!";    // 이전과 마찬가지로 크기 조절되는 형식으로 "SUCCESS!!" 문구 띄우기
            ofRectangle bounds = verdana30.getStringBoundingBox(scaleAA, 0, 0);
            ofTranslate(410 + bounds.width/2, 384 + bounds.height / 2, 0);
            ofScale( 2.0+ sin(ofGetElapsedTimef()), 2.0+sin(ofGetElapsedTimef()), 1.0);
            ofSetColor(ofColor::mediumSeaGreen);
            verdana30.drawString(scaleAA, -bounds.width/2, bounds.height/2 );
        ofPopMatrix();
       
        ofFill();
        ofSetColor(ofColor::lightSkyBlue);      // 하늘색으로
        ofDrawRectangle(220, 500, 150, 75);     // 첫번째 버튼:
        ofSetColor(ofColor::white);
        verdana30.drawString("Menu", 250, 550); // '메뉴' 버튼 그리기
        
        ofSetColor(ofColor::lightGreen);        // 연두색으로
        ofDrawRectangle(420, 500, 150, 75);     // 두번째 버튼:
        ofSetColor(ofColor::white);
        verdana30.drawString("Retry", 450, 550);    // '재시도' 버튼 그리기
        
        ofSetColor(ofColor::springGreen);       // 형광 연두색으로
        ofDrawRectangle(620, 500, 150, 75);     // 세번째 버튼:
        ofSetColor(ofColor::white);
        verdana30.drawString("Next", 650, 550); // '다음 레벨' 버튼 그리기
        
        drawStar(360, 250, tmpScore, 0.7);      // 점수에 따라 별 모양 그리기
    }
    
    //-----
    if(this->FAIL){                         // 실패하면,
        this->flow_flag = false;
        //this->draw_flag = false;
        water_sound.stop();                 // 물소리 멈추기
        
        ofFill();
        ofSetColor(ofColor::ivory);         // 아이보리 색으로
        ofDrawRectangle(170, 100, 650, 525);    // 팝업 창 띄우고
        
        ofPushMatrix();
            string scaleAA = "FAILURE!!";   // 이전과 마찬가지로 크기 조절되는 형식으로 "FAILURE!!" 문구 띄우기
            ofRectangle bounds = verdana30.getStringBoundingBox(scaleAA, 0, 0);
            ofTranslate(410 + bounds.width/2, 384 + bounds.height / 2, 0);
            ofScale( 2.0+ sin(ofGetElapsedTimef()), 2.0+sin(ofGetElapsedTimef()), 1.0);
            ofSetColor(245, 58, 135); //색깔 붉은색
            verdana30.drawString(scaleAA, -bounds.width/2, bounds.height/2 );
        ofPopMatrix();
        
        ofFill();
        ofSetColor(ofColor::lightSkyBlue);      // 하늘색으로
        ofDrawRectangle(220, 500, 150, 75);     // 첫번째 버튼:
        ofSetColor(ofColor::white);
        verdana30.drawString("Menu", 250, 550); // '메뉴' 버튼 그리기
        
        ofSetColor(ofColor::lightGreen);        // 연두색으로
        ofDrawRectangle(420, 500, 150, 75);     // 두번째 버튼:
        ofSetColor(ofColor::white);
        verdana30.drawString("Retry", 450, 550); // '재시도' 버튼 그리기
        
        drawStar(360, 250, 0, 0.7);             // 0점에 해당되는 별 그리기
    }
    //-----
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //cout << key << endl;
    if(key==OF_KEY_RETURN){                     // 엔터 키 누르면
        string progress_input="progress.txt";   // 진행 상황 파일 입력받기
        ifstream input_file(ofToDataPath(progress_input), ifstream::in);    // 지정된 파일 이름을 파일 입력
        
        selection_sound.play();     // 선택 사운드 재생
        
        int tmp;
        if(input_file.is_open()){   // 파일 열림 성공시
            //선분
            for(int i=0; i<10; i++){    //레벨 10까지 진행상황 콘솔에 출력
                input_file >> LEVELS[i];
                cout << "Progress " << i+1 << " : " << LEVELS[i] << endl;
            }
        }
        else cout << "Failed to open progress_input" << endl;   // 열림 실패 시 에러 메시지 콘솔에 출력
        
        cout << "Game Start!" << endl;
        if(this->start_page){
            this->start_page=false;     // 시작 화면 끄기
            this->menu_page=true;       // 메뉴 화면 키기
        }
    }
    
    if (key=='s' && draw_flag && !flow_flag){   // 's' 키를 누르면,
        this->flow_flag=1;                      // 물 흐름 flag 켜기
        cout << "Start Flow key Pressed" << endl;
        water_sound.setLoop(true);              // 물 소리 loop 설정
        water_sound.play();                     // 물 소리 켜기
        ofResetElapsedTimeCounter(); // 시간 시작!!
    }
    
    if (key=='e'){                              // 'e' 키를 누르면,
        this->flow_flag=0;                      // 물 흐름 중지
        cout << "End Flow key Pressed" << endl;
        water_sound.stop();                     // 물 소리 중지
        idx=0;                                  // idx 초기화
    }
    
    if (key==OF_KEY_LEFT){                      // 왼쪽 방향 키 눌렀을 시,
        cout << "Left key Pressed" << endl;
        
        if(this->draw_flag==1){                 // 만약 draw_flag가 켜져 있다면
            //최소까지 이동
            if(activeLine>0){                   // 맨 왼쪽에서 누르면 맨 오른쪽으로 선택 이동
                this->lines[activeLine--].active=false;
                this->lines[activeLine].active=true;
            }
            else{                               // 그 경우가 아니라면 선택 라인이 왼쪽으로 이동
                this->lines[activeLine].active=false;
                this->lines[this->lines.size()-1].active=true;
                activeLine=this->lines.size()-1;
            }
        }
        
    }
    
    if (key==OF_KEY_RIGHT){                 // 오른쪽 방향 키 눌렀을 시,
        //idx=0;
        cout << "Right key Pressed" << endl;
        if(this->draw_flag==1){             // 만약 draw_flag가 켜져 있다면
            //최대까지 이동
            if(activeLine< this->lines.size()-1){   // 맨 오른쪽에서 누르면 맨 왼쪽으로 선택 이동
                this->lines[activeLine++].active=false;
                this->lines[activeLine].active=true;
            }
            else{                                   // 그 경우가 아니라면 선택 라인이 오른쪽으로 이동
                this->lines[activeLine].active=false;
                this->lines[0].active=true;
                activeLine=0;
            }
        }
        
    }
    
    if (key==OF_KEY_DOWN && this->flow_flag){       // 아래 방향 키 눌렀을 시,
        if(lines[activeLine].slope > -1.5){         // 각도 제한까지 물받침판을 회전 가능
            cout << "Down key Pressed" << endl;
            click_sound.play(); // 클릭킹 사운드 재생
            
                this->lines[activeLine].toY -= 5;   // 끝점 y좌표 5 감소
                this->lines[activeLine].toX += 5*lines[activeLine].slope; // 끝점 x좌표 5*기울기만큼 감소
                
                this->lines[activeLine].startY += 5;    // 시작 점 y좌표 5 증가
                this->lines[activeLine].startX -= 5*lines[activeLine].slope; // 시작 점 x좌표 5*기울기만큼 감소
            
                lines[activeLine].slope = (double)(lines[activeLine].toY-lines[activeLine].startY) / (double)(lines[activeLine].toX-lines[activeLine].startX); // 해당 물받침판의 기울기 갱신
        }
    }
    if (key==OF_KEY_UP && this->flow_flag){     // 위 방향 키를 눌렀을 시,
        if(lines[activeLine].slope < 1.5){      // 각도 제한까지 물받침판을 회전 가능
            cout << "Up key Pressed" << endl;
            click_sound.play(); // 클릭킹 사운드 재생
            
            this->lines[activeLine].toY += 5;   // 끝점 y좌표 5 증가
            this->lines[activeLine].toX -= 5*lines[activeLine].slope; // 끝점 x좌표 5*기울기만큼 감소
            
            this->lines[activeLine].startY -= 5;    // 시작 점 y좌표 5만큼 감소
            this->lines[activeLine].startX += 5*lines[activeLine].slope; // 시작 점 x좌표 5*기울기만큼 증가
                    
            lines[activeLine].slope = (double)(lines[activeLine].toY-lines[activeLine].startY) / (double)(lines[activeLine].toX-lines[activeLine].startX); // 해당 물받침판의 기울기 갱신
            }
        }
    
    if (key=='q'){      // 'q' 키를 누르면, 프로그램을 종료한다.
        cout << "Quit" << endl;
        save();         // progress.txt에 저장
        
        this->lines.clear();    // lines 개수 0개로 바꾸기
        this->points.clear();   // points 개수 0개로 바꾸기
        this->drops.clear();    // drops 개수 0개로 바꾸기
        this->dests.clear();    // dests 개수 0개로 바꾸기
        this->obstacles.clear();    // obstacles 개수 0개로 바꾸기
        
        vector<WaterPoint>().swap(this->points);    // swap method를 사용하여 실질적 메모리 해제를 진행한다.
        vector<WaterLine>().swap(this->lines);      // 빈 벡터와 swap.
        vector<WaterDrop>().swap(this->drops);
        vector<Destination>().swap(this->dests);
        vector<pair<int, int>>().swap(this->obstacles);
        
        OF_EXIT_APP(0);         // 프로그램 종료
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){   // 마우스 놓음 함수

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){  // 마우스 움직임 함수

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){ // 마우스 드래그 함수

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){ // 마우스 누름 함수
    
    if(this->draw_flag){                            // 만약 draw_flag가 켜져 있을 때 (모양이 그려져 있는 상태일 때)
        for(int i=0; i<points.size(); i++){
            float distance=ofDist(x, y, points[i].X, points[i].Y);  // 각 물 시작점들과의 마우스 거리를 측정한다.
            if(distance <10){       // 반지름 이내에 들어온다면
                idx=0;              // 물 index를 0으로 만글고 (시작으로 초기화)
                for(int k=0; k<points.size(); k++) points[k].active=false;
                activeIdx=i;        // 해당 물 시작 점만을 활성화시킨다. (active)
                points[i].active=true;
            }
        }
    }
    
    
    if(this->SUCCESS){      // 만약 성공한 상태라면, 그에 해당하는 버튼에 대한, 마우스 입력 좌표값을 처리한다.
        if(LEVELS[current_level+1] < 1) LEVELS[current_level+1] = 1;
        // 다음 레벨이 아직 활성화되지 않은 상태라면, 점수를 1 부여해 활성화
        if(220<x && x<370 && 500<y && y<575){   // '메뉴' 버튼
            selection_sound.play();             // 선택 소리 재생
            //flow_flag=false;
            idx=0;                              // 물 index 초기화
            draw_flag=false;                    // 그리기 멈춤
            this->activeIdx = 0;                // 물 시작 점 위치 초기화
            this->activeLine = 0;               // 선택된 물받침대 위치 초기화
            if(LEVELS[current_level+1] < 1)     // 다음 레벨이 아직 활성화되지 않은 상태라면, 점수를 1 부여해 활성화
                LEVELS[current_level+1] = 1;
            //this->LEVELS[current_level+1] = 1;
            
            
            this->SUCCESS = false;              // 성공 창 띄웠던 것 다시 지우기
            this->menu_page = true;             // 메뉴 창 띄우기
        }
        if(420<x && x<570 && 500<y && y<575){   // '재시도' 버튼
            selection_sound.play();             // 선택 소리 재생
            OPEN(current_level+1);              // 현재 레벨 다시 오픈
            this->activeLine=0;                 // 선택된 물받침대 위치 초기화
            this->activeIdx=0;                  // 물 시작 점 위치 초기화
            idx=0;                              // 물 index 초기화
            this->draw_flag=1;                  // 그리기 flag 켜기
            this->SUCCESS=false;                // 성공 창 띄웠던 것 다시 지우기
            cout << "Level " << current_level+1 << " selected again" << endl;
            this->menu_page=false;              // 메뉴 창 띄우지 않기
            this->lines[0].active=true;         // 첫번째 물받침대 활성화
        
        }
        if(620<x && x<770 && 500<y && y<575){   // '다음 레벨' 버튼
            
            if(current_level>=9) cout << "No more level to open!" << endl;  // 10레벨이면 다음 레벨 진행 불가
            else {
                selection_sound.play();     // 선택 소리 재생
                current_level++;            // 현재 레벨 1 증가
                OPEN(current_level+1);      // 다음 레벨 오픈
                
                this->activeLine=0;         // 선택된 물받침대 위치 초기화
                this->activeIdx=0;          // 물 시작 점 위치 초기화
                idx=0;                      // 물 index 초기화
                this->draw_flag=1;          // 그리기 flag 켜기
                this->SUCCESS = false;      // 성공 창 띄웠던 것 다시 지우기
                cout << "Level " << current_level+1 << " selected" << endl;
                this->menu_page=false;      // 메뉴 창 띄우지 않기
                this->lines[0].active=true; // 첫번째 물받침대 활성화
            
            }
            //ofResetElapsedTimeCounter(); // 시간 시작!!
        }
    }
    
    
    if(this->FAIL){     // 만약 성공한 상태라면, 그에 해당하는 버튼에 대한, 마우스 입력 좌표값을 처리한다.
        if(220<x && x<370 && 500<y && y<575){   // '메뉴' 버튼
            selection_sound.play();             // 선택 소리 재생
            //flow_flag=false;
            this->FAIL = false;                 // 실패 창 지우기
            idx=0;                              // 물 index 초기화
            draw_flag=false;                    // 그리기 flag 끄기
            this->activeIdx = 0;                // 물 시작 점 위치 초기화
            this->activeLine = 0;               // 선택된 물받침대 위치 초기화
            this->SUCCESS = false;              // 성공 상태 아님 상기.
            this->menu_page = true;             // 메뉴 창 띄우기
        }
        if(420<x && x<570 && 500<y && y<575){   // '다음 레벨' 버튼
            selection_sound.play();             // 선택 사운드 재생
            this->FAIL = false;                 // 실패 창 지우기
            
            OPEN(current_level+1);              // 다음 레벨 열기
            this->activeLine=0;                 // 선택된 물받침대 위치 초기화
            this->activeIdx=0;                  // 물 시작 점 위치 초기화
            idx=0;                              // 물 index 초기화
            this->draw_flag=1;                  // 그리기 flag 켜기
            this->SUCCESS=false;                // 성공 상태 아님 상기.
            cout << "Level " << current_level+1 << " selected again" << endl;
            this->menu_page=false;              // 메뉴 창 띄우지 않기
            this->lines[0].active=true;         // 첫번째 물받침대 활성화
        
        }
    }
    
    
    if(this->menu_page){    // 만약 전체 메뉴 페이지가 띄워져 있을 때
        //--button start--
        for(int i=0; i<10; i++){    // 10개의 레벨 버튼에 대하여
            if(LEVELS[i]>0){
                if(20+100*i<x && x<110+100*i && 150<y && y<275){    // 버튼 범위 안에 오면,
                    selection_sound.play(); // 선택 소리 재생
                    OPEN(i+1);              // 그에 해당하는 레벨 파일 오픈
                    current_level=i;        // 현재 레벨 번호 설정
                    this->draw_flag=1;      // 그리기 시작
                    cout << "Level " << i+1 << " selected" << endl;
                    this->menu_page=false;  // 메뉴 페이지 띄웠던 것 지우기
                    this->lines[0].active=true; // 첫번째 물받침대 활성화
                    //ofResetElapsedTimeCounter(); // 시간 시작!!
                }
            }
            
        }
        //---button finnish--
    }
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){    // 마우스 놓음 함수

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){ // 마우스 입력 함수

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){  // 마우스 벗어남 함수

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){    // 창 크기 조정 함수

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){  // 메시지 받기 함수

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ // 드래그 사건 함수

}

void ofApp::OPEN(int level){    // 파일 입력 함수: 주어진 레벨에 맞는 파일을 입력받는다.
    this->lines.clear();        // lines 개수 0으로 초기화
    this->points.clear();       // points 개수 0으로 초기화
    this->dests.clear();        // dests 개수 0으로 초기화
    this->obstacles.clear();    // obstacles 개수 0으로 초기화
    
    string line;                // 입력받을 한 줄 단위에 대한 string
    int lines, startX, startY, toX, toY, pointX, pointY;    // 줄 개수, 시작 x,y좌표, 끝 x,y좌표
    int des;    // 목적지 좌표
    float slope;    // 기울기
    
    //파일 열기
    string file_name="level";   // 입력받을 이름은
    string level_num;
    level_num=to_string(level);
    string filetype=".txt";
    
    file_name = file_name + level_num + filetype;   // level1.txt와 같이 level + (레벨 번호) + .txt이다.
    
    
    ifstream input_file(ofToDataPath(file_name), ifstream::in);
    // 입력 스트림 설정
    
    if(input_file.is_open()){   // 파일이 열린다면,
        //선분
        input_file >> lines;    // 입력받을 선분 개수 입력
        for(int i=0; i<lines; i++){ // 선분 개수만큼 입력
            input_file >> startX;   // 시작 x좌표
            input_file >> startY;   // 시작 y좌표
            input_file >> toX;      // 끝 x좌표
            input_file >> toY;      // 끝 y좌표
            
            cout << "[read line] stX: " << startX << ", stY: " << startY << ", toX: " << toX << ", toY: " << toY <<endl;
            
            WaterLine line;         // WaterLine 임시 구조체 선언.
            line.startX=startX;     // 좌표값들을 집어넣고,
            line.startY=startY;
            line.toX=toX;
            line.toY=toY;
            
            //기울기 계산하고 넣기
            slope = (double)(line.toY-line.startY)/(double)(line.toX-line.startX);  // 기울기도 계산한다.
            line.slope = slope;
            
            this->lines.push_back(line);    // 해당 입력값을 push_back하여 lines vector에 집어넣는다.
        }
        
        //점
        input_file >> lines;    // 입력받을 점 개수 입력
        for(int i=0; i<lines; i++){ // 점 개수만큼 입력
            input_file >> pointX;   // x좌표
            input_file >> pointY;   // y좌표
            cout << "[read point] x: " <<pointX << ", y: " << pointY << endl;
            
            WaterPoint point_tmp;   // WaterPoint 임시 구조체 선언.
            point_tmp.X=pointX;     // 좌표값들을 집어넣고,
            point_tmp.Y=pointY;
            //
            if(i==0) point_tmp.active=true; // 첫번째 점만 active시켜 놓는다. (초기 설정)
            else point_tmp.active=false;
            
            //
            this->points.push_back(point_tmp);  // 해당 입력값을 push_back하여 points vector에 집어넣는다.
        }
        
        //목표
        input_file >> lines;    // 입력받을 목적지 개수 입력
        for(int i=0; i<lines; i++){ // 목적지 개수만큼 입력
            input_file >> des;      // 목적지 좌표 입력
            cout << "[read Destination] des: " << des << endl;
            
            Destination dest_tmp;   // Destination 임시 구조체 선언.
            dest_tmp.x1 = des;      // 좌표값을 집어넣고,
            this->dests.push_back(dest_tmp);    // 해당 입력값을 push_back하여 dests vector에 집어넣는다.
        }
        
        //장애물
        int obX, obY;       // 장애물 좌표
        input_file >> lines;    // 입력받을 장애물 개수 입력
        for(int i=0; i<lines; i++){ // 장애물 개수만큼 입력
            input_file >> obX;      // 좌표값들을 입력받고,
            input_file >> obY;
            obstacles.push_back(make_pair(obX, obY));   // make_pair을 통해 pair을 만들고 이를 obstacles vector에
                                                        // push_back해서 추가한다.
        }
        
    }
    else cout << "Failed to open file" << endl;         // 파일 열기 실패 시 에러 메시지
    
}


void ofApp::UserLine(WaterLine line) {      // 물받침대 그리기 함수
    if(line.active){                        // 활성화된 물받침대라면,
        //ofSetColor(0, 255, 0);
        
        ofSetColor(ofColor::turquoise);     // 색깔 다르게 설정.
        ofDrawLine(line.startX, line.startY, line.toX, line.toY);   // 선 그리기
    }
    else{
        ofSetColor(ofColor::paleVioletRed); // 아니면 붉은 계열 색깔
        ofDrawLine(line.startX, line.startY, line.toX, line.toY);   // 선 그리기
    }
}

void ofApp::UserCircle(WaterPoint point) {  // 물 시작 점 그리기 함수
    if(point.active){                       // 활성화된 상태라면,
//        ofSetColor(255, 0, 0);
        ofSetColor(0, 0, 255);              // 파란색으로 그리기.
        ofDrawCircle(point.X, point.Y, 10); // 반지름 10인 원 모양
    }
    else{
        ofSetColor(0, 0, 0);                // 아니면 검은색
        ofDrawCircle(point.X, point.Y, 10); // 반지름 10인 원 모양
    }
    
}

void ofApp::UserDest(Destination dest){     // 목적지 그리기 함수
    ofFill();                               // 채운 사각형.
    if(dest.reached==false){                // 아직 도달하지 못한 목적지라면,
        //ofSetColor(0, 255, 0);
        
        ofSetColor(ofColor::mediumOrchid);  // 보라색으로 그리기.
        ofDrawRectangle(dest.x1-10, 768, 20, -10);  // 목적지 모양은 직사각형 (가로 20)
    }
    else {
        ofSetColor(0, 0, 255);              // 도달한 적이 있다면, 파란색으로 그리기.
        ofDrawRectangle(dest.x1-10, 768, 20, -10);  // 목적지 모양은 직사각형 (가로 20)
    }
}



void ofApp::drawDrop(WaterDrop water){      // 물방울 그리기 함수
    ofSetColor(0, 0, 255);                  // 파란색으로 그리기
    
    ofDrawCircle(water.xpos, water.ypos, 2);    // 반지름 2인 원 모양
    
    if(water.ypos>758 && water.ypos<770) {                    // 만약 밑바닥에 도착했다면,
        for(int i=0; i<dests.size(); i++){  // 각각의 목적지에 닿는지 확인
            if(dests[i].x1-10 < water.xpos && water.xpos < dests[i].x1+10){ // 목적지는 가로가 20
                dests[i].reached=true;  // 닿는다면, 해당 목적지에 도달했음을 저장.
            }
        }
    }
    
    for(int i=0; i< obstacles.size(); i++){     // 장애물에 물이 닿는지 점검.
        float dis = ofDist(water.xpos, water.ypos, obstacles[i].first, obstacles[i].second);
        if(dis <= 20) {         // 만약 물방울과 장애물 사이의 거리가 20(장애물 반지름) 이하라면,
            this->FAIL = true;  // 실패.
        }
    }
    
}


void ofApp::drawObstacles(pair<int, int> p1){   // 장애물 그리기 함수
    ofSetColor(ofColor::red);                   // 색깔은 빨간색
    ofDrawCircle(p1.first, p1.second, 20);      // 주어진 좌표에 반지름 10인 원으로 그리기
}


//---
void ofApp::drawStar(int x, int y, int point, float size){  // 별 모양 그리기 함수
    // STAR
    int dis;
    ofFill();                   // 채우기
    ofSetHexColor(0xe0be21);    // 색깔: 노란색 계통
    int starX=x, starY=y;       // 별 모양 x,y좌표
    
    if(size < 0.2){             // 작은 원이라면, 별 사이 거리 작게 설정
        dis=200;
        ofSetLineWidth(0.3);    // 선도 얇게 설정
    } else{
        dis = 285;              // 아닌 경우, 그대로.
        ofSetLineWidth(5);
    }
    
    
    for(int i=0; i<3; i++){     // 세 개의 별 모양에 대해,
        int k;                  // 10씩 감소해나가며 점수에 해당하는 별점 그리기. (PolyMode를 다르게 설정)
        if(point>=10)
            ofSetPolyMode(OF_POLY_WINDING_NONZERO);    // 10점이라면, 별 하나 꽉 채우기
        if(2<point && point <10)
            ofSetPolyMode(OF_POLY_WINDING_ODD);         // 5점이라면, 별 하나 반 채우기
        if(point <= 2){
            ofSetPolyMode(OF_POLY_WINDING_NONZERO);     // 0점이라면, 채우지 않기
            ofSetColor(ofColor::gray);                  // 색깔은 회색으로.
            ofNoFill();                                 // 별 모양 내부를 채우지 않는다.
        }
            
        
        ofBeginShape();     // 모양 시작
            ofVertex(starX,starY);          // 별 크기, 별 사이 거리 가중치만큼의 크기를 가진 별 그리기
            ofVertex(starX-185*size,starY); // ofVertex로 꼭짓점들 체크
            ofVertex(starX-35*size,starY-110*size);
            ofVertex(starX-95*size,starY+65*size);
            ofVertex(starX-150*size,starY-110*size);
        ofEndShape();       // 모양 끝
        
        point -= 10;    // 10점씩 감소
        starX += dis*size;  // 별들 사이 거리 가중치 계산
    }
    
    ofSetLineWidth(5);
}

//---

void ofApp::moveCal(WaterPoint activePoint, vector<WaterLine> lines){   // 물방울 움직임 위치 계산 함수
    //float epsilonSlope;
    this->drops.clear();    // drops 개수 0개로 초기화
    
    float xPos, yPos;       // 활성화된 물 시작 위치 x,y좌표
    xPos=activePoint.X;     // 물 시작 위치 좌표 대입
    yPos=activePoint.Y;
    
    WaterDrop tmpWater;     // WaterDrop 임시 구조체 선언.
        
    for(int i=0; i<10000; i++){ // 창 높이에 맞는 만큼의 물방울 개수 계산
        yPos++;                 // y좌표를 1씩 증가시키며
        for(int j=0; j<lines.size(); j++){  // 각 물 받침대에 닿는지 확인.
            
            if((int)yPos == (int)(lines[j].startY+ (xPos-lines[j].startX)*lines[j].slope)){
                // 만약 물방울의 y좌표가 물받침대 시작 y좌표 + 기울기 * dx라면
                if(lines[j].startX<=xPos && xPos<=lines[j].toX){
                    // x좌표가 물받침대의 시작, 끝 안에 있을 때까지는
                    xPos += pow((lines[j].slope), 2)*(1/lines[j].slope);
                    // 가중치(기울기의 제곱) * (1/기울기)만큼 x 좌표 증가
                    yPos--; // y 좌표 감소
                    yPos += pow((lines[j].slope), 2)*(lines[j].slope-1);
                    // y좌표는 가중치(기울기의 제곱) * (기울기-1)만큼 증가
                }
            }
            
        }
        tmpWater.xpos=xPos; // 계산된 x, y좌표
        tmpWater.ypos=yPos;
            
        this->drops.push_back(tmpWater);    // drops vector에 push_back하여 추가
    }
}

void ofApp::save(){     // 세이브 함수
    ofFile saveFile;
    saveFile.open("progress.txt", ofFile::WriteOnly);   // progress.txt에 1-10레벨까지의 점수 저장.
    
    for(int i=0; i<10; i++)     // 한 줄에 한 레벨의 점수, 총 10줄 입력
        saveFile << LEVELS[i] << endl;
}
