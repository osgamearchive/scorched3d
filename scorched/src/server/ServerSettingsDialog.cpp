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


#include <server/ServerSettingsDialog.h>
#include <server/OptionsServer.h>
#include <common/OptionsGame.h>
#include <windows.h>
#include "resource.h"

#ifdef _WIN32
static HINSTANCE hInst;

bool setupMainTab(HWND window)
{
	// Setup server name
	HWND serverbox = GetDlgItem(window, IDC_SERVER_NAME); 
	SetWindowText(serverbox, OptionsServer::instance()->getServerName());

	// Server port
	HWND portbox = GetDlgItem(window, IDC_SERVER_PORT); 
	char buffer[20];
	sprintf(buffer, "%i", OptionsServer::instance()->getPortNo());
	SetWindowText(portbox, buffer);

	// Simultaneous mode
	CheckDlgButton(window, IDC_SERVER_SIMUL, 
		OptionsGame::instance()->getSimultaneous()?BST_CHECKED:BST_UNCHECKED);

	// Min max players are rounds combos
	HWND mincombo = GetDlgItem(window, IDC_SERVER_MIN_PLAYERS); 
	HWND maxcombo = GetDlgItem(window, IDC_SERVER_MAX_PLAYERS); 
	HWND roundscombo = GetDlgItem(window, IDC_SERVER_ROUNDS); 
	int i;
	for (i=25; i>1; i--)
	{
		char string[20];
		sprintf(string, "%i", i);

		SendMessage(mincombo, CB_INSERTSTRING , 0, (LPARAM) string); 
		SendMessage(maxcombo, CB_INSERTSTRING , 0, (LPARAM) string); 
		SendMessage(roundscombo, CB_INSERTSTRING , 0, (LPARAM) string); 
	}
	if (SendMessage(mincombo, CB_SETCURSEL , (WPARAM) OptionsGame::instance()->getNoMinPlayers() - 2,
		(LPARAM) 0) == CB_ERR) return false;
	if (SendMessage(maxcombo, CB_SETCURSEL , (WPARAM) OptionsGame::instance()->getNoMaxPlayers() - 2,
		(LPARAM) 0) == CB_ERR) return false;
	if (SendMessage(roundscombo, CB_SETCURSEL , (WPARAM) OptionsGame::instance()->getNoRounds() - 2,
		(LPARAM) 0) == CB_ERR) return false;

	// The waiting time
	HWND shottimecombo = GetDlgItem(window, IDC_SHOT_TIME); 
	HWND idletimecombo = GetDlgItem(window, IDC_IDLE_TIME); 
	HWND waittimecombo = GetDlgItem(window, IDC_WAIT_TIME); 
	for (i=90; i>=0; i-=5)
	{
		char string[20];
		if (i==0) sprintf(string, "%i (Infinite)", i);	
		else sprintf(string, "%i Seconds", i);		
		SendMessage(shottimecombo, CB_INSERTSTRING , 0, (LPARAM) string);
		if (i!=0) 
		{
			SendMessage(waittimecombo, CB_INSERTSTRING , 0, (LPARAM) string);
			SendMessage(idletimecombo, CB_INSERTSTRING , 0, (LPARAM) string);
		}
	}
	sprintf(buffer, "%i", OptionsGame::instance()->getShotTime());
	if (SendMessage(shottimecombo,  CB_SELECTSTRING, (WPARAM) -1,
		(LPARAM) buffer) == CB_ERR) return false;

	sprintf(buffer, "%i", OptionsGame::instance()->getWaitTime());
	if (SendMessage(waittimecombo,  CB_SELECTSTRING, (WPARAM) -1,
		(LPARAM) buffer) == CB_ERR) return false;

	sprintf(buffer, "%i", OptionsGame::instance()->getIdleKickTime());
	if (SendMessage(idletimecombo,  CB_SELECTSTRING, (WPARAM) -1,
		(LPARAM) buffer) == CB_ERR) return false;

	return true;
}

void saveMainTab(HWND window)
{
	// Read port
	char buffer[128];
	HWND port = GetDlgItem(window, IDC_SERVER_PORT); 
	GetWindowText(port, buffer, 128);
	OptionsServer::instance()->setPortNo(atol(buffer));

	// Read imultaneous
	OptionsGame::instance()->setSimultaneous(
		(IsDlgButtonChecked(window, IDC_SERVER_SIMUL) == BST_CHECKED));

	// Read min + max players
	HWND mincombo = GetDlgItem(window, IDC_SERVER_MIN_PLAYERS); 
	HWND maxcombo = GetDlgItem(window, IDC_SERVER_MAX_PLAYERS); 
	HWND waittimecombo = GetDlgItem(window, IDC_WAIT_TIME); 
	HWND shottimecombo = GetDlgItem(window, IDC_SHOT_TIME); 
	HWND idletimecombo = GetDlgItem(window, IDC_IDLE_TIME); 
	HWND maxrounds = GetDlgItem(window, IDC_SERVER_ROUNDS); 
	HWND serverDialog = GetDlgItem(window, IDC_SERVER_NAME); 
	
	GetWindowText(mincombo, buffer, 128);
	int minPlayers = 2;
	sscanf(buffer, "%i", &minPlayers);

	GetWindowText(maxcombo, buffer, 128);
	int maxPlayers = 10;
	sscanf(buffer, "%i", &maxPlayers);

	GetWindowText(maxrounds, buffer, 128);
	int noRounds = 5;
	sscanf(buffer, "%i", &noRounds);

	GetWindowText(shottimecombo, buffer, 128);
	int shotTime = 30;
	sscanf(buffer, "%i", &shotTime);

	GetWindowText(waittimecombo, buffer, 128);
	int waitTime = 30;
	sscanf(buffer, "%i", &waitTime);

	GetWindowText(idletimecombo, buffer, 128);
	int idleTime = 30;
	sscanf(buffer, "%i", &idleTime);
	
	GetWindowText(serverDialog, buffer, 128);
	OptionsServer::instance()->setServerName(buffer);
	OptionsGame::instance()->setNoMinPlayers(minPlayers);
	OptionsGame::instance()->setNoMaxPlayers(maxPlayers);
	OptionsGame::instance()->setNoRounds(noRounds);
	OptionsGame::instance()->setShotTime(shotTime);
	OptionsGame::instance()->setWaitTime(waitTime);
	OptionsGame::instance()->setIdleKickTime(idleTime);
}

bool setupEnvTab(HWND window)
{
	// Wind force
	HWND forcecombo = GetDlgItem(window, IDC_COMBO_FORCE); 
	for (int i=5; i>=0; i--)
	{
		char buffer[25];
		sprintf(buffer, "Force %i%s", i, ((i==0)?" (No Wind)":""));
		SendMessage(forcecombo, CB_INSERTSTRING , 0, (LPARAM) buffer); 
	}
	SendMessage(forcecombo, CB_INSERTSTRING , 0, (LPARAM) "Random");
	if (SendMessage(forcecombo, CB_SETCURSEL , (WPARAM) OptionsGame::instance()->getWindForce(),
		(LPARAM) 0) == CB_ERR) return false;

	// Wind changes
	HWND changescombo = GetDlgItem(window, IDC_COMBO_WINDCHANGES); 
	SendMessage(changescombo, CB_INSERTSTRING , 0, (LPARAM) "On Move");
	SendMessage(changescombo, CB_INSERTSTRING , 0, (LPARAM) "On Round");
	if (SendMessage(changescombo, CB_SETCURSEL , (WPARAM) OptionsGame::instance()->getWindType(),
		(LPARAM) 0) == CB_ERR) return false;

	// Wall type
	HWND wallcombo = GetDlgItem(window, IDC_COMBO_WALLTYPE); 
	SendMessage(wallcombo, CB_INSERTSTRING , 0, (LPARAM) "Bouncy");
	SendMessage(wallcombo, CB_INSERTSTRING , 0, (LPARAM) "Concrete");
	SendMessage(wallcombo, CB_INSERTSTRING , 0, (LPARAM) "Random");
	if (SendMessage(wallcombo, CB_SETCURSEL , (WPARAM) OptionsGame::instance()->getWallType(),
		(LPARAM) 0) == CB_ERR) return false;

	// Weapon Scale
	HWND scalecombo = GetDlgItem(window, IDC_COMBO_WEAPONSCALE); 
	SendMessage(scalecombo, CB_INSERTSTRING , 0, (LPARAM) "Large");
	SendMessage(scalecombo, CB_INSERTSTRING , 0, (LPARAM) "Medium");
	SendMessage(scalecombo, CB_INSERTSTRING , 0, (LPARAM) "Small");
	if (SendMessage(scalecombo, CB_SETCURSEL , (WPARAM) OptionsGame::instance()->getWeapScale(),
		(LPARAM) 0) == CB_ERR) return false;

	return true;
}

void saveEnvTab(HWND window)
{
	HWND forcecombo = GetDlgItem(window, IDC_COMBO_FORCE); 
	OptionsGame::instance()->setWindForce(
		(OptionsGame::WindForce) SendMessage(forcecombo, CB_GETCURSEL , (WPARAM) 0, (LPARAM) 0));

	HWND changescombo = GetDlgItem(window, IDC_COMBO_WINDCHANGES); 
	OptionsGame::instance()->setWindType(
		(OptionsGame::WindType) SendMessage(changescombo, CB_GETCURSEL , (WPARAM) 0, (LPARAM) 0));

	HWND wallcombo = GetDlgItem(window, IDC_COMBO_WALLTYPE); 
	OptionsGame::instance()->setWallType(
		(OptionsGame::WallType) SendMessage(wallcombo, CB_GETCURSEL , (WPARAM) 0, (LPARAM) 0));

	HWND scalecombo = GetDlgItem(window, IDC_COMBO_WEAPONSCALE); 
	OptionsGame::instance()->setWeapScale(
		(OptionsGame::WeapScale) SendMessage(scalecombo, CB_GETCURSEL , (WPARAM) 0, (LPARAM) 0));
}

bool setupEcoTab(HWND window)
{
	// Buy on round
	char buffer[25];
	int i;
	HWND buyonround = GetDlgItem(window, IDC_BUYONROUND); 
	for (i=25; i>=1; i-=1)
	{	
		sprintf(buffer, "%i", i);
		SendMessage(buyonround, CB_INSERTSTRING , 0, (LPARAM) buffer); 	
	}
	sprintf(buffer, "%i", OptionsGame::instance()->getBuyOnRound());
	if (SendMessage(buyonround, CB_SELECTSTRING, (WPARAM) -1,
		(LPARAM) buffer) == CB_ERR) return false;

	// Money per hit
	HWND moneyperhit = GetDlgItem(window, IDC_MONEYPERHIT);
	for (i=2000; i>=0; i-=100)
	{	
		sprintf(buffer, "%i", i);
		SendMessage(moneyperhit, CB_INSERTSTRING , 0, (LPARAM) buffer); 	
	}
	sprintf(buffer, "%i", OptionsGame::instance()->getMoneyWonPerHitPoint());
	if (SendMessage(moneyperhit, CB_SELECTSTRING, (WPARAM) -1,
		(LPARAM) buffer) == CB_ERR) return false;

	// Start Money
	HWND startmoneycombo = GetDlgItem(window, IDC_STARTMONEY); 
	HWND moneyperround = GetDlgItem(window, IDC_MONEYPERROUND);
	for (i=50000; i>=0; i-=5000)
	{	
		sprintf(buffer, "%i", i);
		SendMessage(startmoneycombo, CB_INSERTSTRING , 0, (LPARAM) buffer); 
		SendMessage(moneyperround, CB_INSERTSTRING , 0, (LPARAM) buffer); 	
	}
	sprintf(buffer, "%i", OptionsGame::instance()->getStartMoney());
	if (SendMessage(startmoneycombo, CB_SELECTSTRING, (WPARAM) -1,
		(LPARAM) buffer) == CB_ERR) return false;
	sprintf(buffer, "%i", OptionsGame::instance()->getMoneyWonForRound());
	if (SendMessage(moneyperround, CB_SELECTSTRING, (WPARAM) -1,
		(LPARAM) buffer) == CB_ERR) return false;

	// Interest
	HWND interestcombo = GetDlgItem(window, IDC_INTEREST); 
	for (i=100; i>=0; i-=5)
	{	
		sprintf(buffer, "%i", i);
		SendMessage(interestcombo, CB_INSERTSTRING , 0, (LPARAM) buffer); 
	}
	sprintf(buffer, "%i", OptionsGame::instance()->getInterest());
	if (SendMessage(interestcombo, CB_SELECTSTRING, (WPARAM) -1,
		(LPARAM) buffer) == CB_ERR) return false;

	return true;
}

void saveEcoTab(HWND window)
{
	char buffer[30];

	HWND hbuyonround = GetDlgItem(window, IDC_BUYONROUND); 
	GetWindowText(hbuyonround, buffer, 128);
	int buyonround = 2;
	sscanf(buffer, "%i", &buyonround);

	HWND hmoneyperhit = GetDlgItem(window, IDC_MONEYPERHIT);
	GetWindowText(hmoneyperhit, buffer, 128);
	int moneyperhit = 2;
	sscanf(buffer, "%i", &moneyperhit);

	HWND hmoneyperround = GetDlgItem(window, IDC_MONEYPERROUND);
	GetWindowText(hmoneyperround, buffer, 128);
	int moneyperround = 2;
	sscanf(buffer, "%i", &moneyperround);

	HWND startmoneycombo = GetDlgItem(window, IDC_STARTMONEY); 
	GetWindowText(startmoneycombo, buffer, 128);
	int startMoney = 2;
	sscanf(buffer, "%i", &startMoney);
	
	HWND interestcombo = GetDlgItem(window, IDC_INTEREST); 
	GetWindowText(interestcombo, buffer, 128);
	int interest = 2;
	sscanf(buffer, "%i", &interest);

	OptionsGame::instance()->setStartMoney(startMoney);
	OptionsGame::instance()->setInterest(interest);
	OptionsGame::instance()->setBuyOnRound(buyonround);
	OptionsGame::instance()->setMoneyWonForRound(moneyperround);
	OptionsGame::instance()->setMoneyWonPerHitPoint(moneyperhit);
}

bool setupLandTab(HWND window)
{
	// Land Coverage
	char buffer[25];
	HWND coveragecombo = GetDlgItem(window, IDC_LANDCOVERAGE); 
	int i;
	for (i=500; i>=10; i-=10)
	{	
		sprintf(buffer, "%i", i);
		SendMessage(coveragecombo, CB_INSERTSTRING , 0, (LPARAM) buffer); 
	}
	sprintf(buffer, "%i", OptionsGame::instance()->getNoHills());
	if (SendMessage(coveragecombo, CB_SELECTSTRING, (WPARAM) -1,
		(LPARAM) buffer) == CB_ERR) return false;

	// Land Height
	HWND heightcombo = GetDlgItem(window, IDC_LANDHEIGHT); 
	for (i=100; i>=0; i-=10)
	{	
		sprintf(buffer, "%i", i);
		SendMessage(heightcombo, CB_INSERTSTRING , 0, (LPARAM) buffer); 
	}
	sprintf(buffer, "%i", OptionsGame::instance()->getMaxHeight());
	if (SendMessage(heightcombo, CB_SELECTSTRING, (WPARAM) -1,
		(LPARAM) buffer) == CB_ERR) return false;

	return true;
}

void saveLandTab(HWND window)
{
	char buffer[30];
	HWND landCover = GetDlgItem(window, IDC_LANDCOVERAGE); 
	GetWindowText(landCover, buffer, 128);
	int ilandCover = 2;
	sscanf(buffer, "%i", &ilandCover);
	
	HWND landHeight = GetDlgItem(window, IDC_LANDHEIGHT); 
	GetWindowText(landHeight, buffer, 128);
	int ilandHeight = 2;
	sscanf(buffer, "%i", &ilandHeight);

	OptionsGame::instance()->setMaxHeight(ilandHeight);
	OptionsGame::instance()->setNoHills(ilandCover);
}

LRESULT CALLBACK MainTabWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_INITDIALOG:
		setupMainTab(hWnd);
		return TRUE;
		break;
	case WM_NOTIFY:
		{
		LPNMHDR pnmh = (LPNMHDR) lParam; 
		switch (pnmh->code)
		{
		case PSN_APPLY:
			saveMainTab(hWnd);
			break;
		}
		}
		break;
	}
	return FALSE;
}

LRESULT CALLBACK EnvTabWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_INITDIALOG:
		setupEnvTab(hWnd);
		return TRUE;
		break;
	case WM_NOTIFY:
		{
		LPNMHDR pnmh = (LPNMHDR) lParam; 
		switch (pnmh->code)
		{
		case PSN_APPLY:
			saveEnvTab(hWnd);
			break;
		}
		}
		break;
	}
	return FALSE;
}

LRESULT CALLBACK EcoTabWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_INITDIALOG:
		setupEcoTab(hWnd);
		return TRUE;
		break;
	case WM_NOTIFY:
		{
		LPNMHDR pnmh = (LPNMHDR) lParam; 
		switch (pnmh->code)
		{
		case PSN_APPLY:
			saveEcoTab(hWnd);
			break;
		}
		}
		break;
	}
	return FALSE;
}

LRESULT CALLBACK LandTabWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_INITDIALOG:
		setupLandTab(hWnd);
		return TRUE;
		break;
	case WM_NOTIFY:
		{
		LPNMHDR pnmh = (LPNMHDR) lParam; 
		switch (pnmh->code)
		{
		case PSN_APPLY:
			saveLandTab(hWnd);
			break;
		}
		}
		break;
	}
	return FALSE;
}

bool showSettingsDialog()
{
	hInst = (HINSTANCE) GetModuleHandle(NULL); 

	PROPSHEETPAGE psp[4];
    PROPSHEETHEADER psh;

    psp[0].dwSize = sizeof(PROPSHEETPAGE);
    psp[0].dwFlags = PSP_USETITLE;
	psp[0].hInstance = hInst;
    psp[0].pszTemplate = MAKEINTRESOURCE(IDD_DIALOG2);
    psp[0].pfnDlgProc = (DLGPROC) MainTabWndProc;
    psp[0].pszTitle = (LPSTR) "Main";
    psp[0].lParam = 0;
    psp[0].pfnCallback = NULL;

    psp[1].dwSize = sizeof(PROPSHEETPAGE);
    psp[1].dwFlags = PSP_USETITLE;
	psp[1].hInstance = hInst;
    psp[1].pszTemplate = MAKEINTRESOURCE(IDD_DIALOG3);
    psp[1].pfnDlgProc = (DLGPROC) EnvTabWndProc;
    psp[1].pszTitle = (LPSTR) "Env";
    psp[1].lParam = 0;
    psp[1].pfnCallback = NULL;

    psp[2].dwSize = sizeof(PROPSHEETPAGE);
    psp[2].dwFlags = PSP_USETITLE;
	psp[2].hInstance = hInst;
    psp[2].pszTemplate = MAKEINTRESOURCE(IDD_DIALOG4);
    psp[2].pfnDlgProc = (DLGPROC) EcoTabWndProc;
    psp[2].pszTitle = (LPSTR) "Eco";
    psp[2].lParam = 0;
    psp[2].pfnCallback = NULL;

    psp[3].dwSize = sizeof(PROPSHEETPAGE);
    psp[3].dwFlags = PSP_USETITLE;
	psp[3].hInstance = hInst;
    psp[3].pszTemplate = MAKEINTRESOURCE(IDD_DIALOG5);
    psp[3].pfnDlgProc = (DLGPROC) LandTabWndProc;
    psp[3].pszTitle = (LPSTR) "Land";
    psp[3].lParam = 0;
    psp[3].pfnCallback = NULL;

    psh.dwSize = sizeof(PROPSHEETHEADER);
    psh.dwFlags = PSH_PROPSHEETPAGE | PSH_NOCONTEXTHELP | PSH_USEHICON;
    psh.hwndParent = GetDesktopWindow();
    psh.hInstance = hInst;
	psh.hIcon = (HICON)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    psh.pszCaption = (LPSTR) "Scorched 3D Server Properties";
    psh.nPages = sizeof(psp) / sizeof(PROPSHEETPAGE);
    psh.nStartPage = 0;
    psh.ppsp = (LPCPROPSHEETPAGE) &psp;
    psh.pfnCallback = NULL;

	int result = (int) PropertySheet(&psh);
	return (result > 0);
}
#endif
