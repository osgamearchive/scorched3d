////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2003
//
//    This file is part of Scorched3D.
//
//    Scorched3D is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    Scorched3D is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Scorched3D; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
////////////////////////////////////////////////////////////////////////////////


#include <client/LauncherDialog.h>
#include <client/ScorchedDialog.h>
#include <client/resource.h>
#include <client/OptionsClient.h>
#include <GLEXT/GLStateExtension.h>
#include <XML/XMLFile.h>
#include <stdio.h>

#ifdef _WIN32
#include <commctrl.h>

extern LPCTSTR lpszAppName;
extern char *scorchedOptionsFile;

void setupDisplayTab(HWND window)
{
	HWND serverDialog = GetDlgItem(window, IDC_EDIT_SERVER); 
	if (strlen(OptionsClient::instance()->getServerName()) > 0)
	{
		CheckDlgButton(window, IDC_JOINSERVER, BST_CHECKED);
		SetWindowText(serverDialog, OptionsClient::instance()->getServerName());
	}

	static bool loadedServers = false;
	if (!loadedServers)
	{
		XMLFile serverList;
		serverList.readFile(PKGDIR "data/svrlist.xml");
		if (serverList.getRootNode())
		{
			std::list<XMLNode *>::iterator itor;
			for (itor = serverList.getRootNode()->getChildren().begin();
				itor != serverList.getRootNode()->getChildren().end();
				itor++)
			{
				XMLNode *current = (*itor);
				XMLNode *nameNode = current->getNamedChild("name");

				if (nameNode)
				{
					const char *server = nameNode->getContent();
					SendMessage( 
						(HWND) serverDialog,         // handle to destination window 
						CB_ADDSTRING,        // message to send
						(WPARAM) 0,     // not used; must be zero
						(LPARAM) server      // string to add (LPCTSTR)
					);
				}
			}
			loadedServers = true;
		}
	}

	HWND sliderBar = GetDlgItem(window, IDC_SLIDER1); 
	SendMessage(sliderBar, TBM_SETRANGE, (WPARAM) (BOOL) 1, 
		(LPARAM) MAKELONG(3, 40)); 

	DWORD brightness = OptionsClient::instance()->getBrightness();
	SendMessage(sliderBar, TBM_SETPOS, (WPARAM) (BOOL) 1, 
		(LPARAM) brightness); 

	DWORD fullscreen = OptionsClient::instance()->getFullScreen()?1:0;
	CheckDlgButton(window, IDC_FULLSCREEN, fullscreen?BST_CHECKED:BST_UNCHECKED);

	DEVMODE defaultDevMode;
	ZeroMemory(&defaultDevMode, sizeof(DEVMODE));
	defaultDevMode.dmSize = sizeof(DEVMODE);
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &defaultDevMode);

	HWND combo = GetDlgItem(window, IDC_DISPLAY); 
	DEVMODE devMode;
	ZeroMemory(&devMode, sizeof(DEVMODE));
	devMode.dmSize = sizeof(DEVMODE);

	static char string[256];
	for (int i=0; EnumDisplaySettings(NULL, i, &devMode); i++)
	{
		sprintf(string, "%i x %i", 
			devMode.dmPelsWidth, 
			devMode.dmPelsHeight);

		if (SendMessage(combo, CB_FINDSTRING, (WPARAM) -1, (LPARAM) string)
			== CB_ERR)
		{
			SendMessage(combo, CB_INSERTSTRING , 0, (LPARAM) string); 
		}

		ZeroMemory(&devMode, sizeof(DEVMODE));
		devMode.dmSize = sizeof(DEVMODE);
	}

	sprintf(string, "%i x %i", 
		OptionsClient::instance()->getScreenWidth(),
		OptionsClient::instance()->getScreenHeight());
	SendMessage(combo, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) string); 
}

void setupTSTab(HWND window)
{
	DWORD fullclear = OptionsClient::instance()->getFullClear()?1:0;
	CheckDlgButton(window, IDC_FULLCLEAR, fullclear?BST_CHECKED:BST_UNCHECKED);

	DWORD noext = OptionsClient::instance()->getNoExt()?1:0;
	CheckDlgButton(window, IDC_NOEXT, noext?BST_CHECKED:BST_UNCHECKED);

	DWORD nosound = OptionsClient::instance()->getNoSound()?1:0;
	CheckDlgButton(window, IDC_NOSOUND, nosound?BST_CHECKED:BST_UNCHECKED);

	DWORD nomultitex = OptionsClient::instance()->getNoMultiTex()?1:0;
	CheckDlgButton(window, IDC_NOMULTITEX, nomultitex?BST_CHECKED:BST_UNCHECKED);

	DWORD noskins = OptionsClient::instance()->getNoSkins()?1:0;
	CheckDlgButton(window, IDC_NOSKINS, noskins?BST_CHECKED:BST_UNCHECKED);

	DWORD texSize = OptionsClient::instance()->getTexSize();
	switch (texSize)
	{
	case 0:
		CheckDlgButton(window, IDC_RADIO_SMALL, BST_CHECKED);
		break;
	case 2:
		CheckDlgButton(window, IDC_RADIO_LARGE, BST_CHECKED);
		break;
	default:
		CheckDlgButton(window, IDC_RADIO_MEDIUM, BST_CHECKED);
		break;
	}
}

void saveDisplayTab(HWND window)
{
	HWND sliderBar = GetDlgItem(window, IDC_SLIDER1); 
	int pos = (int) SendMessage(sliderBar, TBM_GETPOS, (WPARAM) 0, (LPARAM) 0); 
	OptionsClient::instance()->setBrightness((int) pos);

	DWORD fullscreen = (IsDlgButtonChecked(window, IDC_FULLSCREEN) == BST_CHECKED);
	OptionsClient::instance()->setFullScreen((fullscreen==1)?true:false);

	HWND combo = GetDlgItem(window, IDC_DISPLAY); 
	char buffer[32];
	GetWindowText(combo, buffer, 32);
	int windowWidth, windowHeight;
	if (sscanf(buffer, 
		"%i x %i", 
		&windowWidth,
		&windowHeight) == 2)
	{
		OptionsClient::instance()->setScreenWidth(windowWidth);
		OptionsClient::instance()->setScreenHeight(windowHeight);
	}

	char serverName[128];
	serverName[0] = '\0';
	if (IsDlgButtonChecked(window, IDC_JOINSERVER) == BST_CHECKED)
	{
		HWND serverDialog = GetDlgItem(window, IDC_EDIT_SERVER); 
		GetWindowText(serverDialog, serverName, 128);
	}
	OptionsClient::instance()->setServerName(serverName);
}

void saveTSTab(HWND window)
{
	DWORD fullclear = (IsDlgButtonChecked(window, IDC_FULLCLEAR) == BST_CHECKED);
	OptionsClient::instance()->setFullClear((fullclear==1)?true:false);

	DWORD noext = (IsDlgButtonChecked(window, IDC_NOEXT) == BST_CHECKED);
	OptionsClient::instance()->setNoExt((noext==1)?true:false);

	DWORD nosound = (IsDlgButtonChecked(window, IDC_NOSOUND) == BST_CHECKED);
	OptionsClient::instance()->setNoSound((nosound==1)?true:false);

	DWORD nomultitex = (IsDlgButtonChecked(window, IDC_NOMULTITEX) == BST_CHECKED);
	OptionsClient::instance()->setNoMultiTex((nomultitex==1)?true:false);

	DWORD noskins = (IsDlgButtonChecked(window, IDC_NOSKINS) == BST_CHECKED);
	OptionsClient::instance()->setNoSkins((noskins==1)?true:false);

	DWORD size = 1;
	if (IsDlgButtonChecked(window, IDC_RADIO_SMALL) == BST_CHECKED) size = 0;
	else if (IsDlgButtonChecked(window, IDC_RADIO_LARGE) == BST_CHECKED) size = 2;
	OptionsClient::instance()->setTexSize((int) size);
}

LRESULT CALLBACK aboutWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

LRESULT CALLBACK DisplayTabWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_INITDIALOG:
		setupDisplayTab(hWnd);
		return TRUE;
		break;
	case WM_COMMAND:
		{
		int wmId    = LOWORD(wParam); 
		int wmEvent = HIWORD(wParam);

		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_JOINSERVER:
			{
				HWND editBox = GetDlgItem(hWnd, IDC_EDIT_SERVER); 
				bool checked = (IsDlgButtonChecked(hWnd, IDC_JOINSERVER)==BST_CHECKED);
				if (!checked) SetWindowText(editBox, "");
				EnableWindow(editBox, (checked? TRUE : FALSE));
			}
			break;
		}
		}
		break;
	case WM_NOTIFY:
		{
		LPNMHDR pnmh = (LPNMHDR) lParam; 
		switch (pnmh->code)
		{
		case PSN_APPLY:
			saveDisplayTab(hWnd);
			break;
		}
		}
		break;
	}
	return FALSE;
}

LRESULT CALLBACK TSTabWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_INITDIALOG:
		setupTSTab(hWnd);
		return TRUE;
		break;
	case WM_NOTIFY:
		{
		LPNMHDR pnmh = (LPNMHDR) lParam; 
		switch (pnmh->code)
		{
		case PSN_APPLY:
			saveTSTab(hWnd);
			break;
		}
		}
		break;
	}
	return FALSE;
}

bool createLauncherDialog()
{
	// Read the options from the file
	OptionsClient::instance()->readOptionsFromFile(scorchedOptionsFile);

	// Set the window title
	CHAR lpszText[1024];
	sprintf(lpszText, "%s - Version %s", lpszAppName, ScorchedVersion);

	// Create all of the dialog tabs
	HINSTANCE hInstance = (HINSTANCE) GetModuleHandle(NULL); 
	PROPSHEETPAGE psp[2];
    PROPSHEETHEADER psh;
    psp[0].dwSize = sizeof(PROPSHEETPAGE);
    psp[0].dwFlags = PSP_USETITLE;
	psp[0].hInstance = hInstance;
    psp[0].pszTemplate = MAKEINTRESOURCE(IDD_LAUNCHER_DIALOG);
    psp[0].pfnDlgProc = (DLGPROC) DisplayTabWndProc;
    psp[0].pszTitle = (LPSTR) "Display";
    psp[0].lParam = 0;
    psp[0].pfnCallback = NULL;

    psp[1].dwSize = sizeof(PROPSHEETPAGE);
    psp[1].dwFlags = PSP_USETITLE;
	psp[1].hInstance = hInstance;
    psp[1].pszTemplate = MAKEINTRESOURCE(IDD_LAUNCHERS_DIALOG);
    psp[1].pfnDlgProc = (DLGPROC) TSTabWndProc;
    psp[1].pszTitle = (LPSTR) "Trouble Shooting";
    psp[1].lParam = 0;
    psp[1].pfnCallback = NULL;

    psh.dwSize = sizeof(PROPSHEETHEADER);
    psh.dwFlags = PSH_PROPSHEETPAGE | PSH_NOCONTEXTHELP | PSH_USEHICON;
    psh.hwndParent = GetDesktopWindow();
    psh.hInstance = hInstance;
	psh.hIcon = (HICON)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    psh.pszCaption = (LPSTR) lpszText;
    psh.nPages = sizeof(psp) / sizeof(PROPSHEETPAGE);
    psh.nStartPage = 0;
    psh.ppsp = (LPCPROPSHEETPAGE) &psp;
    psh.pfnCallback = NULL;

	int result = (int) PropertySheet(&psh);
	if (result >0)
	{
		// Save these options
		OptionsClient::instance()->writeOptionsToFile(scorchedOptionsFile);

		// Continue game
		return true;
	}

	// Exit game
	return false;
}

#endif
