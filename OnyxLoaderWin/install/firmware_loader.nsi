# set the name of the installer
Name "OnyxLinkInstaller"
!define VERSION_NAME "v1_0"


Outfile "OnyxLink_installer_${VERSION_NAME}.exe"

# define the directory to install to, Program Files (x86) in this case as specified  
# by the predefined $PROGRAMFILES variable
Section "Copy binaries"

  Var /GLOBAL INSTALL_LOCATION
  StrCpy $INSTALL_LOCATION $PROGRAMFILES\OnyxLink

  # define the output path for this file
  SetOutPath $INSTALL_LOCATION
  File /nonfatal /a /r "OnyxLink\" #note back slash at the end
SectionEnd

# Visual Studio 2013 Redistributable packages
Section "Visual Studio 2013 Redistributable packages"
SectionIn RO
  ExecWait '"$INSTALL_LOCATION\vcredist_x86.exe" /q /norestart'
SectionEnd

Section "Create desktop shortcut"
  CreateShortCut "$DESKTOP\OnyxLink.lnk" "$INSTALL_LOCATION\OnyxLink.exe" ""
SectionEnd