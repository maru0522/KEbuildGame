#include "Block.h"

void Block::Initilize(Model* blockModel, uint32_t blockTexture, uint32_t indexBlock)
{
    // NULLポインタチェック
    assert(blockModel);

    // 引数情報の受け渡し
    model_ = blockModel;
    texture_ = blockTexture; 
    indexBlock_ = indexBlock;

    worldTransform_.Initialize();
    worldTransform_.UpdateMatrix();

    // シングルトンインスタンスを取得する
    input_ = Input::GetInstance();
    debugText_ = DebugText::GetInstance();
}

void Block::Update()
{
#ifdef _DEBUG
    // デバッグテキスト
    //debugText_->SetPos(800, 200);
    //debugText_->Printf("block:(%f,%f,%f)",
    //                   testWT_.translation_.x,
    //                   testWT_.translation_.y,
    //                   testWT_.translation_.z);
#endif
}

void Block::Draw(ViewProjection viewProjection)
{
    model_->Draw(worldTransform_, viewProjection, texture_);
}
