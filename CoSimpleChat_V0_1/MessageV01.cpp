#include "MessageV01.h"

Messages::Messages()
{
}

void Messages::creatingMessage(SQLHANDLE& sqlStmtHandle, std::string author)
{
	_checking = false;
	_checking = _requestUsers.checkingLogin(sqlStmtHandle, _recipient); // проверяем наличие пользователя в базе
	if (!_checking)
		std::cout << "There is no such login in the chat, try writing a message to an existing user!" << std::endl;

	else
	{
		std::cout << "\nEnter a message:\n";  // если пользователь есть пишем сообщение
		//(std::cin >> message).get();// если не будет работать cin раскоментить эту строку
		getline(std::cin, _message);

		_query = "INSERT INTO user_messages VALUES (null, (select id_users from users_info where login = '" + author + "'), (select id_users from users_info where login = '" + _recipient + "'), '" + _message + "', CURRENT_DATE, 0);";
		_wsQuery = std::wstring(_query.begin(), _query.end());
		if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)_wsQuery.c_str(), SQL_NTS))
		{
			std::cout << "Error querying SQL Server \n";
		}
	}
}

void Messages::readMessage(SQLHANDLE& sqlStmtHandle, std::string getName, int choice)
{
	_checking = false;
	if (choice == 3) // если пользователь хочет прочитать сообщение от одного отправителя
	{
		while (!_checking)
		{
			_checking = _requestUsers.checkingLogin(sqlStmtHandle, _author);
			if (!_checking)
				std::cout << "There is no such login in the chat, try to read the message of an existing user!" << std::endl;
		}
		_query = "SELECT login,message_text,date_message,id_message,message_status FROM user_messages JOIN users_info ON id_users = id_author WHERE id_recipient = (select id_users from users_info where login = '" + getName + "') AND message_status > 1 AND  id_author = (select id_users from users_info where login= '" + _author + "'); ";
		_wsQuery = std::wstring(_query.begin(), _query.end());
	}
	else // запрашиваем все непрочитанные сообщения для текущего пользователя
	{
		_query = "SELECT login,message_text,date_message,id_message,message_status FROM user_messages JOIN users_info ON id_users = id_author WHERE id_recipient = (select id_users from users_info where login = '" + getName + "') AND message_status > 1;";
		_wsQuery = std::wstring(_query.begin(), _query.end());
	}
	// выводим на экран сообщения
	if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)_wsQuery.c_str(), SQL_NTS))
	{
		std::cout << "Error querying SQL Server \n";
	}
	else
	{

		SQLCHAR nikNameSQL[SQL_RESULT_LEN];
		SQLCHAR messageSQL[SQL_RESULT_LEN];
		SQLCHAR dateSQL[SQL_RESULT_LEN];
		SQLINTEGER id_message;
		SQLINTEGER status;
		while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS)
		{
			SQLGetData(sqlStmtHandle, 1, SQL_CHAR, nikNameSQL, SQL_RESULT_LEN, NULL);
			SQLGetData(sqlStmtHandle, 2, SQL_CHAR, messageSQL, SQL_RESULT_LEN, NULL);
			SQLGetData(sqlStmtHandle, 3, SQL_CHAR, dateSQL, SQL_RESULT_LEN, NULL);
			SQLGetData(sqlStmtHandle, 4, SQL_INTEGER, &id_message, sizeof(SQLINTEGER), NULL);
			SQLGetData(sqlStmtHandle, 5, SQL_INTEGER, &status, sizeof(SQLINTEGER), NULL);
			//Выводим на экран логин отправителя, само сообщение и дату создания сообщения

			std::cout << "\n******************************************************\n";
			if (status == 2)  // если сообщение не прочитано добавляем пометку NEW
			{
				std::cout << " \tNEW \t Sender " << nikNameSQL << "  ( " << dateSQL << " )" << ": " << messageSQL << std::endl;
				// id прочитанного сообщения добавляем в контейнер
				_readMessages.push_back(id_message);
			}
			if (status == 3) // если сообщение прочитано добавляем пометку READ
				std::cout << "\t\READ\t Sender " << nikNameSQL << "  ( " << dateSQL << " )" << ": " << messageSQL << std::endl;
		}
	}
	// меняем статус прочитанных сообщений
	updateStaus(sqlStmtHandle);
}
void Messages::updateStaus(SQLHANDLE& sqlStmtHandle)
{
	// проходим по всему массиву с id прочитанных сообщений и  меняем их статус
	for (int i : _readMessages)
	{
		sprintf_s(_chQuery, "UPDATE user_messages SET message_status = 3  WHERE  id_message =  %d ;", i);
		_query = _chQuery;
		_wsQuery = std::wstring(_query.begin(), _query.end());
		if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)_wsQuery.c_str(), SQL_NTS))
		{
			std::cout << "Error querying SQL Server \n\n";
		}
	}
	_readMessages.clear(); // очищаем массив
}

void  Messages::amountNewMessenger(SQLHANDLE& sqlStmtHandle, std::string getName)
{

	// выводим общее количество новых сообщений
	_query = "SELECT  count(*) FROM user_messages JOIN users_info on id_users = id_author WHERE id_recipient = (SELECT id_users FROM users_info WHERE login = '" + getName + "') AND message_status = 2;  ";
	_wsQuery = std::wstring(_query.begin(), _query.end());
	if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)_wsQuery.c_str(), SQL_NTS))
	{
		std::cout << "Error querying SQL Server \n";
	}
	else
	{
		SQLINTEGER count_messages;
		while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS)
		{
			SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &count_messages, sizeof(SQLINTEGER), NULL);

			std::cout << "\n\t\t\t\t\t\t!!!!!You have " << count_messages << " new messages!!!!\n";
		}
	}
	// выводим список пользователей написавших сообщения и количество написанных ими сообщений
	_query = "SELECT  login, count(*) FROM user_messages JOIN users_info on id_users = id_author WHERE id_recipient = (SELECT id_users FROM users_info WHERE login = '" + getName + "') AND message_status = 2 GROUP BY id_author;  ";
	_wsQuery = std::wstring(_query.begin(), _query.end());
	if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)_wsQuery.c_str(), SQL_NTS))
	{
		std::cout << "Error querying SQL Server \n";
	}
	else
	{
		SQLCHAR nikNameSQL[SQL_RESULT_LEN];
		SQLINTEGER count_messages;
		while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS)
		{
			SQLGetData(sqlStmtHandle, 1, SQL_CHAR, nikNameSQL, SQL_RESULT_LEN, NULL);
			SQLGetData(sqlStmtHandle, 2, SQL_INTEGER, &count_messages, sizeof(SQLINTEGER), NULL);

			std::cout << "\n\t\t\t\t\t\t from " << nikNameSQL << " - " << count_messages << " new messages\n";
		}
	}

}