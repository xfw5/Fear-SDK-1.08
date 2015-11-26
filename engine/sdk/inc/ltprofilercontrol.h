//------------------------------------------------------------------------------------------
// LTProfilerControl.h
// 
// Provides prototypes for functions that handle interaction with potential profilers that
// can be attached to the engine. These are intended only for development purposes. In order
// to get this to link correctly, if these functions are used, one of the ltprofilercontrol_xxx.cpp
// files must be linked into the project as well.
//
//------------------------------------------------------------------------------------------

#ifndef __LTPROFILERCONTROL_H__
#define __LTPROFILERCONTROL_H__

//called to pause the sampling of the profiler
void	LTPauseProfiler();

//called to resume sampling of the profiler
void	LTResumeProfiler();

#endif
