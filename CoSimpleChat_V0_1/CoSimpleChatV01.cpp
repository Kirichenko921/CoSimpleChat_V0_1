/// CoSimpleChat.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//


#include"UserV01.h"
#include "MessageV01.h"
#include "SHA1.h"
constexpr auto SQL_RETURN_CODE_LEN = 1024;
void exitProg(bool& userMenu, bool& messageMenu) //функция выхода
{
	std::cout << "--- See you next time! ---\n";
	userMenu = false;
	messageMenu = false;
}

//----------------------------------------------------------------------------------------------------------------------
void currentUser(const std::string& getName) // вывод пользователя который в настоящее время пользуется чатом
{
	if (getName.empty())
		std::cout << "\n\t\t\t\t\t\t---No one is using the chat right now---\n";

	else
		std::cout << "\n\t\t\t\t\t\t--Now the chat is used by: " << getName << "---\n";

}
int main()
{
	setlocale(LC_ALL, "");
	std::string getName;  //Ник пользователя работающего в чате
	size_t count = 0; // контроль вывода списка пользователей
	char modeMenu;  //Переменная, в которой хранится выбранный режим
	bool userMenu = true; // меню пользователя
	bool messageMenu = false;//меню сообщений
	User workingUserData; // работа с данными пользователя
	Messages messageProcessing;// работа с сообщениями
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

	//#########################################################################################################

	while (userMenu)
	{

		std::cout << "********************************************\n";
		std::cout << "* This is a simple chat. Select an action!\n *";
		std::cout << "********************************************\n";
		std::cout << "   1 - registration, 2 - login, 0 - exit\n";
		std::cout << "--------------------------------------------\n";
		currentUser(getName); // вывод текущего пользователя
		count = workingUserData.listUsers(sqlStmtHandle, count);// вывод списка пользователей

		std::cin >> modeMenu;
		std::cin.ignore(32767, '\n');
		switch (modeMenu)
		{
		case '1':  //регистрация
		{
			getName = workingUserData.regUser(sqlStmtHandle);
			currentUser(getName);
			count = workingUserData.listUsers(sqlStmtHandle, count);// вывод списка пользователей
			messageMenu = true;
			break;
		}

		case '2':  //вход
		{
			messageMenu = workingUserData.loginUser(sqlStmtHandle, getName);
			currentUser(getName);
			if (messageMenu)
			{
				messageProcessing.amountNewMessenger(sqlStmtHandle, getName);
			}
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
				messageProcessing.readMessage(sqlStmtHandle, getName, 2);  // читаем все сообщения
				break;
			}

			case '3':  // прочитать сообщения от определённого пользователя
			{
				messageProcessing.readMessage(sqlStmtHandle, getName, 3);  // читаем  сообщения от одного пользователя по выбору
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

