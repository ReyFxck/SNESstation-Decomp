#define STRICT
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fstream.h>
#include "InputCustom.h"
#include "wsnes9x.h"
#include "language.h"

static TCHAR szClassName[] = _T("InputCustom");

LRESULT CALLBACK InputCustomWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef struct
{
    COLORREF crForeGnd;    // Foreground text colour
    COLORREF crBackGnd;    // Background text colour
    HFONT    hFont;        // The font
    HWND     hwnd;         // The control's window handle
} InputCust;


extern SJoyState JoystickF [16];

HWND funky;
WPARAM tid;


void JoystickChanged( short ID, short Movement)
{	
	WORD JoyKey;

    JoyKey = 0x8000;
	JoyKey |= (WORD)(ID << 8);
	JoyKey |= Movement;
	SendMessage(funky,WM_USER+45,JoyKey,0);
	KillTimer(funky,tid);


}

int FunkyNormalize(int cur, int min, int max)
{
	int Result = 0;

    if ((max - min) == 0)
	
        return (Result);

    Result = cur - min;
    Result = (Result * 200) / (max - min);
    Result -= 100;

    return Result;
}


void CheckAxis (short joy, short control, int val, 
                                       int min, int max,
                                       bool &first, bool &second)
{



    if (FunkyNormalize (val, min, max) < -S9X_JOY_NEUTRAL)

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

    if (FunkyNormalize (val, min, max) > S9X_JOY_NEUTRAL)
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

void FunkyJoyStickTimer()
{
	JOYINFOEX jie;
	
    for (short C = 0; C != 16; C ++)
    {
		
        jie.dwSize = sizeof( jie);
        jie.dwFlags = JOY_RETURNALL;

        if (joyGetPosEx (JOYSTICKID1 + C, &jie) != JOYERR_NOERROR)
            continue;

        CheckAxis (C, 0, jie.dwXpos, 
                   JoystickF[C].Caps.wXmin, JoystickF[C].Caps.wXmax,
                   JoystickF[C].Left, JoystickF[C].Right);
        CheckAxis (C, 2, jie.dwYpos,
                   JoystickF[C].Caps.wYmin, JoystickF[C].Caps.wYmax,
                   JoystickF[C].Up, JoystickF[C].Down);
        if(JoystickF[C].Caps.wCaps & JOYCAPS_HASZ)
		{
		CheckAxis (C, 41, jie.dwZpos,
                   JoystickF[C].Caps.wZmin, JoystickF[C].Caps.wZmax,
                   JoystickF[C].ZUp, JoystickF[C].ZDown);
		}
		if(JoystickF[C].Caps.wCaps & JOYCAPS_HASR)
		{
        CheckAxis (C, 43, jie.dwRpos,
                   JoystickF[C].Caps.wRmin, JoystickF[C].Caps.wRmax,
                   JoystickF[C].RUp, JoystickF[C].RDown);
		}
		if(JoystickF[C].Caps.wCaps & JOYCAPS_HASU)
		{
        CheckAxis (C, 45, jie.dwUpos,
                   JoystickF[C].Caps.wUmin, JoystickF[C].Caps.wUmax,
                   JoystickF[C].UUp, JoystickF[C].UDown);
		}
        if(JoystickF[C].Caps.wCaps & JOYCAPS_HASV)
		{
		CheckAxis (C, 47, jie.dwVpos,
                   JoystickF[C].Caps.wVmin, JoystickF[C].Caps.wVmax,
                   JoystickF[C].VUp, JoystickF[C].VDown);
		}

        switch (jie.dwPOV)
        {
            case JOY_POVBACKWARD:
                if( !JoystickF[C].PovDown)
                {   JoystickChanged( C, 7); }

                JoystickF[C].PovDown = true;
                JoystickF[C].PovUp = false;
                JoystickF[C].PovLeft = false;
                JoystickF[C].PovRight = false;
				JoystickF[C].PovDnLeft = false;
				JoystickF[C].PovDnRight = false;
				JoystickF[C].PovUpLeft = false;
				JoystickF[C].PovUpRight = false;
                break;
			case 4500:
				if( !JoystickF[C].PovUpRight)
                {   JoystickChanged( C, 52); }
				JoystickF[C].PovDown = false;
                JoystickF[C].PovUp = false;
                JoystickF[C].PovLeft = false;
                JoystickF[C].PovRight = false;
				JoystickF[C].PovDnLeft = false;
				JoystickF[C].PovDnRight = false;
				JoystickF[C].PovUpLeft = false;
				JoystickF[C].PovUpRight = true;
				break;
			case 13500:
				if( !JoystickF[C].PovDnRight)
                {   JoystickChanged( C, 50); }
				JoystickF[C].PovDown = false;
                JoystickF[C].PovUp = false;
                JoystickF[C].PovLeft = false;
                JoystickF[C].PovRight = false;
				JoystickF[C].PovDnLeft = false;
				JoystickF[C].PovDnRight = true;
				JoystickF[C].PovUpLeft = false;
				JoystickF[C].PovUpRight = false;
				break;
			case 22500:
				if( !JoystickF[C].PovDnLeft)
                {   JoystickChanged( C, 49); }
				JoystickF[C].PovDown = false;
                JoystickF[C].PovUp = false;
                JoystickF[C].PovLeft = false;
                JoystickF[C].PovRight = false;
				JoystickF[C].PovDnLeft = true;
				JoystickF[C].PovDnRight = false;
				JoystickF[C].PovUpLeft = false;
				JoystickF[C].PovUpRight = false;
				break;
			case 31500:
				if( !JoystickF[C].PovUpLeft)
                {   JoystickChanged( C, 51); }
				JoystickF[C].PovDown = false;
                JoystickF[C].PovUp = false;
                JoystickF[C].PovLeft = false;
                JoystickF[C].PovRight = false;
				JoystickF[C].PovDnLeft = false;
				JoystickF[C].PovDnRight = false;
				JoystickF[C].PovUpLeft = true;
				JoystickF[C].PovUpRight = false;
				break;

            case JOY_POVFORWARD:
                if( !JoystickF[C].PovUp)
                {   JoystickChanged( C, 6); }

                JoystickF[C].PovDown = false;
                JoystickF[C].PovUp = true;
                JoystickF[C].PovLeft = false;
                JoystickF[C].PovRight = false;
				JoystickF[C].PovDnLeft = false;
				JoystickF[C].PovDnRight = false;
				JoystickF[C].PovUpLeft = false;
				JoystickF[C].PovUpRight = false;
                break;

            case JOY_POVLEFT:
                if( !JoystickF[C].PovLeft)
                {   JoystickChanged( C, 4); }

				JoystickF[C].PovDown = false;
				JoystickF[C].PovUp = false;
				JoystickF[C].PovLeft = true;
				JoystickF[C].PovRight = false;
				JoystickF[C].PovDnLeft = false;
				JoystickF[C].PovDnRight = false;
				JoystickF[C].PovUpLeft = false;
				JoystickF[C].PovUpRight = false;
                break;

            case JOY_POVRIGHT:
                if( !JoystickF[C].PovRight)
                {   JoystickChanged( C, 5); }

				JoystickF[C].PovDown = false;
				JoystickF[C].PovUp = false;
				JoystickF[C].PovLeft = false;
				JoystickF[C].PovRight = true;
				JoystickF[C].PovDnLeft = false;
				JoystickF[C].PovDnRight = false;
				JoystickF[C].PovUpLeft = false;
				JoystickF[C].PovUpRight = false;
                break;

            default:
                JoystickF[C].PovDown = false;
                JoystickF[C].PovUp = false;
                JoystickF[C].PovLeft = false;
                JoystickF[C].PovRight = false;
				JoystickF[C].PovDnLeft = false;
				JoystickF[C].PovDnRight = false;
				JoystickF[C].PovUpLeft = false;
				JoystickF[C].PovUpRight = false;
                break;
        }

        for( short B = 0; B != 32; B ++, jie.dwButtons >>= 1)
        if( (jie.dwButtons&1))
        {
            if( !JoystickF[C].Button[B])
            {
                JoystickChanged( C, (short)(8+B));
                JoystickF[C].Button[B] = true;
            }
        }
        else
        {   JoystickF[C].Button[B] = false; }

    }

	



}
void TranslateKey(WORD keyz,char *out)
{
	char temp[100];
	if(keyz&0x8000)
	{
		sprintf(out,GAMEDEVICE_JOYNUMPREFIX,((keyz>>8)&0xF));
		switch(keyz&0xFF)
		{
		case 0:  strncat(out,GAMEDEVICE_XNEG,4); break;
		case 1:  strncat(out,GAMEDEVICE_XPOS,5); break;
        case 2:  strncat(out,GAMEDEVICE_YPOS,2); break;
		case 3:  strncat(out,GAMEDEVICE_YNEG,4); break;
		case 4:  strncat(out,GAMEDEVICE_POVLEFT,8); break;
		case 5:  strncat(out,GAMEDEVICE_POVRIGHT,9); break;
		case 6:  strncat(out,GAMEDEVICE_POVUP,6); break;
		case 7:  strncat(out,GAMEDEVICE_POVDOWN,8); break;
		case 49: strncat(out,GAMEDEVICE_POVDNLEFT,11); break;
		case 50: strncat(out,GAMEDEVICE_POVDNRIGHT,12); break;
		case 51: strncat(out,GAMEDEVICE_POVUPLEFT,11); break;
		case 52: strncat(out,GAMEDEVICE_POVUPRIGHT,12); break;
		case 41: strncat(out,GAMEDEVICE_ZPOS,4); break;
		case 42: strncat(out,GAMEDEVICE_ZNEG,6); break;
		case 43: strncat(out,GAMEDEVICE_RPOS,4); break;
		case 44: strncat(out,GAMEDEVICE_RNEG,6); break;
		case 45: strncat(out,GAMEDEVICE_UPOS,4); break;
		case 46: strncat(out,GAMEDEVICE_UNEG,6); break;
		case 47: strncat(out,GAMEDEVICE_VPOS,4); break;
		case 48: strncat(out,GAMEDEVICE_VNEG,6); break;
		default:	                
			if ((keyz & 0xff) > 40)
            {
				sprintf(temp,GAMEDEVICE_JOYBUTPREFIX,keyz&0xFF);
				strncat(out,temp,strlen(temp));
				break;
            }

			sprintf(temp,GAMEDEVICE_BUTTON,(keyz&0xFF)-8);
			strncat(out,temp,strlen(temp));
			break;
				
		}
		return;
	}
	sprintf(out,GAMEDEVICE_KEY,keyz);
	if((keyz>='0' && keyz<='9')||(keyz>='A' &&keyz<='Z'))
	{
		sprintf(out,"%c",keyz);
		return;
	}
	    if( keyz >= VK_NUMPAD0 && keyz <= VK_NUMPAD9)
    {
        
		sprintf(out,GAMEDEVICE_NUMPADPREFIX,'0'+(keyz-VK_NUMPAD0));
   
        return ;
    }
	switch(keyz)
    {
        case 0:				sprintf(out,GAMEDEVICE_DISABLED); break;
        case VK_TAB:		sprintf(out,GAMEDEVICE_VK_TAB); break;
        case VK_BACK:		sprintf(out,GAMEDEVICE_VK_BACK); break;
        case VK_CLEAR:		sprintf(out,GAMEDEVICE_VK_CLEAR); break;
        case VK_RETURN:		sprintf(out,GAMEDEVICE_VK_RETURN); break;
        case VK_LSHIFT:		sprintf(out,GAMEDEVICE_VK_LSHIFT); break;
		case VK_RSHIFT:		sprintf(out,GAMEDEVICE_VK_RSHIFT); break;
        case VK_LCONTROL:	sprintf(out,GAMEDEVICE_VK_LCONTROL); break;
		case VK_RCONTROL:	sprintf(out,GAMEDEVICE_VK_RCONTROL); break;
        case VK_LMENU:		sprintf(out,GAMEDEVICE_VK_LMENU); break;
		case VK_RMENU:		sprintf(out,GAMEDEVICE_VK_RMENU); break;
        case VK_PAUSE:		sprintf(out,GAMEDEVICE_VK_PAUSE); break;
        case VK_CAPITAL:	sprintf(out,GAMEDEVICE_VK_CAPITAL); break;
        case VK_ESCAPE:		sprintf(out,GAMEDEVICE_VK_ESCAPE); break;
        case VK_SPACE:		sprintf(out,GAMEDEVICE_VK_SPACE); break;
        case VK_PRIOR:		sprintf(out,GAMEDEVICE_VK_PRIOR); break;
        case VK_NEXT:		sprintf(out,GAMEDEVICE_VK_NEXT); break;
        case VK_HOME:		sprintf(out,GAMEDEVICE_VK_HOME); break;
        case VK_END:		sprintf(out,GAMEDEVICE_VK_END); break;
        case VK_LEFT:		sprintf(out,GAMEDEVICE_VK_LEFT ); break;
        case VK_RIGHT:		sprintf(out,GAMEDEVICE_VK_RIGHT); break;
        case VK_UP:			sprintf(out,GAMEDEVICE_VK_UP); break;
        case VK_DOWN:		sprintf(out,GAMEDEVICE_VK_DOWN); break;
        case VK_SELECT:		sprintf(out,GAMEDEVICE_VK_SELECT); break;
        case VK_PRINT:		sprintf(out,GAMEDEVICE_VK_PRINT); break;
        case VK_EXECUTE:	sprintf(out,GAMEDEVICE_VK_EXECUTE); break;
        case VK_SNAPSHOT:	sprintf(out,GAMEDEVICE_VK_SNAPSHOT); break;
        case VK_INSERT:		sprintf(out,GAMEDEVICE_VK_INSERT); break;
        case VK_DELETE:		sprintf(out,GAMEDEVICE_VK_DELETE); break;
        case VK_HELP:		sprintf(out,GAMEDEVICE_VK_HELP); break;
        case VK_LWIN:		sprintf(out,GAMEDEVICE_VK_LWIN); break;
        case VK_RWIN:		sprintf(out,GAMEDEVICE_VK_RWIN); break;
        case VK_APPS:		sprintf(out,GAMEDEVICE_VK_APPS); break;
        case VK_MULTIPLY:	sprintf(out,GAMEDEVICE_VK_MULTIPLY); break;
        case VK_ADD:		sprintf(out,GAMEDEVICE_VK_ADD); break;
        case VK_SEPARATOR:	sprintf(out,GAMEDEVICE_VK_SEPARATOR); break;
		case VK_OEM_1:		sprintf(out,GAMEDEVICE_VK_OEM_1); break;
		case VK_OEM_7:		sprintf(out,GAMEDEVICE_VK_OEM_7); break;
		case VK_OEM_COMMA:	sprintf(out,GAMEDEVICE_VK_OEM_COMMA );break;
		case VK_OEM_PERIOD:	sprintf(out,GAMEDEVICE_VK_OEM_PERIOD);break;
        case VK_SUBTRACT:	sprintf(out,GAMEDEVICE_VK_SUBTRACT); break;
        case VK_DECIMAL:	sprintf(out,GAMEDEVICE_VK_DECIMAL); break;
        case VK_DIVIDE:		sprintf(out,GAMEDEVICE_VK_DIVIDE); break;
        case VK_NUMLOCK:	sprintf(out,GAMEDEVICE_VK_NUMLOCK); break;
        case VK_SCROLL:		sprintf(out,GAMEDEVICE_VK_SCROLL); break;

		
		
    }
	
    return ;



}
COLORREF CheckKey( WORD Key)
{
	COLORREF red,blue,white;
	red =RGB(255,0,0);
	blue = RGB(0,0,255);
	white = RGB(255,255,255);
    // Check for windows keys
    if( Key == VK_MENU || Key == VK_CAPITAL || Key == VK_LWIN ||
        Key == VK_RWIN || Key == VK_APPS)
    {
        //return 1;
		return red;
    }

    // Check for Snes9X keys
    if ((Key >= VK_F1 && Key <= VK_F9) || Key == VK_F12 || Key == VK_TAB ||
		Key == VK_OEM_MINUS || Key == VK_OEM_PLUS || Key == VK_OEM_3 || (Key >= 0x30 && Key <= 0x39))
    {
        return red;
    }

    // Check for duplicate keys
    int Found = 0;
    for( int J = 0; J != 5; J++)
    {



        //if( J == OldJoypad)
        //{ continue; }

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




    if( Found > 1)
    {
        return blue;
    }

    return white;
}
void InitInputCustomControl()
{

    WNDCLASSEX wc;
    
    wc.cbSize         = sizeof(wc);
    wc.lpszClassName  = szClassName;
    wc.hInstance      = GetModuleHandle(0);
    wc.lpfnWndProc    = InputCustomWndProc;
    wc.hCursor        = LoadCursor (NULL, IDC_ARROW);
    wc.hIcon          = 0;
    wc.lpszMenuName   = 0;
    wc.hbrBackground  = (HBRUSH)GetSysColorBrush(COLOR_BTNFACE);
    wc.style          = 0;
    wc.cbClsExtra     = 0;
    wc.cbWndExtra     = sizeof(InputCust *);
    wc.hIconSm        = 0;


    RegisterClassEx(&wc);

}
HWND CreateInputCustom(HWND hwndParent)
{
    HWND hwndCtrl;

    hwndCtrl = CreateWindowEx(
                 WS_EX_CLIENTEDGE, // give it a standard border
                 szClassName,
                 _T("A custom control"),
                 WS_VISIBLE | WS_CHILD,
                 0, 0, 100, 100,
                 hwndParent,
                 NULL, GetModuleHandle(0), NULL
               );

    return hwndCtrl;
}
static InputCust * GetInputCustom(HWND hwnd)
{
	return (InputCust *)GetWindowLong(hwnd, 0);
}

void SetInputCustom(HWND hwnd, InputCust *icp)
{
    SetWindowLong(hwnd, 0, (LONG)icp);
}

LRESULT InputCustom_OnPaint(InputCust *ccp, WPARAM wParam, LPARAM lParam)
{
    HDC				hdc;
    PAINTSTRUCT		ps;
    HANDLE			hOldFont;
    TCHAR			szText[200];
    RECT			rect;
	SIZE			sz;
	int				x,y;

    // Get a device context for this window
    hdc = BeginPaint(ccp->hwnd, &ps);

    // Set the font we are going to use
    hOldFont = SelectObject(hdc, ccp->hFont);

    // Set the text colours
    SetTextColor(hdc, ccp->crForeGnd);
    SetBkColor  (hdc, ccp->crBackGnd);

    // Find the text to draw
    GetWindowText(ccp->hwnd, szText, sizeof(szText));
   
    // Work out where to draw
    GetClientRect(ccp->hwnd, &rect);


    // Find out how big the text will be
    GetTextExtentPoint32(hdc, szText, lstrlen(szText), &sz);

    // Center the text
    x = (rect.right  - sz.cx) / 2;
    y = (rect.bottom - sz.cy) / 2;
    
    // Draw the text
    ExtTextOut(hdc, x, y, ETO_OPAQUE, &rect, szText, lstrlen(szText), 0);

    // Restore the old font when we have finished
    SelectObject(hdc, hOldFont);

    // Release the device context
    EndPaint(ccp->hwnd, &ps);

    return 0;
}

static LRESULT CALLBACK InputCustomWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// retrieve the custom structure POINTER for THIS window
    InputCust *icp = GetInputCustom(hwnd);
	HWND pappy = (HWND__ *)GetWindowLongPtr(hwnd,GWL_HWNDPARENT);
	funky= hwnd;
	
	char temp[100];
	COLORREF col;
    switch(msg)
    {
	case WM_GETDLGCODE:
		return DLGC_WANTARROWS|DLGC_WANTALLKEYS|DLGC_WANTCHARS;
		break;
	

    case WM_NCCREATE:
        
        // Allocate a new CustCtrl structure for this window.
        icp = (InputCust *) malloc( sizeof(InputCust) );
        
        // Failed to allocate, stop window creation.
        if(icp == NULL) return FALSE;

        // Initialize the CustCtrl structure. 
        icp->hwnd      = hwnd;
        icp->crForeGnd = GetSysColor(COLOR_WINDOWTEXT);
        icp->crBackGnd = GetSysColor(COLOR_WINDOW);
        icp->hFont     = (HFONT__ *) GetStockObject(DEFAULT_GUI_FONT);

        // Assign the window text specified in the call to CreateWindow.
        SetWindowText(hwnd, ((CREATESTRUCT *)lParam)->lpszName);

        // Attach custom structure to this window.
        SetInputCustom(hwnd, icp);

		InvalidateRect(icp->hwnd, NULL, FALSE);
		UpdateWindow(icp->hwnd); 
		return true;

        // Continue with window creation.
        return TRUE;

    // Clean up when the window is destroyed.
    case WM_NCDESTROY:
        free(icp);
        break;
	case WM_PAINT:
		return InputCustom_OnPaint(icp,wParam,lParam);
		break;
	case WM_ERASEBKGND:
		return 1;
	case WM_USER+45:
	case WM_KEYDOWN:
		TranslateKey(wParam,temp);
		col = CheckKey(wParam);
		
		icp->crForeGnd = ((~col) & 0x00ffffff);
		icp->crBackGnd = col;
		SetWindowText(hwnd,temp);
		InvalidateRect(icp->hwnd, NULL, FALSE);
		UpdateWindow(icp->hwnd); 
		SendMessage(pappy,WM_USER+43,wParam,(LPARAM)hwnd);
		
		break;
	case WM_USER+44:
		
		TranslateKey(wParam,temp);
		if(IsWindowEnabled(hwnd))
		{
			col = CheckKey(wParam);
		}
		else
		{
			col = RGB( 192,192,192);
		}
		icp->crForeGnd = ((~col) & 0x00ffffff);
		icp->crBackGnd = col;
		SetWindowText(hwnd,temp);
		InvalidateRect(icp->hwnd, NULL, FALSE);
		UpdateWindow(icp->hwnd); 

		break;

	case WM_SETFOCUS:
	{
		col = RGB( 0,255,0);
		icp->crForeGnd = ((~col) & 0x00ffffff);
		icp->crBackGnd = col;
		InvalidateRect(icp->hwnd, NULL, FALSE);
		UpdateWindow(icp->hwnd); 
		tid = wParam;
		

		SetTimer(hwnd,tid,125,NULL);
	

		break;
	}
	case WM_KILLFOCUS:
	{
		/*col = RGB( 255,255,255);
		icp->crForeGnd = ((~col) & 0x00ffffff);
		icp->crBackGnd = col;
		InvalidateRect(icp->hwnd, NULL, FALSE);
		UpdateWindow(icp->hwnd);*/ 
		break;
	}

	case WM_TIMER:
		FunkyJoyStickTimer();
		break;
	case WM_LBUTTONDOWN:
		SetFocus(hwnd);
		break;
	case WM_ENABLE:
		COLORREF col;
		if(wParam)
		{
			col = RGB( 255,255,255);
			icp->crForeGnd = ((~col) & 0x00ffffff);
			icp->crBackGnd = col;
		}
		else
		{
			col = RGB( 192,192,192);
			icp->crForeGnd = ((~col) & 0x00ffffff);
			icp->crBackGnd = col;
		}
		InvalidateRect(icp->hwnd, NULL, FALSE);
		UpdateWindow(icp->hwnd); 
		return true;
    default:
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}