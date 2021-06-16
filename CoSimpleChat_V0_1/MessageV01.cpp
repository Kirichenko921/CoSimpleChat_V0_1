#include "MessageV01.h"


Message::Message(const string& message, const string& fromLetter, const string& toLetter, const int& dayMessage, const int& monthMessage, const int& yearMessage) :
	_message(message), _fromLetter(fromLetter), _toLetter(toLetter), _dayMessage(dayMessage), _monthMessage(monthMessage), _yearMessage(yearMessage)
{
}

string Message::getSendFromUser() const
{
	return _fromLetter;
}

string Message::getSendToUser() const
{
	return _toLetter;
}

string Message::getMessage() const
{
	return _message;
}

int Message::getdayMessage() const
{
	return _dayMessage;
}

int Message::getmonthMessage() const
{
	return _monthMessage;
}

int Message::getyearMessage() const
{
	return _yearMessage;
}
