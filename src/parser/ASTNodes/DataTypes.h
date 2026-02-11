#ifndef DATATYPES_H
#define DATATYPES_H

#include "../../lexer/Token/Token.h"
#include <string>

enum DataTypes { Null, Int, Number, Boolean, String };

DataTypes toDataTypes(Token token);
std::string fromDataTypes(DataTypes dataType);

#endif // DATATYPES_H