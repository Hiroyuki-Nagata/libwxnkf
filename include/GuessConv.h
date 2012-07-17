/*
 * GuessConv.h
 * 文字コードを推定しながら変換するメソッドを専用のクラスとした
 *  Created on: 2012/07/17
 * Contributor: Hiroyuki Nagata
 */

#ifndef GUESSCONV_H_
#define GUESSCONV_H_

#include <string>
#include "FlagPool.h"
#include "NKFNativeEncodings.h"
#include "NKFEncoding.h"
#include "LibNKF.h"
#include "InputCodeList.h"

class GuessConv {
public:
	/**
	 * nkf内のh_conv()にあたるメソッド
	 */
	static int GuessIConv(FILE *f, nkf_char c1, nkf_char c2);
	/**
	 * 文字コードの状態を判定するしてクラスに設定する
	 */
	static void CodeStatus(nkf_char c);

private:
	static nkf_char hold_buf[HOLD_SIZE * 2];

	static int hold_count;

	/**
	 * ファイルポインタから1バイトnkf用のデータを読み取って返す
	 */
	static nkf_char StdGetC(FILE *f) {
		return getc(f);
	}
	;
	/**
	 * ファイルポインタではなく引数に指定したnkf_charを返す
	 */
	static nkf_char StdUnGetC(nkf_char c, FILE *f) {
		return c;
	}
	;
	/**
	 *
	 */
	static nkf_char PushHoldBuf(nkf_char c2);
};

#endif /* GUESSCONV_H_ */
