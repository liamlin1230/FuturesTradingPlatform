#ifndef ODBC_H
#define ODBC_H

#pragma once
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqlucode.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <stdlib.h>
#include <sal.h>
#include <string>
#include <iostream>


#define TRYODBC(h, ht, x)   {   RETCODE rc = x;\
                                if (rc != SQL_SUCCESS) \
                                { \
                                    HandleDiagnosticRecord (h, ht, rc); \
                                } \
                                if (rc == SQL_ERROR) \
                                { \
                                    fwprintf(stderr, L"Error in " L#x L"\n"); \
                                }  \
                            }

void HandleDiagnosticRecord(SQLHANDLE      hHandle,
	SQLSMALLINT    hType,
	RETCODE        RetCode);


class odbc
{
private:
	SQLHENV     hEnv;
	SQLHDBC     hDbc;
	SQLHSTMT    hStmt;
	WCHAR*      pwszConnStr;
	WCHAR*      wszInput;

public:
	odbc();
	void execQ(char id[31], char day[9], char HMS[9], int MS, double price, int volume, double turnover,
		double BP1, int BV1, double Ap1, int AV1, double BP2, int BV2, double AP2, int AV2,
		double BP3, int BV3, double AP3, int AV3, double BP4, int BV4, double AP4, int AV4,
		double BP5, int BV5, double AP5, int AV5);
	~odbc();
};



#endif