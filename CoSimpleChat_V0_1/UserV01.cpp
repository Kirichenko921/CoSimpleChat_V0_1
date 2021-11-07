#include "UserV01.h"
User::User()
{
}

bool User::checkingLogin(SQLHANDLE& sqlStmtHandle, std::string& checkNik)
{
	std::cout << "Enter a Nik: \n";
	//(std::cin >> nik).get();// если не будет работать cin раскоментить эту строку
	getline(std::cin, _nik);  // ввод логина (ника)
	std::cout << "\n";
	_query = ("SELECT login FROM users_info  WHERE login LIKE  '" + _nik + "'; ");// формируем запрос
	_wsQuery = std::wstring(_query.begin(), _query.end()); // преобразовываем запрос
	if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)_wsQuery.c_str(), SQL_NTS)) // запрашиваем в базе
	{
		std::cout << "Error querying SQL Server \n";
		//goto COMPLETED;
	}
	else
	{
		SQLCHAR nikName[SQL_RESULT_LEN];  // переменная для хранения результата запроса
		while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS)
		{
			SQLGetData(sqlStmtHandle, 1, SQL_CHAR, nikName, SQL_RESULT_LEN, NULL);
		}
		_ssTemp << nikName;  // преобразовываем результат запроса  в string
		checkNik = _ssTemp.str();
		if (checkNik == _nik)   //  если логин есть в базе
		{
			std::stringstream().swap(_ssTemp);  // очищаем переменную
			return true;
		}
		else
		{
			checkNik = _nik;
			std::stringstream().swap(_ssTemp);  // очищаем переменную
			return false;
		}

	}
}
//--------------------------------------------------------------------------------------------------------------
std::string User::regUser(SQLHANDLE& sqlStmtHandle)
{
	_checking = true;
	// проверяем наличие логина в базе 
	while (_checking)
	{
		_checking = checkingLogin(sqlStmtHandle, _tempNik);
		if (_checking)
			std::cout << "Nick's busy. Enter another one!\n\n";
	}

	std::cout << "Enter a name: \n";
	//(std::cin >> name).get();// если не будет работать cin раскоментить эту строку
	getline(std::cin, _name);
	std::cout << "\n";

	std::cout << "Enter a surname: \n";
	//(std::cin >> surname).get();// если не будет работать cin раскоментить эту строку
	getline(std::cin, _surname);
	std::cout << "\n";

	std::cout << "Enter a email: \n";
	//(std::cin >> emailUser).get();// если не будет работать cin раскоментить эту строку
	getline(std::cin, _emailUser);
	std::cout << "\n";

	std::cout << "Enter a password: \n";
	//(std::cin >> password).get();// если не будет работать cin раскоментить эту строку
	getline(std::cin, _password);

	// обновление таблицы данных пользователя
	_query = "INSERT INTO users_info (login, name, surname, email_users) VALUES ( '" + _tempNik + "', '" + _name + "', '" + _surname + "','" + _emailUser + "');";  // запрос на добавление данных пользователя
	_wsQuery = std::wstring(_query.begin(), _query.end());  // преобразовываем запрос в wstring

	if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)_wsQuery.c_str(), SQL_NTS)) // добавляем данные в базу
	{
		std::cout << "Error querying SQL Server \n";
	}
	// обновление таблицв паролей
	_checksum.update(_password);
	_hUserPassword = _checksum.final();
	_query = "update users_passwords SET password_users = '" + _hUserPassword + "' ORDER BY users_id DESC LIMIT 1;"; //  формируем запрос надобавления пароля в базу
	_wsQuery = std::wstring(_query.begin(), _query.end());
	if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)_wsQuery.c_str(), SQL_NTS))  //  добавляем хэш пароля в базу 
	{
		std::cout << "Error querying SQL Server \n";
	}

	std::cout << "\n--- You have successfully registered! ---\n\n";

	return _tempNik;
}
//--------------------------------------------------------------------------------------------------------
bool User::loginUser(SQLHANDLE& sqlStmtHandle, std::string& userNik)
{
	_checking = false;
	// ввод и проверка логина .
	while (!_checking)
	{
		_checking = checkingLogin(sqlStmtHandle, _tempNik);
		if (!_checking)
			std::cout << "There is no user with this nickname!\n\n";
	}
	// ввод и проверка пароля
	std::cout << "Enter password:\n";
	//(std::cin >> password).get();// если не будет работать cin раскоментить эту строку
	getline(std::cin, _password);
	std::cout << "\n";

	_checksum.update(_password);
	_hUserPassword = _checksum.final();
	_query = ("SELECT password_users   FROM users_passwords  WHERE users_id IN (select id_users from users_info where login LIKE '" + _tempNik + "') AND password_users LIKE '" + _hUserPassword + "'; ");// формируем запрос на проверку наличия логина в базе
	_wsQuery = std::wstring(_query.begin(), _query.end()); // преобразовываем запрос

	if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)_wsQuery.c_str(), SQL_NTS))// запрашиваем в базе
	{
		std::cout << "Error querying SQL Server \n";
	}
	else
	{
		SQLCHAR sqlPassword[SQL_RESULT_LEN];// переменная для хранения результата запроса
		while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS)
		{
			SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlPassword, SQL_RESULT_LEN, NULL);
		}
		_ssTemp << sqlPassword;// преобразовываем результат запроса  в string
		if (_hUserPassword == _ssTemp.str())  //Сравниваем пароли,если совпадают
		{
			std::cout << "--- You have successfully logged in! ---\n\n";
			userNik = _tempNik; // меняем пользователя чата
			std::stringstream().swap(_ssTemp);  // очищаем переменную
			return true;
		}
		else  //иначе выходим  и пользователь остаётся прежним
		{
			std::cout << "Bed password!\n\n";
			std::stringstream().swap(_ssTemp);  // очищаем переменную
			return false;
		}
	}

}
//------------------------------------------------------------------------------------------------------------------
int User::listUsers(SQLHANDLE& sqlStmtHandle, size_t index)
{
	// считаем количество пользователей
	if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT COUNT(*) FROM users_info", SQL_NTS))
	{
		std::cout << "Error querying SQL Server \n";
	}
	else
	{
		SQLINTEGER id_users;
		while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS)
		{
			SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &id_users, sizeof(SQLINTEGER), NULL);
			_amountUsers = id_users;
		}
	}
	if (_amountUsers > index)  // если пользователей увеличилось, выводим обновлённый список
	{
		std::cout << "\t\t\t\t\t\t\t\tList of users:\n";
		if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT login FROM users_info", SQL_NTS))
		{
			std::cout << "Error querying SQL Server \n";
		}
		else
		{
			SQLCHAR nikName[SQL_RESULT_LEN];
			while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS)
			{
				SQLGetData(sqlStmtHandle, 1, SQL_CHAR, nikName, SQL_RESULT_LEN, NULL);
				//Выведем на экран номер и имя
				std::cout << "  \t\t\t\t\t\t\t\t -" << nikName << " -" << std::endl;
			}
			return ++index;
		}
	}
	if (_amountUsers == 0)
	{
		std::cout << "\t\t\t\t\t\tNo users yet\n"; // пользователей пока нет
	}
	return index;
}