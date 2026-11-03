//---------------------------------------------------------------------------
#ifndef UfrmNetPlayOptionsH
#define UfrmNetPlayOptionsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "cspin.h"
#include <Mask.hpp>
//---------------------------------------------------------------------------
#include "../snes9x/snes9x.h"
#include "../snes9x/netplay.h"
#include "CSPIN.h"
//---------------------------------------------------------------------------
class TfrmNetPlayOptions : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TButton *btnCancel;
    TButton *btnOK;
    TGroupBox *GroupBox1;
    TGroupBox *GroupBox2;
    TCSpinEdit *when_to_pause_spin;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TCSpinEdit *max_frame_skip_spin;
    TLabel *Label1;
    TMaskEdit *server_port_text;
    TGroupBox *GroupBox3;
    TCheckBox *send_rom_check;
    TCheckBox *sync_by_reset_check;
    TCheckBox *server_check;
    void __fastcall OK(TObject *Sender);
private:	// User declarations
    struct SSettings *settings;
    struct SNetPlay *netplay;
    struct SNPServer *server;
public:		// User declarations
    __fastcall TfrmNetPlayOptions (TComponent* Owner, struct SSettings *,
                                   struct SNetPlay *, struct SNPServer *);
    void __fastcall init ();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmNetPlayOptions *frmNetPlayOptions;
//---------------------------------------------------------------------------
#endif
