//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UfrmOpenROM.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmOpenROM *frmOpenROM;
//---------------------------------------------------------------------------
__fastcall TfrmOpenROM::TfrmOpenROM(TComponent* Owner)
    : TForm(Owner)
{
    sort_column = 0;
}
//---------------------------------------------------------------------------
#define ROM_NAME_LEN 23
static BOOL AllASCII (char *b, int size)
{
    for (int i = 0; i < size; i++)
    {
	if (b[i] < 32 || b[i] > 126)
	    return (FALSE);
    }
    return (TRUE);
}

static int ScoreHiROM ( char *Buffer)
{
    int score = 0;
    if ((Buffer [0xffdc] + (Buffer [0xffdd] << 8) +
	 Buffer [0xffde] + (Buffer [0xffdf] << 8)) == 0xffff)
	score += 2;

    if (Buffer [0xffda] == 0x33)
	score += 2;
    if ((Buffer [0xffd5] & 0xf) < 4)
	score += 2;
    if (!(Buffer [0xfffd] & 0x80))
	score -= 4;
    if ((1 << (Buffer [0xffd7] - 7)) > 48)
	score -= 1;
    if (!AllASCII (Buffer+0xffb0, 6))
	score -= 1;
    if (!AllASCII (Buffer+0xffc0, ROM_NAME_LEN - 1))
	score -= 1;

    return (score);
}

static int ScoreLoROM ( char *Buffer)
{
    int score = 0;

    if ((Buffer [0x7fdc] + (Buffer [0x7fdd] << 8) +
	 Buffer [0x7fde] + (Buffer [0x7fdf] << 8)) == 0xffff)
	score += 2;

    if (Buffer [0x7fda] == 0x33)
	score += 2;
    if ((Buffer [0x7fd5] & 0xf) < 4)
	score += 2;
    if (!(Buffer [0x7ffd] & 0x80))
	score -= 4;
    if ((1 << (Buffer [0x7fd7] - 7)) > 48)
	score -= 1;
    if (!AllASCII (Buffer+0x7fb0, 6))
	score -= 1;
    if (!AllASCII (Buffer+0x7fc0, ROM_NAME_LEN - 1))
	score -= 1;

    return (score);
}

char *Safe (const char *s)
{
    static char *safe = NULL;
    static int safe_len = 0;

    int len = strlen (s);
    if (!safe || len + 1> safe_len)
    {
	delete safe;
	safe = new char [safe_len = len + 1];
    }

    for (int i = 0; i < len; i++)
    {
	if (s [i] >= 32 && s [i] < 127 && s [i] != '%' && s [i] != '\\')
	    safe [i] = s[i];
	else
	    safe [i] = ' ';
    }
    safe [len] = 0;
    return (safe);
}

void __fastcall TfrmOpenROM::IdleHandler( TObject *Sender, bool &Done)
{
TListItem *pItem;
char *Buffer;
FILE *ROM;
long int Size;
long double X;
System::String File;

    pItem = NULL;
    if( ListView -> Selected != NULL)
    if( ListView -> Selected -> Data == (void *)1)
    {   pItem = ListView -> Selected;   }

    if( pItem == NULL)
    {
    int C;
        for( C = 0; C != ListView -> Items -> Count; C ++)
        if( ListView -> Items -> Item[C] -> Data == (void *)1)
        {
            pItem = ListView -> Items -> Item[C];
            break;
        }
    }

    if( pItem == NULL)
        return;
    pItem -> Data = NULL;

    File = "";
    if( DirectoryTreeView -> Selected != NULL)
      File = File + ((char *)(int(DirectoryTreeView -> Selected -> Data) + 1)) + pItem -> Caption;

    System::String ext = ExtractFileExt (File);
    ext = ext.UpperCase ();
    if( CompareStr( ext, ".ZIP") == 0)
    {
        pItem -> SubItems -> Add( "Not available");
        pItem -> SubItems -> Add( "N/A Mbit");
        return;
    }

    Buffer = new char[128*1024];
    ZeroMemory( Buffer, 128*1024);

    ROM = fopen( File.c_str(), "rb");
    if( ROM == NULL)
    {
        delete Buffer;
        return;
    }
    Size = filelength( fileno( ROM));
    fread( Buffer, 1, Size - ((int (Size/0x2000))*0x2000), ROM);
    fread( Buffer, 128, 1024, ROM);
    fclose( ROM);

    if( (CompareStr( ExtractFileExt( File), ".078") == 0 ||
         CompareStr( ExtractFileExt( File), ".058") == 0 ||
         CompareStr( ExtractFileExt( File), ".048") == 0) &&
        (File.Pos( "A.") != 0 || File.Pos( "a.") != 0))
    {
    int Pos;

        Pos = File.Pos( "A.");
        if( Pos == 0)
            Pos = File.Pos( "a.");

        while(true)
        {
            File[ Pos]++;

            ROM = fopen( File.c_str(), "rb");
            
            if( ROM == NULL)
            {   break;  }

            Size += filelength( fileno( ROM));
            fclose( ROM);
        }
    }

    if( CompareStr( ExtractFileExt( File), ".1") == 0)
    {
    int Pos;

        Pos = File.Pos( ".1");
        while(true)
        {
            File[ Pos+1]++;

            ROM = fopen( File.c_str(), "rb");
            
            if( ROM == NULL)
            {   break;  }

            Size += filelength( fileno( ROM));
            fclose( ROM);
        }
    }

    if( (ScoreLoROM( Buffer) > ScoreHiROM( Buffer)) || Size < 128*1024)
    {
        Buffer [0x7fc0 + ROM_NAME_LEN - 2] = '\0';
        pItem -> SubItems -> Add( Safe( &Buffer [0x7fc0]));
    }
    else
    {
        Buffer [0xffc0 + ROM_NAME_LEN - 2] = '\0';
        pItem -> SubItems -> Add( Safe( &Buffer [0xffc0]));
    }

    X = (int (Size/0x2000))*0x2000;
    X /= 1024*1024;
    X *= 8;
    pItem -> SubItems -> Add( FormatFloat( "0.00 Mbit", X));

    delete  Buffer;

    Done = false;
}
//---------------------------------------------------------------------------
void __fastcall ExpandLevel( TTreeView* TV, TTreeNode *N, System::String P)
{
  if( N -> Data == NULL)
  {
    N -> Data = new char[P.Length()+2];
    ((char *)(N -> Data))[0] = 0;
    StrPCopy( ((char *)(int(N -> Data) + 1)), P);
  }

  P = ((char *)(int(N -> Data) + 1));

  if( ((char *)(N -> Data))[0] == 0)
  {
    TStringList *D = new TStringList();

    TSearchRec F;
    int R = FindFirst( P + "*.*", faDirectory, F);

    while( R == 0)
    {
      if( F.Attr&faDirectory)
      if( F.Name != ".")
      if( F.Name != "..")
        D -> Add( F.Name);

      R = FindNext( F);
    }
    FindClose( F);

    D -> Sorted = true;
    for( int c = 0; c < D -> Count; c = c + 1)
    {
    TTreeNode *CN;
      if( N -> HasChildren)
      { CN = TV -> Items -> AddChild( N, D -> Strings[c]); }
      else
      { CN = TV -> Items -> AddChildFirst( N, D -> Strings[c]); }

      System::String S;

      S = P + D -> Strings[c] + '\\';

      CN -> Data = new char[S.Length()+2];
      ((char *)(CN -> Data))[0] = 0;
      StrPCopy( ((char *)(int(CN -> Data) + 1)), S);

      CN -> ImageIndex = 4;
      CN -> SelectedIndex = 5;
    }
    ((char *)(N -> Data))[0] = 1;

    if( N -> Level == 0)
    {
      switch( GetDriveType( ((char *)(int(N -> Data) + 1))))
      {
        case DRIVE_UNKNOWN:
        case DRIVE_NO_ROOT_DIR:
        case DRIVE_RAMDISK:
        case DRIVE_FIXED:
          N -> ImageIndex = 1;
          break;

        case DRIVE_REMOVABLE:
          N -> ImageIndex = 0;
          break;

        case DRIVE_REMOTE:
          N -> ImageIndex = 2;
          break;

        case DRIVE_CDROM:
          N -> ImageIndex = 3;
          break;
      }
      N -> SelectedIndex = N -> ImageIndex;
    }
    delete D;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmOpenROM::FormCreate(TObject *Sender)
{
TRegistry *Registry;
DWORD D, H;
char R[4], V[512];
System::String S, C;


    DirectoryTreeView -> Items -> Clear();

    D = GetLogicalDrives();

    // Skip A: & B:
    D = D >> 2;

    for( int i = 3; i != 32; i = i + 1)
    {
      if( D&1)
      {
        StrPCopy( R, char('A'+i-1)+":\\");

        V[0] = 0;

        GetVolumeInformation( R, V, 512, NULL, &H, &H, NULL, 0);

        // Get all the subdirs in the root of each drive

        C = "";
        C = C + char('A'+i-1)+":\\";

        S = V;
        S = S + "("+char('A'+i-1)+":)";

        ExpandLevel( DirectoryTreeView, DirectoryTreeView -> Items -> Add( NULL, S), C);
      }
      D = D >> 1;
    }

    Registry = new TRegistry();
    if( Registry -> OpenKey( "\\Software\\Emulators\\Snes9X\\1.x", true))
    if( Registry -> ValueExists( "Last Directory"))
        FindDirectory( DirectoryTreeView, Registry -> ReadString( "Last Directory"));
    delete Registry;

    Application->OnIdle = IdleHandler;
}
//---------------------------------------------------------------------------
void __fastcall TfrmOpenROM::btn_OpenClick(TObject *Sender)
{
TRegistry *Registry;
    Application->OnIdle = NULL;

    Registry = new TRegistry();
    if( Registry -> OpenKey( "\\Software\\Emulators\\Snes9X\\1.x", true))
    if( DirectoryTreeView -> Selected != NULL)
      Registry -> WriteString( "Last Directory", ((char *)(int(DirectoryTreeView -> Selected -> Data) + 1)));
    delete Registry;

    ModalResult = 1;
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenROM::SpeedButton2Click(TObject *Sender)
{
    ModalResult = 2;
}
//---------------------------------------------------------------------------


void __fastcall TfrmOpenROM::FormDestroy(TObject *Sender)
{
    Application->OnIdle = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenROM::ListViewDblClick(TObject *Sender)
{
    if( ListView -> Selected != NULL)
        btn_Open -> OnClick( Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmOpenROM::ListViewKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if( Key == VK_RETURN)
    ListViewDblClick( Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmOpenROM::ListViewEnter(TObject *Sender)
{
  if( ListView -> Items -> Count > 0)
  if( ListView -> Selected == NULL)
    ListView -> Selected = ListView -> Items -> Item[0];
}
//---------------------------------------------------------------------------
TStringList * __fastcall SplitDirectory( System::String Directory)
{
  TStringList *Result = new TStringList();

  for( int x = 1; x != Directory.Length(); x = x + 1)
  if( (Directory[x] == '\\') || (Directory[x] == '/') )
  {
    if( Directory[x] == '/')
      Directory[x] = '\\';

    Result -> Add( Directory.SubString( 1, x));
  }

  return Result;
}
//---------------------------------------------------------------------------
void __fastcall TfrmOpenROM::DirectoryTreeViewChange(TObject *Sender,
      TTreeNode *Node)
{
TSearchRec F;
int R;
System::String Ext, Dir;
TListItem *pItem;

    if( ListView == NULL)
        return;

    ListView->Items->BeginUpdate ();

    ListView -> Items -> Clear();
    ListView -> SortType = stNone;

    Dir = "";

    if( DirectoryTreeView -> Selected != NULL)
      Dir = Dir + ((char *)(int(DirectoryTreeView -> Selected -> Data) + 1));


    if( DirectoryTreeView -> Selected != NULL)
      R = FindFirst( Dir+"*.*", faAnyFile, F);
    else
      R = -1;
    while( R == 0)
    {
        Ext = ExtractFileExt( F.Name);
        Ext.UpperCase();
        Ext = UpperCase( Ext);

        if( CompareStr( Ext, ".SMC") == 0 || CompareStr( Ext, ".SWC") == 0 ||
            CompareStr( Ext, ".FIG") == 0 || CompareStr( Ext, ".USA") == 0 ||
            CompareStr( Ext, ".JAP") == 0 || CompareStr( Ext, ".1") == 0 ||
            CompareStr( Ext, ".078") == 0 || CompareStr( Ext, ".058") == 0 ||
            CompareStr( Ext, ".048") == 0 || CompareStr( Ext, ".ZIP") == 0 ||
	    CompareStr( Ext, ".SFC") == 0)
        {
            if( (CompareStr( Ext, ".078") == 0 || 
                 CompareStr( Ext, ".058") == 0 ||
                 CompareStr( Ext, ".048") == 0) &&
                (F.Name.Pos( "B.") != 0 || F.Name.Pos( "b.") != 0 ||
                 F.Name.Pos( "C.") != 0 || F.Name.Pos( "c.") != 0 ||
                 F.Name.Pos( "D.") != 0 || F.Name.Pos( "d.") != 0 ||
                 F.Name.Pos( "E.") != 0 || F.Name.Pos( "e.") != 0 ||
                 F.Name.Pos( "F.") != 0 || F.Name.Pos( "f.") != 0 ))
            {
                R = FindNext( F);
                continue;
            }

            pItem = ListView -> Items -> Add();
            pItem -> Caption = ExtractFileName( F.Name);
            pItem -> Data = (void *)1;
        }

        R = FindNext( F);
    }
    FindClose( F);
    ListView -> SortType = stText;
    ListView->Items->EndUpdate ();
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenROM::DirectoryTreeViewDeletion(TObject *Sender,
      TTreeNode *Node)
{
  delete Node -> Data;
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenROM::DirectoryTreeViewExpanding(TObject *Sender,
      TTreeNode *Node, bool &AllowExpansion)
{
  for( int C = 0; C < Node -> Count; C = C + 1)
    ExpandLevel( DirectoryTreeView, Node -> Item[C], "");
}
//---------------------------------------------------------------------------
void __fastcall FindDirectory( TTreeView* TV, System::String P)
{
  if( P == "")
    return;

  if( TV -> Items -> Count == 0)
    return;

  if( (P[ P.Length()-1] != '\\') && (P[ P.Length()-1] != '/') )
     P = P + '\\';

  TStringList *D;

  D = SplitDirectory( P);

  for( int i = 0; i < D -> Count; i = i + 1)
  {
    TTreeNode *R;

    for( int c = 0; c < TV -> Items -> Count; c = c + 1)
    {
      System::String S;
      R = NULL;

      S = ((char *)(int(TV -> Items -> Item[c] -> Data) + 1));
      if( S.UpperCase() == D -> Strings[i].UpperCase())
      {
        ExpandLevel( TV, TV -> Items -> Item[c], "");
        R = TV -> Items -> Item[c];
        break;
      }
    }

    if( R == NULL)
      return;

    TV -> Selected = R;
  }

  delete D;
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenROM::CompareListItems(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
{
    if (sort_column == 0)
        Compare = stricmp (Item1->Caption.c_str (), Item2->Caption.c_str ());
    else
    if (sort_column == 1)
        Compare = stricmp (Item1->SubItems->Strings [0].c_str (),
                           Item2->SubItems->Strings [0].c_str ());
    else
        Compare = stricmp (Item1->SubItems->Strings [1].c_str (),
                           Item2->SubItems->Strings [1].c_str ());
}
//---------------------------------------------------------------------------

void __fastcall TfrmOpenROM::SortColumn(TObject *Sender,
      TListColumn *Column)
{
    // Sort column
    for (sort_column = 0; sort_column < 2; sort_column++)
        if (Column == ListView->Column [sort_column])
            break;

    ListView->Items->BeginUpdate ();
    ListView->SortType = stNone;
    ListView->SortType = stText;
    ListView->Items->EndUpdate ();
}
//---------------------------------------------------------------------------

