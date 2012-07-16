/*
 * GuessConv.h
 * 文字コードを推定しながら変換するメソッドを専用のクラスとした
 *  Created on: 2012/07/17
 * Contributor: Hiroyuki Nagata
 */

#ifndef GUESSCONV_H_
#define GUESSCONV_H_

#include "FlagPool.h"
#include "NKFNativeEncodings.h"
#include "NKFEncoding.h"
#include "LibNKF.h"

class GuessConv {
public:
	/**
	 * nkf内のh_conv()にあたるメソッド
	 */
	static int GuessIConv(FILE *f, nkf_char c1, nkf_char c2);
private:
	static nkf_char hold_buf[HOLD_SIZE * 2];
	static int hold_count = 0;
	/**
	 *
	 */
	static nkf_char pushHoldBuf(nkf_char c2);
};

#endif /* GUESSCONV_H_ */
