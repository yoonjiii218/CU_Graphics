#include <iostream>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "RotatingCube3App.h"


GLuint g_window_w = 800;
GLuint g_window_h = 600;




int main(int argc, char** argv)
{

	//////////////////////////////////////////////////////////////////////////////////////
	//// 1. freeglut �ʱ�ȭ, ������ �����ϱ�. 
	////    Ref: https://en.wikibooks.org/wiki/OpenGL_Programming/Installation/GLUT
	//////////////////////////////////////////////////////////////////////////////////////

	//// 1.1. �ֱ�ȭ
	glutInit(&argc, argv);
	
	//// 1.2. ������ ��� ����. 
	// ������۸�, RGB �����ӹ���, Depth ���۸� ����Ѵ�.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	//// 1.3. ������ ������ ���� (����, ���� �ȼ� ��).
	glutInitWindowSize(g_window_w, g_window_h);

	//// 1.4. OpenGL Context�� ����� ������ ����.
	glutCreateWindow("Computer Graphics");
	
	//// 1.5. Callback functions 
	// Ref: http://freeglut.sourceforge.net/docs/api.php#GlobalCallback
	//      http://freeglut.sourceforge.net/docs/api.php#WindowCallback
	//      https://www.opengl.org/resources/libraries/glut/spec3/node45.html
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc((unsigned int)(1000 /60), Timer, (1000 / 60));
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);



	//////////////////////////////////////////////////////////////////////////////////////
	//// 2. GLEW �ʱ�ȭ. 
	////    Ref: http://glew.sourceforge.net/basic.html
	//////////////////////////////////////////////////////////////////////////////////////
	glewExperimental = GL_TRUE;
	if ( glewInit() != GLEW_OK )
	{
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "GLEW OK\n";
	}



	//////////////////////////////////////////////////////////////////////////////////////
	//// 3. Shader Programs ���
	//// 4. OpenGL ����
	//// 5. VAO, VBO ����
	//////////////////////////////////////////////////////////////////////////////////////
	InitOpenGL();


	//// 1.6. freeglut ������ �̺�Ʈ ó�� ����. �����찡 ���������� ���ѷ��� ����.
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutMainLoop();


	ClearOpenGLResource();

	return 0;
}








