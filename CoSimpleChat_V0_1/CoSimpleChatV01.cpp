// CoSimpleChat.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//


#include"UserV01.h"
#include "MessageV01.h"
#include "CounterMessagesV01.h"
#include <vector>
#include <string>
#include<Windows.h>


//---------------------------------------------------------------------------------------------------------------------------------
string verifyingRecipient(const vector <User>& ollUsers) // проверяем есть ли пользователь которому мы будем писать сообщение
{
	const auto size = ollUsers.size();
	string toUser;
	while (true)
	{
		cout << "Enter the recipient's nickname:\n";
		cout << "If you want to send the message all enter - all:\n";
		//(cin >> toUser).get();// если не будет работать cin раскоментить эту строку
		getline(cin, toUser);
		for (size_t i = 0; i < size; ++i)
		{
			if (toUser == ollUsers[i].getUserNik() || toUser == "all")
			{
				return toUser;
			}
		}
		cout << "\n\tThere is no such user";
	}
}

//--------------------------------------------------------------------------------------------------------------------
void  readMessage(const string& user, const vector<Message>& allmess) // чтение всех сообщений для пользователя
{
	int count = 0;
	const auto size = allmess.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (allmess[i].getSendToUser() == user || allmess[i].getSendToUser() == "all")
		{
			cout << "From whom: " << allmess[i].getSendFromUser() << endl;
			cout << "Date of the message: " << allmess[i].getdayMessage() << "/"
				<< allmess[i].getmonthMessage() << "/" << allmess[i].getyearMessage() << "/" << endl;
			cout << "Message: " << allmess[i].getMessage() << endl;
			++count;
		}
	}
	if (count == 0)
	{
		cout << "There are no messages for you!" << endl;
	}
}
//------------------------------------------------------------------------------------------------------------------------------
void  readMessageUser(const string& user, const vector<Message>& allmess)// чтение сообщений от определённого пользователя
{
	int count = 0;
	string fromUser;
	cout << "Enter the nickname of the user whose messages you want to read\n";
	//(cin >> fromUser).get();// если не будет работать cin раскоментить эту строку
	getline(cin, fromUser);
	const auto size = allmess.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (allmess[i].getSendToUser() == user || allmess[i].getSendToUser() == "all")
		{
			if (fromUser == allmess[i].getSendFromUser())
			{
				cout << "From whom: " << allmess[i].getSendFromUser() << endl;
				cout << "Date of the message: " << allmess[i].getdayMessage() << "/"
					<< allmess[i].getmonthMessage() << "/" << allmess[i].getyearMessage() << "/" << endl;
				cout << "Message: " << allmess[i].getMessage() << std::endl;
				++count;
			}
		}

	}
}
//-------------------------------------------------------------------------------------------------------------------------
void regUser()  //Функция регистрации пользователя
{
	string name;  //Имя, которое вводит пользователь
	string nik;  //Nik, который вводит пользователь
	string password;  //Пароль, который вводит пользователь
	User workingUserData;
	bool checking = false;
	while (!checking)
	{
		std::cout << "Enter a Nik: " << endl;
		//(cin >> nik).get();// если не будет работать cin раскоментить эту строку
		getline(std::cin, nik);
		std::cout << "\n";
		checking = workingUserData.checkingLogin(nik);
		if (!checking)
			std::cout << "Nick's busy. Enter another one!\n\n";
	}
	std::cout << "Enter a name: \n";
	//(cin >> name).get();// если не будет работать cin раскоментить эту строку
	getline(std::cin, name);
	std::cout << "\n";

	std::cout << "Enter a password: \n";
	//(cin >> password).get();// если не будет работать cin раскоментить эту строку
	getline(std::cin, password);
		workingUserData.registration(nik, password, name);  //помещаем пользователя в контейнер
		cout << "\n--- You have successfully registered! ---\n\n";
}
//------------------------------------------------------------------------------------------------------------------------------------
void loginUser(string& userNik, vector <User>& users)  //Функция входа
{
	bool enterUser{ false };
	string nik;  //Nik, который вводит пользователь
	string accountPassword;  //пароль который сохранил пользователь для своего аккаунта

	do
	{
		cout << "Enter your nickname:" << endl;
		//(cin >> nik).get();// если не будет работать cin раскоментить эту строку
		getline(cin, nik);
		cout << "" << endl;


		for (size_t i = 0; i < users.size(); i++)  //ищем пользователя с таким ником
		{
			if (users[i].getUserNik() == nik)
			{
				accountPassword = users[i].getUserPassword();
				enterUser = true;
				break;
			}
		}

		if (!enterUser)  //если в база не оказалось такого ника
		{
			cout << "There is no user with this nickname!" << endl;
			cout << "" << endl;
		}

	} while (!enterUser);

	string enterPassword;  //Пароль, который вводит пользователь
	cout << "Enter password:" << endl;
	//(cin >> password).get();// если не будет работать cin раскоментить эту строку
	getline(cin, enterPassword);
	cout << "" << endl;


	if (enterPassword == accountPassword)  //Сравниваем пароли,если совпадают
	{
		cout << "--- You have successfully logged in! ---" << endl;
		cout << "" << endl;
		userNik = nik; // меняем пользователя чата
	}
	else  //иначе выходим  и пользователь остаётся прежним
	{
		cout << "Bed password!" << endl;
		cout << "" << endl;
	}

}
//------------------------------------------------------------------------------------------------------------------
void exitProg(bool& userMenu, bool& messageMenu) //функция выхода
{
	cout << "--- See you next time! ---" << endl;
	userMenu = false;
	messageMenu = false;
}
//------------------------------------------------------------------------------------------------------------------
size_t listUsers(size_t index, vector <User>& ollUsers) // вывод списка пользователей
{
	const auto size = ollUsers.size();
	if (size > index)  // если пользователей увеличилось, выводим обновлённый список
	{
		cout << "\t\t\t\t\t\t\t\tList of users:\n";
		for (size_t i = 0; i < size; ++i)
		{
			cout << "\t\t\t\t\t\t" << i + 1 << ") " << ollUsers[i].getUserNik() << endl;
		}
		return ++index;
	}
	else if (size == 0)
	{
		cout << "\t\t\t\t\t\tNo users yet\n"; // пользователей пока нет
	}
	return index;
}
//----------------------------------------------------------------------------------------------------------------------
void currentUser(const string& getName) // вывод пользователя который в настоящее время пользуется чатом
{
	if (getName.empty())
	{
		cout << "\n\t\t\t\t\t\t---No one is using the chat right now---\n";
	}
	else
	{
		cout << "\n\t\t\t\t\t\t--Now the chat is used by: " << getName << "---" << endl;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
int newMessenger(vector <Message>& ollMessage, vector <CounterMessages>& oldMessage, const bool showMessages, const string& nik) // извещеие о наличии новых сообщений
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
		cout << "\n\t\t\t\t\t\t!!!!!You have " << newCounter - oldCounter << " new messages!!!!\n";
	}
	return newCounter - oldCounter;

}
//------------------------------------------------------------------------------------------------------------------------
void getTimeMessage(int& dayMessage, int& monthMessage, int& yearMessage)
{
	SYSTEMTIME dateMessage;
	GetSystemTime(&dateMessage);
	dayMessage = dateMessage.wDay;
	monthMessage = dateMessage.wMonth;
	yearMessage = dateMessage.wYear;
}
int main()
{
	setlocale(LC_ALL, "");

	vector <User> users;  //Создаем контейнер для хранения данных пользователей
	char modeUsers;  //Переменная, в которой хранится выбранный режим
	char modeMessage; // 
	string message;// вводимое пользователем сообщение
	string getName;  //Ник пользователя работающего в чате
	vector <Message> ollMessage; // Хранение всех сообщений
	vector <CounterMessages> newMessage;
	vector <CounterMessages> oldMessage;
	size_t count = 0; // контроль вывода списка пользователей
	char modeUsers;  //Переменная, в которой хранится выбранный режим
	bool userMenu = true;
	bool messageMenu;
	std::string nik;  //Nik, который вводит пользователь
	User workingUserData;
	while (userMenu)
	{

		std::cout << "********************************************\n";
		std::cout << "* This is a simple chat. Select an action!\n *";
		std::cout << "********************************************\n";
		std::cout << "   1 - registration, 2 - login, 0 - exit\n";
		std::cout << "--------------------------------------------\n";
		currentUser(getName);
		count = listUsers(count, users);// вывод списка пользователей

		std::cin >> modeUsers;
		std::cin.ignore(32767, '\n');
		switch (modeUsers)
		{
		case '1':  //регистрация
		{
			regUser();
			//messageMenu = true;
			//currentUser(getName);
					
			break;
		}

		case '2':  //вход
		{
			loginUser(getName, users);
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
			cout << "\nThere is no such menu item. Choose 1,2 or 0!!!\n";
			userMenu = true;
			messageMenu = false;
			break;
		}


		}
		int dayMessage;
		int monthMessage;
		int yearMessage;
		vector <Message> timeMessage; // Время создания всех сообщений
		while (messageMenu)
		{

			cout << "***************************" << endl;
			cout << "* What do you want to do? *" << endl;
			cout << "***************************" << endl;
			cout << "1 - write a message  " << endl;
			cout << "2 - read message     " << endl;
			cout << "3 - read message User" << endl;
			cout << "0 - exit" << endl;
			cout << "---------------------------" << endl;

			cin >> modeMessage;
			cin.ignore(32767, '\n');
			cout << endl;
			switch (modeMessage)
			{
			case '1':  //написать сообщение
			{
				string toUser = verifyingRecipient(users);
				cout << "\nEnter a message:\n";
				//(cin >> message).get();// если не будет работать cin раскоментить эту строку
				getline(cin, message);
				getTimeMessage(dayMessage, monthMessage, yearMessage);
				cout << endl;
				ollMessage.push_back(Message(message, getName, toUser, dayMessage, monthMessage, yearMessage));//добавляем к списку сообщений
				newMessage.push_back(CounterMessages(toUser));// добавляем в счётчик новых сообщений
				break;
			}


			case '2':  //прочитать сообщения
			{
				readMessage(getName, ollMessage);  // читаем все сообщения
				const int number = newMessenger(ollMessage, oldMessage, 0, getName); // присваиваем индексу разницу между количеством старых и новых сообщений
				for (int i = 0; i < number; ++i)                      // выравниваем количество новых и старых сообщений
				{
					oldMessage.push_back(CounterMessages(getName));         // теперь все сообщения прочитаны
				}
				cout << endl;
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
				cout << endl;
				break;
			}


			case '0':  //выход из кейса сообщений
			{
				messageMenu = false;
				break;
			}
			default:
			{
				cout << "\nThere is no such menu item. Choose 1,2,3 or 0!!!\n";
				userMenu = true;
				break;
			}
			}

		}
	}
	return 0;
}
