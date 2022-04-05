#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>

#include "database/db_sqlite3.h"

#include <list>
#include <string>

using namespace std;

int db_write_strings(sqlite3* db, const char* tablename,
	list<wstring> *string_list)
{
	struct sqlite3_table_column_entry tces[] = {
		{ "id", "int", 0 },
		{ "cs", "varchar", 0 },

		{ NULL, NULL, 0 }
	};
	struct sqlite3_table_entry pte[1] = {
		{ NULL, tces }
	};
	sqlite3_stmt* stmt;
	char sql[2048];
	list<wstring>::iterator it;
	unsigned int i;
	unsigned int id;
	int result = 0;
	unsigned char* buffer0;

	pte->name = tablename;

	db_sqlite3_drop_table(db, sql, sizeof(sql), pte->name, NULL);

	db_sqlite3_create_table(db, sql, sizeof(sql), pte, NULL);
	{
		db_sqlite3_business(db, 1, NULL);

		if (db_sqlite3_prepare_statement(db, &stmt, sql, sizeof(sql), pte) == 0)
		{
			id = 0;

			for (it = string_list->begin(); it != string_list->end(); ++it)
			{
				i = 1;

				sqlite3_bind_int(stmt, i++, id++);
				buffer0 = db_sqlite3_bind_buffer(stmt, i++, it->c_str());

				sqlite3_step(stmt);
				sqlite3_reset(stmt);

				if (buffer0)
				{
					FREE(buffer0);
				}

				result++;
			}

			sqlite3_finalize(stmt);
		}

		db_sqlite3_business(db, 0, NULL);
	}

	return(result);
}
int db_read_strings(sqlite3* db, const char* tablename,
	list<wstring>* string_list)
{
	sqlite3_stmt* stmt;
	char sql[2048];
	unsigned int i;
	unsigned int j;
	unsigned int l;
	int result = 0;
	unsigned int id;
	WCHAR text[256];
	const unsigned char* buffer;

	j = 0;
	strcpy(sql + j, "select");
	j += strlen(sql + j);
	sql[j++] = ' ';
	strcpy(sql + j, "id");
	j += strlen(sql + j);
	sql[j++] = ',';
	strcpy(sql + j, "cs");
	j += strlen(sql + j);
	//
	sql[j++] = ' ';
	strcpy(sql + j, "from");
	j += strlen(sql + j);
	sql[j++] = ' ';
	strcpy(sql + j, tablename);

	//db_sqlite3_business(db, 1, NULL);

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == 0)
	{
		//db_sqlite3_reset(stmt);
		while (sqlite3_step(stmt) == 100)
		{
			i = 0;

			id = (unsigned int)sqlite3_column_int(stmt, i++);

			text[0] = L'\0';
			buffer = (const unsigned char*)sqlite3_column_text(stmt, i++);
			if (buffer)
			{
				utf8_decode(NULL, &l, 0, buffer, 0);
				if (l <= sizeof(text) / sizeof(text[0]))
				{
					utf8_decode(text, &l, l, buffer, 0);

					string_list->push_back(text);
				}
			}

			result++;
		}

		sqlite3_finalize(stmt);
	}

	//db_sqlite3_business(db, 0, NULL);

	return(result);
}

int wmain(int argc, WCHAR* argv[])
{
	sqlite3* db;
	list<wstring> string_list;
	list<wstring>::iterator it;
	WCHAR filename[1024];
	unsigned int k;

	k = GetModuleFileName(NULL, filename, sizeof(filename) / sizeof(filename[0]));
	if (k && k < sizeof(filename) / sizeof(filename[0]))
	{
		while (k)
		{
			k--;

			if (filename[k] == '\\' || filename[k] == '/')
			{
				k++;

				break;
			}
		}

		wcscpy(filename + k, L"sqlite3_demo.db");

		char utf8_filename[1024];
		unsigned int cslength;
		unsigned int l;

		l = utf8_encode((unsigned char*)utf8_filename, sizeof(utf8_filename), filename, &cslength);
		if (l && l <= sizeof(utf8_filename))
		{
			string_list.push_back(L"1");
			string_list.push_back(L"2");
			string_list.push_back(L"3");
			string_list.push_back(L"4");
			string_list.push_back(L"5");
			string_list.push_back(L"6");
			string_list.push_back(L"7");
			string_list.push_back(L"8");
			string_list.push_back(L"9");
			string_list.push_back(L"0");

			if (sqlite3_open(utf8_filename, &db) == 0)
			{
				db_write_strings(db, "tb_demo",
					&string_list);

				sqlite3_close(db);
			}

			string_list.clear();

			if (sqlite3_open(utf8_filename, &db) == 0)
			{
				db_read_strings(db, "tb_demo",
					&string_list);

				sqlite3_close(db);
			}

			for (it = string_list.begin(); it != string_list.end(); ++it)
			{
				wprintf(L"%s\r\n", it->c_str());
			}
		}
	}

	getchar();

	return(0);
}