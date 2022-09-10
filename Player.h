#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <cassert>
#include "input.h"
#include "DebugText.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>
#include "Stage.h"

enum Floor {
	Top = 1,	// 1
	Bottom,		// 2
	Right,		// 3
	Left,		// 4
	Front,		// 5
	Back		// 6
};

enum Direction {
	North = 1,	// 1
	East,		// 2
	South,		// 3
	West		// 4
};

class Player
{
public:
	// ������
	void Initialize(Model* model, uint32_t textureHandle);
	// �ړ�
	void Move();
	// �ǂ̖ʂ̏�ɗ����Ă��邩���f
	void OnFace();
	// �ǂ̕����B
	void OnDirection();

	// ����]
	/*void Rotate();*/
	// �U��
	/*void Attack();*/

	// �X�V
	void Update();
	// �`��
	void Draw(ViewProjection viewProjection);

	// �Փ˂����o������Ăт������R�[���o�b�N�֐�
	//void OnCollision();

	// ���[���h���W���擾
	Vector3 GetWorldPosition();

	void SetParent(WorldTransform* worldTransform) { worldTransform_.parent_ = worldTransform; }
private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	Vector3 nCubePos_ = { 0,0,0 };

	uint32_t face_ = 0;

	uint32_t worldRot_ = 0;

	// ���f��
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Stage* stage_ = nullptr;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

};