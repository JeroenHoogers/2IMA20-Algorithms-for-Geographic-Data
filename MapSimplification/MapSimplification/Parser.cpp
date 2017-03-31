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
	int linen = 0;
	string gmlString;
	string STRING;
	string skip;
	string pointString;
	string pointX;
	string pointY;

	ifstream infile(input);

	while (getline(infile, STRING))
	{
		stringstream iss(STRING);
		vector<glm::vec2> points;

		getline(iss, lineNr, ':');
		linen = stof(lineNr);

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
		lines.push_back(new Line(linen, points));
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
		points.push_back(point);
	}

	infile.close();

	return points;
}

void Parser::WriteOutput(vector<Line*> lines)
{
	ofstream lineFile;
	int count = 1;
	lineFile.open("Data\\lines_out_output.txt");

	for (std::vector<Line*>::const_iterator l_it = lines.begin(), l_e = lines.end(); l_it < l_e; l_it++)
	{
		//Write line number with first gml tags
		lineFile << (*l_it)->lnr << ":<gml:LineString srsName=\"EPSG:54004\" xmlns:gml=\"http ://www.opengis.net/gml\"><gml:coordinates decimal=\".\" cs=\",\" ts=\" \">";

		//Write line points
		for (std::vector<glm::vec2>::const_iterator v_it = (*l_it)->verts.begin(), v_e = (*l_it)->verts.end(); v_it < v_e; v_it++)
		{
			lineFile << (*v_it).x << ',' << (*v_it).y << ' ';
		}

		//Write end line tags
		lineFile << "</gml:coordinates></gml:LineString>\n";
		count++;
	}
	lineFile.close();
}