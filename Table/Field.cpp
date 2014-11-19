#include <regex>
#include "Field.h"

using namespace std;

Field::Field(void)
{

}

Field::Field(const string& name, const string& type): _name(name), _type(TableElement::string2type(type))
{

}

Field::Field(const std::string& str)
{
	string temp = str;
	regex reg("\\s*([^\\s]*)\\s*-\\s*([^\\s]*)\\s*");
	smatch sm;
	regex_match(temp, sm, reg);
	string name = sm[1];
	string type = sm[2];

	_name = name;
	_type = TableElement::string2type(type);
}

Field::~Field(void)
{
}

ElementType Field::get_type() const
{
	return _type;
}

std::string Field::get_name() const
{
	return _name;
}

void Field::set_type(const ElementType& type)
{
	_type = type;
}

void Field::set_name(const std::string& name)
{
	_name = name;
}

string Field::str() 
{
	return _name + "-" + TableElement::type2string(_type);
}

bool Field::is_field_string(const std::string& str)
{
	string temp = str;
	regex reg("\\s*([^\\s]*)\\s*-\\s*([^\\s]*)\\s*");
	smatch sm;
	regex_match(temp, sm, reg);
	string name = sm[1];
	string type = sm[2];

	return sm.size() == 3 && TableElement::is_type_string(type);
}
