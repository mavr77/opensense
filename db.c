#include "opensense.h"

/*
  returns 0 if url not fount. or returns 1 if url in db in blacklist
*/
int db_blacklist_logic(char *blacklist, char url[URL_SIZE])
{
  char blacklist_url[URL_SIZE];
  sqlite3 *db = 0; // хэндл объекта соединение к БД
  char *err = 0;

  char *SQL;

  if(sqlite3_open("blacklist_url.dblite", &db)){
    fprintf(stderr, "Error opening db: %s\n", sqlite3_errmsg(db));
    return 0;
  // executing SQL
  } else if(sqlite3_exec(db, SQL, 0, 0, &err))
  {
    fprintf(stderr, "Error SQL: %s\n", err);
    sqlite3_free(err);
    return 0;
  }
  if(){

  }
  // closing db connection
  sqlite3_close(db);

  return 0;
}
