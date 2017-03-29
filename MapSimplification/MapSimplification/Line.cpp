#include "Line.h"


Line::Line()
{
	m_pNearControlPoints = std::vector<const glm::vec2*>();
	m_pNearLines = std::vector<const Line*>();
}


Line::Line(std::vector<glm::vec2> vertices)
{
	m_pNearControlPoints = std::vector<const glm::vec2*>();
	m_pNearLines = std::vector<const Line*>();

	verts = vertices;

	//CalculateAABB();
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
		if (min.x > vertex.x) min.x = vertex.x;
		if (min.y > vertex.y) min.y = vertex.y;

		if (max.x < vertex.x) max.x = vertex.x;
		if (max.y < vertex.y) max.y = vertex.y;
	}

	glm::vec2 c = glm::vec2((min.x + max.x) * 0.5, (min.y + max.y) * 0.5);
	glm::vec2 h = glm::vec2(fabs(c.x - min.x), fabs(c.y - min.y));

	aabb = AABB(c, h);
}


bool Line::AABBContainsControlPoint(const glm::vec2& controlPoint)
{
	// check if the control point is contained in the aabb
	if (fabs(aabb.c.x - controlPoint.x) > aabb.r.x) return false;
	if (fabs(aabb.c.y - controlPoint.y) > aabb.r.y) return false;

	// control point is contained in the bounding box
	m_pNearControlPoints.push_back(&controlPoint);

	return true;
}


bool Line::AABBIntersectsLineAABB(const Line& line)
{
	// check if the line bounding boxes overlap
	if (fabs(aabb.c.x - line.aabb.c.x) > (aabb.r.x + line.aabb.r.x)) return false;
	if (fabs(aabb.c.y - line.aabb.c.y) > (aabb.r.y + line.aabb.r.y)) return false;

	// bounding boxes ovelap
	m_pNearLines.push_back(&line);

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


void Line::RemoveVertex(int indexVertex)
{
	verts.erase(verts.begin() + indexVertex);
}

float Line::sign(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool Line::HasPointInTriangle(const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3)
{
	bool b1, b2, b3;

	for (std::vector<const glm::vec2*>::const_iterator it = m_pNearControlPoints.begin(), v_e = m_pNearControlPoints.end(); it < v_e; it++)
	{
		b1 = sign(**it, v1, v2) < 0.0f;
		b2 = sign(**it, v2, v3) < 0.0f;
		b3 = sign(**it, v3, v1) < 0.0f;

		if ((b1 == b2) && (b2 == b3))
			return true;
	}
	return false;
}
