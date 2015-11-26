#ifndef __ILTMODEL_H__
#define __ILTMODEL_H__

#ifndef __LTBASEDEFS_H__
#include "ltbasedefs.h"
#endif

#ifndef __LTMODULE_H__
#include "ltmodule.h"
#endif

class CPhysicsWeightSet;
typedef uint32							HPHYSICSWEIGHTSET;
#define INVALID_PHYSICS_WEIGHT_SET		(0xFFFFFFFF)

/*!
Node Control Data
This structure holds all the data that is passed into the node contol callback
*/
struct NodeControlData
{
	//this is the full transform for the model that this node belongs to
	const LTTransform*			m_pModelTransform;

	//this is the transform of the parent of this node
	const LTRigidTransform*		m_pParentTransform;

	//this specifies the transform from the parent to the child in binding
	//position space
	const LTRigidTransform*		m_pFromParentTransform;

	//this transform holds the current transform of the node. This includes
	//the animation, other previous node controllers, and this is the transform
	//that should be modified and placed in local space
	LTRigidTransform*			m_pNodeTransform;

	//the handle of the model
	HOBJECT				m_hModel;

	//the node that is being modified
	HMODELNODE			m_hNode;
};

typedef void (*NodeControlFn)(const NodeControlData&, void* pUserData);


/*!
The \b ILTModel interface contains functions for manipulating models and animations.

There are two implementations of this interface; one for server, one for client.

Define a holder to get this interface like this:
\code
define_holder_to_instance(ILTModel, your_var, Client);
define_holder_to_instance(ILTModel, your_var, Server);
\endcode
*/

class ILTModel : public IBase {
public:
    interface_version(ILTModel, 0);
		

/*!
\param h
 */

/*! 
\param hObj Model Reference.
\param pSocketName name of the socket to be fetched.
\param hSocket return parameter of socket handle.
\return \b LT_NOTINITIALIZED if the Model Reference is invalid.
		\b LT_NOTFOUND if socket it not found in the model eference.

Method used to get sockets handles from models by name. 

Used for: Models and Animation.
*/
    virtual LTRESULT GetSocket(HOBJECT hObj, const char *pSocketName, HMODELSOCKET &hSocket);

/*! 
\param hObj     Model instance handle.
\param hSocket  Socket instance handle.
\param transform  Transform
\param bWorldSpace What space the transform is in in relation to parent, true is in world space, false the transform
is local to model space. 
\return \b LT_OK if operation succeeded.
		\b LT_ERROR one of these caused the error :
			1. The transform hierarchy has become invalid, by having a transformation that
			   is not invertable.
			2. the socket is invalid.
		\b LT_NOTINITIALIZED if there is no model associated with hObj.

Fill \b pos and \b rot with the current transform for the socket. If
\b bWorldSpace is \b TRUE, then the transform is returned in world
space. Otherwise, the transform is local to the model (based on its
position and rotation). 

  \note This is potentially an expensive process: the model's animation has to be (partially) 
  revaluated for every call to get GetSocketTransform. If this function is going to be called 
  a lot, consider caching the animation data by enabling the transformation cache. 
 
Used for: Models and Animation.  
*/
	virtual LTRESULT GetSocketTransform(HOBJECT hObj, 
                                        HMODELSOCKET hSocket,
                                        LTTransform &transform,
                                        bool bWorldSpace);

/*!
\param hObj		  Model instance reference.
\param NumPieces  return value, representing the number of pieces the model has.
\return \b LT_INVALIDPARAM invalid model instance
		\b LT_OK			operation is done.

	Get the number of pieces a model has.
*/
	virtual LTRESULT GetNumPieces( HOBJECT hObj, uint32 & NumPieces );

/*! 
\param hObj       Model instance reference.
\param pPieceName name of model piece.
\param hPiece     return param handle.
 
\return \b LT_OK is success .
		\b LT_NOTINITIALIZED if there is no model associated with hObj.
		\b LT_NOTFOUND  if the requested piece does not exist.
	

Used for: Models and Animation.
*/
	virtual LTRESULT GetPiece(HOBJECT hObj, 
                              const char *pPieceName,
                              HMODELPIECE &hPiece);

/*! 
\param hObj		Model instance reference.
\param nIndex	Index of model piece.
\param hPiece   return param handle.
 
\return \b LT_OK is success .
		\b LT_NOTINITIALIZED if there is no model associated with hObj.
		\b LT_NOTFOUND  if the requested piece does not exist.
	

Used for: Models and Animation.
*/
	virtual LTRESULT GetPiece(HOBJECT hObj, 
                              uint32 nIndex,
                              HMODELPIECE &hPiece);

/*!
\param hObj				Model instance reference
\param hPiece			Handle to the model piece
\param pPieceName		Return buffer
\param nPieceNameLen	Length of the return buffer


returns the name of the given piece

Used for: Models
*/
	virtual LTRESULT GetPieceName(HOBJECT hObj, 
								  HMODELPIECE &hPiece,
								  char *pPieceName,
								  uint32 nPieceNameLen);

/*! 
\param hObj   Model instance handle.
\param hPiece Piece instance handle.
\param bHidden return parameter on hidden status.
 
\return \b LT_OK is success .
		\b LT_INVALIDPARAMS if hObj is not associated with an OT_MODEL
		


Discover whether a piece on the model is hidden.  Only supports the
first \em 32 pieces in the model.  SetPieceHideStatus() returns
\b LT_NOCHANGE if the piece is already hidden (not an error).

\see SetPieceHideStatus()

Used for: Models and Animation.  
*/
	virtual LTRESULT GetPieceHideStatus(HOBJECT hObj, 
                                        HMODELPIECE hPiece,
                                        bool &bHidden);

/*! 
\param hObj  Model instance handle.
\param hPiece  Model piece instance handle.
\param bHidden true/false param to switch the hidden status of piece.
\return \b LT_OK is success .
		\b LT_INVALIDPARAMS if hObj is not associated with an OT_MODEL
		\b LT_NOCHANGE piece already hidden.

Hide or unhide a piece on the model (they are all unhidden by
default). Only supports the first \em 32 pieces in the model.
SetPieceHideStatus() returns \b LT_NOCHANGE if the piece is already
hidden (not an error).

\see GetPieceHideStatus()

Used for: Models and Animation.  
*/
	virtual LTRESULT SetPieceHideStatus(HOBJECT hObj, 
                                        HMODELPIECE hPiece,
                                        bool bHidden);

/*! 
\param hObj  Model instance handle.
\param fLODDistBias returns the LOD distance bias associated with the model
\return \b LT_OK is success .
		\b LT_INVALIDPARAMS if hObj is not associated with an OT_MODEL

Called to get the LOD distance bias of the provided model. This distance bias is used
when determining the distance that will be used to determine the LOD to render the model
at. Positive values will bias towards lower LODs (further away), and negative values
will bias towards higher resolution LODs. If this function fails, the passed in value
will be set to zero.

\see SetLODDistanceBias()

Used for: Models and Animation.  
*/

	virtual LTRESULT GetLODDistanceBias(HOBJECT hModel, float& fLODDistBias);

/*! 
\param hObj  Model instance handle.
\param fLODDistBias the new LOD distance bias to associate with the model
\return \b LT_OK is success .
		\b LT_INVALIDPARAMS if hObj is not associated with an OT_MODEL

Called to set the LOD distance bias of the provided model. This distance bias is used
when determining the distance that will be used to determine the LOD to render the model
at. Positive values will bias towards lower LODs (further away), and negative values
will bias towards higher resolution LODs. NOTE: This is not currently distributed from
the server object to the client objects.

\see GetLODDistanceBias()

Used for: Models and Animation.  
*/

	virtual LTRESULT SetLODDistanceBias(HOBJECT hModel, float fLODDistBias);

/*! 
\param hObj    Model instance name.
\param pNodeName name of node in the model's model transformation hierarchy.
\param hNode     return param holding a handle to node.
\return \b LT_OK on success 
		\b LT_INVALIDPARAMS if hObj is not an OT_MODEL
		\b LT_NOTINITIALIZED if hObj is not currently associated with a model.
		\b LT_NOTFOUND       if the requested node is not found.

	
Get a handle on named node.

\see GetNodeTransform
Used for: Models and Animation.
*/
	virtual LTRESULT GetNode(HOBJECT hObj, 
                             const char *pNodeName,
                             HMODELNODE &hNode);

	
/*!
\param hObject Model to query.
\param hNode Node to query.
\param pName (return) Buffer to hold name.
\param maxLen Length of buffer.

\return \b LT_INVALIDPARAMS if a parameter is null, else \b LT_OK

Get the name of a model transformation node.

Used for: Models and Animation.
*/
	virtual LTRESULT		GetNodeName(HOBJECT hObj, HMODELNODE hNode, char *name, uint32 maxlen);


/*! 
\param hObj Model instance handle.
\param hNode    Model node handle.
\param transform transform 
\param bWorldSpace 
\return \b LT_OK on success 
		\b LT_INVALIDPARAMS if hObj is not an OT_MODEL
		\b LT_NOTINITIALIZED if hObj is not currently associated with a model.
		\b LT_ERROR if 
			a) The transform hierarchy could not be evaluated for the model
			b) The resultant transformation is invalid.

Get the transformation for a node on a model.  If \b bWorldSpace is \b
TRUE, then the transform is returned in world space. Otherwise, the
transform is local to the model.

\see GetNode

Used for: Models and Animation.  
*/
	virtual LTRESULT GetNodeTransform(HOBJECT hObj,
                                      HMODELNODE hNode,
                                      LTTransform &transform,
                                      bool bWorldSpace);


	
	
/*!
\param hObject Model Instance.
\param hNode Current node. Use \b INVALID_MODEL_NODE to get first node.
\param pNext (return) Next node in list. \b LT_FINISHED means the end of the list.

\return \b LT_FINISHED when there are no more; otherwise, returns \b LT_OK.

Iterate through the nodes of the model. The order of the nodes is as if the 
hierarchy was traversed using pre-order traversal.

\par    Example:

\code
  HMODELNODE hCurNode = INVALID_MODEL_NODE;
  while (interface->GetNextModelNode(hModel, hCurNode, &hCurNode) == LT_OK)
      { ... }
\endcode

Used for: Models and Animation.
*/
	/**/
    virtual LTRESULT GetNextNode(HOBJECT hObject, 
								 HMODELNODE hNode,
								 HMODELNODE &pNext);

/*!
\param hObj ModelInstance handle
\param hNode a handle to the root node.
\return \bLT_INVALIDPARAMS if object is either invalid, or not an OT_MODEL

  GetRootNode returns a handle to the root of the transformation hierarchy for the 
  model. 

  Used for: Models and Animation.
*/

	/**/
	virtual LTRESULT				GetRootNode(HOBJECT hObj, HMODELNODE &hNode);

/*!
\param hObj ModelInstance handle
\param hNode a handle to the node 
\param NumChildren return value of query.
\return \bLT_INVALIDPARAMS if object is either invalid, or not an OT_MODEL

  Returns the number of children this node has. If it is a leaf in the transform hierarchy
  the value will be zero.

Used for: Models and Animation.
*/

	virtual LTRESULT 				GetNumChildren(HOBJECT hObj, HMODELNODE hNode, uint32 &NumChildren );

/*!
\param hObj ModelInstance handle
\param hNode a handle to the parent node 
\param index 
\param NumChildren return value of query.
\return \bLT_INVALIDPARAMS if object is either invalid, or not an OT_MODEL

  GetChild returns a handle to the nth child of parent indicated.

 Used for: Models and Animation.
*/

	virtual LTRESULT 				GetChild(HOBJECT hObj, 
											 HMODELNODE parent, 
											 uint32 index,
											 HMODELNODE &child);

/*!
\param hObj ModelInstance handle
\param hNode a handle to the node 
\param parent a handle to the parent of the node.
\return \bLT_INVALIDPARAMS if object is either invalid, or not an OT_MODEL

	Given a node, return a handle to its parent. The value INVALID_MODEL_NODE will
	be returned if the node is the root node.

 Used for: Models and Animation.
*/

	virtual LTRESULT				GetParent(HOBJECT hObj, HMODELNODE node, HMODELNODE &parent);

/*!
\param hObj ModelInstance handle
\param NumNodes return value.
\return \bLT_INVALIDPARAMS if object is either invalid, or not an OT_MODEL

	Returns the number of transformation nodes the model has.

 Used for: Models and Animation.
*/

	virtual LTRESULT				GetNumNodes( HOBJECT hObj, uint32 &num_nodes);

	/*!
	\param hObj modelinstance handle
	\param iNode which node.
	\param return value: matrix representing bind pose

  Gets the bind pose's tranform for the node.

  Used for: Models and Animation 

	*/
	virtual LTRESULT			GetBindPoseNodeTransform( HOBJECT hObj, HMODELNODE node, LTRigidTransform &tBindSpace );

/*!
\param  hObj        Model to process.
\param  hNode		Node to process.
\param  fn          Function to call.
\param  pUserData   User data.
\return 
	\b LT_INVALIDPARAMS     if hObj is either NULL, or not of type \b OT_MODEL
	\b LT_OK                otherwise

  Set callback that gets the world position of the animation node just after it has
  been evaluated.

\note Your function should return as quickly as possible -- it will be
called often. The NodeControlFn will be called for each node on the
model, allowing you to translate and rotate it. 

\par To disable the NodeControlFn, use the RemoveNodeControlFn functions

Used for: Models and Animation.  
*/
	virtual LTRESULT	AddNodeControlFn( HOBJECT hObj, HMODELNODE hNode, NodeControlFn pFn, void *pUserData);

/*!
\param  hObj        Model to process.
\param  fn          Function to call.
\param  pUserData   User data.
\return 
	\b LT_INVALIDPARAMS     if hObj is either NULL, or not of type \b OT_MODEL
	\b LT_OK                otherwise

  Set a callback node processing for every node in a model. See above AddNodeControlFn
  function for details.

Used for: Models and Animation.  
*/
	virtual LTRESULT	AddNodeControlFn( HOBJECT hObj, NodeControlFn pFn, void *pUserData);

/*!
\param  hObj        Model to have function removed from.
\param  hNode		Node to have function removed from.
\param  fn          Function to remove.
\param  pUserData   User data of function to remove (or NULL for any).
\return 
	\b LT_INVALIDPARAMS     if hObj is either NULL, or not of type \b OT_MODEL
	\b LT_OK                otherwise

  Given a node it will look through its registered callbacks, and upon finding any 
  that matches the specified criteria, it will remove it from the list of callback
  functions.

\note If the pUserData value is NULL, the user data of the callbacks will not be
  checked for matching criteria, but if it is non-null it will be checked.

Used for: Models and Animation.  
*/
	virtual LTRESULT	RemoveNodeControlFn( HOBJECT hObj, HMODELNODE hNode, NodeControlFn pFn, void* pUserData);

/*!
\param  hObj        Model to have function removed from.
\param  fn          Function to remove.
\param  pUserData   User data of function to remove (or NULL for any).
\return 
	\b LT_INVALIDPARAMS     if hObj is either NULL, or not of type \b OT_MODEL
	\b LT_OK                otherwise

  Given a model it will look through its registered callbacks, and upon finding any 
  that matches the specified criteria, it will remove it from the list of callback
  functions.

\note If the pUserData value is NULL, the user data of the callbacks will not be
  checked for matching criteria, but if it is non-null it will be checked.

Used for: Models and Animation.  
*/
	virtual LTRESULT	RemoveNodeControlFn( HOBJECT hObj, NodeControlFn pFn, void* pUserData);


/*!
\param pObj Model Instance handle.
\param fUpdateDelta time in miliseconds.
\return \b LT_INVALIDPARAMS on invalid hObj, otherwise \b LT_OK.

Update the main anim tracker by \b fUpdateDelta seconds (This is useful
for updating the animation on a model outside of a client update.)

Used for: Models and Animation.  
*/
	virtual LTRESULT UpdateMainTracker(HOBJECT hObj, float fUpdateDelta) = 0;

/*!
Get the user dimensions of a model animation (the
dimensions set in ModelEdit).

Used for: Models and Animation.  
*/
    virtual LTRESULT GetModelAnimUserDims(HOBJECT hModel, HMODELANIM hAnim, LTVector *pDims);

/*! 
\param hModel the model that the animation provided came from
\param hAnim the animation to get the length of.
\param length   the length in milliseconds of the animation.
\return \b LT_OK operation succeeded.
		\b LT_INVALIDPARAMS if pModel is null.

Get the length of the specified animation (in milliseconds).

Used for: Models and Animation.
*/
	virtual LTRESULT GetAnimLength(HOBJECT hModel, HMODELANIM hAnim, uint32 &length);

/*! 
\param hModel the model that the animation provided came from
\param hAnim the animation to get the length of.
\param nNumKeyFrames   the number of key frames within the animation
\return \b LT_OK operation succeeded.
		\b LT_INVALIDPARAMS if hModel is null.

Gets the number of key frames associated with the provided animation. This will be set
to zero if the query should fail.

Used for: Models and Animation.
*/
	virtual LTRESULT GetAnimKeyFrames(HOBJECT hModel, HMODELANIM hAnim, uint32 &nNumKeyFrames);

/*! 
\param hModel the model that the animation provided came from
\param hAnim the animation to get the position from.
\param hNode the node to query the animation transform of
\param nKeyFrame the key frame to get the animation transform of
\param tTransform the resulting transform of the node for that frame of the animation
\return \b LT_OK operation succeeded.
		\b LT_INVALIDPARAMS if hModel is null, hAnim is invalid, hNode is invalid, or
							nKeyFrame is not in range.

Given the appropriate information, this will look at the specified animation, at the specified
key frame, and get the transformation of the node at that key frame in that animation in object
space. Note that this call can take a fair amount of time depending upon how deep in the node
hierarchy the node being queried is. The resulting transform will be set to the identity transform
should the call fail.

Used for: Models and Animation.
*/
	virtual LTRESULT GetAnimNodeTransform(	HOBJECT hModel, HMODELANIM hAnim, HMODELNODE hNode, 
											uint32 nKeyFrame, LTRigidTransform& tTransform);

/*! 
\param hModel     Model instance handle.
\param TrackerID  Animation tracker identifier.
\param flags     state flags.
\return \b LT_INVALIDPARAMS on invalid hModel, \b LT_INVALIDVERSION if \em not using SMP networking, otherwise \b LT_OK.

Fills in flags with a combination of the \b MS_ flags in basedefs_de.h.

Used for: Models and Animation.  
*/
	virtual LTRESULT GetPlaybackState(HOBJECT hModel, ANIMTRACKERID TrackerID, 
                                      uint32 &flags);

/*! 
\param hModel     Model instance handle.
\param hModel handle to a model.
\param TrackerID  Animation tracker identifier to be associated with new tracker.
\param bNetwork   Whether or not this tracker should be networked or if it should be client/server only
\return \b LT_INVALIDPARAMS on invalid hModel,
		\b LT_OVERFLOW if there are already \b MAX_TRACKERS_PER_MODEL attached,
		\b LT_INVALIDVERSION if \em not using SMP networking,
		\b LT_ALREADYEXISTS if the TrackerID is already in use for the model instance,
		otherwise \b LT_OK.

Add trackers on the model.

Used for: Models and Animation.
*/
	virtual LTRESULT AddTracker(HOBJECT hModel, ANIMTRACKERID TrackerID, bool bNetwork);

/*! 
\param hModel     Model instance handle.
\param TrackerID  Animation tracker identifier associated with tracker to remove.
\return \b LT_INVALIDPARAMS on invalid hModel or if you try to remove the main tracker, \b LT_INVALIDVERSION if \em not using SMP networking, \b LT_NOTFOUND if TrackerID not in use for the model instance; otherwise \b LT_OK.

Remove trackers on the model.

Used for: Models and Animation.
*/
	virtual LTRESULT RemoveTracker(HOBJECT hModel, ANIMTRACKERID TrackerID);

/*! 
\param hModel     Model instance handle.
\param TrackerID  Animation tracker identifier associated with tracker to blend.
\param fBlend     Blend weight applied to the specified tracker.
\return \b LT_INVALIDPARAMS on invalid hModel, if the fBlend is less than 0 or greater than 1,  or if you try to set the blend on the main tracker, \b LT_NOTFOUND if TrackerID not in use for the model instance; otherwise \b LT_OK.

Sets the blend value for an animation tracker.  

  Notes: 
  1. Blending is used only when interpolation blending is used; is has no effect on additive blends.
  2. Blending is not usable on the main tracker, as it is not blended in.
 
Used for: Models and Animation.
*/
	virtual LTRESULT SetAnimBlend( HOBJECT hModel, ANIMTRACKERID TrackerID, float fBlend );

/*! 
\param hModel     Model instance handle.
\param SourceTrackerID	Animation tracker containing state to be copied.
\param DestTrackerID    Animation tracker to receive state.
\return \b LT_INVALIDPARAMS on invalid hModel, \b \b LT_NOTFOUND if SourceTrackerID or DestTrackerID not in use for the model instance; otherwise \b LT_OK.

Copies the animation state from one animation tracker to another.

Used for: Models and Animation.
*/
	virtual LTRESULT CopyAnimation( HOBJECT hModel, ANIMTRACKERID SourceTrackerID, ANIMTRACKERID DestTrackerID );

/*!
\param hModel		Model instance handle.
\param pAnimName	Name of the animation 
\param anim_index	return value: index of animation in model database.
\return \b LT_INVALIDPARAMS on invalid hModel, or if there is no model database associated with hmodel.
		\b LT_OK
	
Used for: Models and Animation.
*/
	virtual LTRESULT GetAnimIndex( HOBJECT hModel, const char *pAnimName, uint32 & anim_index );

/*!
\param hModel		Model instance handle.
\param dwAnimIndex	Index of animation in model database. 
\param pszBuffer	return value: Buffer for animation name.
\param dwBufferLen	Length of return buffer.
\return \b LT_INVALIDPARAMS on invalid hModel, or if there is no model database associated with hmodel.
		\b LT_NOTFOUND  if the requested animation does not exist.
		\b LT_OK

Used for: Models and Animation.
*/
	virtual LTRESULT GetAnimName( HOBJECT hModel, uint32 dwAnimIndex, char *pszBuffer, uint32 dwBufferLen );

/*! 
\param hModel     Model instance handle.
\param TrackerID  Animation tracker identifier.
\param hAnim    animation handle.
\return \b LT_INVALIDPARAMS on invalid TrackerID, \b LT_INVALIDVERSION if \em not using SMP networking, otherwise \b LT_OK.

Get the current animation for the tracker.

Used for: Models and Animation.
*/
	virtual LTRESULT GetCurAnim(HOBJECT hModel, ANIMTRACKERID TrackerID, 
                                HMODELANIM &hAnim);

/*! 
\param hModel     Model instance handle.
\param TrackerID  Animation tracker identifier.
\param hAnim    Animation Handle
\param bInterpolate Determines whether this animation should start from the initial frame, or should be interpolated into
\return \b LT_INVALIDPARAMS on invalid TrackerID, \b LT_INVALIDVERSION if \em not using SMP networking, otherwise \b LT_OK.

Set the current animation for the tracker.

Used for: Models and Animation.
*/
	virtual LTRESULT SetCurAnim(HOBJECT hModel, ANIMTRACKERID TrackerID, HMODELANIM hAnim, bool bInterpolate);

/*! 
\param hModel     Model instance handle.
\param TrackerID  Animation tracker identifier.
\return \b LT_YES if interpolating, LT_NO if not interpolating.

Get the interpolating setting for the tracker.

Used for: Models and Animation.
*/
	virtual LTRESULT GetIsInterpolating(HOBJECT hModel, ANIMTRACKERID TrackerID);

/*! 
\param hModel     Model instance handle.
\param TrackerID  Animation tracker identifier.
\return \b LT_INVALIDVERSION if \em not using SMP networking, otherwise LT_OK

Resets all animation to first frame.

Used for: Models and Animation.
*/
	virtual LTRESULT ResetAnim(HOBJECT hModel, ANIMTRACKERID TrackerID);

/*!
\param hModel     Model instance handle.
\param TrackerID  Animation tracker identifier.
\return \b LT_NO if not looping, or an error, \b LT_INVALIDVERSION if \em not using SMP networking; otherwise, returns \b LT_YES.

Used for: Models and Animation.
*/
	virtual LTRESULT GetLooping(HOBJECT hModel, ANIMTRACKERID TrackerID);

/*! 
\param hModel     Model instance handle.
\param TrackerID  Animation tracker identifier.
\param bLooping looping flag, \b LT_YES loops, \b LT_NO, stops looping.
\return         \b LT_INVALIDVERSION if \em not using SMP networking, otherwise \b LT_OK 

Set whether the current animation on this tracker is looping.

Used for: Models and Animation.
*/
	virtual LTRESULT SetLooping(HOBJECT hModel, ANIMTRACKERID TrackerID, bool bLooping);

/*! 
\param hModel     Model instance handle.
\param TrackerID  Animation tracker identifier.
\return \b LT_NO if not Playing, or an error, \b LT_INVALIDVERSION if \em not using SMP networking; otherwise, returns \b LT_YES.

Used for: Models and Animation.
*/
	virtual LTRESULT GetPlaying(HOBJECT hModel, ANIMTRACKERID TrackerID);

/*! 
\param hModel     Model instance handle.
\param TrackerID  Animation tracker identifier of choice.
\param bPlaying boolean flag, true sets animtracker to play, false to not play.
\return         \b LT_INVALIDPARAMS if TrackerID is invalid, \b LT_INVALIDVERSION if \em not using SMP networking, otherwise, returns \b LT_OK.
                
Set whether this tracker is playing its animation.

Used for: Models and Animation.
*/
	virtual LTRESULT SetPlaying(HOBJECT hModel, ANIMTRACKERID TrackerID, bool bPlaying);

/*! 
\param hModel     Model instance handle.
\param TrackerID  Animation tracker identifier of choice.
\param length   the length in miliseconds of the animation.
\return \b LT_OK operation succeeded.
		\b LT_INVALIDPARAMS if TrackerID is null.
		\b LT_NOTINITIALIZED if the TrackerID is not currently associated with animation data.
		\b LT_INVALIDVERSION if \em not using SMP networking.

Get the length of the current animation (in milliseconds).

Used for: Models and Animation.
*/
	virtual LTRESULT GetCurAnimLength(HOBJECT hModel, ANIMTRACKERID TrackerID, 
		uint32 &length);

/*! 
\param hModel     Model instance handle.
\param TrackerID  Animation tracker identifier of choice.
\param length   the length in miliseconds of the animatoin.
\return \b LT_OK operation succeeded.
		\b LT_INVALIDPARAMS if TrackerID is null.
		\b LT_NOTINITIALIZED if the TrackerID is not currently associated with animation data.
		\b LT_INVALIDVERSION if \em not using SMP networking.

Get the length of the current animation (in milliseconds).

Used for: Models and Animation.
*/
	virtual LTRESULT GetCurAnimTime(HOBJECT hModel, ANIMTRACKERID TrackerID, 
		uint32 &curTime);

/*! 
\param hModel     Model instance handle.
\param TrackerID  Animation tracker identifier of choice.
\param curTime time in miliseconds
\return \b LT_OK or if TrackerID is invalid
        \b LT_INVALID_PARAMS  if TrackerID is null
		\b LT_NOTINITIALIZED if the TrackerID is not currently associated with animation data
		\b LT_INVALIDVERSION if \em not using SMP networking.

Moves the animation's current time to curTime. 

Used for: Models and Animation.
*/
	virtual LTRESULT SetCurAnimTime(
                           HOBJECT hModel, ANIMTRACKERID TrackerID, 
			               uint32 curTime);

/*!
\param hModel     Model instance handle.
\param TrackerID  Animation tracker identifier
\param rate      a float value indicating the rate modifier on the animation.
\return  \b LT_OK
         \b LT_INVALID_PARAMS. if TrackerID is invalid
		 \b LT_INVALIDVERSION if \em not using SMP networking

Sets the current rate of change between frames. Zero rate would stop the animation. Minus one, runs the animation backwards. This is effects only the rate during updates. 

Functional area: Model Animations.
*/
     virtual LTRESULT SetAnimRate( HOBJECT hModel, ANIMTRACKERID TrackerID, float fRate );

/*!
\param hModel     Model instance handle.
\param TrackerID  Animation tracker identifier
\param rate      a float return value indicating the rate modifier on the animation.
\return  \b LT_INVALID_PARAMS if TrackerID is invalid, \b LT_INVALIDVERSION if \em not using SMP networking; otherwise, returns LT_OK.

Gets the current rate of change between frames. 

Used for: Models and Animation.
*/
     virtual LTRESULT GetAnimRate( HOBJECT hModel, ANIMTRACKERID TrackerID, float &fRate );


/*! 
\param hObj     Model instance.
\param pSetName name of animation weight set.
\param hSet     return reference parameter to weight set.
\return 	    \b LT_NOTINITIALIZED if hObj is invalid, \b LT_OK, success, \b LT_NOTFOUND, weight set not found.

Find the animation blending weight set with the given name.

Used for: Models and Animation.
*/
	virtual LTRESULT FindWeightSet( HOBJECT hObj, const char *pSetName, HMODELWEIGHTSET &hSet);

/*! 
\param hModel     Model instance handle.
\param TrackerID  Animation tracker identifier
\param hSet    Model animation blending weight set handle.
\return \b LT_OK operation succeeded.
		\b LT_INVALIDPARAMS TrackerID is invalid
		\b LT_INVALIDVERSION if \em not using SMP networking

Get the weight set for this tracker.

Used for: Models and Animation.
*/
	virtual LTRESULT GetWeightSet( HOBJECT hModel, ANIMTRACKERID TrackerID, HMODELWEIGHTSET &hSet);

/*! 
\param hModel     Model instance handle.
\param TrackerID  Animation tracker identifier
\param hSet     Model animation blending weight set handle.
\return \b LT_OK operation succeeded.
		\b LT_INVALIDPARAMS TrackerID is invalid
		\b LT_INVALIDVERSION if \em not using SMP networking

Set the animation blending weight set for this tracker.

Used for: Models and Animation.
*/
	virtual LTRESULT SetWeightSet(HOBJECT hModel, ANIMTRACKERID TrackerID, HMODELWEIGHTSET hSet);

/*! 
\param hObj     Model instance.
\param pszName  Name of physics weight set.
\param hSet     return reference parameter to weight set.
\return 	    \b LT_INVALIDPARAMS if hObj is invalid, \b LT_OK, success, \b LT_NOTFOUND, weight set not found.

Find the physics weight set with the given name.

Used for: Models and Animation.
*/
	virtual LTRESULT FindPhysicsWeightSet(HOBJECT hModel, const char* pszName, HPHYSICSWEIGHTSET& hSet);

/*! 
\param hObj     Model instance to install the physics weight set onto
\param hSet		The physics weight set to install onto the model
\return 	    \b LT_INVALIDPARAMS if hObj is invalid, \b LT_OK, success

Sets the specified weight set for the provided model instance. This will update the pinned state
of all the associated rigid bodies and is also used to control the weights that influence the physical
blending of this model. INVALID_PHYSICS_WEIGHT_SET can be specified, in which case it will simply act
as a full rag doll.

Used for: Models and Animation.
*/
	virtual LTRESULT SetPhysicsWeightSet(HOBJECT hModel, HPHYSICSWEIGHTSET hWeightSet);

/*! 
\param hObj     Model instance to install the physics weight set onto
\param hSet		The physics weight set currently installed on the model
\return 	    \b LT_INVALIDPARAMS if hObj is invalid, \b LT_OK, success

Acquires the currently assigned physics weight set of the model

Used for: Models and Animation.
*/
	virtual LTRESULT GetPhysicsWeightSet(HOBJECT hModel, HPHYSICSWEIGHTSET& hWeightSet);

/*! 
\param hModel   Model instance to obtain the physics visibility node of
\param hNode	The model node that represents the center of the physical visibility
\return 	    \b LT_INVALIDPARAMS if hObj is invalid, \b LT_OK, success

Acquires the node that has been designated by the content creators as being the center of
visibility when the model is in a physically simulated state.

Used for: Models and Animation.
*/
	virtual LTRESULT GetPhysicsVisNode(HOBJECT hModel, HMODELNODE& hNode);

/*!
\param hModel The model that should have all of the associated nodes dirtied

This function will tag all nodes associated with a model as being dirty and needing
to be re-evaluated. This should typically only be done when an operation has been
performed that has changed the animation state of the full model and the corresponding
updated transforms need to be either queried or used for rendering
*/
	virtual LTRESULT DirtyAllNodeTransforms(HOBJECT hModel);

/*!
\param hModel The model that should have all of the associated nodes dirtied
\param hNode  The model node that should be marked as dirty

This function will tag the specified model node as being dirty and needing
to be re-evaluated. This should typically only be done when an operation has changed
the animation state associated with a single node and that new state is needed
either for subsequent queries or for rendering. This will also handle
marking all children nodes of this node as needing to be reevaluated. Note that
if all nodes are dirtied, it is much more efficient to call DirtyAllNodeTransforms
that this function multiple times.
*/
	virtual LTRESULT DirtyNodeTransform(HOBJECT hModel, HMODELNODE hNode);

/*!
\param hModel The model that will have the corresponding attachments updated

This function will update all the attachments associated with the model. Typically
this is required when the node transforms have changed through game code control
and now attachments need to be updated to be placed correctly based upon the new
node transforms.
*/
	virtual LTRESULT UpdateModelAttachments(HOBJECT hModel);

/*!
\param hObj Model to query.
\param pFilename (return) Buffer for model filename (can use \b NULL).
\param fileBufLen The \b pFilename buffer length.

\return \b LT_INVALIDPARAMS if the object is not a model.
		\b LT_UNSUPPORTED if called from the client.
		\b LT_OK          otherwise.

Get the model filename.  Initializes \b pFilename to zero
length if the model doesn't have a filename or skin.

  \note This function is only valid on the server.

Used for: Models and Animation.
*/

	virtual LTRESULT GetModelFilename(HOBJECT hObj, char *pRetFilename, uint32 nFileBufLen) = 0;

/*!
\param hObj Model to query.
\param nIndex Index of the material being queried
\param pFilename (return) Buffer for material filename (can use \b NULL).
\param fileBufLen The \b pFilename buffer length.

\return \b LT_INVALIDPARAMS if the object is not a model.
		\b LT_OK          otherwise.

Get the material filename.  Initializes \b pFilename to zero
length if the model doesn't have a material in that range or slot.

Used for: Models and Animation.
*/
	virtual LTRESULT GetMaterialFilename(HOBJECT hObj, uint32 nIndex, char *pRetFilename, uint32 nFileBufLen) = 0 ;

/*!
\param hObj Model to change material of.
\param nIndex Index of the material to be set
\param pFilename The new material filename (can use \b NULL).

\return \b LT_INVALIDPARAMS if the object is not a model.
		\b LT_OK          otherwise.

Set the material filename.  

Used for: Models and Animation.
*/
	virtual LTRESULT SetMaterialFilename(HOBJECT hObj, uint32 nIndex, const char *pFilename) = 0 ;

/*!
\param hModel Model to query the accumulated movement encoding transform of
\param tTransform [out] the accumulated movement encoding transform of the model, identity if the model is invalid

\return \b LT_INVALIDPARAMS if the object is not a model.
		\b LT_OK          otherwise.

Called to query the accumulated movement encoding transform that the model has accumulated. Please note that
this will only be updated if an animation tracker has been setup to act as the movement encoding tracker.

Used for: Models and Animation.
*/
	virtual LTRESULT GetMovementEncodingTransform(HOBJECT hModel, LTRigidTransform& tTransform);

/*!
\param hModel Model to reset the accumulated movement encoding transform of

\return \b LT_INVALIDPARAMS if the object is not a model.
		\b LT_OK          otherwise.

Called to reset the accumulated movement encoding transform. This should be called after updating the object
to reflect the current movement encoding state.

Used for: Models and Animation.
*/
	virtual LTRESULT ResetMovementEncodingTransform(HOBJECT hModel);

/*!
\param hModel Model that owns the animation tracker that should have movement encoding enabled
\param TrackerID The ID of the animation tracker that will be responsible for movement encoding

\return \b LT_INVALIDPARAMS if the object is not a model or if the tracker can't be found
		\b LT_OK          otherwise.

Called to set the tracker with the corresponding ID as the animation tracker that will provide
movement encoding data. This will clear any existing trackers that are already setup for movement encoding.

Used for: Models and Animation.
*/
	virtual LTRESULT SetMovementEncodingTracker(HOBJECT hModel, ANIMTRACKERID TrackerID);

/*!
\param hModel Model to disable movement encoding of

\return \b LT_INVALIDPARAMS if the object is not a model
		\b LT_OK          otherwise.

This will disable movement encoding on all of the trackers associated with this model

Used for: Models and Animation.
*/
	virtual LTRESULT DisableMovementEncoding(HOBJECT hModel);
};



/*!
The \b ILTModelClient interface contains client only functions for 
manipulating models and animations.

Define a holder to get this interface like this:
\code
define_holder(ILTModelClient, your_var);
\endcode
*/

class ILTModelClient : public ILTModel {
public:
    interface_version_derived(ILTModelClient, ILTModel, 0);

/*!
\param hModel       ModelInstance handle.
\param nIndex		Index to the material slot
\param pMaterial	Result handle to the material
\return \b LT_OK

Get's the model material

Used for: Model Rendering.  
*/
    virtual LTRESULT GetMaterial(HOBJECT pObj, uint32 nIndex, HMATERIAL *pMaterial) = 0;


/*!
\param hModel       Model instance handle.
\param nIndex		Index to the material slot
\param hMaterial	Handle to the material
\return \b LT_OK

Set's the model material

Used for: Model Rendering.  
*/
    virtual LTRESULT SetMaterial(HOBJECT pObj, uint32 nIndex, HMATERIAL hMaterial) = 0;
};
		

#endif //! __ILTMODEL_H__
















