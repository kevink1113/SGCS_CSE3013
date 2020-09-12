#pragma once

#include "ofMain.h"
#include <vector>


struct WaterLine{
    int startX;
    int startY;
    int toX;
    int toY;
};

struct WaterPoint{
    bool active;
    int X;
    int Y;
};

class ofApp : public ofBaseApp{
    private:
        int draw_flag;
        int activeIdx=0;
        vector<WaterLine> lines;
        vector<WaterPoint> points;
        
	public:
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
		
    
    //int xPos;
    // WaterFall-related member variables
        //int draw_flag;
        //int load_flag;
    
    // WaterFall-related member functions
        void processOpenFileSelection(ofFileDialogResult openFileResult);
        void UserLine(WaterLine line);
        void UserCircle(WaterPoint point);
        
};
