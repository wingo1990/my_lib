#include <string>
#include <sstream>
#include <cassert>

#include "TableElement.h"

using namespace std;

TableElement::TableElement(const TableElement &ele):_eletype(ele._eletype), _eleptr(0)
{
	if (_eletype == ElementTypeNULL)
		return;

	_eleptr = get_element_space();
	copy_element_space(_eleptr, ele._eleptr);
}

TableElement::TableElement(const int &i):_eletype(ElementInt), _eleptr(0)
{
	_eleptr = get_element_space();
	*(int*)_eleptr = i;
}

TableElement::TableElement(const double &d):_eletype(ElementDouble), _eleptr(0)
{
	_eleptr = get_element_space();
	*(double*)_eleptr = d;
}

TableElement::TableElement(const std::string &str):_eletype(ElementString), _eleptr(0)
{
	_eleptr = get_element_space();
	*(std::string*)_eleptr = str;
}

TableElement::TableElement(const ElementType type, std::string str /* = "0" */):_eletype(type), _eleptr(0)
{
	switch (_eletype)
	{
	case ElementInt:
		{
			_eleptr = get_element_space();
			*(int*)_eleptr = string2as_<int>(str);
			break;
		}
	case ElementDouble:
		{
			_eleptr = get_element_space();
			*(double*)_eleptr = string2as_<double>(str);
			break;
		}
	case ElementString:
		{
			_eleptr = get_element_space();
			*(std::string*)_eleptr = string2as_<std::string>(str);
			break;
		}
	default:
		// ElementTypeNull will do nothing. ptr=0.
		break;
	}
}

TableElement::~TableElement()
{
	if (_eleptr != 0)
		delete _eleptr;
}

std::string TableElement::get_value_as_string()
{
	ostringstream oss;

	switch (_eletype)
	{
	case ElementInt:
		{
			oss << *(int*)_eleptr;
			break;
		}
	case ElementDouble:
		{
			oss << *(double*)_eleptr;
			break;
		}
	case ElementString:
		{
			oss << *(string*)_eleptr;
			break;
		}
	case ElementTypeNULL:
		{
			oss << "NULL";
			break;
		}
	default:
		break;
	}

	return oss.str();
}

int TableElement::get_value_as_int()
{
	int temp = 0;
	switch (_eletype)
	{
	case ElementInt:
		{
			temp = *(int*)_eleptr;
			break;
		}
	case ElementDouble:
		{
			temp = (int)*(double*)_eleptr;
			break;
		}
	case ElementString:
		{
			istringstream iss(*(string*)_eleptr);
			iss >> temp;
			break;
		}
	case ElementTypeNULL:
		{
			temp = 0;
			break;
		}
	default:
		break;
	}

	return temp;
}

double TableElement::get_value_as_double()
{
	double temp = 0;
	switch (_eletype)
	{
	case ElementInt:
		{
			temp = *(int*)_eleptr;
			break;
		}
	case ElementDouble:
		{
			temp = *(double*)_eleptr;
			break;
		}
	case ElementString:
		{
			istringstream iss(*(string*)_eleptr);
			iss >> temp;
			break;
		}
	case ElementTypeNULL:
		{
			temp = 0;
			break;
		}
	default:
		break;
	}

	return temp;
}

ElementType TableElement::get_element_type()
{
	return _eletype;
}

TableElement &TableElement::operator=(const TableElement &ele)
{
	_eletype = ele._eletype;

	_eleptr = get_element_space();

	copy_element_space(_eleptr, ele._eleptr);

	return *this;
}

std::ostream &operator<<(std::ostream &out, const TableElement &ele)
{
	ele.printout(out);

	return out;
}
std::istream &operator>>(std::istream &in, TableElement &ele)
{
	assert(ele._eletype != ElementTypeNULL);
	assert(ele._eleptr != NULL);
	//ele._eleptr = ele.get_element_space();
	ele.input(in);
	return in;
}

bool TableElement::operator==(const int& n)
{
	return (n == *(int*)_eleptr);
}

bool TableElement::operator==(const std::string &str)
{
	return (str == *(std::string*)_eleptr);
}

bool TableElement::operator==(const double& d)
{
	return (d == *(double*)_eleptr);
}

bool TableElement::operator==(const TableElement &tele)
{
	assert(_eletype == tele._eletype);

	return compare_equal(tele._eleptr);
}

bool TableElement::operator<(const TableElement& tele)
{
	assert(_eletype == tele._eletype);

	return compare_less(tele._eleptr);
}

bool TableElement::operator>(const TableElement& tele)
{
	assert(_eletype == tele._eletype);

	return compare_greater(tele._eleptr);
}

void* TableElement::get_element_space()
{
	if (_eleptr != 0)
	{
		delete _eleptr;
		_eleptr = 0;
	}

	switch (_eletype)
	{
	case ElementInt:
		return new int;
		break;
	case ElementDouble:
		return new double;
		break;
	case ElementString:
		return new std::string;
		break;
	default:
		break;
	}

	return 0;
}

void TableElement::copy_element_space(void *dst, void *src)
{
	switch (_eletype)
	{
	case ElementInt:
		*(int*)dst = *(int*)src;
		break;
	case ElementDouble:
		*(double*)dst = *(double*)src;
		break;
	case ElementString:
		*(std::string*)dst = *(std::string*)src;
		break;
	case ElementTypeNULL:
		break;
	default:
		assert(NULL);
		break;
	}
}

void TableElement::printout(std::ostream &out) const
{
	switch (_eletype)
	{
	case ElementInt:
		{
			out << *(int*)_eleptr;
			break;
		}
	case ElementDouble:
		{
			out << *(double*)_eleptr;
			break;
		}
	case ElementString:
		{
			out << *(std::string*)_eleptr;
			break;
		}
	case ElementTypeNULL:
		{
			out << "NULL";
			break;
		}
	default:
		break;
	}
}

void TableElement::input(std::istream &in)
{
	assert(_eleptr != 0);
	assert(_eletype != ElementTypeNULL);

	switch (_eletype)
	{
	case ElementInt:
		in >> *(int*)_eleptr;
		break;
	case ElementDouble:
		in >> *(double*)_eleptr;
		break;
	case ElementString:
		in >> *(std::string*)_eleptr;
		break;
	default:
		assert(NULL);
		break;
	}
}

bool TableElement::compare_less(void *ptr)
{
	assert(_eletype != ElementTypeNULL);
	assert(ptr != NULL && _eleptr != NULL);

	switch (_eletype)
	{
	case ElementInt:
		return *(int*)_eleptr < *(int*)ptr;
	case ElementDouble:
		return *(double*)_eleptr < *(double*)ptr;
	case ElementString:
		return *(std::string*)_eleptr < *(std::string*)ptr;
	default:
		assert(NULL);
		return false;		
	}
}

bool TableElement::compare_equal(void *ptr)
{
	assert(_eletype != ElementTypeNULL);
	assert(ptr != NULL && _eleptr != NULL);

	switch (_eletype)
	{
	case ElementInt:
		return *(int*)ptr == *(int*)_eleptr;
	case ElementDouble:
		return *(double*)ptr == *(double*)_eleptr;
	case ElementString:
		return *(std::string*)ptr == *(std::string*)_eleptr;
	default:
		assert(NULL);
		return false;		
	}
}

bool TableElement::compare_greater(void *ptr)
{
	assert(_eletype != ElementTypeNULL);
	assert(ptr != NULL && _eleptr != NULL);

	switch (_eletype)
	{
	case ElementInt:
		return *(int*)_eleptr > *(int*)ptr;
	case ElementDouble:
		return *(double*)_eleptr > *(double*)ptr;
	case ElementString:
		return *(std::string*)_eleptr > *(std::string*)ptr;
	default:
		assert(NULL);
		return false;		
	}
}

ElementType TableElement::string2type(const std::string& str)
{
	if (str == "int")
		return ElementInt;
	else if (str == "double")
		return ElementDouble;
	else if (str == "string")
		return ElementString;
	else
		return ElementTypeNULL;
}

std::string TableElement::type2string(ElementType et)
{
	if (et == ElementInt)
		return "int";
	else if (et == ElementDouble)
		return "double";
	else if (et == ElementString)
		return "string";
	else
		return "";
}

bool TableElement::is_type_string(const std::string& str)
{
	return str=="int" || str=="double" || str=="string";
}
