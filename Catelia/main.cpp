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

	ModelManager m;
	TextureManager t;

	shared_ptr<Model> sphere = m.getModel("animegirl");

	//objl::Loader l = objl::Loader();

	//l.LoadFile("./testsphere.obj");

	glClearColor(0.2, 0.2, 0.2, 0.2);
	glViewport(0, 0, 640, 480);
	gluPerspective(60, (float)(640.0 / 480.0), 2, 500);
	gluLookAt(0, 0, -200, 0, 0, 0, 0, 1, 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_TEXTURE_2D);
	
	while (true) {
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		sphere.get()->render();
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}