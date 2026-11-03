//---------------------------------------------------------------------------
#ifndef UfrmNetPlayConnectH
#define UfrmNetPlayConnectH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include <Registry.hpp>
//---------------------------------------------------------------------------
class TfrmNetPlayConnect : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TButton *cancel_button;
    TButton *ok_button;
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TButton *clear_history_button;
    TMaskEdit *server_port_text;
        TComboBox *history_combo;
    void __fastcall OK(TObject *Sender);
    void __fastcall ClearHistory(TObject *Sender);
    
    void __fastcall NowVisible(TObject *Sender);
    void __fastcall HostnameEnter(TObject *Sender);
    void __fastcall HostnameExit(TObject *Sender);
private:	// User declarations
    enum { NETPLAY_HISTORY_SIZE = 10 };
    TRegistry *reg;
    System::AnsiString history [NETPLAY_HISTORY_SIZE];
    int hist_size;
    
public:		// User declarations
    __fastcall TfrmNetPlayConnect(TComponent* Owner, int port);
    __fastcall ~TfrmNetPlayConnect();
    void __fastcall init (int _port);

    int port;
    System::AnsiString value;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmNetPlayConnect *frmNetPlayConnect;
//---------------------------------------------------------------------------
#endif
