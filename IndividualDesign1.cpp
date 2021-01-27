#include "IndividualDesign1.h"


void InitGraphics()
{
	// request the display modes
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	// set the initial window configuration
	Scale = std::max(Scale, MINSCALE);

	// make sure scene is resonably sized
	sceneRight = std::max(sceneRight, sceneLeft+ (float)0.1);
	sceneTop = std::max(sceneTop, sceneBottom + (float)0.1);

	glutInitWindowPosition(300, 0);
	glutInitWindowSize(winWidth, winHeight);

	// open the window and set its title
	glutCreateWindow("visualization");


	// set the framebuffer clear values
	glClearColor(1., 1., 1., 0.);


	//glutSetWindow(MainWindow);
	glutDisplayFunc(Display);
	glutReshapeFunc(Resize);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(NULL);
	//glutVisibilityFunc(Visibility);
	glutEntryFunc(NULL);
	glutSpecialFunc(NULL);
	glutSpaceballMotionFunc(NULL);
	glutSpaceballRotateFunc(NULL);
	glutSpaceballButtonFunc(NULL);
	glutButtonBoxFunc(NULL);
	glutDialsFunc(NULL);
	glutTabletMotionFunc(NULL);
	glutTabletButtonFunc(NULL);
	glutMenuStateFunc(NULL);
	glutTimerFunc(-1, NULL, 0);
	//glutIdleFunc(Animate);

}

//// initialize the display lists that will not change:
//// (a display list is a way to store opengl commands in
////  memory so that they can be played back efficiently at a later time
////  with a call to glCallList( )
//
//void
//InitLists()
//{
//	glutSetWindow(MainWindow);
//
//	// create Lists:
//
//	SceneList = glGenLists(1);
//	glNewList(SceneList, GL_COMPILE);
//
//	//int i;
//	//struct point *p0, *p1, *p2;
//	//struct tri *tp;
//	//float p01[3], p02[3], n[3];
//
//	glColor3f(1, 1, 0);
//	glPushMatrix();
//	glTranslatef(-2., -2, 2.);
//	glutSolidSphere(1.5,20,20);
//	glPopMatrix();
//
//
//
//	glEndList();
//	// create the axes:
//
//	AxesList = glGenLists(1);
//	glNewList(AxesList, GL_COMPILE);
//	glLineWidth(AXES_WIDTH);
//	Axes(1.5);
//	glLineWidth(1.);
//	glEndList();
//}

void setup(void)
{
	glEnable(GL_DEPTH_TEST); // Enable depth testing.
							 // Turn on OpenGL lighting.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glShadeModel(GL_SMOOTH);
	// Light property vectors.

	glDisable(GL_LIGHTING);
	if (lightsON) {
		glEnable(GL_LIGHTING);

		// Light properties.
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

		glEnable(GL_LIGHT0); // Enable particular light source.

		//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE); // Enable two-sided lighting.
		//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.
	}
}

// Drawing routine.
void Display(void)
{
	//int  i, j;
	//point *p = new struct point [1];
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, winWidth/ winHeight, 0.01, 100.0);
	//glOrtho(-3., 3., -3., 3., 0.1, 1000.);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt((sceneLeft+sceneRight)/2. + xShift, (sceneBottom+sceneTop)/2.+yShift, 40.0, (sceneLeft+sceneRight)/2.+xShift, (sceneBottom+sceneTop)/2.+yShift, 0.0, 0.0, 1.0, 0.0);

	// since we are using glScalef( ), be sure normals get unitized:
	glEnable(GL_NORMALIZE);

	glColor3f(0, 0, 0);


	// Rotate the scene with mouse
	glRotatef((GLfloat)Yrot, 0., 1., 0.);
	glRotatef((GLfloat)Xrot, 1., 0., 0.);
	// Rotate the scene with keyboard
	glRotatef(Zangle, 0.0, 0.0, 1.0);
	glRotatef(Yangle, 0.0, 1.0, 0.0);
	glRotatef(Xangle, 1.0, 0.0, 0.0);


	glScalef((GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale);

	
										// TODO use display lists ?

	if (lightsON) {
		// Material properties.
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambientMat);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMat);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shineMat);

		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, canvasDiffuse);
	}else 
		glColor4fv(canvasDiffuse);

	// Canvas
	//glPushMatrix();
	//	glBegin(GL_QUADS);
	//		glNormal3f(0.,0.,1.);
	//		glVertex3f(sceneLeft,sceneBottom,0);
	//		glVertex3f(sceneRight,sceneBottom,0);
	//		glVertex3f(sceneRight,sceneTop,0);
	//		glVertex3f(sceneLeft,sceneTop,0);
	//	glEnd();
	//glPopMatrix();
	
	drawGraph(patients[0]);

	glutSwapBuffers();
}

void drawGraph(Patient pat) {
	float xPos = (sceneRight + sceneLeft)/2;
	float yPos = (sceneTop + sceneBottom)/2;
	float maxWidth = 0.;
	float *minDistance = new float[patientsCount - 1];
	char txt[30];
	//sprintf(txt, "       Patient\n%d", pat.ID );

	glPushMatrix();
		glTranslatef(xPos, yPos, 0.);
		glScalef(wScale, hScale, wScale);

		if (lightsON) 
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, baseDiffuse);
		else
			glColor4fv(baseDiffuse);

		glutSolidSphere(10, 20, 20);

		if (lightsON) 
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, textDiffuse);
		else
			glColor4fv(textDiffuse);
		
		DoRasterString(0., 0., .01, txt);

		drawMesh(pat.ring1p, pat.T4Lng);
		drawMesh(pat.ring2p, nrLongitudes);
		drawMesh(pat.ring3p, 50*(1-(pat.FTV1 - pat.FTV4)/pat.FTV1)*nrLongitudes);
		drawMesh(pat.ring4p, floor((pat.age -20)/60. * nrLongitudes));
	glPopMatrix();
}

void drawMesh(Point *p, int maxLat) {
	int iLat, iLng;

	for (iLng = 0; iLng < maxLat-1; iLng++)
	{
		glBegin(GL_QUADS);
		for (iLat = 0; iLat < nrLatitudes - 1; iLat++)
		{
			drawPoint(p[nrLatitudes * iLng + iLat]);
			drawPoint(p[nrLatitudes * iLng + (iLat + 1)]);
			drawPoint(p[nrLatitudes * (iLng + 1) + (iLat + 1)]);
			drawPoint(p[nrLatitudes * (iLng + 1) + iLat]);
		}
		drawPoint(p[nrLatitudes * iLng + iLat]);					// TODO connect the last vertices to the first (close the circle)
		drawPoint(p[nrLatitudes * iLng]);
		drawPoint(p[nrLatitudes * (iLng + 1)]);
		drawPoint(p[nrLatitudes * (iLng + 1) + iLat]);

		glEnd();
	}
}



void setPointMaterial(float matID) {
	float clr[4];

	if (floor(matID) ==-1.) {
		clr[0] = 0.; clr[1] = 0.; clr[2]=0.; clr[3] = 1.;
	}
	else if (matID >= 0. && matID <= 1.) {
		mapColor(matID, clr[0], clr[1], clr[2]);
		clr[3] = 1.;
	}
	else if (int(matID) == 2) {
		copyVec(T2T3Diffuse, clr, 4);
	}
	else if (int(matID) == 3) {
		copyVec(T3T4Diffuse, clr, 4);
	}
	else {
		copyVec(T1T2Diffuse, clr, 4);
	}

	if (lightsON) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMat);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMat);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shineMat);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, clr);
	}
	else
		glColor4fv(clr);
}

void mapColor(float value, float &red, float &green, float &blue)
{
	float aR = 0., aG = 0., aB = 1.;  // RGB for our 1st color (blue in this case).
	float bR = 1., bG = 0., bB = 0.;    // RGB for our 2nd color (red in this case).

	red = (bR - aR) * value + aR;      // Evaluated as -255*value + 255.
	green = (bG - aG) * value + aG;      // Evaluates as 0.
	blue = (bB - aB) * value + aB;      // Evaluates as 255*value + 0.
}

/*void setGlyphMaterial(Patient pat, int row) {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMat);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMat);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shineMat);


	if (row <= pat.T4Lng) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, baseDiffuse);
	}
	else if (row <= pat.T3Lng) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, T3T4Diffuse);
	}
	else if (row <= pat.T2Lng) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, T2T3Diffuse);
	}
	else {
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, T1T2Diffuse);
	}
}*/


void makeGraph(Patient *pat)
{
	float rgb[3];
	float R = 100, *tmpRadii, *pntColors;
	float ring1OutR = 100.;
	float ring2OutR = 0.725* ring1OutR, ring2InR = 10.;
	float ring3OutR = ring2OutR - 2 * ring2InR, ring3InR = ring2InR;
	float ring4OutR = 0.3 * ring2OutR , ring4InR = ring2InR;

	float h1 = (float)pat->T1T2days / MAXDAYS;
	float h2 = (float)pat->T2T3days / MAXDAYS;
	float h3 = (float)pat->T3T4days / MAXDAYS;
	float r1 = FTV_SCALE * max(MINRADIUS, pat->FTV1);
	float r2 = FTV_SCALE * max(MINRADIUS, pat->FTV2);
	float r3 = FTV_SCALE * max(MINRADIUS, pat->FTV3);
	float r4 = FTV_SCALE * max(MINRADIUS, pat->FTV4);

	//float maxAzi = (h1 + h2 + h3) * 2 * PI;


	// replace -1 (missing data) and scale 
	//float base = RFS_SCALE * max(MINHEIGHT, (float)pat->RFS) * hScale;
	//float h0 = max(MINHEIGHT, (float)pat->glyphHeight) / totalHeight;


	// ensure some minimum number of rows and columns
	nrLongitudes = std::max(nrLongitudes, 5);
	nrLatitudes = std::max(nrLatitudes, 30);

	// index of the row where the slope/color changes
	//pat->T1Lat = 0;
	pat->T2Lng = floor(h1*nrLongitudes);  // .../totalHeight;
	pat->T3Lng = floor((h1 + h2)*nrLongitudes);
	pat->T4Lng = floor((h1 + h2 + h3)*nrLongitudes);

	// allocate the point data structure:
	pat->ring1p = new Point[nrLongitudes * nrLatitudes];
	pat->ring2p = new Point[nrLongitudes * nrLatitudes];
	pat->ring3p = new Point[nrLongitudes * nrLatitudes];
	pat->ring4p = new Point[nrLongitudes * nrLatitudes];
	pntColors = new float[nrLongitudes * nrLatitudes];  // color ID for each point
	setVec<float>(pntColors, 0., nrLongitudes * nrLatitudes);

	printf("Lats:  %d   %d   %d\n", pat->T2Lng, pat->T3Lng, pat->T4Lng);

	// Compute the radius for each row 
	pat->radius = new float[nrLongitudes];
	setVec<float>(pat->radius, 0., nrLongitudes);
	for (int iLng = 0; iLng < nrLongitudes; iLng++) {

		if (iLng <= pat->T2Lng) { // T1-T2
			pat->radius[iLng] = r1 + (iLng)*(r2 - r1) / (h1 * nrLongitudes);// / totalHeight			
		}
		else if (iLng <= pat->T3Lng) { // T2-T3
			pat->radius[iLng] = r2 + (iLng - pat->T2Lng)*(r3 - r2) / (h2 * nrLongitudes);
		}
		else if (iLng <= pat->T4Lng) { // T3-T4
			pat->radius[iLng] = r3 + (iLng - pat->T3Lng)*(r4 - r3) / (h3 * nrLongitudes);
		}
		else  //Above the glyph
			break;

		setVec<float>(pntColors+iLng*nrLatitudes, pat->radius[iLng], nrLatitudes);
		//printf("-----r[%d]=%f\n", iLng, pat->radius[iLng]);
	}
	// lines between test
	setVec<float>(pntColors + pat->T2Lng * nrLatitudes, -1., nrLatitudes);
	setVec<float>(pntColors + pat->T3Lng * nrLatitudes, -1., nrLatitudes);
	setVec<float>(pntColors + pat->T4Lng * nrLatitudes, -1., nrLatitudes);


	divideVec(pntColors, MAXFTV ,nrLongitudes * nrLatitudes);
	TorusPoints(pat->ring1p, pat->radius, R, 0., pntColors);
	
	tmpRadii = new float[nrLongitudes];  // used for fixed-radius torii
	setVec<float>(tmpRadii, ring2InR, nrLongitudes);
	setVec<float>(pntColors, 2., nrLongitudes * nrLatitudes);
	TorusPoints(pat->ring2p, tmpRadii, ring2OutR, 0., pntColors);

	//printf("norm=%f\n", norm(pntColors, nrLongitudes * nrLatitudes));

	setVec<float>(tmpRadii, ring3InR, nrLongitudes);
	setVec<float>(pntColors, 3., nrLongitudes * nrLatitudes);
	TorusPoints(pat->ring3p, tmpRadii, ring3OutR, 0., pntColors);

	setVec<float>(tmpRadii, ring4InR, nrLongitudes);
	setVec<float>(pntColors, 4., nrLongitudes * nrLatitudes);
	TorusPoints(pat->ring4p, tmpRadii, ring4OutR, 2./3.*PI, pntColors);
}

template <typename T> void setVec (T *a, T val, int len){
	//a = new T[len];
	for (int i = 0; i<len; i++)
		a[i] = val;
}

// TODO radii not precise
void TorusPoints(Point *p, float *inRs, float outR, float thetaOffset, float *clrs){
	int indx;
	float theta, phi;
	 //compute the coordinate of the mesh
	for (int iLng = 0; iLng < nrLongitudes; iLng++) {
		//if (iLng > T1Lat)
		//	break;
		theta = (float)iLng/ nrLongitudes * 2. * PI - PI/2 + thetaOffset;
		//printf("theta(%d) %f\n", iLng, theta);
																						
		for (int iLat = 0; iLat <= nrLatitudes; iLat++) {
			//pointIndex(iLat, iLng);
			indx = nrLatitudes * iLng + iLat;
			phi = (float)iLat/ nrLatitudes * 2. * PI;
			
			p[indx].x = cos(theta) * (outR + inRs[iLng] + inRs[iLng] * sin(phi));
			p[indx].y = -sin(theta) * (outR + inRs[iLng] + inRs[iLng] * sin(phi));
			p[indx].z = inRs[iLng] * cos(phi);

			p[indx].nx = cos(theta) / sqrt(2.);				// TODO compute the right normals
			p[indx].ny = -1 / sqrt(2);
			p[indx].nz = sin(theta) / sqrt(2.);
			normalize(&p[indx].nx, &p[indx].ny, &p[indx].nz);

			p[indx].clrID = clrs[indx];
			//printf("-----p[%d]=%f\n", indx, clrs[indx]);
			// 
			//if (iLng <= T4Lng) {
			//	p[indx].setRGBr(0., 1., 0.);
			//	p->setMaterial(baseDiffuse);
			//}
			//else if (iLng <= T3Lng) {
			//	p[indx].setRGBr(0., 0., 1.);
			//	p->setMaterial(T3T4Diffuse);
			//}
			//else if (iLng <= T2Lng) {
			//	p[indx].setRGBr(1., 1., 0.);
			//	p->setMaterial(T2T3Diffuse);
			//}
			//else {
			//	p[indx].setRGBr(1., 0., 0.);
			//	p->setMaterial(T1T2Diffuse);
			//}
		}
	}
}

void normalize(float *x, float *y, float *z) {
	float norm = sqrt(pow(*x, 2) + pow(*y, 2) + pow(*z, 2));
	*x = *x / norm;
	*y = *y / norm;
	*z = *z / norm;
}
void divideVec(float *vec, float divisor, int len) {
	for (int i = 0; i < len; i++) 
		vec[i] /= divisor;
}
float norm(float *x, int len) {
	float sum=0;
	for (int i = 0; i < len; i++) {
		sum += x[i];
		//printf("----  r[%d] = %f\n", i, x[i]);
	}
	return sum;
}
void copyVec(float *src, float *snk, int len) {
	for (int i = 0; i < len; i++)
		 snk[i] = src[i];
}
void loadData(string dataFileName, string IDFileName) {
	float xScale, yScale;
	string line;
	size_t pos=0;
	int count=0, buffSize = 200, tmpInt;
	int NumDataTypes=19;
	vector <string> strVec(NumDataTypes);

	int *usedIDs =  new int[buffSize];
	int *ID = new int[buffSize];
	int *sstat = new int[buffSize];
	int *RFS = new int[buffSize];
	int *T1T2days = new int[buffSize];
	int *T2T3days = new int[buffSize];
	int *T3T4days = new int[buffSize];
	int *RCBClass  = new int[buffSize];
	int *PCR  = new int[buffSize];
	int *HR_HER2 = new int[buffSize];

	float *age = new float[buffSize];
	float *FTV1 = new float[buffSize];
	float *FTV2 = new float[buffSize];
	float *FTV3 = new float[buffSize];
	float *FTV4 = new float[buffSize];



	ifstream dataFile(dataFileName);
	ifstream IDFile(IDFileName);
	if (!dataFile.is_open() || !dataFile.is_open())
		return;

	while (getline(IDFile, line))
	{
		if (line.length() == 4) {
			usedIDs[count] = stoi(line);
			count++;
		}
	}
	
	patientsCount = count;

	count = 0;
	getline(dataFile, line); // discard the header line
	while (getline(dataFile, line))
	{
		//cout << line << '\n';
		pos = 0;
		tmpInt = stoi(line.substr(pos, line.find(",", pos) - pos));
		for (int k = 0; k < patientsCount; k++) {
			if (usedIDs[k] == tmpInt) {
				for (int i = 0; i < NumDataTypes; i++) {
					strVec.at(i) = line.substr(pos, line.find(",", pos) - pos);
					if (strVec.at(i).compare("#N/A") == 0 || strVec.at(i).empty())
						strVec.at(i) = "-1";
					pos = line.find(",", pos) + 1;
				}

				ID[count] = stoi(strVec.at(0));
				FTV1[count] = stof(strVec.at(1));
				FTV2[count] = stof(strVec.at(2));
				FTV3[count] = stof(strVec.at(3));
				FTV4[count] = stof(strVec.at(4));
				age[count] = stof(strVec.at(8));
				sstat[count] = stoi(strVec.at(9));
				RFS[count] = stoi(strVec.at(10));
				RCBClass[count] = stoi(strVec.at(11));
				PCR[count] = stoi(strVec.at(12));
				HR_HER2[count] = stoi(strVec.at(13));

				T1T2days[count] = dates2days(strVec.at(16), strVec.at(15), "YYYYMMDD");
				T2T3days[count] = dates2days(strVec.at(17), strVec.at(16), "YYYYMMDD");
				T3T4days[count] = dates2days(strVec.at(18), strVec.at(17), "YYYYMMDD");
				
				printf("*%d*  %d FTVs(%f , %f , %f , %f) \t Days(%d , %d , %d) \t RFS %d \n",count, ID[count], FTV1[count], FTV2[count], FTV3[count], FTV4[count], T1T2days[count], T2T3days[count], T3T4days[count], RFS[count]);
				
				count++;
				break;
			}
		}
	}

	dataFile.close();
	IDFile.close();

	patients = new Patient[patientsCount];
	for (int i = 0; i < patientsCount; i++) {
		patients[i].set(ID[i], FTV1[i], FTV2[i], FTV3[i], FTV4[i], age[i], sstat[i], RFS[i], RCBClass[i], PCR[i], HR_HER2[i], T1T2days[i], T2T3days[i], T3T4days[i]);
	}

	//patientsCount = 4;			
	//patients[0].set(9999, 50, 20, 10, 0, 99, 7, 80, 0, 0, 0, 30, 30, 30);
	//patients[1].set(9999, 90, 60, 50, 0, 99, 8, 40, 0, 0, 0, 30, 30, 30);
	//patients[2].set(9999, 50, 40, 10, 5, 99, 7, 200, 0, 0, 0 ,30, 30, 30);
	//patients[3].set(9999, 30, 30, 10, 0, 99, 7, 210, 0, 0, 0, 30, 30, 30);

	for (int i = 0; i < patientsCount; i++) {
		//totalWidth +=  patients[i].glyphWidth;
		totalHeight = std::max(totalHeight, patients[i].glyphHeight);
	}

	for (int i = 0; i < patientsCount; i++) {
		makeGraph(patients + i);
	}
	//scaleFact = std::min(xScale, yScale);
																		

	delete[] usedIDs;
	delete[] ID;
	delete[] sstat;
	delete[] RFS;
	delete[] T1T2days;
	delete[] T2T3days;
	delete[] T3T4days;
	delete[] RCBClass;
	delete[] PCR;
	delete[] HR_HER2;

	delete[] age;
	delete[] FTV1;
	delete[] FTV2;
	delete[] FTV3;
	delete[] FTV4;
}

long dates2days(string date1, string date2, string format) {
	int year1 = -1, month1 = -1, day1 = -1;
	int year2 = -1, month2 = -1, day2 = -1;
	int dayesTillMonth[13] = {0,0,31,59,90,120,151,181,212,243,273,304,334};
	long int date1Days=0, date2Days=0;
	//int difference=0;
	//bool date1Larger;
	if (date1.compare("-1")==0 || date2.compare("-1")==0)
		return -1;

	if (format.compare("YYYYMMDD") == 0) {
		if (date1.length() < 8 || date2.length() < 8)
			return -1;
		year1 = stoi(date1.substr(0, 4));
		month1 = stoi(date1.substr(4, 2));
		day1 = stoi(date1.substr(6, 2));
		year2 = stoi(date2.substr(0, 4));
		month2 = stoi(date2.substr(4, 2));
		day2 = stoi(date2.substr(6, 2));
	}
	else {
		printf("Unknown date format\n");
		return -1;
	}
	date1Days = year1 * 365 + dayesTillMonth[month1] + day1;
	date2Days = year2 * 365 + dayesTillMonth[month2] + day2;

	return abs(date1Days - date2Days);
}

void drawPoint(Point p)
{
	//printf("clr = %f\n", p.clrID);

	setPointMaterial(p.clrID);

	//glColor3f(p.r, p.g, p.b);
	glNormal3f(p.nx, p.ny, p.nz);
	//glTexCoord2f(p.s, p.t);
	glVertex3f(p.x, p.y, p.z);
}
//int pointIndex(int Lng, int Lat)
//{
//	if (Lng < 0)	Lng += (nrLongitudes - 1);
//	if (Lat < 0)	Lat += (nrLatitudes - 1);
//	if (Lng > nrLongitudes - 1)	Lng -= (Lng - 1);
//	if (Lat > nrLatitudes - 1)	Lat -= (Lat - 1);
//	return nrLatitudes * Lng + Lat;
//}

void DoRasterString(float x, float y, float z, char *s)
{
	glRasterPos3f((GLfloat)x, (GLfloat)y, (GLfloat)z);

	char c;			// one character to print
	for (; (c = *s) != '\0'; s++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}
}

// OpenGL window reshape routine.
void Resize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, (float)winWidth, (float)winHeight);
	glutPostRedisplay();
}

void MouseButton(int button, int state, int x, int y)
{
	int b = 0;			// LEFT, MIDDLE, or RIGHT


	// get the proper button bit mask:

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		b = LEFT;		break;

	case GLUT_MIDDLE_BUTTON:
		b = MIDDLE;		break;

	case GLUT_RIGHT_BUTTON:
		b = RIGHT;		break;

	default:
		b = 0;
		fprintf(stderr, "Unknown mouse button: %d\n", button);
	}


	// button down sets the bit, up clears the bit:

	if (state == GLUT_DOWN)
	{
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b;		// set the proper bit
	}
	else
	{
		ActiveButton &= ~b;		// clear the proper bit
	}
}
void MouseMotion(int x, int y)
{

	int dx = x - Xmouse;		// change in mouse coords
	int dy = y - Ymouse;

	if ((ActiveButton & LEFT) != 0)
	{
		Xrot += (ROTSTEP*dy);
		Yrot += (ROTSTEP*dx);
	}


	if ((ActiveButton & MIDDLE) != 0)
	{
		Scale += SCLSTEP * (float)(dx - dy);
		// keep object from turning inside-out or disappearing:
		Scale = max(Scale, MINSCALE);
	}

	if ((ActiveButton & RIGHT) != 0)
	{
		xShift += SHFTSTEP * (float)dx;
		yShift += SHFTSTEP * (float)-dy;
	}
	
	Xmouse = x;			// new current position
	Ymouse = y;

	glutPostRedisplay();
}

// Keyboard input processing routine.
void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'r': // reset
		Xrot = 0.; Yrot = 0.;
		xShift = 0.; yShift = 0.;
		Xangle = 0.; Yangle = 0.; Zangle = 0.;
		Scale = 1.;
		glutPostRedisplay();
		break;
	case 'x':
		Xangle += 5.0;
		if (Xangle > 360.0) Xangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'X':
		Xangle -= 5.0;
		if (Xangle < 0.0) Xangle += 360.0;
		glutPostRedisplay();
		break;
	case 'y':
		Yangle += 5.0;
		if (Yangle > 360.0) Yangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'Y':
		Yangle -= 5.0;
		if (Yangle < 0.0) Yangle += 360.0;
		glutPostRedisplay();
		break;
	case 'z':
		Zangle += 5.0;
		if (Zangle > 360.0) Zangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'Z':
		Zangle -= 5.0;
		if (Zangle < 0.0) Zangle += 360.0;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

// Main routine.
int main(int argc, char **argv)
{
	//printInteraction();
	glutInit(&argc, argv);
	//glutCreateWindow("glyph");
	InitGraphics();
	//InitLists();
	setup();
	loadData(DATA_FILE,ID_FILE);
	glutMainLoop();
	return 0;
}
