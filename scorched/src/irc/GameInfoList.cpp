#include "irc/GameInfoList.h"

GameInfoList::GameInfoList()
{
	mutex_=SDL_CreateMutex();
	entries_ = 0;
}

GameInfoList::~GameInfoList()
{
	ClearEntries();
}

void GameInfoList::ClearEntries()
{
	SDL_LockMutex(mutex_);
	list_.clear();
	entries_=0;
	SDL_UnlockMutex(mutex_);
}

void GameInfoList::AddEntry(GameInfoEntry &entry)
{
	SDL_LockMutex(mutex_);
	list_.push_back(entry);
	entries_++;
	SDL_UnlockMutex(mutex_);
}


int  GameInfoList::Entries()
{
	return entries_;
}


GameInfoEntry GameInfoList::GetEntry(int item)
{
	static int last=-1;
	static std::list <GameInfoEntry>::iterator itor;
	if (item==last)
		return (*itor);
	SDL_LockMutex(mutex_);
	for ( itor=list_.begin();item--;itor++); 
	last=item;
	SDL_UnlockMutex(mutex_);
	return (*itor);
}
