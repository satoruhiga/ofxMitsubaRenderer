#include "ofxMitsubaRenderer.h"

#include "ofxPolyline2Mesh.h"

#include <zlib.h>

namespace ofxMitsuba
{

static const float INV_CHAR = 1.f / 255.f;
static Renderer *currentRenderer = NULL;
static ofPtr<Renderer> mitsuba;
static ofPtr<ofRendererCollection> rendererCollection;

static void setupMitsubaRenderer();
static bool zcompress(const string& input, string& output);

Renderer::Renderer():graphics3d(this)
{
	currentRenderer = this;
	
	globalMatrix.makeIdentityMatrix();
	currentMatrix.makeIdentityMatrix();
	
	enableVertexColor();
	
	currentBackgroundColor.set(0.3);
	currentColor.set(1);

	applicationPath = "/Applications/Mitsuba.app/Contents/MacOS/mitsuba";
	
	saveAsPreview = false;
	fill = OF_FILLED;
	rectMode = OF_RECTMODE_CENTER;

    ofAddListener(ofEvents().draw, this, &Renderer::onDraw);
	ofAddListener(ofEvents().update, this, &Renderer::onUpdate);
	
	// for ofGLRenderer uninitialized bug
	ofSetCoordHandedness(OF_RIGHT_HANDED);
	
	backgroundAlpha = true;
}

Renderer::~Renderer()
{
	currentRenderer = NULL;

	try
	{
        ofRemoveListener(ofEvents().draw, this, &Renderer::onDraw);
		ofRemoveListener(ofEvents().update, this, &Renderer::onUpdate);
	}
	catch (...)
	{
	}
}
	
void Renderer::setupGraphicDefaults()
{
}

//
// viewport
//

void Renderer::viewport(ofRectangle viewport)
{
	currentViewport = viewport;
}

void Renderer::viewport(float x, float y, float width, float height, bool invertY)
{
    if (width < 0) width = ofGetWidth();
    if (height < 0) height = ofGetHeight();
	viewport(ofRectangle(x, y, width, height));
}

ofRectangle Renderer::getCurrentViewport() const
{
	return currentViewport;
}

int Renderer::getViewportWidth() const
{
	return currentViewport.width;
}

int Renderer::getViewportHeight() const
{
	return currentViewport.height;
}

//
// matrix
//

void Renderer::pushMatrix()
{
    matrixStack.push(currentMatrix);
    globalMatrixStack.push(globalMatrix);
    
    globalMatrix.preMult(currentMatrix);
    currentMatrix.makeIdentityMatrix();
}

void Renderer::popMatrix()
{
    if (matrixStack.empty())
    {
        ofLogError("ofxMitsuba::Renderer", "matrix stack underflow");
        return;
    }
    
    currentMatrix = matrixStack.top();
    matrixStack.pop();
    
    globalMatrix = globalMatrixStack.top();
    globalMatrixStack.pop();
}
    
void Renderer::translate(const glm::vec3 & p)
{
    translate(p.x, p.y, p.z);
}

void Renderer::translate(float x, float y, float z)
{
	currentMatrix.glTranslate(x, y, z);
}

void Renderer::scale(float xAmnt, float yAmnt, float zAmnt)
{
	currentMatrix.glScale(xAmnt, yAmnt, zAmnt);
}
    
void Renderer::rotateRad(float degrees, float vecX, float vecY, float vecZ)
{
    currentMatrix.glRotateRad(degrees, vecX, vecY, vecZ);
}

void Renderer::rotateXRad(float degrees)
{
    currentMatrix.glRotateRad(degrees, 1, 0, 0);
}

void Renderer::rotateYRad(float degrees)
{
    currentMatrix.glRotateRad(degrees, 0, 1, 0);
}

void Renderer::rotateZRad(float degrees)
{
    currentMatrix.glRotateRad(degrees, 0, 0, 1);
}

void Renderer::rotateRad(float degrees)
{
    currentMatrix.glRotateRad(degrees, 0, 0, 1);
}

void Renderer::rotateDeg(float degrees, float vecX, float vecY, float vecZ)
{
	currentMatrix.glRotate(degrees, vecX, vecY, vecZ);
}

void Renderer::rotateXDeg(float degrees)
{
	currentMatrix.glRotate(degrees, 1, 0, 0);
}

void Renderer::rotateYDeg(float degrees)
{
	currentMatrix.glRotate(degrees, 0, 1, 0);
}

void Renderer::rotateZDeg(float degrees)
{
	currentMatrix.glRotate(degrees, 0, 0, 1);
}

void Renderer::rotateDeg(float degrees)
{
	currentMatrix.glRotate(degrees, 0, 0, 1);
}

//
// color
//

void Renderer::setColor(int r, int g, int b)
{
	currentColor.set(r, g, b);
	currentColor *= INV_CHAR;
}

void Renderer::setColor(int r, int g, int b, int a)
{
	currentColor.set(r, g, b, a);
	currentColor *= INV_CHAR;
}

void Renderer::setColor(const ofColor& color)
{
	currentColor.set(color * INV_CHAR);
	currentColor *= INV_CHAR;
}

void Renderer::setColor(const ofColor & color, int _a)
{
	currentColor = ofColor(color, _a);
	currentColor *= INV_CHAR;
}

void Renderer::setColor(int gray)
{
	currentColor.set(gray);
	currentColor *= INV_CHAR;
}

void Renderer::setHexColor(int hexColor)
{
	currentColor.setHex(hexColor);
	currentColor *= INV_CHAR;
}

ofColor Renderer::getBackgroundColor()
{
	return currentBackgroundColor;
}

void Renderer::background(const ofColor & c)
{
	currentBackgroundColor = c;
	currentBackgroundColor *= INV_CHAR;
}

void Renderer::background(float brightness)
{
	currentBackgroundColor.set(brightness);
	currentBackgroundColor *= INV_CHAR;
}
void Renderer::background(int hexColor, float _a)
{
	ofColor c(hexColor, _a);
	currentBackgroundColor = c;
	currentBackgroundColor *= INV_CHAR;
}

void Renderer::background(int r, int g, int b, int a)
{
	currentBackgroundColor.set(r, g, b, a);
	currentBackgroundColor *= INV_CHAR;
}

//
// clear
//
    
void Renderer::clear()
{
    
}

void Renderer::clear(float r, float g, float b, float a)
{
	background(r, g, b, a);
}

void Renderer::clear(float brightness, float a)
{
	background(brightness, a);
}

void Renderer::clearAlpha()
{
	NOT_IMPL;
}

//
// draw
//

void Renderer::draw(const ofPolyline & poly) const
{
	NOT_IMPL;
}

void Renderer::draw(const ofPath & shape) const
{
	NOT_IMPL;
}

void Renderer::draw(const ofMesh& vertexData) const
{
	ShapeData s = serializeMesh(vertexData);
    vector<ShapeData> & x = (vector<ShapeData>&) serializedMeshes;
	x.push_back(s);
}

void Renderer::draw(const ofMesh& vertexData, ofPolyRenderMode renderType) const
{
	draw(vertexData);
}

void Renderer::draw(const ofMesh& vertexData, bool useColors, bool useTextures, bool useNormals) const
{
    draw(vertexData);
}    

void Renderer::draw(const ofMesh& vertexData, ofPolyRenderMode renderType, bool useColors, bool useTextures, bool useNormals) const
{
    draw(vertexData);
}

void Renderer::draw(const of3dPrimitive& model, ofPolyRenderMode renderType) const
{
    draw(model.getMesh(), renderType);
}
	
void Renderer::draw(const vector<ofPoint>& vertexData, ofPrimitiveMode drawMode) const
{
	NOT_IMPL;
}

void Renderer::draw(const ofImage& image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const
{
	NOT_IMPL;
}

void Renderer::draw(const ofFloatImage& image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const
{
	NOT_IMPL;
}

void Renderer::draw(const ofShortImage& image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const
{
	NOT_IMPL;
}

void Renderer::drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const
{
	NOT_IMPL;
}

void Renderer::drawRectangle(float x, float y, float z, float w, float h) const
{
	ofMesh mesh;

	mesh.addNormal(ofVec3f(0, 0, 1));
	mesh.addNormal(ofVec3f(0, 0, 1));
	mesh.addNormal(ofVec3f(0, 0, 1));
	mesh.addNormal(ofVec3f(0, 0, 1));

	if (rectMode == OF_RECTMODE_CORNER)
	{
		mesh.addVertex(ofVec3f(x, y, z));
		mesh.addVertex(ofVec3f(x + w, y, z));
		mesh.addVertex(ofVec3f(x + w, y + h, z));
		mesh.addVertex(ofVec3f(x, y + h, z));
	}
	else
	{
		mesh.addVertex(ofVec3f(x - w / 2.0f, y - h / 2.0f, z));
		mesh.addVertex(ofVec3f(x + w / 2.0f, y - h / 2.0f, z));
		mesh.addVertex(ofVec3f(x + w / 2.0f, y + h / 2.0f, z));
		mesh.addVertex(ofVec3f(x - w / 2.0f, y + h / 2.0f, z));
	}
	
	mesh.addTriangle(0, 1, 2);
	mesh.addTriangle(2, 3, 0);
	
	if (fill)
		mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	else
		mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
	
	draw(mesh);
}

void Renderer::drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) const
{
	// NOT_IMPL;
}

void Renderer::drawCircle(float x, float y, float z, float radius) const
{
	// NOT_IMPL;
}

void Renderer::drawSphere(float x, float y, float z, float radius) const
{
    // NOT_IMPL;
}
    
void Renderer::drawEllipse(float x, float y, float z, float width, float height) const
{
	// NOT_IMPL;
}

void Renderer::drawString(std::string text, float x, float y, float z) const
{
	// NOT_IMPL;
}

//
// camera
//

void Renderer::setupScreenPerspective(float width, float height, float fov, float nearDist, float farDist)
{
	if (width < 0) width = ofGetWidth();
	if (height < 0) height = ofGetHeight();

	float viewW = ofGetViewportWidth();
	float viewH = ofGetViewportHeight();

	float eyeX = viewW / 2;
	float eyeY = viewH / 2;
	float halfFov = PI * fov / 360;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;
	float aspect = (float)viewW / viewH;

	if (nearDist == 0) nearDist = dist / 10.0f;
	if (farDist == 0) farDist = dist * 10.0f;

	this->fov = fov;
	this->nearClip = nearDist;
	this->farClip = farDist;
	this->eyeX = eyeX;
	this->eyeY = eyeY;
	this->dist = dist;

//    if(!vFlip)
//    {
//        vscale.set(1, -1, 1);
//        vtranslate.set(0, height, 0);
//    }
//    else
//    {
        vscale.set(1, 1, 1);
        vtranslate.set(0, 0, 0);
//    }
}

void Renderer::setupScreenOrtho(float width, float height, float nearDist, float farDist)
{
	NOT_IMPL;
}

//
// runloop
//

void Renderer::onUpdate(ofEventArgs&)
{
	currentMatrix.makeIdentityMatrix();
	globalMatrix.makeIdentityMatrix();

	matrixStack = stack<ofMatrix4x4>();
	matrixStack.push(currentMatrix);

	globalMatrixStack = stack<ofMatrix4x4>();
	globalMatrixStack.push(globalMatrix);

	serializedMeshes.clear();
}
    
void Renderer::onDraw(ofEventArgs&)
{
    if (saveAsPreview)
    {
        saveAsPreview = false;
        
        string integrator = "<integrator type='path'/>";
        string     sampler = "<sampler type='ldsampler'></sampler>";
        
        exportFrame("preview", settings);
        
        string cmd = getApplicationPath() + " -o "
        + ofToDataPath("preview.png") + " "
        + ofToDataPath("preview.xml"); // + " -r 10";
        system(cmd.c_str());
        
        cmd = "open " + ofToDataPath("preview.png");
        system(cmd.c_str());
        
        ofSetCurrentRenderer(ofGetGLRenderer());
    }
}

void Renderer::exportFrame(string filename, ofxMitsuba::Settings settings)
{
	if (!serializedMeshes.empty())
	{
		ostringstream header_stream;
		uint16_t header = 0x041c;
		uint16_t version = 3;
		
		header_stream.write((char*)&header, sizeof(uint16_t));
		header_stream.write((char*)&version, sizeof(uint16_t));
		
		ofstream ofs(ofToDataPath(filename + ".serialized").c_str());
		
		uint32_t offset = 0;
		vector<uint32_t> offsets;
		
		for (int i = 0; i < serializedMeshes.size(); i++)
		{
			string mesh;
			mesh += header_stream.str();
			mesh += serializedMeshes[i].serializedData;
			
			offsets.push_back(offset);
			offset += mesh.size();
			
			ofs << mesh;
		}
		
		for (int i = 0; i < offsets.size(); i++)
		{
			uint32_t s = offsets[i];
			ofs.write((char*)&s, sizeof(uint32_t));
		}
		
		uint32_t num = serializedMeshes.size();
		ofs.write((char*)&num, sizeof(uint32_t));
	}
	
	ofstream xml(ofToDataPath(filename + ".xml").c_str());
	
	xml << "<?xml version='1.0' encoding='utf-8'?>" << endl;
	xml << "<scene version='0.5.0'>" << endl;
	
	xml << settings.getIntegrator() << endl;
	
	xml << endl;
	
	if (!backgroundAlpha)
	{
		xml << "<emitter type='constant'>" << endl;
		xml << "<srgb name='intensity' value='" << currentBackgroundColor.r << "," << currentBackgroundColor.g << "," << currentBackgroundColor.b << "'/>" << endl;
		xml << "</emitter>" << endl << endl;
	}
	
	xml << "<sensor type='perspective'>" << endl;
	
	xml << "<float name='fov' value='" << fov << "'/>" << endl;
	xml << "<float name='nearClip' value='" << nearClip << "'/>" << endl;
	xml << "<float name='farClip' value='" << farClip << "'/>" << endl;
	
	xml << "<transform name='toWorld'>" << endl;
	xml << "<lookAt target='" << eyeX << ", " << eyeY << ", 0' origin='" << eyeX << ", " << eyeY << ", " << dist << "' up='0, -1, 0'/>" << endl;
	xml << "<scale x='" << vscale.x << "' y='" << vscale.y << "' z='" << vscale.z << "'/>" << endl;
	xml << "<translate x='" << vtranslate.x << "' y='" << vtranslate.y << "' z='" << vtranslate.z << "'/>" << endl;
	xml << "</transform>" << endl;
	
	xml << "<film type='ldrfilm'>" << endl;
	xml << "<integer name='width' value='" << currentViewport.width << "'/>" << endl;
	xml << "<integer name='height' value='" << currentViewport.height << "'/>" << endl;
	
//    if (backgroundAlpha)
//        xml << "<boolean name='alpha' value='true'/>" << endl;
//    else
//        xml << "<boolean name='alpha' value='false'/>" << endl;
	
	xml << "<boolean name='banner' value='false'/>" << endl;
	xml << "<rfilter type='gaussian'/>" << endl;
	xml << "</film>" << endl;
	
	xml << settings.getSampler() << endl;
	
	xml << "</sensor>" << endl;
	
	xml << endl;
	
	for (int i = 0; i < serializedMeshes.size(); i++)
	{
		ShapeData &s = serializedMeshes[i];
		xml << "<shape id='mesh-" << i << "' type='serialized'>" << endl;
		xml << "<string name='filename' value='" << filename << ".serialized'/>" << endl;
		xml << "<integer name='shapeIndex' value='" << s.index << "'/>" << endl;
		xml << "<boolean name='faceNormals' value='true'/>" << endl;
		xml << s.bsdf << endl;
		xml << "</shape>" << endl;
	}
	
	xml << "</scene>\n";
}

//
// mesh
//

enum ETriMeshFlags
{
	EHasNormals      = 0x0001,
	EHasTexcoords    = 0x0002,
	EHasTangents     = 0x0004,
	EHasColors       = 0x0008,
	EFaceNormals     = 0x0010,
	ESinglePrecision = 0x1000,
	EDoublePrecision = 0x2000
};
	
ofMesh Renderer::mesh2TrianglesMesh(const ofMesh& vertexData) const
{
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	
	ofPrimitiveMode m = vertexData.getMode();
	
	if (m == OF_PRIMITIVE_TRIANGLES)
	{
		mesh = vertexData;
		
		if (!vertexData.hasColors())
		{
			for (int i = 0; i < mesh.getNumVertices(); i++)
			{
				mesh.addColor(currentColor);
			}
		}
	}
	else if (m == OF_PRIMITIVE_TRIANGLE_STRIP)
	{
		if (vertexData.hasIndices())
		{
			for (int i = 0; i < vertexData.getNumIndices() - 2; i++)
			{
				int idx1 = vertexData.getIndex(i);
				int idx2 = vertexData.getIndex(i + 1);
				int idx3 = vertexData.getIndex(i + 2);
				
				mesh.addVertex(vertexData.getVertex(idx1));
				mesh.addVertex(vertexData.getVertex(idx2));
				mesh.addVertex(vertexData.getVertex(idx3));
				
				if (vertexData.hasNormals())
				{
					mesh.addNormal(vertexData.getNormal(idx1));
					mesh.addNormal(vertexData.getNormal(idx2));
					mesh.addNormal(vertexData.getNormal(idx3));
				}
				
				if (vertexData.hasColors())
				{
					mesh.addColor(vertexData.getColor(idx1));
					mesh.addColor(vertexData.getColor(idx2));
					mesh.addColor(vertexData.getColor(idx3));
				}
				else
				{
					mesh.addColor(currentColor);
					mesh.addColor(currentColor);
					mesh.addColor(currentColor);
				}
			}
		}
		else
		{
			for (int i = 0; i < vertexData.getNumVertices() - 2; i++)
			{
				mesh.addVertex(vertexData.getVertex(i));
				mesh.addVertex(vertexData.getVertex(i + 1));
				mesh.addVertex(vertexData.getVertex(i + 2));
				
				if (vertexData.hasNormals())
				{
					mesh.addNormal(vertexData.getNormal(i));
					mesh.addNormal(vertexData.getNormal(i + 1));
					mesh.addNormal(vertexData.getNormal(i + 2));
				}
				
				if (vertexData.hasColors())
				{
					mesh.addColor(vertexData.getColor(i));
					mesh.addColor(vertexData.getColor(i + 1));
					mesh.addColor(vertexData.getColor(i + 2));
				}
				else
				{
					mesh.addColor(currentColor);
					mesh.addColor(currentColor);
					mesh.addColor(currentColor);
				}
			}
		}
	}
	else if (m == OF_PRIMITIVE_TRIANGLE_FAN)
	{
		if (vertexData.hasIndices())
		{
			for (int i = 1; i < vertexData.getNumIndices() - 1; i++)
			{
				const int idx1 = 0;
				const int idx2 = vertexData.getIndex(i);
				const int idx3 = vertexData.getIndex(i + 1);
				
				mesh.addVertex(vertexData.getVertex(idx1));
				mesh.addVertex(vertexData.getVertex(idx2));
				mesh.addVertex(vertexData.getVertex(idx3));
				
				if (vertexData.hasNormals())
				{
					mesh.addNormal(vertexData.getNormal(idx1));
					mesh.addNormal(vertexData.getNormal(idx2));
					mesh.addNormal(vertexData.getNormal(idx3));
				}
				
				if (vertexData.hasColors())
				{
					mesh.addColor(vertexData.getColor(idx1));
					mesh.addColor(vertexData.getColor(idx2));
					mesh.addColor(vertexData.getColor(idx3));
				}
				else
				{
					mesh.addColor(currentColor);
					mesh.addColor(currentColor);
					mesh.addColor(currentColor);
				}
			}
		}
		else
		{
			for (int i = 1; i < vertexData.getNumVertices() - 1; i++)
			{
				const int idx1 = 0;
				const int idx2 = i;
				const int idx3 = i + 1;
				
				mesh.addVertex(vertexData.getVertex(idx1));
				mesh.addVertex(vertexData.getVertex(idx2));
				mesh.addVertex(vertexData.getVertex(idx3));
				
				if (vertexData.hasNormals())
				{
					mesh.addNormal(vertexData.getNormal(idx1));
					mesh.addNormal(vertexData.getNormal(idx2));
					mesh.addNormal(vertexData.getNormal(idx3));
				}
				
				if (vertexData.hasColors())
				{
					mesh.addColor(vertexData.getColor(idx1));
					mesh.addColor(vertexData.getColor(idx2));
					mesh.addColor(vertexData.getColor(idx3));
				}
				else
				{
					mesh.addColor(currentColor);
					mesh.addColor(currentColor);
					mesh.addColor(currentColor);
				}
			}
		}
	}
	else if (m == OF_PRIMITIVE_LINES)
	{
		ofxPolyline2Mesh poly;
		poly.setupTube();
		poly.width(lineWidth / 2);
		poly.color(currentColor);
		
		if (vertexData.hasIndices())
		{
			for (int i = 0; i < vertexData.getNumIndices(); i += 2)
			{
				int idx1 = vertexData.getIndex(i);
				int idx2 = vertexData.getIndex(i + 1);

				poly.clear();
				
				if (vertexData.hasColors())
					poly.color(vertexData.getColor(idx1));
				poly.addVertex(vertexData.getVertex(idx1));
				
				if (vertexData.hasColors())
					poly.color(vertexData.getColor(idx2));
				poly.addVertex(vertexData.getVertex(idx2));
				
				ofMesh &m = poly.getMesh();
				mesh.addColors(m.getColors());
				mesh.addNormals(m.getNormals());
				mesh.addVertices(m.getVertices());
			}
		}
		else
		{
			for (int i = 0; i < vertexData.getNumVertices(); i += 2)
			{
				poly.clear();
				
				if (vertexData.hasColors())
					poly.color(vertexData.getColor(i));
				poly.addVertex(vertexData.getVertex(i));
				
				if (vertexData.hasColors())
					poly.color(vertexData.getColor(i + 1));
				poly.addVertex(vertexData.getVertex(i + 1));
				
				ofMesh &m = poly.getMesh();
				mesh.addColors(m.getColors());
				mesh.addNormals(m.getNormals());
				mesh.addVertices(m.getVertices());
			}
		}
	}
	else if (m == OF_PRIMITIVE_LINE_STRIP)
	{
		ofxPolyline2Mesh poly;
		poly.setupTube();
		poly.width(lineWidth / 2);
		poly.color(currentColor);

		if (vertexData.hasIndices())
		{
			for (int i = 0; i < vertexData.getNumIndices(); i++)
			{
				int idx = vertexData.getIndex(i);
				
				if (vertexData.hasColors())
					poly.color(vertexData.getColor(idx));
				
				poly.addVertex(vertexData.getVertex(idx));
			}
		}
		else
		{
			for (int i = 0; i < vertexData.getNumVertices(); i++)
			{
				if (vertexData.hasColors())
					poly.color(vertexData.getColor(i));
				
				poly.addVertex(vertexData.getVertex(i));
			}
		}
		
		mesh = poly.getMesh();
	}
	else if (m == OF_PRIMITIVE_LINE_LOOP)
	{
		ofxPolyline2Mesh poly;
		poly.setupTube();
		poly.width(lineWidth / 2);
		poly.color(currentColor);
		
		if (vertexData.hasIndices())
		{
			for (int i = 0; i < vertexData.getNumIndices(); i++)
			{
				int idx = vertexData.getIndex(i);
				
				if (vertexData.hasColors())
					poly.color(vertexData.getColor(idx));
				
				poly.addVertex(vertexData.getVertex(idx));
			}
			
			if (vertexData.hasColors())
				poly.color(vertexData.getColor(vertexData.getIndex(0)));
			
			poly.addVertex(vertexData.getVertex(vertexData.getIndex(0)));
		}
		else
		{
			for (int i = 0; i < vertexData.getNumVertices(); i++)
			{
				if (vertexData.hasColors())
					poly.color(vertexData.getColor(i));
				
				poly.addVertex(vertexData.getVertex(i));
			}
			
			if (vertexData.hasColors())
				poly.color(vertexData.getColor(0));
			
			poly.addVertex(vertexData.getVertex(0));
		}
		
		mesh = poly.getMesh();
	}
	else
	{
		cout << "e::" << m << endl;
		NOT_IMPL;
	}
	
	return mesh;
}

Renderer::ShapeData Renderer::serializeMesh(const ofMesh &vertexData) const
{
	ofMesh mesh = mesh2TrianglesMesh(vertexData);
	
	uint32_t flags = ESinglePrecision | EHasColors;
	uint64_t vertexCount = 0, triangleCount = 0;
	
	if (mesh.hasNormals())
		flags |= EHasNormals;

	if (mesh.hasTexCoords())
		flags |= EHasTexcoords;

	vertexCount = mesh.getNumVertices();

	if (mesh.getNumIndices() > 0)
		triangleCount = mesh.getNumIndices() / 3;
	else
		triangleCount = mesh.getNumVertices() / 3;

	ostringstream oss;

	oss.write((char*)&flags, sizeof(uint32_t));

	oss.write((char*)&vertexCount, sizeof(uint64_t));
	oss.write((char*)&triangleCount, sizeof(uint64_t));

	ofMatrix4x4 matrix = globalMatrix;
	matrix.preMult(currentMatrix);
	
	// vertices
	for (int i = 0; i < mesh.getNumVertices(); i++)
	{
		ofVec3f v = mesh.getVertex(i);
		v = matrix.preMult(v);
		oss.write((char*)&v, sizeof(ofVec3f));
	}

	// normals
	if (flags & EHasNormals)
	{
		assert(mesh.getNumVertices() == mesh.getNumNormals());

		ofMatrix4x4 rot = matrix.getRotate();

		for (int i = 0; i < mesh.getNumNormals(); i++)
		{
			ofVec3f n = mesh.getNormal(i);
			n = rot.preMult(n);
			oss.write((char*)&n, sizeof(ofVec3f));
		}
	}

	// texcorrds
	if (flags & EHasTexcoords)
	{
		assert(mesh.getNumVertices() == mesh.getNumTexCoords());
		oss.write((char*)mesh.getTexCoordsPointer(),
				  sizeof(ofVec2f) * mesh.getNumTexCoords());
	}

	// colors
	if (!mesh.hasColors())
	{
		for (int i = 0; i < mesh.getNumVertices(); i++)
		{
			oss.write((char*)&currentColor, sizeof(float) * 3);
		}
	}
	else
	{
		assert(mesh.getNumVertices() == mesh.getNumColors());

		ofFloatColor *src = mesh.getColorsPointer();
		for (int i = 0; i < mesh.getNumColors(); i++)
		{
			oss.write((char*)src, sizeof(float) * 3);
			src++;
		}
	}
	
	// indecies
	if (mesh.getNumIndices() > 0)
	{
		ofIndexType *src = mesh.getIndexPointer();
		for (int i = 0; i < mesh.getNumIndices(); i++)
		{
			oss.write((char*)src, sizeof(ofIndexType));
			src++;
		}
	}
	else
	{
		ofIndexType index = 0;
		for (int i = 0; i < mesh.getNumVertices(); i++)
		{
			oss.write((char*)&index, sizeof(ofIndexType));
			index++;
		}
	}

	string output;
	zcompress(oss.str(), output);
	
	ShapeData data;
	data.index = serializedMeshes.size();
	data.serializedData = output;
	data.bsdf = bsdf;
	
	return data;
}

//
// material
//

void Renderer::enableVertexColor()
{
	setBsdf("<bsdf type='twosided'><bsdf type='diffuse'><texture type='vertexcolors' name='reflectance'/></bsdf></bsdf>");
}

void Renderer::setBsdf(string xml)
{
	bsdf = xml;
}

void Renderer::captrueScreenShot(ofxMitsuba::Settings settings)
{
	this->settings = settings;
	saveAsPreview = true;
}

bool zcompress(const string& input, string& output)
{
	z_stream z;
	
	z.zalloc = Z_NULL;
	z.zfree = Z_NULL;
	z.opaque = Z_NULL;
	
	output.resize(input.size() * 1.01 + 12);
	
	int status = Z_OK;
	
	status = deflateInit(&z, Z_DEFAULT_COMPRESSION);
	
	if (status != Z_OK)
	{
		return -1;
	}
	
	z.total_out = 0;
	z.next_in = (Bytef*)input.data();
	z.avail_in = input.size();
	
	do
	{
		z.next_out = (Bytef*)output.data() + z.total_out;
		z.avail_out = output.size() - z.total_out;
		status = deflate(&z, Z_FINISH);
	}
	while (status == Z_OK);
	
	output.resize(z.total_out);
	
	deflateEnd(&z);
}

void setup()
{
	if (!rendererCollection)
	{
		mitsuba = ofPtr<Renderer>(new Renderer);
		
		rendererCollection = ofPtr<ofRendererCollection>(new ofRendererCollection);
		rendererCollection->renderers.push_back(ofGetCurrentRenderer());
		rendererCollection->renderers.push_back(mitsuba);
	}
}

void setBackgroundAlpha(bool v)
{
	ofxMitsuba::setup();
	ofxMitsuba::mitsuba->setBackgroundAlpha(v);
}
	
void enableVertexColor()
{
	ofxMitsuba::setup();
	ofxMitsuba::mitsuba->enableVertexColor();
}

void setBsdf(string xml)
{
	ofxMitsuba::setup();
	ofxMitsuba::mitsuba->setBsdf(xml);
}

void preview(ofxMitsuba::Settings settings)
{
	ofxMitsuba::setup();
	ofSetCurrentRenderer(ofxMitsuba::rendererCollection);
	ofxMitsuba::mitsuba->captrueScreenShot(settings);
}

}
