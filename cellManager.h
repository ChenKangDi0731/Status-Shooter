//===========================================================================
// [�V�[���I�u�W�F�N�g�}�l�[�W��]
// 1�D�G�L�����N�^�[
// 2�D�w�i�i���Ƃ��j
//===========================================================================

#ifndef _CELLMAMAGER_H_
#define _CELLMANAGER_H_

class CellMgr {
private:
	std::map<E_CellType, std::list<BaseCell*>> cellDic;

private:
	//�`��f�[�^�̃e���v���[�g
	BaseShaderParam* enemy_1_shaderParam;
	BaseShaderParam* enemy_2_shaderParam;
	BaseShaderParam* enemy_3_shaderParam;

	BaseShaderParam* grass_shaderParam;
public:
	CellMgr();
	~CellMgr();

	void DoInit();

	BaseCell* CreateCellEntity(E_CellType cellType);
	BaseCell* GetCell(E_CellType cellType);

	void RegisterCell(BaseCell* cell);
};


#endif