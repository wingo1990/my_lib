#ifndef _DBTABLE_H_
#define _DBTABLE_H_

#include "Table.h"
#include "Field.h"
#include "mysql_public_iface.h"

class DBTable :	public Table
{
public:
	bool open_table(TableName);

	void close_table();

	bool create_table(TableName, std::string sclns);

	bool delete_table(TableName);

	bool insert_entry(std::string);

	bool add_column(std::string, FieldID id);

	bool delete_column(std::string);

	int update_entry(std::string, std::string);

	TableResultSet *select_entry(std::string);

	int delete_entry(std::string);

	DBTable(void): _table_name(""), _driver(0), _conn(0), _stmt(0){}
	~DBTable(void)
	{
		//_driver is a shared_ptr.

		if (_conn != 0) delete _conn;
		if (_stmt != 0) delete _stmt;
	}

	// the string must "host:xxxx db:xxxxx user:xxxxx passwd:xxxxx"
	static void Configure(std::string);

private:
	TableName _table_name;

	// it is a shared_ptr, can destroy auto.
	sql::Driver * _driver;

	//must be manufacture destroyed.
	sql::Connection *_conn;
	sql::Statement *_stmt;

	bool db_connect();

	bool db_create_table(const std::vector<Field>& vcln);

	bool db_execute(const std::string&);

	sql::ResultSet* db_execute_query(const std::string&);

	int db_execute_update(const std::string&);

	std::string db_field_name(const FieldID&);
	std::string db_field_type(const FieldID&);
	std::string db_kv_expression(const std::string&);
	std::string db_insert_values(const std::string&);

	void expression_interpret(const std::string&, std::string&, std::string&, std::string&);

	static std::string _host;
	static std::string _db;
	static std::string _user;
	static std::string _passwd;
};

#endif //_DBTABLE_H_
