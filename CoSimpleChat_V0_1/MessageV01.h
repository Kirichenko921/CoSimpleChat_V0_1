#pragma once
#include "User.h"
#include<vector>
#include <stdio.h>
class Messages
{
public:
	Messages();
	void creatingMessage(SQLHANDLE& sqlStmtHandle, std::string author); // создание сообщения
	void readMessage(SQLHANDLE& sqlStmtHandle, std::string getName); // чтение всех сообщений
	void updateStaus(SQLHANDLE& sqlStmtHandle);// изменение статуса сообщения

	~Messages() = default;
private:

	std::string  recipient; // получатель сообщения
	std::string message; // создание сообщения
	std::stringstream ssTemp; // Переменная для преобразования результатов запросов в string 
	char chQuery[300]; // для записи, через sprintf_s, запросов, принимающих переменную типа int
	std::string query; // запрос к базе данных (string принимает запросы в которых есть переменные)
	std::wstring wsQuery; // приведение string к wstring (wstring запросы с переменными не принимает)
	bool checking = false;//  условие для выхода из цикла проверки уникальности логина
	std::vector <int> readMessages;// контейнер для хранения id прочитанных сообщений

};