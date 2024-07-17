#include "..\common.h"
#include "..\IntExplorer\ParseRQ.h"
#include "import.h"
#include "BasicDiploRelation.h"
#include "DIP_SimpleBuilding.h"
#include "DiplomacySystem.h"
#include "DString.h"
#include "BattleDialogs.h"
CIMPORT
bool ReadWinString(GFILE* F,char* STR,int Max);
CIMPORT
void ActivateDipDialog(char* request);
CIMPORT
void SendDipCommand(char* Data,int size);
DLLIMPORT void SXP_SetVar(int Index, char* Name, char* value);
DLLIMPORT char* SXP_GetVar(int Index, char* Name);