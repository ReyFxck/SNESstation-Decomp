/*******************************************************************************
  Snes9x - Portable Super Nintendo Entertainment System (TM) emulator.
 
  (c) Copyright 1996 - 2003 Gary Henderson (gary.henderson@ntlworld.com) and
                            Jerremy Koot (jkoot@snes9x.com)

  (c) Copyright 2002 - 2003 Matthew Kendora and
                            Brad Jorsch (anomie@users.sourceforge.net)
 

                      
  C4 x86 assembler and some C emulation code
  (c) Copyright 2000 - 2003 zsKnight (zsknight@zsnes.com),
                            _Demo_ (_demo_@zsnes.com), and
                            Nach (n-a-c-h@users.sourceforge.net)
                                          
  C4 C++ code
  (c) Copyright 2003 Brad Jorsch

  DSP-1 emulator code
  (c) Copyright 1998 - 2003 Ivar (ivar@snes9x.com), _Demo_, Gary Henderson,
                            John Weidman (jweidman@slip.net),
                            neviksti (neviksti@hotmail.com), and
                            Kris Bleakley (stinkfish@bigpond.com)
 
  DSP-2 emulator code
  (c) Copyright 2003 Kris Bleakley, John Weidman, neviksti, Matthew Kendora, and
                     Lord Nightmare (lord_nightmare@users.sourceforge.net

  OBC1 emulator code
  (c) Copyright 2001 - 2003 zsKnight, pagefault (pagefault@zsnes.com)
  Ported from x86 assembler to C by sanmaiwashi

  SPC7110 and RTC C++ emulator code
  (c) Copyright 2002 Matthew Kendora with research by
                     zsKnight, John Weidman, and Dark Force

  S-RTC C emulator code
  (c) Copyright 2001 John Weidman
  
  Super FX x86 assembler emulator code 
  (c) Copyright 1998 - 2003 zsKnight, _Demo_, and pagefault 

  Super FX C emulator code 
  (c) Copyright 1997 - 1999 Ivar and Gary Henderson.



 
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


#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "snes9x.h"
#include "memmap.h"
#include "debug.h"
#include "ppu.h"
#include "snapshot.h"
#include "gfx.h"
#include "display.h"
#include "apu.h"

#define NEED_GRAPHTYPE_SYMS

#define uint32 ggi_uint32 // Ugly workarount, ggi's unit32 is int, not long...
#include <ggi/ggi.h>
#undef uint32

static ggi_visual_t vis;
static ggi_directbuffer_t dbuf;
static ggi_pixellinearbuffer *plb = NULL;

static int mouse_x = 0;
static int mouse_y = 0;
static uint32 mouse_buttons = 0;

static bool8 superscope = FALSE;
static uint32 superscope_turbo = 0;
static uint32 superscope_pause = 0;

static ggi_mode mode;
static bool8 double_buffer; /* Use double_buffering, only for 8 bpp and 16 bpp (565). */

/* For double buffering: */
static uint32 start_y; /* Start-line of the visible screen */
static uint32 framebuffer_screen_size;

extern uint32 joypads [5];

static void Convert16to8(int width, int height);
static void Convert16to15(int width, int height);
static void Convert16to16(int width, int height);
static void Convert16to24(int width, int height);
static void Convert16to32(int width, int height);
static void Convert8to8(int width, int height);


void S9xDeinitDisplay ()
{
	ggiClose(vis);
}

static void SetModeType(ggi_graphtype type, bool8 double_height)
{
  mode.frames = 1;
  mode.visible.x = IMAGE_WIDTH;
  mode.visible.y = IMAGE_HEIGHT;
  mode.virt.x = IMAGE_WIDTH;
  mode.virt.y = (double_height)?(IMAGE_HEIGHT*2):(IMAGE_HEIGHT);
  mode.graphtype = type;
  mode.size.x = GGI_AUTO;
  mode.size.y = GGI_AUTO;
  mode.dpp.x = GGI_AUTO;
  mode.dpp.y = GGI_AUTO;
}


static bool8 InitSixteenBit(void)
{
  SetModeType(GT_16BIT, TRUE);
  ggiCheckMode(vis, &mode);
  if (mode.graphtype == GT_16BIT) {
    mode.virt.y = 2*mode.virt.y;
    if (ggiCheckMode(vis, &mode)==0) {
      double_buffer = TRUE;
      return TRUE;
    }
    mode.virt.y = mode.virt.y;
    if (ggiCheckMode(vis, &mode)==0) {
      double_buffer = FALSE;
      return TRUE;
    }
  }
  SetModeType(GT_15BIT, FALSE);
  if (mode.graphtype == GT_15BIT) {
    if (ggiCheckMode(vis, &mode)==0) {
      double_buffer = FALSE;
      return TRUE;
    }
  }
  SetModeType(GT_32BIT, FALSE);
  if (mode.graphtype == GT_32BIT) { 
    if (ggiCheckMode(vis, &mode)==0) {
      double_buffer = FALSE;
      return TRUE;
    }
  }
  SetModeType(GT_24BIT, FALSE);
  if (mode.graphtype == GT_24BIT) {
    if (ggiCheckMode(vis, &mode)==0) {
      double_buffer = FALSE;
      return TRUE;
    }
  }
  
  return FALSE; 
}

static bool8 InitEightBit(void)
{
  SetModeType(GT_8BIT, TRUE);
  ggiCheckMode(vis, &mode);
  if (mode.graphtype == GT_8BIT) {
    mode.virt.y = 2*mode.virt.y;
    if (ggiCheckMode(vis, &mode)==0) {
      double_buffer = TRUE;
      return TRUE;
    }
    mode.virt.y = mode.virt.y;
    if (ggiCheckMode(vis, &mode)==0) {
      double_buffer = FALSE;
      return TRUE;
    }
  }
  return FALSE;
}


void S9xInitDisplay (int, char **)
{

  if (ggiInit() != 0) {
    fprintf(stderr, "unable to initialize libggi, exiting.\n");
    S9xExit ();
  }
  if ((vis=ggiOpen(NULL)) == NULL) {
    fprintf(stderr, "unable to open default visual, exiting.\n");
    S9xExit ();
  }

  if (InitSixteenBit()) {
    Settings.SixteenBit = TRUE;
    if (Settings.ForceNoTransparency) {
      Settings.Transparency = FALSE;
    } else {
      Settings.Transparency = TRUE;
    }
  } else if (InitEightBit()) {
    if (Settings.ForceTransparency) {
      fprintf(stderr, "Transparency not supported in 256 color mode.\n");
      S9xExit();
      Settings.SixteenBit = TRUE;
      Settings.Transparency = TRUE;
    } else {
      Settings.SixteenBit = FALSE;
      Settings.Transparency = FALSE;
    }
  } else {
    fprintf(stderr, "Cannot find suitable graphics mode, exiting.\n");
    S9xExit ();
  }
  
  printf("Opening screen: %dx%d (%dx%d) (%s)\n",
	 mode.visible.x, mode.visible.y,
	 mode.virt.x, mode.virt.y,
	 ggi_graphmask_sym[mode.graphtype+1]);
  
  if (ggiSetMode(vis, &mode) != 0) {
    fprintf(stderr, "Cannot open graphic mode!\n");
    S9xExit ();
  }

  ggiSetFlags(vis, GGIFLAG_ASYNC);

  if ( ggiDBGetBuffer (vis, &dbuf) ) {
    fprintf (stderr, "Error getting display buffer.\n");
    S9xExit ();
  }

  if (ggiDBGetLayout (dbuf) != blPixelLinearBuffer) {
    fprintf (stderr, "Error: nonlinear display buffer.\n");
    S9xExit ();
  }
  
  if ( (plb = ggiDBGetPLB (dbuf)) == NULL) {
    fprintf(stderr, "Error getting pixel linear buffer.\n");
    S9xExit ();
  }

  framebuffer_screen_size = plb->stride * mode.visible.y;

  if (double_buffer) {
    if ( ((mode.graphtype==GT_8BIT) && (plb->setup==sp8a8i8)) ||
	 ((mode.graphtype==GT_16BIT) && (plb->setup==sp16a16r5g6b5)) ) {
      GFX.Screen = (uint8 *) plb->write + framebuffer_screen_size;
      GFX.Pitch =  plb->stride;
      GFX.SubScreen = (uint8 *) malloc (plb->stride * mode.visible.y );
      
      start_y = 0;
      ggiSetOrigin(vis, 0, start_y);
    } else {
      fprintf(stderr, "Wrong buffer type, fallback to non double_buffer\n");
      double_buffer = FALSE;
    }
  }
  if (!double_buffer) {
    GFX.Screen = (uint8 *) malloc (plb->stride * mode.visible.y );
    GFX.Pitch =  plb->stride;
    GFX.SubScreen = (uint8 *) malloc (plb->stride * mode.visible.y );
  }
}

void S9xPutImage (int width, int height)
{
  if (double_buffer) {
    if (start_y == 0) { 
      start_y = mode.visible.y;
      GFX.Screen = (uint8 *) plb->write;
    } else {
      start_y = 0;
      GFX.Screen = (uint8 *) plb->write + framebuffer_screen_size;
    }
    
    GFX.Delta = (GFX.SubScreen - GFX.Screen) >> 1;
    
    ggiSetOrigin(vis, 0, start_y);
  } else {
    if (Settings.SixteenBit) {
      /* 16 bpp source */
      switch (mode.graphtype) {
      case GT_8BIT:
	Convert16to8(width, height);
	break;
      case GT_15BIT:
	Convert16to15(width, height);
	break;
      case GT_16BIT:
	Convert16to16(width, height);
	break;
      case GT_24BIT:
	Convert16to24(width, height);
	break;
      case GT_32BIT:
	Convert16to32(width, height);
	break;
      default:
	break;
      }
    } else {
      /* 8 bpp source */
      switch (mode.graphtype) {
      case GT_8BIT:
	Convert8to8(width, height);
	break;
      default:
	break;
      }
    }
  }
  
  ggiFlush(vis);
}

void S9xSetTitle (const char *string)
{
}

bool8 S9xReadMousePosition (int which1, int &x, int &y, uint32 &buttons)
{
    if (which1 == 0)
    {
      x = mouse_x;
      y = mouse_y;
      buttons = mouse_buttons;
      return (TRUE);
    }
    return (FALSE);
}

bool8 S9xReadSuperScopePosition (int &x, int &y, uint32 &buttons)
{
  x = mouse_x;
  y = mouse_y;
  buttons = (mouse_buttons & 3) | (superscope_turbo << 2) |
    (superscope_pause << 3);
  return (TRUE);
}

void S9xProcessEvents (bool8 block)
{
  ggi_event ev;
  struct timeval tv = {0,0};
  ggi_event_mask mask;
  uint32 keysym;
  
  if ((!block) && ((mask=ggiEventPoll(vis, emAll, &tv))==0)) {
    return;
  }

  do {
    ggiEventRead(vis, &ev, emAll);
    
    uint8 byte1 = 0;
    uint8 byte2 = 0;
    uint8 byte3 = 0;
    uint8 byte4 = 0;
	
    switch(ev.any.type) {
    case evKeyRepeat:
      break;
    case evKeyPress:
    case evKeyRelease:
      keysym = ev.key.sym;
      switch ( keysym) {
      case 'k':
      case GGI_KEY_RIGHT:	byte2 = 1;	break;
      case 'h':
      case GGI_KEY_LEFT:	byte2 = 2;	break;
      case 'j':
      case 'n':
      case GGI_KEY_DOWN:	byte2 = 4;	break;
      case 'u':
      case GGI_KEY_UP:		byte2 = 8;	break;

      case GGI_KEY_RETURN:	byte2 = 16;	break; // Start
      case ' ':	byte2 = 32;	break; // Select

      case '.':
      case 't':
      case 'd':		byte1 = 128;	break; // A

      case '/':
      case 'y':
      case 'c':		byte2 = 128;	break; // B

      case 'm':
      case 'e':
      case 's':		byte1 = 64;	break; // X

      case ',':
      case 'r':
      case 'x':		byte2 = 64;	break; // Y

      case 'v':
      case 'q':
      case 'a':		byte1 = 32;	break; // TL

      case 'b':
      case 'w':
      case 'z':		byte1 = 16;	break; // TR

      case GGI_KEY_P4:	byte4 = 1;	break;
      case GGI_KEY_P6:	byte4 = 2;	break;
      case GGI_KEY_P2:	byte4 = 4;	break;
      case GGI_KEY_P8:	byte4 = 8;	break;
	    
      case GGI_KEY_PENTER:	byte4 = 16;	break; // Start
      case GGI_KEY_PPLUS:     byte4 = 32;	break; // Select
      case GGI_KEY_PGUP:	byte3 = 128;	break; // A
      case GGI_KEY_PGDN:	byte4 = 128;	break; // B
      case GGI_KEY_HOME:	byte3 = 64;	break; // X
      case GGI_KEY_END:	        byte4 = 64;	break; // Y
      case GGI_KEY_INSERT:	byte3 = 32;	break; // TL
      case GGI_KEY_DELETE:	byte3 = 16;	break; // TR

      case 27:	S9xExit ();	break;

      case '0':
	if (ev.any.type == evKeyPress)
	  Settings.DisableHDMA = !Settings.DisableHDMA;
	break;
      case '1':
	if (ev.any.type == evKeyPress)
	  PPU.BG_Forced ^= 1;
	break;
      case '2':
	if (ev.any.type == evKeyPress)
	  PPU.BG_Forced ^= 2;
	break;
      case '3':
	if (ev.any.type == evKeyPress)
	  PPU.BG_Forced ^= 4;
	break;
      case '4':
	if (ev.any.type == evKeyPress)
	  PPU.BG_Forced ^= 8;
	break;
      case '5':
	if (ev.any.type == evKeyPress)
	  PPU.BG_Forced ^= 16;
	break;
      case '6':
	if (ev.any.type == evKeyPress)
	  Settings.SwapJoypads = !Settings.SwapJoypads;
	break;
      case '9':
	if (ev.any.type == evKeyPress)
	  if (Settings.SixteenBit)
	    Settings.Transparency = !Settings.Transparency;
	break;
      case '8':
	if (ev.any.type == evKeyPress)
	  Settings.BGLayering = !Settings.BGLayering;
	break;
      case '7':
	if (ev.any.type == evKeyPress)
	  S9xNextController ();
	break;

      case '-':
	if (ev.any.type == evKeyPress) {
	  if (Settings.SkipFrames <= 1)
	    Settings.SkipFrames = AUTO_FRAMERATE;
	  else
	    if (Settings.SkipFrames != AUTO_FRAMERATE)
	      Settings.SkipFrames--;
	}
	break;

      case '=':
      case '+':
	if (ev.any.type == evKeyPress) {
	  if (Settings.SkipFrames == AUTO_FRAMERATE)
	    Settings.SkipFrames = 1;
	  else
	    if (Settings.SkipFrames < 10)
	      Settings.SkipFrames++;
	}
	break;
	
      case GGI_KEY_BACKSPACE:
	if (ev.any.type == evKeyPress) {
	  Settings.DisableGraphicWindows = !Settings.DisableGraphicWindows;
	}
	break;
	//case GGI_KEY_SCROLL_LOCK:
      case GGI_KEY_BREAK:
	if (ev.any.type == evKeyPress)
	  Settings.Paused ^= 1;
	break;
      case GGI_KEY_TAB:
	if (ev.any.type == evKeyPress)
	  superscope_turbo = !superscope_turbo;
	break;

      case GGI_KEY_DGRAVE: /* XK_grave? what's this for key? (alex) */
	superscope_pause = ev.any.type == evKeyPress;
	break;
      case GGI_KEY_F1:
#ifdef DEBUGGER
	if (ev.any.type == evKeyPress && (ev.key.effect & (1<<mkAlt)))	{
	  CPU.Flags |= DEBUG_MODE_FLAG;
	  break;
	}
#endif
	// Fall...
      case GGI_KEY_F2:
	if (ev.any.type == evKeyPress && (ev.key.effect & (1<<mkAlt)))	{
	  S9xLoadSnapshot (S9xChooseFilename (TRUE));
	  break;
	}
	// Fall...
      case GGI_KEY_F3:
	if (ev.any.type == evKeyPress && (ev.key.effect & (1<<mkAlt)))	{
	  Snapshot (S9xChooseFilename (FALSE));
	  break;
	}
	// Fall...
      case GGI_KEY_F4:
      case GGI_KEY_F5:
      case GGI_KEY_F6:
      case GGI_KEY_F7:
      case GGI_KEY_F8:
      case GGI_KEY_F9:
      case GGI_KEY_F10:
      case GGI_KEY_F11:
      case GGI_KEY_F12:
	if (ev.any.type == evKeyPress)	{
	  if (!(ev.key.effect & ((1<<mkShift) | (1<<mkAlt)))) {
	    if (keysym == GGI_KEY_F11) {
	      S9xLoadSnapshot (S9xChooseFilename (TRUE));
	      break;
	    }  else if (keysym == GGI_KEY_F12) {
	      Snapshot (S9xChooseFilename (FALSE));
	      break;
	    }
	    char def [PATH_MAX];
	    char filename [PATH_MAX];
	    char drive [_MAX_DRIVE];
	    char dir [_MAX_DIR];
	    char ext [_MAX_EXT];
	    
	    _splitpath (Memory.ROMFilename, drive, dir, def, ext);
	    sprintf (filename, "%s%s%s.%03d",
		     S9xGetSnapshotDirectory (), SLASH_STR, def,
		     keysym - GGI_KEY_F1);
	    S9xLoadSnapshot (filename);
	  }  else
	    if (ev.key.effect & (1<<mkAlt)) {
	      if (keysym >= GGI_KEY_F4)
		S9xToggleSoundChannel (keysym - GGI_KEY_F4);
	    } else {
	      char def [PATH_MAX];
	      char filename [PATH_MAX];
	      char drive [_MAX_DRIVE];
	      char dir [_MAX_DIR];
	      char ext [_MAX_EXT];
	      
	      _splitpath (Memory.ROMFilename, drive, dir, def, ext);
	      sprintf (filename, "%s%s%s.%03d",
		       S9xGetSnapshotDirectory (), SLASH_STR, def,
		       keysym - GGI_KEY_F1);
	      Snapshot (filename);
	    }
	}
	break;
      }
      if (ev.any.type == evKeyPress) {
	joypads [0] |= byte1;
	joypads [0] |= (byte2 << 8);
	joypads [1] |= byte3;
	joypads [1] |= (byte4 << 8);
      } else {
	joypads [0] &= ~byte1;
	joypads [0] &= ~(byte2 << 8);
	joypads [1] &= ~byte3;
	joypads [1] &= ~(byte4 << 8);
      }
      break;
    case evPtrRelative:
      mouse_x += ev.pmove.x;
      mouse_y += ev.pmove.y;
      break;
    case evPtrAbsolute:
      mouse_x = ev.pmove.x;
      mouse_y = ev.pmove.y;
      break;
    case evPtrButtonPress:
      mouse_buttons |= ev.pbutton.button;
      break;
    case evPtrButtonRelease:
      mouse_buttons &= ~ev.pbutton.button;
      break;
    case evPtrState:
      /* What kind of event do i get here? */
      /* Check it up, should resynchronize if relative ptr maybe.. */
      break;
   default:
      break;
    }
  }  while (ggiEventPoll(vis, emAll, &tv)!=0);
}

void S9xSetPalette ()
{
  int i;
  uint16 Brightness = (IPPU.MaxBrightness) * 140;
  static ggi_color cmap[256];
	    
  for (i = 0; i < 256; i++) {
    cmap[i].r = ((PPU.CGDATA[i] >> 0) & 0x1F) * Brightness;
    cmap[i].g = ((PPU.CGDATA[i] >> 5) & 0x1F) * Brightness;
    cmap[i].b = ((PPU.CGDATA[i] >> 10) & 0x1F) * Brightness;
  }

  ggiSetPaletteVec(vis, 0, 256, cmap);
  
}

const char *S9xSelectFilename (const char *def, const char *dir1,
			    const char *ext1, const char *title)
{
    static char path [PATH_MAX];
    char buffer [PATH_MAX];
    
    printf ("\n%s (default: %s): ", title, def);
    fflush (stdout);
    if (fgets (buffer, sizeof (buffer) - 1, stdin))
    {
	char *p = buffer;
	while (isspace (*p))
	    p++;
	if (!*p)
	{
	    strcpy (buffer, def);
	    p = buffer;
	}

	char *q = strrchr (p, '\n');
	if (q)
	    *q = 0;

	char fname [PATH_MAX];
	char drive [_MAX_DRIVE];
	char dir [_MAX_DIR];
	char ext [_MAX_EXT];

	_splitpath (p, drive, dir, fname, ext);
	_makepath (path, drive, *dir ? dir : dir1, fname, *ext ? ext : ext1);
	return (path);
    }

    return (NULL);
}

void S9xTextMode ()
{
}

void S9xGraphicsMode ()
{
}

void S9xParseDisplayArg (char **, int &, int)
{
    S9xUsage ();
}

void S9xExtraUsage ()
{
}

int S9xMinCommandLineArgs ()
{
    return (2);
}

void S9xMessage (int type, int number, const char *message)
{
    fprintf (stdout, "%s\n", message);
}

static void Convert16to8(int width, int height)
{
  fprintf(stderr, "16 -> 8 bit conversion not implemented yet.\n");
}

static void Convert16to15(int width, int height)
{
  for (register y = 0; y < SNES_HEIGHT_EXTENDED; y++) {
    register uint16 *d = (uint16 *) ((uint8 *)plb->write + y * plb->stride);
    register uint16 *s = (uint16 *) (GFX.Screen + y * GFX.RealPitch);
    
    for (register x = 0; x < SNES_WIDTH; x++) {
      register uint16 pixel = *s++;
      *d++ = (pixel&0x1F) | ((pixel&0xFFC0)>>1);
    }
  }
}

static void Convert16to16(int width, int height)
{
  for (register y = 0; y < SNES_HEIGHT_EXTENDED; y++) {
    register uint16 *d = (uint16 *) ((uint8 *)plb->write + y * plb->stride);
    register uint16 *s = (uint16 *) (GFX.Screen + y * GFX.RealPitch);
    
    for (register x = 0; x < SNES_WIDTH; x++)
      *d++ = *s++;
  }
}

static void Convert16to24(int width, int height)
{
  for (register y = 0; y < SNES_HEIGHT_EXTENDED; y++) {
    register uint8 *d = (uint8 *) ((uint8 *)plb->write + y * plb->stride);
    register uint16 *s = (uint16 *) (GFX.Screen + y * GFX.RealPitch);
    
    for (register x = 0; x < SNES_WIDTH; x++) {
      register uint32 pixel = *s++;
      
      *d++ = (pixel & 0x1f) << 3;
      *d++ = ((pixel >> 5) & 0x3f) << 2;
      *d++ = ((pixel >> 11) & 0x1f) << 3;
    }
  }
}

static void Convert16to32(int width, int height)
{
  for (register y = 0; y < SNES_HEIGHT_EXTENDED; y++) {
    register uint32 *d = (uint32 *) ((uint8 *)plb->write + y * plb->stride);
    register uint16 *s = (uint16 *) (GFX.Screen + y * GFX.RealPitch);
    
    for (register x = 0; x < SNES_WIDTH; x++) {
      register uint32 pixel = *s++;
      
      *d++ = (((pixel & 0x1f) << 3) << 16) |
	((((pixel >> 5) & 0x3f) << 2) << 8) |
	((((pixel >> 11) & 0x1f) << 3) << 0);
    }
  }
}

static void Convert8to8(int width, int height)
{
  for (register y = 0; y < SNES_HEIGHT_EXTENDED; y++) {
    register uint8 *d = (uint8 *) ((uint8 *)plb->write + y * plb->stride);
    register uint8 *s = (uint8 *) (GFX.Screen + y * GFX.RealPitch);
    
    for (register x = 0; x < SNES_WIDTH; x++)
      *d++ = *s++;
  }
}




