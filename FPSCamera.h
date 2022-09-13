#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"
#include "DebugText.h"

class FPSCamera
{
public:
    void Initialize(Vector3 pos, Vector3 rad);
    void Update(WorldTransform& WT);

    ViewProjection GetViewProjection() { return viewProjection_; }
    WorldTransform* GetWorldTransform() { return &worldTransform_; }

    // ワールド座標を取得
    Vector3 GetWorldPosition();
    void SetWorldPosition(Vector3& pos);
    void SetParent(WorldTransform* worldTransform) { worldTransform_.parent_ = worldTransform; }
private:
    void Move();

private:
    // ワールド変換データ
    WorldTransform worldTransform_;
    // ビュープロジェクション
    ViewProjection viewProjection_;

    Input* input_ = nullptr;
    DebugText* debugText_ = nullptr;
};