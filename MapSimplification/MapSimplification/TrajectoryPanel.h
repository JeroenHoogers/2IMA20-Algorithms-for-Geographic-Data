#pragma once
#include "GLUtil.h"

using namespace std;

class TrajectoryPanel
{
private:
	vector<glm::vec2> m_points;
	vector<vector<glm::vec2>> m_lines;
	
	glm::vec2 m_min;
	glm::vec2 m_max;

public:
	TrajectoryPanel();
	~TrajectoryPanel();

	void loadData(vector<glm::vec2> points, vector<vector<glm::vec2>> lines);
	void draw();


};

