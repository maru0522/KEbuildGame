#include "RailCamera.h"
#include "Calc.h"


void RailCamera::Initialize(Vector3 pos, Vector3 rad)
{
    // ワールドトランスフォームの初期設定
    worldTransform_.translation_ = pos;
    worldTransform_.rotation_ = rad;
   
    worldTransform_.Initialize();

    // ビュープロジェクションの初期化
    viewProjection_.Initialize();

    debugText_ = DebugText::GetInstance();
}

void RailCamera::Update()
{
#pragma region worldTransform
    // ワールドトランスフォームの座標の数値を加算（移動）
    //worldTransform_.translation_.z -= 0.01;

    // ワールドトランスフォームの角度の数値を加算（回転）

    // ワールドトランスフォームのワールド行列再計算
    worldTransform_.UpdateMatrix();
#pragma endregion

#pragma region viewProjection
    viewProjection_.eye = worldTransform_.translation_;

    // ワールド前方ベクトル
    Vector3 forward(0, 0, 1);

    // レールカメラの回転を反映
    forward = Calc::DotVecMat(forward, worldTransform_.matWorld_);

    // 視点から前方に適当な距離進んだ位置が注意点
    viewProjection_.target = viewProjection_.eye + forward;

    // ワールド上方ベクトル
    Vector3 up(0, 1, 0);

    // レールカメラの回転を反映（レールカメラの上方ベクトル）
    viewProjection_.up = Calc::DotVecMat(up, worldTransform_.matWorld_);

    //worldTransform_.UpdateMatrix();
    // ビュープロジェクションを更新
    viewProjection_.TransferMatrix();
    viewProjection_.UpdateMatrix();
#pragma endregion

#ifdef _DEBUG
    // デバッグテキスト
    debugText_->SetPos(80, 10);
    debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

    debugText_->SetPos(10, 30);
    debugText_->Printf("vwCameraPos:(%f,%f,%f)", viewProjection_.constMap->cameraPos.x, viewProjection_.constMap->cameraPos.y, viewProjection_.constMap->cameraPos.z);

    debugText_->SetPos(20, 50);
    debugText_->Printf("cameraPos:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
#endif
}

Vector3 RailCamera::GetWorldPosition()
{
    // ワールド座標を入れる変数
    Vector3 worldPos;
    // ワールド行列の平行移動成分を取得（ワールド座標）
    worldPos.x = worldTransform_.matWorld_.m[3][0];
    worldPos.y = worldTransform_.matWorld_.m[3][1];
    worldPos.z = worldTransform_.matWorld_.m[3][2];

    return worldPos;
}
