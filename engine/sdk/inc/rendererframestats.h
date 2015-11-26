//------------------------------------------------------------------------------------------
// RendererFrameStats.h
//
//    Defines the CRendererFrameStats object which is used during rendering to accumulate
// frame statistics which can then be used by the user for dynamically controlling LOD of
// the scene or reporting feedback to the user.
//
//------------------------------------------------------------------------------------------
#ifndef __RENDERERFRAMESTATS_H__
#define __RENDERERFRAMESTATS_H__


//The listing of the different types of renderer frame statistics available
enum	ERendererFrameStats	{	
								//texture memory amounts
								eFS_VidMemory_Textures,
								eFS_VidMemory_TexturesUncompressed,
								eFS_VidMemory_SystemTextures,
								eFS_VidMemory_ScreenBuffers,
								eFS_VidMemory_RenderTargets,
								eFS_VidMemory_ApproxAvailable,
								eFS_VidMemory_VertexBuffers,
								eFS_VidMemory_IndexBuffers,
								
								//mesh rendering counts
								eFS_NumMeshes_World,
								eFS_NumMeshes_WorldShadow,
								eFS_NumMeshes_Model,
								eFS_NumMeshes_ModelShadow,
								eFS_NumMeshes_WorldModel,
								eFS_NumMeshes_WorldModelShadow,
								eFS_NumMeshes_CustomRender,

								//world rendering counts
								eFS_WorldRender_SkyPortals,

								//rendering counts
								eFS_Render_MaterialChanges,
								eFS_Render_MaterialInstanceChanges,
								eFS_Render_PassChanges,
								eFS_Render_ObjectChanges,
								eFS_Render_RenderCalls,
								eFS_Render_BatchedMeshes,
								eFS_Render_MissedBatchedMeshes,
								
								//triangle counts
								eFS_Triangles_Model,
								eFS_Triangles_ModelShadow,
								eFS_Triangles_World,
								eFS_Triangles_WorldShadow,
								eFS_Triangles_WorldModel,
								eFS_Triangles_WorldModelShadow,
								eFS_Triangles_CustomRender,

								//visibility
								eFS_Vis_SectorsVisible,
								eFS_Vis_ObjectsVisible,
								eFS_Vis_TotalSectorsVisited,
								eFS_Vis_TotalPortalsTested,
								eFS_Vis_TotalPortalsVisible,
								eFS_Vis_TotalObjectsTested,
								eFS_Vis_TotalObjectsVisible,
								eFS_Vis_VisQueries,
								
								//light counts
								eFS_Light_PointLights,
								eFS_Light_PointLightShadows,
								eFS_Light_PointFillLights,
								eFS_Light_SpotProjectors,
								eFS_Light_SpotProjectorShadows,
								eFS_Light_CubeProjectors,
								eFS_Light_CubeProjectorShadows,
								eFS_Light_Directionals,
								eFS_Light_DirectionalShadows,
								eFS_Light_BlackLights,
								eFS_Light_BlackLightShadows,

								//object counts
								eFS_VisObjects_WorldModels,
								eFS_VisObjects_Models,
								eFS_VisObjects_CustomRender,

								//Total count of frame stats, this must come last
								eFS_NumFrameStats
							};

class CRendererFrameStats
{
public:

	//lifetime operations
	CRendererFrameStats() : m_nFrameCode(0)	{ ClearFrameStats(); }
	~CRendererFrameStats()	{}

	//called to reset all the frame statistics to be zero, and updates the frame code so that
	//unique values will be added again
	void ClearFrameStats() 
	{
		memset(m_nFrameStat, 0, sizeof(m_nFrameStat));
		m_nFrameCode++;
	}

	//called to access an individual frame stat
	int32 GetFrameStat(ERendererFrameStats eFS) const
	{
		return m_nFrameStat[eFS];
	}

	//called to specify a value for a frame stat
	void SetFrameStat(ERendererFrameStats eFS, int32 nVal)
	{
		m_nFrameStat[eFS] = nVal;
	}

	void IncFrameStat(ERendererFrameStats eFS, int32 nInc)
	{
		m_nFrameStat[eFS] += nInc;
	}

	//called to access the frame code associated with these frame statistics
	uint32	GetFrameCode() const			
	{ 
		return m_nFrameCode;
	}

	//----------------------------------
	// Operators

	//operators for accumulating multiple frame stat objects together
	CRendererFrameStats operator+(const CRendererFrameStats& rhs) const
	{
		CRendererFrameStats rv;
		for(uint32 nCurrFrameStat = 0; nCurrFrameStat < eFS_NumFrameStats; nCurrFrameStat++)
		{
			rv.m_nFrameStat[nCurrFrameStat] = m_nFrameStat[nCurrFrameStat] + rhs.m_nFrameStat[nCurrFrameStat];
		}
		return rv;
	}

	CRendererFrameStats operator-(const CRendererFrameStats& rhs) const
	{
		CRendererFrameStats rv;
		for(uint32 nCurrFrameStat = 0; nCurrFrameStat < eFS_NumFrameStats; nCurrFrameStat++)
		{
			rv.m_nFrameStat[nCurrFrameStat] = m_nFrameStat[nCurrFrameStat] - rhs.m_nFrameStat[nCurrFrameStat];
		}
		return rv;
	}

	CRendererFrameStats& operator+=(const CRendererFrameStats& rhs)
	{
		//just levarage the operator + implementation
		*this = *this + rhs;
		return *this;
	}

	CRendererFrameStats& operator-=(const CRendererFrameStats& rhs)
	{
		//just levarage the operator - implementation
		*this = *this - rhs;
		return *this;
	}

private:

	//the frame code of this frame stat object. This is used to add unique values only once. This
	//is most often used when tracking texture memory, as each texture should only be added once,
	//and therefore must use this frame code to determine when it should be added again. This will
	//be incremented once each time the stats are cleared.
	uint32		m_nFrameCode;

	//the array of actual frame statistics
	int32		m_nFrameStat[eFS_NumFrameStats];
};

#endif
