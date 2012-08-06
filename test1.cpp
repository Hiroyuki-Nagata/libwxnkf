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

	LibNKF* nkf1 = new LibNKF();
	std::cout << "CP932からUTF-8" << std::endl << std::endl;
	nkf1->Convert("./test/CP932.txt", "--ic=CP932 --oc=UTF-8");
	std::wcout << nkf1->GetConvertedString();
	std::cout << std::endl << std::endl;
	delete nkf1;

	LibNKF* nkf2 = new LibNKF();
	std::cout << "EUC-JPからUTF-8" << std::endl << std::endl;
	nkf2->Convert("./test/EUC-JP.txt", "--ic=EUCJP-NKF --oc=UTF-8");
	// std::stringに変換する
	std::string euc;
	narrow(nkf2->GetConvertedString(), euc);
	std::cout << euc << std::endl << std::endl;
	delete nkf2;

	LibNKF* nkf3 = new LibNKF();
	std::cout << "JISからUTF-8" << std::endl << std::endl;
	nkf3->Convert("./test/JIS.txt", "--ic=ISO-2022-JP --oc=UTF-8");
	// std::stringに変換する
	std::string jis;
	narrow(nkf3->GetConvertedString(), jis);
	std::cout << jis << std::endl << std::endl;
	delete nkf3;

	LibNKF* nkf4 = new LibNKF();
	std::cout << "UnicodeからUTF-8" << std::endl << std::endl;
	nkf4->Convert("./test/Unicode.txt", "--ic=UTF-16 --oc=UTF-8");
	// std::stringに変換する
	std::string uni;
	narrow(nkf4->GetConvertedString(), uni);
	std::cout << uni << std::endl << std::endl;
	delete nkf4;
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
