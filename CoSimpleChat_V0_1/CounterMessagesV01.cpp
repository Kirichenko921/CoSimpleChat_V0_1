#include "CounterMessagesV01.h"


CounterMessages::CounterMessages(const std::string& name) :_name(name)
{
}

CounterMessages::CounterMessages(const int count) : _count(count)
{
}

int CounterMessages::getCountMessages()
{
	return _count;
}

std::string CounterMessages::getNikUser() const
{
	return _name;
}

int CounterMessages::totalMessages()
{
	return _total;
}