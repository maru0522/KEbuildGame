#pragma once
#include "Model.h"
#include "WorldTransform.h"

class PlayerBullet {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	bool isDead() const { return isDead_; }

	// 衝突を検出したら呼びだされるコールバック関数
	void OnCollision();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;

	// 速度
	Vector3 velocity_;

	// 寿命 <frame>
	static const int32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
};