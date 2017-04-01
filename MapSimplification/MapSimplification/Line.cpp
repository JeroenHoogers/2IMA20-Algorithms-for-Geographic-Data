#include "Line.h"


Line::Line()
{
	m_pNearControlPoints = std::vector<const glm::vec2*>();
	m_pNearLines = std::vector<const Line*>();
}


Line::Line(const int& lineNr, std::vector<glm::vec2> vertices)
{
	id = lineNr;
	m_pNearControlPoints = std::vector<const glm::vec2*>();
	m_pNearLines = std::vector<const Line*>();

	verts = vertices;

	u = &verts[0];
	v = &verts[verts.size() - 1];
	
	// Check whether this is an island, if so add fake control point in the middle
	if (*u == *v)
		addHelperPoint(verts);
	
	//CalculateAABB();

	// 
}

Line::~Line()
{
}

// Checks whether a point is inside this line, assuming it is a closed loop (island) 
bool Line::isPointInsidePolygon(const glm::vec2& p, const std::vector<glm::vec2> polygon)
{
	//// Check whether the point is inside the bounding box
	//if (fabs(aabb.c.x - p.x) > aabb.r.x) return false;
	//if (fabs(aabb.c.y - p.y) > aabb.r.y) return false;

	bool inside = false;

	// Check whether the point is inside the polygon
	for (int i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++)
	{
		if ((polygon[i].y > p.y) != (polygon[j].y > p.y) &&
			p.x < (polygon[j].x - polygon[i].x) * (p.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x)
		{
			inside = !inside;
		}
	}

	return inside;
}

// Adds a "fake" control point inside a closed loop line (island) so the algorithm does not collapse this line to just 2 endpoints
void Line::addHelperPoint(const std::vector<glm::vec2> polygon)
{
	glm::vec2* p;
	glm::vec2 triangleCenter;

		// Use 3 consecutive points in the line to form a triangle and take the center of this triangle as a potential control point
	for (int i = 2; i < polygon.size(); i++)
	{
		triangleCenter = glm::vec2((polygon[i - 2] + polygon[i - 1] + polygon[i]) / 3.0f);

		// Check whether the triangle center lies within the polygon, if so use it as a control point, otherwise advance to the next triangle
		// Note that there must be at least one triangle with it's center inside the polygon
		if (isPointInsidePolygon(triangleCenter, polygon))
		{
			p = new glm::vec2(triangleCenter.x, triangleCenter.y);
			m_pHelperPoints.push_back(p);
			return;
		}
	}
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
	if (fabs(aabb.c.x - line.aabb.c.x) >= (aabb.r.x + line.aabb.r.x)) return false;
	if (fabs(aabb.c.y - line.aabb.c.y) >= (aabb.r.y + line.aabb.r.y)) return false;

	// bounding boxes ovelap
	// Exclude self by comparing aabbs
	if (id != line.id)
	{
		m_pNearLines.push_back(&line);

		// Add fake point
		if (*u == *line.u && *v == *line.v)
		{
			std::vector<glm::vec2> polygon = verts;
			std::reverse(polygon.begin(), polygon.end());
			polygon.insert(polygon.end(), line.verts.begin() + 1, line.verts.end() - 1);
			bool needHelper = true;
			for (std::vector<const glm::vec2*>::const_iterator v_it = m_pNearControlPoints.begin(), v_e = m_pNearControlPoints.end(); v_it < v_e; v_it++)
			{
				if (isPointInsidePolygon(**v_it, polygon))
				{
					needHelper = false;
					break;
				}
			}
			for (std::vector<const glm::vec2*>::const_iterator v_it = line.m_pNearControlPoints.begin(), v_e = line.m_pNearControlPoints.end(); v_it < v_e; v_it++)
			{
				if (isPointInsidePolygon(**v_it, polygon))
				{
					needHelper = false;
					break;
				}
			}
			if (needHelper)
				addHelperPoint(polygon);
		}
		else if (*u == *line.v && *v == *line.u)
		{
			std::vector<glm::vec2> polygon = verts;
			polygon.insert(polygon.end(), line.verts.begin() + 1, line.verts.end() - 1); 
			bool needHelper = true;
			for (std::vector<const glm::vec2*>::const_iterator v_it = m_pNearControlPoints.begin(), v_e = m_pNearControlPoints.end(); v_it < v_e; v_it++)
			{
				if (isPointInsidePolygon(**v_it, polygon))
				{
					needHelper = false;
					break;
				}
			}
			for (std::vector<const glm::vec2*>::const_iterator v_it = line.m_pNearControlPoints.begin(), v_e = line.m_pNearControlPoints.end(); v_it < v_e; v_it++)
			{
				if (isPointInsidePolygon(**v_it, polygon))
				{
					needHelper = false;
					break;
				}
			}
			if (needHelper)
				addHelperPoint(polygon);
		}
	}


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
	for (std::vector<const glm::vec2*>::const_iterator it = m_pHelperPoints.begin(), v_e = m_pHelperPoints.end(); it < v_e; it++)
	{
		b1 = sign(**it, v1, v2) < 0.0f;
		b2 = sign(**it, v2, v3) < 0.0f;
		b3 = sign(**it, v3, v1) < 0.0f;

		if ((b1 == b2) && (b2 == b3))
			return true;
	}
	return false;
}


bool Line::HasLineInTriangle(const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3)
{
	// TODO: fix floating point errors
	bool b1, b2, b3;

	for (std::vector<const Line*>::const_iterator it = m_pNearLines.begin(), e = m_pNearLines.end(); it < e; it++)
	{
		// Parallel case
		//for each (glm::vec2 vert in (*it)->verts)

		for (std::vector<glm::vec2>::const_iterator v_it = (*it)->verts.begin() + 1, v_e = (*it)->verts.end() - 1; v_it < v_e; v_it++)
		{
			
			b1 = sign(*v_it, v1, v2) < 0.0f;
			b2 = sign(*v_it, v2, v3) < 0.0f;
			b3 = sign(*v_it, v3, v1) < 0.0f;

			if ((b1 == b2) && (b2 == b3))
				return true;
		}
	}
	return false;
}

void Line::DrawHelperPoints()
{
	glColor3f(0.2, 0.3, 0.9);
	glBegin(GL_POINTS);
	for each (const glm::vec2* vert in m_pHelperPoints)
	{
		glVertex2f(vert->x, vert->y);
	}
	glEnd();

}