#include <iostream>
#include <GL/freeglut.h>


const int glutWindowWidth = 800;
const int glutWindowHeight = 600;

class C_figura
{
public:
	double w, h;//wymiary	
	double x, y;//polozenie

	void DrawRectangle(double a, double b)
	{
		glPushMatrix();
		glTranslated(x, y, 0);

		glColor3d(0.4, 1, 0.5);

		glBegin(GL_POLYGON);
		{
			glVertex3d(a / 2, b / 2, 0);
			glVertex3d(a / 2, -b / 2, 0);
			glVertex3d(-a / 2, -b / 2, 0);
			glVertex3d(-a / 2, b / 2, 0);

		}
		glEnd();
		glPopMatrix();


	}
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
	}

	

	void DrawCzolg()
	{
		glPushMatrix();

		glPushMatrix();
		glTranslated(x, y, 0);
		DrawRectangle(w, h); //body

		
		glPushMatrix();
		glTranslated(0, h / 2 + h / 4, 0);
		DrawRectangle( w / 2, h / 2); //wieza

		
		glPushMatrix();
		glRotated(angle, 0, 0, 1); //obrot
		glPushMatrix();
		glTranslated(w/2, 0, 0);
		DrawRectangle(w / 1.4, h / 10); //lufa



		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();

		glPopMatrix();

	}
};

C_czolg czolg1(0.25, 0.125, 1, 0);

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

static void display(void)
{
	// wyczyszenie sceny
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();


	czolg1.DrawCzolg();

	glPopMatrix();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{

	case 'a':
	{
		czolg1.x -= 0.05;
		break;
	}

	case 'd':
	{
		czolg1.x += 0.05;
		break;
	}
		
	case 'w':
	{
		czolg1.angle += 10;
		break;
	}

	case 's':
	{
		czolg1.angle -= 10;
		break;
	}


	}
}


int main(int argc, char *argv[])
{

	glutInitWindowSize(glutWindowWidth, glutWindowHeight);
	glutInitWindowPosition(40, 40);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

	glutCreateWindow("OpenGLUT Shapes"); //tworzy okno, jak nazwa mowi

	glutPostRedisplay();
	glutReshapeFunc(resize);
	glutDisplayFunc(display);

	glutIdleFunc(idle);

	glutKeyboardFunc(keyboard);

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