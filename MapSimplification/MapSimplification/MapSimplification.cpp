#include "MapSimplification.h"

MapSimplification::MapSimplification(vector<Line*> originalLines, vector<Line*> simplifiedLines, vector<glm::vec2> points) : GLApp()
{
	m_originalLines = originalLines;
	m_simplifiedLines = simplifiedLines;
	m_points = points;
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
	center = glm::vec2(m_width * 0.25, m_height * 0.5);

	m_showAABBs = false;
	m_showEndpoints = false;
	m_showControlPoints = true;
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


	//Parser parser;

	//vector<vector<glm::vec2>> lines = parser.ParseLineFile("Data\\training_data5\\lines_out.txt");
	//for each (vector<glm::vec2> line in lines)
	//{
	//	m_lines.push_back(new Line(line));
	//}

	//m_points = parser.ParsePointFile("Data\\training_data5\\points_out.txt");

	// Obtain min and max coordinates
	m_min = glm::vec2(FLT_MAX);
	m_max = glm::vec2(-FLT_MAX);

	for (vector<Line*>::const_iterator it = m_originalLines.begin(), e = m_originalLines.end(); it != e; ++it)
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

	glm::vec2 panelDims = glm::vec2((float)m_width * 0.5, (float)m_height);
	glm::vec2 pointDims = glm::vec2(glm::distance(glm::vec2(m_max.x, 0), glm::vec2(m_min.x, 0)), glm::distance(m_max.y, m_min.y));
	glm::vec2 ratio = panelDims / pointDims;

	m_scale = (ratio.x < ratio.y) ? ratio.x : ratio.y;

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
		DrawPanel(m_originalLines);
		glColor3f(1.0, 1.0, 1.0);
		DrawString("Original", GLUT_BITMAP_9_BY_15, 25, 25);
		glDisable(GL_SCISSOR_TEST);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(m_width * 0.5, 0, m_width, m_height);
		glTranslatef(midX, 0, 0);
		//DrawOriginalPanel();
		//DrawSimplifiedPanel();
		DrawPanel(m_simplifiedLines);
		glColor3f(1.0, 1.0, 1.0);
		DrawString("Simplification", GLUT_BITMAP_9_BY_15, 25, 25);
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
* Draws a panel
*/
void MapSimplification::DrawPanel(const vector<Line*>& lines)
{
	// Set line and point sizes
	glLineWidth(2.5);
	glPointSize(6.0);

	// Draw polygons

	// calculate scale
//	float panelRatio = (m_width * 0.5) / m_height;


//	float scale = ratio.x;
	glPushMatrix();
		// padding
		glTranslatef((m_width * 0.25) + m_center.x * m_zoom, (m_height * 0.5) + m_center.y * m_zoom, 0);

		// Draw points
		glScalef(m_scale * m_zoom, -m_scale * m_zoom, 0);
		glTranslatef(-(m_min.x + m_max.x) * 0.5 , -(m_min.y + m_max.y) * 0.5, 0);
		
		// Draw lines
		Line* l = nullptr;

		for (vector<Line*>::const_iterator it = lines.begin(), e = lines.end(); it != e; ++it)
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

		// Draw points
		if (m_showControlPoints)
		{
			glColor3f(0.4, 1.0, 0.4);

			glBegin(GL_POINTS);
			for each (glm::vec2 vert in m_points)
			{
				glVertex2f(vert.x, vert.y);
			}
			glEnd();
		}

	glPopMatrix();
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
