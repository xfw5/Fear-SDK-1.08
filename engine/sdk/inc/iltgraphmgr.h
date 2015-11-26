//--------------------------------------------------------------------------------------
// ILTGraphMgr.h
//
// Provides an interface to the graphing manager, which is a development only tool used
// to handle creating graphs which can have samples, each with multiple values added into
// them by users, and provide a visual representation that clients can view.
//
//--------------------------------------------------------------------------------------
#ifndef __ILTGRAPHMGR_H__
#define __ILTGRAPHMGR_H__

//the handle to the graph type
class CGraph;
typedef CGraph* HGRAPH;
#define INVALID_GRAPH	NULL;

#ifndef __LTMODULE_H__
#	include "ltmodule.h"
#endif

class ILTGraphMgr :
	public IBase
{
public:

	//interface support
    interface_version(ILTGraphMgr, 0);
	declare_interface(ILTGraphMgr);


	ILTGraphMgr()				{}
	virtual ~ILTGraphMgr()		{}

	//----------------------------
	// Graph Creation

	//called to create a new graph object with the associated name. This graph will be keep a
	//memory of the specified number of samples, each one able to store the specified number of
	//unique values. This can fail if there is not enough memory, or if there is already a graph
	//created that shares the same name. This returned graph needs to have a corresponding release
	//call made to it to prevent a resource leak. The graph dimensions (visible samples and height)
	//are restricted to 1024 to prevent memory and performance issues.
	virtual HGRAPH		CreateGraph(const char* pszName, uint32 nNumSamples, uint32 nNumValues,
									uint32 nNumVisibleSamples, uint32 nGraphHeight,
									const char* pszInfoString);

	//called to add a reference to the graph. There needs to be a corresponding release call made
	//to prevent a resource leak
	virtual LTRESULT	AddGraphRef(HGRAPH hGraph);

	//called to release a reference to a graph. The handle passed into this function must not be
	//used after this call
	virtual LTRESULT	ReleaseGraph(HGRAPH hGraph);

	//----------------------------
	// Graph Manipulation

	//called to setup the visible range of a graph to the specified min/max range. This will do nothing
	//if the graph is invalid
	virtual LTRESULT	SetGraphRange(HGRAPH hGraph, float fMin, float fMax);

	//called to push a sample onto a graph. This takes an array of values, and the number of values
	//provided. It will then use those values for the values of the sample, and 0 for any that aren't
	//provided. This will do nothing if the graph is invalid
	virtual LTRESULT	AddGraphSample(HGRAPH hGraph, float* pValues, uint32 nNumValues);

	//called to set the color associated with a value. This color should be created using the
	//SETRGBA macro, and will be ignored if the graph, or value index is invalid
	virtual LTRESULT	SetValueColor(HGRAPH hGraph, uint32 nValueIndex, uint32 nColor);

	//called to set the background color associated with the graph. This color should be created
	//using the SETRGBA macro.
	virtual LTRESULT	SetBackgroundColor(HGRAPH hGraph, uint32 nColor);

	//called to get the average value of a specified value from the graph. This will be zero if the
	//graph or value index is invalid.
	virtual LTRESULT	GetValueAverage(HGRAPH hGraph, uint32 nValueIndex, float& fAverage);

	//called to set whether or not the values for this graph should be rendered as stacked, or as
	//absolute
	virtual LTRESULT	SetGraphValuesStacked(HGRAPH hGraph, bool bStacked);

	//called to set the user value associated with the graph that is displayed with the graph. Typically
	//this is to communicate extra numerical value associated with the graph to the user
	virtual LTRESULT	SetGraphUserValue(HGRAPH hGraph, float fValue);
};

#endif
