#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	file_.addListener(this, &ofApp::fileEvent);
	file_.addListener(this, &ofApp::fileEvent2);
	file_.setTargetPath("hoge.txt");
}

void ofApp::fileEvent(ofFile &file)
{
	std::cout << "loaded event function called" << std::endl;
}
void ofApp::fileEvent2(const void *sender, ofFile &file)
{
	std::cout << "loaded event function (with sender) called" << std::endl;
}
//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
