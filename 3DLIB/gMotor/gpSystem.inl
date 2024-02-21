/*****************************************************************
/*  File:   GPSystem.inl                                         *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   Mar 2002                                             *
/*****************************************************************/

/*****************************************************************
/*  GPSystem implementation										 *
/*****************************************************************/
_inl GPBatchMode GPSystem::SetBatchMode( GPBatchMode	mode )
{ 
	GPBatchMode old = batchMode;
	batchMode = mode; 
	return old;
} // GPSystem::SetBatchMode

_inl int GPSystem::SetCurShader( int shID ) 
{ 
	int res = GPBucket::s_CurShader; 
	GPBucket::s_CurShader = shID; 
	return res;
} // GPSystem::SetCurShader
	
_inl void GPSystem::SetCurrentZ( float z )
{
	curZ = z;	
}

_inl float	GPSystem::GetCurrentZ()	const
{
	return curZ;
}
_inl DWORD	GPSystem::GetCurrentDiffuse() const
{
	return curDiffuse;
}

_inl void GPSystem::SetCurrentDiffuse( DWORD color )
{
	curDiffuse = color;
}

_inl int GPSystem::GPNFrames( int seqID ) const
{
	if (seqID < 0 || seqID >= gpHash.numElem()) return 0;
	const GPSeq* seq = c_GetSeq( seqID );
	if (!seq) return 0x7FFFFFFE;
	return seq->GetNFrames();
} // GPSystem::GPNFrames

_inl const char* GPSystem::GetGPName( int seqID ) const
{
	static char empty[] = "";
	if (seqID < 0 || seqID >= gpHash.numElem()) return empty;
	return c_GetSeq( seqID )->GetFileName();
} // GPSystem::GetGPName

_inl int GPSystem::GetGPHeight( int seqID, int frameIdx ) const
{
	if (seqID < 0 || seqID >= gpHash.numElem() ||
		frameIdx >= GPNFrames( seqID ) || frameIdx < 0) return 0;
	return c_GetFrame( seqID, frameIdx )->height;
} // GPSystem::GetGPHeight

_inl bool GPSystem::GetGPBoundFrame( int seqID, int frameIdx, Frame& frame )
{
	if (seqID < 0 || seqID >= gpHash.numElem() ||
		frameIdx >= GPNFrames( seqID ) || frameIdx < 0) return false;
	const GPFrameInst* frInst = GetFrameInst( seqID, frameIdx, 0 );
	massert( frInst, "GPSystem::GetGPBoundFrame - no frame instance." );
	int left, top;
	if (!frInst->GetLeftTop( left, top )) return false;
	frame.x = left;
	frame.y = top;
	frame.w = GetGPWidth( seqID, frameIdx ) - frame.x;
	frame.h = GetGPHeight( seqID, frameIdx ) - frame.y;
	return true;
} // GPSystem::GetGPBoundFrame
	

_inl GPFrame* GPSystem::GetFrame( int seqID, int sprID )
{
	return c_GetSeq( seqID )->GetFrame( UnswizzleFrameIndex( seqID, sprID ) );
} // GPSystem::GetFrame

_inl const GPFrame* GPSystem::c_GetFrame( int seqID, int sprID ) const
{
	return c_GetSeq( seqID )->GetFrame( UnswizzleFrameIndex( seqID, sprID ) );
} // GPSystem::GetFrame

_inl int GPSystem::GetGPWidth( int seqID, int frameIdx ) const
{
	if (seqID < 0 || seqID >= gpHash.numElem() ||
		frameIdx >= GPNFrames( seqID ) || frameIdx < 0) return 0;
	return c_GetFrame( seqID, frameIdx )->width;
} // GPSystem::GetGPWidth

_inl int GPSystem::GetGPNum() const
{
	return gpHash.numElem();
}

_inl void GPSystem::SetCurrentCamera( Camera* cam )
{
	curCamera = cam;
} // GPSystem::setCurrentCamera

_inl float GPSystem::GetScale() const
{
	return gpScale;
} // GPSystem::GetScale
	
_inl float GPSystem::SetScale( float scale )
{
	float old = gpScale;
	gpScale = scale;

	if (gpScale > 0.5f)
	{
		curLOD = 0;
	}
	else if (gpScale > 0.25f)
	{
		curLOD = 1;
	}
	else if (gpScale > 0.125f)
	{
		curLOD = 2;
	}
	else
	{
		curLOD = 3;
	}

	return scale;
} // GPSystem::SetScale

_inl void GPSystem::FlushPrimBucket()
{
	if (lineBucket2D.getNVert() != 0)
	{
		RC::iRS->Draw( lineBucket2D );
		lineBucket2D.setNInd ( 0 );
		lineBucket2D.setNVert( 0 );
		lineBucket2D.setNPri ( 0 );
	}

	if (lineBucket3D.getNVert() != 0)
	{
		RC::iRS->Draw( lineBucket3D );
		lineBucket3D.setNInd ( 0 );
		lineBucket3D.setNVert( 0 );
		lineBucket3D.setNPri ( 0 );
	}

	if (rectBucket2D.getNVert() != 0)
	{
		RC::iRS->Draw( rectBucket2D );
		rectBucket2D.setNInd ( 0 );
		rectBucket2D.setNVert( 0 );
		rectBucket2D.setNPri ( 0 );
	}

	if (rectBucket3D.getNVert() != 0)
	{
		RC::iRS->Draw( rectBucket3D );
		rectBucket3D.setNInd ( 0 );
		rectBucket3D.setNVert( 0 );
		rectBucket3D.setNPri ( 0 );
	}
} // GPSystem::flushBucket

_inl void GPSystem::FlushSpriteBucket( int bucketIdx )
{
	assert( bucketIdx >= 0 && bucketIdx < s_GPTexNum );
	sprBucket[bucketIdx].Flush();
}

_inl int GPSystem::GetNumTextureCacheSurfaces()
{
	return s_GPTexNum;
}

_inl bool GPSystem::SetClipArea( DWORD x, DWORD y, 
								 DWORD w, DWORD h )
{
	if (curViewPort.x == x && curViewPort.y == y &&
		curViewPort.width == w && curViewPort.height == h)
	{
		return false;
	}
	curViewPort.x		= x;
	curViewPort.y		= y;
	curViewPort.width	= w;
	curViewPort.height	= h;
	curViewPort.MinZ	= 0.0f;
	curViewPort.MaxZ	= 1.0f;

	//  viewport is changed, so we need to flush all batched stuff here
	if (RC::iRS) 
	{
		FlushBatches();
		RC::iRS->SetViewPort( curViewPort );
	}
	return true;
} // GPSystem::SetClipArea

_inl const ViewPort& GPSystem::GetClipArea() const
{
	return curViewPort;
} // GPSystem::GetClipArea

_inl GPSeq*	GPSystem::GetSeq( int idx ) 
{
	return gpHash.elem( idx );
} // GPSystem::GetSeq

_inl GPSeq*	GPSystem::c_GetSeq( int idx ) const
{
	return gpHash.c_elem( idx );
}

GPFrameInst* GPSystem::GetFrameInstForDrawing( int& gpID, int& sprID, DWORD color, WORD lod )
{
	GPSeq* seq = gpHash.elem( gpID );
	if (!seq) LoadGP( gpID );
	seq = gpHash.elem( gpID );
	assert( seq );
	massert( sprID < seq->GetNFrames(), "Frame index is outside boundary." );

	sprID = UnswizzleFrameIndex( gpID, sprID );
	GPFrameInst* frameInst = GetFrameInst( gpID, sprID, color, lod );

	// ensure that pixel data is already in texture memory
	if (!frameInst->Touch()) return NULL;
	frameInst->Hit( curCacheFactor );
	return frameInst;
} // GPSystem::GetFrameInstForDrawing

/*---------------------------------------------------------------------------*/
/*	Func:	GPSystem::UnswizzleFrameIndex
/*	Desc:	Unswizzle frame index in case of "directional indexing"
/*	Parm:	Converts frame indexing from representation where frames are
/*			ordedered like
/*			<dir0:anmFrame0, dir1:anmFrame0, ...> 
/*			<dir0:anmFrame1, dir1:anmFrame1, ...>...
/*			to the order
/*			<dir0:anmFrame0, dir0:anmFrame1, ...> 
/*			<dir1:anmFrame0, dir1:anmFrame1, ...>...
/*			First case is convenient for cossacks engine, second for compression
/*	Ret:	Converted frame index
/*---------------------------------------------------------------------------*/
_inl int GPSystem::UnswizzleFrameIndex( int gpID, int sprID ) const
{
	GPSeq* seq = c_GetSeq( gpID );
	//  awfully bad hack - fix it:
	if (!seq) 
	{
		((GPSystem*)this)->LoadGP( gpID );
		seq = c_GetSeq( gpID );
	}
	assert( seq );
	int nDir = seq->GetNDirections();
	if (nDir > 1)
	{
		int nSpr = seq->GetNFrames();
		int seqLen = nSpr / nDir;
		sprID = sprID / nDir + seqLen * (sprID % nDir);
	}
	return sprID;
} // GPSystem::UnswizzleFrameIndex

_inl GPFrameInst* GPSystem::GetFrameInst( int gpID, int sprID, DWORD color, WORD lod )
{
	//  find given frame reference in hash
	int frameID = frameHash.find( GPFrameInst::Key( gpID, sprID, color, lod ) );
	if (frameID == NO_ELEMENT)
	//  create frame instance
	{
		GPFrameInst* newfr = new GPFrameInst( gpHash.elem( gpID ), sprID, color, lod );
		frameID = frameHash.add( GPFrameInst::Key( gpID, sprID, color, lod ), newfr );
		newfr->InitPrefix();
	}

	return frameHash.elem( frameID );
} // GPSystem::GetFrameInst





