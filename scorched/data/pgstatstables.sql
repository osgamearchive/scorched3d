drop table scorched3d_main, scorched3d_players, scorched3d_weapons, 
        scorched3d_eventtypes, scorched3d_events, scorched3d_names, 
        scorched3d_ipaddress, scorched3d_binary cascade;

create table scorched3d_main (
	name varchar(64),
	prefix varchar(64),
	published varchar(64),
	games integer not null default 0,
	rounds integer not null default 0,
	primary key (name)
);

create table scorched3d_binary (
	binaryid serial,
	name varchar(32) not null default '',
	crc integer not null default 0,
	length integer not null default 0,
	data bytea,
	primary key(binaryid)
);

create table scorched3d_players (
	playerid serial not null,
	uniqueid varchar(64),
	name varchar(32),
	ipaddress varchar(32),
	osdesc varchar(32) not null default '',
	lastconnected timestamp,
	kills integer not null default 0,
	deaths integer not null default 0,
	selfkills integer not null default 0,
	teamkills integer not null default 0,
	connects integer not null default 0,
	shots integer not null default 0,
	wins integer not null default 0,
	overallwinner integer not null default 0,
	resigns integer not null default 0,
	gamesplayed integer not null default 0,
	timeplayed integer not null default 0,
	roundsplayed integer not null default 0,
	moneyearned integer not null default 0,
	skill integer not null default 1000,
	avatarid integer default null
            references scorched3d_binary ( binaryid ),
	primary key (playerid),
	unique (uniqueid)
);

create table scorched3d_weapons (
	weaponid serial not null,
	name varchar(64),
	icon varchar(64),
	description varchar(255) not null default 'no desc',
	cost integer not null default 0,
	bundlesize integer not null default 0,
	armslevel integer not null default 0,
	kills integer not null default 0,
	shots integer not null default 0,
	deathshots integer not null default 0,
	deathkills integer not null default 0,
	primary key (weaponid)
);

create table scorched3d_eventtypes (
	eventtype integer,
	name varchar(32),
	primary key (eventtype)
);

create table scorched3d_events (
	eventid serial not null,
	eventtype integer references scorched3d_eventtypes on delete cascade,
	playerid integer references scorched3d_players on delete cascade,
	otherplayerid integer references scorched3d_players ( playerid ) 
            on delete cascade,
	weaponid integer references scorched3d_weapons on delete cascade,
	eventtime timestamp,
	primary key (eventid)
);

create table scorched3d_names (
	playerid integer not null default 0 references scorched3d_players,
	name varchar(32) not null default '',
	count integer not null default 0,
	primary key (playerid, name)
);

create table scorched3d_ipaddress (
	playerid integer not null default 0 references scorched3d_players,
	ipaddress varchar(32) not null default '',
	count integer not null default 0,
	primary key (playerid, ipaddress)
);

