#include "odbc.h"
#include <string>
#include <vector>

void HandleDiagnosticRecord(SQLHANDLE      hHandle,
	SQLSMALLINT    hType,
	RETCODE        RetCode)
{
	SQLSMALLINT iRec = 0;
	SQLINTEGER  iError;
	WCHAR       wszMessage[1000];
	WCHAR       wszState[SQL_SQLSTATE_SIZE + 1];


	if (RetCode == SQL_INVALID_HANDLE)
	{
		fwprintf(stderr, L"Invalid handle!\n");
		return;
	}

	while (SQLGetDiagRecW(hType,
		hHandle,
		++iRec,
		wszState,
		&iError,
		wszMessage,
		(SQLSMALLINT)(sizeof(wszMessage) / sizeof(WCHAR)),
		(SQLSMALLINT *)NULL) == SQL_SUCCESS)
	{
		// Hide data truncated..
		if (wcsncmp(wszState, L"01004", 5))
		{
			fwprintf(stderr, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError);
		}
	}

}

odbc::odbc()
{
	hEnv = NULL;
	hDbc = NULL;
	hStmt = NULL;

	// Allocate an environment
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) == SQL_ERROR)
	{
		fwprintf(stderr, L"Unable to allocate an environment handle\n");
		exit(-1);
	}

	TRYODBC(hEnv,
		SQL_HANDLE_ENV,
		SQLSetEnvAttr(hEnv,
			SQL_ATTR_ODBC_VERSION,
			(SQLPOINTER)SQL_OV_ODBC3,
			0));

	// Allocate a connection
	TRYODBC(hEnv,
		SQL_HANDLE_ENV,
		SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc));

	pwszConnStr = L"Driver={ODBC Driver 13 for SQL Server};server=localhost;database=Futures;trusted_connection=Yes;";

	// Connect to the driver.
	TRYODBC(hDbc,
		SQL_HANDLE_DBC,
		SQLDriverConnectW(hDbc,
			GetDesktopWindow(),
			pwszConnStr,
			SQL_NTS,
			NULL,
			0,
			NULL,
			SQL_DRIVER_COMPLETE));

	TRYODBC(hDbc,
		SQL_HANDLE_DBC,
		SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt));
}

void odbc::execQ(char id[31], char day[9], char HMS[9], int MS, double price, int volume, double turnover, 
	double BP1, int BV1, double AP1, int AV1, double BP2, int BV2, double AP2, int AV2, 
	double BP3, int BV3, double AP3, int AV3, double BP4, int BV4, double AP4, int AV4, 
	double BP5, int BV5, double AP5, int AV5) {
	using namespace std;
	string HMS_string(HMS); HMS_string.erase(2, 1); HMS_string.erase(4, 1);
	vector<double*> Ps = {&BP2, &AP2, &BP3, &AP3, &BP4, &AP4, &BP5, &AP5};
	for (vector<double*>::iterator it = Ps.begin(); it < Ps.end(); it++) {
		if (**it > BP1 * 10 || **it < BP1 * 10) { **it = BP1; }
	}
	string q = "INSERT INTO " + string(id) + " VALUES(" + string(day) + ","+ HMS_string + "," + 
		to_string(MS/100) + "," + to_string(price) + "," + to_string(volume) + "," + to_string(turnover) + 
		"," + to_string(BP1) + "," + to_string(BV1) + "," + to_string(AP1) + "," + to_string(AV1) + 
		"," + to_string(BP2) + "," + to_string(BV2) + "," + to_string(AP2) + "," + to_string(AV2) + 
		"," + to_string(BP3) + "," + to_string(BV3) + "," + to_string(AP3) + "," + to_string(AV3) + 
		"," + to_string(BP4) + "," + to_string(BV4) + "," + to_string(AP4) + "," + to_string(AV4) + 
		"," + to_string(BP5) + "," + to_string(BV5) + "," + to_string(AP5) + "," + to_string(AV5) + ");";
	//cout << q << endl;
	const char* q_cstr = q.c_str();
	size_t origsize = strlen(q_cstr) + 1;
	const size_t newsize = 1000;
	size_t convertedChars = 0;
	wchar_t input[newsize];
	mbstowcs_s(&convertedChars, input, origsize, q_cstr, _TRUNCATE);
	//wcout << input << endl;
	SQLExecDirectW(hStmt, input, SQL_NTS);
}

odbc::~odbc()
{
	// Free ODBC handles and exit
	if (hStmt)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}

	if (hDbc)
	{
		SQLDisconnect(hDbc);
		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	}

	if (hEnv)
	{
		SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
	}
	delete hEnv;
	delete hDbc;
	delete hStmt;
	delete pwszConnStr;
	delete wszInput;
}
