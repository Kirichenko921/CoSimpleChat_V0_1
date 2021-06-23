#include "UserV01.h"

User::UserData::UserData(const std::string userName, uint* userPassword) :_userName(userName),_userPassword(userPassword)
{
}

User::User()
{
}

bool User::checkingLogin(const std::string nik) // проверка наличия логина
{
	
	for (auto x : _hashTableUser)
	{
		if (x.first == nik)
			return false;
	}
	return true;
}

void User::registration(const std::string nik, const std::string userPassword, std::string userName)//регистрация пользователя
{
	char cUserPassword[PASSLENGTH];
	userPassword.copy(cUserPassword, PASSLENGTH);
	cUserPassword[userPassword.length()] = '\0';
		uint* digest = sha1(cUserPassword, PASSLENGTH); // вычисляем хэш пароля через алгоритм SHA1
	UserData currentData( userName,digest);
	_hashTableUser.insert({ nik,currentData });
	++countUser;
}
void User::showLogins()
{
	for (std::unordered_map< std::string, UserData>::iterator it = _hashTableUser.begin(); it != _hashTableUser.end(); ++it)
	{
		std::cout <<"\t\t\t\t\t\t\t"<< it->first <<"\n";
	}
}
std::string User::getUserNik() const
{
	return std::string();
}
bool User::comparisonLogin(std::string nik, std::string userPassword)// сличение введённой пары логин-пароль с сущесвующей в массиве
{
	char cUserPassword[PASSLENGTH];
	userPassword.copy(cUserPassword, PASSLENGTH);
	cUserPassword[userPassword.length()] = '\0';
    uint* digest = sha1(cUserPassword, PASSLENGTH);//генериуем хэш пароля
    int index = -1, i = 0;

    for (std::unordered_map< std::string, UserData>::iterator it = _hashTableUser.begin(); it != _hashTableUser.end(); ++it) // ищем логин
    {
		if (it->first == nik) // если нашли логин
        {
            if (!memcmp(digest, it->second._userPassword, SHA1HASHLENGTHBYTES)) // сличаем хэши паролей 
                return true;
            
                return false;
        }

    }
    // если логин не нашли
    return false;
}