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


	m_showAABBs = false;
	m_showEndpoints = false;
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

	vector<vector<glm::vec2>> lines = parser.ParseLineFile("Data\\training_data5\\lines_out.txt");
	for each (vector<glm::vec2> line in lines)
	{
		m_lines.push_back(new Line(line));
	}

	m_points = parser.ParsePointFile("Data\\training_data5\\points_out.txt");

	// Obtain min and max coordinates
	m_min = glm::vec2(FLT_MAX);
	m_max = glm::vec2(-FLT_MAX);

	for (vector<Line*>::const_iterator it = m_lines.begin(), e = m_lines.end(); it != e; ++it)
	{
		//line.CalculateAABB();
		for each (glm::vec2 vert in (*it)->verts)
		{
			// Store min coords
			if (m_min.x > vert.x) m_min.x = vert.x;
			if (m_min.y > vert.y) m_min.y = vert.y;

			// Store max coords
			if (m_max.x < vert.x) m_max.x = vert.x;
			if (m_max.y < vert.y) m_max.y = vert.y;
		}
	}

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
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, 0, m_width * 0.5, m_height);
		DrawOriginalPanel();
		glDisable(GL_SCISSOR_TEST);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(m_width * 0.5, 0, m_width, m_height);
		glTranslatef(midX, 0, 0);
		DrawOriginalPanel();
		//DrawSimplifiedPanel();
		glDisable(GL_SCISSOR_TEST);
	}
	glPopMatrix();

	// Draw divider
	glColor3f(1.0, 1.0, 1.0);

	glLineWidth(20.0);
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
	// Set line and point sizes
	glLineWidth(2.5);
	glPointSize(6.0);

	// Draw polygons

	// calculate scale
//	float panelRatio = (m_width * 0.5) / m_height;
	glm::vec2 panelDims = glm::vec2((float)m_width * 0.5 - 20, (float)m_height - 20);
	glm::vec2 pointDims = glm::vec2(glm::distance(glm::vec2(m_max.x, 0), glm::vec2(m_min.x, 0)), glm::distance(m_max.y, m_min.y));
	glm::vec2 ratio = panelDims / pointDims;

	float scale = (ratio.x < ratio.y) ? ratio.x : ratio.y;

	glPushMatrix();
		// padding
		glTranslatef(10, 10, 0);
		glTranslatef(m_topleft.x, m_topleft.y, 0);

		// Draw points
		glScalef(scale * m_zoom, -scale * m_zoom, 0);
		glTranslatef(-m_min.x, -m_min.y - pointDims.y, 0);
		

		
		// Draw lines
		Line* l = nullptr;

		for (vector<Line*>::const_iterator it = m_lines.begin(), e = m_lines.end(); it != e; ++it)
		{
			l = *it;

			glColor3f(0.2, 0.2, 0.2);
			glLineWidth(2.5);
			//float col = ((float)i / (float)m_lines.size());
			//glColor3f(0.2, 0.2 + col * 0.8, 0.2 + col * 0.8);

			// Draw line
			glBegin(GL_LINE_STRIP);
			for each (glm::vec2 vert in l->verts)
			{
				glVertex2f(vert.x, vert.y);
			}
			glEnd();


			// Draw points
			if (m_showEndpoints)
			{
				glBegin(GL_POINTS);
				{
					glVertex2f(l->verts[0].x, l->verts[0].y);
					glVertex2f(l->verts[l->verts.size() - 1].x, l->verts[l->verts.size() - 1].y);
				}
				glEnd();
			}

			if(m_showAABBs)
				l->DrawAABB();
		}

		glColor3f(0.4, 1.0, 0.4);

		// Draw points
		glBegin(GL_POINTS);
		for each (glm::vec2 vert in m_points)
		{
			glVertex2f(vert.x, vert.y);
		}
		glEnd();

	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
	DrawString("Original", GLUT_BITMAP_9_BY_15, 25, 25);
}

/**
* Draws the simplified map
*/
void MapSimplification::DrawSimplifiedPanel()
{
	// Set line and point sizes
	glLineWidth(2.5);
	glPointSize(6.0);

	// Draw polygons

	// calculate scale
	//	float panelRatio = (m_width * 0.5) / m_height;
	glm::vec2 panelDims = glm::vec2((float)m_width * 0.5, (float)m_height);
	glm::vec2 pointDims = glm::vec2(glm::distance(glm::vec2(m_max.x, 0), glm::vec2(m_min.x, 0)), glm::distance(m_max.y, m_min.y));
	glm::vec2 ratio = panelDims / pointDims;

	float scale = (ratio.x < ratio.y) ? ratio.x : ratio.y;

	glPushMatrix();
	// padding
	//glTranslatef(10, 10, 0);
	glTranslatef(m_topleft.x * 0.5, m_topleft.y * 0.5, 0);

	// Draw points
	glScalef(scale * m_zoom, -scale * m_zoom, 0);
	glTranslatef(-m_min.x, -m_min.y - pointDims.y, 0);

	glColor3f(0.2, 0.2, 0.2);

	// Draw lines
	for each (Line* line in m_lines)
	{
		//float col = ((float)i / (float)m_lines.size());
		//glColor3f(0.2, 0.2 + col * 0.8, 0.2 + col * 0.8);

		// Draw line
		glBegin(GL_LINE_STRIP);
		for each (glm::vec2 vert in line->verts)
		{
			glVertex2f(vert.x, vert.y);
		}
		glEnd();


		//glColor3f(1.0, 1.0, 0.4);

		// Draw points
		glBegin(GL_POINTS);

		glVertex2f(line->verts[0].x, line->verts[0].y);
		glVertex2f(line->verts[line->verts.size() - 1].x, line->verts[line->verts.size() - 1].y);
		glEnd();
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

	glColor3f(1.0, 1.0, 1.0);
	DrawString("Simplified", GLUT_BITMAP_8_BY_13, 25, 25);
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