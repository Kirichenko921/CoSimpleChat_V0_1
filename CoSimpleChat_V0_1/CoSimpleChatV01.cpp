﻿// CoSimpleChat.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//


#include"UserV01.h"
#include "MessageV01.h"
#include "CounterMessagesV01.h"
#include <vector>


//------------------------------------------------------------------------------------------------------------------------------
void  readMessageUser(const std::string& user, const std::vector<Message>& allmess)// чтение сообщений от определённого пользователя
{
	std::string fromUser;
	std::cout << "Enter the nickname of the user whose messages you want to read\n";
	//(std::cin >> fromUser).get();// если не будет работать cin раскоментить эту строку
	getline(std::cin, fromUser);
	const auto size = allmess.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (allmess[i].getSendToUser() == user || allmess[i].getSendToUser() == "all")
		{
			if (fromUser == allmess[i].getSendFromUser())
			{
				std::cout << "From whom: " << allmess[i].getSendFromUser() << std::endl;
				std::cout << "Date of the message: " << allmess[i].getdayMessage() << "/"
					<< allmess[i].getmonthMessage() << "/" << allmess[i].getyearMessage() << "/" << std::endl;
				std::cout << "Message: " << allmess[i].getMessage() << std::endl;
			}
		}

	}
}
//################################################################################################################################################
std::string regUser()  //Функция регистрации пользователя
{
	std::string nik;  //Nik, который вводит пользователь
	std::string name;  //Имя, которое вводит пользователь
	std::string surname;  //Фамилия, которую вводит пользователь
	std::string emailUser;  //почта пользователя
	std::string password;  //Пароль, который вводит пользователь
	std::stringstream ssTemp; // Переменная для преобразования результатов запросов в string 
	std::string query; // запрос к базе данных (string принимает запросы в которых есть переменные)
	std::wstring wsQuery; // приведение string к wstring (wstring запросы с переменными не принимает)
	SHA1 checksum; // хэширование пароля
	bool checking = true;
	// проверяем наличие логина в базе 
	while (cheking)
	{
		checking = checkingLogin(sqlStmtHandle, nik);
		if (checking)
			std::cout << "Nick's busy. Enter another one!\n\n";
	}

	std::cout << "Enter a name: \n";
	//(std::cin >> name).get();// если не будет работать cin раскоментить эту строку
	getline(std::cin, name);
	std::cout << "\n";

	std::cout << "Enter a surname: \n";
	//(std::cin >> surname).get();// если не будет работать cin раскоментить эту строку
	getline(std::cin, surname);
	std::cout << "\n";

	std::cout << "Enter a email: \n";
	//(std::cin >> emailUser).get();// если не будет работать cin раскоментить эту строку
	getline(std::cin, emailUser);
	std::cout << "\n";

	std::cout << "Enter a password: \n";
	//(std::cin >> password).get();// если не будет работать cin раскоментить эту строку
	getline(std::cin, password);

	// обновление таблицы данных пользователя
	query = "INSERT INTO users_info (login, name, surname, email_users) VALUES ( '" + nik + "', '" + name + "', '" + surname + "','" + emailUser + "');";  // запрос на добавление данных пользователя
	wsQuery = std::wstring(query.begin(), query.end());  // преобразовываем запрос в wstring

	if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)wsQuery.c_str(), SQL_NTS)) // добавляем данные в базу
	{
		cout << "Error querying SQL Server \n";
		goto COMPLETED;
	}
	// обновление таблицв паролей
	checksum.update(password);
	hUserPassword = checksum.final();
	query = "update users_passwords SET password_users = '" + hUserPassword + "' ORDER BY users_id DESC LIMIT 1;"; //  формируем запрос надобавления пароля в базу
	wsQuery = std::wstring(query.begin(), query.end());
	if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)wsQuery.c_str(), SQL_NTS))  //  добавляем хэш пароля в базу 
	{
		cout << "Error querying SQL Server \n";
		goto COMPLETED;
	}

	std::cout << "\n--- You have successfully registered! ---\n\n";

	return nik;
}
//################################################################################################################################################
void loginUser(std::string& userNik)  //Функция входа
{
	std::string nik;  //Nik, который вводит пользователь
	std::string query; // Переменная для SQL запрроса
	std::wstring wsQuery;//  Переменная для преобразования запроса в wstring
	std::stringstream ssTemp; // Переменная для преобразования результатов запросов в string 
	SHA1 checksum; // хэширование пароля
	bool checking = false;
	// ввод и проверка логина .
	while (!checking)
	{
		checking = checkingLogin(sqlStmtHandle, nik);
		if (!checking)
			std::cout << "There is no user with this nickname!\n\n";
	}
	// ввод и проверка пароля
	std::cout << "Enter password:\n";
	//(std::cin >> password).get();// если не будет работать cin раскоментить эту строку
	getline(std::cin, password);
	std::cout << "\n";

	checksum.update(password);
	hUserPassword = checksum.final();
	query = ("SELECT password_users   FROM users_passwords  WHERE users_id IN (select id_users from users_info where login LIKE '" + nik + "') AND password_users LIKE '" + hUserPassword + "'; ");// формируем запрос на проверку наличия логина в базе
	wsQuery = std::wstring(query.begin(), query.end()); // преобразовываем запрос

	if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)wsQuery.c_str(), SQL_NTS))// запрашиваем в базе
	{
		std::cout << "Error querying SQL Server \n";
		goto COMPLETED;
	}
	else
	{
		SQLCHAR sqlPassword[SQL_RESULT_LEN];// переменная для хранения результата запроса
		while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS)
		{
			SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlPassword, SQL_RESULT_LEN, NULL);
			std::cout << sqlPassword << std::endl;
		}
		ssTemp << sqlPassword;// преобразовываем результат запроса  в string
		if (hUserPassword == ssTemp.str())  //Сравниваем пароли,если совпадают
		{
			std::cout << "--- You have successfully logged in! ---\n\n";
			userNik = nik; // меняем пользователя чата
		}
		else  //иначе выходим  и пользователь остаётся прежним
		{
			std::cout << "Bed password!\n\n";
			std::stringstream().swap(ssTemp);  // очищаем переменную
		}
	}

}
//################################################################################################################################################
void exitProg(bool& userMenu, bool& messageMenu) //функция выхода
{
	std::cout << "--- See you next time! ---\n";
	userMenu = false;
	messageMenu = false;
}
//------------------------------------------------------------------------------------------------------------------
int listUsers(size_t index, User& workingUserData) // вывод списка пользователей
{

	if (workingUserData.countUser > index)  // если пользователей увеличилось, выводим обновлённый список
	{
		std::cout << "\t\t\t\t\t\t\t\tList of users:\n";
		workingUserData.showLogins();
		return ++index;
	}
	else if (workingUserData.countUser == 0)
	{
		std::cout << "\t\t\t\t\t\tNo users yet\n"; // пользователей пока нет
	}
	return index;
}
//----------------------------------------------------------------------------------------------------------------------
void currentUser(const std::string& getName) // вывод пользователя который в настоящее время пользуется чатом
{
	if (getName.empty())
		std::cout << "\n\t\t\t\t\t\t---No one is using the chat right now---\n";

	else
		std::cout << "\n\t\t\t\t\t\t--Now the chat is used by: " << getName << "---\n";

}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
int newMessenger(std::vector <Message>& ollMessage, std::vector <CounterMessages>& oldMessage, const bool showMessages, const std::string& nik) // извещеие о наличии новых сообщений
{
	int oldCounter = 0;
	int newCounter = 0;
	const auto newIndex = ollMessage.size();
	const auto oldIndex = oldMessage.size();

	for (size_t i = 0; i < newIndex; ++i)
	{
		if (ollMessage[i].getSendToUser() == nik)
		{
			newCounter++;
		}
	}
	for (size_t i = 0; i < oldIndex; ++i)
	{
		if (oldMessage[i].getNikUser() == nik)
		{
			oldCounter++;
		}
	}
	if (showMessages)
	{
		std::cout << "\n\t\t\t\t\t\t!!!!!You have " << newCounter - oldCounter << " new messages!!!!\n";
	}
	return newCounter - oldCounter;

}
//------------------------------------------------------------------------------------------------------------------------


int main()
{
	setlocale(LC_ALL, "");
	std::string message;// вводимое пользователем сообщение
	std::string getName;  //Ник пользователя работающего в чате
	std::vector <Message> ollMessage; // Хранение всех сообщений
	std::vector <CounterMessages> newMessage;
	std::vector <CounterMessages> oldMessage;
	size_t count = 0; // контроль вывода списка пользователей
	char modeMenu;  //Переменная, в которой хранится выбранный режим
	bool userMenu = true;
	bool messageMenu = false;
	std::string nik;  //Nik, который вводит пользователь
	std::string password; // пароль вводимый пользователем
	User workingUserData;
	Messages messageProcessing;
	//####################################################################################

	SQLHANDLE sqlConnHandle{ nullptr }; // дескриптор для соединения с базой данных
	SQLHANDLE sqlEnvHandle{ nullptr }; // дескриптор окружения базы данных
	SQLHANDLE sqlStmtHandle{ nullptr };  //
	SQLWCHAR retconstring[SQL_RETURN_CODE_LEN]{}; // строка для кода возврата из функций API ODBC
	// Выделяем дескриптор для базы данных
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
		goto COMPLETED;
	if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
		goto COMPLETED;
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
		goto COMPLETED;
	std::cout << "Attempting connection to SQL Server...\n";
	// Устанавливаем соединение с сервером
	switch (SQLDriverConnect(sqlConnHandle,
		GetDesktopWindow(),
		(SQLWCHAR*)L"DRIVER={MySQL ODBC 8.0 ANSI Driver};SERVER=localhost;PORT=3306;DATABASE=data_users;UID=root;PWD=root;",
		//(SQLWCHAR*)L"DRIVER={MySQL ODBC 8.0 ANSI Driver};SERVER=MySQL_testdb64;DATABASE=testdb;UID=root;PWD=root;",
		SQL_NTS,
		retconstring,
		1024,
		NULL,
		SQL_DRIVER_COMPLETE)) {
	case SQL_SUCCESS:
	case SQL_SUCCESS_WITH_INFO:
		std::cout << "Successfully connected to SQL Server\n";
		break;
	case SQL_INVALID_HANDLE:
	case SQL_ERROR:
		std::cout << "Could not connect to SQL Server\n";
		goto COMPLETED;
	default:
		break;
	}
	// Если соединение не установлено, то выходим из программы
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
		goto COMPLETED;
	std::cout << "\nExecuting T-SQL query...\n";
	// Если выполнение запроса с ошибками, то выходим из программы
	if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT @@VERSION", SQL_NTS)) {
		std::cout << "Error querying SQL Server \n";
		goto COMPLETED;
	}
	else {
		//Объявление структуры данных для результата запроса версии SQL
		SQLCHAR sqlVersion[SQL_RESULT_LEN];
		SQLLEN ptrSqlVersion;
		while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
			SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlVersion, SQL_RESULT_LEN, &ptrSqlVersion);
			//Выведем на экран версию SQL
			std::cout << "\nQuery Result:\n\n";
			std::cout << sqlVersion << std::endl;
		}
	}
	//#########################################################################################################

	while (userMenu)
	{

		std::cout << "********************************************\n";
		std::cout << "* This is a simple chat. Select an action!\n *";
		std::cout << "********************************************\n";
		std::cout << "   1 - registration, 2 - login, 0 - exit\n";
		std::cout << "--------------------------------------------\n";
		currentUser(getName);
		count = listUsers(count, workingUserData);// вывод списка пользователей

		std::cin >> modeMenu;
		std::cin.ignore(32767, '\n');
		switch (modeMenu)
		{
		case '1':  //регистрация
		{
			getName = regUser();
			currentUser(getName);
			messageMenu = true;
			break;
		}

		case '2':  //вход
		{
			loginUser(getName);
			messageMenu = true;
			currentUser(getName);
			newMessenger(ollMessage, oldMessage, 1, getName);
			break;
		}

		case '0':  //выход
		{
			exitProg(userMenu, messageMenu);
			break;
		}
		default:
		{
			std::cout << "\nThere is no such menu item. Choose 1,2 or 0!!!\n";
			userMenu = true;
			messageMenu = false;
			break;
		}


		}

		while (messageMenu)
		{

			std::cout << "***************************\n";
			std::cout << "* What do you want to do? *\n";
			std::cout << "***************************\n";
			std::cout << "1 - write a message  \n";
			std::cout << "2 - read message     \n";
			std::cout << "3 - read message User\n";
			std::cout << "0 - exit\n";
			std::cout << "---------------------------\n";

			std::cin >> modeMenu;
			std::cin.ignore(32767, '\n');
			std::cout << "\n";
			switch (modeMenu)
			{
			case '1':  //написать сообщение
			{
				messageProcessing.creatingMessage(sqlStmtHandle, getName);
				break;
			}

			case '2':  //прочитать сообщения
			{
				messageProcessing.readMessage(sqlStmtHandle,getName);  // читаем все сообщения
				break;
			}
			
			case '3':  // прочитать сообщения от определённого пользователя
			{
				readMessageUser(getName, ollMessage);
				const int number = newMessenger(ollMessage, oldMessage, 0, getName);
				for (int i = 0; i < number; ++i)
				{
					oldMessage.push_back(CounterMessages(getName));
				}
				std::cout << "\n";
				break;
			}


			case '0':  //выход из кейса сообщений
			{
				messageMenu = false;
				break;
			}
			default:
			{
				std::cout << "\nThere is no such menu item. Choose 1,2,3 or 0!!!\n";
				userMenu = true;
				break;
			}
			}

		}
	}
	//###################################################################
	// Закрываем соединение и выходим из программы
COMPLETED:
	SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
	SQLDisconnect(sqlConnHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
	// Пауза перед закрытием консольного приложения
	std::cout << "\nPress any key to exit...";
	getchar();
}
