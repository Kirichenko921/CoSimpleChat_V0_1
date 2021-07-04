#pragma once
#include <string>
#include  "SHA1.h"
#include <unordered_map> 
#define PASSLENGTH  50
#include <iostream>

//Класс, содержащий в себе персональные данные пользователя
class User
{

public:
	User();
	~User(){}
	bool checkingLogin(const std::string& nik);
	void registration(const std::string& nik, const std::string& userPassword,const std::string& userName );
	bool comparisonLogin(const std::string& nik,const std::string& userPassword);// сличение введённой пары логин-пароль с сущесвующей в массиве
	void showLogins();
   	size_t countUser = 0;
private:
	struct  UserData
	{
		UserData(const std::string& userName, uint* userPassword);
		~UserData()
		{
			//if (_userPassword != nullptr)
				//delete[] _userPassword;
		}
		std::string  _userName;
		uint* _userPassword{nullptr};
	};
	
	std::unordered_map < std:: string, UserData > _hashTableUser;
	
};