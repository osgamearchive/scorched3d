drop table scorched3d_main, scorched3d_events, scorched3d_eventtypes, scorched3d_names, scorched3d_players, scorched3d_weapons;

create table if not exists scorched3d_main (
	name varchar(64),
	prefix varchar(64),
	games INTEGER NOT NULL DEFAULT 0,
	rounds INTEGER NOT NULL DEFAULT 0,
	PRIMARY KEY (name)
);

create table if not exists scorched3d_players (
	playerid INTEGER auto_increment,
	uniqueid varchar(64),
	name varchar(32),
	ipaddress varchar(32),
	osdesc varchar(32) NOT NULL DEFAULT '',
	lastconnected DATETIME,
	kills INTEGER NOT NULL DEFAULT 0,
	deaths INTEGER NOT NULL DEFAULT 0,
	selfkills INTEGER NOT NULL DEFAULT 0,
	teamkills INTEGER NOT NULL DEFAULT 0,
	connects INTEGER NOT NULL DEFAULT 0,
	shots INTEGER NOT NULL DEFAULT 0,
	wins INTEGER NOT NULL DEFAULT 0,
	overallwinner INTEGER NOT NULL DEFAULT 0,
	resigns INTEGER NOT NULL DEFAULT 0,
	gamesplayed INTEGER NOT NULL DEFAULT 0,
	timeplayed INTEGER NOT NULL DEFAULT 0,
	roundsplayed INTEGER NOT NULL DEFAULT 0,
	moneyearned INTEGER NOT NULL DEFAULT 0,
	PRIMARY KEY (playerid),
	UNIQUE (uniqueid)
);

create table if not exists scorched3d_weapons (
	weaponid INTEGER auto_increment,
	name varchar(64),
	description varchar(255) NOT NULL DEFAULT 'No Desc',
	cost INTEGER NOT NULL DEFAULT 0,
	bundlesize INTEGER NOT NULL DEFAULT 0,
	armslevel INTEGER NOT NULL DEFAULT 0,
	kills INTEGER NOT NULL DEFAULT 0,
	shots INTEGER NOT NULL DEFAULT 0,
	deathshots INTEGER NOT NULL DEFAULT 0,
	deathkills INTEGER NOT NULL DEFAULT 0,
	PRIMARY KEY (weaponid)
);

create table if not exists scorched3d_eventtypes (
	eventtype INTEGER,
	name varchar(32),
	PRIMARY KEY (eventtype)
);

create table if not exists scorched3d_events (
	eventid INTEGER auto_increment, 
	eventtype INTEGER,
	playerid INTEGER,
	otherplayerid INTEGER,
	weaponid INTEGER,
	eventtime DATETIME,
	PRIMARY KEY (eventid),
	FOREIGN KEY (playerid) REFERENCES scorched3d_players(playerid),
	FOREIGN KEY (eventtype) REFERENCES scorched3d_eventtypes(eventtype)
);

create table if not exists scorched3d_names (
	playerid INTEGER NOT NULL DEFAULT 0,
	name varchar(32) BINARY NOT NULL DEFAULT "",
	count INTEGER NOT NULL DEFAULT 0,
	PRIMARY KEY (playerid, name),
	FOREIGN KEY (playerid) REFERENCES scorched3d_players(playerid)
);

