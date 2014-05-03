# set the name of the installer
!define VERSION_NAME "v0_2"

Outfile "Onyx_firmware_loader_${VERSION_NAME}.exe"

# define the directory to install to, Program Files (x86) in this case as specified  
# by the predefined $PROGRAMFILES variable
InstallDir $PROGRAMFILES\OnyxLoader 

# create a default section.
Section
  # define the output path for this file
  SetOutPath $PROGRAMFILES"\OnyxLoader"
  File /nonfatal /a /r "OnyxLoader\" #note back slash at the end
SectionEnd

;Visual Studio 2013 Redistributable packages
Section "Visual Studio 2013 Redistributable packages"
SectionIn RO
  ExecWait '"$PROGRAMFILES\OnyxLoader\vcredist_x86.exe" /q /norestart'
SectionEnd

Section "Create desktop shortcut"
  CreateShortCut "$DESKTOP\OnyxLoader.lnk" "$PROGRAMFILES\OnyxLoader\OnyxLoader.exe" ""
SectionEnd