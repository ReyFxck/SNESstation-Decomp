//---------------------------------------------------------------------------
#ifndef UfrmApplyCheatsH
#define UfrmApplyCheatsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
#include "../snes9x/snes9x.h"
#include "../snes9x/cheats.h"
//---------------------------------------------------------------------------
class TfrmApplyCheats : public TForm
{
__published:	// IDE-managed Components
    TListView *lv_Cheats;
    TPanel *Panel1;
    TPanel *Panel2;
    TButton *SpeedButton1;
    TEdit *Edt_Description;
    TMaskEdit *mskEdt_Code;
    TLabel *Label1;
    TLabel *Label2;
    TButton *btnAdd;
    TButton *btnDelete;
    TButton *Button1;
    TButton *btnChange;
    TLabel *Label3;
    TLabel *Label4;
    TButton *btnClear;
    TMaskEdit *Edt_Address;
    TMaskEdit *Edt_Value;
    void __fastcall cbx_TypeChange(TObject *Sender);
    
    
    
    void __fastcall mskEdt_CodeChange(TObject *Sender);
    void __fastcall btnAddClick(TObject *Sender);
    
    void __fastcall lv_CheatsClick(TObject *Sender);
    void __fastcall btnDeleteClick(TObject *Sender);
    
    void __fastcall lv_CheatsDoubleClick(TObject *Sender);
    void __fastcall CheatCodeReturn(TObject *Sender, char &Key);
    void __fastcall DescriptionReturn(TObject *Sender, char &Key);
    void __fastcall NowVisible(TObject *Sender);
    void __fastcall Cancel(TObject *Sender);
    void __fastcall OK(TObject *Sender);
    
    void __fastcall AddressChange(TObject *Sender);
    void __fastcall ValueChange(TObject *Sender);
    void __fastcall btnChangeClick(TObject *Sender);
    void __fastcall btnClearClick(TObject *Sender);
    void __fastcall AddressReturn(TObject *Sender, char &Key);
    void __fastcall ValueReturn(TObject *Sender, char &Key);
    void __fastcall lv_CheatsChange(TObject *Sender, TListItem *Item,
          TItemChange Change);
private:	// User declarations
    struct SCheatData *data;
    struct SCheat cheats [MAX_CHEATS];
    int num_cheats;
    void __fastcall ReadCode();
    void __fastcall ReadAddress();
    void __fastcall ReadValue();
    void __fastcall Clear (bool set_focus = true);
    void __fastcall CheatSelected ();
    char * __fastcall StripSpaces (char *);
    bool valid_code;
    bool valid_address;
    bool valid_value;
    bool block;
    uint32 address;
    uint8 value;
public:		// User declarations
    __fastcall TfrmApplyCheats (TComponent* Owner, struct SCheatData *);
    void __fastcall init ();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmApplyCheats *frmApplyCheats;
//---------------------------------------------------------------------------
#endif
