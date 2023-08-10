//===========================================================================
// [�Q�[�������ׂẴI�u�W�F�N�g���Ǘ�����}�l�[�W��]
// �]�����蔻�菈���������ōs���܂�
// �]�e�̊Ǘ��������ōs���܂�
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

	int cellIdCounter;//�I�u�W�F�N�gID

	std::map<int, std::map<int, BaseCell*>> drawCellDic;//�I�u�W�F�N�g�`�惊�X�g

	std::map<int, BaseCell*> cellDic;//�I�u�W�F�N�g���X�g

	std::map<int, Light*> lightDic;

	std::map<E_CellType, std::map<int, BaseCell*>> cellTypeDic;//�I�u�W�F�N�g���X�g�i��ނŕ����ĕۑ�����

	Player* m_player;//�v���C���[

	//=============================�e�̃p�����[�^
	//�e�̕`��f�[�^�̃e���v���[�g
	Collider* tempCollider;
	BaseShaderParam* normalBullet_ShaderParam;
	BaseShaderParam* laserBullet_ShaderParam;
	BaseShaderParam* cannonBullet_ShaderParam;
	BaseShaderParam* enemyBullet_normal_ShaderParam;
	BaseShaderParam* enemyBullet_normal_reflected_ShaderParam;

	std::list<Bullet*> bulletPools;
	//============================�e�̃p�����[�^
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

	void DoInitCell();//�I�u�W�F�N�g�̏�����

	void DoUpdate(float deltatime);//���ׂẴQ�[���I�u�W�F�N�g�̍X�V����
	void DoLateUpdate(float deltatime);
	void DoDraw();//���ׂẴQ�[���I�u�W�F�N�g�̕`�揈��

	void RegisterCell(int id, BaseCell* cell);
	void RegisterCell(BaseCell* cell);
	void UnRegisterCell(int id);
	void UnRegisterCell(BaseCell* cell);

	BaseCell* GetCell(int id);
	std::list<BaseCell*> GetCellsByRange(E_CellType cellType, vector3s pos, float range, bool includeInactive);
	void ShowByCellType(E_CellType cellType, bool show);

	void ChangeCellLayer(BaseCell* cell, E_DrawLayer layer);

	/// <summary>
	/// �v���C���[�����蔻��
	/// </summary>
	/// <param name="player"></param>
	void CheckPlayerCollision(Player* player);

	/// <summary>
	/// �v���C���[���擾
	/// </summary>
	/// <returns></returns>
	Player* GetPlayer();

	Light* GetLightWithIndex(int index);
	Light* GetLight(int lightID);

	//�I�u�W�F�N�gID�i���ׂẴI�u�W�F�N�g��ID�͗B�ꖳ��ł���
	int GetID() {
		return cellIdCounter++;
	}

	Collider* GetTempCollider();

	//�v���C���[�Ɋւ���֐�
	void InitPlayerTitleState();
	void InitPlayerObjState();
	void UninitPlayerObjState();

	//=============================================�e�Ɋւ��锟��
	Bullet* CreateBulletEntity(E_BulletType bulletType);//�e�𐶐�����
	Bullet* GetBullet(E_BulletType bulletType);//�e���l��
	void ShootBullet(BaseCell* attacker, int atk, E_BulletType bulletType, vector3s startPoint, vector3s dir, float speed);
	void RecycleBullet(Bullet* bullet);//�e�����
	bool CheckBulletCollision(Bullet* bullet);//�e�̓����蔻��
};


#endif
