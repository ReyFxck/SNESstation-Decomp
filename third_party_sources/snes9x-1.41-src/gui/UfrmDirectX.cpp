//---------------------------------------------------------------------------
#include <vcl.h>
#include <ddraw.h>
#include <stdio.h>
#pragma hdrstop

#include "UfrmDirectX.h"
#include "UfrmTestDisplayMode.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmDirectX *frmDirectX;
LPDIRECTDRAW lpDD;
//---------------------------------------------------------------------------
__fastcall TfrmDirectX::TfrmDirectX(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmDirectX::init ()
{
}
//---------------------------------------------------------------------------
HRESULT WINAPI EnumModesCallback( LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext)
{
char S[80];
class TDisplayMode *DM;
class TListItem *ListItem;

    if( (lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount != 8 &&
         lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount != 15 &&
         lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount != 16) ||
        (lpDDSurfaceDesc->dwWidth < 256 ||
         lpDDSurfaceDesc->dwHeight < 239))
        return DDENUMRET_OK;

    sprintf( S, "%ix%ix%i", lpDDSurfaceDesc->dwWidth, lpDDSurfaceDesc->dwHeight, lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount);

    switch( frmDirectX->cbx_Show->ItemIndex)
    {
        case 0:
            break;
        case 1:
            if( frmDirectX->Registry->ValueExists( S) )
            if( frmDirectX->Registry->ReadInteger( S) == 1)
                return DDENUMRET_OK;
            break;
        case 2:
            if( frmDirectX->Registry->ValueExists( S) )
            {
                if( frmDirectX->Registry->ReadInteger( S) != 3)
                    return DDENUMRET_OK;
            }
            else
            {   return DDENUMRET_OK;    }

            break;
    }

    DM = new TDisplayMode();
    DM->Width = lpDDSurfaceDesc->dwWidth;
    DM->Height = lpDDSurfaceDesc->dwHeight;
    DM->Depth = lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount;

    if( frmDirectX->Registry->ValueExists( S) )
    {
       DM->Tested = ((frmDirectX->Registry->ReadInteger( S) & 1) != 0);
       DM->Worked = ((frmDirectX->Registry->ReadInteger( S) & 2) != 0);
    }

    ListItem = frmDirectX->lv_DisplayModes->Items->Add();

    ListItem->Caption = S;
    ListItem->Data = DM;
    ListItem->SubItems->Add( DM->Tested ? (DM->Worked ? "Works" : "Failed") : "No");

	return DDENUMRET_OK;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDirectX::BtBtn_TestClick(TObject *Sender)
{
char S[80];
class TDisplayMode *DM;

    if( frmDirectX->lv_DisplayModes->Selected == NULL)
    {
        MessageDlg( "Please select a display mode first.", mtError,\
                    TMsgDlgButtons() << mbOK, 0);

        return;
    }

    DM = (TDisplayMode *) frmDirectX->lv_DisplayModes->Selected->Data;

    if( lpDD->SetDisplayMode( DM->Width, DM->Height, DM->Depth) != DD_OK)
    {
        DM->Tested = true;
        DM->Worked = false;

        lv_DisplayModes->Selected->SubItems->Strings[0] = DM->Worked ? "Works" : "Failed";

        MessageDlg( "An error accured while setting the display mode.", mtError,\
                    TMsgDlgButtons() << mbOK, 0);
        return;
    }

    sprintf( S, "Display mode : %ix%ix%i", DM->Width, DM->Height, DM->Depth);

    frmTestDisplayMode = new TfrmTestDisplayMode( frmDirectX);
    frmTestDisplayMode->l_Displaymode->Caption = S;

    DM->Tested = true;
    DM->Worked = frmTestDisplayMode->ShowModal() == mrYes;

    lv_DisplayModes->Selected->SubItems->Strings[0] = DM->Worked ? "Works" : "Failed";

    Registry->WriteInteger( lv_DisplayModes->Selected->Caption, (DM->Worked ? 2 : 0) | (DM->Tested ? 1 : 0));

    frmTestDisplayMode->Free();

    lpDD->SetDisplayMode( dWidth, dHeight, dDepth);

    if( cbx_Show->ItemIndex != 0 && !DM->Worked)
    {
        lv_DisplayModes->Items->Clear();
        lpDD->EnumDisplayModes( 0, NULL, NULL, EnumModesCallback);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmDirectX::BtBtn_SelectClick(TObject *Sender)
{
class TDisplayMode *DM;
TRegistry *R;

    if( frmDirectX->lv_DisplayModes->Selected == NULL)
    {
        MessageDlg( "Please select a display mode first.", mtError,\
                    TMsgDlgButtons() << mbOK, 0);

        return;
    }

    DM = (TDisplayMode *) frmDirectX->lv_DisplayModes->Selected->Data;

    if( !DM->Tested || !DM->Worked)
    {
        if( MessageDlg( "The currently selected mode did not work properly or "\
                    "was not tested,\ndo you want to continue ?", mtWarning,\
                    TMsgDlgButtons() << mbYes << mbNo, 0) == mrNo)
        {    return;    }
    }

    R = new TRegistry();
    R -> OpenKey( "Software\\Emulators\\Snes9X\\1.x", true);
    R -> WriteInteger( "Width", DM->Width);
    R -> WriteInteger( "Height", DM->Height);
    R -> WriteInteger( "Depth", DM->Depth);
    delete R;

    dWidth = DM->Width;
    dHeight = DM->Height;
    dDepth = DM->Depth;
    l_Current->Caption = "Selected display mode : "+IntToStr( dWidth)+"x"+IntToStr( dHeight)+"x"+IntToStr( dDepth);
}
//---------------------------------------------------------------------------
void __fastcall TfrmDirectX::lv_DisplayModesDblClick(TObject *Sender)
{
    BtBtn_Test->OnClick( Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmDirectX::FormResize(TObject *Sender)
{
    Width = hWidth;
    Height = hHeight;

    FormCenter();
}
//---------------------------------------------------------------------------
void __fastcall TfrmDirectX::FormCenter()
{
    Left = (Screen->Width - Width) / 2;
    Top = (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDirectX::cbx_ShowChange(TObject *Sender)
{
    Registry->WriteInteger( "ShowModes", cbx_Show->ItemIndex);

    lv_DisplayModes->Items->Clear();
    lpDD->EnumDisplayModes( 0, NULL, NULL, EnumModesCallback);
}
//---------------------------------------------------------------------------


void __fastcall TfrmDirectX::FormCreate(TObject *Sender)
{
    hHeight = Height;
    hWidth = Width;

    Registry = new TRegistry();
    Registry->OpenKey( "Software\\Emulators\\Snes9X\\1.x\\DisplayModes", true);

    cbx_Show->ItemIndex = 0;
    if( Registry->ValueExists( "ShowModes"))
        cbx_Show->ItemIndex = Registry->ReadInteger( "ShowModes");
}
//---------------------------------------------------------------------------

void __fastcall TfrmDirectX::FormDestroy(TObject *Sender)
{
    delete Registry;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDirectX::FormShow(TObject *Sender)
{
    if( lpDD == NULL)
        DirectDrawCreate( NULL, &lpDD, NULL);

    lpDD->SetCooperativeLevel( frmDirectX->Handle, DDSCL_FULLSCREEN|DDSCL_ALLOWMODEX|DDSCL_EXCLUSIVE|DDSCL_ALLOWREBOOT);

    lv_DisplayModes->Items->Clear();
    lpDD->EnumDisplayModes( 0, NULL, NULL, EnumModesCallback);

    l_Current->Caption = "Selected display mode : "+IntToStr( dWidth)+"x"+IntToStr( dHeight)+"x"+IntToStr( dDepth);

    pgctrl -> OnChange( this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDirectX::cbx_AutoFramerateClick(TObject *Sender)
{
    tb_SkipFrames -> Enabled = !cbx_AutoFramerate -> Checked;
    cbx_LimitFrameRate -> Enabled = cbx_AutoFramerate -> Checked;
    if( cbx_AutoFramerate -> Checked)
        tb_SkipFrames -> Position = 0; 
}
//---------------------------------------------------------------------------




void __fastcall TfrmDirectX::pgctrlChange(TObject *Sender)
{
    cbx_Sixteenbit -> Enabled = true;
    cbx_Transparency -> Enabled = cbx_Sixteenbit -> Enabled;
    if( !cbx_Transparency -> Enabled)
    {   cbx_Transparency -> Checked = false;    }

    cbx_HiRES -> Enabled = (dWidth >= 512 && dHeight >= 478) || !cbx_FullScreen -> Checked;
    cbx_Stretch -> Enabled = cbx_HiRES -> Enabled;
    if( !cbx_HiRES -> Enabled)
    {
        cbx_HiRES -> Checked = false;
        cbx_Stretch -> ItemIndex = 0;
    }

    if( cbx_Sixteenbit -> Checked)
    {
        if( cbx_Stretch -> Items -> Count != 10)
        {
        int Old = cbx_Stretch -> ItemIndex;
          cbx_Stretch -> Items -> Clear();
          cbx_Stretch -> Items -> Add( "None");
          cbx_Stretch -> Items -> Add( "Scale X2");
          cbx_Stretch -> Items -> Add( "Scanlines");
          cbx_Stretch -> Items -> Add( "Kreed's SuperEagle");
          cbx_Stretch -> Items -> Add( "Kreed's 2xSaI");
          cbx_Stretch -> Items -> Add( "Kreed's Super2xSaI");
	  cbx_Stretch -> Items -> Add( "TV Mode");
	  cbx_Stretch -> Items -> Add( "3D/FX bi-linear");
	  cbx_Stretch -> Items -> Add( "OpenGL bi-linear");
	  cbx_Stretch -> Items -> Add( "OpenGL bump-map");

          cbx_Stretch -> ItemIndex = Old;
        }
    }
    else
    {
        if( cbx_Stretch -> Items -> Count != 3)
        {
        int Old = cbx_Stretch -> ItemIndex;

          cbx_Stretch -> Items -> Clear();
          cbx_Stretch -> Items -> Add( "None");
          cbx_Stretch -> Items -> Add( "Scale X2");
          cbx_Stretch -> Items -> Add( "Scanlines");

          if( Old < 3)
          {     cbx_Stretch -> ItemIndex = Old;    }
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmDirectX::SpeedButton1Click(TObject *Sender)
{
    pgctrl -> OnChange( this);    
    ModalResult = 1;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDirectX::SpeedButton2Click(TObject *Sender)
{
    ModalResult = 2;    
}
//---------------------------------------------------------------------------

void __fastcall TfrmDirectX::cbx_TransparencyClick(TObject *Sender)
{
  if( cbx_Transparency -> Checked)
  {
    cbx_Sixteenbit -> Checked = true;
  }
}
//---------------------------------------------------------------------------


