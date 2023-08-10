//=============================
//
//スプライト描画処理
//=============================


#pragma once
#include "main.h"

//========================
//マクロ定義
//========================
class Transform;
class Collider;
class DrawData;
struct DrawVertexData;
class Bullet_Laser;

struct BaseShaderParam;
struct VERTEX_3D;

enum class E_BatchingType;


//========================
//プロトタイプ宣言
//========================
void InitSprite();
void UninitSprite();

void DrawCell(Transform* transform, DrawData* data);

void DrawTile(Transform* transform, DrawData* data);
void DrawTilePart(DrawVertexData& drawData, DrawData* data);



void DrawLine(vector3s startPos, vector3s dir, float length);

#pragma region extern_method

int GetEmptyCellVertexDataIndex(E_BatchingType batchingType);

void UpdateCellVertexArrayData(E_BatchingType batchingType, int index, bool state);

#pragma endregion extern_method

