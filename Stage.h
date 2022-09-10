#pragma once
#include "Model.h"
#include "input.h"
#include "DebugText.h"
#include "Block.h"

class Stage
{
public: // �ÓI�����o�ϐ�
    static const int squareLengthX_ = 100;
    static const int squareLengthY_ = 100;
    static const int squareLengthZ_ = 100;

public: // �����o�ϐ�
    std::array<std::array<std::array<bool, squareLengthX_>, squareLengthY_>, squareLengthZ_> isFillCubes_{};

    // ���f���ǂݍ��݂��ꂽ�u���b�N�̈�ӂ̒���
    const float blockSideLength_ = 2.0f;

    // �`�悷��u���b�N���
    std::vector<Block> blocks_{};

public: // �����o�֐�
    void LoadStage(std::string path);

    void InitStage(Model* blockModel, uint32_t blockTexture);

    void UpdateStage();

    void DrawStage(ViewProjection viewProjection);

private:
    

    // �u���b�N���f���@*�f�t�H���g
    Model* blockModel_ = nullptr;

    // �e�N�X�`���n���h��
    uint32_t blockTexture_ = 0u;

    Input* input_ = nullptr;
    DebugText* debugText_ = nullptr;
};

Stage* GetInstanceStage();