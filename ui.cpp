#include "main.h"
#include "gameObject.h"
#include "ui.h"

#pragma region ui_method

UI::UI() :uiType(E_UIType::UI_None), uiContentType(E_UIContent::UIContent_None) {
	cellType = E_CellType::CellType_UI;
	appearTime = -1;
	appearTimePass = 0.0f;
}

UI::UI(E_UIType uiT) : uiType(uiT), uiContentType(E_UIContent::UIContent_None) {
	cellType = E_CellType::CellType_UI;
	appearTime = -1;
	appearTimePass = 0.0f;
}

UI::~UI() {

}

void UI::DoUpdate(float deltatime)
{
	if (enable) {
		if (appearTime > 0) {
			if (appearTimePass > appearTime) {
				//recycle ui
				ShowUI(false, -1.0f, false);
			}
			else {
				appearTimePass += deltatime;
			}
		}
	}

	BaseCell::DoUpdate(deltatime);
}

void UI::ShowUI(bool show, float appearT, bool resetTimer)
{
	if (enable == show && resetTimer == false) {
		return;
	}

	this->SetState(show);
	if (resetTimer == true) {
		appearTime = appearT;
	}
	appearTimePass = 0.0f;
}

void UI::UpdateState()
{
}

void UI::SetUIContentType(E_UIContent contentType)
{
	uiContentType = contentType;
}

E_UIType UI::GetUIType()
{
	return uiType;
}

E_UIContent UI::GetUIContentType()
{
	return uiContentType;
}

#pragma region ui_group

void UIGroup::UpdateChildPos()
{
	vector3s baseScale = this->GetTransform()->GetScale();

	//update child pos
	vector3s curPos = this->GetTransform()->GetPos();
	vector3s  rot = this->GetTransform()->GetRot();
	bool hasRot = ((int)rot.z) != 0 ? true : false;

	float tempX = 0.0f;
	float tempY = 0.0f;
	float cosRotZ = hasRot ? cosf(rot.z * DEG2RAD) : 0.0f;
	float sinRotZ = hasRot ? sinf(rot.z * DEG2RAD) : 0.0f;

	std::map<int, UI*>::iterator iter;
	for (iter = childDic.begin(); iter != childDic.end(); iter++) {
		if (iter->second == nullptr)continue;

		iter->second->Reset();//reset first

		vector3s offset = vector3s();
		if (childOffsets.find(iter->first) != childOffsets.end()) {
			offset = childOffsets[iter->first];
			offset.x *= baseScale.x;
			offset.y *= baseScale.y;
		}

		//rotate
		//rot z
		if (hasRot) {
			tempX = offset.x;
			tempY = offset.y;

			offset.x = (tempX * cosRotZ - tempY * sinRotZ);
			offset.y = (tempX * sinRotZ + tempY * cosRotZ);

		}

		vector3s curScale = iter->second->GetTransform()->GetScale();
		curScale.x *= baseScale.x;
		curScale.y *= baseScale.y;
		iter->second->SetScale(curScale);
		iter->second->SetRot(rot);
		iter->second->SetPos(curPos + offset,true);

		iter->second->UpdateState();
	}
}

UIGroup::UIGroup() :UI(E_UIType::UI_Group), deleteChild(true) {
	childDic.clear();
	childOffsets.clear();
}

UIGroup::~UIGroup() {
	if (deleteChild == true) {
		if (childDic.size() != 0) {
			std::map<int, UI*>::iterator iter;
			for (iter = childDic.begin(); iter != childDic.end(); iter++) {
				if (iter->second == nullptr)continue;
				delete iter->second;
			}
		}
	}
	childDic.clear();
	childOffsets.clear();
}

bool UIGroup::RegisterChild(int id, UI* child, vector3s offset) {
	if (child == nullptr)return false;
	if (childDic.find(id) != childDic.end()) {
		return false;
	}
	childDic[id] = child;
	childOffsets[id] = offset;

	child->ShowUI(true, -1, true);//default state
	child->RecordCurState();
	return true;
}

void UIGroup::ClearChild()
{
	if (childDic.size() != 0) {
		std::map<int, UI*>::iterator iter;
		for (iter = childDic.begin(); iter != childDic.end(); iter++) {
			if (iter->second == nullptr)continue;
			if (deleteChild == true) {
				delete iter->second;
			}
			else {
				iter->second->ShowUI(false, -1, true);
			}
		}
	}
	childDic.clear();
	childOffsets.clear();
}

void UIGroup::DoUpdate(float deltatime)
{
	if (enable == true) {
		UpdateChildPos();
	}

	UI::DoUpdate(deltatime);
}

void UIGroup::UpdateState()
{
	UpdateChildPos();
}

void UIGroup::ShowUI(bool show, float appearT, bool resetTimer)
{
	if (show == true) {
		UpdateState();
	}
	if (childDic.size() != 0) {
		std::map<int, UI*>::iterator iter;
		for (iter = childDic.begin(); iter != childDic.end(); iter++) {
			if (iter->second == nullptr)continue;
			iter->second->ShowUI(show, -1, true);
		}
	}
	UI::ShowUI(show, appearT, resetTimer);
}

std::map<int, UI*>& UIGroup::GetChildDic()
{
	return childDic;
}

void UIGroup::SetChildOffset(int childID, vector3s offset)
{
	if (childDic.find(childID) != childDic.end() && childDic[childID] != nullptr) {
		childOffsets[childID] = offset;
	}
}

#pragma endregion ui_group

#pragma region ui_sprite

UISprite::UISprite() :UI(E_UIType::UI_Sprite) {

}

UISprite::~UISprite() {

}

void UISprite::SetSprite(E_UIContent content) {

}

#pragma endregion ui_sprite

#pragma region ui_word

void UIWord::UpdateWordPos()
{
	int startIndex = 0;
	if (alignmentMode == E_UIWordAlignment::UIWordAlignment_Center) {
		startIndex = (UI_WORD_CHILD_COUNT / 2) - (int)(curWordCount / 2.0f + 0.5f);
	}
	else {
		startIndex = alignmentMode == E_UIWordAlignment::UIWordAlignment_Left ? 0 : UI_WORD_CHILD_COUNT - curWordCount;
	}
	if (startIndex < 0 || startIndex >= UI_WORD_CHILD_COUNT)return;//error;

	vector3s scale = this->GetTransform()->GetScale();

	//calc startPos
	vector3s wordStartPos = this->GetTransform()->GetPos();
	int offsetWordCount = (int)(UI_WORD_CHILD_COUNT * 0.5f + 0.5f);
	wordStartPos.x -= (fontSize + wordInterval) * ((float)offsetWordCount - 0.5f);

	for (int index = UI_WORD_CHILD_COUNT - 1; index >= 0; index--) {
		if (index < startIndex || index >= startIndex + curWordCount) {
			wordArray[index].SetState(false);
			continue;
		}

		vector3s newPos = wordStartPos;
		newPos.x += (fontSize + wordInterval * scale.x) * index;
		//newPos.y += (index * 2.0f);

		wordArray[index].SetPos(newPos,true);
		wordArray[index].SetState(true);
	}
}

void UIWord::UpdateWord(bool setEnable)
{
	int startIndex = 0;
	if (alignmentMode == E_UIWordAlignment::UIWordAlignment_Center) {
		startIndex = (UI_WORD_CHILD_COUNT / 2) - (int)(curWordCount / 2.0f + 0.5f);
	}
	else {
		startIndex = alignmentMode == E_UIWordAlignment::UIWordAlignment_Left ? 0 : UI_WORD_CHILD_COUNT - curWordCount;
	}

	if (startIndex < 0 || startIndex >= UI_WORD_CHILD_COUNT)return;//error;

	vector3s baseScale = this->GetTransform()->GetScale();

	int wordCodeIndex = UI_WORD_CHILD_COUNT - 1;

	//calc startPos
	for (int index = UI_WORD_CHILD_COUNT - 1; index >= 0; index--) {
		if (index < startIndex || index >= (startIndex + curWordCount)) {
			wordArray[index].SetState(false);
			continue;
		}

		if (wordCodeIndex < 0 || wordCodes[wordCodeIndex] < 0) {
			wordArray[index].SetState(false);
			wordCodeIndex--;
			continue;
		}

		wordArray[index].Reset();

		DrawData* tempData = wordArray[index].GetDrawData();
		if (tempData == nullptr)continue;
		tempData->SetShaderIntParam(1, GetNumWordTexIndex(wordCodes[wordCodeIndex]));
		wordCodeIndex--;

		vector3s curScale = wordArray[index].GetTransform()->GetScale();
		curScale.x *= baseScale.x;
		curScale.y *= baseScale.y;
		wordArray[index].GetTransform()->SetScale(curScale);

		wordArray[index].SetState(setEnable);
	}
}

UIWord::UIWord() :UI(E_UIType::UI_Word), alignmentMode(E_UIWordAlignment::UIWordAlignment_Center), wordInterval(0.0f), curWordCount(0) {
	fontSize = DEFAULT_FONT_SIZE;
	for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
		wordArray[index].RecordCurState();
	}
	Clear();
}

UIWord::~UIWord() {
}

void UIWord::SetWord(const std::string& words) {
	if (words.size() == 0)return;
	const char* strArray = words.c_str();

	int wordSize = (int)words.size();
	if (wordSize == 0)return;
	int charArrayIndex = wordSize - 1;
	int wordCount = 0;
	for (int index = UI_WORD_CHILD_COUNT - 1; index >= 0; index--) {
		if (wordCount >= UI_WORD_CHILD_COUNT || charArrayIndex < 0) {
			break;
		}

		wordCodes[index] = GetNumWordTexIndexWithAscCode(words[charArrayIndex]);
		charArrayIndex--;
		wordCount++;
	}
	curWordCount = wordCount;

	UpdateWord(true);
	UpdateWordPos();

}

void UIWord::SetWord(int numWord) {
	Clear();
	int tempNum = numWord;
	int numCount = 0;
	for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
		int tempDiv = (int)pow(10, index);
		if (index == 0) {
			int tempValue = tempNum % 10;
			wordCodes[UI_WORD_CHILD_COUNT - index - 1] = tempValue;
			numCount++;
		}
		else {
			if (tempNum < tempDiv) {
				break;
			}
			int tempValue = tempNum / tempDiv;
			tempValue %= 10;
			wordCodes[UI_WORD_CHILD_COUNT - index - 1] = tempValue;
			numCount++;
		}
	}

	curWordCount = numCount;

	int startIndex = (UI_WORD_CHILD_COUNT / 2) - (int)(curWordCount / 2.0f + 0.5f);
	if (startIndex < 0 || startIndex >= UI_WORD_CHILD_COUNT)return;//error;

	UpdateWord(true);
	UpdateWordPos();
}

void UIWord::DoUpdate(float deltatime)
{
	if (enable) {
		//update child ui pos
		UpdateWordPos();
	}

	UI::DoUpdate(deltatime);
}

void UIWord::UpdateState()
{
	UpdateWord(true);
	UpdateWordPos();
}

void UIWord::ShowUI(bool show, float appearT, bool resetTimer)
{
	//update pos
	if (show) {
		UpdateWord(true);
		UpdateWordPos();
	}
	UI::ShowUI(show, appearT, resetTimer);
}

void UIWord::SetWordInterval(float interval)
{
	wordInterval = interval;
}

void UIWord::SetAlignment(E_UIWordAlignment alignmentT)
{
	alignmentMode = alignmentT;
}

UISprite* UIWord::GetSpriteArray()
{
	return wordArray;
}

void UIWord::Clear()
{
	for (int index = 0; index < UI_WORD_CHILD_COUNT; index++) {
		wordArray[index].Reset();
		wordArray[index].SetState(false);
	}

	curWordCount = 0;
	memset(wordCodes, -1, sizeof(int) * UI_WORD_CHILD_COUNT);
}


#pragma endregion ui_word


#pragma endregion ui_method
