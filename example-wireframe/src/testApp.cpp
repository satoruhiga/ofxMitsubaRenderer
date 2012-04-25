#include "testApp.h"

#include "ofxMitsubaRenderer.h"

ofMesh mesh;

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	ofBackground(255);
	
	ofxMitsuba::setBackgroundAlpha(false);
	
	mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
	
	int n = 500;
	for (int i = 0; i < 100; i++)
	{
		ofVec3f v;
		v.x = ofRandom(-n, n);
		v.y = ofRandom(150, -200);
		v.z = ofRandom(-n, n);
		
		ofColor c = ofColor::fromHsb(ofRandom(255), 255, 255);
		
		mesh.addVertex(v);
		mesh.addColor(c);
	}
}

//--------------------------------------------------------------
void testApp::update()
{

}

//--------------------------------------------------------------
void testApp::draw()
{
	glEnable(GL_DEPTH_TEST);
	
	ofTranslate(0, 100);

	ofTranslate(ofGetWindowSize()/ 2);
	
	ofRotate(15, -1, 0, 0);
	ofRotate(ofGetElapsedTimef() * 10, 0, 1, 0);
	
	ofFill();
	ofSetColor(255);
	ofPushMatrix();
	ofRotate(90, 1, 0, 0);
	ofRect(-1000, -1000, 2000, 2000);
	ofPopMatrix();

	ofTranslate(0, -100);
	
	ofSetLineWidth(5);
	mesh.draw();

	ofSetLineWidth(20);
	ofNoFill();
	ofSetColor(255, 0, 0);
	ofBox(200);
	
	ofFill();
	ofSetColor(0, 255, 0);
	ofBox(100);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	if (key == ' ')
		ofxMitsuba::preview(ofxMitsuba::DirectIllumination());
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