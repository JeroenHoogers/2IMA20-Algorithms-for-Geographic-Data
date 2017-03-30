#pragma once

#include "Line.h"
#include "GLUtil.h"
#include "Parser.h"
#include "MapSimplification.h"
#include <string>
#include <ctime>
#include <iostream>
using namespace std;

class SimplificationAlgorithm
{
private:
	vector<Line*> m_lines;
	vector<glm::vec2> m_points;
	vector<Line*> m_simplifiedLines;

	void Preprocess();
	void LoadInput(string inputLines, string inputPoints);
	float CalculateArea(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);
	void VisvalingamWhyatt();


public:
	SimplificationAlgorithm();
	~SimplificationAlgorithm();

	void Simplify(int verticesToRemove, string inputLinesPath, string inputPointsPath, string outputPath);
};