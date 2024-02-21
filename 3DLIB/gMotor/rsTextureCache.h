/*****************************************************************/
/*  File:   TextureCache.h
/*  Desc:   
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#ifndef __TEXTURECACHE_H__
#define __TEXTURECACHE_H__

#include <vector>

const int c_MaxItemInBlock		= 1024;
const int c_MaxTexCacheItems	= 2048;
const int c_MaxTexCacheBlocks	= 32;

class TextureCacheItem{
public:
	TextureCacheItem();
	
	int					itemID;
	int					resID;		//  identifier of texture resource
	int					LRU;		//  least recently used value
	vector<BaseMesh*>	meshes;		//  list of meshes who share this texture
}; // struct TextureCacheItem

struct TextureCacheBlock{
	TextureDescr				tdescr;				//  textures type description
	TextureCacheItem*			item[c_MaxItemInBlock];	//  array of textures of same type
	int							numItem;
	int							dismissFrequency;	//  statistics - TODO
}; // struct TextureCacheBlock

/*****************************************************************/
/*  Class:	TextureCache
/*  Desc:	
/*****************************************************************/
class TextureCache
{
public:	
	TextureCache();
	~TextureCache();

	void InitBlock( const TextureDescr& tdescr, int numTex );
	
	int	AllocTextureItem( const TextureDescr& tdescr );
	TextureCacheItem& AddTextureItem( const TextureDescr& tdescr, bool isCacheItem = true );

	TextureCacheItem* GetItem( int itemID );

	void BindToItem( BaseMesh* item );

protected:
private:
	TextureCacheBlock			block[c_MaxTexCacheBlocks];
	TextureCacheItem			tcitem[c_MaxTexCacheItems];
	int							numItem;
	int							numBlocks;
};  // class TextureCache

#endif // __TEXTURECACHE_H__