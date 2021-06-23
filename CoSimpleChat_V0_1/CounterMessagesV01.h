#pragma once
#include <string>


class CounterMessages
{
public:
	explicit CounterMessages(const std::string& name); // подсчёт всех сообщений пользователя 
	explicit CounterMessages(const int count);  // подсчёт старых сообщений пользователя
	int getCountMessages();
	std::string getNikUser() const;
	int totalMessages();

private:
	int _count = 0;
	int _total = 0;
	std::string _name;
};
