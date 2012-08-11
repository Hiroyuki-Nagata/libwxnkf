/*
 * util.cpp
 *
 * nkf common method
 *  Created on: 2012/07/12
 * Contributor: Hiroyuki Nagata
 */

#include "util.h"

/**
 * judge baseID and return wxNKFEncoding instance
 * 0: set input
 * 1: set output
 */
void Util::NKFEncFind(const char* name, wxNKFEncoding* enc, int io) {
	// setting unique encode index(int)
	int idx = -1;
	// encode name to encode index
	idx = NKFEncFindIndex(name);
	// set data to encode class instance
	NKFEncFromIndex(idx, enc, io);
}
/**
 * return wxNKFEncoding class per encoding ID
 */
void Util::NKFEncFromIndex(int idx, wxNKFEncoding* enc, int io) {

	/**
	 * base name & base id
	 */
	int baseId;
	wxString baseName;
	/**
	 * unique name & unique id
	 */
	int uniqueId = idx;
	wxString uniqueName;

	/*
	 *  set unique encode id and base encoding
	 */
	if (idx == ASCII || idx == ISO_8859_1 || idx == BINARY) {
		// NkfEncodingASCII
		baseId = ASCII;
		baseName = wxT("ASCII");

		// idを設定する
		switch (idx) {
		case ASCII:
			uniqueName = wxT("US-ASCII");
			break;
		case ISO_8859_1:
			uniqueName = wxT("ISO-8859-1");
			break;
		case BINARY:
			uniqueName = wxT("BINARY");
			break;
		}

	} else if (idx == ISO_2022_JP || idx == CP50220 || idx == CP50221
			|| idx == CP50222 || idx == ISO_2022_JP_1 || idx == ISO_2022_JP_3
			|| idx == ISO_2022_JP_2004) {
		// NkfEncodingISO_2022_JPの場合
		baseId = ISO_2022_JP;
		baseName = wxT("ISO-2022-JP");

		// idを設定する
		switch (idx) {
		case ISO_2022_JP:
			uniqueName = wxT("ISO-2022-JP");
			break;
		case CP50220:
			uniqueName = wxT("CP50220");
			break;
		case CP50221:
			uniqueName = wxT("CP50221");
			break;
		case CP50222:
			uniqueName = wxT("CP50222");
			break;
		case ISO_2022_JP_1:
			uniqueName = wxT("ISO-2022-JP-1");
			break;
		case ISO_2022_JP_3:
			uniqueName = wxT("ISO-2022-JP-3");
			break;
		case ISO_2022_JP_2004:
			uniqueName = wxT("ISO-2022-JP-2004");
			break;
		}

	} else if (idx == SHIFT_JIS || idx == WINDOWS_31J || idx == CP10001
			|| idx == SHIFT_JISX0213 || idx == SHIFT_JIS_2004) {
		// NkfEncodingShift_JISの場合
		baseId = SHIFT_JIS;
		baseName = wxT("Shift_JIS");

		// idを設定する
		switch (idx) {
		case SHIFT_JIS:
			uniqueName = wxT("Shift_JIS");
			break;
		case WINDOWS_31J:
			uniqueName = wxT("Windows-31J");
			break;
		case CP10001:
			uniqueName = wxT("CP10001");
			break;
		case SHIFT_JISX0213:
			uniqueName = wxT("Shift_JISX0213");
			break;
		case SHIFT_JIS_2004:
			uniqueName = wxT("Shift_JIS-2004");
			break;
		}

	} else if (idx == EUC_JP || idx == EUCJP_NKF || idx == CP51932
			|| idx == EUCJP_MS || idx == EUCJP_ASCII || idx == EUC_JISX0213
			|| idx == EUC_JIS_2004) {
		// NkfEncodingEUC_JPの場合
		baseId = EUC_JP;
		baseName = wxT("EUC-JP");

		// idを設定する
		switch (idx) {
		case EUC_JP:
			uniqueName = wxT("EUC-JP");
			break;
		case EUCJP_NKF:
			uniqueName = wxT("eucJP-nkf");
			break;
		case CP51932:
			uniqueName = wxT("CP51932");
			break;
		case EUCJP_MS:
			uniqueName = wxT("eucJP-MS");
			break;
		case EUCJP_ASCII:
			uniqueName = wxT("eucJP-ASCII");
			break;
		case EUC_JISX0213:
			uniqueName = wxT("EUC-JISX0213");
			break;
		case EUC_JIS_2004:
			uniqueName = wxT("EUC-JIS-2004");
			break;
		}

	} else if (idx == UTF_8 || idx == UTF_8N || idx == UTF_8_BOM
			|| idx == UTF8_MAC) {
		// NkfEncodingUTF_8の場合
		baseId = UTF_8;
		baseName = wxT("UTF-8");

		// idを設定する
		switch (idx) {
		case UTF_8:
			uniqueName = wxT("UTF-8");
			break;
		case UTF_8N:
			uniqueName = wxT("UTF-8N");
			break;
		case UTF_8_BOM:
			uniqueName = wxT("UTF-8-BOM");
			break;
		case UTF8_MAC:
			uniqueName = wxT("UTF8-MAC");
			break;
		}

	} else if (idx == UTF_16 || idx == UTF_16BE || idx == UTF_16BE_BOM
			|| idx == UTF_16LE || idx == UTF_16LE_BOM) {
		// NkfEncodingUTF_16の場合
		baseId = UTF_16;
		baseName = wxT("UTF-16");

		// idを設定する
		switch (idx) {
		case UTF_16:
			uniqueName = wxT("UTF-16");
			break;
		case UTF_16BE:
			uniqueName = wxT("UTF-16BE");
			break;
		case UTF_16BE_BOM:
			uniqueName = wxT("UTF-16BE-BOM");
			break;
		case UTF_16LE:
			uniqueName = wxT("UTF-16LE");
			break;
		case UTF_16LE_BOM:
			uniqueName = wxT("UTF-16LE-BOM");
			break;
		}

	} else if (idx == UTF_32 || idx == UTF_32BE || idx == UTF_32BE_BOM
			|| idx == UTF_32LE || idx == UTF_32LE_BOM) {
		// NkfEncodingUTF_32の場合
		baseId = UTF_32;
		baseName = wxT("UTF-32");

		// idを設定する
		switch (idx) {
		case UTF_32:
			uniqueName = wxT("UTF-32");
			break;
		case UTF_32BE:
			uniqueName = wxT("UTF-32BE");
			break;
		case UTF_32BE_BOM:
			uniqueName = wxT("UTF-32BE-BOM");
			break;
		case UTF_32LE:
			uniqueName = wxT("UTF-32LE");
			break;
		case UTF_32LE_BOM:
			uniqueName = wxT("UTF-32LE-BOM");
			break;
		}
	} else {
		// idxが異常な値だった場合
		baseId = -1;
		baseName = wxEmptyString;
		enc = 0;
	}

	// set data to encode instance
	if (SET_INPUT_MODE==io) {
		enc->inputMode = baseId;
		enc->inputBaseName = baseName;
		enc->iCharID = idx;
		enc->iCharName = uniqueName;
	}

	if (SET_OUTPUT_MODE==io) {
		enc->outputMode = baseId;
		enc->outputBaseName = baseName;
		enc->oCharID = idx;
		enc->iCharName = uniqueName;
	}
}
/**
 * get encode ID by name
 */
int Util::NKFEncFindIndex(const char *name) {

	if (name[0] == 'X' && *(name + 1) == '-')
		name += 2;

	// NULLであれば問答無用でエラー
	if (name == 0)
		return -1;

	int id = 0;
	/**
	 * 文字コード名の最初の1文字をリテラルとして取り出してidを判定する
	 */
	switch (name[0]) {

	case '6':
		id = ASCII;
		break;

	case 'A':
		id = ASCII;
		break;

	case 'B':
		id = BINARY;
		break;

	case 'C': // 最初の文字が「C」の場合
		switch (name[1]) {

		case 'P': // 「CP**」の場合
			if (StrncmpFromHead("CP50220", name)) {
				id = CP50220;
			} else if (StrncmpFromHead("CP50221", name)) {
				id = CP50221;
			} else if (StrncmpFromHead("CP50222", name)) {
				id = CP50222;
			} else if (StrncmpFromHead("CP932", name)) {
				id = WINDOWS_31J;
			} else if (StrncmpFromHead("CP10001", name)) {
				id = CP10001;
			} else if (StrncmpFromHead("CP51932", name)) {
				id = CP51932;
			}
			break;

		case 'S': // 「CS**」の場合
			if (StrncmpFromHead("CSISO2022JP", name)) {
				id = CP50221;
			} else if (StrncmpFromHead("CSWINDOWS31J", name)) {
				id = WINDOWS_31J;
			}
			break;
		}
		break;

	case 'E': // 最初の文字が「E」の場合, EUC**
		switch (name[3]) {

		case 'J': // 「EUCJ**」の場合
			if (StrncmpFromHead("EUCJP", name)) {
				id = EUC_JP;
			} else if (StrncmpFromHead("EUCJP-NKF", name)) {
				id = EUCJP_NKF;
			} else if (StrncmpFromHead("EUCJP-MS", name)) {
				id = EUCJP_MS;
			} else if (StrncmpFromHead("EUCJPMS", name)) {
				id = EUCJP_MS;
			} else if (StrncmpFromHead("EUCJP-ASCII", name)) {
				id = EUCJP_ASCII;
			}
			break;

		case '-': // 「EUC-**」の場合
			if (StrncmpFromHead("EUC-JP", name)) {
				id = EUC_JP;
			} else if (StrncmpFromHead("EUC-JP-MS", name)) {
				id = EUCJP_MS;
			} else if (StrncmpFromHead("EUC-JP-ASCII", name)) {
				id = EUCJP_ASCII;
			} else if (StrncmpFromHead("EUC-JISX0213", name)) {
				id = EUC_JISX0213;
			} else if (StrncmpFromHead("EUC-JIS-2004", name)) {
				id = EUC_JIS_2004;
			}
			break;
		}
		break;

	case 'I': // 最初の文字が「I」の場合, ISO**
		if (StrncmpFromHead("ISO-2022-JP", name)) {
			id = ISO_2022_JP;
		} else if (StrncmpFromHead("ISO2022JP-CP932", name)) {
			id = CP50220;
		} else if (StrncmpFromHead("ISO-2022-JP-1", name)) {
			id = ISO_2022_JP_1;
		} else if (StrncmpFromHead("ISO-2022-JP-3", name)) {
			id = ISO_2022_JP_3;
		} else if (StrncmpFromHead("ISO-2022-JP-2004", name)) {
			id = ISO_2022_JP_2004;
		}
		break;

	case 'M': // 最初の文字が「M」の場合
		if (StrncmpFromHead("MS_Kanji", name)) {
			id = SHIFT_JIS;
		} else if (StrncmpFromHead("MS932", name)) {
			id = WINDOWS_31J;
		}
		break;

	case 'P':
		id = SHIFT_JIS;
		break;

	case 'R':
		id = ASCII;
		break;

	case 'S': // 最初の文字が「S」の場合
		if (StrncmpFromHead("SHIFT_JIS", name)) {
			id = SHIFT_JIS;
		} else if (StrncmpFromHead("SJIS", name)) {
			id = SHIFT_JIS;
		} else if (StrncmpFromHead("SHIFT_JISX0213", name)) {
			id = SHIFT_JISX0213;
		} else if (StrncmpFromHead("SHIFT_JIS-2004", name)) {
			id = SHIFT_JIS_2004;
		}
		break;

	case 'U': // 最初の文字が「U」の場合

		switch (name[4]) { // 「UTF-*」の場合

		case '8':
			if (StrncmpFromHead("UTF-8", name)) {
				id = UTF_8;
			} else if (StrncmpFromHead("UTF-8N", name)) {
				id = UTF_8N;
			} else if (StrncmpFromHead("UTF-8-BOM", name)) {
				id = UTF_8_BOM;
			} else if (StrncmpFromHead("UTF-8-MAC", name)) {
				id = UTF8_MAC;
			}
			break;

		case '-':
			id = UTF8_MAC;
			break;

		case '1':
			if (StrncmpFromHead("UTF-16", name)) {
				id = UTF_16;
			} else if (StrncmpFromHead("UTF-16BE", name)) {
				id = UTF_16BE;
			} else if (StrncmpFromHead("UTF-16BE-BOM", name)) {
				id = UTF_16BE_BOM;
			} else if (StrncmpFromHead("UTF-16LE", name)) {
				id = UTF_16LE;
			} else if (StrncmpFromHead("UTF-16LE-BOM", name)) {
				id = UTF_16LE_BOM;
			}
			break;

		case '3':
			if (StrncmpFromHead("UTF-32", name)) {
				id = UTF_32;
			} else if (StrncmpFromHead("UTF-32BE", name)) {
				id = UTF_32BE;
			} else if (StrncmpFromHead("UTF-32BE-BOM", name)) {
				id = UTF_32BE_BOM;
			} else if (StrncmpFromHead("UTF-32LE", name)) {
				id = UTF_32LE;
			} else if (StrncmpFromHead("UTF-32LE-BOM", name)) {
				id = UTF_32LE_BOM;
			}
			break;
		}
		break;

	case 'W':
		id = WINDOWS_31J;
		break;
	}

	return id;
}
wxNKFEncoding* Util::NKFDefaultEncoding() {
}

nkf_char Util::E2wConv(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags) {
}

nkf_char Util::S2eConv(nkf_char c2, nkf_char c1, nkf_char* p2, nkf_char* p1,
		std::bitset<nkf_flag_num> nkfFlags) {
}

nkf_char Util::W2eConv(nkf_char c2, nkf_char c1, nkf_char c0, nkf_char* p2,
		nkf_char* p1, std::bitset<nkf_flag_num> nkfFlags) {
}

nkf_char Util::E2sConv(nkf_char c2, nkf_char c1, nkf_char* p2, nkf_char* p1,
		std::bitset<nkf_flag_num> nkfFlags) {
}

/**
 * get default os locale
 */
void Util::NKFLocaleEncoding(wxNKFEncoding* enc) {

}

void Util::NKFUnicodeToUTF8(nkf_char val, nkf_char* p1, nkf_char* p2,
		nkf_char* p3, nkf_char* p4) {
}

nkf_char Util::NKFUTF8ToUnicode(nkf_char c1, nkf_char c2, nkf_char c3,
		nkf_char c4) {
}

int Util::UnicodeToJISCommon(nkf_char c2, nkf_char c1, nkf_char c0,
		nkf_char* p2, nkf_char* p1, std::bitset<nkf_flag_num> nkfFlags) {
}

int Util::UnicodeToJISCommon2(nkf_char c1, nkf_char c0,
		const unsigned short * const * pp, nkf_char psize, nkf_char* p2,
		nkf_char* p1, std::bitset<nkf_flag_num> nkfFlags) {
}

nkf_char Util::W16eConv(nkf_char val, nkf_char* p2, nkf_char* p1,
		std::bitset<nkf_flag_num> nkfFlags) {
}

nkf_char Util::X0212Shift(nkf_char c) {
}

nkf_char Util::X0212Unshift(nkf_char c) {
}

bool Util::StrncmpFromHead(const char* charCode, const char* name) {
}

