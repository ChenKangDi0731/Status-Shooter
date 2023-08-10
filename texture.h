#pragma once
#include "renderer.h"


enum class E_Texture : int {
	Texture_None = -1,
	Texture_Test_1 = 0,
	Texture_Test_2 = 1,

	Texture_Floor,
	Texture_Character,
	Texture_Floor_Test,

	Texture_Colony_Green_Stone,
	Texture_Colony_Green_Normal,
	Texture_Colony_Green_Movement,
	Texture_Colony_Red_Stone,
	Texture_Colony_Red_Normal,
	Texture_Colony_Red_Movement,

	//border
	Texture_Colony_Green_Stone_Bottom,
	Texture_Colony_Green_Normal_Bottom,
	Texture_Colony_Green_Movement_Bottom,
	Texture_Colony_Red_Stone_Bottom,
	Texture_Colony_Red_Normal_Bottom,
	Texture_Colony_Red_Movement_Bottom,

	//character
	Texture_Character_Package,

	//bullet
	Texture_Bullet_04,
	Texture_Bullet_RedLaser,
	Texture_Bullet_Cannon,
	Texture_Bullet_Package,

	//effect
	Texture_HitEffect,
	Texture_Effect_Package,

	//UI
	Texture_UI_HUD_Status,
	Texture_UI_HUD_Package_Test_1,
	Texture_UI_HUD_Package_Test_2,

	//other
	//background
	Texture_Grass,
};

enum class E_TextureType {
	RGB = 1,
	RGBA = 2,
};

//テクスチャのロード
//IN テクスチャファイル名の仙頭ポンタ
int LoadTexture(const char* filename);

//テクスチャの終了処理
//ロード済みのテクスチャをすべて解放
void UninitTexture();


//指定のテクスチャ情報を取得
//IN テクスチャハンドル
ID3D11ShaderResourceView** GetTexture(int index);


class Texture {
private:
	bool m_unloadSign;
	std::map<E_Texture, unsigned int> m_textureDic;
public:
	Texture() {
		m_textureDic.clear();
	}
	~Texture() {
		if (m_unloadSign == true) {
			//map<E_Texture, unsigned int>::iterator iter;
			//for (iter = m_textureDic.begin(); iter != m_textureDic.end(); iter++) {
			//	UnloadTexture()
			//}
			UninitTexture();
		}
	}

	unsigned int LoadTextureS(E_Texture _textureType, const char* _fileName, E_TextureType _formatType = E_TextureType::RGB);

	void UnloadTexture(E_Texture _textureType) {
		//TODO
	}

	std::map<E_Texture, unsigned int>::iterator GetTexture(int _textureIndex) {
		return GetTexture((E_Texture)_textureIndex);
	}

	std::map<E_Texture, unsigned int>::iterator GetTexture(E_Texture _textureType) {
		return m_textureDic.find(_textureType);
	}

	bool SetUnloadSign(int _sign) {
		m_unloadSign = _sign;
	}

};