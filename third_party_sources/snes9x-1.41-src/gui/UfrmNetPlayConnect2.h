//---------------------------------------------------------------------------
#ifndef UfrmNetPlayConnect2H
#define UfrmNetPlayConnect2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
#include <ExtCtrls.hpp>
#include <Registry.hpp>
//---------------------------------------------------------------------------
class TfrmNetPlayConnect2 : public TForm
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
        void __fastcall HostnameEnter(TObject *Sender);
        void __fastcall HostnameExit(TObject *Sender);
        void __fastcall ClearHistory(TObject *Sender);
        void __fastcall OK(TObject *Sender);
        void __fastcall NowVisible(TObject *Sender);
private:	// User declarations
    enum { NETPLAY_HISTORY_SIZE = 10 };
    TRegistry *reg;
    System::AnsiString history [NETPLAY_HISTORY_SIZE];
    int hist_size;
    
public:		// User declarations
    __fastcall TfrmNetPlayConnect2(TComponent* Owner, int port);
    __fastcall ~TfrmNetPlayConnect2();
    void __fastcall init (int _port);

    int port;
    System::AnsiString value;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmNetPlayConnect2 *frmNetPlayConnect2;
//---------------------------------------------------------------------------
#endif


