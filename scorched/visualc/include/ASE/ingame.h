/* CALLBACK FUNCTIONS (the game has to implement these) */

void ASEBrowser_invalidate(void);
/* called when some data in the serverlist has changed and the list should be redrawn */

void ASEBrowser_progress(void);
/* called when progress indicator position has changed */


/* INITIALIZATION */

extern int ASEBrowser_init(void);
/* has to be called before calling any other functions */


/* EXPORTED DATATYPES */

extern int ASEBrowser_progresspos;
/* progressbar position for the current operation 0-4096 */

extern char *ASEBrowser_statusmessage;
/* status message, linked to progress position */

extern int ASEBrowser_servers;
/* number of servers currently in the server list */

extern int ASEBrowser_pinging;
/* flag indicating whether we are pinging or idle */


/* GENERAL OPERATIONS */

extern void ASEBrowser_setconfig(int upstreambandwidth, int downstreambandwidth, int packetheadersize, int packetspersec);
/* used to set connection specific settings */

extern int ASEBrowser_refresh(char *data);
/*data is ASE filter code (used for server side filtering, explained elsewhere).
Pass an empty string "" to get all internet servers.*/

extern int ASEBrowser_refreshlan(int port);
/* call to find and refresh LAN servers */
/* port = default join port for the game */

/* returns non-zero on success */

extern void ASEBrowser_cancel(void);
/* stops the current refresh */


/* SERVER LIST OPERATIONS */

extern char *ASEBrowser_getinfo(int servernum, char *key);
/* gets a serverinfo value for the selected server, key = "hostname", "ping", "game", "map", "clients", "maxclients", "country" or any other serverinfo/rule key */
/* returns NULL on error */

extern void ASEBrowser_setsortcolumn(char *column);
/* sets the sort key for the server list, column = "hostname", "ping", "game", "map", "flux", "pl", "players", "country", "address") */


/* PLAYER INFO OPERATIONS */

extern int ASEBrowser_getfirstplayer(int servernum);
/* sets the internal pointers to the first player of server number servernum */
/* returns non-zero on success */

extern int ASEBrowser_getnextplayer(void);
/* sets the internal pointers to the next player for the current server */
/* returns non-zero on success */

extern char *ASEBrowser_getplayerinfo(char *key);
/* gets a player info string for the selected player, key = "name", "score", "ping", "team", "skin" or "time") */
/* returns NULL on error */


/* SERVER RULE OPERATIONS */

extern int ASEBrowser_getfirstrule(int servernum);
/* sets the internal pointers to the first rule of server number servernum */
/* returns non-zero on success */

extern int ASEBrowser_getnextrule(void);
/* sets the internal pointers to the next rule for the current server */
/* returns non-zero on success */

extern char *ASEBrowser_getrulekey(void);
/* gets the name of the selected rule */
/* returns NULL on error */

extern char *ASEBrowser_getrulevalue(void);
/* gets the value of the selected rule */
/* returns NULL on error */
