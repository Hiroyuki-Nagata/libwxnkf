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
	std::cout << "libnkfcppの動作確認テスト" << std::endl;

	LibNKF* nkf;
	nkf->ShowUsage();
}


