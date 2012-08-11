/*
 * Copyright (c) 1987, Fujitsu LTD. (Itaru ICHIKAWA).
 * Copyright (c) 1996-2010, The nkf Project.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 */

/*
 * wxnkf.cpp
 *
 *  Created on: 2012/08/09
 * Contributor: Hiroyuki Nagata
 */
#include "wx/wxnkf.h"

#define NKF_VERSION "2.1.2"
#define NKF_RELEASE_DATE "2011-09-08"
#define COPY_RIGHT \
    "Copyright (C) 1987, FUJITSU LTD. (I.Ichikawa).\n" \
    "Copyright (C) 1996-2011, The nkf Project."

/**
 * constructor
 */
wxNKF::wxNKF() {
	/* prepare nkf flag set */
	FlagSet* flag = new FlagSet();
	nkfFlags = flag->GetFlagSet();
	delete flag;

	/* prepare wstring */
	oConvStr = new std::wstring();

	/* prepare encode setting class */
	wxEnc = new wxNKFEncoding();
}
/**
 * destructor
 */
wxNKF::~wxNKF() {
	delete oConvStr;
	delete wxEnc;
}
/**
 * convert charcter code in file, with option
 */
int wxNKF::Convert(const wxString inputFilePath, const wxString outputFilePath,
		const wxString option) {

	// validate
	if (0 == inputFilePath.Len() || 0 == outputFilePath.Len()
			|| 0 == option.Len()) {
		return -1;
	}

	// set option
	if (0 != SetOption(option))
		return -1;

	// prepare file system
	wxFileSystem* fileSystem = new wxFileSystem();
	wxFSFile* file = fileSystem->OpenFile(inputFilePath);

	if (!file) {
		// cannot get filestream
		delete fileSystem;
		return -1;
	}

	wxInputStream* in = file->GetStream();

	// prepare outputstream
	wxFileOutputStream output(outputFilePath);
	wxDataOutputStream out(output);

	// Convert char code
	if (0 != KanjiConvert(in, &out)) {
		delete fileSystem;
		return -1;
	}

	// delete resource
	delete fileSystem;
}
/**
 * main method of this class
 * convert charcode
 */
int wxNKF::KanjiConvert(wxInputStream* in, wxDataOutputStream* out) {
	while (!in->Eof()) {
		out->Write8(in->GetC());
	}
}
/**
 * convert charcter code in string, with option
 */
wxString wxNKF::Convert(const wxString inputFilePath, const wxString option) {

	// validate
	if (0 == inputFilePath.Len() || 0 == option.Len()) {
		return wxEmptyString;
	}

	// set option
	if (0 != SetOption(option))
		return wxEmptyString;

	// prepare file system
	wxFileSystem* fileSystem = new wxFileSystem();
	wxFSFile* file = fileSystem->OpenFile(inputFilePath);

	if (!file) {
		// cannot get filestream
		delete fileSystem;
		return wxEmptyString;
	}

	wxInputStream* in = file->GetStream();
	wxString result = KanjiConvert(in);
	delete fileSystem;

	return result;
}
/**
 * main method of this class convert char to string
 */
wxString wxNKF::KanjiConvert(wxInputStream* in) {

	std::wstring wideStr;

	while (!in->Eof()) {
		wideStr.push_back(in->GetC());
	}

	wchar_t* wideChar = (wchar_t*) wideStr.c_str();
	wxString result(wideChar);

	return result;
}
/**
 * SetOption : setting and judge options
 *
 * return values:
 *    0: success
 *   -1: ArgumentError
 */
int wxNKF::SetOption(const wxString option) {

	const char* buf = option.c_str();
	unsigned char* cp = (unsigned char*)buf;
	nkf_char i, j;
	unsigned char *p;
	unsigned char *cp_back = NULL;
	bool bitshift;

	if (nkfFlags[option_mode])
		return 0;

	while (*cp && *cp++ != '-')
		;
	while (*cp || cp_back) {
		if (!*cp) {
			cp = cp_back;
			cp_back = NULL;
			continue;
		}
		p = 0;
		switch (*cp++) {
		case '-': /* literal options */
			if (!*cp || *cp == SP) { /* ignore the rest of arguments */
				nkfFlags[option_mode] = TRUE;
				return 0;
			}
			for (i = 0;
					i < (int) (sizeof(long_option) / sizeof(long_option[0]));
					i++) {
				p = (unsigned char *) long_option[i].name;
				for (j = 0; *p && *p != '=' && *p == cp[j]; p++, j++)
					;
				if (*p == cp[j] || cp[j] == SP) {
					p = &cp[j] + 1;
					break;
				}
				p = 0;
			}
			if (p == 0) {
				return -1;
			}
			while (*cp && *cp != SP && cp++)
				;
			if (long_option[i].alias[0]) {
				cp_back = cp;
				cp = (unsigned char *) long_option[i].alias;
			} else {
				if (strcmp(long_option[i].name, "help") == 0) {
					ShowUsage();
					exit(EXIT_SUCCESS);
				}
				if (strcmp(long_option[i].name, "ic=") == 0) {
					Util::NKFEncFind((char *) p, wxEnc, SET_INPUT_MODE);
					continue;
				}
				if (strcmp(long_option[i].name, "oc=") == 0) {
					Util::NKFEncFind((char *) p, wxEnc, SET_OUTPUT_MODE);
					/* if (enc <= 0) continue; */
					if (!wxEnc->oCharName.Len())
						continue;
					continue;
				}
				if (strcmp(long_option[i].name, "guess=") == 0) {
					if (p[0] == '0' || p[0] == '1') {
						nkfFlags[guess_f] = 1;
					} else {
						nkfFlags[guess_f] = 2;
					}
					continue;
				}
				if (strcmp(long_option[i].name, "overwrite") == 0) {
					nkfFlags[file_out_f] = TRUE;
					nkfFlags[overwrite_f] = TRUE;
					nkfFlags[preserve_time_f] = TRUE;
					continue;
				}
				if (strcmp(long_option[i].name, "overwrite=") == 0) {
					nkfFlags[file_out_f] = TRUE;
					nkfFlags[overwrite_f] = TRUE;
					nkfFlags[preserve_time_f] = TRUE;
					nkfFlags[backup_f] = TRUE;
					//nkfFlags[backup_suffix = (char *) p;
					continue;
				}
				if (strcmp(long_option[i].name, "in-place") == 0) {
					nkfFlags[file_out_f] = TRUE;
					nkfFlags[overwrite_f] = TRUE;
					nkfFlags[preserve_time_f] = FALSE;
					continue;
				}
				if (strcmp(long_option[i].name, "in-place=") == 0) {
					nkfFlags[file_out_f] = TRUE;
					nkfFlags[overwrite_f] = TRUE;
					nkfFlags[preserve_time_f] = FALSE;
					nkfFlags[backup_f] = TRUE;
					//backup_suffix = (char *) p;
					continue;
				}

				if (strcmp(long_option[i].name, "cap-input") == 0) {
					nkfFlags[cap_f] = TRUE;
					continue;
				}
				if (strcmp(long_option[i].name, "url-input") == 0) {
					nkfFlags[url_f] = TRUE;
					continue;
				}

				if (strcmp(long_option[i].name, "numchar-input") == 0) {
					nkfFlags[numchar_f] = TRUE;
					continue;
				}

				if (strcmp(long_option[i].name, "no-output") == 0) {
					nkfFlags[noout_f] = TRUE;
					continue;
				}
				if (strcmp(long_option[i].name, "debug") == 0) {
					nkfFlags[debug_f] = TRUE;
					continue;
				}

				if (strcmp(long_option[i].name, "cp932") == 0) {

					nkfFlags[cp51932_f] = TRUE;
					nkfFlags[cp932inv_f] = -TRUE;
					nkfFlags[ms_ucs_map_f] = UCS_MAP_CP932;

					continue;
				}
				if (strcmp(long_option[i].name, "no-cp932") == 0) {

					nkfFlags[cp51932_f] = FALSE;
					nkfFlags[cp932inv_f] = FALSE;
					nkfFlags[ms_ucs_map_f] = UCS_MAP_ASCII;

					continue;
				}

				if (strcmp(long_option[i].name, "cp932inv") == 0) {
					nkfFlags[cp932inv_f] = -TRUE;
					continue;
				}

				if (strcmp(long_option[i].name, "x0212") == 0) {
					nkfFlags[x0212_f] = TRUE;
					continue;
				}

				if (strcmp(long_option[i].name, "no-cp932ext") == 0) {
					nkfFlags[no_cp932ext_f] = TRUE;
					continue;
				}
				if (strcmp(long_option[i].name, "no-best-fit-chars") == 0) {
					nkfFlags[no_best_fit_chars_f] = TRUE;
					continue;
				}
//				if (strcmp(long_option[i].name, "fb-skip") == 0) {
//					encode_fallback = NULL;
//					continue;
//				}
//				if (strcmp(long_option[i].name, "fb-html") == 0) {
//					encode_fallback = encode_fallback_html;
//					continue;
//				}
//				if (strcmp(long_option[i].name, "fb-xml") == 0) {
//					encode_fallback = encode_fallback_xml;
//					continue;
//				}
//				if (strcmp(long_option[i].name, "fb-java") == 0) {
//					encode_fallback = encode_fallback_java;
//					continue;
//				}
//				if (strcmp(long_option[i].name, "fb-perl") == 0) {
//					encode_fallback = encode_fallback_perl;
//					continue;
//				}
//				if (strcmp(long_option[i].name, "fb-subchar") == 0) {
//					encode_fallback = encode_fallback_subchar;
//					continue;
//				}
//				if (strcmp(long_option[i].name, "fb-subchar=") == 0) {
//					encode_fallback = encode_fallback_subchar;
//					unicode_subchar = 0;
//					if (p[0] != '0') {
//						/* decimal number */
//						for (i = 0; i < 7 && nkf_isdigit(p[i]); i++) {
//							unicode_subchar *= 10;
//							unicode_subchar += hex2bin(p[i]);
//						}
//					} else if (p[1] == 'x' || p[1] == 'X') {
//						/* hexadecimal number */
//						for (i = 2; i < 8 && nkf_isxdigit(p[i]); i++) {
//							unicode_subchar <<= 4;
//							unicode_subchar |= hex2bin(p[i]);
//						}
//					} else {
//						/* octal number */
//						for (i = 1; i < 8 && nkf_isoctal(p[i]); i++) {
//							unicode_subchar *= 8;
//							unicode_subchar += hex2bin(p[i]);
//						}
//					}
//					w16e_conv(unicode_subchar, &i, &j);
//					unicode_subchar = i << 8 | j;
//					continue;
//				}

				if (strcmp(long_option[i].name, "ms-ucs-map") == 0) {
					nkfFlags[ms_ucs_map_f] = UCS_MAP_MS;
					continue;
				}

				if (strcmp(long_option[i].name, "utf8mac-input") == 0) {
					nkfFlags[nfc_f] = TRUE;
					continue;
				}

				if (strcmp(long_option[i].name, "prefix=") == 0) {
					if (nkf_isgraph(p[0])) {
						for (i = 1; nkf_isgraph(p[i]); i++) {
							//prefix_table[p[i]] = p[0];
						}
					}
					continue;
				}

				fprintf(stderr, "unsupported long option: --%s\n",
						long_option[i].name);

				return -1;
			}
			continue;
		case 'b': /* buffered mode */
			nkfFlags[unbuf_f] = FALSE;
			continue;
		case 'u': /* non bufferd mode */
			nkfFlags[unbuf_f] = TRUE;
			continue;
		case 't': /* transparent mode */
			if (*cp == '1') {
				/* alias of -t */
				cp++;
				nkfFlags[nop_f] = TRUE;
			} else if (*cp == '2') {
				/*
				 * -t with put/get
				 *
				 * nkf -t2MB hoge.bin | nkf -t2mB | diff -s - hoge.bin
				 *
				 */
				cp++;
				nkfFlags[nop_f] = 2;
			} else
				nkfFlags[nop_f] = TRUE;
			continue;
		case 'j': /* JIS output */
		case 'n':
			Util::NKFEncFromIndex(ISO_2022_JP, wxEnc, SET_OUTPUT_MODE);
			continue;
		case 'e': /* AT&T EUC output */
			Util::NKFEncFromIndex(EUCJP_NKF, wxEnc, SET_OUTPUT_MODE);
			continue;
		case 's': /* SJIS output */
			Util::NKFEncFromIndex(SHIFT_JIS, wxEnc, SET_OUTPUT_MODE);
			continue;
		case 'l': /* ISO8859 Latin-1 support, no conversion */
			nkfFlags[iso8859_f] = TRUE; /* Only compatible with ISO-2022-JP */
			Util::NKFEncFromIndex(ISO_8859_1, wxEnc, SET_INPUT_MODE);
			continue;
		case 'i': /* Kanji IN ESC-$-@/B */
			if (*cp == '@' || *cp == 'B')
				nkfFlags[kanji_intro] = *cp++;
			continue;
		case 'o': /* ASCII IN ESC-(-J/B/H */
			/* ESC ( H was used in initial JUNET messages */
			if (*cp == 'J' || *cp == 'B' || *cp == 'H')
				nkfFlags[ascii_intro] = *cp++;
			continue;
		case 'h':
			/*
			 bit:1   katakana->hiragana
			 bit:2   hiragana->katakana
			 */
			if ('9' >= *cp && *cp >= '0') {
				unsigned int shift = nkfFlags[hira_f];
				shift |= (*cp++ - '0');
				nkfFlags[hira_f] = shift;
			} else {
				unsigned int shift = nkfFlags[hira_f];
				shift |= 1;
				nkfFlags[hira_f] = shift;
			}
			continue;
		case 'r':
			nkfFlags[rot_f] = TRUE;
			continue;

//		case 'V':
//			LibNKF::ShowConfiguration();
//			exit(EXIT_SUCCESS);
//			break;
		case 'v':
			ShowVersion();
			exit(EXIT_SUCCESS);
			break;

		case 'w': /* UTF-{8,16,32} output */
			if (cp[0] == '8') {
				cp++;
				if (cp[0] == '0') {
					cp++;
					Util::NKFEncFromIndex(UTF_8N, wxEnc, SET_OUTPUT_MODE);
				} else {
					nkfFlags[output_bom_f] = TRUE;
					Util::NKFEncFromIndex(UTF_8_BOM, wxEnc, SET_OUTPUT_MODE);
				}
			} else {
				int enc_idx;
				if ('1' == cp[0] && '6' == cp[1]) {
					cp += 2;
					enc_idx = UTF_16;
				} else if ('3' == cp[0] && '2' == cp[1]) {
					cp += 2;
					enc_idx = UTF_32;
				} else {
					Util::NKFEncFromIndex(UTF_8, wxEnc, SET_OUTPUT_MODE);
					continue;
				}
				if (cp[0] == 'L') {
					cp++;
					wxEnc->oEndian = ENDIAN_LITTLE;
					nkfFlags[output_bom_f] = TRUE;
				} else if (cp[0] == 'B') {
					cp++;
					nkfFlags[output_bom_f] = TRUE;
				}
				if (cp[0] == '0') {
					nkfFlags[output_bom_f] = FALSE;
					cp++;
					enc_idx =
							enc_idx == UTF_16 ?
									(wxEnc->oEndian == ENDIAN_LITTLE ?
											UTF_16LE : UTF_16BE) :
									(wxEnc->oEndian == ENDIAN_LITTLE ?
											UTF_32LE : UTF_32BE);
				} else {
					enc_idx =
							enc_idx == UTF_16 ?
									(wxEnc->oEndian == ENDIAN_LITTLE ?
											UTF_16LE_BOM : UTF_16BE_BOM) :
									(wxEnc->oEndian == ENDIAN_LITTLE ?
											UTF_32LE_BOM : UTF_32BE_BOM);
				}
				Util::NKFEncFromIndex(enc_idx, wxEnc, SET_OUTPUT_MODE);
			}
			continue;

		case 'W': /* UTF input */
			if (cp[0] == '8') {
				cp++;
				Util::NKFEncFromIndex(UTF_8, wxEnc, SET_INPUT_MODE);
			} else {
				int enc_idx;
				if ('1' == cp[0] && '6' == cp[1]) {
					cp += 2;
					wxEnc->iEndian = ENDIAN_BIG;
					enc_idx = UTF_16;
				} else if ('3' == cp[0] && '2' == cp[1]) {
					cp += 2;
					wxEnc->iEndian = ENDIAN_BIG;
					enc_idx = UTF_32;
				} else {
					Util::NKFEncFromIndex(UTF_8, wxEnc, SET_OUTPUT_MODE);
					continue;
				}
				if (cp[0] == 'L') {
					cp++;
					wxEnc->iEndian = ENDIAN_LITTLE;
				} else if (cp[0] == 'B') {
					cp++;
					wxEnc->iEndian = ENDIAN_BIG;
				}
				enc_idx = (
						enc_idx == UTF_16 ?
								(wxEnc->iEndian == ENDIAN_LITTLE ?
										UTF_16LE : UTF_16BE) :
								(wxEnc->iEndian == ENDIAN_LITTLE ?
										UTF_32LE : UTF_32BE));
				Util::NKFEncFromIndex(enc_idx, wxEnc, SET_INPUT_MODE);
			}
			continue;

			/* Input code assumption */
		case 'J': /* ISO-2022-JP input */
			Util::NKFEncFromIndex(ISO_2022_JP, wxEnc, SET_INPUT_MODE);
			continue;
		case 'E': /* EUC-JP input */
			Util::NKFEncFromIndex(EUCJP_NKF, wxEnc, SET_INPUT_MODE);
			continue;
		case 'S': /* Shift_JIS input */
			Util::NKFEncFromIndex(SHIFT_JIS, wxEnc, SET_INPUT_MODE);
			continue;
		case 'Z': /* Convert X0208 alphabet to asii */
			/* alpha_f
			 bit:0   Convert JIS X 0208 Alphabet to ASCII
			 bit:1   Convert Kankaku to one space
			 bit:2   Convert Kankaku to two spaces
			 bit:3   Convert HTML Entity
			 bit:4   Convert JIS X 0208 Katakana to JIS X 0201 Katakana
			 */
			while ('0' <= *cp && *cp <= '4') {
				unsigned int shift1 = 1 << (*cp++ - '0');
				unsigned int shift2 = shift1 |= nkfFlags[alpha_f];
				nkfFlags[alpha_f] = shift2;
			}
			bitshift = nkfFlags[alpha_f];
			bitshift |= 1;
			nkfFlags[alpha_f] = bitshift;

			continue;
		case 'x': /* Convert X0201 kana to X0208 or X0201 Conversion */
			nkfFlags[x0201_f] = FALSE; /* No X0201->X0208 conversion */
			/* accept  X0201
			 ESC-(-I     in JIS, EUC, MS Kanji
			 SI/SO       in JIS, EUC, MS Kanji
			 SS2         in EUC, JIS, not in MS Kanji
			 MS Kanji (0xa0-0xdf)
			 output  X0201
			 ESC-(-I     in JIS (0x20-0x5f)
			 SS2         in EUC (0xa0-0xdf)
			 0xa0-0xd    in MS Kanji (0xa0-0xdf)
			 */
			continue;
		case 'X': /* Convert X0201 kana to X0208 */
			nkfFlags[x0201_f] = TRUE;
			continue;
		case 'F': /* prserve new lines */
			nkfFlags[fold_preserve_f] = TRUE;
		case 'f': /* folding -f60 or -f */
			nkfFlags[fold_f] = TRUE;
			fold_len = 0;
			while ('0' <= *cp && *cp <= '9') { /* we don't use atoi here */
				fold_len *= 10;
				fold_len += *cp++ - '0';
			}
			if (!(0 < fold_len && fold_len < BUFSIZ))
				fold_len = DEFAULT_FOLD;
			if (*cp == '-') {
				fold_margin = 0;
				cp++;
				while ('0' <= *cp && *cp <= '9') { /* we don't use atoi here */
					fold_margin *= 10;
					fold_margin += *cp++ - '0';
				}
			}
			continue;
		case 'm': /* MIME support */
			/* mime_decode_f = TRUE; *//* this has too large side effects... */
			if (*cp == 'B' || *cp == 'Q') {
				nkfFlags[mime_decode_mode] = *cp++;
				nkfFlags[mimebuf_f] = FIXED_MIME;
			} else if (*cp == 'N') {
				nkfFlags[mime_f] = TRUE;
				cp++;
			} else if (*cp == 'S') {
				nkfFlags[mime_f] = STRICT_MIME;
				cp++;
			} else if (*cp == '0') {
				nkfFlags[mime_decode_f] = FALSE;
				nkfFlags[mime_f] = FALSE;
				cp++;
			} else {
				nkfFlags[mime_f] = STRICT_MIME;
			}
			continue;
		case 'M': /* MIME output */
			if (*cp == 'B') {
				mimeout_mode = 'B';
				nkfFlags[mimeout_f] = FIXED_MIME;
				cp++;
			} else if (*cp == 'Q') {
				mimeout_mode = 'Q';
				nkfFlags[mimeout_f] = FIXED_MIME;
				cp++;
			} else {
				nkfFlags[mimeout_f] = TRUE;
			}
			continue;
		case 'B': /* Broken JIS support */
			/*  bit:0   no ESC JIS
			 bit:1   allow any x on ESC-(-x or ESC-$-x
			 bit:2   reset to ascii on NL
			 */
			if ('9' >= *cp && *cp >= '0') {
				int bitshift1 = 1 << (*cp++ - '0');
				int bitshift2 = nkfFlags[broken_f];
				bitshift2 |= bitshift1;
				nkfFlags[broken_f] = bitshift2;
			} else {
				int bitshift1 = nkfFlags[broken_f];
				bitshift1 |= TRUE;
				nkfFlags[broken_f] = bitshift1;
			}
			continue;

		case 'O':/* for Output file */
			nkfFlags[file_out_f] = TRUE;
			continue;

		case 'c':/* add cr code */
			nkfFlags[eolmode_f] = CRLF;
			continue;
		case 'd':/* delete cr code */
			nkfFlags[eolmode_f] = LF;
			continue;
		case 'I': /* ISO-2022-JP output */
			nkfFlags[iso2022jp_f] = TRUE;
			continue;
		case 'L': /* line mode */
			if (*cp == 'u') { /* unix */
				nkfFlags[eolmode_f] = LF;
				cp++;
			} else if (*cp == 'm') { /* mac */
				nkfFlags[eolmode_f] = CR;
				cp++;
			} else if (*cp == 'w') { /* windows */
				nkfFlags[eolmode_f] = CRLF;
				cp++;
			} else if (*cp == '0') { /* no conversion  */
				nkfFlags[eolmode_f] = 0;
				cp++;
			}
			continue;

		case 'g':
			if ('2' <= *cp && *cp <= '9') {
				nkfFlags[guess_f] = 2;
				cp++;
			} else if (*cp == '0' || *cp == '1') {
				nkfFlags[guess_f] = 1;
				cp++;
			} else {
				nkfFlags[guess_f] = 1;
			}
			continue;

		case SP:
			/* module muliple options in a string are allowed for Perl moudle  */
			while (*cp && *cp++ != '-')
				;
			continue;
		default:

			fprintf(stderr, "unknown option: -%c\n", *(cp - 1));

			/* bogus option but ignored */
			return -1;
		}
	}
	return 0;
}
/**
 * show usage
 */
void wxNKF::ShowUsage() {
	fprintf(HELP_OUTPUT,
			"Usage:  nkf -[flags] [--] [in file] .. [out file for -O flag]\n"
					" j/s/e/w  Specify output encoding ISO-2022-JP, Shift_JIS, EUC-JP\n"
					"          UTF options is -w[8[0],{16,32}[{B,L}[0]]]\n"
					" J/S/E/W  Specify input encoding ISO-2022-JP, Shift_JIS, EUC-JP\n"
					"          UTF option is -W[8,[16,32][B,L]]\n"
					" J/S/E    Specify output encoding ISO-2022-JP, Shift_JIS, EUC-JP\n");
	fprintf(HELP_OUTPUT,
			" m[BQSN0] MIME decode [B:base64,Q:quoted,S:strict,N:nonstrict,0:no decode]\n"
					" M[BQ]    MIME encode [B:base64 Q:quoted]\n"
					" f/F      Folding: -f60 or -f or -f60-10 (fold margin 10) F preserve nl\n");
	fprintf(HELP_OUTPUT,
			" Z[0-4]   Default/0: Convert JISX0208 Alphabet to ASCII\n"
					"          1: Kankaku to one space  2: to two spaces  3: HTML Entity\n"
					"          4: JISX0208 Katakana to JISX0201 Katakana\n"
					" X,x      Convert Halfwidth Katakana to Fullwidth or preserve it\n");
	fprintf(HELP_OUTPUT, " O        Output to File (DEFAULT 'nkf.out')\n"
			" L[uwm]   Line mode u:LF w:CRLF m:CR (DEFAULT noconversion)\n");
	fprintf(HELP_OUTPUT, " --ic=<encoding>        Specify the input encoding\n"
			" --oc=<encoding>        Specify the output encoding\n"
			" --hiragana --katakana  Hiragana/Katakana Conversion\n"
			" --katakana-hiragana    Converts each other\n");
	fprintf(HELP_OUTPUT,
			" --{cap, url}-input     Convert hex after ':' or '%%'\n"
					" --numchar-input        Convert Unicode Character Reference\n"
					" --fb-{skip, html, xml, perl, java, subchar}\n"
					"                        Specify unassigned character's replacement\n");
	fprintf(HELP_OUTPUT, " --in-place[=SUF]       Overwrite original files\n"
			" --overwrite[=SUF]      Preserve timestamp of original files\n"
			" -g --guess             Guess the input code\n"
			" -v --version           Print the version\n"
			" --help/-V              Print this help / configuration\n");
	ShowVersion();
}
/**
 * show version
 */
void wxNKF::ShowVersion() {
	fprintf(HELP_OUTPUT,
			"Network Kanji Filter Version " NKF_VERSION " (" NKF_RELEASE_DATE ") \n" COPY_RIGHT "\n");
}

/**
 * set flag for Output
 */
void wxNKF::SetInputEncoding(wxNKFEncoding* enc) {

	switch (enc->iCharID) {

	case ISO_8859_1:
		nkfFlags[iso8859_f] = TRUE;
		break;
	case CP50221:
	case CP50222:
		if (nkfFlags[x0201_f] == NKF_UNSPECIFIED)
			nkfFlags[x0201_f] = FALSE; /* -x specified implicitly */
	case CP50220:
		nkfFlags[cp51932_f] = TRUE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_CP932;
		break;
	case ISO_2022_JP_1:
		nkfFlags[x0212_f] = TRUE;
		break;
	case ISO_2022_JP_3:
		nkfFlags[x0212_f] = TRUE;
		nkfFlags[x0213_f] = TRUE;
		break;
	case ISO_2022_JP_2004:
		nkfFlags[x0212_f] = TRUE;
		nkfFlags[x0213_f] = TRUE;
		break;
	case SHIFT_JIS:
		break;
	case WINDOWS_31J:
		if (nkfFlags[x0201_f] == NKF_UNSPECIFIED)
			nkfFlags[x0201_f] = FALSE; /* -x specified implicitly */
		nkfFlags[cp51932_f] = TRUE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_CP932;
		break;
		break;
	case CP10001:
		nkfFlags[cp51932_f] = TRUE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_CP10001;
		break;
	case EUC_JP:
		break;
	case EUCJP_NKF:
		break;
	case CP51932:
		if (nkfFlags[x0201_f] == NKF_UNSPECIFIED)
			nkfFlags[x0201_f] = FALSE; /* -x specified implicitly */
		nkfFlags[cp51932_f] = TRUE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_CP932;
		break;
	case EUCJP_MS:
		if (nkfFlags[x0201_f] == NKF_UNSPECIFIED)
			nkfFlags[x0201_f] = FALSE; /* -x specified implicitly */
		nkfFlags[cp51932_f] = FALSE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_MS;
		break;
	case EUCJP_ASCII:
		if (nkfFlags[x0201_f] == NKF_UNSPECIFIED)
			nkfFlags[x0201_f] = FALSE; /* -x specified implicitly */
		nkfFlags[cp51932_f] = FALSE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_ASCII;
		break;
	case SHIFT_JISX0213:
	case SHIFT_JIS_2004:
		nkfFlags[x0213_f] = TRUE;
		nkfFlags[cp51932_f] = FALSE;
		break;
	case EUC_JISX0213:
	case EUC_JIS_2004:
		nkfFlags[x0213_f] = TRUE;
		nkfFlags[cp51932_f] = FALSE;
		break;
	case UTF8_MAC:
		nkfFlags[nfc_f] = TRUE;
		break;
	case UTF_16:
	case UTF_16BE:
	case UTF_16BE_BOM:
		enc->iEndian = ENDIAN_BIG;
		break;
	case UTF_16LE:
	case UTF_16LE_BOM:
		enc->iEndian = ENDIAN_LITTLE;
		break;
	case UTF_32:
	case UTF_32BE:
	case UTF_32BE_BOM:
		enc->iEndian = ENDIAN_BIG;
		break;
	case UTF_32LE:
	case UTF_32LE_BOM:
		enc->iEndian = ENDIAN_LITTLE;
		break;
	}
}
void wxNKF::SetOutputEncoding(wxNKFEncoding* enc) {

	switch (enc->oCharID) {

	case CP50220:
		if (nkfFlags[cp932inv_f] == TRUE)
			nkfFlags[cp932inv_f] = FALSE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_CP932;
		break;
	case CP50221:
		if (nkfFlags[x0201_f] == NKF_UNSPECIFIED)
			nkfFlags[x0201_f] = FALSE; /* -x specified implicitly */
		if (nkfFlags[cp932inv_f] == TRUE)
			nkfFlags[cp932inv_f] = FALSE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_CP932;
		break;
	case ISO_2022_JP:
		if (nkfFlags[cp932inv_f] == TRUE)
			nkfFlags[cp932inv_f] = FALSE;
		break;
	case ISO_2022_JP_1:
		nkfFlags[x0212_f] = TRUE;
		if (nkfFlags[cp932inv_f] == TRUE)
			nkfFlags[cp932inv_f] = FALSE;
		break;
	case ISO_2022_JP_3:
		nkfFlags[x0212_f] = TRUE;
		nkfFlags[x0213_f] = TRUE;
		if (nkfFlags[cp932inv_f] == TRUE)
			nkfFlags[cp932inv_f] = FALSE;
		break;
	case SHIFT_JIS:
		break;
	case WINDOWS_31J:
		if (nkfFlags[x0201_f] == NKF_UNSPECIFIED)
			nkfFlags[x0201_f] = FALSE; /* -x specified implicitly */
		nkfFlags[ms_ucs_map_f] = UCS_MAP_CP932;
		break;
	case CP10001:
		nkfFlags[ms_ucs_map_f] = UCS_MAP_CP10001;
		break;
	case EUC_JP:
		nkfFlags[x0212_f] = TRUE;
		if (nkfFlags[cp932inv_f] == TRUE)
			nkfFlags[cp932inv_f] = FALSE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_ASCII;
		break;
	case EUCJP_NKF:
		nkfFlags[x0212_f] = FALSE;
		if (nkfFlags[cp932inv_f] == TRUE)
			nkfFlags[cp932inv_f] = FALSE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_ASCII;
		break;
	case CP51932:
		if (nkfFlags[x0201_f] == NKF_UNSPECIFIED)
			nkfFlags[x0201_f] = FALSE; /* -x specified implicitly */
		if (nkfFlags[cp932inv_f] == TRUE)
			nkfFlags[cp932inv_f] = FALSE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_CP932;
		break;
	case EUCJP_MS:
		if (nkfFlags[x0201_f] == NKF_UNSPECIFIED)
			nkfFlags[x0201_f] = FALSE; /* -x specified implicitly */
		nkfFlags[x0212_f] = TRUE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_MS;
		break;
	case EUCJP_ASCII:
		if (nkfFlags[x0201_f] == NKF_UNSPECIFIED)
			nkfFlags[x0201_f] = FALSE; /* -x specified implicitly */
		nkfFlags[x0212_f] = TRUE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_ASCII;
		break;
	case SHIFT_JISX0213:
	case SHIFT_JIS_2004:
		nkfFlags[x0213_f] = TRUE;
		if (nkfFlags[cp932inv_f] == TRUE)
			nkfFlags[cp932inv_f] = FALSE;
		break;
	case EUC_JISX0213:
	case EUC_JIS_2004:
		nkfFlags[x0212_f] = TRUE;
		nkfFlags[x0213_f] = TRUE;
		if (nkfFlags[cp932inv_f] == TRUE)
			nkfFlags[cp932inv_f] = FALSE;
		break;
	case UTF_8_BOM:
		nkfFlags[output_bom_f] = TRUE;
		break;
	case UTF_16:
	case UTF_16BE_BOM:
		nkfFlags[output_bom_f] = TRUE;
		break;
	case UTF_16LE:
		enc->oEndian = ENDIAN_LITTLE;
		nkfFlags[output_bom_f] = FALSE;
		break;
	case UTF_16LE_BOM:
		enc->oEndian = ENDIAN_LITTLE;
		nkfFlags[output_bom_f] = TRUE;
		break;
	case UTF_32:
	case UTF_32BE_BOM:
		nkfFlags[output_bom_f] = TRUE;
		break;
	case UTF_32LE:
		enc->oEndian = ENDIAN_LITTLE;
		nkfFlags[output_bom_f] = FALSE;
		break;
	case UTF_32LE_BOM:
		enc->oEndian = ENDIAN_LITTLE;
		nkfFlags[output_bom_f] = TRUE;
		break;
	}
}

