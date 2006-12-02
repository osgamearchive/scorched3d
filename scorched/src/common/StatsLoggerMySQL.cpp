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

#ifdef HAVE_MYSQL

#include <common/StatsLoggerMySQL.h>
#include <common/Logger.h>

StatsLoggerMySQL::StatsLoggerMySQL() : mysql_(0)
{

}

StatsLoggerMySQL::~StatsLoggerMySQL()
{
}

bool StatsLoggerMySQL::runQuery(const char *fmt, ...)
{
	if (!success_) return false;

	static char text[50000];
	va_list ap;
	va_start(ap, fmt);
	int used = vsnprintf(text, 50000, fmt, ap);
	va_end(ap);

	if (used == 50000) return false;
	return (mysql_real_query(mysql_, text, strlen(text)) == 0);
}

std::list<StatsLoggerDatabase::RowResult> StatsLoggerMySQL::runSelectQuery(const char *fmt, ...)
{
	std::list<StatsLoggerDatabase::RowResult> results;
	if (!success_) return results;

	static char text[50000];
	va_list ap;
	va_start(ap, fmt);
	int used = vsnprintf(text, 50000, fmt, ap);
	va_end(ap);

	if (used == 50000) return results;
	if (mysql_real_query(mysql_, text, strlen(text)) != 0) return results;

	MYSQL_RES *result = mysql_store_result(mysql_);
	if (result)
	{
		int rows = (int) mysql_num_rows(result);
		int cols = (int) mysql_num_fields(result);
		for (int r=0; r<rows; r++)
		{
			StatsLoggerDatabase::RowResult rowResult;
			MYSQL_ROW row = mysql_fetch_row(result);

			for (int c=0; c<cols; c++)
			{
				rowResult.columns.push_back(row[c]);
			}

			results.push_back(rowResult);
		}
		mysql_free_result(result);
	}

	return results;
}

bool StatsLoggerMySQL::connectDatabase(const char *host, const char *user, 
	const char *passwd, const char *db)
{
    mysql_ = mysql_init(0);
	if (!mysql_)
	{
		Logger::log( "Failed to init mysql");
		return false;
	}

	if (!mysql_real_connect(
		mysql_,
		host,
		user,
		passwd,
		db,
		0, "/tmp/mysql.sock", 0))
	{
		Logger::log(formatString("mysql stats logger failed to start. "
			"Error: %s",
			mysql_error(mysql_)));
		Logger::log(formatString("mysql params : host %s, user %s, passwd %s, db %s",
			host, user, passwd, db));
		return false;
	}

	return true;
}

int StatsLoggerMySQL::getLastInsertId()
{
	return (int) mysql_insert_id(mysql_);
}

void StatsLoggerMySQL::escapeString(char *to, const char *from, unsigned long length)
{
	mysql_real_escape_string(mysql_, to, from, length);
}

#endif // HAVE_MYSQL
