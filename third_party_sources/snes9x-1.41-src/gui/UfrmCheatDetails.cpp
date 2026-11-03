//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UfrmCheatDetails.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmCheatDetails *frmCheatDetails;
//---------------------------------------------------------------------------
void __fastcall TFrmCheatDetails::NewValueKeyPress(TObject *Sender,
      char &Key)
{
    if (Key == 10 || Key == 13)
        description_text->SetFocus ();
}
//---------------------------------------------------------------------------
void __fastcall TFrmCheatDetails::DescriptionEnter(TObject *Sender)
{
    ok_button->Default = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmCheatDetails::DescriptionExit(TObject *Sender)
{
    ok_button->Default = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmCheatDetails::DescriptionKeyPress(TObject *Sender,
      char &Key)
{
    if (Key == 10 || Key == 13)
        OK (Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFrmCheatDetails::NowVisible(TObject *Sender)
{
    new_value_text->SetFocus ();
}
//---------------------------------------------------------------------------


void __fastcall TFrmCheatDetails::OK(TObject *Sender)
{
    unsigned long d;
    if (sscanf (new_value_text->Text.c_str (), "%lu", &d) == 1)
    {
        for (int i = 0; i < num_bytes && data->num_cheats < MAX_CHEATS; i++)
        {
            data->c [data->num_cheats].address = address + i;
            data->c [data->num_cheats].byte = (uint8) (d >> (i * 8));
            data->c [data->num_cheats].saved_byte = 0;
            data->c [data->num_cheats].enabled = TRUE;
            data->c [data->num_cheats].saved = FALSE;
            data->c [data->num_cheats].saved_byte = 0;
            strncpy (data->c [data->num_cheats].name, description_text->Text.c_str (), 21);
            data->c [data->num_cheats++].name [21] = 0;
        }
        ModalResult = 1;
        Visible = false;
    }
}
//---------------------------------------------------------------------------

__fastcall TFrmCheatDetails::TFrmCheatDetails(TComponent* Owner,
                                              struct SCheatData *d,
                                              uint32 addr, uint32 curr_val,
                                              uint32 prev_val,
                                              int num_bytes)
    : TForm(Owner), data(d), address(addr), current_value(curr_val),
      previous_value(prev_val), num_bytes(num_bytes)
{
    char buffer [100];

    sprintf (buffer, "%06X", address);
    address_text->Text = buffer;
    sprintf (buffer, "%d", current_value);
    current_value_text->Text = buffer;
    sprintf (buffer, "%d", previous_value);
    previous_value_text->Text = buffer;
}

void __fastcall TFrmCheatDetails::SetParameters (uint32 addr, uint32 curr_val,
                                                 uint32 prev_val,
                                                 int nbytes)
{
    address = addr;
    current_value = curr_val;
    previous_value = prev_val;
    num_bytes = nbytes;

    char buffer [100];

    sprintf (buffer, "%06X", address);
    address_text->Text = buffer;
    sprintf (buffer, "%d", current_value);
    current_value_text->Text = buffer;
    sprintf (buffer, "%d", previous_value);
    previous_value_text->Text = buffer;
    new_value_text->Text = "";
    description_text->Text = "";
}
