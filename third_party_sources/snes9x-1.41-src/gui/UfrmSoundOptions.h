//---------------------------------------------------------------------------
#ifndef UfrmSoundOptionsH
#define UfrmSoundOptionsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "../snes9x/snes9x.h"
//---------------------------------------------------------------------------
class TfrmSoundOptions : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TButton *btnCancel;
    TButton *btnOK;
    TGroupBox *GroupBox1;
    TGroupBox *GroupBox2;
    TComboBox *playback_rate_combo;
    TLabel *playback_rate_label;
    TCheckBox *sixteen_bit_check;
    TCheckBox *stereo_check;
    TCheckBox *reverse_stereo_check;
    TComboBox *buffer_len_combo;
    TLabel *buffer_len_label;
    TCheckBox *envx_check;
    TCheckBox *echo_check;
    TCheckBox *cache_check;
    TCheckBox *master_volume_check;
    TCheckBox *linear_interpolate_check;
    TCheckBox *sync_sound_check;
    TCheckBox *mute_check;
    TCheckBox *enable_spc700_check;
    TComboBox *cpu_skip_combo;
    TLabel *cpu_skip_label;
    TCheckBox *alt_sample_decode_check;
    TLabel *sound_driver_label;
    TComboBox *sound_driver_combo;
    TLabel *mix_interval_label;
    TComboBox *mix_interval_combo;
    void __fastcall EnableSPC700(TObject *Sender);
    void __fastcall Stereo(TObject *Sender);
    void __fastcall OK(TObject *Sender);
    void __fastcall PlaybackRateChanged(TObject *Sender);
    void __fastcall EnableAltSampleDecode(TObject *Sender);
    void __fastcall SoundDriverChanged(TObject *Sender);
private:	// User declarations
    struct SSettings *settings;
    void __fastcall EnableControls ();
public:		// User declarations
    __fastcall TfrmSoundOptions(TComponent* Owner, struct SSettings *settings);
    void __fastcall init ();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSoundOptions *frmSoundOptions;
//---------------------------------------------------------------------------
#endif
