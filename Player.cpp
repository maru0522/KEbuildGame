#include "Player.h"
#include "Calc.h"

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.translation_ = { 0,0,20 };

	worldTransform_.Initialize();
}

void Player::Move()
{
	// キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_W)) {
		move = { 0,kCharacterSpeed,0 };
	}
	if (input_->PushKey(DIK_S)) {
		move = { 0,-kCharacterSpeed,0 };
	}
	if (input_->PushKey(DIK_A)) {
		move = { -kCharacterSpeed,0,0 };
	}
	if (input_->PushKey(DIK_D)) {
		move = { kCharacterSpeed,0,0 };
	}

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	// 移動限界座標
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
}

//void Player::Rotate()
//{
//	if (input_->PushKey(DIK_U)) {
//		worldTransform_.rotation_.y -= 0.05f;
//	}
//	if (input_->PushKey(DIK_I)) {
//		worldTransform_.rotation_.y += 0.05f;
//	}
//
//}
//
//void Player::Attack()
//{
//	if (input_->PushKey(DIK_SPACE)) {
//		// 弾の速度
//		const float kBulletSpeed = 1.0f;
//		Vector3 velocity(0, 0, kBulletSpeed);
//
//		// 速度ベクトルを自機の向きに合わせて回転させる
//		velocity = Calc::DotVecMat(velocity, worldTransform_.matWorld_);
//
//		// 弾を発射し、初期化
//		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
//		newBullet->Initialize(model_, GetWorldPosition() , velocity);
//
//		// 弾を登録する
//		bullets_.push_back(std::move(newBullet));
//	}
//}

void Player::Update()
{
#ifdef _DEBUG
	// デバッグテキスト
	debugText_->SetPos(50, 50);
	debugText_->Printf("player:(%f,%f,%f)",
					   worldTransform_.translation_.x,
					   worldTransform_.translation_.y,
					   worldTransform_.translation_.z);
#endif

	// 移動
	Move();

	//ワールド行列更新
	worldTransform_.UpdateMatrix();
}

void Player::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

//void Player::OnCollision()
//{
//}

Vector3 Player::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}