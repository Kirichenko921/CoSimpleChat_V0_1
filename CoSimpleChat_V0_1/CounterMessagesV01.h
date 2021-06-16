#pragma once
#include <string>
using namespace std;

class CounterMessages
{
public:
	explicit CounterMessages(const string& name); // подсчёт всех сообщений пользователя 
	explicit CounterMessages(const int count);  // подсчёт старых сообщений пользователя
	int getCountMessages();
	string getNikUser() const;
	int totalMessages();

private:
	int _count = 0;
	int _total = 0;
	string _name;
};
