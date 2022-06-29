#include "G2D.h"

#ifdef __linux__ 
#include "GL/glut.h"
#endif

#ifdef  _WIN32
#include "glut.h" 
#endif


#ifdef __APPLE__
#include "GLUT/glut.h"
#endif


#include <string>
#include <map>
#include <vector>
#include <chrono>


using namespace std;




/////////////////////////////////////////////////////////////
//
//	    Key Management
//
/////////////////////////////////////////////////////////////



Key keyTable[256];
Key specialKeyTable[256];

void InitKeys()
{
	for (int i = 0; i < 256; i++) keyTable[i] = specialKeyTable[i] = Key::Unknown;

	keyTable['A'] = keyTable['a'] = Key::A;
	keyTable['B'] = keyTable['b'] = Key::B;
	keyTable['C'] = keyTable['c'] = Key::C;
	keyTable['D'] = keyTable['d'] = Key::D;
	keyTable['E'] = keyTable['e'] = Key::E;
	keyTable['F'] = keyTable['f'] = Key::F;
	keyTable['G'] = keyTable['g'] = Key::G;
	keyTable['H'] = keyTable['h'] = Key::H;
	keyTable['I'] = keyTable['i'] = Key::I;
	keyTable['J'] = keyTable['j'] = Key::J;
	keyTable['K'] = keyTable['k'] = Key::K;
	keyTable['L'] = keyTable['l'] = Key::L;
	keyTable['M'] = keyTable['m'] = Key::M;
	keyTable['N'] = keyTable['n'] = Key::N;
	keyTable['O'] = keyTable['o'] = Key::O;
	keyTable['P'] = keyTable['p'] = Key::P;
	keyTable['Q'] = keyTable['q'] = Key::Q;
	keyTable['R'] = keyTable['r'] = Key::R;
	keyTable['S'] = keyTable['s'] = Key::S;
	keyTable['T'] = keyTable['t'] = Key::T;
	keyTable['U'] = keyTable['u'] = Key::U;
	keyTable['V'] = keyTable['v'] = Key::V;
	keyTable['W'] = keyTable['w'] = Key::W;
	keyTable['X'] = keyTable['x'] = Key::X;
	keyTable['Y'] = keyTable['y'] = Key::Y;
	keyTable['Z'] = keyTable['z'] = Key::Z;
	keyTable[' '] = Key::SPACE;

	keyTable['\r'] = Key::ENTER;

	specialKeyTable[GLUT_KEY_LEFT] = Key::LEFT;
	specialKeyTable[GLUT_KEY_DOWN] = Key::DOWN;
	specialKeyTable[GLUT_KEY_RIGHT] = Key::RIGHT;
	specialKeyTable[GLUT_KEY_UP] = Key::UP;
	specialKeyTable[GLUT_KEY_F1] = Key::F1;
	specialKeyTable[GLUT_KEY_F2] = Key::F2;
	specialKeyTable[GLUT_KEY_F3] = Key::F3;
	specialKeyTable[GLUT_KEY_F4] = Key::F4;
	specialKeyTable[GLUT_KEY_F5] = Key::F5;
	specialKeyTable[GLUT_KEY_F6] = Key::F6;
	specialKeyTable[GLUT_KEY_F7] = Key::F7;
	specialKeyTable[GLUT_KEY_F8] = Key::F8;
	specialKeyTable[GLUT_KEY_F9] = Key::F9;
	specialKeyTable[GLUT_KEY_F10] = Key::F10;
	specialKeyTable[GLUT_KEY_F11] = Key::F11;
	specialKeyTable[GLUT_KEY_F12] = Key::F12;
}

map<Key, bool> ActiveKeys;

void keyboard(unsigned char key, int x, int y)
{
	if (key == 0x1b) exit(0);
	ActiveKeys[keyTable[key]] = true;
}
void keyUp(unsigned char key, int x, int y) { ActiveKeys[keyTable[key]] = false; }


void keySpecial(int key, int x, int y) { ActiveKeys[specialKeyTable[key]] = true; }
void keySpecialUp(int key, int x, int y) { ActiveKeys[specialKeyTable[key]] = false; }

bool G2D::IsKeyPressed(Key k)
{
	if (ActiveKeys.find(k) == ActiveKeys.end())      return false;
	return ActiveKeys[k];
}


/////////////////////////////////////////////////////////////
//
//	    window size
//
/////////////////////////////////////////////////////////////


int Wwidth, Wheight;

void reshape(int w, int h)
{
	glutReshapeWindow(Wwidth, Wheight);
}

/////////////////////////////////////////////////////////////
//
//	    Mouse
//
/////////////////////////////////////////////////////////////

bool mousePressed[16] = { false, false, false };
bool mouseClicked[16] = { false, false, false };

int  MouseX = 300;
int  MouseY = 300;

void G2D::GetMousePos(int* x, int* y)
{
	*x = MouseX;
	*y = MouseY;
}

bool G2D::DetectLeftClick()
{

	bool r = mouseClicked[GLUT_LEFT_BUTTON];
	mouseClicked[GLUT_LEFT_BUTTON] = false; // on efface l'ev click
	return r;
}

bool G2D::DetectRightClick()
{
	bool r = mouseClicked[GLUT_RIGHT_BUTTON];
	mouseClicked[GLUT_RIGHT_BUTTON] = false; // on efface l'ev click
	return r;
}


bool G2D::IsAnyMouseButtonPressed()
{
	return mousePressed[0] | mousePressed[1] | mousePressed[2];
}

bool G2D::IsMouseLeftButtonPressed()
{
	return mousePressed[GLUT_LEFT_BUTTON];
}

bool G2D::IsMouseRightButtonPressed()
{
	return mousePressed[GLUT_RIGHT_BUTTON];
}
void mouseMove(int x, int y)
{
	MouseX = x;
	MouseY = Wheight - y;
}
void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) { mousePressed[button] = true;   mouseClicked[button] = true; mouseMove(x, y); }
	if (state == GLUT_UP) { mousePressed[button] = false;  mouseClicked[button] = false; mouseMove(x, y); }
}





/////////////////////////////////////////////////////////////
//
//	    Init
//
/////////////////////////////////////////////////////////////



void InitColors();

void G2D::InitWindow(int& argc, char**& argv, V2 WindowSize, V2 WindowStartPos, std::string name)
{
	glutInit(&argc, argv);

	Wwidth = WindowSize.x;
	Wheight = WindowSize.y;

	glutInitWindowPosition(WindowStartPos.x, WindowStartPos.y);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(Wwidth, Wheight);
	glutCreateWindow(name.c_str());


	InitColors();
}

void(*_animCallBack)(void);

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::duration<double> duration;

auto T0 = Time::now();
auto TPreviousFrame = Time::now();
auto TNewFrame = Time::now();

bool FirstFrame = true;

void anim(int value)
{
	if (FirstFrame)
	{
		T0 = Time::now();   // démarrage du jeu
		TPreviousFrame = Time::now();
		TNewFrame = Time::now();
		FirstFrame = false;
	}
	else
	{
		TPreviousFrame = TNewFrame;
		TNewFrame = Time::now();
	}
	_animCallBack();
	glutPostRedisplay();
	glutTimerFunc(10, anim, value);

}


double G2D::ElapsedTimeFromStartSeconds()
{
	duration elapsed = TNewFrame - T0;
	return elapsed.count();
}

double G2D::ElapsedTimeFromLastCallbackSeconds()
{
	duration elapsed = TNewFrame - TPreviousFrame;
	return elapsed.count();
}



void G2D::Run(void logic(), void render())
{
	glLoadIdentity();
	glOrtho(0, Wwidth, 0, Wheight, -1, 10);    // donne l'echelle x et y
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	// glEnable(GL_POLYGON_SMOOTH); à éviter rendu erroné

	// Init 	
	InitKeys();


	// callbacks
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(keySpecial);
	glutSpecialUpFunc(keySpecialUp);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);


	// logic
	_animCallBack = logic;
	glutDisplayFunc(render);            // fonction appelée lors d'un repaint
	glutTimerFunc(10, anim, 0);         // callback de timer avec id = 0, toutes les 50m
	glutMainLoop();
}