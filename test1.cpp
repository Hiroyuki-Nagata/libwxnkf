/*
 * test1.cpp
 *
 * libnkfcppの動作確認を行う
 *
 *  Created on: 2012/07/22
 * Contributor: Hiroyuki Nagata
 */

#include <iostream>
#include "LibNKF.h"

void narrow(const std::wstring &src, std::string &dest);
void widen(const std::string &src, std::wstring &dest);

int main() {
	std::cout << "libnkfcppの動作確認テスト" << std::endl << std::endl;

	LibNKF* nkf = new LibNKF();
	nkf->Convert("./test/CP932.txt", "./CP932test.txt", "--ic=CP932 --oc=UTF-8");
	std::wcout << nkf->GetConvertedString();
	delete nkf;
}

/**
 * ワイド文字列からマルチバイト文字列(ロケール依存)
 */
void narrow(const std::wstring& src, std::string& dest) {
	char *mbs = new char[src.length() * MB_CUR_MAX + 1];
	wcstombs(mbs, src.c_str(), src.length() * MB_CUR_MAX + 1);
	dest = mbs;
	delete[] mbs;
}
/**
 * マルチバイト文字列からワイド文字列(ロケール依存)
 */
void widen(const std::string& src, std::wstring& dest) {
	wchar_t *wcs = new wchar_t[src.length() + 1];
	mbstowcs(wcs, src.c_str(), src.length() + 1);
	dest = wcs;
	delete[] wcs;
}
