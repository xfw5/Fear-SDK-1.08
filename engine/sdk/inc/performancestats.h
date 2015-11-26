// ----------------------------------------------------------------------- //
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
// ----------------------------------------------------------------------- //

#ifndef __PERFORMANCESTATS_H__
#define __PERFORMANCESTATS_H__

struct SPerformanceStats
{
	// Approximate CPU frequency, in Ghz
	float m_fCPUFrequency;
	// Number of CPUs
	uint32 m_nCPUCount;
	// Amount of main memory available to the CPU, in MB
	uint32 m_nCPUMemory;
	// Maximum pixel shader version support
	float m_fGPUMaxPS;
	// Maximum vertex shader version support
	float m_fGPUMaxVS;
	// Amount of GPU memory available, in MB
	uint32 m_nGPUMemory;
	// PS.1.1 fill rate, in megapixels per second
	float m_fGPUPS11Fill;
	// PS.2.0 fill rate, in megapixels per second (0 if unsupported)
	float m_fGPUPS20Fill;
};

#endif //__PERFORMANCESTATS_H__

