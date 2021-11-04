#include "UserV01.h"

bool checkingLogin(SQLHANDLE& sqlStmtHandle, std::string& checkNik) 
{
	//bool checking = false;//  условие для выхода из цикла проверки уникальности логина
	std::string nik;  //Nik, который вводит пользователь
	std::stringstream ssTemp; // Переменная для преобразования результатов запросов в string
	std::string query; // запрос к базе данных (string принимает запросы в которых есть переменные)
	std::wstring wsQuery; // приведение string к wstring (wstring запросы с переменными не принимает)

	std::cout << "Enter a Nik: \n";
	//(std::cin >> nik).get();// если не будет работать cin раскоментить эту строку
	getline(std::cin, nik);  // ввод логина (ника)
	std::cout << "\n";
	query = ("SELECT login FROM users_info  WHERE login LIKE  '" + nik + "'; ");// формируем запрос
	wsQuery = std::wstring(query.begin(), query.end()); // преобразовываем запрос
	if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)wsQuery.c_str(), SQL_NTS)) // запрашиваем в базе
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
		ssTemp << nikName;  // преобразовываем результат запроса  в string
		checkNik = ssTemp.str();
		if (checkNik == nik)   //  если логин есть в базе
		{
						return true;
		}
		else
		{
			return false;
		}

	}
}


