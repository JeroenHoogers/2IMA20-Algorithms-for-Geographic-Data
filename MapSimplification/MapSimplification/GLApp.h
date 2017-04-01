#pragma once

#include "GLUtil.h"

class GLApp
{
public:
	static int LIBRARY_VERSION;
	static float MIN_CAMERA_ZOOM;
	static float VWIDTH_MIN;
	static float VWIDTH_MAX;
	static float MOUSE_WHEEL_FACTOR;
	static int FPS;


public:
	GLApp(void);
	GLApp(HINSTANCE hInstance);
	virtual ~GLApp(void);

	// Main application loop
	void Run();

	// Framework methods
	virtual bool Init();
	virtual void Render();
	virtual void Reshape(int width, int height);
	virtual void Idle();

	virtual void SetView() = 0;
	virtual void DrawScene() = 0;
	virtual void DrawGUI() = 0;
	virtual void InitMenu();
	
	// Input
	virtual void HandleKeyboardInput(unsigned char key, int x, int y );
	virtual void HandleMouseInput(int button, int state, int x, int y);
	virtual void HandleMouseWheelInput(int button, int dir, int x, int y);
	virtual void HandleMouseMotionInput(int x, int y);

	virtual void HandleItemMenuInput(int menuItemNumber);

protected:
	UINT		m_ClientWidth;
	UINT		m_ClientHeight;
	char*		m_AppTitle;
	float		m_FPS;

	UINT		m_width;
	UINT		m_height;

	float		m_zoom;
	glm::vec2	m_center;

	bool		m_showAABBs;
	bool		m_showEndpoints;
	bool		m_showControlPoints;
	bool		m_showHelperPoints;

	__int64		m_prevTime;
	__int64		m_countsPerSec;
	float		m_secondsPerCount;

	int			m_menuId;

protected:

	// Initialize OpenGL
	bool InitGL();

	// Calculate frames per second
	void CalculateFPS(float dt);

private:
	
	// Position of mouse drag source.
	int dragSourceX;
	int dragSourceY;

	int beginDragX;
	int beginDragY;

	// Last mouse button pressed.
	int mouseButton;
};

