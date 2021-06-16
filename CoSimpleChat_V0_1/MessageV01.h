#pragma once
#include<string>
using namespace std;


class Message
{
public:
    // конструктор принимает логины от и кому от User. И само сообщение
    Message(const string& message, const  string& fromLetter, const string& toLetter, const int& dayMessage, const int& monthMessage, const int& yearMessage);
    string getSendFromUser() const;
    string getSendToUser() const;
    string getMessage() const;
    int getdayMessage() const;
    int getmonthMessage() const;
    int getyearMessage() const;
    ~Message() = default;
private:
    string _message;
    string _fromLetter;
    string _toLetter;
    int _dayMessage = 0;
    int _monthMessage = 0;
    int _yearMessage = 0;
};