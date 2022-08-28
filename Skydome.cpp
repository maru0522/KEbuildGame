#include "SkyDome.h"
#include <cassert>

void Skydome::Initialize(Model* model)
{
	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	worldTransform_.Initialize();

	worldTransform_.scale_ = { 2.0f, 2.0f, 2.0f };
}

void Skydome::Update()
{
}

void Skydome::Draw(ViewProjection viewProjection_)
{
    // 3Dモデル描画
    model_->Draw(worldTransform_, viewProjection_);
}
