#include "Player.h"
#include "Calc.h"

void Player::Initialize(Model* model, uint32_t textureHandle, uint32_t textureHandle2)
{
    // NULL�|�C���^�`�F�b�N
    assert(model);

    // �����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
    model_ = model;
    textureHandle_ = textureHandle;
    textureHandle2_ = textureHandle2;

    stage_ = GetInstanceStage();

    // �V���O���g���C���X�^���X���擾����
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

    // �s���񐔂�0���傫����
    if (indexMoveActionRemain_ > 0) {
        // �����������ňړ��x�N�g����ύX
        if (input_->TriggerKey(DIK_W)) {
            if (1 < nCubePos_.x && nCubePos_.x < stage_->squareLengthX_ - 2 && // �}�X�ڒP�ʁF1 < x & x < (100 - 2) 
                1 < nCubePos_.y && nCubePos_.y < stage_->squareLengthY_ - 2 && // �}�X�ڒP�ʁF1 < y & y < (100 - 2) 
                1 < nCubePos_.z && nCubePos_.z < stage_->squareLengthZ_ - 2) { // �}�X�ڒP�ʁF1 < z & z < (100 - 2) 
                if (face_ == Top) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y, floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Bottom) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Right) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Left) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Front) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,0,floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { 0,floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { 0,floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,0,floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Back) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,0,floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { 0,floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,0,floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
            }
            indexMoveActionRemain_--;
            keyMemory_.push_back('W');
        }
        if (input_->TriggerKey(DIK_S)) {
            if (1 < nCubePos_.x && nCubePos_.x < stage_->squareLengthX_ - 2 && // �}�X�ڒP�ʁF1 < x & x < (100 - 2) 
                1 < nCubePos_.y && nCubePos_.y < stage_->squareLengthY_ - 2 && // �}�X�ڒP�ʁF1 < y & y < (100 - 2) 
                1 < nCubePos_.z && nCubePos_.z < stage_->squareLengthZ_ - 2) { // �}�X�ڒP�ʁF1 < z & z < (100 - 2) 
                if (face_ == Top) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(180),floorDefRot_[Top].z };

                            // 1�}�X������
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                                 // �܂�Top�̂܂ܐ^��������
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180), floorDefRot_[Top].z };

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y, floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // Top�̂܂ܐ^��������
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z };

                            // 1�}�X������
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                                 // Top�̂܂ܐ^��������
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z };

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // Top�̂܂܌�����ς���
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(0),floorDefRot_[Top].z };

                            // 1�}�X������
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Top�̂܂܌�����ς���
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z };

                            // 1�}�X������
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Bottom) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // Bottom�̂܂܌�����ς���
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z };

                            // 1�}�X������
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // Bottom�̂܂܌�����ς���
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z };

                            // 1�}�X������
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Bottom�̂܂܌�����ς���
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z };

                            // 1�}�X������
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Bottom�̂܂܌�����ς���
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z };

                            // 1�}�X������
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Right) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Right�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90), floorDefRot_[Right].y,floorDefRot_[Right].z };

                            // 1�}�X������
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Right�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z };

                            // 1�}�X������
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0), floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Right�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90), floorDefRot_[Right].y,floorDefRot_[Right].z };

                            // 1�}�X������
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Right�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z };

                            // 1�}�X������
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Left) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Left�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z };

                            // 1�}�X������
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Left�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z };

                            // 1�}�X������
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Left�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z };

                            // 1�}�X������
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Left�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z };

                            // 1�}�X������
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Front) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Front�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z };

                            // 1�}�X������
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,0,floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Front�̂܂܌�����ς���
                            worldTransform_.rotation_ = { 0,floorDefRot_[Front].y,floorDefRot_[Front].z };

                            // 1�}�X������
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Front�̐^��̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z };

                            // 1�}�X������
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,0,floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Front�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z };

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { 0,floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { 0,floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Back) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Back�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z };

                            // 1�}�X������
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,0,floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Back�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z };

                            // 1�}�X������
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Back�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z };

                            // 1�}�X������
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,0,floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Back�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z };

                            // 1�}�X������
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
            }
            indexMoveActionRemain_--;
            keyMemory_.push_back('S');
        }
        if (input_->TriggerKey(DIK_A)) {
            if (1 < nCubePos_.x && nCubePos_.x < stage_->squareLengthX_ - 2 && // �}�X�ڒP�ʁF1 < x & x < (100 - 2) 
                1 < nCubePos_.y && nCubePos_.y < stage_->squareLengthY_ - 2 && // �}�X�ڒP�ʁF1 < y & y < (100 - 2) 
                1 < nCubePos_.z && nCubePos_.z < stage_->squareLengthZ_ - 2) { // �}�X�ڒP�ʁF1 < z & z < (100 - 2) 
                if (face_ == Top) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // ���̃}�X�Ƀu���b�N���Ȃ� & ���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //��������
                            //Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // ���̃}�X�Ƀu���b�N������

                            //��������
                            //Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // ���̃}�X�Ƀu���b�N���Ȃ� & ���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            //��������
                            //Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y, floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // ���̃}�X�Ƀu���b�N���Ȃ� & ���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Bottom) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // ���̃}�X�Ƀu���b�N���Ȃ� & ���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // ���̃}�X�Ƀu���b�N���Ȃ� & ���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // ���̃}�X�Ƀu���b�N������

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // ���̃}�X�Ƀu���b�N���Ȃ� & ���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Right) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // ���̃}�X�Ƀu���b�N���Ȃ� & ���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Left) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // ���̃}�X�Ƀu���b�N���Ȃ� & ���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // ���̃}�X�Ƀu���b�N������

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Front) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j


                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // ���̃}�X�Ƀu���b�N���Ȃ� & ���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Back) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].y,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].y,Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
            }
            indexMoveActionRemain_--;
            keyMemory_.push_back('A');
        }
        if (input_->TriggerKey(DIK_D)) {
            if (1 < nCubePos_.x && nCubePos_.x < stage_->squareLengthX_ - 2 && // �}�X�ڒP�ʁF1 < x & x < (100 - 2) 
                1 < nCubePos_.y && nCubePos_.y < stage_->squareLengthY_ - 2 && // �}�X�ڒP�ʁF1 < y & y < (100 - 2) 
                1 < nCubePos_.z && nCubePos_.z < stage_->squareLengthZ_ - 2) { // �}�X�ڒP�ʁF1 < z & z < (100 - 2) 
                if (face_ == Top) {
                    if (worldRot_ == North) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left�ɏ�肽��

                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y, floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // �E�̃}�X�Ƀu���b�N���Ȃ� & �E�̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false) {

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��																				 // 1�}�X�i��

                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������
                             //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��																				 // 1�}�X�i��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��																				 // 1�}�X�i��

                            //���@����E�̎΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Bottom) {
                    if (worldRot_ == North) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�̎΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�̎΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�̎΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�̎΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Right) {
                    if (worldRot_ == North) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�̎΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                         // ���W�͕ς��Ȃ�
                         // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                         // Top�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0), floorDefRot_[Front].y, floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0), floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Left) {
                    if (worldRot_ == North) {

                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                             // ���W�͕ς��Ȃ�
                             // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                             // Front�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // z�����΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {

                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // z�����΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // z�����΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // z�����΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Front) {
                    if (worldRot_ == North) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }

                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0), floorDefRot_[Right].y, floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }

                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(0),  floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }

                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180), floorDefRot_[Left].y, floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }

                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(0), floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Back) {
                    if (worldRot_ == North) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                             // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
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
    // �s���񐔂�0���傫����
    if (indexMoveActionRemain_ > 0) {
        // �i�[�������Q��
        if (keyMemory_[indexVecRef_] == 'W') {
            // W�������Ă����DIK_W���Ɠ����̋���
            if (1 < nCubePos_.x && nCubePos_.x < stage_->squareLengthX_ - 2 && // �}�X�ڒP�ʁF1 < x & x < (100 - 2) 
                1 < nCubePos_.y && nCubePos_.y < stage_->squareLengthY_ - 2 && // �}�X�ڒP�ʁF1 < y & y < (100 - 2) 
                1 < nCubePos_.z && nCubePos_.z < stage_->squareLengthZ_ - 2) { // �}�X�ڒP�ʁF1 < z & z < (100 - 2) 
                if (face_ == Top) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y, floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Bottom) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Right) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Left) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Front) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,0,floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { 0,floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { 0,floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,0,floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Back) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,0,floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { 0,floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,0,floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
            }
            indexVecRef_++;
            indexMoveActionRemain_--;
        }
        else if (keyMemory_[indexVecRef_] == 'S') {
            // S�������Ă����DIK_S���Ɠ����̋���
            if (1 < nCubePos_.x && nCubePos_.x < stage_->squareLengthX_ - 2 && // �}�X�ڒP�ʁF1 < x & x < (100 - 2) 
                1 < nCubePos_.y && nCubePos_.y < stage_->squareLengthY_ - 2 && // �}�X�ڒP�ʁF1 < y & y < (100 - 2) 
                1 < nCubePos_.z && nCubePos_.z < stage_->squareLengthZ_ - 2) { // �}�X�ڒP�ʁF1 < z & z < (100 - 2) 
                if (face_ == Top) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(180),floorDefRot_[Top].z };

                            // 1�}�X������
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                                 // �܂�Top�̂܂ܐ^��������
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180), floorDefRot_[Top].z };

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y, floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // Top�̂܂ܐ^��������
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z };

                            // 1�}�X������
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                                 // Top�̂܂ܐ^��������
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z };

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // Top�̂܂܌�����ς���
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(0),floorDefRot_[Top].z };

                            // 1�}�X������
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Top�̂܂܌�����ς���
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z };

                            // 1�}�X������
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Bottom) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // Bottom�̂܂܌�����ς���
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z };

                            // 1�}�X������
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // Bottom�̂܂܌�����ς���
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z };

                            // 1�}�X������
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Bottom�̂܂܌�����ς���
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z };

                            // 1�}�X������
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Bottom�̂܂܌�����ς���
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z };

                            // 1�}�X������
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Right) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Right�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90), floorDefRot_[Right].y,floorDefRot_[Right].z };

                            // 1�}�X������
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Right�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z };

                            // 1�}�X������
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0), floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Right�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90), floorDefRot_[Right].y,floorDefRot_[Right].z };

                            // 1�}�X������
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Right�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z };

                            // 1�}�X������
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Left) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Left�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z };

                            // 1�}�X������
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Left�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z };

                            // 1�}�X������
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Left�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z };

                            // 1�}�X������
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Left�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z };

                            // 1�}�X������
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Front) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Front�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z };

                            // 1�}�X������
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,0,floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Front�̂܂܌�����ς���
                            worldTransform_.rotation_ = { 0,floorDefRot_[Front].y,floorDefRot_[Front].z };

                            // 1�}�X������
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Front�̐^��̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z };

                            // 1�}�X������
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,0,floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Front�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z };

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { 0,floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { 0,floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Back) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Back�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z };

                            // 1�}�X������
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,0,floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Back�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z };

                            // 1�}�X������
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Back�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z };

                            // 1�}�X������
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // �^���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,0,floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //Back�̂܂܌�����ς���
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z };

                            // 1�}�X������
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // �^���̃}�X�Ƀu���b�N������

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // �^���̃}�X�Ƀu���b�N���Ȃ� & �^���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
            }
            indexVecRef_++;
            indexMoveActionRemain_--;
        }
        else if (keyMemory_[indexVecRef_] == 'A') {
            // A�������Ă����DIK_A���Ɠ����̋���
            if (1 < nCubePos_.x && nCubePos_.x < stage_->squareLengthX_ - 2 && // �}�X�ڒP�ʁF1 < x & x < (100 - 2) 
                1 < nCubePos_.y && nCubePos_.y < stage_->squareLengthY_ - 2 && // �}�X�ڒP�ʁF1 < y & y < (100 - 2) 
                1 < nCubePos_.z && nCubePos_.z < stage_->squareLengthZ_ - 2) { // �}�X�ڒP�ʁF1 < z & z < (100 - 2) 
                if (face_ == Top) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // ���̃}�X�Ƀu���b�N���Ȃ� & ���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //��������
                            //Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // ���̃}�X�Ƀu���b�N������

                            //��������
                            //Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // ���̃}�X�Ƀu���b�N���Ȃ� & ���̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            //��������
                            //Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y, floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // ���̃}�X�Ƀu���b�N���Ȃ� & ���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Bottom) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // ���̃}�X�Ƀu���b�N���Ȃ� & ���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // ���̃}�X�Ƀu���b�N���Ȃ� & ���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // ���̃}�X�Ƀu���b�N������

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // ���̃}�X�Ƀu���b�N���Ȃ� & ���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Right) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // ���̃}�X�Ƀu���b�N���Ȃ� & ���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Left) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // ���̃}�X�Ƀu���b�N���Ȃ� & ���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // ���̃}�X�Ƀu���b�N������

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Front) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j


                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // ���̃}�X�Ƀu���b�N���Ȃ� & ���̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Front
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Back) {
                    if (worldRot_ == North) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].y,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].y,Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���Ƀu���b�N������i��������j

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) { // ���ʂ̃}�X�Ƀu���b�N���Ȃ� & ���ʂ̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j

                            // ��������
                            // Back
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���ʎ΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
            }
            indexVecRef_++;
            indexMoveActionRemain_--;
        }
        else if (keyMemory_[indexVecRef_] == 'D') {
            // D�������Ă����DIK_D���Ɠ����̋���
            if (1 < nCubePos_.x && nCubePos_.x < stage_->squareLengthX_ - 2 && // �}�X�ڒP�ʁF1 < x & x < (100 - 2) 
                1 < nCubePos_.y && nCubePos_.y < stage_->squareLengthY_ - 2 && // �}�X�ڒP�ʁF1 < y & y < (100 - 2) 
                1 < nCubePos_.z && nCubePos_.z < stage_->squareLengthZ_ - 2) { // �}�X�ڒP�ʁF1 < z & z < (100 - 2) 
                if (face_ == Top) {
                    if (worldRot_ == North) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left�ɏ�肽��

                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y, floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) { // �E�̃}�X�Ƀu���b�N���Ȃ� & �E�̃}�X��1�}�X���Ƀu���b�N������i��������j

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false) {

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��																				 // 1�}�X�i��

                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) { // ���ʂ̃}�X�Ƀu���b�N������
                             //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��																				 // 1�}�X�i��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��																				 // 1�}�X�i��

                            //���@����E�̎΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Bottom) {
                    if (worldRot_ == North) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�̎΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�̎΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�̎΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�̎΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Right) {
                    if (worldRot_ == North) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�̎΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                         // ���W�͕ς��Ȃ�
                         // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                         // Top�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x - 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0), floorDefRot_[Front].y, floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0), floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x - 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Top].x,Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Left) {
                    if (worldRot_ == North) {

                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                             // ���W�͕ς��Ȃ�
                             // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                             // Front�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // z�����΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {

                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(-90),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x + 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // z�����΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(90),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Back�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // z�����΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Front�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(-90),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x + 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // z�����΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(90),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Front) {
                    if (worldRot_ == North) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }

                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0), floorDefRot_[Right].y, floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x - 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }

                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(0),  floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y - 1][nCubePos_.x] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }

                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180), floorDefRot_[Left].y, floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y][nCubePos_.x + 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }

                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(0), floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z - 1][nCubePos_.y + 1][nCubePos_.x] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Front].y,floorDefRot_[Front].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
                else if (face_ == Back) {
                    if (worldRot_ == North) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                             // 1�}�X�i��
                            nCubePos_.x++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x + 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x + 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == East) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(180),floorDefRot_[Top].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y - 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y - 1][nCubePos_.x] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x, Calc::ConvertToRadian(0),floorDefRot_[Bottom].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == South) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.x--;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Right�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Right].y,floorDefRot_[Right].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y][nCubePos_.x - 1] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y][nCubePos_.x - 1] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(0),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.x--;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Left�ɏ�肽��
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(180),floorDefRot_[Left].y,floorDefRot_[Left].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                    else if (worldRot_ == West) {
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���Ƀu���b�N������i��������j
                        if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                            stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == true) {
                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // 1�}�X�i��
                            nCubePos_.y++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;
                        }
                        // ���@����E�̃}�X�Ƀu���b�N������
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == true) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���W�͕ς��Ȃ�
                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Bottom�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Bottom].x,Calc::ConvertToRadian(180),floorDefRot_[Bottom].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                        // ���@����E�̃}�X�Ƀu���b�N���Ȃ� & ���@����E�̃}�X��1�}�X���ɂ��u���b�N���Ȃ��i�����Ȃ��j
                        else if (stage_->isFillCubes_[nCubePos_.z][nCubePos_.y + 1][nCubePos_.x] == false &&
                                 stage_->isFillCubes_[nCubePos_.z + 1][nCubePos_.y + 1][nCubePos_.x] == false) {

                            //�������Ă��������E���Ɍ����悤�ɉ�]
                            worldTransform_.rotation_ = { Calc::ConvertToRadian(-90),floorDefRot_[Back].y,floorDefRot_[Back].z }; // �����X�ɕς��悤�ɂ��鏈���ɗv�u��

                            // ���@����E�΂߉��̃}�X�ɐi�ށi�����̓����u���b�N�̈Ⴄ�ʂ̏�ɗ��C���[�W�j
                            nCubePos_.y++;
                            nCubePos_.z++;
                            worldTransform_.translation_.x = nCubePos_.x * stage_->blockSideLength_;
                            worldTransform_.translation_.y = nCubePos_.y * stage_->blockSideLength_;
                            worldTransform_.translation_.z = nCubePos_.z * stage_->blockSideLength_;

                            // TODO: ���ɑ΂��Č������ς��悤�ɂ���
                            // Top�ɏ�肽��
                            worldTransform_.rotation_ = { floorDefRot_[Top].x, Calc::ConvertToRadian(0),floorDefRot_[Top].z }; // �����ʐ^�̐�5�ʒu �����X�ɕς��悤�ɂ��鏈���ɗv�u��
                        }
                    }
                }
            }
            indexVecRef_++;
            indexMoveActionRemain_--;
        }

        // �Q�Ɣԍ����v�f���Ɠ����ɂȂ�����Q�Ɣԍ���0�ɖ߂�
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
//		// �e�̑��x
//		const float kBulletSpeed = 1.0f;
//		Vector3 velocity(0, 0, kBulletSpeed);
//
//		// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
//		velocity = Calc::DotVecMat(velocity, worldTransform_.matWorld_);
//
//		// �e�𔭎˂��A������
//		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
//		newBullet->Initialize(model_, GetWorldPosition() , velocity);
//
//		// �e��o�^����
//		bullets_.push_back(std::move(newBullet));
//	}
//}

void Player::Update()
{
#ifdef _DEBUG
    // �f�o�b�O�e�L�X�g
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
        // �ړ�
        Move();
    }
    else {
        if (fpsCount_ % speedRepeat_ == 0) {
            // �����I�Ɉړ�
            RepeatMove();
        }
    }

    // �ǂ̖ʂɗ����Ă��邩
    OnFace();

    // ���[���h����
    OnDirection();

    // ���̎�ނ̃u���b�N�̏�ɗ����Ă��邩
    OnBlock();
    if (underfootBlockId_ != MEMORY) {
        isRepeat_ = Repeat;
    }

    //���[���h�s��X�V
    worldTransform_.UpdateMatrix();

    // �v���C���[�̕ϐ��������֐�
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
    // ���[���h���W������ϐ�
    Vector3 worldPos;
    // ���[���h�s��̕��s�ړ��������擾�i���[���h���W�j
    worldPos.x = worldTransform_.matWorld_.m[3][0];
    worldPos.y = worldTransform_.matWorld_.m[3][1];
    worldPos.z = worldTransform_.matWorld_.m[3][2];

    return worldPos;
}