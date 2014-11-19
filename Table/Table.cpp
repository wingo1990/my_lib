#include "Table.h"
#include "FileTable.h"
#include "DBTable.h"

Table *GetTableInstance(XTable xt)
{
	if (xt == FILE_TABLE)
		return new FileTable();
	else if (xt == DB_TABLE)
		return new DBTable();
	else
		return NULL;
}

void TableConfiure(std::string conf, XTable xt)
{
	if (xt == FILE_TABLE)
	{
		FileTable::Configure(conf);
	} 
	else if (xt == DB_TABLE)
	{
		DBTable::Configure(conf);
	}
}