#include "AccountDB.h"
#include "../Config/ConfigLoader.h"
#include <format>

AccountDB::~AccountDB()
{
	//핸들 해제
	SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	SQLDisconnect(hdbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

void AccountDB::Init()
{
	// todo : DB 비동기로 돌리기
	// 
	//환경 핸들
	SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);

	// 버전설정
	ret = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

	//DB연결 핸들
	ret = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

	std::string connectionstr1 = "DRIVER={SQL Server};";
	std::string connectionstr2 = std::format("SERVER={}, {};DATABASE=AccountDB;UID={};PWD={}",
		gConfig.GetInstance().GetAccountDBInfo().IP, gConfig.GetInstance().GetAccountDBInfo().Port, gConfig.GetInstance().GetAccountDBInfo().ID, gConfig.GetInstance().GetAccountDBInfo().Password);
	connectionstr1.append(connectionstr2);
	std::wstring stemp = std::wstring(connectionstr1.begin(), connectionstr1.end());
	SQLWCHAR* connectionString = (SQLWCHAR*)stemp.c_str();

	// DB 연결
	ret = SQLDriverConnectW(hdbc, NULL, connectionString, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);
	
	if (ret != 1)
	{
		std::cout << "SQL SERVER DB 접속 실패";
		return;
	}

	//문장 핸들 생성
	ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
}

bool AccountDB::CreateAccount(const std::string& id, const std::string& pw)
{
	SQLCHAR* sqlID = (SQLCHAR*)(const_cast<char*>(id.c_str()));
	SQLCHAR* sqlPassword = (SQLCHAR*)(const_cast<char*>(pw.c_str()));
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 10, 0, sqlID, 0, 0);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 10, 0, sqlPassword, 0, 0);
	SQLRETURN ret = SQLExecDirect(hstmt, (SQLWCHAR*)L"{call usp_Account_Create(?,?)}", SQL_NTS);

	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
	{
		return true;
	}

	return false;
}

bool AccountDB::SelectAccount(const std::string& id, const std::string& pw)
{
	SQLCHAR* sqlID = (SQLCHAR*)(const_cast<char*>(id.c_str()));
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 10, 0, sqlID, 0, 0);
	SQLRETURN ret = SQLExecDirect(hstmt, (SQLWCHAR*)L"{call usp_Account_Select(?)}", SQL_NTS);

	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
	{
		SQLWCHAR password[10];
		while (SQLFetch(hstmt) == SQL_SUCCESS)
		{
			SQLGetData(hstmt, 3, SQL_C_WCHAR, password, 20, NULL);
			std::wstring ws(password);
			std::string strPassword(ws.begin(), ws.end());
			//strPassword.replace(strPassword.find(" "), strPassword.length(), "");
			if (!strPassword.compare(pw))
			{
				SQLFreeStmt(hstmt, SQL_UNBIND);
				SQLFreeStmt(hstmt, SQL_CLOSE);
				return true;
			}
		}

		SQLFreeStmt(hstmt, SQL_UNBIND);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return false;
	}

	SQLFreeStmt(hstmt, SQL_UNBIND);
	SQLFreeStmt(hstmt, SQL_CLOSE);
	return false;
}