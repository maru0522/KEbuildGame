#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <cassert>
#include "input.h"
#include "DebugText.h"
#include "EnemyBullet.h"

// 自機クラスの前方宣言
class Player;

enum class Phase {
	Approach,	// 接近する
	Leave,		// 離脱する
};

class Enemy
{
private:
	// 発射間隔
	static const int kFireInterval = 60;

	// 発射タイマ
	int32_t fireTimer = 0;

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	DebugText* debugText_ = nullptr;

	// フェーズ
	Phase phase_ = Phase::Approach;

	// 弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	// 自キャラ
	Player* player_ = nullptr;

	// デスフラグ
	bool isDead_ = false;

public:
	void Initialize(Model* model, uint32_t textureHandle, Vector3& pos);
	void Fire();
	void PhaseInitApproach();
	void PhaseApproach();
	void PhaseLeave();
	void Update();
	void Draw(ViewProjection viewProjection);
	void SetPlayer(Player* player) { player_ = player; }

	bool isDead() const { return isDead_; }

	// 衝突を検出したら呼びだされるコールバック関数
	void OnCollision();

	// 弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	void SetParent(WorldTransform* worldTransform) { worldTransform_.parent_ = worldTransform; }
};