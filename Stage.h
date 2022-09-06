#pragma once
#include "Model.h"

class Stage
{
public:
    void InitStage1(Model* blockModel, uint32_t blockTexture);

    void UpdateStage1();

    void DrawStage1(ViewProjection viewProjection);

private:
    // テクスチャハンドル
    uint32_t blockTexture_ = 0u;

    // ブロックモデル　*デフォルト
    Model* blockModel_ = nullptr;

    static const uint32_t indexBlock_ = 100;

    // ブロックの位置を格納した配列
    std::array<WorldTransform, indexBlock_> blockData_;
};