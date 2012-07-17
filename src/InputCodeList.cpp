/*
 * InputCodeList.cpp
 *
 *  Created on: 2012/07/17
 *      Author: learning
 */

#include "InputCodeList.h"

InputCode::InputCode() {
}

/**
 * 指定された文字コードごとのメソッドを提供する
 */
void InputCode::StatusFunc(InputCode* inputCode, nkf_char c) {
	if (this->name == "EUC-JP") {
		EStatus(inputCode, c);
	} else if (this->name == "Shift_JIS") {
		SStatus(inputCode, c);
	} else if (this->name == "UTF-8") {
		WStatus(inputCode, c);
	} else {
		// do nothing
	}
}

nkf_char InputCode::IconvFunc(nkf_char c2, nkf_char c1, nkf_char c0) {
	if (this->name == "EUC-JP") {
		return EIconv(c2, c1, c0);
	} else if (this->name == "Shift_JIS") {
		return SIconv(c2, c1, c0);
	} else if (this->name == "UTF-8") {
		return WIconv(c2, c1, c0);
	} else if (this->name == "UTF-16") {
		return WIconv16(c2, c1, c0);
	} else if (this->name == "UTF-32") {
		return WIconv32(c2, c1, c0);
	} else {
		// do nothing
	}
}

void InputCode::EStatus(InputCode* inputCode, nkf_char c) {
	switch (inputCode->stat) {
	case -1:
		InputCode::StatusCheck(ptr, c);
		break;
	case 0:
		if (c <= DEL) {
			break;
		} else if (nkf_char_unicode_p(c)) {
			break;
		} else if (SS2 == c || (0xa1 <= c && c <= 0xfe)) {
			ptr->stat = 1;
			status_push_ch(ptr, c);
		} else if (0x8f == c) {
			ptr->stat = 2;
			status_push_ch(ptr, c);
		} else {
			status_disable (ptr);
		}
		break;
	case 1:
		if (0xa1 <= c && c <= 0xfe) {
			status_push_ch(ptr, c);
			code_score (ptr);
			status_clear(ptr);
		} else {
			status_disable (ptr);
		}
		break;
	case 2:
		if (0xa1 <= c && c <= 0xfe) {
			ptr->stat = 1;
			status_push_ch(ptr, c);
		} else {
			status_disable (ptr);
		}
	}
}

void InputCode::SStatus(InputCode* inputCode, nkf_char c) {
}

void InputCode::WStatus(InputCode* inputCode, nkf_char c) {
}

nkf_char InputCode::SIconv(nkf_char c2, nkf_char c1, nkf_char c0) {
}

nkf_char InputCode::EIconv(nkf_char c2, nkf_char c1, nkf_char c0) {
}

nkf_char InputCode::WIconv(nkf_char c2, nkf_char c1, nkf_char c0) {
}

nkf_char InputCode::WIconv16(nkf_char c2, nkf_char c1, nkf_char c0) {
}

nkf_char InputCode::WIconv32(nkf_char c2, nkf_char c1, nkf_char c0) {
}

void InputCode::StatusCheck(InputCode* ptr, nkf_char c) {
	if (c <= DEL && FlagPool::estab_f) {
		InputCode::StatusReset(ptr);
	}
}

void InputCode::StatusReset(InputCode* ptr) {
	InputCode::StatusClear(ptr);
	ptr->score = SCORE_INIT;
}

void InputCode::StatusClear(InputCode* ptr) {
    ptr->stat = 0;
    ptr->index = 0;
}



