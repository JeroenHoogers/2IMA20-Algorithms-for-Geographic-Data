#include "SimplificationAlgorithm.h"



SimplificationAlgorithm::SimplificationAlgorithm()
{
	m_nrThreads = 4;
	m_nrVerticesRemoved = 0;
	m_nrVertices = 0;
}


SimplificationAlgorithm::~SimplificationAlgorithm()
{
}


//-----------------------------------------------------------------------------------------------------------------//
void SimplificationAlgorithm::Simplify(int verticesToRemove, string inputLinesPath, string inputPointsPath, string outputPath)
{
	m_nrVerticesToRemove = verticesToRemove;
	const clock_t beginTime = clock();
	clock_t partTime = clock();
	Parser parser;

	// Running algorithm using the following parameters:
	cout << "===========================================================" << endl;
	cout << "Running Simplification" << endl;
	cout << "===========================================================" << endl;
	cout << "<PointsToRemove>:\t" << verticesToRemove << endl;
	cout << "<LineInputFilePath>:\t" << inputLinesPath << endl;
	cout << "<PointInputFilePath>:\t" << inputPointsPath << endl;
	cout << "<OutputFilePath>:\t" << outputPath << endl;
	cout << "===========================================================" << endl;

	//Parse input files
	cout << "Parsing Input... "; partTime = clock();
	LoadInput(inputLinesPath, inputPointsPath);			// Load / Parse input files
	cout << " Done (" << float(clock() - partTime) / CLOCKS_PER_SEC << "s)" << endl;

	// do something
	std::vector<Line*> OriginalLines = m_lines;
	int n = m_lines.size();
	int start = 0;
	int end = m_lines.size();

	//  Calculate AABBs for lines
	cout << "Calculating AABBs for lines... "; partTime = clock();
	CalculateAABBs(start, end);							// Run AABB calculation
	cout << " Done (" << float(clock() - partTime) / CLOCKS_PER_SEC << "s)" << endl;

	// Detect nearby control points and lines using the AABB
	// TODO : Speed up with sweepline
	cout << "Detecting nearby control points and lines... "; partTime = clock();
	Preprocess(start, end);								// Run Preprocessing
	cout << " Done (" << float(clock() - partTime) / CLOCKS_PER_SEC << "s)" << endl;

	//Run simplification algorithm VisvalingamWhyatt
	cout << "Simplifying... "; partTime = clock();
	VisvalingamWhyatt(start, end); 						// Run Simplification
	cout << " Done (" << float(clock() - partTime) / CLOCKS_PER_SEC << "s)" << endl;

	// Writing output file
	cout << "Writing output... "; partTime = clock();
	parser.WriteOutput(m_simplifiedLines, outputPath);	// Write output file
	cout << " Done (" << float(clock() - partTime) / CLOCKS_PER_SEC << "s)" << endl;

	// Total time

	for (std::vector<Line*>::const_iterator l1_it = m_lines.begin(), l1_e = m_lines.end(); l1_it < l1_e; l1_it++)
	{
		m_nrVertices += (*l1_it)->verts.size();
	}
	// Show vertices removed
	cout << "===========================================================" << endl;
	cout << "Amount removed " << m_nrVerticesRemoved << " removed of total " << m_nrVertices << endl;
	cout << "Finished in " << float(clock() - beginTime) / CLOCKS_PER_SEC << "s" << endl;
	cout << "===========================================================" << endl;

	int previouslyRemoved = 0;
	while (m_nrVerticesRemoved < verticesToRemove && previouslyRemoved < m_nrVerticesRemoved)
	{
		previouslyRemoved = m_nrVerticesRemoved;
		cout << "Amount to remove (" << verticesToRemove << ") not reached" << endl;
		cout << "===========================================================" << endl;
		cout << "Run second pass... " << endl;
		cout << "===========================================================" << endl;

		m_lines = m_simplifiedLines;
		m_simplifiedLines = vector<Line*>();
		n = m_lines.size();
		start = 0;
		end = m_lines.size();

		//  Calculate AABBs for lines
		cout << "Calculating AABBs for lines... "; partTime = clock();
		CalculateAABBs(start, end);							// Run AABB calculation
		cout << " Done (" << float(clock() - partTime) / CLOCKS_PER_SEC << "s)" << endl;

		// Detect nearby control points and lines using the AABB
		// TODO : Speed up with sweepline
		cout << "Detecting nearby control points and lines... "; partTime = clock();
		Preprocess(start, end);								// Run Preprocessing
		cout << " Done (" << float(clock() - partTime) / CLOCKS_PER_SEC << "s)" << endl;

		//Run simplification algorithm VisvalingamWhyatt
		cout << "Simplifying... "; partTime = clock();
		VisvalingamWhyatt(start, end); 						// Run Simplification
		cout << " Done (" << float(clock() - partTime) / CLOCKS_PER_SEC << "s)" << endl;

		// Writing output file
		cout << "Writing output... "; partTime = clock();
		parser.WriteOutput(m_simplifiedLines, outputPath);	// Write output file
		cout << " Done (" << float(clock() - partTime) / CLOCKS_PER_SEC << "s)" << endl;

		// Show vertices removed
		cout << "===========================================================" << endl;
		cout << "Amount removed " << m_nrVerticesRemoved << " removed of total " << m_nrVertices << endl;
		cout << "Finished in " << float(clock() - beginTime) / CLOCKS_PER_SEC << "s" << endl;
		cout << "===========================================================" << endl;
	}
	//TODO : Calculate grade?

	// Open results in OpenGL view
	MapSimplification mapSimplification(OriginalLines, m_simplifiedLines, m_points);

	if (mapSimplification.Init())
		mapSimplification.Run();
}

//-----------------------------------------------------------------------------------------------------------------//
void SimplificationAlgorithm::LoadInput(string inputLines, string inputPoints)
{
	Parser parser;
	m_lines = parser.ParseLineFile(inputLines);
	m_points = parser.ParsePointFile(inputPoints);
}


//-----------------------------------------------------------------------------------------------------------------//
void SimplificationAlgorithm::CalculateAABBs(int start, int end)
{
	// Calculate AABBs and detect nearby control points
	for (std::vector<Line*>::const_iterator l_it = m_lines.begin() + start, l_e = m_lines.begin() + end; l_it < l_e; l_it++)
	{
		// Calculate AABB
		(*l_it)->CalculateAABB();
	}
}

//-----------------------------------------------------------------------------------------------------------------//
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

//-----------------------------------------------------------------------------------------------------------------//
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
		while (deletionPossible && m_nrVerticesRemoved < m_nrVerticesToRemove)
		{
			//Calculate minimal effective area
			minArea = FLT_MAX;
			
			indexToRemove = -1;
			i = 1;
			for (std::vector<glm::vec2>::const_iterator v_it = simplifiedLine->verts.begin() + 1, v_e = simplifiedLine->verts.end(); v_it < v_e - 1; v_it++)
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
			{
				simplifiedLine->RemoveVertex(indexToRemove); 
				m_nrVerticesRemoved++;
			}
			else
				deletionPossible = false;
			count++;
		}
		m_simplifiedLines.push_back(simplifiedLine);
	}
}


float SimplificationAlgorithm::CalculateArea(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
{
	//Calculate triangle area for 3 points
	float area = std::fabs(a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2;
	return area;
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
	SimplificationAlgorithm simplificationAlgorithm;
	if (argc < 5) {
		// Tell the user how to run the program
		std::cerr << "Usage: " << argv[0] << " <amount_to_remove>" << " <input lines>" << " <input points>" << " <output>" << std::endl;
		/* "Usage messages" are a conventional way of telling the user
		* how to run a program if they enter the command incorrectly.
		*/
	//	return 1;
		simplificationAlgorithm.Simplify(5, "Data\\training_data5\\lines_out.txt", "Data\\training_data5\\points_out.txt", "output.txt");
		return 1;
	}

	simplificationAlgorithm.Simplify(stoi(argv[1]), argv[2], argv[3], argv[4]);
	return 0;
}