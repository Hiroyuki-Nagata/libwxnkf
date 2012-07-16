/*
 * Util.cpp
 *
 *  Created on: 2012/07/12
 * Contributor: Hiroyuki Nagata
 */

#include "Util.h"

/**
 * 文字コード名からNKFEncodingクラスを判別し、インスタンスを返す
 */
NKFEncoding* Util::NKFEncFind(const char* name) {
	int idx = -1;
	idx = NKFEncFindIndex(name);
	if (idx < 0)
		return 0;
	return NKFEncFromIndex(idx);
}

/**
 * IDに対応するNKFEncodingクラスを返却する
 */
NKFEncoding* Util::NKFEncFromIndex(int idx) {
	if (idx < 0 || NKF_ENCODING_TABLE_SIZE <= idx) {
		return 0;
	}
	return Util::NKFEncodingTable(idx);
}

NKFEncoding* Util::NKFDefaultEncoding() {
	NKFNativeEncoding* enc;
	enc->id = 0;

#ifdef DEFAULT_CODE_LOCALE
	enc = NKFLocaleEncoding();
#elif defined(DEFAULT_ENCIDX)
	enc = Util::NKFEncFromIndex(DEFAULT_ENCIDX);
#endif
	if (!enc)
		NKFNativeEncoding* enc;

	return enc;
}

/**
 * 文字コード名から文字コードに対応するIDを取得する
 */
int Util::NKFEncFindIndex(const char* name) {
	int i;
	if (name[0] == 'X' && *(name + 1) == '-')
		name += 2;
	for (i = 0; encoding_name_to_id_table[i].id >= 0; i++) {
		if (nkf_str_caseeql(encoding_name_to_id_table[i].name, name)) {
			return encoding_name_to_id_table[i].id;
		}
	}
	return -1;
}
/**
 * 文字コードを表すIDを元にNKFEncodingクラスを設定して返す
 */
NKFEncoding* Util::NKFEncodingTable(int idx) {

	// 返却用のクラスのインスタンス
	NKFEncoding* enc;
	enc->id = idx;

	/*
	 *  使用するbaseEncodingとidを設定する
	 */
	if (idx == ASCII || idx == ISO_8859_1 || idx == BINARY) {
		// NkfEncodingASCIIの場合
		enc->baseEncoding->id = ASCII;
		enc->baseEncoding->name = "ASCII";
		enc->baseEncoding->iconvName = "e_iconv";
		enc->baseEncoding->oconvName = "e_oconv";

		// idを設定する
		switch (idx) {
		case ASCII:
			enc->name = "US-ASCII";
			break;
		case ISO_8859_1:
			enc->name = "ISO-8859-1";
			break;
		case BINARY:
			enc->name = "BINARY";
		}

	} else if (idx == ISO_2022_JP || idx == CP50220 || idx == CP50221
			|| idx == CP50222 || idx == ISO_2022_JP_1 || idx == ISO_2022_JP_3
			|| idx == ISO_2022_JP_2004) {
		// NkfEncodingISO_2022_JPの場合
		enc->baseEncoding->id = ISO_2022_JP;
		enc->baseEncoding->name = "ISO-2022-JP";
		enc->baseEncoding->iconvName = "e_iconv";
		enc->baseEncoding->oconvName = "j_oconv";

		// idを設定する
		switch (idx) {
		case ISO_2022_JP:
			enc->name = "ISO-2022-JP";
			break;
		case CP50220:
			enc->name = "CP50220";
			break;
		case CP50221:
			enc->name = "CP50221";
			break;
		case CP50222:
			enc->name = "CP50222";
			break;
		case ISO_2022_JP_1:
			enc->name = "ISO-2022-JP-1";
			break;
		case ISO_2022_JP_3:
			enc->name = "ISO-2022-JP-3";
			break;
		case ISO_2022_JP_2004:
			enc->name = "ISO-2022-JP-2004";
			break;
		}

	} else if (idx == SHIFT_JIS || idx == WINDOWS_31J || idx == CP10001
			|| idx == SHIFT_JISX0213 || idx == SHIFT_JIS_2004) {
		// NkfEncodingShift_JISの場合
		enc->baseEncoding->id = SHIFT_JIS;
		enc->baseEncoding->name = "Shift_JIS";
		enc->baseEncoding->iconvName = "s_iconv";
		enc->baseEncoding->oconvName = "s_oconv";

		// idを設定する
		switch (idx) {
		case SHIFT_JIS:
			enc->name = "Shift_JIS";
			break;
		case WINDOWS_31J:
			enc->name = "Windows-31J";
			break;
		case CP10001:
			enc->name = "CP10001";
			break;
		case SHIFT_JISX0213:
			enc->name = "Shift_JISX0213";
			break;
		case SHIFT_JIS_2004:
			enc->name = "Shift_JIS-2004";
			break;
		}

	} else if (idx == EUC_JP || idx == EUCJP_NKF || idx == CP51932
			|| idx == EUCJP_MS || idx == EUCJP_ASCII || idx == EUC_JISX0213
			|| idx == EUC_JIS_2004) {
		// NkfEncodingEUC_JPの場合
		enc->baseEncoding->id = EUC_JP;
		enc->baseEncoding->name = "EUC-JP";
		enc->baseEncoding->iconvName = "e_iconv";
		enc->baseEncoding->oconvName = "e_oconv";

		// idを設定する
		switch (idx) {
		case EUC_JP:
			enc->name = "EUC-JP";
			break;
		case EUCJP_NKF:
			enc->name = "eucJP-nkf";
			break;
		case CP51932:
			enc->name = "CP51932";
			break;
		case EUCJP_MS:
			enc->name = "eucJP-MS";
			break;
		case EUCJP_ASCII:
			enc->name = "eucJP-ASCII";
			break;
		case EUC_JISX0213:
			enc->name = "EUC-JISX0213";
			break;
		case EUC_JIS_2004:
			enc->name = "EUC-JIS-2004";
			break;
		}

	} else if (idx == UTF_8 || idx == UTF_8N || idx == UTF_8_BOM
			|| idx == UTF8_MAC) {
		// NkfEncodingUTF_8の場合
		enc->baseEncoding->id = UTF_8;
		enc->baseEncoding->name = "UTF-8";
		enc->baseEncoding->iconvName = "w_iconv";
		enc->baseEncoding->oconvName = "w_oconv";

		// idを設定する
		switch (idx) {
		case UTF_8:
			enc->name = "UTF-8";
			break;
		case UTF_8N:
			enc->name = "UTF-8N";
			break;
		case UTF_8_BOM:
			enc->name = "UTF-8-BOM";
			break;
		case UTF8_MAC:
			enc->name = "UTF8-MAC";
			break;
		}

	} else if (idx == UTF_16 || idx == UTF_16BE || idx == UTF_16BE_BOM
			|| idx == UTF_16LE || idx == UTF_16LE_BOM) {
		// NkfEncodingUTF_16の場合
		enc->baseEncoding->id = UTF_16;
		enc->baseEncoding->name = "UTF-16";
		enc->baseEncoding->iconvName = "w_iconv16";
		enc->baseEncoding->oconvName = "w_oconv16";

		// idを設定する
		switch (idx) {
		case UTF_16:
			enc->name = "UTF-16";
			break;
		case UTF_16BE:
			enc->name = "UTF-16BE";
			break;
		case UTF_16BE_BOM:
			enc->name = "UTF-16BE-BOM";
			break;
		case UTF_16LE:
			enc->name = "UTF-16LE";
			break;
		case UTF_16LE_BOM:
			enc->name = "UTF-16LE-BOM";
			break;
		}

	} else if (idx == UTF_32 || idx == UTF_32BE || idx == UTF_32BE_BOM
			|| idx == UTF_32LE || idx == UTF_32LE_BOM) {
		// NkfEncodingUTF_32の場合
		enc->baseEncoding->id = UTF_32;
		enc->baseEncoding->name = "UTF-32";
		enc->baseEncoding->iconvName = "w_iconv32";
		enc->baseEncoding->oconvName = "w_oconv32";

		// idを設定する
		switch (idx) {
		case UTF_32:
			enc->name = "UTF-32";
			break;
		case UTF_32BE:
			enc->name = "UTF-32BE";
			break;
		case UTF_32BE_BOM:
			enc->name = "UTF-32BE-BOM";
			break;
		case UTF_32LE:
			enc->name = "UTF-32LE";
			break;
		case UTF_32LE_BOM:
			enc->name = "UTF-32LE-BOM";
			break;
		}
	} else {
		// idxが異常な値だった場合
		enc->id = -1;
		enc->name = NULL;
		enc->baseEncoding = NULL;
	}

	return enc;
}
/**
 * OSのロケールカらデフォルトのNKFEncodingを取得し返す
 */
NKFEncoding* Util::NKFLocaleEncoding() {
	NKFNativeEncoding* enc;
	enc->id = 0;
	const char *encname = Util::NKFLocaleCharmap();
	if (encname)
		// encnameに何らかの文字列が設定されていた場合
		enc = Util::NKFEncFind(encname);
	return enc;
}

nkf_char Util::E2sConv(nkf_char c2, nkf_char c1, nkf_char* p2, nkf_char* p1) {
	nkf_char ndx;
	if (is_eucg3(c2)) {
		ndx = c2 & 0x7f;
		if (FlagPool::x0213_f) {
			if ((0x21 <= ndx && ndx <= 0x2F)) {
				if (p2)
					*p2 = ((ndx - 1) >> 1) + 0xec - ndx / 8 * 3;
				if (p1)
					*p1 = c1 + ((ndx & 1) ? ((c1 < 0x60) ? 0x1f : 0x20) : 0x7e);
				return 0;
			} else if (0x6E <= ndx && ndx <= 0x7E) {
				if (p2)
					*p2 = ((ndx - 1) >> 1) + 0xbe;
				if (p1)
					*p1 = c1 + ((ndx & 1) ? ((c1 < 0x60) ? 0x1f : 0x20) : 0x7e);
				return 0;
			}
			return 1;
		}

		else if (nkf_isgraph(ndx)) {
			nkf_char val = 0;
			const unsigned short *ptr;
			ptr = UTF8Table::x0212_shiftjis[ndx - 0x21];
			if (ptr) {
				val = ptr[(c1 & 0x7f) - 0x21];
			}
			if (val) {
				c2 = val >> 8;
				c1 = val & 0xff;
				if (p2)
					*p2 = c2;
				if (p1)
					*p1 = c1;
				return 0;
			}
			c2 = Util::X0212Shift(c2);
		}
	}
	if (0x7F < c2)
		return 1;
	if (p2)
		*p2 = ((c2 - 1) >> 1) + ((c2 <= 0x5e) ? 0x71 : 0xb1);
	if (p1)
		*p1 = c1 + ((c2 & 1) ? ((c1 < 0x60) ? 0x1f : 0x20) : 0x7e);
	return 0;
}

nkf_char Util::S2eConv(nkf_char c2, nkf_char c1, nkf_char* p2, nkf_char* p1) {

	nkf_char val;
	static const char shift_jisx0213_s1a3_table[5][2] = { { 1, 8 }, { 3, 4 }, {
			5, 12 }, { 13, 14 }, { 15, 0 } };
	if (0xFC < c1)
		return 1;

	if (!FlagPool::cp932inv_f && is_ibmext_in_sjis(c2)) {
		val = UTF8Table::shiftjis_cp932[c2 - CP932_TABLE_BEGIN][c1 - 0x40];
		if (val) {
			c2 = val >> 8;
			c1 = val & 0xff;
		}
	}
	if (FlagPool::cp932inv_f
			&& CP932INV_TABLE_BEGIN <= c2&& c2 <= CP932INV_TABLE_END) {
		val = UTF8Table::cp932inv[c2 - CP932INV_TABLE_BEGIN][c1 - 0x40];
		if (val) {
			c2 = val >> 8;
			c1 = val & 0xff;
		}
	}
	if (!FlagPool::x0213_f && is_ibmext_in_sjis(c2)) {
		val = UTF8Table::shiftjis_x0212[c2 - 0xfa][c1 - 0x40];
		if (val) {
			if (val > 0x7FFF) {
				c2 = PREFIX_EUCG3 | ((val >> 8) & 0x7f);
				c1 = val & 0xff;
			} else {
				c2 = val >> 8;
				c1 = val & 0xff;
			}
			if (p2)
				*p2 = c2;
			if (p1)
				*p1 = c1;
			return 0;
		}
	}
	if (c2 >= 0x80) {
		if (FlagPool::x0213_f && c2 >= 0xF0) {
			if (c2 <= 0xF3 || (c2 == 0xF4 && c1 < 0x9F)) { /* k=1, 3<=k<=5, k=8, 12<=k<=15 */
				c2 = PREFIX_EUCG3 | 0x20
						| shift_jisx0213_s1a3_table[c2 - 0xF0][0x9E < c1];
			} else { /* 78<=k<=94 */
				c2 = PREFIX_EUCG3 | (c2 * 2 - 0x17B);
				if (0x9E < c1)
					c2++;
			}
		} else {
#define         SJ0162  0x00e1          /* 01 - 62 ku offset */
#define         SJ6394  0x0161          /* 63 - 94 ku offset */
			c2 = c2 + c2 - ((c2 <= 0x9F) ? SJ0162 : SJ6394);
			if (0x9E < c1)
				c2++;
		}
		if (c1 < 0x9F)
			c1 = c1 - ((c1 > DEL) ? SP : 0x1F);
		else {
			c1 = c1 - 0x7E;
		}
	}
	c2 = Util::X0212Unshift(c2);
	if (p2)
		*p2 = c2;
	if (p1)
		*p1 = c1;
	return 0;
}

nkf_char Util::X0212Shift(nkf_char c) {
	nkf_char ret = c;
	c &= 0x7f;
	if (is_eucg3(ret)) {
		if (0x75 <= c && c <= 0x7f) {
			ret = c + (0x109 - 0x75);
		}
	} else {
		if (0x75 <= c && c <= 0x7f) {
			ret = c + (0x113 - 0x75);
		}
	}
	return ret;
}

nkf_char Util::X0212Unshift(nkf_char c) {
	nkf_char ret = c;
	if (0x7f <= c && c <= 0x88) {
		ret = c + (0x75 - 0x7f);
	} else if (0x89 <= c && c <= 0x92) {
		ret = PREFIX_EUCG3 | 0x80 | (c + (0x75 - 0x89));
	}
	return ret;
}

nkf_char Util::W16eConv(nkf_char val, nkf_char* p2, nkf_char* p1) {
	nkf_char c1, c2, c3, c4;
	nkf_char ret = 0;
	val &= VALUE_MASK;
	if (val < 0x80) {
		*p2 = 0;
		*p1 = val;
	} else if (nkf_char_unicode_bmp_p(val)) {
		Util::NKFUnicodeToUTF8(val, &c1, &c2, &c3, &c4);
		ret = Util::UnicodeToJISCommon(c1, c2, c3, p2, p1);
		if (ret > 0) {
			*p2 = 0;
			*p1 = nkf_char_unicode_new(val);
			ret = 0;
		}
	} else {
		*p2 = 0;
		*p1 = nkf_char_unicode_new(val);
	}
	return ret;
}

void Util::NKFUnicodeToUTF8(nkf_char val, nkf_char* p1, nkf_char* p2,
		nkf_char* p3, nkf_char* p4) {
	val &= VALUE_MASK;
	if (val < 0x80) {
		*p1 = val;
		*p2 = 0;
		*p3 = 0;
		*p4 = 0;
	} else if (val < 0x800) {
		*p1 = 0xc0 | (val >> 6);
		*p2 = 0x80 | (val & 0x3f);
		*p3 = 0;
		*p4 = 0;
	} else if (nkf_char_unicode_bmp_p(val)) {
		*p1 = 0xe0 | (val >> 12);
		*p2 = 0x80 | ((val >> 6) & 0x3f);
		*p3 = 0x80 | (val & 0x3f);
		*p4 = 0;
	} else if (nkf_char_unicode_value_p(val)) {
		*p1 = 0xf0 | (val >> 18);
		*p2 = 0x80 | ((val >> 12) & 0x3f);
		*p3 = 0x80 | ((val >> 6) & 0x3f);
		*p4 = 0x80 | (val & 0x3f);
	} else {
		*p1 = 0;
		*p2 = 0;
		*p3 = 0;
		*p4 = 0;
	}
}

int Util::UnicodeToJISCommon(nkf_char c2, nkf_char c1, nkf_char c0,
		nkf_char* p2, nkf_char* p1) {
	const unsigned short * const *pp;
	const unsigned short * const * const *ppp;
	static const char no_best_fit_chars_table_C2[] = { 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 2, 1, 1, 2, 0, 0, 1, 1, 0, 1,
			0, 1, 2, 1, 1, 1, 1, 1, 1, 1 };
	static const char no_best_fit_chars_table_C2_ms[] = { 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0,
			1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0 };
	static const char no_best_fit_chars_table_932_C2[] = { 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0,
			1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0 };
	static const char no_best_fit_chars_table_932_C3[] = { 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1 };
	nkf_char ret = 0;

	if (c2 < 0x80) {
		*p2 = 0;
		*p1 = c2;
	} else if (c2 < 0xe0) {
		if (FlagPool::no_best_fit_chars_f) {
			if (FlagPool::ms_ucs_map_f == UCS_MAP_CP932) {
				switch (c2) {
				case 0xC2:
					if (no_best_fit_chars_table_932_C2[c1 & 0x3F])
						return 1;
					break;
				case 0xC3:
					if (no_best_fit_chars_table_932_C3[c1 & 0x3F])
						return 1;
					break;
				}
			} else if (!FlagPool::cp932inv_f) {
				switch (c2) {
				case 0xC2:
					if (no_best_fit_chars_table_C2[c1 & 0x3F])
						return 1;
					break;
				case 0xC3:
					if (no_best_fit_chars_table_932_C3[c1 & 0x3F])
						return 1;
					break;
				}
			} else if (FlagPool::ms_ucs_map_f == UCS_MAP_MS) {
				if (c2 == 0xC2 && no_best_fit_chars_table_C2_ms[c1 & 0x3F])
					return 1;
			} else if (FlagPool::ms_ucs_map_f == UCS_MAP_CP10001) {
				switch (c2) {
				case 0xC2:
					switch (c1) {
					case 0xA2:
					case 0xA3:
					case 0xA5:
					case 0xA6:
					case 0xAC:
					case 0xAF:
					case 0xB8:
						return 1;
					}
					break;
				}
			}
		}
		pp = FlagPool::ms_ucs_map_f == UCS_MAP_CP932 ?
				UTF8Table::utf8_to_euc_2bytes_932 :
				FlagPool::ms_ucs_map_f == UCS_MAP_MS ?
						UTF8Table::utf8_to_euc_2bytes_ms :
				FlagPool::ms_ucs_map_f == UCS_MAP_CP10001 ?
						UTF8Table::utf8_to_euc_2bytes_mac :
						UTF8Table::utf8_to_euc_2bytes;
		ret = Util::UnicodeToJISCommon2(c2, c1, pp, sizeof_utf8_to_euc_2bytes,
				p2, p1);

	} else if (c0 < 0xF0) {
		if (FlagPool::no_best_fit_chars_f) {
			if (FlagPool::ms_ucs_map_f == UCS_MAP_CP932) {
				if (c2 == 0xE3 && c1 == 0x82 && c0 == 0x94)
					return 1;
			} else if (FlagPool::ms_ucs_map_f == UCS_MAP_MS) {
				switch (c2) {
				case 0xE2:
					switch (c1) {
					case 0x80:
						if (c0 == 0x94 || c0 == 0x96 || c0 == 0xBE)
							return 1;
						break;
					case 0x88:
						if (c0 == 0x92)
							return 1;
						break;
					}
					break;
				case 0xE3:
					if (c1 == 0x80 || c0 == 0x9C)
						return 1;
					break;
				}
			} else if (FlagPool::ms_ucs_map_f == UCS_MAP_CP10001) {
				switch (c2) {
				case 0xE3:
					switch (c1) {
					case 0x82:
						if (c0 == 0x94)
							return 1;
						break;
					case 0x83:
						if (c0 == 0xBB)
							return 1;
						break;
					}
					break;
				}
			} else {
				switch (c2) {
				case 0xE2:
					switch (c1) {
					case 0x80:
						if (c0 == 0x95)
							return 1;
						break;
					case 0x88:
						if (c0 == 0xA5)
							return 1;
						break;
					}
					break;
				case 0xEF:
					switch (c1) {
					case 0xBC:
						if (c0 == 0x8D)
							return 1;
						break;
					case 0xBD:
						if (c0 == 0x9E && !FlagPool::cp932inv_f)
							return 1;
						break;
					case 0xBF:
						if (0xA0 <= c0 && c0 <= 0xA5)
							return 1;
						break;
					}
					break;
				}
			}
		}
		ppp = FlagPool::ms_ucs_map_f == UCS_MAP_CP932 ?
				UTF8Table::utf8_to_euc_3bytes_932 :
				FlagPool::ms_ucs_map_f == UCS_MAP_MS ?
						UTF8Table::utf8_to_euc_3bytes_ms :
				FlagPool::ms_ucs_map_f == UCS_MAP_CP10001 ?
						UTF8Table::utf8_to_euc_3bytes_mac :
						UTF8Table::utf8_to_euc_3bytes;
		ret = Util::UnicodeToJISCommon2(c1, c0, ppp[c2 - 0xE0],
				sizeof_utf8_to_euc_C2, p2, p1);
	} else
		return -1;
	if (!ret && !FlagPool::cp932inv_f && is_eucg3(*p2)) {
		nkf_char s2, s1;
		if (Util::E2sConv(*p2, *p1, &s2, &s1) == 0) {
			Util::S2eConv(s2, s1, p2, p1);
		} else {
			ret = 1;
		}
	}
	return ret;
}

int Util::UnicodeToJISCommon2(nkf_char c1, nkf_char c0,
		const unsigned short * const * pp, nkf_char psize, nkf_char* p2,
		nkf_char* p1) {

	nkf_char c2;
	const unsigned short *p;
	unsigned short val;

	if (pp == 0)
		return 1;

	c1 -= 0x80;
	if (c1 < 0 || psize <= c1)
		return 1;
	p = pp[c1];
	if (p == 0)
		return 1;

	c0 -= 0x80;
	if (c0 < 0 || sizeof_utf8_to_euc_C2 <= c0)
		return 1;
	val = p[c0];
	if (val == 0)
		return 1;
	if (FlagPool::no_cp932ext_f && ((val >> 8) == 0x2D || /* NEC special characters */
	val > NKF_INT32_C(0xF300) /* IBM extended characters */
	))
		return 1;

	c2 = val >> 8;
	if (val > 0x7FFF) {
		c2 &= 0x7f;
		c2 |= PREFIX_EUCG3;
	}
	if (c2 == SO)
		c2 = JIS_X_0201_1976_K;
	c1 = val & 0xFF;
	if (p2)
		*p2 = c2;
	if (p1)
		*p1 = c1;
	return 0;
}

