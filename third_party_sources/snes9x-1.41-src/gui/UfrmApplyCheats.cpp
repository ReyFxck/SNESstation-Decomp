//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UfrmApplyCheats.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmApplyCheats *frmApplyCheats;
//---------------------------------------------------------------------------


__fastcall TfrmApplyCheats::TfrmApplyCheats(TComponent* Owner,
                                            struct SCheatData *d)
    : TForm(Owner), data(d)
{
    init ();
}
//---------------------------------------------------------------------------
void __fastcall TfrmApplyCheats::init ()
{
    // Make a copy of the current cheats - so that cancelling the dialog can 
    // undo any edits.
    memmove ((void *) cheats, (void *) data->c, sizeof (cheats));
    num_cheats = data->num_cheats;

    lv_Cheats->Items->BeginUpdate ();

    // Copy all the cheat data into the list view component.
    for (int i = 0; i < num_cheats; i++)
    {
        TListItem *item = lv_Cheats->Items->Add ();
        bool8 enable = cheats [i].enabled;

        char buffer [100];
        sprintf (buffer, "%06X", cheats [i].address);
        item->SubItems->Add (buffer);
        sprintf (buffer, "%02X", cheats [i].byte);
        item->SubItems->Add (buffer);
        item->SubItems->Add (cheats [i].name);
        item->Checked = enable;
    }
    lv_Cheats->Items->EndUpdate ();
    valid_code = false;
    valid_address = false;
    valid_value = false;
    block = false;
    Clear (false);
}
//---------------------------------------------------------------------------
void __fastcall TfrmApplyCheats::cbx_TypeChange(TObject *Sender)
{
    mskEdt_CodeChange (Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmApplyCheats::mskEdt_CodeChange(TObject *Sender)
{
    if (!block)
    {
        bool cleared = false;
        ReadCode ();

        if (valid_code || (valid_address && valid_value))
        {
            btnAdd->Enabled = true;
            btnChange->Enabled = (lv_Cheats->Selected != NULL);
            if (valid_code)
            {
                char buffer [100];

                block = true;
                cleared = true;
                sprintf (buffer, "%06X", address);
                Edt_Address->Text = buffer;
                valid_address = true;
                sprintf (buffer, "%u", value);
                Edt_Value->Text = buffer;
                valid_value = true;
                block = false;
            }
        }
        else
        {
            btnAdd->Enabled = false;
            btnChange->Enabled = false;
        }
        if (!cleared)
        {
            Edt_Address->Text = "";
            Edt_Value->Text = "";
            valid_address = false;
            valid_value = false;
        }
    }
}

char * __fastcall TfrmApplyCheats::StripSpaces (char *text)
{
    while (*text == ' ')
        text++;
    while (strlen (text) && *(text + strlen (text) - 1) == ' ')
        *(text + strlen (text) - 1) = 0;

    return (text);
}

void __fastcall TfrmApplyCheats::ReadCode ()
{
    char txt [30];
    char *text = txt;
    uint32 addr;
    uint8 v;

    memset (text, 0, sizeof (txt));
    mskEdt_Code->GetTextBuf (text, sizeof (txt) - 1);
    
    valid_code = false;

    text = StripSpaces (text);
    if (S9xGameGenieToRaw (text, addr, v) == NULL ||
        S9xProActionReplayToRaw (text, addr, v) == NULL)
    {
        valid_code = true;
        valid_address = true;
        valid_value = true;
        address = addr;
        value = v;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmApplyCheats::btnAddClick(TObject *Sender)
{
    if (valid_address && valid_value)
    {
        if (num_cheats < MAX_CHEATS)
        {
            TListItem *ListItem = lv_Cheats->Items->Add ();
            char buffer [100];
            sprintf (buffer, "%06X", address);
            ListItem->SubItems->Add (buffer);
            sprintf (buffer, "%02X", value);
            ListItem->SubItems->Add (buffer);
            memset (buffer, 0, sizeof (buffer));
            Edt_Description->GetTextBuf (buffer, 21);
            buffer [21] = 0;
            ListItem->SubItems->Add (buffer);
            ListItem->Checked = true;
            //lv_Cheats->Selected = ListItem;
            cheats [num_cheats].address = address;
            cheats [num_cheats].byte = value;
            cheats [num_cheats].saved_byte = 0;
            cheats [num_cheats].enabled = TRUE;
            cheats [num_cheats].saved = FALSE;
            strcpy (cheats [num_cheats].name, buffer);
            num_cheats++;

            Clear ();
        }
    }
}

//---------------------------------------------------------------------------

void __fastcall TfrmApplyCheats::lv_CheatsClick(TObject *Sender)
{
    CheatSelected ();
    for (int i = 0; i < num_cheats; i++)
        cheats [i].enabled = lv_Cheats->Items->Item [i]->Checked;
}

void __fastcall TfrmApplyCheats::CheatSelected ()
{
    btnDelete->Enabled = (lv_Cheats->Selected != NULL);

    if (lv_Cheats->Selected)
    {
        int ind = lv_Cheats->Items->IndexOf (lv_Cheats->Selected);

        btnChange->Enabled = false;
        btnAdd->Enabled = false;

        block = true;
        Edt_Description->Text = cheats [ind].name;

        char buffer [100];
        sprintf (buffer, "%06X%02X", address = cheats [ind].address,
                 value = cheats [ind].byte);
        mskEdt_Code->Text = buffer;
        sprintf (buffer, "%06X", address);
        Edt_Address->Text = buffer;
        sprintf (buffer, "%d", value);
        Edt_Value->Text = buffer;
        block = false;

        valid_code = true;
        valid_address = true;
        valid_value = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmApplyCheats::btnDeleteClick(TObject *Sender)
{
    if (lv_Cheats->Selected)
    {
        int ind = lv_Cheats->Items->IndexOf (lv_Cheats->Selected);

        lv_Cheats->Items->Delete (ind);
        memmove ((void *) &cheats [ind], (void *) &cheats [ind + 1],
                 sizeof (cheats [0]) * (num_cheats - ind - 1));
        num_cheats--;
        Clear ();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmApplyCheats::lv_CheatsDoubleClick(TObject *Sender)
{
    if (lv_Cheats->Selected)
    {
        int ind = lv_Cheats->Items->IndexOf (lv_Cheats->Selected);
        lv_Cheats->Selected->Checked = !lv_Cheats->Selected->Checked;
        cheats [ind].enabled = lv_Cheats->Selected->Checked;
        CheatSelected ();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmApplyCheats::CheatCodeReturn(TObject *Sender,
      char &Key)
{
    if (Key == 10 || Key == 13)
        Edt_Description->SetFocus ();
}
//---------------------------------------------------------------------------

void __fastcall TfrmApplyCheats::DescriptionReturn(TObject *Sender,
      char &Key)
{
    if (Key == 10 || Key == 13)
    {
        if (valid_code || (valid_address && valid_value))
            btnAddClick (Sender);
        else
            Edt_Address->SetFocus ();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmApplyCheats::NowVisible(TObject *Sender)
{
    mskEdt_Code->SetFocus ();
}
//---------------------------------------------------------------------------

void __fastcall TfrmApplyCheats::Cancel(TObject *Sender)
{
    ModalResult = 2;
}
//---------------------------------------------------------------------------

void __fastcall TfrmApplyCheats::OK(TObject *Sender)
{
    ModalResult = 1;
    memmove ((void *) data->c, (void *) cheats, sizeof (cheats));
    data->num_cheats = num_cheats;
}
//---------------------------------------------------------------------------
void __fastcall TfrmApplyCheats::AddressChange(TObject *Sender)
{
    if (!block)
    {
        ReadAddress ();
        
        if (valid_address)
        {
            block = true;
            mskEdt_Code->Text = "";
            block = false;
            valid_code = false;
        }
        
        if ((valid_address && valid_value) || valid_code)
        {
            btnAdd->Enabled = true;
            btnChange->Enabled = (lv_Cheats->Selected != NULL);
        }
        else
        {
            btnAdd->Enabled = false;
            btnChange->Enabled = false;
        }
    }
}

void __fastcall TfrmApplyCheats::ReadAddress ()
{
    char txt [10];
    char *text = txt;
    uint32 addr;
    bool hex = true;

    memset (text, 0, sizeof (txt));
    Edt_Address->GetTextBuf (text, sizeof (txt) - 1);
    
    valid_address = false;
    text = StripSpaces (text);
    if (*text == '$')
        text++;
    else
    if (strlen (text) > 2 && *text == '0' && 
        (*(text + 1) == 'x' || *(text + 1) == 'X'))
        text += 2;
    else
    if (*(text + strlen (text) - 1) == 'd' ||
        *(text + strlen (text) - 1) == 'D')
        hex = false;
    
    if ((hex && sscanf (text, "%x", &addr) == 1) ||
        (!hex && sscanf (text, "%lu", & addr) == 1))
    {
        valid_address = true;
        address = addr;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmApplyCheats::ValueChange(TObject *Sender)
{
    if (!block)
    {
        ReadValue ();
        if (valid_value)
        {
            block = true;
            mskEdt_Code->Text = "";
            valid_code = false;
            block = false;
        }
        if (valid_code || (valid_address && valid_value))
        {
            btnAdd->Enabled = true;
            btnChange->Enabled = (lv_Cheats->Selected != NULL);
        }
        else
        {
            btnAdd->Enabled = false;
            btnChange->Enabled = false;
        }
    }
}

void __fastcall TfrmApplyCheats::ReadValue ()
{
    char txt [10];
    char *text = txt;
    uint32 v;

    memset (text, 0, sizeof (txt));
    Edt_Value->GetTextBuf (text, sizeof (txt) - 1);
    valid_value = false;
    
    bool hex = false;
    text = StripSpaces (text);
    
    if (*text == '$')
    {
        text++;
        hex = true;
    }
    else
    if (strlen (text) > 2 && *text == '0' && 
        (*(text + 1) == 'x' || *(text + 1) == 'X'))
    {
        text += 2;
        hex = true;
    }
    else
    if (*(text + strlen (text) - 1) == 'h' || 
        *(text + strlen (text) - 1) == 'H')
    {
        hex = true;
    }
    
    if ((hex && sscanf (text, "%x", &v) == 1) ||
        (!hex && sscanf (text, "%lu", &v) == 1))
    {
        valid_value = true;
        value = (uint8) v;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmApplyCheats::btnChangeClick(TObject *Sender)
{
    if (valid_address && valid_value && lv_Cheats->Selected)
    {
        char buffer [100];
        TListItem *ListItem = lv_Cheats->Selected;
        int ind = lv_Cheats->Items->IndexOf (ListItem);

        sprintf (buffer, "%06X", address);
        ListItem->SubItems->Strings [0] = buffer;
        sprintf (buffer, "%02X", value);
        ListItem->SubItems->Strings [1] = buffer;
        memset (buffer, 0, sizeof (buffer));
        Edt_Description->GetTextBuf (buffer, 21);
        ListItem->SubItems->Strings [2] = buffer;
        ListItem->Checked = true;
        lv_Cheats->Selected = ListItem;
        cheats [ind].address = address;
        cheats [ind].byte = value;
        cheats [ind].saved_byte = 0;
        cheats [ind].enabled = TRUE;
        cheats [ind].saved = FALSE;
        strcpy (cheats [ind].name, buffer);

        CheatSelected ();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmApplyCheats::btnClearClick(TObject *Sender)
{
    Clear ();
}

void __fastcall TfrmApplyCheats::Clear (bool set_focus)
{
    lv_Cheats->Selected = NULL;

    block = true;
    Edt_Description->Text = "";
    mskEdt_Code->Text = "";
    Edt_Address->Text = "";
    Edt_Value->Text = "";
    block = false;
    btnAdd->Enabled = false;
    btnDelete->Enabled = false;
    btnChange->Enabled = false;
    if (set_focus)
        mskEdt_Code->SetFocus ();
    valid_code = false;
    valid_address = false;
    valid_value = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmApplyCheats::AddressReturn(TObject *Sender, char &Key)
{
    if (Key == 10 || Key == 13)
        Edt_Value->SetFocus ();
}
//---------------------------------------------------------------------------

void __fastcall TfrmApplyCheats::ValueReturn(TObject *Sender, char &Key)
{
    if (Key == 10 || Key == 13)
    {
        if (valid_code || (valid_address && valid_value))
            btnAddClick (Sender);
        else
            mskEdt_Code->SetFocus ();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmApplyCheats::lv_CheatsChange(TObject *Sender,
      TListItem *Item, TItemChange Change)
{
   //
}
//---------------------------------------------------------------------------

