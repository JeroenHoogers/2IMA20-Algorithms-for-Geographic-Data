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

	std::vector<const glm::vec2*> m_pNearControlPoints;
	std::vector<const Line*> m_pNearLines;

	float sign(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3);

	void addPointInsideIsland();
	bool isPointInsideIsland(const glm::vec2& p);

public:
	
	int lnr;
	AABB aabb;
	std::vector<glm::vec2> verts;

	Line();
	Line(const int& lineNr, std::vector<glm::vec2> vertices);
	~Line();

	void CalculateAABB();
	bool AABBContainsControlPoint(const glm::vec2& controlPoint);
	bool AABBIntersectsLineAABB(const Line&);
	bool HasPointInTriangle(const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3);
	bool HasLineInTriangle(const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3);
	void PrintLineInfo() { std::cout << "nearby points: " << m_pNearControlPoints.size() <<" nearby lines: " << m_pNearLines.size() << std::endl; };

	// TODO: Remove vertex
	void RemoveVertex(int indexVertex);

	void DrawAABB();
};

