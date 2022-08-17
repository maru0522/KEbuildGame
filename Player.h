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
	void Rotate();
	// 攻撃
	void Attack();
	// 更新
	void Update();
	// 描画
	void Draw(ViewProjection viewProjection);

	// 衝突を検出したら呼びだされるコールバック関数
	void OnCollision();

	// 弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	// 弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
};