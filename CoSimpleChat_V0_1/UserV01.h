#pragma once
#include <string>
#include  "SHA1.h"
#include <iostream>
#include <sstream>
#include <Windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

constexpr auto SQL_RESULT_LEN = 240;

bool checkingLogin(SQLHANDLE& sqlStmtHandle, std::string& checkNik);// проверка наличия логина