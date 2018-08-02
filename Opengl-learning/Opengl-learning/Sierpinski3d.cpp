#include "Angel.h"
#include <GLFW/glfw3.h>
#include"vec.h"

const int NumVertices = 10000;

void
init()
{
	vec3 points[NumVertices];
	
	vec3 vertices[4] = {
		vec3(-1.0,-1.0,-1.0),
		vec3(1.0,-1.0,-1.0),
		vec3(0.0,1.0,-1.0),
		vec3(0.0,0.0,1.0)
	};

	points[0] = vec3(0.0, 0.0, 0.0);

	for (int i = 1; i < NumVertices; i++) {
		int j = rand() % 4;
		points[i] = (vertices[j] + points[i - 1]) / 2.0;
	}

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	GLuint program = InitShader("vshader23.glsl", "fshader23.glsl");
	glUseProgram(program);

	GLuint loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	glClearColor(1.0, 1.0, 1.0, 1.0); // white background
}



void
display(void)
{
	
	glClear(GL_COLOR_BUFFER_BIT);     // clear the window
	glDrawArrays(GL_POINTS, 0, NumVertices);    // draw the points
	glFlush();
}

int main(int argc, char **argv) {

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(520, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glewInit();

	init();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Draw a triangle */
		display();



		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;

}

