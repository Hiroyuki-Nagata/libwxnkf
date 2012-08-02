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

int testroutine(std::string option, std::string filename, std::string explanation);

int main() {
	std::cout << "libnkfcppの動作確認テスト" << std::endl << std::endl;

	// CP932からUTF-8
	testroutine("--ic=CP932 --oc=UTF-8", "./test/CP932.txt", "CP932からUTF-8への変換終わり");
	// EUC-JPからUTF-8
	testroutine("--ic=EUC-JP --oc=UTF-8", "./test/EUC-JP.txt", "EUC-JPからUTF-8への変換終わり");
	// JISからUTF-8
	testroutine("--ic=ISO-2022-JP --oc=UTF-8", "./test/JIS.txt", "ISO-2022-JPからUTF-8への変換終わり");
}

int testroutine(std::string option, std::string filename, std::string explanation) {
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
	std::wcout << nkf->Convert(fp) << std::endl << std::endl;
	std::cout << nkf->GetConvertedStringLength() << "バイト" << std::endl;
	std::cout << explanation << std::endl << std::endl;

	delete nkf;
}
