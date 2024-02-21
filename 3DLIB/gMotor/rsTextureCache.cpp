/*****************************************************************/
/*  File:   rsTextureCache.cpp
/*  Desc:   
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#include "stdafx.h"
#include "Drawn.h"
#include "rsRenderSystemDX.h"
#include "rsTextureCache.h"

/*****************************************************************/
/*  TextureCache implementation
/*****************************************************************/
TextureCache::TextureCache()
{
	numItem		= 0;
	numBlocks	= 0;
}

TextureCache::~TextureCache()
{
}

TextureCacheItem*
TextureCache::GetItem( int itemID )
{
	if (itemID <= 0 || itemID > numItem) return 0;
	return &(tcitem[itemID - 1]);
}

TextureCacheItem& 
TextureCache::AddTextureItem( const TextureDescr& tdescr, bool isCacheItem )
{
	int i;
	for (i = 0; i < numBlocks; i++)
	{
		if (!(block[i].tdescr.equal( tdescr ))) continue;
		//  this type of texture already present
		break;
	}

	if (i == numBlocks)
	//  create block
	{
		block[numBlocks].tdescr = tdescr;
		block[numBlocks].numItem = 0;
		numBlocks++;
	}

	TextureCacheBlock* tb = &(block[i]);
	//  create textures
	TextureCacheItem& tci = tcitem[numItem];
	tci.itemID = numItem + 1;
	tci.resID  = 0;

	if (isCacheItem)
	{
		tb->item[tb->numItem] = &tci;
		tb->numItem++;
	}
	numItem++;
	return tci;
}

void 
TextureCache::BindToItem( BaseMesh* item )
{
	tcitem[item->getTexture() - 1].meshes.push_back( item );	
}

int 
TextureCache::AllocTextureItem( const TextureDescr& tdescr )
{
	int i;
	for (i = 0; i < numBlocks; i++)
	{
		if (!(block[i].tdescr.equal( tdescr ))) continue;
		TextureCacheBlock* tcb = &(block[i]);

		if (block[i].numItem == 0) return 0;
		int maxLRU = tcb->item[0]->LRU;
		int itLRU = 0;

		for (int j = 1; j < tcb->numItem; j++)
		{
			if (maxLRU == 0) break;
			if (tcb->item[j]->LRU < maxLRU)
			{
				maxLRU	= tcb->item[j]->LRU;
				itLRU	= j;
			}
		}
		
		//  say all drawns they have no more this texture in VRAM
		{
			TextureCacheItem* tci = tcb->item[itLRU];
			tci->LRU = GetTickCount();
			int numMeshes = tci->meshes.size();
			for (int i = 0; i < numMeshes; i++)
			{
				tci->meshes[i]->setTexture( 0 );
			}
			tci->meshes.clear();
			return tci->itemID;
		}
	}
	return -1;
}

/*****************************************************************
/*	TextureCacheItem
/*****************************************************************/
TextureCacheItem::TextureCacheItem() : itemID(0), LRU(0), resID(0)
{}



