//#include <stdio.h>
//#include <stdlib.h>
//#include <ctype.h>
//
//#define _USE_MATH_DEFINES
//#include <math.h>
//
//#ifdef WIN32
//#include <windows.h>
//#pragma warning(disable:4996)
//#include "glew.h"
//#endif
//
//#include <GL/gl.h>
//#include <GL/glu.h>
//#include "glut.h"
#include <iostream>
#include <string>
#include <algorithm> 
#include <fstream>
#include <vector>
#include <cmath>

# include "glut.h"

using namespace std;

// Globals.
//const int glyphRows = 550; // Number of grid columns.
//const int nrLatitudes = 50; // Number of grid rows
						  //static float *vertices = NULL; // Vertex array of the mapped sample on the cone.
						  //static float *normals = NULL; // Normal array of the mapped sample on the cylinder.


class Point {
public:	
	float x, y, z;		// coordinates
	float nx, ny, nz;	// vertex normal
	float clrID;
	//float r, g, b;		// vertex color
	//float diffuse[4];	// diffuse reflection property


	void setPos(float X, float Y, float Z) { x = X; y = Y; z = Z; }
	void setNor(float X, float Y, float Z) { nx = X; ny = Y; nz = Z; }
	//void setRGBr(float R, float G, float B) { r = R; g = G; b = B; }
	//void setMaterial(float *diff) { diffuse[0] = diff[0]; diffuse[1] = diff[1]; diffuse[2] = diff[2]; diffuse[3] = diff[3]; }

	Point() { setPos(0, 0, 0); setNor(0, 0, 0); }//setRGBr(0, 0, 0);
};

class Patient {
public:
	float age, FTV1, FTV2, FTV3, FTV4;
	int ID, sstat, RFS, T1T2days, T2T3days, T3T4days, RCBClass, PCR, HR_HER2;
	float glyphWidth, glyphHeight;
	int T4Lng, T3Lng, T2Lng;
	float *radius;
	
	Point *ring1p, *ring2p, *ring3p, *ring4p;
	// add slope?

	//Glyph(float RFS, float FTV1, float FTV2, float FTV3, float  FTV4float, float T1Time, float T2Time, float T3Time, float T4Time);
	void set(int inID, float inFTV1, float inFTV2, float inFTV3, float  inFTV4, float inAge, int inSstat, int inRFS, int inRCBClass, int inPCR, int inHR_HER2, int inT1T2days, int inT2T3days, int inT3T4days);
	Patient() { set(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); }
};


const float PI = 3.14159265358979f;
const int LEFT = 4;
const int MIDDLE = 2;
const int RIGHT = 1;
const float ROTSTEP = 1.f; // rotation step
const float SCLSTEP = 0.005f; // scale step
const float SHFTSTEP = 0.05f; // view shift step
const float MINSCALE = 0.02f;
const float MINRADIUS = 0.5f;
const float MINHEIGHT = 30.f;
const std::string DATA_FILE = "../../../Breast Tumor Descriptors/Data/Input_AllTCIA.csv";
const std::string ID_FILE = "../../../Breast Tumor Descriptors/Data/Input_IDs_Test.csv";
//Input_IDs_Age 20s-30s.csv   Input_IDs_HER2 Pos   Input_IDs_HR Pos-HER2 Neg     Input_IDs_Triple Neg
const float RFS_SCALE = 1.f;  //TODO If this is used, update the total height, T1rows, T2Rows,...,  and scale factor  
const float FTV_SCALE = 1.f;  //TODO If this is used, update the total height, T1rows, T2Rows,...,  and scale factor  
const int MAXDAYS = 300;  // max of time axis for ring1. max maps to 2pi												// todo find the max duration between T1 -> T4
const float MAXFTV = 73.07495117;   // max volume at T4

//const float MINFTV = 0.;  // 


float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate the scene.
int		Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;				// rotation angles in degrees
int		ActiveButton;			// current button that is down
float	Scale = 1.;					// scaling factor
Patient	*patients;
int		patientsCount = 0;
float	totalWidth = 0.;
float	totalHeight = 0.;
float	sceneLeft = -20;
float	sceneRight = 20;
float	sceneBottom = -20;
float	sceneTop = 20;
float	scaleFact = 1.;
float	wScale = 0.15;
float	hScale = 0.15;
int		winWidth = 1000;
int		winHeight = 750;
float	xShift = 0;
float	yShift = 0;
int nrLongitudes = 100;//(sceneTop - sceneBottom) * 10; // Number of vertical grid points, defined as a function of canvas height
int nrLatitudes = 50; // Number of angular grid points.
float glyphClearance = 0.02; // Clearance between glyphs as percentace value

// Lighing parameters
bool lightsON = true;
float canvasDiffuse[4] = {0.8, 0.8, 0.8, 1.0 };
float textDiffuse[4] = {0., 0., 0., 1.0 };
float T1T2Diffuse[4] = {0.9, 0.0, 0.0, 1. };
float T2T3Diffuse[4] = {0.7, 0.5, 0.0, 1. };
float T3T4Diffuse[4] = {0.0, 0.0, 0.9, 1. };
float baseDiffuse[4] = {0.0, 0.6, 0.0, 1. };
float lightAmb[] = { 0.3, 0.3, 0.3, 1.0 };
float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
float ambientMat[] = { 0.6, 0.6, 0.6, 1.0 };
float specularMat[] = { 1.0, 1.0, 1.0, 1.0 };
float shineMat = 35.0;
float lightPos[] = { 0.0, (sceneBottom + sceneTop)/2-80, 2.0, 1.0 };


//void	Animate();
void	Display();
void	InitGraphics();
void	InitLists();
void	Keyboard(unsigned char, int, int);
void	MouseMotion(int x, int y);
void	MouseButton(int button, int state, int x, int y);
void	Resize(int, int);
void	DoRasterString(float x, float y, float z, char *s);
void	loadData(std::string dataFileName, std::string IDFileName);
void	makeGraph(Patient *g);
void	drawPoint(Point p);
//int		pointIndex(int i, int j);
long	dates2days(string date1, string date2, string format);
void	normalize(float *x, float *y, float *z);
void	setPointMaterial(float matID);
void	drawGraph(Patient pat);
void	drawMesh(Point *p, int maxLat);
void	TorusPoints(Point *p, float *inRs, float outR, float thetaOffset, float *clrs);
void	divideVec(float *vec, float divisor,int len);
float	norm(float *x, int len);
void	copyVec(float *src, float *snk, int len);
template <typename T> void setVec(T *a, T val, int len);
void	mapColor(float value, float &red, float &green, float &blue);
void Patient::set(int inID, float inFTV1, float inFTV2, float inFTV3, float  inFTV4, float inAge, int inSstat, int inRFS, int inRCBClass, int inPCR, int inHR_HER2, int inT1T2days, int inT2T3days, int inT3T4days) {
	ID = inID;
	FTV1 = max(inFTV1, MINRADIUS);
	FTV2 = max(inFTV2, MINRADIUS);
	FTV3 = max(inFTV3, MINRADIUS);
	FTV4 = max(inFTV4, MINRADIUS);
	age = inAge;
	sstat = inSstat;
	RFS = inRFS;
	RCBClass = inRCBClass;
	PCR = inPCR;
	HR_HER2 = inHR_HER2;
	T1T2days = max(inT1T2days, (int)MINHEIGHT);
	T2T3days = max(inT2T3days, (int)MINHEIGHT);
	T3T4days = max(inT3T4days, (int)MINHEIGHT);

	glyphWidth = 2.* std::max(FTV1, std::max(FTV2, std::max(FTV3, FTV4)));
	glyphHeight = (float)RFS + (float)T1T2days + (float)T2T3days + (float)T3T4days;
}

//void DrawPoint(struct point *p);
//void makeCones(float r1, float r2, float r3, float r4, float h1, float h2, float h3, float base, int numRow, int numCol);

