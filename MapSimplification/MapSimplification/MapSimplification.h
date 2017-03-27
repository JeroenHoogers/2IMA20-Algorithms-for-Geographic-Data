#include "GLApp.h"
#include "Parser.h"
#include <ctime>

class MapSimplification : public GLApp
{
public:
	MapSimplification();
	~MapSimplification();

	// overrides
	bool Init() override;
	//void Update(float dt) override;
	//void Render() override;

	

	void SetView() override;
	void DrawScene() override;
	void DrawGUI() override;
	void DrawBackground();

	void DrawOriginalPanel();
	void DrawSimplifiedPanel();

private:
	glm::vec2 center;
	glm::vec2 camPos;

	vector<vector<glm::vec2>> m_lines;
	vector<glm::vec2> m_points;
	
};