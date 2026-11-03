# Microsoft Developer Studio Project File - Name="Snes9X" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Snes9X - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Snes9Xw.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Snes9Xw.mak" CFG="Snes9X - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Snes9X - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Snes9X - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Snes9X - Win32 C core" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "snes9xw"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G6 /Zp4 /MT /W3 /O2 /Ob2 /I "unzip" /I "H:\glide2x" /I "e:\zlib" /I "snes9x\unzip" /I "H:\FMOD\api\inc" /D "ZSNES_C4" /D "SPC700_C" /D "HAVE_LIBPNG" /D "SPC700_SHUTDOWN" /D "CPU_SHUTDOWN" /D "ZLIB" /D "VAR_CYCLES" /D "UNZIP_SUPPORT" /D "ZSNES_FX" /D "EXECUTE_SUPERFX_PER_LINE" /D "__WIN32__" /D "__MSC__" /D "USE_GLIDE" /D "USE_OPENGL" /D "NDEBUG" /D "NETPLAY_SUPPORT" /D "FMOD_SUPPORT" /D "OLD_COLOUR_BLENDING" /FR /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x413 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 zlibmt.lib libpngmt.lib comctl32.lib shell32.lib ole32.lib winmm.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib wsock32.lib advapi32.lib ddraw.lib dsound.lib dxguid.lib snes9xw.lib glide2x.lib delayimp.lib fmodvc.lib /nologo /subsystem:windows /incremental:yes /machine:I386 /out:"Release/snes9x.exe" /libpath:"H:\FMOD\api\lib" /libpath:"H:\glide2x\lib" /delayload:fmod.dll
# SUBTRACT LINK32 /profile /debug

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Snes9X___Win32_Debug"
# PROP BASE Intermediate_Dir "Snes9X___Win32_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Snes9X___Win32_Debug"
# PROP Intermediate_Dir "Snes9X___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /Zp4 /MT /W3 /GX /O2 /Ob2 /I "unzip" /I "H:\glide2x" /I "e:\zlib" /I "snes9x\unzip" /I "H:\FMOD\api\inc" /D "SPC700_SHUTDOWN" /D "CPU_SHUTDOWN" /D "ZLIB" /D "VAR_CYCLES" /D "SPC700_C" /D "UNZIP_SUPPORT" /D "ZSNES_C4" /D "ZSNES_FX" /D "EXECUTE_SUPERFX_PER_LINE" /D "__WIN32__" /D "__MSC__" /D "USE_GLIDE" /D "USE_OPENGL" /D "NDEBUG" /D "NETPLAY_SUPPORT" /D "FMOD_SUPPORT" /D "OLD_COLOUR_BLENDING" /FR /YX /FD /c
# ADD CPP /nologo /G6 /Zp4 /MTd /W3 /GX /Zi /Od /I "unzip" /I "H:\glide2x" /I "e:\zlib" /I "snes9x\unzip" /I "H:\FMOD\api\inc" /D "SPC700_C" /D "HAVE_LIBPNG" /D "SPC700_SHUTDOWN" /D "CPU_SHUTDOWN" /D "ZLIB" /D "VAR_CYCLES" /D "UNZIP_SUPPORT" /D "ZSNES_FX" /D "EXECUTE_SUPERFX_PER_LINE" /D "__WIN32__" /D "__MSC__" /D "USE_GLIDE" /D "USE_OPENGL" /D "NDEBUG" /D "NETPLAY_SUPPORT" /D "FMOD_SUPPORT" /D "OLD_COLOUR_BLENDING" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x413 /d "NDEBUG"
# ADD RSC /l 0x413 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib wsock32.lib advapi32.lib winmm.lib ddraw.lib dsound.lib dxguid.lib snes9xw.lib glide2x.lib delayimp.lib fmodvc.lib /nologo /subsystem:windows /profile /machine:I386 /libpath:"H:\FMOD\api\lib" /libpath:"H:\glide2x\lib" /delayload:fmod.dll
# ADD LINK32 libpngmtd.lib comctl32.lib shell32.lib ole32.lib winmm.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib wsock32.lib advapi32.lib ddraw.lib dsound.lib dxguid.lib snes9xw.lib glide2x.lib delayimp.lib fmodvc.lib /nologo /subsystem:windows /incremental:yes /debug /machine:I386 /out:"Snes9X___Win32_Debug/Snes9x.exe" /libpath:"H:\FMOD\api\lib" /libpath:"H:\glide2x\lib" /delayload:fmod.dll
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Snes9X___Win32_C_core"
# PROP BASE Intermediate_Dir "Snes9X___Win32_C_core"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Snes9X___Win32_C_core"
# PROP Intermediate_Dir "Snes9X___Win32_C_core"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp4 /MTd /W3 /GX /ZI /Od /I "unzip" /I "H:\glide2x" /I "e:\zlib" /I "snes9x\unzip" /I "H:\FMOD\api\inc" /D "SPC700_SHUTDOWN" /D "CPU_SHUTDOWN" /D "ZLIB" /D "VAR_CYCLES" /D "SPC700_C" /D "UNZIP_SUPPORT" /D "ZSNES_C4" /D "ZSNES_FX" /D "EXECUTE_SUPERFX_PER_LINE" /D "__WIN32__" /D "__MSC__" /D "USE_GLIDE" /D "USE_OPENGL" /D "NDEBUG" /D "NETPLAY_SUPPORT" /D "FMOD_SUPPORT" /D "OLD_COLOUR_BLENDING" /FR /YX /FD /c
# ADD CPP /nologo /Zp4 /MT /W3 /Zi /O2 /I "unzip" /I "H:\glide2x" /I "e:\zlib" /I "snes9x\unzip" /I "H:\FMOD\api\inc" /D "HAVE_LIBPNG" /D "SPC700_SHUTDOWN" /D "CPU_SHUTDOWN" /D "ZLIB" /D "VAR_CYCLES" /D "UNZIP_SUPPORT" /D "ZSNES_FX" /D "EXECUTE_SUPERFX_PER_LINE" /D "__WIN32__" /D "__MSC__" /D "USE_OPENGL" /D "NDEBUG" /D "NETPLAY_SUPPORT" /D "FMOD_SUPPORT" /D "OLD_COLOUR_BLENDING" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x413 /d "NDEBUG"
# ADD RSC /l 0x413 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib wsock32.lib advapi32.lib winmm.lib ddraw.lib dsound.lib dxguid.lib snes9xw.lib glide2x.lib delayimp.lib fmodvc.lib /nologo /subsystem:windows /incremental:yes /debug /machine:I386 /libpath:"H:\FMOD\api\lib" /libpath:"H:\glide2x\lib" /delayload:fmod.dll
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 libpngmt.lib comctl32.lib shell32.lib ole32.lib winmm.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib wsock32.lib advapi32.lib ddraw.lib dsound.lib dxguid.lib snes9xw.lib delayimp.lib fmodvc.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:"H:\FMOD\api\lib" /libpath:"H:\glide2x\lib" /delayload:fmod.dll
# SUBTRACT LINK32 /profile /incremental:yes

!ENDIF 

# Begin Target

# Name "Snes9X - Win32 Release"
# Name "Snes9X - Win32 Debug"
# Name "Snes9X - Win32 C core"
# Begin Group "zLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\zLib\adler32.c

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zLib\compress.c

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zLib\crc32.c

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zLib\deflate.c

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zLib\deflate.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zLib\gzio.c

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zLib\infblock.c

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zLib\infblock.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zLib\infcodes.c

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zLib\infcodes.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zLib\inffast.c

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zLib\inffast.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zLib\inflate.c

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zLib\inftrees.c

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zLib\inftrees.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zLib\infutil.c

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zLib\infutil.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zLib\trees.c

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zLib\uncompr.c

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zLib\zconf.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zLib\zlib.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zLib\zutil.c

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zLib\zutil.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/zLib"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/zLib"
# PROP Intermediate_Dir "Release/zLib"

!ENDIF 

# End Source File
# End Group
# Begin Group "GUI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DirectX.cpp
# End Source File
# Begin Source File

SOURCE=.\DirectX.h
# End Source File
# Begin Source File

SOURCE=.\Render.cpp
# End Source File
# Begin Source File

SOURCE=.\Render.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\Snes9X\Win32.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\wsnes9x.cpp
# End Source File
# End Group
# Begin Group "Emu"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Snes9x\2xsai.cpp
# End Source File
# Begin Source File

SOURCE=.\Snes9X\65c816.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\apu.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\apu.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9x\Apudebug.cpp
# End Source File
# Begin Source File

SOURCE=.\Snes9X\apumem.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\DevStudio\Vc98\Include\Basetsd.h
# End Source File
# Begin Source File

SOURCE=.\Snes9x\C4.cpp
# End Source File
# Begin Source File

SOURCE=.\Snes9X\cheats.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\cheats.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9x\cheats2.cpp
# End Source File
# Begin Source File

SOURCE=.\Snes9X\clip.cpp
# End Source File
# Begin Source File

SOURCE=.\Snes9X\cpu.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"
# ADD CPP /Ob0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\cpuaddr.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\cpuexec.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1
# PROP Ignore_Default_Tool 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# ADD CPP /Ob0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\cpuexec.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\cpumacro.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\cpuops.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# ADD CPP /Ob0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\cpuops.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\data.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\debug.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\debug.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\display.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\dma.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\dma.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\snes9x\dsp1.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# ADD CPP /Ze

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# ADD CPP /Ob0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\snes9x\dsp1.h
# End Source File
# Begin Source File

SOURCE=.\Snes9X\fxdbg.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\fxemu.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\fxemu.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\fxinst.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\fxinst.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\getset.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\gfx.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\gfx.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\globals.cpp
# End Source File
# Begin Source File

SOURCE=.\SNES9X\loadzip.CPP
# End Source File
# Begin Source File

SOURCE=.\Snes9X\memmap.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# ADD CPP /O2 /Ob2
# SUBTRACT CPP /Z<none>

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\memmap.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\snes9x\messages.h
# End Source File
# Begin Source File

SOURCE=.\Snes9X\missing.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9x\Netplay.cpp
# End Source File
# Begin Source File

SOURCE=.\Snes9x\Netplay.h
# End Source File
# Begin Source File

SOURCE=.\snes9x\pixform.h
# End Source File
# Begin Source File

SOURCE=.\Snes9X\port.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\ppu.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"
# ADD CPP /Ob2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\ppu.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9x\Sa1.cpp
# End Source File
# Begin Source File

SOURCE=.\Snes9x\sa1cpu.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9x\Sdd1.cpp
# End Source File
# Begin Source File

SOURCE=.\Snes9x\Server.cpp
# End Source File
# Begin Source File

SOURCE=.\Snes9X\snaporig.cpp
# End Source File
# Begin Source File

SOURCE=.\snes9x\snaporig.h
# End Source File
# Begin Source File

SOURCE=.\Snes9X\snapshot.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\snapshot.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9x\Snes9x.cpp
# End Source File
# Begin Source File

SOURCE=.\Snes9X\snes9x.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X.rc
# End Source File
# Begin Source File

SOURCE=.\Snes9X\soundux.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\soundux.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\spc700.cpp
# End Source File
# Begin Source File

SOURCE=.\Snes9X\spc700.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9x\Srtc.cpp
# End Source File
# Begin Source File

SOURCE=.\Snes9x\Srtc.h
# End Source File
# Begin Source File

SOURCE=.\Snes9X\tile.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9X\tile.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Intermediate_Dir "Release/Emu"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Intermediate_Dir "Release/Emu"
# PROP Intermediate_Dir "Release/Emu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9x\I386\cpuexec.obj

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9x\I386\cpuops.obj

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9x\I386\fxemu2.obj
# End Source File
# Begin Source File

SOURCE=.\Snes9x\I386\fxemu2b.obj
# End Source File
# Begin Source File

SOURCE=.\Snes9x\I386\fxemu2c.obj
# End Source File
# Begin Source File

SOURCE=.\Snes9x\I386\fxtable.obj
# End Source File
# Begin Source File

SOURCE=.\Snes9x\I386\sa1ops.obj

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9x\I386\sfxproc.obj
# End Source File
# Begin Source File

SOURCE=.\Snes9x\I386\zsnes.obj
# End Source File
# Begin Source File

SOURCE=.\Snes9x\I386\spc.obj
# End Source File
# Begin Source File

SOURCE=.\Snes9x\I386\zsnesc4.obj

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9x\I386\c4.obj

!IF  "$(CFG)" == "Snes9X - Win32 Release"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Snes9x\I386\2xsaimmx.OBJ
# End Source File
# Begin Source File

SOURCE=.\Snes9x\I386\bilinear.OBJ
# End Source File
# End Group
# Begin Group "UnZip"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\Microsoft Visual Studio\VC98\Include\BASETSD.H"
# End Source File
# Begin Source File

SOURCE=.\Snes9x\unzip\Explode.c
# End Source File
# Begin Source File

SOURCE=.\Snes9x\unzip\Unreduce.c
# End Source File
# Begin Source File

SOURCE=.\Snes9x\unzip\Unshrink.c
# End Source File
# Begin Source File

SOURCE=.\Snes9x\unzip\Unz.h
# End Source File
# Begin Source File

SOURCE=.\Snes9x\unzip\Unzip.c
# End Source File
# Begin Source File

SOURCE=.\Snes9x\unzip\Unzip.h
# End Source File
# Begin Source File

SOURCE=.\Snes9x\unzip\unzipP.h
# End Source File
# End Group
# Begin Group "D3D"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\D3d\d3denum.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\D3d\d3denum.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\D3d\d3dframe.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\D3d\d3dframe.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\D3d\d3dmath.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\D3d\d3dmath.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\D3d\d3dtextr.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\D3d\d3dtextr.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\D3d\d3dutil.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\D3d\d3dutil.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\D3d\resource.h

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\D3d\userdlg.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\D3d\videotex.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\D3d\winmain.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\Auxmath.cpp
# End Source File
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\snes9x\c4emu.cpp

!IF  "$(CFG)" == "Snes9X - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Snes9X - Win32 Debug"

!ELSEIF  "$(CFG)" == "Snes9X - Win32 C core"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cursor_b.cur
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\nodrop.cur
# End Source File
# Begin Source File

SOURCE=.\snes9x\obc1.cpp
# End Source File
# Begin Source File

SOURCE=.\snes9x\screenshot.cpp
# End Source File
# Begin Source File

SOURCE=.\snes9x\seta.cpp
# End Source File
# Begin Source File

SOURCE=.\snes9x\seta.h
# End Source File
# Begin Source File

SOURCE=.\snes9x\seta010.cpp
# End Source File
# Begin Source File

SOURCE=.\snes9x\seta011.cpp
# End Source File
# Begin Source File

SOURCE=.\snes9x\seta018.cpp
# End Source File
# Begin Source File

SOURCE=.\snes9x\spc7110.cpp
# End Source File
# Begin Source File

SOURCE=.\snes9x\spc7110.h
# End Source File
# Begin Source File

SOURCE=.\Package\windows.txt
# End Source File
# End Target
# End Project
