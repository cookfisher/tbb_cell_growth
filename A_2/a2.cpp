#include <windows.h>
#include <string>
#include <iostream>
#include <thread>
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include "tbb/tick_count.h"
#include "tbb/task.h"
#include "tbb/tbb.h"
#include "tbb/task_group.h"
#include <cstdio>

using namespace tbb;
using namespace std;
 
GLfloat r = 0.0f, g = 0.0f, b = 0.0f;
const int WIDTH = 1024, HEIGHT = 768;
int cell[WIDTH][HEIGHT];
// injection circle points x, y
int cx[9];
int cy[9];
// storage temporary data
int s0, s1, s2, s3, s4, s5, s6, s7, s8;
int s[] = { s0, s1, s2, s3, s4, s5, s6, s7, s8 };
int s10, s11, s12, s13, s14, s15, s16, s17, s18;
int si[9] = { s10, s11, s12, s13, s14, s15, s16, s17, s18 };
int sii[9];
int q[17];

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-0.5f, WIDTH - 0.5f, -0.5f, HEIGHT - 0.5f);
}

// simple solution
void inject(int x, int y, int m) {
	/*x = (2 * m - 1) * x / 2;
	y = y / 2;*/
	
	for (int i = 1; i < 3; i++) {
		int j = i + 1;
		q[1] = cell[x][y - i];
		q[2] = cell[x][y + i];
		q[3] = cell[x + i][y - i];
		q[4] = cell[x + i][y];
		q[5] = cell[x + i][y + i];
		q[6] = cell[x - i][y - i];
		q[7] = cell[x - i][y];
		q[8] = cell[x - i][y + i];

		q[9] = cell[x][y - j];
		q[10] = cell[x][y + j];
		q[11] = cell[x + j][y - j];
		q[12] = cell[x + j][y];
		q[13] = cell[x + j][y + j];
		q[14] = cell[x - j][y - j];
		q[15] = cell[x - j][y];
		q[16] = cell[x - j][y + j];

		cell[x][y - i] = 4;
		cell[x][y + i] = 4;
		cell[x + i][y - i] = 4;
		cell[x + i][y] = 4;
		cell[x + i][y + i] = 4;
		cell[x - i][y - i] = 4;
		cell[x - i][y] = 4;
		cell[x - i][y + i] = 4;

		cell[x][y - j] = 4;
		cell[x][y + j] = 4;
		cell[x + j][y - j] = 4;
		cell[x + j][y] = 4;
		cell[x + j][y + j] = 4;
		cell[x - j][y - j] = 4;
		cell[x - j][y] = 4;
		cell[x - j][y + j] = 4;

	}
}

void fun(void)
{
	cout << "Exiting because of outside screen or memery overstack";
}

// medicine cell moves in 8 directions
// receive data from stored in s[] after injection()
void move() {
	for (int k = 0; k < 50; k++) {
		for (int i = 1; i < 9; i++) {
			if (cx[i] > 1020 || cy[i] > 760 || cx[i] < 0 || cy[i] < 0) {
				atexit(fun);
				_Exit(10);
			}
			if (cell[cx[i]][cy[i]] == 4) {
				cell[cx[i]][cy[i]] = s[i];
				switch (i) {
				case 1: {
					//cx[1] = x;
					cy[i] = cy[i] - 1;
				}
					  break;
				case 2: {
					cx[i] = cx[i] + 1;
					cy[i] = cy[i] - 1;
				}
					  break;
				case 3: {
					cx[i] = cx[i] + 1;
					//cy[3] = y;
				}
					  break;
				case 4: {
					cx[i] = cx[i] + 1;
					cy[i] = cy[i] + 1;
				}
					  break;
				case 5: {
					//cx[5] = x;
					cy[i] = cy[i] + 1;
				}
					  break;
				case 6: {
					cx[i] = cx[i] - 1;
					cy[i] = cy[i] + 1;
				}
					  break;
				case 7: {
					cx[i] = cx[i] - 1;
					//cy[7] = y;
				}
					  break;
				case 8: {
					cx[i] = cx[i] - 1;
					cy[i] = cy[i] - 1;
				}
					  break;
				}//end switch
				if (cx[i] > 1020 || cy[i] > 760 || cx[i] < 0 || cy[i] < 0) {
					atexit(fun);
					_Exit(10);
				}
				s[i] = cell[cx[i]][cy[i]];
				cell[cx[i]][cy[i]] = 4;
			}// endif
		}
	}
}

// change the first circle of the injection point into medicine cells
// store temporary data in s[]
void injection(int x, int y, int m, int num) {
	// multipoints injection
	/*x = (2 * m - 1) * x / 2;
	y = y / 2;*/
	// single injection
	x = x;
	y = y;

	cx[0] = x;
	cy[0] = y;
	cx[1] = x;
	cy[1] = y - 1;
	cx[2] = x + 1;
	cy[2] = y - 1;
	cx[3] = x + 1;
	cy[3] = y;
	cx[4] = x + 1;
	cy[4] = y + 1;
	cx[5] = x;
	cy[5] = y + 1;
	cx[6] = x - 1;
	cy[6] = y + 1;
	cx[7] = x - 1;
	cy[7] = y;
	cx[8] = x - 1;
	cy[8] = y - 1;

	if (cell[cx[0]][cy[0]] == 3) {
		if (num > 5) {
			for (int i = 0; i < 9; i++) {
				if (cell[cx[i]][cy[i]] == 3) {
					cell[cx[i]][cy[i]] = 2;
				}
			}
		}
		else {
			for (int i = 1; i <= num; i++) {
				
				s[i] = cell[cx[i]][cy[i]];
				cell[cx[i]][cy[i]] = 4;
				
			}
		}
	}
	else {
		//else if (cell[cx[0]][cy[0]] == 2) {
		for (int i = 1; i <= num; i++) {
			
			s[i] = cell[cx[i]][cy[i]];
			cell[cx[i]][cy[i]] = 4;
			
		}
	}
	//move();
}

// store temporary data in si[]
void injectioni(int x, int y, int m, int num) {
	// multipoints injection
	/*x = (2 * m - 1) * x / 2;
	y = y / 2;*/
	// single injection
	x = x;
	y = y;

	cx[0] = x;
	cy[0] = y;
	cx[1] = x;
	cy[1] = y - 1;
	cx[2] = x + 1;
	cy[2] = y - 1;
	cx[3] = x + 1;
	cy[3] = y;
	cx[4] = x + 1;
	cy[4] = y + 1;
	cx[5] = x;
	cy[5] = y + 1;
	cx[6] = x - 1;
	cy[6] = y + 1;
	cx[7] = x - 1;
	cy[7] = y;
	cx[8] = x - 1;
	cy[8] = y - 1;

	if (cell[cx[0]][cy[0]] == 3) {
		if (num > 5) {
			for (int i = 0; i < 9; i++) {
				if (cell[cx[i]][cy[i]] == 3) {
					cell[cx[i]][cy[i]] = 2;
				}
			}
		}
		else {
			for (int i = 1; i <= num; i++) {
				
				si[i] = cell[cx[i]][cy[i]];
				cell[cx[i]][cy[i]] = 4;
				
			}
		}
	}
	else {
		//else if (cell[cx[0]][cy[0]] == 2) {
		for (int i = 1; i <= num; i++) {
			
			si[i] = cell[cx[i]][cy[i]];
			cell[cx[i]][cy[i]] = 4;
			
		}
	}
	//move();
}

// store temporary data in sii[]
void injectionii(int x, int y, int m, int num) {
	// multipoints injection
	/*x = (2 * m - 1) * x / 2;
	y = y / 2;*/
	// single injection
	x = x;
	y = y;

	cx[0] = x;
	cy[0] = y;
	cx[1] = x;
	cy[1] = y - 1;
	cx[2] = x + 1;
	cy[2] = y - 1;
	cx[3] = x + 1;
	cy[3] = y;
	cx[4] = x + 1;
	cy[4] = y + 1;
	cx[5] = x;
	cy[5] = y + 1;
	cx[6] = x - 1;
	cy[6] = y + 1;
	cx[7] = x - 1;
	cy[7] = y;
	cx[8] = x - 1;
	cy[8] = y - 1;

	if (cell[cx[0]][cy[0]] == 3) {
		if (num > 5) {
			for (int i = 0; i < 9; i++) {
				if (cell[cx[i]][cy[i]] == 3) {
					cell[cx[i]][cy[i]] = 2;
				}
			}
		}
		else {
			for (int i = 1; i <= num; i++) {

				sii[i] = cell[cx[i]][cy[i]];
				cell[cx[i]][cy[i]] = 4;

			}
		}
	}
	else {
		//else if (cell[cx[0]][cy[0]] == 2) {
		for (int i = 1; i <= num; i++) {

			sii[i] = cell[cx[i]][cy[i]];
			cell[cx[i]][cy[i]] = 4;

		}
	}
	//move();
}

// receive temporary data from stored in si[] after injectioni()
void movei() {
	for (int k = 0; k < 50; k++) {
		for (int i = 1; i < 9; i++) {
			if (cx[i] > 1020 || cy[i] > 760 || cx[i] < 0 || cy[i] < 0) {
				atexit(fun);
				_Exit(10);
			}
			if (cell[cx[i]][cy[i]] == 4) {
				cell[cx[i]][cy[i]] = si[i];
				switch (i) {
				case 1: {
					//cx[1] = x;
					cy[i] = cy[i] - 1;
				}
					  break;
				case 2: {
					cx[i] = cx[i] + 1;
					cy[i] = cy[i] - 1;
				}
					  break;
				case 3: {
					cx[i] = cx[i] + 1;
					//cy[3] = y;
				}
					  break;
				case 4: {
					cx[i] = cx[i] + 1;
					cy[i] = cy[i] + 1;
				}
					  break;
				case 5: {
					//cx[5] = x;
					cy[i] = cy[i] + 1;
				}
					  break;
				case 6: {
					cx[i] = cx[i] - 1;
					cy[i] = cy[i] + 1;
				}
					  break;
				case 7: {
					cx[i] = cx[i] - 1;
					//cy[7] = y;
				}
					  break;
				case 8: {
					cx[i] = cx[i] - 1;
					cy[i] = cy[i] - 1;
				}
					  break;
				}//end switch
				if (cx[i] > 1020 || cy[i] > 760 || cx[i] < 0 || cy[i] < 0) {
					atexit(fun);
					_Exit(10);
				}
				si[i] = cell[cx[i]][cy[i]];
				cell[cx[i]][cy[i]] = 4;
			}// endif
		}
	}
}

// receive temporary data from stored in sii[] after injectionii() 
void moveii() {
	for (int i = 1; i < 9; i++) {
		if (cx[i] > 1020 || cy[i] > 760 || cx[i] < 0 || cy[i] < 0) {
			atexit(fun);
			_Exit(10);
		}
		if (cell[cx[i]][cy[i]] == 4) {
			cell[cx[i]][cy[i]] = sii[i];
			switch (i) {
			case 1: {
				//cx[1] = x;
				cy[i] = cy[i] - 1;
			}
				  break;
			case 2: {
				cx[i] = cx[i] + 1;
				cy[i] = cy[i] - 1;
			}
				  break;
			case 3: {
				cx[i] = cx[i] + 1;
				//cy[3] = y;
			}
				  break;
			case 4: {
				cx[i] = cx[i] + 1;
				cy[i] = cy[i] + 1;
			}
				  break;
			case 5: {
				//cx[5] = x;
				cy[i] = cy[i] + 1;
			}
				  break;
			case 6: {
				cx[i] = cx[i] - 1;
				cy[i] = cy[i] + 1;
			}
				  break;
			case 7: {
				cx[i] = cx[i] - 1;
				//cy[7] = y;
			}
				  break;
			case 8: {
				cx[i] = cx[i] - 1;
				cy[i] = cy[i] - 1;
			}
				  break;
			}//end switch
			if (cx[i] > 1020 || cy[i] > 760 || cx[i] < 0 || cy[i] < 0) {
				atexit(fun);
				_Exit(10);
			}
			sii[i] = cell[cx[i]][cy[i]];
			cell[cx[i]][cy[i]] = 4;
		}// endif
	}
}

void setup(int x, int y, int m) {
	int w = (m * x) + 2;

	for (int i = (w - x); i < w; i++) {
		for (int j = 2; j < y + 2; j++) {

			cell[i][j] = (rand() % 2 + 2); // 2,3
		}
	}
}

void changeColor(GLfloat red, GLfloat green, GLfloat blue) {
	r = red;
	g = green;
	b = blue;
}

//Check status of individual cell and apply the rules: 3 is cancer, 2 is health cell, 4 is medicine
static int checkStatus(int status, int x, int y) {
	int cancerNeighbours = 0;
	int medicineNeighbours = 0;

	for (int i = (x - 1); i < (x + 2); i++) {
		if (cell[i][y - 1] == 3) {
			cancerNeighbours++;
		}
		if (cell[i][y + 1] == 3) {
			cancerNeighbours++;
		}
	}
	if (cell[x - 1][y] == 3) {
		cancerNeighbours++;
	}
	if (cell[x + 1][y] == 3) {
		cancerNeighbours++;
	}

	for (int i = (x - 1); i < (x + 2); i++) {
		if (cell[i][y - 1] == 4) {
			medicineNeighbours++;
		}
		if (cell[i][y + 1] == 4) {
			medicineNeighbours++;
		}
	}
	if (cell[x - 1][y] == 4) {
		medicineNeighbours++;
	}
	if (cell[x + 1][y] == 4) {
		medicineNeighbours++;
	}

	if (status == 3 && medicineNeighbours >= 6) {
		status = 2;
	}
	else if (status == 2 && cancerNeighbours >= 6) {
		status = 3;
	}
	return status;
}

//Display individual pixels.
static void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	GLfloat red, green, blue;


	for (int i = 5; i < (WIDTH - 5); i++) {
		for (int j = 5; j < (HEIGHT - 5); j++) {
			//Check the updated status of the current cell.
			int cellV = checkStatus(cell[i][j], i, j);
			if (cellV == 0) {
				red = r;
				green = 0.0f;
				blue = 1.0;
				cell[i][j] = 0;
			}
			else if (cellV == 2) {
				red = r;
				green = 0.4f;
				blue = b;
				cell[i][j] = 2;
			}
			else if (cellV == 3) {
				red = 0.4f;
				green = g;
				blue = b;
				cell[i][j] = 3;
			}
			else if (cellV == 4) {
				red = 1.0f;
				green = 1.0f;
				blue = 0.0f;
				cell[i][j] = 4;
			}

			glPointSize(1.0f);
			glColor3f(red, green, blue);
			glBegin(GL_POINTS);
			glVertex2i(i, j);
			glEnd();
		}
	}
	glutSwapBuffers();
}

void update(int value) {
	/*try {
		move();
	}
	catch (...) {}*/
	glutPostRedisplay();
	glutTimerFunc(1000 / 30, update, 0);
}

void update1(int value) {
	/*try {
		movei();
	}
	catch (...) {}*/
	glutPostRedisplay();
	glutTimerFunc(1000 / 30, update1, 0);
}

void update2(int value) {
	try {
		moveii();
	}
	catch (...) {}
	glutPostRedisplay();
	glutTimerFunc(1000 / 30, update2, 0);
}

// temporary data store in s[] after injectioni(), move() 50 times
class injects {
	 int px;
	 int py;
	 int num;
public:
	injects(int x, int y, int n) : px(x), py(y), num(n) {}
	void operator() () const {
		injection(px, py, 1, num);
		move();
		glutDisplayFunc(display);
		glutTimerFunc(1000 / 30, update, 0);
		changeColor(r, g, b);
	}
};

// temporary data store in si[] after injectioni(), movei() 50 times
class injects1 {
	int px;
	int py;
	int num;
public:
	injects1(int x, int y, int n) : px(x), py(y), num(n) {}
	void operator() () const {
		injectioni(px, py, 1, num);
		movei();
		glutDisplayFunc(display);
		glutTimerFunc(1000 / 30, update1, 0);
		changeColor(r, g, b);
	}
};

// temporary data store in sii[] after injection(), moveii() is inside update2() and keep refresh frame in 30fps
class injects2 {
	int px;
	int py;
	int num;
public:
	injects2(int x, int y, int n) : px(x), py(y), num(n) {}
	void operator() () const {
		injectionii(px, py, 1, num);
		glutDisplayFunc(display);
		glutTimerFunc(1000 / 30, update2, 0);
		changeColor(r, g, b);
	}
};
 
int main(int argc, char** argv)
{
	int x = 1020;
	int y = 766;
	int m = 1;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Cell Growth Simulator");
	init();

	setup(x, y, m);

	tbb::task_group tg;

	//====== test 1 ================
	tg.run(injects2(600,400,5));
	tg.run(injects1(400,600,4));
	tg.run(injects(200, 500, 3));

	//====== test 2 ================
	/*tg.run(injects2(600, 400, 5));
	tg.run(injects1(400, 600, 4));
	tg.run(injects(200, 500, 3));
	tg.run(injects1(500, 200, 2));*/

	//====== test 3 ================
	/*tg.run(injects(100, 100, 5));
	tg.run(injects(200, 200, 4));
	tg.run(injects1(500, 500, 3));
	tg.run(injects1(600, 600, 2));*/

	//====== test 4 =================
	/*tg.run(injects1(600, 400, 5));
	tg.run(injects1(400, 600, 4));
	tg.run(injects2(200, 500, 3));*/

	//====== test 5 =================
	/*tg.run(injects2(600, 400, 5));
	tg.run(injects2(400, 600, 4));
	tg.run(injects2(200, 500, 3));*/

	//====== test 6 =================
	/*tg.run(injects2(600, 400, 5));
	tg.run(injects1(400, 400, 4));*/

	tg.wait();
	
	glutMainLoop();
    
	return 0;
}