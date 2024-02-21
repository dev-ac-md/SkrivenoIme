/*****************************************************************************/
/*	File:	rsTextureDX.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	29.01.2003
/*****************************************************************************/

/*****************************************************************************/
/*	HashedTexture implementation
/*****************************************************************************/
_inl HashedTexture::HashedTexture( const char* texName )
{
	name		= texName;
	iTexture	= NULL;
	iSurface0	= NULL;
} // HashedTexture::HashedTexture

_inl HashedTexture::HashedTexture()
{
	iTexture	= NULL;
	iSurface0	= NULL;
} // HashedTexture::HashedTexture

_inl unsigned int HashedTexture::hash() const
{
	return hash0( (BYTE*)name.c_str(), name.size() );
} // HashedTexture::hash

_inl bool HashedTexture::equal( const HashedTexture& el )
{
	return (name == el.name );
} // HashedTexture::equal
	
_inl void HashedTexture::copy( const HashedTexture& el )
{
	name	  = el.name;
	iTexture  = el.iTexture;
	iSurface0 = el.iSurface0;
	descr	  = el.descr;
} // HashedTexture::copy

/*****************************************************************************/
/*	Texture Cache implementation
/*****************************************************************************/
_inl DXSurface*	
TextureCache::GetTextureSurface( int texID, int level )
{
	DXSurface* &iSurface0 = getHashedTexture( texID ).iSurface0;
	if (iSurface0 != NULL) return iSurface0;
	getDXTex( texID )->GetSurfaceLevel( level, &iSurface0 );
	return iSurface0;
} // TextureCache::GetTextureSurface

_inl void TextureCache::CopyTexture( int destID, int srcID )
{
	DX_CHK( pDevice->UpdateTexture( getDXTex( srcID ), getDXTex( destID ) ) );
} // TextureCache::CopyTexture

_inl void TextureCache::CopyTexture( int destID, int srcID, 
									 RECT* rectList, int nRect, 
									 POINT* ptList )
{
	DX_CHK( pDevice->CopyRects( GetTextureSurface( srcID ),  rectList, nRect, 
								GetTextureSurface( destID ), ptList ) );
} // TextureCache::CopyTexture

_inl void TextureCache::CopyTexture( DXSurface* destSurf, int srcID, 
									 RECT* rectList, int nRect, POINT* ptList )
{
	assert( destSurf );
	DX_CHK( pDevice->CopyRects( GetTextureSurface( srcID ), rectList, nRect, 
								destSurf, ptList ) );
} // TextureCache::CopyTexture

_inl void TextureCache::SetTexture( int texID, int stage )
{
	if (texID == curTexture[stage] || texID == c_AnyTexture) return;
	if (texID == 0)
	{
		pDevice->SetTexture( stage, NULL );
		curTexture[stage] = 0;
		return;
	}
	pDevice->SetTexture( stage, hash.elem( texID ).iTexture );
	curTexture[stage] = texID;
	INC_COUNTER( TexSwitches, 1 );
} // TextureCache::SetTexture

_inl int TextureCache::GetTextureSizeBytes( int texID )
{
	assert( texID >= 0 && texID < hash.numElem() );
	return hash.elem( texID ).GetSizeBytes( pDevice );
} // TextureCache::GetTextureSizeBytes

_inl BYTE* TextureCache::LockTexture( int texID, int& pitch, int level )
{
	if (texID == 0) 
	{
		pitch = 0;
		return NULL;
	}
	D3DLOCKED_RECT rct;
	DXTexture* dxTex = getDXTex( texID );
	assert( dxTex != NULL );
	DX_CHK( dxTex->LockRect( level, &rct, 0, 0 ) );
	pitch = rct.Pitch;
	return (BYTE*)(rct.pBits);
} // TextureCache::LockTexture

_inl void TextureCache::UnlockTexture( int texID, int level )
{
	if (texID == 0) return;
	DX_CHK( getDXTex( texID )->UnlockRect( level ) );
} // TextureCache::UnlockTexture

_inl int TextureCache::GetTextureID( const char* texName )
{
	HashedTexture htex( texName );
	int idx = hash.find( htex );
	if (idx == NO_ELEMENT)
	{
		if (!htex.CreateFromFile( pDevice ))
		{
			//LogStatus();
			return 0;
		}
		idx = hash.add( htex );

		HashedTexture&	newTex	= hash.elem( idx );
		TextureDescr&	td		= newTex.descr;
		td.setID( idx );
	}
	return idx;
} // TextureCache::GetTextureID

_inl int TextureCache::GetTextureID( const char* texName, const TextureDescr& td )
{
	HashedTexture htex( texName );
	int idx = hash.find( htex );
	if (idx == NO_ELEMENT)
	{
		if (!htex.CreateFromFile( pDevice, true ))
		{
			LogStatus();
		}
		idx = hash.add( htex );
		HashedTexture&	newTex	= hash.elem( idx );
		newTex.descr.setID( idx );
	}
	return idx;
} // TextureCache::GetTextureID

_inl int TextureCache::GetCurrentTexture( int stage ) const
{
	assert( stage >= 0 && stage < c_MaxTextureStages );
	return curTexture[stage];
} // TextureCache::GetCurrentTexture


_inl void TextureCache::resetCurTextures()
{
	for (int i = 0; i < c_MaxTextureStages; i++)
	{
		curTexture[i] = 0;
	}
} // TextureCache::resetCurTextures

_finl DXTexture* TextureCache::getDXTex( int id ) 
{
	assert( id > 0 && id < hash.numElem() );
	assert( hash.elem( id ).iTexture );
	return hash.elem( id ).iTexture;
} // TextureCache::getDXTex

_inl HashedTexture&	TextureCache::getHashedTexture( int id )
{
	assert( id > 0 && id < hash.numElem() );
	return hash.elem( id );
} // TextureCache::getHashedTexture

