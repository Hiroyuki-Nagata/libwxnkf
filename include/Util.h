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
	static nkf_char E2wConv(nkf_char c2, nkf_char c1, FlagPool* flagPool);
	/**
	 * Shift_JISからEUC-JPへの変換
	 */
	static nkf_char S2eConv(nkf_char c2, nkf_char c1, nkf_char *p2,
			nkf_char *p1, FlagPool* flagPool);
	/**
	 * UTF-8からEUC-JPへの変換
	 */
	static nkf_char W2eConv(nkf_char c2, nkf_char c1, nkf_char c0, nkf_char *p2,
			nkf_char *p1, FlagPool* flagPool);
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
			nkf_char *p1, FlagPool* flagPool);
	static nkf_char X0212Shift(nkf_char c);
	static nkf_char X0212Unshift(nkf_char c);
	static nkf_char W16eConv(nkf_char val, nkf_char *p2, nkf_char *p1, FlagPool* flagPool);

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
	/**
	 * UnicodeからSJISへの変換
	 */
	static int UnicodeToJISCommon(nkf_char c2, nkf_char c1, nkf_char c0,
			nkf_char *p2, nkf_char *p1, FlagPool* flagPool);
	/**
	 * UnicodeからSJISへの変換
	 */
	static int UnicodeToJISCommon2(nkf_char c1, nkf_char c0,
			const unsigned short * const *pp, nkf_char psize, nkf_char *p2,
			nkf_char *p1, FlagPool* flagPool);
	/**
	 * テーブルから
	 */
	static int NKFStrCaseEql(const char *src, const char *target);

};
#endif /* UTIL_H_ */
