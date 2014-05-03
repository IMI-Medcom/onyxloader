# set the name of the installer
Name "Onyx Firmware Loader"
!define VERSION_NAME "v0_2"

Outfile "Onyx_firmware_loader_${VERSION_NAME}.exe"

# define the directory to install to, Program Files (x86) in this case as specified  
# by the predefined $PROGRAMFILES variable
Section "Copy binaries"

  Var /GLOBAL INSTALL_LOCATION
  StrCpy $INSTALL_LOCATION $PROGRAMFILES\OnyxLoader

  # define the output path for this file
  SetOutPath $INSTALL_LOCATION
  File /nonfatal /a /r "OnyxLoader\" #note back slash at the end
SectionEnd

# Visual Studio 2013 Redistributable packages
Section "Visual Studio 2013 Redistributable packages"
SectionIn RO
  ExecWait '"$INSTALL_LOCATION\vcredist_x86.exe" /q /norestart'
SectionEnd

Section "Create desktop shortcut"
  CreateShortCut "$DESKTOP\OnyxLoader.lnk" "$INSTALL_LOCATION\OnyxLoader.exe" ""
SectionEnd