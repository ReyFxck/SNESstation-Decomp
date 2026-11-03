/*******************************************************************************
  Snes9x - Portable Super Nintendo Entertainment System (TM) emulator.
 
  (c) Copyright 1996 - 2002 Gary Henderson (gary.henderson@ntlworld.com) and
                            Jerremy Koot (jkoot@snes9x.com)

  (c) Copyright 2001 - 2004 John Weidman (jweidman@slip.net)

  (c) Copyright 2002 - 2004 Brad Jorsch (anomie@users.sourceforge.net),
                            funkyass (funkyass@spam.shaw.ca),
                            Joel Yliluoma (http://iki.fi/bisqwit/)
                            Kris Bleakley (codeviolation@hotmail.com),
                            Matthew Kendora,
                            Nach (n-a-c-h@users.sourceforge.net),
                            Peter Bortas (peter@bortas.org) and
                            zones (kasumitokoduck@yahoo.com)

  C4 x86 assembler and some C emulation code
  (c) Copyright 2000 - 2003 zsKnight (zsknight@zsnes.com),
                            _Demo_ (_demo_@zsnes.com), and Nach

  C4 C++ code
  (c) Copyright 2003 Brad Jorsch

  DSP-1 emulator code
  (c) Copyright 1998 - 2004 Ivar (ivar@snes9x.com), _Demo_, Gary Henderson,
                            John Weidman, neviksti (neviksti@hotmail.com),
                            Kris Bleakley, Andreas Naive

  DSP-2 emulator code
  (c) Copyright 2003 Kris Bleakley, John Weidman, neviksti, Matthew Kendora, and
                     Lord Nightmare (lord_nightmare@users.sourceforge.net

  OBC1 emulator code
  (c) Copyright 2001 - 2004 zsKnight, pagefault (pagefault@zsnes.com) and
                            Kris Bleakley
  Ported from x86 assembler to C by sanmaiwashi

  SPC7110 and RTC C++ emulator code
  (c) Copyright 2002 Matthew Kendora with research by
                     zsKnight, John Weidman, and Dark Force

  S-DD1 C emulator code
  (c) Copyright 2003 Brad Jorsch with research by
                     Andreas Naive and John Weidman
 
  S-RTC C emulator code
  (c) Copyright 2001 John Weidman
  
  ST010 C++ emulator code
  (c) Copyright 2003 Feather, Kris Bleakley, John Weidman and Matthew Kendora

  Super FX x86 assembler emulator code 
  (c) Copyright 1998 - 2003 zsKnight, _Demo_, and pagefault 

  Super FX C emulator code 
  (c) Copyright 1997 - 1999 Ivar, Gary Henderson and John Weidman


  SH assembler code partly based on x86 assembler code
  (c) Copyright 2002 - 2004 Marcus Comstedt (marcus@mc.pp.se) 

  Win32 AVI Output module
  (c) 2004 blip
  
  Specific ports contains the works of other authors. See headers in
  individual files.
 
  Snes9x homepage: http://www.snes9x.com
 
  Permission to use, copy, modify and distribute Snes9x in both binary and
  source form, for non-commercial purposes, is hereby granted without fee,
  providing that this license information and copyright notice appear with
  all copies and any derived work.
 
  This software is provided 'as-is', without any express or implied
  warranty. In no event shall the authors be held liable for any damages
  arising from the use of this software.
 
  Snes9x is freeware for PERSONAL USE only. Commercial users should
  seek permission of the copyright holders first. Commercial use includes
  charging money for Snes9x or software derived from Snes9x.
 
  The copyright holders request that bug fixes and improvements to the code
  should be forwarded to them so everyone can benefit from the modifications
  in future versions.
 
  Super NES and Super Nintendo Entertainment System are trademarks of
  Nintendo Co., Limited and its subsidiary companies.
*******************************************************************************/

#include <windows.h>
#include <vfw.h>
#include "wsnes9x.h"			// for GUI.hWnd
#include "AVIOutput.h"

#define VIDEO_STREAM	0
#define AUDIO_STREAM	1

struct AVIFile
{
	bool				valid;
	int					fps;
	int					frameskip;

	bool				video_added;
	BITMAPINFOHEADER	bitmap_format;

	bool				sound_added;
	WAVEFORMATEX		wave_format;

	AVISTREAMINFO		avi_video_header;
	AVISTREAMINFO		avi_sound_header;
	PAVIFILE			avi_file;
	PAVISTREAM			streams[2];
	PAVISTREAM			compressed_streams[2];

	AVICOMPRESSOPTIONS	compress_options[2];
	AVICOMPRESSOPTIONS*	compress_options_ptr[2];

	int					video_frames;
	int					sound_samples;
};

static bool truncate_existing(const char* filename)
{
	// this is only here because AVIFileOpen doesn't seem to do it for us
	FILE* fd = fopen(filename, "wb");
	if(fd)
	{
		fclose(fd);
		return true;
	}

	return false;
}

static void clean_up(AVIFile* _avi)
{
	AVIFile& avi = *_avi;

	if(avi.sound_added)
	{
		if(avi.compressed_streams[AUDIO_STREAM])
		{
			AVIStreamClose(avi.compressed_streams[AUDIO_STREAM]);
			avi.compressed_streams[AUDIO_STREAM] = NULL;
			avi.streams[AUDIO_STREAM] = NULL;				// compressed_streams[AUDIO_STREAM] is just a copy of streams[AUDIO_STREAM]
		}
	}

	if(avi.video_added)
	{
		if(avi.compressed_streams[VIDEO_STREAM])
		{
			AVIStreamClose(avi.compressed_streams[VIDEO_STREAM]);
			avi.compressed_streams[VIDEO_STREAM] = NULL;
		}

		if(avi.streams[VIDEO_STREAM])
		{
			AVIStreamClose(avi.streams[VIDEO_STREAM]);
			avi.streams[VIDEO_STREAM] = NULL;
		}
	}

	if(avi.avi_file)
	{
		AVIFileClose(avi.avi_file);
		avi.avi_file = NULL;
	}
}

#ifdef __cplusplus
extern "C" {
#endif

void AVICreate(struct AVIFile** avi_out)
{
	*avi_out = new AVIFile;
	memset(*avi_out, 0, sizeof(AVIFile));
	AVIFileInit();
}

void AVIClose(struct AVIFile** avi_out)
{
	if(*avi_out)
	{
		clean_up(*avi_out);
		delete *avi_out;
	}
	*avi_out = NULL;
	AVIFileExit();
}

void AVISetFramerate(const int fps, const int frameskip, struct AVIFile* avi_out)
{
	(*avi_out).fps = fps;
	(*avi_out).frameskip = frameskip;
}

void AVISetVideoFormat(const BITMAPINFOHEADER* bitmap_format, struct AVIFile* avi_out)
{
	memcpy(&((*avi_out).bitmap_format), bitmap_format, sizeof(BITMAPINFOHEADER));
	(*avi_out).video_added = true;
}

void AVISetSoundFormat(const WAVEFORMATEX* wave_format, struct AVIFile* avi_out)
{
	memcpy(&((*avi_out).wave_format), wave_format, sizeof(WAVEFORMATEX));
	(*avi_out).sound_added = true;
}

int AVIBegin(const char* filename, struct AVIFile* _avi_out)
{
	AVIFile& avi = *_avi_out;
	int result = 0;

	do
	{
		if(!avi.video_added)
			break;

		if(!truncate_existing(filename))
			break;

		// open the file
		if(FAILED(AVIFileOpen(&avi.avi_file, filename, OF_CREATE | OF_WRITE, NULL)))
			break;

		// create the video stream
		memset(&avi.avi_video_header, 0, sizeof(AVISTREAMINFO));
		avi.avi_video_header.fccType = streamtypeVIDEO;
		avi.avi_video_header.dwScale = avi.frameskip;
		avi.avi_video_header.dwRate = avi.fps;
		avi.avi_video_header.dwSuggestedBufferSize = avi.bitmap_format.biSizeImage;
		if(FAILED(AVIFileCreateStream(avi.avi_file, &avi.streams[VIDEO_STREAM], &avi.avi_video_header)))
			break;

		// get compression options
		memset(&avi.compress_options[VIDEO_STREAM], 0, sizeof(AVICOMPRESSOPTIONS));
		avi.compress_options_ptr[VIDEO_STREAM] = &avi.compress_options[0];
		if(!AVISaveOptions(GUI.hWnd, 0, 1, &avi.streams[VIDEO_STREAM], &avi.compress_options_ptr[VIDEO_STREAM]))
			break;

		// create compressed stream
		if(FAILED(AVIMakeCompressedStream(&avi.compressed_streams[VIDEO_STREAM], avi.streams[VIDEO_STREAM], &avi.compress_options[VIDEO_STREAM], NULL)))
			break;

		// set the stream format
		if(FAILED(AVIStreamSetFormat(avi.compressed_streams[VIDEO_STREAM], 0, (void*)&avi.bitmap_format, avi.bitmap_format.biSize)))
			break;

		// add sound (if requested)
		if(avi.sound_added)
		{
			// create the audio stream
			memset(&avi.avi_sound_header, 0, sizeof(AVISTREAMINFO));
			avi.avi_sound_header.fccType = streamtypeAUDIO;
			avi.avi_sound_header.dwQuality = (DWORD)-1;
			avi.avi_sound_header.dwScale = avi.wave_format.nBlockAlign;
			avi.avi_sound_header.dwRate = avi.wave_format.nAvgBytesPerSec;
			avi.avi_sound_header.dwSampleSize = avi.wave_format.nBlockAlign;
			avi.avi_sound_header.dwInitialFrames = 1;
			if(FAILED(AVIFileCreateStream(avi.avi_file, &avi.streams[AUDIO_STREAM], &avi.avi_sound_header)))
				break;

			// AVISaveOptions doesn't seem to work for audio streams
			// so here we just copy the pointer for the compressed stream
			avi.compressed_streams[AUDIO_STREAM] = avi.streams[AUDIO_STREAM];

			// set the stream format
			if(FAILED(AVIStreamSetFormat(avi.compressed_streams[AUDIO_STREAM], 0, (void*)&avi.wave_format, sizeof(WAVEFORMATEX))))
				break;
		}

		// initialize counters
		avi.video_frames = 0;
		avi.sound_samples = 0;

		// success
		result = 1;
		avi.valid = true;

	} while(false);

	if(!result)
	{
		clean_up(&avi);
		avi.valid = false;
	}

	return result;
}

int AVIGetVideoFormat(const struct AVIFile* avi_out, const BITMAPINFOHEADER** ppFormat)
{
	if(!(*avi_out).valid ||
		!(*avi_out).video_added)
	{
		if(ppFormat)
		{
			*ppFormat = NULL;
		}
		return 0;
	}
	if(ppFormat)
	{
		*ppFormat = &(*avi_out).bitmap_format;
	}
	return 1;
}

int AVIGetSoundFormat(const struct AVIFile* avi_out, const WAVEFORMATEX** ppFormat)
{
	if(!(*avi_out).valid ||
		!(*avi_out).sound_added)
	{
		if(ppFormat)
		{
			*ppFormat = NULL;
		}
		return 0;
	}
	if(ppFormat)
	{
		*ppFormat = &(*avi_out).wave_format;
	}
	return 1;
}

void AVIAddVideoFrame(void* bitmap_data, struct AVIFile* avi_out)
{
	if(!(*avi_out).valid ||
		!(*avi_out).video_added)
	{
		return;
	}

    if(FAILED(AVIStreamWrite((*avi_out).compressed_streams[VIDEO_STREAM],
                             (*avi_out).video_frames,
                             1,
                             bitmap_data,
                             (*avi_out).bitmap_format.biSizeImage,
                             AVIIF_KEYFRAME, NULL, NULL)))
	{
		(*avi_out).valid = false;
		return;
	}

	(*avi_out).video_frames++;
}

void AVIAddSoundSamples(void* sound_data, const int num_samples, struct AVIFile* avi_out)
{
	if(!(*avi_out).valid ||
		!(*avi_out).sound_added)
	{
		return;
	}

	int data_length = num_samples * (*avi_out).wave_format.nBlockAlign;
    if(FAILED(AVIStreamWrite((*avi_out).compressed_streams[AUDIO_STREAM],
                             (*avi_out).sound_samples,
                             num_samples,
                             sound_data,
                             data_length,
                             0, NULL, NULL)))
	{
		(*avi_out).valid = false;
		return;
	}

	(*avi_out).sound_samples += num_samples;
}

#ifdef __cplusplus
}
#endif
