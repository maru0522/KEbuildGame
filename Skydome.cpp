#include "SkyDome.h"
#include <cassert>

void Skydome::Initialize(Model* model)
{
    // NULL�|�C���^�`�F�b�N
    assert(model);

    // �����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
    model_ = model;

    worldTransform_.Initialize();
    worldTransform_.translation_ = { 200,200,200 };
    worldTransform_.scale_ = { 2.0f, 2.0f, 2.0f };
    worldTransform_.UpdateMatrix();

    input_ = Input::GetInstance();
    debugText_ = DebugText::GetInstance();
}

void Skydome::Update()
{
#ifdef _DEBUG
    // �ړ��x�N�g��
    Vector3 move = { 0,0,0 };

    // �ړ�����
    const float kCharacterSpeed = 1.0f;

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

    worldTransform_.translation_.x += move.x;
    worldTransform_.translation_.y += move.y;
    worldTransform_.translation_.z += move.z;

    worldTransform_.UpdateMatrix();

    // �f�o�b�O�e�L�X�g
    debugText_->SetPos(500, 10);
    debugText_->Printf("skyDome:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
#endif
}

void Skydome::Draw(ViewProjection viewProjection_)
{
    // 3D���f���`��
    model_->Draw(worldTransform_, viewProjection_);
}
