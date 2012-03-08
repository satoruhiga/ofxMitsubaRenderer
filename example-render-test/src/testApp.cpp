#include "testApp.h"

#include "ofxMitsubaRenderer.h"


vector<ofVec3f> points;

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	ofBackground(255, 255, 255);
	
	points.resize(500);
	
	for (int i = 0; i < points.size(); i++)
	{
		ofVec3f p = ofVec3f(ofRandom(-200, 200), ofRandom(-500, 100), ofRandom(-200, 200));
		points[i] = p;
	}
}

//--------------------------------------------------------------
void testApp::update()
{

}

//--------------------------------------------------------------
void testApp::draw()
{
	ofPushMatrix();
	
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
	ofRotate(ofGetElapsedTimef() * 20, 0, 1, 0);

	glEnable(GL_DEPTH_TEST);
	
	ofSetColor(0, 255, 0);
	
	for (int i = 0; i < points.size(); i++)
	{
		ofVec3f p = points[i];
		ofBox(p.x, p.y, p.z, 50);
	}
	
	ofxMitsubaEnableVertexColor();
	
	ofSetColor(255, 0, 0);
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofPushMatrix();
	ofRotate(90, 1, 0, 0);
	ofRect(ofVec3f(0, 0, -100), 2000, 2000);
	ofPopMatrix();
	
	ofPopMatrix();
	
	ofSetColor(0);
	ofDrawBitmapString("Press space key", 10, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	if (key == ' ')
		ofxMitsubaPreview(ofxMitsuba::PhotonMapper(4));
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}