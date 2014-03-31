# set the name of the installer
Outfile "Onyx_firmware_loader.exe"
 
# define the directory to install to, the desktop in this case as specified  
# by the predefined $DESKTOP variable
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

  ExecWait '"$PROGRAMFILES\vcredist_x86.exe" /q /norestart'

SectionEnd
