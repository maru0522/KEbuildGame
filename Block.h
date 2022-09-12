#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "input.h"
#include "DebugText.h"
#include <cassert>
#include "TextureManager.h"

enum blockID {
    FLOOR = 1,
    WoodBox,
    MEMORY,
    GOAL
};

class Block
{
public:
    void Initilize(Model* blockModel, uint32_t indexBlock);
    void Update();
    void Draw(ViewProjection viewProjection);

    WorldTransform* GetWorldTransform() { return &worldTransform_; }
    void SetWorldTransform(WorldTransform worldTransform) { worldTransform_ = worldTransform; }

    uint32_t& GetIndexBlock() { return indexBlock_; }
    void SetIndexBlock(uint32_t id) { indexBlock_ = id; }

private:
    WorldTransform worldTransform_;

    // ブロックモデル　*デフォルト
    Model* model_ = nullptr;

    // テクスチャハンドルの配列
    std::array<uint32_t, 4> textures_ = {};

    // ブロックID
    uint32_t indexBlock_ = 0;

    Input* input_ = nullptr;
    DebugText* debugText_ = nullptr;
};

