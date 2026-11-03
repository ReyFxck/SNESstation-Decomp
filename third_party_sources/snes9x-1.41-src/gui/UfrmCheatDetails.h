//---------------------------------------------------------------------------
#ifndef UfrmCheatDetailsH
#define UfrmCheatDetailsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "../snes9x/snes9x.h"
#include "../snes9x/cheats.h"
//---------------------------------------------------------------------------
class TFrmCheatDetails : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TButton *btnCancel;
    TButton *ok_button;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *address_text;
    TEdit *current_value_text;
    TEdit *previous_value_text;
    TEdit *new_value_text;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TEdit *description_text;
    void __fastcall NewValueKeyPress(TObject *Sender, char &Key);
    void __fastcall DescriptionEnter(TObject *Sender);
    void __fastcall DescriptionExit(TObject *Sender);
    void __fastcall DescriptionKeyPress(TObject *Sender, char &Key);
    void __fastcall NowVisible(TObject *Sender);
    
    
    
    void __fastcall OK(TObject *Sender);
    
private:	// User declarations
    struct SCheatData *data;
    uint32 address;
    uint32 current_value;
    uint32 previous_value;
    int    num_bytes;
public:		// User declarations
    __fastcall TFrmCheatDetails(TComponent* Owner, 
                                struct SCheatData *d,
                                uint32 addr, uint32 curr_val,
                                uint32 prev_val,
                                int num_bytes);
    void __fastcall SetParameters (uint32 addr, uint32 curr_val,
                                   uint32 prev_val, int nbytes);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmCheatDetails *frmCheatDetails;
//---------------------------------------------------------------------------
#endif
