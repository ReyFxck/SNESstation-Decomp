//---------------------------------------------------------------------------
#ifndef UfrmJoypadH
#define UfrmJoypadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <MMSystem.hpp>
#include <Buttons.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------

class TfrmJoypad : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel13;
    TImage *Image2;
    TEdit *Edt_Right;
    TEdit *Edt_Up;
    TEdit *Edt_X;
    TEdit *Edt_Y;
    TEdit *Edt_R;
    TEdit *Edt_L;
    TEdit *Edt_Left;
    TEdit *Edt_Down;
    TEdit *Edt_Select;
    TEdit *Edt_Start;
    TEdit *Edt_B;
    TEdit *Edt_A;
    TTimer *t_Joystick;
    TComboBox *cbx_Joypad;
    TLabel *Label1;
    TBevel *Bevel1;
    TPanel *Panel1;
    TPanel *Panel2;
    TLabel *Label2;
    TLabel *Label3;
        TButton *SpeedButton1;
        TButton *SpeedButton2;
        TCheckBox *cb_Enabled;
    TEdit *Edt_Right_Up;
    TEdit *Edt_Left_Up;
    TEdit *Edt_Left_Down;
    TEdit *Edt_Right_Down;
    void __fastcall t_JoystickTimer(TObject *Sender);
    void __fastcall Edt_UpEnter(TObject *Sender);
    void __fastcall Edt_UpExit(TObject *Sender);
    void __fastcall cbx_JoypadChange(TObject *Sender);
    void __fastcall Edt_SetState(TEdit *Edt, WORD Key);
    void __fastcall Edt_KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    
    void __fastcall SpeedButton2Click(TObject *Sender);
    void __fastcall SpeedButton1Click(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
    void __fastcall JoystickChanged( short ID, short Movement);
    void __fastcall CheckAxis (short joy, short control, int val, 
                               int min, int max,
                               bool &first, bool &second);
public:		// User declarations
    __fastcall TfrmJoypad(TComponent* Owner);
    void __fastcall init ();

    int __fastcall CheckKey( WORD Key);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmJoypad *frmJoypad;
//---------------------------------------------------------------------------
#endif
