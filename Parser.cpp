#include "Parser.h"



Parser::Parser()
{
}


Parser::~Parser()
{
}

vector<Line*> Parser::ParseLineFile(string input)
{
	vector<Line*> lines;
	
	string lineNr;
	string gmlString;
	string STRING;
	string skip;
	string pointString;
	string pointX;
	string pointY;

	ifstream infile(input);
	//infile.open(input);
	while (getline(infile, STRING))
	{
		stringstream iss(STRING);
		vector<glm::vec2> points;

		getline(iss, lineNr, ':');
		getline(iss, skip, '>');
		getline(iss, skip, '>');
		while (getline(iss, pointX, ',') && pointX[0] != '<')
		{
			getline(iss, pointY, ' ');
			glm::vec2 point;
			point.x = stof(pointX);
			point.y = stof(pointY);
			points.push_back(point);
		}
		lines.push_back(new Line(points));
	}

	infile.close();

	return lines;
}
std::vector<glm::vec2> Parser::ParsePointFile(std::string input)
{
	vector<glm::vec2> points;
	string lineNr;
	string skip;
	string pointX;
	string pointY;
	string gmlString;
	string STRING;

	ifstream infile(input);

	while (getline(infile, STRING))
	{
		stringstream iss(STRING);
		getline(iss, lineNr, ':');
		getline(iss, skip, '>');
		getline(iss, skip, '>');
		getline(iss, pointX, ',');
		getline(iss, pointY, '<');
		glm::vec2 point;
		point.x = stof(pointX);
		point.y = stof(pointY);
	}

	infile.close();

	return points;
}

void Parser::WriteOutput(vector<Line*> lines)
{
	ofstream lineFile;
	//ofstream pointFile;
	int count = 1;
	lineFile.open("Data\\lines_out_output.txt");

	for (std::vector<Line*>::const_iterator l_it = lines.begin(), l_e = lines.end(); l_it < l_e; l_it++)
	{
		lineFile << count << ":<gml:LineString srsName=\"EPSG:54004\" xmlns:gml=\"http ://www.opengis.net/gml\"><gml:coordinates decimal=\".\" cs=\",\" ts=\" \">";

		for (std::vector<glm::vec2>::const_iterator v_it = (*l_it)->verts.begin(), v_e = (*l_it)->verts.end(); v_it < v_e; v_it++)
		{
			lineFile << (*v_it).x << ',' << (*v_it).y << ' ';
		}
		lineFile << "</gml:coordinates></gml:LineString>\n";
		count++;
	}
	lineFile.close();
/*
	count = 1;
	pointFile.open("Data\\points_out_output.txt"); 
	for (std::vector<glm::vec2>::const_iterator p_it = points.begin() + 2, p_e = points.end(); p_it < p_e; p_it++)
	{
		pointFile << count << ":<gml:Point srsName=\"EPSG:54004\" xmlns:gml=\"http ://www.opengis.net/gml\"><gml:coordinates decimal=\".\" cs=\",\" ts=\" \">";
		pointFile << (*p_it).x << ',' << (*p_it).y << ' ';
		pointFile << "</gml:coordinates></gml:Point>\n";
		count++;
	}
	pointFile.close();*/
}