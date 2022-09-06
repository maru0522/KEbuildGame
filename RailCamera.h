#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"
#include "DebugText.h"

class RailCamera
{
public:
    void Initialize(Vector3 pos,Vector3 rad);
    void Update();

    ViewProjection GetViewProjection() { return viewProjection_; }
    WorldTransform* GetWorldTransform() { return &worldTransform_; }

    // ���[���h���W���擾
    Vector3 GetWorldPosition();
private:
    // ���[���h�ϊ��f�[�^
    WorldTransform worldTransform_;
    // �r���[�v���W�F�N�V����
    ViewProjection viewProjection_;

    Input* input_ = nullptr;
    DebugText* debugText_ = nullptr;
};