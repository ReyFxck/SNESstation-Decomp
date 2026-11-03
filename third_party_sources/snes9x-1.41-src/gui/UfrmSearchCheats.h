//---------------------------------------------------------------------------
#ifndef UfrmSearchCheatsH
#define UfrmSearchCheatsH
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
class TfrmSearchCheats : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TButton *SpeedButton1;
    TButton *Button1;
    TButton *search_button;
    TButton *reset_button;
    TButton *add_cheat_button;
    TMaskEdit *exact_value_text;
    TLabel *exact_value_label;
    TListView *list;
    TRadioGroup *search_type_radio_box;
    TRadioGroup *data_size_radio_box;
    TRadioGroup *compare_to_radio_box;
    TRadioGroup *data_type_radio_box;
    void __fastcall DataSizeChanged(TObject *Sender);
    void __fastcall SearchTypeChange(TObject *Sender);
    void __fastcall AddCheat(TObject *Sender);
    void __fastcall Search(TObject *Sender);
    void __fastcall Reset(TObject *Sender);
    void __fastcall ListClicked(TObject *Sender);
    void __fastcall ListDoubleClicked(TObject *Sender);
    void __fastcall CompareToChange(TObject *Sender);
    void __fastcall DataTypeChange(TObject *Sender);
private:	// User declarations
    unsigned long exact_value;
    int num_bytes;
    struct SCheatData *data;
    S9xCheatDataSize data_size;
    S9xCheatComparisonType comparison_type;
    int data_type;
    int compare_to;
    void __fastcall UpdateList ();
    
public:		// User declarations
    boolean searched;
    __fastcall TfrmSearchCheats(TComponent* Owner, struct SCheatData *d);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSearchCheats *frmSearchCheats;
//---------------------------------------------------------------------------
#endif
