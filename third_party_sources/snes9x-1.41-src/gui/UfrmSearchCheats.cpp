//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UfrmSearchCheats.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSearchCheats *frmSearchCheats;
//---------------------------------------------------------------------------
#include "UfrmCheatDetails.h"
#include "memmap.h"
//---------------------------------------------------------------------------
__fastcall TfrmSearchCheats::TfrmSearchCheats(TComponent* Owner,
                                              struct SCheatData *d)
    : TForm(Owner), data(d)
{
    Reset (this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearchCheats::DataSizeChanged(TObject *Sender)
{
    if ((int) data_size_radio_box->ItemIndex != (int) data_size)
    {
        searched = false;
        search_button->Enabled = true;
        num_bytes = (int) data_size_radio_box->ItemIndex + 1;
        data_size = (S9xCheatDataSize) data_size_radio_box->ItemIndex;
        UpdateList ();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearchCheats::SearchTypeChange(TObject *Sender)
{
    comparison_type = (S9xCheatComparisonType) search_type_radio_box->ItemIndex;
}

//---------------------------------------------------------------------------

void __fastcall TfrmSearchCheats::AddCheat(TObject *Sender)
{
    if (list->Selected)
    {
        uint32 address;
        uint32 curr_val;
        uint32 prev_val;
        char *format = "%lu";

        if (data_type == 1)
            format = "%ld";
        else
        if (data_type == 2)
            format = "%lx";

        if (sscanf (list->Selected->Caption.c_str (), "%x", &address) == 1 &&
            sscanf (list->Selected->SubItems->Strings [0].c_str (), format,
                    &curr_val) == 1 &&
            sscanf (list->Selected->SubItems->Strings [1].c_str (), format, 
                    &prev_val) == 1)
        {
            if (!frmCheatDetails)
                frmCheatDetails = new TFrmCheatDetails (this, data, address,
                                                        curr_val, prev_val,
                                                        num_bytes);
            else
                frmCheatDetails->SetParameters (address, curr_val, prev_val,
                                                num_bytes);

            frmCheatDetails->ShowModal ();
            frmCheatDetails->Visible = false;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmSearchCheats::Search(TObject *Sender)
{
    if (compare_to == 1)
    {
        char *format = "%lu";

        if (data_type == 1)
            format = "%ld";
        else
        if (data_type == 2)
            format = "%lx";

        if (sscanf (exact_value_text->Text.c_str (), format, &exact_value) != 1)
        {
            MessageDlg ("Please enter a value to compare against.", 
                        mtError, TMsgDlgButtons() << mbOK, 0);
            return;
        }
        S9xSearchForValue (data, comparison_type, data_size, exact_value,
                           data_type == 1, FALSE);
        UpdateList ();
        S9xSearchForValue (data, comparison_type, data_size, exact_value,
                           data_type == 1, TRUE);
    }
    else
    {
        S9xSearchForChange (data, comparison_type, data_size, 
                            data_type == 1, FALSE);
        UpdateList ();
        S9xSearchForChange (data, comparison_type, data_size,
                            data_type == 1, TRUE);
        search_button->Enabled = false;
        searched = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearchCheats::Reset(TObject *Sender)
{
    data_size = S9X_8_BITS;
    comparison_type = S9X_LESS_THAN;
    compare_to = 0;
    data_type = 0;
    search_type_radio_box->ItemIndex = 0;
    data_size_radio_box->ItemIndex = 0;
    compare_to_radio_box->ItemIndex = 0;
    data_type_radio_box->ItemIndex = 0;

    num_bytes = 1;
    exact_value_text->Text = "";
    exact_value_text->Enabled = false;
    S9xStartCheatSearch (data);
    search_button->Enabled = false;
    searched = true;
    add_cheat_button->Enabled = false;
    UpdateList ();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSearchCheats::ListClicked(TObject *Sender)
{
    add_cheat_button->Enabled = list->Selected != NULL;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearchCheats::ListDoubleClicked(TObject *Sender)
{
    if (list->Selected)
        AddCheat (Sender);
}
//---------------------------------------------------------------------------
#define TEST_BIT(a,v) \
((a)[(v) >> 5] & (1 << ((v) & 31)))

#define _D(s,m,o) \
((s) == S9X_8_BITS ? *((m) + (o)) : \
 (s) == S9X_16_BITS ? (*((m) + (o)) + (*((m) + (o) + 1) << 8)) : \
 (s) == S9X_24_BITS ? (*((m) + (o)) + (*((m) + (o) + 1) << 8) + (*((m) + (o) + 2) << 16)) : \
 (*((m) + (o)) + (*((m) + (o) + 1) << 8) + (*((m) + (o) + 2) << 16) + (*((m) + (o) + 3) << 24)))

#define _DS(s,m,o) \
((s) == S9X_8_BITS ? ((int8) *((m) + (o))) : \
 (s) == S9X_16_BITS ? ((int16) (*((m) + (o)) + (*((m) + (o) + 1) << 8))) : \
 (s) == S9X_24_BITS ? (((int32) ((*((m) + (o)) + (*((m) + (o) + 1) << 8) + (*((m) + (o) + 2) << 16)) << 8)) >> 8): \
 ((int32) (*((m) + (o)) + (*((m) + (o) + 1) << 8) + (*((m) + (o) + 2) << 16) + (*((m) + (o) + 3) << 24))))

void __fastcall TfrmSearchCheats::UpdateList ()
{
    list->Items->BeginUpdate ();
    list->Items->Clear ();

    int count = 0;
    for (int i = 0; i < 0x20000 - num_bytes && count < 400; i++)
    {
        if (TEST_BIT (data->WRAM_BITS, i))
        {
            TListItem *item = list->Items->Add ();
            char buffer [100];
            char format [10];
            sprintf (buffer, "%06X", i + 0x7E0000);
            item->Caption = buffer;
            if (data_type == 0)
                sprintf (buffer, "%lu", _D(data_size, data->RAM, i));
            else
            if (data_type == 1)
                sprintf (buffer, "%ld", _DS(data_size, data->RAM, i));
            else
            {
                sprintf (format, "%%0%dX", num_bytes * 2);
                sprintf (buffer, format, _D(data_size, data->RAM, i));
            }
            item->SubItems->Add (buffer);
            if (data_type == 0)
                sprintf (buffer, "%lu", _D(data_size, data->CWRAM, i));
            else
            if (data_type == 1)
                sprintf (buffer, "%ld", _DS(data_size, data->CWRAM, i));
            else
            {
                sprintf (format, "%%0%dX", num_bytes * 2);
                sprintf (buffer, format, _D(data_size, data->CWRAM, i));
            }
            item->SubItems->Add (buffer);
            count++;
        }
    }
    list->Items->EndUpdate ();
}

//---------------------------------------------------------------------------
void __fastcall TfrmSearchCheats::CompareToChange(TObject *Sender)
{
    compare_to = compare_to_radio_box->ItemIndex;

    if (compare_to == 0)
    {
        exact_value_text->Enabled = false;
        exact_value_label->Enabled = false;
        search_button->Enabled = !searched;
    }
    else
    {
        exact_value_text->Enabled = true;
        exact_value_label->Enabled = true;
        search_button->Enabled = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearchCheats::DataTypeChange(TObject *Sender)
{
    int old_data_type = data_type;
    
    data_type = data_type_radio_box->ItemIndex;

    if (data_type != old_data_type)
    {
        search_button->Enabled = true;
        UpdateList ();
    }
}
//---------------------------------------------------------------------------
