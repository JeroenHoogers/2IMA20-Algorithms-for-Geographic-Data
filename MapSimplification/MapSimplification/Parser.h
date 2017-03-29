#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "GLUtil.h"
#include "Line.h"
using namespace std;

class Parser
{
public:
	Parser();
	~Parser();
	
	vector<Line*> ParseLineFile(string input);
	vector<glm::vec2> ParsePointFile(string input);
};

