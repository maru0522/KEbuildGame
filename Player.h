#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <cassert>
#include "input.h"
#include "DebugText.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>

class Player
{
public:
	// ������
	void Initialize(Model* model, uint32_t textureHandle);
	// �ړ�
	void Move();

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
	// ���f��
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

};