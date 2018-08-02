#include "Angel.h"
#include <GLFW/glfw3.h>
#include"vec.h"

const int NumTimesToSubdivide = 5;
const int NumTriangle = 729;
const int NumVertices = 3 * NumTriangle;
vec2 points[NumVertices];
int Index = 0;


//----------------------------------------------------------------------------

void
triangle(const vec2&a, const vec2&b, const vec2&c) {
	points[Index++] = a;
	points[Index++] = b;
	points[Index++] = c;
}

void
divide_triangle(const vec2&a, const vec2&b, const vec2&c, int count)
{
	if (count > 0) {
		//计算三角形每边的中点
		vec2 v0 = (a + b) / 2.0;
		vec2 v1 = (a + c) / 2.0;
		vec2 v2 = (b + c) / 2.0;

		divide_triangle(a, v0, v1, count - 1);
		divide_triangle(c, v1, v2, count - 1);
		divide_triangle(b, v2, v0, count - 1);

	}
	else {
		triangle(a, b, c);
	}
}


void
init(void)
{
	

	// Specifiy the vertices for a triangle
	vec2 vertices[3] = {
		vec2(-1.0, -1.0), vec2(0.0, 1.0), vec2(1.0, -1.0)
	};

	divide_triangle(vertices[0], vertices[1], vertices[2], NumTimesToSubdivide);
	

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader21.glsl", "fshader21.glsl");
	glUseProgram(program);

	// Initialize the vertex position attribute from the vertex shader
	GLuint loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	
}

//----------------------------------------------------------------------------

void
display(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0); // white background
	glClear(GL_COLOR_BUFFER_BIT);     // clear the window
	glDrawArrays(GL_TRIANGLES, 0, NumTriangle);    // draw the points
	glFlush();
}


int main(int argc,char **argv) {
	
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(920, 780, "Hello World", NULL, NULL);
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


