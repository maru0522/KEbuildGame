#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <cassert>
#include "input.h"
#include "DebugText.h"
#include "Calc.h"

class Object
{
public:
	void Initialize(Model* model, Vector3 pos, Vector3 rot);
	void Initialize(Model* model, uint32_t textureHandle, Vector3 pos, Vector3 rot);
	void Update();
	void Draw(ViewProjection viewProjection, bool isTexture);

	void SetIsRolling(bool isRolling) { isRolling_ = isRolling; }
	void SetIsLeftRoll(bool isLeftRoll) { isLeftRoll_ = isLeftRoll; }
	void SetPos(Vector3 pos) { worldTransform_.translation_ = pos; }
	void SetScale(Vector3 scale) { worldTransform_.scale_ = scale; }
private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	bool isRolling_ = false;
	bool isLeftRoll_= false;

	DebugText* debugText_ = nullptr;
};

