#pragma once
#include<string>



class Message
{
public:
    // конструктор принимает логины от и кому от User. И само сообщение
    Message(const std::string& message, const  std::string& fromLetter, const std::string& toLetter, const int& dayMessage, const int& monthMessage, const int& yearMessage);
    std::string getSendFromUser() const;
    std::string getSendToUser() const;
    std::string getMessage() const;
    int getdayMessage() const;
    int getmonthMessage() const;
    int getyearMessage() const;
    ~Message() = default;
private:
    std::string _message;
    std::string _fromLetter;
    std::string _toLetter;
    int _dayMessage = 0;
    int _monthMessage = 0;
    int _yearMessage = 0;
};