//-------------------------------------------------------------------------------------
// LTMatrix3x4.h
//
// This class represents a 3x4 matrix transform. Such transforms are very common for 
// representing linear transformations, but the 3x4 approach is more compact memory wise
// and also allows for fewer computations to transform. However, in order to make this
// a useful primitive, it needs to support multiplication with other matrices and also
// transforming 4 value vectors, and therefore this really acts like a 4x4 matrix with
// the last row being implicitly {0, 0, 0, 1}. This allows it to still get the advantage
// of the small size and efficient calculations without compromising flexibility and
// interoperability.
//
//-------------------------------------------------------------------------------------
#ifndef __LTMATRIX3X4_H__
#define __LTMATRIX3X4_H__

#ifndef __LTVECTOR_H__
#include "ltvector.h"
#endif

class LTMatrix3x4
{
public:

	//------------------------------
	// Initialization

	//default constructor does not initialize to avoid performance overhead
	LTMatrix3x4()
	{
	}

	LTMatrix3x4(float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23)
	{
		Init(m00, m01, m02, m03,
			 m10, m11, m12, m13,
			 m20, m21, m22, m23);
	}

	//allows for explicit initialization of all of the matrix components
	void Init (	float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23)
	{
		m[0][0] = m00;  m[0][1] = m01;  m[0][2] = m02;  m[0][3] = m03;
		m[1][0] = m10;  m[1][1] = m11;  m[1][2] = m12;  m[1][3] = m13;
		m[2][0] = m20;  m[2][1] = m21;  m[2][2] = m22;  m[2][3] = m23;
	}

	//------------------------------
	// Identity matrix

	//gets the identity value for this primitive
	static LTMatrix3x4 GetIdentity() 
	{ 
		return LTMatrix3x4(	1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f);
	}

	//initializes this matrix to the identity matrix
	void Init() 
	{
		Init(1.0f, 0.0f, 0.0f, 0.0f,
			 0.0f, 1.0f, 0.0f, 0.0f,
			 0.0f, 0.0f, 1.0f, 0.0f);
	}

	//------------------------------
	// Basis vectors

	//called to extract out the basis vectors from this matrix
	void GetBasisVectors(LTVector& vX, LTVector& vY, LTVector& vZ) const
	{
		vX.Init(m[0][0], m[1][0], m[2][0]);
		vY.Init(m[0][1], m[1][1], m[2][1]);
		vZ.Init(m[0][2], m[1][2], m[2][2]);
	}

	//called to extract out the basis vectors from this matrix
	void SetBasisVectors(const LTVector& vX, const LTVector& vY, const LTVector& vZ)
	{
		m[0][0] = vX.x; m[1][0] = vX.y; m[2][0] = vX.z;
		m[0][1] = vY.x; m[1][1] = vY.y; m[2][1] = vY.z;
		m[0][2] = vZ.x; m[1][2] = vZ.y; m[2][2] = vZ.z;
	}

	//------------------------------
	// Basis vector scale

	//gets the scale of the x, y, and z basis vectors
	LTVector GetScale() const
	{
		return LTVector(
			LTSqrt(m[0][0]*m[0][0] + m[1][0]*m[1][0] + m[2][0]*m[2][0]),
			LTSqrt(m[0][1]*m[0][1] + m[1][1]*m[1][1] + m[2][1]*m[2][1]),
			LTSqrt(m[0][2]*m[0][2] + m[1][2]*m[1][2] + m[2][2]*m[2][2])
			);
	}

	//scales all three of the vectors by the provided values
	void Scale(const LTVector& vScales)
	{
		m[0][0] *= vScales.x;
		m[1][0] *= vScales.x;
		m[2][0] *= vScales.x;

		m[0][1] *= vScales.y;
		m[1][1] *= vScales.y;
		m[2][1] *= vScales.y;

		m[0][2] *= vScales.z;
		m[1][2] *= vScales.z;
		m[2][2] *= vScales.z;
	}

	//called to normalize the basis vectors of this matrix
	void Normalize()
	{
		LTVector vScale = GetScale();
		Scale(LTVector(1.0f / vScale.x, 1.0f / vScale.y, 1.0f / vScale.z));
	}

	//------------------------------
	// Translation

	//called to get the translation for this matrix
	void GetTranslation(LTVector &t) const
	{
		t.Init(m[0][3], m[1][3], m[2][3]);
	}

	//called to set the translation on this matrix
	void SetTranslation(const LTVector& t)
	{
		m[0][3] = t.x;
		m[1][3] = t.y;
		m[2][3] = t.z;
	}

	//called to transpose the upper left 3x3 portion of this matrix
	void Transpose3x3()
	{
		float temp;
		float *d = (float*)m;

		temp = d[1]; d[1] = d[4]; d[4] = temp;
		temp = d[2]; d[2] = d[8]; d[8] = temp;
		temp = d[9]; d[9] = d[6]; d[6] = temp;
	}

	//------------------------------
	// LTVector 3 Transform

	//called to transform 3 component vectors by the full matrix
	void Transform(const LTVector& vVec, LTVector& vResult) const
	{
		vResult.x = vVec.x * m[0][0] + vVec.y * m[0][1] + vVec.z * m[0][2] + m[0][3];
		vResult.y = vVec.x * m[1][0] + vVec.y * m[1][1] + vVec.z * m[1][2] + m[1][3];
		vResult.z = vVec.x * m[2][0] + vVec.y * m[2][1] + vVec.z * m[2][2] + m[2][3];
	}

	LTVector Transform(const LTVector& vVec) const
	{
		LTVector vResult;
		Transform(vVec, vResult);
		return vResult;
	}

	LTVector operator*(const LTVector& vVec) const
	{
		return Transform(vVec);
	}

	//------------------------------
	// LTVector 3 3x3 Transform

	//called to transform 3 component vectors by just the 3x3 component of the matrix
	void Transform3x3(const LTVector& vVec, LTVector& vResult) const
	{
		vResult.x = vVec.x * m[0][0] + vVec.y * m[0][1] + vVec.z * m[0][2];
		vResult.y = vVec.x * m[1][0] + vVec.y * m[1][1] + vVec.z * m[1][2];
		vResult.z = vVec.x * m[2][0] + vVec.y * m[2][1] + vVec.z * m[2][2];
	}

	LTVector Transform3x3(const LTVector& vVec) const
	{
		LTVector vResult;
		Transform3x3(vVec, vResult);
		return vResult;
	}

	//------------------------------
	// LTVector 4 Transform

	//called to transform 4 component vectors by the full matrix
	void Transform(const LTVector4& vVec, LTVector4& vResult) const
	{
		vResult.x = vVec.x * m[0][0] + vVec.y * m[0][1] + vVec.z * m[0][2] + vVec.w * m[0][3];
		vResult.y = vVec.x * m[1][0] + vVec.y * m[1][1] + vVec.z * m[1][2] + vVec.w * m[1][3];
		vResult.z = vVec.x * m[2][0] + vVec.y * m[2][1] + vVec.z * m[2][2] + vVec.w * m[2][3];
		vResult.w = vVec.w;
	}

	LTVector4 Transform(const LTVector4& vVec) const
	{
		LTVector4 vResult;
		Transform(vVec, vResult);
		return vResult;
	}

	LTVector4 operator*(const LTVector4& vVec) const
	{
		return Transform(vVec);
	}

	//------------------------------
	// LTVector 4 3x3 Transform

	//called to transform 4 component vectors by the full matrix
	void Transform3x3(const LTVector4& vVec, LTVector4& vResult) const
	{
		vResult.x = vVec.x * m[0][0] + vVec.y * m[0][1] + vVec.z * m[0][2];
		vResult.y = vVec.x * m[1][0] + vVec.y * m[1][1] + vVec.z * m[1][2];
		vResult.z = vVec.x * m[2][0] + vVec.y * m[2][1] + vVec.z * m[2][2];
		vResult.w = 0.0f;
	}

	LTVector4 Transform3x3(const LTVector4& vVec) const
	{
		LTVector4 vResult;
		Transform3x3(vVec, vResult);
		return vResult;
	}

	//------------------------------
	// Matrix multiplication

	void Multiply(const LTMatrix3x4& mInput, LTMatrix3x4& mOutput) const
	{
		//for both of these matrices, the last row is {0, 0, 0, 1} so we can optimize away
		//a lot of multiplications and additions (29 muls, 21 adds)
		mOutput.m[0][0] = m[0][0] * mInput.m[0][0] + m[0][1] * mInput.m[1][0] + m[0][2] * mInput.m[2][0];
		mOutput.m[0][1] = m[0][0] * mInput.m[0][1] + m[0][1] * mInput.m[1][1] + m[0][2] * mInput.m[2][1];
		mOutput.m[0][2] = m[0][0] * mInput.m[0][2] + m[0][1] * mInput.m[1][2] + m[0][2] * mInput.m[2][2];
		mOutput.m[0][3] = m[0][0] * mInput.m[0][3] + m[0][1] * mInput.m[1][3] + m[0][2] * mInput.m[2][3] + m[0][3];

		mOutput.m[1][0] = m[1][0] * mInput.m[0][0] + m[1][1] * mInput.m[1][0] + m[1][2] * mInput.m[2][0];
		mOutput.m[1][1] = m[1][0] * mInput.m[0][1] + m[1][1] * mInput.m[1][1] + m[1][2] * mInput.m[2][1];
		mOutput.m[1][2] = m[1][0] * mInput.m[0][2] + m[1][1] * mInput.m[1][2] + m[1][2] * mInput.m[2][2];
		mOutput.m[1][3] = m[1][0] * mInput.m[0][3] + m[1][1] * mInput.m[1][3] + m[1][2] * mInput.m[2][3] + m[1][3];

		mOutput.m[2][0] = m[2][0] * mInput.m[0][0] + m[2][1] * mInput.m[1][0] + m[2][2] * mInput.m[2][0];
		mOutput.m[2][1] = m[2][0] * mInput.m[0][1] + m[2][1] * mInput.m[1][1] + m[2][2] * mInput.m[2][1];
		mOutput.m[2][2] = m[2][0] * mInput.m[0][2] + m[2][1] * mInput.m[1][2] + m[2][2] * mInput.m[2][2];
		mOutput.m[2][3] = m[2][0] * mInput.m[0][3] + m[2][1] * mInput.m[1][3] + m[2][2] * mInput.m[2][3] + m[2][3];
	}

	LTMatrix3x4 Multiply(const LTMatrix3x4& mInput) const
	{
		LTMatrix3x4 mResult;
		Multiply(mInput, mResult);
		return mResult;
	}

	LTMatrix3x4 operator*(const LTMatrix3x4& mRhs) const
	{
		return Multiply(mRhs);
	}

	LTMatrix3x4& operator*=(const LTMatrix3x4& mRhs)
	{
		*this = Multiply(mRhs);
		return *this;
	}

	//------------------------------
	// Plane transformation

	//this handles transforming a plane. This plane transformation really only works for standard
	//orthonormal transforms (basically a rotation and an offset)
	LTPlane OrthoTransformPlane(const LTPlane& Src) const
	{
		//this takes advantage of the fact that N' = R * N, and D' = N' . (R * (N * D) + O)
		LTVector vTranslation;
		GetTranslation(vTranslation);

		LTPlane Result;
		Result.m_Normal = Transform3x3(Src.Normal());
		Result.m_Dist = Src.Dist() + Result.m_Normal.Dot(vTranslation);	
		return Result;
	}

	//------------------------------
	// Internal matrix storage

	//the actual matrix data stored m[row][col] form
	float m[3][4];
};

//function that will determine if any values within this matrix are a NaN
inline bool LTIsNaN(const LTMatrix3x4& m)
{
	for(uint32 nRow = 0; nRow < 3; nRow++)
		for(uint32 nCol = 0; nCol < 4; nCol++)
			if(LTIsNaN(m.m[nRow][nCol]))
				return true;
	return false;
}

#endif
