#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(15); // Limit the speed of our program to 15 frames per second
    
    // We still want to draw on a black background, so we need to draw
    // the background before we do anything with the brush
    ofBackground(255,255,255);
    ofSetLineWidth(4);
    
    draw_flag = 0;
    load_flag = 0;
    dot_diameter = 20.0f;
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(127,23,31);  // Set the drawing color to brown
    
    // Draw shapes for ceiling and floor
    ofDrawRectangle(0, 0, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofDrawRectangle(0, 728, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofSetLineWidth(5);
    
    
    ofSetLineWidth(5);
    if( draw_flag ){
		int i;
		/* COMSIL1-TODO 3 : Draw the line segment and dot in which water starts to flow in the screen.
		 Note that after drawing line segment and dot, you have to make selected water start dot colored in red.
		 */
		for (i = 0; i < this->lines.size(); i++) {
			ofDrawLine(this->lines[i].startX, this->lines[i].startY, this->lines[i].endX, this->lines[i].endY); //draw lines
		}

		for (i = 0; i < this->dots.size(); i++) {
			if (this->dots[i].isActive) {
				ofSetColor(255, 0, 0);  // If dot is selected, set color to red
			}
			else ofSetColor(0, 0, 0);   // Set the drawing color to black
			ofDrawCircle(this->dots[i].X, this->dots[i].Y, dot_diameter / 2);
		}
        // 2nd week portion.
        ofSetLineWidth(2);
		if (waterfall_draw_flag) {
			ofSetColor(0, 0, 255);	//color to blue to draw waterfall
			for (i = 0; i < this->water.size() - 1; i++) {
				ofDrawLine(this->water[i].X, this->water[i].Y, this->water[i+1].X, this->water[i+1].Y);
			}
		}
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'v') {
        // HACK: only needed on windows, when using ofSetAutoBackground(false)
        glReadBuffer(GL_FRONT);
        ofSaveScreen("savedScreenshot_"+ofGetTimestampString()+".png");
    }
    if (key == 'q'){
        // Reset flags
        draw_flag = 0;
        
        // Free the dynamically allocated memory exits.
		this->lines.clear();
		vector<WaterLine>().swap(lines);
		this->dots.clear();
		vector<WaterDot>().swap(dots);

        cout << "Dynamically allocated memory has been freed." << endl;
        
        _Exit(0);
    }
    if (key == 'd'){
        if( !load_flag) return;

		draw_flag = 1;
        /* COMSIL1-TODO 2: This is draw control part.
        You should draw only after when the key 'd' has been pressed.
        */
    }
    if (key == 's'){
        // 2nd week portion.
		initializeWaterLines();
		calculatePath();
		waterfall_draw_flag = 1;
    }
    if (key == 'e'){
        // 2nd week portion.
		waterfall_draw_flag = 0;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if( key == 'l'){
        // Open the Open File Dialog
        ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a only txt for Waterfall");
        
        // Check whether the user opened a file
        if( openFileResult.bSuccess){
            ofLogVerbose("User selected a file");
            
            // We have a file, so let's check it and process it
            processOpenFileSelection(openFileResult);
            load_flag = 1;
        }
    }
    
    /* COMSIL1-TODO 4: This is selection dot control part.
     You can select dot in which water starts to flow by left, right direction key (<- , ->).
     */
	int dotnum = this->dots.size();		//dotnum saves number of dots
    if (key == OF_KEY_RIGHT){
		if (!waterfall_draw_flag) {
			if (isActiveIndex + 1 >= dotnum)
				isActiveIndex = (isActiveIndex + 1) % dotnum;
			else isActiveIndex++;
			for (int i = 0; i < dotnum; i++) {
				if (i == isActiveIndex)
					this->dots[i].isActive = true;
				else this->dots[i].isActive = false;
			}
			cout << "Selcted Dot Coordinate is (" << this->dots[isActiveIndex].X << ", " << this->dots[isActiveIndex].Y << ")" << endl;
			initializeWaterLines();
		}
    }
    if (key == OF_KEY_LEFT){
		if (!waterfall_draw_flag) {
			if (isActiveIndex - 1 < 0)
				isActiveIndex = (isActiveIndex - 1) % dotnum + 4;
			else isActiveIndex--;
			for (int i = 0; i < dotnum; i++) {
				if (i == isActiveIndex)
					this->dots[i].isActive = true;
				else this->dots[i].isActive = false;
			}
			cout << "Selcted Dot Coordinate is (" << this->dots[isActiveIndex].X << ", " << this->dots[isActiveIndex].Y << ")" << endl;
			initializeWaterLines();
		}
    }
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
    //Path to the comma delimited file
    //string fileName = "input.txt";
    
    string fileName = openFileResult.getName();
    ofFile file(fileName);
    
    if( !file.exists()) cout << "Target file does not exists." << endl;
    else cout << "We found the target file." << endl;
    
    ofBuffer buffer(file);
    
    /* This variable is for indicating which type of input is being received.
     IF input_type == 0, then work of getting line number is in progress.
     IF input_type == 1, then work of getting line input is in progress.
	 IF input_type == 2, then work of getting dot input is in progress.
	 dot number is considered by if else statement.
	*/
    int input_type = 0;
    
    
    /* COMSIL1-TODO 1 : Below code is for getting the number of line and dot, getting coordinates.
     You must maintain those information. But, currently below code is not complete.
     Also, note that all of coordinate should not be out of screen size.
     However, all of coordinate do not always turn out to be the case.
     So, You have to develop some error handling code that can detect whether coordinate is out of screen size.
    */
    
    
    // Read file line by line
    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        string line = *it;
        
        // Split line into strings
        vector<string> words = ofSplitString(line, " ");
        
        if( words.size() == 1){
            if( input_type == 0){ // Input for the number of lines.
                num_of_line = atoi(words[0].c_str());
                cout << "The number of line is: " << num_of_line << endl;
				input_type = 1;
            }
            else{ // Input for the number of dots.
                num_of_dot = atoi(words[0].c_str());
                cout << "The number of dot is: " << num_of_dot << endl;
				input_type = 2;
            }
        }
        else if (words.size() >= 2){
			if (input_type == 1) { // Input for actual information of lines
				WaterLine line;

				line.startX = atoi(words[0].c_str());
				line.startY = atoi(words[1].c_str());
				line.endX = atoi(words[2].c_str());
				line.endY = atoi(words[3].c_str());
				line.slope = (double) (line.endY - line.startY) / (line.endX - line.startX);
				if (line.startX <= 1024 && line.startX >= 0)
					if (line.startY <= 728 && line.startY >= 0)
						if (line.endX <= 1024 && line.endX >= 0)
							if (line.endY <= 728 && line.endY >= 0)         //if both sides of line is within screen range, push to vector
								this->lines.push_back(line);
			}
			else if (input_type == 2) { // Input for actual information of dots.
				WaterDot dot;

				dot.X = atoi(words[0].c_str());
				dot.Y = atoi(words[1].c_str());
				dot.isActive = false;
				if (dot.X <= 1024 && dot.X >= 0)
					if (dot.Y <= 728 && dot.Y >= 0)
						this->dots.push_back(dot);                          //if dot is within screen range, push to vector
			}
        } // End of else if.
    } // End of for-loop (Read file line by line).
	dots.front().isActive = true;       //first dot is selected
    initializeWaterLines();
}

void ofApp::initializeWaterLines() {
	Waterfall tmpwater;
    if (water.size() != 0) vector<Waterfall>().swap(water);					//clear vector
	WaterfallIndex = 0;
	tmpwater.X = dots[isActiveIndex].X;										//dot position is start of waterfall
	tmpwater.Y = dots[isActiveIndex].Y;
	this->water.push_back(tmpwater);										//first push start point to vector
}

void ofApp::calculatePath() {
	Waterfall tmp;
	int startX = water[WaterfallIndex].X;
	int startY = water[WaterfallIndex].Y;
	WaterfallIndex++;

	for (; startY <= ofGetHeight() - 50; startY++) {
		for (int i = 0; i < num_of_line; i++) {
			if (startY >= lines[i].startY && startY >= lines[i].endY) continue;
			if (lines[i].startX < lines[i].endX) {
				if (startX <= lines[i].startX || startX >= lines[i].endX)
					continue;
			}
			else if (lines[i].startX > lines[i].endX) {
				if (startX <= lines[i].endX || startX <= lines[i].startX)
					continue;
			}
			double tmp_slope = (double)(startY - lines[i].startY) / (startX - lines[i].startX);

			if (abs(tmp_slope - lines[i].slope) <= EPSILON) {		//if slope offset is smaller than EPSILON, we consider it to touch line
				Waterfall tmpwater;
				tmpwater.X = startX;
				tmpwater.Y = startY+2;
				this->water.push_back(tmpwater);					//push water coord touching the line into vector
				WaterfallIndex++;	
				if (lines[i].slope < 0) {
					water[WaterfallIndex-1].X++;
					startX = lines[i].startX;
					startY = lines[i].startY - 2;
				}
				else {
					water[WaterfallIndex - 1].X--;
					startX = lines[i].endX;
					startY = lines[i].endY - 2;
				}
				Waterfall tmp;
				tmp.X = startX;
				tmp.Y = startY;
				this->water.push_back(tmp);							//push water coord at the end of slope into vector
				WaterfallIndex++;
			}
		}
	}
	tmp.X = startX;
	tmp.Y = startY;
	this->water.push_back(tmp);										//last path (onto floor)
	WaterfallIndex++;
}


