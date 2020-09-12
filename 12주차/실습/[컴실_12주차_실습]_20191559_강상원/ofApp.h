#pragma once

#include "ofMain.h"

struct MazeLines{
    float startX;
    float startY;
    float endX;
    float endY;
};

class ofApp : public ofBaseApp{

	public:
        int draw_flag;
        int dfs_flag;
        int bfs_flag;
        vector<MazeLines> LINES;
    
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
        void freeMemory();
       
		vector<vector<char>> maze;
};
