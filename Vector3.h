#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct Vector3 : public XMFLOAT3 {
	Vector3() = default;
	Vector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Vector3(XMFLOAT3 num) {
		this->x = num.x;
		this->y = num.y;
		this->z = num.z;
	}
	Vector3(const XMVECTOR &other) :XMFLOAT3() {
		XMVECTOR temp = other;
		XMStoreFloat3(this, temp);
	}

	//演算
	inline bool operator == (const Vector3 &r) const { return x == r.x && y == r.y && z == r.z; }
	inline bool operator != (const Vector3 &r) const { return x != r.x || y != r.y || z != r.z; }
	inline XMVECTOR operator *(const float r) const { return Vector3(x * r, y * r, z * r); }
	inline XMVECTOR operator /(const float r) const { return Vector3(x / r, y / r, z / r); }

	//二項演算
	Vector3 operator+=(const Vector3 &r) {
		*this = *this + r;
		return *this;
	}
	Vector3 operator-=(const Vector3 &r) {
		*this = *this + -r;
		return *this;
	}

	// ベクトルの内積
	float VDot(Vector3 In) { return x * In.x + y * In.y + z * In.z; }
	// ベクトルの外積
	Vector3 VCross(Vector3 In) { return Vector3(y * In.z - z * In.y, z * In.x - x * In.z, x * In.y - y * In.x); }
	// ベクトルのスケーリング
	Vector3 VScale(float Scale) { Vector3 Result = { x * Scale, y * Scale, z * Scale }; return Result; }

	//代入
	Vector3 &operator=(const XMVECTOR &other) {
		XMVECTOR temp = other;
		XMStoreFloat3(this, temp);
		return *this;
	}
	//キャスト
	operator XMVECTOR() const {
		return XMLoadFloat3(this);
	}
	//長さ
	float Length() const {
		return (static_cast<Vector3>(XMVector3Length(XMVECTOR(*this)))).x;
	}

	//ベクトルの長さを求める
	float length() const
	{
		return (float)sqrtf(x * x + y * y + z * z);
	}

	//長さ二乗
	float LengthSq() const {
		return pow(this->Length(),2);
	}
	//正規化
	void Normalize() {
		*this = XMVector3Normalize(XMVECTOR(*this));
	}

	void RotationX(float angle) {
	
	}
	void AddRotationY(float angle) {
		*this = {
			this->x * cosf(angle)	+0			+this->z * sinf(angle),
			0						+this->y	+0,	
			this->x * -sinf(angle)	+0			+this->z * cosf(angle)
		};
	}
	void RotationZ(float angle) {

	}
};
