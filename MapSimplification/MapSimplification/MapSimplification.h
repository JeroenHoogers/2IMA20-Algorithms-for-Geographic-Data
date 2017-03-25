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


private:
	glm::vec2 center;
	glm::vec2 camPos;
	
};