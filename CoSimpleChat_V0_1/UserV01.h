
#pragma once

#include  "SHA1.h"
#include <Windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

constexpr auto SQL_RESULT_LEN = 240;
class User
{
public:
	User();
	bool checkingLogin(SQLHANDLE& sqlStmtHandle, std::string& checkNik);// проверка наличия логина
	std::string regUser(SQLHANDLE& sqlStmtHandle);  //Функция регистрации пользователя
	bool loginUser(SQLHANDLE& sqlStmtHandle, std::string& userNik);  //Функция входа
	int listUsers(SQLHANDLE& sqlStmtHandle, size_t index); // вывод списка пользователей

private:
	std::string _nik;  //Nik, который вводит пользователь
	std::string _name;  //Имя, которое вводит пользователь
	std::string _surname;  //Фамилия, которую вводит пользователь
	std::string _emailUser;  //почта пользователя
	std::string _password;  //Пароль, который вводит пользователь
	std::string _tempNik;  //временныое хранение логина
	std::string _query; // Переменная для SQL запрроса
	std::wstring _wsQuery;//  Переменная для преобразования запроса в wstring
	std::stringstream _ssTemp; // Переменная для преобразования результатов запросов в string 
	SHA1 _checksum; // хэширование пароля
	std::string _hUserPassword; // хэш пароля
	bool _checking; //  условие для выхода из цикла проверки уникальности логина
	int _amountUsers;//количество пользователей
};