#pragma once

#include "ofMain.h"
#include <string>
#include <vector>
#include <stack>

struct MazeLines{
    float startX;
    float startY;
    float endX;
    float endY;
};

struct block{
    bool isBlock;
    bool up;
    bool down;
    bool left;
    bool right;
};

struct BLOCK {
    int x;
    int y;
};

class ofApp : public ofBaseApp{

    public:
        int mag;
        
        int draw_flag;
        int dfs_flag;
        int bfs_flag;
    
        int draw_fin;
    
        vector<MazeLines> LINES;
    
        stack<BLOCK> s_blocks;
        stack<BLOCK> s_path;
        stack<BLOCK> all_path;
    
        int x_move[4]={0, 0, 1, -1};
        int y_move[4]={1, -1, 0, 0};
    
        int realHEIGHT=0, realWIDTH=0;
        int WIDTH, HEIGHT;
    
        void setup();
        void update();
        void draw();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
        void processOpenFileSelection(ofFileDialogResult openFileResult);
        bool readFile();
        void MazeLine(MazeLines lines);
        bool DFS();
    
        void freeMemory();
       
        vector<vector<char>> maze;
        vector<vector<bool>> visited;
    
        vector<vector<block>> mazeBlock;
    
};
