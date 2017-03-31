#include "SimplificationAlgorithm.h"



SimplificationAlgorithm::SimplificationAlgorithm()
{
	m_nrThreads = 4;
}


SimplificationAlgorithm::~SimplificationAlgorithm()
{
}


float SimplificationAlgorithm::Simplify(int verticesToRemove, string inputLinesPath, string inputPointsPath, string outputPath)
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
	cout << " Done (" << seconds << "s)" << endl;

	//Calculate AABB's
	//TODO : Speed up with sweepline
	cout << "Calculating AABB and detecting nearby control points and lines... ";
	partTime = clock();

	int n = m_lines.size();
	int start = 0;
	int end = 0;
	vector<thread> threads;

	//for (int i = 0; i < m_nrThreads; i++)
	//{
	//	start = i * (n / m_nrThreads);
	//	end = (i == m_nrThreads - 1) ? n : (i + 1) * (n / m_nrThreads);

	//	//VisvalingamWhyatt(start, end);
	//	threads.push_back(thread(&SimplificationAlgorithm::CalculateAABBs, this, start, end));
	//}

	//for (int i = 0; i < m_nrThreads; i++)
	//	threads[i].join();

	//threads.clear();

	CalculateAABBs(0, m_lines.size());


	// Do preprocessing
	for (int i = 0; i < m_nrThreads; i++)
	{
		start = i * (n / m_nrThreads);
		end = (i == m_nrThreads - 1) ? n : (i + 1) * (n / m_nrThreads);

		//VisvalingamWhyatt(start, end);
		threads.push_back(thread(&SimplificationAlgorithm::Preprocess, this, start, end));
	}

	for (int i = 0; i < m_nrThreads; i++)
		threads[i].join();

	threads.clear();

	seconds = float(clock() - partTime) / CLOCKS_PER_SEC;
	cout << " Done (" << seconds << "s)" << endl;

	//Run simplification algorithm VisvalingamWhyatt
	cout << "Simplifying... ";
	partTime = clock();


//	m_nrThreads = 4;
	for (int i = 0; i < m_nrThreads; i++)
	{
		start = i * (n / m_nrThreads);
		end = (i == m_nrThreads - 1) ? n : (i + 1) * (n / m_nrThreads);

		//VisvalingamWhyatt(start, end);
		threads.push_back(thread(&SimplificationAlgorithm::VisvalingamWhyatt, this, start, end));
	}

	for (int i = 0; i < m_nrThreads; i++)
		threads[i].join();

	threads.clear();

	//VisvalingamWhyatt(0, 0);
	
	seconds = float(clock() - partTime) / CLOCKS_PER_SEC;
	cout << " Done (" << seconds << "s)" << endl;


	// Writing output file
	cout << "Writing output... ";
	partTime = clock();

	Parser parser;
	parser.WriteOutput(m_simplifiedLines);

	//TODO : Write output
	seconds = float(clock() - partTime) / CLOCKS_PER_SEC;
	cout << " Done (" << seconds << "s)" << endl;


	// Total time
	seconds = float(clock() - beginTime) / CLOCKS_PER_SEC;
	cout << "===========================================================" << endl;
	cout << "Finished in " << seconds << "s" << endl;
	cout << "===========================================================" << endl;

	//TODO : Calculate grade

	//TODO : Open result


	MapSimplification mapSimplification(m_lines, m_simplifiedLines, m_points);

	if (mapSimplification.Init())
		mapSimplification.Run();

	return seconds;
}


void SimplificationAlgorithm::CalculateAABBs(int start, int end)
{
	// Calculate AABBs and detect nearby control points
	for (std::vector<Line*>::const_iterator l_it = m_lines.begin() + start, l_e = m_lines.begin() + end; l_it < l_e; l_it++)
	{
		// Calculate AABB
		(*l_it)->CalculateAABB();
	}
}


void SimplificationAlgorithm::Preprocess(int start, int end)
{
	// Check AABB overlap to find lines that potentially cause topology errors when simplified
	for (std::vector<Line*>::const_iterator l1_it = m_lines.begin() + start, l1_e = m_lines.begin() + end; l1_it < l1_e; l1_it++)
	{
		// Find control points inside the AABB 
		// Now O(n^2) could be improved using a sweepline algorithm
		for (std::vector<glm::vec2>::const_iterator v_it = m_points.begin(), v_e = m_points.end(); v_it < v_e; v_it++)
		{
			(*l1_it)->AABBContainsControlPoint(*v_it);
		}


		// Find potential overlapping lines
		for (std::vector<Line*>::const_iterator l2_it = m_lines.begin(), l2_e = m_lines.end(); l2_it < l2_e; l2_it++)
		{
			(*l1_it)->AABBIntersectsLineAABB(**l2_it);
		}

		// Debug print numbers of nearby points and lines found
		//(*l1_it)->PrintLineInfo();
	}
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

void SimplificationAlgorithm::VisvalingamWhyatt(int start, int end)
{
	Line* l = nullptr;
	glm::vec2 v;
	glm::vec2 v_next;
	glm::vec2 v_prev;
	int indexToRemove = 0;
	int i = 0;
	int count = 0;
	float minArea = FLT_MAX;

	for (std::vector<Line*>::const_iterator l_it = m_lines.begin() + start, l_e = m_lines.begin() + end; l_it < l_e; l_it++)
	{
		l = *l_it;

		//Create simplified Line
		std::vector<glm::vec2> vertices = (*l).verts;
		Line* simplifiedLine = new Line(l->id, vertices);

		count = 0;
		bool deletionPossible = true;
		//Find point to remove from line
		while (deletionPossible)
		{

			//Calculate minimal effective area
			minArea = FLT_MAX;
			
			indexToRemove = -1;
			i = 1;
			for (std::vector<glm::vec2>::const_iterator v_it = simplifiedLine->verts.begin() + 1, v_e = simplifiedLine->verts.end(); v_it < v_e - 1; v_it++) //int i = 2; i < simplifiedLine.verts.size() - 1; i++)
			{
				v = *v_it;
				v_next = *(v_it + 1);
				v_prev = *(v_it - 1);


				//Check if point in Area
				if (!l->HasPointInTriangle(v_prev, v, v_next) && !l->HasLineInTriangle(v_prev, v, v_next))
				{
					float curArea = CalculateArea(v_prev, v, v_next);
					if (minArea > curArea)
					{
						minArea = curArea;
						indexToRemove = i;
					}
				}
				i++;
			}
			if (indexToRemove >= 0)
				simplifiedLine->RemoveVertex(indexToRemove);
			else
				deletionPossible = false;
			count++;
		}
		m_simplifiedLines.push_back(simplifiedLine);
	}
}

/**
* Main program execution body, delegates to an instance of the MapSimplification
* implementation.
*/
int _tmain(int argc, char* argv[])
{
	// TODO: Extract arguments
	glutInit(&argc, argv);
	// Check the number of parameters
	if (argc < 5) {
		// Tell the user how to run the program
		std::cerr << "Usage: " << argv[0] << " <amount_to_remove>" << " <input lines>" << " <input points>" << " <output>" << std::endl;
		/* "Usage messages" are a conventional way of telling the user
		* how to run a program if they enter the command incorrectly.
		*/
		return 1;
	}
	SimplificationAlgorithm simplificationAlgorithm;
	//return floor(simplificationAlgorithm.Simplify(5, "Data\\training_data5\\lines_out.txt", "Data\\training_data5\\points_out.txt", "output.txt"));
	return simplificationAlgorithm.Simplify(stoi(argv[1]), argv[2], argv[3], argv[4]);

	return 0;
}