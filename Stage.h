#pragma once
#include "Model.h"

class Stage
{
public:
    void InitStage1(Model* blockModel, uint32_t blockTexture);

    void UpdateStage1();

    void DrawStage1(ViewProjection viewProjection);

private:
    // �e�N�X�`���n���h��
    uint32_t blockTexture_ = 0u;

    // �u���b�N���f���@*�f�t�H���g
    Model* blockModel_ = nullptr;

    static const uint32_t indexBlock_ = 100;

    // �u���b�N�̈ʒu���i�[�����z��
    std::array<WorldTransform, indexBlock_> blockData_;
};