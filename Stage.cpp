#include "Stage.h"
#include <cassert>

void Stage::InitStage1(Model* blockModel, uint32_t blockTexture)
{
    // NULLポインタチェック
    assert(blockModel);

    // 引数情報の受け渡し
    blockModel_ = blockModel;
    blockTexture_ = blockTexture;
}

void Stage::UpdateStage1()
{
}

void Stage::DrawStage1(ViewProjection viewProjection)
{
    for (int i = 0; i < indexBlock_; i++) {
        blockModel_->Draw(blockData_[i], viewProjection);
    }
}
