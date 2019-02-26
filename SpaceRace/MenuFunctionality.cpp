#include "MenuFunctionality.h"



void MenuFunctionality::ScrollUp() {
	curChosen = 0;
	int nextY = startUIY - (curChosen *UIOffset);
	nextY = Math::Clamp<int>(nextY, 0, 1000);
	highlightedPanel->SetPosition(Vector3(highlightedPanel->GetPosition().x, nextY, 1));
}

void MenuFunctionality::ScrollDown() {
	curChosen = 1;
	int nextY = startUIY - (curChosen *UIOffset);
	nextY = Math::Clamp<int>(nextY, 0, (allButtons.size() - 1)* UIOffset);
	highlightedPanel->SetPosition(Vector3(highlightedPanel->GetPosition().x, nextY, 1));
}
void MenuFunctionality::InitialiseMenu(short int _sX, short _sY, float _offX,float _offY) {
	startUIX = _sX;
	startUIY = _sY;
	UIOffset = _offY * 0.5f;

	highlightedPanel->SetPosition(Vector3(_sX - _offX * 2, _sY , 1));
	highlightedPanel->SetScale(Vector3(_offX * 0.1f, 0.1f, 1));

	for (int i = 0; i < allButtons.size(); i++) {
		allButtons[i]->SetPosition(Vector3(_sX, _sY - (_offY  * (i) * 5.5f), 1));
		allButtons[i]->SetScale(Vector3(_offX, _offY,1));
	}
}

MenuFunctionality::MenuFunctionality()
{
	//InitialiseMenu(0, 0, 10);
}

int MenuFunctionality::GetChosen() const {
	return curChosen;
}

MenuFunctionality::MenuFunctionality(Mesh* curPanel)
{
	highlightedPanel->curObject = curPanel;
	InitialiseMenu(0, 0, 1.0f,1.0f);
}
MenuFunctionality::MenuFunctionality(Mesh* curPanel,short _sX,short _sY, float _offX, float _offY)
{
	highlightedPanel->curObject = curPanel;
	InitialiseMenu(_sX, _sY, _offX,_offY);
}

std::vector<GameObject*> MenuFunctionality::GetAllButtons() {
	return allButtons;
}

void MenuFunctionality::SetHighlight(GameObject* _high) {
	highlightedPanel = _high;
}

void MenuFunctionality::AddButton(GameObject* _button) {
	allButtons.push_back(_button);
}

GameObject* MenuFunctionality::GetHighLight() {
	return highlightedPanel;
}

MenuFunctionality::~MenuFunctionality()
{
}
