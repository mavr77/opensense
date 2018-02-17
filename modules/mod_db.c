#include "../opensense.h"

/*
  returns 0 if url not fount. or returns 1 if url in db in blacklist
*/
int db_blacklist_logic(char *blacklist, char url[URL_SIZE])
{
  char blacklist_url[URL_SIZE];
  sqlite3 *db = 0; // хэндл объекта соединение к БД
  sqlite3_stmt *res;

  char *SQL = "select * from blacklist where instr( ? ,url);";

  // rc - resource connection
  int rc = sqlite3_open(blacklist, &db);
  // executing SQL
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 0;
  }

  // -1 check the size of sql string on your own, we could do SQL_SIZE if db could calculate
  rc = sqlite3_prepare_v2(db, SQL, -1, &res, 0);

  if (rc == SQLITE_OK) {
    sqlite3_bind_text(res, 1, url, -1, SQLITE_STATIC);
  } else {
    fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    return 0;
  }

  // step is the same es exec, just executing by steps, by row not all rows at once. same as reading from file.
  while (sqlite3_step(res) == SQLITE_ROW)
  {
    return 1;
  }

  // close all structure and stuff in memory and db. Some dbs has is build in close some dont, read docs.
  sqlite3_finalize(res);

  // closing db connection
  sqlite3_close(db);

  return 0;
}
