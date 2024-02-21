# Microsoft Developer Studio Project File - Name="gMotor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=gMotor - Win32 Debug9
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gMotor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gMotor.mak" CFG="gMotor - Win32 Debug9"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gMotor - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "gMotor - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "gMotor - Win32 Mtd" (based on "Win32 (x86) Static Library")
!MESSAGE "gMotor - Win32 Mtd9" (based on "Win32 (x86) Static Library")
!MESSAGE "gMotor - Win32 Release9" (based on "Win32 (x86) Static Library")
!MESSAGE "gMotor - Win32 Debug9" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Cossacks II/3dlib/gMotor", GIDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gMotor - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../_release/gMotor/"
# PROP Intermediate_Dir "../../_release/gMotor/"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_INLINES" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x422 /d "NDEBUG"
# ADD RSC /l 0x422 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib_release/gMotorR.lib"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../_debug/gMotor/"
# PROP Intermediate_Dir "../../_debug/gMotor/"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x422 /d "_DEBUG"
# ADD RSC /l 0x422 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/gMotor.lib"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Mtd"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gMotor___Win32_Mtd"
# PROP BASE Intermediate_Dir "gMotor___Win32_Mtd"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../_mtd/gMotor/"
# PROP Intermediate_Dir "../../_mtd/gMotor/"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x422 /d "_DEBUG"
# ADD RSC /l 0x422 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../lib/gMotor.lib"
# ADD LIB32 /nologo /out:"../../lib_mtd/gMotor.lib"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Mtd9"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gMotor___Win32_Mtd9"
# PROP BASE Intermediate_Dir "gMotor___Win32_Mtd9"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../_mtd9/gMotor/"
# PROP Intermediate_Dir "../../_mtd9/gMotor/"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_DX9" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x422 /d "_DEBUG"
# ADD RSC /l 0x422 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../../lib_mtd/gMotor.lib"
# ADD LIB32 /nologo /out:"../../lib_mtd/gMotor.lib"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Release9"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "gMotor___Win32_Release9"
# PROP BASE Intermediate_Dir "gMotor___Win32_Release9"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../_release9/gMotor/"
# PROP Intermediate_Dir "../../_release9/gMotor/"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_INCLUDES" /FAs /Fa"../../_listings/gMotor/" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_INLINES" /D "_DX9" /FAs /Fa"../../_listings/gMotor/" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x422 /d "NDEBUG"
# ADD RSC /l 0x422 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../../lib/gMotor.lib"
# ADD LIB32 /nologo /out:"../../lib/gMotor.lib"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Debug9"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gMotor___Win32_Debug9"
# PROP BASE Intermediate_Dir "gMotor___Win32_Debug9"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../_debug9/gMotor/"
# PROP Intermediate_Dir "../../_debug9/gMotor/"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_DX9" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x422 /d "_DEBUG"
# ADD RSC /l 0x422 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../../lib/gMotor.lib"
# ADD LIB32 /nologo /out:"../../lib/gMotor.lib"

!ENDIF 

# Begin Target

# Name "gMotor - Win32 Release"
# Name "gMotor - Win32 Debug"
# Name "gMotor - Win32 Mtd"
# Name "gMotor - Win32 Mtd9"
# Name "gMotor - Win32 Release9"
# Name "gMotor - Win32 Debug9"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\fexG16RealTime.cpp
# End Source File
# Begin Source File

SOURCE=.\fexLzoDecompress.cpp
# End Source File
# Begin Source File

SOURCE=.\fexUclDecompress.cpp
# End Source File
# Begin Source File

SOURCE=.\gpAnimModel.cpp
# End Source File
# Begin Source File

SOURCE=.\gpAuxPrim.cpp
# End Source File
# Begin Source File

SOURCE=.\gpBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\gpBucket.cpp
# End Source File
# Begin Source File

SOURCE=.\gpColor.cpp
# End Source File
# Begin Source File

SOURCE=.\gpGN16.cpp
# End Source File
# Begin Source File

SOURCE=.\gpGU16.cpp
# End Source File
# Begin Source File

SOURCE=.\gpMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\gpPackage.cpp
# End Source File
# Begin Source File

SOURCE=.\gpRenderSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\gpSeries.cpp
# End Source File
# Begin Source File

SOURCE=.\gpSkyBox.cpp
# End Source File
# Begin Source File

SOURCE=.\gpSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\gpText.cpp
# End Source File
# Begin Source File

SOURCE=.\kAssert.cpp
# End Source File
# Begin Source File

SOURCE=.\kBmptool.cpp
# End Source File
# Begin Source File

SOURCE=.\kCache.cpp
# End Source File
# Begin Source File

SOURCE=.\kInput.cpp
# End Source File
# Begin Source File

SOURCE=.\kIO.cpp
# End Source File
# Begin Source File

SOURCE=.\kLog.cpp
# End Source File
# Begin Source File

SOURCE=.\kResfile.cpp
# End Source File
# Begin Source File

SOURCE=.\kResource.cpp
# End Source File
# Begin Source File

SOURCE=.\kStatistics.cpp
# End Source File
# Begin Source File

SOURCE=.\kString.cpp
# End Source File
# Begin Source File

SOURCE=.\kStrUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\kThread.cpp
# End Source File
# Begin Source File

SOURCE=.\kUtilities.cpp
# End Source File
# Begin Source File

SOURCE=.\mArcBall.cpp
# End Source File
# Begin Source File

SOURCE=.\mCamera.cpp

!IF  "$(CFG)" == "gMotor - Win32 Release"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Debug"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Mtd"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Mtd9"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Release9"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Debug9"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mMath3D.cpp

!IF  "$(CFG)" == "gMotor - Win32 Release"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Debug"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Mtd"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Mtd9"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Release9"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Debug9"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rsDX.cpp
# End Source File
# Begin Source File

SOURCE=.\rsDXErr.cpp
# End Source File
# Begin Source File

SOURCE=.\rsPrimitiveBufferDX.cpp

!IF  "$(CFG)" == "gMotor - Win32 Release"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Debug"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Mtd"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Mtd9"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Release9"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Debug9"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rsRenderSystemDX.cpp

!IF  "$(CFG)" == "gMotor - Win32 Release"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Debug"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Mtd"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Mtd9"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Release9"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Debug9"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rsSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\rsShaderDX.cpp

!IF  "$(CFG)" == "gMotor - Win32 Release"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Debug"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Mtd"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Mtd9"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Release9"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Debug9"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rsTextureDX.cpp

!IF  "$(CFG)" == "gMotor - Win32 Release"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Debug"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Mtd"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Mtd9"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Release9"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Debug9"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rsVertex.cpp

!IF  "$(CFG)" == "gMotor - Win32 Release"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Debug"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Mtd"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Mtd9"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Release9"

!ELSEIF  "$(CFG)" == "gMotor - Win32 Debug9"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\fexG16Common.h
# End Source File
# Begin Source File

SOURCE=.\fexPack.h
# End Source File
# Begin Source File

SOURCE=.\gmDefines.h
# End Source File
# Begin Source File

SOURCE=.\gMotor.h
# End Source File
# Begin Source File

SOURCE=.\gMotorRC.h
# End Source File
# Begin Source File

SOURCE=.\gpAnimModel.h
# End Source File
# Begin Source File

SOURCE=.\gpAuxPrim.h
# End Source File
# Begin Source File

SOURCE=.\gpBitmap.h
# End Source File
# Begin Source File

SOURCE=.\gpBucket.h
# End Source File
# Begin Source File

SOURCE=.\gpColor.h
# End Source File
# Begin Source File

SOURCE=.\gpGN16.h
# End Source File
# Begin Source File

SOURCE=.\gpGU16.h
# End Source File
# Begin Source File

SOURCE=.\gpMesh.h
# End Source File
# Begin Source File

SOURCE=.\gpPackage.h
# End Source File
# Begin Source File

SOURCE=.\gpRenderSystem.h
# End Source File
# Begin Source File

SOURCE=.\gpSeries.h
# End Source File
# Begin Source File

SOURCE=.\gpSkyBox.h
# End Source File
# Begin Source File

SOURCE=.\gpSystem.h
# End Source File
# Begin Source File

SOURCE=.\gpSystem.hpp
# End Source File
# Begin Source File

SOURCE=.\gpText.h
# End Source File
# Begin Source File

SOURCE=.\IRenderSystem.h
# End Source File
# Begin Source File

SOURCE=.\kAssert.h
# End Source File
# Begin Source File

SOURCE=.\kBmptool.h
# End Source File
# Begin Source File

SOURCE=.\kCache.h
# End Source File
# Begin Source File

SOURCE=.\kInput.h
# End Source File
# Begin Source File

SOURCE=.\kIO.h
# End Source File
# Begin Source File

SOURCE=.\kLog.h
# End Source File
# Begin Source File

SOURCE=.\kResfile.h
# End Source File
# Begin Source File

SOURCE=.\kResource.h
# End Source File
# Begin Source File

SOURCE=.\kStatistics.h
# End Source File
# Begin Source File

SOURCE=.\kString.h
# End Source File
# Begin Source File

SOURCE=.\kStrUtil.h
# End Source File
# Begin Source File

SOURCE=.\kThread.h
# End Source File
# Begin Source File

SOURCE=.\kUtilities.h
# End Source File
# Begin Source File

SOURCE=.\mArcBall.h
# End Source File
# Begin Source File

SOURCE=.\mCamera.h
# End Source File
# Begin Source File

SOURCE=.\mMath3D.h
# End Source File
# Begin Source File

SOURCE=.\rsDX.h
# End Source File
# Begin Source File

SOURCE=.\rsDXErr.h
# End Source File
# Begin Source File

SOURCE=.\rsPrimitiveBufferDX.h
# End Source File
# Begin Source File

SOURCE=.\rsRenderSystemDX.h
# End Source File
# Begin Source File

SOURCE=.\rsSettings.h
# End Source File
# Begin Source File

SOURCE=.\rsShaderDX.h
# End Source File
# Begin Source File

SOURCE=.\rsTextureDX.h
# End Source File
# Begin Source File

SOURCE=.\rsVertex.h
# End Source File
# Begin Source File

SOURCE=.\Settings.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "inl"
# Begin Source File

SOURCE=.\gpBitmap.inl
# End Source File
# Begin Source File

SOURCE=.\gpBucket.inl
# End Source File
# Begin Source File

SOURCE=.\gpColor.inl
# End Source File
# Begin Source File

SOURCE=.\gpGN16.inl
# End Source File
# Begin Source File

SOURCE=.\gpGU16.inl
# End Source File
# Begin Source File

SOURCE=.\gpMesh.inl
# End Source File
# Begin Source File

SOURCE=.\gpPackage.inl
# End Source File
# Begin Source File

SOURCE=.\gpSystem.inl
# End Source File
# Begin Source File

SOURCE=.\kArray.hpp
# End Source File
# Begin Source File

SOURCE=.\kCache.inl
# End Source File
# Begin Source File

SOURCE=.\kHash.hpp
# End Source File
# Begin Source File

SOURCE=.\kIO.inl
# End Source File
# Begin Source File

SOURCE=.\kResource.inl
# End Source File
# Begin Source File

SOURCE=.\kString.inl
# End Source File
# Begin Source File

SOURCE=.\kStrUtil.inl
# End Source File
# Begin Source File

SOURCE=.\kUtilities.inl
# End Source File
# Begin Source File

SOURCE=.\mCamera.inl
# End Source File
# Begin Source File

SOURCE=.\mMath3d.inl
# End Source File
# Begin Source File

SOURCE=.\rsDX.inl
# End Source File
# Begin Source File

SOURCE=.\rsPrimitiveBufferDX.inl
# End Source File
# Begin Source File

SOURCE=.\rsShaderDX.inl
# End Source File
# Begin Source File

SOURCE=.\rsTextureDX.inl
# End Source File
# End Group
# Begin Group "Dox Files"

# PROP Default_Filter "txt"
# Begin Source File

SOURCE=.\FormatInfo.txt
# End Source File
# Begin Source File

SOURCE=.\FormatNonUniform.txt
# End Source File
# Begin Source File

SOURCE=.\FormatPalette.txt
# End Source File
# Begin Source File

SOURCE=.\FormatUniform.txt
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc"
# Begin Source File

SOURCE=.\err.bmp
# End Source File
# End Group
# End Target
# End Project
