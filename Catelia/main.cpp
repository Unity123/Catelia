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
	glMatrixMode(GL_PROJECTION);
	gluPerspective(60, (float)(640.0 / 480.0), 2, 1000);
	glPushMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float camX = 0;
	float camY = 0;
	float camZ = 200;
	
	while (true) {
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			camX -= 1;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			camX += 1;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			camZ += 1;
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			camZ -= 1;
		}
		if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
			camY += 1;
		}
		if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
			camY -= 1;
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glPushMatrix();
		gluLookAt(camX, camY, camZ, camX, camY, 0, 0, 1, 0);
		sphere->render();
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}