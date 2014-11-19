#ifndef _TABLE_ENTRY_H_
#define _TABLE_ENTRY_H_

#include <vector>
#include <iostream>

#include "Table.h"
#include "Field.h"
#include "TableElement.h"

class TableEntry
{
public:
	TableEntry(std::vector<Field>*, std::string = "");
	
	int size() const;
	std::string str();
	bool insert_element(FieldID, TableElement&);
	bool delete_element(FieldID);
	void set_specific_element(FieldID, TableElement &);
	const TableElement& get_specific_element(FieldID) const;

private:
	unsigned int _num;
	std::vector<TableElement> _entry;
	std::vector<Field> *_ptypes;
};

#endif //_TABLE_ENTRY_H_

