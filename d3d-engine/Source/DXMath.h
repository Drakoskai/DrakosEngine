#pragma once

#include <DirectXMath.h>

struct Matrix;

#pragma region Class Definitions

#pragma region Vector2

struct  Vector2 : DirectX::XMFLOAT2 {
	Vector2() : XMFLOAT2(0.f, 0.f) {}
	explicit Vector2(float x) : XMFLOAT2(x, x) {}
	Vector2(float x, float y) : XMFLOAT2(x, y) {}
	explicit Vector2(_In_reads_(2) const float *pArray) : XMFLOAT2(pArray) {}
	Vector2(DirectX::FXMVECTOR V) { XMStoreFloat2(this, V); }
	Vector2(const XMFLOAT2& V) { this->x = V.x; this->y = V.y; }
	explicit Vector2(const DirectX::XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; }

	operator DirectX::XMVECTOR() const { return XMLoadFloat2(this); }

	Vector2& operator= (const Vector2& V) { x = V.x; y = V.y; return *this; }
	Vector2& operator= (const DirectX::XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; return *this; }

	static const Vector2 Zero;
	static const Vector2 One;
	static const Vector2 UnitX;
	static const Vector2 UnitY;
};

#pragma endregion Vector2

#pragma region Vector3

struct  Vector3 : DirectX::XMFLOAT3 {
	Vector3() : XMFLOAT3(0.f, 0.f, 0.f) {}
	explicit Vector3(float x) : XMFLOAT3(x, x, x) {}
	Vector3(float x, float y, float z) : XMFLOAT3(x, y, z) {}
	explicit Vector3(_In_reads_(3) const float *pArray) : XMFLOAT3(pArray) {}
	Vector3(DirectX::FXMVECTOR V) { XMStoreFloat3(this, V); }
	Vector3(const XMFLOAT3& V) { this->x = V.x; this->y = V.y; this->z = V.z; }
	explicit Vector3(const DirectX::XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; }

	operator DirectX::XMVECTOR() const { return XMLoadFloat3(this); }

	bool operator == (const Vector3& V) const {
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR v2 = XMLoadFloat3(&V);
		return XMVector3Equal(v1, v2);
	}

	bool operator != (const Vector3& V) const {
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR v2 = XMLoadFloat3(&V);
		return XMVector3NotEqual(v1, v2);
	}

	Vector3& operator= (const Vector3& V) { x = V.x; y = V.y; z = V.z; return *this; }
	Vector3& operator= (const DirectX::XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; z = F.f[2]; return *this; }

	Vector3& operator+= (const Vector3& V);
	Vector3& operator-= (const Vector3& V);
	Vector3& operator*=(const Vector3& V);
	Vector3& operator*= (float S);
	Vector3& operator/= (float S);
	Vector3 operator- () const;

	static void Transform(const Vector3& v, const Matrix& m, Vector3& result);
	static Vector3 Transform(const Vector3& v, const Matrix& m);

	static const Vector3 Zero;
	static const Vector3 One;
	static const Vector3 UnitX;
	static const Vector3 UnitY;
	static const Vector3 UnitZ;
	static const Vector3 Up;
	static const Vector3 Down;
	static const Vector3 Right;
	static const Vector3 Left;
	static const Vector3 Forward;
	static const Vector3 Backward;
};

#pragma endregion Vector3
#pragma region Vector4

struct  Vector4 : DirectX::XMFLOAT4 {
	Vector4() : XMFLOAT4(0.f, 0.f, 0.f, 0.f) {}
	explicit Vector4(float x) : XMFLOAT4(x, x, x, x) {}
	Vector4(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}
	explicit Vector4(_In_reads_(4) const float *pArray) : XMFLOAT4(pArray) {}
	Vector4(DirectX::FXMVECTOR V) { XMStoreFloat4(this, V); }
	Vector4(const XMFLOAT4& V) { this->x = V.x; this->y = V.y; this->z = V.z; this->w = V.w; }
	explicit Vector4(const DirectX::XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

	operator DirectX::XMVECTOR() const { return XMLoadFloat4(this); }

	Vector4& operator= (const Vector4& V) { x = V.x; y = V.y; z = V.z; w = V.w; return *this; }
	Vector4& operator= (const DirectX::XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }

	static const Vector4 Zero;
	static const Vector4 One;
	static const Vector4 UnitX;
	static const Vector4 UnitY;
	static const Vector4 UnitZ;
	static const Vector4 UnitW;
};

#pragma endregion Vector4

#pragma region Color

struct Color : DirectX::XMFLOAT4 {
	Color() : XMFLOAT4(0, 0, 0, 1.f) {}
	Color(float r, float g, float b) : XMFLOAT4(r, g, b, 1.f) {}
	Color(float r, float g, float b, float a) : XMFLOAT4(r, g, b, a) {}
	explicit Color(const Vector3& clr) : XMFLOAT4(clr.x, clr.y, clr.z, 1.f) {}
	explicit Color(const Vector4& clr) : XMFLOAT4(clr.x, clr.y, clr.z, clr.w) {}
	explicit Color(_In_reads_(4) const float *pArray) : XMFLOAT4(pArray) {}
	Color(DirectX::FXMVECTOR V) { XMStoreFloat4(this, V); }
	Color(const XMFLOAT4& c) { this->x = c.x; this->y = c.y; this->z = c.z; this->w = c.w; }
	explicit Color(const DirectX::XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

	operator DirectX::XMVECTOR() const { return XMLoadFloat4(this); }
	operator const float*() const { return reinterpret_cast<const float*>(this); }

	Color& operator= (const Color& c) { x = c.x; y = c.y; z = c.z; w = c.w; return *this; }
	//Color& operator= (const XMFLOAT4& c) { x = c.x; y = c.y; z = c.z; w = c.w; return *this; }
	Color& operator= (const DirectX::XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
};

#pragma endregion Color

#pragma region Matrix

struct Matrix : DirectX::XMFLOAT4X4 {
	Matrix();

	Matrix(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	explicit Matrix(const Vector3& r0, const Vector3& r1, const Vector3& r2);

	explicit Matrix(const Vector4& r0, const Vector4& r1, const Vector4& r2, const Vector4& r3)
		: XMFLOAT4X4(
			r0.x, r0.y, r0.z, r0.w,
			r1.x, r1.y, r1.z, r1.w,
			r2.x, r2.y, r2.z, r2.w,
			r3.x, r3.y, r3.z, r3.w) {}

	Matrix(const XMFLOAT4X4& M) { memcpy_s(this, sizeof(float) * 16, &M, sizeof(XMFLOAT4X4)); }

	Matrix(const DirectX::XMFLOAT3X3& M);

	Matrix(const DirectX::XMFLOAT4X3& M);

	explicit Matrix(_In_reads_(16) const float *pArray) : XMFLOAT4X4(pArray) {}

	Matrix(DirectX::CXMMATRIX M) { XMStoreFloat4x4(this, M); }

	operator DirectX::XMMATRIX() const { return XMLoadFloat4x4(this); }

	Matrix& operator= (const Matrix& M) { memcpy_s(this, sizeof(float) * 16, &M, sizeof(float) * 16); return *this; }

	Matrix& operator=(const DirectX::XMFLOAT3X3& M);

	Matrix& operator+=(const Matrix& M);

	Matrix& operator-=(const Matrix& M);

	Matrix& operator*=(const Matrix& M);

	Matrix& operator*=(float S);

	Matrix& operator/=(float S);

	Matrix& operator/=(const Matrix& M);

	Matrix Transpose() const;

	void Transpose(Matrix& result) const;

	static Matrix CreateFromYawPitchRoll(float yaw, float pitch, float roll);

	static Matrix CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up);
};

#pragma endregion Matrix

#pragma endregion Class Definitions

#pragma region Class Implementation

#pragma region Vector3

inline Vector3& Vector3::operator+= (const Vector3& V) {
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&V);
	XMVECTOR X = XMVectorAdd(v1, v2);
	XMStoreFloat3(this, X);
	return *this;
}

inline Vector3& Vector3::operator-= (const Vector3& V) {
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&V);
	XMVECTOR X = XMVectorSubtract(v1, v2);
	XMStoreFloat3(this, X);
	return *this;
}

inline Vector3& Vector3::operator*=(const Vector3& V) {
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&V);
	XMVECTOR X = XMVectorMultiply(v1, v2);
	XMStoreFloat3(this, X);
	return *this;
}

inline Vector3& Vector3::operator*= (float S) {
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR X = XMVectorScale(v1, S);
	XMStoreFloat3(this, X);
	return *this;
}

inline Vector3& Vector3::operator/= (float S) {
	using namespace DirectX;
	assert(S != 0.0f);
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR X = XMVectorScale(v1, 1.f / S);
	XMStoreFloat3(this, X);
	return *this;
}

inline Vector3 Vector3::operator- () const {
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR X = XMVectorNegate(v1);
	Vector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

inline Vector3 operator+ (const Vector3& V1, const Vector3& V2) {
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&V1);
	XMVECTOR v2 = XMLoadFloat3(&V2);
	XMVECTOR X = XMVectorAdd(v1, v2);
	Vector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

inline Vector3 operator- (const Vector3& V1, const Vector3& V2) {
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&V1);
	XMVECTOR v2 = XMLoadFloat3(&V2);
	XMVECTOR X = XMVectorSubtract(v1, v2);
	Vector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

inline Vector3 operator* (const Vector3& V1, const Vector3& V2) {
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&V1);
	XMVECTOR v2 = XMLoadFloat3(&V2);
	XMVECTOR X = XMVectorMultiply(v1, v2);
	Vector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

inline Vector3 operator* (const Vector3& V, float S) {
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&V);
	XMVECTOR X = XMVectorScale(v1, S);
	Vector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

inline Vector3 operator/ (const Vector3& V1, const Vector3& V2) {
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&V1);
	XMVECTOR v2 = XMLoadFloat3(&V2);
	XMVECTOR X = XMVectorDivide(v1, v2);
	Vector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

inline Vector3 operator* (float S, const Vector3& V) {
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&V);
	XMVECTOR X = XMVectorScale(v1, S);
	Vector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

inline void Vector3::Transform(const Vector3& v, const Matrix& m, Vector3& result) {
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&v);
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVECTOR X = XMVector3TransformCoord(v1, M);
	XMStoreFloat3(&result, X);
}

inline Vector3 Vector3::Transform(const Vector3& v, const Matrix& m) {
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&v);
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVECTOR X = XMVector3TransformCoord(v1, M);

	Vector3 result;
	XMStoreFloat3(&result, X);
	return result;
}

#pragma endregion Vector3

#pragma region Matrix

inline Matrix::Matrix()
	: XMFLOAT4X4(
		1.f, 0, 0, 0,
		0, 1.f, 0, 0,
		0, 0, 1.f, 0,
		0, 0, 0, 1.f) {}

inline Matrix::Matrix(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)
	: XMFLOAT4X4(
		m00, m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23,
		m30, m31, m32, m33) {}

inline Matrix::Matrix(const Vector3& r0, const Vector3& r1, const Vector3& r2) : XMFLOAT4X4(
	r0.x, r0.y, r0.z, 0,
	r1.x, r1.y, r1.z, 0,
	r2.x, r2.y, r2.z, 0,
	0, 0, 0, 1.f) {}

inline Matrix& Matrix::operator=(const DirectX::XMFLOAT3X3& M) {
	_11 = M._11;
	_12 = M._12;
	_13 = M._13;
	_14 = 0.f;
	_21 = M._21;
	_22 = M._22;
	_23 = M._23;
	_24 = 0.f;
	_31 = M._31;
	_32 = M._32;
	_33 = M._33;
	_34 = 0.f;
	_41 = 0.f;
	_42 = 0.f;
	_43 = 0.f;
	_44 = 1.f;
	return *this;
}

inline Matrix& Matrix::operator+=(const Matrix& M) {
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_41));

	XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
	XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
	XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
	XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

	x1 = XMVectorAdd(x1, y1);
	x2 = XMVectorAdd(x2, y2);
	x3 = XMVectorAdd(x3, y3);
	x4 = XMVectorAdd(x4, y4);

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
	return *this;
}

inline Matrix& Matrix::operator-=(const Matrix& M) {
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_41));

	XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
	XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
	XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
	XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

	x1 = XMVectorSubtract(x1, y1);
	x2 = XMVectorSubtract(x2, y2);
	x3 = XMVectorSubtract(x3, y3);
	x4 = XMVectorSubtract(x4, y4);

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
	return *this;
}

inline Matrix& Matrix::operator*=(const Matrix& M) {
	using namespace DirectX;
	XMMATRIX M1 = XMLoadFloat4x4(this);
	XMMATRIX M2 = XMLoadFloat4x4(&M);
	XMMATRIX X = XMMatrixMultiply(M1, M2);
	XMStoreFloat4x4(this, X);
	return *this;
}

inline Matrix& Matrix::operator*=(float S) {
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_41));

	x1 = XMVectorScale(x1, S);
	x2 = XMVectorScale(x2, S);
	x3 = XMVectorScale(x3, S);
	x4 = XMVectorScale(x4, S);

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
	return *this;
}

inline Matrix& Matrix::operator/=(float S) {
	using namespace DirectX;
	assert(S != 0.f);
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_41));

	float rs = 1.f / S;

	x1 = XMVectorScale(x1, rs);
	x2 = XMVectorScale(x2, rs);
	x3 = XMVectorScale(x3, rs);
	x4 = XMVectorScale(x4, rs);

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
	return *this;
}

inline Matrix& Matrix::operator/=(const Matrix& M) {
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<XMFLOAT4*>(&_41));

	XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
	XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
	XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
	XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

	x1 = XMVectorDivide(x1, y1);
	x2 = XMVectorDivide(x2, y2);
	x3 = XMVectorDivide(x3, y3);
	x4 = XMVectorDivide(x4, y4);

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
	return *this;
}

inline Matrix Matrix::Transpose() const {
	using namespace DirectX;
	XMMATRIX M = XMLoadFloat4x4(this);
	Matrix R;
	XMStoreFloat4x4(&R, XMMatrixTranspose(M));
	return R;
}

inline void Matrix::Transpose(Matrix& result) const {
	using namespace DirectX;
	XMMATRIX M = XMLoadFloat4x4(this);
	XMStoreFloat4x4(&result, XMMatrixTranspose(M));
}

inline Matrix Matrix::CreateFromYawPitchRoll(float yaw, float pitch, float roll) {
	using namespace DirectX;
	Matrix R;
	XMStoreFloat4x4(&R, XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
	return R;
}

inline Matrix Matrix::CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
	using namespace DirectX;
	Matrix R;
	XMVECTOR eyev = XMLoadFloat3(&eye);
	XMVECTOR targetv = XMLoadFloat3(&target);
	XMVECTOR upv = XMLoadFloat3(&up);
	XMStoreFloat4x4(&R, XMMatrixLookAtLH(eyev, targetv, upv));
	return R;
}

#pragma endregion Matrix

#pragma endregion ClassImplementation