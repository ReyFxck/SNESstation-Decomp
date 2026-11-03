//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UfrmOptions.h"
//---------------------------------------------------------------------------
#include <sys/types.h>
#include <sys/stat.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma link "CSPIN"
#pragma resource "*.dfm"
TfrmSettingsDialog *frmSettingsDialog;
//---------------------------------------------------------------------------
__fastcall TfrmSettingsDialog::TfrmSettingsDialog(TComponent* Owner, SSettings *s, sGUI *g)
    : TForm(Owner), settings(s), gui(g)
{
    init ();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSettingsDialog::init ()
{
    pathname_text->Text = gui->FreezeFileDir;
    auto_save_spin->Value = settings->AutoSaveDelay;
    turbo_frame_skip_spin->Value = settings->TurboSkipFrames;
    auto_frame_skip_spin->Value = settings->AutoMaxSkipFrames;
    cbx_ToggleTurboMode->Checked = gui->TurboModeToggle;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSettingsDialog::Browse(TObject *Sender)
{
    char buffer [_MAX_PATH + 1];
    open_dialog->InitialDir = pathname_text->Text;
    strncpy (buffer, pathname_text->Text.c_str (), _MAX_PATH);
    buffer [_MAX_PATH] = 0;
    strcat (buffer, "\\_.000");
    open_dialog->FileName = buffer;

    if (open_dialog->Execute ())
    {
        struct stat info;

        strcpy (buffer, open_dialog->FileName.c_str ());
        while (strlen (buffer) > 3 &&
               (_stat (buffer, &info) < 0 || !(info.st_mode & S_IFDIR)))
        {
            char *p = strrchr (buffer, '\\');
            if (p)
                *p = '\0';
            else
                break;
        }

        pathname_text->Text = buffer;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmSettingsDialog::OK(TObject *Sender)
{
    strncpy (gui->FreezeFileDir, pathname_text->Text.c_str (), _MAX_PATH);
    gui->FreezeFileDir [_MAX_PATH] = 0;
    settings->AutoSaveDelay =  auto_save_spin->Value;
    settings->TurboSkipFrames = turbo_frame_skip_spin->Value;
    settings->AutoMaxSkipFrames = auto_frame_skip_spin->Value;
    gui->TurboModeToggle = cbx_ToggleTurboMode->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSettingsDialog::NowVisible(TObject *Sender)
{
    pathname_text->SetFocus ();
}
//---------------------------------------------------------------------------
