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

#ifndef _3D_H_
#define _3D_H_

#if defined(USE_OPENGL)
#include <GL/gl.h>
#include <GL/glu.h>

#ifdef __linux__
#include <GL/glx.h>
#endif

typedef struct
{
    bool8       packed_pixels_extension_present;
    bool8       draw_cube;
    uint32      version;
    // Texture format
    GLint       internal_format;
    GLint       format;
    GLint       type;

    GLint       max_texture_size;// 256 or 512
    GLint       texture_size;
    uint32      num_textures;    // 1 if max_texture_size == 256, 2 otherwise
    GLuint      textures [2];
} OpenGLData;

extern OpenGLData OpenGL;

bool8 S9xOpenGLInit ();
bool8 S9xOpenGLInit2 ();
void S9xOpenGLPutImage (int width, int height);
void S9xOpenGLDeinit ();

#endif

#ifdef USE_GLIDE
#include <glide.h>

typedef struct
{
    bool8	voodoo_present;
    GrVertex	sq[4];
    GrTexInfo	texture;
    int32	texture_mem_size;
    int32	texture_mem_start;
    float	x_offset, y_offset;
    float	x_scale, y_scale;
    float	voodoo_width;
    float	voodoo_height;
} GlideData;

extern GlideData Glide;
bool8 S9xGlideEnable (bool8 enable);
void S9xGlideDeinit ();
bool8 S9xGlideInit ();
bool8 S9xVoodooInitialise ();
#endif

#endif
