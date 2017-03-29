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
	
	int lineNr;
	string gmlString;
	string STRING;

	ifstream infile;
	infile.open(input);

	while (!infile.eof())
	{
		//Find all points for each line
		getline(infile, STRING);
		if (STRING != "")
		{
			vector<glm::vec2> points;

			//Extract XML data
			lineNr = stoi(STRING.substr(0, STRING.find(':')));
			gmlString = STRING.substr((STRING.find(':') + 1), STRING.length());
			while (gmlString.find('<') == 0)
			{
				gmlString = gmlString.substr(gmlString.find('>') + 1, gmlString.length());
			}
			gmlString = gmlString.substr(0, gmlString.find('<'));

			//Find each point set
			int a = gmlString.find(',');
			while (gmlString.find(',') >= 0 && !gmlString.empty())
			{
				a = gmlString.find(',');
				glm::vec2 point;

				point.x = stof(gmlString.substr(0, gmlString.find(',')));
				point.y = stof(gmlString.substr(gmlString.find(',') + 1, gmlString.find(' ')));

				points.push_back(point);

				//Continue with next points
				gmlString = gmlString.substr(gmlString.find(' ') + 1, gmlString.length());
			}

			lines.push_back(new Line(points));
		}
	}
	infile.close();

	return lines;
}
std::vector<glm::vec2> Parser::ParsePointFile(std::string input)
{
	vector<glm::vec2> points;

	int lineNr;
	string gmlString;
	string STRING;

	ifstream infile;
	infile.open(input);

	while (!infile.eof())
	{
		//Find each point
		getline(infile, STRING);
		if (STRING != "")
		{
			//Extract XML data
			lineNr = stoi(STRING.substr(0, STRING.find(':')));
			gmlString = STRING.substr((STRING.find(':') + 1), STRING.length());
			while (gmlString.find('<') == 0)
			{
				gmlString = gmlString.substr(gmlString.find('>') + 1, gmlString.length());
			}
			gmlString = gmlString.substr(0, gmlString.find('<'));

			//Get the actual point
			glm::vec2 point;
			point.x = stof(gmlString.substr(0, gmlString.find(',')));
			point.y = stof(gmlString.substr(gmlString.find(',') + 1, gmlString.find(' ')));

			points.push_back(point);

			//Continue with next point
			gmlString = gmlString.substr(gmlString.find(' ') + 1, gmlString.length());
		}
	}
	infile.close();

	return points;
}