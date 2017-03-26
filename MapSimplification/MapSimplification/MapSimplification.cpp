#include "MapSimplification.h"

MapSimplification::MapSimplification() : GLApp()
{
	m_AppTitle = "Geometry Simplification";
}

MapSimplification::~MapSimplification()
{
}

bool MapSimplification::Init()
{
	// Initialize GLApp
	if(!GLApp::Init())
		return false;

	// Initialize the camera
	center = glm::vec2(0, 0);
	camPos = glm::vec2(0, 0);

	// TODO: Initialization

	// Enable blending.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Anti-aliasing can be enabled by uncommenting the following 4 lines.
	// This can however cause problems on some graphics cards.
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	// Enable depth testing.
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	//glEnable(GL_CULL_FACE);

	// Normalize normals.2
	glEnable(GL_NORMALIZE);


	Parser parser;

	m_lines = parser.ParseLineFile("Data\\training_data5\\lines_out.txt");
	m_points = parser.ParsePointFile("Data\\training_data5\\points_out.txt");

	return true;
}

/**
* Configures the viewing transform.
*/
void MapSimplification::SetView()
{
	// Select part of window.
	glViewport(0, 0, m_width, m_height);


	// Set projection matrix.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Background color.
	glClearColor(0.6, 0.6, 0.6, 0);


	// Clear background.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw Background
	DrawBackground();

	// Set camera.
	glOrtho(0, m_width, m_height, 0, -1000, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/**
* Draw background gradient
*/
void MapSimplification::DrawBackground()
{
	// Background properties
	//float innerColor[] = {1,1,1};
	//float outerColor[] = {0.6, 0.6, 0.6};
	//int slices = 50;
	//float radius = 1.5;
	//float incr = (float) (2 * M_PI / slices);

	//// Switch to 2D mode
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_LIGHTING);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	//// Draw background gradient
	//glBegin(GL_QUADS);

	//	//glColor3fv(innerColor);
	//	glVertex2f(-1.0f, -1.0f);
	//	glVertex2f(1.0f, -1.0f);
	//	glVertex2f(-1.0f, 1.0f);
	//	glVertex2f(1.0f, 1.0f);

	//	//glColor3fv(outerColor);

	//	//for(int i = 0; i < slices; i++)
	//	//{
	//	//	float angle = incr * i;

	//	//	float x = (float) cos(angle) * radius;
	//	//	float y = (float) sin(angle) * radius;

	//	//	glVertex2f(x, y);
	//	//}

	//	//glVertex2f(radius, 0.0f);

	//glEnd();

	//// Set projection matrix.
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
}


/**
* Draws the entire scene.
*/
void MapSimplification::DrawScene()
{
	int midX = m_width * 0.5;

	glColor3f(0, 0, 0);

	// Draw original and simplified map
	glPushMatrix();
	{
		DrawOriginalPanel();
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(midX, 0, 0);
		DrawSimplifiedPanel();
	}
	glPopMatrix();

	// Draw divider
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_STRIP);
		glVertex2f(midX, 0);
		glVertex2f(midX, m_height);
	glEnd();

}


/**
* Draws the original map
*/
void MapSimplification::DrawOriginalPanel()
{
	// TODO: Draw polygons
	glLineWidth(2.5);
	glPointSize(6.0);




	// Draw polygons
	

	// Obtain min and max coordinates
	glm::vec2 min = glm::vec2(FLT_MAX);
	glm::vec2 max = glm::vec2(-FLT_MAX);

	for each (vector<glm::vec2> line in m_lines)
	{
		for each (glm::vec2 vert in line)
		{
			// Store min coords
			if (min.x > vert.x) min.x = vert.x;
			if (min.y > vert.y) min.y = vert.y;

			// Store max coords
			if (max.x < vert.x) max.x = vert.x;
			if (max.y < vert.y) max.y = vert.y;
		}
	}

	// calculate scale
//	float panelRatio = (m_width * 0.5) / m_height;
	glm::vec2 panelDims = glm::vec2((float)m_width * 0.5 - 20, (float)m_height - 20);
	glm::vec2 pointDims = glm::vec2(glm::distance(glm::vec2(max.x, 0), glm::vec2(min.x, 0)), glm::distance(max.y, min.y));
	glm::vec2 ratio = panelDims / pointDims;

	float scale = (ratio.x < ratio.y) ? ratio.x : ratio.y;

	glPushMatrix();
		// padding
		glTranslatef(10, 10, 0);

		// Draw points
		glScalef(scale, scale, 0);
		glTranslatef(-min.x, -min.y, 0);
		
		glColor3f(0.2, 0.2, 0.2);
		// Draw lines
		int i = 1;
		for each (vector<glm::vec2> line in m_lines)
		{
			//float col = ((float)i / (float)m_lines.size());
			//glColor3f(0.2, 0.2 + col * 0.8, 0.2 + col * 0.8);

			// Draw line
			glBegin(GL_LINE_STRIP);
			for each (glm::vec2 vert in line)
			{
				glVertex2f(vert.x, vert.y);
			}
			glEnd();
			i++;
		}


		glColor3f(1.0, 1.0, 0.4);

		// Draw points
		glBegin(GL_POINTS);
		for each (glm::vec2 vert in m_points)
		{
			glVertex2f(vert.x, vert.y);
		}
		glColor3f(1.0, 0.4, 0.4);
		glEnd();

	glPopMatrix();
	//glVertex2f(100, 100);
	//glVertex2f(250, 300);
	//glVertex2f(500, 200);
	//glVertex2f(480, 320);
	//glVertex2f(550, 400);

	

	// Draw point set

	//glBegin(GL_POINTS);
	//	glVertex2f(105, 120);
	//	glVertex2f(330, 210);
	//	glVertex2f(460, 290);



	//glEnd();

	//min = glm::vec2(FLT_MAX);
	//max = glm::vec2(FLT_MIN);

	//for each (glm::vec2 vert in m_points)
	//{
	//	// Store min coords
	//	if (min.x > vert.x) min.x = vert.x;
	//	if (min.y > vert.y) min.y = vert.y;

	//	// Store max coords
	//	if (max.x < vert.x) max.x = vert.x;
	//	if (max.y < vert.y) max.y = vert.y;
	//}


	glColor3f(1.0, 1.0, 1.0);
	DrawString("Original", GLUT_BITMAP_8_BY_13, 25, 25);
}

/**
* Draws the simplified map
*/
void MapSimplification::DrawSimplifiedPanel()
{
	glColor3f(1.0, 1.0, 1.0);
	DrawString("Simplified", GLUT_BITMAP_8_BY_13, 25, 25);
	// TODO: Draw polygons
	glLineWidth(2.5);
	glPointSize(6.0);
	glColor3f(0.4, 0.4, 1.0);

	// Draw polygons
	glColor3f(0.2, 0.2, 0.2);
	glBegin(GL_LINE_STRIP);
	{
		glVertex2f(100, 100);
		glVertex2f(250, 300);
		glVertex2f(500, 200);
		glVertex2f(550, 400);
	}
	glEnd();

	// Draw point set
	glColor3f(1.0, 1.0, 0.4);
	glBegin(GL_POINTS);
	{
		glVertex2f(105, 120);
		glVertex2f(330, 210);
		glVertex2f(460, 290);
	}
	glEnd();
}

/**
* Draws the GUI components
*/
void MapSimplification::DrawGUI()
{
	// Switch to orthographic 2D mode for the GUI
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, m_width, m_height, 0, -1000, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(0.2, 0.2, 0.2);

	// Draw menu hint
	DrawString("Open menu by clicking the Right Mouse Button", GLUT_BITMAP_8_BY_13, 25, m_height - 25);
}

/**
 * Main program execution body, delegates to an instance of the MapSimplification
 * implementation.
 */
int _tmain(int argc, char** argv)
{
	// TODO: Extract arguments

	glutInit( &argc, argv);

	MapSimplification mapSimplification;

	if(!mapSimplification.Init())
		return 1;

	mapSimplification.Run();

	return 0;
}