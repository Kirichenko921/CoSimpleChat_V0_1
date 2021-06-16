#include "UserV01.h"

User::UserData::UserData(const std::string userName, uint* userPassword) :_userName(userName)
{
	memcpy(_userPassword, userPassword, SHA1HASHLENGTHUINTS);
}

User::User()
{
}

bool User::checkingLogin(const std::string nik)
{
	
	for (auto x : _hashTableUser)
	{
		if (x.first == nik)
			return false;
	}
	return true;
}

void User::registration(const std::string nik, const std::string userPassword, std::string userName)
{
	char cUserPassword[PASSLENGTH];
	userPassword.copy(cUserPassword, PASSLENGTH);
	cUserPassword[userPassword.length()] = '\0';
		uint* digest = sha1(cUserPassword, PASSLENGTH); // вычисляем хэш пароля через алгоритм SHA1
	UserData currentData( userName,digest);
	_hashTableUser.insert({ nik,currentData });

}