/*
 * Util.h
 *	NKFの中で共通で使われているメソッドを集めたクラス
 *  Created on: 2012/07/12
 * Contributor: Hiroyuki Nagata
 */

#ifndef UTIL_H_
#define UTIL_H_

#include "FlagPool.h"
#include "UTF8Table.h"
#include "NKFEncoding.h"

/*
 * エンコード名とIDのテーブル
 */
struct {
	const char *name;
	const int id;
} encoding_name_to_id_table[] = { { "US-ASCII", ASCII }, { "ASCII", ASCII }, {
		"646", ASCII }, { "ROMAN8", ASCII }, { "ISO-2022-JP", ISO_2022_JP }, {
		"ISO2022JP-CP932", CP50220 }, { "CP50220", CP50220 }, { "CP50221",
		CP50221 }, { "CSISO2022JP", CP50221 }, { "CP50222", CP50222 }, {
		"ISO-2022-JP-1", ISO_2022_JP_1 }, { "ISO-2022-JP-3", ISO_2022_JP_3 }, {
		"ISO-2022-JP-2004", ISO_2022_JP_2004 }, { "SHIFT_JIS", SHIFT_JIS }, {
		"SJIS", SHIFT_JIS }, { "MS_Kanji", SHIFT_JIS }, { "PCK", SHIFT_JIS }, {
		"WINDOWS-31J", WINDOWS_31J }, { "CSWINDOWS31J", WINDOWS_31J }, {
		"CP932", WINDOWS_31J }, { "MS932", WINDOWS_31J },
		{ "CP10001", CP10001 }, { "EUCJP", EUC_JP }, { "EUC-JP", EUC_JP }, {
				"EUCJP-NKF", EUCJP_NKF }, { "CP51932", CP51932 }, { "EUC-JP-MS",
				EUCJP_MS }, { "EUCJP-MS", EUCJP_MS }, { "EUCJPMS", EUCJP_MS }, {
				"EUC-JP-ASCII", EUCJP_ASCII }, { "EUCJP-ASCII", EUCJP_ASCII }, {
				"SHIFT_JISX0213", SHIFT_JISX0213 }, { "SHIFT_JIS-2004",
				SHIFT_JIS_2004 }, { "EUC-JISX0213", EUC_JISX0213 }, {
				"EUC-JIS-2004", EUC_JIS_2004 }, { "UTF-8", UTF_8 }, { "UTF-8N",
				UTF_8N }, { "UTF-8-BOM", UTF_8_BOM }, { "UTF8-MAC", UTF8_MAC },
		{ "UTF-8-MAC", UTF8_MAC }, { "UTF-16", UTF_16 },
		{ "UTF-16BE", UTF_16BE }, { "UTF-16BE-BOM", UTF_16BE_BOM }, {
				"UTF-16LE", UTF_16LE }, { "UTF-16LE-BOM", UTF_16LE_BOM }, {
				"UTF-32", UTF_32 }, { "UTF-32BE", UTF_32BE }, { "UTF-32BE-BOM",
				UTF_32BE_BOM }, { "UTF-32LE", UTF_32LE }, { "UTF-32LE-BOM",
				UTF_32LE_BOM }, { "BINARY", BINARY }, { NULL, -1 } };

class Util {
public:
	/**
	 * 文字コード名からNKFEncodingクラスを判別し、インスタンスを返す
	 */
	static NKFEncoding* NKFEncFind(const char *name);
	/**
	 * IDに対応するNKFEncodingクラスを返却する
	 */
	static NKFEncoding* NKFEncFromIndex(int idx);
	/**
	 * 文字コードを初期化する
	 */
	static NKFEncoding* NKFDefaultEncoding();
	/**
	 * EUC-JPからUTF-8への変換
	 */
	static nkf_char E2wConv(nkf_char c2, nkf_char c1);
	/**
	 * Shift_JISからEUC-JPへの変換
	 */
	static nkf_char S2eConv(nkf_char c2, nkf_char c1, nkf_char *p2,
			nkf_char *p1);
	/**
	 * UTF-8からEUC-JPへの変換
	 */
	static nkf_char W2eConv(nkf_char c2, nkf_char c1, nkf_char c0, nkf_char *p2,
			nkf_char *p1);
	/**
	 * 文字コード名から文字コードに対応するIDを取得する
	 */
	static int NKFEncFindIndex(const char *name);
	/**
	 * 文字コードを表すIDを元にNKFEncodingクラスを設定して返す
	 */
	static NKFEncoding* NKFEncodingTable(int idx);
	/**
	 * OSのロケールからデフォルトのNKFEncodingを取得し返す
	 */
	static NKFEncoding* NKFLocaleEncoding();

	static nkf_char E2sConv(nkf_char c2, nkf_char c1, nkf_char *p2,
			nkf_char *p1);
	static nkf_char X0212Shift(nkf_char c);
	static nkf_char X0212Unshift(nkf_char c);
	static nkf_char W16eConv(nkf_char val, nkf_char *p2, nkf_char *p1);

	/**
	 * UnicodeからUTF-8への変換
	 */
	static void NKFUnicodeToUTF8(nkf_char val, nkf_char *p1, nkf_char *p2,
			nkf_char *p3, nkf_char *p4);
	/**
	 * UTF-8からUnicodeへの変換
	 */
	static nkf_char NKFUTF8ToUnicode(nkf_char c1, nkf_char c2, nkf_char c3,
			nkf_char c4);

	static int UnicodeToJISCommon(nkf_char c2, nkf_char c1, nkf_char c0,
			nkf_char *p2, nkf_char *p1);
	static int UnicodeToJISCommon2(nkf_char c1, nkf_char c0,
			const unsigned short * const *pp, nkf_char psize, nkf_char *p2,
			nkf_char *p1);
	/**
	 * テーブルから
	 */
	static int NKFStrCaseEql(const char *src, const char *target);
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
