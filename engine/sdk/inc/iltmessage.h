/*!  This file defines the ILTMessage_* classes, which handle
reading/writing of messages.  */

#ifndef __ILTMESSAGE_H__
#define __ILTMESSAGE_H__


#ifndef __ILTBASEDEFS_H__
#include "ltbasedefs.h"
#endif

#ifndef __STDARG_H__
#include <stdarg.h>
#define __STDARG_H__
#endif

#ifndef __ILTREFCOUNT_H__
#include "iltrefcount.h"
#endif

#ifndef __IDATABASEMGR_H__
#include "idatabasemgr.h"
#endif

#ifndef __ILTTIMER_H__
#include "ilttimer.h"
#endif

class ILTMessage_Read;

class ILTMessage_Write : public ILTRefCount {
public:
	// Clear the message and start over
	virtual void Reset() = 0;

	// Return an ILTMessage_Read initialized with this message's data
	// Note : This will reset the message
	virtual ILTMessage_Read *Read() = 0;

	// Number of bits which have been written to this message
	virtual uint32 Size() const = 0;

	// Basic data writing functions
	// Note : nSize is in bits
	virtual void WriteBits(uint32 nValue, uint32 nSize) = 0;
	virtual void WriteBits64(uint64 nValue, uint32 nSize) = 0;
	virtual void WriteData(const void *pData, uint32 nSize) = 0;

	// Complex data type writing functions
	virtual void WriteMessage(const ILTMessage_Read *pMsg) = 0;
	// Exactly like WriteMessage, only without the size indicator.  (So it can't be read with ReadMessage.)
	virtual void WriteMessageRaw(const ILTMessage_Read *pMsg) = 0;
	virtual void WriteString(const char *pString) = 0;
	virtual void WriteWString(const wchar_t *pwString) = 0;
	virtual void WriteCompLTVector(const LTVector &vVec) = 0;
	virtual void WriteCompPos(const LTVector &vPos) = 0;
	virtual void WriteCompLTRotation(const LTRotation &cRotation) = 0;
	virtual void WriteObject(HOBJECT hObj) = 0;
	virtual void WriteYRotation(const LTRotation &cRotation) = 0;
	virtual void WriteDatabaseRecord( IDatabaseMgr *pDatabase, HRECORD hRecord ) = 0;
	virtual void WriteTimer(HENGINETIMER hTimer) = 0;
	virtual void WriteCompLTPolarCoord(const LTPolarCoord &vVec) = 0;

	// Convenience functions
	void Writebool(bool bValue) { WriteBits(bValue ? 1 : 0, 1); }
	void Writeuint8(uint8 nValue) { WriteBits(nValue, 8); }
	void Writeuint16(uint16 nValue) { WriteBits(nValue, 16); }
	void Writeuint32(uint32 nValue) { WriteBits(nValue, 32); }
	void Writeuint64(uint64 nValue) { WriteBits64(nValue, 64); }
	void Writeint8(int8 nValue) { WriteBits((uint32)nValue, 8); }
	void Writeint16(int16 nValue) { WriteBits((uint32)nValue, 16); }
	void Writeint32(int32 nValue) { WriteBits((uint32)nValue, 32); }
	void Writeint64(int32 nValue) { WriteBits64((uint64)nValue, 32); }
	void Writefloat(float fValue) { WriteBits(reinterpret_cast<const uint32&>(fValue), 32); }
	void Writedouble(double fValue) { WriteBits64(reinterpret_cast<const uint64&>(fValue), 64); }
	void WriteLTVector2(const LTVector2 &vValue) { WriteType(vValue); }
	void WriteLTVector(const LTVector &vValue) { WriteType(vValue); }
	void WriteLTVector4(const LTVector4 &vValue) { WriteType(vValue); }
	void WriteLTRotation(const LTRotation &cValue) { WriteType(cValue); }
	void WriteLTRigidTransform(const LTRigidTransform &tValue) { WriteType(tValue); }
	void WriteLTTransform(const LTTransform &tValue) { WriteType(tValue); }
	void WriteLTRect2f(const LTRect2f &rValue) { WriteType(rValue); }
	void WriteLTRect3f(const LTRect3f &rValue) { WriteType(rValue); }
	void WriteHMODELNODE(const HMODELNODE hNode) { WriteType(hNode); }
	void WriteHMODELANIM(const HMODELANIM hAnim) { WriteType(hAnim); }
	void WriteLTPolarCoord(const LTPolarCoord &polarVec) { WriteType(polarVec); }
	template <class T>
	void WriteType(const T &tValue) { 
		switch (sizeof(T))
		{
			case 1 : Writeuint8(reinterpret_cast<const uint8 &>(tValue)); break;
			case 2 : Writeuint16(reinterpret_cast<const uint16 &>(tValue)); break;
			case 4 : Writeuint32(reinterpret_cast<const uint32 &>(tValue)); break;
			case 8 : Writeuint64(reinterpret_cast<const uint64 &>(tValue)); break;
			default : WriteData(&tValue, sizeof(T) * 8); break;
		}
	}
};

typedef CLTReference<ILTMessage_Write> CLTMsgRef_Write;

class ILTMessage_Read : public ILTRefCount {
public:
	// Create a duplicate of this message reading object
	virtual ILTMessage_Read *Clone() const = 0;
	// Create a sub-message of this message starting at nPos
	virtual ILTMessage_Read *SubMsg(uint32 nPos) const = 0;
	// Create a sub-message of this message starting at nPos with a length of nLength
	virtual ILTMessage_Read *SubMsg(uint32 nPos, uint32 nLength) const = 0;

	// Number of bits this message contains	
	virtual uint32 Size() const = 0;
	// Seek by a number of bits in the message relative to the current read position
	virtual void Seek(int32 nOffset) = 0;
	// Seek to a position in the message
	virtual void SeekTo(uint32 nPos) = 0;
	// Where is the read position in the message?
	virtual uint32 Tell() const = 0;
	// Where is the read position in the message relative to the end?
	virtual uint32 TellEnd() const = 0;
	// Is the read position at the end of the message?
	virtual bool EOM() const = 0;

	// Basic data reading functions
	virtual uint32 ReadBits(uint32 nBits) = 0;
	virtual uint64 ReadBits64(uint32 nBits) = 0;
	virtual void ReadData(void *pData, uint32 nBits) = 0;

	// Complex data type reading functions
	virtual ILTMessage_Read *ReadMessage() = 0;
	// Returns the full length of the string
	virtual uint32 ReadString(char *pDest, uint32 nMaxLen) = 0;
	virtual uint32 ReadWString(wchar_t *pwDest, uint32 nMaxLen) = 0;
	virtual LTVector ReadCompLTVector() = 0;
	virtual LTVector ReadCompPos() = 0;
	virtual LTRotation ReadCompLTRotation() = 0;
    virtual HOBJECT ReadObject() = 0;
	virtual LTRotation ReadYRotation() = 0;
	virtual HRECORD ReadDatabaseRecord( IDatabaseMgr *pDatabase, HCATEGORY hCategory ) = 0;
	virtual LTPolarCoord ReadCompLTPolarCoord() = 0;
	virtual HENGINETIMER ReadTimer() = 0;

	// Convenience functions
	bool Readbool() { return ReadBits(1) != 0; }
	uint8 Readuint8() { return (uint8)ReadBits(8); }
	uint16 Readuint16() { return (uint16)ReadBits(16); }
	uint32 Readuint32() { return (uint32)ReadBits(32); }
	uint64 Readuint64() { return (uint64)ReadBits64(64); }
	int8 Readint8() { return (int8)ReadBits(8); }
	int16 Readint16() { return (int16)ReadBits(16); }
	int32 Readint32() { return (int32)ReadBits(32); }
	int64 Readint64() { return (int64)ReadBits64(64); }
	float Readfloat() { uint32 nTemp = Readuint32(); return reinterpret_cast<const float&>(nTemp); }
	double Readdouble() { uint64 nTemp = Readuint64(); return reinterpret_cast<const double&>(nTemp); }
	LTVector2 ReadLTVector2() { LTVector2 vResult; ReadType(&vResult); return vResult; }
	LTVector ReadLTVector() { LTVector vResult; ReadType(&vResult); return vResult; }
	LTVector4 ReadLTVector4() { LTVector4 vResult; ReadType(&vResult); return vResult; }
	LTRotation ReadLTRotation() { LTRotation cResult; ReadType(&cResult); return cResult; }
	LTRigidTransform ReadLTRigidTransform() { LTRigidTransform tResult; ReadType(&tResult); return tResult; }
	LTTransform ReadLTTransform() { LTTransform tResult; ReadType(&tResult); return tResult; }
	LTRect2f ReadLTRect2f() { LTRect2f rResult; ReadType(&rResult); return rResult; }
	LTRect3f ReadLTRect3f() { LTRect3f rResult; ReadType(&rResult); return rResult; }
	LTVector ReadLTPolarCoord() { LTPolarCoord polarResult; ReadType(&polarResult); return polarResult; }
	HMODELNODE ReadHMODELNODE() { HMODELNODE hResult; ReadType(&hResult); return hResult; }
	HMODELANIM ReadHMODELANIM() { HMODELANIM hResult; ReadType(&hResult); return hResult; }
	template <class T>
	void ReadType(T *pValue) { 
		switch (sizeof(T))
		{
			case 1 : { uint8 nTemp = Readuint8(); *pValue = reinterpret_cast<const T &>(nTemp); break; }
			case 2 : { uint16 nTemp = Readuint16(); *pValue = reinterpret_cast<const T &>(nTemp); break; }
			case 4 : { uint32 nTemp = Readuint32(); *pValue = reinterpret_cast<const T &>(nTemp); break; }
			case 8 : { uint64 nTemp = Readuint64(); *pValue = reinterpret_cast<const T &>(nTemp); break; }
			default : { ReadData(pValue, sizeof(T) * 8); break; }
		}
	}

	// Basic data peeking functions
	virtual uint32 PeekBits(uint32 nBits) const = 0;
	virtual uint64 PeekBits64(uint32 nBits) const = 0;
	virtual void PeekData(void *pData, uint32 nBits) const = 0;

	// Complex data type peeking functions
	virtual ILTMessage_Read *PeekMessage() const = 0;
	// Returns the full length of the string
	virtual uint32 PeekString(char *pDest, uint32 nMaxLen) const = 0;
	virtual uint32 PeekWString(wchar_t *pwDest, uint32 nMaxLen) const = 0;
	virtual LTVector PeekCompLTVector() const = 0;
	virtual LTVector PeekCompPos() const = 0;
	virtual LTRotation PeekCompLTRotation() const = 0;
	virtual HOBJECT PeekObject() const = 0;
	virtual LTRotation PeekYRotation() const = 0;
	virtual HRECORD PeekDatabaseRecord( IDatabaseMgr *pDatabase, HCATEGORY hCategory ) = 0;

	// Convenience functions
	bool Peekbool() const { return PeekBits(1) != 0; }
	uint8 Peekuint8() const { return (uint8)PeekBits(8); }
	uint16 Peekuint16() const { return (uint16)PeekBits(16); }
	uint32 Peekuint32() const { return (uint32)PeekBits(32); }
	uint64 Peekuint64() const { return (uint64)PeekBits64(64); }
	int8 Peekint8() const { return (int8)PeekBits(8); }
	int16 Peekint16() const { return (int16)PeekBits(16); }
	int32 Peekint32() const { return (int32)PeekBits(32); }
	int64 Peekint64() const { return (int64)PeekBits64(64); }
	float Peekfloat() const { uint32 nTemp = Peekuint32(); return reinterpret_cast<const float&>(nTemp); }
	double Peekdouble() const { uint64 nTemp = Peekuint64(); return reinterpret_cast<const double&>(nTemp); }
	LTVector PeekLTVector() const { LTVector vResult; PeekType(&vResult); return vResult; }
	LTRotation PeekLTRotation() const { LTRotation cResult; PeekType(&cResult); return cResult; }
	template <class T>
	void PeekType(T *pValue) const { 
		switch (sizeof(T))
		{
			case 1 : { uint8 nTemp = Peekuint8(); *pValue = reinterpret_cast<const T &>(nTemp); break; }
			case 2 : { uint16 nTemp = Peekuint16(); *pValue = reinterpret_cast<const T &>(nTemp); break; }
			case 4 : { uint32 nTemp = Peekuint32(); *pValue = reinterpret_cast<const T &>(nTemp); break; }
			case 8 : { uint64 nTemp = Peekuint64(); *pValue = reinterpret_cast<const T &>(nTemp); break; }
			default : { PeekData(pValue, sizeof(T) * 8); break; }
		}
	}
};

typedef CLTReference<ILTMessage_Read> CLTMsgRef_Read;

#endif //! __ILTMESSAGE_H__

