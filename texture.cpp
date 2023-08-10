#include "main.h"
#include "Texture.h"


//マクロ定義
#define MAX_TEXTURE_NUM 100	//ロード限界数

//グローバル変数
//テクスチャ情報のポインタの配列
static ID3D11ShaderResourceView* g_pTexture[MAX_TEXTURE_NUM];

//ロード済みのテクスチャの数
static UINT g_TextureCount;

//テクスチャファイル名のバッファ ファイル名255文字まで
static char g_TextureName[MAX_TEXTURE_NUM][256];

//指定のテクスチャ情報を取得
//IN テクスチャハンドル
//OUT テクスチャ情報のポインタの入ったポインタ変数のポインタ
ID3D11ShaderResourceView** GetTexture(int index) {
	//エラーチェック
	if (index < 0) {
		return nullptr;//0未満
	}

	if (index >= (int)g_TextureCount) {
		return nullptr;//ロード数オーバー
	}


	//インデックスが示す場所の配列のポインタを返す
	return &g_pTexture[index];


}

//テクスチャの終了処理
//ロード済みのテクスチャをすべて解放
void UninitTexture() {

	for (int index = 0; index < (int)g_TextureCount; index++) {
		if (g_pTexture[index] != nullptr) {//何かデータが入っていたら
			g_pTexture[index]->Release();//解放する
			g_pTexture[index] = nullptr;//初期化しておく
		}
	}

}

int LoadTexture(const char* filename) {
	//すでに読み込まれたテクスチャ名を調べて同名のテクスチャ名が存在した場合は、テクスチャ名の
	//格納された配列の番号を返す
	for (int index = 0; index < (int)g_TextureCount; index++) {
		//テクスチャ名を比較
		if (strcmp(g_TextureName[index], filename) == 0) {
			// if(strcmp(&g_TextureName[index][0] , filename) ==0)
			return index;
		}
	}

	//読み込み数が限界を超えていないか
	if (g_TextureCount ==  MAX_TEXTURE_NUM) {
		return -1;//読み込めないのでエラーコードを返す
	}

	//テクスチャのロード
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(
		GetDevice(),
		filename,
		nullptr,
		nullptr,
		&g_pTexture[g_TextureCount],
		nullptr
	);

	if (hr != S_OK) {
		return -1;//読み込み失敗
	}
	//ロードしたテクスチャ名の登録
	strcpy_s(g_TextureName[g_TextureCount],256,filename);


	int result = g_TextureCount;//現在の数を一時退避
	g_TextureCount++;//現在の数を増やしておく

	return result;//退避していた値を返す
}

unsigned int Texture::LoadTextureS(E_Texture _textureType, const char* _fileName, E_TextureType _formatType)
{

	if (_fileName == nullptr)return 0;

	std::map<E_Texture, unsigned int>::iterator iter = GetTexture(_textureType);
	if (iter != m_textureDic.end()) {
		return iter->second;
	}

	unsigned int textureNo = LoadTexture(_fileName);
	if (textureNo == -1) {
		//load failed
		return -1;
	}

	m_textureDic[_textureType] = textureNo;

	return textureNo;

}
