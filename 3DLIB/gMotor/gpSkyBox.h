/*****************************************************************************/
/*	File:	SkyBox.h
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08.07.2002
/*****************************************************************************/
#ifndef __SKYBOX_H__
#define __SKYBOX_H__

/*****************************************************************************/
/*	Class:	SkyBox
/*	Desc:
/*****************************************************************************/
class SkyBox
{
	static Vector3D		vec[8];
	static int			irt[6];
	static int			ilt[6];
	static int			irb[6]; 
	static int			ilb[6]; 

	float				halfSide;

	BaseMesh			wall[6];

public:
	SkyBox();

	void	create( IRenderSystem* irs, const char* skyName, float side );
	void	render( IRenderSystem* irs, Camera* cam );

}; // class SkyBox

#endif // __SKYBOX_H__