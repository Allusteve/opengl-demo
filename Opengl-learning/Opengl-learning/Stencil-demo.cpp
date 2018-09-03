#include <iostream>

#include "vgl.h"
#include "LoadShaders.h"
#include "vmath.h"
#include "vapp.h"
#include "vutils.h"
#include "vbm.h"

 BEGIN_APP_DECLARATION(Stencil_demo)

 // Override functions from base class
 virtual void Initialize(const char * title);
 virtual void Display(bool auto_redraw);
 virtual void Finalize(void);
 virtual void Resize(int width, int height);

 //成员变量
 struct Vertex
 {
	 GLubyte color[4];
	 GLfloat position[2];
 };

 Vertex Verts[3] = { { { 255,0,0,255 },{ 0.0f, 0.9f } },
 { { 0,255,0,255 },{ -0.9f, -0.9f } },
 { { 0,0,255,255 },{ 0.9f, -0.9f } } };

 float VertsNone[3][2] = { { 0.0f, 0.4f },
 { -0.4f, -0.4f } ,
 { 0.4f, -0.4f } };

 GLuint Program = 0;
 GLuint vert;
 GLuint vbo;
 GLuint vertNone;
 GLuint vboNone;
 float aspect;


 END_APP_DECLARATION()

 DEFINE_APP(Stencil_demo, "Stencil_demo")

void Stencil_demo::Initialize(const char * title)
 {

	 base::Initialize(title);         //the key
	 ShaderInfo si[] = {
		 { GL_VERTEX_SHADER, "Chpater04.vert" },
	     { GL_FRAGMENT_SHADER, "Chapter04.frag" },
	     { GL_NONE, NULL } };
	 Program = LoadShaders(si);

	 glGenVertexArrays(1, &vert);
	 glBindVertexArray(vert);
	 glGenBuffers(1, &vbo);
	 glBindBuffer(GL_ARRAY_BUFFER, vbo);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(Verts), Verts, GL_STATIC_DRAW);
	 glVertexAttribPointer(0, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), BUFFER_OFFSET(0));
	 glEnableVertexAttribArray(0);
	 glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(Verts->color)));
	 glEnableVertexAttribArray(1);



	 glGenVertexArrays(1, &vertNone);
	 glBindVertexArray(vertNone);
	 glGenBuffers(1, &vboNone);
	 glBindBuffer(GL_ARRAY_BUFFER, vboNone);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(VertsNone), VertsNone, GL_STATIC_DRAW);
	 glDisableVertexAttribArray(0);
	 glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	 glEnableVertexAttribArray(1);


	 glUseProgram(Program);

	 glClearColor(0.0, 0.0, 0.0, 1.0);//设置背景颜色
	 glClearStencil(0x0);//设置模板初始值
 }


 void Stencil_demo::Display(bool autodrew)
 {
	 glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);//绘制背景颜色，并将模板值清除设为 0x0
														  //glEnable(GL_SCISSOR_TEST);
														  //glScissor(0, 0, 300, 300);
	 glEnable(GL_STENCIL_TEST);

	 //绘制镂空区域
	 glBindVertexArray(vertNone);
	 //设置模板测试总是通过
	 glStencilFunc(GL_ALWAYS, 0x1, 0x1);
	 //设置模板值总是替换为 0x1
	 glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	 //禁止绘制颜色，从而不会绘制出图形，只会更新模板值
	 glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);       //控制RGBA四个通道是否显示
	 glDrawArrays(GL_TRIANGLES, 0, 3);
	 glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	 //glClear(GL_COLOR_BUFFER_BIT);

	 //绘制彩色三角形
	 glBindVertexArray(vert);
	 //设置模板值不等于 0x1时才通过模板测试，这样镂空三角形区域内像素不会通过模板测试
	 glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
	 //设置模板值不更新
	 glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	 glDrawArrays(GL_TRIANGLES, 0, 3);

	
	 Sleep(10);
	 base::Display();
 }

 void Stencil_demo::Resize(int width, int height)
 {
	 glViewport(0, 0, width, height);

	 aspect = float(height) / float(width);
 }

 void Stencil_demo::Finalize(void)             //删除着色器
 {
	 glUseProgram(0);
	 glDeleteProgram(Program);
	 glDeleteVertexArrays(1, &vert);
	 glDeleteBuffers(1, &vbo);
 }