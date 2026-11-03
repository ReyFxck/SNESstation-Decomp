//----------------------------------------------------------------------------
#ifndef OCBH
#define OCBH
//----------------------------------------------------------------------------
#include <System.hpp>
#include <Windows.hpp>
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Controls.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//----------------------------------------------------------------------------
class TfrmAboutDialog : public TForm
{
__published:
	TButton *OKBtn;
	TBevel *Bevel1;
    TMemo *memo;
private:
public:
	virtual __fastcall TfrmAboutDialog(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TfrmAboutDialog *frmAboutDialog;
//----------------------------------------------------------------------------
#endif
