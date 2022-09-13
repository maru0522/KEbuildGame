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
	// 初期化
	void Initialize(Model* model, uint32_t textureHandle, uint32_t textureHandle2);
	// 移動
	void Move();
	// isRepeatがRepeat(true)の時用の移動関数
	void RepeatMove();
	// どの面の上に立っているか判断
	void OnFace();
	// どの方向。
	void OnDirection();
	// 何の種類のブロックの上に立っているか
	void OnBlock();

	// 軸回転
	/*void Rotate();*/
	// 攻撃
	/*void Attack();*/

	// 更新
	void Update();
	// 描画
	void Draw(ViewProjection viewProjection);

	// プレイヤーの変数初期化
	void Reset();

	// 衝突を検出したら呼びだされるコールバック関数
	//void OnCollision();

	// ワールド座標を取得
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
	// ワールド変換データ
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

	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	uint32_t textureHandle2_ = 0u;

	Stage* stage_ = nullptr;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
};