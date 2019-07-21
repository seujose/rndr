#include "rndrStream.h"
rndrStream::rndrStream()
{
}
rndrStream::~rndrStream()
{
}
int rndrStream::sync()
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *FString(str().c_str()));
	str("");
	return std::stringbuf::sync();
}