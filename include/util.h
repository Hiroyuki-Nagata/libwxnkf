/*
 * util.h
 *
 * nkf common method
 *  Created on: 2012/07/12
 * Contributor: Hiroyuki Nagata
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <cstring>
#include "flagset.h"
#include "wxnkfencoding.h"

class Util {

public:
	/**
	 * judge baseID and return wxNKFEncoding instance
	 */
	static void NKFEncFind(const char *name, wxNKFEncoding* enc);
	/**
	 * return wxNKFEncoding class per encoding ID
	 */
	static void NKFEncFromIndex(int idx, wxNKFEncoding* enc);
	/**
	 * get encode ID by name
	 */
	static int NKFEncFindIndex(const char *name);
	/**
	 * Initialize character code
	 */
	static NKFNativeEncoding* NKFDefaultEncoding();
	/**
	 * convert EUC-JP to UTF-8
	 */
	static nkf_char E2wConv(nkf_char c2, nkf_char c1,
			std::bitset<nkf_flag_num> nkfFlags);
	/**
	 * convert Shift_JIS to EUC-JP
	 */
	static nkf_char S2eConv(nkf_char c2, nkf_char c1, nkf_char *p2,
			nkf_char *p1, std::bitset<nkf_flag_num> nkfFlags);
	/**
	 * convert UTF-8 to EUC-JP
	 */
	static nkf_char W2eConv(nkf_char c2, nkf_char c1, nkf_char c0, nkf_char *p2,
			nkf_char *p1, std::bitset<nkf_flag_num> nkfFlags);
	/**
	 * convert UTF-8 to Shift_JIS
	 */
	static nkf_char E2sConv(nkf_char c2, nkf_char c1, nkf_char *p2,
			nkf_char *p1, std::bitset<nkf_flag_num> nkfFlags);
	/**
	 * get default os locale
	 */
	static void NKFLocaleEncoding(wxNKFEncoding* enc);
	/**
	 * convert Unicode to UTF-8
	 */
	static void NKFUnicodeToUTF8(nkf_char val, nkf_char *p1, nkf_char *p2,
			nkf_char *p3, nkf_char *p4);
	/**
	 * convert UTF-8 to Unicode
	 */
	static nkf_char NKFUTF8ToUnicode(nkf_char c1, nkf_char c2, nkf_char c3,
			nkf_char c4);
	/**
	 * convert Unicode to SJIS
	 */
	static int UnicodeToJISCommon(nkf_char c2, nkf_char c1, nkf_char c0,
			nkf_char *p2, nkf_char *p1, std::bitset<nkf_flag_num> nkfFlags);
	/**
	 * convert Unicode to SJIS 2
	 */
	static int UnicodeToJISCommon2(nkf_char c1, nkf_char c0,
			const unsigned short * const *pp, nkf_char psize, nkf_char *p2,
			nkf_char *p1, std::bitset<nkf_flag_num> nkfFlags);
	/**
	 * convert UTF-16 to EUC-JP
	 */
	static nkf_char W16eConv(nkf_char val, nkf_char *p2, nkf_char *p1,
			std::bitset<nkf_flag_num> nkfFlags);

	static nkf_char X0212Shift(nkf_char c);
	static nkf_char X0212Unshift(nkf_char c);

private:
	/**
	 * ２つの引数を先頭から比較してboolを返す、比較する長さは１つ目の引数の長さ
	 */
	static bool StrncmpFromHead(const char* charCode, const char* name);
};

#endif /* UTIL_H_ */
