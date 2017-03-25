#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "GLApp.h"
using namespace std;

class Parser
{
public:
	Parser();
	~Parser();
	
	vector<vector<glm::vec2>> ParseLineFile(string input);
	vector<glm::vec2> ParsePointFile(string input);
};

