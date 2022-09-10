#include "Player.h"
#include "Calc.h"

void Player::Initialize(Model* model, uint32_t textureHandle)
{
    // NULLポインタチェック
    assert(model);

    // 引数として受け取ったデータをメンバ変数に記録する
    model_ = model;
    textureHandle_ = textureHandle;

    stage_ = GetInstanceStage();

    // シングルトンインスタンスを取得する
    input_ = Input::GetInstance();
    debugText_ = DebugText::GetInstance();

    worldTransform_.translation_ = { 96,90,88 };
    worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),Calc::ConvertToRadian(90),Calc::ConvertToRadian(-90) };
    //worldTransform_.rotation_ = { Calc::ConvertToRadian(-10),0,0 };

    worldTransform_.Initialize();
}

void Player::Move()
{
    nCubePos_.x = static_cast<int32_t>(GetWorldPosition().x / stage_->blockSideLength_);
    nCubePos_.y = static_cast<int32_t>(GetWorldPosition().y / stage_->blockSideLength_);
    nCubePos_.z = static_cast<int32_t>(GetWorldPosition().z / stage_->blockSideLength_);

    // 押した方向で移動ベクトルを変更
    if (input_->TriggerKey(DIK_W)) {
        if (nCubePos_.y > 1 && nCubePos_.z < stage_->squareLengthZ_ - 2) { // マス目単位：y > 1 & z < (100 - 2) 
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
                        // X軸に対して-90度回転 ※後に270度に置換
                        worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),Calc::ConvertToRadian(90),Calc::ConvertToRadian(-90) }; // ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して90度回転 
                        worldTransform_.rotation_ = { Calc::ConvertToRadian(90),Calc::ConvertToRadian(90),Calc::ConvertToRadian(90) }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して-90度回転 ※後に270度に置換
                        worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),0,Calc::ConvertToRadian(90) }; // ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して90度回転 
                        worldTransform_.rotation_ = { Calc::ConvertToRadian(90),0,Calc::ConvertToRadian(-90) }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して-90度回転 ※後に270度に置換
                        worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),Calc::ConvertToRadian(90),Calc::ConvertToRadian(90) }; // ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して90度回転 
                        worldTransform_.rotation_ = { Calc::ConvertToRadian(90),Calc::ConvertToRadian(-90),Calc::ConvertToRadian(90) }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して-90度回転 ※後に270度に置換
                        worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),0,Calc::ConvertToRadian(-90) }; // ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して90度回転 
                        worldTransform_.rotation_ = { Calc::ConvertToRadian(90),Calc::ConvertToRadian(180),Calc::ConvertToRadian(-90) }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して-90度回転 ※後に270度に置換
                        worldTransform_.rotation_ = { Calc::ConvertToRadian(90),Calc::ConvertToRadian(90),Calc::ConvertToRadian(-90) }; // ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して90度回転 
                        worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),Calc::ConvertToRadian(90),Calc::ConvertToRadian(90) }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して-90度回転 ※後に270度に置換
                        worldTransform_.rotation_ = { Calc::ConvertToRadian(90),Calc::ConvertToRadian(0),Calc::ConvertToRadian(-90) }; // ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して90度回転 
                        worldTransform_.rotation_ = { Calc::ConvertToRadian(0),Calc::ConvertToRadian(180),Calc::ConvertToRadian(90) }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して-90度回転 ※後に270度に置換
                        worldTransform_.rotation_ = { Calc::ConvertToRadian(90),Calc::ConvertToRadian(90),Calc::ConvertToRadian(90) }; // ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して90度回転 
                        worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),Calc::ConvertToRadian(-90),Calc::ConvertToRadian(90) }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して-90度回転 ※後に270度に置換
                        worldTransform_.rotation_ = { Calc::ConvertToRadian(90),Calc::ConvertToRadian(0),Calc::ConvertToRadian(90) }; // ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して90度回転 
                        worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),Calc::ConvertToRadian(180),Calc::ConvertToRadian(90) }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して-90度回転 ※後に270度に置換
                        worldTransform_.rotation_ = { 0,0,Calc::ConvertToRadian(180) }; // ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して90度回転 
                        worldTransform_.rotation_ = { 0,Calc::ConvertToRadian(180),0 }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して-90度回転 ※後に270度に置換
                        worldTransform_.rotation_ = { 0,0,Calc::ConvertToRadian(-90) }; // ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して90度回転 
                        worldTransform_.rotation_ = { 0,Calc::ConvertToRadian(180),Calc::ConvertToRadian(-90) }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して-90度回転 ※後に270度に置換
                        worldTransform_.rotation_ = { 0,0,0 }; // ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して90度回転 
                        worldTransform_.rotation_ = { 0,Calc::ConvertToRadian(180),Calc::ConvertToRadian(180) }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して-90度回転 ※後に270度に置換
                        worldTransform_.rotation_ = { 0,Calc::ConvertToRadian(180),Calc::ConvertToRadian(-90) }; // ※徐々に変わるようにする処理に要置換
                    }
                    else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                             stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                        // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                        nCubePos_.x++;
                        nCubePos_.z--;
                        worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                        worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                        worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                        // TODO: 床に対して向きが変わるようにする
                        // X軸に対して90度回転 
                        worldTransform_.rotation_ = { 0,Calc::ConvertToRadian(180),Calc::ConvertToRadian(90) }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して-90度回転 ※後に270度に置換
                        worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),Calc::ConvertToRadian(-90),Calc::ConvertToRadian(90) }; // ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して90度回転 
                        worldTransform_.rotation_ = { 0,0,0 }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して-90度回転 ※後に270度に置換
                        worldTransform_.rotation_ = { Calc::ConvertToRadian(0),Calc::ConvertToRadian(180),Calc::ConvertToRadian(0) }; // ※徐々に変わるようにする処理に要置換
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
                        // X軸に対して90度回転 
                        worldTransform_.rotation_ = { 0,0,0 }; // メモ写真の青5位置 ※徐々に変わるようにする処理に要置換
                    }
                }
            }
        }
    }
    if (input_->TriggerKey(DIK_S)) {
        nCubePos_.z--;
        worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
        worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
        worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
    }
    if (input_->TriggerKey(DIK_A)) {
        nCubePos_.x--;
        worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
        worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
        worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
    }
    if (input_->TriggerKey(DIK_D)) {
        nCubePos_.x++;
        worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
        worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
        worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
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

void Player::OnFace()
{
    // TOP
    if (worldTransform_.rotation_.x == 0 && worldTransform_.rotation_.z == 0 ||
        worldTransform_.rotation_.x == Calc::ConvertToRadian(180) && worldTransform_.rotation_.z == Calc::ConvertToRadian(180)) {
        face_ = Top;
    }
    // BOTTOM
    else if (worldTransform_.rotation_.x == 0 && worldTransform_.rotation_.z == Calc::ConvertToRadian(180) ||
             worldTransform_.rotation_.x == Calc::ConvertToRadian(180) && worldTransform_.rotation_.z == 0) {
        face_ = Bottom;
    }
    // RIGHT
    else if (worldTransform_.rotation_.y == 0 && worldTransform_.rotation_.z == Calc::ConvertToRadian(-90) ||
             worldTransform_.rotation_.y == Calc::ConvertToRadian(180) && worldTransform_.rotation_.z == Calc::ConvertToRadian(90)) {
        face_ = Right;
    }
    // Left
    else if (worldTransform_.rotation_.y == 0 && worldTransform_.rotation_.z == Calc::ConvertToRadian(90) ||
             worldTransform_.rotation_.y == Calc::ConvertToRadian(180) && worldTransform_.rotation_.z == Calc::ConvertToRadian(-90)) {
        face_ = Left;
    }
    // Front
    else if (worldTransform_.rotation_.y == Calc::ConvertToRadian(90) && worldTransform_.rotation_.z == Calc::ConvertToRadian(90) ||
             worldTransform_.rotation_.y == Calc::ConvertToRadian(-90) && worldTransform_.rotation_.z == Calc::ConvertToRadian(-90)) {
        face_ = Front;
    }
    // Back
    else if (worldTransform_.rotation_.y == Calc::ConvertToRadian(-90) && worldTransform_.rotation_.z == Calc::ConvertToRadian(90) ||
             worldTransform_.rotation_.y == Calc::ConvertToRadian(90) && worldTransform_.rotation_.z == Calc::ConvertToRadian(-90)) {
        face_ = Back;
    }
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
        if (worldTransform_.rotation_.y == Calc::ConvertToRadian(180)) {
            worldRot_ = North;
        }
        if (worldTransform_.rotation_.y == Calc::ConvertToRadian(90)) {
            worldRot_ = East;
        }
        if (worldTransform_.rotation_.y == 0) {
            worldRot_ = South;
        }
        if (worldTransform_.rotation_.y == Calc::ConvertToRadian(-90) || worldTransform_.rotation_.y == Calc::ConvertToRadian(270)) {
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
    debugText_->SetPos(60, 190);
    debugText_->Printf("playerOnFace:(%d)", face_);
    debugText_->SetPos(60, 210);
    debugText_->Printf("OnDirection:(%d)", worldRot_);
#endif

    // 移動
    Move();

    // どの面に立っているか
    OnFace();

    // ワールド方向
    OnDirection();

    //ワールド行列更新
    worldTransform_.UpdateMatrix();
}

void Player::Draw(ViewProjection viewProjection)
{
    model_->Draw(worldTransform_, viewProjection, textureHandle_);
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