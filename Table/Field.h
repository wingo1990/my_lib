#ifndef _FIELD_H_
#define _FIELD_H_

#include <string>
#include "TableElement.h"

class Field
{
public:
	Field(void);

	// first is name,  second is type.
	Field(const std::string&, const std::string&);

	// must ensure `str` format is "name-type"
	Field(const std::string&);
	~Field(void);
	
	ElementType get_type() const;
	std::string get_name() const;
	void set_type(const ElementType&);
	void set_name(const std::string&);
	std::string str();

	// TODO: it is for construct Field object successfully.
	// can put it in constructor and use exception to handle.
	static bool is_field_string(const std::string&);

private:
	ElementType _type;
	std::string _name;
};


#endif //_FIELD_H_

