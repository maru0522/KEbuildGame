#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Skydome
{
public:
    void Initialize(Model* model);
    void Update();
    void Draw(ViewProjection viewProjection_);

    void SetParent(WorldTransform* worldTransform) { worldTransform_.parent_ = worldTransform; }

private:
    // ワールド変換データ
    WorldTransform worldTransform_;
    // モデル
    Model* model_ = nullptr;
};

