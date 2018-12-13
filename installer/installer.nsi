; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "NAGA"
!define PRODUCT_VERSION "1.0"
!define PRODUCT_PUBLISHER "MapleStory"
!define PRODUCT_WEB_SITE "https://bobnaga679428248.wordpress.com/"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\PECmd.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "GUI.ico"

; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "eula.txt"
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\Naga.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Korean"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "NAGA_Setup.exe"
InstallDir "$PROGRAMFILES\NAGA"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite on
  File "result.json"
  File "PECmd.exe"
  File "Naga.exe"
  CreateDirectory "$SMPROGRAMS\NAGA"
  CreateShortCut "$SMPROGRAMS\NAGA\NAGA.lnk" "$INSTDIR\Naga.exe"
  CreateShortCut "$DESKTOP\NAGA.lnk" "$INSTDIR\Naga.exe"
  File "compare.exe"
  File "combine.json"
  File "boost_thread-vc141-mt-x64-1_67.dll"
  File "boost_system-vc141-mt-x64-1_67.dll"
  File "boost_date_time-vc141-mt-x64-1_67.dll"
SectionEnd

Section -AdditionalIcons
  CreateShortCut "$SMPROGRAMS\NAGA\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\PECmd.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\PECmd.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name)��(��) ������ ���ŵǾ����ϴ�."
FunctionEnd

Function un.onInit
!insertmacro MUI_UNGETLANGUAGE
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "$(^Name)��(��) �����Ͻðڽ��ϱ�?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\boost_date_time-vc141-mt-x64-1_67.dll"
  Delete "$INSTDIR\boost_system-vc141-mt-x64-1_67.dll"
  Delete "$INSTDIR\boost_thread-vc141-mt-x64-1_67.dll"
  Delete "$INSTDIR\combine.json"
  Delete "$INSTDIR\compare.exe"
  Delete "$INSTDIR\Naga.exe"
  Delete "$INSTDIR\PECmd.exe"
  Delete "$INSTDIR\result.json"

  Delete "$SMPROGRAMS\NAGA\Uninstall.lnk"
  Delete "$DESKTOP\NAGA.lnk"
  Delete "$SMPROGRAMS\NAGA\NAGA.lnk"

  RMDir "$SMPROGRAMS\NAGA"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd