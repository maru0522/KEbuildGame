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
	// 初期化
	void Initialize(Model* model, uint32_t textureHandle);
	// 移動
	void Move();

	// 軸回転
	/*void Rotate();*/
	// 攻撃
	/*void Attack();*/

	// 更新
	void Update();
	// 描画
	void Draw(ViewProjection viewProjection);

	// 衝突を検出したら呼びだされるコールバック関数
	//void OnCollision();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	void SetParent(WorldTransform* worldTransform) { worldTransform_.parent_ = worldTransform; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

};