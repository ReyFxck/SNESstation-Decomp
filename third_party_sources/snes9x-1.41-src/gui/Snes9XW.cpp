//---------------------------------------------------------------------------
#include <vcl.h>
#include <ddraw.h>
#include "..\wsnes9x.h"
#include "..\snes9x\snes9x.h"
#include "..\snes9x\port.h"
#include "..\snes9x\cheats.h"
#include "..\snes9x\netplay.h"

#pragma hdrstop
//---------------------------------------------------------------------------
//   Important note about DLL memory management when your DLL uses the
//   static version of the RunTime Library:
//
//   If your DLL exports any functions that pass String objects (or structs/
//   classes containing nested Strings) as parameter or function results,
//   you will need to add the library MEMMGR.LIB to both the DLL project and
//   any other projects that use the DLL.  You will also need to use MEMMGR.LIB
//   if any other projects which use the DLL will be perfomring new or delete
//   operations on any non-TObject-derived classes which are exported from the
//   DLL. Adding MEMMGR.LIB to your project will change the DLL and its calling
//   EXE's to use the BORLNDMM.DLL as their memory manager.  In these cases,
//   the file BORLNDMM.DLL should be deployed along with your DLL.
//
//   To avoid using BORLNDMM.DLL, pass string information using "char *" or
//   ShortString parameters.
//
//   If your DLL uses the dynamic version of the RTL, you do not need to
//   explicitly add MEMMGR.LIB as this will be done implicitly for you
//---------------------------------------------------------------------------
USEFORM("UfrmOpenROM.cpp", frmOpenROM);
USEFORM("UfrmOptions.cpp", frmSettingsDialog);
USEFORM("UfrmJoypad.cpp", frmJoypad);
USEFORM("UfrmDirectX.cpp", frmDirectX);
USEFORM("UfrmTestDisplayMode.cpp", frmTestDisplayMode);
USERES("Snes9XW.res");
USEFORM("UfrmApplyCheats.cpp", frmApplyCheats);
USEUNIT("..\SNES9X\Cheats.cpp");
USEFORM("UfrmSearchCheats.cpp", frmSearchCheats);
USEFORM("UfrmCheatDetails.cpp", frmCheatDetails);
USEFORM("UfrmAboutDialog.cpp", frmAboutDialog);
USEFORM("UfrmNetPlayOptions.cpp", frmNetPlayOptions);
USEFORM("UfrmNetPlayConnect.cpp", frmNetPlayConnect);
USEFORM("UfrmNetPlayConnect2.cpp", frmNetPlayConnect2);
USEFORM("UfrmProgress.cpp", frmProgress);
USEFORM("Snes9x.cpp", frmSnes9X);
USEFORM("UfrmSoundOptions.cpp", frmSoundOptions);
//---------------------------------------------------------------------------
extern "C" void __export __cdecl InitDLL( HANDLE hWnd)
{
    Application->Handle = hWnd;
}
#include "UfrmOpenROM.h"
#include "..\\snes9x\\snes9x.h"
extern "C" bool __export __cdecl OpenROM (char *FileName, struct SSettings *Settings)
{
    bool Result = false;
    if (!frmOpenROM)
        frmOpenROM = new TfrmOpenROM (Application);

    frmOpenROM -> cbx_MemMap -> ItemIndex = 0;
    frmOpenROM -> cbx_Interleave -> ItemIndex = 0;
    frmOpenROM -> cbx_System -> ItemIndex = 0;
    frmOpenROM -> cbx_Header -> ItemIndex = 0;

    if (frmOpenROM -> ShowModal() == 1)
    {
        System::String FN;

        Settings -> ForceLoROM = false;
        Settings -> ForceHiROM = false;
        switch( frmOpenROM -> cbx_MemMap -> ItemIndex)
        {
            case 1: Settings -> ForceLoROM = true; break;
            case 2: Settings -> ForceHiROM = true; break;
        }

        Settings -> ForceNotInterleaved = false;
        Settings -> ForceInterleaved = false;
        Settings -> ForceInterleaved2 = false;
        switch( frmOpenROM -> cbx_Interleave -> ItemIndex)
        {
            case 1: Settings -> ForceNotInterleaved = true; break;
            case 2: Settings -> ForceInterleaved = true; break;
            case 3: Settings -> ForceInterleaved2 = true; break;
        }

        Settings -> ForcePAL = false;
        Settings -> ForceNTSC = false;
        switch( frmOpenROM -> cbx_System -> ItemIndex)
        {
            case 1: Settings -> ForcePAL = true; break;
            case 2: Settings -> ForceNTSC = true; break;
        }

        Settings -> ForceHeader = false;
        Settings -> ForceNoHeader = false;
        switch (frmOpenROM -> cbx_System -> ItemIndex)
        {
            case 1: Settings -> ForceNoHeader = true; break;
            case 2: Settings -> ForceHeader = true; break;
        }

        FN = "";
        if (frmOpenROM->DirectoryTreeView->Selected != NULL &&
            frmOpenROM->ListView->Selected != NULL)
        {
            FN = ((char *) (int(frmOpenROM->DirectoryTreeView->Selected->Data) + 1)) + frmOpenROM->ListView->Selected->Caption;
        }

        StrPCopy (FileName, FN);
        Result = true;
    }
    return Result;
}
//---------------------------------------------------------------------------
#include "UfrmJoypad.h"
extern struct SJoypad Joypad[5];
extern "C" void __export __cdecl JoypadConfig( struct SJoypad *J)
{
    if (!frmJoypad)
        frmJoypad = new TfrmJoypad (Application);
    else
        frmJoypad->init ();

    Joypad[0] = J[0];
    Joypad[1] = J[1];
    Joypad[2] = J[2];
    Joypad[3] = J[3];
    Joypad[4] = J[4];
    if (frmJoypad -> ShowModal() == 1)
    {
        J[0] = Joypad[0];
        J[1] = Joypad[1];
        J[2] = Joypad[2];
        J[3] = Joypad[3];
        J[4] = Joypad[4];
    }
}
//---------------------------------------------------------------------------
#include "UfrmDirectX.h"
extern LPDIRECTDRAW lpDD;

extern "C" void __export __cdecl DirectXConfig( LPDIRECTDRAW plpDD, struct SSettings *Settings, struct sGUI *GUI, bool *showFPS)
{
    if (!frmDirectX)
        frmDirectX = new TfrmDirectX (Application);

    lpDD = plpDD;
    frmDirectX -> cbx_Sixteenbit -> Checked = Settings -> SixteenBit;
    frmDirectX -> cbx_Transparency -> Checked = Settings -> Transparency;
    frmDirectX -> cbx_HiRES -> Checked = Settings -> SupportHiRes;
    frmDirectX -> cbx_Mode7 -> Checked = Settings -> Mode7Interpolate;
    frmDirectX -> cbx_AutoFramerate -> Checked = Settings -> SkipFrames == 200;
    frmDirectX -> cbx_LimitFrameRate -> Checked = Settings -> AutoMaxSkipFrames == 0;
    frmDirectX -> cbx_LimitFrameRate -> Enabled = frmDirectX -> cbx_AutoFramerate->Checked;

    frmDirectX -> tb_SkipFrames -> Position = Settings -> SkipFrames < 200 ? Settings -> SkipFrames : 0;
    frmDirectX -> tb_SkipFrames -> Enabled = !frmDirectX -> cbx_AutoFramerate->Checked;

    frmDirectX -> dWidth = GUI -> Width;
    frmDirectX -> dHeight = GUI -> Height;
    frmDirectX -> dDepth = GUI -> Depth;
    frmDirectX -> cbx_Stretch -> ItemIndex = GUI -> NextScale;
    frmDirectX -> cbx_ShowFPS -> Checked = *showFPS;
    frmDirectX -> cbx_DoubleBuffer -> Checked = GUI -> DoubleBuffered;
    frmDirectX -> cbx_StretchBox -> Checked = GUI -> Stretch;
    frmDirectX -> cbx_FullScreen -> Checked = GUI -> FullScreen;

    if( frmDirectX -> ShowModal() == 1)
    {
        Settings -> SixteenBit = frmDirectX -> cbx_Sixteenbit -> Checked;
        Settings -> Transparency = frmDirectX -> cbx_Transparency -> Checked;
        Settings -> SupportHiRes = frmDirectX -> cbx_HiRES -> Checked;
        Settings -> Mode7Interpolate = frmDirectX -> cbx_Mode7 -> Checked;
        Settings -> SkipFrames = frmDirectX -> cbx_AutoFramerate -> Checked ? 200 : frmDirectX -> tb_SkipFrames -> Position;
        if (frmDirectX->cbx_LimitFrameRate->Checked &&
            frmDirectX->cbx_LimitFrameRate->Enabled)
        {
            Settings->AutoMaxSkipFrames = 0;
        }
        else
            if (Settings->AutoMaxSkipFrames == 0)
                Settings->AutoMaxSkipFrames = 10;

        GUI -> Width = frmDirectX -> dWidth;
        GUI -> Height = frmDirectX -> dHeight;
        GUI -> Depth = frmDirectX -> dDepth;
        GUI -> NextScale = frmDirectX -> cbx_Stretch -> ItemIndex;
        GUI -> DoubleBuffered = frmDirectX -> cbx_DoubleBuffer -> Checked;
        *showFPS = frmDirectX -> cbx_ShowFPS -> Checked;
        GUI -> FullScreen = frmDirectX -> cbx_FullScreen -> Checked;
        GUI -> Stretch = frmDirectX -> cbx_StretchBox -> Checked;
    }
}
//---------------------------------------------------------------------------
#include "UfrmApplyCheats.h"
extern "C" void __export __cdecl CheatCodeEntry ( struct SCheatData *data)
{
    if (!frmApplyCheats)
        frmApplyCheats = new TfrmApplyCheats (Application, data);
    else
        frmApplyCheats->init ();
    frmApplyCheats->ShowModal();
}
//---------------------------------------------------------------------------
#include "UfrmSearchCheats.h"
extern "C" void __export __cdecl SearchForCheats (struct SCheatData *data)
{
    if (!frmSearchCheats)
        frmSearchCheats = new TfrmSearchCheats (Application, data);
    else
    {
        frmSearchCheats->search_button->Enabled = true;
        frmSearchCheats->searched = false;
    }

    frmSearchCheats->ShowModal();
    frmSearchCheats->Visible = false;
}
//---------------------------------------------------------------------------
#include "UfrmNetPlayConnect2.h"
#include "UfrmNetPlayConnect.h"
extern "C" bool __export __cdecl NetPlayConnect (char *hostname, int *port)
{
    if (!frmNetPlayConnect2)
        frmNetPlayConnect2 = new TfrmNetPlayConnect2 (Application, *port);
    else
        frmNetPlayConnect2->init (*port);

    if (frmNetPlayConnect2->ShowModal() == 1)
    {
	StrPCopy (hostname, frmNetPlayConnect->value.c_str ());
	*port = frmNetPlayConnect2->port;
        return (true);
    }

    return (false);
}
//---------------------------------------------------------------------------
#include "UfrmNetPlayOptions.h"
extern "C" bool __export __cdecl NetPlayOptions (struct SSettings *settings,
                                                 struct SNetPlay *netplay,
                                                 struct SNPServer *server)
{
    if (!frmNetPlayOptions)
        frmNetPlayOptions = new TfrmNetPlayOptions (Application, settings,
                                                    netplay, server);
    else
        frmNetPlayOptions->init ();

    return (frmNetPlayOptions->ShowModal() == 1);
}
//---------------------------------------------------------------------------
#include "UfrmAboutDialog.h"
extern "C" void __export __cdecl AboutDialog (const char *version)
{
    if (!frmAboutDialog)
    {
        frmAboutDialog = new TfrmAboutDialog (Application);

        char buf [100];
        
        sprintf (buf, "Snes9X v%s for Windows 95/98/Me/NT/2000.", version);
        frmAboutDialog->memo->Lines->Append (buf);
        frmAboutDialog->memo->Lines->Append ("Copyright (c) 1996-2001 by Gary Henderson and Jerremy Koot.");
        frmAboutDialog->memo->Lines->Append ("");
        frmAboutDialog->memo->Lines->Append ("Snes9X is a Super Nintendo Entertainment System");
        frmAboutDialog->memo->Lines->Append ("emulator that allows you to play most games designed");
        frmAboutDialog->memo->Lines->Append ("for the SNES on your PC.");
        frmAboutDialog->memo->Lines->Append ("");
        frmAboutDialog->memo->Lines->Append ("Please visit http://www.snes9x.com for");
        frmAboutDialog->memo->Lines->Append ("up-to-the-minute information and help on Snes9X."); 
        frmAboutDialog->memo->Lines->Append ("");
        frmAboutDialog->memo->Lines->Append ("Nintendo is a trade mark.");
    }

    frmAboutDialog->ShowModal();
}
//---------------------------------------------------------------------------
#include "UfrmOptions.h"
extern "C" void __export __cdecl SettingsDialog (SSettings *s, sGUI *g)
{
    if (!frmSettingsDialog)
        frmSettingsDialog = new TfrmSettingsDialog (Application, s, g);
    else
        frmSettingsDialog->init ();
    frmSettingsDialog->ShowModal();
}
//---------------------------------------------------------------------------
#include "UfrmSoundOptions.h"
extern "C" bool __export __cdecl SoundOptions (struct SSettings *settings)
{
    if (!frmSoundOptions)
        frmSoundOptions = new TfrmSoundOptions (Application, settings);
    else
        frmSoundOptions->init ();

    return (frmSoundOptions->ShowModal() == 1);
}
//---------------------------------------------------------------------------
#include "UfrmProgress.h"
extern "C" void __export __cdecl ProgressDialog (const char *message, 
                                                 int progress)
{
    if (!frmProgress)
        frmProgress = new TfrmProgress (Application);
    if (progress >= 0)
    {
        frmProgress->progress->Progress = progress;
        frmProgress->label->Caption = message;
        frmProgress->Show ();
    }
    else
        frmProgress->Hide ();
}
//---------------------------------------------------------------------------
#include "Snes9X.h"
extern "C" HWND __export __cdecl MainWindow ()
{
    if (!frmSnes9X)
    {
        frmSnes9X = new TfrmSnes9X (Application);
        frmSnes9X->Show ();
    }
    return (frmSnes9X->Handle);
}
//---------------------------------------------------------------------------
extern "C" int __export __cdecl GUIVersion ()
{
    return (GUI_VERSION);
}

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
    return 1;
}
//---------------------------------------------------------------------------
