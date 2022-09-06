#include "Stage.h"
#include <cassert>

void Stage::InitStage1(Model* blockModel, uint32_t blockTexture)
{
    // NULL�|�C���^�`�F�b�N
    assert(blockModel);

    // �������̎󂯓n��
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
