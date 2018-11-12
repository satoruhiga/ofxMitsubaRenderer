#pragma once

#include "ofMain.h"
#include "ofxMitsubaSettings.h"

#define NOT_IMPL ofLogError("ofxMitsubaRenderer", "Not implemented: " + string(__PRETTY_FUNCTION__)), throw "Not implemented";

namespace ofxMitsuba
{
	
class Renderer : public ofBaseRenderer
{
public:
	
	Renderer();
	virtual ~Renderer();

	const string& getType() { static string type = "Mitsuba"; return type; }
    
    void startRender() {
        viewport();
    }
    void finishRender() {}

	void update() {}

	void draw(const ofPolyline& poly) const;
	void draw(const ofPath& shape) const;
	void draw(const ofMesh& vertexData) const;
	void draw(const ofMesh& vertexData, ofPolyRenderMode renderType) const;
	void draw(const ofMesh& vertexData, bool useColors, bool useTextures, bool useNormals) const;
	void draw(const ofMesh& vertexData, ofPolyRenderMode renderType, bool useColors, bool useTextures, bool useNormals) const;
	void draw(const vector<ofPoint>& vertexData, ofPrimitiveMode drawMode) const;
	void draw(const ofImage& image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;
	void draw(const ofFloatImage& image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;
	void draw(const ofShortImage& image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;
	void draw(const of3dPrimitive& model, ofPolyRenderMode renderType) const;
    void draw(const ofNode& model) const {
        NOT_IMPL;
    }
    void draw(const ofBaseVideoDraws & video, float x, float y, float w, float h) const {
        NOT_IMPL;
    }
	
	void pushView() {}
	void popView() {}

	void viewport(ofRectangle viewport);
    void viewport(float x = 0, float y = 0, float width = -1, float height = -1, bool vflip=true);

    ofRectangle getCurrentViewport() const;
    ofRectangle getNativeViewport() const {
        NOT_IMPL;
    }
	int getViewportWidth() const;
	int getViewportHeight() const;
    bool isVFlipped() const {
        NOT_IMPL;
    }
    
    void setCoordHandedness(ofHandednessType handedness) {
        NOT_IMPL;
    }
    ofHandednessType getCoordHandedness() const {
        NOT_IMPL;
    }

    void setupScreenPerspective(float width = -1, float height = -1, float fov = 60, float nearDist = 0, float farDist = 0);
    void setupScreenOrtho(float width = -1, float height = -1, float nearDist = -1, float farDist = 1);
    void setOrientation(ofOrientation orientation, bool vFlip) {
        NOT_IMPL;
    }
    
    glm::mat4 getCurrentMatrix(ofMatrixMode matrixMode_) const {
        NOT_IMPL;
        return glm::mat4();
    }
    glm::mat4 getCurrentOrientationMatrix() const {
        NOT_IMPL;
    }
    glm::mat4 getCurrentViewMatrix() const {
        NOT_IMPL;
    }
    glm::mat4 getCurrentNormalMatrix() const {
        NOT_IMPL;
    }

	void pushMatrix();
	void popMatrix();
    void translate(const glm::vec3 & p);
	void translate(float x, float y, float z = 0);
	void scale(float xAmnt, float yAmnt, float zAmnt = 1);
	void rotate(float degrees, float vecX, float vecY, float vecZ);
    void rotateRad(float degrees, float vecX, float vecY, float vecZ);
    void rotateXRad(float degrees);
    void rotateYRad(float degrees);
    void rotateZRad(float degrees);
    void rotateRad(float degrees);
    void rotateDeg(float degrees, float vecX, float vecY, float vecZ);
	void rotateXDeg(float degrees);
	void rotateYDeg(float degrees);
	void rotateZDeg(float degrees);
	void rotateDeg(float degrees);
    
    void matrixMode(ofMatrixMode mode) {
        currentMatrixMode = mode;
    }
    void loadIdentityMatrix (void) {
        NOT_IMPL;
    }
    void loadMatrix (const glm::mat4 & m) {
        NOT_IMPL;
    }
    void loadMatrix (const float *m) {
        NOT_IMPL;
    }
    void multMatrix (const glm::mat4 & m) {
        currentMatrix = ofMatrix4x4(m) * currentMatrix;
    }
    void multMatrix (const float *m) {
        multMatrix(glm::make_mat4(m));
    }
    void loadViewMatrix(const glm::mat4 & m) {
        NOT_IMPL;
    }
    void multViewMatrix(const glm::mat4 & m) {
        NOT_IMPL;
    }

	void setupGraphicDefaults();
	void setupScreen() { setupScreenPerspective(); }

	void setRectMode(ofRectMode mode) { rectMode = mode; }
	ofRectMode getRectMode() { return rectMode; }
	void setFillMode(ofFillFlag fill) { this->fill = fill; }
	ofFillFlag getFillMode() { return fill; }
	void setLineWidth(float v) { lineWidth = v; }
	void setDepthTest(bool depthTest) {}
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

	bool bClearBg() { return true; };
	void background(const ofColor& c);
	void background(float brightness);
	void background(int hexColor, float _a = 255.0f);
	void background(int r, int g, int b, int a = 255);

    bool getBackgroundAuto() { NOT_IMPL; }
    void setBackgroundAuto(bool bManual) { NOT_IMPL; }

    void clear();
	void clear(float r, float g, float b, float a = 0);
	void clear(float brightness, float a = 0);
	void clearAlpha();

	// drawing
	void drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const;
	void drawRectangle(float x, float y, float z, float w, float h) const;
    void drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) const;
	void drawCircle(float x, float y, float z, float radius) const;
	void drawSphere(float x, float y, float z, float radius) const;
	void drawEllipse(float x, float y, float z, float width, float height) const;
    void drawString(std::string text, float x, float y, float z) const;
    void drawString(const ofTrueTypeFont & font, std::string text, float x, float y) const {
        NOT_IMPL;
    }

	ofHandednessType getCoordHandedness() { printf("getCoordHandedness\n"); return OF_LEFT_HANDED; }
	bool rendersPathPrimitives() { return false; }

	void enableVertexColor();
	void setBsdf(string xml);
	
	void captrueScreenShot(Settings settings);
	void exportFrame(string filename, Settings settings);
	
	string getApplicationPath() { return applicationPath; }
	
    void onDraw(ofEventArgs&);
	void onUpdate(ofEventArgs&);
	
	void setBackgroundAlpha(bool yn) { backgroundAlpha = yn; }
    ofColor getBackgroundColor();
    void setBackgroundColor(const ofColor & c) { background(c); }
    
    void bind(const ofCamera & camera, const ofRectangle & viewport) {
        NOT_IMPL;
    }
    void unbind(const ofCamera & camera) {
        NOT_IMPL;
    }
    
    void enableAntiAliasing() {
        NOT_IMPL;
    }
    void disableAntiAliasing() {
        NOT_IMPL;
    }
    
    void setBitmapTextMode(ofDrawBitmapMode mode) {
        NOT_IMPL;
    }
    
    ofPath & getPath() {
        NOT_IMPL;
    }
    ofStyle getStyle() const {
        NOT_IMPL;
    }
    void setStyle(const ofStyle & style) {
        NOT_IMPL;
    }
    void pushStyle() {
        NOT_IMPL;
    }
    void popStyle() {
        NOT_IMPL;
    }
    void setCurveResolution(int resolution) {
        NOT_IMPL;
    }
    void setPolyMode(ofPolyWindingMode mode) {
        NOT_IMPL;
    }
    const of3dGraphics & get3dGraphics() const {
        NOT_IMPL;
    }
    of3dGraphics & get3dGraphics() {
        NOT_IMPL;
    }
	
protected:

	ofFillFlag fill;
	ofRectMode rectMode;
	
	ofFloatColor currentBackgroundColor;
	ofFloatColor currentColor;
	
	ofRectangle currentViewport;
	
	float fov, nearClip, farClip;
	float eyeX, eyeY, dist;
	ofVec3f vscale, vtranslate;
	
    ofMatrixMode currentMatrixMode;
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
	ShapeData serializeMesh(const ofMesh &vertexData) const;
	
	Settings settings;
	string bsdf;
	
	bool saveAsPreview;
	string applicationPath;
	
	ofMesh mesh2TrianglesMesh(const ofMesh& mesh) const;
    
    of3dGraphics graphics3d;
	
};

void setBackgroundAlpha(bool v);

void enableVertexColor();
void setBsdf(string xml);

void preview(ofxMitsuba::Settings settings = ofxMitsuba::DirectIllumination());

}
