/*
 * GuessConv.cpp
 *
 *  Created on: 2012/07/17
 * Contributor: Hiroyuki Nagata
 */

#include "GuessConv.h"

int GuessConv::GuessIConv(FILE* f, nkf_char c1, nkf_char c2) {
	int ret;
	int hold_index;
	nkf_char c3, c4;

	/** it must NOT be in the kanji shifte sequence      */
	/** it must NOT be written in JIS7                   */
	/** and it must be after 2 byte 8bit code            */

	hold_count = 0;
	pushHoldBuf(c1);
	pushHoldBuf(c2);

	while ((c2 = LibNKF::StdGetC(f)) != EOF) {
		if (c2 == ESC) {
			LibNKF::StdUnGetC(c2, f);
			break;
		}
		code_status(c2);
		if (pushHoldBuf(c2) == EOF || FlagPool::estab_f) {
			break;
		}
	}

	if (!estab_f) {
		struct input_code *p = input_code_list;
		struct input_code *result = p;
		if (c2 == EOF) {
			code_status(c2);
		}
		while (p->name) {
			if (p->status_func && p->score < result->score) {
				result = p;
			}
			p++;
		}
		set_iconv(TRUE, result->iconv_func);
	}

	/** now,
	 ** 1) EOF is detected, or
	 ** 2) Code is established, or
	 ** 3) Buffer is FULL (but last word is pushed)
	 **
	 ** in 1) and 3) cases, we continue to use
	 ** Kanji codes by oconv and leave estab_f unchanged.
	 **/

	ret = c2;
	hold_index = 0;
	while (hold_index < hold_count) {
		c1 = hold_buf[hold_index++];
		if (nkf_char_unicode_p(c1)) {
			(*oconv)(0, c1);
			continue;
		} else if (c1 <= DEL) {
			(*iconv)(0, c1, 0);
			continue;
		} else if (iconv == s_iconv && 0xa1 <= c1 && c1 <= 0xdf) {
			(*iconv)(JIS_X_0201_1976_K, c1, 0);
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
			code_status(c2);
		}
		c3 = 0;
		switch ((*iconv)(c1, c2, 0)) { /* can be EUC/SJIS/UTF-8 */
		case -2:
			/* 4 bytes UTF-8 */
			if (hold_index < hold_count) {
				c3 = hold_buf[hold_index++];
			} else if ((c3 = LibNKF::StdGetC(f)) == EOF) {
				ret = EOF;
				break;
			}
			code_status(c3);
			if (hold_index < hold_count) {
				c4 = hold_buf[hold_index++];
			} else if ((c4 = LibNKF::StdGetC(f)) == EOF) {
				c3 = ret = EOF;
				break;
			}
			code_status(c4);
			(*iconv)(c1, c2, (c3 << 8) | c4);
			break;
		case -1:
			/* 3 bytes EUC or UTF-8 */
			if (hold_index < hold_count) {
				c3 = hold_buf[hold_index++];
			} else if ((c3 = LibNKF::StdGetC(f)) == EOF) {
				ret = EOF;
				break;
			} else {
				code_status(c3);
			}
			(*iconv)(c1, c2, c3);
			break;
		}
		if (c3 == EOF)
			break;
	}
	return ret;
}

nkf_char GuessConv::pushHoldBuf(nkf_char c2) {
	if (hold_count >= HOLD_SIZE * 2)
		return (EOF);
	hold_buf[hold_count++] = c2;
	return ((hold_count >= HOLD_SIZE * 2) ? EOF : hold_count);
}
