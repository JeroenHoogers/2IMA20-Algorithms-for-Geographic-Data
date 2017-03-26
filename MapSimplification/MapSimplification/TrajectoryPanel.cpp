#include "TrajectoryPanel.h"



TrajectoryPanel::TrajectoryPanel()
{
	m_points = vector<glm::vec2>();
	m_lines = vector<vector<glm::vec2>>();
}


TrajectoryPanel::~TrajectoryPanel()
{
}

void TrajectoryPanel::loadData(vector<glm::vec2> points, vector<vector<glm::vec2>> lines)
{
	m_points = points;
	m_lines = lines;

	// Obtain min and max coordinates
	glm::vec2 min = glm::vec2(FLT_MAX);
	glm::vec2 max = glm::vec2(FLT_MIN);

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

	// Also take points into account
	//for each (glm::vec2 vert in m_points)
	//{
	//	// Store min coords
	//	if (min.x > vert.x)
	//		min.x = vert.x;

	//	if (min.y > vert.y)
	//		min.y = vert.y;

	//	// Store max coords
	//	if (max.x < vert.x)
	//		max.x = vert.x;

	//	if (max.y < vert.y)
	//		max.y = vert.y;
	//}

}

void TrajectoryPanel::draw()
{
	glPushMatrix();
	{

		// Draw lines
	}
	glPopMatrix();
}

