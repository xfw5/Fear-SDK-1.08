/*! The ILTTexture interface allows the game to modify textures. */

#ifndef __ILTTEXTUREMGR_H__
#define __ILTTEXTUREMGR_H__

#ifndef __LTBASETYPES_H__
#include "ltbasetypes.h"
#endif

#ifndef __LTMODULE_H__
#include "ltmodule.h"
#endif

//the handle to refer to dynamic texture resources
class CDynamicTexture;
typedef CDynamicTexture*	HDYNAMICTEXTURE;

//forward declarations
class CTextureData;
class ILTInStream;

/*!  
The ILTTexInterface interface exposes texture manipulation functionality 
to the game code. It enables direct access to texture data. This 
direct access means that the texture won't be converted to a new format, 
and will therefore result in faster texture rendering.

Define a holder to get this interface like this:
\code
define_holder(ILTTexInterface, your_var);
\endcode
*/

class ILTTextureMgr : public IBase {
public:
    //set the interface version number
    interface_version(ILTTextureMgr, 0);

/*!
\param hTexture     The address of the converted texture.
\param pFilename    Points to a char array containing the relative path to the resource directory in which the .DTX file is located.

\return \b LT_MISSINGFILE - \em pFilename is invalid (can't find file).
\return \b LT_ERROR - Internal error.
\return \b LT_OK - Successful.

Creates a texture from a .DTX file. A .DTX file is created using the WorldEdit tool (see the Game Content Creation Guide).
If the .DTX file was created with the DTX_32BITSYSCOPY bit set, the texture will not be converted (the system memory copy will remain RGBA_8888).

Used for: Special FX.
*/
    virtual LTRESULT CreateTextureFromFile(HTEXTURE &hTexture, const char *pFilename) = 0;

/*!
\param hTexture     Texture handle.
\param nWidth       (return) Texture width.
\param nHeight      (return) Texture height.

\return \b LT_OK - Successful.

Get information about the texture.
        
Used for: Special FX.
*/
    virtual LTRESULT GetTextureDims(HTEXTURE hTexture, uint32 &nWidth, uint32 &nHeight) = 0;

/*!
\param hTexture     Texture handle.
\param nWidth       (return) Texture width.
\param nHeight      (return) Texture height.

\return \b LT_OK - Successful.

Get information about the texture.

Used for: Special FX.
*/
	virtual LTRESULT GetTextureDims(HDYNAMICTEXTURE hTexture, uint32 &nWidth, uint32 &nHeight) = 0;

/*!
\param hTexture     Texture handle.

\return \b LT_OK - Successful.

You should call ReleaseTextureHandle after you are done with your texture
so that it can be freed.  If it is not also being used by the engine, it 
will be freed.

Used for: Special FX.
*/
    virtual LTRESULT ReleaseTexture(HTEXTURE hTexture) = 0;

/*!
\param hTexture		Texture handle

\return \b Reference count of texture

Call AddRefTextureHandle to artificially increment the reference count of a texture.

Used for: Special FX.
*/
	virtual LTRESULT AddTextureRef(HTEXTURE hTexture) = 0;

/*!
\param hTexture                 (return) Dynamic texture handle.
\param TextureData				The texture data that should be used to create the dynamic texture

\return \b LT_ERROR - Texture could not be created.
\return \b LT_OK - Successful.
        
Creates a dynamic texture from the passed in data.

Used for: Special FX.
*/
    virtual LTRESULT CreateDynamicTexture(HDYNAMICTEXTURE &hTexture, const CTextureData& TextureData) = 0;

/*!
\param hTexture             (return) Dynamic texture handle.
\param pInFile				File stream containing the texture data that should be used to create the dynamic texture

\return \b LT_ERROR - Texture could not be created.
\return \b LT_OK - Successful.

Creates a dynamic texture from the passed in data.

*/
	virtual LTRESULT CreateDynamicTexture(HDYNAMICTEXTURE &hTexture, ILTInStream* pInFile) = 0;


/*!
\param hTexture     Texture handle.

\return \b LT_OK - Successful.

You should call this function after you are done with your texture
so that it can be freed.  If it is not also being used by the engine, it 
will be freed.

Used for: Special FX.
*/
    virtual LTRESULT ReleaseDynamicTexture(HDYNAMICTEXTURE hTexture) = 0;

/*!
\param hTexture		Texture handle

\return \b Reference count of texture

Call AddRefTextureHandle to artificially increment the reference count of a texture.

Used for: Special FX.
*/
	virtual LTRESULT AddDynamicTextureRef(HDYNAMICTEXTURE hTexture) = 0;



};

#endif  //! __ILTTEXINTERFACE_H__
