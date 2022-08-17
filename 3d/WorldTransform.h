#pragma once

#include "Vector3.h"
#include "Matrix4.h"
#include <d3d12.h>
#include <wrl.h>

// 定数バッファ用データ構造体
struct ConstBufferDataWorldTransform {
	Matrix4 matWorld;           // ローカル → ワールド変換行列
};

/// <summary>
/// ワールド変換データ
/// </summary>
struct WorldTransform {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataWorldTransform* constMap = nullptr;
	// ローカルスケール
	Vector3 scale_ = {1, 1, 1};
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = {0, 0, 0};
	// ローカル座標
	Vector3 translation_ = {0, 0, 0};
	// ローカル → ワールド変換行列
	Matrix4 matWorld_;
	// 親となるワールド変換へのポインタ
	WorldTransform* parent_ = nullptr;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransferMatrix();

	void SetMatrixScale(Matrix4& matScale_);
	void SetMatrixRotateX(Matrix4& matRotX_);
	void SetMatrixRotateY(Matrix4& matRotY_);
	void SetMatrixRotateZ(Matrix4& matRotZ_);
	void SetMatrixRotate(Matrix4& matRot_, Matrix4 matRotX_, Matrix4 matRotY_, Matrix4 matRotZ_);
	void SetMatrixTranslate(Matrix4& matTrans_);
	void SetMatrixWorldTransform(Matrix4& worldTransform_, Matrix4 matScale_, Matrix4 matRot_, Matrix4 matTrans_);

	void Scale();
	void Rotation();
	void Translation();
	void UpdateMatrix();
};