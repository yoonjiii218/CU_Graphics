


#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "InitShader.h"
#include "Camera.h"
#include "CubeObj.h"
#include "GroundObj.h"
#include "RotatingCube3App.h"


// Window and User Interface
static bool g_left_button_pushed;
static bool g_right_button_pushed;
static int g_last_mouse_x;
static int g_last_mouse_y;

// Window 
extern GLuint g_window_w;
extern GLuint g_window_h;


//////////////////////////////////////////////////////////////////////
// Camera 
//////////////////////////////////////////////////////////////////////
static Camera g_camera;

//////////////////////////////////////////////////////////////////////
//// Define Shader Programs
//////////////////////////////////////////////////////////////////////
GLuint s_program_id;



//////////////////////////////////////////////////////////////////////
//// Animation Parameters
//////////////////////////////////////////////////////////////////////
float g_elaped_time_s = 0.f;	// 




/**
InitOpenGL: ���α׷� �ʱ� �� ������ ���� ���� �� �� ȣ��Ǵ� �Լ�. (main �Լ� ����)
OpenGL�� ������ �ʱ� ���� ���α׷��� �ʿ��� �ٸ� �ʱ� ���� �����Ѵ�.
�������, VAO�� VBO�� ���⼭ ������ �� �ִ�.
*/
void InitOpenGL()
{
	//////////////////////////////////////////////////////////////////////////////////////
	//// 3. Shader Programs ���
	////    Ref: https://www.khronos.org/opengl/wiki/Shader_Compilation
	//////////////////////////////////////////////////////////////////////////////////////
	s_program_id = CreateFromFiles("../RotatingCube3/v_shader.glsl", "../RotatingCube3/f_shader.glsl");
	glUseProgram(s_program_id);



	////////////////////////////////////////////////////////////////////////////////////
	//// 4. OpenGL ����
	//////////////////////////////////////////////////////////////////////////////////////
	glViewport(0, 0, (GLsizei)g_window_w, (GLsizei)g_window_h);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


	// Initial State of Camera
	// ī�޶� �ʱ� ��ġ �����Ѵ�.
	g_camera.lookAt(glm::vec3(3.f, 2.f, 3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));


	////////////////////////////////////////////////////////////////////////////////////
	//// 5. VAO, VBO ����
	////    Ref: https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Array_Object
	///////////////////////////////////////////////////////////////////////////////////

	// Cube VAO ����
	InitCube();

	// �ٴ� ���� VAO ����
	InitGround();

}







/**
ClearOpenGLResource: ���α׷��� ������ �޸� ������ ���� �� �� ȣ��Ǵ� �Լ�. (main �Լ� ����)
���α׷����� ����� �޸𸮸� ���⿡�� ������ �� �ִ�.
�������, VAO�� VBO�� ���⼭ ���� �� �ִ�.
*/
void ClearOpenGLResource()
{
	// Delete (VAO, VBO)
	DeleteCube();
	DeleteGround();
}





/**
Display: ������ ȭ���� ������Ʈ �� �ʿ䰡 ���� �� ȣ��Ǵ� callback �Լ�.

������ �� ���� ����� ������ �ϴ� �ڵ�� �� �Լ� ���� �����ؾ��Ѵ�.
�����찡 ó�� ���� ��, ������ ũ�Ⱑ �ٲ� ��, �ٸ� �����쿡 ���� ȭ���� �Ϻ�
�Ǵ� ��ü�� �������ٰ� �ٽ� ��Ÿ�� �� �� �ý����� �ش� ������ ���� �׸��� ����
������Ʈ�� �ʿ��ϴٰ� �Ǵ��ϴ� ��� �ڵ����� ȣ��ȴ�.
���� ȣ���� �ʿ��� ��쿡�� glutPostRedisplay() �Լ��� ȣ���ϸ�ȴ�.

�� �Լ��� �ҽÿ� ����ϰ� ȣ��ȴٴ� ���� ����ϰ�, ������ ���� ��ȭ�� ������
1ȸ�� �ڵ�� �������� �� �Լ� �ۿ� �����ؾ��Ѵ�. Ư�� �޸� �Ҵ�, VAO, VBO ����
���� �ϵ���� ������ �õ��ϴ� �ڵ�� Ư���� ������ ���ٸ� ���� �� �Լ��� ���Խ�Ű��
�ȵȴ�. ���� ���, �޽� ���� �����ϰ� VAO, VBO�� �����ϴ� �κ��� ���� 1ȸ��
�����ϸ�ǹǷ� main() �Լ� �� �ܺο� �����ؾ��Ѵ�. ���ǵ� �޽� ���� ������ ���ۿ�
�׸����� �����ϴ� �ڵ常 �� �Լ��� �����ϸ� �ȴ�.

����, �� �Լ� ������ ���� �޸� �Ҵ��� �ؾ��ϴ� ��찡 �ִٸ� �ش� �޸𸮴� �ݵ��
�� �Լ��� ������ ���� ���� �ؾ��Ѵ�.

ref: https://www.opengl.org/resources/libraries/glut/spec3/node46.html#SECTION00081000000000000000
*/
void Display()
{
	// ��ü ȭ���� �����.
	// glClear�� Display �Լ� ���� �� �κп��� �� ���� ȣ��Ǿ���Ѵ�.
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Vertex shader �� matrix �������� location�� �޾ƿ´�.
	int m_proj_loc = glGetUniformLocation(s_program_id, "proj_matrix");
	int m_view_loc = glGetUniformLocation(s_program_id, "view_matrix");
	int m_model_loc = glGetUniformLocation(s_program_id, "model_matrix");


	// Projection Transform Matrix ����.
	glm::mat4 projection_matrix = glm::perspective(glm::radians(45.f), (float)g_window_w/g_window_h, 0.01f, 10000.f);
	glUniformMatrix4fv(m_proj_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	// Camera Transform Matrix ����.
	//glm::mat4 view_matrix = glm::lookAt(glm::vec3(0.f, 2.f, 3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	//glUniformMatrix4fv(m_view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(m_view_loc, 1, GL_FALSE, glm::value_ptr(g_camera.GetGLViewMatrix()));


	// �ٴ� ����
	glm::mat4 T0(1.f); // ���� ���
	glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, glm::value_ptr(T0));
	
	DrawGround();


	//// �¾�
	float ss = 0.2f * sin(g_elaped_time_s) + 1.5f;
	glm::mat4 scale_s = glm::scale(glm::vec3(1.f * ss, 1.f * ss, 1.f * ss));
	glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, glm::value_ptr(scale_s));

	DrawCube();


	//// ����
	glm::mat4 trans_x = glm::translate(glm::vec3(2.f, 0.5f, 0.f));
	glm::mat4 scale_half = glm::scale(glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 R1 = glm::rotate(glm::radians(360.f * (g_elaped_time_s / 2.f)), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 T1 = glm::rotate(glm::radians(360.f * (g_elaped_time_s / 20.f)), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 T_E = T1 * trans_x * R1 * scale_half;
	glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, glm::value_ptr(T_E));

	DrawCube();


	//// ��
	glm::mat4 scale_m = glm::scale(glm::vec3(0.3f, 0.3f, 0.3f));
	glm::mat4 trans_m = glm::translate(glm::vec3(0.5f, 0.5f, 0.f));
	glm::mat4 T2 = glm::translate(glm::vec3(2.f, 0.f, 0.f));
	glm::mat4 T_M = T1 * T2 * R1 * trans_m * scale_m;	
	glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, glm::value_ptr(T_M));

	DrawCube();


	//// ���༺��
	int k = 0;
	for (int i = 0; i < 72; i++) {
		glm::mat4 trans_a = glm::translate(glm::vec3(5.f, 0.f, 0.f));
		glm::mat4 scale_a = glm::scale(glm::vec3(0.4f, 0.4f, 0.4f));
		glm::mat4 R2 = glm::rotate(glm::radians((360.f * (g_elaped_time_s / 10.f)) + k), glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 T_A = R2 * trans_a * scale_a;
		glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, glm::value_ptr(T_A));

		DrawCube();
		k += 5;
	}


	// flipping the float buffers
	// glutSwapBuffers�� �׻� Display �Լ� ���� �Ʒ� �κп��� �� ���� ȣ��Ǿ���Ѵ�.
	glutSwapBuffers();
}



/**
Timer: ������ �ð� �Ŀ� �ڵ����� ȣ��Ǵ� callback �Լ�.
ref: https://www.opengl.org/resources/libraries/glut/spec3/node64.html#SECTION000819000000000000000
*/
void Timer(int value)
{
	// Timer ȣ�� �ð� ������ �����Ͽ�, ���� Timer�� ȣ��� �ĺ��� ������� �귯�� ����Ѵ�.
	g_elaped_time_s += value/1000.f;

	// glutPostRedisplay�� ������ ���� �ð� �ȿ� ��ü �׸��� �ٽ� �׸� ���� �ý��ۿ� ��û�Ѵ�.
	// ��������� Display() �Լ��� ȣ�� �ȴ�.
	glutPostRedisplay();

	// 1/60 �� �Ŀ� Timer �Լ��� �ٽ� ȣ��Ƿη� �Ѵ�.
	// Timer �Լ� �� ������ �ð� �������� �ݺ� ȣ��ǰ��Ͽ�,
	// �ִϸ��̼� ȿ���� ǥ���� �� �ִ�
	glutTimerFunc((unsigned int)(1000 / 60), Timer, (1000 / 60));
}



/**
Reshape: �������� ũ�Ⱑ ������ ������ �ڵ����� ȣ��Ǵ� callback �Լ�.

@param w, h�� ���� ������ �������� ���� ũ��� ���� ũ�� (�ȼ� ����).
ref: https://www.opengl.org/resources/libraries/glut/spec3/node48.html#SECTION00083000000000000000
*/
void Reshape(int w, int h)
{
	//  w : window width   h : window height
	g_window_w = w;
	g_window_h = h;

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glutPostRedisplay();
}

/**
Keyboard: Ű���� �Է��� ���� ������ �ڵ����� ȣ��Ǵ� �Լ�.
@param key�� ������ Ű������ ���ڰ�.
@param x,y�� ���� ���콺 �������� ��ǥ��.
ref: https://www.opengl.org/resources/libraries/glut/spec3/node49.html#SECTION00084000000000000000

*/
void Keyboard(unsigned char key, int x, int y)
{
	// keyboard '1' �� �������� ��.
	if (key == '1')
	{
		// Fragment shader�� ���� �Ǿ��ִ� 'shading_mode' ������ location�� �޾ƿ´�.
		int shading_mode_loc = glGetUniformLocation(s_program_id, "shading_mode");

		// 'shading_mode' ������ 1�� ����.
		glUniform1i(shading_mode_loc, 1);


		// glutPostRedisplay�� ������ ���� �ð� �ȿ� ��ü �׸��� �ٽ� �׸� ���� �ý��ۿ� ��û�Ѵ�.
		// ��������� Display() �Լ��� ȣ���ϰ� �ȴ�.
		glutPostRedisplay();
	}

	// keyboard '2' �� �������� ��.
	else if (key == '2')
	{
		// Fragment shader�� ���� �Ǿ��ִ� 'shading_mode' ������ location�� �޾ƿ´�.
		int shading_mode_loc = glGetUniformLocation(s_program_id, "shading_mode");

		// 'shading_mode' ������ 2�� ����.
		glUniform1i(shading_mode_loc, 2);


		// glutPostRedisplay�� ������ ���� �ð� �ȿ� ��ü �׸��� �ٽ� �׸� ���� �ý��ۿ� ��û�Ѵ�.
		// ��������� Display() �Լ��� ȣ���ϰ� �ȴ�.
		glutPostRedisplay();
	}
}





/**
Mouse: ���콺 ��ư�� �Էµ� ������ �ڵ����� ȣ��Ǵ� �Լ�.
�Ķ������ �ǹ̴� ������ ����.
@param button: ���� ��ư�� ����
  GLUT_LEFT_BUTTON - ���� ��ư
  GLUT_RIGHT_BUTTON - ������ ��ư
  GLUT_MIDDLE_BUTTON - ��� ��ư (���� �������� ��)
  3 - ���콺 �� (���� ���� ���� ����).
  4 - ���콺 �� (���� �Ʒ��� ���� ����).
@param state: ���� ����
  GLUT_DOWN - ���� ����
  GLUT_UP - ��������
@param x,y: ������ �Ͼ�� ��, ���콺 �������� ��ǥ��.
*/
void Mouse(int button, int state, int x, int y)
{
	float mouse_xd = (float)x / g_window_w;
	float mouse_yd = 1 - (float)y / g_window_h;
	float last_mouse_xd = (float)g_last_mouse_x / g_window_w;
	float last_mouse_yd = 1 - (float)g_last_mouse_y / g_window_h;


	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		g_left_button_pushed = true;

	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		g_left_button_pushed = false;

	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		g_right_button_pushed = true;

	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
		g_right_button_pushed = false;
	else if (button == 3)
	{
		g_camera.inputMouse(Camera::IN_TRANS_Z, 0, -1, 0.01f);
		glutPostRedisplay();
	}
	else if (button == 4)
	{
		g_camera.inputMouse(Camera::IN_TRANS_Z, 0, 1, 0.01f);
		glutPostRedisplay();
	}

	g_last_mouse_x = x;
	g_last_mouse_y = y;
}





/**
MouseMotion: ���콺 �����Ͱ� ������ ������ �ڵ����� ȣ��Ǵ� �Լ�.
@prarm x,y�� ���� ���콺 �������� ��ǥ���� ��Ÿ����.
*/
void MouseMotion(int x, int y)
{
	float mouse_xd = (float)x / g_window_w;
	float mouse_yd = 1 - (float)y / g_window_h;
	float last_mouse_xd = (float)g_last_mouse_x / g_window_w;
	float last_mouse_yd = 1 - (float)g_last_mouse_y / g_window_h;

	if (g_left_button_pushed)
	{
		g_camera.inputMouse(Camera::IN_ROTATION_Y_UP, last_mouse_xd, last_mouse_yd, mouse_xd, mouse_yd);
		glutPostRedisplay();
	}
	else if (g_right_button_pushed)
	{
		g_camera.inputMouse(Camera::IN_TRANS, last_mouse_xd, last_mouse_yd, mouse_xd, mouse_yd, 0.01f);
		glutPostRedisplay();
	}

	g_last_mouse_x = x;
	g_last_mouse_y = y;
}
