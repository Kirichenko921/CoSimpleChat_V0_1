#include "UserV01.h"
#include<vector>
#include <stdio.h>
class Messages
{
public:
	Messages();
	void creatingMessage(SQLHANDLE& sqlStmtHandle, std::string author); // создание сообщения
	void readMessage(SQLHANDLE& sqlStmtHandle, std::string getName, int choice); // чтение всех сообщений
	void updateStaus(SQLHANDLE& sqlStmtHandle);// изменение статуса сообщения
	void  amountNewMessenger(SQLHANDLE& sqlStmtHandle, std::string getName); // извещеие о наличии новых сообщений
	~Messages() = default;
private:
	std::string  _author; // отправитель сообщения
	std::string  _recipient; // получатель сообщения
	std::string _message; // создание сообщения
	std::stringstream _ssTemp; // Переменная для преобразования результатов запросов в string 
	char _chQuery[300]; // для записи, через sprintf_s, запросов, принимающих переменную типа int
	std::string _query; // запрос к базе данных (string принимает запросы в которых есть переменные)
	std::wstring _wsQuery; // приведение string к wstring (wstring запросы с переменными не принимает)
	bool _checking;//  условие для выхода из цикла проверки уникальности логина
	std::vector <int> _readMessages;// контейнер для хранения id прочитанных сообщений
	User _requestUsers; // rзапросы к классу user
	int  _choice; // выбор - какие сообщения читаем все или от определённого пользователя
};