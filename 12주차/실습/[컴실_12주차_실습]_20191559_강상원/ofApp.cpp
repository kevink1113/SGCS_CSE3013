#include "ofApp.h"
#include <string>
#include <vector>

using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
    //ofSetFrameRate(15);
    ofSetBackgroundColor(255, 255, 255);
    this->draw_flag=0;
    ofSetLineWidth(5);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    if(this->draw_flag==1){
        ofSetColor(255, 0, 0);
        
        for(int i=0; i<this->LINES.size(); i++)
            MazeLine(this->LINES[i]);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //cout << key << endl;
    if (key=='r'){
        cout << "r key Pressed" << endl;
        readFile();
        this->draw_flag=1;
    }
    
    if (key=='b'){
        this->bfs_flag=1;
        cout << "BFS key Pressed" << endl;
    }
    
    
    if (key=='d'){
        this->dfs_flag=1;
        cout << "DFS key Pressed" << endl;
        
    }
    
    if (key=='q'){
        cout << "Quit" << endl;
        freeMemory();
        
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

bool ofApp::readFile() {
    freeMemory();
    ofFileDialogResult openFileResult=ofSystemLoadDialog("Select .maz file");
    
    if (openFileResult.bSuccess){
        int ith=0;
        string line;
        int realWIDTH, realHEIGHT=0;
        int WIDTH, HEIGHT;
        
        
        //파일 열기
        string file_location=openFileResult.getPath();
        ifstream input_file, input_file2;
        input_file.open(file_location);
        input_file2.open(file_location);
        
        if(input_file.is_open()){
            
            while (getline(input_file, line)) {
                realWIDTH = line.length();
                WIDTH = (line.length()-1)/2;
                realHEIGHT++;
            }
            HEIGHT = (realHEIGHT-1)/2;
            cout << "WIDTH : " << WIDTH << "  HEIGHT : " << HEIGHT << endl;

            maze.resize(realHEIGHT, vector<char>(realWIDTH));
            
            ////char maze[realHEIGHT][realWIDTH];
            string tmp;
            
            for(int i=0; i<realHEIGHT; i++){
                getline(input_file2, tmp);
                for(int j=0; j<realWIDTH; j++)
                    maze[i][j]=tmp[j];
            }
            
            MazeLines line_tmp;
       
            for(int i=0; i<realHEIGHT; i++){
                for(int j=0; j<realWIDTH; j++){
                    if(maze[i][j] == '-') {
                        line_tmp.startX=j-0.5; line_tmp.startY=i+0.5;
                        line_tmp.endX=j+1.5; line_tmp.endY=i+0.5;
                        this->LINES.push_back(line_tmp);
                    }
                    
                    else if (maze[i][j] == '|') {
                        line_tmp.startX=j+0.5; line_tmp.startY=i-0.5;
                        line_tmp.endX=j+0.5; line_tmp.endY=i+1.5;
                        this->LINES.push_back(line_tmp);
                        
                    }
                }
            }
            return true;
        }
        else{
            cout << "Failed to open file" << endl;
            return false;
        }
        
    }
}

void ofApp::MazeLine(MazeLines lines) {
    ofSetColor(100);
    ofSetLineWidth(5);
    ofDrawLine(20*lines.startX, 20*lines.startY, 20*lines.endX, 20*lines.endY);
}

void ofApp::freeMemory(){
    this->maze.clear();
    vector<vector<char>>().swap(this->maze);
    
    this->LINES.clear();
    vector<MazeLines>().swap(this->LINES);
}
