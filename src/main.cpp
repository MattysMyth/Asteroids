#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "SceneGraph.h"

#if _WIN32
#include <windows.h>
#endif
#if __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#define GAME_TITLE "Asteroid Arena"
#define KEY_ESC 27

typedef struct
{
	int width, height, x_pos, y_pos;
	bool is_fullscreen;
} window_t;

window_t g_mainwin;

SceneGraph* sceneGraph;

float formerTime;
float arenaRadius;
bool gameStart;

void end_app()
{
	exit(EXIT_SUCCESS);
}

void render_frame()
{
	sceneGraph->render();
}

void render_start_screen()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// Set the projection - use this to render text
	gluOrtho2D(0, g_mainwin.width, 0, g_mainwin.height);
	glMatrixMode(GL_MODELVIEW);

	int x = g_mainwin.width / 3;
	int y = g_mainwin.height / 2;
	std::string string = "Press any key to start...except ESC, that will end things real quick";

	glRasterPos2f(x, y);
	int len = string.length();
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}

	glMatrixMode(GL_PROJECTION);

	// Restore the original projection matrix for rendering everything else
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
}

void on_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if (gameStart)
	{
		if (sceneGraph != nullptr)
		{
			render_frame();
		}
	}
	else
	{
		render_start_screen();
	}

	glutSwapBuffers();
}

void update_game_state()
{
	if (gameStart)
	{
		if (formerTime == 0)
		{
			formerTime = glutGet(GLUT_ELAPSED_TIME);
		}
		float timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
		float deltaTime = (timeSinceStart - formerTime) / 1000.0;
		formerTime = timeSinceStart;
		sceneGraph->update(deltaTime);
	}
}

void on_idle()
{
	if (gameStart)
	{
		if (sceneGraph == nullptr)
		{
			sceneGraph = new SceneGraph(arenaRadius);
		}
		else
		{
			update_game_state();
		}
	}
	glutPostRedisplay();
}

void on_key_press(unsigned char key, int x, int y)
{
	if (!gameStart)
	{
		gameStart = true;
	}
	switch (key)
	{
	case KEY_ESC:
		end_app();
		break;
	default:
		if (sceneGraph != nullptr)
		{
			sceneGraph->keyPress(key);
		}
		break;
	}
}

void on_special_key_press(int key, int x, int y)
{
}

void on_key_release(unsigned char key, int x, int y)
{
	if (sceneGraph != nullptr)
	{
		sceneGraph->keyRelease(key);
	}
}

void on_special_key_release(int key, int x, int y)
{
}

void on_mouse_button(int button, int state, int x, int y)
{
	if (sceneGraph != nullptr)
	{
		sceneGraph->mouseClick(button, state);
	}
}

void on_mouse_move(int x, int y)
{
}

void on_mouse_drag(int x, int y)
{
}

void on_reshape(int w, int h)
{
	g_mainwin.width = w;
	g_mainwin.height = h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
	{
		float aspect = (float)h / (float)w;
		// Sets world coords
		glOrtho(arenaRadius * -1.0, arenaRadius, arenaRadius * -1.0 * aspect, arenaRadius * aspect, -1.0, 1.0);
	}
	else
	{
		float aspect = (float)w / (float)h;
		// Sets world coords
		glOrtho(arenaRadius * -1.0 * aspect, arenaRadius * aspect, arenaRadius * -1.0, arenaRadius, -1.0, 1.0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init_app(int* argcp, char** argv, window_t* mainwinp)
{
	// GLUT & OpenGL setup
	glutInit(argcp, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	// Display related setup
	glutInitWindowPosition(mainwinp->x_pos, mainwinp->y_pos);
	glutInitWindowSize(mainwinp->width, mainwinp->height);
	glutCreateWindow(GAME_TITLE);

	if (mainwinp->is_fullscreen == true)
	{
		glutFullScreen();
	}

	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);

	// Keyboard and Mouse related setup
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutKeyboardFunc(on_key_press);
	glutSpecialFunc(on_special_key_press);
	glutKeyboardUpFunc(on_key_release);
	glutSpecialUpFunc(on_special_key_release);
	glutMouseFunc(on_mouse_button);
	glutPassiveMotionFunc(on_mouse_move);
	glutMotionFunc(on_mouse_drag);

	// Define the idle function
	glutIdleFunc(on_idle);

	formerTime = 0;
}

void run_app()
{
	glutMainLoop();
}

void load_config(int* argcp, char** argv, window_t* mainwin_p)
{
	mainwin_p->width = 1024;
	mainwin_p->height = 768;
	mainwin_p->is_fullscreen = true;
}

int main(int argc, char** argv)
{
	gameStart = false;
	sceneGraph = nullptr;
	arenaRadius = 15.0f;
	load_config(&argc, argv, &g_mainwin);
	init_app(&argc, argv, &g_mainwin);
	run_app();
	return (EXIT_SUCCESS);
}
