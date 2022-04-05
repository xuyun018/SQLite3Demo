
#include "db_sqlite3.h"
//---------------------------------------------------------------------------
int db_sqlite3_business(sqlite3 *db, int enable, char **errmsg)
{
	const char *sqls[2] = { "begin;","commit;" };
	int index;
	int result;

	if (enable)
	{
		index = 0;
	}
	else
	{
		index = 1;
	}
	result = sqlite3_exec(db, sqls[index], NULL, NULL, errmsg);
	return(result);
}

int db_sqlite3_drop_table(sqlite3 *db, char *sql, int sqlsize, const char *tablename, char **errmsg)
{
	// "drop table if exists t"
	char sz_drop_table_if_exists[21] = { 'd', 'r', 'o', 'p', ' ', 't', 'a', 'b', 'l', 'e', ' ', 'i', 'f', ' ', 'e', 'x', 'i', 's', 't', 's', '\0' };
	int l;
	int offset = 0;
	int result;

	if (errmsg != NULL)
	{
		*errmsg = NULL;
	}

	if (0 < sqlsize)
	{
		sql[0] = '\0';
	}

	l = strlen(sz_drop_table_if_exists);
	if (offset + l < sqlsize)
	{
		strcpy(sql + offset, sz_drop_table_if_exists);
	}
	offset += l;
	if (offset + 1 < sqlsize)
	{
		sql[offset] = ' ';

		sql[offset + 1] = '\0';
	}
	offset++;
	l = strlen(tablename);
	if (offset + l < sqlsize)
	{
		strcpy(sql + offset, tablename);
	}
	//offset += l;
	result = sqlite3_exec(db, sql, NULL, NULL, errmsg);
	return(result);
}
int db_sqlite3_create_table(sqlite3 *db, char *sql, int sqlsize, struct sqlite3_table_entry *pte, char **errmsg)
{
	// create table
	char sz_create_table[13] = { 'c', 'r', 'e', 'a', 't', 'e', ' ', 't', 'a', 'b', 'l', 'e', '\0' };
	int l0, l1, l2;
	int i;
	int result = -1;

	l0 = 0;
	l1 = 13 - 1;
	if (l0 + l1 < sqlsize)
	{
		strcpy(sql + l0, sz_create_table);
	}
	l0 += l1;
	if (l0 + 1 < sqlsize)
	{
		sql[l0] = ' ';
	}
	l0++;
	if (pte->name != NULL)
	{
		l1 = strlen(pte->name);
		if (l0 + l1 < sqlsize)
		{
			strcpy(sql + l0, pte->name);
		}
		l0 += l1;
		if (l0 + 1 < sqlsize)
		{
			sql[l0] = '(';
		}
		l0++;
		l2 = l0;
		i = 0;
		while (pte->column[i].name != NULL && pte->column[i].name[0] != '\0' && l0 < sqlsize)
		{
			l1 = strlen(pte->column[i].name);
			if (l0 + l1 < sqlsize)
			{
				strcpy(sql + l0, pte->column[i].name);
			}
			l0 += l1;
			if (pte->column[i].definition != NULL && pte->column[i].definition[0] != '\0')
			{
				if (l0 + 1 < sqlsize)
				{
					sql[l0] = ' ';
				}
				l0++;
				l1 = strlen(pte->column[i].definition);
				if (l0 + l1 < sqlsize)
				{
					strcpy(sql + l0, pte->column[i].definition);
				}
				l0 += l1;
			}
			if (l0 + 1 < sqlsize)
			{
				sql[l0] = ',';
			}
			l2 = l0;
			l0++;

			i++;
		}
		l0 = l2;
		if (l0 + 1 < sqlsize)
		{
			sql[l0] = ')';
		}
		l0++;
		if (l0 + 1 < sqlsize)
		{
			sql[l0] = '\0';
		}
		l0++;
		if (l0 <= sqlsize)
		{
			result = sqlite3_exec(db, sql, NULL, NULL, errmsg);
		}
	}
	return(result);
}

int db_sqlite3_prepare_statement(sqlite3 *db, sqlite3_stmt **stmt, char *sql, int sqlsize, struct sqlite3_table_entry *pte)
{
	// insert into
	char sz_insert_into[12] = { 'i', 'n', 's', 'e', 'r', 't', ' ', 'i', 'n', 't', 'o', '\0' };
	char sz_values[7] = { 'v', 'a', 'l', 'u', 'e', 's', '\0' };
	int l0, l1, l2;
	int i;
	int result = -1;

	l0 = 0;
	l1 = 12 - 1;
	if (l0 + l1 < sqlsize)
	{
		strcpy(sql + l0, sz_insert_into);
	}
	l0 += l1;
	if (l0 + 1 < sqlsize)
	{
		sql[l0] = ' ';
	}
	l0++;
	if (pte->name != NULL)
	{
		l1 = strlen(pte->name);
		if (l0 + l1 < sqlsize)
		{
			strcpy(sql + l0, pte->name);
		}
		l0 += l1;
		if (l0 + 1 < sqlsize)
		{
			sql[l0] = '(';
		}
		l0++;
		l2 = l0;
		i = 0;
		while (pte->column[i].name != NULL && pte->column[i].name[0] != '\0' && l0 < sqlsize)
		{
			l1 = strlen(pte->column[i].name);
			if (l0 + l1 < sqlsize)
			{
				strcpy(sql + l0, pte->column[i].name);
			}
			l0 += l1;
			if (l0 + 1 < sqlsize)
			{
				sql[l0] = ',';
			}
			l2 = l0;
			l0++;

			i++;
		}
		l0 = l2;
		if (l0 + 1 < sqlsize)
		{
			sql[l0] = ')';
		}
		l0++;
		if (l0 + 1 < sqlsize)
		{
			sql[l0] = ' ';
		}
		l0++;
		l1 = 7 - 1;
		if (l0 + l1 < sqlsize)
		{
			strcpy(sql + l0, sz_values);
		}
		l0 += l1;
		if (l0 + 1 < sqlsize)
		{
			sql[l0] = '(';
		}
		l0++;
		if (l0 + i + i < sqlsize)
		{
			l2 = l0;
			l1 = i;
			while (l1 > 0)
			{
				l1--;

				sql[l0++] = '?';
				l2 = l0;
				sql[l0++] = ',';
			}
			l0 = l2;
		}
		else
		{
			l0 += i + i;
		}
		if (l0 + 1 < sqlsize)
		{
			sql[l0] = ')';
		}
		l0++;
		if (l0 + 1 < sqlsize)
		{
			sql[l0] = '\0';
		}
		l0++;
		if (l0 <= sqlsize)
		{
			result = sqlite3_prepare_v2(db, sql, -1, stmt, NULL);
		}
		else
		{
			result = l0;
		}
	}
	return(result);
}

unsigned char *db_sqlite3_bind_buffer(sqlite3_stmt *stmt,
	int i,
	const wchar_t *text
)
{
	unsigned char *result = NULL;
	int l = 0;

	if (text != NULL && text[0] != '\0')
	{
		l = utf8_encode(NULL, 0, text, NULL);
		result = (unsigned char *)MALLOC(l);
		if (result != NULL)
		{
			utf8_encode(result, l, text, NULL);

			//l--;
		}
		else
		{
			l = 0;
		}
	}
	sqlite3_bind_text(stmt, i, (const char *)result, l, NULL);
	return(result);
}
//---------------------------------------------------------------------------