#include "ofApp.h"

using namespace std;



ofPolyline path;
ofPolyline allpath;

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
        //ofSetColor(255, 0, 0);
        
        if(this->dfs_flag==1){
            ofSetColor(255, 0, 0);
            int iterateall=all_path.size();

            for(int i=0; i<iterateall; i++){
                allpath.addVertex(20*all_path.top().x+10, 20*all_path.top().y+10);
                all_path.pop();
            }
            allpath.draw();
            
            
            ofSetColor(0, 255, 0);
            int iterate=s_blocks.size();

            for(int i=0; i<iterate; i++){
                path.addVertex(20*s_blocks.top().x+10, 20*s_blocks.top().y+10);
                s_blocks.pop();
            }
            path.draw();
        }
        
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
        //this->draw_flag=1;
        if(this->draw_flag==1){
            this->dfs_flag=1;
            
            cout << "DFS key Pressed" << endl;
            
            int iterate=s_blocks.size();
            if(DFS()) path.draw();
        }
        
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

//--------------------------------------------------------------
bool ofApp::readFile() {
    freeMemory();
    realHEIGHT=0;
    realWIDTH=0;
    ofFileDialogResult openFileResult=ofSystemLoadDialog("Select .maz file");
    
    if (openFileResult.bSuccess){
        int ith=0;
        string line;
        //int realWIDTH, realHEIGHT=0;
        //int WIDTH, HEIGHT;
        
        
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
            this->draw_flag=0;
            return false;
        }
        
    }
}

//--------------------------------------------------------------
void ofApp::MazeLine(MazeLines lines) {
    ofSetColor(100);
    ofSetLineWidth(5);
    ofDrawLine(20*lines.startX, 20*lines.startY, 20*lines.endX, 20*lines.endY);
}

//--------------------------------------------------------------
bool ofApp::DFS(){
    
    this->draw_flag=1;
    this->dfs_flag=1;
    visited.resize(realHEIGHT, vector<bool>(realWIDTH));
    mazeBlock.resize(realHEIGHT, vector<block>(realWIDTH));
    
    for(int i=0; i<realHEIGHT;  i++){
        for(int j=0; j<realWIDTH; j++){
            if(maze[i][j] != ' ') visited[i][j]=true;
            else visited[i][j]=false;
        }
    }
    BLOCK v;
    BLOCK u;
    //BLOCK target;
    v.x=1; v.y=1;
    visited[1][1]=true;
    
    int k;
    //target.x=realWIDTH-1; target.y=realHEIGHT-1;
    
    bool move_possible;
    
    s_blocks.push(v);
    all_path.push(v);
    
    while(!s_blocks.empty()){
        all_path.push(s_blocks.top());
        
        cout << "top : " << s_blocks.top().y << ' ' << s_blocks.top().x << endl;
        
        
        if(s_blocks.top().x == realWIDTH-2 && s_blocks.top().y == realHEIGHT-2) {
            cout << "search complete!" << endl << "Press D one more timw to show diagonal line" <<endl;
            return true;
        }
        move_possible=false;
        for(int i=0; i<4; i++){
            u.y = s_blocks.top().y + y_move[i];
            u.x = s_blocks.top().x + x_move[i];
            
            //all_path.push(u);
            
            //cout << "searching..." << ' ' << s_blocks.top().y+y_move[i] << ' ' << s_blocks.top().x+x_move[i] << endl;
            if(!visited[u.y][u.x]){
                //u.y = s_blocks.top().y + y_move[i];
                //u.x = s_blocks.top().x + x_move[i];
                all_path.push(u);
                
                //cout << u.y << ' ' << u.x << endl;
                
                visited[ s_blocks.top().y + y_move[i] ][ s_blocks.top().x + x_move[i]] = true;
                s_blocks.push(u);
                
                move_possible=true;
                break;
            }
        }
        if(!move_possible){
            //cout << "move possible not detected" << endl;
            s_blocks.pop();
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::freeMemory(){
    this->maze.clear();
    vector<vector<char>>().swap(this->maze);
    
    this->LINES.clear();
    vector<MazeLines>().swap(this->LINES);
    
    this->visited.clear();
    vector<vector<bool>>().swap(this->visited);
    
    this->mazeBlock.clear();
    vector<vector<block>>().swap(this->mazeBlock);
    
    while(!s_blocks.empty())
        s_blocks.pop();
    while(!s_path.empty())
        s_path.pop();
    while(!all_path.empty())
        all_path.pop();
    
    this->draw_flag=0;
    this->dfs_flag=0;
    
    path.clear();
    allpath.clear();
}
