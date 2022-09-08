#include "Block.h"
#include <cassert>


void Block::Initilize(Model* blockModel, uint32_t indexBlock)
{
    // NULLポインタチェック
    assert(blockModel);
    assert(indexBlock > 0);


    // 引数情報の受け渡し
    model_ = blockModel;
    indexBlock_ = indexBlock;

    textures_[0] = TextureManager::Load("cube/cube.jpg");
    textures_[1] = TextureManager::Load("Task1_2Resources/bullet.png");
    textures_[2] = TextureManager::Load("Task1_2Resources/enemyBullet.png");

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
    model_->Draw(worldTransform_, viewProjection, textures_[indexBlock_ - 1]);
}
