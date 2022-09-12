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
	// 初期化
	void Initialize(Model* model, uint32_t textureHandle);
	// 移動
	void Move();
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

	void SetParent(WorldTransform* worldTransform) { worldTransform_.parent_ = worldTransform; }
	void SetPos(Vector3 pos) { worldTransform_.translation_ = pos; }
private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	Vector3 nCubePos_ = { 0,0,0 };

	uint32_t face_ = 0;

	uint32_t worldRot_ = 0;

	std::array<Vector3, 7> floorDefRot_;

	uint32_t underfootBlockId_ = 0;

	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Stage* stage_ = nullptr;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
};