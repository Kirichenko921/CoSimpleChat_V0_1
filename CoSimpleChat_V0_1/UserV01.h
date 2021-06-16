#pragma once
#include <string>
#include  "SHA1.h"
#include <unordered_map> 
#define PASSLENGTH  50
#include <iostream>


//�����, ���������� � ���� ������������ ������ ������������
class User
{
/*public:
	User(const string& name, const string& nik, const string& password);  //�����������
	virtual ~User(); //����������

	string getUserName() const;  //���������� ��� ������������
	string getUserNik() const;  //���������� ���
	string getUserPassword() const; //���������� ������

private:
	string _userName;
	string _userNik;
	string _userPassword;*/

public:
	User();
	~User(){}
	bool checkingLogin(const std::string nik);
	void registration(const std::string nik, const std::string userPassword, std::string userName );

private:
	struct  UserData
	{
		UserData(const std::string userName, uint* userPassword);
		~UserData()
		{
			if (_userPassword != 0)
				delete[] _userPassword;
		}
		std::string  _userName;
		uint* _userPassword;
	};
	
	std::unordered_map < std:: string, UserData > _hashTableUser;
};