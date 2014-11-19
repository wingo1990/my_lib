#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include "Table.h"
#include "TableResultSet.h"

int main()
{
	Table* table = GetTableInstance();

	//if (!table->create_table("contacts","name-string age-int phone-string address-string money-double"))
	//{
	//	cout << "create table error." << endl;
	//	return -1;
	//}

	if (!table->open_table("contacts"))
	{
		cout << "open table fail." << endl;
		return -1;
	}

	//cout << table->insert_entry("zhangwentao 24 18505817253 anhuimaanshan 100.0") << endl;
	//cout << table->insert_entry("zhanghua 25 18897654321 anhuizongyang 45.9") << endl;
	//cout << table->insert_entry("zhangyu 23 18890876789 anhuimengcheng 56.7") << endl;
	//cout << table->insert_entry("zhangfeng 25 1850588976 anhuidangtu 43.8") << endl;

	//cout << table->delete_entry(" money = 45.9 ") << endl;

	table->update_entry("name=zhanghua", "money = 100");
	table->update_entry("age=23", "money = 100");
	table->update_entry("name=zhangfeng", "money = 45.9");
	table->update_entry("age=24", "money = 45.9");
	table->update_entry("name=wangdi", "money = 56.7");
	table->update_entry("age=28", "money = 56.7");
	table->update_entry("name=zhang", "money = 43.8");
	table->update_entry("age=20", "money = 43.8 ");


	//table->delete_column("name");
	//table->delete_column("age");
	//table->add_column("age -int ", 1);



	//TableResultSet *res = table->select_entry(" age = 25 ");
	//res->last();
	//{
	//	double money;
	//	int age;
	//	string name, address, phone;
	//	name = res->getString("name");
	//	age = res->getInt("age");
	//	phone = res->getString("phone");
	//	address = res->getString("address");
	//	money = res->getDouble("money");

	//	cout << name << " " << age << " " << phone << " " << address << " " << money << endl;
	//}

	table->close_table();

	return 0;
}