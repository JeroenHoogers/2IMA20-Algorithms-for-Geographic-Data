#include "Line.h"

#pragma once
class VisvalingamWhyatt
{
public:
	VisvalingamWhyatt();
	~VisvalingamWhyatt();

	std::vector<Line> Algorithm(std::vector<Line*> lines, int number_to_keep);

private:
	float CalculateArea(glm::vec2 *a, glm::vec2 *b, glm::vec2 *c);
};

