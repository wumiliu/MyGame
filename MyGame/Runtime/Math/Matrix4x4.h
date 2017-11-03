#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include "Vector3.h"
#include "Vector4.h"


class Matrix3x3f;
class Matrix4x4f;
class Quaternionf;


/// Uniform transform scales x, y, z in the same amount,
/// NonUniform transform scales x, y, z differently and might contain skew.
/// kOddNegativeScaleTransform means that FrontFace(CCW) should be used (An odd number of scale axes is negative)
enum TransformType
{
	kNoScaleTransform = 0,
	kUniformScaleTransform = 1 << 0,
	kNonUniformScaleTransform = 1 << 1,
	kOddNegativeScaleTransform = 1 << 2
};
ENUM_FLAGS(TransformType);

bool InvertMatrix4x4_Full(const float* m, float* out);
bool InvertMatrix4x4_General3D(const float* m, float* out);

//列向量，内部存储的还是行优先的
//矩阵的变换 要从右往左看
/// Matrices in unity are column major.
class  Matrix4x4f
{
public:
	float m_Data[16];


	Matrix4x4f() {}
	Matrix4x4f(const Matrix3x3f &other);
	explicit Matrix4x4f(const float data[16]);

	float& Get(int row, int column) { return m_Data[row + (column * 4)]; }
	const float& Get(int row, int column)const { return m_Data[row + (column * 4)]; }
	float* GetPtr() { return m_Data; }
	const float* GetPtr()const { return m_Data; }

	float operator [] (int index) const { return m_Data[index]; }
	float& operator [] (int index) { return m_Data[index]; }



	Matrix4x4f& operator *= (const Matrix4x4f& inM);

	Matrix4x4f& operator = (const Matrix3x3f& m);

	Vector3f MultiplyVector3(const Vector3f& inV) const;
	void MultiplyVector3(const Vector3f& inV, Vector3f& output) const;
	bool PerspectiveMultiplyVector3(const Vector3f& inV, Vector3f& output) const;
	Vector3f MultiplyPoint3(const Vector3f& inV) const;
	void MultiplyPoint3(const Vector3f& inV, Vector3f& output) const;
	bool PerspectiveMultiplyPoint3(const Vector3f& inV, Vector3f& output) const;
	Vector3f InverseMultiplyPoint3Affine(const Vector3f& inV) const;
	Vector3f InverseMultiplyVector3Affine(const Vector3f& inV) const;

	bool IsIdentity(float epsilon = Vector3f::epsilon) const;

	double GetDeterminant() const;

	Matrix4x4f& Invert_Full() {
		InvertMatrix4x4_Full(m_Data, m_Data);
		return *this;
	}
	static bool Invert_Full(const Matrix4x4f &inM, Matrix4x4f &outM) {
		return InvertMatrix4x4_Full(inM.m_Data, outM.m_Data);
	}
	static bool Invert_General3D(const Matrix4x4f &inM, Matrix4x4f &outM) {
		return InvertMatrix4x4_General3D(inM.m_Data, outM.m_Data);
	}

	Matrix4x4f& Transpose();

	Matrix4x4f& Copy(const Matrix4x4f& inM);

	Matrix4x4f& SetIdentity();
	Matrix4x4f& SetPerspective(float fovy, float aspect, float zNear, float zFar);
	// rad = Deg2Rad(fovy/2), contanHalfFOV = cos(rad)/sin(rad)
	Matrix4x4f& SetPerspectiveCotan(float cotanHalfFOV, float zNear, float zFar);
	Matrix4x4f& SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	Matrix4x4f& SetFrustum(float left, float right, float bottom, float top, float nearval, float farval);

	Vector3f GetAxisX() const;
	Vector3f GetAxisY() const;
	Vector3f GetAxisZ() const;
	Vector3f GetPosition() const;
	Vector4f GetRow(int row) const;
	Vector4f GetColumn(int col) const;
	// these set only these components of the matrix, everything else is untouched!
	void SetAxisX(const Vector3f& v);
	void SetAxisY(const Vector3f& v);
	void SetAxisZ(const Vector3f& v);
	void SetPosition(const Vector3f& v);
	void SetRow(int row, const Vector4f& v);
	void SetColumn(int col, const Vector4f& v);

	Matrix4x4f& SetTranslate(const Vector3f& inTrans);
	Matrix4x4f& SetOrthoNormalBasis(const Vector3f& inX, const Vector3f& inY, const Vector3f& inZ);
	Matrix4x4f& SetOrthoNormalBasisInverse(const Vector3f& inX, const Vector3f& inY, const Vector3f& inZ);
	Matrix4x4f& SetScale(const Vector3f& inScale);
	Matrix4x4f& SetPositionAndOrthoNormalBasis(const Vector3f& inPosition, const Vector3f& inX, const Vector3f& inY, const Vector3f& inZ);

	Matrix4x4f& Translate(const Vector3f& inTrans);
	Matrix4x4f& Scale(const Vector3f& inScale);

	Matrix4x4f& SetFromToRotation(const Vector3f& from, const Vector3f& to);

	void SetTR(const Vector3f& pos, const Quaternionf& q);
	void SetTRS(const Vector3f& pos, const Quaternionf& q, const Vector3f& s);
	void SetTRInverse(const Vector3f& pos, const Quaternionf& q);

	static const Matrix4x4f identity;
};


// Binary operators
Matrix4x4f operator* (const Matrix4x4f& M1, const Matrix4x4f& M2);



bool CompareApproximately(const Matrix4x4f& lhs, const Matrix4x4f& rhs, float dist = Vector3f::epsilon);

/// Transforms an array of vertices. input may be the same as output.
void  TransformPoints3x3(const Matrix4x4f &matrix, const Vector3f* input, Vector3f* ouput, int count);
void  TransformPoints3x4(const Matrix4x4f &matrix, const Vector3f* input, Vector3f* ouput, int count);
void  TransformPoints3x3(const Matrix4x4f &matrix, const Vector3f* input, size_t inStride, Vector3f* ouput, size_t outStride, int count);
void  TransformPoints3x4(const Matrix4x4f &matrix, const Vector3f* input, size_t inStride, Vector3f* ouput, size_t outStride, int count);

void MultiplyMatrices3x4(const Matrix4x4f& lhs, const Matrix4x4f& rhs, Matrix4x4f& res);

void MultiplyMatrices4x4REF(const Matrix4x4f* __restrict lhs, const Matrix4x4f* __restrict rhs, Matrix4x4f* __restrict res);
void CopyMatrixREF(const float* __restrict lhs, float* __restrict res);
void TransposeMatrix4x4REF(const Matrix4x4f* __restrict lhs, Matrix4x4f* __restrict res);

// foreach R[i] = A[i] * B[i]
void MultiplyMatrixArray4x4REF(const Matrix4x4f* __restrict arrayA, const Matrix4x4f* __restrict arrayB,
	Matrix4x4f* __restrict arrayRes, size_t count);
// foreach R[i] = BASE * A[i] * B[i]
void MultiplyMatrixArrayWithBase4x4REF(const Matrix4x4f* __restrict base,
	const Matrix4x4f* __restrict arrayA, const Matrix4x4f* __restrict arrayB,
	Matrix4x4f* __restrict arrayRes, size_t count);


#define CopyMatrix						CopyMatrixREF
#define TransposeMatrix4x4				TransposeMatrix4x4REF

#define MultiplyMatrices4x4				MultiplyMatrices4x4REF
#define MultiplyMatrixArray4x4			MultiplyMatrixArray4x4REF
#define MultiplyMatrixArrayWithBase4x4	MultiplyMatrixArrayWithBase4x4REF


inline Matrix4x4f operator* (const Matrix4x4f& inM1, const Matrix4x4f& inM2)
{
	Matrix4x4f tmp;
	MultiplyMatrices4x4(&inM1, &inM2, &tmp);
	return tmp;
}


inline Vector3f Matrix4x4f::GetAxisX() const {
	return Vector3f(Get(0, 0), Get(1, 0), Get(2, 0));
}
inline Vector3f Matrix4x4f::GetAxisY() const {
	return Vector3f(Get(0, 1), Get(1, 1), Get(2, 1));
}
inline Vector3f Matrix4x4f::GetAxisZ() const {
	return Vector3f(Get(0, 2), Get(1, 2), Get(2, 2));
}
inline Vector3f Matrix4x4f::GetPosition() const {
	return Vector3f(Get(0, 3), Get(1, 3), Get(2, 3));
}
inline Vector4f Matrix4x4f::GetRow(int row) const {
	return Vector4f(Get(row, 0), Get(row, 1), Get(row, 2), Get(row, 3));
}
inline Vector4f Matrix4x4f::GetColumn(int col) const {
	return Vector4f(Get(0, col), Get(1, col), Get(2, col), Get(3, col));
}
inline void Matrix4x4f::SetAxisX(const Vector3f& v) {
	Get(0, 0) = v.x; Get(1, 0) = v.y; Get(2, 0) = v.z;
}
inline void Matrix4x4f::SetAxisY(const Vector3f& v) {
	Get(0, 1) = v.x; Get(1, 1) = v.y; Get(2, 1) = v.z;
}
inline void Matrix4x4f::SetAxisZ(const Vector3f& v) {
	Get(0, 2) = v.x; Get(1, 2) = v.y; Get(2, 2) = v.z;
}
inline void Matrix4x4f::SetPosition(const Vector3f& v) {
	Get(0, 3) = v.x; Get(1, 3) = v.y; Get(2, 3) = v.z;
}
inline void Matrix4x4f::SetRow(int row, const Vector4f& v) {
	Get(row, 0) = v.x; Get(row, 1) = v.y; Get(row, 2) = v.z; Get(row, 3) = v.w;
}
inline void Matrix4x4f::SetColumn(int col, const Vector4f& v) {
	Get(0, col) = v.x; Get(1, col) = v.y; Get(2, col) = v.z; Get(3, col) = v.w;
}


inline Vector3f Matrix4x4f::MultiplyPoint3(const Vector3f& v) const
{
	Vector3f res;
	res.x = m_Data[0] * v.x + m_Data[4] * v.y + m_Data[8] * v.z + m_Data[12];
	res.y = m_Data[1] * v.x + m_Data[5] * v.y + m_Data[9] * v.z + m_Data[13];
	res.z = m_Data[2] * v.x + m_Data[6] * v.y + m_Data[10] * v.z + m_Data[14];
	return res;
}

inline void Matrix4x4f::MultiplyPoint3(const Vector3f& v, Vector3f& output) const
{
	output.x = m_Data[0] * v.x + m_Data[4] * v.y + m_Data[8] * v.z + m_Data[12];
	output.y = m_Data[1] * v.x + m_Data[5] * v.y + m_Data[9] * v.z + m_Data[13];
	output.z = m_Data[2] * v.x + m_Data[6] * v.y + m_Data[10] * v.z + m_Data[14];
}


inline Vector3f Matrix4x4f::MultiplyVector3(const Vector3f& v) const
{
	Vector3f res;
	res.x = m_Data[0] * v.x + m_Data[4] * v.y + m_Data[8] * v.z;
	res.y = m_Data[1] * v.x + m_Data[5] * v.y + m_Data[9] * v.z;
	res.z = m_Data[2] * v.x + m_Data[6] * v.y + m_Data[10] * v.z;
	return res;
}

inline void Matrix4x4f::MultiplyVector3(const Vector3f& v, Vector3f& output) const
{
	output.x = m_Data[0] * v.x + m_Data[4] * v.y + m_Data[8] * v.z;
	output.y = m_Data[1] * v.x + m_Data[5] * v.y + m_Data[9] * v.z;
	output.z = m_Data[2] * v.x + m_Data[6] * v.y + m_Data[10] * v.z;
}


inline bool Matrix4x4f::PerspectiveMultiplyPoint3(const Vector3f& v, Vector3f& output) const
{
	Vector3f res;
	float w;
	res.x = Get(0, 0) * v.x + Get(0, 1) * v.y + Get(0, 2) * v.z + Get(0, 3);
	res.y = Get(1, 0) * v.x + Get(1, 1) * v.y + Get(1, 2) * v.z + Get(1, 3);
	res.z = Get(2, 0) * v.x + Get(2, 1) * v.y + Get(2, 2) * v.z + Get(2, 3);
	w = Get(3, 0) * v.x + Get(3, 1) * v.y + Get(3, 2) * v.z + Get(3, 3);
	if (Abs(w) > 1.0e-7f)
	{
		float invW = 1.0f / w;
		output.x = res.x * invW;
		output.y = res.y * invW;
		output.z = res.z * invW;
		return true;
	}
	else
	{
		output.x = 0.0f;
		output.y = 0.0f;
		output.z = 0.0f;
		return false;
	}
}

inline bool Matrix4x4f::PerspectiveMultiplyVector3(const Vector3f& v, Vector3f& output) const
{
	Vector3f res;
	float w;
	res.x = Get(0, 0) * v.x + Get(0, 1) * v.y + Get(0, 2) * v.z;
	res.y = Get(1, 0) * v.x + Get(1, 1) * v.y + Get(1, 2) * v.z;
	res.z = Get(2, 0) * v.x + Get(2, 1) * v.y + Get(2, 2) * v.z;
	w = Get(3, 0) * v.x + Get(3, 1) * v.y + Get(3, 2) * v.z;
	if (Abs(w) > 1.0e-7f)
	{
		float invW = 1.0f / w;
		output.x = res.x * invW;
		output.y = res.y * invW;
		output.z = res.z * invW;
		return true;
	}
	else
	{
		output.x = 0.0f;
		output.y = 0.0f;
		output.z = 0.0f;
		return false;
	}
}

inline Vector3f Matrix4x4f::InverseMultiplyPoint3Affine(const Vector3f& inV) const
{
	Vector3f v(inV.x - Get(0, 3), inV.y - Get(1, 3), inV.z - Get(2, 3));
	Vector3f res;
	res.x = Get(0, 0) * v.x + Get(1, 0) * v.y + Get(2, 0) * v.z;
	res.y = Get(0, 1) * v.x + Get(1, 1) * v.y + Get(2, 1) * v.z;
	res.z = Get(0, 2) * v.x + Get(1, 2) * v.y + Get(2, 2) * v.z;
	return res;
}

inline Vector3f Matrix4x4f::InverseMultiplyVector3Affine(const Vector3f& v) const
{
	Vector3f res;
	res.x = Get(0, 0) * v.x + Get(1, 0) * v.y + Get(2, 0) * v.z;
	res.y = Get(0, 1) * v.x + Get(1, 1) * v.y + Get(2, 1) * v.z;
	res.z = Get(0, 2) * v.x + Get(1, 2) * v.y + Get(2, 2) * v.z;
	return res;
}

inline bool IsFinite(const Matrix4x4f& f)
{
	return
		IsFinite(f.m_Data[0]) & IsFinite(f.m_Data[1]) & IsFinite(f.m_Data[2]) &
		IsFinite(f.m_Data[4]) & IsFinite(f.m_Data[5]) & IsFinite(f.m_Data[6]) &
		IsFinite(f.m_Data[8]) & IsFinite(f.m_Data[9]) & IsFinite(f.m_Data[10]) &
		IsFinite(f.m_Data[12]) & IsFinite(f.m_Data[13]) & IsFinite(f.m_Data[14]) & IsFinite(f.m_Data[15]);
}

#endif
