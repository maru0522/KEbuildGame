#include "FPSCamera.h"
#include "Calc.h"

void FPSCamera::Initialize(Vector3 pos, Vector3 rad)
{
    // ワールドトランスフォームの初期設定
    worldTransform_.translation_ = pos;
    worldTransform_.rotation_ = rad;

    //worldTransform_.rotation_.x = Calc::ConvertToRadian(10);

    worldTransform_.Initialize();
    //worldTransform_.rotation_ = { Calc::ConvertToRadian(10),Calc::ConvertToRadian(0),Calc::ConvertToRadian(0) };

    // ビュープロジェクションの初期化
    viewProjection_.Initialize();

    input_ = Input::GetInstance();
    debugText_ = DebugText::GetInstance();

    worldTransform_.UpdateMatrix();
}

void FPSCamera::Update(WorldTransform& WT)
{
#pragma region worldTransform
    // ワールドトランスフォームの座標の数値を加算（移動）
    worldTransform_.translation_ = WT.translation_;
    // ワールドトランスフォームの角度の数値を加算（回転）
    worldTransform_.rotation_ = WT.rotation_;
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
    debugText_->SetPos(780, 10);
    debugText_->Printf("fpseye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

    debugText_->SetPos(710, 30);
    debugText_->Printf("fpsvwCameraPos:(%f,%f,%f)", viewProjection_.constMap->cameraPos.x, viewProjection_.constMap->cameraPos.y, viewProjection_.constMap->cameraPos.z);

    debugText_->SetPos(720, 50);
    debugText_->Printf("fpscameraPos:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

    debugText_->SetPos(720, 70);
    debugText_->Printf("fpscameraRot:(%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z);
#endif
}

Vector3 FPSCamera::GetWorldPosition()
{
    // ワールド座標を入れる変数
    Vector3 worldPos;
    // ワールド行列の平行移動成分を取得（ワールド座標）
    worldPos.x = worldTransform_.matWorld_.m[3][0];
    worldPos.y = worldTransform_.matWorld_.m[3][1];
    worldPos.z = worldTransform_.matWorld_.m[3][2];

    return worldPos;
}

void FPSCamera::SetWorldPosition(Vector3& pos)
{
    worldTransform_.translation_ = pos;
}

void FPSCamera::Move()
{
    // 移動ベクトル
    Vector3 move = { 0,0,0 };

    // 移動速さ
    const float kCharacterSpeed = 0.2f;

    // 押した方向で移動ベクトルを変更
    if (input_->PushKey(DIK_UPARROW)) {
        move = { 0,0,kCharacterSpeed };
    }
    if (input_->PushKey(DIK_DOWNARROW)) {
        move = { 0,0,-kCharacterSpeed };
    }
    if (input_->PushKey(DIK_LEFTARROW)) {
        move = { -kCharacterSpeed,0,0 };
    }
    if (input_->PushKey(DIK_RIGHTARROW)) {
        move = { kCharacterSpeed,0,0 };
    }

    //// 押した方向で移動ベクトルを変更
    //if (input_->TriggerKey(DIK_W)) {
    //    move = { 0,0,kCharacterSpeed };
    //}
    //if (input_->TriggerKey(DIK_S)) {
    //    move = { 0,0,-kCharacterSpeed };
    //}
    //if (input_->TriggerKey(DIK_A)) {
    //    move = { -kCharacterSpeed,0,0 };
    //}
    //if (input_->TriggerKey(DIK_D)) {
    //    move = { kCharacterSpeed,0,0 };
    //}
#ifdef _DEBUG
    if (input_->PushKey(DIK_NUMPAD8)) {
        move = { 0,kCharacterSpeed,0 };
    }
    if (input_->PushKey(DIK_NUMPAD2)) {
        move = { 0,-kCharacterSpeed,0 };
    }

    /*if (input_->TriggerKey(DIK_SPACE)) {
        move = { 0,kCharacterSpeed,0 };
    }
    if (input_->TriggerKey(DIK_LSHIFT)) {
        move = { 0,-kCharacterSpeed,0 };
    }*/

    if (input_->TriggerKey(DIK_NUMPAD6)) {
        worldTransform_.rotation_.y += Calc::ConvertToRadian(45);
    }
    if (input_->TriggerKey(DIK_NUMPAD4)) {
        worldTransform_.rotation_.y -= Calc::ConvertToRadian(45);
    }

    if (input_->TriggerKey(DIK_NUMPAD9)) {
        worldTransform_.rotation_.x += Calc::ConvertToRadian(45);
    }
    if (input_->TriggerKey(DIK_NUMPAD3)) {
        worldTransform_.rotation_.x -= Calc::ConvertToRadian(45);
    }
#endif

    worldTransform_.translation_.x += move.x;
    worldTransform_.translation_.y += move.y;
    worldTransform_.translation_.z += move.z;
}
