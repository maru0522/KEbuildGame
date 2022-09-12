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
    std::array<int, 4> coordinate; // coordinate[3]はindexBlockID
    while (std::getline(ifs, line)) {
        std::istringstream line_stream(line);
        for (int i = 0; i < 4; i++) {
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
        tmpBlock_.Initilize(blockModel_, coordinate[3]);
        blocks_.push_back(tmpBlock_);
    }
}

void Stage::InitStage(Model* blockModel, uint32_t blockTexture)
{
    // NULLポインタチェック
    assert(blockModel);

    // 引数情報の受け渡し
    blockModel_ = blockModel;
    blockTexture_ = blockTexture;

    // シングルトンインスタンスを取得する
    input_ = Input::GetInstance();
    debugText_ = DebugText::GetInstance();
}

void Stage::UpdateStage()
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

void Stage::DrawStage(ViewProjection viewProjection)
{
    for (Block& block : blocks_) {
        block.Draw(viewProjection);
    }
}

void Stage::Reset()
{
    // isFillCubes_の初期化
    for (int z = 0; z < squareLengthZ_; z++) {
        for (int y = 0; y < squareLengthY_; y++) {
            for (int x = 0; x < squareLengthX_; x++) {
                isFillCubes_[z][y][x] = false;
            }
        }
    }

    // blocks_の初期化
    blocks_.clear();
}

Stage stage_;
Stage* GetInstanceStage()
{
    return &stage_;
}
