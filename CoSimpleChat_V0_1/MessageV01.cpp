#include "MessageV01.h"


Messages::Messages()
{
}

void Messages::creatingMessage(SQLHANDLE& sqlStmtHandle, std::string author)
{

	checking = checkingLogin(sqlStmtHandle, recipient); // проверяем наличие пользователя в базе
	if (!checking)
		std::cout << "There is no such login in the chat, try writing a message to an existing user!" << std::endl;

	else
	{
		std::cout << "\nEnter a message:\n";  // если пользователь есть пишем сообщение
		//(std::cin >> message).get();// если не будет работать cin раскоментить эту строку
		getline(std::cin, message);

		query = "INSERT INTO user_messages VALUES (null, (select id_users from users_info where login = '" + author + "'), (select id_users from users_info where login = '" + recipient + "'), '" + message + "', CURRENT_DATE, 0);";
		wsQuery = std::wstring(query.begin(), query.end());
		if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)wsQuery.c_str(), SQL_NTS))
		{
			std::cout << "Error querying SQL Server \n";
		}
	}
}

void Messages::readMessage(SQLHANDLE& sqlStmtHandle, std::string getName)
{
	// запрашиваем все непрочитанные сообщения для текущего пользователя
	query = "SELECT login,message_text,date_message,id_message FROM user_messages JOIN users_info ON id_users = id_author WHERE id_recipient = (select id_users from users_info where login = '" + getName + "') AND message_status = 1;";
	wsQuery = std::wstring(query.begin(), query.end());
	if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)wsQuery.c_str(), SQL_NTS))
	{
		std::cout << "Error querying SQL Server \n";
	}
	else
	{

		SQLCHAR nikNameSQL[SQL_RESULT_LEN];
		SQLCHAR messageSQL[SQL_RESULT_LEN];
		SQLCHAR dateSQL[SQL_RESULT_LEN];
		SQLINTEGER id_message;
		while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS)
		{
			SQLGetData(sqlStmtHandle, 1, SQL_CHAR, nikNameSQL, SQL_RESULT_LEN, NULL);
			SQLGetData(sqlStmtHandle, 2, SQL_CHAR, messageSQL, SQL_RESULT_LEN, NULL);
			SQLGetData(sqlStmtHandle, 3, SQL_CHAR, dateSQL, SQL_RESULT_LEN, NULL);
			SQLGetData(sqlStmtHandle, 4, SQL_INTEGER, &id_message, sizeof(SQLINTEGER), NULL);
			//Выводим на экран логин отправителя, само сообщение и дату создания сообщения
			std::cout << "\n******************************************************\n";
			std::cout << "     Sender " << nikNameSQL << ": " << messageSQL << "  ( " << dateSQL << " )" << std::endl;
			std::cout << "******************************************************\n";
			// id прочитанного сообщения добавляем в контейнер
			readMessages.push_back(id_message);
		}
	}
	// меняем статус прочитанных сообщений
	updateStaus(sqlStmtHandle);
}
void Messages::updateStaus(SQLHANDLE& sqlStmtHandle)
{
	// проходим по всему массиву с id прочитанных сообщений и  меняем их статус
	for (int i : readMessages)
	{
		sprintf_s(chQuery, "UPDATE user_messages SET message_status = 3  WHERE  id_message =  %d ;", i);
		query = chQuery;
		wsQuery = std::wstring(query.begin(), query.end());
		if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)wsQuery.c_str(), SQL_NTS))
		{
			std::cout << "Error querying SQL Server \n\n";
		}
	}
		readMessages.clear(); // очищаем массив
}