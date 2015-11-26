//---------------------------------------------------------------------------
//
// LTOBB.h
//
//---------------------------------------------------------------------------

#ifndef __LTOBB_H__
#define __LTOBB_H__

#ifndef __LTVECTOR_H__
#include "ltvector.h"
#endif//__LTVECTOR_H__

#ifndef __LTROTATION_H__
#include "ltrotation.h"
#endif//__LTROTATION_H__

//---------------------------------------------------------------------------

class LTOBB
{
	public:

		//---------------------------------------------------------------------------
		// Construction and initialization methods

		LTOBB() {}

		LTOBB( const LTVector& vCenter, const LTVector& vHalfDims, const LTVector& vRight, const LTVector& vUp, const LTVector& vForward )
		{
			m_vCenter = vCenter;
			m_vHalfDims = vHalfDims;
			m_vAxes[ 0 ] = vRight;
			m_vAxes[ 1 ] = vUp;
			m_vAxes[ 2 ] = vForward;
		}

		LTOBB( const LTVector& vCenter, const LTVector& vHalfDims, const LTRotation& rRot )
		{
			m_vCenter = vCenter;
			m_vHalfDims = vHalfDims;
			m_vAxes[ 0 ] = rRot.Right();
			m_vAxes[ 1 ] = rRot.Up();
			m_vAxes[ 2 ] = rRot.Forward();
		}

		void Init( const LTVector& vCenter, const LTVector& vHalfDims, const LTVector& vRight, const LTVector& vUp, const LTVector& vForward )
		{
			m_vCenter = vCenter;
			m_vHalfDims = vHalfDims;
			m_vAxes[ 0 ] = vRight;
			m_vAxes[ 1 ] = vUp;
			m_vAxes[ 2 ] = vForward;
		}

		void Init( const LTVector& vCenter, const LTVector& vHalfDims, const LTRotation& rRot )
		{
			m_vCenter = vCenter;
			m_vHalfDims = vHalfDims;
			m_vAxes[ 0 ] = rRot.Right();
			m_vAxes[ 1 ] = rRot.Up();
			m_vAxes[ 2 ] = rRot.Forward();
		}


		//---------------------------------------------------------------------------
		// Data access

		const LTVector&		Center() const						{ return m_vCenter; }
		const LTVector&		HalfDims() const						{ return m_vHalfDims; }
		const LTVector		Dims() const							{ return ( m_vHalfDims * 2.0f ); }

		const LTVector&		Right() const							{ return m_vAxes[ 0 ]; }
		const LTVector&		Up() const								{ return m_vAxes[ 1 ]; }
		const LTVector&		Forward() const						{ return m_vAxes[ 2 ]; }


	public:

		//---------------------------------------------------------------------------
		// OBB data

		LTVector			m_vCenter;
		LTVector			m_vHalfDims;
		LTVector			m_vAxes[ 3 ];
};

//---------------------------------------------------------------------------

#endif//__LTOBB_H__

