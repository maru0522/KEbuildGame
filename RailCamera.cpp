#include "RailCamera.h"
#include "Calc.h"


void RailCamera::Initialize(Vector3 pos, Vector3 rad)
{
    // ���[���h�g�����X�t�H�[���̏����ݒ�
    worldTransform_.translation_ = pos;
    worldTransform_.rotation_ = rad;
   
    worldTransform_.Initialize();

    // �r���[�v���W�F�N�V�����̏�����
    viewProjection_.Initialize();

    debugText_ = DebugText::GetInstance();
}

void RailCamera::Update()
{
#pragma region worldTransform
    // ���[���h�g�����X�t�H�[���̍��W�̐��l�����Z�i�ړ��j
    //worldTransform_.translation_.z -= 0.01;

    // ���[���h�g�����X�t�H�[���̊p�x�̐��l�����Z�i��]�j

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
    debugText_->SetPos(80, 10);
    debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

    debugText_->SetPos(10, 30);
    debugText_->Printf("vwCameraPos:(%f,%f,%f)", viewProjection_.constMap->cameraPos.x, viewProjection_.constMap->cameraPos.y, viewProjection_.constMap->cameraPos.z);

    debugText_->SetPos(20, 50);
    debugText_->Printf("cameraPos:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
#endif
}

Vector3 RailCamera::GetWorldPosition()
{
    // ���[���h���W������ϐ�
    Vector3 worldPos;
    // ���[���h�s��̕��s�ړ��������擾�i���[���h���W�j
    worldPos.x = worldTransform_.matWorld_.m[3][0];
    worldPos.y = worldTransform_.matWorld_.m[3][1];
    worldPos.z = worldTransform_.matWorld_.m[3][2];

    return worldPos;
}
