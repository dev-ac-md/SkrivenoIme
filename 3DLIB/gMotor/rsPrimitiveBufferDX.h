/*****************************************************************************/
/*	File:	PrimitiveBufferDX.h
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	26.12.2002
/*****************************************************************************/
#ifndef __PRIMITIVEBUFFER_H__
#define __PRIMITIVEBUFFER_H__

class BaseMesh;

/*****************************************************************************/
/*	Class:	IndexBuffer
/*	Desc:	wrapper for index buffer functionality
/*****************************************************************************/
class IndexBuffer
{
protected:
	DXIndexBuffer*				dxIB;

	int							maxIdx;
	int							maxByte;
	
	int							curIdx;
	int							curByte;

	static DXDevice*			dev;
	static IndexBuffer*			curIB;

	bool						softwareVP;

public:
	_inl void		SetCurrent( unsigned int baseIndex = 0 );

	void			Release();
	
	virtual bool	Create( DXDevice* pDevice, int nInd ) = 0;
	virtual bool	FillData( WORD* idxPtr, int nInd, BaseMesh& bm ) = 0;

}; // class IndexBuffer

/*****************************************************************************/
/*	Class:	StaticIB
/*	Desc:	
/*****************************************************************************/
class StaticIB : public IndexBuffer
{
public:

	virtual bool Create( DXDevice* pDevice, int nInd );
	virtual bool FillData( WORD* idxPtr, int nInd, BaseMesh& bm );
}; // class StaticIB

/*****************************************************************************/
/*	Class: DynamicIB
/*	Desc:
/*****************************************************************************/
class DynamicIB : public IndexBuffer
{
public:

	virtual bool Create( DXDevice* pDevice, int nInd );
	virtual bool FillData( WORD* idxPtr, int nInd, BaseMesh& bm );
}; // class StaticIB

/*****************************************************************************/
/*	Class: SoftwareProcessIB
/*	Desc:
/*****************************************************************************/
class SoftwareProcessIB : public DynamicIB
{
public:

	virtual bool Create( DXDevice* pDevice, int nInd );
	virtual bool FillData( WORD* idxPtr, int nInd, BaseMesh& bm );
}; // class SoftwareProcessIB

/*****************************************************************************/
/*	Class:	SharedIB
/*	Desc:	
/*****************************************************************************/
class SharedIB : public StaticIB
{
public:

	virtual bool FillData( WORD* idxPtr, int nInd, BaseMesh& bm );
}; // class SharedIB

/*****************************************************************************/
/*	Class:	QuadIB
/*	Desc:
/*****************************************************************************/
class QuadIB : public SharedIB
{
public:

	virtual bool Create( DXDevice* pDevice, int nInd );
}; // class QuadIB

const int c_DefaultStride = 32;

/*****************************************************************************/
/*	Class:	VertexBuffer
/*	Desc:
/*****************************************************************************/
class VertexBuffer
{
protected:
	DXVertexBuffer*				dxVB;
	
	int							maxByte;

	int							curVert;
	int							curByte;

	int							curFVF;
	int							curStride;
	bool						softwareVP;

	static DXDevice*			dev;
	static VertexBuffer*		curVB;

public:
	virtual void	Create( DXDevice* pDevice, int nBytes, VertexFormat vf );
	void			Release();
	virtual void	FillData( void* vert, int nVert, BaseMesh& bm, int newStride ) = 0;
	_inl void		SetCurrent( DWORD fvf, int stride );

private:
	friend class	PrimitiveBuffer;
	
}; // class VertexBuffer

/*****************************************************************************/
/*	Class:	StaticVB
/*	Desc:	Wrapper for static vertex buffers
/*****************************************************************************/
class StaticVB : public VertexBuffer
{
public:
	virtual void	Create( DXDevice* pDevice, int nBytes, VertexFormat vf );
	virtual void	FillData( void* vert, int nVert, BaseMesh& bm, int newStride );


}; // class StaticVB

/*****************************************************************************/
/*	Class:	DynamicVB
/*	Desc:	Wrapper for dynamic vertex buffers
/*****************************************************************************/
class DynamicVB : public VertexBuffer
{
public:
	virtual void	Create( DXDevice* pDevice, int nBytes, VertexFormat vf );
	virtual void	FillData( void* vert, int nVert, BaseMesh& bm, int newStride );

}; // class DynamicVB

/*****************************************************************************/
/*	Class:	SoftwareProcessVB
/*	Desc:	Vertex buffer processed in software
/*****************************************************************************/
class SoftwareProcessVB : public VertexBuffer
{
public:
	virtual void	Create( DXDevice* pDevice, int nBytes, VertexFormat vf );
	virtual void	FillData( void* vert, int nVert, BaseMesh& bm, int newStride );
}; // class SoftwareProcessVB

/*****************************************************************************/
/*	Class:	PrimitiveBuffer
/*	Desc:
/*****************************************************************************/
class PrimitiveBuffer
{
public:
	_inl		PrimitiveBuffer();

protected:
	bool		AdjustVBSize();	
	_inl bool	Draw( BaseMesh& bm, int firstPri, int numPri, int firstVert, int numVert );
	_inl bool	Draw( BaseMesh& bm );
	_inl void	SetVertexFormat( VertexFormat vertF );

	void		AttachVB( VertexBuffer* _vb ) { vb = _vb; }
	void		AttachIB( IndexBuffer* _ib ) { ib = _ib; }
	
	static DWORD				curFVF;

private:
	VertexBuffer*				vb;
	IndexBuffer*				ib;

	DWORD						FVF;

	VertexFormat				vf;
	int							stride;

	friend class				PrimitiveCache;
	friend class				VertexBuffer;
}; // class PrimitiveBuffer

const int c_StaticVBufferBytes			= 65535 * 32;
const int c_DynamicVBufferBytes			= 65535 * 32;

const int c_DynamicBufferInd			= 65535;
const int c_StaticBufferInd				= 65535;
const int c_QuadBufferInd				= 65535;

const int c_IBStride					= 2;

/*****************************************************************************/
/*	Class:	PrimitiveCache
/*	Desc:	Contains and manages primitive buffers
/*****************************************************************************/
class PrimitiveCache
{
public:
	void			Init( DXDevice* pDev );
	void			Shut();

	bool			InvalidateDeviceObjects();
	bool			RestoreDeviceObjects();
	void			SetDevice( DXDevice* pDev ) { pDevice = pDev; }


	_inl bool		Draw( BaseMesh& bm, int firstPri, int numPri, int firstVert, int numVert ); 
	_inl bool		Draw( BaseMesh& bm ); 

protected:

private:

	QuadIB					quadIB;		// static index buffer for quad data
	StaticIB				staticIB;	// static index buffer for any data
	DynamicIB				dynamicIB;  // dynamic index buffer for any data

	StaticVB				staticVB;
	DynamicVB				dynamicVB;

	DWORD					curFVF;

	PrimitiveBuffer			pbQuad		[c_NumVertexTypes];
	PrimitiveBuffer			pbStatic	[c_NumVertexTypes];
	PrimitiveBuffer			pbDynamic	[c_NumVertexTypes];

	PrimitiveBuffer			pbVshStatic;
	PrimitiveBuffer			pbVshDynamic;


	DXDevice*				pDevice;
}; // class PrimitiveCache

_inl DWORD				VertexFormatFVF	( VertexFormat	vf		);
_inl D3DPRIMITIVETYPE	PriTypeDX		( PrimitiveType priType );


#ifdef _INLINES
#include "rsPrimitiveBufferDX.inl"
#endif // _INLINES

#endif // __PRIMITIVEBUFFER_H__
