# Microsoft Developer Studio Project File - Name="demo1" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=demo1 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "demo1.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "demo1.mak" CFG="demo1 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "demo1 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "demo1 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "demo1 - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "demo1 - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "demo1 - Win32 Release"
# Name "demo1 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\demo1.cpp
# End Source File
# Begin Source File

SOURCE=.\demo1.rc
# End Source File
# Begin Source File

SOURCE=.\demo1Doc.cpp
# End Source File
# Begin Source File

SOURCE=.\demo1View.cpp
# End Source File
# Begin Source File

SOURCE=.\Detect.cpp
# End Source File
# Begin Source File

SOURCE=.\FaceDetect.cpp
# End Source File
# Begin Source File

SOURCE=.\FourierTrans.cpp
# End Source File
# Begin Source File

SOURCE=.\GeometryMoveDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GeometryTrans.cpp
# End Source File
# Begin Source File

SOURCE=.\GrayTrans.cpp
# End Source File
# Begin Source File

SOURCE=.\HistogramDrawDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HistSegmentDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageDib.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageEnhance.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageFreqEnhance.cpp
# End Source File
# Begin Source File

SOURCE=.\ImgSegment.cpp
# End Source File
# Begin Source File

SOURCE=.\LinearStepParamDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MedianSmoothDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Morphology.cpp
# End Source File
# Begin Source File

SOURCE=.\NeiAverSmoothDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Register.cpp
# End Source File
# Begin Source File

SOURCE=.\RotateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StrechWindowDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreshStrechDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WaveletTrans.cpp
# End Source File
# Begin Source File

SOURCE=.\Zoomdlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\demo1.h
# End Source File
# Begin Source File

SOURCE=.\demo1Doc.h
# End Source File
# Begin Source File

SOURCE=.\demo1View.h
# End Source File
# Begin Source File

SOURCE=.\Detect.h
# End Source File
# Begin Source File

SOURCE=.\FaceDetect.h
# End Source File
# Begin Source File

SOURCE=.\FourierTrans.h
# End Source File
# Begin Source File

SOURCE=.\GeometryMoveDlg.h
# End Source File
# Begin Source File

SOURCE=.\GeometryTrans.h
# End Source File
# Begin Source File

SOURCE=.\GrayTrans.h
# End Source File
# Begin Source File

SOURCE=.\HistogramDrawDlg.h
# End Source File
# Begin Source File

SOURCE=.\HistSegmentDlg.h
# End Source File
# Begin Source File

SOURCE=.\ImageDib.h
# End Source File
# Begin Source File

SOURCE=.\ImageEnhance.h
# End Source File
# Begin Source File

SOURCE=.\ImageFreqEnhance.h
# End Source File
# Begin Source File

SOURCE=.\ImgSegment.h
# End Source File
# Begin Source File

SOURCE=.\LinearStepParamDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MedianSmoothDlg.h
# End Source File
# Begin Source File

SOURCE=.\Morphology.h
# End Source File
# Begin Source File

SOURCE=.\NeiAverSmoothDlg.h
# End Source File
# Begin Source File

SOURCE=.\Register.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RotateDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StrechWindowDlg.h
# End Source File
# Begin Source File

SOURCE=.\ThreshStrechDlg.h
# End Source File
# Begin Source File

SOURCE=.\WaveletTrans.h
# End Source File
# Begin Source File

SOURCE=.\Zoomdlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\demo1.ico
# End Source File
# Begin Source File

SOURCE=.\res\demo1.rc2
# End Source File
# Begin Source File

SOURCE=.\res\demo1Doc.ico
# End Source File
# Begin Source File

SOURCE=.\res\OK.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\demo1.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
