//---------------------------------------------------------------------------
//
// LTSphere.h
//
//---------------------------------------------------------------------------

#ifndef __LTSPHERE_H__
#define __LTSPHERE_H__

#ifndef __LTVECTOR_H__
#include "ltvector.h"
#endif//__LTVECTOR_H__

//---------------------------------------------------------------------------

class LTSphere
{
	public:

		//---------------------------------------------------------------------------
		// Construction and initialization methods

		LTSphere() {}

		LTSphere( const LTVector& vCenter, float fRadius )
		{
			Init( vCenter, fRadius );
		}

		LTSphere( float x, float y, float z, float fRadius )
		{
			Init( x, y, z, fRadius );
		}

		void Init( const LTVector& vCenter, float fRadius )
		{
			m_vCenter = vCenter;
			m_fRadius = fRadius;
		}

		void Init( float x, float y, float z, float fRadius )
		{
			m_vCenter.Init( x, y, z );
			m_fRadius = fRadius;
		}


		//---------------------------------------------------------------------------
		// Creation methods

		void CreateBoundingSphere( const LTVector* pPoints, uint32 nPoints )
		{
			LTASSERT( pPoints && ( nPoints > 0 ), "LTSphere::Create() -- Function needs valid parameters!" );

			// Find the center of the sphere
			m_vCenter = pPoints[ 0 ];

			for( uint32 i = 1; i < nPoints; ++i )
			{
				m_vCenter += pPoints[ i ];
			}

			m_vCenter /= ( float )nPoints;

			// Now find the longest distance for the radius
			float fDistSqr = 0.0f;
			m_fRadius = 0.0f;

			for( uint32 i = 0; i < nPoints; ++i )
			{
				fDistSqr = ( m_vCenter - pPoints[ i ] ).MagSqr();

				if( fDistSqr > m_fRadius )
				{
					m_fRadius = fDistSqr;
				}
			}

			m_fRadius = sqrtf( m_fRadius );
		}


		//---------------------------------------------------------------------------
		// Modification methods

		// The expand interfaces cause the sphere's radius to get larger, based on the input data.
		// The center point will remain the same.
		void Expand( const LTVector& v )
		{
			float fMag = ( v - m_vCenter ).Mag();

			if( fMag > m_fRadius )
			{
				m_fRadius = fMag;
			}
		}

		void Expand( const LTSphere& s )
		{
			float fMag = ( ( s.m_vCenter - m_vCenter ).Mag() + s.m_fRadius );

			if( fMag > m_fRadius )
			{
				m_fRadius = fMag;
			}
		}

		// The encompass interfaces cause the sphere to recenter and grow around the input data.
		void Encompass( const LTVector& v )
		{
			LTVector vDelta = ( v - m_vCenter );
			float fMag = vDelta.Mag();
			float fDelta = ( fMag - m_fRadius );

			if( fDelta > 0.0f )
			{
				float fAdjustment = ( ( fDelta * 0.5f ) / fMag );

				m_fRadius += fDelta;
				m_vCenter = ( m_vCenter + ( vDelta * fAdjustment ) );
			}
		}

		void Encompass( const LTSphere& s )
		{
			LTVector vDelta = ( s.m_vCenter - m_vCenter );
			float fMag = vDelta.Mag();
			float fDelta = ( fMag - m_fRadius + s.m_fRadius );

			if( fDelta > 0.0f )
			{
				float fAdjustment = ( ( fDelta * 0.5f ) / fMag );

				m_fRadius += fDelta;
				m_vCenter = ( m_vCenter + ( vDelta * fAdjustment ) );
			}
		}


		//---------------------------------------------------------------------------
		// Data access

		const LTVector&		Center() const						{ return m_vCenter; }
		float				Radius() const						{ return m_fRadius; }
		float				RadiusSqr() const					{ return ( m_fRadius * m_fRadius ); }


	public:

		//---------------------------------------------------------------------------
		// Sphere data

		LTVector			m_vCenter;
		float				m_fRadius;
};

//---------------------------------------------------------------------------

#endif//__LTSPHERE_H__

