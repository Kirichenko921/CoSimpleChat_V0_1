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
	bool checkingLogin(const std::string nik);
	void registration(const std::string nik, const std::string userPassword, std::string userName );
	bool comparisonLogin(std::string nik, std::string userPassword);// сличение введённой пары логин-пароль с сущесвующей в массиве
	void showLogins();
    std:: string getUserNik() const;  //Возвращает ник
	int countUser = 0;
private:
	struct  UserData
	{
		UserData(const std::string userName, uint* userPassword);
		~UserData()
		{
			//if (_userPassword != 0)
				//delete[] _userPassword;
		}
		std::string  _userName;
		uint* _userPassword;
	};
	
	std::unordered_map < std:: string, UserData > _hashTableUser;
	
};