// GSCset.cpp: implementation of the CGSCset class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "GSCset.h"

#include <stdio.h>
#include "..\ddini.h"
#include "GSCtypes.h"
#include "GSCarch.h"
#include "GSCset.h"
    #include <fstream>
#include <string>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGSCset::CGSCset()
{

}

CGSCset::~CGSCset()
{

}

LPGSCfile CGSCset::gOpenFile(LPCSTR lpcsFileName,bool Only)
{
	LPGSCfile		gFile=NULL;
	HANDLE			hFindFile;
	WIN32_FIND_DATA	FindData;
	BOOL			isArch=TRUE;

	if(!Only){
		hFindFile=FindFirstFile(lpcsFileName,&FindData);
		if(hFindFile!=INVALID_HANDLE_VALUE){
    		isArch=FALSE;
			FindClose(hFindFile);
		};
	};

	if(isArch){
		LPGSCArchList	pArchList=m_ArchList;

		while(pArchList){
			gFile=pArchList->m_Arch->GetFileHandle(lpcsFileName);
			if(gFile){
				gFile->m_Arch=pArchList->m_Arch;
				return gFile;
			};
			pArchList=pArchList->m_NextArch;
		};
	}
	else
	{
		gFile=new TGSCfile;
		gFile->m_Flags=0;
		gFile->m_Position=0;
		gFile->m_Arch=NULL;
		gFile->m_Data=NULL;

		gFile->m_FileHandle=DWORD(CreateFile(lpcsFileName,
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				0));

		if(gFile->m_FileHandle==(DWORD)INVALID_HANDLE_VALUE){
			delete gFile;
			return NULL;
		};

		gFile->m_Size=GetFileSize((HANDLE)gFile->m_FileHandle,NULL);
	};

	return gFile;
}

VOID CGSCset::gCloseFile(LPGSCfile gFile)
{
    if (nullptr == gFile)//BUGFIX: Exception when closing game via Alt+F4
    {
        return;
    }
	if((gFile->m_Flags)&0x80){
		gFile->m_Arch->CloseFileHandle(gFile);
	}else{
		CloseHandle(HANDLE(gFile->m_FileHandle));
		SAFE_DELETE(gFile);
	};
}

VOID CGSCset::gReadFile(LPGSCfile gFile, LPBYTE lpbBuffer, DWORD dwSize)
{
	DWORD	dwBytesRead=0;

	if((gFile->m_Flags)&0x80){
		gFile->m_Arch->ReadFile(gFile,lpbBuffer,dwSize);
	}else{
		ReadFile(HANDLE(gFile->m_FileHandle),lpbBuffer,dwSize,&dwBytesRead,NULL);
	};
}


VOID CGSCset::gSeekFile(LPGSCfile gFile, DWORD dwPosition)
{
	if((gFile->m_Flags)&0x80){
		gFile->m_Position=dwPosition;
	}else{
		SetFilePointer(HANDLE(gFile->m_FileHandle),dwPosition,NULL,FILE_BEGIN);
	};
}

// -- Inserted by Silver ---21.02.2003
VOID CGSCset::gSeekFileFromCurrent(LPGSCfile gFile, DWORD dwAmount)
{
	if((gFile->m_Flags)&0x80){
		gFile->m_Position += dwAmount;
	}else{
		SetFilePointer(HANDLE(gFile->m_FileHandle),dwAmount,NULL,FILE_CURRENT);
	};
} // CGSCset::gSeekFileFromCurrent


DWORD CGSCset::gFilePos(LPGSCfile gFile)
{
	if((gFile->m_Flags)&0x80){
		return gFile->m_Position;
	}else{
		return SetFilePointer(HANDLE(gFile->m_FileHandle),0,NULL,FILE_CURRENT);
	};
}

VOID AddItem(LPGSCFileList pFileList, LPCSTR lpcsFileName)
{
	LPGSCFileList	pList=pFileList, pItem;
	BOOL	Exists=FALSE;

	while(pList){
		if(!strcmp(pList->m_FileName,lpcsFileName)){
			Exists=TRUE;
			break;
		};
		pList=pList->m_NextEntry;
	};

	if(!Exists){
		pList=pFileList;

		while(pList->m_NextEntry)
			pList=pList->m_NextEntry;

		pItem=new TGSCFileList;
		pItem->m_NextEntry=NULL;
		strcpy(pItem->m_FileName,lpcsFileName);

		pList->m_NextEntry=pItem;

	};
}

LPGSCFindInfo CGSCset::gFindFirst(LPCSTR lpcsMask)
{
	LPGSCFindInfo lpFindInfo=NULL;
	WIN32_FIND_DATA	FindData;
	HANDLE	hFindFile;
	LPGSCFileList	lpFileList=NULL;

	CHAR	sUpMask[64];
	ZeroMemory(sUpMask,64);
	strcpy(sUpMask,lpcsMask);
	_strupr(sUpMask);

	hFindFile=FindFirstFile(sUpMask,&FindData);

	if(hFindFile!=INVALID_HANDLE_VALUE){
		lpFindInfo=new TGSCFindInfo;
		lpFindInfo->m_Pos=NULL;
		lpFindInfo->m_FileList=NULL;

		lpFileList=new TGSCFileList;
		lpFindInfo->m_FileList=lpFileList;				
		lpFindInfo->m_Pos=lpFileList;				
		lpFileList->m_NextEntry=NULL;
		strcpy(lpFileList->m_FileName,FindData.cFileName);
		_strupr(FindData.cFileName);

		while(FindNextFile(hFindFile,&FindData)){
			lpFileList->m_NextEntry=new TGSCFileList;
			lpFileList=lpFileList->m_NextEntry;
			lpFileList->m_NextEntry=NULL;
			strcpy(lpFileList->m_FileName,FindData.cFileName);
			_strupr(FindData.cFileName);
		};
		FindClose(hFindFile);
	};

	LPGSCArchList	pArchList=m_ArchList;
	LPGSCFindData	pFindData;

	while(pArchList){
		pFindData=pArchList->m_Arch->FindFile(sUpMask);
		
		if(pFindData){
			if(!lpFindInfo){
				lpFindInfo=new TGSCFindInfo;
				lpFindInfo->m_Pos=NULL;
				lpFindInfo->m_FileList=NULL;

				lpFileList=new TGSCFileList;
				lpFindInfo->m_FileList=lpFileList;				
				lpFindInfo->m_Pos=lpFileList;				
				lpFileList->m_NextEntry=NULL;
				strcpy(lpFileList->m_FileName,pFindData->m_FileName);
			}else
				AddItem(lpFindInfo->m_FileList,pFindData->m_FileName);

			while(pArchList->m_Arch->NextFile(pFindData)){
				AddItem(lpFindInfo->m_FileList,pFindData->m_FileName);
			};
		};	

		pArchList=pArchList->m_NextArch;
	};

	if(lpFindInfo){
		strcpy(lpFindInfo->m_FileName,lpFindInfo->m_Pos->m_FileName);
		return lpFindInfo;
	}
	else
		return NULL;
}

BOOL CGSCset::gFindNext(LPGSCFindInfo gFindInfo)
{
 gFindInfo->m_Pos=gFindInfo->m_Pos->m_NextEntry;

 if(gFindInfo->m_Pos){
	strcpy(gFindInfo->m_FileName,gFindInfo->m_Pos->m_FileName);
	return TRUE;
 }else{
	LPGSCFileList	lpFileList1,lpFileList2;

	lpFileList2=gFindInfo->m_FileList;

	while(lpFileList2){
		lpFileList1=lpFileList2;
		lpFileList2=lpFileList1->m_NextEntry;
		
		delete lpFileList1;
	};	 
	 
	delete gFindInfo;
		 
	return FALSE;
 };	

}
char ModName[20];
int ModIterator = 0;
void GSC_OpenError();
BOOL CGSCset::gOpen()
{
	WIN32_FIND_DATA		FindData;
	HANDLE				hFindFile;
	LPGSCArchList		pArchList=NULL;

	m_ArchList=NULL;
//  
    /*std::fstream fs;
    fs.open("modload.dat", std::fstream::in);
    char line[256]; 
    char ModName[20];
    int ModIterator=0;
    while (fs.getline(line, sizeof(line))) {
        int i = 0;
        int ModNmbr = 0;

        while (line[i] != '\0') {
            if (line[i] == ';') {  
                ModName[ModNmbr] = '\0'; 
                ModNmbr = 0;
            }
            else {
                ModName[ModNmbr++] = line[i];
                ModIterator++;
            }
            i++;
        }
        if (ModNmbr > 0) {
            //ModIterator++;
            ModName[ModNmbr] = '\0';
        }
    }
    fs.close();
    if (ModIterator > 0) {
        for (int j = 0; j < ModIterator; j++) {
            char imemoda = ModName[j];
            for (int i = 10; i > 0; i--) {
                char mods[50];
                sprintf(mods, "mods\\%d\\mods0%d.gs1", imemoda, i);

                hFindFile = FindFirstFile(mods, &FindData);
                if (hFindFile != INVALID_HANDLE_VALUE) {
                    if (m_ArchList == NULL) {
                        pArchList = new TGSCArchList;
                        m_ArchList = pArchList;
                    }
                    else {
                        pArchList->m_NextArch = new TGSCArchList;
                        pArchList = pArchList->m_NextArch;
                    };

                    pArchList->m_NextArch = NULL;
                    pArchList->m_Arch = new CGSCarch;

                    pArchList->m_Arch->Open(FindData.cFileName);

                    while (FindNextFile(hFindFile, &FindData)) {
                        pArchList->m_NextArch = new TGSCArchList;
                        pArchList = pArchList->m_NextArch;

                        pArchList->m_Arch = new CGSCarch;
                        pArchList->m_NextArch = NULL;
                        pArchList->m_Arch->Open(FindData.cFileName);
                    };
                    FindClose(hFindFile);
                };
            }

            char singlemod[50];
            sprintf(singlemod, "mods\\%d\\mods.gs1", imemoda);
            hFindFile = FindFirstFile(singlemod, &FindData);

            if (hFindFile != INVALID_HANDLE_VALUE) {
                if (m_ArchList == NULL) {
                    pArchList = new TGSCArchList;
                    m_ArchList = pArchList;
                }
                else {
                    pArchList->m_NextArch = new TGSCArchList;
                    pArchList = pArchList->m_NextArch;
                };

                pArchList->m_NextArch = NULL;
                pArchList->m_Arch = new CGSCarch;

                pArchList->m_Arch->Open(FindData.cFileName);

                while (FindNextFile(hFindFile, &FindData)) {
                    pArchList->m_NextArch = new TGSCArchList;
                    pArchList = pArchList->m_NextArch;

                    pArchList->m_Arch = new CGSCarch;
                    pArchList->m_NextArch = NULL;
                    pArchList->m_Arch->Open(FindData.cFileName);
                };
                FindClose(hFindFile);
            };

            for (int i = 10; i > 0; i--) {

                char patch[50];
                sprintf(patch, "mods\\%d\\patch0%d.gs1", imemoda, i);

                hFindFile = FindFirstFile(patch, &FindData);

                if (hFindFile != INVALID_HANDLE_VALUE) {
                    if (m_ArchList == NULL) {
                        pArchList = new TGSCArchList;
                        m_ArchList = pArchList;
                    }
                    else {
                        pArchList->m_NextArch = new TGSCArchList;
                        pArchList = pArchList->m_NextArch;
                    };

                    pArchList->m_NextArch = NULL;
                    pArchList->m_Arch = new CGSCarch;

                    pArchList->m_Arch->Open(FindData.cFileName);

                    while (FindNextFile(hFindFile, &FindData)) {
                        pArchList->m_NextArch = new TGSCArchList;
                        pArchList = pArchList->m_NextArch;

                        pArchList->m_Arch = new CGSCarch;
                        pArchList->m_NextArch = NULL;
                        pArchList->m_Arch->Open(FindData.cFileName);
                    };
                    FindClose(hFindFile);
                };
            }

            //if(hFindFile!=INVALID_HANDLE_VALUE){
            //	pArchList=new TGSCArchList;

            //	m_ArchList=pArchList;

            //	pArchList->m_NextArch=NULL;
            //	pArchList->m_Arch=new CGSCarch;
            //	pArchList->m_Arch->Open(FindData.cFileName);
            //	FindClose(hFindFile);
            //};
        //
            char all[50];
            sprintf(all, "mods\\%d\\A*.gsc", imemoda);
            hFindFile = FindFirstFile(all, &FindData);

            if (hFindFile != INVALID_HANDLE_VALUE) {

                if (m_ArchList == NULL) {
                    pArchList = new TGSCArchList;
                    m_ArchList = pArchList;
                }
                else {
                    pArchList->m_NextArch = new TGSCArchList;
                    pArchList = pArchList->m_NextArch;
                };

                pArchList->m_NextArch = NULL;
                pArchList->m_Arch = new CGSCarch;

                pArchList->m_Arch->Open(FindData.cFileName);

                while (FindNextFile(hFindFile, &FindData)) {
                    pArchList->m_NextArch = new TGSCArchList;
                    pArchList = pArchList->m_NextArch;

                    pArchList->m_Arch = new CGSCarch;
                    pArchList->m_NextArch = NULL;
                    pArchList->m_Arch->Open(FindData.cFileName);
                };
                FindClose(hFindFile);
            };
        }
    }
    else {*/
        for (int i = 10; i > 0; i--) {
            char mods[50];
            sprintf(mods, "mods0%d.gs1", i);

            hFindFile = FindFirstFile(mods, &FindData);
            if (hFindFile != INVALID_HANDLE_VALUE) {
                if (m_ArchList == NULL) {
                    pArchList = new TGSCArchList;
                    m_ArchList = pArchList;
                }
                else {
                    pArchList->m_NextArch = new TGSCArchList;
                    pArchList = pArchList->m_NextArch;
                };

                pArchList->m_NextArch = NULL;
                pArchList->m_Arch = new CGSCarch;

                pArchList->m_Arch->Open(FindData.cFileName);

                while (FindNextFile(hFindFile, &FindData)) {
                    pArchList->m_NextArch = new TGSCArchList;
                    pArchList = pArchList->m_NextArch;

                    pArchList->m_Arch = new CGSCarch;
                    pArchList->m_NextArch = NULL;
                    pArchList->m_Arch->Open(FindData.cFileName);
                };
                FindClose(hFindFile);
            };
        }

        hFindFile = FindFirstFile("mods.gs1", &FindData);

        if (hFindFile != INVALID_HANDLE_VALUE) {
            if (m_ArchList == NULL) {
                pArchList = new TGSCArchList;
                m_ArchList = pArchList;
            }
            else {
                pArchList->m_NextArch = new TGSCArchList;
                pArchList = pArchList->m_NextArch;
            };

            pArchList->m_NextArch = NULL;
            pArchList->m_Arch = new CGSCarch;

            pArchList->m_Arch->Open(FindData.cFileName);

            while (FindNextFile(hFindFile, &FindData)) {
                pArchList->m_NextArch = new TGSCArchList;
                pArchList = pArchList->m_NextArch;

                pArchList->m_Arch = new CGSCarch;
                pArchList->m_NextArch = NULL;
                pArchList->m_Arch->Open(FindData.cFileName);
            };
            FindClose(hFindFile);
        };

        for (int i = 10; i > 0; i--) {

            char patch[50];
            sprintf(patch, "patch0%d.gs1", i);

            hFindFile = FindFirstFile(patch, &FindData);

            if (hFindFile != INVALID_HANDLE_VALUE) {
                if (m_ArchList == NULL) {
                    pArchList = new TGSCArchList;
                    m_ArchList = pArchList;
                }
                else {
                    pArchList->m_NextArch = new TGSCArchList;
                    pArchList = pArchList->m_NextArch;
                };

                pArchList->m_NextArch = NULL;
                pArchList->m_Arch = new CGSCarch;

                pArchList->m_Arch->Open(FindData.cFileName);

                while (FindNextFile(hFindFile, &FindData)) {
                    pArchList->m_NextArch = new TGSCArchList;
                    pArchList = pArchList->m_NextArch;

                    pArchList->m_Arch = new CGSCarch;
                    pArchList->m_NextArch = NULL;
                    pArchList->m_Arch->Open(FindData.cFileName);
                };
                FindClose(hFindFile);
            };
        }

        //if(hFindFile!=INVALID_HANDLE_VALUE){
        //	pArchList=new TGSCArchList;

        //	m_ArchList=pArchList;

        //	pArchList->m_NextArch=NULL;
        //	pArchList->m_Arch=new CGSCarch;
        //	pArchList->m_Arch->Open(FindData.cFileName);
        //	FindClose(hFindFile);
        //};
    //
        hFindFile = FindFirstFile("A*.gsc", &FindData);

        if (hFindFile != INVALID_HANDLE_VALUE) {

            if (m_ArchList == NULL) {
                pArchList = new TGSCArchList;
                m_ArchList = pArchList;
            }
            else {
                pArchList->m_NextArch = new TGSCArchList;
                pArchList = pArchList->m_NextArch;
            };

            pArchList->m_NextArch = NULL;
            pArchList->m_Arch = new CGSCarch;

            pArchList->m_Arch->Open(FindData.cFileName);

            while (FindNextFile(hFindFile, &FindData)) {
                pArchList->m_NextArch = new TGSCArchList;
                pArchList = pArchList->m_NextArch;

                pArchList->m_Arch = new CGSCarch;
                pArchList->m_NextArch = NULL;
                pArchList->m_Arch->Open(FindData.cFileName);
            };
            FindClose(hFindFile);
        };
    //}
	
	return TRUE;
}

VOID CGSCset::gClose()
{
	LPGSCArchList	lpArchList1,lpArchList2;

	lpArchList2=m_ArchList;

	while(lpArchList2){
		lpArchList1=lpArchList2;
		lpArchList2=lpArchList1->m_NextArch;
		
		lpArchList1->m_Arch->Close();
		delete lpArchList1->m_Arch;

		delete lpArchList1;
	};	 
	 
}

DWORD CGSCset::gFileSize(LPGSCfile gFile)
{
	if((gFile->m_Flags)&0x80){
		return gFile->m_Size;
	}else{
		return GetFileSize(HANDLE(gFile->m_FileHandle),NULL);
	};
}

LPBYTE CGSCset::gMapFile(LPGSCfile gFile)
{
	if((gFile->m_Flags)&0x80){
		return gFile->m_Arch->GetFileData(gFile);
	}else{
		return NULL;
	};
}

BOOL CGSCset::gWriteFile(LPGSCfile gFile, LPBYTE lpbBuffer, DWORD dwSize)
{
	DWORD dwBytesWritten;
	if((gFile->m_Flags)&0x80){
		return FALSE;
	}else{
		WriteFile(HANDLE(gFile->m_FileHandle),lpbBuffer,dwSize,&dwBytesWritten,NULL);
		return TRUE;
	};
}

LPGSCfile CGSCset::gWriteOpen(LPCSTR lpcsFileName)
{
	LPGSCfile gFile;
	
	gFile=new TGSCfile;
	gFile->m_Flags=0;
	gFile->m_Position=0;
	gFile->m_Arch=NULL;
	gFile->m_Data=NULL;
	gFile->m_Size=0;	

	gFile->m_FileHandle=DWORD(CreateFile(lpcsFileName,
								GENERIC_READ | GENERIC_WRITE,
								FILE_SHARE_READ,
								NULL,
								CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL,
								0));
	if(gFile->m_FileHandle==(DWORD)INVALID_HANDLE_VALUE){
		delete gFile;
		return NULL;
	};

	return gFile;
}

