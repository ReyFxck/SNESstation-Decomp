//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UfrmProgress.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cgauges"
#pragma link "CGAUGES"
#pragma resource "*.dfm"
TfrmProgress *frmProgress;
//---------------------------------------------------------------------------
__fastcall TfrmProgress::TfrmProgress(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
