#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"

class Skydome
{
public:
    void Initialize(Model* model);
    void Update();
    void Draw(ViewProjection viewProjection_);

    void SetParent(WorldTransform* worldTransform) { worldTransform_.parent_ = worldTransform; }

private:
    // ���[���h�ϊ��f�[�^
    WorldTransform worldTransform_;
    // ���f��
    Model* model_ = nullptr;

    Input* input_ = nullptr;
    DebugText* debugText_ = nullptr;
};

