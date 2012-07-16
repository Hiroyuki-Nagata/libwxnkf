/*
 * NKFNativeEncodings.cpp
 *
 *  Created on: 2012/07/11
 * Contributor: Hiroyuki Nagata
 */

#include "NKFNativeEncodings.h"

NKFNativeEncoding::NKFNativeEncoding(const std::string name) {

	// "ASCII" "ISO-2022-JP" "Shift_JIS" "EUC-JP" "UTF-8" "UTF-16" "UTF-32"のうちどれかがはいる
	this->name = name;

	// IDを設定する
	if (this->name == "ASCII") {
		this->id = ASCII;
	} else if (this->name == "ISO-2022-JP") {
		this->id = ISO_2022_JP;
	} else if (this->name == "Shift_JIS") {
		this->id = SHIFT_JIS;
	} else if (this->name == "EUC-JP") {
		this->id = EUC_JP;
	} else if (this->name == "UTF-8") {
		this->id = UTF_8;
	} else if (this->name == "UTF-16") {
		this->id = UTF_16;
	} else if (this->name == "UTF-32") {
		this->id = UTF_32;
	}
}

nkf_char NKFNativeEncoding::Iconv(nkf_char c2, nkf_char c1, nkf_char c0) {

	switch (this->id) {

	case : ASCII
	EIconv(nkf_char c2, nkf_char c1, nkf_char c0);
	break;
	case : ISO_2022_JP
	EIconv(nkf_char c2, nkf_char c1, nkf_char c0);
	break;
	case : SHIFT_JIS
	SIconv(nkf_char c2, nkf_char c1, nkf_char c0);
	break;
	case : EUC_JP
	EIconv(nkf_char c2, nkf_char c1, nkf_char c0);
	break;
	case : UTF_8
	WIconv(nkf_char c2, nkf_char c1, nkf_char c0);
	break;
	case : UTF_16
	WIconv16(nkf_char c2, nkf_char c1, nkf_char c0);
	break;
	case : UTF_32
	break;
	WIconv32(nkf_char c2, nkf_char c1, nkf_char c0);
	default:
	break;
	}
}

void NKFNativeEncoding::Oconv(nkf_char c2, nkf_char c1) {

	switch (this->id) {

	case	: ASCII
	EOconv(nkf_char c2, nkf_char c1);
	break;
	case : ISO_2022_JP
	JOconv(nkf_char c2, nkf_char c1);
	break;
	case : SHIFT_JIS
	SOconv(nkf_char c2, nkf_char c1);
	break;
	case : EUC_JP
	EOconv(nkf_char c2, nkf_char c1);
	break;
	case : UTF_8
	WOconv(nkf_char c2, nkf_char c1);
	break;
	case : UTF_16
	break;
	WOconv16(nkf_char c2, nkf_char c1);
	break;
	case : UTF_32
	WIconv32(nkf_char c2, nkf_char c1);
	break;
}
}

nkf_char NKFNativeEncoding::SIconv(nkf_char c2, nkf_char c1, nkf_char c0) {
}

nkf_char NKFNativeEncoding::EIconv(nkf_char c2, nkf_char c1, nkf_char c0) {
}

nkf_char NKFNativeEncoding::WIconv(nkf_char c2, nkf_char c1, nkf_char c0) {
}

nkf_char NKFNativeEncoding::WIconv16(nkf_char c2, nkf_char c1, nkf_char c0) {
}

nkf_char NKFNativeEncoding::WIconv32(nkf_char c2, nkf_char c1, nkf_char c0) {
}

void NKFNativeEncoding::JOconv(nkf_char c2, nkf_char c1) {
}

void NKFNativeEncoding::SOconv(nkf_char c2, nkf_char c1) {
}

void NKFNativeEncoding::EOconv(nkf_char c2, nkf_char c1) {
}

void NKFNativeEncoding::WOconv(nkf_char c2, nkf_char c1) {
}

void NKFNativeEncoding::WOconv16(nkf_char c2, nkf_char c1) {
}

void NKFNativeEncoding::WOconv32(nkf_char c2, nkf_char c1) {
}
