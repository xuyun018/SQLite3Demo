#ifndef DB_SQLITE3_H
#define DB_SQLITE3_H
//---------------------------------------------------------------------------
// 封装的 sqlite3 库
//---------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS

#include "utf8.h"

#include "sqlite3.h"
//---------------------------------------------------------------------------
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
//---------------------------------------------------------------------------
// Sqlite3表入口结构体
struct sqlite3_table_column_entry
{
	const char *name;
	const char *definition;
	int type;
};
// Sqlite3表结构体
struct sqlite3_table_entry
{
	const char *name;

	struct sqlite3_table_column_entry *column;
};
//---------------------------------------------------------------------------
// 删除表,用不着
int db_sqlite3_drop_table(sqlite3 *db, char *sql, int sqlsize, const char *tablename, char **errmsg);
int db_sqlite3_create_table(sqlite3 *db, char *sql, int sqlsize, struct sqlite3_table_entry *pte, char **errmsg);

int db_sqlite3_prepare_statement(sqlite3 *db, sqlite3_stmt **stmt, char *sql, int sqlsize, struct sqlite3_table_entry *pte);

unsigned char *db_sqlite3_bind_buffer(sqlite3_stmt *stmt,
	int i,
	const wchar_t *text
);

int db_sqlite3_business(sqlite3 *db, int enable, char **errmsg);
//---------------------------------------------------------------------------
#endif