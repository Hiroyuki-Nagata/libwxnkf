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
	nkf->ShowUsage();

	std::cout << "オプションの設定" << std::endl << std::endl;
	nkf->SetOption("--ic=CP932 --oc=UTF-8");

	FILE* fp;
	if ((fp = fopen("cp932_sample.txt", "r")) == NULL) {
		printf("file open error!!\n");
		delete nkf;
		exit(EXIT_FAILURE);
	}
	std::wcout << nkf->Convert(fp) << std::endl;

	std::cout << "終わりです" << std::endl << std::endl;
	delete nkf;
}

