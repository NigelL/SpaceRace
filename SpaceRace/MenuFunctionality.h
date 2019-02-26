#ifndef MENUFUNCTIONALITY
#define MENUFUNCTIONALITY
#include "GameObject.h"
#include <vector>


class MenuFunctionality
{
private:
	short int startUIX, startUIY,UIOffset;
	int curChosen = 0;
	GameObject* highlightedPanel;
	std::vector<GameObject*> allButtons;

public:
	
	

	void ScrollDown();
	void ScrollUp();
	int GetChosen() const;
	GameObject* GetHighLight();
	std::vector<GameObject*> GetAllButtons();

	void InitialiseMenu(short int _sX, short _sY, float _offX, float _offY);
	void SetHighlight(GameObject* _high);
	void AddButton(GameObject* _button);

	MenuFunctionality();
	MenuFunctionality(Mesh* curPanel);

	MenuFunctionality(Mesh* curPanel, short _sX, short _sY, float _offX, float _offY);

	~MenuFunctionality();
};
#endif

