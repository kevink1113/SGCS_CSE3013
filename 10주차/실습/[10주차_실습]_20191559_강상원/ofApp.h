#pragma once

#include "ofMain.h"
#include <vector>


struct WaterLine{
    int startX;
    int startY;
    int toX;
    int toY;
    
    float slope;
};

struct WaterPoint{
    bool active;
    int X;
    int Y;
};

struct WaterDrop{
    float xpos;
    float ypos;
    
    //double distance;
};

class ofApp : public ofBaseApp{
    private:
        int draw_flag;
        int flow_flag;
        int activeIdx=0;
        vector<WaterLine> lines;
        vector<WaterPoint> points;
        vector<WaterDrop> drops;
        
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
        void drawDrop(WaterDrop water);
    
        //void moveCal(this->points[this->activeIdx], this->lines);
        void moveCal(WaterPoint activePoint, vector<WaterLine> lines);
        //float distance(float x1, float x2, float y1, float y2);
        
};
