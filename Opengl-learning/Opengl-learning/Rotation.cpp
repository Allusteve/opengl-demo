#include<Angel.h>
#include<GLFW\glfw3.h>


typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

const int NumVertices = 36;

point4 points[NumVertices];
color4 colors[NumVertices];

//中心位于坐标原点，边与坐标轴平行的单位立方体的顶点
point4 vertices[8] = {

	point4(-0.5, -0.5, 0.5, 1.0),
	point4(-0.5,0.5,0.5,1.0),
	point4(0.5,0.5,0.5,1.0),
	point4(0.5,-0.5,0.5,1.0),
	point4(-0.5,-0.5,-0.5,1.0),
	point4(-0.5,0.5,-0.5,1.0),
	point4(0.5,0.,5 - 0.5,1.0),
	point4(0.5,-0.5,-0.5,1.0)
};

//RGBA颜色

color4 vertex_colors[8] = {

	color4(0.0,0.0,0.0,1.0),  //黑色
	color4(1.0,0.0,0.0,1.0),  //红色
	color4(1.0,1.0,0.0,1.0),  //黄色
	color4(0.0,1.0,0.0,1.0),  //绿色
	color4(0.0,0.0,1.0,1.0),  //蓝色
	color4(1.0,0.0,1.0,1.0),  //品红色
	color4(1.0,1.0,1.0,1.0),  //白色
	color4(0.0,1.0,1.0,1.0)   //青色
};

//相对于每个坐标轴的旋转角度
enum{ Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3};

int Axis = Xaxis;
GLfloat Theta[NumAxes] = { 0.0,0.0,0.0 };

GLuint theta;  //顶点着色器中uniform变量"theta"的存储位置

//立方体的每个面生成两个三角形， 并且将颜色赋给每个顶点
int Index = 0;

void quad(int a, int b, int c, int d)
{
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;

}


//生成12个三角形: 36个顶点（每个顶点赋一种颜色）
void
colorcube(void)
{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}


void init(void)
{
	colorcube();

	GLuint program = InitShader("D:\\Shader\\vshader36.glsl", "D:\\Shader\\fshader36.glsl");
	glUseProgram(program);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);  

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,BUFFER_OFFSET(sizeof(points)));

	theta = glGetUniformLocation(program, "theta");
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);

}


void
display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUniform3fv(theta, 1, Theta);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);


}

//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, GLFW_TRUE);
//}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		Axis = Xaxis;
		Theta[Axis] += 1;
		if (Theta[Axis] > 360.0) {
			Theta[Axis] -= 360.0;
		}
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		Axis = Yaxis;
		Theta[Axis] += 1;
		if (Theta[Axis] > 360.0) {
			Theta[Axis] -= 360.0;
		}
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		Axis = Zaxis;
		Theta[Axis] += 1;
		if (Theta[Axis] > 360.0) {
			Theta[Axis] -= 360.0;
		}
		break;
	default:
		return;
	}
	return;
}


int main(int argc, char **argv) {

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;
	
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(820, 680, "Hello World", NULL, NULL);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);   // 主版本号
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
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
		//reshape(780, 680);


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;

}