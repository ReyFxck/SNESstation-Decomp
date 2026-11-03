//---------------------------------------------------------------------------
#ifndef UfrmDirectXH
#define UfrmDirectXH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Registry.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmDirectX : public TForm
{
__published:	// IDE-managed Components
    TPageControl *pgctrl;
    TTabSheet *tbsht_DisplayMode;
    TBitBtn *BtBtn_Select;
    TBitBtn *BtBtn_Test;
    TListView *lv_DisplayModes;
    TComboBox *cbx_Show;
    TTabSheet *tbsht_General;
    TPanel *Panel1;
    TLabel *l_Current;
    TGroupBox *gbx_Auto;
    TTrackBar *tb_SkipFrames;
    TCheckBox *cbx_AutoFramerate;
    TGroupBox *GroupBox1;
    TCheckBox *cbx_Sixteenbit;
    TCheckBox *cbx_Transparency;
    TGroupBox *GroupBox2;
    TCheckBox *cbx_ShowFPS;
    TGroupBox *GroupBox3;
        TButton *SpeedButton1;
        TButton *SpeedButton2;
    TComboBox *cbx_Stretch;
        TCheckBox *cbx_DoubleBuffer;
    TCheckBox *cbx_StretchBox;
    TCheckBox *cbx_FullScreen;
    TCheckBox *cbx_HiRES;
    TCheckBox *cbx_Mode7;
    TCheckBox *cbx_LimitFrameRate;
    void __fastcall BtBtn_TestClick(TObject *Sender);
    void __fastcall BtBtn_SelectClick(TObject *Sender);
    
    void __fastcall lv_DisplayModesDblClick(TObject *Sender);
    
    void __fastcall FormResize(TObject *Sender);
    void __fastcall cbx_ShowChange(TObject *Sender);
    
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall cbx_AutoFramerateClick(TObject *Sender);
    
    void __fastcall pgctrlChange(TObject *Sender);
    void __fastcall SpeedButton1Click(TObject *Sender);
    void __fastcall SpeedButton2Click(TObject *Sender);
        void __fastcall cbx_TransparencyClick(TObject *Sender);
private:	// User declarations
    int hHeight, hWidth;
public:		// User declarations
    int dHeight, dWidth, dDepth;
    TRegistry *Registry;

    void __fastcall FormCenter( void);
    void __fastcall init ();
    __fastcall TfrmDirectX(TComponent* Owner);
};

class TDisplayMode : public TObject
{
public:
    int Width;
    int Height;
    int Depth;
    bool Tested;
    bool Worked;
};

HRESULT WINAPI EnumModesCallback( LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext);
//---------------------------------------------------------------------------
extern PACKAGE TfrmDirectX *frmDirectX;
//---------------------------------------------------------------------------
#endif
