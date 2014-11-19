#include <string>
#include <sstream>
#include <cassert>

#include <mysql_public_iface.h>
#include <regex>

#include "DBTable.h"
#include "DBTableResultSet.h"

using namespace std;

#define MYSQL_VARCHAR "varchar(45)"

string DBTable::_host = "127.0.0.1";
string DBTable::_db = "test";
string DBTable::_user = "root";
string DBTable::_passwd = "mysql";

void DBTable::Configure(std::string conf)
{
	regex reg("\\s*host=(.*),\\s*db=(.*),\\s*user=(.*),\\s*passwd=(.+)\\s*");
	smatch sm;
	regex_match(conf, sm, reg);

	_host = sm[1];
	_db = sm[2];
	_user = sm[3];
	_passwd = sm[4];
}


bool DBTable::open_table(TableName tn)
{
	_table_name = tn;

	if (!db_connect())
		return false;
 	
	// try to check the table is exist.
	sql::ResultSet*res = _stmt->executeQuery("SHOW TABLES LIKE '%" + _table_name + "%'");

	while(res->next()) 
		return true;

	return false;
}

void DBTable::close_table()
{
	// nothing.
}

bool DBTable::create_table(TableName table_name, std::string fields_str)
{
	_table_name = table_name;

	if (!db_connect())
		return false;

	vector<Field> field_vec;

	regex reg("\\s+");
	sregex_token_iterator it(fields_str.begin(), fields_str.end(), reg, -1);
	sregex_token_iterator end;

	while(it != end)
	{
		string temp = it->str();
		if (temp.size() != 0)
		{
			if (Field::is_field_string(temp))
			{
				Field field(temp);
				field_vec.push_back(field);
			}
			else
				return false;
		}

		it++;
	}

	return db_create_table(field_vec);
}

bool DBTable::delete_table(TableName tn)
{
	if (!db_connect())
		return false;

	string command = "DROP TABLE IF EXISTS " + tn;

	return db_execute(command);
}

bool DBTable::insert_entry(std::string se)
{
	string command = "INSERT " + _db + "." + _table_name + " VALUES (" + db_insert_values(se) + ")";

	return db_execute(command);
}

bool DBTable::add_column(std::string field_str, FieldID id)
{
	assert(Field::is_field_string(field_str));
	if (!Field::is_field_string(field_str))
		return false;

	Field field(field_str);

	string name = field.get_name();
	string type = TableElement::type2string(field.get_type());

	if (type == "string")
		type = MYSQL_VARCHAR;

	string dot = "`";
	string command = "ALTER TABLE " + dot + _table_name + dot + " ADD " + dot + name + dot + " " + type + " ";

	if (id == 0)
		command += "FIRST";
	else if (id > 0)
	{

		command += "AFTER `" + db_field_name(id) + "`";
	}
	else
	{
		//nothing, will append the column.
	}

	return db_execute(command);
}

bool DBTable::delete_column(std::string fieldname)
{
	string command = "ALTER TABLE " + _table_name + " DROP " + fieldname;

	return db_execute(command);
}

int DBTable::update_entry(std::string strset, std::string strwhere)
{
	string command = "UPDATE " + _table_name + " SET " + db_kv_expression(strset) + " WHERE " + db_kv_expression(strwhere);

	return db_execute_update(command);
}

TableResultSet * DBTable::select_entry(std::string str)
{
	string command = "SELECT * FROM " + _table_name + " WHERE " + db_kv_expression(str);
	sql::ResultSet *res = db_execute_query(command);

	DBTableResultSet *rp = new DBTableResultSet(res);

	return rp;
}

int DBTable::delete_entry(std::string wherestr)
{
	string command = "DELETE FROM " + _table_name + " WHERE " + db_kv_expression(wherestr);
	return db_execute_update(command);
}

bool DBTable::db_create_table(const std::vector<Field>& filed_vec)
{
	size_t n = filed_vec.size();
	if (n == 0)
		return false;

	string dot = "`";
	string command = "CREATE TABLE ";
	command = command + dot + _db + dot + "." + dot + _table_name + dot + " (";

	for (int i=0; i<n; i++)
	{
		command += dot + filed_vec[i].get_name() + dot + " ";
		string type = TableElement::type2string(filed_vec[i].get_type());
		if (type == "string")
			type = MYSQL_VARCHAR;

		command += type + (i==n-1 ? ")":", ");
	}

	return db_execute(command);
}

bool DBTable::db_connect()
{
	// if connected to db.
	if (_driver != NULL && _conn != NULL && _stmt != NULL)
		return true;

	try {
		_driver = get_driver_instance();

		/* Using the Driver to create a connection */
		_conn = _driver->connect(_host, _user, _passwd);

		/* Creating a "simple" statement - "simple" = not a prepared statement */
		_stmt = _conn->createStatement();

		// connect to database.
		_stmt->execute("USE " + _db);

	}  catch (sql::SQLException &e) {
		/*
		The MySQL Connector/C++ throws three different exceptions:

		- sql::MethodNotImplementedException (derived from sql::SQLException)
		- sql::InvalidArgumentException (derived from sql::SQLException)
		- sql::SQLException (derived from std::runtime_error)
		*/
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;

		return false;
	} catch (std::runtime_error &e) {

		cout << "# ERR: runtime_error in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what() << endl;

		return false;
	}

	return true;
}

bool DBTable::db_execute(const string& command)
{
	try {

		_stmt->execute(command);
	
	} catch (sql::SQLException &e) {
		/*
		The MySQL Connector/C++ throws three different exceptions:

		- sql::MethodNotImplementedException (derived from sql::SQLException)
		- sql::InvalidArgumentException (derived from sql::SQLException)
		- sql::SQLException (derived from std::runtime_error)
		*/
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;

		return false;
	} catch (std::runtime_error &e) {

		cout << "# ERR: runtime_error in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what() << endl;

		return false;
	}

	return true;
}

sql::ResultSet* DBTable::db_execute_query(const string& command)
{
	try {

		return _stmt->executeQuery(command);
	
	} catch (sql::SQLException &e) {
		/*
		The MySQL Connector/C++ throws three different exceptions:

		- sql::MethodNotImplementedException (derived from sql::SQLException)
		- sql::InvalidArgumentException (derived from sql::SQLException)
		- sql::SQLException (derived from std::runtime_error)
		*/
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;

		return NULL;
	} catch (std::runtime_error &e) {

		cout << "# ERR: runtime_error in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what() << endl;

		return NULL;
	}
}

int DBTable::db_execute_update(const string& command)
{
	try {

		return _stmt->executeUpdate(command);
	
	} catch (sql::SQLException &e) {
		/*
		The MySQL Connector/C++ throws three different exceptions:

		- sql::MethodNotImplementedException (derived from sql::SQLException)
		- sql::InvalidArgumentException (derived from sql::SQLException)
		- sql::SQLException (derived from std::runtime_error)
		*/
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;

		return NULL;
	} catch (std::runtime_error &e) {

		cout << "# ERR: runtime_error in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what() << endl;

		return NULL;
	}
}


std::string DBTable::db_field_name(const FieldID& id)
{
	assert(id > 0);

	sql::ResultSet *res = db_execute_query("show fields from " + _table_name);
	
	unsigned int num = id;
	while (res->next())
	{
		num --;
		if (num == 0)
			break;
	}

	assert(!res->isAfterLast());
	if (res->isAfterLast())
		return "";

	string fieldname = res->getString("Field");

	return fieldname;
}

std::string DBTable::db_field_type(const FieldID& id)
{
	sql::ResultSet *res = db_execute_query("show fields from " + _table_name);

	unsigned int num = id;
	while (res->next())
	{
		if (num == 0)
			break;
		num --;
	}

	if (res->isAfterLast())
		return "";

	string fieldtype = res->getString("Type");

	return fieldtype;
}


std::string DBTable::db_kv_expression(const std::string& str)
{
	string key, op, val;
	expression_interpret(str, key, op, val);
	string exp = "`" + key + "`" + "=" + "\"" + val + "\"";

	return exp;
}

std::string DBTable::db_insert_values(const std::string& str)
{
	string values_str;
	regex reg("\\s+");
	sregex_token_iterator it(str.begin(), str.end(), reg, -1);
	sregex_token_iterator end;

	int num = 0;
	while(it != end)
	{
		if (it->str().size())
		{
			if (db_field_type(num) == MYSQL_VARCHAR)
				values_str += "\"" + it->str() + "\"" + ",";
			else
				values_str += it->str() + ",";
		
			num ++;
		}
		it++;
	}

	values_str.erase(values_str.size()-1);

	return values_str;
}

void DBTable::expression_interpret(const std::string& str, std::string& key, std::string& op, std::string& val)
{
	string temp = str;

	regex reg("\\s*(.*?)\\s*(>|<|=)\\s*(.*?)\\s*");
	smatch sm;
	
	regex_match(temp, sm, reg);

	key = sm[1];
	op = sm[2];
	val = sm[3];
}