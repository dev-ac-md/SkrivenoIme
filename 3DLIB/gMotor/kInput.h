/*****************************************************************
/*  File:   Input.h                                              *
/*  Desc:   Input dispatching manager                            *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   Mar 2002                                             *
/*****************************************************************/
#ifndef __INPUTT_H__
#define __INPUTT_H__

enum InputEvent{
	ieUnknown		= 0,
	ieKeyDown		= 1, 
	ieMouseWheel	= 2,
	ieMouseMove		= 3,
	ieMouseLUp		= 4,
	ieMouseLDown	= 5,
	ieMouseLDbl		= 6,
	ieMouseRUp		= 7,
	ieMouseRDown	= 8,
	ieMouseRDbl		= 9,
	ieMouseMUp		= 10,
	ieMouseMDown	= 11,
	ieMouseMDbl		= 12
}; // enum InputEvent

class InputDispatcher
{
public:
	InputDispatcher();

	_inl void setVirtKey( int key )			{ curKey		= key;		}
	_inl void setKeyData( int data )		{ curKeyData	= data;		}
	_inl void setWheelRotation( short rot )	{ curWheelRot	= rot;		}
	_inl void setKeyFlags( int flags )		{ curKeyFlags	= flags;	}  
	_inl void setXPos( int xpos )			{ curX			= xpos;		}
	_inl void setYPos( int ypos )			{ curY			= ypos;		}
    _inl void setInputEvent( InputEvent event ) { lastEvent = event;	}
	_inl InputEvent getInputEvent() { return lastEvent; }


    _inl int	getXPos()		const	{ return curX;			}
	_inl int	getYPos()		const	{ return curY;			}
    _inl short	getWheelRot()	const	{ return curWheelRot;	}
	_inl int	getVirtKey()	const	{ return curKey;		}
	_inl int	getKeyData()	const	{ return curKeyData;	}

    _inl InputEvent getLastEvent() const { return lastEvent; }
    _inl bool   isLB() const { return (curKeyFlags & MK_LBUTTON) != 0; }
    _inl bool   isRB() const { return (curKeyFlags & MK_RBUTTON) != 0; }
    _inl bool   isMB() const { return (curKeyFlags & MK_MBUTTON) != 0; }
    _inl bool   isCtrl() const { return (curKeyFlags & MK_CONTROL) != 0;	}
    _inl bool   isShift() const { return (curKeyFlags & MK_SHIFT) != 0;		}

	virtual bool __stdcall onInputEvent( InputEvent inpev ){return false;} 

private:
	int			curKey;
	int			curKeyData;
	int			curKeyFlags;
	int			curX;
	int			curY;
	int			curWheelRot;
    InputEvent  lastEvent;

	int			inputEventID;

}; // class InputDispatcher

#endif // __INPUTT_H__
