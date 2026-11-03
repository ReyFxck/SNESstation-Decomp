//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UfrmNetPlayOptions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma link "CSPIN"
#pragma resource "*.dfm"
TfrmNetPlayOptions *frmNetPlayOptions;
//---------------------------------------------------------------------------
__fastcall TfrmNetPlayOptions::TfrmNetPlayOptions(TComponent* Owner,
                                                  struct SSettings *set,
                                                  struct SNetPlay *net,
                                                  struct SNPServer *srv)
    : TForm(Owner), settings(set), netplay(net), server(srv)
{
    init ();
}
//---------------------------------------------------------------------------
void __fastcall TfrmNetPlayOptions::init ()
{
    send_rom_check->Checked = server->SendROMImageOnConnect;
    sync_by_reset_check->Checked = server->SyncByReset;
    server_check->Checked = settings->NetPlayServer;
    max_frame_skip_spin->Value = netplay->MaxFrameSkip;
    when_to_pause_spin->Value = netplay->MaxBehindFrameCount;
    
    char buf [10];
    sprintf (buf, "%d", settings->Port);
    server_port_text->Text = buf;
}
//---------------------------------------------------------------------------
void __fastcall TfrmNetPlayOptions::OK(TObject *Sender)
{
    server->SendROMImageOnConnect = send_rom_check->Checked;
    server->SyncByReset = sync_by_reset_check->Checked;
    settings->NetPlayServer = server_check->Checked;
    netplay->MaxFrameSkip = max_frame_skip_spin->Value;
    netplay->MaxBehindFrameCount = when_to_pause_spin->Value;
    unsigned long _port;
    if (sscanf (server_port_text->Text.c_str (), "%lu", &_port) == 1)
    {
        settings->Port = (int) _port;
    }
}
//---------------------------------------------------------------------------

