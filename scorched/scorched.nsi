; Helper defines
!define PRODUCT_NAME "Scorched3D"
!define PRODUCT_VERSION "38.1"
!define PRODUCT_PUBLISHER "Scorched"
!define PRODUCT_WEB_SITE "http://www.scorched3d.co.uk"
!define PRODUCT_DONATE_WEB_SITE "https://www.paypal.com/xclick/business=donations%40scorched3d.co.uk&item_name=Scorched3D&no_note=1&tax=0&currency_code=GBP"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor lzma

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "data\windows\tank2.ico"
!define MUI_UNICON "data\windows\tank2.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "COPYING"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "Scorched3D-${PRODUCT_VERSION}.exe"
InstallDir "$PROGRAMFILES\Scorched3D"
ShowInstDetails show
ShowUnInstDetails show

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite try
  
  File /r /x "CVS" /x "*.db" "data"
  File /r /x "CVS" /x "*.db" "documentation"
  File "*.dll"
  File "AUTHORS"
  File "COPYING"
  File "README"
  File "TODO"
  File "src\scorched\Debug - MySql\scorched.exe"
SectionEnd

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}-donate.url" "InternetShortcut" "URL" "${PRODUCT_DONATE_WEB_SITE}"
  
  CreateDirectory "$SMPROGRAMS\Scorched3D"
  
  CreateShortCut "$SMPROGRAMS\Scorched3D\Uninstall Scorched3D.lnk" "$INSTDIR\uninst.exe"
  CreateShortCut "$SMPROGRAMS\Scorched3D\Scorched3D.lnk" "$INSTDIR\scorched.exe" "" "$INSTDIR\data\windows\tank2.ico"
  CreateShortCut "$SMPROGRAMS\Scorched3D\Scorched3D Documentation.lnk" "$INSTDIR\documentation\html\index.html"
  CreateShortCut "$SMPROGRAMS\Scorched3D\Scorched3D Homepage.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\Scorched3D\Scorched3D Donations.lnk" "$INSTDIR\${PRODUCT_NAME}-donate.url"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  RMDir /r "$INSTDIR"
  RMDir /r "$SMPROGRAMS\Scorched3D"
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  SetAutoClose true
SectionEnd
