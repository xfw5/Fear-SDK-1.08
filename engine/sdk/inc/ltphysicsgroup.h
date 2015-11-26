//-------------------------------------------------------------------------------------------
//
// LTPhysicsGroup.h
//
// This file contains a listing of all the physics groups that a rigid body object can belong
// to. The engine reserves a certain range and then provides the rest for game code usage. For
// more information on where these should be used, see the interfaces for rigid bodies, and also
// managing the group interactions in the main physics simulations.
//
//-------------------------------------------------------------------------------------------
#ifndef __LTPHYSICSGROUP_H__
#define __LTPHYSICSGROUP_H__

enum EPhysicsGroup
{
	//the first physics group is reserved and cannot be changed
	ePhysicsGroup_Reserved		= 0,
	//objects that are non-solid belong in this group
	ePhysicsGroup_NonSolid,
	//this group contains the main world
	ePhysicsGroup_World,
	//this group contains all world models using MOPP physics
	ePhysicsGroup_MOPPWorldModels,
	//this group contains all world models not using MOPP physics
	ePhysicsGroup_WorldModels,
	//this group contains all models
	ePhysicsGroup_Models,

	//this indicates the beginning of the user groups that can be used by game code
	ePhysicsGroup_UserStart		= 8,
	//the number of physics groups that can actually be used
	ePhysicsGroup_NumUserGroups	= 24,

	//a listing of the user groups so that the can be used directly
	ePhysicsGroup_User0			= 8,
	ePhysicsGroup_User1,
	ePhysicsGroup_User2,
	ePhysicsGroup_User3,
	ePhysicsGroup_User4,
	ePhysicsGroup_User5,
	ePhysicsGroup_User6,
	ePhysicsGroup_User7,
	ePhysicsGroup_User8,
	ePhysicsGroup_User9,
	ePhysicsGroup_User10,
	ePhysicsGroup_User11,
	ePhysicsGroup_User12,
	ePhysicsGroup_User13,
	ePhysicsGroup_User14,
	ePhysicsGroup_User15,	

	//the maximum number of groups that are allowed
	ePhysicsGroup_MaxGroups		= 32,

	//the maximum number of system groups that are allowed
	ePhysicsGroup_MaxSystems	= (1<<16),
};

#endif

