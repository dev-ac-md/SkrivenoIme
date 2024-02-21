/*****************************************************************/
/*  File:   AuxDrawn.h
/*  Desc:   
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#ifndef __COORSYS_H__
#define __COORSYS_H__

namespace AuxPrim
{


/*****************************************************************/
/*  Class:	CoorSys
/*  Desc:	Cooordinate system axis
/*****************************************************************/
class CoorSys : public BaseMesh
{
public:
	CoorSys();
	~CoorSys();
	void Init(  IRenderSystem* irs, float axisLen = 100.0f );

protected:
private:
};  // class CoorSys

void	DrawAABB( IRenderSystem* iRS, const AABoundBox& aabb, DWORD color );
void	DrawSphere(		IRenderSystem* iRS, 
						const Vector3D& center, float radius, 
						int nSeg, DWORD color );

} // namespace AuxPrim

#endif // __COORSYS_H__