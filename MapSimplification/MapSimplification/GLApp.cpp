#include "GLApp.h"

// APP Constants

// Library version number.
int GLApp::LIBRARY_VERSION = 4;
	
// Minimum camera zoom
float GLApp::MIN_CAMERA_ZOOM = 1;

// Distance multiplier per mouse wheel tick.
float GLApp::MOUSE_WHEEL_FACTOR = 1.2;
	
// Minimum value of vWidth.
float GLApp::VWIDTH_MIN = 1;
	
// Maximum value of vWidth.
float GLApp::VWIDTH_MAX = 1000;
	
// Desired frames per second.
int GLApp::FPS = 60;

namespace
{
	// Global app
	GLApp* g_pApp = NULL;
}


// Callback functions
void RenderCallback()
{
	if(g_pApp)
		return g_pApp->Render();
}

void ReshapeCallback(int width, int height)
{
	if(g_pApp)
		return g_pApp->Reshape(width, height);
}

void HandleKeyboardInputCallback(unsigned char key, int x, int y)
{
	if(g_pApp)
		return g_pApp->HandleKeyboardInput(key, x, y);
}

void HandleMouseInputCallback(int button, int state, int x, int y)
{
	if(g_pApp)
		return g_pApp->HandleMouseInput(button, state, x, y);
}

void HandleMouseMotionInputCallback(int x, int y)
{
	if(g_pApp)
		return g_pApp->HandleMouseMotionInput(x, y);
}

void HandleMouseWheelInputCallback(int button, int dir, int x, int y)
{
	if(g_pApp)
		return g_pApp->HandleMouseWheelInput(button, dir, x, y);
}

void HandleItemMenuInputCallback(int menuItemNumber)
{
	if(g_pApp)
		return g_pApp->HandleItemMenuInput(menuItemNumber);
}


void IdleCallback()
{
	if(g_pApp)
		return g_pApp->Idle();
}


GLApp::GLApp(void)
{
	m_FPS = 0.0f;


	m_width = 800;
	m_height = 600;

	g_pApp = this;
}

GLApp::~GLApp(void)
{
}

// Main application loop
void GLApp::Run()
{
	glutMainLoop();
}

bool GLApp::InitGL()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(m_width, m_height);

	glutCreateWindow("MapSimplification");

	// Callback functions
	glutKeyboardFunc(&HandleKeyboardInputCallback);
	glutMouseFunc(&HandleMouseInputCallback);
	glutMotionFunc(&HandleMouseMotionInputCallback);
	glutMouseWheelFunc(&HandleMouseWheelInputCallback);
	glutDisplayFunc(&RenderCallback);
	glutReshapeFunc(&ReshapeCallback);
	glutIdleFunc(&IdleCallback);

	// Initialize glew
	if(glewInit())
		return OutErrorMsg("Failed to initialize glew");

	return true;
}

bool GLApp::Init()
{
	if(!InitGL())
		return false;

//	if(!SetupShadowMapping())
//		return false;

	// Calculate timing
	m_prevTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&m_prevTime);

	m_countsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_countsPerSec);
	m_secondsPerCount = 1.0f / m_countsPerSec;

	InitMenu();

	return true;
}

void GLApp::HandleMouseInput(int button, int state, int x, int y)
{
	// Open menu when the mouse was not dragged during a RightMouseClick
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP && x == beginDragX && y == beginDragY)
	{
		std::cout << "Opening context menu" << std::endl;
		// Attach menu to the Middle mouse button
		glutAttachMenu(GLUT_MIDDLE_BUTTON);

		// HACK: Simulate a Middle Mouse Button cick to open the contextmenu 
		// There is no proper way to open a contextmenu without attaching it to a Mouse Button
		HWND glutwindow = GetActiveWindow();
		PostMessage(glutwindow, WM_MBUTTONDOWN, MK_MBUTTON, MAKELPARAM(x, y));
		PostMessage(glutwindow, WM_MBUTTONUP, MK_MBUTTON, MAKELPARAM(x, y));

		mouseButton = -1;
	}
	else
	{
		// Detach menu from the Middle Mouse Button
		glutDetachMenu(GLUT_MIDDLE_BUTTON);

		dragSourceX = x;
		dragSourceY = y;

		beginDragX = x;
		beginDragY = y;

		mouseButton = button;
	}
}

/*
* Mouse drag
*/
void GLApp::HandleMouseMotionInput(int x, int y)
{
	//std::cout << "mouseinput (x:" << x << ", y: " << y <<")\n";

	float dX = x - dragSourceX;
	float dY = y - dragSourceY;
		
	// TODO: handle mouse input

		
	dragSourceX = x;
	dragSourceY = y;
}

void GLApp::HandleMouseWheelInput(int button, int dir, int x, int y)
{
	//std::cout << "mouseWheelinput (x:" << x << ", y: " << y <<")\n";
	m_zoom = (float) max(MIN_CAMERA_ZOOM,
							m_zoom *
							pow(MOUSE_WHEEL_FACTOR, -dir));
}

void GLApp::HandleKeyboardInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':	// Right
		break;
	default:
		break;
	
	
	}
}

// Glut Idle func
void GLApp::Idle()
{
	glutPostRedisplay();
}

// Glut Reshape func
void GLApp::Reshape(int width, int height)
{
	m_width = width;
	m_height = height;

	SetView();
}

// Create context menu
void GLApp::InitMenu()
{
	// Dataset selection submenu
	int datasetmenu = glutCreateMenu(HandleItemMenuInputCallback);
	glutAddMenuEntry("training data 1", 40);
	glutAddMenuEntry("training data 2", 41);
	glutAddMenuEntry("training data 3", 42);
	glutAddMenuEntry("training data 4", 43);
	glutAddMenuEntry("training data 5", 44);
	
	// Main menu
	m_menuId = glutCreateMenu(HandleItemMenuInputCallback);
	glutAddSubMenu("Dataset", datasetmenu);
	glutAddMenuEntry("Reset", 0);
}


void GLApp::HandleItemMenuInput(int menuItemNumber)
{
	switch (menuItemNumber)
	{
	case 0:	// Reset
		
		break;

	// Dataset selection
	case 40:	// Training data 1
		// TODO: switch dataset
		break;
	case 41:	// Training data 2
		// TODO: switch dataset
		break;
	case 42:	// Training data 3
		// TODO: switch dataset
		break;
	case 43:	// Training data 4
		// TODO: switch dataset
		break;
	case 44:	// Training data 5
		// TODO: switch dataset
		break;

	default:
		break;
	}
}

void GLApp::Render()
{
	// Update wall time, and reset if required.

	// Calculate delta time
	__int64 curTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
	float deltaTime = (curTime - m_prevTime) * m_secondsPerCount;

	CalculateFPS(deltaTime);

	// Reset prev time
	m_prevTime = curTime;

	SetView();
	DrawScene();

	DrawGUI();

	glutSwapBuffers();
	
	// Report OpenGL errors.
	GLenum errorCode = glGetError();

	while(errorCode != GL_NO_ERROR) 
	{
		LPCSTR errorMessage = (LPCSTR)gluErrorString(errorCode); 
		//printf(errorMessage);
		std::cout << errorCode << " " << errorMessage << std::endl;;
		//OutErrorMsg(errorCode + " " + *errorMessage);
		errorCode = glGetError();
	}
}

void GLApp::CalculateFPS(float dt)
{
	static float elapsed = 0;
	static int frameCnt = 0;

	elapsed += dt;
	frameCnt++;

	if(elapsed >= 1.0f)
	{
		m_FPS = (float)frameCnt;
		
		// Output to window title
		std::stringstream ss;
		ss << m_AppTitle << " FPS:" << m_FPS << " " << glGetString(GL_VENDOR) << 
			" " <<  glGetString(GL_RENDERER) << " " << glGetString(GL_VERSION) << 
			"" << glGetString(GL_SHADING_LANGUAGE_VERSION);
		
		glutSetWindowTitle(ss.str().c_str());
		//SetWindowText(m_hAppWnd, ss.str().c_str());

		elapsed = 0.0f;
		frameCnt = 0;
	}
}