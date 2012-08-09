/*
 * wxnkfencoding.cpp
 *
 *  Created on: 2012/08/09
 * Contributor: Hiroyuki Nagata
 */

#include "wxnkfencoding.h"

nkf_char wxNKFEncoding::Iconv(nkf_char c2, nkf_char c1, nkf_char c0,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {
}

nkf_char wxNKFEncoding::Iconv(nkf_char c2, nkf_char c1, nkf_char c0,
		std::bitset<nkf_flag_num> nkfFlags, wxDataOutputStream* out) {
}

void wxNKFEncoding::Oconv(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {
}

void wxNKFEncoding::Oconv(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags, wxDataOutputStream* out) {
}

nkf_char wxNKFEncoding::SIconv(nkf_char c2, nkf_char c1, nkf_char c0,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {
}

nkf_char wxNKFEncoding::SIconv(nkf_char c2, nkf_char c1, nkf_char c0,
		std::bitset<nkf_flag_num> nkfFlags, wxDataOutputStream* out) {
}

nkf_char wxNKFEncoding::EIconv(nkf_char c2, nkf_char c1, nkf_char c0,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {
}

nkf_char wxNKFEncoding::EIconv(nkf_char c2, nkf_char c1, nkf_char c0,
		std::bitset<nkf_flag_num> nkfFlags, wxDataOutputStream* out) {
}

nkf_char wxNKFEncoding::WIconv(nkf_char c1, nkf_char c2, nkf_char c3,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {
}

nkf_char wxNKFEncoding::WIconv(nkf_char c1, nkf_char c2, nkf_char c3,
		std::bitset<nkf_flag_num> nkfFlags, wxDataOutputStream* out) {
}

nkf_char wxNKFEncoding::WIconv16(nkf_char c2, nkf_char c1, nkf_char c0,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {
}

nkf_char wxNKFEncoding::WIconv16(nkf_char c2, nkf_char c1, nkf_char c0,
		std::bitset<nkf_flag_num> nkfFlags, wxDataOutputStream* out) {
}

nkf_char wxNKFEncoding::WIconv32(nkf_char c2, nkf_char c1, nkf_char c0,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {
}

nkf_char wxNKFEncoding::WIconv32(nkf_char c2, nkf_char c1, nkf_char c0,
		std::bitset<nkf_flag_num> nkfFlags, wxDataOutputStream* out) {
}

void wxNKFEncoding::JOconv(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {
}

void wxNKFEncoding::JOconv(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags, wxDataOutputStream* out) {
}

void wxNKFEncoding::SOconv(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {
}

void wxNKFEncoding::SOconv(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags, wxDataOutputStream* out) {
}

void wxNKFEncoding::EOconv(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {
}

void wxNKFEncoding::EOconv(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags, wxDataOutputStream* out) {
}

void wxNKFEncoding::WOconv(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {
}

void wxNKFEncoding::WOconv(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags, wxDataOutputStream* out) {
}

void wxNKFEncoding::WOconv16(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {
}

void wxNKFEncoding::WOconv16(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags, wxDataOutputStream* out) {
}

void wxNKFEncoding::WOconv32(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {
}

void wxNKFEncoding::WOconv32(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags, wxDataOutputStream* out) {
}

void wxNKFEncoding::OutputAsciiEscapeSequence(int mode,
		std::wstring* oConvStr) {
}

void wxNKFEncoding::OutputAsciiEscapeSequence(int mode,
		wxDataOutputStream* out) {
}
