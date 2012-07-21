/*
 * NKFEncoding.h
 * 構造体nkf_encodingのクラス化
 *  Created on: 2012/07/16
 * Contributor: Hiroyuki Nagata
 */

#ifndef NKFENCODING_H_
#define NKFENCODING_H_

#include "NKFNativeEncodings.h"

class NKFEncoding {
public:
	NKFEncoding();
    int id;
    std::string name;
    NKFNativeEncoding* baseEncoding;
};

#endif /* NKFENCODING_H_ */
