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
        
        if(this->bfs_flag==1){
            
            for(int i=0; i<this->PATHS.size(); i++)
                PathLine(this->PATHS[i]);
            
            ofSetColor(0, 255, 0);
            
            MazeLines path_tmp;

            int kp=0;
            for(int i=1; i<realHEIGHT-1; i++){
                for(int j=1; j<realWIDTH-1; j++){
                    if(path_block[i][j] == 1)
                    {
                        if(wall[i-1][j] || wall[i+1][j]){   // 가로 줄
                            // 가로 줄을 하는 건 좋은데, 왼쪽도 확인하자
                            if(wall[i][j-1]&& !wall[i][j+1]){
                                path_tmp.startX=j+0.5; path_tmp.startY=i+0.5;
                                path_tmp.endX=j+0.5; path_tmp.endY=i+0.5;
                                this->PATHS.push_back(path_tmp);
                            } else if (wall[i][j+1] && wall[i][j-1]){
                                kp++;
                            } else if(wall[i][j+1]&& !wall[i][j-1]){
                                kp++;
                            }
                            else{
                                path_tmp.startX=j-0.5; path_tmp.startY=i+0.5;
                                path_tmp.endX=j+1.5; path_tmp.endY=i+0.5;
                                this->PATHS.push_back(path_tmp);
                            }
                        }
                        else if (wall[i][j-1] || wall[i][j+1]){ // 세로 줄
                            path_tmp.startX=j+0.5; path_tmp.startY=i-0.5;
                            path_tmp.endX=j+0.5; path_tmp.endY=i+1.5;
                            this->PATHS.push_back(path_tmp);
                        }
                    }
                }
            }
            int iterate2=s_blocks_2.size();

            for(int i=0; i<iterate2; i++){
                path.addVertex(20*s_blocks_2.top().x+10, 20*s_blocks_2.top().y+10);
                s_blocks_2.pop();
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
        if(this->dfs_flag){
            this->dfs_flag=0;
            
            path.clear();
            allpath.clear();
            while(!s_blocks.empty())
                s_blocks.pop();
            while(!s_blocks_2.empty())
                s_blocks_2.pop();

        }
        if(this->draw_flag==1){
            this->bfs_flag=1;
            BFS();
            cout << "BFS key Pressed" << endl;
            
            FIND();
        }
    }
    
    
    if (key=='d'){
        if(this->bfs_flag) {
            this->bfs_flag=0;
            path.clear();
            allpath.clear();
            while(!s_blocks_2.empty())
                s_blocks_2.pop();
        }
        //this->draw_flag=1;
        if(this->draw_flag==1){
            this->dfs_flag=1;
            
            cout << "DFS key Pressed" << endl;
            
            //int iterate=s_blocks.size();
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

void ofApp::PathLine(MazeLines lines) {
    ofSetColor(255, 0, 0);
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
            cout << "search complete!" << endl << "Press D one more time to show diagonal line" <<endl;
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

bool ofApp::BFS(){
    this->draw_flag=1;
    this->bfs_flag=1;
    visited.resize(realHEIGHT, vector<bool>(realWIDTH));
    wall.resize(realHEIGHT, vector<bool>(realWIDTH));

    mazeBlock.resize(realHEIGHT, vector<block>(realWIDTH));
    path_block.resize(realHEIGHT, vector<bool>(realWIDTH));
    
    for(int i=0; i<realHEIGHT;  i++){
        for(int j=0; j<realWIDTH; j++){
            if(maze[i][j] != ' ') {
                visited[i][j]=true;
                wall[i][j]=true;
            } else {
                visited[i][j]=false;
                wall[i][j]=false;
            }
        }
    }
    BLOCK v;
    BLOCK u;
    v.x=1; v.y=1;
    visited[1][1]=true;
    path_block[1][1]=true;
    
    bool move_possible;
    
    q_blocks.push(v);
    all_path_2.push(v);
    
    while(!q_blocks.empty()){
        path_block[q_blocks.front().y][q_blocks.front().x]=true;
        
        //cout << "top : " << q_blocks.front().y << ' ' << q_blocks.front().x << endl;
        if(q_blocks.front().x == realWIDTH-2 && q_blocks.front().y == realHEIGHT-2) {
            cout << "search complete!" << endl << "Press B one more time to show diagonal line" <<endl;
            return true;
        }
        move_possible=false;
        for(int i=0; i<4; i++){
            u.y = q_blocks.front().y + y_move[i];
            u.x = q_blocks.front().x + x_move[i];
            u.count = q_blocks.front().count + 1;
            
            if(!visited[u.y][u.x]){
                cout << u.y << ' ' << u.x << '\t' << u.count << endl;
                visited[ q_blocks.front().y + y_move[i] ][ q_blocks.front().x + x_move[i]] = true;
                q_blocks.push(u);
                //
                all_path_2.push(u);
                //
                path_block[ u.y ][ u.x ] = true;
                
                move_possible=true;
            }
        }
        if(!move_possible)
            q_blocks.pop();
    }
    
}

void ofApp::FIND(){
    this->draw_flag=1;
    visited.resize(realHEIGHT, vector<bool>(realWIDTH));
    mazeBlock.resize(realHEIGHT, vector<block>(realWIDTH));
    for(int i=0; i<realHEIGHT;  i++){
        for(int j=0; j<realWIDTH; j++){
            if(maze[i][j] != ' ') visited[i][j]=true;
            else visited[i][j]=false;
        }
    }
    BLOCK v, u;
    v.x=1; v.y=1;
    visited[1][1]=true;
    bool move_possible;
    s_blocks_2.push(v);
    while(!s_blocks_2.empty()){
        if(s_blocks_2.top().x == realWIDTH-2 && s_blocks_2.top().y == realHEIGHT-2) return true;
        move_possible=false;
        for(int i=0; i<4; i++){
            u.y = s_blocks_2.top().y + y_move[i]; u.x = s_blocks_2.top().x + x_move[i];
            if(!visited[u.y][u.x]){
                visited[ s_blocks_2.top().y + y_move[i] ][ s_blocks_2.top().x + x_move[i]] = true;
                s_blocks_2.push(u);
                move_possible=true;
                break;
            }
        }
        if(!move_possible) s_blocks_2.pop();
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
    
    this->wall.clear();
    vector<vector<bool>>().swap(this->wall);
    
    this->mazeBlock.clear();
    vector<vector<block>>().swap(this->mazeBlock);
    
    this->path_block.clear();
    vector<vector<bool>>().swap(this->path_block);
    
    this->PATHS.clear();
    vector<MazeLines>().swap(this->PATHS);
    
    while(!s_blocks.empty())
        s_blocks.pop();
    
    while(!s_blocks_2.empty())
        s_blocks_2.pop();
    
    while(!s_path.empty())
        s_path.pop();
    while(!all_path.empty())
        all_path.pop();
    
    while(!q_blocks.empty())
        q_blocks.pop();
    while(!q_path.empty())
        q_path.pop();
    while(!all_path_2.empty())
        all_path_2.pop();
    while(!all_path_3.empty())
        all_path_3.pop();
    
    this->draw_flag=0;
    this->dfs_flag=0;
    
    path.clear();
    allpath.clear();
    this->dfs_flag=0;
    this->bfs_flag=0;
}
