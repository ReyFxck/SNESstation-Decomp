//---------------------------------------------------------------------------
#ifndef UfrmProgressH
#define UfrmProgressH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "cgauges.h"
#include <ExtCtrls.hpp>
#include "CGAUGES.h"
//---------------------------------------------------------------------------
class TfrmProgress : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TButton *cancel_button;
    TLabel *label;
    TCGauge *progress;
private:	// User declarations
public:		// User declarations
    __fastcall TfrmProgress(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmProgress *frmProgress;
//---------------------------------------------------------------------------
#endif
