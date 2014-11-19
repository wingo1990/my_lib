#ifndef _TABLE_ELEMENT_H_
#define _TABLE_ELEMENT_H_

#include <iostream>
#include <sstream>
#include <string>

typedef enum tagElementType
{
	ElementTypeNULL,
	ElementString,
	ElementInt,
	ElementDouble,
}ElementType;

typedef enum tagComparison
{
	OPERATOR_NULL,
	OPERATOR_LESS,
	OPERATOR_EQUAL,
	OPERATOR_GREATER
	//continue condition to be consider.
}CompareOperator;

class TableElement
{
public:
	//TableElement();
	TableElement(const TableElement&);
	TableElement(const int &);
	TableElement(const double &);
	TableElement(const std::string&);
	TableElement(const ElementType, std::string = "0");

	~TableElement();

	ElementType get_element_type();
	std::string get_value_as_string();
	int get_value_as_int();
	double get_value_as_double();

	TableElement &operator=(const TableElement &);
	friend std::ostream &operator<<(std::ostream &, const TableElement &);
	friend std::istream &operator>>(std::istream &, TableElement &);
	bool operator==(const int&);
	bool operator==(const std::string&);
	bool operator==(const double&);
	bool operator==(const TableElement&);
	
	bool operator<(const TableElement&);
	bool operator>(const TableElement&);

	static ElementType string2type(const std::string&);
	static std::string type2string(ElementType);
	static bool is_type_string(const std::string&);

private:
	ElementType _eletype;
	void* _eleptr;

	void *get_element_space();
	void copy_element_space(void *dst, void *src);
	void printout(std::ostream &) const;
	void input(std::istream &);
	bool compare_less(void *);
	bool compare_equal(void *);
	bool compare_greater(void *);

	template <typename T>
	T string2as_(std::string& str)
	{
		T temp;
		std::istringstream iss(str);
		iss >> temp;
		return temp;
	}
};

#endif  //_TABLE_ELEMENT_H_