/*
 * NKFNativeEncodings.cpp
 *
 *  Created on: 2012/07/11
 * Contributor: Hiroyuki Nagata
 */

#include "NKFNativeEncodings.h"
#include "LibNKF.h"

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

nkf_char NKFNativeEncoding::Iconv(nkf_char c2, nkf_char c1, nkf_char c0,
		FlagPool* flagPool) {

	switch (this->id) {

	case ASCII:
		return EIconv(c2, c1, c0, flagPool);
		break;
	case ISO_2022_JP:
		return EIconv(c2, c1, c0, flagPool);
		break;
	case SHIFT_JIS:
		return SIconv(c2, c1, c0, flagPool);
		break;
	case EUC_JP:
		return EIconv(c2, c1, c0, flagPool);
		break;
	case UTF_8:
		return WIconv(c2, c1, c0, flagPool);
		break;
	case UTF_16:
		return WIconv16(c2, c1, c0, flagPool);
		break;
	case UTF_32:
		return WIconv32(c2, c1, c0, flagPool);
		break;
	default:
		return -1;
	}
}

void NKFNativeEncoding::Oconv(nkf_char c2, nkf_char c1, FlagPool* flagPool) {

	switch (this->id) {

	case ASCII:
		EOconv(c2, c1, flagPool);
		break;
	case ISO_2022_JP:
		JOconv(c2, c1, flagPool);
		break;
	case SHIFT_JIS:
		SOconv(c2, c1, flagPool);
		break;
	case EUC_JP:
		EOconv(c2, c1, flagPool);
		break;
	case UTF_8:
		WOconv(c2, c1, flagPool);
		break;
	case UTF_16:
		break;
		WOconv16(c2, c1, flagPool);
		break;
	case UTF_32:
		WOconv32(c2, c1, flagPool);
		break;
	}
}

nkf_char NKFNativeEncoding::SIconv(nkf_char c2, nkf_char c1, nkf_char c0,
		FlagPool* flagPool) {
	if (c2 == JIS_X_0201_1976_K || (0xA1 <= c2 && c2 <= 0xDF)) {
		if (flagPool->iso2022jp_f && !flagPool->x0201_f) {
			c2 = GETA1;
			c1 = GETA2;
		} else {
			c1 &= 0x7f;
		}
	} else if ((c2 == EOF) || (c2 == 0) || c2 < SP) {
		/* NOP */
	} else if (!flagPool->x0213_f && 0xF0 <= c2 && c2 <= 0xF9 && 0x40 <= c1
			&& c1 <= 0xFC) {
		/* CP932 UDC */
		if (c1 == 0x7F)
			return 0;
		c1 =
				nkf_char_unicode_new((c2 - 0xF0) * 188 + (c1 - 0x40 - (0x7E < c1)) + 0xE000);
		c2 = 0;
	} else {
		nkf_char ret = Util::S2eConv(c2, c1, &c2, &c1, flagPool);
		if (ret)
			return ret;
	}
	LibNKF::outputEncoding->baseEncoding->Oconv(c2, c1, flagPool);
	return 0;
}

nkf_char NKFNativeEncoding::EIconv(nkf_char c2, nkf_char c1, nkf_char c0,
		FlagPool* flagPool) {

	if (c2 == JIS_X_0201_1976_K || c2 == SS2) {
		if (flagPool->iso2022jp_f && !flagPool->x0201_f) {
			c2 = GETA1;
			c1 = GETA2;
		} else {
			c2 = JIS_X_0201_1976_K;
			c1 &= 0x7f;
		}
	} else if (c2 == 0x8f) {
		if (c0 == 0) {
			return -1;
		}
		if (!flagPool->cp51932_f && !flagPool->x0213_f && 0xF5 <= c1
				&& c1 <= 0xFE && 0xA1 <= c0 && c0 <= 0xFE) {
			/* encoding is eucJP-ms, so invert to Unicode Private User Area */
			c1 = nkf_char_unicode_new((c1 - 0xF5) * 94 + c0 - 0xA1 + 0xE3AC);
			c2 = 0;
		} else {
			c2 = (c2 << 8) | (c1 & 0x7f);
			c1 = c0 & 0x7f;
			if (flagPool->cp51932_f) {
				nkf_char s2, s1;
				if (Util::E2sConv(c2, c1, &s2, &s1, flagPool) == 0) {
					Util::S2eConv(s2, s1, &c2, &c1, flagPool);
					if (c2 < 0x100) {
						c1 &= 0x7f;
						c2 &= 0x7f;
					}
				}
			}
		}
	} else if ((c2 == EOF) || (c2 == 0) || c2 < SP || c2 == ISO_8859_1) {
		/* NOP */
	} else {
		if (!flagPool->cp51932_f && flagPool->ms_ucs_map_f && 0xF5 <= c2
				&& c2 <= 0xFE && 0xA1 <= c1 && c1 <= 0xFE) {
			/* encoding is eucJP-ms, so invert to Unicode Private User Area */
			c1 = nkf_char_unicode_new((c2 - 0xF5) * 94 + c1 - 0xA1 + 0xE000);
			c2 = 0;
		} else {
			c1 &= 0x7f;
			c2 &= 0x7f;
			if (flagPool->cp51932_f && 0x79 <= c2 && c2 <= 0x7c) {
				nkf_char s2, s1;
				if (Util::E2sConv(c2, c1, &s2, &s1, flagPool) == 0) {
					Util::S2eConv(s2, s1, &c2, &c1, flagPool);
					if (c2 < 0x100) {
						c1 &= 0x7f;
						c2 &= 0x7f;
					}
				}
			}
		}
	}
	LibNKF::outputEncoding->baseEncoding->Oconv(c2, c1, flagPool);
	return 0;
}

nkf_char NKFNativeEncoding::WIconv(nkf_char c1, nkf_char c2, nkf_char c3,
		FlagPool* flagPool) {

	nkf_char ret = 0, c4 = 0;
	static const char w_iconv_utf8_1st_byte[] = { /* 0xC0 - 0xFF */
	20, 20, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
			21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 30, 31, 31, 31,
			31, 31, 31, 31, 31, 31, 31, 31, 31, 32, 33, 33, 40, 41, 41, 41, 42,
			43, 43, 43, 50, 50, 50, 50, 60, 60, 70, 70 };

	if (c3 > 0xFF) {
		c4 = c3 & 0xFF;
		c3 >>= 8;
	}

	if (c1 < 0 || 0xff < c1) {
	} else if (c1 == 0) { /* 0 : 1 byte*/
		c3 = 0;
	} else if ((c1 & 0xC0) == 0x80) { /* 0x80-0xbf : trail byte */
		return 0;
	} else {
		switch (w_iconv_utf8_1st_byte[c1 - 0xC0]) {
		case 21:
			if (c2 < 0x80 || 0xBF < c2)
				return 0;
			break;
		case 30:
			if (c3 == 0)
				return -1;
			if (c2 < 0xA0 || 0xBF < c2 || (c3 & 0xC0) != 0x80)
				return 0;
			break;
		case 31:
		case 33:
			if (c3 == 0)
				return -1;
			if ((c2 & 0xC0) != 0x80 || (c3 & 0xC0) != 0x80)
				return 0;
			break;
		case 32:
			if (c3 == 0)
				return -1;
			if (c2 < 0x80 || 0x9F < c2 || (c3 & 0xC0) != 0x80)
				return 0;
			break;
		case 40:
			if (c3 == 0)
				return -2;
			if (c2 < 0x90 || 0xBF < c2 || (c3 & 0xC0) != 0x80
					|| (c4 & 0xC0) != 0x80)
				return 0;
			break;
		case 41:
			if (c3 == 0)
				return -2;
			if (c2 < 0x80 || 0xBF < c2 || (c3 & 0xC0) != 0x80
					|| (c4 & 0xC0) != 0x80)
				return 0;
			break;
		case 42:
			if (c3 == 0)
				return -2;
			if (c2 < 0x80 || 0x8F < c2 || (c3 & 0xC0) != 0x80
					|| (c4 & 0xC0) != 0x80)
				return 0;
			break;
		default:
			return 0;
			break;
		}
	}
	if (c1 == 0 || c1 == EOF) {
	} else if ((c1 & 0xf8) == 0xf0) { /* 4 bytes */
		c2 = nkf_char_unicode_new(Util::NKFUTF8ToUnicode(c1, c2, c3, c4));
		c1 = 0;
	} else {
		ret = Util::W2eConv(c1, c2, c3, &c1, &c2, flagPool);
	}
	if (ret == 0) {
		LibNKF::outputEncoding->baseEncoding->Oconv(c1, c2, flagPool);
	}
	return ret;
}

nkf_char NKFNativeEncoding::WIconv16(nkf_char c2, nkf_char c1, nkf_char c0,
		FlagPool* flagPool) {

	LibNKF::outputEncoding->baseEncoding->Oconv(c2, c1, flagPool);
	return 16; /* different from w_iconv32 */
}

nkf_char NKFNativeEncoding::WIconv32(nkf_char c2, nkf_char c1, nkf_char c0,
		FlagPool* flagPool) {

	LibNKF::outputEncoding->baseEncoding->Oconv(c2, c1, flagPool);
	return 32; /* different from w_iconv16 */
}

void NKFNativeEncoding::JOconv(nkf_char c2, nkf_char c1, FlagPool* flagPool) {

	if (c2 == 0 && nkf_char_unicode_p(c1)) {
		Util::W16eConv(c1, &c2, &c1, flagPool);
		if (c2 == 0 && nkf_char_unicode_p(c1)) {
			c2 = c1 & VALUE_MASK;
			if (flagPool->ms_ucs_map_f && 0xE000 <= c2 && c2 <= 0xE757) {
				/* CP5022x UDC */
				c1 &= 0xFFF;
				c2 = 0x7F + c1 / 94;
				c1 = 0x21 + c1 % 94;
			} else {
				//if (encode_fallback)
				//	(*encode_fallback)(c1);
				//return;
			}
		}
	}
	if (c2 == 0) {
		LibNKF::OutputAsciiEscapeSequence(ASCII);
		LibNKF::OPutC(c1);
	} else if (c2 == EOF) {
		LibNKF::OutputAsciiEscapeSequence(ASCII);
		LibNKF::OPutC(EOF);
	} else if (c2 == ISO_8859_1) {
		LibNKF::OutputAsciiEscapeSequence(ISO_8859_1);
		LibNKF::OPutC(c1 | 0x80);
	} else if (c2 == JIS_X_0201_1976_K) {
		LibNKF::OutputAsciiEscapeSequence(JIS_X_0201_1976_K);
		LibNKF::OPutC(c1);
	} else if (is_eucg3(c2)) {
		LibNKF::OutputAsciiEscapeSequence(
				flagPool->x0213_f ? JIS_X_0213_2 : JIS_X_0212);
		LibNKF::OPutC(c2 & 0x7f);
		LibNKF::OPutC(c1);
	} else {
		if (flagPool->ms_ucs_map_f ?
				c2 < 0x20 || 0x92 < c2 || c1 < 0x20 || 0x7e < c1 :
				c2 < 0x20 || 0x7e < c2 || c1 < 0x20 || 0x7e < c1)
			return;
		LibNKF::OutputAsciiEscapeSequence(
				flagPool->x0213_f ? JIS_X_0213_1 : JIS_X_0208);
		LibNKF::OPutC(c2);
		LibNKF::OPutC(c1);
	}
}

void NKFNativeEncoding::SOconv(nkf_char c2, nkf_char c1, FlagPool* flagPool) {

	if (c2 == 0 && nkf_char_unicode_p(c1)) {
		Util::W16eConv(c1, &c2, &c1, flagPool);
		if (c2 == 0 && nkf_char_unicode_p(c1)) {
			c2 = c1 & VALUE_MASK;
			if (!flagPool->x0213_f && 0xE000 <= c2 && c2 <= 0xE757) {
				/* CP932 UDC */
				c1 &= 0xFFF;
				c2 = c1 / 188 + (flagPool->cp932inv_f ? 0xF0 : 0xEB);
				c1 = c1 % 188;
				c1 += 0x40 + (c1 > 0x3e);
				LibNKF::OPutC(c2);
				LibNKF::OPutC(c1);
				return;
			} else {
				//if (encode_fallback)
				//	(*encode_fallback)(c1);
				//return;
			}
		}
	}
	if (c2 == EOF) {
		LibNKF::OPutC(EOF);
		return;
	} else if (c2 == 0) {
		LibNKF::outputMode = ASCII;
		LibNKF::OPutC(c1);
	} else if (c2 == JIS_X_0201_1976_K) {
		LibNKF::outputMode = SHIFT_JIS;
		LibNKF::OPutC(c1 | 0x80);
	} else if (c2 == ISO_8859_1) {
		LibNKF::outputMode = ISO_8859_1;
		LibNKF::OPutC(c1 | 0x080);
	} else if (is_eucg3(c2)) {
		LibNKF::outputMode = SHIFT_JIS;
		if (Util::E2sConv(c2, c1, &c2, &c1, flagPool) == 0) {
			LibNKF::OPutC(c2);
			LibNKF::OPutC(c1);
		}
	} else {
		if (!nkf_isprint(c1) || !nkf_isprint(c2)) {
			// fix me !
			//LibNKF::SetIconv(FALSE, 0, flagPool);
			return; /* too late to rescue this char */
		}
		LibNKF::outputMode = SHIFT_JIS;
		Util::E2sConv(c2, c1, &c2, &c1, flagPool);

		if (flagPool->cp932inv_f
				&& CP932INV_TABLE_BEGIN <= c2&& c2 <= CP932INV_TABLE_END) {
			nkf_char c = UTF8Table::cp932inv[c2 - CP932INV_TABLE_BEGIN][c1
					- 0x40];
			if (c) {
				c2 = c >> 8;
				c1 = c & 0xff;
			}
		}

		LibNKF::OPutC(c2);
		if (LibNKF::prefix_table[(unsigned char) c1]) {
			LibNKF::OPutC(LibNKF::prefix_table[(unsigned char) c1]);
		}
		LibNKF::OPutC(c1);
	}
}

void NKFNativeEncoding::EOconv(nkf_char c2, nkf_char c1, FlagPool* flagPool) {

	if (c2 == 0 && nkf_char_unicode_p(c1)) {
		Util::W16eConv(c1, &c2, &c1, flagPool);
		if (c2 == 0 && nkf_char_unicode_p(c1)) {
			c2 = c1 & VALUE_MASK;
			if (flagPool->x0212_f && 0xE000 <= c2 && c2 <= 0xE757) {
				/* eucJP-ms UDC */
				c1 &= 0xFFF;
				c2 = c1 / 94;
				c2 += c2 < 10 ? 0x75 : 0x8FEB;
				c1 = 0x21 + c1 % 94;
				if (is_eucg3(c2)) {
					LibNKF::OPutC(0x8f);
					LibNKF::OPutC((c2 & 0x7f) | 0x080);
					LibNKF::OPutC(c1 | 0x080);
				} else {
					LibNKF::OPutC((c2 & 0x7f) | 0x080);
					LibNKF::OPutC(c1 | 0x080);
				}
				return;
			} else {
				//if (encode_fallback)
				//	(*encode_fallback)(c1);
				//return;
			}
		}
	}

	if (c2 == EOF) {
		LibNKF::OPutC(EOF);
	} else if (c2 == 0) {
		LibNKF::outputMode = ASCII;
		LibNKF::OPutC(c1);
	} else if (c2 == JIS_X_0201_1976_K) {
		LibNKF::outputMode = EUC_JP;
		LibNKF::OPutC(SS2);
		LibNKF::OPutC(c1 | 0x80);
	} else if (c2 == ISO_8859_1) {
		LibNKF::outputMode = ISO_8859_1;
		LibNKF::OPutC(c1 | 0x080);
	} else if (is_eucg3(c2)) {
		LibNKF::outputMode = EUC_JP;
		if (!flagPool->cp932inv_f) {
			nkf_char s2, s1;
			if (Util::E2sConv(c2, c1, &s2, &s1, flagPool) == 0) {
				Util::S2eConv(s2, s1, &c2, &c1, flagPool);
			}
		}
		if (c2 == 0) {
			LibNKF::outputMode = ASCII;
			LibNKF::OPutC(c1);
		} else if (is_eucg3(c2)) {
			if (flagPool->x0212_f) {
				LibNKF::OPutC(0x8f);
				LibNKF::OPutC((c2 & 0x7f) | 0x080);
				LibNKF::OPutC(c1 | 0x080);
			}
		} else {
			LibNKF::OPutC((c2 & 0x7f) | 0x080);
			LibNKF::OPutC(c1 | 0x080);
		}
	} else {
		if (!nkf_isgraph(c1) || !nkf_isgraph(c2)) {
			// fix me !
			//LibNKF::SetIconv(FALSE, 0, flagPool);
			return; /* too late to rescue this char */
		}
		LibNKF::outputMode = EUC_JP;
		LibNKF::OPutC(c2 | 0x080);
		LibNKF::OPutC(c1 | 0x080);
	}
}

void NKFNativeEncoding::WOconv(nkf_char c2, nkf_char c1, FlagPool* flagPool) {

	nkf_char c3, c4;
	nkf_char val;

	if (flagPool->output_bom_f) {
		flagPool->output_bom_f = FALSE;
		LibNKF::OPutC('\357');
		LibNKF::OPutC('\273');
		LibNKF::OPutC('\277');
	}

	if (c2 == EOF) {
		LibNKF::OPutC(EOF);
		return;
	}

	if (c2 == 0 && nkf_char_unicode_p(c1)) {
		val = c1 & VALUE_MASK;
		Util::NKFUnicodeToUTF8(val, &c1, &c2, &c3, &c4);
		LibNKF::OPutC(c1);
		if (c2)
			LibNKF::OPutC(c2);
		if (c3)
			LibNKF::OPutC(c3);
		if (c4)
			LibNKF::OPutC(c4);
		return;
	}

	if (c2 == 0) {
		LibNKF::OPutC(c1);
	} else {
		val = Util::E2wConv(c2, c1, flagPool);
		if (val) {
			Util::NKFUnicodeToUTF8(val, &c1, &c2, &c3, &c4);
			LibNKF::OPutC(c1);
			if (c2)
				LibNKF::OPutC(c2);
			if (c3)
				LibNKF::OPutC(c3);
			if (c4)
				LibNKF::OPutC(c4);
		}
	}
}

void NKFNativeEncoding::WOconv16(nkf_char c2, nkf_char c1, FlagPool* flagPool) {

	if (flagPool->output_bom_f) {
		flagPool->output_bom_f = FALSE;
		if (LibNKF::outputEndian == ENDIAN_LITTLE) {
			LibNKF::OPutC(0xFF);
			LibNKF::OPutC(0xFE);
		} else {
			LibNKF::OPutC(0xFE);
			LibNKF::OPutC(0xFF);
		}
	}

	if (c2 == EOF) {
		LibNKF::OPutC(EOF);
		return;
	}

	if (c2 == 0 && nkf_char_unicode_p(c1)) {
		if (nkf_char_unicode_bmp_p(c1)) {
			c2 = (c1 >> 8) & 0xff;
			c1 &= 0xff;
		} else {
			c1 &= VALUE_MASK;
			if (c1 <= UNICODE_MAX) {
				c2 = (c1 >> 10) + NKF_INT32_C(0xD7C0); /* high surrogate */
				c1 = (c1 & 0x3FF) + NKF_INT32_C(0xDC00); /* low surrogate */
				if (LibNKF::outputEndian == ENDIAN_LITTLE) {
					LibNKF::OPutC(c2 & 0xff);
					LibNKF::OPutC((c2 >> 8) & 0xff);
					LibNKF::OPutC(c1 & 0xff);
					LibNKF::OPutC((c1 >> 8) & 0xff);
				} else {
					LibNKF::OPutC((c2 >> 8) & 0xff);
					LibNKF::OPutC(c2 & 0xff);
					LibNKF::OPutC((c1 >> 8) & 0xff);
					LibNKF::OPutC(c1 & 0xff);
				}
			}
			return;
		}
	} else if (c2) {
		nkf_char val = Util::E2wConv(c2, c1, flagPool);
		c2 = (val >> 8) & 0xff;
		c1 = val & 0xff;
		if (!val)
			return;
	}

	if (LibNKF::outputEndian == ENDIAN_LITTLE) {
		LibNKF::OPutC(c1);
		LibNKF::OPutC(c2);
	} else {
		LibNKF::OPutC(c2);
		LibNKF::OPutC(c1);
	}
}

void NKFNativeEncoding::WOconv32(nkf_char c2, nkf_char c1, FlagPool* flagPool) {

	if (flagPool->output_bom_f) {
		flagPool->output_bom_f = FALSE;
		if (LibNKF::outputEndian == ENDIAN_LITTLE) {
			LibNKF::OPutC(0xFF);
			LibNKF::OPutC(0xFE);
			LibNKF::OPutC(0);
			LibNKF::OPutC(0);
		} else {
			LibNKF::OPutC(0);
			LibNKF::OPutC(0);
			LibNKF::OPutC(0xFE);
			LibNKF::OPutC(0xFF);
		}
	}

	if (c2 == EOF) {
		LibNKF::OPutC(EOF);
		return;
	}

	if (c2 == ISO_8859_1) {
		c1 |= 0x80;
	} else if (c2 == 0 && nkf_char_unicode_p(c1)) {
		c1 &= VALUE_MASK;
	} else if (c2) {
		c1 = Util::E2wConv(c2, c1, flagPool);
		if (!c1)
			return;
	}
	if (LibNKF::outputEndian == ENDIAN_LITTLE) {
		LibNKF::OPutC(c1 & 0xFF);
		LibNKF::OPutC((c1 >> 8) & 0xFF);
		LibNKF::OPutC((c1 >> 16) & 0xFF);
		LibNKF::OPutC(0);
	} else {
		LibNKF::OPutC(0);
		LibNKF::OPutC((c1 >> 16) & 0xFF);
		LibNKF::OPutC((c1 >> 8) & 0xFF);
		LibNKF::OPutC(c1 & 0xFF);
	}
}
