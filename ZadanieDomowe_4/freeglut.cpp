
#include <iostream>
#include <GL/freeglut.h>
#include <math.h>
#include <vector>
const int glutWindowWidth = 800;
const int glutWindowHeight = 600;
const double M_PI = 3.14159265359;
const double PH_G = 0.6;
using namespace std;


class C_figura
{
public:
	double w, h;//wymiary	
	double x, y;//polozenie
	double red, green, blue;
	void DrawRectangle(double a, double b)
	{
	//	glPushMatrix();


		glColor3d(red, green, blue);

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
	virtual void Draw()=0;
	
};

vector<C_figura*> vec_figur;
class C_bullet;
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
		red = 0.4;
		green = 0.4;
		blue = 0.4;
	}

	void Draw()
	{


		glPushMatrix();
		glTranslated(x, y, 0);
		DrawRectangle(w, h); //body


		glPushMatrix();
		glTranslated(0, h / 2 + h / 4, 0);
		DrawRectangle(w / 2, h / 2); //wieza
		glPushMatrix();
		glRotated(angle, 0, 0, 1); //obrot 
		glPushMatrix();
		glTranslated(w / 2, 0, 0);
		DrawRectangle(w / 1.3, h / 10); //i lufa

		glPopMatrix();
		glPopMatrix();
		glPopMatrix();

		glPopMatrix();
		glPopMatrix();
	}

	
	void trafiony(C_bullet& pocisk);

};

class C_bullet : public C_figura
{
public:

	C_czolg *czolg;
	double time;
	C_bullet(){}
	bool czy_leci;
	double kat_strzalu;
	//konstruktor
	C_bullet(C_czolg czolg ) //pocisk mozna stworzyc tylko z czolgu, pojawia sie na koncu lufy
	{	
		
		//pozycja srodka czolgu + dl lufy*cos kata (pozycja x konca lufy)
		x = czolg.x + (czolg.w / 1.3)*cos(czolg.angle * M_PI / 180);

		//pozycja srodka czolgu + polowa wysokosci (do styku wieza/korpus) + polowa wiezy + dl lufy * sin kata (pozycja y konca lufy)
		y = czolg.y + 3 * czolg.h / 4 + (czolg.w / 1.3) * sin(czolg.angle * M_PI / 180); 
		time = 0;
		czy_leci = false;
		red = 1;
		green = 0;
		blue = 0;
	}
	

	void Draw()
	{
		if (czy_leci)
		{

			x += 0.2* time * cos(kat_strzalu* M_PI / 180);
			y += 0.2* time * sin(kat_strzalu* M_PI / 180) - (PH_G* (time * time)) / 2;

			glPushMatrix();
			glTranslated(x, y, 0);
			DrawRectangle(0.05, 0.05);
			glPopMatrix();
		
		}
		else
		{
			//pozycja srodka czolgu + dl lufy*cos kata (pozycja x konca lufy)  
			x = czolg->x + (czolg->w / 1.4)*cos(czolg->angle * M_PI / 180);

			//pozycja srodka czolgu + polowa wysokosci (do styku wieza/korpus) + polowa wiezy + dl lufy * sin kata (pozycja y konca lufy)
			y = czolg->y + 3 * czolg->h / 4 + (czolg->w / 1.4) *sin(czolg->angle * M_PI / 180);
		}
	

	}

	~C_bullet(){}
};

void C_czolg::trafiony(C_bullet& pocisk)
{
	if (pocisk.x > x - w / 2 && pocisk.x < x + w / 2 && pocisk.y > y - h / 2 && pocisk.y < y + (3 * h) / 4)
	{
		red = 1;
		green = 0;
		blue = 0;
		pocisk.czy_leci = false;
	}

}

C_czolg *czolg1= new C_czolg(0.25, 0.125, -1, 0);
C_czolg *czolg2= new C_czolg (0.20, 0.120, 1, 0);
C_bullet *bullet1=new C_bullet(*czolg1);
C_bullet *bullet2=new C_bullet(*czolg2);





bool keyStates[256]; 

void utworz_powiazania()
{	
	czolg2->angle = 18000;
	bullet1->czolg = czolg1;
	bullet2->czolg = czolg2;
	
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

void lec(C_bullet &bullet)
{
	if (bullet.czy_leci)
	{
		bullet.time += 0.01;
	}

	if (bullet.x > 3 || bullet.y > 3 || bullet.x < -4 || bullet.y < -3) //poza mapa
		bullet.czy_leci = false;
}

void Timer1(int t)
{

	lec(*bullet1);
	lec(*bullet2);
	czolg1->trafiony(*bullet2);
	czolg2->trafiony(*bullet1);
	glutPostRedisplay();

	// nastêpne wywo³anie funkcji timera
	glutTimerFunc(1, Timer1, bullet1->time);
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
		if (czolg1->x >-3.2)
			czolg1->x -= 0.01;
		break;
	}

	case 'd':
	{
		if (czolg1->x <3.2)
			czolg1->x += 0.01;
		break;
	}

	case 'w':
	{
		czolg1->angle += 1;
		break;
	}

	case 's':
	{
		czolg1->angle -= 1;
		break;
	}
	
	case 'c':
	{
	
		if (!bullet1->czy_leci) //blokuje wystrzelenie 2 pocisku
		{
			bullet1->time = 0.01;
			bullet1->czy_leci = true;
			bullet1->kat_strzalu = czolg1->angle;
		}
		break;
	}

		
	case 'j':
	{
		if (czolg1->y <3.2 && czolg1->y >-3.2)
			czolg2->x -= 0.01;
		break;
	}

	case 'l':
	{
		if (czolg1->y <3.2 && czolg1->y >-3.2)
			czolg2->x += 0.01;
		break;
	}

	case 'i':
	{
		czolg2->angle += 1;
		break;
	}

	case 'k':
	{
		czolg2->angle -= 1;
		break;
	}

	case 'n':
	{

		if (!bullet2->czy_leci) //blokuje wystrzelenie 2 pocisku
		{
			bullet2->time = 0.01;
			bullet2->czy_leci = true;
			bullet2->kat_strzalu = czolg2->angle;
		}
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

	if (keyStates['c'])
		keyPressed('c', x, y);
	else
		keyUp('c', x, y);


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

	if (keyStates['n'])
		keyPressed('n', x, y);
	else
		keyUp('n', x, y);
}
void draw_bg(void)
{
	glPushMatrix();
	glColor3d(0, 1, 0);
	glBegin(GL_POLYGON);
	{

		glVertex3d(3.5, 0.01, 0);
		glVertex3d(3.5, -3.5, 0);
		glVertex3d(-3.5, -3.5, 0);
		glVertex3d(-3.5, 0.01, 0);

	}
	glPopMatrix();
}
static void display()
{
	
	// wyczyszenie sceny
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	keyOperations();
	//draw_bg();
	glPushMatrix();
	
	for ( int i=0; i < vec_figur.size(); i++)
	{
		vec_figur[i]->Draw();
	}

	
	

	
	
	glPopMatrix();



	glutSwapBuffers();
}


int main(int argc, char *argv[])
{
	


	utworz_powiazania();
	vec_figur.push_back(czolg1);
	vec_figur.push_back(czolg2);
	vec_figur.push_back(bullet1);
	vec_figur.push_back(bullet2);

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

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	// set white as clear colour
	glClearColor(1, 1, 1, 1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glutTimerFunc(1, Timer1, 0);

	glutMainLoop();

	return 0;
}

