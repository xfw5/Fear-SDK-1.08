//------------------------------------------------------------------------------
// ILTPhysicsSim.h
//
// Provides the interface for interacting with the physical simulation run by
// the engine. There are two simulations, one on the server, and another on the
// client. Each one of these simulations are independant of each other.
//
//------------------------------------------------------------------------------
#ifndef __ILTPHYSICSSIM_H__
#define __ILTPHYSICSSIM_H__

//-------------------------------------------------------
// Includes
//-------------------------------------------------------

#ifndef __LTBASEDEFS_H__
#	include "ltbasedefs.h"
#endif

#ifndef __LTMODULE_H__
#	include "ltmodule.h"
#endif

#ifndef __LTPHYSICSGROUP_H__
#	include "ltphysicsgroup.h"
#endif

//-------------------------------------------------------
// Object Handles
//-------------------------------------------------------

//forward declarations
class CPhysicsSimulation;

//handle to a shape. This shape is simply a geometric representation
//of an object, and can be instanced within several rigid bodies
class CPhysicsShape;
typedef CPhysicsShape*				HPHYSICSSHAPE;
#define INVALID_PHYSICS_SHAPE		NULL

//handle to a rigid body. This is actually an instance of a shape that can
//move and interact within a level.
class CPhysicsRigidBody;
typedef CPhysicsRigidBody*			HPHYSICSRIGIDBODY;
#define INVALID_PHYSICS_RIGID_BODY	NULL

//handle to a constraint. This will make sure that two rigid bodies are
//constrained together based upon the constraint type. This can be used
//to attach two rigid bodies together.
class hkConstraint;
typedef hkConstraint*				HPHYSICSCONSTRAINT;
#define INVALID_PHYSICS_CONSTRAINT	NULL

//handle to a breakable constraint. This wraps a normal constraint, giving
//it the ability to be broken once a certain force limit is exceeded.
class CPhysicsBreakableConstraint;
typedef CPhysicsBreakableConstraint*	HPHYSICSBREAKABLE;
#define INVALID_PHYSICS_BREAKABLE		NULL

//handle to a collision notifier, which is an object that allows for game code
//to register for collisions between rigid bodies
class CPhysicsCollisionNotifier;
typedef CPhysicsCollisionNotifier*			HPHYSICSCOLLISIONNOTIFIER;
#define INVALID_PHYSICS_COLLISION_NOTIFIER	NULL

//handle to a physics container, which is a region of space that is used to determine
//what rigid bodies overlap a particular region of space
class CPhysicsContainer;
typedef CPhysicsContainer*			HPHYSICSCONTAINER;
#define INVALID_PHYSICS_CONTAINER	NULL

//handle to a physics action. This will be called each time step and can be associated with
//a rigid body. This will reference the rigid body that it is associated with, and if that
//rigid body is removed from the simulation, the action will be removed from the simulation
//as well.
class CPhysicsAction;
typedef CPhysicsAction*				HPHYSICSACTION;
#define INVALID_PHYSICS_ACTION		NULL

//-------------------------------------------------------
// Callback Prototypes
//-------------------------------------------------------

//this callback is triggered when a breakable constraint is broken. This allows for
//the client to perform operations such as play effects, play sounds, etc.
typedef void (*TFnBreakableBrokenCB)(HPHYSICSBREAKABLE hBreakable, void* pUserData);

//this callback is triggered in response to a collision between two rigid bodies. The first two
//parameters are the rigid bodies that collided, followed by the point at which the collision was
//registered. The collision normal always points such that moving the first object in the direction 
//of the normal separates the bodies. In other words, moving the first object object by -|Normal| 
//units in the direction of the normal will bring the bodies into exact (surface) contact. The 
//direction of the normal does not change if the bodies become penetrating, but the distance will 
//become negative. Thus the normal always points into the first object, and out of the second object.
//The velocity is the relative velocity of the objects colliding, projected onto the collision normal.
//The bIgnoreCollision parameter can be set in the callback to indicate whether or not the collision
//should be ignored. If it is ignored, the objects will be free to move through each other.
typedef void (*TFnRigidBodyCollisionCB)(HPHYSICSRIGIDBODY hBody1, HPHYSICSRIGIDBODY hBody2,
										const LTVector& vCollisionPt, const LTVector& vCollisionNormal,
										float fVelocity, bool& bIgnoreCollision, void* pUser);

//callback for when a physics action is triggered. This provides the rigid body that the action
//was associated with, and also the elapsed time for the simulation update. Within this callback you
//should only perform operations such as applying forces, and NOT remove objects, use the current
//frame time, or add/remove other physical primitives.
typedef void (*TFNPhysicsActionCB)(HPHYSICSRIGIDBODY hBody, float fUpdateTimeS, void* pUserData);

//this is a callback interface used for traversal through shape heirarchies, and can be used to perform
//game side evaluation of physical shapes. This will handle such shapes as transforms and lists behind
//the scenes, and will accumulate transforms appropriately and inform the user of shapes as they are
//encountered in the traversal. 
class IShapeTraversalCallback
{
public:

	virtual ~IShapeTraversalCallback()		{}

	//called when a sphere shape is encountered. This will provide the center of the sphere relative
	//to the transform heirarchy and the radius of that sphere
	virtual void HandleSphere(const LTVector& vCenter, float fRadius, float fMassKg, float fDensityG)							{}

	//called when a capsule shape is encounered. This will provide the two end points of the capsule
	//relative to the transform heirarchy and the radius of the capsule
	virtual void HandleCapsule(const LTVector& vPt1, const LTVector& vPt2, float fRadius, float fMassKg, float fDensityG)		{}

	//called when an OBB shape is encountered. This will provide the transform of the OBB relative to
	//the transform heirarchy and the half dimensions of the OBB
	virtual void HandleOBB(const LTRigidTransform& tTransform, const LTVector& vHalfDims, float fMassKg, float fDensityG)		{}
};

//-------------------------------------------------------
// Constants
//-------------------------------------------------------

//the minimum radius that can be used for a sphere in game units
#define MIN_SPHERE_SHAPE_RADIUS_GU		0.5f

//the minimum radius that can be used for a side of the half dims of a box in game units
#define MIN_BOX_SHAPE_HALF_DIM_GU		0.5f

//the value that should try to be used as a collision radius for different shapes
#define DEFAULT_SHAPE_COLLISION_RADIUS	0.3f

//the default friction value used for rigid bodies
#define DEFAULT_RIGID_BODY_FRICTION		0.5f

//the default coefficient of restitution value used for rigid bodies
#define DEFAULT_RIGID_BODY_COR			0.4f

//-------------------------------------------------------
// ILTPhysicsSim
//-------------------------------------------------------
class ILTPhysicsSim:
	public IBase
{
public:

	//Module support
	interface_version(ILTPhysicsSim, 0);

	virtual ~ILTPhysicsSim();

	//-------------------------------------------------------------------
	// Shape Management
	//
	// This allows for creating and destroying different physics shapes. These
	// shapes can then be used for creating rigid bodies.
	//-------------------------------------------------------------------

	//given the name of a world model, this will get the shape associated with that world model. This
	//shape must be released later in order to avoid a memory leak. If no shape is associated with that
	//world model, or if there is not a world model with that name, an invalid shape handle will be
	//returned. In addition, this will provide the transform relative to the object that maps between
	//the space of the object and the space of the shape. So to get a shape to line up exactly to a world model,
	//either the object that uses this shape should be offset by the provided transform, or an additional
	//transform shape should be made the root of this shape (less optimal). This will be the identity
	//transform if the shape cannot be found.
	virtual HPHYSICSSHAPE GetWorldModelShape(const char* pszWorldModel, LTRigidTransform& tTransform);

	//called to create a spherical shape of the specified radius and mass. This will
	//return an invalid shape if the call fails. The radius must be at least MIN_SPHERE_SHAPE_RADIUS
	virtual HPHYSICSSHAPE CreateSphereShape(float fRadius, float fMassKg, float fDensityG);

	//called to create a box shape with the specified half dimensions and mass. This will
	//return an invalid shape if the call fails. The collision radius is an additional radius
	//around the box that is added to all sides to allow for faster collision resolution. This
	//can be set to zero, but doing so makes the shape more expensive. The dimensions of the box
	//must be at least MIN_BOX_SHAPE_HALF_DIM for each dimension, or it will be increased to that
	//amount
	virtual HPHYSICSSHAPE CreateBoxShape(const LTVector& vHalfDims, float fMassKg, float fDensityG, float fCollisionRadius);

	//called to create a capsule shape that is oriented so the cylinder reaches from the specified
	//end points, and has the specified radius from that line. The radius should be at least one centimeter
	//long and will be expanded to this if it is not. This will return INVALID_PHYSICS_SHAPE if the shape
	//cannot be created
	virtual HPHYSICSSHAPE CreateCapsuleShape(const LTVector& vEndPt1, const LTVector& vEndPt2, float fRadius, float fMassKg, float fDensityG);

	//called to create a transform shape that will transform the provided child shape. This
	//will return an invalid shape should it fail, or if no child shape is provided
	virtual HPHYSICSSHAPE CreateTransformShape(const LTRigidTransform& tTransform, HPHYSICSSHAPE hChildShape);

	//called to create a list shape that can contain a collection of other shapes. This will not include
	//any invalid shapes that are passed in, and will return an invalid shape if the operation
	//fails, or if there are no children
	virtual HPHYSICSSHAPE CreateListShape(uint32 nNumShapes, HPHYSICSSHAPE* pChildren);

	//called to create a bounding volume shape. These are commonly used for optimizations, but allow for
	//a region shape that will be tested before testing the child shape. Some important things to note
	//are that both of these shapes must be valid, and to work properly, the bounding shape should encompass
	//the child shape
	virtual HPHYSICSSHAPE CreateBoundingVolumeShape(HPHYSICSSHAPE hBoundingShape, HPHYSICSSHAPE hChild);

	//called to create a container shape. This will define the region of the container to be the collision
	//volume of the provided shape. This will fail if either of these objects are invalid. Note that this
	//container shape is not actually solid.
	virtual HPHYSICSSHAPE CreateContainerShape(HPHYSICSSHAPE hBoundingShape, HPHYSICSCONTAINER hContainer);

	//given a shape, this will add a reference onto the shape. This should be done if the user
	//will be storing an additional handle to the shape
	virtual LTRESULT AddShapeRef(HPHYSICSSHAPE hShape);

	//given a shape, this will release a reference to the shape. The passed in handle should
	//not be used after this call is made
	virtual LTRESULT ReleaseShape(HPHYSICSSHAPE hShape);

	//given a shape, this will return the total mass of the shape, and all children
	//shapes where appropriate. This will return 0 if the shape is invalid
	virtual LTRESULT GetShapeMass(HPHYSICSSHAPE hShape, float& fMassKg);

	//given a shape, this will return the total volume of the shape, and all children
	//shapes where appropriate. This will return 0 if the shape is invalid
	virtual LTRESULT GetShapeVolume(HPHYSICSSHAPE hShape, float& fVolume);

	//given a shape and a starting transform, this will run through the shape heirarchy that
	//this shape composes, and will accumulate transforms relative to the base transform provided, and
	//for appropriate shapes will call back into the IShapeTraversal interface provided. This will
	//fail if either the shape or provided interface is invalid.
	virtual LTRESULT TraverseShapeHeirarchy(HPHYSICSSHAPE hShape, IShapeTraversalCallback* pICallback,
											const LTRigidTransform& tInitialTransform);

	//-------------------------------------------------------------------
	// Rigid Body Management
	//
	// Handles creation and managing rigid bodies. Each rigid body is an instance
	// of a shape that can be moved around and interact with the rest of the physics
	// simulation.
	//-------------------------------------------------------------------

	//this will create a physically simulated rigid body. This rigid body can be
	//inserted and removed from the physics simulation, and represents a single
	//piece of rigid geometry that can be moved and influenced physically. This can
	//fail for lack of memory, or because the shape is invalid.
	virtual HPHYSICSRIGIDBODY CreateRigidBody(	HPHYSICSSHAPE hShape,
												const LTRigidTransform& tStartAt,
												bool bPinned,
												EPhysicsGroup eGroup,
												uint32 nSystem,
												float fFriction,
												float fCOR);


	//called to add a reference to a rigid body. This should be called whenever an additional
	//handle will be held to this rigid body.
	virtual LTRESULT AddRigidBodyRef(HPHYSICSRIGIDBODY hRigidBody);

	//called to release a rigid body. The provided handle should not be used
	//after this call.
	virtual LTRESULT ReleaseRigidBody(HPHYSICSRIGIDBODY hRigidBody);

	//called to get the shape associated with a rigid body. This shape handle needs to be released.
	//If the rigid body is invalid, the shape handle will be invalid
	virtual LTRESULT GetRigidBodyShape(HPHYSICSRIGIDBODY hRigidBody, HPHYSICSSHAPE& hShape);

	//called to pin or unpin the rigid body. When a rigid body is pinned, it is locked
	//in place. It is important to note that pinning can be overridden if the rigid body
	//is owned by an object. This can occur when FLAG2_RIGIDBODY is changed on the object,
	//or for models, when the physics weight set associated with the model is changed
	virtual LTRESULT PinRigidBody(HPHYSICSRIGIDBODY hRigidBody, bool bPin);

	//called to determine if a rigid body is pinned. If the rigid body is invalid, the result will
	//always be false
	virtual LTRESULT IsRigidBodyPinned(HPHYSICSRIGIDBODY hRigidBody, bool& bPinned);

	//called to enable collisions between this rigid body and other pinned rigid bodies, even if this
	//rigid body is pinned. Pinned->pinned collisions are disabled by default for performance, but if
	//this is important (such as for detecting collisions) this can be called to force those collisions
	//to come through still. This is not distributed to other simulations.
	virtual LTRESULT EnableRigidBodyPinnedCollisions(HPHYSICSRIGIDBODY hRigidBody, bool bEnable);

	//called to teleport the rigid body to the specified location. This is typically not safe
	//and should not be done unless the area it is teleporting to has been checked to see if it
	//is clear
	virtual LTRESULT TeleportRigidBody(HPHYSICSRIGIDBODY hRigidBody, const LTRigidTransform& tTeleportTo);

	//called to get the current position and orientation of the rigid body. This will be the
	//identity if the object is invalid
	virtual LTRESULT GetRigidBodyTransform(HPHYSICSRIGIDBODY hRigidBody, LTRigidTransform& tTransform);

	//called to get the center of mass of the rigid body in world space. This will be a zero vector
	//if the object is invalid.
	virtual LTRESULT GetRigidBodyCenterOfMassInWorld(HPHYSICSRIGIDBODY hRigidBody, LTVector& vCOM);

	//Get the inertia tensor of the rigid body in world space.  This will be a identity
	//if the object is invalid.
	virtual LTRESULT GetRigidBodyInertiaWorld(HPHYSICSRIGIDBODY hRigidBody, LTMatrix3x4& matInertiaWorld);

	//Get the inverse inertia tensor of the rigid body in world space.  This will be a zero vector
	//if the object is invalid.
	virtual LTRESULT GetRigidBodyInertiaInvWorld(HPHYSICSRIGIDBODY hRigidBody, LTMatrix3x4& matInertiaInvWorld);

	//called to access the object associated with this rigid body. This will return NULL if the rigid
	//body was not created as part of either a world model, model, or the main world, or if the provided
	//rigid body is invalid
	virtual LTRESULT GetRigidBodyObject(HPHYSICSRIGIDBODY hRigidBody, HOBJECT& hObject);

	//called to control which physics group and physics system that this rigid body belongs
	//to. This is used to control which other objects that this will collide with
	virtual LTRESULT SetRigidBodyCollisionInfo(HPHYSICSRIGIDBODY hRigidBody, EPhysicsGroup eGroup, uint32 nSystem);

	//called to get information about the physics group that the rigid body belongs to. Both
	//of these will be zero if the rigid body is invalid
	virtual LTRESULT GetRigidBodyCollisionInfo(HPHYSICSRIGIDBODY hRigidBody, EPhysicsGroup& eGroup, uint32& nSystem);

	//called to just set the collision group that the specified rigid body belongs to
	virtual LTRESULT SetRigidBodyCollisionGroup(HPHYSICSRIGIDBODY hRigidBody, EPhysicsGroup eGroup);

	//called to just set the collision group that the specified rigid body belongs to
	virtual LTRESULT SetRigidBodyCollisionSystem(HPHYSICSRIGIDBODY hRigidBody, uint32 nSystem);

	//called to get the phyiscs solid flag.  Rigid bodies with the physics solid flag set to false
	//will behave as if they are part of the NonSolid group.
	virtual LTRESULT GetRigidBodySolid(HPHYSICSRIGIDBODY hRigidBody, bool& bSolid);

	//called to set the phyiscs solid flag.  Rigid bodies with the physics solid flag set to false
	//will behave as if they are part of the NonSolid group.
	virtual LTRESULT SetRigidBodySolid(HPHYSICSRIGIDBODY hRigidBody, bool bSolid);

	//called to set the friction value of this rigid body
	virtual LTRESULT SetRigidBodyFriction(HPHYSICSRIGIDBODY hRigidBody, float fFriction);

	//called to get the friction value of this rigid body. This will be zero if the rigid body is invalid
	virtual LTRESULT GetRigidBodyFriction(HPHYSICSRIGIDBODY hRigidBody, float& fFriction);

	//called to set the coefficient of restitution value of this rigid body
	virtual LTRESULT SetRigidBodyCOR(HPHYSICSRIGIDBODY hRigidBody, float fCOR);

	//called to get the coefficient of restitution value of this rigid body. 
	//This will be zero if the rigid body is invalid
	virtual LTRESULT GetRigidBodyCOR(HPHYSICSRIGIDBODY hRigidBody, float& fCOR);

	//called to set the velocity of the rigid body. This will override any existing velocity on the
	//rigid body, and should only be used in limited cases as this will appear to make the object move
	//in a very different manner suddenly. The velocity magnitude is in cm/s
	virtual LTRESULT SetRigidBodyVelocity(HPHYSICSRIGIDBODY hRigidBody, const LTVector& vVel);

	//called to get the velocity associated with the provided rigid body. The magnitude of this
	//vector is in cm/s, and will be zero if the provided rigid body is invalid
	virtual LTRESULT GetRigidBodyVelocity(HPHYSICSRIGIDBODY hRigidBody, LTVector& vVel);

	//called to set the angular velocity of the rigid body. This will override any existing angular velocity 
	//on the rigid body, and should only be used in limited cases as this will appear to make the object 
	//move in a very different manner suddenly. Each value is in radians/s
	virtual LTRESULT SetRigidBodyAngularVelocity(HPHYSICSRIGIDBODY hRigidBody, const LTVector& vAngVel);

	//called to get the angular velocity associated with the provided rigid body. Each value
	//is in radians/s, and will be zero if the provided rigid body is invalid
	virtual LTRESULT GetRigidBodyAngularVelocity(HPHYSICSRIGIDBODY hRigidBody, LTVector& vAngVel);

	//called to apply a force to a rigid body in world space. The force vector indicates the direction
	//of the force in world space, and the magnitude of that vector indicates the amount of force in
	//game unit Newtons. This will have no effect if the rigid body is invalid.
	virtual LTRESULT ApplyRigidBodyForceWorldSpace(HPHYSICSRIGIDBODY hRigidBody, const LTVector& vApplyAtWS,
													const LTVector& vWSForce);

	//called to apply an impulse to a rigid body in world space. The impulse vector indicates the direction
	//of the impulse in world space, and the magnitude of that vector indicates the amount of force in
	//game unit Newton seconds. This will have no effect if the rigid body is invalid.
	virtual LTRESULT ApplyRigidBodyImpulseWorldSpace(HPHYSICSRIGIDBODY hRigidBody, const LTVector& vApplyAtWS,
														const LTVector& vWSImpulse);

	//called to apply torque (angular force) to a rigid body in world space. The units are in Newton meters
	//around each axis of rotation. This will have no effect if the rigid body is invalid. This is applied
	//around the center of mass of the object.
	virtual LTRESULT ApplyRigidBodyTorque(HPHYSICSRIGIDBODY hRigidBody, const LTVector& vTorqueNm);

	//called to apply an angular impulse to a rigid body. The impulse is applied around the center of mass
	//of the rigid body and is specified in Kg * Rad / s for each axis of rotation. This will have no
	//effect if an invalid rigid body is supplied.
	virtual LTRESULT ApplyRigidBodyAngularImpulse(HPHYSICSRIGIDBODY hRigidBody, const LTVector& vAngImpulse);

	//called to determine if the provided rigid body is active or not. If the rigid body is invalid, the
	//value will be inactive
	virtual LTRESULT IsRigidBodyActive(HPHYSICSRIGIDBODY hRigidBody, bool& bActive);

	//called to create a callback notifier when a specific rigid body collides with another rigid body.
	//This returns a handle that must be released when the callback is no longer needed.
	virtual HPHYSICSCOLLISIONNOTIFIER RegisterCollisionNotifier(HPHYSICSRIGIDBODY hRigidBody,
																TFnRigidBodyCollisionCB pfnCallback,
																void* pUserData);

	//called to release a collision notifier
	virtual LTRESULT ReleaseCollisionNotifier(HPHYSICSCOLLISIONNOTIFIER hNotifier);

	//called to remove the rigid body from the simulation. From this point on the rigid body cannot
	//be added back into the simulation, and it no longer will influence anything such as constraints or
	//receive collisions, although the rigid body will still be around and can have properties changed.
	virtual LTRESULT RemoveRigidBodyFromSimulation(HPHYSICSRIGIDBODY hRigidBody);

	//called to keyframe a rigid body to the specified transform over the specified interval of time.
	//If the specified interval of time is zero, this will just teleport to the provided transform.
	virtual LTRESULT KeyframeRigidBody(HPHYSICSRIGIDBODY hRigidBody, const LTRigidTransform& tTrans, float fTimeInterval);

	//given two rigid bodies, this will find the closest point between the two rigid bodies, and return
	//that point as well as the distance to the caller. This will return an error if the rigid
	//bodies are invalid or if no point is found
	virtual LTRESULT GetClosestPoint(HPHYSICSRIGIDBODY hRigidBody1, HPHYSICSRIGIDBODY hRigidBody2,
									 LTVector& vClosestPt, float& fClosestDist);

	//-------------------------------------------------------------------
	// Constraint Construction
	//
	// For creating all of these constraints, the values are in rigid body space for each rigid body
	// and if a rigid body handle is invalid, it is assumed that the constraint will constrain
	// to the world. These can fail if there is no memory, or if the constraint is constrained
	// to the same object twice. All constraints are created but not placed into the
	// simulation. This must be done manually using the appropriate functions.
	//-------------------------------------------------------------------

	//called to create a ball and socket constraint that will bind the two rigid bodies together at
	//the point specified in world space. 
	virtual HPHYSICSCONSTRAINT CreateBallAndSocket(	HPHYSICSRIGIDBODY hBody1, HPHYSICSRIGIDBODY hBody2, 
													const LTVector& vPivot1, const LTVector& vPivot2);

	//creates a stiff spring constraint that connects the two rigid bodies with a stiff spring between
	//them which will keep them fixed at the appropriate point, but a constant distance apart.
	virtual HPHYSICSCONSTRAINT CreateStiffSpring(	HPHYSICSRIGIDBODY hBody1, HPHYSICSRIGIDBODY hBody2, 
													const LTVector& vPivot1, const LTVector& vPivot2,
													float fDistance);

	//creates a hinge that the two constrained shapes can pivot around. Similar to a ball and socket,
	//but can only rotate around one axis
	virtual HPHYSICSCONSTRAINT CreateHinge( HPHYSICSRIGIDBODY hBody1, HPHYSICSRIGIDBODY hBody2, 
											const LTVector& vPivot1, const LTVector& vPivot2, 
											const LTVector& vPivotDir1, const LTVector& vPivotDir2);

	//creates a limited hinge, which is identical to a normal hinge, but allows limiting to a certain
	//range of angles and a frictional value to slow the rotation. This requires an additional
	//perpendicular vector that is used to give the angles a frame of reference. This perpendicular
	//vector must be perpendicular to the pivot direction, and serves as angle 0. The friction is an
	//empirical value.
	virtual HPHYSICSCONSTRAINT CreateLimitedHinge(HPHYSICSRIGIDBODY hBody1, HPHYSICSRIGIDBODY hBody2,
													const LTVector& vPivot1, const LTVector& vPivot2, 
													const LTVector& vPivotDir1, const LTVector& vPivotDir2,
													const LTVector& vPivotPerp1, const LTVector& vPivotPerp2,
													float fAngleMin, float fAngleMax,
													float fFriction);

	//creates a ragdoll constraint, which is a constraint that has a cone of movement, and is restricted
	//on either side by two restricting cones. The twist axis indicates the primary cone of movement,
	//while the perp axis indicates the facing of the first cone of restriction, and the opposite direction
	//is the facing of the second cone of restriction. The twist angles control how much the constraint
	//should allow the other rigid body to twist around the twist axis.
	virtual HPHYSICSCONSTRAINT CreateRagdoll(HPHYSICSRIGIDBODY hBody1, HPHYSICSRIGIDBODY hBody2,
												const LTVector& vPivot1, const LTVector& vPivot2, 
												const LTVector& vTwist1, const LTVector& vTwist2, 
												const LTVector& vPerp1, const LTVector& vPerp2, 
												float fConeAngleRad,
												float fPosRestrictAngleRad, float fNegRestrictAngleRad,
												float fTwistMinRad, float fTwistMaxRad,
												float fFriction);

	//creates a wheel constraint, which is a constraint that fixes two rigid bodies together at a point,
	//but allows the first rigid body to rotate freely around the rotation axis provided, and also allows
	//the first rigid body to be moved along the suspension axis based upon the provided limits and
	//parameters. Note that suspension min should be less than suspension max, and the range defined
	//by the two should include zero. Also strength and damping should be in the range of [0..1].
	//This function will enforce this.
	virtual HPHYSICSCONSTRAINT CreateWheel(HPHYSICSRIGIDBODY hBody1, HPHYSICSRIGIDBODY hBody2,
												const LTVector& vPivot1, const LTVector& vPivot2, 
												const LTVector& vRotation1, const LTVector& vRotation2, 
												const LTVector& vSuspension2, 
												float fSuspensionMin, float fSuspensionMax,
												float fSuspensionStrength, float fSuspensionDamping);

	//creates a prismatic constraint, which is a constraint that fixes two rigid bodies together at a point,
	//but allows the first rigid body to move freely along a single axis within the range specified, applying
	//friction as the first object moves. Note that the range should include zero. 
	virtual HPHYSICSCONSTRAINT CreatePrismatic(HPHYSICSRIGIDBODY hBody1, HPHYSICSRIGIDBODY hBody2,
												const LTVector& vPivot1, const LTVector& vPivot2, 
												const LTVector& vAxis1, const LTVector& vAxis2, 
												const LTVector& vAxisPerp1, const LTVector& vAxisPerp2, 
												float fMovementMin, float fMovementMax,
												float fFriction);

	//called to add a reference to a constraint. This should be called whenever an additional handle
	//to this constraint is stored
	virtual LTRESULT AddConstraintRef(HPHYSICSCONSTRAINT hConstraint);

	//called to release a constraint reference. The provided handle should not be used after this call
	virtual LTRESULT ReleaseConstraint(HPHYSICSCONSTRAINT hConstraint);

	//called to add a constraint to the simulation. Only constraints that are actually in the simulation
	//will effect rigid bodies.
	virtual LTRESULT AddConstraintToSimulation(HPHYSICSCONSTRAINT hConstraint);

	//called to remove a constraint from the simulation. This means the constraint will no longer
	//effect the associated rigid bodies
	virtual LTRESULT RemoveConstraintFromSimulation(HPHYSICSCONSTRAINT hConstraint);

	//-------------------------------------------------------------------
	// Breakable Constraints
	//
	// Any constraint can be made breakable. By doing so, the constraint can be broken at any point
	// by a force that exceeds the breakable threshold. This allows for say a hanging light to sway
	// when pushed, but break when shot. Each breakable constraint can have a callback hooked into
	// it to allow for notification when the constraint is broken.
	//-------------------------------------------------------------------

	//creates a breakable constraint that wraps the provided constraint. This corresponding constraint
	//will be removed from the simulation if it is in the simulation, and be owned by this breakable
	//constraint. This will return an invalid handle if there is no memory, or if the provided
	//constraint is invalid
	virtual HPHYSICSBREAKABLE CreateBreakable(HPHYSICSCONSTRAINT hConstraint, float fBreakableForce);

	//called to add a reference to a breakable object. his should be called whenever an additional handle
	//to this breakable is stored
	virtual LTRESULT AddBreakableRef(HPHYSICSBREAKABLE hBreakable);

	//called to release a breakable reference. The provided handle should not be used after this call
	virtual LTRESULT ReleaseBreakable(HPHYSICSBREAKABLE hBreakable);

	//called to determine if the breakable constraint is currently broken. This will be set to true
	//if the breakable is invalid
	virtual LTRESULT IsBreakableBroken(HPHYSICSBREAKABLE hBreakable, bool& bBroken);

	//called to set whether or not this breakable is broken or not. This can be used to manually break
	//it, or manually repair it.
	virtual LTRESULT SetBreakableBroken(HPHYSICSBREAKABLE hBreakable, bool bBroken);

	//called to register a callback that will be triggered when the breakable is broken. Only
	//one callback can be associated with a breakable object at a time.
	virtual LTRESULT RegisterBreakableCallback(HPHYSICSBREAKABLE hBreakable, TFnBreakableBrokenCB pfnCallback, void* pUser);

	//called to add a breakable to the simulation. Only breakables that are actually in the simulation
	//will effect rigid bodies.
	virtual LTRESULT AddBreakableToSimulation(HPHYSICSBREAKABLE hBreakable);

	//called to remove a breakable from the simulation. This means the breakable will no longer
	//effect the associated rigid bodies
	virtual LTRESULT RemoveBreakableFromSimulation(HPHYSICSBREAKABLE hBreakable);
	
	//-------------------------------------------------------------------
	// Container Management
	//
	// These functions allow for creating and maintaining containers, which are objects that detect
	// other rigid bodies that are penetrating. These containers can then be bundled in a shape and
	// associated with a rigid body to be placed within a level.
	//-------------------------------------------------------------------

	//called to create a new container object. This can be used to create a container shape, which can
	//then be associated with a rigid body. Then, any objects that the shape overlaps as it moves with
	//the rigid body can be iterated over using the associated functions below. Note that this returned
	//handle must be released to avoid a resource leak.
	virtual HPHYSICSCONTAINER CreateContainer();

	//called to add a reference to the provided physics container. There should be a corresponding release
	//for this additional reference to avoid resource leaks
	virtual LTRESULT AddContainerRef(HPHYSICSCONTAINER hContainer);

	//called to release a reference to the specified physics container. Note that the provided reference
	//should not be used after this call is made
	virtual LTRESULT ReleaseContainer(HPHYSICSCONTAINER hContainer);

	//called to determine the number of rigid bodies that exist within the provided container. This will
	//be zero if the container is invalid
	virtual LTRESULT GetNumRigidBodiesInContainer(HPHYSICSCONTAINER hContainer, uint32& nNumRigidBodies);

	//called to get the list of rigid bodies inside of the container. For each of these objects it will
	//add a reference to the rigid body, so they will need to be released to avoid a resource leak. This
	//will indicate how many it filled in.
	virtual LTRESULT GetRigidBodiesInContainer(HPHYSICSCONTAINER hContainer, HPHYSICSRIGIDBODY* pList, 
												uint32 nListSize, uint32& nNumFilledInObjects);

	//-------------------------------------------------------------------
	// Actions
	//
	// These functions allow for creating and maintaining actions. Actions are created to allow for
	// notification of a single physics step, and in response should apply forces to the appropriate
	// objects. This is triggered after the simulation step has updated collisions for the frame. For
	// more information on what should and should not be done inside of an action, see the comments
	// for the action function prototype.
	//-------------------------------------------------------------------

	//called to create a physics action associated with the provided rigid body. This action can then
	//have a callback associated with it to be notified each frame. This will fail if no valid rigid body
	//is associated. This returned action must be released to avoid a resource leak.
	virtual HPHYSICSACTION	CreateAction(HPHYSICSRIGIDBODY hBody);

	//called to add a reference to an action, and should be called when a new persistant pointer is held
	//onto this action. For each call, a corresponding release call is needed to avoid resource leaks.
	virtual LTRESULT AddActionRef(HPHYSICSACTION hAction);

	//called to release a reference to an action. After this call is made, the provided handle should
	//not be used.
	virtual LTRESULT ReleaseAction(HPHYSICSACTION hAction);

	//called to setup the callback information associated with an action. Setting the callback to NULL
	//will disable any callbacks from being triggered, but the objects will still be around.
	virtual LTRESULT SetActionCallback(HPHYSICSACTION hAction, TFNPhysicsActionCB pfnCallback, void* pUserData);

	//-------------------------------------------------------------------
	// Collision Group Support
	//
	// These functions allow for the game code to control which objects will intersect
	// with other objects. Once all of the collision settings have changed, the
	// function ApplyCollisionGroupChanges should be called to ensure that objects
	// are updated to reflect this new collision state. These collision changes
	// will only affect this simulation and are not sent to the other client/server
	// simulation. Note that for none of these can ePhysicsGroup_Reserved be passed.
	//-------------------------------------------------------------------

	//called to enable or disable collisions between the two specified groups
	virtual LTRESULT EnableCollisions(EPhysicsGroup eGroup1, EPhysicsGroup eGroup2, bool bEnable);

	//called to enable or disable collisions between this group and all of the other groups
	virtual LTRESULT EnableAllCollisions(EPhysicsGroup eGroup, bool bEnable);

	//called to determine if a collision is allowed between the two provided groups. The default will
	//be false if any of the parameters are invalid.
	virtual LTRESULT IsCollisionEnabled(EPhysicsGroup eGroup1, EPhysicsGroup eGroup2, bool& bEnabled);

	//this must be called after collision group changes have been made. This is a very expensive
	//function, and therefore is not implicitly called inside of the other collision group functions,
	//thereby allowing for lots of collision group enabling/disabling and then calling this function
	//once. 
	virtual LTRESULT ApplyCollisionGroupChanges();

	//called to get a unique system ID for the simulation. These will loop after 65k, but typically this shouldn't
	//cause a problem since the odds of two systems encountering each other after 65k other
	//systems have been created is slim to none. This will default to zero if no simulation is around
	virtual LTRESULT GetUniqueSystemGroup(uint32& nID);

	//-------------------------------------------------------------------
	// Global Event Management
	//
	// These functions provide support for controlling global events, such
	// as setting up global listeners for collision events.
	//-------------------------------------------------------------------

	//this will take a callback function and user data and set this up as the global
	//collision event listener. This will be called for every collision event in the world
	//so it should be as efficient as possible. For information on the data provided, see
	//the comments for the callback function. This will overwrite any existing global collision
	//event listener, and it is safe to pass NULL in if you want to completely disable the
	//global collision listener
	virtual LTRESULT SetGlobalCollisionListener(TFnRigidBodyCollisionCB pfnCallback, void* pUserData);

	//-------------------------------------------------------------------
	// Engine Object Physics
	//
	// These functions provide support for accessing the physical properties of the
	// different engine objects.
	//-------------------------------------------------------------------

	//this will step the simulation forward by the specified amount. This will also allow for 
	//suggesting substepping intervals and limiting the number of substeps that should be used. Note
	//that this is not intended for general physics stepping as the main updating will handle that,
	//this is more intended for situations such as settling physics at level startup.
	virtual LTRESULT StepSimulationForward(float fElapsedTime, float fTargetUpdateRate, uint32 nMaxSteps);

	//called to get the rigid body associated with a world model object. This will
	//return an invalid rigid body handle if the object is invalid, not a world model,
	//or doesn't have a rigid body associated with it. This returned handle needs to
	//be released.
	virtual LTRESULT GetWorldModelRigidBody(HOBJECT hWorldModel, HPHYSICSRIGIDBODY& hRigidBody);

	//called to get the number of rigid bodies that a model contains. This will return zero
	//if the object is invalid or if the object is not a model
	virtual LTRESULT GetNumModelRigidBodies(HOBJECT hModel, uint32& nNumRigidBodies);

	//called to get an individual rigid body from a model. This will be invalid if
	//the object is not a valid model or if the index is out of range. This returned handle
	//must be released
	virtual LTRESULT GetModelRigidBody(HOBJECT hModel, uint32 nRigidBody, HPHYSICSRIGIDBODY& hRigidBody);

	//called to get the rigid body for the specified model for the specified node. This will
	//return an invalid handle if either of the provided parameters are invalid.
	virtual LTRESULT GetModelNodeRigidBody(HOBJECT hModel, HMODELNODE hNode, HPHYSICSRIGIDBODY& hRigidBody);

	//given an object, this will run through all of the rigid bodies associated with it, and will
	//set them all to the specified physics group. This is mainly for convenience to avoid having
	//to determine the object type, iterate through all of the rigid bodies, and set the groups
	//explicitly. This will return LT_INVALIDPARAMS if the object is not a type that can have physics
	//shapes, and LT_OK otherwise
	virtual LTRESULT SetObjectPhysicsGroup(HOBJECT hObject, EPhysicsGroup eGroup);

protected:

	//used to provide client/server independant access to the physics simulation that is being used
	virtual CPhysicsSimulation*		GetSimulation() = 0;

};


#endif
