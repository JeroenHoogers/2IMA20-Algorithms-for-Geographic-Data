#pragma once
#include "GLApp.h"

struct AABB
{
	AABB() : c(), r() {}

	AABB(const glm::vec2 & center, const glm::vec2 & halfwidths)
		: c(center)
		, r(halfwidths)
	{}

	glm::vec2 c;        // center point
	glm::vec2 r;        // halfwidths
};

class Line
{
private:

	std::vector<glm::vec2*> m_pNearControlPoints;
	std::vector<Line*> m_pNearLines;


public:

	AABB aabb;
	std::vector<glm::vec2> verts;

	Line();
	Line(std::vector<glm::vec2> vertices);
	~Line();

	void CalculateAABB();
	bool AABBContainsControlPoint(glm::vec2* controlPoint);
	bool AABBIntersectsLineAABB(Line* line);

	// TODO: Remove vertex
	void RemoveVertex(int indexVertex);

	void DrawAABB();
};

