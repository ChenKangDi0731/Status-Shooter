#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "effectManager.h"
//========================
//マクロ定義
//========================
#define DEFAULT_CELL_COUNT 1000
#define DEFAULT_VERTEX_COUNT                                (DEFAULT_CELL_COUNT * 4)

VERTEX_3D defaultVertices[4];
VERTEX_3D defaultCircleVertices[16];

vector4s defaultColliderColor = vector4s(0.0f, 1.0f, 0.0f, 1.0f);

#define NUM_VERTEX 4

int indices[6] = {
    0,2,1,
    2,3,1
};

#pragma region scene_cell_param

VERTEX_3D cellVertexArray[DEFAULT_VERTEX_COUNT];
VERTEX_3D* rectColliderBoxVertexArray;
VERTEX_3D* circleColliderBoxVertexArray;
bool vertexDataSignArray[DEFAULT_CELL_COUNT];

#pragma endregion scene_cell_param


//========================
//グローバル変数
//========================
static	ID3D11Buffer* g_VertexBufferTriangle = nullptr;

static ID3D11Buffer* g_testVertexBuffer;

void InitSprite() {
    {
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(VERTEX_3D) * 4;//頂点1個＊頂点数のサイズで作る
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        GetDevice()->CreateBuffer(&bd, nullptr, &g_testVertexBuffer);
    }

}


void UninitSprite() {
    if (g_VertexBufferTriangle != nullptr) {
        g_VertexBufferTriangle->Release();
        g_VertexBufferTriangle = nullptr;
    }
}

#pragma region 描画

/// <summary>
/// 描画関数
/// </summary>
/// <param name="transform">Transformコンポーネント（位置座標）</param>
/// <param name="data">　描画パラメータ</param>
void DrawCell(Transform* transform, DrawData* data) {
    if (transform == nullptr || data == nullptr)return;

    if (data->GetDrawDataType() == E_DrawDataType::DrawData_Tile) {
        return;
    }
    else if (data->GetDrawDataType() == E_DrawDataType::DrawData_Normal) {

        GetDeviceContext()->PSSetShaderResources(0, 1, GetTexture(((ShaderParam_Normal*)(data->GetShaderParam()))->texNo));//テクスチャのセット
        vector3s offsetPos = GameMgr::GetInstance()->sceneMgr->GetSceneCameraPos();

        vector3s pos = transform->GetPos();//位置
        vector3s rot = transform->GetRot();//回転
        vector3s scale = transform->GetScale();//スケール
        D3DCOLOR Diffuse = data->GetColor();//カラー
        vector2s size = data->GetSize();//元のサイズ

        //OPENGLからDXへ移植のため、描画座標を修正
        pos.x += SCREEN_WIDTH * 0.5f;
        pos.y = (-pos.y + SCREEN_HEIGHT * 0.5f);

        //カメラ揺れ
        if (GameMgr::GetInstance()->effectMgr->cameraShakeSwitch == true && data->shakeWithCamera == true) {
            pos = pos + GameMgr::GetInstance()->effectMgr->shakePosOffset;
        }


        float halfX = (float)(fabs((double)scale.x * (double)size.x * 0.5f));//実際の横の半分のサイズ（スケールに応じて拡大や縮小
        float halfY = (float)(fabs((double)scale.y * (double)size.y * 0.5f));//実際の縦の半分のサイズ（スケールに応じて拡大や縮小

        //map vertex buffer
        D3D11_MAPPED_SUBRESOURCE msr;
        VERTEX_3D* curArray = nullptr;
        //map vertex buffer
        GetDeviceContext()->Map(g_testVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

        curArray = (VERTEX_3D*)msr.pData;

        curArray[0].Position = vector3s(-halfX, -halfY, pos.z);
        curArray[1].Position = vector3s(halfX, -halfY, pos.z);
        curArray[2].Position = vector3s(-halfX, halfY, pos.z);
        curArray[3].Position = vector3s(halfX, halfY, pos.z);
        curArray[0].Diffuse = Diffuse;
        curArray[1].Diffuse = Diffuse;
        curArray[2].Diffuse = Diffuse;
        curArray[3].Diffuse = Diffuse;

        // z軸回転
        if ((int)(rot.z) != 0) {
            float tempX = 0.0f;
            float tempY = 0.0f;
            float cosRotZ = cosf(rot.z * DEG2RAD);
            float sinRotZ = sinf(rot.z * DEG2RAD);
            for (int index = 0; index < 0 + 4; index++) {
                tempX = curArray[index].Position.x;
                tempY = curArray[index].Position.y;

                curArray[index].Position.x = (tempX * cosRotZ - tempY * sinRotZ);
                curArray[index].Position.y = (tempX * sinRotZ + tempY * cosRotZ);
            }
        }

        //mappint to [-1,1]
        for (int index = 0; index < 0 + 4; index++) {
            curArray[index].Position.x += pos.x;
            curArray[index].Position.y += pos.y;
        }

        int xRotateFactor = (int)rot.x != 0 ? -1 : 0;
        int yRotateFactor = (int)rot.y != 0 ? -1 : 0;

        ShaderParam_Normal* shaderData = (ShaderParam_Normal*)data->GetShaderParam();
        //テクスチャ―マッピング
        if (shaderData->usePreComputeUV == false) {//UV座標をテクスチャ―パラメータによって計算で取得する場合
            curArray[1].TexCoord = vector2s(shaderData->texIndex.x + 1 + xRotateFactor, shaderData->texIndex.y - yRotateFactor);//1,0 ---- 1
            curArray[0].TexCoord = vector2s(shaderData->texIndex.x - xRotateFactor, shaderData->texIndex.y - yRotateFactor);//0,0 ---- 0
            curArray[3].TexCoord = vector2s(shaderData->texIndex.x + 1 + xRotateFactor, shaderData->texIndex.y + 1 + yRotateFactor);//1,1 ---- 3
            curArray[2].TexCoord = vector2s(shaderData->texIndex.x - xRotateFactor, shaderData->texIndex.y + 1 + yRotateFactor);//0,1 ---- 2
        }
        else {//あらかじめ決まったUV座標を使う場合

            curArray[1].TexCoord = vector2s(shaderData->preComputeUV.x + (shaderData->preComputeUVSize.x * (xRotateFactor + 1)),
                shaderData->preComputeUV.y - (shaderData->preComputeUVSize.y * yRotateFactor));//---- 1
            curArray[0].TexCoord = vector2s(shaderData->preComputeUV.x - (shaderData->preComputeUVSize.x * xRotateFactor),
                shaderData->preComputeUV.y - (shaderData->preComputeUVSize.y * yRotateFactor));//---- 0
            curArray[3].TexCoord = vector2s(shaderData->preComputeUV.x + (shaderData->preComputeUVSize.x * (xRotateFactor + 1)),
                shaderData->preComputeUV.y + (shaderData->preComputeUVSize.y * (yRotateFactor + 1)));//---- 3
            curArray[2].TexCoord = vector2s(shaderData->preComputeUV.x - (shaderData->preComputeUVSize.x * xRotateFactor),
                shaderData->preComputeUV.y + (shaderData->preComputeUVSize.y * (yRotateFactor + 1)));//---- 2
        }

        //mapping to [0,1]
        for (int index = 0; index < 0 + 4; index++) {
            curArray[index].TexCoord.x *= shaderData->tex_texel_size.x;
            curArray[index].TexCoord.y *= shaderData->tex_texel_size.y;
        }

        GetDeviceContext()->Unmap(g_testVertexBuffer, 0);

        UINT stride = sizeof(VERTEX_3D);
        UINT offset = 0;

        GetDeviceContext()->IASetVertexBuffers(0, 1, &g_testVertexBuffer, &stride, &offset);

        GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

        GetDeviceContext()->Draw(NUM_VERTEX, 0);

    }
}

/// <summary>
/// 9Sliceタイル描画
/// </summary>
/// <param name="transform"></param>
/// <param name="data"></param>
void DrawTile(Transform* transform, DrawData* data) {
    if (data == NULL || data->GetDrawDataType() != E_DrawDataType::DrawData_Tile)return;


    TileData* tileData = data->GetTileData();
    if (tileData == NULL)return;


    vector3s pos = vector3s(0.0f, 0.0f, 0.0f);
    vector3s rot = pos;//z axis
    vector3s scale = vector3s(1.0f, 1.0f, 1.0f);
    if (transform != NULL) {
        pos = transform->GetPos();
        rot = transform->GetRot();
        scale = transform->GetScale();
    }

    vector2s size = data->GetSize();
    vector2s scaleSize = size * vector2s(scale.x, scale.y);
    float halfX = (float)fabs(scale.x * size.x * 0.5f);
    float halfY = (float)fabs(scale.y * size.y * 0.5f);

    vector2s defaultCornerSize_1 = tileData->cornerSize_1;
    vector2s defaultCornerSize_2 = tileData->cornerSize_2;
    vector2s cornerSize_1 = tileData->cornerSize_1;
    vector2s cornerSize_2 = tileData->cornerSize_2;
    cornerSize_1 = cornerSize_1 * tileData->cornerScale_1;
    cornerSize_2 = cornerSize_2 * tileData->cornerScale_2;

    if ((cornerSize_1.x + cornerSize_2.x) > tileData->texRawSize.x
        || (cornerSize_1.y + cornerSize_2.y) > tileData->texRawSize.y) {
        return;//size error
    }

    vector2s texSize_I = tileData->GetTexSize_Inverse();

    vector2s centerTexSize = tileData->GetCenterTexSize();
    vector2s centerSize = vector2s(size.x - cornerSize_1.x - cornerSize_2.x, size.y - cornerSize_1.y - cornerSize_2.y);

    //LT corner
    vector3s tl_tl = vector3s(pos.x - halfX, pos.y - halfY, pos.z);
    vector3s tl_tr = vector3s(tl_tl.x + cornerSize_1.x, tl_tl.y, pos.z);
    vector3s tl_bl = vector3s(tl_tl.x, tl_tl.y + cornerSize_1.y, pos.z);//origin point
    vector3s tl_br = vector3s(tl_tl.x + cornerSize_1.x, tl_tl.y + cornerSize_1.y, pos.z);

    vector2s tl_bl_uv = vector2s(0.0f, (tileData->texRawSize.y - defaultCornerSize_1.y) * texSize_I.y);//origin point
    vector2s tl_tl_uv = vector2s(0.0f, 1.0f);
    vector2s tl_tr_uv = vector2s(defaultCornerSize_1.x * texSize_I.x, 1.0f);
    vector2s tl_br_uv = vector2s(defaultCornerSize_1.x * texSize_I.x, (tileData->texRawSize.y - defaultCornerSize_1.y) * texSize_I.y);

    //RT corner
    vector3s tr_tr = vector3s(pos.x + halfX, pos.y - halfY, pos.z);
    vector3s tr_tl = vector3s(tr_tr.x - cornerSize_2.x, tr_tr.y, pos.z);
    vector3s tr_bl = vector3s(tr_tr.x - cornerSize_2.x, tr_tr.y + cornerSize_1.y, pos.z);//origin point
    vector3s tr_br = vector3s(tr_tr.x, tr_tr.y + cornerSize_1.y, pos.z);

    vector2s tr_bl_uv = vector2s((tileData->texRawSize.x - defaultCornerSize_2.x) * texSize_I.x,
        (tileData->texRawSize.y - defaultCornerSize_1.y) * texSize_I.y);//origin point
    vector2s tr_tl_uv = vector2s((tileData->texRawSize.x - defaultCornerSize_2.x) * texSize_I.x, 1.0f);
    vector2s tr_tr_uv = vector2s(1.0f, 1.0f);
    vector2s tr_br_uv = vector2s(1.0f, (tileData->texRawSize.y - defaultCornerSize_1.y) * texSize_I.y);

    //LB corner
    vector3s bl_bl = vector3s(pos.x - halfX, pos.y + halfY, pos.z);//origin point
    vector3s bl_tl = vector3s(bl_bl.x, bl_bl.y - cornerSize_2.y, pos.z);
    vector3s bl_tr = vector3s(bl_bl.x + cornerSize_1.x, bl_bl.y - cornerSize_2.y, pos.z);
    vector3s bl_br = vector3s(bl_bl.x + cornerSize_1.x, bl_bl.y, pos.z);

    vector2s bl_bl_uv = vector2s(0.0f, 0.0f);//origin point
    vector2s bl_tl_uv = vector2s(0.0f, defaultCornerSize_2.y * texSize_I.y);
    vector2s bl_tr_uv = vector2s(defaultCornerSize_1.x * texSize_I.x, defaultCornerSize_2.y * texSize_I.y);
    vector2s bl_br_uv = vector2s(defaultCornerSize_1.x * texSize_I.x, 0.0f);

    //RB corner
    vector3s br_br = vector3s(pos.x + halfX, pos.y + halfY, pos.z);
    vector3s br_bl = vector3s(br_br.x - cornerSize_2.x, br_br.y, pos.z);//origin point
    vector3s br_tr = vector3s(br_br.x, br_br.y - cornerSize_2.y, pos.z);
    vector3s br_tl = vector3s(br_br.x - cornerSize_2.x, br_br.y - cornerSize_2.y, pos.z);

    vector2s br_bl_uv = vector2s((tileData->texRawSize.x - defaultCornerSize_2.x) * texSize_I.x, 0.0f);//origin point
    vector2s br_tl_uv = vector2s((tileData->texRawSize.x - defaultCornerSize_2.x) * texSize_I.x,
        defaultCornerSize_2.y * texSize_I.y);
    vector2s br_tr_uv = vector2s(1.0f, defaultCornerSize_2.y * texSize_I.y);
    vector2s br_br_uv = vector2s(1.0f, 0.0f);

    if ((int)(rot.z) != 0) {
        float tempX = 0.0f;
        float tempY = 0.0f;
        float cosRotZ = cosf(rot.z * DEG2RAD);
        float sinRotZ = sinf(rot.z * DEG2RAD);

        //tl corner
        tempX = tl_tl.x;
        tempY = tl_tl.y;
        tl_tl.x = (tempX * cosRotZ - tempY * sinRotZ);
        tl_tl.y = (tempX * sinRotZ + tempY * cosRotZ);
        tempX = tl_bl.x;
        tempY = tl_bl.y;
        tl_bl.x = (tempX * cosRotZ - tempY * sinRotZ);
        tl_bl.y = (tempX * sinRotZ + tempY * cosRotZ);
        tempX = tl_tr.x;
        tempY = tl_tr.y;
        tl_tl.x = (tempX * cosRotZ - tempY * sinRotZ);
        tl_tl.y = (tempX * sinRotZ + tempY * cosRotZ);
        tempX = tl_br.x;
        tempY = tl_br.y;
        tl_bl.x = (tempX * cosRotZ - tempY * sinRotZ);
        tl_bl.y = (tempX * sinRotZ + tempY * cosRotZ);

        //bl corner
        tempX = bl_tl.x;
        tempY = bl_tl.y;
        bl_tl.x = (tempX * cosRotZ - tempY * sinRotZ);
        bl_tl.y = (tempX * sinRotZ + tempY * cosRotZ);
        tempX = bl_bl.x;
        tempY = bl_bl.y;
        bl_bl.x = (tempX * cosRotZ - tempY * sinRotZ);
        bl_bl.y = (tempX * sinRotZ + tempY * cosRotZ);
        tempX = bl_tr.x;
        tempY = bl_tr.y;
        bl_tl.x = (tempX * cosRotZ - tempY * sinRotZ);
        bl_tl.y = (tempX * sinRotZ + tempY * cosRotZ);
        tempX = bl_br.x;
        tempY = bl_br.y;
        bl_bl.x = (tempX * cosRotZ - tempY * sinRotZ);
        bl_bl.y = (tempX * sinRotZ + tempY * cosRotZ);

        //tr corner
        tempX = tr_tl.x;
        tempY = tr_tl.y;
        tr_tl.x = (tempX * cosRotZ - tempY * sinRotZ);
        tr_tl.y = (tempX * sinRotZ + tempY * cosRotZ);
        tempX = tr_bl.x;
        tempY = tr_bl.y;
        tr_bl.x = (tempX * cosRotZ - tempY * sinRotZ);
        tr_bl.y = (tempX * sinRotZ + tempY * cosRotZ);
        tempX = tr_tr.x;
        tempY = tr_tr.y;
        tr_tl.x = (tempX * cosRotZ - tempY * sinRotZ);
        tr_tl.y = (tempX * sinRotZ + tempY * cosRotZ);
        tempX = tr_br.x;
        tempY = tr_br.y;
        tr_bl.x = (tempX * cosRotZ - tempY * sinRotZ);
        tr_bl.y = (tempX * sinRotZ + tempY * cosRotZ);

        //br corner
        tempX = br_tl.x;
        tempY = br_tl.y;
        br_tl.x = (tempX * cosRotZ - tempY * sinRotZ);
        br_tl.y = (tempX * sinRotZ + tempY * cosRotZ);
        tempX = br_bl.x;
        tempY = br_bl.y;
        br_bl.x = (tempX * cosRotZ - tempY * sinRotZ);
        br_bl.y = (tempX * sinRotZ + tempY * cosRotZ);
        tempX = br_tr.x;
        tempY = br_tr.y;
        br_tl.x = (tempX * cosRotZ - tempY * sinRotZ);
        br_tl.y = (tempX * sinRotZ + tempY * cosRotZ);
        tempX = br_br.x;
        tempY = br_br.y;
        br_bl.x = (tempX * cosRotZ - tempY * sinRotZ);
        br_bl.y = (tempX * sinRotZ + tempY * cosRotZ);
    }


    DrawVertexData vertexData;

    //TL
    vertexData.tl = VecTransformTToD(tl_tl);
    vertexData.tr = VecTransformTToD(tl_tr);
    vertexData.bl = VecTransformTToD(tl_bl);
    vertexData.br = VecTransformTToD(tl_br);
    vertexData.tl_uv = tl_tl_uv;
    vertexData.tr_uv = tl_tr_uv;
    vertexData.bl_uv = tl_bl_uv;
    vertexData.br_uv = tl_br_uv;
    vertexData.AdjustTexCoord();
    vertexData.z = pos.z;
    vertexData.useDefaultTexture = true;
    DrawTilePart(vertexData, data);

    //TR
    vertexData.tl = VecTransformTToD(tr_tl);
    vertexData.tr = VecTransformTToD(tr_tr);
    vertexData.bl = VecTransformTToD(tr_bl);
    vertexData.br = VecTransformTToD(tr_br);
    vertexData.tl_uv = tr_tl_uv;
    vertexData.tr_uv = tr_tr_uv;
    vertexData.bl_uv = tr_bl_uv;
    vertexData.br_uv = tr_br_uv;
    vertexData.AdjustTexCoord();
    vertexData.useDefaultTexture = true;
    DrawTilePart(vertexData, data);

    //BL
    vertexData.tl = VecTransformTToD(bl_tl);
    vertexData.tr = VecTransformTToD(bl_tr);
    vertexData.bl = VecTransformTToD(bl_bl);
    vertexData.br = VecTransformTToD(bl_br);
    vertexData.tl_uv = bl_tl_uv;
    vertexData.tr_uv = bl_tr_uv;
    vertexData.bl_uv = bl_bl_uv;
    vertexData.br_uv = bl_br_uv;
    vertexData.AdjustTexCoord();
    vertexData.useDefaultTexture = true;
    DrawTilePart(vertexData, data);

    //BR
    vertexData.tl = VecTransformTToD(br_tl);
    vertexData.tr = VecTransformTToD(br_tr);
    vertexData.bl = VecTransformTToD(br_bl);
    vertexData.br = VecTransformTToD(br_br);
    vertexData.tl_uv = br_tl_uv;
    vertexData.tr_uv = br_tr_uv;
    vertexData.bl_uv = br_bl_uv;
    vertexData.br_uv = br_br_uv;
    vertexData.AdjustTexCoord();
    vertexData.useDefaultTexture = true;
    DrawTilePart(vertexData, data);

    vector2s topBorderSize = vector2s(scaleSize.x - cornerSize_1.x - cornerSize_2.x, cornerSize_1.y);
    vector2s bottomBorderSize = vector2s(scaleSize.x - cornerSize_1.x - cornerSize_2.x, cornerSize_2.y);
    vector2s leftBorderSize = vector2s(cornerSize_1.x, scaleSize.y - cornerSize_1.y - cornerSize_2.y);
    vector2s rightBorderSize = vector2s(cornerSize_2.x, scaleSize.y - cornerSize_1.y - cornerSize_2.y);

    /// <summary>
    /// スケーリングモード
    /// </summary>
    /// <param name="transform"></param>
    /// <param name="data"></param>
    if (tileData->borderTileType == E_TileType::TileType_Stretch) {
        //Up Border
        vector3s top_border_tl = tl_tr;
        vector3s top_border_bl = tl_br;
        vector3s top_border_tr = tr_tl;
        vector3s top_border_br = tr_bl;

        vector2s top_border_tl_uv = tl_tr_uv;
        vector2s top_border_bl_uv = tl_br_uv;
        vector2s top_border_tr_uv = tr_tl_uv;
        vector2s top_border_br_uv = tr_bl_uv;
        vertexData.tl = VecTransformTToD(top_border_tl);
        vertexData.tr = VecTransformTToD(top_border_tr);
        vertexData.bl = VecTransformTToD(top_border_bl);
        vertexData.br = VecTransformTToD(top_border_br);
        vertexData.tl_uv = top_border_tl_uv;
        vertexData.tr_uv = top_border_tr_uv;
        vertexData.bl_uv = top_border_bl_uv;
        vertexData.br_uv = top_border_br_uv;
        vertexData.AdjustTexCoord();
        vertexData.useDefaultTexture = true;
        DrawTilePart(vertexData, data);

        //Bottom Border
        vector3s bottom_border_tl = bl_tr;
        vector3s bottom_border_bl = bl_br;
        vector3s bottom_border_tr = br_tl;
        vector3s bottom_border_br = br_bl;

        vector2s bottom_border_tl_uv = bl_tr_uv;
        vector2s bottom_border_bl_uv = bl_br_uv;
        vector2s bottom_border_tr_uv = br_tl_uv;
        vector2s bottom_border_br_uv = br_bl_uv;
        vertexData.tl = VecTransformTToD(bottom_border_tl);
        vertexData.tr = VecTransformTToD(bottom_border_tr);
        vertexData.bl = VecTransformTToD(bottom_border_bl);
        vertexData.br = VecTransformTToD(bottom_border_br);
        vertexData.tl_uv = bottom_border_tl_uv;
        vertexData.tr_uv = bottom_border_tr_uv;
        vertexData.bl_uv = bottom_border_bl_uv;
        vertexData.br_uv = bottom_border_br_uv;
        vertexData.AdjustTexCoord();
        vertexData.useDefaultTexture = true;
        DrawTilePart(vertexData, data);


        //Left Border
        vector3s left_border_tl = tl_bl;
        vector3s left_border_bl = bl_tl;
        vector3s left_border_tr = tl_br;
        vector3s left_border_br = bl_tr;

        vector2s left_border_tl_uv = tl_bl_uv;
        vector2s left_border_bl_uv = bl_tl_uv;
        vector2s left_border_tr_uv = tl_br_uv;
        vector2s left_border_br_uv = bl_tr_uv;
        vertexData.tl = VecTransformTToD(left_border_tl);
        vertexData.tr = VecTransformTToD(left_border_tr);
        vertexData.bl = VecTransformTToD(left_border_bl);
        vertexData.br = VecTransformTToD(left_border_br);
        vertexData.tl_uv = left_border_tl_uv;
        vertexData.tr_uv = left_border_tr_uv;
        vertexData.bl_uv = left_border_bl_uv;
        vertexData.br_uv = left_border_br_uv;
        vertexData.AdjustTexCoord();
        vertexData.useDefaultTexture = true;
        DrawTilePart(vertexData, data);

        //Right Border
        vector3s right_border_tl = tr_bl;
        vector3s right_border_bl = br_tl;
        vector3s right_border_tr = tr_br;
        vector3s right_border_br = br_tr;

        vector2s right_border_tl_uv = tr_bl_uv;
        vector2s right_border_bl_uv = br_tl_uv;
        vector2s right_border_tr_uv = tr_br_uv;
        vector2s right_border_br_uv = br_tr_uv;
        vertexData.tl = VecTransformTToD(right_border_tl);
        vertexData.tr = VecTransformTToD(right_border_tr);
        vertexData.bl = VecTransformTToD(right_border_bl);
        vertexData.br = VecTransformTToD(right_border_br);
        vertexData.tl_uv = right_border_tl_uv;
        vertexData.tr_uv = right_border_tr_uv;
        vertexData.bl_uv = right_border_bl_uv;
        vertexData.br_uv = right_border_br_uv;
        vertexData.AdjustTexCoord();
        vertexData.useDefaultTexture = true;
        DrawTilePart(vertexData, data);

    }
    else if (tileData->borderTileType == E_TileType::TileType_Repeat) {//テクスチャーリピートモード

        //Up Border
        vector3s top_border_tl = tl_tr;
        vector3s top_border_bl = tl_br;
        vector3s top_border_tr = tr_tl;
        vector3s top_border_br = tr_bl;
        if (tileData->topBorderTex == -1) {

            vector2s top_border_tl_uv = tl_tr_uv;
            vector2s top_border_bl_uv = tl_br_uv;
            vector2s top_border_tr_uv = tr_tl_uv;
            vector2s top_border_br_uv = tr_bl_uv;
            vertexData.tl = VecTransformTToD(top_border_tl);
            vertexData.tr = VecTransformTToD(top_border_tr);
            vertexData.bl = VecTransformTToD(top_border_bl);
            vertexData.br = VecTransformTToD(top_border_br);
            vertexData.tl_uv = top_border_tl_uv;
            vertexData.tr_uv = top_border_tr_uv;
            vertexData.bl_uv = top_border_bl_uv;
            vertexData.br_uv = top_border_br_uv;
            vertexData.AdjustTexCoord();
            vertexData.useDefaultTexture = true;
            DrawTilePart(vertexData, data);
        }
        else {
            float repeatCount = tileData->tb_repeatCount.x;
            if (repeatCount <= 0) {
                repeatCount = topBorderSize.x < 0.000001f ? 1.0f : topBorderSize.x / (tileData->topBorderTexSize.x * tileData->tb_texScale.x);
            }
            else {
                repeatCount = 1.0f;
            }

            vector2s top_border_tl_uv = vector2s(0.0f, 1.0f);
            vector2s top_border_bl_uv = vector2s(0.0f, 0.0f);
            vector2s top_border_tr_uv = vector2s(repeatCount, 1.0f);
            vector2s top_border_br_uv = vector2s(repeatCount, 0.0f);
            vertexData.tl = VecTransformTToD(top_border_tl);
            vertexData.tr = VecTransformTToD(top_border_tr);
            vertexData.bl = VecTransformTToD(top_border_bl);
            vertexData.br = VecTransformTToD(top_border_br);
            vertexData.tl_uv = top_border_tl_uv;
            vertexData.tr_uv = top_border_tr_uv;
            vertexData.bl_uv = top_border_bl_uv;
            vertexData.br_uv = top_border_br_uv;
            vertexData.AdjustTexCoord();
            vertexData.useDefaultTexture = false;
            vertexData.curTexNo = tileData->topBorderTex;
            DrawTilePart(vertexData, data);
        }

        //Bottom Border
        vector3s bottom_border_tl = bl_tr;
        vector3s bottom_border_bl = bl_br;
        vector3s bottom_border_tr = br_tl;
        vector3s bottom_border_br = br_bl;
        if (tileData->bottomBorderTex == -1) {

            vector2s bottom_border_tl_uv = bl_tr_uv;
            vector2s bottom_border_bl_uv = bl_br_uv;
            vector2s bottom_border_tr_uv = br_tl_uv;
            vector2s bottom_border_br_uv = br_bl_uv;
            vertexData.tl = VecTransformTToD(bottom_border_tl);
            vertexData.tr = VecTransformTToD(bottom_border_tr);
            vertexData.bl = VecTransformTToD(bottom_border_bl);
            vertexData.br = VecTransformTToD(bottom_border_br);
            vertexData.tl_uv = bottom_border_tl_uv;
            vertexData.tr_uv = bottom_border_tr_uv;
            vertexData.bl_uv = bottom_border_bl_uv;
            vertexData.br_uv = bottom_border_br_uv;
            vertexData.AdjustTexCoord();
            vertexData.useDefaultTexture = true;
            DrawTilePart(vertexData, data);
        }
        else {
            float repeatCount = tileData->tb_repeatCount.y;
            if (repeatCount <= 0) {
                repeatCount = bottomBorderSize.x < 0.000001f ? 1.0f : bottomBorderSize.x / (tileData->bottomBorderTexSize.x * tileData->tb_texScale.y);
            }
            else {
                repeatCount = 1.0f;
            }

            vector2s bottom_border_tl_uv = vector2s(0.0f, 1.0f);
            vector2s bottom_border_bl_uv = vector2s(0.0f, 0.0f);
            vector2s bottom_border_tr_uv = vector2s(repeatCount, 1.0f);
            vector2s bottom_border_br_uv = vector2s(repeatCount, 0.0f);
            vertexData.tl = VecTransformTToD(bottom_border_tl);
            vertexData.tr = VecTransformTToD(bottom_border_tr);
            vertexData.bl = VecTransformTToD(bottom_border_bl);
            vertexData.br = VecTransformTToD(bottom_border_br);
            vertexData.tl_uv = bottom_border_tl_uv;
            vertexData.tr_uv = bottom_border_tr_uv;
            vertexData.bl_uv = bottom_border_bl_uv;
            vertexData.br_uv = bottom_border_br_uv;
            vertexData.AdjustTexCoord();
            vertexData.useDefaultTexture = false;
            vertexData.curTexNo = tileData->bottomBorderTex;
            DrawTilePart(vertexData, data);
        }

        //Left Border
        vector3s left_border_tl = tl_bl;
        vector3s left_border_bl = bl_tl;
        vector3s left_border_tr = tl_br;
        vector3s left_border_br = bl_tr;
        if (tileData->leftBorderTex == -1) {

            vector2s left_border_tl_uv = tl_bl_uv;
            vector2s left_border_bl_uv = bl_tl_uv;
            vector2s left_border_tr_uv = tl_br_uv;
            vector2s left_border_br_uv = bl_tr_uv;
            vertexData.tl = VecTransformTToD(left_border_tl);
            vertexData.tr = VecTransformTToD(left_border_tr);
            vertexData.bl = VecTransformTToD(left_border_bl);
            vertexData.br = VecTransformTToD(left_border_br);
            vertexData.tl_uv = left_border_tl_uv;
            vertexData.tr_uv = left_border_tr_uv;
            vertexData.bl_uv = left_border_bl_uv;
            vertexData.br_uv = left_border_br_uv;
            vertexData.AdjustTexCoord();
            vertexData.useDefaultTexture = true;
            DrawTilePart(vertexData, data);
        }
        else {
            float repeatCount = tileData->lr_repeatCount.x;
            if (repeatCount <= 0) {
                repeatCount = leftBorderSize.y < 0.000001f ? 1.0f : leftBorderSize.y / (tileData->leftBorderTexSize.y * tileData->lr_texScale.x);
            }
            else {
                repeatCount = 1.0f;
            }

            vector2s left_border_tl_uv = vector2s(0.0f, repeatCount);
            vector2s left_border_bl_uv = vector2s(0.0f, 0.0f);
            vector2s left_border_tr_uv = vector2s(1.0f, repeatCount);
            vector2s left_border_br_uv = vector2s(1.0f, 0.0f);
            vertexData.tl = VecTransformTToD(left_border_tl);
            vertexData.tr = VecTransformTToD(left_border_tr);
            vertexData.bl = VecTransformTToD(left_border_bl);
            vertexData.br = VecTransformTToD(left_border_br);
            vertexData.tl_uv = left_border_tl_uv;
            vertexData.tr_uv = left_border_tr_uv;
            vertexData.bl_uv = left_border_bl_uv;
            vertexData.br_uv = left_border_br_uv;
            vertexData.useDefaultTexture = false;
            vertexData.curTexNo = tileData->leftBorderTex;
            DrawTilePart(vertexData, data);
        }

        //Right Border
        vector3s right_border_tl = tr_bl;
        vector3s right_border_bl = br_tl;
        vector3s right_border_tr = tr_br;
        vector3s right_border_br = br_tr;
        if (tileData->rightBorderTex == -1) {

            vector2s right_border_tl_uv = tr_bl_uv;
            vector2s right_border_bl_uv = br_tl_uv;
            vector2s right_border_tr_uv = tr_br_uv;
            vector2s right_border_br_uv = br_tr_uv;
            vertexData.tl = VecTransformTToD(right_border_tl);
            vertexData.tr = VecTransformTToD(right_border_tr);
            vertexData.bl = VecTransformTToD(right_border_bl);
            vertexData.br = VecTransformTToD(right_border_br);
            vertexData.tl_uv = right_border_tl_uv;
            vertexData.tr_uv = right_border_tr_uv;
            vertexData.bl_uv = right_border_bl_uv;
            vertexData.br_uv = right_border_br_uv;
            vertexData.AdjustTexCoord();
            vertexData.useDefaultTexture = true;
            DrawTilePart(vertexData, data);
        }
        else {
            float repeatCount = tileData->lr_repeatCount.y;
            if (repeatCount <= 0) {
                repeatCount = rightBorderSize.y < 0.000001f ? 1.0f : rightBorderSize.y / (tileData->rightBorderTexSize.y * tileData->tb_texScale.y);
            }
            else {
                repeatCount = 1.0f;
            }

            vector2s right_border_tl_uv = vector2s(0.0f, repeatCount);
            vector2s right_border_bl_uv = vector2s(0.0f, 0.0f);
            vector2s right_border_tr_uv = vector2s(1.0f, repeatCount);
            vector2s right_border_br_uv = vector2s(1.0f, 0.0f);
            vertexData.tl = VecTransformTToD(right_border_tl);
            vertexData.tr = VecTransformTToD(right_border_tr);
            vertexData.bl = VecTransformTToD(right_border_bl);
            vertexData.br = VecTransformTToD(right_border_br);
            vertexData.tl_uv = right_border_tl_uv;
            vertexData.tr_uv = right_border_tr_uv;
            vertexData.bl_uv = right_border_bl_uv;
            vertexData.br_uv = right_border_br_uv;
            vertexData.useDefaultTexture = false;
            vertexData.curTexNo = tileData->rightBorderTex;
            DrawTilePart(vertexData, data);
        }
    }

    //スケーリングモード
    if (tileData->centerTileType == E_TileType::TileType_Stretch) {
        //center
        vector3s center_tl = tl_br;
        vector3s center_bl = bl_tr;
        vector3s center_tr = tr_bl;
        vector3s center_br = br_tl;

        vector2s center_tl_uv = tl_br_uv;
        vector2s center_bl_uv = bl_tr_uv;
        vector2s center_tr_uv = tr_bl_uv;
        vector2s center_br_uv = br_tl_uv;

        //RIGHT
        vertexData.tl = VecTransformTToD(center_tl);
        vertexData.tr = VecTransformTToD(center_tr);
        vertexData.bl = VecTransformTToD(center_bl);
        vertexData.br = VecTransformTToD(center_br);
        vertexData.tl_uv = center_tl_uv;
        vertexData.tr_uv = center_tr_uv;
        vertexData.bl_uv = center_bl_uv;
        vertexData.br_uv = center_br_uv;
        vertexData.AdjustTexCoord();
        vertexData.useDefaultTexture = false;
        vertexData.curTexNo = tileData->centerTex;
        DrawTilePart(vertexData, data);
    }
    else if (tileData->centerTileType == E_TileType::TileType_Repeat) {//テクスチャーリピートモード
        //center
        vector3s center_tl = tl_br;
        vector3s center_bl = bl_tr;
        vector3s center_tr = tr_bl;
        vector3s center_br = br_tl;

        if (tileData->centerTex == -1) {

            vector2s center_tl_uv = tl_br_uv;
            vector2s center_bl_uv = bl_tr_uv;
            vector2s center_tr_uv = tr_bl_uv;
            vector2s center_br_uv = br_tl_uv;

            vertexData.tl = VecTransformTToD(center_tl);
            vertexData.tr = VecTransformTToD(center_tr);
            vertexData.bl = VecTransformTToD(center_bl);
            vertexData.br = VecTransformTToD(center_br);
            vertexData.tl_uv = center_tl_uv;
            vertexData.tr_uv = center_tr_uv;
            vertexData.bl_uv = center_bl_uv;
            vertexData.br_uv = center_br_uv;
            vertexData.AdjustTexCoord();
            vertexData.useDefaultTexture = true;
            DrawTilePart(vertexData, data);
        }
        else {
            float repeatCountX = 1.0f;
            float repeatCountY = 1.0f;

            repeatCountX = topBorderSize.x < 0.000001f ? 1.0f : topBorderSize.x / (tileData->centerTexSize.x * tileData->center_texScale.x);
            repeatCountY = rightBorderSize.y < 0.000001f ? 1.0f : rightBorderSize.y / (tileData->centerTexSize.y * tileData->center_texScale.y);

            vector2s center_tl_uv = vector2s(0.0f, repeatCountY);
            vector2s center_bl_uv = vector2s(0.0f, 0.0f);
            vector2s center_tr_uv = vector2s(repeatCountX, repeatCountY);
            vector2s center_br_uv = vector2s(repeatCountX, 0.0f);

            vertexData.tl = VecTransformTToD(center_tl);
            vertexData.tr = VecTransformTToD(center_tr);
            vertexData.bl = VecTransformTToD(center_bl);
            vertexData.br = VecTransformTToD(center_br);
            vertexData.tl_uv = center_tl_uv;
            vertexData.tr_uv = center_tr_uv;
            vertexData.bl_uv = center_bl_uv;
            vertexData.br_uv = center_br_uv;
            vertexData.AdjustTexCoord();
            vertexData.useDefaultTexture = false;
            vertexData.curTexNo = tileData->centerTex;
            DrawTilePart(vertexData, data);
        }
    }
}

/// <summary>
/// 9sliceタイル描画（一部
/// </summary>
/// <param name="drawData"></param>
/// <param name="data"></param>
void DrawTilePart(DrawVertexData& drawData, DrawData* data) {
    if (data == NULL)return;

    if (drawData.useDefaultTexture == true) {
        GetDeviceContext()->PSSetShaderResources(0, 1, GetTexture(data->GetTextureNo()));//テクスチャのセット

    }
    else {
        GetDeviceContext()->PSSetShaderResources(0, 1, GetTexture(drawData.curTexNo));//テクスチャのセット
    }

    D3D11_MAPPED_SUBRESOURCE msr;
    GetDeviceContext()->Map(g_VertexBufferTriangle, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

    VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

    vertex[0].Position = vector3s(drawData.tl.x, drawData.tl.y, drawData.z);
    vertex[1].Position = vector3s(drawData.tr.x, drawData.tr.y, drawData.z);
    vertex[2].Position = vector3s(drawData.bl.x, drawData.bl.y, drawData.z);
    vertex[3].Position = vector3s(drawData.br.x, drawData.br.y, drawData.z);
    vertex[0].TexCoord = drawData.tl_uv;
    vertex[1].TexCoord = drawData.tr_uv;
    vertex[2].TexCoord = drawData.bl_uv;
    vertex[3].TexCoord = drawData.br_uv;

    vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    GetDeviceContext()->Unmap(g_VertexBufferTriangle, 0);

    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;

    GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBufferTriangle, &stride, &offset);

    GetDeviceContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    GetDeviceContext()->Draw(NUM_VERTEX, 0);

}

/// <summary>
/// 補助線を描画
/// </summary>
/// <param name="startPos"></param>
/// <param name="dir"></param>
/// <param name="length"></param>
void DrawLine(vector3s startPos, vector3s dir, float length)
{
    startPos.z = 0 * screen_size.y * 2.0f;
    vector3s dirNormal = VecNormalize(dir);
    vector3s endPos = startPos + dirNormal * length;

    //OPENGLからDXへ移植のため、描画座標を修正
    vector3s start = startPos;

    start.x += SCREEN_WIDTH * 0.5f;
    start.y = (-start.y + SCREEN_HEIGHT * 0.5f);

    endPos.x += SCREEN_WIDTH * 0.5f;
    endPos.y = (-endPos.y + SCREEN_HEIGHT * 0.5f);

    //カメラ揺れ
    if (GameMgr::GetInstance()->effectMgr->cameraShakeSwitch == true) {
        start = start + GameMgr::GetInstance()->effectMgr->shakePosOffset;
        endPos = endPos + GameMgr::GetInstance()->effectMgr->shakePosOffset;
    }

    D3D11_MAPPED_SUBRESOURCE msr;
    //頂点バッファへの書き込み許可をもらう
    GetDeviceContext()->Map(g_testVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
    //頂点構造体の型にポインター型を変gg換をする
    VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

    vertex[0].Position = start;
    vertex[1].Position = endPos;
    vertex[2].Position = endPos;
    vertex[3].Position = endPos;


    //mappint to [-1,1]
    for (int index = 0; index < 4; index++) {
        vertex[index].TexCoord = D3DXVECTOR2(1, 1);
        vertex[index].Diffuse = D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f);
    }

    GetDeviceContext()->Unmap(g_testVertexBuffer, 0);

    GetDeviceContext()->PSSetShaderResources(0, 1, GetTexture(LoadTexture((char*)"data\\texture\\fade_white.png")));

    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    GetDeviceContext()->IASetVertexBuffers(0, 1, &g_testVertexBuffer, &stride, &offset);
    GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

    GetDeviceContext()->Draw(4, 0);
}

#pragma endregion 描画

int GetEmptyCellVertexDataIndex(E_BatchingType batchingType)
{
    switch (batchingType) {
    case E_BatchingType::BatchingType_None:
        for (int index = 0; index < DEFAULT_CELL_COUNT; index++) {
            if (vertexDataSignArray[index] == false) {
                return index;
            }
        }
        break;
    default:
        for (int index = 0; index < DEFAULT_CELL_COUNT; index++) {
            if (vertexDataSignArray[index] == false) {
                return index;
            }
        }
        break;
    }

    return -1;
}

void UpdateCellVertexArrayData(E_BatchingType batchingType, int index, bool state)
{
    if (index < 0)return;

    switch (batchingType) {
    case E_BatchingType::BatchingType_None:
        if (index >= DEFAULT_CELL_COUNT)return;
        vertexDataSignArray[index] = state;
        break;
    default:
        if (index >= DEFAULT_CELL_COUNT)return;
        vertexDataSignArray[index] = state;
        break;
    }
}

