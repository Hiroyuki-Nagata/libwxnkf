/*
 * NKFNativeEncodings.h
 *
 *  Created on: 2012/07/11
 * Contributor: Hiroyuki Nagata
 */

#ifndef NKFNATIVEENCODINGS_H_
#define NKFNATIVEENCODINGS_H_

#include <string>
#include "FlagPool.h"
#include "UTF8Table.h"
#include "NKFBaseConfig.h"
/**
 * 構造体：nkf_native_encodingをクラス化したもの
 * "ASCII" "ISO-2022-JP" "Shift_JIS" "EUC-JP" "UTF-8" "UTF-16" "UTF-32" が存在する
 * フラグによってIconvメソッドとOconvメソッドの処理を変える
 */
class NKFNativeEncoding {
public:
	NKFNativeEncoding(const std::string name);
	int id;
	std::string name;
	std::string iconvName;
	std::string oconvName;
	nkf_char Iconv(nkf_char c2, nkf_char c1, nkf_char c0, FlagPool* flagPool);
	void Oconv(nkf_char c2, nkf_char c1, FlagPool* flagPool);

private:
	/**
	 * s_iconv
	 */
	nkf_char SIconv(nkf_char c2, nkf_char c1, nkf_char c0, FlagPool* flagPool);
	/**
	 * e_iconv
	 */
	nkf_char EIconv(nkf_char c2, nkf_char c1, nkf_char c0, FlagPool* flagPool);
	/**
	 * w_iconv
	 */
	nkf_char WIconv(nkf_char c2, nkf_char c1, nkf_char c0, FlagPool* flagPool);
	/**
	 * w_iconv16
	 */
	nkf_char WIconv16(nkf_char c2, nkf_char c1, nkf_char c0, FlagPool* flagPool);
	/**
	 * w_iconv32
	 */
	nkf_char WIconv32(nkf_char c2, nkf_char c1, nkf_char c0, FlagPool* flagPool);
	/**
	 * j_oconv
	 */
	void JOconv(nkf_char c2, nkf_char c1, FlagPool* flagPool);
	/**
	 * s_oconv
	 */
	void SOconv(nkf_char c2, nkf_char c1, FlagPool* flagPool);
	/**
	 * e_oconv
	 */
	void EOconv(nkf_char c2, nkf_char c1, FlagPool* flagPool);
	/**
	 * w_oconv
	 */
	void WOconv(nkf_char c2, nkf_char c1, FlagPool* flagPool);
	/**
	 * w_oconv16
	 */
	void WOconv16(nkf_char c2, nkf_char c1, FlagPool* flagPool);
	/**
	 * w_oconv32
	 */
	void WOconv32(nkf_char c2, nkf_char c1, FlagPool* flagPool);
};

#endif /* NKFNATIVEENCODINGS_H_ */
