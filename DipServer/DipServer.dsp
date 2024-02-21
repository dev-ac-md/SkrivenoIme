# Microsoft Developer Studio Project File - Name="DipServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=DipServer - Win32 Debug_CII
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DipServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DipServer.mak" CFG="DipServer - Win32 Debug_CII"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DipServer - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DipServer - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DipServer - Win32 Debug_CII" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/cossacks2", LAAAAAAA"
# PROP Scc_LocalPath "\cossacks2\"
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DipServer - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DIPSERVER_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DIPSERVER_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /map /machine:I386 /out:"c:\AC\COSSACKS2\DipServer.dll" /libpath:"../lib" /libpath:"../_release" /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "DipServer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../_debug"
# PROP Intermediate_Dir "../_debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DIPSERVER_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /ML /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DIPSERVER_EXPORTS" /D "AC_ADDON" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\bin\dmcr.lib intExplorer.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /map:"..\bin\DipServer.map" /debug /machine:I386 /out:"..\bin\DipServer.dll" /pdbtype:sept /libpath:"../lib" /libpath:"../_debug" /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=..\bat\all.bat
# End Special Build Tool

!ELSEIF  "$(CFG)" == "DipServer - Win32 Debug_CII"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DipServer___Win32_Debug_CII"
# PROP BASE Intermediate_Dir "DipServer___Win32_Debug_CII"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../_debug/DipServer"
# PROP Intermediate_Dir "../_debug/DipServer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /ML /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DIPSERVER_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "DIPSERVER_EXPORTS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_COSSACKS2" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /map /debug /machine:I386 /out:"e:\arc_usa\DipServer.dll" /pdbtype:sept /libpath:"../lib" /libpath:"../_debug" /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ..\lib\cossacks2.lib intExplorer.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /map /debug /machine:I386 /out:"c:\cossacks2\DipServer.dll" /pdbtype:sept /libpath:"../lib" /libpath:"../_debug" /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Copying $(InputName).lib to the ..\lib directory...
InputPath=\COSSACKS2\DipServer.dll
InputName=DipServer
SOURCE="$(InputPath)"

"..\lib\$(InputName).lib" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if           not           exist            ..\lib\             md             ..\lib\  
	copy ..\_debug\DipServer\$(InputName).lib ..\lib\$(InputName).lib 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "DipServer - Win32 Release"
# Name "DipServer - Win32 Debug"
# Name "DipServer - Win32 Debug_CII"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BasicDiploRelation.cpp
# End Source File
# Begin Source File

SOURCE=.\DIP_SimpleBuilding.cpp
# End Source File
# Begin Source File

SOURCE=.\DipInit.cpp
# End Source File
# Begin Source File

SOURCE=.\DiplomacySystem.cpp
# End Source File
# Begin Source File

SOURCE=.\DipMain.cpp
# End Source File
# Begin Source File

SOURCE=.\DipServer.cpp
# End Source File
# Begin Source File

SOURCE=.\DString.cpp
# End Source File
# Begin Source File

SOURCE=.\Economic.cpp
# End Source File
# Begin Source File

SOURCE=..\IntExplorer\ParseRQ.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddScenario.h
# End Source File
# Begin Source File

SOURCE=.\BasicDiploRelation.h
# End Source File
# Begin Source File

SOURCE=.\CommonDip.h
# End Source File
# Begin Source File

SOURCE=.\DIP_SimpleBuilding.h
# End Source File
# Begin Source File

SOURCE=.\DiplomacySystem.h
# End Source File
# Begin Source File

SOURCE=.\DString.h
# End Source File
# Begin Source File

SOURCE=.\Economic.h
# End Source File
# Begin Source File

SOURCE=.\Import.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\UnitsGroup.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\..\Acr\Battle\tmp.cml
# End Source File
# Begin Source File

SOURCE=..\..\..\UpgCategory.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Acr\War01.scr
# End Source File
# End Group
# Begin Group "vital"

# PROP Default_Filter ""
# Begin Group " battle scripts"

# PROP Default_Filter "*.scr"
# Begin Source File

SOURCE=..\..\..\Acr\Battle\War01.scr
# End Source File
# Begin Source File

SOURCE=..\..\..\Acr\Battle\War02.scr
# End Source File
# Begin Source File

SOURCE=..\..\..\Acr\Battle\War03.scr
# End Source File
# Begin Source File

SOURCE=..\..\..\Acr\Battle\War04.scr
# End Source File
# Begin Source File

SOURCE=..\..\..\Acr\Battle\War05.scr
# End Source File
# Begin Source File

SOURCE=..\..\..\Acr\Battle\War06.scr
# End Source File
# Begin Source File

SOURCE=..\..\..\Acr\Battle\War07.scr
# End Source File
# Begin Source File

SOURCE=..\..\..\Acr\Battle\War08.scr
# End Source File
# Begin Source File

SOURCE=..\..\..\Acr\Battle\War09.scr
# End Source File
# Begin Source File

SOURCE=..\..\..\Acr\Battle\War10.scr
# End Source File
# End Group
# Begin Source File

SOURCE=.\BattleDialogs.cpp
# End Source File
# Begin Source File

SOURCE=.\BattleDialogs.h
# End Source File
# Begin Source File

SOURCE=.\Mind.cpp
# End Source File
# Begin Source File

SOURCE=.\Mind.h
# End Source File
# Begin Source File

SOURCE=.\Script.cpp
# End Source File
# Begin Source File

SOURCE=.\Script.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
