/*****************************************************************************/
/*	File:	MaxUtil.cpp
/*	Desc:	3ds max helper functions
/*	Author:	Ruslan Shestopalyuk
/*	Date:	07.11.2002
/*****************************************************************************/
#include "Max.h"
#include "MaxUtil.h"

/*---------------------------------------------------------------------------*/
/*	Func:	TotalChildrenNum	
/*	Desc:	returns total number of nodes in subtree
/*---------------------------------------------------------------------------*/
int TotalChildrenNum( INode* node )
{
	int nNodes = 1;
	int numCh = node->NumberOfChildren();
	for (int i = 0; i < numCh; i++) 
		nNodes += TotalChildrenNum( node->GetChildNode( i ) );
	return nNodes;
} // TotalChildrenNum