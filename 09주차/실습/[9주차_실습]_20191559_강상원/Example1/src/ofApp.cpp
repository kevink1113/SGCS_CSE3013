#include "ofApp.h"
#include <stdio.h>
#include <fstream>

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLineWidth(5);
    ofSetBackgroundColor(255, 255, 255);
    this->draw_flag=0;
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(this->draw_flag==1){
        ofSetColor(143, 0, 25);
        for(int i=0; i<this->lines.size(); i++)
            UserLine(this->lines[i]);
        
        ofSetColor(0, 0, 0);
        for(int i=0; i<this->points.size(); i++)
            UserCircle(this->points[i]);
        
        //요러코롬 하면 됨!!!
        //this->points[3].active=true;
        //
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //cout << key << endl;
    if (key=='l'){
        cout << "L key Pressed" << endl;
        ofFileDialogResult fileResult=ofSystemLoadDialog("Load input file");
        processOpenFileSelection(fileResult);
    }
    
    if (key=='d'){
        this->draw_flag=1;
        cout << "Draw key Pressed" << endl;
    }
    
    if (key==OF_KEY_LEFT){
        cout << "Left key Pressed" << endl;
        //
        //this->draw_flag++;
        if(this->draw_flag==1){
            //최소까지 이동
            if(activeIdx>0){
                this->points[activeIdx--].active=false;
                this->points[activeIdx].active=true;
            }
            else{
                this->points[activeIdx].active=false;
                this->points[this->points.size()-1].active=true;
                activeIdx=this->points.size()-1;
            }
        }
        
    }
    
    if (key==OF_KEY_RIGHT){
        cout << "Right key Pressed" << endl;
        //cout << "Num of point: " << this->points.size() << endl;
        //
        //this->draw_flag++;
        if(this->draw_flag==1){
            //최대까지 이동
            if(activeIdx< this->points.size()-1){
                this->points[activeIdx++].active=false;
                this->points[activeIdx].active=true;
            }
            else{
                this->points[activeIdx].active=false;
                this->points[0].active=true;
                activeIdx=0;
            }
        }
    }
    
    if (key=='q'){
        cout << "Quit" << endl;
        
        //free(this);
        //delete this;
        this->lines.clear();
        this->points.clear();
        
        vector<WaterPoint>().swap(this->points);
        vector<WaterLine>().swap(this->lines);
        
        OF_EXIT_APP(0);
    }
    
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) { 
    this->lines.clear();
    this->points.clear();
    
    if (openFileResult.bSuccess){
        string line;
        int lines, startX, startY, toX, toY, pointX, pointY;
        
        //파일 열기
        string file_location=openFileResult.getPath();
        ifstream input_file;
        input_file.open(file_location);
        
        if(input_file.is_open()){
            //선분
            input_file >> lines;
            for(int i=0; i<lines; i++){
                input_file >> startX;
                input_file >> startY;
                input_file >> toX;
                input_file >> toY;
                
                cout << "[read line] stX: " << startX << ", stY: " << startY << ", toX: " << toX << ", toY: " << toY <<endl;
                
                WaterLine line;
                line.startX=startX;
                line.startY=startY;
                line.toX=toX;
                line.toY=toY;
                this->lines.push_back(line);
            }
            
            //점
            input_file >> lines;
            for(int i=0; i<lines; i++){
                input_file >> pointX;
                input_file >> pointY;
                cout << "[read point] x: " <<pointX << ", y: " << pointY << endl;
                
                WaterPoint point_tmp;
                point_tmp.X=pointX;
                point_tmp.Y=pointY;
                //
                if(i==0) point_tmp.active=true;
                else point_tmp.active=false;
                
                //
                this->points.push_back(point_tmp);
            }
        }
        else cout << "Failed to open file" << endl;
    }
}

void ofApp::UserLine(WaterLine line) {
    ofDrawLine(line.startX, line.startY, line.toX, line.toY);
}

void ofApp::UserCircle(WaterPoint point) {
    if(point.active){
        ofSetColor(255, 0, 0);
        ofDrawCircle(point.X, point.Y, 10);
    }
    else{
        ofSetColor(0, 0, 0);
        ofDrawCircle(point.X, point.Y, 10);
    }
    
}
