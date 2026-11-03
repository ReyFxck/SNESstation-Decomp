//---------------------------------------------------------------------------
#ifndef UfrmOptionsH
#define UfrmOptionsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
#include "..\wsnes9x.h"
#include "..\snes9x\snes9x.h"
#include "cspin.h"
#include "CSPIN.h"
//---------------------------------------------------------------------------

class TfrmSettingsDialog : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TButton *cancel_button;
    TButton *ok_button;
    TEdit *pathname_text;
    TButton *browse_button;
    TLabel *Label1;
    TSaveDialog *open_dialog;
    TCSpinEdit *auto_save_spin;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TCSpinEdit *turbo_frame_skip_spin;
    TLabel *Label5;
    TCheckBox *cbx_ToggleTurboMode;
    TCSpinEdit *auto_frame_skip_spin;
    TLabel *Label6;
    TLabel *Label7;
    void __fastcall Browse(TObject *Sender);
    void __fastcall OK(TObject *Sender);
    void __fastcall NowVisible(TObject *Sender);
private:	// User declarations
    SSettings *settings;
    sGUI *gui;
public:		// User declarations
    __fastcall TfrmSettingsDialog(TComponent* Owner, SSettings *, sGUI *);
    void __fastcall init ();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSettingsDialog *frmSettingsDialog;
//---------------------------------------------------------------------------
#endif
