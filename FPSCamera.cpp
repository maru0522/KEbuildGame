#include "FPSCamera.h"
#include "Calc.h"

void FPSCamera::Initialize(Vector3 pos, Vector3 rad)
{
    // ���[���h�g�����X�t�H�[���̏����ݒ�
    worldTransform_.translation_ = pos;
    worldTransform_.rotation_ = rad;

    //worldTransform_.rotation_.x = Calc::ConvertToRadian(10);

    worldTransform_.Initialize();
    //worldTransform_.rotation_ = { Calc::ConvertToRadian(10),Calc::ConvertToRadian(0),Calc::ConvertToRadian(0) };

    // �r���[�v���W�F�N�V�����̏�����
    viewProjection_.Initialize();

    input_ = Input::GetInstance();
    debugText_ = DebugText::GetInstance();

    worldTransform_.UpdateMatrix();
}

void FPSCamera::Update(WorldTransform& WT)
{
#pragma region worldTransform
    // ���[���h�g�����X�t�H�[���̍��W�̐��l�����Z�i�ړ��j
    worldTransform_.translation_ = WT.translation_;
    // ���[���h�g�����X�t�H�[���̊p�x�̐��l�����Z�i��]�j
    worldTransform_.rotation_ = WT.rotation_;
    // ���[���h�g�����X�t�H�[���̃��[���h�s��Čv�Z
    worldTransform_.UpdateMatrix();
#pragma endregion

#pragma region viewProjection
    viewProjection_.eye = worldTransform_.translation_;

    // ���[���h�O���x�N�g��
    Vector3 forward(0, 0, 1);

    // ���[���J�����̉�]�𔽉f
    forward = Calc::DotVecMat(forward, worldTransform_.matWorld_);

    // ���_����O���ɓK���ȋ����i�񂾈ʒu�����ӓ_
    viewProjection_.target = viewProjection_.eye + forward;

    // ���[���h����x�N�g��
    Vector3 up(0, 1, 0);

    // ���[���J�����̉�]�𔽉f�i���[���J�����̏���x�N�g���j
    viewProjection_.up = Calc::DotVecMat(up, worldTransform_.matWorld_);

    //worldTransform_.UpdateMatrix();
    // �r���[�v���W�F�N�V�������X�V
    viewProjection_.TransferMatrix();
    viewProjection_.UpdateMatrix();
#pragma endregion

#ifdef _DEBUG
    // �f�o�b�O�e�L�X�g
    debugText_->SetPos(780, 10);
    debugText_->Printf("fpseye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

    debugText_->SetPos(710, 30);
    debugText_->Printf("fpsvwCameraPos:(%f,%f,%f)", viewProjection_.constMap->cameraPos.x, viewProjection_.constMap->cameraPos.y, viewProjection_.constMap->cameraPos.z);

    debugText_->SetPos(720, 50);
    debugText_->Printf("fpscameraPos:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

    debugText_->SetPos(720, 70);
    debugText_->Printf("fpscameraRot:(%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z);
#endif
}

Vector3 FPSCamera::GetWorldPosition()
{
    // ���[���h���W������ϐ�
    Vector3 worldPos;
    // ���[���h�s��̕��s�ړ��������擾�i���[���h���W�j
    worldPos.x = worldTransform_.matWorld_.m[3][0];
    worldPos.y = worldTransform_.matWorld_.m[3][1];
    worldPos.z = worldTransform_.matWorld_.m[3][2];

    return worldPos;
}

void FPSCamera::SetWorldPosition(Vector3& pos)
{
    worldTransform_.translation_ = pos;
}

void FPSCamera::Move()
{
    // �ړ��x�N�g��
    Vector3 move = { 0,0,0 };

    // �ړ�����
    const float kCharacterSpeed = 0.2f;

    // �����������ňړ��x�N�g����ύX
    if (input_->PushKey(DIK_UPARROW)) {
        move = { 0,0,kCharacterSpeed };
    }
    if (input_->PushKey(DIK_DOWNARROW)) {
        move = { 0,0,-kCharacterSpeed };
    }
    if (input_->PushKey(DIK_LEFTARROW)) {
        move = { -kCharacterSpeed,0,0 };
    }
    if (input_->PushKey(DIK_RIGHTARROW)) {
        move = { kCharacterSpeed,0,0 };
    }

    //// �����������ňړ��x�N�g����ύX
    //if (input_->TriggerKey(DIK_W)) {
    //    move = { 0,0,kCharacterSpeed };
    //}
    //if (input_->TriggerKey(DIK_S)) {
    //    move = { 0,0,-kCharacterSpeed };
    //}
    //if (input_->TriggerKey(DIK_A)) {
    //    move = { -kCharacterSpeed,0,0 };
    //}
    //if (input_->TriggerKey(DIK_D)) {
    //    move = { kCharacterSpeed,0,0 };
    //}
#ifdef _DEBUG
    if (input_->PushKey(DIK_NUMPAD8)) {
        move = { 0,kCharacterSpeed,0 };
    }
    if (input_->PushKey(DIK_NUMPAD2)) {
        move = { 0,-kCharacterSpeed,0 };
    }

    /*if (input_->TriggerKey(DIK_SPACE)) {
        move = { 0,kCharacterSpeed,0 };
    }
    if (input_->TriggerKey(DIK_LSHIFT)) {
        move = { 0,-kCharacterSpeed,0 };
    }*/

    if (input_->TriggerKey(DIK_NUMPAD6)) {
        worldTransform_.rotation_.y += Calc::ConvertToRadian(45);
    }
    if (input_->TriggerKey(DIK_NUMPAD4)) {
        worldTransform_.rotation_.y -= Calc::ConvertToRadian(45);
    }

    if (input_->TriggerKey(DIK_NUMPAD9)) {
        worldTransform_.rotation_.x += Calc::ConvertToRadian(45);
    }
    if (input_->TriggerKey(DIK_NUMPAD3)) {
        worldTransform_.rotation_.x -= Calc::ConvertToRadian(45);
    }
#endif

    worldTransform_.translation_.x += move.x;
    worldTransform_.translation_.y += move.y;
    worldTransform_.translation_.z += move.z;
}