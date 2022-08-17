#include "Player.h"
#include "Enemy.h"
#include "Calc.h"
#include "Vector3.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle)
{
    // NULLポインタチェック
    assert(model);

    // 引数として受け取ったデータをメンバ変数に記録する
    model_ = model;
    textureHandle_ = textureHandle;

    // シングルトンインスタンスの取得
    debugText_ = DebugText::GetInstance();

    worldTransform_.translation_.z = 30;

    PhaseInitApproach();

    worldTransform_.Initialize();
}

void Enemy::Fire()
{
    assert(player_);

    // 弾の速度
    const float kBulletSpeed = 0.5f;

    // 自キャラのワールド座標を取得する
    Vector3 pWorldPos = player_->GetWorldPosition();
    // 敵キャラのワールド座標を取得する
    Vector3 eWorldPos = GetWorldPosition();
    // 敵キャラ->自キャラの差分ベクトルを求める
    Vector3 diffWorldPos = pWorldPos - eWorldPos;
    // ベクトルの正規化
    diffWorldPos.normalize();
    // ベクトルの長さを、速さに合わせる
    diffWorldPos *= kBulletSpeed;

    // 弾を発射し、初期化
    std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
    newBullet->Initialize(model_, worldTransform_.translation_, diffWorldPos);

    // 弾を登録する
    bullets_.push_back(std::move(newBullet));
}

void Enemy::PhaseInitApproach()
{
    // 発射タイマを初期化
    fireTimer = kFireInterval;
}

void Enemy::PhaseApproach()
{
    const float kApproachSpeed = 0.0f;
    Vector3 move = { 0,0,0 };

    move = { 0,0,-kApproachSpeed };
    // 移動（ベクトルを加算）
    worldTransform_.translation_ += move;

    // 発射タイマカウントダウン
    fireTimer--;

    // 指定時間に達した
    if (fireTimer <= 0) {
        Fire();
        // 発射タイマの初期化
        PhaseInitApproach();
    }

    // 規定の位置に到達したら離脱
    if (worldTransform_.translation_.z < 0.0f) {
        phase_ = Phase::Leave;
    }
}

void Enemy::PhaseLeave()
{
    const float kApproachSpeed = 0.2f;

    Vector3 move = { -kApproachSpeed,kApproachSpeed,0 };
    // 移動（ベクトルを加算）
    worldTransform_.translation_ += move;
}


void Enemy::Update()
{
    // デスフラグの立った弾を削除
    bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bulet) {
        return bulet->isDead();
                       });

    // デバッグテキスト
    debugText_->SetPos(50, 70);
    debugText_->Printf("Enemy:(%f,%f,%f)",
                       worldTransform_.translation_.x,
                       worldTransform_.translation_.y,
                       worldTransform_.translation_.z);

    switch (phase_) {
        case Phase::Approach:
        default:
            PhaseApproach();
            break;
        case Phase::Leave:
            PhaseLeave();
            break;
    }

    // 弾更新
    for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
        bullet->Update();
    }

    //ワールド行列更新
    worldTransform_.UpdateMatrix();
}


void Enemy::Draw(ViewProjection viewProjection)
{
    model_->Draw(worldTransform_, viewProjection, textureHandle_);
    for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
        bullet->Draw(viewProjection);
    }
}

void Enemy::OnCollision()
{
}

Vector3 Enemy::GetWorldPosition()
{
    // ワールド座標を入れる変数
    Vector3 worldPos;
    // ワールド行列の平行移動成分を取得（ワールド座標）
    worldPos.x = worldTransform_.translation_.x;
    worldPos.y = worldTransform_.translation_.y;
    worldPos.z = worldTransform_.translation_.z;

    return worldPos;
}