//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UfrmJoypad.h"
#include "../wsnes9x.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmJoypad *frmJoypad;
struct SJoyState Joystick [16];

struct SJoypad Joypad [5];
short OldJoypad = -1;

int __fastcall Normalize( int cur, int min, int max)
{
    int Result = 0;

    if ((max - min) == 0)
        return (Result);

    Result = cur - min;
    Result = (Result * 200) / (max - min);
    Result -= 100;

    return (Result);
}

// Returns:
// 0 - Key is just fine, nothing wrong
// 1 - Key is a windows key
// 2 - Key is a Snes9X key
// 3 - Key is used twice
int __fastcall TfrmJoypad::CheckKey( WORD Key)
{
    // Check for windows keys
    if( Key == VK_MENU || Key == VK_CAPITAL || Key == VK_LWIN ||
        Key == VK_RWIN || Key == VK_APPS)
    {
        return 1;
    }

    // Check for Snes9X keys
    if ((Key >= VK_F1 && Key <= VK_F9))
    {
        return 2;
    }

    // Check for duplicate keys
    int Found = 0;
    for( int J = 0; J != 5; J++)
    {
        if( J == OldJoypad)
        { continue; }

        if( Key == Joypad[J].Left)
        { Found ++; }
        if( Key == Joypad[J].Left_Up)
        { Found ++; }
        if( Key == Joypad[J].Left_Down)
        { Found ++; }
        if( Key == Joypad[J].Right)
        { Found ++; }
        if( Key == Joypad[J].Right_Up)
        { Found ++; }
        if( Key == Joypad[J].Right_Down)
        { Found ++; }
        if( Key == Joypad[J].Up)
        { Found ++; }
        if( Key == Joypad[J].Down)
        { Found ++; }
        if( Key == Joypad[J].Start)
        { Found ++; }
        if( Key == Joypad[J].Select)
        { Found ++; }
        if( Key == Joypad[J].A)
        { Found ++; }
        if( Key == Joypad[J].B)
        { Found ++; }
        if( Key == Joypad[J].X)
        { Found ++; }
        if( Key == Joypad[J].Y)
        { Found ++; }
        if( Key == Joypad[J].L)
        { Found ++; }
        if( Key == Joypad[J].R)
        { Found ++; }
    }
    if( Key == (WORD) Edt_Left -> Tag)
    { Found ++; }
    if( Key == (WORD) Edt_Left_Up -> Tag)
    { Found ++; }
    if( Key == (WORD) Edt_Left_Down -> Tag)
    { Found ++; }
    if( Key == (WORD) Edt_Right -> Tag)
    { Found ++; }
    if( Key == (WORD) Edt_Right_Up -> Tag)
    { Found ++; }
    if( Key == (WORD) Edt_Right_Down -> Tag)
    { Found ++; }
    if( Key == (WORD) Edt_Up -> Tag)
    { Found ++; }
    if( Key == (WORD) Edt_Down -> Tag)
    { Found ++; }
    if( Key == (WORD) Edt_Start -> Tag)
    { Found ++; }
    if( Key == (WORD) Edt_Select -> Tag)
    { Found ++; }
    if( Key == (WORD) Edt_A -> Tag)
    { Found ++; }
    if( Key == (WORD) Edt_B -> Tag)
    { Found ++; }
    if( Key == (WORD) Edt_X -> Tag)
    { Found ++; }
    if( Key == (WORD) Edt_Y -> Tag)
    { Found ++; }
    if( Key == (WORD) Edt_L -> Tag)
    { Found ++; }
    if( Key == (WORD) Edt_R -> Tag)
    { Found ++; }

    if( Found > 1)
    {
        return 3;
    }

    return 0;
}

System::String TranslateKey (WORD Key)
{
System::String Result;

    if (Key&0x8000)
    {
        Result = "(J"+IntToStr( (Key>>8)&0xF)+")";
        switch (Key&0xFF)
        {
            case 0:  Result = Result + "Left"; break;
            case 1:  Result = Result + "Right"; break;
            case 2:  Result = Result + "Up"; break;
            case 3:  Result = Result + "Down"; break;
            case 4:  Result = Result + "POV Left"; break;
            case 5:  Result = Result + "POV Right"; break;
            case 6:  Result = Result + "POV Up"; break;
            case 7:  Result = Result + "POV Down"; break;
            case 41: Result = Result + "Z Up"; break;
            case 42: Result = Result + "Z Down"; break;
            case 43: Result = Result + "R Up"; break;
            case 44: Result = Result + "R Down"; break;
            case 45: Result = Result + "U Up"; break;
            case 46: Result = Result + "U Down"; break;
            case 47: Result = Result + "V Up"; break;
            case 48: Result = Result + "V Down"; break;
            default:
                if ((Key & 0xff) > 40)
                {
                    Result = Result + "#["+IntToStr(Key&0xFF)+"]";
                    break;
                }

                Result = Result + "Button "+IntToStr( (Key&0xFF)-8);
                break;
        }
        return Result;
    }

    Result = "#"+IntToStr( Key);

    // 0~9, A~Z
    if( (Key >= '0' && Key <= '9') || (Key >= 'A' && Key <= 'Z'))
    {
        Result = char(Key);
        return Result;
    }

    if( Key >= VK_NUMPAD0 && Key <= VK_NUMPAD9)
    {
        Result = char('0'+(Key-VK_NUMPAD0));
        Result = "Numpad-"+Result;
        return Result;
    }

    switch(Key)
    {
        case 0: Result = "Disabled"; break;
        case VK_TAB: Result = "Tab"; break;
        case VK_BACK: Result = "Backspace"; break;
        case VK_CLEAR: Result = "Delete"; break;
        case VK_RETURN: Result = "Enter"; break;
        case VK_SHIFT:  Result = "Shift"; break;
        case VK_CONTROL: Result = "CTRL"; break;
        case VK_MENU: Result = "Alt"; break;
        case VK_PAUSE: Result = "Pause"; break;
        case VK_CAPITAL: Result = "Capslock"; break;
        case VK_ESCAPE: Result = "Escape"; break;
        case VK_SPACE: Result = "Space"; break;
        case VK_PRIOR: Result = "PgDn"; break;
        case VK_NEXT: Result = "PgUp"; break;
        case VK_HOME: Result = "Home"; break;
        case VK_END: Result = "End"; break;
        case VK_LEFT: Result = "Left"; break;
        case VK_RIGHT: Result = "Right"; break;
        case VK_UP: Result = "Up"; break;
        case VK_DOWN: Result = "Down"; break;
        case VK_SELECT: Result = "Select"; break;
        case VK_PRINT: Result = "Print"; break;
        case VK_EXECUTE: Result = "Execute"; break;
        case VK_SNAPSHOT: Result = "SnapShot"; break;
        case VK_INSERT: Result = "Insert"; break;
        case VK_DELETE: Result = "Delete"; break;
        case VK_HELP: Result = "Help"; break;
        case VK_LWIN: Result = "LWinKey"; break;
        case VK_RWIN: Result = "RWinKey"; break;
        case VK_APPS: Result = "AppKey"; break;
        case VK_MULTIPLY: Result = "*"; break;
        case VK_ADD: Result = "+"; break;
        case VK_SEPARATOR: Result = "\\"; break;
        case VK_SUBTRACT: Result = "-"; break;
        case VK_DECIMAL: Result = "."; break;
        case VK_DIVIDE: Result = "/"; break;
        case VK_NUMLOCK: Result = "Num-lock"; break;
        case VK_SCROLL: Result = "Scroll-lock"; break;
    }
    return Result;
}
//---------------------------------------------------------------------------
// 0 - Left
// 1 - Right
// 2 - Up
// 3 - Down
// 4 - PovLeft
// 5 - PovRight
// 6 - PovUp
// 7 - PovDown
// 8 ~ 40 - Button 1 ~ 32
//---------------------------------------------------------------------------
void __fastcall TfrmJoypad::JoystickChanged( short ID, short Movement)
{
    if( ActiveControl == NULL)
        return;

    if( !CompareStr( ActiveControl -> ClassName(), "TEdit"))
    {
        WORD JoyKey = 0x8000;

        JoyKey |= (WORD)(ID << 8);
        JoyKey |= Movement;

        ((TEdit *)(ActiveControl)) -> OnKeyDown( ActiveControl, JoyKey, TShiftState());
    }
}
//---------------------------------------------------------------------------
__fastcall TfrmJoypad::TfrmJoypad(TComponent* Owner)
    : TForm(Owner)
{
    init ();
}
//---------------------------------------------------------------------------
void __fastcall TfrmJoypad::init ()
{
    for( short C = 0; C != 16; C ++)
        Joystick[C].Attached = joyGetDevCaps( JOYSTICKID1+C, &Joystick[C].Caps, sizeof( JOYCAPS)) == JOYERR_NOERROR;
}
//---------------------------------------------------------------------------
void __fastcall TfrmJoypad::CheckAxis (short joy, short control, int val, 
                                       int min, int max,
                                       bool &first, bool &second)
{
    if (Normalize (val, min, max) < -S9X_JOY_NEUTRAL)
    {
        second = false;
        if (!first)
        {
            JoystickChanged (joy, control);
            first = true;
        }
    }
    else
        first = false;
    
    if (Normalize (val, min, max) > S9X_JOY_NEUTRAL)
    {
        first = false;
        if (!second)
        {
            JoystickChanged (joy, (short) (control + 1));
            second = true;
        }
    }
    else
        second = false;
}

void __fastcall TfrmJoypad::t_JoystickTimer(TObject *Sender)
{
    JOYINFOEX jie;

    for (short C = 0; C != 16; C ++)
    {
        jie.dwSize = sizeof( jie);
        jie.dwFlags = JOY_RETURNALL;

        if (joyGetPosEx (JOYSTICKID1 + C, &jie) != JOYERR_NOERROR)
            continue;

        CheckAxis (C, 0, jie.dwXpos, 
                   Joystick[C].Caps.wXmin, Joystick[C].Caps.wXmax,
                   Joystick[C].Left, Joystick[C].Right);
        CheckAxis (C, 2, jie.dwYpos,
                   Joystick[C].Caps.wYmin, Joystick[C].Caps.wYmax,
                   Joystick[C].Up, Joystick[C].Down);
        CheckAxis (C, 41, jie.dwZpos,
                   Joystick[C].Caps.wZmin, Joystick[C].Caps.wZmax,
                   Joystick[C].ZUp, Joystick[C].ZDown);
        CheckAxis (C, 43, jie.dwRpos,
                   Joystick[C].Caps.wRmin, Joystick[C].Caps.wRmax,
                   Joystick[C].RUp, Joystick[C].RDown);
        CheckAxis (C, 45, jie.dwUpos,
                   Joystick[C].Caps.wUmin, Joystick[C].Caps.wUmax,
                   Joystick[C].UUp, Joystick[C].UDown);
        CheckAxis (C, 47, jie.dwVpos,
                   Joystick[C].Caps.wVmin, Joystick[C].Caps.wVmax,
                   Joystick[C].VUp, Joystick[C].VDown);

        switch (jie.dwPOV)
        {
            case JOY_POVBACKWARD:
                if( !Joystick[C].PovDown)
                {   JoystickChanged( C, 7); }

                Joystick[C].PovDown = true;
                Joystick[C].PovUp = false;
                Joystick[C].PovLeft = false;
                Joystick[C].PovRight = false;
                break;

            case JOY_POVFORWARD:
                if( !Joystick[C].PovUp)
                {   JoystickChanged( C, 6); }

                Joystick[C].PovDown = false;
                Joystick[C].PovUp = true;
                Joystick[C].PovLeft = false;
                Joystick[C].PovRight = false;
                break;

            case JOY_POVLEFT:
                if( !Joystick[C].PovLeft)
                {   JoystickChanged( C, 4); }

                Joystick[C].PovDown = false;
                Joystick[C].PovUp = false;
                Joystick[C].PovLeft = true;
                Joystick[C].PovRight = false;
                break;

            case JOY_POVRIGHT:
                if( !Joystick[C].PovRight)
                {   JoystickChanged( C, 5); }

                Joystick[C].PovDown = false;
                Joystick[C].PovUp = false;
                Joystick[C].PovLeft = false;
                Joystick[C].PovRight = true;
                break;

            default:
                Joystick[C].PovDown = false;
                Joystick[C].PovUp = false;
                Joystick[C].PovLeft = false;
                Joystick[C].PovRight = false;
                break;
        }

        for( short B = 0; B != 32; B ++, jie.dwButtons >>= 1)
        if( (jie.dwButtons&1))
        {
            if( !Joystick[C].Button[B])
            {
                JoystickChanged( C, (short)(8+B));
                Joystick[C].Button[B] = true;
            }
        }
        else
        {   Joystick[C].Button[B] = false; }
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmJoypad::Edt_UpEnter(TObject *Sender)
{
    ((TEdit *)(Sender)) -> Color = clLime;
}
//---------------------------------------------------------------------------

void __fastcall TfrmJoypad::Edt_UpExit(TObject *Sender)
{
WORD Key = (WORD) (((TEdit *)(Sender)) -> Tag);
    switch( CheckKey( Key))
    {
        case 0: ((TEdit *)(Sender)) -> Color = clWindow; break;
        case 1: ((TEdit *)(Sender)) -> Color = clRed; break;
        case 2: ((TEdit *)(Sender)) -> Color = clRed; break;
        case 3: ((TEdit *)(Sender)) -> Color = clBlue; break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmJoypad::cbx_JoypadChange(TObject *Sender)
{
    if( OldJoypad != -1)
    {
        Joypad[OldJoypad].Enabled = cb_Enabled -> Checked;
        Joypad[OldJoypad].Left = (WORD) Edt_Left -> Tag;
        Joypad[OldJoypad].Left_Up = (WORD) Edt_Left_Up -> Tag;
        Joypad[OldJoypad].Left_Down = (WORD) Edt_Left_Down -> Tag;
        Joypad[OldJoypad].Right = (WORD) Edt_Right -> Tag;
        Joypad[OldJoypad].Right_Up = (WORD) Edt_Right_Up -> Tag;
        Joypad[OldJoypad].Right_Down = (WORD) Edt_Right_Down -> Tag;
        Joypad[OldJoypad].Up = (WORD) Edt_Up -> Tag;
        Joypad[OldJoypad].Down = (WORD) Edt_Down -> Tag;
        Joypad[OldJoypad].Start = (WORD) Edt_Start -> Tag;
        Joypad[OldJoypad].Select = (WORD) Edt_Select -> Tag;
        Joypad[OldJoypad].A = (WORD) Edt_A -> Tag;
        Joypad[OldJoypad].B = (WORD) Edt_B -> Tag;
        Joypad[OldJoypad].X = (WORD) Edt_X -> Tag;
        Joypad[OldJoypad].Y = (WORD) Edt_Y -> Tag;
        Joypad[OldJoypad].L = (WORD) Edt_L -> Tag;
        Joypad[OldJoypad].R = (WORD) Edt_R -> Tag;
    }

    OldJoypad = (short) (cbx_Joypad -> ItemIndex);

    if( OldJoypad != -1)
    {
        cb_Enabled -> Checked = Joypad[OldJoypad].Enabled;
        Edt_SetState( Edt_Left, Joypad[OldJoypad].Left);
        Edt_SetState( Edt_Left_Up, Joypad[OldJoypad].Left_Up);
        Edt_SetState( Edt_Left_Down, Joypad[OldJoypad].Left_Down);
        Edt_SetState( Edt_Right, Joypad[OldJoypad].Right);
        Edt_SetState( Edt_Right_Up, Joypad[OldJoypad].Right_Up);
        Edt_SetState( Edt_Right_Down, Joypad[OldJoypad].Right_Down);
        Edt_SetState( Edt_Up, Joypad[OldJoypad].Up);
        Edt_SetState( Edt_Down, Joypad[OldJoypad].Down);
        Edt_SetState( Edt_Start, Joypad[OldJoypad].Start);
        Edt_SetState( Edt_Select, Joypad[OldJoypad].Select);
        Edt_SetState( Edt_A, Joypad[OldJoypad].A);
        Edt_SetState( Edt_B, Joypad[OldJoypad].B);
        Edt_SetState( Edt_X, Joypad[OldJoypad].X);
        Edt_SetState( Edt_Y, Joypad[OldJoypad].Y);
        Edt_SetState( Edt_L, Joypad[OldJoypad].L);
        Edt_SetState( Edt_R, Joypad[OldJoypad].R);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmJoypad::Edt_SetState(TEdit *Edt, WORD Key)
{
    Edt -> Tag = Key;
    Edt -> Text = TranslateKey( (WORD) Edt -> Tag);

    switch( CheckKey( (WORD) Edt -> Tag))
    {
        case 0: Edt -> Color = clWindow; break;
        case 1: Edt -> Color = clRed; break;
        case 2: Edt -> Color = clRed; break;
        case 3: Edt -> Color = clBlue; break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmJoypad::Edt_KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    WORD k = Key;
    if (k == VK_ESCAPE)
        k = 0;
    ((TEdit *)(Sender)) -> Tag = k;
    ((TEdit *)(Sender)) -> Text = TranslateKey (k);

    if( Key&0x8000)
        Key = 0;

    Perform( WM_NEXTDLGCTL, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TfrmJoypad::SpeedButton2Click(TObject *Sender)
{
    cbx_Joypad -> ItemIndex = -1;
    cbx_Joypad -> OnChange( this);
    
    ModalResult = 1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmJoypad::SpeedButton1Click(TObject *Sender)
{
    ModalResult = 2;
}
//---------------------------------------------------------------------------

void __fastcall TfrmJoypad::FormShow(TObject *Sender)
{
    cbx_Joypad -> ItemIndex = 0;
    OldJoypad = -1;
    cbx_Joypad -> OnChange( this);
}
//---------------------------------------------------------------------------
