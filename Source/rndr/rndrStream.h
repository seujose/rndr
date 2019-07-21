#pragma once
#include "CoreMinimal.h"
#include <sstream>
class rndrStream: public std::stringbuf
{
public:
	rndrStream();
	~rndrStream();
	int sync();
};