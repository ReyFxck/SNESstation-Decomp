//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UfrmSoundOptions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSoundOptions *frmSoundOptions;
//---------------------------------------------------------------------------
static uint32 PlaybackRates [] = {
    0, 8000, 11025, 16000, 22050, 30000, 35000, 44000, 48000
};

static int32 SoundBufferLengths [] = {
    1, 2, 4, 8, 16, 32, 64
};
#define COUNT(a) (sizeof (a) / sizeof (a[0]))

//---------------------------------------------------------------------------
__fastcall TfrmSoundOptions::TfrmSoundOptions(TComponent* Owner, 
                                              struct SSettings *set)
    : TForm(Owner), settings (set)
{
    init ();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSoundOptions::init ()
{
    sixteen_bit_check->Checked = settings->SixteenBitSound;
    stereo_check->Checked = settings->Stereo;
    reverse_stereo_check->Checked = settings->ReverseStereo;
    linear_interpolate_check->Checked = settings->InterpolatedSound;
    sync_sound_check->Checked = settings->SoundSync;
    
    envx_check->Checked = settings->SoundEnvelopeHeightReading;
    echo_check->Checked = !settings->DisableSoundEcho;
    cache_check->Checked = !settings->DisableSampleCaching;
    master_volume_check->Checked = !settings->DisableMasterVolume;
    mute_check->Checked = settings->Mute;
    alt_sample_decode_check->Checked = settings->AltSampleDecode;
    enable_spc700_check->Checked = settings->NextAPUEnabled;
    cpu_skip_combo->ItemIndex = settings->SoundSkipMethod;

    playback_rate_combo->ItemIndex = 3;

    int i;
    for (i = 0; i < COUNT(PlaybackRates); i++)
    {
        if (PlaybackRates [i] == settings->SoundPlaybackRate)
            playback_rate_combo->ItemIndex = i;
    }

    buffer_len_combo->ItemIndex = 0;
    for (i = 0; i < COUNT(SoundBufferLengths); i++)
    {
        if (SoundBufferLengths [i] == settings->SoundBufferSize)
            buffer_len_combo->ItemIndex = i;
    }
    sound_driver_combo->ItemIndex = settings->SoundDriver;
    mix_interval_combo->ItemIndex = (settings->SoundMixInterval / 10) - 1;
    EnableControls ();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSoundOptions::EnableControls ()
{
    bool en = enable_spc700_check->Checked && 
              playback_rate_combo->ItemIndex != 0;

    sound_driver_combo->Enabled = true;
    sound_driver_label->Enabled = true;
    cpu_skip_combo->Enabled = !enable_spc700_check->Checked;
    cpu_skip_label->Enabled = !enable_spc700_check->Checked;
    playback_rate_combo->Enabled = enable_spc700_check->Checked;
    playback_rate_label->Enabled = enable_spc700_check->Checked;
    buffer_len_combo->Enabled = en && 
                                sound_driver_combo->ItemIndex == WIN_SNES9X_DIRECT_SOUND_DRIVER;
    buffer_len_label->Enabled = buffer_len_combo->Enabled;
    mix_interval_combo->Enabled = en && 
                                sound_driver_combo->ItemIndex == WIN_SNES9X_DIRECT_SOUND_DRIVER;
    mix_interval_label->Enabled = mix_interval_combo->Enabled;
    sixteen_bit_check->Enabled = en;
    stereo_check->Enabled = en;
    reverse_stereo_check->Enabled = en && stereo_check->Checked;
    linear_interpolate_check->Enabled = en;
    sync_sound_check->Enabled = en &&
                                sound_driver_combo->ItemIndex == WIN_SNES9X_DIRECT_SOUND_DRIVER;
    
    envx_check->Enabled = en;
    echo_check->Enabled = en;
    cache_check->Enabled = en && !alt_sample_decode_check->Checked;
    master_volume_check->Enabled = en;
    alt_sample_decode_check->Enabled = en;
    mute_check->Enabled = en;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSoundOptions::EnableSPC700(TObject *Sender)
{
    EnableControls ();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSoundOptions::Stereo(TObject *Sender)
{
    EnableControls ();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSoundOptions::OK(TObject *Sender)
{
    settings->SixteenBitSound = sixteen_bit_check->Checked;
    settings->Stereo = stereo_check->Checked;
    settings->ReverseStereo = reverse_stereo_check->Checked;
    settings->InterpolatedSound = linear_interpolate_check->Checked;
    settings->SoundSync = sync_sound_check->Checked;
    
    settings->SoundEnvelopeHeightReading = envx_check->Checked;
    settings->DisableSoundEcho = !echo_check->Checked;
    settings->DisableSampleCaching = !cache_check->Checked;
    settings->DisableMasterVolume = !master_volume_check->Checked;
    settings->Mute = mute_check->Checked;
    settings->AltSampleDecode = alt_sample_decode_check->Checked;
    settings->NextAPUEnabled = enable_spc700_check->Checked;
    settings->SoundSkipMethod = (uint8) cpu_skip_combo->ItemIndex;
    if (playback_rate_combo->ItemIndex >= 0)
        settings->SoundPlaybackRate = PlaybackRates [playback_rate_combo->ItemIndex];
    if (buffer_len_combo->ItemIndex >= 0)
        settings->SoundBufferSize = SoundBufferLengths [buffer_len_combo->ItemIndex];
    if (mix_interval_combo->ItemIndex >= 0)
        settings->SoundMixInterval = mix_interval_combo->ItemIndex * 10 + 10;

    if (sound_driver_combo->ItemIndex >= 0)
        settings->SoundDriver = sound_driver_combo->ItemIndex;
}

//---------------------------------------------------------------------------

void __fastcall TfrmSoundOptions::PlaybackRateChanged(TObject *Sender)
{
    EnableControls ();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSoundOptions::EnableAltSampleDecode(TObject *Sender)
{
    EnableControls ();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSoundOptions::SoundDriverChanged(TObject *Sender)
{
    EnableControls ();
}
//---------------------------------------------------------------------------

