#include "Player.h"
#include "Calc.h"

void Player::Initialize(Model* model, uint32_t textureHandle, uint32_t textureHandle2)
{
    // NULLポインタチェック
    assert(model);

    // 引数として受け取ったデータをメンバ変数に記録する
    model_ = model;
    textureHandle_ = textureHandle;
    textureHandle2_ = textureHandle2;

    stage_ = GetInstanceStage();

    // シングルトンインスタンスを取得する
    input_ = Input::GetInstance();
    debugText_ = DebugText::GetInstance();

    worldTransform_.translation_ = { 92,96,92 };
    worldTransform_.rotation_ = { Calc::ConvertToRadian(0),Calc::ConvertToRadian(0),Calc::ConvertToRadian(00) };
    //worldTransform_.rotation_ = { Calc::ConvertToRadian(-10),0,0 };

    worldTransform_.Initialize();

    floorDefRot_[0] = {};
    floorDefRot_[Top] = { Calc::ConvertToRadian(0) ,0, Calc::ConvertToRadian(0) };      // XZ
    floorDefRot_[Bottom] = { Calc::ConvertToRadian(0) ,0, Calc::ConvertToRadian(180) }; // XZ
    floorDefRot_[Right] = { 0 ,Calc::ConvertToRadian(0), Calc::ConvertToRadian(-90) };  // YZ
    floorDefRot_[Left] = { 0 ,Calc::ConvertToRadian(180), Calc::ConvertToRadian(-90) }; // YZ
    floorDefRot_[Front] = { 0 ,Calc::ConvertToRadian(90), Calc::ConvertToRadian(90) };  // YZ
    floorDefRot_[Back] = { 0 ,Calc::ConvertToRadian(-90), Calc::ConvertToRadian(90) };  // YZ
}

void Player::Move()
{
    nCubePos_.x = static_cast<int32_t>(GetWorldPosition().x / stage_->blockSideLength_);
    nCubePos_.y = static_cast<int32_t>(GetWorldPosition().y / stage_->blockSideLength_);
    nCubePos_.z = static_cast<int32_t>(GetWorldPosition().z / stage_->blockSideLength_);

    // 行動回数が0より大きい時
    if (indexMoveActionRemain_ > 0) {
        // 押した方向で移動ベクトルを変更
        if (input_->TriggerKey(DIK_W)) {
            if (1 < nCubePos_.x && nCubePos_.x < stage_->squareLengthX_ - 2 && // マス目単位：1 < x & x < (100 - 2) 
                1 < nCubePos_.y && nCubePos_.y < stage_->squareLengthY_ - 2 && // マス目単位：1 < y & y < (100 - 2) 
                1 < nCubePos_.z && nCubePos_.z < stage_->squareLengthZ_ - 2) { // マス目単位：1 < z & z < (100 - 2) 
                if (face_ == Top) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y, floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Bottom) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Right) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Left) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Front) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,0,floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { 0,floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { 0,floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,0,floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Back) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,0,floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { 0,floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,0,floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
            }
            indexMoveActionRemain_--;
            keyMemory_.push_back('W');
        }
        if (input_->TriggerKey(DIK_S)) {
            if (1 < nCubePos_.x && nCubePos_.x < stage_->squareLengthX_ - 2 && // マス目単位：1 < x & x < (100 - 2) 
                1 < nCubePos_.y && nCubePos_.y < stage_->squareLengthY_ - 2 && // マス目単位：1 < y & y < (100 - 2) 
                1 < nCubePos_.z && nCubePos_.z < stage_->squareLengthZ_ - 2) { // マス目単位：1 < z & z < (100 - 2) 
                if (face_ == Top) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(180),floorDefRot_[Top].z };

                            // 1マス下がる
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                                 // まずTopのまま真後ろを向く
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180), floorDefRot_[Top].z };

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y, floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            // Topのまま真後ろを向く
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z };

                            // 1マス下がる
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                                 // Topのまま真後ろを向く
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z };

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            // Topのまま向きを変える
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(0),floorDefRot_[Top].z };

                            // 1マス下がる
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Topのまま向きを変える
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z };

                            // 1マス下がる
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Bottom) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            // Bottomのまま向きを変える
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z };

                            // 1マス下がる
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // Bottomのまま向きを変える
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z };

                            // 1マス下がる
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Bottomのまま向きを変える
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z };

                            // 1マス下がる
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Bottomのまま向きを変える
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z };

                            // 1マス下がる
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Right) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Rightのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90), floorDefRot_[Right].y,floorDefRot_[Right].z };

                            // 1マス下がる
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 真後ろのマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            //Rightのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z };

                            // 1マス下がる
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0), floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Rightのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90), floorDefRot_[Right].y,floorDefRot_[Right].z };

                            // 1マス下がる
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Rightのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z };

                            // 1マス下がる
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Left) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Leftのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z };

                            // 1マス下がる
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Leftのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z };

                            // 1マス下がる
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Leftのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z };

                            // 1マス下がる
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Leftのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z };

                            // 1マス下がる
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Front) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Frontのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z };

                            // 1マス下がる
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,0,floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Frontのまま向きを変える
                            worldTransform_.rotation_ = { 0,floorDefRot_[Front].y,floorDefRot_[Front].z };

                            // 1マス下がる
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Frontの真後のまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z };

                            // 1マス下がる
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,0,floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Frontのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z };

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { 0,floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { 0,floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Back) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Backのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z };

                            // 1マス下がる
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,0,floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Backのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z };

                            // 1マス下がる
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Backのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z };

                            // 1マス下がる
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,0,floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Backのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z };

                            // 1マス下がる
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
            }
            indexMoveActionRemain_--;
            keyMemory_.push_back('S');
        }
        if (input_->TriggerKey(DIK_A)) {
            if (1 < nCubePos_.x && nCubePos_.x < stage_->squareLengthX_ - 2 && // マス目単位：1 < x & x < (100 - 2) 
                1 < nCubePos_.y && nCubePos_.y < stage_->squareLengthY_ - 2 && // マス目単位：1 < y & y < (100 - 2) 
                1 < nCubePos_.z && nCubePos_.z < stage_->squareLengthZ_ - 2) { // マス目単位：1 < z & z < (100 - 2) 
                if (face_ == Top) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // 左のマスにブロックがない & 左のマスの1マス下にブロックがある（床がある）

                            //左向かす
                            //Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 左のマスにブロックがある

                            //左向かす
                            //Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // 左のマスにブロックがない & 左のマスの1マス下にもブロックがない（床がない）

                            //左向かす
                            //Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 左斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y, floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 左のマスにブロックがない & 左のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Bottom) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // 左のマスにブロックがない & 左のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 左のマスにブロックがない & 左のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 左のマスにブロックがある

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // 左のマスにブロックがない & 左のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Right) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 左のマスにブロックがない & 左のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Left) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 左のマスにブロックがない & 左のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 左のマスにブロックがある

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Front) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）


                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 左のマスにブロックがない & 左のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Back) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].y,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].y,Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
            }
            indexMoveActionRemain_--;
            keyMemory_.push_back('A');
        }
        if (input_->TriggerKey(DIK_D)) {
            if (1 < nCubePos_.x && nCubePos_.x < stage_->squareLengthX_ - 2 && // マス目単位：1 < x & x < (100 - 2) 
                1 < nCubePos_.y && nCubePos_.y < stage_->squareLengthY_ - 2 && // マス目単位：1 < y & y < (100 - 2) 
                1 < nCubePos_.z && nCubePos_.z < stage_->squareLengthZ_ - 2) { // マス目単位：1 < z & z < (100 - 2) 
                if (face_ == Top) {
                    if (worldRot_ == North) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Leftに乗りたい

                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Rightに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y, floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 右のマスにブロックがない & 右のマスの1マス下にブロックがある（床がある）

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Frontに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false) {

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Backに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Rightに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Leftに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換																				 // 1マス進む

                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある
                             //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換																				 // 1マス進む

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Backに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換																				 // 1マス進む

                            //自機から右の斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Frontに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Bottom) {
                    if (worldRot_ == North) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Rightに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右の斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Leftに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Frontに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右の斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Backに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Leftに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右の斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Rightに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Backに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右の斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Frontに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Right) {
                    if (worldRot_ == North) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Backに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右の斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Frontに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                         // 座標は変わらない
                         // TODO: 床に対して向きが変わるようにする
                         // Topに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottomに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Frontに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0), floorDefRot_[Front].y, floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Backに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0), floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottomに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Topに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Left) {
                    if (worldRot_ == North) {

                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                             // 座標は変わらない
                             // TODO: 床に対して向きが変わるようにする
                             // Frontに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // z軸奥斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Backに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {

                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Topに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // z軸奥斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottomに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Backに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // z軸奥斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Frontに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottomに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // z軸奥斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Topに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Front) {
                    if (worldRot_ == North) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }

                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Rightに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0), floorDefRot_[Right].y, floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Leftに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }

                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Topに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(0),  floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottomに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }

                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Leftに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180), floorDefRot_[Left].y, floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Rightに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }

                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottomに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(0), floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Topに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Back) {
                    if (worldRot_ == North) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                             // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Leftに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Rightに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Topに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottomに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Rightに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Leftに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottomに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Topに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
            }
            indexMoveActionRemain_--;
            keyMemory_.push_back('D');
        }
    }

#ifdef _DEBUG
    if (input_->TriggerKey(DIK_SPACE)) {
        nCubePos_.y++;
        worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
        worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
        worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
    }
    if (input_->TriggerKey(DIK_LSHIFT)) {
        nCubePos_.y--;
        worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
        worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
        worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
    }

    if (input_->TriggerKey(DIK_I)) {
        worldTransform_.rotation_.x += Calc::ConvertToRadian(45);
    }
    if (input_->TriggerKey(DIK_U)) {
        worldTransform_.rotation_.x -= Calc::ConvertToRadian(45);
    }
    if (input_->TriggerKey(DIK_K)) {
        worldTransform_.rotation_.y += Calc::ConvertToRadian(45);
    }
    if (input_->TriggerKey(DIK_J)) {
        worldTransform_.rotation_.y -= Calc::ConvertToRadian(45);
    }
    if (input_->TriggerKey(DIK_M)) {
        worldTransform_.rotation_.z += Calc::ConvertToRadian(45);
    }
    if (input_->TriggerKey(DIK_N)) {
        worldTransform_.rotation_.z -= Calc::ConvertToRadian(45);
    }
#endif
}

void Player::RepeatMove()
{
    // 行動回数が0より大きい時
    if (indexMoveActionRemain_ > 0) {
        // 格納文字を参照
        if (keyMemory_[indexVecRef_] == 'W') {
            // Wが入っていればDIK_W時と同等の挙動
            if (1 < nCubePos_.x && nCubePos_.x < stage_->squareLengthX_ - 2 && // マス目単位：1 < x & x < (100 - 2) 
                1 < nCubePos_.y && nCubePos_.y < stage_->squareLengthY_ - 2 && // マス目単位：1 < y & y < (100 - 2) 
                1 < nCubePos_.z && nCubePos_.z < stage_->squareLengthZ_ - 2) { // マス目単位：1 < z & z < (100 - 2) 
                if (face_ == Top) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y, floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Bottom) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Right) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Left) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Front) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,0,floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { 0,floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { 0,floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,0,floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Back) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,0,floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { 0,floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,0,floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
            }
            indexVecRef_++;
            indexMoveActionRemain_--;
        }
        else if (keyMemory_[indexVecRef_] == 'S') {
            // Sが入っていればDIK_S時と同等の挙動
            if (1 < nCubePos_.x && nCubePos_.x < stage_->squareLengthX_ - 2 && // マス目単位：1 < x & x < (100 - 2) 
                1 < nCubePos_.y && nCubePos_.y < stage_->squareLengthY_ - 2 && // マス目単位：1 < y & y < (100 - 2) 
                1 < nCubePos_.z && nCubePos_.z < stage_->squareLengthZ_ - 2) { // マス目単位：1 < z & z < (100 - 2) 
                if (face_ == Top) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(180),floorDefRot_[Top].z };

                            // 1マス下がる
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                                 // まずTopのまま真後ろを向く
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180), floorDefRot_[Top].z };

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y, floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            // Topのまま真後ろを向く
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z };

                            // 1マス下がる
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                                 // Topのまま真後ろを向く
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z };

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            // Topのまま向きを変える
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(0),floorDefRot_[Top].z };

                            // 1マス下がる
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Topのまま向きを変える
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z };

                            // 1マス下がる
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Bottom) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            // Bottomのまま向きを変える
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z };

                            // 1マス下がる
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // Bottomのまま向きを変える
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z };

                            // 1マス下がる
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Bottomのまま向きを変える
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z };

                            // 1マス下がる
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Bottomのまま向きを変える
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z };

                            // 1マス下がる
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Right) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Rightのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90), floorDefRot_[Right].y,floorDefRot_[Right].z };

                            // 1マス下がる
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 真後ろのマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            //Rightのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z };

                            // 1マス下がる
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0), floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Rightのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90), floorDefRot_[Right].y,floorDefRot_[Right].z };

                            // 1マス下がる
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Rightのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z };

                            // 1マス下がる
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Left) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Leftのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z };

                            // 1マス下がる
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Leftのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z };

                            // 1マス下がる
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Leftのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z };

                            // 1マス下がる
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Leftのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z };

                            // 1マス下がる
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Front) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Frontのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z };

                            // 1マス下がる
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,0,floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Frontのまま向きを変える
                            worldTransform_.rotation_ = { 0,floorDefRot_[Front].y,floorDefRot_[Front].z };

                            // 1マス下がる
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Frontの真後のまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z };

                            // 1マス下がる
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,0,floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Frontのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z };

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { 0,floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { 0,floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Back) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Backのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z };

                            // 1マス下がる
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,0,floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Backのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z };

                            // 1マス下がる
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Backのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z };

                            // 1マス下がる
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 真後ろ斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,0,floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                            //Backのまま向きを変える
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z };

                            // 1マス下がる
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 真後ろのマスにブロックがある

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
            }
            indexVecRef_++;
            indexMoveActionRemain_--;
        }
        else if (keyMemory_[indexVecRef_] == 'A') {
            // Aが入っていればDIK_A時と同等の挙動
            if (1 < nCubePos_.x && nCubePos_.x < stage_->squareLengthX_ - 2 && // マス目単位：1 < x & x < (100 - 2) 
                1 < nCubePos_.y && nCubePos_.y < stage_->squareLengthY_ - 2 && // マス目単位：1 < y & y < (100 - 2) 
                1 < nCubePos_.z && nCubePos_.z < stage_->squareLengthZ_ - 2) { // マス目単位：1 < z & z < (100 - 2) 
                if (face_ == Top) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // 左のマスにブロックがない & 左のマスの1マス下にブロックがある（床がある）

                            //左向かす
                            //Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 左のマスにブロックがある

                            //左向かす
                            //Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // 左のマスにブロックがない & 左のマスの1マス下にもブロックがない（床がない）

                            //左向かす
                            //Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 左斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y, floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 左のマスにブロックがない & 左のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Bottom) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // 左のマスにブロックがない & 左のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 左のマスにブロックがない & 左のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 左のマスにブロックがある

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // 左のマスにブロックがない & 左のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Right) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 左のマスにブロックがない & 左のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Left) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 左のマスにブロックがない & 左のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 左のマスにブロックがある

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Front) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）


                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 左のマスにブロックがない & 左のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Back) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].y,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].y,Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // 正面のマスにブロックがある

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                            // 左向かす
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
            }
            indexVecRef_++;
            indexMoveActionRemain_--;
        }
        else if (keyMemory_[indexVecRef_] == 'D') {
            // Dが入っていればDIK_D時と同等の挙動
            if (1 < nCubePos_.x && nCubePos_.x < stage_->squareLengthX_ - 2 && // マス目単位：1 < x & x < (100 - 2) 
                1 < nCubePos_.y && nCubePos_.y < stage_->squareLengthY_ - 2 && // マス目単位：1 < y & y < (100 - 2) 
                1 < nCubePos_.z && nCubePos_.z < stage_->squareLengthZ_ - 2) { // マス目単位：1 < z & z < (100 - 2) 
                if (face_ == Top) {
                    if (worldRot_ == North) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Leftに乗りたい

                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Rightに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y, floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // 右のマスにブロックがない & 右のマスの1マス下にブロックがある（床がある）

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Frontに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false) {

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Backに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Rightに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Leftに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換																				 // 1マス進む

                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // 正面のマスにブロックがある
                             //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換																				 // 1マス進む

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Backに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換																				 // 1マス進む

                            //自機から右の斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Frontに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Bottom) {
                    if (worldRot_ == North) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Rightに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右の斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Leftに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Frontに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右の斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Backに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Leftに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右の斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Rightに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Backに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右の斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Frontに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Right) {
                    if (worldRot_ == North) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Backに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右の斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Frontに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                         // 座標は変わらない
                         // TODO: 床に対して向きが変わるようにする
                         // Topに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottomに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Frontに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0), floorDefRot_[Front].y, floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Backに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0), floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottomに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Topに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Left) {
                    if (worldRot_ == North) {

                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                             // 座標は変わらない
                             // TODO: 床に対して向きが変わるようにする
                             // Frontに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // z軸奥斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Backに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {

                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Topに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // z軸奥斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottomに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Backに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // z軸奥斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Frontに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottomに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換

                            // z軸奥斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Topに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Front) {
                    if (worldRot_ == North) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }

                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Rightに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0), floorDefRot_[Right].y, floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Leftに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }

                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Topに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(0),  floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottomに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }

                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Leftに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180), floorDefRot_[Left].y, floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Rightに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }

                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottomに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(0), floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Topに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
                else if (face_ == Back) {
                    if (worldRot_ == North) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                             // 1マス進む
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Leftに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Rightに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == East) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Topに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Bottomに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == South) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Rightに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Leftに乗りたい
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                    else if (worldRot_ == West) {
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にブロックがある（床がある）
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) {
                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 1マス進む
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // 自機から右のマスにブロックがある
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 座標は変わらない
                            // TODO: 床に対して向きが変わるようにする
                            // Bottomに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // ※徐々に変わるようにする処理に要置換
                        }
                        // 自機から右のマスにブロックがない & 自機から右のマスの1マス下にもブロックがない（床がない）
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) {

                            //今向いてる方向から右側に向くように回転
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // ※徐々に変わるようにする処理に要置換

                            // 自機から右斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: 床に対して向きが変わるようにする
                            // Topに乗りたい
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                        }
                    }
                }
            }
            indexVecRef_++;
            indexMoveActionRemain_--;
        }

        // 参照番号が要素数と同じになったら参照番号を0に戻す
        if (indexVecRef_ == keyMemory_.size()) {
            indexVecRef_ = 0;
        }
    }
}

void Player::OnFace()
{
    // TOP
    if (worldTransform_.rotation_.x == floorDefRot_[Top].x && worldTransform_.rotation_.z == floorDefRot_[Top].z) {
        face_ = Top;
    }
    // BOTTOM
    else if (worldTransform_.rotation_.x == floorDefRot_[Bottom].x && worldTransform_.rotation_.z == floorDefRot_[Bottom].z) {
        face_ = Bottom;
    }
    // RIGHT
    else if (worldTransform_.rotation_.y == floorDefRot_[Right].y && worldTransform_.rotation_.z == floorDefRot_[Right].z) {
        face_ = Right;
    }
    // Left
    else if (worldTransform_.rotation_.y == floorDefRot_[Left].y && worldTransform_.rotation_.z == floorDefRot_[Left].z) {
        face_ = Left;
    }
    // Front
    else if (worldTransform_.rotation_.y == floorDefRot_[Front].y && worldTransform_.rotation_.z == floorDefRot_[Front].z) {
        face_ = Front;
    }
    // Back
    else if (worldTransform_.rotation_.y == floorDefRot_[Back].y && worldTransform_.rotation_.z == floorDefRot_[Back].z) {
        face_ = Back;
    }
#ifdef _DEBUG
    // TOP
    if (worldTransform_.rotation_.x == Calc::ConvertToRadian(180) && worldTransform_.rotation_.z == Calc::ConvertToRadian(180)) {
        face_ = 99;
    }
    // BOTTOM
    else if (worldTransform_.rotation_.x == Calc::ConvertToRadian(180) && worldTransform_.rotation_.z == 0) {
        face_ = 99;
    }
    // RIGHT
    else if (worldTransform_.rotation_.y == Calc::ConvertToRadian(180) && worldTransform_.rotation_.z == Calc::ConvertToRadian(90)) {
        face_ = 99;
    }
    // Left
    else if (worldTransform_.rotation_.y == 0 && worldTransform_.rotation_.z == Calc::ConvertToRadian(90)) {
        face_ = 99;
    }
    // Front
    else if (worldTransform_.rotation_.y == Calc::ConvertToRadian(-90) && worldTransform_.rotation_.z == Calc::ConvertToRadian(-90)) {
        face_ = 99;
    }
    // Back
    else if (worldTransform_.rotation_.y == Calc::ConvertToRadian(90) && worldTransform_.rotation_.z == Calc::ConvertToRadian(-90)) {
        face_ = 99;
    }
#endif

}

void Player::OnDirection()
{
    if (face_ == Top) {
        if (worldTransform_.rotation_.y == 0) {
            worldRot_ = North;
        }
        if (worldTransform_.rotation_.y == Calc::ConvertToRadian(90)) {
            worldRot_ = East;
        }
        if (worldTransform_.rotation_.y == Calc::ConvertToRadian(180)) {
            worldRot_ = South;
        }
        if (worldTransform_.rotation_.y == Calc::ConvertToRadian(-90) || worldTransform_.rotation_.y == Calc::ConvertToRadian(270)) {
            worldRot_ = West;
        }
    }
    else if (face_ == Bottom) {
        if (worldTransform_.rotation_.y == 0) {
            worldRot_ = North;
        }
        if (worldTransform_.rotation_.y == Calc::ConvertToRadian(-90) || worldTransform_.rotation_.y == Calc::ConvertToRadian(270)) {
            worldRot_ = East;
        }
        if (worldTransform_.rotation_.y == Calc::ConvertToRadian(180)) {
            worldRot_ = South;
        }
        if (worldTransform_.rotation_.y == Calc::ConvertToRadian(90)) {
            worldRot_ = West;
        }
    }
    else if (face_ == Right) {
        if (worldTransform_.rotation_.x == Calc::ConvertToRadian(-90) || worldTransform_.rotation_.x == Calc::ConvertToRadian(270)) {
            worldRot_ = North;
        }
        if (worldTransform_.rotation_.x == 0) {
            worldRot_ = East;
        }
        if (worldTransform_.rotation_.x == Calc::ConvertToRadian(90)) {
            worldRot_ = South;
        }
        if (worldTransform_.rotation_.x == Calc::ConvertToRadian(180)) {
            worldRot_ = West;
        }
    }
    else if (face_ == Left) {
        if (worldTransform_.rotation_.x == Calc::ConvertToRadian(-90) || worldTransform_.rotation_.x == Calc::ConvertToRadian(270)) {
            worldRot_ = North;
        }
        if (worldTransform_.rotation_.x == 0) {
            worldRot_ = East;
        }
        if (worldTransform_.rotation_.x == Calc::ConvertToRadian(90)) {
            worldRot_ = South;
        }
        if (worldTransform_.rotation_.x == Calc::ConvertToRadian(180)) {
            worldRot_ = West;
        }
    }
    else if (face_ == Front) {
        if (worldTransform_.rotation_.x == Calc::ConvertToRadian(-90) || worldTransform_.rotation_.x == Calc::ConvertToRadian(270)) {
            worldRot_ = North;
        }
        if (worldTransform_.rotation_.x == Calc::ConvertToRadian(180)) {
            worldRot_ = East;
        }
        if (worldTransform_.rotation_.x == Calc::ConvertToRadian(90)) {
            worldRot_ = South;
        }
        if (worldTransform_.rotation_.x == 0) {
            worldRot_ = West;
        }
    }
    else if (face_ == Back) {
        if (worldTransform_.rotation_.x == Calc::ConvertToRadian(-90) || worldTransform_.rotation_.x == Calc::ConvertToRadian(270)) {
            worldRot_ = North;
        }
        if (worldTransform_.rotation_.x == Calc::ConvertToRadian(180)) {
            worldRot_ = East;
        }
        if (worldTransform_.rotation_.x == Calc::ConvertToRadian(90)) {
            worldRot_ = South;
        }
        if (worldTransform_.rotation_.x == 0) {
            worldRot_ = West;
        }
    }
}

void Player::OnBlock()
{
    Vector3 pUnderBlockTrans(0, 0, 0);
    if (face_ == Top) {
        pUnderBlockTrans = {
            worldTransform_.translation_.x,
            worldTransform_.translation_.y - stage_->blockSideLength_ * 1,
            worldTransform_.translation_.z };

        for (Block& block : stage_->blocks_) {
            if (block.GetWorldTransform()->translation_ == pUnderBlockTrans) {
                underfootBlockId_ = block.GetIndexBlock();
            }
        }
    }
    else if (face_ == Bottom) {
        pUnderBlockTrans = {
            worldTransform_.translation_.x,
            worldTransform_.translation_.y + stage_->blockSideLength_ * 1,
            worldTransform_.translation_.z };

        for (Block& block : stage_->blocks_) {
            if (block.GetWorldTransform()->translation_ == pUnderBlockTrans) {
                underfootBlockId_ = block.GetIndexBlock();
            }
        }
    }
    else if (face_ == Right) {
        pUnderBlockTrans = {
            worldTransform_.translation_.x - stage_->blockSideLength_ * 1,
            worldTransform_.translation_.y,
            worldTransform_.translation_.z };

        for (Block& block : stage_->blocks_) {
            if (block.GetWorldTransform()->translation_ == pUnderBlockTrans) {
                underfootBlockId_ = block.GetIndexBlock();
            }
        }
    }
    else if (face_ == Left) {
        pUnderBlockTrans = {
            worldTransform_.translation_.x + stage_->blockSideLength_ * 1,
            worldTransform_.translation_.y,
            worldTransform_.translation_.z };

        for (Block& block : stage_->blocks_) {
            if (block.GetWorldTransform()->translation_ == pUnderBlockTrans) {
                underfootBlockId_ = block.GetIndexBlock();
            }
        }
    }
    else if (face_ == Front) {
        pUnderBlockTrans = {
            worldTransform_.translation_.x,
            worldTransform_.translation_.y,
            worldTransform_.translation_.z - stage_->blockSideLength_ * 1 };

        for (Block& block : stage_->blocks_) {
            if (block.GetWorldTransform()->translation_ == pUnderBlockTrans) {
                underfootBlockId_ = block.GetIndexBlock();
            }
        }
    }
    else if (face_ == Back) {
        pUnderBlockTrans = {
            worldTransform_.translation_.x,
            worldTransform_.translation_.y,
            worldTransform_.translation_.z + stage_->blockSideLength_ * 1 };

        for (Block& block : stage_->blocks_) {
            if (block.GetWorldTransform()->translation_ == pUnderBlockTrans) {
                underfootBlockId_ = block.GetIndexBlock();
            }
        }
    }
}

//void Player::Rotate()
//{
//	if (input_->PushKey(DIK_U)) {
//		worldTransform_.rotation_.y -= 0.05f;
//	}
//	if (input_->PushKey(DIK_I)) {
//		worldTransform_.rotation_.y += 0.05f;
//	}
//
//}
//
//void Player::Attack()
//{
//	if (input_->PushKey(DIK_SPACE)) {
//		// 弾の速度
//		const float kBulletSpeed = 1.0f;
//		Vector3 velocity(0, 0, kBulletSpeed);
//
//		// 速度ベクトルを自機の向きに合わせて回転させる
//		velocity = Calc::DotVecMat(velocity, worldTransform_.matWorld_);
//
//		// 弾を発射し、初期化
//		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
//		newBullet->Initialize(model_, GetWorldPosition() , velocity);
//
//		// 弾を登録する
//		bullets_.push_back(std::move(newBullet));
//	}
//}

void Player::Update()
{
#ifdef _DEBUG
    // デバッグテキスト
    debugText_->SetPos(70, 150);
    debugText_->Printf("player:(%f,%f,%f)",
                       worldTransform_.translation_.x,
                       worldTransform_.translation_.y,
                       worldTransform_.translation_.z);
    debugText_->SetPos(70, 170);
    debugText_->Printf("playerRot:(%f,%f,%f)",
                       worldTransform_.rotation_.x,
                       worldTransform_.rotation_.y,
                       worldTransform_.rotation_.z);
#pragma region face
    debugText_->SetPos(60, 190);
    if (face_ == Top) {
        debugText_->Printf("playerOnFace:(Top)");
    }
    else if (face_ == Bottom) {
        debugText_->Printf("playerOnFace:(Bottom)");
    }
    else if (face_ == Right) {
        debugText_->Printf("playerOnFace:(Right)");
    }
    else if (face_ == Left) {
        debugText_->Printf("playerOnFace:(Left)");
    }
    else if (face_ == Front) {
        debugText_->Printf("playerOnFace:(Front)");
    }
    else if (face_ == Back) {
        debugText_->Printf("playerOnFace:(Back)");
    }
    else if (face_ == 99) {
        debugText_->Printf("playerOnFace:(error.this Rot isnt setting at DefRot)");
    }
#pragma endregion
#pragma region Direction
    debugText_->SetPos(60, 210);
    if (worldRot_ == North) {
        debugText_->Printf("OnDirection:(North)");
    }
    else if (worldRot_ == East) {
        debugText_->Printf("OnDirection:(East)");
    }
    else if (worldRot_ == South) {
        debugText_->Printf("OnDirection:(South)");
    }
    else if (worldRot_ == West) {
        debugText_->Printf("OnDirection:(West)");
    }
#pragma endregion
    debugText_->SetPos(100, 230);
    debugText_->Printf("nCubePos:(%f,%f,%f)", nCubePos_.x, nCubePos_.x, nCubePos_.x);
    debugText_->SetPos(100, 250);
    debugText_->Printf("underBlock:(%d)", underfootBlockId_);
    debugText_->SetPos(80, 250);
    debugText_->Printf("remain_:(%d)", indexMoveActionRemain_);
    debugText_->SetPos(100, 310);
    debugText_->Printf("key_w:(%d)", input_->PushKey(DIK_W));
#endif
    if (isRepeat_ == Learn) {
        // 移動
        Move();
    }
    else {
        if (fpsCount_ % speedRepeat_ == 0) {
            // 自動的に移動
            RepeatMove();
        }
    }

    // どの面に立っているか
    OnFace();

    // ワールド方向
    OnDirection();

    // 何の種類のブロックの上に立っているか
    OnBlock();
    if (underfootBlockId_ != MEMORY) {
        isRepeat_ = Repeat;
    }

    //ワールド行列更新
    worldTransform_.UpdateMatrix();

    // プレイヤーの変数初期化関数
    //Reset();

    fpsCount_++;
}

void Player::Draw(ViewProjection viewProjection)
{
    if (isRepeat_ == Learn) {
        model_->Draw(worldTransform_, viewProjection, textureHandle_);
    }
    else {
        model_->Draw(worldTransform_, viewProjection, textureHandle2_);
    }
}

void Player::Reset()
{
    nCubePos_ = { 0,0,0 };

    worldTransform_.translation_ = { 96,96,96 };
    worldTransform_.rotation_ = { 0,0,0 };

    face_ = 0;
    worldRot_ = 0;

    underfootBlockId_ = MEMORY;

    isRepeat_ = Learn;
    keyMemory_.clear();

    fpsCount_ = 0;
}

//void Player::OnCollision()
//{
//}

Vector3 Player::GetWorldPosition()
{
    // ワールド座標を入れる変数
    Vector3 worldPos;
    // ワールド行列の平行移動成分を取得（ワールド座標）
    worldPos.x = worldTransform_.matWorld_.m[3][0];
    worldPos.y = worldTransform_.matWorld_.m[3][1];
    worldPos.z = worldTransform_.matWorld_.m[3][2];

    return worldPos;
}