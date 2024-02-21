#ifndef __C2EXPDIALOG_H__
#define __C2EXPDIALOG_H__


typedef void (*ExportCallback)();

BOOL CALLBACK dlgProc(  HWND hWnd,
						UINT message,
						WPARAM wParam,
						LPARAM lParam );

void Msg( const char* format, ... );

void SetProgressPos( int pos );
void SetProgressRange( int min, int max );
void DisableStuff();
void EnableStuff();

void GetValues();
void SetValues();

class IExporter
{
public:
	virtual void StartExporting() = 0;
};

void ShowExportDialog( IExporter* exp );


#endif // __C2EXPDIALOG_H__