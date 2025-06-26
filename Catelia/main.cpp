#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <stdio.h>

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

	//objl::Loader l = objl::Loader();

	//l.LoadFile("./testsphere.obj");

	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);

	while (true) {
		
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}