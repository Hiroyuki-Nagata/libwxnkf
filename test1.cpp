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
	std::cout << "libnkfcppの動作確認テスト\n" << std::endl;

	LibNKF* nkf = new LibNKF();
	nkf->ShowUsage();

	std::cout << "メソッドを呼んでみます\n" << std::endl;
	nkf->SetOption("--ic=CP932 --oc=UTF-8");

	std::cout << "終わりです\n" << std::endl;
	delete nkf;
}


