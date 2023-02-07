#pragma once

#include "ofMain.h"
#include "ofxWatchFile.h"

class AutoReloadImage : public ofxWatchFile, public ofImage
{
private:
	bool reload(ofFile &file) override {
		ofImage::load(file.path());
		return true;
	}
};

class ofApp : public ofBaseApp{
	
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
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
private:
	AutoReloadImage file_;
};
