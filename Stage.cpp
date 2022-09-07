#include "Stage.h"
#include <cassert>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "GameScene.h"

void Stage::LoadStage(std::string pathCSV)
{
    std::ifstream ifs(pathCSV);

    std::string line, tmp;
    std::array<int, 3> coordinate;
    while (std::getline(ifs, line)) {
        std::istringstream line_stream(line);
        for (int i = 0; i < 3; i++) {
            std::getline(line_stream, tmp, ',');
            coordinate[i] = std::stoi(tmp);
        }
        isFillCubes_[coordinate[2]][coordinate[1]][coordinate[0]] = true;
        WorldTransform tmpWT_;
        tmpWT_.translation_ = {
            coordinate[0] * blockSideLength_,
            coordinate[1] * blockSideLength_,
            coordinate[2] * blockSideLength_
        };
        Block tmpBlock_;
        tmpBlock_.SetWorldTransform(tmpWT_);
        //tmpBlock_.GetWorldTransform()->Initialize();
        //tmpBlock_.GetWorldTransform()->UpdateMatrix();
        tmpBlock_.Initilize(blockModel_, blockTexture_, index);
        blocks_.push_back(tmpBlock_);
        // TODO:csv�̋L���ɂ���ăe�N�X�`���ύX�B
    }
}

void Stage::InitStage(Model* blockModel, uint32_t blockTexture)
{
    // NULL�|�C���^�`�F�b�N
    assert(blockModel);

    // �������̎󂯓n��
    blockModel_ = blockModel;
    blockTexture_ = blockTexture;

    // �V���O���g���C���X�^���X���擾����
    input_ = Input::GetInstance();
    debugText_ = DebugText::GetInstance();
}

void Stage::UpdateStage()
{
#ifdef _DEBUG
    // �f�o�b�O�e�L�X�g
    //debugText_->SetPos(800, 200);
    //debugText_->Printf("block:(%f,%f,%f)",
    //                   testWT_.translation_.x,
    //                   testWT_.translation_.y,
    //                   testWT_.translation_.z);
#endif
}

void Stage::DrawStage(ViewProjection viewProjection)
{
    for (Block& block : blocks_) {
        block.Draw(viewProjection);
    }
}

Stage stage_;
Stage* GetInstanceStage()
{
    return &stage_;
}
