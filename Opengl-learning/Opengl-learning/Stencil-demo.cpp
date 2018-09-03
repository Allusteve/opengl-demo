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

 //��Ա����
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

	 glClearColor(0.0, 0.0, 0.0, 1.0);//���ñ�����ɫ
	 glClearStencil(0x0);//����ģ���ʼֵ
 }


 void Stencil_demo::Display(bool autodrew)
 {
	 glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);//���Ʊ�����ɫ������ģ��ֵ�����Ϊ 0x0
														  //glEnable(GL_SCISSOR_TEST);
														  //glScissor(0, 0, 300, 300);
	 glEnable(GL_STENCIL_TEST);

	 //�����ο�����
	 glBindVertexArray(vertNone);
	 //����ģ���������ͨ��
	 glStencilFunc(GL_ALWAYS, 0x1, 0x1);
	 //����ģ��ֵ�����滻Ϊ 0x1
	 glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	 //��ֹ������ɫ���Ӷ�������Ƴ�ͼ�Σ�ֻ�����ģ��ֵ
	 glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);       //����RGBA�ĸ�ͨ���Ƿ���ʾ
	 glDrawArrays(GL_TRIANGLES, 0, 3);
	 glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	 //glClear(GL_COLOR_BUFFER_BIT);

	 //���Ʋ�ɫ������
	 glBindVertexArray(vert);
	 //����ģ��ֵ������ 0x1ʱ��ͨ��ģ����ԣ������ο����������������ز���ͨ��ģ�����
	 glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
	 //����ģ��ֵ������
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

 void Stencil_demo::Finalize(void)             //ɾ����ɫ��
 {
	 glUseProgram(0);
	 glDeleteProgram(Program);
	 glDeleteVertexArrays(1, &vert);
	 glDeleteBuffers(1, &vbo);
 }