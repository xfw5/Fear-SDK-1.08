//---------------------------------------------------------------------------
//
// LTIntersect.h
//
// Provides a collection of intersection routines that can be used. Each one
// returns a boolean which is true if an intersection occurred, or false if
// no intersection occurred. Two primitives touching at a single point, such
// as a sphere touching the boundary of another sphere is considered an 
// intersection.
//
//---------------------------------------------------------------------------

#ifndef __LTINTERSECT_H__
#define __LTINTERSECT_H__

#ifndef __LTVECTOR_H__
#include "ltvector.h"
#endif//__LTVECTOR_H__

#ifndef __LTRECT_H__
#include "ltrect.h"
#endif//__LTRECT_H__

#ifndef __LTOBB_H__
#include "ltobb.h"
#endif//__LTOBB_H__

#ifndef __LTSPHERE_H__
#include "ltsphere.h"
#endif//__LTSPHERE_H__

//----------------------------------------------------------------------------------------------

namespace LTIntersect
{
	//----------------------------------------------------------------------------------------------
	// Interface declarations ( implementations are listed below )
	//----------------------------------------------------------------------------------------------

	// 3D DISTANCE INTERFACES

	// Takes a point in space, and two points on an infinite line.  The return value is
	// the closest distance of the point to the line ( squared ).  The vClosestPt variable
	// gets filled in with the closest point on the line, and the fClosestTime variable
	// is set to the time along the line (based on the input points segment) that
	// vClosestPt represents.
	inline float Point_Line_DistSqr( const LTVector& vPt, const LTVector& vStart, const LTVector& vEnd,
										LTVector& vClosestPt, float& fClosestTime );

	// Takes a point in space, and two points representing a segment.  The return value is
	// the closest distance of the point to the segment ( squared ).  The vClosestPt variable
	// gets filled in with the closest point on the segment, and the fClosestTime variable
	// is set to the 0.0 -> 1.0 time along the segment that vClosestPt represents.
	inline float Point_Segment_DistSqr( const LTVector& vPt, const LTVector& vStart, const LTVector& vEnd,
										LTVector& vClosestPt, float& fClosestTime );

	// Takes start and end points for two segments.  The return value is the closest distance
	// between the two segments ( squared ).  The closest point variables get filled in with
	// the two closest points on each segment.
	inline float Segment_Segment_DistSqr( const LTVector& vStart1, const LTVector& vEnd1,
											const LTVector& vStart2, const LTVector& vEnd2,
											LTVector& vClosestPt1, LTVector& vClosestPt2 );

	// 2D TRUE / FALSE INTERFACES ( mainly for optimal performance when not concerning one axis )

	// Takes 2D oriented box data in the form of a center point, half dims, and a radian rotation
	// angle... and a 2D segment.  Returns true if the two collide.
	inline bool OBB2D_Segment2D( const LTVector2& vBoxCenter, const LTVector2& vBoxHalfDims, float fBoxAngle,
									const LTVector2& vStart, const LTVector2& vEnd, float& fCollisionTime );

	// 3D TRUE / FALSE INTERFACES

	// Returns true if the boxes overlap ( false if only an edge touches ).
	inline bool AABB_AABB( const LTRect3f& rAABB1, const LTRect3f& rAABB2 );

	// Returns true if the point lies within the box or on the edge of the box.
	inline bool AABB_Point( const LTRect3f& rAABB, const LTVector& vPt );

	// Returns true if the sphere overlaps or just touches the box.
	inline bool AABB_Sphere( const LTRect3f& rAABB, const LTSphere& sSphere );

	// Takes an AABB and a segment. Returns true if the segment intersects or just touches the AABB
	// ( including if the start point is contained by the AABB ).
	inline bool AABB_Segment( const LTRect3f& rAABB, const LTVector& vStart, const LTVector& vEnd, float& fCollisionTime );

	// Returns true if the spheres overlap or just touch at the edge.
	inline bool Sphere_Sphere( const LTSphere& sSphere1, const LTSphere& sSphere2 );

	// Returns true if the point is within or on the edge of the sphere.
	inline bool Sphere_Point( const LTSphere& sSphere, const LTVector &vPt );

	// Takes a sphere and two points defining a segment. Returns true if the segment intersects
	// or just touches the sphere ( including if the start point is contained by the sphere ).
	inline bool Sphere_Segment( const LTSphere& sSphere, const LTVector& vStart, const LTVector& vEnd );

	// Takes a segment with radius and a sphere, and returns whether the two collide.  The
	// fCollisionTime variables gets filled in with the time along the segment that the
	// collision occured... this can be used to find the actual collision point if needed.
	inline bool SweptSphere_Sphere( const LTVector& vStart, const LTVector& vEnd, float fSweptRadius,
									const LTSphere& sSphere, float& fCollisionTime );

	// Returns true if the point lies within, or on the edge of the box.
	inline bool OBB_Point( const LTOBB& rOBB, const LTVector& vPt );

	// Returns true if the sphere overlaps or just touches the box.
	inline bool OBB_Sphere( const LTOBB& rOBB, const LTSphere& sSphere );

	// Takes an OBB and a segment. Returns true if the segment intersects or just touches the OBB
	// ( including if the start point is contained by the OBB ).
	inline bool OBB_Segment( const LTOBB& rOBB, const LTVector& vStart, const LTVector& vEnd, float& fCollisionTime );

	// Takes convex hull data, and returns true of the point lies within or on the hull.
	inline bool Hull_Point( const LTPlane* pPlanes, uint32 nPlanes, const LTVector& vPt );

	// Takes convex hull data, and returns true of the sphere intersects or just touches the hull.
	// NOTE: Not totally accurate, but should be close enough for most cases.
	inline bool Hull_Sphere( const LTPlane* pPlanes, uint32 nPlanes, const LTSphere& sSphere );


	//----------------------------------------------------------------------------------------------
	// Implementations
	//----------------------------------------------------------------------------------------------

	float Point_Line_DistSqr( const LTVector& vPt, const LTVector& vStart, const LTVector& vEnd,
								LTVector& vClosestPt, float& fClosestTime )
	{
		LTVector vDir = ( vEnd - vStart );
		LTVector vSec = ( vPt - vStart );

		float fD1 = vSec.Dot( vDir );
		float fD2 = vDir.Dot( vDir );

		fClosestTime = LTNearlyEquals( fD2, 0.0f, 0.0001f ) ? 0.0f : ( fD1 / fD2 );
		vClosestPt = vStart + ( vDir * fClosestTime );

		return vPt.DistSqr( vClosestPt );
	}

	//----------------------------------------------------------------------------------------------

	float Point_Segment_DistSqr( const LTVector& vPt, const LTVector& vStart, const LTVector& vEnd,
									LTVector& vClosestPt, float& fClosestTime )
	{
		LTVector vDir = ( vEnd - vStart );
		LTVector vSec = ( vPt - vStart );

		float fD1 = vSec.Dot( vDir );
		float fD2 = vDir.Dot( vDir );

		if( fD1 < 0.0f )
		{
			fClosestTime = 0.0f;
			vClosestPt = vStart;
		}
		else if( fD2 < fD1 )
		{
			fClosestTime = 1.0f;
			vClosestPt = vEnd;
		}
		else
		{
			fClosestTime = LTNearlyEquals( fD2, 0.0f, 0.0001f ) ? 0.0f : ( fD1 / fD2 );
			vClosestPt = vStart + ( vDir * fClosestTime );
		}

		return vPt.DistSqr( vClosestPt );
	}

	//----------------------------------------------------------------------------------------------

	float Segment_Segment_DistSqr( const LTVector& vStart1, const LTVector& vEnd1,
									const LTVector& vStart2, const LTVector& vEnd2,
									LTVector& vClosestPt1, LTVector& vClosestPt2 )
	{
		LTVector vDir1 = ( vEnd1 - vStart1 );
		LTVector vDir2 = ( vEnd2 - vStart2 );
		LTVector vDiff = ( vStart1 - vStart2 );

		const float fSmall = 0.00000001f;

		float fADot = vDir1.Dot( vDir1 );
		float fBDot = vDir1.Dot( vDir2 );
		float fCDot = vDir2.Dot( vDir2 );
		float fDDot = vDir1.Dot( vDiff );
		float fEDot = vDir2.Dot( vDiff );

		float fD = ( fADot * fCDot ) - ( fBDot * fBDot );

		float sC, sN, sD = fD;
		float tC, tN, tD = fD;


		// Compute the line parameters of the two closest points
		if( fD < fSmall )
		{
			// The lines are almost parallel...
			sN = 0.0;
			sD = 1.0;
			tN = fEDot;
			tD = fCDot;
		}
		else
		{
			// Get the closest points on the infinite lines...
			sN = ( fBDot * fEDot ) - ( fCDot * fDDot );
			tN = ( fADot * fEDot ) - ( fBDot * fDDot );

			if( sN < 0.0 )
			{
				sN = 0.0;
				tN = fEDot;
				tD = fCDot;
			}
			else if( sN > sD )
			{
				sN = sD;
				tN = fEDot + fBDot;
				tD = fCDot;
			}
		}

		if( tN < 0.0 )
		{
			tN = 0.0;

			if( -fDDot < 0.0 )
			{
				sN = 0.0;
			}
			else if( -fDDot > fADot )
			{
				sN = sD;
			}
			else
			{
				sN = -fDDot;
				sD = fADot;
			}
		}
		else if( tN > tD )
		{
			tN = tD;

			if( ( -fDDot + fBDot ) < 0.0 )
			{
				sN = 0;
			}
			else if( ( -fDDot + fBDot ) > fADot )
			{
				sN = sD;
			}
			else
			{
				sN = ( -fDDot + fBDot );
				sD = fADot;
			}
		}

		sC = ( fabs( sN ) < fSmall ) ? 0.0f : ( sN / sD );
		tC = ( fabs( tN ) < fSmall ) ? 0.0f : ( tN / tD );


		// Get the difference of the two closest points
		LTVector dP = vDiff + ( vDir1 * sC ) - ( vDir2 * tC );

		vClosestPt1 = vStart1 + ( vDir1 * sC );
		vClosestPt2 = vStart2 + ( vDir2 * tC );

		return dP.MagSqr();
	}

	//----------------------------------------------------------------------------------------------

	bool OBB2D_Segment2D( const LTVector2& vBoxCenter, const LTVector2& vBoxHalfDims, float fBoxAngle,
							const LTVector2& vStart, const LTVector2& vEnd, float& fCollisionTime )
	{
		// Offset the segment by the box center
		LTVector2 vSegS = ( vStart - vBoxCenter );
		LTVector2 vSegE = ( vEnd - vBoxCenter );


		// Now rotate them into the space of the box
		float fBoxCos = cosf( -fBoxAngle );
		float fBoxSin = sinf( -fBoxAngle );

		vSegS.Init( ( ( vSegS.x * fBoxCos ) - ( vSegS.y * fBoxSin ) ), ( ( vSegS.x * fBoxSin ) + ( vSegS.y * fBoxCos ) ) );
		vSegE.Init( ( ( vSegE.x * fBoxCos ) - ( vSegE.y * fBoxSin ) ), ( ( vSegE.x * fBoxSin ) + ( vSegE.y * fBoxCos ) ) );


		// Common time tracking variables
		float fTempTimeS, fTempTimeE;
		float fFinalTimeS = 0.0f;
		float fFinalTimeE = 1.0f;


		// Handle collision at each axis separately
		for( uint32 i = 0; i < 2; ++i )
		{
			if( vSegS[ i ] < vSegE[ i ] )
			{
				// Do a fast exclusion check
				if( ( vSegS[ i ] > vBoxHalfDims[ i ] ) || ( vSegE[ i ] < -vBoxHalfDims[ i ] ) )
				{
					return false;
				}

				// Calculate the new times
				float fDelta = ( vSegE[ i ] - vSegS[ i ] );
				if( !LTNearlyEquals( fDelta, 0.0f, 0.0001f ))
				{
					fTempTimeS = ( vSegS[ i ] < -vBoxHalfDims[ i ] ) ? ( ( -vBoxHalfDims[ i ] - vSegS[ i ] ) / fDelta ) : 0.0f;
					fTempTimeE = ( vSegE[ i ] > vBoxHalfDims[ i ] ) ? ( ( vBoxHalfDims[ i ] - vSegS[ i ] ) / fDelta ) : 1.0f;
				}
				else
				{
					fTempTimeS = 0.0f;
					fTempTimeE = 1.0f;
				}
			}
			else
			{
				// Do a fast exclusion check
				if( ( vSegS[ i ] < -vBoxHalfDims[ i ] ) || ( vSegE[ i ] > vBoxHalfDims[ i ] ) )
				{
					return false;
				}

				// Calculate the new times
				float fDelta = ( vSegE[ i ] - vSegS[ i ] );
				if( !LTNearlyEquals( fDelta, 0.0f, 0.0001f ))
				{
					fTempTimeS = ( vSegS[ i ] > -vBoxHalfDims[ i ] ) ? ( ( vBoxHalfDims[ i ] - vSegS[ i ] ) / fDelta ) : 0.0f;
					fTempTimeE = ( vSegE[ i ] < vBoxHalfDims[ i ] ) ? ( ( -vBoxHalfDims[ i ] - vSegS[ i ] ) / fDelta ) : 1.0f;
				}
				else
				{
					fTempTimeS = 0.0f;
					fTempTimeE = 1.0f;
				}
			}

			// Update the final times
			if( fTempTimeS > fFinalTimeS ) fFinalTimeS = fTempTimeS;
			if( fTempTimeE < fFinalTimeE ) fFinalTimeE = fTempTimeE;


			// If the times have passed each other... there is no collision
			if( fFinalTimeS > fFinalTimeE )
			{
				return false;
			}
		}


		// Assign the collision time and return
		fCollisionTime = fFinalTimeS;
		return true;
	}

	//----------------------------------------------------------------------------------------------

	bool AABB_AABB( const LTRect3f& rAABB1, const LTRect3f& rAABB2 )
	{
		return rAABB1.Overlaps( rAABB2 );
	}

	//----------------------------------------------------------------------------------------------

	bool AABB_Point( const LTRect3f& rAABB, const LTVector& vPt )
	{
		return rAABB.Contains( vPt );
	}

	//----------------------------------------------------------------------------------------------

	bool AABB_Sphere( const LTRect3f& rAABB, const LTSphere& sSphere )
	{
		// This algorithm works by finding the closest point on the AABB to the center of the sphere
		// and then checking the distance to that against the radius. This works because if the distance
		// is equal to x^2 + y^2 + z^2, so we find if the center is on either side of the bounding
		// planes for each axis, and use the closest one.

		float fTotalDist = 0.0f;
		float fRadiusSqr = sSphere.RadiusSqr();

		for( uint32 nCurrAxis = 0; nCurrAxis < 3; ++nCurrAxis )
		{
			// See if it lies outside of either bounding plane, and if so, find the distance
			// in that dimension to our box.
			if( sSphere.m_vCenter[ nCurrAxis ] < rAABB.m_vMin[ nCurrAxis ] )
			{
				float fDist = ( sSphere.m_vCenter[ nCurrAxis ] - rAABB.m_vMin[ nCurrAxis ] );
				fTotalDist += ( fDist * fDist );

				// Now if we are outside of the sphere radius, we don't intersect.
				if( fTotalDist > fRadiusSqr )
				{
					return false;
				}
			}
			else if( sSphere.m_vCenter[ nCurrAxis ] > rAABB.m_vMax[ nCurrAxis ] )
			{
				float fDist = ( sSphere.m_vCenter[ nCurrAxis ] - rAABB.m_vMax[ nCurrAxis ] );
				fTotalDist += ( fDist * fDist );

				// Now if we are outside of the sphere radius, we don't intersect.
				if( fTotalDist > fRadiusSqr )
				{
					return false;
				}
			}
		}

		// The shapes overlap, or touch on an edge!
		return true;
	}

	//----------------------------------------------------------------------------------------------

	bool AABB_Segment( const LTRect3f& rAABB, const LTVector& vStart, const LTVector& vEnd, float& fCollisionTime )
	{
		// Common time tracking variables
		float fTempTimeS, fTempTimeE;
		float fFinalTimeS = 0.0f;
		float fFinalTimeE = 1.0f;


		// Handle collision at each axis separately
		for( uint32 i = 0; i < 3; ++i )
		{
			if( vStart[ i ] < vEnd[ i ] )
			{
				// Do a fast exclusion check
				if( ( vStart[ i ] > rAABB.m_vMax[ i ] ) || ( vEnd[ i ] < rAABB.m_vMin[ i ] ) )
				{
					return false;
				}

				// Calculate the new times
				float fDelta = ( vEnd[ i ] - vStart[ i ] );
				if( !LTNearlyEquals( fDelta, 0.0f, 0.0001f ))
				{
					fTempTimeS = ( vStart[ i ] < rAABB.m_vMin[ i ] ) ? ( ( rAABB.m_vMin[ i ] - vStart[ i ] ) / fDelta ) : 0.0f;
					fTempTimeE = ( vEnd[ i ] > rAABB.m_vMax[ i ] ) ? ( ( rAABB.m_vMax[ i ] - vStart[ i ] ) / fDelta ) : 1.0f;
				}
				else
				{
					fTempTimeS = 0.0f;
					fTempTimeE = 1.0f;
				}
			}
			else
			{
				// Do a fast exclusion check
				if( ( vStart[ i ] < rAABB.m_vMin[ i ] ) || ( vEnd[ i ] > rAABB.m_vMax[ i ] ) )
				{
					return false;
				}

				// Calculate the new times
				float fDelta = ( vEnd[ i ] - vStart[ i ] );
				if( !LTNearlyEquals( fDelta, 0.0f, 0.0001f ))
				{
					fTempTimeS = ( vStart[ i ] > rAABB.m_vMin[ i ] ) ? ( ( rAABB.m_vMax[ i ] - vStart[ i ] ) / fDelta ) : 0.0f;
					fTempTimeE = ( vEnd[ i ] < rAABB.m_vMax[ i ] ) ? ( ( rAABB.m_vMin[ i ] - vStart[ i ] ) / fDelta ) : 1.0f;
				}
				else
				{
					fTempTimeS = 0.0f;
					fTempTimeE = 1.0f;
				}
			}

			// Update the final times
			if( fTempTimeS > fFinalTimeS ) fFinalTimeS = fTempTimeS;
			if( fTempTimeE < fFinalTimeE ) fFinalTimeE = fTempTimeE;


			// If the times have passed each other... there is no collision
			if( fFinalTimeS > fFinalTimeE )
			{
				return false;
			}
		}


		// Assign the collision time and return
		fCollisionTime = fFinalTimeS;
		return true;
	}

	//----------------------------------------------------------------------------------------------

	bool Sphere_Sphere( const LTSphere& sSphere1, const LTSphere& sSphere2 )
	{
		float fDistSqr = ( sSphere2.m_vCenter - sSphere1.m_vCenter ).MagSqr();
		float fTotalRadius = ( sSphere1.m_fRadius + sSphere2.m_fRadius );

		return ( fDistSqr <= ( fTotalRadius * fTotalRadius ) );
	}

	//----------------------------------------------------------------------------------------------

	bool Sphere_Point( const LTSphere& sSphere, const LTVector& vPt )
	{
		return ( ( sSphere.m_vCenter - vPt ).MagSqr() <= sSphere.RadiusSqr() );
	}

	//----------------------------------------------------------------------------------------------

	bool Sphere_Segment( const LTSphere& sSphere, const LTVector& vStart, const LTVector& vEnd )
	{
		LTVector vClosestPt;
		float fClosestTime;

		float fClosestDistSqr = Point_Segment_DistSqr( sSphere.m_vCenter, vStart, vEnd, vClosestPt, fClosestTime );
		return ( fClosestDistSqr <= sSphere.RadiusSqr() );
	}

	//----------------------------------------------------------------------------------------------

	bool SweptSphere_Sphere( const LTVector& vStart, const LTVector& vEnd, float fSweptRadius,
								const LTSphere& sSphere, float& fCollisionTime )
	{
		// Find the distances between the swept path and the point
		LTVector vClosestPt;
		float fClosestTime;

		float fTotalRadius = ( fSweptRadius + sSphere.m_fRadius );
		float fTotalRadiusSqr = ( fTotalRadius * fTotalRadius );
		float fClosestDistSqr = Point_Segment_DistSqr( sSphere.m_vCenter, vStart, vEnd, vClosestPt, fClosestTime );

		// If there was no intersection, just return
		if( fClosestDistSqr >= fTotalRadiusSqr )
		{
			return false;
		}


		// Calculate the time at which the swept sphere intersects with the other sphere, in relation
		// to the direction of the swept path.  Negative times are possible if the start of the swept
		// path is already colliding with the other sphere.
		LTVector vDir = ( vEnd - vStart );
		float fDirLen = vDir.Mag();

		float fProjBackLen = sqrtf( fTotalRadiusSqr - fClosestDistSqr );
		float fProjBackTime = ( fProjBackLen / fDirLen );

		fCollisionTime = ( fClosestTime - fProjBackTime );

		return true;
	}

	//----------------------------------------------------------------------------------------------

	bool OBB_Point( const LTOBB& rOBB, const LTVector& vPt )
	{
		// Get the point in the space of the OBB...
		LTVector vTransPt = ( vPt - rOBB.m_vCenter );

		// Find the distance along each axis
		for( uint32 nAxis = 0; nAxis < 3; ++nAxis )
		{
			float fDot = vTransPt.Dot( rOBB.m_vAxes[ nAxis ] );

			// If the distance is outside the box... return false
			if( ( fDot < -rOBB.m_vHalfDims[ nAxis ] ) || ( fDot > rOBB.m_vHalfDims[ nAxis ] ) )
			{
				return false;
			}
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------

	bool OBB_Sphere( const LTOBB& rOBB, const LTSphere& sSphere )
	{
		// This method uses the same intersection code as the AABB vs Sphere function.  But
		// instead, it uses the transformed center and axes during the calculations.

		float fRadiusSqr = ( sSphere.m_fRadius * sSphere.m_fRadius );
		float fTotalDist = 0.0f;

		// Get the center in the space of the OBB
		LTVector vTransCenter = ( sSphere.m_vCenter - rOBB.m_vCenter );

		for( uint32 nAxis = 0; nAxis < 3; ++nAxis )
		{
			float fAxisDist = rOBB.m_vAxes[ nAxis ].Dot( vTransCenter );

			// See if it lies outside of either bounding plane, and if so, find the distance
			// in that dimension to our box.
			if( fAxisDist < -rOBB.m_vHalfDims[ nAxis ] )
			{
				float fDist = ( fAxisDist + rOBB.m_vHalfDims[ nAxis ] );
				fTotalDist += ( fDist * fDist );

				// Now if we are outside of the sphere radius, we don't intersect.
				if( fTotalDist > fRadiusSqr )
				{
					return false;
				}
			}
			else if( fAxisDist > rOBB.m_vHalfDims[ nAxis ] )
			{
				float fDist = ( fAxisDist - rOBB.m_vHalfDims[ nAxis ] );
				fTotalDist += ( fDist * fDist );

				// Now if we are outside of the sphere radius, we don't intersect.
				if( fTotalDist > fRadiusSqr )
				{
					return false;
				}
			}
		}

		// The shapes overlap, or touch on an edge!
		return true;
	}

	//----------------------------------------------------------------------------------------------

	bool OBB_Segment( const LTOBB& rOBB, const LTVector& vStart, const LTVector& vEnd, float& fCollisionTime )
	{
		// Offset the segment by the box center
		LTVector vTempS = ( vStart - rOBB.m_vCenter );
		LTVector vTempE = ( vEnd - rOBB.m_vCenter );

		// Now get them into the space of the box
		LTVector vSegS( vTempS.Dot( rOBB.m_vAxes[ 0 ] ), vTempS.Dot( rOBB.m_vAxes[ 1 ] ), vTempS.Dot( rOBB.m_vAxes[ 2 ] ) );
		LTVector vSegE( vTempE.Dot( rOBB.m_vAxes[ 0 ] ), vTempE.Dot( rOBB.m_vAxes[ 1 ] ), vTempE.Dot( rOBB.m_vAxes[ 2 ] ) );


		// Common time tracking variables
		float fTempTimeS, fTempTimeE;
		float fFinalTimeS = 0.0f;
		float fFinalTimeE = 1.0f;


		// Handle collision at each axis separately
		for( uint32 i = 0; i < 3; ++i )
		{
			if( vSegS[ i ] < vSegE[ i ] )
			{
				// Do a fast exclusion check
				if( ( vSegS[ i ] > rOBB.m_vHalfDims[ i ] ) || ( vSegE[ i ] < -rOBB.m_vHalfDims[ i ] ) )
				{
					return false;
				}

				// Calculate the new times
				float fDelta = ( vSegE[ i ] - vSegS[ i ] );
				if( !LTNearlyEquals( fDelta, 0.0f, 0.0001f ))
				{
					fTempTimeS = ( vSegS[ i ] < -rOBB.m_vHalfDims[ i ] ) ? ( ( -rOBB.m_vHalfDims[ i ] - vSegS[ i ] ) / fDelta ) : 0.0f;
					fTempTimeE = ( vSegE[ i ] > rOBB.m_vHalfDims[ i ] ) ? ( ( rOBB.m_vHalfDims[ i ] - vSegS[ i ] ) / fDelta ) : 1.0f;
				}
				else
				{
					fTempTimeS = 0.0f;
					fTempTimeE = 1.0f;
				}
			}
			else
			{
				// Do a fast exclusion check
				if( ( vSegS[ i ] < -rOBB.m_vHalfDims[ i ] ) || ( vSegE[ i ] > rOBB.m_vHalfDims[ i ] ) )
				{
					return false;
				}

				// Calculate the new times
				float fDelta = ( vSegE[ i ] - vSegS[ i ] );
				if( !LTNearlyEquals( fDelta, 0.0f, 0.0001f ))
				{
					fTempTimeS = ( vSegS[ i ] > -rOBB.m_vHalfDims[ i ] ) ? ( ( rOBB.m_vHalfDims[ i ] - vSegS[ i ] ) / fDelta ) : 0.0f;
					fTempTimeE = ( vSegE[ i ] < rOBB.m_vHalfDims[ i ] ) ? ( ( -rOBB.m_vHalfDims[ i ] - vSegS[ i ] ) / fDelta ) : 1.0f;
				}
				else
				{
					fTempTimeS = 0.0f;
					fTempTimeE = 1.0f;
				}
			}

			// Update the final times
			if( fTempTimeS > fFinalTimeS ) fFinalTimeS = fTempTimeS;
			if( fTempTimeE < fFinalTimeE ) fFinalTimeE = fTempTimeE;


			// If the times have passed each other... there is no collision
			if( fFinalTimeS > fFinalTimeE )
			{
				return false;
			}
		}


		// Assign the collision time and return
		fCollisionTime = fFinalTimeS;
		return true;
	}

	//----------------------------------------------------------------------------------------------

	bool Hull_Point( const LTPlane* pPlanes, uint32 nPlanes, const LTVector& vPt )
	{
		// Go through each plane and see if this point is outside
		for( uint32 i = 0; i < nPlanes; ++i )
		{
			if( pPlanes[ i ].DistTo( vPt ) > 0.0f )
			{
				return false;
			}
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------

	bool Hull_Sphere( const LTPlane* pPlanes, uint32 nPlanes, const LTSphere& sSphere )
	{
		// Go through each plane and see if the sphere is outside
		for( uint32 i = 0; i < nPlanes; ++i )
		{
			if( ( pPlanes[ i ].DistTo( sSphere.m_vCenter ) - sSphere.m_fRadius ) > 0.0f )
			{
				return false;
			}
		}

		return true;
	}
}

//----------------------------------------------------------------------------------------------

#endif

