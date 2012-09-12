#pragma once

#include "ofMain.h"
#include "ofxMitsubaSettings.h"

namespace ofxMitsuba
{
	
class Renderer : public ofBaseRenderer
{
public:
	
	Renderer();
	virtual ~Renderer();

	string getType() { return "Mitsuba"; }

	void update() {}

	void draw(ofPolyline& poly);
	void draw(ofPath& shape);
    void draw(ofMesh& vertexData);
	void draw(ofMesh& vertexData, ofPolyRenderMode renderType);
	void draw(ofMesh& vertexData, bool useColors, bool useTextures, bool useNormals);
	void draw(ofMesh& vertexData, ofPolyRenderMode renderType, bool useColors, bool useTextures, bool useNormals);
	void draw(vector<ofPoint>& vertexData, ofPrimitiveMode drawMode);
	void draw(ofImage& image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh);
	void draw(ofFloatImage& image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh);
	void draw(ofShortImage& image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh);

	void pushView() {}
	void popView() {}

	void viewport(ofRectangle viewport);
	void viewport(float x = 0, float y = 0, float width = 0, float height = 0, bool invertY = true);

	ofRectangle getCurrentViewport();
	int getViewportWidth();
	int getViewportHeight();

	void setupScreenPerspective(float width = 0, float height = 0, ofOrientation orientation = OF_ORIENTATION_UNKNOWN, bool vFlip = true, float fov = 60, float nearDist = 0, float farDist = 0);
	void setupScreenOrtho(float width = 0, float height = 0, ofOrientation orientation = OF_ORIENTATION_UNKNOWN, bool vFlip = true, float nearDist = -1, float farDist = 1);

	void pushMatrix();
	void popMatrix();
	void translate(float x, float y, float z = 0);
	void translate(const ofPoint & p);
	void scale(float xAmnt, float yAmnt, float zAmnt = 1);
	void rotate(float degrees, float vecX, float vecY, float vecZ);
	void rotateX(float degrees);
	void rotateY(float degrees);
	void rotateZ(float degrees);
	void rotate(float degrees);

	void setupGraphicDefaults();
	void setupScreen() { setupScreenPerspective(); }

	void setRectMode(ofRectMode mode) { rectMode = mode; }
	ofRectMode getRectMode() { return rectMode; }
	void setFillMode(ofFillFlag fill) { this->fill = fill; }
	ofFillFlag getFillMode() { return fill; }
	void setLineWidth(float v) { lineWidth = v; }
	void setBlendMode(ofBlendMode blendMode) {}
	void setLineSmoothing(bool smooth) {}
	void setCircleResolution(int res) {}
	void enablePointSprites() {}
	void disablePointSprites() {}

	void setColor(int r, int g, int b);
	void setColor(int r, int g, int b, int a);
	void setColor(const ofColor& color);
	void setColor(const ofColor& color, int _a);
	void setColor(int gray);
	void setHexColor(int hexColor);

	ofFloatColor& getBgColor();
	bool bClearBg() { return true; };
	void background(const ofColor& c);
	void background(float brightness);
	void background(int hexColor, float _a = 255.0f);
	void background(int r, int g, int b, int a = 255);

	void setBackgroundAuto(bool bManual) {}

	void clear(float r, float g, float b, float a = 0);
	void clear(float brightness, float a = 0);
	void clearAlpha();

	// drawing
	void drawLine(float x1, float y1, float z1, float x2, float y2, float z2);
	void drawRectangle(float x, float y, float z, float w, float h);
	void drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
	void drawCircle(float x, float y, float z, float radius);
    void drawSphere(float x, float y, float z, float radius);
	void drawEllipse(float x, float y, float z, float width, float height);
	void drawString(string text, float x, float y, float z, ofDrawBitmapMode mode);

	ofHandednessType getCoordHandedness() { printf("getCoordHandedness\n"); return OF_LEFT_HANDED; }
	bool rendersPathPrimitives() { return false; }

	void enableVertexColor();
	void setBsdf(string xml);
	
	void captrueScreenShot(Settings settings);
	void exportFrame(string filename, Settings settings);
	
	string getApplicationPath() { return applicationPath; }
	
	void onUpdate(ofEventArgs&);
	void onDraw(ofEventArgs&);
	
	void setBackgroundAlpha(bool yn) { backgroundAlpha = yn; }
	
protected:

	ofFillFlag fill;
	ofRectMode rectMode;
	
	ofFloatColor currentBackgroundColor;
	ofFloatColor currentColor;
	
	ofRectangle currentViewport;
	
	float fov, nearClip, farClip;
	float eyeX, eyeY, dist;
	ofVec3f vscale, vtranslate;
	
	ofMatrix4x4 currentMatrix, globalMatrix;
	stack<ofMatrix4x4> matrixStack, globalMatrixStack;
	
	float lineWidth;
	bool backgroundAlpha;
	
	struct ShapeData
	{
		int index;
		string serializedData;
		string bsdf;
	};
	
	vector<ShapeData> serializedMeshes;
	const ShapeData serializeMesh(ofMesh &vertexData);
	
	Settings settings;
	string bsdf;
	
	bool saveAsPreview;
	string applicationPath;
	
	ofMesh mesh2TrianglesMesh(ofMesh& mesh);
	
};

void setBackgroundAlpha(bool v);

void enableVertexColor();
void setBsdf(string xml);

void preview(ofxMitsuba::Settings settings = ofxMitsuba::DirectIllumination());

}
