#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <stdio.h>
#include "Model.h"

/*void showMesh(objl::Mesh m) {
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < m.Indices.size(); i++) {
		glColor3f(i % 2, (float)(i % 3) / 2, (float)(i % 4) / 3);
		glVertex3fv()
	}
	glEnd();
}*/

int main() {
	if (!glfwInit())
	{
		const char* error;
		glfwGetError(&error);
		printf("GLFW failed to intialize: %s", error);
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(640, 480, "Catelia", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

	ModelManager m;
	TextureManager t;

	shared_ptr<Model> sphere = m.getModel("animegirl");

	//objl::Loader l = objl::Loader();

	//l.LoadFile("./testsphere.obj");

	glClearColor(0.2, 0.2, 0.2, 0.2);
	glViewport(0, 0, 640, 480);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(60, (float)(640.0 / 480.0), 2, 1000);
	glPushMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float x = 0;
	float y = 0;
	float z = 0;
	float xRot = 0;
	float yRot = 0;
	float zRot = 0;
	float xScale = 1;
	float yScale = 1;
	float zScale = 1;
	
	while (true) {
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			xScale -= 0.01;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			xScale += 0.01;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			zScale += 0.01;
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			zScale -= 0.01;
		}
		if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
			yScale += 0.01;
		}
		if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
			yScale -= 0.01;
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glPushMatrix();
		gluLookAt(0, 0, 200, 0, 0, 0, 0, 1, 0);
		//GLfloat* lightdef = new GLfloat[4]{ camX, camY, camZ, 1 };
		/*glLightfv(GL_LIGHT0, GL_POSITION, lightdef);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.01);*/
		//delete[] lightdef;
		sphere->renderX(x, y, z, xRot, yRot, zRot, xScale, yScale, zScale);
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}