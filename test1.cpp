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

int testroutine(std::string option, std::string filename, std::string explanation, bool narrow);
void narrow(const std::wstring &src, std::string &dest);
void widen(const std::string &src, std::wstring &dest);

int main() {
	std::cout << "libnkfcppの動作確認テスト" << std::endl << std::endl;

	// CP932からUTF-8
	//testroutine("--ic=CP932 --oc=UTF-8", "./test/CP932.txt", "CP932からUTF-8への変換終わり", false);
	// EUC-JPからUTF-8
	//testroutine("--ic=EUC-JP --oc=UTF-8", "./test/EUC-JP.txt", "EUC-JPからUTF-8への変換終わり", true);
	// JISからUTF-8
	//testroutine("--ic=ISO-2022-JP --oc=UTF-8", "./test/JIS.txt", "ISO-2022-JPからUTF-8への変換終わり", true);

	// ファイルからファイルに書き出す
	LibNKF* nkf = new LibNKF();
	nkf->Convert("./test/CP932.txt", "./test.txt", "--ic=CP932 --oc=UTF-8");
	delete nkf;

//	LibNKF* nkf2 = new LibNKF();
//	nkf2->Convert("./test/Shift_JIS.txt", "./test2.txt", "--ic=Shift_JIS -w8");
//	delete nkf2;
}

int testroutine(std::string option, std::string filename, std::string explanation, bool flag) {
	LibNKF* nkf = new LibNKF();
	FILE* fp;

	std::cout << "オプションの設定" << std::endl << std::endl;
	nkf->SetOption(option);

	if ((fp = fopen(filename.c_str(), "r")) == NULL) {
		// CP932のファイルオープンに失敗
		if (nkf)
			delete nkf;
		exit( EXIT_FAILURE );
	}
	// 出力
	if (flag) {
		std::string out;
		narrow(nkf->Convert(fp), out);
		std::cout << out << std::endl << std::endl;
	} else {
		std::wcout << nkf->Convert(fp) << std::endl << std::endl;
	}

	std::cout << nkf->GetConvertedStringLength() << "バイト" << std::endl;
	std::cout << explanation << std::endl << std::endl;

	delete nkf;

	return 0;
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
