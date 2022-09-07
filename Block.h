#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "input.h"
#include "DebugText.h"
#include <cassert>
#include "TextureManager.h"

class Block
{
public:
    void Initilize(Model* blockModel, uint32_t blockTexture, uint32_t indexBlock);
    void Update();
    void Draw(ViewProjection viewProjection);

    WorldTransform* GetWorldTransform() { return &worldTransform_; }
    WorldTransform SetWorldTransform(WorldTransform worldTransform) { worldTransform_ = worldTransform; }

private:
    WorldTransform worldTransform_;

    // �u���b�N���f���@*�f�t�H���g
    Model* model_ = nullptr;

    // �e�N�X�`���n���h��
    uint32_t texture_ = 0u;

    // �u���b�NID
    uint32_t indexBlock_ = 0;

    Input* input_ = nullptr;
    DebugText* debugText_ = nullptr;
};

