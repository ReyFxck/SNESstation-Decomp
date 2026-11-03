//---------------------------------------------------------------------------
#ifndef UfrmOpenROMH
#define UfrmOpenROMH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <SysUtils.hpp>
#include <registry.hpp>

#include <io.h>
#include <stdio.h>
#include <Buttons.hpp>

#include "..\\snes9x\\snes9x.h"
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class TfrmOpenROM : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TSplitter *Splitter1;
    TPanel *Panel4;
    TTreeView *DirectoryTreeView;
    TListView *ListView;
        TButton *btn_Open;
        TButton *SpeedButton2;
    TComboBox *cbx_MemMap;
    TComboBox *cbx_Interleave;
    TComboBox *cbx_System;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TComboBox *cbx_Header;
    
    TImageList *ImageList;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall btn_OpenClick(TObject *Sender);
    void __fastcall SpeedButton2Click(TObject *Sender);
    
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall ListViewDblClick(TObject *Sender);
    void __fastcall ListViewKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall ListViewEnter(TObject *Sender);
    void __fastcall DirectoryTreeViewChange(TObject *Sender,
          TTreeNode *Node);
    void __fastcall DirectoryTreeViewDeletion(TObject *Sender,
          TTreeNode *Node);
    void __fastcall DirectoryTreeViewExpanding(TObject *Sender,
          TTreeNode *Node, bool &AllowExpansion);
    void __fastcall CompareListItems(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
    void __fastcall SortColumn(TObject *Sender, TListColumn *Column);
private:	// User declarations
    void __fastcall IdleHandler( TObject *Sender, bool &Done);
    int  sort_column;
public:		// User declarations
    __fastcall TfrmOpenROM(TComponent* Owner);
};

void __fastcall FindDirectory( TTreeView* TV, System::String P);
//---------------------------------------------------------------------------
extern PACKAGE TfrmOpenROM *frmOpenROM;
//---------------------------------------------------------------------------
#endif
 
