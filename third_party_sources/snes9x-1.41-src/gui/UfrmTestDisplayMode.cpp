//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UfrmTestDisplayMode.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmTestDisplayMode *frmTestDisplayMode;
//---------------------------------------------------------------------------
__fastcall TfrmTestDisplayMode::TfrmTestDisplayMode(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmTestDisplayMode::t_TimesUpTimer(TObject *Sender)
{
  ModalResult = mrNo;    
}
//---------------------------------------------------------------------------
