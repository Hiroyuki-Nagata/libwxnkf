/*
 * Util.h
 *
 *  Created on: 2012/07/12
 *      Author: learning
 */

#ifndef UTIL_H_
#define UTIL_H_

#include "FlagPool.h"
#include "NKFNativeEncodings.h"

class Util {
public:
	/**
	 * 文字コード名からNKFEncodingクラスを判別し、インスタンスを返す
	 */
	static NKFNativeEncoding* NKFEncFind(const char *name);
	/**
	 * IDに対応するNKFEncodingクラスを返却する
	 */
	static NKFNativeEncoding* NKFEncFromIndex(int idx);
	/**
	 * 文字コードを初期化する
	 */
	static NKFNativeEncoding* NKFDefaultEncoding();
private:
	/**
	 * 文字コード名から文字コードに対応するIDを取得する
	 */
	static int NKFEncFindIndex(const char *name);

	static NKFNativeEncoding* NKFLocaleEncoding();

	/**
	 * デフォルトのロケール名を取得する
	 */
	static const char*
	NKFLocaleCharmap() {
#ifdef HAVE_LANGINFO_H
		return nl_langinfo(CODESET);
#elif defined(__WIN32__)
		static char buf[16];
		sprintf(buf, "CP%d", GetACP());
		return buf;
#endif
		return NULL;
	}
	;
};

#endif /* UTIL_H_ */
