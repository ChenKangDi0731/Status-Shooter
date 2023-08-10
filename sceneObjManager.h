//===========================================================================
// [ゲーム内すべてのオブジェクトを管理するマネージャ]
// ‐当たり判定処理もここで行われます
// ‐弾の管理もここで行われます
//===========================================================================
#ifndef _SCENEOBJMANAGER_H_
#define _SCENEOBJMANAGER_H_
#include <map>
#include <list>
#include "gameObject.h"

class BaseCell;
class Player;
class Collider;
class Bullet;

class SceneObjMgr {
private:
	bool initOnce;

	int cellIdCounter;//オブジェクトID

	std::map<int, std::map<int, BaseCell*>> drawCellDic;//オブジェクト描画リスト

	std::map<int, BaseCell*> cellDic;//オブジェクトリスト

	std::map<int, Light*> lightDic;

	std::map<E_CellType, std::map<int, BaseCell*>> cellTypeDic;//オブジェクトリスト（種類で分けて保存する

	Player* m_player;//プレイヤー

	//=============================弾のパラメータ
	//弾の描画データのテンプレート
	Collider* tempCollider;
	BaseShaderParam* normalBullet_ShaderParam;
	BaseShaderParam* laserBullet_ShaderParam;
	BaseShaderParam* cannonBullet_ShaderParam;
	BaseShaderParam* enemyBullet_normal_ShaderParam;
	BaseShaderParam* enemyBullet_normal_reflected_ShaderParam;

	std::list<Bullet*> bulletPools;
	//============================弾のパラメータ
public:

	float resetDelay = 3.0f;
	float resetDelayTimePass = 0.0f;
	bool resetSign = false;

	//light
	int light_1_id;
	int light_2_id;
	int light_3_id;

public:
	SceneObjMgr();
	~SceneObjMgr();
	void DoInit();
	void UnInit(bool deleteCell);

	void DoInitCell();//オブジェクトの初期化

	void DoUpdate(float deltatime);//すべてのゲームオブジェクトの更新処理
	void DoLateUpdate(float deltatime);
	void DoDraw();//すべてのゲームオブジェクトの描画処理

	void RegisterCell(int id, BaseCell* cell);
	void RegisterCell(BaseCell* cell);
	void UnRegisterCell(int id);
	void UnRegisterCell(BaseCell* cell);

	BaseCell* GetCell(int id);
	std::list<BaseCell*> GetCellsByRange(E_CellType cellType, vector3s pos, float range, bool includeInactive);
	void ShowByCellType(E_CellType cellType, bool show);

	void ChangeCellLayer(BaseCell* cell, E_DrawLayer layer);

	/// <summary>
	/// プレイヤー当たり判定
	/// </summary>
	/// <param name="player"></param>
	void CheckPlayerCollision(Player* player);

	/// <summary>
	/// プレイヤーを取得
	/// </summary>
	/// <returns></returns>
	Player* GetPlayer();

	Light* GetLightWithIndex(int index);
	Light* GetLight(int lightID);

	//オブジェクトID（すべてのオブジェクトのIDは唯一無二である
	int GetID() {
		return cellIdCounter++;
	}

	Collider* GetTempCollider();

	//プレイヤーに関する関数
	void InitPlayerTitleState();
	void InitPlayerObjState();
	void UninitPlayerObjState();

	//=============================================弾に関する函数
	Bullet* CreateBulletEntity(E_BulletType bulletType);//弾を生成する
	Bullet* GetBullet(E_BulletType bulletType);//弾を獲得
	void ShootBullet(BaseCell* attacker, int atk, E_BulletType bulletType, vector3s startPoint, vector3s dir, float speed);
	void RecycleBullet(Bullet* bullet);//弾を回収
	bool CheckBulletCollision(Bullet* bullet);//弾の当たり判定
};


#endif
