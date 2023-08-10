#include "gameLogic.h"
#include "gameObject.h"
#include "component.h"
#include "sceneObjManager.h"
#include "effectManager.h"
#include "battleManager.h"
#include "uiManager.h"
#include "ui.h"
#include "player.h"

#pragma region component_method

Component::Component() :componentType(E_Component::Component_None), enable(true), parent(nullptr)
{

}

Component::~Component()
{
	parent = nullptr;
}

Component::Component(BaseCell* p, E_Component cType) :enable(true), parent(p)
{

}

BaseCell* Component::GetParent()
{
	return parent;
}

void Component::SetParent(BaseCell* p)
{
	parent = p;
}


#pragma region Transform_method

Transform::Transform() :m_pos(vector3s(0.0f, 0.0f, 0.0f)), m_rot(vector3s(0.0f, 0.0f, 0.0f)), m_scale(vector3s(1.0f, 1.0f, 1.0f)), prePos(vector3s(0.0f, 0.0f, 0.0f)),
defaultPos(vector3s(0.0f, 0.0f, 0.0f)), defaultRot(vector3s(0.0f, 0.0f, 0.0f)), defaultScale(vector3s(1.0f, 1.0f, 1.0f))
{
	tempPosSign = false;
	componentType = E_Component::Component_Transform;
}

Transform::Transform(vector3s pos, vector3s rot, vector3s scale) : m_pos(pos), m_rot(rot), m_scale(scale), prePos(pos)
{
	tempPosSign = false;
	componentType = E_Component::Component_Transform;
}

void Transform::DoInit()
{
	Component::DoInit();
}

void Transform::UnInit()
{
}

void Transform::SetPos(vector3s p, bool updatePrePos)
{
	updatePrePos = true;
	if (updatePrePos) {
		prePos = m_pos;
	}
	else {
		tempPosSign = true;
		tempPos = m_pos;
	}
	m_pos = p;
}

void Transform::Copy(Component& src)
{
	if (src.GetComponentType() != E_Component::Component_Transform) {
		//error
		return;
	}
	try {
		Transform* srcT = (Transform*)(&src);

		this->parent = srcT->parent;
		this->m_pos = srcT->m_pos;
		this->m_rot = srcT->m_rot;
		this->m_scale = srcT->m_scale;

		if (this->parent != nullptr) {
			this->parent->RegisterComponent(E_Component::Component_Transform, this, true);
		}
	}
	catch (std::exception e) {

	}
}

void Transform::RecordCurState()
{
	prePos = m_pos;
	defaultPos = m_pos;
	defaultRot = m_rot;
	defaultScale = m_scale;
}

void Transform::Reset()
{
	prePos = defaultPos;
	m_pos = defaultPos;
	m_rot = defaultRot;
	m_scale = defaultScale;
}

bool Transform::AdjustPosition(BaseCell* cell, vector3s center, vector2s rect, bool inner)
{
	if (cell == nullptr)return false;

	vector3s pos = cell->GetTransform()->GetPos();
	vector2s size = vector2s(cell->GetTransform()->GetScale().x, cell->GetTransform()->GetScale().y) * cell->GetDrawData()->GetSize();

	if (inner) {
		bool adjustSign = false;
		float border = center.x - rect.x * 0.5f;
		if (pos.x - size.x * 0.5f < border) {
			pos.x = border + size.x * 0.5f;
			adjustSign = true;
		}

		border = center.x + rect.x * 0.5f;
		if (pos.x + size.x * 0.5f > border) {
			pos.x = border - size.x * 0.5f;
			adjustSign = true;
		}

		border = center.y - rect.y * 0.5f;
		if (pos.y - size.y * 0.5f < border) {
			pos.y = border + size.y * 0.5f;
			adjustSign = true;
		}
		border = center.y + rect.y * 0.5f;
		if (pos.y + size.y * 0.5f > border) {
			pos.y = border - size.y * 0.5f;
			adjustSign = true;
		}

		cell->SetPos(pos, true);

		return adjustSign;
	}
	else {

	}

	return false;
}

void Transform::AdjustPositionDirection(vector3s& pos, vector2s size, vector3s rectPos, vector2s rectSize, bool isHorizontal)
{

	if (isHorizontal) {
		float border = rectPos.x + rectSize.x * 0.5f;//right
		if (pos.x + size.x * 0.5f > border) {
			pos.x = border - size.x * 0.5f;
		}

		border = rectPos.x - rectSize.x * 0.5f;//left
		if (pos.x - size.x * 0.5f < border) {
			pos.x = border + size.x * 0.5f;
		}
	}
	else {
		float border = rectPos.y + rectSize.y * 0.5f;//up
		if (pos.y + size.y * 0.5f > border) {
			pos.y = border - size.y * 0.5f;
		}

		border = rectPos.y - rectSize.y * 0.5f;//down
		if (pos.y - size.y * 0.5f < border) {
			pos.y = border + size.y * 0.5f;
		}
	}

}

void Transform::AdjustZPos(BaseCell* cell)
{
	if (cell == nullptr)return;

	vector3s tempPos = cell->GetTransform()->GetPos();
	E_CellType cellType = cell->GetCellType();
	if (cellType == E_CellType::CellType_Player || cellType == E_CellType::CellType_Light) {
		if (GameMgr::GetInstance()->GetCurGameState() != E_GameState::GameState_Game_Normal) {
			tempPos.z = Z_AXIS_OFFSET_TITLE_PLAYER;
		}
		else {
			tempPos.z = Z_AXIS_OFSFET_PLAYER;//title
		}
	}
	else if (cellType == E_CellType::CellType_Bullet || cellType == E_CellType::CellType_Effect) {
		tempPos.z = Z_AXIS_OFFSET_SCENE_SPRITE;
	}
	else if (cellType == E_CellType::CellType_Enemy) {
		tempPos.z = Z_AXIS_OFFSET_SCENE_ENEMY;
	}
	else if (cellType == E_CellType::CellType_Grass) {
		tempPos.z = Z_AXIS_OFFSET_BG_ITEM;
	}
	else if (cellType == E_CellType::CellType_UI) {
		E_UIContent tempContent = ((UI*)cell)->GetUIContentType();
		if ((int)tempContent >= 30 && (int)tempContent <= 40) {//menu item
			tempPos.z = Z_AXIS_OFFSET_MENU_ITEM;
		}
		else if (tempContent == E_UIContent::UIContent_Bg) {
			tempPos.z = Z_AXIS_OFFSET_BG;
		}
		else if (tempContent == E_UIContent::UIContent_Img_Title_Selecotr) {
			tempPos.z = Z_AXIS_OFFSET_SELECTOR;
		}
		else if (tempContent == E_UIContent::UIContent_Card_Bg) {
			tempPos.z = Z_AXIS_OFFSET_CARD;
		}
		else {
			tempPos.z = Z_AXIS_OFFSET_SCENE_MENU_ITEM;
		}
	}
	else if (cellType == cell->GetCellType() || cellType == E_CellType::CellType_GuideUI) {
		tempPos.z = Z_AXIS_OFFSET_BASE_GUIDE;
	}
	else {
		tempPos.z = Z_AXIS_OFFSET_SCENE_MENU_ITEM;
	}
	//tempPos.z = 0.0f;
	cell->SetPos(tempPos, false);
}



#pragma endregion Transform_method


#pragma region Collider_method

ColliderData::ColliderData() :id(-1), parent(nullptr), isTrigger(false),
colliderType(E_ColliderType::Collider_None), size(vector2s(0.0f, 0.0f)), offset(vector3s(0.0f, 0.0f, 0.0f))
{

}

/// <summary>
/// データをほかのインスタンスからコピペする
/// </summary>
/// <param name="src"></param>
ColliderData::ColliderData(const ColliderData& src)
{
	this->id = src.id;
	//this->parent = src.parent;
	this->parent = nullptr;
	this->isTrigger = src.isTrigger;
	this->colliderType = src.colliderType;
	this->size = src.size;
	this->offset = src.offset;

}

ColliderData::ColliderData(int i, Collider* p, bool trigger, E_ColliderType cType, vector2s s, vector3s o) :id(i), parent(p), isTrigger(trigger)
, colliderType(cType), size(s), offset(o) {

}

ColliderData::~ColliderData()
{
	parent = nullptr;//削除する時に所有者ポインターの変数をヌルに設定する必要がある
}

void ColliderData::Copy(const ColliderData& src)
{
	this->id = src.id;
	this->parent = src.parent;
	this->isTrigger = src.isTrigger;
	this->colliderType = src.colliderType;
	this->size = src.size;
	this->offset = src.offset;
}

Collider::Collider()
{
	componentType = E_Component::Component_Collider;//コンポーネントの種類を設定する
	this->colliderDataArray.clear();//コライダー配列を初期化する
}

Collider::Collider(const Collider& src) :Component(src.parent, E_Component::Component_Collider)
{
	this->colliderDataArray.clear();//コライダー配列を初期化する
	int arraySize = (int)src.colliderDataArray.size();//コライダーのデータをコピーする
	for (int index = 0; index < arraySize; index++) {
		if (src.colliderDataArray[index] != nullptr) {
			this->colliderDataArray.push_back(new ColliderData(*(src.colliderDataArray[index])));
		}
	}

	if (arraySize != this->colliderDataArray.size()) {
		//error
	}
}

Collider::~Collider()
{
	/// <summary>
	/// すべてのコライダーを削除する
	/// </summary>
	int arrayLength = (int)colliderDataArray.size();
	for (int index = 0; index < arrayLength; index++) {
		if (colliderDataArray[index] == nullptr)continue;
		delete colliderDataArray[index];
	}

	colliderDataArray.clear();

}

void Collider::DoInit()
{
	Component::DoInit(); // 親クラスの初期化関数を呼び出す
}

void Collider::UnInit()
{
}

/// <summary>
/// コライダーを取得する
/// </summary>
/// <param name="id">　コライダーのID　</param>
/// <returns></returns>
ColliderData* Collider::GetColliderData(int id)
{

	int size = (int)colliderDataArray.size();
	for (int index = 0; index < size; index++) {
		if (colliderDataArray[index] == nullptr)continue;

		if (colliderDataArray[index]->id == id) {
			return colliderDataArray[index];
		}
	}

	return nullptr;
}

/// <summary>
/// コライダーデータを追加する
/// </summary>
/// <param name="data"></param>
void Collider::AddColliderData(ColliderData* data) {
	if (data == nullptr)return;
	colliderDataArray.push_back(data);
}

/// <summary>
/// コライダーデータを全部削除する
/// </summary>
void Collider::ClearColliderDataArray()
{
	int arrayLength = (int)colliderDataArray.size();
	for (int index = 0; index < arrayLength; index++) {
		if (colliderDataArray[index] == nullptr)continue;
		delete colliderDataArray[index];
	}

	colliderDataArray.clear();
}

void Collider::Copy(const Component& src)
{
	if (src.GetComponentType() != E_Component::Component_Collider)return;
	Collider* tempC = (Collider*)(&src);
	int colliderDataSize = tempC->GetColliderDataArraySize();
	if (tempC == nullptr || colliderDataSize == 0)return;
	ClearColliderDataArray();
	for (int index = 0; index < colliderDataSize; index++) {
		if (tempC->colliderDataArray[index] != nullptr) {
			this->colliderDataArray.push_back(new ColliderData(*tempC->colliderDataArray[index]));
		}
	}
}

/// <summary>
/// コライダーのサイズを設定する
/// </summary>
/// <param name="size"></param>
/// <param name="colliderDataIndex">　</param>
void Collider::SetColliderSize(vector2s size, int colliderDataIndex)
{
	if (colliderDataArray.size() == 0 || colliderDataArray.size() <= colliderDataIndex || colliderDataArray[colliderDataIndex] == nullptr)return;
	colliderDataArray[colliderDataIndex]->size = size;
}

/// <summary>
/// 当たり判定（円と円、円と四角形、四角形と四角形）
/// </summary>
/// <param name="cell_1">　一番目のゲームオブジェクト</param>
/// <param name="cell_2">　二番目のゲームオブジェクト</param>
/// <returns></returns>
bool Collider::CollisionDetect(BaseCell* cell_1, BaseCell* cell_2)
{
	if (cell_1 == nullptr || cell_2 == nullptr)return false;
	if (cell_1 == cell_2)return true;

	//コライダーコンポーネントを取得
	Collider* c_1 = (Collider*)cell_1->GetComponent(E_Component::Component_Collider);
	Collider* c_2 = (Collider*)cell_2->GetComponent(E_Component::Component_Collider);

	if (c_1 == nullptr || c_2 == nullptr)return false;
	if (!c_1->enable || !c_2->enable)
		return false;

	//コライダーデータの配列を取得
	std::vector<ColliderData*> c_array_1 = c_1->GetColliderDataArray();
	std::vector<ColliderData*> c_array_2 = c_2->GetColliderDataArray();
	int size_1 = (int)c_array_1.size();
	int size_2 = (int)c_array_2.size();
	if (size_1 == 0 || size_2 == 0)return false;

	for (int index_1 = 0; index_1 < size_1; index_1++) {
		if (c_array_1[index_1] == nullptr)continue;

		for (int index_2 = 0; index_2 < size_2; index_2++) {

			if (c_array_2[index_2] == nullptr)continue;

			if (c_array_1[index_1]->colliderType == c_array_2[index_2]->colliderType) {//コライダー種類が同じの場合
				if (c_array_1[index_1]->colliderType == E_ColliderType::Collider_Rect) {//四角形と四角形
					if (RectCollisionDetect(*(cell_1->GetTransform()), *(cell_2->GetTransform()), *c_array_1[index_1], *c_array_2[index_2])) {
						return true;
					}
				}
				else {//円と円
					if (CircleCollisionDetect(*(cell_1->GetTransform()), *(cell_2->GetTransform()), *c_array_1[index_1], *c_array_2[index_2])) {
						return true;
					}
				}
			}
			else {//コライダー種類が違う場合　‐　円と四角形
				if (RectCircleCollisionDetect(*(cell_1->GetTransform()), *(cell_2->GetTransform()), *c_array_1[index_1], *c_array_2[index_2])) {
					return true;
				}
			}
		}
	}


	return false;
}

bool Collider::CollisionDetect(BaseCell* cell_1, Transform* transform_2, Collider* c_2) {
	if (cell_1 == nullptr || transform_2 == nullptr || c_2 == nullptr)return false;

	Collider* c_1 = (Collider*)cell_1->GetComponent(E_Component::Component_Collider);

	if (c_1 == nullptr)return false;

	std::vector<ColliderData*> c_array_1 = c_1->GetColliderDataArray();
	std::vector<ColliderData*> c_array_2 = c_2->GetColliderDataArray();
	int size_1 = (int)c_array_1.size();
	int size_2 = (int)c_array_2.size();
	if (size_1 == 0 || size_2 == 0)return false;

	for (int index_1 = 0; index_1 < size_1; index_1++) {
		if (c_array_1[index_1] == nullptr)continue;

		for (int index_2 = 0; index_2 < size_2; index_2++) {

			if (c_array_2[index_2] == nullptr)continue;

			if (c_array_1[index_1]->colliderType == c_array_2[index_2]->colliderType) {
				if (c_array_1[index_1]->colliderType == E_ColliderType::Collider_Rect) {
					if (RectCollisionDetect(*(cell_1->GetTransform()), *transform_2, *c_array_1[index_1], *c_array_2[index_2])) {
						return true;
					}
				}
				else {
					if (CircleCollisionDetect(*(cell_1->GetTransform()), *transform_2, *c_array_1[index_1], *c_array_2[index_2])) {
						return true;
					}
				}
			}
			else {
				if (RectCircleCollisionDetect(*(cell_1->GetTransform()), *transform_2, *c_array_1[index_1], *c_array_2[index_2])) {
					return true;
				}
			}
		}
	}
	return false;
}

/// <summary>
/// 円と四角形の当たり判定
/// </summary>
/// <param name="transform_1">　一番目のゲームオブジェクトのトランスフォーマーデータ</param>
/// <param name="transform_2">　二番目のゲームオブジェクトのトランスフォーマーデータ</param>
/// <param name="data_1">一番目のゲームオブジェクトのコライダーデータ</param>
/// <param name="data_2">二番目のゲームオブジェクトのコライダーデータ</param>
/// <returns>　当たるかどうか</returns>
bool Collider::RectCircleCollisionDetect(Transform& transform_1, Transform& transform_2, ColliderData& data_1, ColliderData& data_2)
{
	ColliderData& rectC = data_1.colliderType == E_ColliderType::Collider_Rect ? data_1 : data_2;
	Transform& rectT = data_1.colliderType == E_ColliderType::Collider_Rect ? transform_1 : transform_2;
	ColliderData& circleC = data_1.colliderType == E_ColliderType::Collider_Circle ? data_1 : data_2;
	Transform& circleT = data_1.colliderType == E_ColliderType::Collider_Circle ? transform_1 : transform_2;

	//situation 1
	vector3s rectCenter = vector3s(0.0f, 0.0f, 0.0f);
	float rectWidth = rectC.size.x * rectT.GetScale().x;
	float rectHeight = rectC.size.y * rectT.GetScale().y;


	vector3s rot = vector3s(0.0f, 0.0f, 0.0f);
	if (rectT.GetParent() != nullptr && rectT.GetParent()->GetCellType() == E_CellType::CellType_Bullet) {
		if (((Bullet*)rectT.GetParent())->GetBulletType() == E_BulletType::BulletType_Laser) {
			rot = ((Bullet_Laser*)rectT.GetParent())->GetCorrectRot();
		}
		else {
			rot = rectT.GetRot();
		}
	}
	else {
		rot = rectT.GetRot();
	}
	

	//get rectStartPoint(rotated)
	vector3s rectStartPoint = rectCenter;
	rectStartPoint.x = rectCenter.x - rectWidth * 0.5f;
	rectStartPoint.y = rectCenter.y - rectHeight * 0.5f;

	float tempX = rectStartPoint.x;
	float tempY = rectStartPoint.y;
	if ((int)(rot.z) != 0) {
		rectStartPoint.x = tempX * cosf(rot.z * DEG2RAD) - tempY * sinf(rot.z * DEG2RAD);
		rectStartPoint.y = tempX * sinf(rot.z * DEG2RAD) + tempY * cosf(rot.z * DEG2RAD);
	}

	rectStartPoint.x += rectC.offset.x + rectT.GetPos().x;
	rectStartPoint.y += rectC.offset.y + rectT.GetPos().y;

	float radius = circleC.size.x * circleT.GetScale().x * 0.5f;

	vector3s circleCenter = circleT.GetPos() + circleC.offset;
	vector3s rect2Circle = circleCenter - rectStartPoint;

	//rotate to rect coordinate
	if ((int)(rot.z) != 0) {
		tempX = rect2Circle.x;
		tempY = rect2Circle.y;
		rect2Circle.x = tempX * cosf(-rot.z * DEG2RAD) - tempY * sinf(-rot.z * DEG2RAD);
		rect2Circle.y = tempX * sinf(-rot.z * DEG2RAD) + tempY * cosf(-rot.z * DEG2RAD);
	}

	//collision detect
	rectStartPoint.x = 0.0f;//set startPoint to originPoint
	rectStartPoint.y = 0.0f;

	vector3s nearestPoint;
	nearestPoint.z = 0.0f;
	nearestPoint.x = s_max(rectStartPoint.x, s_min(rect2Circle.x, rectStartPoint.x + rectWidth));
	nearestPoint.y = s_max(rectStartPoint.y, s_min(rect2Circle.y, rectStartPoint.y + rectHeight));

	if (VecDist(rect2Circle, nearestPoint) < radius) {
		return true;
	}
	else {
		return false;
	}
}

bool Collider::RectCollisionDetect(Transform& transform_1, Transform& transform_2, ColliderData& data_1, ColliderData& data_2)
{
	vector3s rot_1 = vector3s(0.0f, 0.0f, 0.0f);
	vector3s rot_2 = vector3s(0.0f, 0.0f, 0.0f);

	//=================レーザーの当たり判定処理：修正前の回転を取得
	if (transform_1.GetParent() != nullptr && transform_1.GetParent()->GetCellType() == E_CellType::CellType_Bullet) {
		if (((Bullet*)transform_1.GetParent())->GetBulletType() == E_BulletType::BulletType_Laser) {
			rot_1 = ((Bullet_Laser*)transform_1.GetParent())->GetCorrectRot();
		}
		else {
			rot_1 = transform_1.GetRot();
		}
	}
	else {
		rot_1 = transform_1.GetRot();
	}

	if (transform_2.GetParent() != nullptr && transform_2.GetParent()->GetCellType() == E_CellType::CellType_Bullet) {
		if (((Bullet*)transform_2.GetParent())->GetBulletType() == E_BulletType::BulletType_Laser) {
			rot_2 = ((Bullet_Laser*)transform_2.GetParent())->GetCorrectRot();
		}
		else {
			rot_2 = transform_2.GetRot();
		}
	}
	else {
		rot_2 = transform_2.GetRot();
	}
	//==================レーザーの当たり判定処理：修正前の回転を取得

	vector2s rectSize_1, rectSize_2;
	//rectSize_1 = data_1.size * vector2s(transform_1.GetScale().x, transform_1.GetScale().y);
	//rectSize_2 = data_2.size * vector2s(transform_2.GetScale().x, transform_2.GetScale().y);
	rectSize_1.x = data_1.size.x * transform_1.GetScale().x;
	rectSize_1.y = data_1.size.y * transform_1.GetScale().y;
	rectSize_2.x = data_2.size.x * transform_2.GetScale().x;
	rectSize_2.y = data_2.size.y * transform_2.GetScale().y;

	vector3s center_1 = transform_1.GetPos() + data_1.offset;
	vector3s center_2 = transform_2.GetPos() + data_2.offset;
	vector2s centerV = vector2s(center_1.x - center_2.x, center_1.y - center_2.y);
	bool sign = false;
	if ((int)(rot_1.z) == 0 && (int)(rot_2.z) == 0) {//回転なし当たり判定
		//AABBバウンディングボックス
		if (fabs(centerV.x) < (static_cast<double>(rectSize_1.x) + static_cast<double>(rectSize_2.x)) * 0.5f && fabs(centerV.y) < (static_cast<double>(rectSize_1.y) + static_cast<double>(rectSize_2.y)) * 0.5f) {
			return true;
		}
		else {
			return false;
		}
	}
	else {//回転あり当たり判定
		//OBBバウンディングボックス
		//(Separating axis theorem）分離軸判定法アルゴリズム
		vector2s axisA_1, axisA_2, axisB_1, axisB_2;
		if ((int)(rot_1.z) != 0) {
			float cos_1 = cosf(rot_1.z * DEG2RAD);
			float sin_1 = sinf(rot_1.z * DEG2RAD);
			axisA_1.x = cos_1;
			axisA_1.y = sin_1;
			axisA_2.x = -sin_1;
			axisA_2.y = cos_1;
		}
		else {
			axisA_1.x = 1;
			axisA_1.y = 0;
			axisA_2.x = 0;
			axisA_2.y = 1;
		}

		if ((int)(rot_2.z) != 0) {
			float cos_2 = cosf(rot_2.z * DEG2RAD);
			float sin_2 = sinf(rot_2.z * DEG2RAD);
			axisB_1.x = cos_2;
			axisB_1.y = sin_2;
			axisB_2.x = -sin_2;
			axisB_2.y = cos_2;
		}
		else {
			axisB_1.x = 1;
			axisB_1.y = 0;
			axisB_2.x = 0;
			axisB_2.y = 1;
		}

		axisA_1 = VecNormalize(axisA_1);
		axisA_2 = VecNormalize(axisA_2);
		axisB_1 = VecNormalize(axisB_1);
		axisB_2 = VecNormalize(axisB_2);

		//projection 1
		float dotV_1 = fabsf(VecDot(axisA_1, axisB_1 * (rectSize_2.x * 0.5f)));
		float dotV_2 = fabsf(VecDot(axisA_1, axisB_2 * (rectSize_2.y * 0.5f)));
		float centerDotV_1 = fabsf(VecDot(axisA_1, centerV));

		if (centerDotV_1 >= dotV_1 + dotV_2 + rectSize_1.x * 0.5f) {
			return false;
		}

		dotV_1 = fabsf(VecDot(axisA_2, axisB_1 * (rectSize_2.x * 0.5f)));
		dotV_2 = fabsf(VecDot(axisA_2, axisB_2 * (rectSize_2.y * 0.5f)));
		centerDotV_1 = fabsf(VecDot(axisA_2, centerV));
		if (centerDotV_1 >= dotV_1 + dotV_2 + rectSize_1.y * 0.5f) {
			return false;
		}

		dotV_1 = fabsf(VecDot(axisB_1, axisA_1 * (rectSize_1.x * 0.5f)));
		dotV_2 = fabsf(VecDot(axisB_1, axisA_2 * (rectSize_1.y * 0.5f)));
		centerDotV_1 = fabsf(VecDot(axisB_1, centerV));
		if (centerDotV_1 >= dotV_1 + dotV_2 + rectSize_2.x * 0.5f) {
			return false;
		}

		dotV_1 = fabsf(VecDot(axisB_2, axisA_1 * (rectSize_1.x * 0.5f)));
		dotV_2 = fabsf(VecDot(axisB_2, axisA_2 * (rectSize_1.y * 0.5f)));
		centerDotV_1 = fabsf(VecDot(axisB_2, centerV));
		if (centerDotV_1 >= dotV_1 + dotV_2 + rectSize_2.y * 0.5f) {
			return false;
		}

		return true;

	}

}

bool Collider::CircleCollisionDetect(Transform& transform_1, Transform& transform_2, ColliderData& data_1, ColliderData& data_2)
{
	vector3s circleCenter_1 = transform_1.GetPos() + data_1.offset;
	vector3s circleCenter_2 = transform_2.GetPos() + data_2.offset;
	float radius_1 = transform_1.GetScale().x * data_1.size.x * 0.5f;
	float radius_2 = transform_2.GetScale().x * data_2.size.x * 0.5f;

	if (VecDistS(circleCenter_1, circleCenter_2) > (radius_1 + radius_2) * (radius_1 + radius_2)) {
		return false;
	}
	else {
		return true;
	}

}

bool Collider::IsContainPoint(BaseCell* cell, vector3s point)
{
	if (cell == nullptr)return false;
	Collider* c = (Collider*)cell->GetComponent(E_Component::Component_Collider);
	if (c == nullptr)return false;

	int size = c->GetColliderDataArraySize();
	if (size == 0)return false;

	std::vector<ColliderData*>& colliderDataArray = c->GetColliderDataArray();
	for (int index = 0; index < size; index++) {
		if (colliderDataArray[index] == nullptr)continue;
		if (colliderDataArray[index]->colliderType == E_ColliderType::Collider_Rect) {
			if (IsRectContainPoint(cell->GetTransform(), *colliderDataArray[index], point)) {
				return true;
			}
		}
		else {
			if (IsCircleContainPoint(cell->GetTransform(), *colliderDataArray[index], point)) {
				return true;
			}
		}
	}
	return false;
}

bool Collider::IsRectContainPoint(Transform* transform, ColliderData& data, vector3s point)
{
	if (transform == nullptr)return false;

	vector3s pos = transform->GetPos() + data.offset;
	vector2s size = vector2s(transform->GetScale().x, transform->GetScale().y) * data.size;
	vector3s rot = transform->GetRot();
	vector2s center2Point = vector2s(pos.x - point.x, pos.y - point.y);

	if ((int)(rot.z) == 0) {
		if (fabsf(center2Point.x) > size.x * 0.5f || fabsf(center2Point.y) > size.y * 0.5f) {
			return false;
		}
		else {
			return true;
		}

	}
	else {

		vector2s xAxis;
		vector2s yAxis;

		float cos_1 = cosf(rot.z * DEG2RAD);
		float sin_1 = sinf(rot.z * DEG2RAD);
		xAxis.x = cos_1;
		xAxis.y = sin_1;
		yAxis.x = -sin_1;
		yAxis.y = cos_1;

		xAxis = VecNormalize(xAxis);
		yAxis = VecNormalize(yAxis);

		float dotV = fabsf(VecDot(xAxis, center2Point));
		if (dotV > size.x * 0.5f) {
			return false;
		}

		dotV = fabsf(VecDot(yAxis, center2Point));
		if (dotV > size.y * 0.5f) {
			return false;
		}
		return true;
	}
}

bool Collider::IsRectContainPoint(vector3s pos, vector2s size, vector3s rot, vector3s point)
{

	vector2s center2Point = vector2s(pos.x - point.x, pos.y - point.y);

	if ((int)(rot.z) == 0) {
		if (fabsf(center2Point.x) > size.x * 0.5f || fabsf(center2Point.y) > size.y * 0.5f) {
			return false;
		}
		else {
			return true;
		}
	}
	else {

		vector2s xAxis;
		vector2s yAxis;

		float cos_1 = cosf(rot.z * DEG2RAD);
		float sin_1 = sinf(rot.z * DEG2RAD);
		xAxis.x = cos_1;
		xAxis.y = sin_1;
		yAxis.x = -sin_1;
		yAxis.y = cos_1;

		xAxis = VecNormalize(xAxis);
		yAxis = VecNormalize(yAxis);

		float dotV = fabsf(VecDot(xAxis, center2Point));
		if (dotV > size.x * 0.5f) {
			return false;
		}

		dotV = fabsf(VecDot(yAxis, center2Point));
		if (dotV > size.y * 0.5f) {
			return false;
		}
		return true;
	}

	return false;
}

bool Collider::IsCircleContainPoint(Transform* transform, ColliderData& data, vector3s point)
{
	if (transform == nullptr)return false;

	vector3s pos = transform->GetPos();
	float radius = transform->GetScale().x * data.size.x * 0.5f;

	float dist_s = VecDistS(pos, point);
	if (dist_s > radius * radius) {
		return false;
	}
	else {
		return true;
	}
}

bool Collider::IsCircleContainPoint(vector3s pos, float radius, vector3s point)
{
	float dist_s = VecDistS(pos, point);
	if (dist_s > radius * radius) {
		return false;
	}
	else {
		return true;
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="cell_1">  detect target cell  </param>
/// <param name="transform_2">  scene obj   </param>
/// <param name="c_2"></param>
/// <returns></returns>
bool Collider::SceneObjCollisionDetect(BaseCell* cell_1, Transform* transform_2, Collider* c_2)
{
	if (cell_1 == nullptr || transform_2 == nullptr || c_2 == nullptr)return false;
	Transform* transform_1 = cell_1->GetTransform();
	Collider* c_1 = (Collider*)cell_1->GetComponent(E_Component::Component_Collider);
	if (c_1 == nullptr)return false;
	std::vector<ColliderData*> c_array_1 = c_1->GetColliderDataArray();
	std::vector<ColliderData*> c_array_2 = c_2->GetColliderDataArray();
	int size_1 = (int)c_array_1.size();
	int size_2 = (int)c_array_2.size();
	if (size_1 == 0 || size_2 == 0)return false;
	ColliderData* data_1 = nullptr;
	ColliderData* data_2 = nullptr;
	for (int index_1 = 0; index_1 < size_1; index_1++) {
		data_1 = c_array_1[index_1];
		if (data_1 == nullptr) {
			continue;
		}
		for (int index_2 = 0; index_2 < size_2; index_2++) {
			data_2 = c_array_2[index_2];
			if (data_2 == nullptr) {
				continue;
			}
			if (data_1->colliderType != E_ColliderType::Collider_Rect || data_2->colliderType != E_ColliderType::Collider_Rect)return false;
			vector2s rectSize_1, rectSize_2;
			rectSize_1 = data_1->size * vector2s(transform_1->GetScale().x, transform_1->GetScale().y);
			rectSize_2 = data_2->size * vector2s(transform_2->GetScale().x, transform_2->GetScale().y);
			vector3s center_1 = transform_1->GetPos() + data_1->offset;
			vector3s center_2 = transform_2->GetPos() + data_2->offset;
			vector2s centerV = vector2s(center_1.x - center_2.x, center_1.y - center_2.y);
			vector3s detectBoxCenter = vector3s(0, 0, 0);
			vector2s detectBoxRect = vector2s(0, 0);
			vector3s detectPointCenter = vector3s(0, 0, 0);
			vector2s detectPointRect = vector2s(0, 0);
			//xAixs
			if (rectSize_1.y > rectSize_2.y) {
				detectBoxCenter = center_1;
				detectBoxRect = rectSize_1;
				detectPointCenter = center_2;
				detectPointRect = rectSize_2;
			}
			else {
				detectBoxCenter = center_2;
				detectBoxRect = rectSize_2;
				detectPointCenter = center_1;
				detectPointRect = rectSize_1;
			}
			int containPointCounter = 0;
			vector3s detectPoint = detectPointCenter;
			vector3s secondDetectPoint = detectPointCenter;
			detectPoint.x = detectPointCenter.x - detectPointRect.x * 0.5f;
			detectPoint.y = detectPointCenter.y;
			if (IsRectContainPoint(detectBoxCenter, detectBoxRect, vector3s(0, 0, 0), detectPoint)) {//left center point
				return true;
			}
			detectPoint.x = detectPointCenter.x - detectPointRect.x * 0.5f;
			detectPoint.y = detectPointCenter.y + detectPointRect.y * 0.5f;
			if (IsRectContainPoint(detectBoxCenter, detectBoxRect, vector3s(0, 0, 0), detectPoint)) {//LT
				//second detect
				secondDetectPoint.x = detectPoint.x + SECOND_DETECT_OFFSET_X;
				secondDetectPoint.y = detectPoint.y;
				if (IsRectContainPoint(detectBoxCenter, detectBoxRect, vector3s(0, 0, 0), secondDetectPoint)) {
					return true;
				}
				secondDetectPoint.x = detectPoint.x;
				secondDetectPoint.y = detectPoint.y - SECOND_DETECT_OFFSET_Y;
				if (IsRectContainPoint(detectBoxCenter, detectBoxRect, vector3s(0, 0, 0), secondDetectPoint)) {
					return true;
				}
			}
			detectPoint.x = detectPointCenter.x + detectPointRect.x * 0.5f;
			detectPoint.y = detectPointCenter.y;
			if (IsRectContainPoint(detectBoxCenter, detectBoxRect, vector3s(0, 0, 0), detectPoint)) {//right center point
				return true;
			}
			detectPoint.x = detectPointCenter.x + detectPointRect.x * 0.5f;
			detectPoint.y = detectPointCenter.y + detectPointRect.y * 0.5f;
			if (IsRectContainPoint(detectBoxCenter, detectBoxRect, vector3s(0, 0, 0), detectPoint)) {//RT
				//second detect
				secondDetectPoint.x = detectPoint.x - SECOND_DETECT_OFFSET_X;
				secondDetectPoint.y = detectPoint.y;
				if (IsRectContainPoint(detectBoxCenter, detectBoxRect, vector3s(0, 0, 0), secondDetectPoint)) {
					return true;
				}
				secondDetectPoint.x = detectPoint.x;
				secondDetectPoint.y = detectPoint.y - SECOND_DETECT_OFFSET_Y;
				if (IsRectContainPoint(detectBoxCenter, detectBoxRect, vector3s(0, 0, 0), secondDetectPoint)) {
					return true;
				}
			}
			//yAixs
			if (rectSize_1.x > rectSize_2.x) {
				detectBoxCenter = center_1;
				detectBoxRect = rectSize_1;
				detectPointCenter = center_2;
				detectPointRect = rectSize_2;
			}
			else {
				detectBoxCenter = center_2;
				detectBoxRect = rectSize_2;
				detectPointCenter = center_1;
				detectPointRect = rectSize_1;
			}
			detectPoint.x = detectPointCenter.x;
			detectPoint.y = detectPointCenter.y + detectPointRect.y * 0.5f;
			if (IsRectContainPoint(detectBoxCenter, detectBoxRect, vector3s(0, 0, 0), detectPoint)) {//up center point
				return true;
			}
			detectPoint.x = detectPointCenter.x - detectPointRect.x * 0.5f;
			detectPoint.y = detectPointCenter.y - detectPointRect.y * 0.5f;
			if (IsRectContainPoint(detectBoxCenter, detectBoxRect, vector3s(0, 0, 0), detectPoint)) {//LB
				//second detect
				secondDetectPoint.x = detectPoint.x + SECOND_DETECT_OFFSET_X;
				secondDetectPoint.y = detectPoint.y;
				if (IsRectContainPoint(detectBoxCenter, detectBoxRect, vector3s(0, 0, 0), secondDetectPoint)) {
					return true;
				}
				secondDetectPoint.x = detectPoint.x;
				secondDetectPoint.y = detectPoint.y + SECOND_DETECT_OFFSET_Y;
				if (IsRectContainPoint(detectBoxCenter, detectBoxRect, vector3s(0, 0, 0), secondDetectPoint)) {
					return true;
				}
			}
			detectPoint.x = detectPointCenter.x;
			detectPoint.y = detectPointCenter.y - detectPointRect.y * 0.5f;
			if (IsRectContainPoint(detectBoxCenter, detectBoxRect, vector3s(0, 0, 0), detectPoint)) {//down center point
				return true;
			}
			detectPoint.x = detectPointCenter.x + detectPointRect.x * 0.5f;
			detectPoint.y = detectPointCenter.y - detectPointRect.y * 0.5f;
			if (IsRectContainPoint(detectBoxCenter, detectBoxRect, vector3s(0, 0, 0), detectPoint)) {//RB
				//second detect
				secondDetectPoint.x = detectPoint.x - SECOND_DETECT_OFFSET_X;
				secondDetectPoint.y = detectPoint.y;
				if (IsRectContainPoint(detectBoxCenter, detectBoxRect, vector3s(0, 0, 0), secondDetectPoint)) {
					return true;
				}
				secondDetectPoint.x = detectPoint.x;
				secondDetectPoint.y = detectPoint.y + SECOND_DETECT_OFFSET_Y;
				if (IsRectContainPoint(detectBoxCenter, detectBoxRect, vector3s(0, 0, 0), secondDetectPoint)) {
					return true;
				}
			}
		}
	}
	return false;
}

bool Collider::LineCollisionDetect(BaseCell* cell_1, BaseCell* cell_2, vector3s& collisionPoint, vector3s& collisionNormal)
{
	if (cell_1 == nullptr || cell_2 == nullptr)return false;

	Collider* collider = (Collider*)cell_1->GetComponent(E_Component::Component_Collider);
	if (collider == nullptr)return false;
	if (collider->GetColliderDataArraySize() == 0)return false;

	ColliderData* data = collider->GetColliderDataArray()[0];
	if (data == nullptr)return false;

	vector3s pos = cell_1->GetTransform()->GetPos();
	vector3s scale = cell_1->GetTransform()->GetScale();
	vector2s size = data->size;

	vector3s startPos = vector3s(-size.x * scale.x * 0.5f, 0.0f, 0.0f);
	vector3s endPos = vector3s(size.x * scale.x * 0.5f, 0.0f, 0.0f);

	//rot
	vector3s rot = cell_1->GetTransform()->GetRot();
	if ((int)(rot.z) != 0) {

		float cosRotZ = cosf(rot.z * DEG2RAD);
		float sinRotZ = sinf(rot.z * DEG2RAD);

		float tempX = startPos.x;
		float tempY = startPos.y;

		startPos.x = (tempX * cosRotZ - tempY * sinRotZ);
		startPos.y = (tempX * sinRotZ + tempY * cosRotZ);

		tempX = endPos.x;
		tempY = endPos.y;

		endPos.x = (tempX * cosRotZ - tempY * sinRotZ);
		endPos.y = (tempX * sinRotZ + tempY * cosRotZ);
	}

	startPos = pos + startPos;
	endPos = pos + endPos;
	//vector3s startPos = pos;
	//startPos.x -= size.x * scale.x * 0.5f;
	//vector3s endPos = pos;
	//endPos.x += size.x * scale.x * 0.5f;

	return LineCollisionDetect(startPos, endPos, cell_2, collisionPoint, collisionNormal);
}

bool Collider::LineCollisionDetect(vector3s startPos, vector3s endPos, BaseCell* cell, vector3s& collisionPoint, vector3s& collisionNormal)
{
	if (cell == nullptr)return false;

	Collider* collider = (Collider*)cell->GetComponent(E_Component::Component_Collider);
	if (collider == nullptr)return false;
	if (collider->GetColliderDataArraySize() == 0)return false;

	ColliderData* data = collider->GetColliderDataArray()[0];
	if (data == nullptr)return false;

	vector3s pos = cell->GetTransform()->GetPos();
	vector3s scale = cell->GetTransform()->GetScale();
	vector3s rot = cell->GetTransform()->GetRot();
	vector2s size = data->size;

	float halfX = size.x * scale.x * 0.5f;
	float halfY = size.y * scale.y * 0.5f;

	vector3s lt_point = vector3s(pos.x - halfX, pos.y - halfY, 0.0f);
	vector3s rt_point = vector3s(pos.x + halfX, pos.y - halfY, 0.0f);
	vector3s lb_point = vector3s(pos.x - halfX, pos.y + halfY, 0.0f);
	vector3s rb_point = vector3s(pos.x + halfX, pos.y + halfY, 0.0f);
	//rot
	if ((int)(rot.z) != 0) {

		float cosRotZ = cosf(rot.z * DEG2RAD);
		float sinRotZ = sinf(rot.z * DEG2RAD);

		float tempX = lt_point.x;
		float tempY = lt_point.y;
		lt_point.x = (tempX * cosRotZ - tempY * sinRotZ);
		lt_point.y = (tempX * sinRotZ + tempY * cosRotZ);

		tempX = rt_point.x;
		tempY = rt_point.y;
		rt_point.x = (tempX * cosRotZ - tempY * sinRotZ);
		rt_point.y = (tempX * sinRotZ + tempY * cosRotZ);

		tempX = lb_point.x;
		tempY = lb_point.y;
		lb_point.x = (tempX * cosRotZ - tempY * sinRotZ);
		lb_point.y = (tempX * sinRotZ + tempY * cosRotZ);

		tempX = rb_point.x;
		tempY = rb_point.y;
		rb_point.x = (tempX * cosRotZ - tempY * sinRotZ);
		rb_point.y = (tempX * sinRotZ + tempY * cosRotZ);
	}

	vector3s topEdge = rt_point - lt_point;
	vector3s leftEdge = lt_point - lb_point;
	vector3s bottomEdge = lb_point - rb_point;
	vector3s rightEdge = rb_point - rt_point;

	bool collisionSign = false;
	vector3s nearestPoint = endPos;
	vector3s normal = vector3s(0, 0, 0);
	//line collision detect
	vector3s collisionPoint_topEdge = vector3s();
	if (LineCollisionDetect(startPos, endPos, rt_point, lt_point, collisionPoint_topEdge) == true) {//top edge
		if (VecDistS(nearestPoint, startPos) > VecDistS(collisionPoint_topEdge, startPos)) {
			nearestPoint = collisionPoint_topEdge;
			normal = VecTransformDToT (upVector);
			collisionSign = true;
		}
	}

	vector3s collisionPoint_leftEdge = vector3s();
	if (LineCollisionDetect(startPos, endPos, lt_point, lb_point, collisionPoint_leftEdge) == true) {
		if (VecDistS(nearestPoint, startPos) > VecDistS(collisionPoint_leftEdge, startPos)) {
			nearestPoint = collisionPoint_leftEdge;
			normal = VecTransformDToT(leftVector);
			collisionSign = true;
		}
	}

	vector3s collisionPoint_bottomEdge = vector3s();
	if (LineCollisionDetect(startPos, endPos, lb_point, rb_point, collisionPoint_bottomEdge) == true) {
		if (VecDistS(nearestPoint, startPos) > VecDistS(collisionPoint_bottomEdge, startPos)) {
			nearestPoint = collisionPoint_bottomEdge;
			normal = VecTransformDToT(downVector);
			collisionSign = true;
		}
	}

	vector3s collisionPoint_rightEdge = vector3s();
	if (LineCollisionDetect(startPos, endPos, rb_point, rt_point, collisionPoint_rightEdge) == true) {
		if (VecDistS(nearestPoint, startPos) > VecDistS(collisionPoint_rightEdge, startPos)) {
			nearestPoint = collisionPoint_rightEdge;
			normal = VecTransformDToT(rightVector);
			collisionSign = true;
		}
	}

	//normal rot
	if ((int)(rot.z) != 0) {
		float cosRotZ = cosf(rot.z * DEG2RAD);
		float sinRotZ = sinf(rot.z * DEG2RAD);

		float tempX = normal.x;
		float tempY = normal.y;
		normal.x = (tempX * cosRotZ - tempY * sinRotZ);
		normal.y = (tempX * sinRotZ + tempY * cosRotZ);
	}
	normal.z = 0.0f;

	collisionNormal = VecNormalize(normal);
	collisionPoint = nearestPoint;

	return collisionSign;
}

bool Collider::LineCollisionDetect(vector3s a, vector3s b, vector3s c, vector3s d, vector3s& collisionPos)
{
	float s_abc = (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
	float s_abd = (a.x - d.x) * (b.y - d.y) - (a.y - d.y) * (b.x - d.x);
	if (s_abc * s_abd >= 0) {
		return false;
	}

	float s_cda = (c.x - a.x) * (d.y - a.y) - (c.y - a.y) * (d.x - a.x);
	float s_cdb = s_cda + s_abc - s_abd;
	if (s_cda * s_cdb >= 0) {
		return false;
	}

	float dx = 0.0f;
	float dy = 0.0f;
	float t = s_abd - s_abc;
	if (abs(t) > 0.000001f) {
		t = s_cda / t;
		dx = t * (b.x - a.x);
		dy = t * (b.y - a.y);
	}
	collisionPos.x = a.x + dx;
	collisionPos.y = a.y + dy;

	return true;
}


#pragma endregion Collider_method

#pragma endregion component_method

#pragma region Animation_method

#pragma region Anim_callback

void Anim_Effect_Normal_CB::Func(AnimClip* baseClip) {
	if (baseClip == nullptr || baseClip->GetParent() == nullptr)return;

	BaseCell* parent = baseClip->GetParent();
	if (parent->GetCellType() != E_CellType::CellType_Effect)return;

	//recycle effect
	GameMgr::GetInstance()->effectMgr->RecycleEffect((Effect*)parent);
}


#pragma endregion Anim_callback

AnimClip::~AnimClip()
{
	parent = nullptr;

	animStart_Callback = nullptr;
	animEnd_Callback = nullptr;
}

void AnimClip::DoUpdate(float deltatime)
{
	if (animState) {
		animTimePass += deltatime;

		if (animTimePass > animTime) {
			if (loop) {
				animTimePass = 0.0f;
			}
			else {
				animState = false;
				OnAnimEnd();
				return;
			}
		}

		float progress = animTimePass * animTime_I;
		if (dir) {
			curIndex = I_Lerp(frameIndexRange.x, frameIndexRange.y, progress);
		}
		else {
			curIndex = I_Lerp(frameIndexRange.y, frameIndexRange.x, progress);
		}

		//set tex index data
		if (parent != nullptr) {
			DrawData* data = parent->GetDrawData();
			if (data != nullptr) {
				data->SetShaderIntParam(3, curIndex);
			}
		}

	}
}

void AnimFSM::RegisterAnim(E_Anim animType, AnimClip* anim, bool setDefaultAnim)
{
	if (anim == nullptr)return;

	if (animDic.find(animType) != animDic.end()) {
		//had same value
	}
	else {
		animDic[animType] = anim;
		if (parent != nullptr) {
			anim->SetParent(parent->GetParent());
		}
		else {
			//error
		}
		if (setDefaultAnim) {
			SwitchAnim(animType, true, false);
			defaultAnim = animType;
		}
	}
}

void Animation::DoInit()
{
	Component::DoInit();
	animFSM.DoInit();
}

void Animation::UnInit()
{
}

void Animation::SetParent(BaseCell* p)
{
	Component::SetParent(p);
	animFSM.SetParent(this);
}

void Animation::DoUpdate(float deltatime)
{
	if (enable) {
		animFSM.DoUpdate(deltatime);
	}
	Component::DoUpdate(deltatime);
}

void Animation::DoLateUpdate(float deltatime)
{
	animFSM.DoLateUpdate(deltatime);
}

void Animation::Copy(Component& src)
{
	if (src.GetComponentType() != E_Component::Component_Animation) {
		//error
		return;
	}
	try {

		Animation* srcA = (Animation*)(&src);

		this->parent = srcA->parent;
		//TODO

	}
	catch (std::exception e) {

	}
}

void Animation::SwitchAnim(E_Anim targetAnim, bool resetAnim, bool lastClipEndCB)
{
	animFSM.SwitchAnim(targetAnim, resetAnim, lastClipEndCB);
}

void Animation::SwitchToDefaultAnim()
{
	animFSM.DoInit();
}

void Animation::StopAnim()
{
	animFSM.DoInit();
	animFSM.SetAnimState(false);
}

void Animation::PauseAnim(bool pause)
{
	animFSM.SetAnimState(!pause);
}


#pragma endregion Animation_method;

#pragma region movement_method

Movement::Movement() :
	parent(nullptr), movementType(E_MovementType::MovementType_None), defaultForward(true), forward(true),
	movementTime(0.0f), movementTime_I(0.0f), isLoop(false), moveTimePass(0.0f), movementState(false), movementDone(true) {

}

Movement::Movement(BaseCell* p, E_MovementType mT, bool movementDefaultDir, bool loop, bool defaultActive, float movementT) :
	parent(p), movementType(mT), defaultForward(movementDefaultDir), forward(movementDefaultDir),
	movementTime(movementT), isLoop(loop), moveTimePass(0.0f), movementState(defaultActive), movementDone(false) {

	if (movementT <= 0) {
		movementTime_I = 1.0f;
	}
	else {
		movementTime_I = 1.0f / movementTime;
	}


}

Movement::~Movement() {
	parent = nullptr;
}

void Movement::DoInit() {
	ResetDefault();
}

void Movement::StartMovement(bool reset) {

}

void Movement::SetMovementState(bool state) {
	if (movementState == state)return;

	movementState = state;//TODO
}

void Movement::StopMovement(bool reset) {
	movementDone = true;
}

void Movement::Reset() {
	moveTimePass = 0.0f;
}

void Movement::ResetDefault()
{
	Reset();
}

void Movement::SetMovementTime(float time) {
	if ((int)(time + 0.5f) <= 0) {
		movementTime = 0.0f;
		movementTime_I = 1.0f;
	}
	else {
		movementTime = time;
		movementTime_I = 1.0f / movementTime;
	}
}

void Movement::SetParent(BaseCell* p) {
	parent = p;
}

bool Movement::CheckDefaultForward()
{
	return defaultForward;
}


#pragma region movenet_axis_method

Movement_Axis::Movement_Axis() :isRelatedPos(true), startPos(vector3s(0, 0, 0)), endPos(vector3s(0, 0, 0)), curPos(vector3s(0, 0, 0)), defaultPos(vector3s(0, 0, 0)) {

}

Movement_Axis::Movement_Axis(BaseCell* p, bool isRelated, bool movementDefaultDir, bool loop, bool defaultActive, float movementT, vector3s defaultP, vector3s startP, vector3s endP) :
	isRelatedPos(isRelated), defaultPos(defaultP), startPos(startP), endPos(endP),
	Movement(p, E_MovementType::MovementType_Axis, movementDefaultDir, loop, defaultActive, movementT) {

	curPos = (movementDefaultDir) ? startPos : endPos;
}

Movement_Axis::~Movement_Axis()
{
}

void Movement_Axis::StartMovement(bool reset) {
	if (reset) {
		Reset();
	}
	movementState = true;
	movementDone = false;
}

void Movement_Axis::SetMovementState(bool state) {

	movementDone = false;

	Movement::SetMovementState(state);

}

void Movement_Axis::StopMovement(bool stopForward) {
	if (stopForward) {
		curPos = defaultForward ? endPos : startPos;
	}
	else {
		curPos = defaultForward ? startPos : endPos;
	}
}

void Movement_Axis::Reset() {
	if (defaultForward) {
		curPos = startPos;
	}
	else {
		curPos = endPos;
	}
	forward = defaultForward;
	moveTimePass = 0.0f;
}

void Movement_Axis::ResetDefult()
{
	Movement::ResetDefault();
	Reset();
}

void Movement_Axis::DoUpdate(float deltatime) {
	if (movementState == true && movementDone == false) {

		if (moveTimePass > movementTime) {
			if (isLoop) {
				forward = !forward;
				moveTimePass = 0.0f;
			}
			else {
				movementDone = true;
				//return
			}
		}
		else {
			moveTimePass += deltatime;
		}

		float progress = moveTimePass * movementTime_I;
		if (forward) {
			curPos = VecLerp(startPos, endPos, progress);
		}
		else {
			curPos = VecLerp(endPos, startPos, progress);
		}
	}
	ApplyMovement();
}

void Movement_Axis::DoLateUpdate(float deltatime) {

}

void Movement_Axis::ApplyMovement() {
	if (parent == nullptr) {
		return;
	}

	//TODO
	if (isRelatedPos) {
		vector3s targetPos = defaultPos + curPos;
		parent->SetPos(targetPos, true);
	}
	else {
		parent->SetPos(curPos, true);
	}

}

void Movement_Axis::SetVecTParam(int index, vector3s param) {
	if (index == 0) {
		defaultPos = param;
	}
	//TODO
}

#pragma endregion movenet_aixs_method

#pragma endregion movement_method

#pragma region movement_appear

Movement_Appear::Movement_Appear() :curCellState(false), waitTime(0.0f), waitTimePass(0.0f), forwardColor(vector4s()), backwardColor(vector4s()), curColor(vector4s())
{
	movementType = E_MovementType::MovementType_Appear;
}

Movement_Appear::Movement_Appear(BaseCell* p, bool defaultState, bool loop, bool defaultActive, float movementT, float waitT) :
	waitTime(waitT), waitTimePass(0.0f), curCellState(defaultState), forwardColor(vector4s()), backwardColor(vector4s()), curColor(vector4s()),
	Movement(p, E_MovementType::MovementType_Appear, defaultState, loop, defaultActive, movementT)
{
}

Movement_Appear::~Movement_Appear()
{

}

void Movement_Appear::StartMovement(bool reset)
{
	if (reset) {
		Reset();
	}
	movementState = true;
	movementDone = false;

	stateChange = false;
	curCellState = !defaultForward;
}

void Movement_Appear::SetMovementState(bool state)
{
	movementDone = false;

	Movement::SetMovementState(state);
}

void Movement_Appear::StopMovement(bool stopForward)
{
	if (stopForward) {
		//check default state
		curColor = forwardColor;
	}
	else {
		//check default state
		curColor = backwardColor;
	}
}

void Movement_Appear::Reset()
{

	forward = defaultForward;
	if (defaultForward) {
		curColor = backwardColor;
	}
	else {
		curColor = forwardColor;
	}

	moveTimePass = 0.0f;
	waitTimePass = 0.0f;

}

void Movement_Appear::ResetDefault()
{
	Movement::ResetDefault();
	Reset();
}

void Movement_Appear::DoUpdate(float deltatime)
{
	if (isLoop) {
		if (moveTimePass > movementTime) {
			if (stateChange == false) {
				stateChange = true;
				curCellState = forward;
			}
			if (movementState == true) {
				//counter wait time
				waitTimePass += deltatime;
				if (waitTimePass > waitTime) {
					forward = !forward;
					waitTimePass = 0.0f;
					moveTimePass = 0.0f;
					stateChange = false;
				}
			}
			//else:do nothing
		}
		else {
			moveTimePass += deltatime;
		}

		float progress = moveTimePass * movementTime_I;
		if (forward) {
			//curColor = VecLerp(backwardColor, forwardColor, progress);
		}
		else {
			//curColor = VecLerp(forwardColor, backwardColor, progress);
		}

	}
	else {

		if (movementState == true && movementDone == false) {

			if (moveTimePass > movementTime) {
				if (stateChange == false) {
					stateChange = true;
					curCellState = forward;
				}
				if (isLoop) {
					waitTimePass += deltatime;
					if (waitTimePass > waitTime) {
						forward = !forward;
						waitTimePass = 0.0f;
						moveTimePass = 0.0f;
						stateChange = false;
					}
					//else: do nothing
				}
				else {
					movementDone = true;
					movementState = false;
				}
			}
			else {
				moveTimePass += deltatime;
			}
			//TODO: Movement
			float progress = moveTimePass * movementTime_I;
			if (forward) {
				//curColor = VecLerp(backwardColor, forwardColor, progress);
				//curColor = progress >= 0.999f ? forwardColor : backwardColor;
			}
			else {
				//curColor = VecLerp(forwardColor, backwardColor, progress);
				//curColor = progress >= 0.999f ? backwardColor : forwardColor;
			}

		}
	}
	ApplyMovement();
}

void Movement_Appear::DoLateUpdate(float deltatime)
{
}

void Movement_Appear::ApplyMovement()
{
	//TODO
	if (parent != nullptr) {
		Collider* tempCollider = (Collider*)parent->GetComponent(E_Component::Component_Collider);
		if (tempCollider != nullptr) {
			//tempCollider->colliderColor = curCellState ? forwardColor : backwardColor;
			//tempCollider->colliderColor = curColor;
		}
		parent->deadlyCell = !curCellState;
		if (parent->GetCellType() == E_CellType::CellType_SceneConstObj) {
		}
	}
}

void Movement_Appear::SetMovementColor(vector4s fColor, vector4s bColor)
{
	forwardColor = fColor;
	backwardColor = bColor;
	curColor = !forward ? forwardColor : backwardColor;
}

void Movement_Appear::SetVecTParam(int index, vector3s param)
{

}

#pragma endregion movement_appear

#pragma region movement_scale

Movement_Scale::Movement_Scale() :defaultScale(vector3s(1.0f, 1.0f, 1.0f)), targetScale(vector3s(1.0f, 1.0f, 1.0f)), delayTime(0.0f), delayTimePass(0.0f) {

}

Movement_Scale::Movement_Scale(BaseCell* p, bool defaultState, vector3s defaultS, vector3s targetS, float movementT, float delayT) :
	defaultScale(defaultS), targetScale(targetS), delayTime(0.0f), delayTimePass(0.0f),
	Movement(p, E_MovementType::MovementType_Scale, true, false, true, movementT)
{
}

Movement_Scale::~Movement_Scale()
{
}

void Movement_Scale::StartMovement(bool reset)
{
	if (reset) {
		Reset();
	}

	movementState = true;
	movementDone = false;

}

void Movement_Scale::SetMovementState(bool state)
{
	movementDone = false;

	Movement::SetMovementState(state);
}

void Movement_Scale::StopMovement(bool stopForward)
{
	if (stopForward) {
		//check default state
		curScale = targetScale;
	}
	else {
		//check default state
		curScale = defaultScale;
	}
}

void Movement_Scale::Reset()
{
	if (defaultForward) {
		curScale = targetScale;
	}
	else {
		curScale = defaultScale;
	}
	forward = defaultForward;
	moveTimePass = 0.0f;
	delayTimePass = 0.0f;
}

void Movement_Scale::ResetDefault()
{
	Movement::ResetDefault();
	Reset();
}

void Movement_Scale::SetMovementTime(float time)
{
	Movement::SetMovementTime(time);
}

void Movement_Scale::DoUpdate(float deltatime)
{
	if (movementState == true && movementDone == false) {

		if (delayTimePass > delayTime) {

			if (moveTimePass > movementTime) {
				if (isLoop) {
					forward = !forward;
					moveTimePass = 0.0f;
					delayTimePass = 0.0f;
				}
				else {
					movementDone = true;
					//return
				}
			}
			else {
				moveTimePass += deltatime;
			}

			float progress = moveTimePass * movementTime_I;
			if (forward) {
				curScale = VecLerp(defaultScale, targetScale, progress);
			}
			else {
				curScale = VecLerp(targetScale, defaultScale, progress);
			}
		}
		else {
			delayTimePass += deltatime;
		}
	}
	ApplyMovement();

}

void Movement_Scale::DoLateUpdate(float deltatime)
{
}

void Movement_Scale::ApplyMovement()
{
	if (parent == nullptr) {
		return;
	}

	//TODO
	parent->SetScale(curScale);
}



#pragma endregion movement_scale

#pragma region status_method

void Status::EnqueueStatus(E_Status status)
{
	if (needRecordChangingData == false)return;
	if (count(changingStatusQueue.begin(), changingStatusQueue.end(), status) == 0) {
		changingStatusQueue.push_back(status);
	}
}

Status::Status() :maxHp(1), curHp(1), maxAtk(1), curAtk(1), maxDef(1), curDef(1), dataChangeSign(true), needRecordChangingData(false)
{
	changingStatusQueue.clear();
}

Status::~Status() {

}

void Status::SetDefaultData(E_Status statusType, float value, bool changeCur = true)
{
	float setValue = 0;
	switch (statusType) {
	case E_Status::Status_Hp:
		setValue = F_Clamp(value, 1, STATUS_LIMIT_HP);
		maxHp = setValue;
		if (changeCur) {
			curHp = maxHp;
			EnqueueStatus(E_Status::Status_Hp);
		}
		EnqueueStatus(E_Status::Status_Max_Hp);
		dataChangeSign = true;
		break;
	case E_Status::Status_Atk:
		setValue = F_Clamp(value, 1, STATUS_LIMIT_ATK);
		maxAtk = setValue;
		if (changeCur) {
			EnqueueStatus(E_Status::Status_Atk);
			curAtk = maxAtk;
		}
		EnqueueStatus(E_Status::Status_Max_Atk);
		dataChangeSign = true;
		break;
	case E_Status::Status_Def:
		setValue = F_Clamp(value, 1, STATUS_LIMIT_DEF);
		maxDef = setValue;
		if (changeCur) {
			EnqueueStatus(E_Status::Status_Def);
			curDef = maxDef;
		}
		EnqueueStatus(E_Status::Status_Max_Def);
		dataChangeSign = true;
		break;
	default:
		break;
	}
}

void Status::ChangeDefaultData(E_Status statusType, float increaseValue, bool changeCur, bool lockMin)
{
	float minValue = lockMin ? 1 : 0;
	switch (statusType) {
	case E_Status::Status_Hp:
	case E_Status::Status_Max_Hp:
		maxHp = F_Clamp(maxHp + increaseValue, minValue, STATUS_LIMIT_HP);
		if (changeCur) {
			EnqueueStatus(E_Status::Status_Hp);
			curHp = maxHp;
			if (curHp <= 0) {
				isDead = true;
			}
		}
		EnqueueStatus(E_Status::Status_Max_Hp);
		dataChangeSign = true;
		break;
	case E_Status::Status_Atk:
	case E_Status::Status_Max_Atk:
		maxAtk = F_Clamp(maxAtk + increaseValue, minValue, STATUS_LIMIT_ATK);
		if (changeCur) {
			EnqueueStatus(E_Status::Status_Atk);
			curAtk = maxAtk;
		}
		EnqueueStatus(E_Status::Status_Max_Atk);
		dataChangeSign = true;
		break;
	case E_Status::Status_Def:
	case E_Status::Status_Max_Def:
		maxDef = F_Clamp(maxDef + increaseValue, minValue, STATUS_LIMIT_DEF);
		if (changeCur) {
			EnqueueStatus(E_Status::Status_Def);
			curDef = maxDef;
		}
		EnqueueStatus(E_Status::Status_Max_Def);
		dataChangeSign = true;
		break;
	default:
		break;
	}
}


void Status::ChangeData(E_Status statusType, float increaseValue, bool lockMin = false)
{
	float setValue = increaseValue;
	float minValue = lockMin ? 1 : 0;
	switch (statusType) {
	case E_Status::Status_Hp:
		curHp = F_Clamp(curHp + increaseValue, minValue, maxHp);

		if (curHp <= 0) {
			isDead = true;
		}
		EnqueueStatus(E_Status::Status_Hp);
		dataChangeSign = true;
		break;
	case E_Status::Status_Atk:
		curAtk = F_Clamp(curAtk + increaseValue, minValue, maxAtk);
		EnqueueStatus(E_Status::Status_Atk);
		dataChangeSign = true;
		break;
	case E_Status::Status_Def:
		curDef = F_Clamp(curDef + increaseValue, minValue, maxDef);
		EnqueueStatus(E_Status::Status_Def);
		dataChangeSign = true;
		break;
	default:
		break;
	}
}

void Status::ResetData(E_Status statusType)
{
	switch (statusType) {
	case E_Status::Status_Hp:
		curHp = maxHp;
		EnqueueStatus(E_Status::Status_Hp);

		if (curHp > 0) {
			isDead = false;
		}
		break;
	case E_Status::Status_Atk:
		curAtk = maxAtk;
		EnqueueStatus(E_Status::Status_Atk);
		break;
	case E_Status::Status_Def:
		EnqueueStatus(E_Status::Status_Def);
		curDef = maxDef;
		break;
	case E_Status::Status_All:
		curHp = maxHp;
		curAtk = maxAtk;
		curDef = maxDef;
		EnqueueStatus(E_Status::Status_Hp);
		EnqueueStatus(E_Status::Status_Atk);
		EnqueueStatus(E_Status::Status_Def);
		if (curHp > 0) {
			isDead = false;
		}
		break;
	default:
		break;
	}
	dataChangeSign = true;
}

void Status::SetInvincible(float invincibleT)
{
	if (isInvincible == true)return;
	invincibleTime = invincibleT;
	invincibleTimePass = 0.0f;
	isInvincible = true;
}

bool Status::CheckCanBeAttack()
{
	return !isInvincible;
}

void Status::DoUpdate(float deltatime)
{
	if (isDead == true) {
		return;
	}

	if (isInvincible) {
		invincibleTimePass += deltatime;
		if (invincibleTimePass >= invincibleTime) {
			isInvincible = false;
		}
	}

	if (isDead == false && (int)(curHp + 0.5f) <= 0) {
		isDead = true;
	}
}

void Status::DoLateUpdate(float deltatime)
{
}

int Status::GetValue(E_Status statusType)
{
	switch (statusType) {
	case E_Status::Status_Hp:
		return curHp;
	case E_Status::Status_Atk:
		return curAtk;
	case E_Status::Status_Def:
		return curDef;
	case E_Status::Status_Max_Hp:
		return maxHp;
	case E_Status::Status_Max_Atk:
		return maxAtk;
	case E_Status::Status_Max_Def:
		return maxDef;
	default:
		break;
	}

	return 0;
}

bool Status::IsDead()
{
	return isDead;
}

std::list<E_Status>& Status::GetChangingStatusQueue()
{
	return changingStatusQueue;
}

#pragma region player_status
Status_Player::Status_Player()
{

}

Status_Player::~Status_Player() {

}

void Status_Player::SetDefaultData(E_Status statusType, float value, bool changeCur)
{
	float setValue = 0;
	switch (statusType) {
	case E_Status::Status_Exp:
		setValue = F_Clamp(value, 0, STATUS_LIMIT_EXP);
		maxExp = setValue;
		if (changeCur) {
			EnqueueStatus(E_Status::Status_Exp);
			curExp = maxExp;
		}
		EnqueueStatus(E_Status::Status_Max_Exp);
		dataChangeSign = true;
		break;
	case E_Status::Status_Level:
		setValue = F_Clamp(value, 0, PLAYER_LIMIT_LV);
		curLv = setValue;
		EnqueueStatus(E_Status::Status_Level);
		dataChangeSign = true;
		break;
	case E_Status::Status_Consume_Hp:
		setValue = F_Clamp(value, 0, PLAYER_STATUS_LIMIT_CONSUME_HP);
		hpBulletConsume = setValue;
		EnqueueStatus(E_Status::Status_Consume_Hp);
		dataChangeSign = true;
		break;
	case E_Status::Status_Consume_Atk:
		setValue = F_Clamp(value, 0, PLAYER_STATUS_LIMIT_CONSUME_ATK);
		atkBulletConsume = setValue;
		EnqueueStatus(E_Status::Status_Consume_Atk);
		dataChangeSign = true;
		break;
	case E_Status::Status_Consume_Def:
		setValue = F_Clamp(value, 0, PLAYER_STATUS_LIMIT_CONSUME_DEF);
		defBulletConsume = setValue;
		EnqueueStatus(E_Status::Status_Consume_Def);
		dataChangeSign = true;
		break;
	case E_Status::Status_Consume_Exp:
		setValue = F_Clamp(value, 0, PLAYER_STATUS_LIMIT_CONSUME_EXP);
		expBulletConsume = setValue;
		EnqueueStatus(E_Status::Status_Consume_Exp);
		dataChangeSign = true;
		break;
	default:
		Status::SetDefaultData(statusType, value, changeCur);
		break;
	}
}

void Status_Player::ChangeDefaultData(E_Status statusType, float increaseValue, bool changeCur, bool lockMin)
{
	float minValue = lockMin ? 1 : 0;
	switch (statusType) {
	case E_Status::Status_Exp:
		maxExp = F_Clamp(maxExp + increaseValue, minValue, STATUS_LIMIT_EXP);
		if (changeCur) {
			EnqueueStatus(E_Status::Status_Exp);
			curExp = maxExp;
		}
		EnqueueStatus(E_Status::Status_Max_Exp);
		dataChangeSign = true;
		break;
	case E_Status::Status_Consume_Hp:
		hpBulletConsume = F_Clamp(hpBulletConsume + increaseValue + 0.5f, minValue, PLAYER_STATUS_LIMIT_CONSUME_HP);
		//if (changeCur)curHpBulletConsume = hpBulletConsume;
		EnqueueStatus(E_Status::Status_Consume_Hp);
		dataChangeSign = true;
		break;
	case E_Status::Status_Consume_Atk:
		atkBulletConsume = F_Clamp(atkBulletConsume + increaseValue + 0.5f, minValue, PLAYER_STATUS_LIMIT_CONSUME_ATK);
		//if (changeCur) curAtkBulletConsume = atkBulletConsume;
		EnqueueStatus(E_Status::Status_Consume_Atk);
		dataChangeSign = true;
		break;
	case E_Status::Status_Consume_Def:
		defBulletConsume = F_Clamp(defBulletConsume + increaseValue + 0.5f, minValue, PLAYER_STATUS_LIMIT_CONSUME_DEF);
		//if (changeCur) curDefBulletConsume = defBulletConsume;
		EnqueueStatus(E_Status::Status_Consume_Def);
		dataChangeSign = true;
		break;
	case E_Status::Status_Consume_Exp:
		expBulletConsume = F_Clamp(expBulletConsume + increaseValue + 0.5f, minValue, PLAYER_STATUS_LIMIT_CONSUME_EXP);
		//if (changeCur) curExpBulletConsume = expBulletConsume;
		EnqueueStatus(E_Status::Status_Consume_Exp);
		dataChangeSign = true;
		break;
	default:
		Status::ChangeDefaultData(statusType, increaseValue, changeCur, lockMin);
		break;
	}
}

void Status_Player::ChangeData(E_Status statusType, float increaseValue, bool lockMin)
{
	float setValue = increaseValue;
	float minValue = lockMin ? 1 : 0;
	switch (statusType) {
	case E_Status::Status_Exp:
		curExp = F_Clamp(curExp + increaseValue, minValue, maxExp);
		EnqueueStatus(E_Status::Status_Exp);
		dataChangeSign = true;
		break;
	case E_Status::Status_Consume_Hp:
		hpBulletConsume = F_Clamp(hpBulletConsume + increaseValue, minValue, PLAYER_STATUS_LIMIT_CONSUME_HP);
		EnqueueStatus(E_Status::Status_Consume_Hp);
		dataChangeSign = true;
		break;
	case E_Status::Status_Consume_Atk:
		atkBulletConsume = F_Clamp(atkBulletConsume + increaseValue, minValue, PLAYER_STATUS_LIMIT_CONSUME_ATK);
		EnqueueStatus(E_Status::Status_Consume_Atk);
		dataChangeSign = true;
		break;
	case E_Status::Status_Consume_Def:
		defBulletConsume = F_Clamp(defBulletConsume + increaseValue, minValue, PLAYER_STATUS_LIMIT_CONSUME_DEF);
		EnqueueStatus(E_Status::Status_Consume_Def);
		dataChangeSign = true;
		break;
	case E_Status::Status_Consume_Exp:
		expBulletConsume = F_Clamp(expBulletConsume + increaseValue, minValue, PLAYER_STATUS_LIMIT_CONSUME_EXP);
		EnqueueStatus(E_Status::Status_Consume_Exp);
		dataChangeSign = true;
		break;

	case E_Status::Status_Level:
		curLv = F_Clamp(curLv + increaseValue, minValue, PLAYER_LIMIT_LV);

		EnqueueStatus(E_Status::Status_Level);
		dataChangeSign = true;
		break;
	default:
		Status::ChangeData(statusType, increaseValue, lockMin);
		break;
	}

}

void Status_Player::DoUpdate(float deltatime)
{
	if (curExp >= maxExp) {
		ChangeData(E_Status::Status_Level, 0, false);

		GameMgr::GetInstance()->uiMgr->UpdateLvPanel(this, true);

		ChangeData(E_Status::Status_Level, 1, false);
		int tempExp = maxExp;
		ChangeDefaultData(E_Status::Status_Hp, GameMgr::GetInstance()->battleMgr->GetLvUpStatusParam(E_Status::Status_Hp,curLv), false, true);
		ChangeDefaultData(E_Status::Status_Atk, GameMgr::GetInstance()->battleMgr->GetLvUpStatusParam(E_Status::Status_Atk,curLv), false, true);
		ChangeDefaultData(E_Status::Status_Def, GameMgr::GetInstance()->battleMgr->GetLvUpStatusParam(E_Status::Status_Def,curLv), false, true);

		SetDefaultData(E_Status::Status_Exp, tempExp, true);
		ChangeDefaultData(E_Status::Status_Exp, GameMgr::GetInstance()->battleMgr->GetLvUpStatusParam(E_Status::Status_Exp,curLv), false, true);
		ResetData(E_Status::Status_All);

		Player* player = GameMgr::GetInstance()->sceneObjMgr->GetPlayer();
		if (player != nullptr) {
			GameMgr::GetInstance()->uiMgr->ShowLevelUpImg(true, player->GetTransform()->GetPos());
		}

		GameMgr::GetInstance()->uiMgr->UpdateLvPanel(this, false);
		//GameMgr::GetInstance()->uiMgr->ShowLvUpPanel(true);
		//GetTimer()->SetTimer(E_Timer::Timer_Game_1, 0.0f);
		GameMgr::GetInstance()->SwitchSubGameState(E_GameState::GameState_Game_Lv_Up, true);
	}

	if (dataChangeSign == true) {
		// update ui
		GameMgr::GetInstance()->uiMgr->UpdateHUD(this);
		changingStatusQueue.clear();
		dataChangeSign = false;
	}

	Status::DoUpdate(deltatime);
}

void Status_Player::ResetData(E_Status statusType)
{
	switch (statusType) {
	case E_Status::Status_Exp:
		curExp = 0;
		EnqueueStatus(E_Status::Status_Exp);
		break;
	case E_Status::Status_Consume_Hp:
		//curHpBulletConsume = hpBulletConsume;
		break;
	case E_Status::Status_Consume_Atk:
		//curAtkBulletConsume = atkBulletConsume;
		break;
	case E_Status::Status_Consume_Def:
		//curDefBulletConsume = defBulletConsume;
		break;
	case E_Status::Status_Consume_Exp:
		//curExpBulletConsume = expBulletConsume;
		break;
	case E_Status::Status_All:

		EnqueueStatus(E_Status::Status_Level);

		curExp = 0;
		EnqueueStatus(E_Status::Status_Exp);

		//curHpBulletConsume = hpBulletConsume;
		//curAtkBulletConsume = atkBulletConsume;
		//curDefBulletConsume = defBulletConsume;
		//curExpBulletConsume = expBulletConsume;

		EnqueueStatus(E_Status::Status_Consume_Hp);
		EnqueueStatus(E_Status::Status_Consume_Atk);
		EnqueueStatus(E_Status::Status_Consume_Def);
		EnqueueStatus(E_Status::Status_Consume_Exp);

		Status::ResetData(statusType);
		break;
	default:
		Status::ResetData(statusType);
		break;
	}
	dataChangeSign = true;
}

bool Status_Player::ShootStatus(E_Status statusType)
{
	bool canShoot = false;
	int consume = 0;
	switch (statusType) {
	case E_Status::Status_Hp:
		if (curHp - hpBulletConsume > 0) {
			canShoot = true;
			consume = hpBulletConsume;
		}
		break;
	case E_Status::Status_Atk:
		if (curAtk - atkBulletConsume > 0) {
			canShoot = true;
			consume = atkBulletConsume;
		}
		break;
	case E_Status::Status_Def:
		if (curDef - defBulletConsume > 0) {
			canShoot = true;
			consume = defBulletConsume;
		}
		break;
	case E_Status::Status_Exp:
		//if (curExp - expBulletConsume >= 0) {
		consume = expBulletConsume;
		//}
		canShoot = true;
	default:
		break;
	}

	if (canShoot) {
		this->ChangeData(statusType, -consume, false);
	}

	return canShoot;
}

int Status_Player::GetValue(E_Status statusType)
{
	switch (statusType) {
	case E_Status::Status_Exp:
		return curExp;
	case E_Status::Status_Max_Exp:
		return maxExp;
	case E_Status::Status_Level:
		return curLv;
	case E_Status::Status_Consume_Hp:
		return hpBulletConsume;
	case E_Status::Status_Consume_Atk:
		return atkBulletConsume;
	case E_Status::Status_Consume_Def:
		return defBulletConsume;
	case E_Status::Status_Consume_Exp:
		return expBulletConsume;
	default:
		return Status::GetValue(statusType);
	}

	return 0;
}

void Status_Player::ResetLevel()
{
	curLv = PLAYER_DEFAULT_LV;
	EnqueueStatus(E_Status::Status_Level);
	dataChangeSign = true;
}

void Status_Player::AddCard(bool isStatusCard, int lv, CardEffectData* card)
{
	if (card == nullptr)return;
	if (isStatusCard == true) {
		if (statusCardDic.find(lv) != statusCardDic.end()) {
			statusCardDic[lv] = card;
			card->ApplyEffect(this);
		}
		else {
			//error
		}
	}
	else {

	}
}



#pragma endregion player_status


#pragma region enemy_status

Status_Enemy::Status_Enemy()
{

}

Status_Enemy::~Status_Enemy() {

}

void Status_Enemy::SetDefaultData(E_Status statusType, float value, bool changeCur)
{
	switch (statusType) {
	default:
		Status::SetDefaultData(statusType, value, changeCur);
		break;
	}
}

void Status_Enemy::ChangeDefaultData(E_Status statusType, float increaseValue, bool changeCur, bool lockMin)
{
	switch (statusType) {
	default:
		Status::ChangeDefaultData(statusType, increaseValue, changeCur, lockMin);
		break;
	}
}

void Status_Enemy::ChangeData(E_Status statusType, float increaseValue, bool lockMin)
{

	switch (statusType) {
	default:
		Status::ChangeData(statusType, increaseValue, lockMin);
		break;
	}

}

void Status_Enemy::ResetData(E_Status statusType)
{
	switch (statusType) {
	default:
		Status::ResetData(statusType);
		break;
	}
}


#pragma endregion enemy_status

#pragma endregion status_method

#pragma region ai_method

AIComponent::AIComponent()
{
	fsmSystem = new FSMSystem();
}

AIComponent::~AIComponent()
{
	if (fsmSystem != nullptr) {
		delete fsmSystem;
	}
}

void AIComponent::DoInit()
{
	fsmSystem->SetParent(this->parent);
	fsmSystem->DoInit();
}

void AIComponent::UnInit()
{

}

void AIComponent::DoUpdate(float deltatime)
{
	if (enable == true) {
		fsmSystem->DoUpdate(deltatime);
	}
}

void AIComponent::Reset()
{
	fsmSystem->DoInit();
}

FSMSystem* AIComponent::GetFSMSystem()
{
	return fsmSystem;
}

#pragma endregion ai_method


