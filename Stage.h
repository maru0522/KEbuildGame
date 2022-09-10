#pragma once
#include "Model.h"
#include "input.h"
#include "DebugText.h"
#include "Block.h"

class Stage
{
public: // 静的メンバ変数
    static const int squareLengthX_ = 100;
    static const int squareLengthY_ = 100;
    static const int squareLengthZ_ = 100;

public: // メンバ変数
    std::array<std::array<std::array<bool, squareLengthX_>, squareLengthY_>, squareLengthZ_> isFillCubes_{};

    // モデル読み込みされたブロックの一辺の長さ
    const float blockSideLength_ = 2.0f;

    // 描画するブロック情報
    std::vector<Block> blocks_{};

public: // メンバ関数
    void LoadStage(std::string path);

    void InitStage(Model* blockModel, uint32_t blockTexture);

    void UpdateStage();

    void DrawStage(ViewProjection viewProjection);

private:
    

    // ブロックモデル　*デフォルト
    Model* blockModel_ = nullptr;

    // テクスチャハンドル
    uint32_t blockTexture_ = 0u;

    Input* input_ = nullptr;
    DebugText* debugText_ = nullptr;
};

Stage* GetInstanceStage();