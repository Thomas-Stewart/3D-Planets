#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

#ifdef __APPLE__
#include <GLUT/glut.h>         /* glut.h includes gl.h and glu.h*/
#else
#include <GL/glut.h>         /* glut.h includes gl.h and glu.h*/
#endif


class Planet {
public:
	float degree;
	float spin;
	float radius;
	float distance;
	float color1, color2, color3;
};

class Sphere{
public:
	double angle;
	double spin;
	double radius;
	double distance;
	double red, green, blue;
	double zAxis;
};

//2D stars for depth
class Star{
public:

	double x;
	double y;
	double z;
};

vector<Sphere> mySpheres; //spheres
Sphere Jupiter; //zaxis test

vector<Planet> myPlanets; //orbiting planets
vector<Planet> myMeteors; //meteor orbit
vector<Star> myStars; //backgournd points
vector<Sphere> myRings; //orbiting spheres of planets

void keyboardInput(unsigned char Key, int x, int y); //keyboard input
void mouse(int button, int state, int x, int y); //mous input
void spinDisplay(void); //contiual spin function

void drawSphere(int i){
	glColor3d(mySpheres[i].red, mySpheres[i].green, mySpheres[i].blue);
	glPushMatrix();
	glTranslatef(mySpheres[i].distance*cos(mySpheres[i].angle), mySpheres[i].distance*sin(mySpheres[i].angle), mySpheres[i].zAxis);
	glutSolidSphere(mySpheres[i].radius, 50, 50);
	glPopMatrix();
}
void drawZAxis(){
	glColor3d(Jupiter.red, Jupiter.green, Jupiter.blue);
	glPushMatrix();
	glTranslatef(Jupiter.zAxis, Jupiter.distance*sin(Jupiter.angle), Jupiter.distance*cos(Jupiter.angle));
	glutSolidSphere(Jupiter.radius, 50, 50);
	glPopMatrix();
}
void drawRings(int i){
	glColor3d(myRings[i].red, myRings[i].green, myRings[i].blue);
	glPushMatrix();
	glTranslatef(myRings[i].distance*cos(myRings[i].angle) + (mySpheres[1].distance*cos(mySpheres[1].angle)), myRings[i].distance*sin(myRings[i].angle) + (mySpheres[1].distance*sin(mySpheres[1].angle)), myRings[i].zAxis);
	glutSolidSphere(myRings[i].radius, 5, 5);
	glPopMatrix();
}

void drawStar(int s){
	float c = 3.14159 / 180; //converting constant
	float theta = 0;

	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	for (float i = 0; i < 200; i++)
	{
		theta = (i*(360 / 200))*c;
		glVertex3f(2 * cos(theta) + myStars[s].x, 2 * sin(theta) + myStars[s].y, 2 * myStars[s].z);
	}
	glEnd();

	//flush buffers
	glFlush();
}

void myCircle(float x, float y, float z, float r, float numPoints, float c1, float c2, float c3)
{
	float c = 3.14159 / 180; //converting constant
	float theta = 0;

	glBegin(GL_POLYGON);
	glColor3f(c1, c2, c3);
	for (float i = 0; i < numPoints; i++)
	{
		theta = (i*(360 / numPoints))*c;
		glVertex3f(r*cos(theta) + x, r*sin(theta) + y, 0);
	}
	glEnd();

	//flush buffers
	glFlush();
}

float randfloat(void)
{
	return (rand() % 1000) / 1000.0;
}

void myFancyCircle(float x0, float y0, float z0, float r, int numPoints)
{
	double x, y, z, thet;
	double inc = 360.0 / numPoints;
	double c = 3.14159 / 180.0;
	z = z0;

	glLineWidth(5.0);
	glBegin(GL_LINE_LOOP);
	for (thet = -180.0; thet<180.0; thet += inc) {
		x = r*sin(c*thet) + x0;
		y = r*cos(c*thet) + y0;
		glColor3f(randfloat(), randfloat(), randfloat());
		glVertex3d(x, y, z);
	}
	glEnd();

	glLineWidth(1.0); //restore default line width
	glColor3f(1.0, 1.0, 1.0); //restore default color
}

//Angle for camera
double C_angle;

const double PI = 3.14156;

//radius of camera
double C_Radius;

//rotation speed of camera
double C_increment;

//pos of camera
double eyex, eyey, eyez;

//where camera is aimed
double centerx, centery, centerz;

//up vector
double upx, upy, upz;

//rotation axis
char directionPressed = 'y';

void init()
{
	float scale = 30.0;

	//LIGHTING
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_DEPTH_TEST);

	GLfloat diffu[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat spec[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat amb[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	GLfloat amb2[] = { 0, 0, 0, 0 };
	GLfloat diffu2[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat spec2[] = { 0.25f, 0.25f, 0.25f, 1.0f };

	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffu2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec2);
	glLightfv(GL_LIGHT2, GL_AMBIENT, amb2);

	GLfloat shinnes[] = { 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffu);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shinnes);


	//**************3D SPHERES INITIALIZE*****************
	Sphere Sun;
	Sun.angle = 0;
	Sun.radius = 20;
	Sun.blue = 0;
	Sun.green = 0.5;
	Sun.red = 1;
	Sun.distance = 0;
	Sun.spin = 0;
	Sun.zAxis = 0;
	mySpheres.push_back(Sun);

	Sphere Earth;
	Earth.angle = 0;
	Earth.blue = 1;
	Earth.green = 0;
	Earth.red = 0;
	Earth.distance = 50;
	Earth.radius = 10;
	Earth.spin = 0.1;
	Earth.zAxis = 0;
	mySpheres.push_back(Earth);

	Sphere Mars;
	Mars.angle = 90;
	Mars.blue = 0;
	Mars.green = 0;
	Mars.red = 1;
	Mars.distance = 80;
	Mars.radius = 14;
	Mars.spin = 0.06;
	Mars.zAxis = 10;
	mySpheres.push_back(Mars);

	Jupiter.angle = 90;
	Jupiter.blue = 0.4;
	Jupiter.green = 0.6;
	Jupiter.red = 0.7;
	Jupiter.distance = 180;
	Jupiter.radius = 24;
	Jupiter.spin = 0.06;
	Jupiter.zAxis = 0;

	Sphere *MeteroRings;
	for (int i = 0; i < 100; i++)
	{
		MeteroRings = new Sphere;
		(*MeteroRings).red = 0.5;
		(*MeteroRings).blue = 0.25;
		(*MeteroRings).green = 0.1;
		(*MeteroRings).zAxis = 10 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (40 - 10)));
		(*MeteroRings).angle = rand() % 360;
		(*MeteroRings).spin = rand() % 80 + 20;
		(*MeteroRings).spin /= 1000;
		(*MeteroRings).radius = rand() % 4 + 1;
		(*MeteroRings).distance = 110 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (130 - 110)));
		mySpheres.push_back(*MeteroRings);
	}

	Sphere *Rings;
	for (int i = 0; i < 1000; i++)
	{
		Rings = new Sphere;
		(*Rings).red = 1.0;
		(*Rings).blue = 1.0;
		(*Rings).green = 1.0;
		(*Rings).zAxis = 0;// 10 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (40 - 10)));
		(*Rings).angle = rand() % 360;
		(*Rings).spin = rand() % 120 + 50;
		(*Rings).spin /= 400;
		(*Rings).radius = rand() % 4 + 1;
		(*Rings).radius /= 6;
		(*Rings).distance = 12 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (18 - 12)));
		myRings.push_back(*Rings);
	}

	/*Stars are too heavy for computer to render in 3D*/

	/*
	Sphere *Stars;
	for (int i = 0; i < 15; i++)
	{
	Stars = new Sphere;
	(*Stars).red = 1.0;
	(*Stars).blue = 1.0;
	(*Stars).green = 1.0;
	(*Stars).zAxis = -1000 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1000 - -1000)));
	(*Stars).angle = rand() % 360;
	(*Stars).spin = 0;
	(*Stars).radius = 2;
	(*Stars).distance = 300 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1000 - 300)));
	myStars.push_back(*Stars);
	}*/


	//**************2D INITIALIZE VALUES******************

	//Stars
	Star *temp;
	for (int i = 0; i < 50; i++)
	{
		temp = new Star;
		(*temp).x = rand() % 1500 - 750;
		(*temp).y = rand() % 1500 - 750;
		(*temp).z = rand() % 1500 - 750;
		myStars.push_back(*temp);
	}

	/*OLD 2D INITIAL DATA*/

	////Initialize planet data
	//Planet Moon;
	//Moon.degree = 300;
	//Moon.spin = 0.09;
	//Moon.radius = 1;
	//Moon.distance = 8;
	//Moon.color1 = 0.5;
	//Moon.color2 = 0.5;
	//Moon.color3 = 0.5;
	//myPlanets.push_back(Moon);

	//Planet Earth2;
	//Earth2.degree = 100;
	//Earth2.spin = 0.04;
	//Earth2.radius = 2;
	//Earth2.distance = 25;
	//Earth2.color1 = 0.1;
	//Earth2.color2 = 0.2;
	//Earth2.color3 = 0.7;
	//myPlanets.push_back(Earth2);

	//Planet Mars;
	//Mars.degree = 200;
	//Mars.spin = 0.02;
	//Mars.radius = 3;
	//Mars.distance = 40;
	//Mars.color1 = 0.7;
	//Mars.color2 = 0.1;
	//Mars.color3 = 0.1;
	//myPlanets.push_back(Mars);

	//Planet Saturn;
	//Saturn.degree = 300;
	//Saturn.spin = 0.012;
	//Saturn.radius = 6.5;
	//Saturn.distance = 90;
	//Saturn.color1 = 0.6;
	//Saturn.color2 = 0.8;
	//Saturn.color3 = 0;
	//myPlanets.push_back(Saturn);

	//Planet *SaturnRings;
	//for (int i = 0; i < 200; i++)
	//{
	//	SaturnRings = new Planet;
	//	(*SaturnRings).degree = rand() % 360;
	//	(*SaturnRings).spin = rand() % 600 + 100;
	//	(*SaturnRings).spin /= 10000;
	//	(*SaturnRings).radius = 0.5;
	//	(*SaturnRings).distance = 10 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (14 - 10)));
	//	myRings.push_back(*SaturnRings);
	//}

	//Planet *MeteroRings;
	//for (int i = 0; i < 30; i++)
	//{
	//	MeteroRings = new Planet;
	//	(*MeteroRings).degree = rand() % 360;
	//	(*MeteroRings).spin = rand() % 80 + 20;
	//	(*MeteroRings).spin /= 1000;
	//	(*MeteroRings).radius = rand() % 4 + 1;
	//	(*MeteroRings).distance = 55 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (65 - 55)));
	//	myMeteors.push_back(*MeteroRings);
	//}

	//Planet Jupiter;
	//Jupiter.degree = 1;
	//Jupiter.spin = 0.015;
	//Jupiter.radius = 10;
	//Jupiter.distance = 125;
	//Jupiter.color1 = 0.8;
	//Jupiter.color2 = 0.5;
	//Jupiter.color3 = 0.1;
	//myPlanets.push_back(Jupiter);

	//set clear color to black
	glClearColor(0.0, 0.0, 0.0, 0.0);

	//set fill  color to white
	glColor3f(1.0, 1.0, 1.0);

	//set up standard orthogonal view with clipping 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glOrtho(-scale, scale, -scale, scale, 0.1, 5*scale);
	gluPerspective(90, 1, 0.1, 2000);
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-5500.0, 5500.0, -5500.0, 5500.0, -3000.0, 3000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void initCameraSetting()
{
	//Angle between up vector and camera
	C_angle = 0;

	//Radius of camera
	C_Radius = 100;

	//Rotation speed of camera
	C_increment = 2 * PI / (360 * 2);

	//Recording the currnet position of the camera.
	eyex = 0; eyey = 0; eyez = C_Radius;

	//Specifies the position of the point looked at as (0,0,0).
	centerx = 0; centery = 0; centerz = 0;

	//Specifies the direction of the up vector. 
	upx = 0; upy = 1;  upz = 0;
}

void moveCamera()
{
	//update angle
	C_angle += C_increment;

	//if angle exceeds 2PI
	if (C_angle > 2 * PI)
		C_angle -= 2 * PI;


	//update rotation axis
	if (directionPressed == 'y')
	{
		upx = 0;
		upy = 1;
		upz = 0;

		eyex = C_Radius * cos(C_angle);
		eyey = 0;
		eyez = C_Radius * sin(C_angle);
	}

	else if (directionPressed == 'x')
	{

		upx = 0;
		upy = cos(C_angle + 3.14 / 2);
		upz = sin(C_angle + 3.14 / 2);

		eyex = 0;
		eyey = C_Radius * cos(C_angle);
		eyez = C_Radius * sin(C_angle);


	}

	else if (directionPressed == 'z')
	{
		upx = cos(C_angle + 3.14 / 2);
		upy = sin(C_angle + 3.14 / 2);
		upz = 0;

		eyex = C_Radius * cos(C_angle);
		eyey = C_Radius * sin(C_angle);
		eyez = 25;
	}

	GLfloat lightpos[] = { 1, 1, 1, 0 };
	glLightfv(GL_LIGHT1, GL_POSITION, lightpos);

	glutPostRedisplay();

}

void display(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);

	/* clear window */
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	//PLANETS*******************************

	//3D
	for (int i = 0; i < mySpheres.size(); i++)
	{
		drawSphere(i);
	}

	//stars
	for (int i = 0; i < myStars.size(); i++)
	{
		drawStar(i);
	}

	//rings
	for (int i = 0; i < myRings.size(); i++)
	{
		drawRings(i);
	}

	//Jupiter
	drawZAxis();

	//Sun
	//glColor3d(1, 0.5, 0);        
	//glPushMatrix();        
	//glutSolidSphere(20, 50, 50);    
	//glPopMatrix();

	////earth
	//glColor3d(0, 0, 1);
	//glPushMatrix();
	//glTranslatef(30, 30, 20);
	//glutSolidSphere(10, 50, 50);
	//glPopMatrix();



	//2D
	//glBegin(GL_POINTS);

	////Draw Stars
	//for (int i = 0; i < myStars.size(); i++)
	//{
	//	glColor3f(1, 1, 1);
	//	glVertex3f(myStars[i].x, myStars[i].y, 0);
	//}

	//glEnd();

	////draw moon
	//myCircle((myPlanets[1].distance*cos(myPlanets[1].degree)) + (myPlanets[0].distance*cos(myPlanets[0].degree)), (myPlanets[1].distance*sin(myPlanets[1].degree)) + (myPlanets[0].distance*sin(myPlanets[0].degree)), 0, myPlanets[0].radius, 360, myPlanets[0].color1, myPlanets[0].color2, myPlanets[0].color3);

	////Saturn's Rings
	//for (int i = 0; i < myRings.size(); i++)
	//{
	//	myCircle((myPlanets[3].distance*cos(myPlanets[3].degree)) + (myRings[i].distance*cos(myRings[i].degree)), (myPlanets[3].distance*sin(myPlanets[3].degree)) + (myRings[i].distance*sin(myRings[i].degree)), 0, myRings[i].radius, 360, 1, 1, 1);
	//}

	////Meteors
	//for (int i = 0; i < myMeteors.size(); i++)
	//{
	//	myCircle(myMeteors[i].distance*cos(myMeteors[i].degree), myMeteors[i].distance*sin(myMeteors[i].degree), 0, myMeteors[i].radius, 6, 0.5, 0.25, 0);
	//}

	////Draw sun
	//myCircle(0, 0, 0, 10, 360, 1, 1, 0.1);

	////Draw Planets
	//for (int i = 1; i < myPlanets.size(); i++)
	//{
	//	myCircle(myPlanets[i].distance*cos(myPlanets[i].degree), myPlanets[i].distance*sin(myPlanets[i].degree), 0, myPlanets[i].radius, 360, myPlanets[i].color1, myPlanets[i].color2, myPlanets[i].color3);
	//}
	//
	/* swap and flush GL buffers */
	glutSwapBuffers();
}

void spinDisplay(void)
{
	//update camera position
	moveCamera();

	//Spheres
	for (int i = 0; i < mySpheres.size(); i++)
	{
		mySpheres[i].angle += mySpheres[i].spin;
		if (mySpheres[i].angle > 360.0)
			mySpheres[i].angle = mySpheres[i].angle - 360.0;
	}

	//Rotating rings around planets
	for (int i = 0; i < myRings.size(); i++)
	{
		myRings[i].angle += myRings[i].spin;
		if (myRings[i].angle > 360.0)
			myRings[i].angle = myRings[i].angle - 360.0;
	}

	//specific for revolution on z axis
	Jupiter.angle += Jupiter.spin;
	if (Jupiter.angle > 360.0)
		Jupiter.angle = Jupiter.angle - 360.0;

	/* OLD 2D DRAWING */
	////planet orbit
	//for (int i = 0; i < myPlanets.size(); i++)
	//{
	//	myPlanets[i].degree += myPlanets[i].spin;
	//	if (myPlanets[i].degree > 360.0)
	//		myPlanets[i].degree = myPlanets[i].degree - 360.0;
	//}

	////saturn rings orbit
	//for (int i = 0; i < myRings.size(); i++)
	//{
	//	myRings[i].degree += myRings[i].spin;
	//	if (myRings[i].degree > 360.0)
	//		myRings[i].degree = myRings[i].degree - 360.0;
	//}

	////meteor orbit
	//for (int i = 0; i < myMeteors.size(); i++)
	//{
	//	myMeteors[i].degree += myMeteors[i].spin;
	//	if (myMeteors[i].degree > 360.0)
	//		myMeteors[i].degree = myMeteors[i].degree - 360.0;
	//}
	glutPostRedisplay();
}

void keyboardInput(unsigned char Key, int x, int y)
{
	switch (Key)
	{
		//speed up revolution of camera
	case 'j':
	case 'J':
		C_increment += 0.01;
		break;

		//slow revolution of camera
	case 'k':
	case 'K':
		if (C_increment < 0.01)
		{
			C_increment = 0;
		}
		else
			C_increment -= 0.01;
		break;

		//decrease radius
	case 'i':
	case 'I':
		if (C_Radius == 0)
			C_Radius = 0;
		else
			C_Radius -= 5;
		break;

		//increase radius
	case 'm':
	case 'M':
		C_Radius += 5;
		break;

	case 'y': //rotate around y axis
	case 'Y':
		directionPressed = 'y';
		break;

	case 'x': //rotate around x axis
	case 'X':
		directionPressed = 'x';
		break;

	case 'z': //rotate around z axis
	case 'Z':
		directionPressed = 'z';
		break;
	}
}

//mouse input
void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(spinDisplay);
		break;
	case GLUT_MIDDLE_BUTTON:
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(moveCamera);
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{

	/* Initialize mode and open a window in upper left corner of screen */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("Planets");

	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboardInput);
	glutDisplayFunc(display);
	glutIdleFunc(spinDisplay);
	init();
	initCameraSetting();
	glutMainLoop();
}
