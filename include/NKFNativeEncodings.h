/*
 * NKFNativeEncodings.h
 *
 *  Created on: 2012/07/11
 * Contributor: Hiroyuki Nagata
 */

#ifndef NKFNATIVEENCODINGS_H_
#define NKFNATIVEENCODINGS_H_

#include "FlagPool.h"
#include "UTF8Table.h"
#include "NKFBaseConfig.h"

/**
 * 構造体：nkf_native_encodingをクラス化したもの
 * フラグによってIconvメソッドとOconvメソッドの処理を変える
 */
class NKFNativeEncoding {
public:
	NKFNativeEncoding();
	const char* name;
	const char* iconvName;
	const char* oconvName;
	nkf_char Iconv(nkf_char c2, nkf_char c1, nkf_char c0);
	void Oconv(nkf_char c2, nkf_char c1);
};

/**
 * 構造体と関係しないメソッドを集合させたクラス
 */
class EncodingUtil {
public:
	static nkf_char E2sConv(nkf_char c2, nkf_char c1, nkf_char *p2,
			nkf_char *p1);
	static nkf_char S2eConv(nkf_char c2, nkf_char c1, nkf_char *p2,
			nkf_char *p1);

	static nkf_char X0212Shift(nkf_char c);
	static nkf_char X0212Unshift(nkf_char c);
	static nkf_char W16eConv(nkf_char val, nkf_char *p2, nkf_char *p1);

	static void NKFUnicodeToUTF8(nkf_char val, nkf_char *p1, nkf_char *p2,
			nkf_char *p3, nkf_char *p4);

	static int UnicodeToJISCommon(nkf_char c2, nkf_char c1, nkf_char c0,
			nkf_char *p2, nkf_char *p1);
	static int UnicodeToJISCommon2(nkf_char c1, nkf_char c0,
			const unsigned short * const *pp, nkf_char psize, nkf_char *p2,
			nkf_char *p1);
};

#endif /* NKFNATIVEENCODINGS_H_ */
