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

int main() {
	std::cout << "libnkfcppの動作確認テスト" << std::endl << std::endl;

	LibNKF* nkf = new LibNKF();
	FILE* fp;

	std::cout << "オプションの設定" << std::endl << std::endl;
	nkf->SetOption("--ic=CP932 --oc=UTF-8");

	if ((fp = fopen("./test/CP932.txt", "r")) == NULL) {
		// CP932のファイルオープンに失敗
		if (nkf)
			delete nkf;
		exit( EXIT_FAILURE );
	}
	// 出力
	std::wcout << nkf->Convert(fp) << std::endl << std::endl;
	std::cout << nkf->GetConvertedStringLength() << "バイト" << std::endl;
	std::cout << "CP932からUTF-8への変換終わりです" << std::endl << std::endl;

	delete nkf;
}
