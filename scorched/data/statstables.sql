drop table scorched3d_main, scorched3d_kills, scorched3d_names, scorched3d_players, scorched3d_weapons;

create table if not exists scorched3d_main (
	name varchar(32),
	games INTEGER NOT NULL DEFAULT 0,
	rounds INTEGER NOT NULL DEFAULT 0,
	PRIMARY KEY (name)
	);

create table if not exists scorched3d_players (
        playerid INTEGER auto_increment,
        uniqueid varchar(32),
        name varchar(32),
	lastconnected DATETIME,
        kills INTEGER NOT NULL DEFAULT 0,
        selfkills INTEGER NOT NULL DEFAULT 0,
        teamkills INTEGER NOT NULL DEFAULT 0,
        connects INTEGER NOT NULL DEFAULT 0,
        shots INTEGER NOT NULL DEFAULT 0,
        wins INTEGER NOT NULL DEFAULT 0,
        overallwinner INTEGER NOT NULL DEFAULT 0,
        resigns INTEGER NOT NULL DEFAULT 0,
	PRIMARY KEY (playerid),
	UNIQUE (uniqueid)
        );

create table if not exists scorched3d_weapons (
        weaponid INTEGER auto_increment,
	kills INTEGER NOT NULL DEFAULT 0,
        name varchar(64),
        description varchar(255) NOT NULL DEFAULT 'No Desc',
        PRIMARY KEY (weaponid)
        );

create table if not exists scorched3d_kills (
	killid INTEGER auto_increment, 
	killer INTEGER,
	killed INTEGER,
	weaponid INTEGER,
	killtime DATETIME,
	PRIMARY KEY (killid),
	FOREIGN KEY (killer) REFERENCES players(playerid),
	FOREIGN KEY (killed) REFERENCES players(playerid),
	FOREIGN KEY (weaponid) REFERENCES weapons(weaponid)
	);

create table if not exists scorched3d_names (
	playerid INTEGER NOT NULL DEFAULT 0,
	name varchar(64) BINARY NOT NULL DEFAULT "",
	count INTEGER NOT NULL DEFAULT 0,
	PRIMARY KEY (playerid, name),
	FOREIGN KEY (playerid) REFERENCES players(playerid)
	);

