#include "SimplificationAlgorithm.h"



SimplificationAlgorithm::SimplificationAlgorithm()
{
}


SimplificationAlgorithm::~SimplificationAlgorithm()
{
}


void SimplificationAlgorithm::Simplify(int verticesToRemove, string inputLinesPath, string inputPointsPath, string outputPath)
{
	const clock_t beginTime = clock();
	clock_t partTime = clock();

	// do something
	float seconds = 0;

	//Parse input files
	cout << "Parsing Input... ";
	partTime = clock();
	LoadInput(inputLinesPath, inputPointsPath);
	seconds = float(clock() - partTime) / CLOCKS_PER_SEC;
	cout << " (" << seconds << "s)" << endl;

	//Calculate AABB's
	//TODO : Speed up with sweepline
	cout << "Calculating AABB and detecting nearby control points... ";
	partTime = clock();
	for (std::vector<Line*>::const_iterator l_it = m_lines.begin(), l_e = m_lines.end(); l_it < l_e; l_it++)
	{
		(*l_it)->CalculateAABB();
		for (std::vector<glm::vec2>::const_iterator v_it = m_points.begin(), v_e = m_points.end(); v_it < v_e; v_it++)
		{
			(*l_it)->AABBContainsControlPoint(*v_it);
		}
	}
	seconds = float(clock() - partTime) / CLOCKS_PER_SEC;
	cout << " (" << seconds << "s)" << endl;

	//Run simplification algorithm VisvalingamWhyatt
	cout << "Simplifying... ";
	partTime = clock();

	VisvalingamWhyatt();
	
	seconds = float(clock() - partTime) / CLOCKS_PER_SEC;
	cout << " (" << seconds << "s)" << endl;

	seconds = float(clock() - beginTime) / CLOCKS_PER_SEC;
	cout << "Finished in " << seconds << "s" << endl;

	//TODO : Write output

	//TODO : Open result

	MapSimplification mapSimplification(m_lines, m_simplifiedLines, m_points);

	if (mapSimplification.Init())
		mapSimplification.Run();
}



void SimplificationAlgorithm::LoadInput(string inputLines, string inputPoints)
{
	Parser parser;
	m_lines = parser.ParseLineFile(inputLines);
	m_points = parser.ParsePointFile(inputPoints);
	
}

float SimplificationAlgorithm::CalculateArea(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
{
	//Calculate triangle area for 3 points
	float area = std::fabs(a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2;
	return area;
}

void SimplificationAlgorithm::VisvalingamWhyatt()
{
	//for (int i = 0; i < lines->size(); i++)
	Line* l = nullptr;
	glm::vec2 v;
	glm::vec2 v_next;
	glm::vec2 v_prev;
	int indexToRemove = 0;
	int i = 0;
	int count = 0;
	float minArea = FLT_MAX;

	for (std::vector<Line*>::const_iterator l_it = m_lines.begin(), l_e = m_lines.end(); l_it < l_e; l_it++)
	{
		l = *l_it;
		//Create simplified Line
		std::vector<glm::vec2> vertices = (*l).verts;//(*lines)[i].verts;
		Line* simplifiedLine = new Line(vertices);

		count = 0;
		int vertsToKeep = 2;// +floor((simplifiedLine->verts.size() - 2) * 0.01);
		//Find point to remove from line
		while (simplifiedLine->verts.size() > vertsToKeep)
		{
			vertsToKeep = 2;
			//Calculate minimal effective area
			minArea = FLT_MAX;
			if (!l->HasPointInTriangle(simplifiedLine->verts[0], simplifiedLine->verts[1], simplifiedLine->verts[2]))
			{
				minArea = CalculateArea(simplifiedLine->verts[0], simplifiedLine->verts[1], simplifiedLine->verts[2]);
				indexToRemove = 1;
			}
			else
			{
				vertsToKeep++;
				indexToRemove = -1;
			}

			i = 2;
			for (std::vector<glm::vec2>::const_iterator v_it = simplifiedLine->verts.begin() + 2, v_e = simplifiedLine->verts.end(); v_it < v_e - 1; v_it++) //int i = 2; i < simplifiedLine.verts.size() - 1; i++)
			{
				v = *v_it;
				v_next = *(v_it + 1);
				v_prev = *(v_it - 1);

				//TODO : Only consider possible simplifications
				//TODO : Check if simplification would intersect another line

				//Check if point in Area, con
				if (!l->HasPointInTriangle(v_prev, v, v_next))
				{
					float curArea = CalculateArea(v_prev, v, v_next);
					if (minArea > curArea)
					{
						minArea = curArea;
						indexToRemove = i;
					}
				}
				else
				{
					vertsToKeep++;
				}
				i++;
			}
			if (indexToRemove >= 0)
				simplifiedLine->RemoveVertex(indexToRemove);
			count++;
		}
		m_simplifiedLines.push_back(simplifiedLine);
	}
}

/**
* Main program execution body, delegates to an instance of the MapSimplification
* implementation.
*/
int _tmain(int argc, char** argv)
{
	// TODO: Extract arguments
	glutInit(&argc, argv);

	SimplificationAlgorithm simplificationAlgorithm;
	simplificationAlgorithm.Simplify(5, "Data\\training_data5\\lines_out.txt", "Data\\training_data5\\points_out.txt", "output.txt");

	return 0;
}