#ifndef __LTROTATION_H__
#define __LTROTATION_H__

#ifndef __LTVECTOR_H__
#include "ltvector.h"
#endif

#ifndef __LTMATRIX_H__
#include "ltmatrix.h"
#endif

#ifndef __LTASSERT_H__
#include "ltassert.h"
#endif

#ifndef __LTMATH_H__
#include "ltmath.h"
#endif

#define LTROTATION_MAX_ERROR 1.5f

#ifndef DOXYGEN_SHOULD_SKIP_THIS
//---------------------------------------------------------------------------//
/*!
Used for: Quaternion representation of a rotation
*/
struct LTRotation
{
	//utility constants for the different components in the quaternion array
	enum
	{
		QX  = 0,
		QY	= 1,
		QZ	= 2,
		QW	= 3
	};

	//-------------------------------
	//Constructors
	LTRotation()												
	{ 
		Init(); 
	}

	LTRotation( const LTRotation &q )							
	{
		*this = q; 
	}

	LTRotation( const LTVector &vAxis, float fAngle )			
	{ 
		Init(vAxis, fAngle); 
	}

	LTRotation( const LTVector &vForward, const LTVector &vUp )
	{
		Init(vForward, vUp);
	}

	LTRotation( float fPitch, float fYaw, float fRoll )
	{
		Init(fPitch, fYaw, fRoll);
	}

	LTRotation( float x, float y, float z, float w )
	{
		Init(x, y, z, w);
	}

	//-------------------------------
	//Initializers

	//initializes the rotation to be the identity orientation
	void Init()
	{
		m_Quat[QX] = 0.0f;
		m_Quat[QY] = 0.0f;
		m_Quat[QZ] = 0.0f;
		m_Quat[QW] = 1.0f;
	}

	//initializes the rotation to be the 4 provided values
	void Init( float x, float y, float z, float w )
	{
		m_Quat[QX] = x;
		m_Quat[QY] = y;
		m_Quat[QZ] = z;
		m_Quat[QW] = w;
		LTASSERT(GetComponentMagSqr() < LTROTATION_MAX_ERROR, "Warning: Quaternion collecting large floating point error.");
	}

	//initializes the quaternion to be a rotation vector that will apply a rotation around the provided
	//vector by the specified angle. Note that this assumes that vAxis is normalized.
	void Init(const LTVector &vAxis, float fAngle)
	{
		fAngle *= 0.5f;
		float fSinAngle = LTSin(fAngle);
		m_Quat[QX] = vAxis.x * fSinAngle;
		m_Quat[QY] = vAxis.y * fSinAngle;
		m_Quat[QZ] = vAxis.z * fSinAngle;
		m_Quat[QW] = LTCos(fAngle);

		LTASSERT(GetComponentMagSqr() < LTROTATION_MAX_ERROR, "Warning: Quaternion collecting large floating point error.");
	}

	//called to initialize the rotation to be an orientation given two non-linear axis, with the first
	//being the forward of the orientation and the second being the up vector
	void Init(const LTVector &vForward, const LTVector &vUp)
	{
		// Are the vectors the same?
		LTVector vTempUp = vUp;
		float fDot = vForward.Dot(vUp);
		if(fDot > 0.99f || fDot < -0.99f)
		{
			// Generate a new up vector
			vTempUp.x = vForward.y;
			vTempUp.y = vForward.z;
			vTempUp.z = vForward.x;
		}

		LTMatrix3x4 mTemp;
		LTVector vRight = vForward.Cross(vTempUp);
		vRight.Normalize( );
		LTVector vTrueUp = vRight.Cross( vForward );
		mTemp.SetBasisVectors(vRight, vTrueUp, vForward);

		ConvertFromMatrix(mTemp);
		LTASSERT(GetComponentMagSqr() < LTROTATION_MAX_ERROR, "Warning: Quaternion collecting large floating point error.");
	}

	//called to create an orientation quaternion given three euler angles. Note that this is an
	//expensive operation and should be avoided if possible
	void Init(float fPitch, float fYaw, float fRoll)
	{
		LTMatrix3x4 mat;

		float yc = LTCos(fYaw), ys = LTSin(fYaw);
		float pc = LTCos(fPitch), ps = LTSin(fPitch);
		float rc = LTCos(fRoll), rs = LTSin(fRoll);

		mat.m[0][0] = rc*yc + rs*ps*ys;
		mat.m[0][1] = -rs*yc + rc*ps*ys;
		mat.m[0][2] = pc*ys;
		mat.m[0][3] = 0.0f;

		mat.m[1][0] = rs*pc;
		mat.m[1][1] = rc*pc;
		mat.m[1][2] = -ps;
		mat.m[1][3] = 0.0f;

		mat.m[2][0] = -rc*ys + rs*ps*yc;
		mat.m[2][1] = rs*ys + rc*ps*yc;
		mat.m[2][2] = pc*yc;
		mat.m[2][3] = 0.0f;

		ConvertFromMatrix(mat);
		LTASSERT(GetComponentMagSqr() < LTROTATION_MAX_ERROR, "Warning: Quaternion collecting large floating point error.");
	}

	//-------------------------------
	//Identity Support

	//called to setup the identity rotation
	void Identity()
	{
		Init();
	}

	//called to get an identity version of this primitive
	static LTRotation	GetIdentity()	{ return LTRotation(0.0f, 0.0f, 0.0f, 1.0f); }

	//-------------------------------
	//Assignment operators

	//allow assignment of other quaternions
	LTRotation& operator = ( const LTRotation &q )
	{
		m_Quat[QX] = q.m_Quat[QX];
		m_Quat[QY] = q.m_Quat[QY];
		m_Quat[QZ] = q.m_Quat[QZ];
		m_Quat[QW] = q.m_Quat[QW];

		LTASSERT(GetComponentMagSqr() < LTROTATION_MAX_ERROR, "Warning: Quaternion collecting large floating point error.");
		return *this;
	}

	//allow assignment of matrices (note, may be deprecated due to cost and somewhat non-intuitive implicit behavior)
	LTRotation& operator = ( const LTMatrix& M )
	{
		ConvertFromMatrix(M);
		return *this;
	}

	//-------------------------------
	//Equality operations

	//determines if two quaternions are exactly equal
	bool operator == ( const LTRotation &q ) const
	{
		return	m_Quat[QX] == q.m_Quat[QX] &&
				m_Quat[QY] == q.m_Quat[QY] &&
				m_Quat[QZ] == q.m_Quat[QZ] &&
				m_Quat[QW] == q.m_Quat[QW];
	}

	//determines if two quaternions are exactly not equal
	bool operator != ( const LTRotation &q ) const
	{
		return	!(*this == q);
	}

	//determines if this quaternion is equal to the provided quaternion within a provided tolerance
	bool NearlyEquals(const LTRotation& q, float fTolerance) const
	{
		return	LTNearlyEquals(m_Quat[QX], q.m_Quat[QX], fTolerance) &&
				LTNearlyEquals(m_Quat[QY], q.m_Quat[QY], fTolerance) &&
				LTNearlyEquals(m_Quat[QZ], q.m_Quat[QZ], fTolerance) &&
				LTNearlyEquals(m_Quat[QW], q.m_Quat[QW], fTolerance);
	}

	//determines if this quaternion is the identity quaternion
	bool IsIdentity() const
	{
		return (*this) == GetIdentity();
	}

	//determines if this quaternion is the identity quaternion within a provided tolerance
	bool IsIdentity(float fTolerance) const
	{
		return NearlyEquals(GetIdentity(), fTolerance);
	}

	//-------------------------------
	//Matrix conversions

	//converts this quaternion into a matrix form
	void ConvertToMatrix( LTMatrix3x4 &M ) const
	{
		float s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

		//get the values for matrix calcuation.
		s = 2.0f / ((m_Quat[QX] * m_Quat[QX]) + (m_Quat[QY] * m_Quat[QY]) + 
					(m_Quat[QZ] * m_Quat[QZ]) + (m_Quat[QW] * m_Quat[QW]));

		xs = m_Quat[QX] * s;
		ys = m_Quat[QY] * s;
		zs = m_Quat[QZ] * s;

		wx = m_Quat[QW] * xs;
		wy = m_Quat[QW] * ys;
		wz = m_Quat[QW] * zs;

		xx = m_Quat[QX] * xs;
		xy = m_Quat[QX] * ys;
		xz = m_Quat[QX] * zs;

		yy = m_Quat[QY] * ys;
		yz = m_Quat[QY] * zs;

		zz = m_Quat[QZ] * zs;

		//Fill in M.mrix
		M.m[0][0] = 1.0f - (yy + zz);
		M.m[0][1] = xy - wz;
		M.m[0][2] = xz + wy;

		M.m[1][0] = xy + wz;
		M.m[1][1] = 1.0f - (xx + zz);
		M.m[1][2] = yz - wx;

		M.m[2][0] = xz - wy;
		M.m[2][1] = yz + wx;
		M.m[2][2] = 1.0f - (xx + yy);

		M.m[0][3] = M.m[1][3] = M.m[2][3] = 0.0f;
	}

	//converts this quaternion into a matrix form
	void ConvertToMatrix( LTMatrix &M ) const
	{
		LTMatrix3x4 mMat;
		ConvertToMatrix(mMat);
        M = mMat;
	}

	//given a matrix, it will construct the quaternion from the three rows, which it assumes
	//are orthonormal
	void ConvertFromMatrix( const LTMatrix3x4 &M )
	{
		float diag, s;
		int i, j, k;

		diag = M.m[0][0] + M.m[1][1] + M.m[2][2];

		if(diag < -0.999f )
		{
			i = QX;
			if( M.m[QY][QY] > M.m[QX][QX] )
				i = QY;
			if( M.m[QZ][QZ] > M.m[i][i] )
				i = QZ;

			j = (i + 1) % 3;
			k = (i + 2) % 3;

			s = LTSqrt( M.m[i][i] - ( M.m[j][j] + M.m[k][k] ) + /*M.m[3][3]*/ 1.0f );

			m_Quat[i] = s * 0.5f;
			s = 0.5f / s;
			m_Quat[QW] = ( M.m[k][j] - M.m[j][k] ) * s;
			m_Quat[j] = ( M.m[j][i] + M.m[i][j] ) * s;
			m_Quat[k] = ( M.m[k][i] + M.m[i][k] ) * s;
			return;
		}

		s = LTSqrt( diag + /*M.m[3][3]*/ 1.0f );

		m_Quat[QW] = s * 0.5f;
		s = 0.5f / s;

		m_Quat[QX] = (M.m[2][1] - M.m[1][2]) * s;
		m_Quat[QY] = (M.m[0][2] - M.m[2][0]) * s;
		m_Quat[QZ] = (M.m[1][0] - M.m[0][1]) * s;

		LTASSERT(GetComponentMagSqr() < LTROTATION_MAX_ERROR, "Warning: Quaternion collecting large floating point error.");
	}

	//converts this quaternion into a matrix form
	void ConvertFromMatrix( const LTMatrix &M )
	{
		ConvertFromMatrix(reinterpret_cast<const LTMatrix3x4&>(M));
	}

	//-------------------------------
	//Normalization

	//gets the magnitude of this quaternion
	float GetComponentMagSqr() const
	{
		return (m_Quat[QX] * m_Quat[QX] + 
				m_Quat[QY] * m_Quat[QY] + 
				m_Quat[QZ] * m_Quat[QZ] + 
				m_Quat[QW] * m_Quat[QW]);
	}

	//called to normalize all of the components within this rotation to ensure that the
	//magnitude of this four component vector is one
	void Normalize()
	{
		float fMag = LTSqrt(GetComponentMagSqr());

		LTASSERT(fMag > 0.01f, "Warning: Attempting to normalize quaternion with very short length");
		m_Quat[QX] /= fMag;
		m_Quat[QY] /= fMag;
		m_Quat[QZ] /= fMag;
		m_Quat[QW] /= fMag;
	}

	//-------------------------------
	//Quaternion operations

	//provides the conjugate (inverse) of the quaternion
	LTRotation Conjugate() const
	{
		return LTRotation(-m_Quat[QX], -m_Quat[QY], -m_Quat[QZ], m_Quat[QW]);
	}

	LTRotation operator ~ () const
	{
		return Conjugate();
	}

	//array access to the individual comonents
	float operator[]( uint32 i ) const
	{
		return m_Quat[i];
	}

	//called to combine two quaternions together
	LTRotation operator * ( const LTRotation &q ) const
	{
		float qX = m_Quat[QW]*q.m_Quat[QX] + m_Quat[QX]*q.m_Quat[QW] + m_Quat[QY]*q.m_Quat[QZ] - m_Quat[QZ]*q.m_Quat[QY];
		float qY = m_Quat[QW]*q.m_Quat[QY] - m_Quat[QX]*q.m_Quat[QZ] + m_Quat[QY]*q.m_Quat[QW] + m_Quat[QZ]*q.m_Quat[QX];
		float qZ = m_Quat[QW]*q.m_Quat[QZ] + m_Quat[QX]*q.m_Quat[QY] - m_Quat[QY]*q.m_Quat[QX] + m_Quat[QZ]*q.m_Quat[QW];
		float qW = m_Quat[QW]*q.m_Quat[QW] - m_Quat[QX]*q.m_Quat[QX] - m_Quat[QY]*q.m_Quat[QY] - m_Quat[QZ]*q.m_Quat[QZ];

		return LTRotation( qX, qY, qZ, qW );
	}

	//multiply assignment operator
	LTRotation operator *= (const LTRotation& q)
	{
		*this = (*this) * q;
		return *this;
	}

	//called to perform a spherical interpolation between two quaternions, this assumes t=[0..1]
	void Slerp( const LTRotation &q0, const LTRotation &q1, float t )
	{
		float	rot1q[4];
		float	omega, cosom, oosinom;
		float	scalerot0, scalerot1;

		//Calculate the cosine
		cosom = q0.m_Quat[0]*q1.m_Quat[0] + q0.m_Quat[1]*q1.m_Quat[1] + q0.m_Quat[2]*q1.m_Quat[2] + q0.m_Quat[3]*q1.m_Quat[3];

		//adjust signs if necessary
		if(cosom < 0.0f)
		{
			cosom = -cosom;
			rot1q[0] = -q1.m_Quat[0];
			rot1q[1] = -q1.m_Quat[1];
			rot1q[2] = -q1.m_Quat[2];
			rot1q[3] = -q1.m_Quat[3];
		}
		else  
		{
			rot1q[0] = q1.m_Quat[0];
			rot1q[1] = q1.m_Quat[1];
			rot1q[2] = q1.m_Quat[2];
			rot1q[3] = q1.m_Quat[3];
		}

		//calculate interpolating coeffs
		if ( (1.0f - cosom) > 0.0001f ) 
		{ 
			//standard case
			omega   = LTArcCos(cosom);
			oosinom = 1.0f / LTSin(omega);
			scalerot0 = LTSin((1.f - t) * omega) * oosinom;
			scalerot1 = LTSin(t * omega) * oosinom;
		}
		else
		{ 
			//rot0 and rot1 very close - just do linear interp.
			scalerot0 = 1.0f - t;
			scalerot1 = t;
		}

		//build the new quarternion
		m_Quat[0] = (scalerot0 * q0.m_Quat[0] + scalerot1 * rot1q[0]);
		m_Quat[1] = (scalerot0 * q0.m_Quat[1] + scalerot1 * rot1q[1]);
		m_Quat[2] = (scalerot0 * q0.m_Quat[2] + scalerot1 * rot1q[2]);
		m_Quat[3] = (scalerot0 * q0.m_Quat[3] + scalerot1 * rot1q[3]);

		LTASSERT(GetComponentMagSqr() < LTROTATION_MAX_ERROR, "Warning: Quaternion collecting large floating point error.");
	}

	// Rotate about unit-length axis vAxis by fAngle radians
	void Rotate(const LTVector &vAxis, float fAngle)
	{
		LTRotation rTemp(vAxis, fAngle);
		*this = rTemp * *this;

		LTASSERT(GetComponentMagSqr() < LTROTATION_MAX_ERROR, "Warning: Quaternion collecting large floating point error.");
	}

	//-------------------------------
	//Axis support

	//provides the +X axis of the quaternions basis space
	LTVector Right() const
	{
		return LTVector(
			1.0f - 2.0f * (m_Quat[QY] * m_Quat[QY] + m_Quat[QZ] * m_Quat[QZ]),
			2.0f * (m_Quat[QX] * m_Quat[QY] + m_Quat[QW] * m_Quat[QZ]),
			2.0f * (m_Quat[QX] * m_Quat[QZ] - m_Quat[QW] * m_Quat[QY]));
	}

	//provides the +Y axis of the quaternions basis space
	LTVector Up() const
	{
		return LTVector(
			2.0f * (m_Quat[QX] * m_Quat[QY] - m_Quat[QW] * m_Quat[QZ]),
			1.0f - 2.0f * (m_Quat[QX] * m_Quat[QX] + m_Quat[QZ] * m_Quat[QZ]),
			2.0f * (m_Quat[QY] * m_Quat[QZ] + m_Quat[QW] * m_Quat[QX]));
	}

	//provides the +Z axis of the quaternions basis space
	LTVector Forward() const
	{
		return LTVector(
			2.0f * (m_Quat[QX] * m_Quat[QZ] + m_Quat[QW] * m_Quat[QY]),
			2.0f * (m_Quat[QY] * m_Quat[QZ] - m_Quat[QW] * m_Quat[QX]),
			1.0f - 2.0f * (m_Quat[QX] * m_Quat[QX] + m_Quat[QY] * m_Quat[QY]));
	}

	//used to get all three axis vectors at once. This is more efficient than getting each
	//individually if all three are needed
	void	GetVectors(LTVector& vRight, LTVector& vUp, LTVector& vForward) const
	{
		LTMatrix3x4 mAxis;
		ConvertToMatrix(mAxis);
		mAxis.GetBasisVectors(vRight, vUp, vForward);
	}

	//-------------------------------
	//Vector rotation support

	// Apply the quaternion rotation to the specified vector. This is the same as
	// converting the quaternion to a matrix and multiplying the vector by that
	// matrix.
	LTVector RotateVector(const LTVector& v) const
	{
		// evaluate quat * vector
		// bw == 0.0f
		float  r1[4] ;
		r1[QX] = m_Quat[QW]*v.x + m_Quat[QY]*v.z - m_Quat[QZ]*v.y;
		r1[QY] = m_Quat[QW]*v.y - m_Quat[QX]*v.z + m_Quat[QZ]*v.x;
		r1[QZ] = m_Quat[QW]*v.z + m_Quat[QX]*v.y - m_Quat[QY]*v.x;
		r1[QW] =-m_Quat[QX]*v.x - m_Quat[QY]*v.y - m_Quat[QZ]*v.z;

		// make the conjugate of quat.
		float  cj[4];
		cj[QX] = -m_Quat[QX]; cj[QY] = -m_Quat[QY];  cj[QZ] = -m_Quat[QZ]; cj[QW] = m_Quat[QW];

		// reassign
		const float *a = r1 ; // result from qv
		const float *b = cj ; // conjuagate

		LTVector vRV;

		// evaluate vector * conj
		vRV.x = a[QW]*b[QX] + a[QX]*b[QW] + a[QY]*b[QZ] - a[QZ]*b[QY];
		vRV.y = a[QW]*b[QY] - a[QX]*b[QZ] + a[QY]*b[QW] + a[QZ]*b[QX];
		vRV.z = a[QW]*b[QZ] + a[QX]*b[QY] - a[QY]*b[QX] + a[QZ]*b[QW];

		return vRV;
	}

	//operator form of the above
	LTVector operator*(const LTVector& v) const
	{
		return RotateVector(v);
	}

	//-------------------------------
	//Quaternion Data

	//our quaternion information stored in (<x,y,z>,w), where the vector represents the axis of rotation
	//with a magnitude of sin(angle/2), and the w component = cos(angle/2)
	float m_Quat[4];
};

//detects if this rotation includes any NaN's within it
inline bool LTIsNaN(const LTRotation& rRot)	
{ 
	return	LTIsNaN(rRot.m_Quat[LTRotation::QX]) || LTIsNaN(rRot.m_Quat[LTRotation::QY]) || 
			LTIsNaN(rRot.m_Quat[LTRotation::QZ]) || LTIsNaN(rRot.m_Quat[LTRotation::QW]);
}

// Read/write.
inline void LTStream_Read(ILTInStream *pStream, LTRotation &quat)
{
	STREAM_READ(quat.m_Quat[LTRotation::QX]);
	STREAM_READ(quat.m_Quat[LTRotation::QY]);
	STREAM_READ(quat.m_Quat[LTRotation::QZ]);
	STREAM_READ(quat.m_Quat[LTRotation::QW]);
}


inline void LTStream_Write(ILTOutStream *pStream, const LTRotation &quat)
{
	STREAM_WRITE(quat.m_Quat[LTRotation::QX]);
	STREAM_WRITE(quat.m_Quat[LTRotation::QY]);
	STREAM_WRITE(quat.m_Quat[LTRotation::QZ]);
	STREAM_WRITE(quat.m_Quat[LTRotation::QW]);
}

#endif//doxygen


#endif
//EOF

