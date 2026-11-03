/* This file is Copyright 2003 Peter Bortas <peter@bortas.org>, 
 *                             Marcus Comstedt <marcus@mc.pp.se> and 
 *                             Per Hedbor <per@hedbor.org>
 *
 *  Permission to use, copy, modify and distribute this file in both binary and
 *  source form, for non-commercial purposes, is hereby granted without fee,
 *  providing that this license information and copyright notice appear with
 *  all copies and any derived work.
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty. In no event shall the authors be held liable for any damages
 *  arising from the use of this software.
 */

//#include <math.h>
#include "snes9x.h"
#include "gfx.h"
#include "aido.h"

#include <sys/ipc.h>
#include <sys/shm.h>

#ifdef USE_AIDO

static unsigned char *data=NULL;
static int offset = 0;

bool8 S9xAIDOInit ()
{
  int shi = Settings.AIDOShmId;
  int id = shmget( shi, 1024*1024*4+100, 0 );
  if(id == -1)
  {
    perror("shmerror");
    fprintf(stderr,
            "FATAL: Snes9X: Unable to get SHM memory. (%d,%d)\n", shi, id);
    exit(1);
  }


  data = (unsigned char *)shmat( id, 0, 0 );
  if((int)data == -1)
  {
    fprintf(stderr, 
            "FATAL: Snes9X: Unable to aquire SHM area. (%d,%d)\n", shi, id);
    exit(1);
  }

  S9xSetRenderPixelFormat (RGB5551);
  //  Settings.SixteenBit = TRUE;
  Settings.SupportHiRes = TRUE;
  Settings.Mode7Interpolate = TRUE;
  Settings.AutoSaveDelay = 10;
  //  Settings.Transparency = TRUE;

  GFX.Pitch = IMAGE_WIDTH*2;                          /* Bytes per line */
  GFX.Screen = (uint8 *) malloc( IMAGE_WIDTH * IMAGE_HEIGHT*2 );
  GFX.ZBuffer = (uint8 *) malloc( IMAGE_WIDTH * IMAGE_HEIGHT );
  GFX.SubScreen = (uint8 *) malloc( IMAGE_WIDTH * IMAGE_HEIGHT*2 );
  GFX.SubZBuffer = (uint8 *) malloc( IMAGE_WIDTH * IMAGE_HEIGHT );
  GFX.Delta = (GFX.SubScreen - GFX.Screen) >> 1;

  return (TRUE);
}

void S9xAIDODeinit ()
{
  fprintf(stderr, "Snes9X exited. This shouldn't happen.\n");
}

#define RED(color) ((color&0x7C00) >> 7)
#define GREEN(color) ((color&0x03E0) >> 2)
#define BLUE(color) ((color&0x001F) << 3)

void S9xAIDOPutImage (int snes_width, int snes_height)
{
  int y, size = GFX.Pitch2*snes_height;
  //  fprintf(stderr, "data: %p   offset:%d   size:%d\n", data, offset, size);

  if( offset >  1024*4*1024 - (snes_width * snes_height * 3) )
    offset = 0;

  for(y=0; y<snes_height; y++)
    memcpy(data+offset+snes_width*y*2, GFX.Screen+GFX.Pitch2*y, GFX.Pitch2);

  {
    static int ow, oh;
    if( snes_width != ow )
    {
      printf("mem_w:%d\n", snes_width);
      ow = snes_width;
    }
    if( snes_height != oh )
    {
      printf("mem_h:%d\n", snes_height);
      oh = snes_height;
    }
  }
  
  printf("mem_offset:%d\n", offset);
  fflush(stdout);
  offset += size;
}

extern uint32 joypads [5];

static void handle_controller (char *input)
{
  uint32 controller = input[0];
  int button = input[1];
  int pressed = input[2];

  uint8 byte1 = 0;
  uint8 byte2 = 0;
  uint8 byte3 = 0;
  uint8 byte4 = 0;

  //  fprintf(stderr, "Handling controller %d.\n", controller);

  switch (button)
  {
   case 'p':
     Settings.Paused ^= 1; return;
   case '7':
     Settings.Mode7Interpolate ^= TRUE; return;
   case 't':
     Settings.TurboMode = pressed; return;
   case 'r':
     byte2 = 1;	        break; // Right
   case 'l':
     byte2 = 2;	        break; // Left
   case 'd':
     byte2 = 4;	        break; // Down
   case 'u':
     byte2 = 8;	        break; // Up
   case 's':	
     byte2 = 16;	break; // Start
   case 'c':	
     byte2 = 32;	break; // Select
   case 'a':
     byte1 = 128;	break; // A
   case 'b':
     byte2 = 128;	break; // B
   case 'x':
     byte1 = 64;	break; // X
   case 'y':	
     byte2 = 64;	break; // Y
   case 'v':	
     byte1 = 32;	break; // TL
   case 'h':	
     byte1 = 16;	break; // RT
   default:
     fprintf(stderr, "Snes9X: Unsupported command %d.\n", button);
  }

  if (pressed)
  {
    joypads [0] |= byte1;
    joypads [0] |= (byte2 << 8);
    joypads [1] |= byte3;
    joypads [1] |= (byte4 << 8);
    joypads[controller] |= byte1;
    joypads[controller] |= (byte2 << 8);
  } else {
    joypads [0] &= ~byte1;
    joypads [0] &= ~(byte2 << 8);
    joypads [1] &= ~byte3;
    joypads [1] &= ~(byte4 << 8);
    joypads[controller] &= ~byte1;
    joypads[controller] &= ~(byte2 << 8);
  }
}

#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <stropts.h>
#include <poll.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>

#define NORMAL_DATA 1
#define HIPRI_DATA 2

//FIXME: Check what block should do.
void S9xAIDOProcessEvents (bool8 block)
{
  struct pollfd poll_list[1];
  int ret;
  char input[3];

  poll_list[0].fd = 0;
  poll_list[0].events = POLLIN|POLLHUP|POLLERR|POLLNVAL;
  poll_list[0].revents = 0;

  do {
    ret = poll(poll_list, 1, 1);

    if( ((poll_list[0].revents&POLLHUP) == POLLHUP) ||
        ((poll_list[0].revents&POLLERR) == POLLERR) ||
        ((poll_list[0].revents&POLLNVAL) == POLLNVAL) )
    {
      fprintf(stderr, "Snes9X: Input poll failed. Exiting.\n");
      exit(1);
    }
    
    if(ret < 0)
    {
      if(errno == EBADF) {
        fprintf(stderr, "Snes9X: Bad fd. Exiting.\n");
        exit(1);
      }        
      return;
    }

    if( (poll_list[0].revents&POLLIN) == POLLIN )
    {
      read(0, input, 3);
      handle_controller(input);
    }
  } while( ret > 0 );
}

#endif /* USE_AIDO */
