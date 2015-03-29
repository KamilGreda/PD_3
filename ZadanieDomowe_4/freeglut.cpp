
#include <iostream>
#include <GL/freeglut.h>
#include <math.h>
#include <vector>
const int glutWindowWidth = 800;
const int glutWindowHeight = 600;
const double M_PI = 3.14159265359;

using namespace std;

class C_figura
{
public:
	double w, h;//wymiary	
	double x, y;//polozenie

	void DrawRectangle(double a, double b, double r, double g, double blue)
	{
	//	glPushMatrix();
	//glTranslated(x, y, 0);

		glColor3d(r, g, blue);

		glBegin(GL_POLYGON);
		{
			
			glVertex3d(a / 2, b / 2, 0);
			glVertex3d(a / 2, -b / 2, 0);
			glVertex3d(-a / 2, -b / 2, 0);
			glVertex3d(-a / 2, b / 2, 0);

		}
		glEnd();
	//	glPopMatrix();


	}
	virtual void Draw(){};
	static vector <C_figura*> vector;
};

class C_czolg : public C_figura
{

public:

	double angle = 0.0;


	C_czolg(double wys, double szer, double _x, double _y)
	{
		x = _x;
		y = _y;
		w = wys;
		h = szer;
	//	C_figura::vector.push_back( &*this );
	}

	void Draw()
	{


		glPushMatrix();
		glTranslated(x, y, 0);
		DrawRectangle(w, h, 0.4, 0.4, 0.4); //body

		
		glPushMatrix();
		glTranslated(0, h / 2 + h / 4, 0);
		DrawRectangle(w / 2, h / 2, 0.4, 0.4, 0.4); //wieza
		glPushMatrix();
		glRotated(angle, 0, 0, 1); //obrot
		glPushMatrix();
		glTranslated(w / 2, 0, 0);
		DrawRectangle(w / 1.4, h / 10, 0.4, 0.4, 0.4);
		
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();

		glPopMatrix();

	}
};

class C_bullet : public C_figura
{
public:

	C_czolg *czolg;

	C_bullet(){}
	C_bullet(C_czolg czolg ) //pocisk mozna stworzyc tylko z czolgu, pojawia sie na koncu lufy
	{	
		
		//pozycja srodka czolgu + dl lufy*cos kata (pozycja x konca lufy)
		x = czolg.x + (czolg.w / 1.4)*cos(czolg.angle * M_PI / 180);

		//pozycja srodka czolgu + polowa wysokosci (do styku wieza/korpus) + polowa wiezy + dl lufy * sin kata (pozycja y konca lufy)
		y = czolg.y + 3 * czolg.h / 4 + (czolg.w / 1.4) * sin(czolg.angle * M_PI / 180); 
	}
	

	void Draw()
	{
		//pozycja srodka czolgu + dl lufy*cos kata (pozycja x konca lufy)  
		x = czolg->x + (czolg->w / 1.4)*cos(czolg->angle * M_PI / 180);

		//pozycja srodka czolgu + polowa wysokosci (do styku wieza/korpus) + polowa wiezy + dl lufy * sin kata (pozycja y konca lufy)
		y = czolg->y + 3 * czolg->h / 4 + (czolg->w / 1.4) *sin(czolg->angle * M_PI / 180);

		glPushMatrix();
		glTranslated(x, y, 0);
		DrawRectangle(0.01, 0.01,1,0,0);
		glPopMatrix();

	}

	~C_bullet(){}
};
/*
void Timer(int value)
{
	value++;

	if (value % 2)
	{
		aktualna->r = 0;
		aktualna->g = 0;
		aktualna->b = 1;
	}
	else
	{
		aktualna->b = 0;
		aktualna->r = 1;
	}
	// wyœwietlenie sceny
	glutPostRedisplay();

	// nastêpne wywo³anie funkcji timera
	glutTimerFunc(432, Timer, value);
}
*/

C_czolg czolg1(0.25, 0.125, -1, 0);
C_czolg czolg2(0.20, 0.120, 1, 0);
C_bullet bullet1(czolg1);
C_bullet bullet2(czolg2);

bool keyStates[256]; // Create an array of boolean values of length 256 (0-255)  

void utworz_powiazania()
{
	bullet1.czolg = &czolg1;
	bullet2.czolg = &czolg2;
}
float proportion = (float)glutWindowWidth / (float)glutWindowHeight;

static void resize(int width, int height)
{
	const float ar = (float)width / (float)height;
	proportion = ar;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void idle(void)
{
	glutPostRedisplay();
}

void keyPressed(unsigned char key, int x, int y)
{
	keyStates[key] = true;

	switch (key)
	{

	case 'a':
	{
		czolg1.x -= 0.01;
		break;
	}

	case 'd':
	{
		czolg1.x += 0.01;
		break;
	}

	case 'w':
	{
		czolg1.angle += 1;
		break;
	}

	case 's':
	{
		czolg1.angle -= 1;
		break;
	}

	case 'j':
	{
		czolg2.x -= 0.01;
		break;
	}

	case 'l':
	{
		czolg2.x += 0.01;
		break;
	}

	case 'i':
	{
		czolg2.angle += 1;
		break;
	}

	case 'k':
	{
		czolg2.angle -= 1;
		break;
	}

	}
}

void keyUp(unsigned char key, int x, int y) 
{
	keyStates[key] = false;

}


void keyOperations(void) 
{
	double x, y;
	x = y = 0;
	if (keyStates['a'])
		keyPressed('a',x,y);
	else 
		keyUp('a', x, y);

	if (keyStates['d'])
		keyPressed('d', x, y);
	else
		keyUp('d', x, y);

	if (keyStates['w'])
		keyPressed('w', x, y);
	else
		keyUp('w', x, y);

	if (keyStates['s'])
		keyPressed('s', x, y);
	else
		keyUp('s', x, y);



	if (keyStates['j'])
		keyPressed('j', x, y);
	else
		keyUp('j', x, y);

	if (keyStates['l'])
		keyPressed('l', x, y);
	else
		keyUp('l', x, y);

	if (keyStates['i'])
		keyPressed('i', x, y);
	else
		keyUp('i', x, y);

	if (keyStates['k'])
		keyPressed('k', x, y);
	else
		keyUp('k', x, y);

}

static void display()
{
	// wyczyszenie sceny
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	keyOperations();

	glPushMatrix();

	
	czolg1.Draw();
	czolg2.Draw();
	bullet1.Draw();
	bullet2.Draw();
	glPopMatrix();


	glutSwapBuffers();
}





int main(int argc, char *argv[])
{
	
	utworz_powiazania();
	
	

	glutInitWindowSize(glutWindowWidth, glutWindowHeight);
	glutInitWindowPosition(40, 40);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

	glutCreateWindow("OpenGLUT Shapes"); //tworzy okno, jak nazwa mowi

	glutPostRedisplay();
	glutReshapeFunc(resize);
	glutDisplayFunc(display);

	glutIdleFunc(idle);

	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);
//	glutKeyboardFunc(keyboard2);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	// set white as clear colour
	glClearColor(1, 1, 1, 1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);


	//glutTimerFunc(432, Timer, 0);

	glutMainLoop();

	return 0;
}

