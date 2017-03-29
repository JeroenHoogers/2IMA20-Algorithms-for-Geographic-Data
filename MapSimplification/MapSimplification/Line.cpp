#include "Line.h"


Line::Line()
{
	m_pNearControlPoints = std::vector<glm::vec2*>();
	m_pNearLines = std::vector<Line*>();
}


Line::Line(std::vector<glm::vec2> vertices)
{
	m_pNearControlPoints = std::vector<glm::vec2*>();
	m_pNearLines = std::vector<Line*>();

	verts = vertices;
}


Line::~Line()
{
}


void Line::CalculateAABB()
{
	glm::vec2 min = glm::vec2(FLT_MAX);
	glm::vec2 max = glm::vec2(-FLT_MAX);

	// Calculate AABB
	for each (glm::vec2	vertex in verts)
	{
		if (min.x < vertex.x) min.x = vertex.x;
		if (min.y < vertex.y) min.y = vertex.y;

		if (max.x > vertex.x) max.x = vertex.x;
		if (max.y > vertex.y) max.y = vertex.y;
	}

	glm::vec2 c = glm::vec2((min.x + max.x) * 0.5, (min.y + max.y) * 0.5);
	glm::vec2 h = glm::vec2(fabs(c.x - min.x), fabs(c.y - min.y));

	aabb = AABB(c, h);
}


bool Line::AABBContainsControlPoint(glm::vec2* pControlPoint)
{
	// check if the control point is contained in the aabb
	if (fabs(aabb.c.x - pControlPoint->x) > aabb.r.x) return false;
	if (fabs(aabb.c.y - pControlPoint->y) > aabb.r.y) return false;

	// control point is contained in the bounding box
	m_pNearControlPoints.push_back(pControlPoint);

	return true;
}


bool Line::AABBIntersectsLineAABB(Line* line)
{
	// check if the line bounding boxes overlap
	if (fabs(aabb.c.x - line->aabb.c.x) > (aabb.r.x + line->aabb.r.x)) return false;
	if (fabs(aabb.c.y - line->aabb.c.y) > (aabb.r.y + line->aabb.r.y)) return false;

	// bounding boxes ovelap
	m_pNearLines.push_back(line);

	return true;
}


void Line::DrawAABB()
{
	glLineWidth(1.0);
	glColor3f(0.2, 0.2, 0.8);
	glBegin(GL_LINE_STRIP);
		glVertex2f(aabb.c.x - aabb.r.x, aabb.c.y - aabb.r.y);
		glVertex2f(aabb.c.x - aabb.r.x, aabb.c.y + aabb.r.y);
		glVertex2f(aabb.c.x + aabb.r.x, aabb.c.y + aabb.r.y);
		glVertex2f(aabb.c.x + aabb.r.x, aabb.c.y - aabb.r.y);

		glVertex2f(aabb.c.x - aabb.r.x, aabb.c.y - aabb.r.y);
	glEnd();
}