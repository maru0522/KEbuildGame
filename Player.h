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

enum Mode {
	Learn,		// 0
	Repeat		// 1
};

class Player
{
public:
	// ������
	void Initialize(Model* model, uint32_t textureHandle, uint32_t textureHandle2);
	// �ړ�
	void Move();
	// isRepeat��Repeat(true)�̎��p�̈ړ��֐�
	void RepeatMove();
	// �ǂ̖ʂ̏�ɗ����Ă��邩���f
	void OnFace();
	// �ǂ̕����B
	void OnDirection();
	// ���̎�ނ̃u���b�N�̏�ɗ����Ă��邩
	void OnBlock();

	// ����]
	/*void Rotate();*/
	// �U��
	/*void Attack();*/

	// �X�V
	void Update();
	// �`��
	void Draw(ViewProjection viewProjection);

	// �v���C���[�̕ϐ�������
	void Reset();

	// �Փ˂����o������Ăт������R�[���o�b�N�֐�
	//void OnCollision();

	// ���[���h���W���擾
	Vector3 GetWorldPosition();

	uint32_t GetUnderBlockId() { return underfootBlockId_; }

	WorldTransform& GetWorldTransform() { return worldTransform_; }
	WorldTransform* GetWorldTransform2() { return &worldTransform_; }

	bool& GetIsRepeat() { return isRepeat_; }

	void SetParent(WorldTransform* worldTransform) { worldTransform_.parent_ = worldTransform; }
	void SetPos(Vector3 pos) { worldTransform_.translation_ = pos; }
	void SetIsRepeat(bool isRepeat) { isRepeat_ = isRepeat; }
	void SetActionRemain(uint32_t index) { indexMoveActionRemain_ = index; }
private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	Vector3 nCubePos_ = { 0,0,0 };

	uint32_t face_ = 0;

	uint32_t worldRot_ = 0;

	std::array<Vector3, 7> floorDefRot_;

	uint32_t underfootBlockId_ = MEMORY;

	uint32_t indexMoveActionRemain_ = 0;
	bool isRepeat_ = Learn;
	std::vector<char> keyMemory_;
	uint32_t indexVecRef_ = 0;
	uint32_t fpsCount_ = 0;

	uint32_t speedRepeat_ = 70;

	// ���f��
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	uint32_t textureHandle2_ = 0u;

	Stage* stage_ = nullptr;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
};