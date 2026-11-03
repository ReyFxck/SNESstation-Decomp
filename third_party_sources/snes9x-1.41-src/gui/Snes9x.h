//---------------------------------------------------------------------------
#ifndef Snes9xH
#define Snes9xH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class TfrmSnes9X : public TForm
{
__published:	// IDE-managed Components
    TToolBar *ToolBar1;
    TStatusBar *StatusBar1;
    TButton *Button1;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
private:	// User declarations
public:		// User declarations
    __fastcall TfrmSnes9X(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSnes9X *frmSnes9X;
//---------------------------------------------------------------------------
#endif
