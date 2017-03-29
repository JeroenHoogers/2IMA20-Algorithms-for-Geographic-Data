#include "VisvalingamWhyatt.h"



VisvalingamWhyatt::VisvalingamWhyatt()
{
}


VisvalingamWhyatt::~VisvalingamWhyatt()
{
}

float  VisvalingamWhyatt::CalculateArea(glm::vec2 *a, glm::vec2 *b, glm::vec2 *c)
{
	//Calculate triangle area for 3 points
	float area = std::abs(a->x * (b->y - c->y) + b->x * (c->y - a->y) + c->x * (a->y - b->y)) / 2;
	return area;
}

std::vector<Line> VisvalingamWhyatt::Algorithm(std::vector<Line*> lines, int number_to_keep)
{
	std::vector<Line> simplifiedLines;

	//for (int i = 0; i < lines->size(); i++)
	Line* l = nullptr;
	glm::vec2 v;
	glm::vec2 v_next;
	glm::vec2 v_prev;
	for (std::vector<Line*>::const_iterator l_it = lines.begin(), l_e = lines.end(); l_it < l_e; l_it++)
	{
		l = *l_it;
		//Create simplified Line
		std::vector<glm::vec2> vertices = l->verts;//(*lines)[i].verts;
		Line simplifiedLine = Line(vertices);

		//Find point to remove from line
		int indexToRemove;
		int i = 0;
		while (i < number_to_keep)
		{
			//Calculate minimal effective area
			float minArea = CalculateArea(&simplifiedLine.verts[0], &simplifiedLine.verts[1], &simplifiedLine.verts[2]);
			for (std::vector<glm::vec2>::const_iterator v_it = simplifiedLine.verts.begin() + 2, v_e = simplifiedLine.verts.end(); v_it < v_e - 1; v_it++) //int i = 2; i < simplifiedLine.verts.size() - 1; i++)
			{
				v = *v_it;
				v_next = *(v_it + 1);
				v_prev = *(v_it - 1);
				
				//TODO : Only consider possible simplifications
					//TODO : Check if simplification would intersect another line
					//TODO : Check if point in Area
				float curArea = CalculateArea(&v_prev, &v, &v_next);
				if (minArea > curArea)
				{
					minArea = curArea;
					indexToRemove = i;
				}
			}
			simplifiedLine.RemoveVertex(indexToRemove);
			i++;
		}
		simplifiedLines.push_back(simplifiedLine);
	}
	return simplifiedLines;
}