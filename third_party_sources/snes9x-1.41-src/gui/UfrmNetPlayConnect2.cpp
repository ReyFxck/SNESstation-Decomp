//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UfrmNetPlayConnect2.h"
//---------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmNetPlayConnect2 *frmNetPlayConnect2;
//---------------------------------------------------------------------------
__fastcall TfrmNetPlayConnect2::~TfrmNetPlayConnect2()
{
    delete reg;
}
//---------------------------------------------------------------------------
__fastcall TfrmNetPlayConnect2::TfrmNetPlayConnect2(TComponent* Owner,
                                                    int _port)
    : TForm(Owner)
{
    init (_port);
}
//---------------------------------------------------------------------------
void __fastcall TfrmNetPlayConnect2::init (int _port)
{
    reg = new TRegistry();
    reg->OpenKey ("Software\\Emulators\\Snes9X\\1.x\\NetPlayServerHistory", true);
    history_combo->Items->Clear ();
    hist_size = 0;

#if 0    
    char buf [10];

    for (int i = 0; i < NETPLAY_HISTORY_SIZE; i++)
    {
        sprintf (buf, "%d", i);
        if (reg->ValueExists (buf))
        {
            history [hist_size++] = reg->ReadString (buf);
            history_combo->Items->Add (reg->ReadString (buf));
        }
    }

    port = _port;
    sprintf (buf, "%d", port);
    server_port_text->Text = buf;

    if (hist_size)
        history_combo->ItemIndex = 0;
#endif
}
//---------------------------------------------------------------------------
void __fastcall TfrmNetPlayConnect2::OK(TObject *Sender)
{
    unsigned long _port;
    if (sscanf (server_port_text->Text.c_str (), "%lu", &_port) == 1)
    {
        port = (int) _port;
    }

    value = history_combo->Text;
    int i;

    for (i = 0; i < hist_size; i++)
    {
        if (value == history [i])
        {
            // Value is already in list, bring to front to list...
            for (int j = i; j > 0; j--)
                history [j] = history [j - 1];

            history [0] = value;
            break;
        }
    }

    if (i >= hist_size)
    {
        // Value not in list, add it to front, drop oldest item off end of list
        // if list is full...
        if (hist_size < NETPLAY_HISTORY_SIZE)
            hist_size++;

        for (int j = hist_size - 1; j > 0; j--)
            history [j] = history [j - 1];

        history [0] = value;
    }

    for (i = 0; i < hist_size; i++)
    {
        char buf [10];

        sprintf (buf, "%d", i);
        reg->WriteString (buf, history [i]);
    }
    ModalResult = 1;
    Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmNetPlayConnect2::ClearHistory(TObject *Sender)
{
    char buf [10];

    for (int i = 0; i < NETPLAY_HISTORY_SIZE; i++)
    {
        sprintf (buf, "%d", i);
        reg->DeleteValue (buf);
    }
    history_combo->Items->Clear ();
    history_combo->Text = "";
    hist_size = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmNetPlayConnect2::NowVisible(TObject *Sender)
{
  //    history_combo->SetFocus ();    
}
//---------------------------------------------------------------------------

void __fastcall TfrmNetPlayConnect2::HostnameEnter(TObject *Sender)
{
  //ok_button->Default = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmNetPlayConnect2::HostnameExit(TObject *Sender)
{
  //ok_button->Default = false;    
}
//---------------------------------------------------------------------------
