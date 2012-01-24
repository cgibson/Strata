/*
 * App.cpp
 *
 *  Created on: Jan 21, 2012
 *      Author: cgibson
 */

#include "Types.h"
#include "STEngine.h"
#include <GL/freeglut.h>

Engine *engine;

void renderFunction()
{
	engine->render_and_update(10);
	glutSwapBuffers();

	printf("RENDERING\n");
	glutMainLoop();
}

int main(int argc, char *argv[])
{
	int width = 500;
	int height = 500;


	glutInit(&argc, argv);

	engine = new STEngine(width, height);
	engine->init();

	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(width,height);
	glutInitWindowPosition(100,100);
	glutCreateWindow("OpenGL - First window demo");
	glutDisplayFunc(renderFunction);
	glutMainLoop();

	printf("Deleting Engine\n");
	delete engine;

	return 0;
}
