# Microsoft Developer Studio Project File - Name="cossacks2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=cossacks2 - WIN32 RELEASE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cossacks2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cossacks2.mak" CFG="cossacks2 - WIN32 RELEASE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cossacks2 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "cossacks2 - Win32 Release3d" (based on "Win32 (x86) Application")
!MESSAGE "cossacks2 - Win32 Release_CII_test" (based on "Win32 (x86) Application")
!MESSAGE "cossacks2 - Win32 Release3d_inl" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/cossacks2", LAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../_release"
# PROP Intermediate_Dir "../_release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /I "InetRaiting" /D "WIN32" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 libc.lib commcore.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib oldnames.lib dsound.lib dxguid.lib dplayx.lib strmiids.lib dsound.lib dplayx.lib ddraw.lib ..\_debug\DipServer.lib ..\_debug\ichat.lib dxguid.lib /nologo /subsystem:windows /map:"..\bin\dmcr.map" /debug /machine:I386 /nodefaultlib /out:"C:\AC\dmcr.exe" /libpath:"../lib" /libpath:"../_debug" /libpath:"../_release" /libpath:"../_release3d" /FIXED:NO /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none /incremental:yes
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=..\bat\all.bat
# End Special Build Tool

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "cossacks2___Win32_Release_C2_Test"
# PROP BASE Intermediate_Dir "cossacks2___Win32_Release_C2_Test"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../_release3d/vc6/"
# PROP Intermediate_Dir "../_release3d/vc6/"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /I "InetRaiting" /D "WIN32" /D "_WINDOWS" /D "_USE3D" /FR /YX /FD /c
# ADD CPP /nologo /MT /w /W0 /GX /Zi /I "../3dlib/gMotor" /I "InetRaiting" /D "WIN32" /D "_WINDOWS" /D "_USE3D" /D "COSSACKS2" /D "_COSSACKS2" /D "NEWMORALE" /D "SIMPLEMANAGE" /D "GETTIRED" /D "HASH_TOP" /FR"../debug/" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib libc.lib oldnames.lib libc.lib commcore.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib oldnames.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib /out:"e:\arc_usa\dmcr.exe" /libpath:"../lib" /libpath:"../_debug" /libpath:"../_release" /libpath:"../_release3d" /FIXED:NO /MAPINFO:EXPORTS /MAPINFO:LINES /FIXED:NO /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT BASE LINK32 /pdb:none /incremental:yes
# ADD LINK32 gMotor.lib ddraw.lib libcpmt.lib libcmt.lib libc.lib commcore.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib oldnames.lib dsound.lib dxguid.lib dplayx.lib strmiids.lib d3d8.lib d3dx8.lib dxerr8.lib ../_debug/ichat/ichat.lib ..\lib\dipserver.lib /nologo /subsystem:windows /incremental:yes /map:"..\bin\dmcr.map" /debug /machine:I386 /nodefaultlib /out:"D:\C2forGEC\cossacks2.exe" /libpath:"../lib" /libpath:"../_debug" /libpath:"../_release" /libpath:"../_release3d" /FIXED:NO /MAPINFO:EXPORTS /MAPINFO:LINES /FIXED:NO /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /verbose /pdb:none /force

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "cossacks2___Win32_Release_CII_test"
# PROP BASE Intermediate_Dir "cossacks2___Win32_Release_CII_test"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "cossacks2___Win32_Release_CII_test"
# PROP Intermediate_Dir "cossacks2___Win32_Release_CII_test"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /I "InetRaiting" /D "WIN32" /D "_WINDOWS" /FR /YX /FD /c
# ADD CPP /nologo /MT /W2 /GX /Zi /I "InetRaiting" /D "WIN32" /D "_WINDOWS" /D "COSSACKS2" /D "NEWMORALE" /D "GETTIRED" /D "SIMPLEMANAGE" /D "HASH_TOP" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libc.lib commcore.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib oldnames.lib /nologo /subsystem:windows /map:"..\bin\dmcr.map" /debug /machine:I386 /nodefaultlib /out:"..\bin\dmcr.exe" /libpath:"../lib" /libpath:"../_debug" /libpath:"../_release" /libpath:"../_release3d" /FIXED:NO /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 libc.lib commcore.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib oldnames.lib dsound.lib dplayx.lib ddraw.lib ..\_debug\DipServer.lib ..\_debug\ichat.lib dxguid.lib /nologo /subsystem:windows /incremental:yes /map:"..\bin\dmcr.map" /debug /machine:I386 /nodefaultlib /out:"e:\Cossack2_from_Renderman\dmcr.exe" /libpath:"../lib" /libpath:"../_debug" /libpath:"../_release" /libpath:"../_release3d" /FIXED:NO /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=..\bat\all.bat
# End Special Build Tool

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "cossacks2___Win32_Release3d_inl"
# PROP BASE Intermediate_Dir "cossacks2___Win32_Release3d_inl"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../_release3d_inl/vc6/"
# PROP Intermediate_Dir "../_release3d_inl/vc6/"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /w /W0 /GX /Zi /I "../3dlib/gMotor" /I "InetRaiting" /D "WIN32" /D "_WINDOWS" /D "_USE3D" /D "COSSACKS2" /FR"../debug/" /YX /FD /c
# ADD CPP /nologo /MT /w /W0 /GX /O2 /Ob2 /I "../3dlib/gMotor" /I "InetRaiting" /D "WIN32" /D "_WINDOWS" /D "_USE3D" /D "COSSACKS2" /D "_COSSACKS2" /D "NEWMORALE" /D "SIMPLEMANAGE" /D "GETTIRED" /D "HASH_TOP" /D "_INLINES" /D "_NOSTAT" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ../_debug/ichat/ichat.lib ../_debug/dipserver/dipserver.lib gMotor.lib d3d8.lib d3dx8.lib dxerr8.lib ddraw.lib libcpmt.lib libcmt.lib libc.lib commcore.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib oldnames.lib dsound.lib dxguid.lib dplayx.lib strmiids.lib /nologo /subsystem:windows /incremental:yes /map:"..\bin\dmcr.map" /debug /machine:I386 /nodefaultlib /out:"c:\cossacks2\cossacks2.exe" /libpath:"../lib" /libpath:"../_debug" /libpath:"../_release" /libpath:"../_release3d" /FIXED:NO /MAPINFO:EXPORTS /MAPINFO:LINES /FIXED:NO /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT BASE LINK32 /verbose /pdb:none /force
# ADD LINK32 ../_debug/ichat/ichat.lib ../_debug/dipserver/dipserver.lib gMotorR.lib ddraw.lib libcpmt.lib libcmt.lib libc.lib commcore.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib oldnames.lib dsound.lib dxguid.lib dplayx.lib strmiids.lib d3d8.lib d3dx8.lib dxerr8.lib /nologo /subsystem:windows /incremental:yes /map:"..\bin\dmcr.map" /debug /machine:I386 /nodefaultlib /out:"c:\cossacks2\cossacks2.exe" /libpath:"../lib" /libpath:"../lib_release" /libpath:"../_debug" /libpath:"../_release" /libpath:"../_release3d" /FIXED:NO /MAPINFO:EXPORTS /MAPINFO:LINES /FIXED:NO /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /verbose /pdb:none /force

!ENDIF 

# Begin Target

# Name "cossacks2 - Win32 Release"
# Name "cossacks2 - Win32 Release3d"
# Name "cossacks2 - Win32 Release_CII_test"
# Name "cossacks2 - Win32 Release3d_inl"
# Begin Group "Dump"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Ddini.h
# End Source File
# Begin Source File

SOURCE=.\Dplay.h
# End Source File
# Begin Source File

SOURCE=.\event.h
# End Source File
# Begin Source File

SOURCE=.\Lines.cpp
# End Source File
# Begin Source File

SOURCE=.\Lines.h
# End Source File
# Begin Source File

SOURCE=.\menu.cpp
# End Source File
# Begin Source File

SOURCE=.\menu.h
# End Source File
# Begin Source File

SOURCE=.\mgraph.h
# End Source File
# Begin Source File

SOURCE=.\mode.h
# End Source File
# Begin Source File

SOURCE=.\MoveToXY.cpp
# End Source File
# Begin Source File

SOURCE=.\Resfile.cpp
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# End Group
# Begin Group "Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Dpchat.h
# End Source File
# Begin Source File

SOURCE=.\Dplobby.h
# End Source File
# Begin Source File

SOURCE=.\fog.h
# End Source File
# Begin Source File

SOURCE=.\Multipl.h
# End Source File
# Begin Source File

SOURCE=.\Nucl.h
# End Source File
# Begin Source File

SOURCE=.\Resfile.h
# End Source File
# End Group
# Begin Group "Arc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\gFile.h
# End Source File
# Begin Source File

SOURCE=.\Arc\GSCarch.cpp
# End Source File
# Begin Source File

SOURCE=.\Arc\GSCarch.h
# End Source File
# Begin Source File

SOURCE=.\Arc\GSCset.cpp
# End Source File
# Begin Source File

SOURCE=.\Arc\GSCset.h
# End Source File
# Begin Source File

SOURCE=.\Arc\GSCtypes.h
# End Source File
# Begin Source File

SOURCE=.\GSINC.h
# End Source File
# Begin Source File

SOURCE=.\gText.cpp
# End Source File
# Begin Source File

SOURCE=.\Arc\isiMasks.cpp
# End Source File
# Begin Source File

SOURCE=.\Arc\isiMasks.h
# End Source File
# End Group
# Begin Group "GameSpy"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Chat\chat.h
# End Source File
# Begin Source File

SOURCE=.\CommCore.h
# End Source File
# Begin Source File

SOURCE=.\CEngine\goaceng.h
# End Source File
# Begin Source File

SOURCE=.\MPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\peer.h
# End Source File
# Begin Source File

SOURCE=.\PeerClass.cpp
# End Source File
# Begin Source File

SOURCE=.\PeerClass.h
# End Source File
# Begin Source File

SOURCE=.\Pinger.h
# End Source File
# Begin Source File

SOURCE=.\Peer.lib
# End Source File
# End Group
# Begin Group "AI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Brigade.cpp
# End Source File
# Begin Source File

SOURCE=.\EinfoClass.cpp
# End Source File
# Begin Source File

SOURCE=.\EinfoClass.h
# End Source File
# Begin Source File

SOURCE=.\Megapolis.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

# ADD CPP /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Megapolis.h
# End Source File
# Begin Source File

SOURCE=.\NewAI.cpp
# End Source File
# Begin Source File

SOURCE=.\NewAI.h
# End Source File
# Begin Source File

SOURCE=.\SafeNet.cpp
# End Source File
# Begin Source File

SOURCE=.\Safety.h
# End Source File
# Begin Source File

SOURCE=.\StrategyResearch.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

# ADD CPP /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# ADD CPP /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD CPP /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StrategyResearch.h
# End Source File
# Begin Source File

SOURCE=.\Towers.h
# End Source File
# End Group
# Begin Group "3D Engine"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\3DBars.cpp
# End Source File
# Begin Source File

SOURCE=.\3DBars.h
# End Source File
# Begin Source File

SOURCE=.\3DGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\3DGraph.h
# End Source File
# Begin Source File

SOURCE=.\3DMapEd.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

# ADD CPP /ZI /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /ZI /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# ADD CPP /ZI /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD CPP /O2
# SUBTRACT CPP /Z<none>

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\3DMapEd.h
# End Source File
# Begin Source File

SOURCE=.\3DRandMap.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

# ADD CPP /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# ADD CPP /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD CPP /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\3DRandMap.h
# End Source File
# Begin Source File

SOURCE=.\3DSurf.cpp
# End Source File
# Begin Source File

SOURCE=.\3DSurf.h
# End Source File
# Begin Source File

SOURCE=.\Masks.h
# End Source File
# End Group
# Begin Group "Graphics&DirectX"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ArchTool.cpp
# End Source File
# Begin Source File

SOURCE=.\ArchTool.h
# End Source File
# Begin Source File

SOURCE=.\bmptool.cpp
# End Source File
# Begin Source File

SOURCE=.\bmptool.h
# End Source File
# Begin Source File

SOURCE=.\Curve.h
# End Source File
# Begin Source File

SOURCE=.\Ddini.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs.h
# End Source File
# Begin Source File

SOURCE=.\DrawForm.h
# End Source File
# Begin Source File

SOURCE=.\DrawForms.cpp
# End Source File
# Begin Source File

SOURCE=.\Fastdraw.cpp
# End Source File
# Begin Source File

SOURCE=.\Fastdraw.h
# End Source File
# Begin Source File

SOURCE=.\fog.cpp
# End Source File
# Begin Source File

SOURCE=.\Fonts.h
# End Source File
# Begin Source File

SOURCE=.\GP_Draw.cpp
# End Source File
# Begin Source File

SOURCE=.\GP_Draw.h
# End Source File
# Begin Source File

SOURCE=.\Graphs.cpp
# End Source File
# Begin Source File

SOURCE=.\Graphs.h
# End Source File
# Begin Source File

SOURCE=.\GVClass.cpp
# End Source File
# Begin Source File

SOURCE=.\Hint.cpp
# End Source File
# Begin Source File

SOURCE=.\Icons.h
# End Source File
# Begin Source File

SOURCE=.\IconTool.cpp
# End Source File
# Begin Source File

SOURCE=.\IconTool.h
# End Source File
# Begin Source File

SOURCE=.\Masks.cpp
# End Source File
# Begin Source File

SOURCE=.\mode.dat
# End Source File
# Begin Source File

SOURCE=.\mouse.h
# End Source File
# Begin Source File

SOURCE=.\Mouse_X.cpp
# End Source File
# Begin Source File

SOURCE=.\MousZone.cpp
# End Source File
# Begin Source File

SOURCE=.\NewMon3D.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /Gi /ZI

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD BASE CPP /Gi /ZI
# ADD CPP /Gi
# SUBTRACT CPP /Z<none>

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewMon3D.h

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Relink.cpp
# End Source File
# Begin Source File

SOURCE=.\Scape3D.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /Gi /ZI

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD BASE CPP /Gi /ZI
# ADD CPP /Gi
# SUBTRACT CPP /Z<none>

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scape3D.h
# End Source File
# Begin Source File

SOURCE=.\Video.cpp
# End Source File
# Begin Source File

SOURCE=.\Video.h
# End Source File
# Begin Source File

SOURCE=.\VirtScr.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

# ADD CPP /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /ZI /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# ADD BASE CPP /Od
# ADD CPP /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD BASE CPP /ZI /Od
# ADD CPP /O2
# SUBTRACT CPP /Z<none>

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\VirtScreen.h
# End Source File
# Begin Source File

SOURCE=.\ZBuffer.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

# ADD CPP /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /Gi /ZI /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# ADD BASE CPP /Od
# ADD CPP /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD BASE CPP /Gi /ZI /Od
# ADD CPP /Gi /O2
# SUBTRACT CPP /Z<none>

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ZBuffer.h
# End Source File
# End Group
# Begin Group "Engine"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Build.cpp
# End Source File
# Begin Source File

SOURCE=.\Ddex1.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

# ADD CPP /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Economy.cpp
# End Source File
# Begin Source File

SOURCE=.\Fractal.cpp
# End Source File
# Begin Source File

SOURCE=.\Groups.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadSave.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

# ADD CPP /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# ADD CPP /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD CPP /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LoadSave.h
# End Source File
# Begin Source File

SOURCE=.\mapa.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

# ADD CPP /ZI /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /Gi /ZI
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# ADD BASE CPP /ZI /Od
# ADD CPP /ZI /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD BASE CPP /Gi /ZI
# SUBTRACT BASE CPP /YX
# ADD CPP /Gi /Zi
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MapDiscr.h
# End Source File
# Begin Source File

SOURCE=.\MapSprites.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

# ADD CPP /Zi /Od /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /ZI /Od /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# ADD BASE CPP /Zi /Ox /Ot /Ow /Oy-
# ADD CPP /Zi /Ox /Ot /Ow /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD BASE CPP /Ox /Ot /Ow /Oy-
# ADD CPP /O2 /Oy-

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MapSprites.h
# End Source File
# Begin Source File

SOURCE=.\Motion.cpp
# ADD CPP /Zi /Ox /Ot /Ow /Oy-
# End Source File
# Begin Source File

SOURCE=.\MPlayer.h
# End Source File
# Begin Source File

SOURCE=.\Multi.cpp
# End Source File
# Begin Source File

SOURCE=.\Nation.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

# ADD CPP /ZI /Od /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /ZI /Od /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# ADD CPP /ZI /Od /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD CPP /Zi /O2 /Oy-

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Nature.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

# ADD CPP /ZI

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /ZI

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# ADD CPP /ZI

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Nature.h
# End Source File
# Begin Source File

SOURCE=.\NewMon.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

# ADD CPP /Zi /Od /Op /Oy- /FAcs

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /Zi /Op- /Oy- /FAcs
# SUBTRACT CPP /Ox /Ot /Ow

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# ADD BASE CPP /Zi /Op /Oy- /FAcs
# ADD CPP /Zi /Op /Oy- /FAcs

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD BASE CPP /Zi /Op- /Oy- /FAcs
# SUBTRACT BASE CPP /Ox /Ot /Ow
# ADD CPP /O2 /Op- /Oy-
# SUBTRACT CPP /Z<none>

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewMon.h
# End Source File
# Begin Source File

SOURCE=.\NewUpgrade.cpp
# End Source File
# Begin Source File

SOURCE=.\NewUpgrade.h
# End Source File
# Begin Source File

SOURCE=.\OnWater.cpp
# End Source File
# Begin Source File

SOURCE=.\path.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

# ADD CPP /Zi /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /Ox /Ot /Ow /Oy-
# SUBTRACT CPP /Og

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# ADD BASE CPP /Zi /Oy-
# ADD CPP /Zi /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD BASE CPP /Ox /Ot /Ow /Oy-
# SUBTRACT BASE CPP /Og
# ADD CPP /O2 /Oy-

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Path.h
# End Source File
# Begin Source File

SOURCE=.\Process.h
# End Source File
# Begin Source File

SOURCE=.\RealWater.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /ZI /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD BASE CPP /ZI /Od
# ADD CPP /O2
# SUBTRACT CPP /Z<none>

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RealWater.h
# End Source File
# Begin Source File

SOURCE=.\Recorder.cpp
# End Source File
# Begin Source File

SOURCE=.\Recorder.h
# End Source File
# Begin Source File

SOURCE=.\River.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveNewMap.cpp
# End Source File
# Begin Source File

SOURCE=.\Timing.cpp
# End Source File
# Begin Source File

SOURCE=.\Transport.cpp
# End Source File
# Begin Source File

SOURCE=.\Transport.h
# End Source File
# Begin Source File

SOURCE=.\Upgrade.cpp
# End Source File
# Begin Source File

SOURCE=.\Upgrade.h
# End Source File
# Begin Source File

SOURCE=.\walls.cpp
# End Source File
# Begin Source File

SOURCE=.\walls.h
# End Source File
# Begin Source File

SOURCE=.\Weapon.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

# ADD CPP /Ox /Ot /Ow /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /Ox /Ot /Ow /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# ADD CPP /Ox /Ot /Ow /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD CPP /O2 /Oy-

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WeaponID.h
# End Source File
# End Group
# Begin Group "Scenario"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ActiveScenary.cpp
# End Source File
# Begin Source File

SOURCE=.\ActiveScenary.h
# End Source File
# Begin Source File

SOURCE=.\ActiveZone.cpp
# End Source File
# Begin Source File

SOURCE=.\ActiveZone.h
# End Source File
# Begin Source File

SOURCE=.\vital\ArmyMind.h
# End Source File
# Begin Source File

SOURCE=.\vital\BattleDialogs.h
# End Source File
# Begin Source File

SOURCE=.\GroupPurpose.cpp
# End Source File
# Begin Source File

SOURCE=.\InteractiveScenario.cpp
# End Source File
# Begin Source File

SOURCE=.\vital\vital.h
# End Source File
# Begin Source File

SOURCE=.\vital\vital_def.h
# End Source File
# End Group
# Begin Group "Debug"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AntiBug.cpp
# End Source File
# Begin Source File

SOURCE=.\AntiBug.h
# End Source File
# Begin Source File

SOURCE=.\Report.cpp
# End Source File
# Begin Source File

SOURCE=.\UnRar.cpp
# End Source File
# Begin Source File

SOURCE=.\Unrar.h
# End Source File
# Begin Source File

SOURCE=.\UnSyncro.cpp
# End Source File
# Begin Source File

SOURCE=.\UnSyncro.h
# End Source File
# End Group
# Begin Group "Device"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Cdirsnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Cdirsnd.h
# End Source File
# Begin Source File

SOURCE=.\Cwave.cpp
# End Source File
# Begin Source File

SOURCE=.\Cwave.h
# End Source File
# Begin Source File

SOURCE=.\DeviceCD.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceCD.h
# End Source File
# Begin Source File

SOURCE=.\GameSound.cpp
# End Source File
# Begin Source File

SOURCE=.\GSound.h
# End Source File
# Begin Source File

SOURCE=.\TMixer.cpp
# End Source File
# Begin Source File

SOURCE=.\TMixer.h
# End Source File
# End Group
# Begin Group "LIB"

# PROP Default_Filter "*.lib"
# Begin Source File

SOURCE=.\ProTest.lib
# End Source File
# Begin Source File

SOURCE=.\Winmm.lib
# End Source File
# Begin Source File

SOURCE=.\Pinger.lib
# End Source File
# Begin Source File

SOURCE=.\wintab\Wintab32.lib
# End Source File
# Begin Source File

SOURCE=.\Bink\binkw32.lib
# End Source File
# End Group
# Begin Group "OLD FILES"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Navorots.cpp
# End Source File
# Begin Source File

SOURCE=.\Nuclear.cpp
# End Source File
# Begin Source File

SOURCE=.\protest.h
# End Source File
# Begin Source File

SOURCE=.\Script1.rc
# End Source File
# End Group
# Begin Group "Internet Rating&Profiles"

# PROP Default_Filter ""
# Begin Group "Stats"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\InetRaiting\darray.c
# End Source File
# Begin Source File

SOURCE=.\InetRaiting\darray.h
# End Source File
# Begin Source File

SOURCE=.\InetRaiting\stats\gbucket.c
# End Source File
# Begin Source File

SOURCE=.\InetRaiting\stats\gbucket.h
# End Source File
# Begin Source File

SOURCE=.\InetRaiting\stats\gpersist.h
# End Source File
# Begin Source File

SOURCE=.\InetRaiting\stats\gstats.c
# End Source File
# Begin Source File

SOURCE=.\InetRaiting\stats\gstats.h
# End Source File
# Begin Source File

SOURCE=.\InetRaiting\hashtable.c
# End Source File
# Begin Source File

SOURCE=.\InetRaiting\hashtable.h
# End Source File
# Begin Source File

SOURCE=.\InetRaiting\md5.h
# End Source File
# Begin Source File

SOURCE=.\InetRaiting\md5c.c
# End Source File
# Begin Source File

SOURCE=.\InetRaiting\nonport.c
# End Source File
# Begin Source File

SOURCE=.\InetRaiting\nonport.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\InetRaiting\gp.h
# End Source File
# Begin Source File

SOURCE=.\queryreporting\Gqueryreporting.h
# End Source File
# Begin Source File

SOURCE=.\IR.h
# End Source File
# Begin Source File

SOURCE=.\InetRaiting\IR_Main.cpp
# End Source File
# Begin Source File

SOURCE=.\GameSpy\Nonport.h
# End Source File
# Begin Source File

SOURCE=.\GameSpy\Peer.h
# End Source File
# Begin Source File

SOURCE=.\InetRaiting\GP.lib
# End Source File
# End Group
# Begin Group "Dialogs"

# PROP Default_Filter ""
# Begin Group "MainMenu"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Dialogs\EnterName.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\HistoryBattle.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\MainMenu.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\MenuOptions.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\MLoadGame.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\MultiplayerRoom.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\PostCampaignMessage.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\ProcessCampagins.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\ProcessCapaigns.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\ProcessLoadingFile.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\ProcessMultiPlayer.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\ProcessSavingFile.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\ProcessSingleCampagin.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\ProcessSingleMission.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\ProcessSinglePlayer.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\ProcessWars.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\SelectGameToJoin.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\SelectProtocol.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\SelectSingleBattle.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\SelectSingleMission.h
# End Source File
# End Group
# Begin Group "Game"

# PROP Default_Filter ""
# Begin Group "Editor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Dialogs\EditMainMenu.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\EditorToolsPanel.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\TerrainEdit.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Dialogs\GameMainMenu.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\GameOptions.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\InGameLoadGame.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\InGameSaveGame.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\LowPanel.h
# End Source File
# Begin Source File

SOURCE=..\..\..\ACR\Battle\mainscript.txt
# End Source File
# Begin Source File

SOURCE=.\Dialogs\Messages.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\RightInfoPanel.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\RightUnitInfoPanel.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\SendResources.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\ShowResPanel.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\UpperMiniMenu.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Dialogs\AllDialogs.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\InitFonts.h
# End Source File
# Begin Source File

SOURCE=.\Interface.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

# ADD CPP /ZI /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# ADD BASE CPP /ZI /Od
# ADD CPP /ZI /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Dialogs\NewLogin.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs\SingleOptions.h
# End Source File
# End Group
# Begin Group "Bink"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Bink\Bink.h
# End Source File
# Begin Source File

SOURCE=.\Bink\PlayBink.cpp
# End Source File
# Begin Source File

SOURCE=.\Bink\Rad.h
# End Source File
# End Group
# Begin Group "MP3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Dsound.h
# End Source File
# Begin Source File

SOURCE=.\mp3\oggvor.cpp
# End Source File
# Begin Source File

SOURCE=.\mp3\oggvor.h
# End Source File
# Begin Source File

SOURCE=.\mp3\PlayMP3.cpp
# End Source File
# Begin Source File

SOURCE=.\mp3\vopl_globals.h
# End Source File
# End Group
# Begin Group "FEX"

# PROP Default_Filter ""
# Begin Group "FEX_Include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Fex\Include\BaseTsd.h
# End Source File
# Begin Source File

SOURCE=.\Fex\Include\dmusici.h
# End Source File
# Begin Source File

SOURCE=.\Fex\Include\MultiMon.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Fex\FEXModule.cpp
# End Source File
# Begin Source File

SOURCE=.\Fex\FEXModule.h
# End Source File
# Begin Source File

SOURCE=.\Fex\FEXSysInfo.cpp
# End Source File
# End Group
# Begin Group "BAT"

# PROP Default_Filter "*.bat"
# Begin Source File

SOURCE=..\Bat\all.bat
# End Source File
# End Group
# Begin Group "BrigAI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BrigadeAI.cpp
# End Source File
# Begin Source File

SOURCE=.\BrigadeAI.h
# End Source File
# End Group
# Begin Group "Topology"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HashTop.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# ADD CPP /ZI /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\HashTop.h
# End Source File
# Begin Source File

SOURCE=.\TopoGraf.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

# ADD CPP /ZI /Od /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /Ox /Ot /Ow /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# ADD CPP /ZI /Od /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD CPP /O2 /Oy-

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TopoGraf.h
# End Source File
# End Group
# Begin Group "G17"

# PROP Default_Filter ""
# Begin Group "bz2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\G17\bz2\blocksort.cpp
# End Source File
# Begin Source File

SOURCE=.\G17\bz2\bzlib.cpp
# End Source File
# Begin Source File

SOURCE=.\G17\bz2\bzlib.h
# End Source File
# Begin Source File

SOURCE=.\G17\bz2\bzlib_private.h
# End Source File
# Begin Source File

SOURCE=.\G17\bz2\compress.cpp
# End Source File
# Begin Source File

SOURCE=.\G17\bz2\crctable.cpp
# End Source File
# Begin Source File

SOURCE=.\G17\bz2\decompress.cpp
# End Source File
# Begin Source File

SOURCE=.\G17\bz2\huffman.cpp
# End Source File
# Begin Source File

SOURCE=.\G17\bz2\randtable.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\G17\FCompressor.cpp
# End Source File
# Begin Source File

SOURCE=.\G17\FCompressor.h
# End Source File
# End Group
# Begin Group "others"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ChkInet.cpp
# End Source File
# Begin Source File

SOURCE=.\ConstStr.cpp
# End Source File
# Begin Source File

SOURCE=.\ConstStr.h
# End Source File
# Begin Source File

SOURCE=.\CTables.h
# End Source File
# Begin Source File

SOURCE=.\Danger.cpp
# End Source File
# Begin Source File

SOURCE=.\Danger.h
# End Source File
# Begin Source File

SOURCE=.\diplomacy.h
# End Source File
# Begin Source File

SOURCE=.\ExConst.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtDial.cpp
# End Source File
# Begin Source File

SOURCE=.\Factures3D.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /ZI /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD BASE CPP /ZI /Od
# ADD CPP /Od
# SUBTRACT CPP /Z<none>

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FBI.cpp
# End Source File
# Begin Source File

SOURCE=.\FMM\FMM.h
# End Source File
# Begin Source File

SOURCE=.\FMM\FMManager.cpp
# End Source File
# Begin Source File

SOURCE=.\FMM\FMManager.h
# End Source File
# Begin Source File

SOURCE=.\GP_Proc.h
# End Source File
# Begin Source File

SOURCE=.\GroundPieces.cpp
# End Source File
# Begin Source File

SOURCE=.\HiBars.cpp
# End Source File
# Begin Source File

SOURCE=.\Http\HttpComm.cpp
# End Source File
# Begin Source File

SOURCE=.\Http\HttpComm.h
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\Limitations.h
# End Source File
# Begin Source File

SOURCE=.\Mechanics.cpp
# End Source File
# Begin Source File

SOURCE=.\Mine.cpp
# End Source File
# Begin Source File

SOURCE=.\MiniMap4X.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /Gi /ZI

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD BASE CPP /Gi /ZI
# ADD CPP /Gi
# SUBTRACT CPP /Z<none>

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Morale.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

# ADD CPP /Ox /Ot /Ow /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /Ox /Ot /Ow /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# ADD CPP /ZI /Od /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD BASE CPP /Ox /Ot /Ow /Oy-
# ADD CPP /O2 /Oy-

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewILogin.cpp
# End Source File
# Begin Source File

SOURCE=.\NewInternetReport.cpp
# End Source File
# Begin Source File

SOURCE=.\newmemory.h
# End Source File
# Begin Source File

SOURCE=.\wintab\Pktdef.h
# End Source File
# Begin Source File

SOURCE=.\Bink\radbase.h
# End Source File
# Begin Source File

SOURCE=.\Reflection.cpp
# End Source File
# Begin Source File

SOURCE=.\Select.cpp
# End Source File
# Begin Source File

SOURCE=.\SelProp.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

# ADD CPP /Ox /Ot /Ow /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /Ox /Ot /Ow /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# ADD CPP /Ox /Ot /Ow /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD CPP /O2 /Oy-

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ShipTrace.cpp
# End Source File
# Begin Source File

SOURCE=.\Sort.h
# End Source File
# Begin Source File

SOURCE=.\stRecog.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

# ADD CPP /ZI /Od /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /Zi /Od /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

# ADD BASE CPP /ZI /Od /Oy-
# ADD CPP /ZI /Od /Oy-

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD BASE CPP /Zi /Od /Oy-
# ADD CPP /Zi /Od /Oy-

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StringHash.h
# End Source File
# Begin Source File

SOURCE=.\StringsHash.cpp
# End Source File
# Begin Source File

SOURCE=.\testserver.cpp
# End Source File
# Begin Source File

SOURCE=.\Waves.cpp
# End Source File
# Begin Source File

SOURCE=.\wintab\Wintab.h
# End Source File
# Begin Source File

SOURCE=.\wintab\Wintabx.h
# End Source File
# Begin Source File

SOURCE=.\wintab\WTSupport.cpp
# End Source File
# End Group
# Begin Group "Effects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PiroObjects.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /ZI /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SpraySystem.cpp
# End Source File
# Begin Source File

SOURCE=.\SpraySystem.h
# End Source File
# Begin Source File

SOURCE=.\SquareObject.cpp

!IF  "$(CFG)" == "cossacks2 - Win32 Release"

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d"

# ADD CPP /ZI /Od

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release_CII_test"

!ELSEIF  "$(CFG)" == "cossacks2 - Win32 Release3d_inl"

# ADD BASE CPP /ZI /Od
# ADD CPP /O2
# SUBTRACT CPP /Z<none>

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SquareObject.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\3DGroundModels.cpp
# End Source File
# End Target
# End Project
