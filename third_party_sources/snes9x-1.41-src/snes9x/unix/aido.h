#ifndef _AIDO_H_
#define _AIDO_H_
#ifdef USE_AIDO

bool8 S9xAIDOInit ();
void S9xAIDOPutImage (int width, int height);
void S9xAIDODeinit ();
void S9xAIDOProcessEvents (bool8 block);

#endif /* USE_AIDO */
#endif /* _AIDO_H_ */

