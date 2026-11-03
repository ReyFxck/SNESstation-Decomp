//---------------------------------------------------------------------------
#ifndef UfrmTestDisplayModeH
#define UfrmTestDisplayModeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmTestDisplayMode : public TForm
{
__published:	// IDE-managed Components
    TLabel *l_Displaymode;
    TButton *Button1;
    TButton *Button2;
    TLabel *Label1;
    TTimer *t_TimesUp;
    void __fastcall t_TimesUpTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmTestDisplayMode(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmTestDisplayMode *frmTestDisplayMode;
//---------------------------------------------------------------------------
#endif
