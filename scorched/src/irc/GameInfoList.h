#ifndef _GAMEINFOLIST_H_
#define _GAMEINFOLIST_H_

#include <SDL/SDL_mutex.h>

#include <list>
#include "irc/GameInfoEntry.h"

class GameInfoList {

      private:
	std::list<GameInfoEntry> list_;
	SDL_mutex *mutex_;
	int entries_;
	
      public:
      	GameInfoList();
	~GameInfoList();
	void ClearEntries();
	void AddEntry(GameInfoEntry &entry);	
	GameInfoEntry GetEntry(int item);
	int Entries();
};

#endif				/* _GAMEINFOLIST_H_ */
