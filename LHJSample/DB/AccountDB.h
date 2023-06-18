#pragma once
#include <Windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <format>
#include <string>

class AccountDB
{
public :
	AccountDB() = default;
	~AccountDB();
	void Init();

	bool CreateAccount(const std::string& id, const std::string& pw);
	bool SelectAccount(const std::string& id, const std::string& pw);

private:
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt;
};