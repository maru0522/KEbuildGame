#include "Object.h"

void Object::Initialize(Model* model, Vector3 pos, Vector3 rot)
{
    // NULL�|�C���^�`�F�b�N
    assert(model);

    // �����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
    model_ = model;

    // �V���O���g���C���X�^���X�̎擾
    debugText_ = DebugText::GetInstance();

    worldTransform_.translation_ = pos;
    worldTransform_.rotation_ = rot;

    worldTransform_.Initialize();
    worldTransform_.UpdateMatrix();
}

void Object::Initialize(Model* model, uint32_t textureHandle, Vector3 pos, Vector3 rot)
{
    // NULL�|�C���^�`�F�b�N
    assert(model);

    // �����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
    model_ = model;
    textureHandle_ = textureHandle;

    // �V���O���g���C���X�^���X�̎擾
    debugText_ = DebugText::GetInstance();

    worldTransform_.translation_ = pos;
    worldTransform_.rotation_ = rot;

    worldTransform_.Initialize();
    worldTransform_.UpdateMatrix();
}

void Object::Update()
{
#ifdef _DEBUG
    // �f�o�b�O�e�L�X�g
    debugText_->SetPos(50, 400);
    debugText_->Printf("objPos:(%f,%f,%f)",
                       worldTransform_.translation_.x,
                       worldTransform_.translation_.y,
                       worldTransform_.translation_.z);
#endif // _DEBUG

    if (isRolling_) {
        if (isLeftRoll_) {
            worldTransform_.rotation_.x -= Calc::ConvertToRadian(1);
        }
        else {
            worldTransform_.rotation_.x += Calc::ConvertToRadian(1);
        }
    }

    //���[���h�s��X�V
    worldTransform_.UpdateMatrix();
}

void Object::Draw(ViewProjection viewProjection, bool isTexture)
{
    if (isTexture) {
        model_->Draw(worldTransform_, viewProjection);
    }
    else {
        model_->Draw(worldTransform_, viewProjection, textureHandle_);

    }
}
