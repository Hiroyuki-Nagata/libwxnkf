/*
 * GuessConv.cpp
 *
 *  Created on: 2012/07/17
 * Contributor: Hiroyuki Nagata
 */

#include "GuessConv.h"

int GuessConv::GuessIConv(FILE* f, nkf_char c1, nkf_char c2, FlagPool* flagPool,
		NKFEncoding* inputEncoding, NKFEncoding* outputEncoding) {

	int ret;
	int hold_index;
	nkf_char c3, c4;

	/** it must NOT be in the kanji shifte sequence      */
	/** it must NOT be written in JIS7                   */
	/** and it must be after 2 byte 8bit code            */

	hold_count = 0;
	iconvForCheck = "";
	std::string inputCodeList[] = { "EUC-JP", "Shift_JIS", "UTF-8", "UTF-16",
			"UTF-32" };
	PushHoldBuf(c1);
	PushHoldBuf(c2);

	/**
	 * nkf_charの2文字目が終端に達するまでファイルポインタからgetcを繰り返す
	 */
	while ((c2 = LibNKF::StdGetC(f)) != EOF) {
		if (c2 == ESC) {
			LibNKF::StdUnGetC(c2, f);
			break;
		}
		// 入力された文字コードを判定する
		CodeStatus(c2, flagPool);

		if (PushHoldBuf(c2) == EOF || flagPool->estab_f) {
			// 1024*2byteを超えたらブレークする
			break;
		}
	}

	/**
	 * 文字コードが確定していない場合もう一度判別を行う
	 */
	if (!flagPool->estab_f) {
		InputCode* p;
		InputCode* result = p;
		if (c2 == EOF) {
			CodeStatus(c2, flagPool);
		}

		for (int i = 0; i < 5; i++) {
			p->name = inputCodeList[i];
			if (p->score < result->score) {
				// scoreが高いほうが入力文字コードとなる
				result = p;
			}
		}
		SetIconv(TRUE, result->name, flagPool);
	}

	/** now,
	 ** 1) EOF is detected, or
	 ** 2) Code is established, or
	 ** 3) Buffer is FULL (but last word is pushed)
	 **
	 ** in 1) and 3) cases, we continue to use
	 ** Kanji codes by oconv and leave estab_f unchanged.
	 **
	 ** 文字コードを判定しながら変換結果を出力していく
	 **/

	ret = c2;
	hold_index = 0;
	while (hold_index < hold_count) {
		c1 = hold_buf[hold_index++];
		if (nkf_char_unicode_p(c1)) {
			outputEncoding->baseEncoding->Oconv(0, c1, flagPool);
			continue;
		} else if (c1 <= DEL) {
			inputEncoding->baseEncoding->Iconv(0, c1, 0, flagPool);
			continue;
		} else if (inputEncoding->baseEncoding->iconvName == "s_iconv"
				&& 0xa1 <= c1 && c1 <= 0xdf) {
			inputEncoding->baseEncoding->Iconv(JIS_X_0201_1976_K, c1,
					0, flagPool);
			continue;
		}
		if (hold_index < hold_count) {
			c2 = hold_buf[hold_index++];
		} else {
			c2 = LibNKF::StdGetC(f);
			if (c2 == EOF) {
				c4 = EOF;
				break;
			}
			CodeStatus(c2, flagPool);
		}
		c3 = 0;
		switch (inputEncoding->baseEncoding->Iconv(c1, c2, 0, flagPool)) { /* can be EUC/SJIS/UTF-8 */
		case -2:
			/* 4 bytes UTF-8 */
			if (hold_index < hold_count) {
				c3 = hold_buf[hold_index++];
			} else if ((c3 = LibNKF::StdGetC(f)) == EOF) {
				ret = EOF;
				break;
			}
			CodeStatus(c3, flagPool);
			if (hold_index < hold_count) {
				c4 = hold_buf[hold_index++];
			} else if ((c4 = LibNKF::StdGetC(f)) == EOF) {
				c3 = ret = EOF;
				break;
			}
			CodeStatus(c4, flagPool);
			inputEncoding->baseEncoding->Iconv(c1, c2, (c3 << 8) | c4, flagPool);
			break;
		case -1:
			/* 3 bytes EUC or UTF-8 */
			if (hold_index < hold_count) {
				c3 = hold_buf[hold_index++];
			} else if ((c3 = LibNKF::StdGetC(f)) == EOF) {
				ret = EOF;
				break;
			} else {
				CodeStatus(c3, flagPool);
			}
			inputEncoding->baseEncoding->Iconv(c1, c2, c3, flagPool);
			break;
		}
		if (c3 == EOF)
			break;
	}
	return ret;
}

void GuessConv::CodeStatus(nkf_char c, FlagPool* flagPool) {
	/**
	 * 最初に入力コードが何であるか総当りで調べる
	 * "EUC-JP","Shift_JIS","UTF-8","UTF-16","UTF-32"の順
	 */
	int action_flag = 1;
	InputCode* result = 0;
	std::string inputCodeList[] = { "EUC-JP", "Shift_JIS", "UTF-8", "UTF-16",
			"UTF-32" };

	/**
	 * InputCode->statが決まるまでループする
	 */
	for (int i = 0; i < 5; i++) {
		InputCode* p;
		p->name = inputCodeList[i];
		p->StatusFunc(p, c, flagPool);
		if (p->stat > 0) {
			action_flag = 0;
		} else if (p->stat == 0) {
			if (result) {
				action_flag = 0;
			} else {
				result = p;
			}
		}
		++i;
	}

	if (action_flag) {
		// resultが確定している場合
		if (result && !flagPool->estab_f) {
			SetIconv(TRUE, result->name, flagPool);
		}
	}
}
/**
 * 入力する文字コードとその処理を決定する
 * inputEncoding, inputCodeNameの決定を行う
 */
void GuessConv::SetIconv(nkf_char f, std::string name, FlagPool* flagPool) {

	/**
	 * inputEncodingが確定していない場合確定フラグを立てる
	 */
	if (f || !LibNKF::inputEncoding) {
		if (flagPool->estab_f != f) {
			flagPool->estab_f = f;
		}
	}
	/**
	 * -TRUE means "FORCE"
	 * inputEncodingが確定していない場合
	 */
	if (name.empty() && (f == -TRUE || !LibNKF::inputEncoding->name.empty())) {
		LibNKF::inputEncoding->name = name;
	}
	if (flagPool->estab_f && iconvForCheck != name) {
		// 入力文字コードが確定している場合
		LibNKF::inputCodeName = name;
		iconvForCheck = name;
	}
}

nkf_char GuessConv::PushHoldBuf(nkf_char c2) {
	if (hold_count >= HOLD_SIZE * 2)
		return (EOF);
	hold_buf[hold_count++] = c2;
	return ((hold_count >= HOLD_SIZE * 2) ? EOF : hold_count);
}
