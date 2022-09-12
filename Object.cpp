#include "Object.h"

void Object::Initialize(Model* model, Vector3 pos, Vector3 rot)
{
    // NULLポインタチェック
    assert(model);

    // 引数として受け取ったデータをメンバ変数に記録する
    model_ = model;

    // シングルトンインスタンスの取得
    debugText_ = DebugText::GetInstance();

    worldTransform_.translation_ = pos;
    worldTransform_.rotation_ = rot;

    worldTransform_.Initialize();
    worldTransform_.UpdateMatrix();
}

void Object::Initialize(Model* model, uint32_t textureHandle, Vector3 pos, Vector3 rot)
{
    // NULLポインタチェック
    assert(model);

    // 引数として受け取ったデータをメンバ変数に記録する
    model_ = model;
    textureHandle_ = textureHandle;

    // シングルトンインスタンスの取得
    debugText_ = DebugText::GetInstance();

    worldTransform_.translation_ = pos;
    worldTransform_.rotation_ = rot;

    worldTransform_.Initialize();
    worldTransform_.UpdateMatrix();
}

void Object::Update()
{
#ifdef _DEBUG
    // デバッグテキスト
    debugText_->SetPos(50, 400);
    debugText_->Printf("objPos:(%f,%f,%f)",
                       worldTransform_.translation_.x,
                       worldTransform_.translation_.y,
                       worldTransform_.translation_.z);
#endif // _DEBUG

    if (isRolling_) {
        if (isLeftRoll_) {
            worldTransform_.rotation_.x -= Calc::ConvertToRadian(1);
        }
        else {
            worldTransform_.rotation_.x += Calc::ConvertToRadian(1);
        }
    }

    //ワールド行列更新
    worldTransform_.UpdateMatrix();
}

void Object::Draw(ViewProjection viewProjection, bool isTexture)
{
    if (isTexture) {
        model_->Draw(worldTransform_, viewProjection);
    }
    else {
        model_->Draw(worldTransform_, viewProjection, textureHandle_);

    }
}
