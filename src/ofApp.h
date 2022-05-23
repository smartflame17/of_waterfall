#pragma once

#include "ofMain.h"
#include <vector>
#define EPSILON 0.01
struct WaterLine {
	int startX;
	int startY;
	int endX;
	int endY;
	double slope;
};

struct WaterDot {
	bool isActive;
	int X;
	int Y;
};

struct Waterfall{
	int X;
	int Y;
};

class ofApp : public ofBaseApp{
	private:
		vector<WaterLine> lines;			//saves all lines
		vector<WaterDot> dots;				//saves all dots
		vector<Waterfall> water;			//saves all water to be drawn
		int isActiveIndex = 0;				//index of currently active dot (default is 0)
		int WaterfallIndex = 0;				//index of waterfall vector
	public:
		void setup();
		void update();
		void draw();
		void calculatePath();

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
    
    /* WaterFall-related member variables Regions */
    
    // flag variables
    int draw_flag;
    int load_flag;
	int waterfall_draw_flag;

    
    // Line segment and dot related variables
    int num_of_line, num_of_dot;
    float dot_diameter;
    
    /* WaterFall-related member functions */
    
    void processOpenFileSelection(ofFileDialogResult openFileResult);
    void initializeWaterLines(); // 2nd week portion.
		
};
