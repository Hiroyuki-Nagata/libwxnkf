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

#include "LibNKF.h"

#define NKF_VERSION "2.1.2"
#define NKF_RELEASE_DATE "2011-09-08"
#define COPY_RIGHT \
    "Copyright (C) 1987, FUJITSU LTD. (I.Ichikawa).\n" \
    "Copyright (C) 1996-2011, The nkf Project."

/**
 * コンストラクタ
 */
LibNKF::LibNKF() {
	flagPool = new FlagPool();
	enc = new NKFEncoding();

	inputEncoding = NULL;
	outputEncoding = NULL;
	inputEndian = ENDIAN_BIG;
	outputEndian = ENDIAN_BIG;
	mime_decode_mode = FALSE;
	mimeout_mode = 0;
	outputMode = ASCII;
	inputMode = ASCII;
}

/**
 * デストラクタ
 */
LibNKF::~LibNKF() {
	delete inputEncoding;
	delete outputEncoding;

	if (NULL != enc)
		delete enc;
	if (NULL != flagPool)
		delete flagPool;

	delete flagPool;
	delete enc;
}

/**
 * LibNKFの初期化処理を行う
 */
void LibNKF::ReInit() {
	for (int i = 0; i < 256; i++) {
		prefix_table[i] = 0;
	}
	//　以降のコーディングで必要と思われる初期化処理はここに記述
	kanji_intro = DEFAULT_J;
	ascii_intro = DEFAULT_R;
}

/**
 * SetOption:オプションの判別と設定を行う
 *
 * return values:
 *    0: success
 *   -1: ArgumentError
 */
int LibNKF::SetOption(const std::string option) {

	// LibNKFの初期化
	LibNKF::ReInit();

	unsigned char* cp = (unsigned char*) option.c_str();
	nkf_char i, j;
	unsigned char *p;
	unsigned char *cp_back = NULL;

	if (flagPool->option_mode == 1)
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
				flagPool->option_mode = 1;
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
					LibNKF::ShowUsage();
					exit(EXIT_SUCCESS);
				}
				if (strcmp(long_option[i].name, "ic=") == 0) {
					enc = Util::NKFEncFind((char *) p);
					if (!enc)
						continue;
					inputEncoding = enc;
					continue;
				}
				if (strcmp(long_option[i].name, "oc=") == 0) {
					enc = Util::NKFEncFind((char *) p);
					/* if (enc <= 0) continue; */
					if (!enc)
						continue;
					outputEncoding = enc;
					continue;
				}
				if (strcmp(long_option[i].name, "guess=") == 0) {
					if (p[0] == '0' || p[0] == '1') {
						flagPool->guess_f = 1;
					} else {
						flagPool->guess_f = 2;
					}
					continue;
				}
				if (strcmp(long_option[i].name, "overwrite") == 0) {
					flagPool->file_out_f = TRUE;
					flagPool->overwrite_f = TRUE;
					flagPool->preserve_time_f = TRUE;
					continue;
				}
				if (strcmp(long_option[i].name, "overwrite=") == 0) {
					flagPool->file_out_f = TRUE;
					flagPool->overwrite_f = TRUE;
					flagPool->preserve_time_f = TRUE;
					flagPool->backup_f = TRUE;
					flagPool->backup_suffix = (char *) p;
					continue;
				}
				if (strcmp(long_option[i].name, "in-place") == 0) {
					flagPool->file_out_f = TRUE;
					flagPool->overwrite_f = TRUE;
					flagPool->preserve_time_f = FALSE;
					continue;
				}
				if (strcmp(long_option[i].name, "in-place=") == 0) {
					flagPool->file_out_f = TRUE;
					flagPool->overwrite_f = TRUE;
					flagPool->preserve_time_f = FALSE;
					flagPool->backup_f = TRUE;
					flagPool->backup_suffix = (char *) p;
					continue;
				}

				if (strcmp(long_option[i].name, "cap-input") == 0) {
					flagPool->cap_f = TRUE;
					continue;
				}
				if (strcmp(long_option[i].name, "url-input") == 0) {
					flagPool->url_f = TRUE;
					continue;
				}

				if (strcmp(long_option[i].name, "numchar-input") == 0) {
					flagPool->numchar_f = TRUE;
					continue;
				}

				if (strcmp(long_option[i].name, "no-output") == 0) {
					flagPool->noout_f = TRUE;
					continue;
				}
				if (strcmp(long_option[i].name, "debug") == 0) {
					flagPool->debug_f = TRUE;
					continue;
				}

				if (strcmp(long_option[i].name, "cp932") == 0) {

					flagPool->cp51932_f = TRUE;
					flagPool->cp932inv_f = -TRUE;

					flagPool->ms_ucs_map_f = UCS_MAP_CP932;

					continue;
				}
				if (strcmp(long_option[i].name, "no-cp932") == 0) {

					flagPool->cp51932_f = FALSE;
					flagPool->cp932inv_f = FALSE;

					flagPool->ms_ucs_map_f = UCS_MAP_ASCII;

					continue;
				}

				if (strcmp(long_option[i].name, "cp932inv") == 0) {
					flagPool->cp932inv_f = -TRUE;
					continue;
				}

				if (strcmp(long_option[i].name, "x0212") == 0) {
					flagPool->x0212_f = TRUE;
					continue;
				}

				if (strcmp(long_option[i].name, "no-cp932ext") == 0) {
					flagPool->no_cp932ext_f = TRUE;
					continue;
				}
				if (strcmp(long_option[i].name, "no-best-fit-chars") == 0) {
					flagPool->no_best_fit_chars_f = TRUE;
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
					flagPool->ms_ucs_map_f = UCS_MAP_MS;
					continue;
				}

				if (strcmp(long_option[i].name, "utf8mac-input") == 0) {
					flagPool->nfc_f = TRUE;
					continue;
				}

				if (strcmp(long_option[i].name, "prefix=") == 0) {
					if (nkf_isgraph(p[0])) {
						for (i = 1; nkf_isgraph(p[i]); i++) {
							prefix_table[p[i]] = p[0];
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
			flagPool->unbuf_f = FALSE;
			continue;
		case 'u': /* non bufferd mode */
			flagPool->unbuf_f = TRUE;
			continue;
		case 't': /* transparent mode */
			if (*cp == '1') {
				/* alias of -t */
				cp++;
				flagPool->nop_f = TRUE;
			} else if (*cp == '2') {
				/*
				 * -t with put/get
				 *
				 * nkf -t2MB hoge.bin | nkf -t2mB | diff -s - hoge.bin
				 *
				 */
				cp++;
				flagPool->nop_f = 2;
			} else
				flagPool->nop_f = TRUE;
			continue;
		case 'j': /* JIS output */
		case 'n':
			outputEncoding = Util::NKFEncFromIndex(ISO_2022_JP);
			continue;
		case 'e': /* AT&T EUC output */
			outputEncoding = Util::NKFEncFromIndex(EUCJP_NKF);
			continue;
		case 's': /* SJIS output */
			outputEncoding = Util::NKFEncFromIndex(SHIFT_JIS);
			continue;
		case 'l': /* ISO8859 Latin-1 support, no conversion */
			flagPool->iso8859_f = TRUE; /* Only compatible with ISO-2022-JP */
			inputEncoding = Util::NKFEncFromIndex(ISO_8859_1);
			continue;
		case 'i': /* Kanji IN ESC-$-@/B */
			if (*cp == '@' || *cp == 'B')
				flagPool->kanji_intro = *cp++;
			continue;
		case 'o': /* ASCII IN ESC-(-J/B/H */
			/* ESC ( H was used in initial JUNET messages */
			if (*cp == 'J' || *cp == 'B' || *cp == 'H')
				flagPool->ascii_intro = *cp++;
			continue;
		case 'h':
			/*
			 bit:1   katakana->hiragana
			 bit:2   hiragana->katakana
			 */
			if ('9' >= *cp && *cp >= '0')
				flagPool->hira_f |= (*cp++ - '0');
			else
				flagPool->hira_f |= 1;
			continue;
		case 'r':
			flagPool->rot_f = TRUE;
			continue;

		case 'V':
			LibNKF::ShowConfiguration();
			exit(EXIT_SUCCESS);
			break;
		case 'v':
			ShowVersion();
			exit(EXIT_SUCCESS);
			break;

		case 'w': /* UTF-{8,16,32} output */
			if (cp[0] == '8') {
				cp++;
				if (cp[0] == '0') {
					cp++;
					outputEncoding = Util::NKFEncFromIndex(UTF_8N);
				} else {
					flagPool->output_bom_f = TRUE;
					outputEncoding = Util::NKFEncFromIndex(UTF_8_BOM);
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
					outputEncoding = Util::NKFEncFromIndex(UTF_8);
					continue;
				}
				if (cp[0] == 'L') {
					cp++;
					outputEndian = ENDIAN_LITTLE;
					flagPool->output_bom_f = TRUE;
				} else if (cp[0] == 'B') {
					cp++;
					flagPool->output_bom_f = TRUE;
				}
				if (cp[0] == '0') {
					flagPool->output_bom_f = FALSE;
					cp++;
					enc_idx =
							enc_idx == UTF_16 ?
									(outputEndian == ENDIAN_LITTLE ?
											UTF_16LE : UTF_16BE) :
									(outputEndian == ENDIAN_LITTLE ?
											UTF_32LE : UTF_32BE);
				} else {
					enc_idx =
							enc_idx == UTF_16 ?
									(outputEndian == ENDIAN_LITTLE ?
											UTF_16LE_BOM : UTF_16BE_BOM) :
									(outputEndian == ENDIAN_LITTLE ?
											UTF_32LE_BOM : UTF_32BE_BOM);
				}
				outputEncoding = Util::NKFEncFromIndex(enc_idx);
			}
			continue;

		case 'W': /* UTF input */
			if (cp[0] == '8') {
				cp++;
				inputEncoding = Util::NKFEncFromIndex(UTF_8);
			} else {
				int enc_idx;
				if ('1' == cp[0] && '6' == cp[1]) {
					cp += 2;
					inputEndian = ENDIAN_BIG;
					enc_idx = UTF_16;
				} else if ('3' == cp[0] && '2' == cp[1]) {
					cp += 2;
					inputEndian = ENDIAN_BIG;
					enc_idx = UTF_32;
				} else {
					inputEncoding = Util::NKFEncFromIndex(UTF_8);
					continue;
				}
				if (cp[0] == 'L') {
					cp++;
					inputEndian = ENDIAN_LITTLE;
				} else if (cp[0] == 'B') {
					cp++;
					inputEndian = ENDIAN_BIG;
				}
				enc_idx = (
						enc_idx == UTF_16 ?
								(inputEndian == ENDIAN_LITTLE ?
										UTF_16LE : UTF_16BE) :
								(inputEndian == ENDIAN_LITTLE ?
										UTF_32LE : UTF_32BE));
				inputEncoding = Util::NKFEncFromIndex(enc_idx);
			}
			continue;

			/* Input code assumption */
		case 'J': /* ISO-2022-JP input */
			inputEncoding = Util::NKFEncFromIndex(ISO_2022_JP);
			continue;
		case 'E': /* EUC-JP input */
			inputEncoding = Util::NKFEncFromIndex(EUCJP_NKF);
			continue;
		case 'S': /* Shift_JIS input */
			inputEncoding = Util::NKFEncFromIndex(SHIFT_JIS);
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
				flagPool->alpha_f |= 1 << (*cp++ - '0');
			}
			flagPool->alpha_f |= 1;
			continue;
		case 'x': /* Convert X0201 kana to X0208 or X0201 Conversion */
			flagPool->x0201_f = FALSE; /* No X0201->X0208 conversion */
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
			flagPool->x0201_f = TRUE;
			continue;
		case 'F': /* prserve new lines */
			flagPool->fold_preserve_f = TRUE;
		case 'f': /* folding -f60 or -f */
			flagPool->fold_f = TRUE;
			flagPool->fold_len = 0;
			while ('0' <= *cp && *cp <= '9') { /* we don't use atoi here */
				flagPool->fold_len *= 10;
				flagPool->fold_len += *cp++ - '0';
			}
			if (!(0 < flagPool->fold_len && flagPool->fold_len < BUFSIZ))
				flagPool->fold_len = DEFAULT_FOLD;
			if (*cp == '-') {
				flagPool->fold_margin = 0;
				cp++;
				while ('0' <= *cp && *cp <= '9') { /* we don't use atoi here */
					flagPool->fold_margin *= 10;
					flagPool->fold_margin += *cp++ - '0';
				}
			}
			continue;
		case 'm': /* MIME support */
			/* mime_decode_f = TRUE; *//* this has too large side effects... */
			if (*cp == 'B' || *cp == 'Q') {
				mime_decode_mode = *cp++;
				flagPool->mimebuf_f = FIXED_MIME;
			} else if (*cp == 'N') {
				flagPool->mime_f = TRUE;
				cp++;
			} else if (*cp == 'S') {
				flagPool->mime_f = STRICT_MIME;
				cp++;
			} else if (*cp == '0') {
				flagPool->mime_decode_f = FALSE;
				flagPool->mime_f = FALSE;
				cp++;
			} else {
				flagPool->mime_f = STRICT_MIME;
			}
			continue;
		case 'M': /* MIME output */
			if (*cp == 'B') {
				mimeout_mode = 'B';
				flagPool->mimeout_f = FIXED_MIME;
				cp++;
			} else if (*cp == 'Q') {
				mimeout_mode = 'Q';
				flagPool->mimeout_f = FIXED_MIME;
				cp++;
			} else {
				flagPool->mimeout_f = TRUE;
			}
			continue;
		case 'B': /* Broken JIS support */
			/*  bit:0   no ESC JIS
			 bit:1   allow any x on ESC-(-x or ESC-$-x
			 bit:2   reset to ascii on NL
			 */
			if ('9' >= *cp && *cp >= '0')
				flagPool->broken_f |= 1 << (*cp++ - '0');
			else
				flagPool->broken_f |= TRUE;
			continue;

		case 'O':/* for Output file */
			flagPool->file_out_f = TRUE;
			continue;

		case 'c':/* add cr code */
			flagPool->eolmode_f = CRLF;
			continue;
		case 'd':/* delete cr code */
			flagPool->eolmode_f = LF;
			continue;
		case 'I': /* ISO-2022-JP output */
			flagPool->iso2022jp_f = TRUE;
			continue;
		case 'L': /* line mode */
			if (*cp == 'u') { /* unix */
				flagPool->eolmode_f = LF;
				cp++;
			} else if (*cp == 'm') { /* mac */
				flagPool->eolmode_f = CR;
				cp++;
			} else if (*cp == 'w') { /* windows */
				flagPool->eolmode_f = CRLF;
				cp++;
			} else if (*cp == '0') { /* no conversion  */
				flagPool->eolmode_f = 0;
				cp++;
			}
			continue;

		case 'g':
			if ('2' <= *cp && *cp <= '9') {
				flagPool->guess_f = 2;
				cp++;
			} else if (*cp == '0' || *cp == '1') {
				flagPool->guess_f = 1;
				cp++;
			} else {
				flagPool->guess_f = 1;
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
 * 文字コードを変換する処理のラッパーで、外部に見せるメソッド
 */
int LibNKF::Convert(const std::wstring src, std::wstring dst) {
	return 0;
}
/**
 * このクラスの主要メソッド：
 * ファイルポインタを引数に文字コードを変換する
 */
int LibNKF::KanjiConvert(FILE* f) {
	nkf_char c1 = 0, c2 = 0, c3 = 0, c4 = 0;
	int shift_mode = 0; /* 0, 1, 2, 3 */
	int g2 = 0;
	int is_8bit = FALSE;

	//if (inputEncoding && !nkf_enc_asciicompat(inputEncoding)) {
	//	is_8bit = TRUE;
	//}

	inputMode = ASCII;
	outputMode = ASCII;

	// 設定されたフラグから使用するクラスとメソッドを決定する
	if (ModuleConnection() < 0) {
		fprintf(stderr, "no output encoding given\n");
		return -1;
	}
	// BOMのチェック
	CheckBom(f);

	/**
	 * 入力がUTF-16の場合ここで処理を行う
	 */
	if (inputEncoding->baseEncoding->iconvName == "w_iconv32") {
		// 4byte必要なUTF-16だった場合の処理
		while ((c1 = LibNKF::StdGetC(f)) != EOF
				&& (c2 = LibNKF::StdGetC(f)) != EOF
				&& (c3 = LibNKF::StdGetC(f)) != EOF
				&& (c4 = LibNKF::StdGetC(f)) != EOF) {
			oConvStr.push_back(UTF16Util::NKFIconvUTF16(c1, c2, c3, c4));
		}
		goto finished;
	} else if (inputEncoding->baseEncoding->iconvName == "w_iconv16") {
		// 2byte必要なUTF-16だった場合の処理
		while ((c1 = LibNKF::StdGetC(f)) != EOF
				&& (c2 = LibNKF::StdGetC(f)) != EOF) {
			if (UTF16Util::NKFIconvUTF16(c1, c2, 0,
					0) == NKF_ICONV_NEED_TWO_MORE_BYTES && (c3 = LibNKF::StdGetC(f)) != EOF
					&& (c4 = LibNKF::StdGetC(f)) != EOF) {oConvStr.push_back(UTF16Util::NKFIconvUTF16(c1, c2, c3, c4));
		}
	}
	goto finished;
}

	while ((c1 = LibNKF::StdGetC(f)) != EOF) {
		//if (!inputEncoding)
		//	code_status(c1);
		if (c2) {
			/* second byte */
			if (c2 > DEL) {
				/* in case of 8th bit is on */
				if (!flagPool->estab_f && !mime_decode_mode) {
					/**
					 * まだ文字コードが確定しておらず曖昧な状態
					 */
					GuessConv* gIConv = new GuessConv();
					if (gIConv->GuessIConv(f, c2, c1, flagPool, inputEncoding, outputEncoding) == EOF) {
						delete gIConv;
						LAST;
					} else {
						delete gIConv;
						SKIP
						;
					}
				} else {
					/* in case of already established */
					if (c1 < 0x40) {
						/* ignore bogus code */
						SKIP
						;
					} else {
						SEND;
					}
				}
			} else {
				/* 2nd byte of 7 bit code or SJIS */
				SEND;
			}
		} else if (nkf_char_unicode_p(c1)) {
			outputEncoding->baseEncoding->Oconv(0, c1, flagPool);
			NEXT;
		} else {
			/* first byte */
			if (inputMode == JIS_X_0208 && DEL <= c1 && c1 < 0x92) {
				/* CP5022x */
				MORE
				;
			} else if (!inputCodeName.empty() && inputCodeName[0] == 'I'
					&& 0xA1 <= c1 && c1 <= 0xDF) {
				/* JIS X 0201 Katakana in 8bit JIS */
				c2 = JIS_X_0201_1976_K;
				c1 &= 0x7f;
				SEND;
			} else if (c1 > DEL) {
				/* 8 bit code */
				if (!flagPool->estab_f && !flagPool->iso8859_f) {
					/* not established yet */
					MORE
					;
				} else { /* estab_f==TRUE */
					if (flagPool->iso8859_f) {
						c2 = ISO_8859_1;
						c1 &= 0x7f;
						SEND;
					} else if ((inputEncoding->baseEncoding->iconvName
							== "s_iconv" && 0xA0 <= c1 && c1 <= 0xDF)
							|| (flagPool->ms_ucs_map_f == UCS_MAP_CP10001
									&& (c1 == 0xFD || c1 == 0xFE))) {
						/* JIS X 0201 */
						c2 = JIS_X_0201_1976_K;
						c1 &= 0x7f;
						SEND;
					} else {
						/* already established */
						MORE
						;
					}
				}
			} else if (SP < c1 && c1 < DEL) {
				/* in case of Roman characters */
				if (shift_mode) {
					/* output 1 shifted byte */
					if (flagPool->iso8859_f) {
						c2 = ISO_8859_1;
						SEND;
					} else if (nkf_byte_jisx0201_katakana_p(c1)) {
						/* output 1 shifted byte */
						c2 = JIS_X_0201_1976_K;
						SEND;
					} else {
						/* look like bogus code */
						SKIP
						;
					}
				} else if (inputMode == JIS_X_0208 || inputMode == JIS_X_0212
						|| inputMode == JIS_X_0213_1
						|| inputMode == JIS_X_0213_2) {
					/* in case of Kanji shifted */
					MORE
					;
				} else if (c1 == '=' && flagPool->mime_f && !mime_decode_mode) {
					/* Check MIME code */
					if ((c1 = LibNKF::StdGetC(f)) == EOF) {
						outputEncoding->baseEncoding->Oconv(0, '=', flagPool);
						LAST;
					} else if (c1 == '?') {
						/* =? is mime conversion start sequence */
						if (flagPool->mime_f == STRICT_MIME) {
							/* check in real detail */
							if (MimeBeginStrict(f) == EOF)
								LAST;
							SKIP
							;
						} else if (MimeBegin(f) == EOF)
							LAST;
						SKIP
						;
					} else {
						outputEncoding->baseEncoding->Oconv(0, '=', flagPool);
						LibNKF::StdUnGetC(c1, f);
						SKIP
						;
					}
				} else {
					/* normal ASCII code */
					SEND;
				}
			} else if (c1 == SI && (!is_8bit || mime_decode_mode)) {
				shift_mode = 0;
				SKIP
				;
			} else if (c1 == SO && (!is_8bit || mime_decode_mode)) {
				shift_mode = 1;
				SKIP
				;
			} else if (c1 == ESC && (!is_8bit || mime_decode_mode)) {
				if ((c1 = LibNKF::StdGetC(f)) == EOF) {
					outputEncoding->baseEncoding->Oconv(0, ESC, flagPool);
					LAST;
				} else if (c1 == '&') {
					/* IRR */
					if ((c1 = LibNKF::StdGetC(f)) == EOF) {
						LAST;
					} else {
						SKIP
						;
					}
				} else if (c1 == '$') {
					/* GZDMx */
					if ((c1 = LibNKF::StdGetC(f)) == EOF) {
						/* don't send bogus code
						 (*oconv)(0, ESC);
						 (*oconv)(0, '$'); */
						LAST;
					} else if (c1 == '@' || c1 == 'B') {
						/* JIS X 0208 */
						SetInputMode(JIS_X_0208);
						SKIP
						;
					} else if (c1 == '(') {
						/* GZDM4 */
						if ((c1 = LibNKF::StdGetC(f)) == EOF) {
							/* don't send bogus code
							 (*oconv)(0, ESC);
							 (*oconv)(0, '$');
							 (*oconv)(0, '(');
							 */
							LAST;
						} else if (c1 == '@' || c1 == 'B') {
							/* JIS X 0208 */
							SetInputMode(JIS_X_0208);
							SKIP
							;
						} else if (c1 == 'D') {
							SetInputMode(JIS_X_0212);
							SKIP
							;
						} else if (c1 == 'O' || c1 == 'Q') {
							SetInputMode(JIS_X_0213_1);
							SKIP
							;
						} else if (c1 == 'P') {
							SetInputMode(JIS_X_0213_2);
							SKIP
							;
						} else {
							/* could be some special code */
							outputEncoding->baseEncoding->Oconv(0, ESC, flagPool);
							outputEncoding->baseEncoding->Oconv(0, '$', flagPool);
							outputEncoding->baseEncoding->Oconv(0, '(', flagPool);
							outputEncoding->baseEncoding->Oconv(0, c1, flagPool);
							SKIP
							;
						}
					} else if (flagPool->broken_f & 0x2) {
						/* accept any ESC-(-x as broken code ... */
						inputMode = JIS_X_0208;
						shift_mode = 0;
						SKIP
						;
					} else {
						outputEncoding->baseEncoding->Oconv(0, ESC, flagPool);
						outputEncoding->baseEncoding->Oconv(0, '$', flagPool);
						outputEncoding->baseEncoding->Oconv(0, c1, flagPool);
						SKIP
						;
					}
				} else if (c1 == '(') {
					/* GZD4 */
					if ((c1 = LibNKF::StdGetC(f)) == EOF) {
						/* don't send bogus code
						 (*oconv)(0, ESC);
						 (*oconv)(0, '('); */
						LAST;
					} else if (c1 == 'I') {
						/* JIS X 0201 Katakana */
						SetInputMode(JIS_X_0201_1976_K);
						SKIP
						;
					} else if (c1 == 'B' || c1 == 'J' || c1 == 'H') {
						/* ISO-646IRV:1983 or JIS X 0201 Roman or JUNET */
						SetInputMode(ASCII);
						SKIP
						;
					} else if (flagPool->broken_f & 0x2) {
						SetInputMode(ASCII);
						SKIP
						;
					} else {
						outputEncoding->baseEncoding->Oconv(0, ESC, flagPool);
						outputEncoding->baseEncoding->Oconv(0, '(', flagPool);
						SEND;
					}
				} else if (c1 == '.') {
					/* G2D6 */
					if ((c1 = LibNKF::StdGetC(f)) == EOF) {
						LAST;
					} else if (c1 == 'A') {
						/* ISO-8859-1 */
						g2 = ISO_8859_1;
						SKIP
						;
					} else {
						outputEncoding->baseEncoding->Oconv(0, ESC, flagPool);
						outputEncoding->baseEncoding->Oconv(0, '.', flagPool);
						SEND;
					}
				} else if (c1 == 'N') {
					/* SS2 */
					c1 = LibNKF::StdGetC(f);
					if (g2 == ISO_8859_1) {
						c2 = ISO_8859_1;
						SEND;
					} else {
						LibNKF::StdUnGetC(c1, f);
						/* lonely ESC  */
						outputEncoding->baseEncoding->Oconv(0, ESC, flagPool);
						SEND;
					}
				} else {
					/* lonely ESC  */
					outputEncoding->baseEncoding->Oconv(0, ESC, flagPool);
					SEND;
				}
			} else if (c1 == ESC
					&& inputEncoding->baseEncoding->iconvName == "s_iconv") {
				/* ESC in Shift_JIS */
				if ((c1 = LibNKF::StdGetC(f)) == EOF) {
					outputEncoding->baseEncoding->Oconv(0, ESC, flagPool);
					LAST;
				} else if (c1 == '$') {
					/* J-PHONE emoji */
					if ((c1 = LibNKF::StdGetC(f)) == EOF) {
						LAST;
					} else if (('E' <= c1 && c1 <= 'G')
							|| ('O' <= c1 && c1 <= 'Q')) {
						/*
						 NUM : 0 1 2 3 4 5
						 BYTE: G E F O P Q
						 C%7 : 1 6 0 2 3 4
						 C%7 : 0 1 2 3 4 5 6
						 NUM : 2 0 3 4 5 X 1
						 */
						static const nkf_char jphone_emoji_first_table[7] = {
								0xE1E0, 0xDFE0, 0xE2E0, 0xE3E0, 0xE4E0, 0xDFE0,
								0xE0E0 };
						c3 =
								nkf_char_unicode_new(jphone_emoji_first_table[c1 % 7]);
						if ((c1 = LibNKF::StdGetC(f)) == EOF)
							LAST;
						while (SP <= c1 && c1 <= 'z') {
							outputEncoding->baseEncoding->Oconv(0, c1 + c3, flagPool);
							if ((c1 = LibNKF::StdGetC(f)) == EOF)
								LAST;
						}
						SKIP
						;
					} else {
						outputEncoding->baseEncoding->Oconv(0, ESC, flagPool);
						outputEncoding->baseEncoding->Oconv(0, '$', flagPool);
						SEND;
					}
				} else {
					/* lonely ESC  */
					outputEncoding->baseEncoding->Oconv(0, ESC, flagPool);
					SEND;
				}
			} else if (c1 == LF || c1 == CR) {
				if (flagPool->broken_f & 4) {
					inputMode = ASCII;
					SetIconv(FALSE, 0, flagPool);
					SEND;
				} else if (flagPool->mime_decode_f && !mime_decode_mode) {
					if (c1 == LF) {
						if ((c1 = LibNKF::StdGetC(f)) != EOF && c1 == SP) {
							LibNKF::StdUnGetC(SP, f);
							continue;
						} else {
							LibNKF::StdUnGetC(c1, f);
						}
						c1 = LF;
						SEND;
					} else { /* if (c1 == CR)*/
						if ((c1 = LibNKF::StdGetC(f)) != EOF) {
							if (c1 == SP) {
								LibNKF::StdUnGetC(SP, f);
								continue;
							} else if (c1 == LF
									&& (c1 = LibNKF::StdGetC(f)) != EOF
									&& c1 == SP) {
								LibNKF::StdUnGetC(SP, f);
								continue;
							} else {
								LibNKF::StdUnGetC(c1, f);
							}
							LibNKF::StdUnGetC(LF, f);
						} else {
							LibNKF::StdUnGetC(c1, f);
						}
						c1 = CR;
						SEND;
					}
				}
			} else
				SEND;
		}
		/* send: */
		switch (inputMode) {
		case ASCII:
			switch (inputEncoding->baseEncoding->Iconv(c2, c1, 0, flagPool)) { /* can be EUC / SJIS / UTF-8 */
			case -2:
				/* 4 bytes UTF-8 */
				if ((c3 = LibNKF::StdGetC(f)) != EOF) {
					GuessConv::CodeStatus(c3, flagPool);
					c3 <<= 8;
					if ((c4 = LibNKF::StdGetC(f)) != EOF) {
						GuessConv::CodeStatus(c4, flagPool);
						inputEncoding->baseEncoding->Iconv(c2, c1, c3 | c4, flagPool);
					}
				}
				break;
			case -1:
				/* 3 bytes EUC or UTF-8 */
				if ((c3 = LibNKF::StdGetC(f)) != EOF) {
					GuessConv::CodeStatus(c3, flagPool);
					inputEncoding->baseEncoding->Iconv(c2, c1, c3, flagPool);
				}
				break;
			}
			break;
		case JIS_X_0208:
		case JIS_X_0213_1:
			if (flagPool->ms_ucs_map_f && 0x7F <= c2 && c2 <= 0x92 && 0x21 <= c1
					&& c1 <= 0x7E) {
				/* CP932 UDC */
				c1 = nkf_char_unicode_new((c2 - 0x7F) * 94 + c1 - 0x21 + 0xE000);
				c2 = 0;
			}
			outputEncoding->baseEncoding->Oconv(c2, c1, flagPool); /* this is JIS, not SJIS/EUC case */
			break;
		case JIS_X_0212:
			outputEncoding->baseEncoding->Oconv(PREFIX_EUCG3 | c2, c1, flagPool);
			break;
		case JIS_X_0213_2:
			outputEncoding->baseEncoding->Oconv(PREFIX_EUCG3 | c2, c1, flagPool);
			break;
		default:
			outputEncoding->baseEncoding->Oconv(inputMode, c1, flagPool); /* other special case */
		}

		c2 = 0;
		c3 = 0;
		continue;
		/* goto next_word */
	}

	finished:
	/* epilogue */
	inputEncoding->baseEncoding->Iconv(EOF, 0, 0, flagPool);
//	if (!input_codename) {
//		if (is_8bit) {
//			struct input_code *p = input_code_list;
//			struct input_code *result = p;
//			while (p->name) {
//				if (p->score < result->score)
//					result = p;
//				++p;
//			}
//			set_input_codename(result->name);
//			debug(result->name);
//		}
//	}
	return 0;
}
/**
 * 設定されたフラグから文字コード変換に使うメソッドを決める
 */
int LibNKF::ModuleConnection() {
	if (inputEncoding)
		SetInputEncoding(inputEncoding);

	if (!outputEncoding) {
		outputEncoding = Util::NKFDefaultEncoding();
	}
	if (!outputEncoding) {
		if (flagPool->noout_f || flagPool->guess_f) {
			outputEncoding = Util::NKFEncFromIndex(ISO_2022_JP);
		} else {
			return -1;
		}
	}
	LibNKF::SetOutputEncoding(outputEncoding);
	// oconv = nkf_enc_to_oconv(outputEncoding);
	// oconvはoutputEncoding->Oconv(); を使う
	// o_putc = std_putc;
	// o_putcは
	if (outputEncoding->baseEncoding->id == UTF_8) {
		// baseEncodingがUTF-8だった場合出力モードはUTF-8
		outputMode = UTF_8;
	}

	if (flagPool->x0201_f == NKF_UNSPECIFIED) {
		flagPool->x0201_f = X0201_DEFAULT;
	}

	/* replace continucation module, from output side */

	/* output redicrection */
//	if (flagPool->noout_f || flagPool->guess_f) {
//		o_putc = no_putc;
//	}
//	if (flagPool->mimeout_f) {
//		o_mputc = o_putc;
//		o_putc = mime_putc;
//		if (flagPool->mimeout_f == TRUE) {
//			o_base64conv = oconv;
//			oconv = base64_conv;
//		}
//		/* base64_count = 0; */
//	}
//	if (flagPool->eolmode_f || flagPool->guess_f) {
//		o_eol_conv = oconv;
//		oconv = eol_conv;
//	}
//	if (flagPool->rot_f) {
//		o_rot_conv = oconv;
//		oconv = rot_conv;
//	}
//	if (flagPool->iso2022jp_f) {
//		o_iso2022jp_check_conv = oconv;
//		oconv = iso2022jp_check_conv;
//	}
//	if (flagPool->hira_f) {
//		o_hira_conv = oconv;
//		oconv = hira_conv;
//	}
//	if (flagPool->fold_f) {
//		o_fconv = oconv;
//		oconv = fold_conv;
//		f_line = 0;
//	}
//	if (flagPool->alpha_f || flagPool->x0201_f) {
//		o_zconv = oconv;
//		oconv = z_conv;
//	}
//	i_getc = std_getc;
//	i_ungetc = std_ungetc;
	/* input redicrection */
//	if (flagPool->cap_f) {
//		i_cgetc = i_getc;
//		i_getc = cap_getc;
//		i_cungetc = i_ungetc;
//		i_ungetc = cap_ungetc;
//	}
//	if (flagPool->url_f) {
//		i_ugetc = i_getc;
//		i_getc = url_getc;
//		i_uungetc = i_ungetc;
//		i_ungetc = url_ungetc;
//	}
//	if (flagPool->numchar_f) {
//		i_ngetc = i_getc;
//		i_getc = numchar_getc;
//		i_nungetc = i_ungetc;
//		i_ungetc = numchar_ungetc;
//	}
//	if (flagPool->nfc_f) {
//		i_nfc_getc = i_getc;
//		i_getc = nfc_getc;
//		i_nfc_ungetc = i_ungetc;
//		i_ungetc = nfc_ungetc;
//	}
//	if (flagPool->mime_f && flagPool->mimebuf_f == FIXED_MIME) {
//		i_mgetc = i_getc;
//		i_getc = mime_getc;
//		i_mungetc = i_ungetc;
//		i_ungetc = mime_ungetc;
//	}
//	if (flagPool->broken_f & 1) {
//		i_bgetc = i_getc;
//		i_getc = broken_getc;
//		i_bungetc = i_ungetc;
//		i_ungetc = broken_ungetc;
//	}
//	if (inputEncoding) {
//		set_iconv(-TRUE, nkf_enc_to_iconv(input_encoding));
//	} else {
//		set_iconv(FALSE, e_iconv);
//	}
//	{
//		struct input_code *p = input_code_list;
//		while (p->name) {
//			status_reinit(p++);
//		}
//	}
	return 0;
}

/**
 * 入力された文字コードからフラグを設定する
 */
void LibNKF::SetInputEncoding(NKFEncoding *enc) {
	switch (enc->id) {
	case ISO_8859_1:
		flagPool->iso8859_f = TRUE;
		break;
	case CP50221:
	case CP50222:
		if (flagPool->x0201_f == NKF_UNSPECIFIED)
			flagPool->x0201_f = FALSE; /* -x specified implicitly */
	case CP50220:
		flagPool->cp51932_f = TRUE;
		flagPool->ms_ucs_map_f = UCS_MAP_CP932;
		break;
	case ISO_2022_JP_1:
		flagPool->x0212_f = TRUE;
		break;
	case ISO_2022_JP_3:
		flagPool->x0212_f = TRUE;
		flagPool->x0213_f = TRUE;
		break;
	case ISO_2022_JP_2004:
		flagPool->x0212_f = TRUE;
		flagPool->x0213_f = TRUE;
		break;
	case SHIFT_JIS:
		break;
	case WINDOWS_31J:
		if (flagPool->x0201_f == NKF_UNSPECIFIED)
			flagPool->x0201_f = FALSE; /* -x specified implicitly */
		flagPool->cp51932_f = TRUE;
		flagPool->ms_ucs_map_f = UCS_MAP_CP932;
		break;
		break;
	case CP10001:
		flagPool->cp51932_f = TRUE;
		flagPool->ms_ucs_map_f = UCS_MAP_CP10001;
		break;
	case EUC_JP:
		break;
	case EUCJP_NKF:
		break;
	case CP51932:
		if (flagPool->x0201_f == NKF_UNSPECIFIED)
			flagPool->x0201_f = FALSE; /* -x specified implicitly */
		flagPool->cp51932_f = TRUE;
		flagPool->ms_ucs_map_f = UCS_MAP_CP932;
		break;
	case EUCJP_MS:
		if (flagPool->x0201_f == NKF_UNSPECIFIED)
			flagPool->x0201_f = FALSE; /* -x specified implicitly */
		flagPool->cp51932_f = FALSE;
		flagPool->ms_ucs_map_f = UCS_MAP_MS;
		break;
	case EUCJP_ASCII:
		if (flagPool->x0201_f == NKF_UNSPECIFIED)
			flagPool->x0201_f = FALSE; /* -x specified implicitly */
		flagPool->cp51932_f = FALSE;
		flagPool->ms_ucs_map_f = UCS_MAP_ASCII;
		break;
	case SHIFT_JISX0213:
	case SHIFT_JIS_2004:
		flagPool->x0213_f = TRUE;
		flagPool->cp51932_f = FALSE;
		break;
	case EUC_JISX0213:
	case EUC_JIS_2004:
		flagPool->x0213_f = TRUE;
		flagPool->cp51932_f = FALSE;
		break;
	case UTF8_MAC:
		flagPool->nfc_f = TRUE;
		break;
	case UTF_16:
	case UTF_16BE:
	case UTF_16BE_BOM:
		inputEndian = ENDIAN_BIG;
		break;
	case UTF_16LE:
	case UTF_16LE_BOM:
		inputEndian = ENDIAN_LITTLE;
		break;
	case UTF_32:
	case UTF_32BE:
	case UTF_32BE_BOM:
		inputEndian = ENDIAN_BIG;
		break;
	case UTF_32LE:
	case UTF_32LE_BOM:
		inputEndian = ENDIAN_LITTLE;
		break;
	}
}
/**
 * 入力する文字コードとその処理を決定する
 * inputEncoding, inputCodeNameの決定を行う
 */
void LibNKF::SetIconv(nkf_char f, std::string name, FlagPool* flagPool) {
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
	if (name.empty() && (f == -TRUE || !inputEncoding->name.empty())) {
		inputEncoding->name = name;
	}
	if (flagPool->estab_f && iconvForCheck != name) {
		// 入力文字コードが確定している場合
		inputCodeName = name;
		iconvForCheck = name;
	}
}
/**
 * 出力する文字コード用にフラグを設定する
 */
void LibNKF::SetOutputEncoding(NKFEncoding *enc) {

	switch (enc->id) {
	case CP50220:
		if (flagPool->cp932inv_f == TRUE)
			flagPool->cp932inv_f = FALSE;
		flagPool->ms_ucs_map_f = UCS_MAP_CP932;
		break;
	case CP50221:
		if (flagPool->x0201_f == NKF_UNSPECIFIED)
			flagPool->x0201_f = FALSE; /* -x specified implicitly */
		if (flagPool->cp932inv_f == TRUE)
			flagPool->cp932inv_f = FALSE;
		flagPool->ms_ucs_map_f = UCS_MAP_CP932;
		break;
	case ISO_2022_JP:
		if (flagPool->cp932inv_f == TRUE)
			flagPool->cp932inv_f = FALSE;
		break;
	case ISO_2022_JP_1:
		flagPool->x0212_f = TRUE;
		if (flagPool->cp932inv_f == TRUE)
			flagPool->cp932inv_f = FALSE;
		break;
	case ISO_2022_JP_3:
		flagPool->x0212_f = TRUE;
		flagPool->x0213_f = TRUE;
		if (flagPool->cp932inv_f == TRUE)
			flagPool->cp932inv_f = FALSE;
		break;
	case SHIFT_JIS:
		break;
	case WINDOWS_31J:
		if (flagPool->x0201_f == NKF_UNSPECIFIED)
			flagPool->x0201_f = FALSE; /* -x specified implicitly */
		flagPool->ms_ucs_map_f = UCS_MAP_CP932;
		break;
	case CP10001:
		flagPool->ms_ucs_map_f = UCS_MAP_CP10001;
		break;
	case EUC_JP:
		flagPool->x0212_f = TRUE;
		if (flagPool->cp932inv_f == TRUE)
			flagPool->cp932inv_f = FALSE;
		flagPool->ms_ucs_map_f = UCS_MAP_ASCII;
		break;
	case EUCJP_NKF:
		flagPool->x0212_f = FALSE;
		if (flagPool->cp932inv_f == TRUE)
			flagPool->cp932inv_f = FALSE;
		flagPool->ms_ucs_map_f = UCS_MAP_ASCII;
		break;
	case CP51932:
		if (flagPool->x0201_f == NKF_UNSPECIFIED)
			flagPool->x0201_f = FALSE; /* -x specified implicitly */
		if (flagPool->cp932inv_f == TRUE)
			flagPool->cp932inv_f = FALSE;
		flagPool->ms_ucs_map_f = UCS_MAP_CP932;
		break;
	case EUCJP_MS:
		if (flagPool->x0201_f == NKF_UNSPECIFIED)
			flagPool->x0201_f = FALSE; /* -x specified implicitly */
		flagPool->x0212_f = TRUE;
		flagPool->ms_ucs_map_f = UCS_MAP_MS;
		break;
	case EUCJP_ASCII:
		if (flagPool->x0201_f == NKF_UNSPECIFIED)
			flagPool->x0201_f = FALSE; /* -x specified implicitly */
		flagPool->x0212_f = TRUE;
		flagPool->ms_ucs_map_f = UCS_MAP_ASCII;
		break;
	case SHIFT_JISX0213:
	case SHIFT_JIS_2004:
		flagPool->x0213_f = TRUE;
		if (flagPool->cp932inv_f == TRUE)
			flagPool->cp932inv_f = FALSE;
		break;
	case EUC_JISX0213:
	case EUC_JIS_2004:
		flagPool->x0212_f = TRUE;
		flagPool->x0213_f = TRUE;
		if (flagPool->cp932inv_f == TRUE)
			flagPool->cp932inv_f = FALSE;
		break;
	case UTF_8_BOM:
		flagPool->output_bom_f = TRUE;
		break;
	case UTF_16:
	case UTF_16BE_BOM:
		flagPool->output_bom_f = TRUE;
		break;
	case UTF_16LE:
		outputEndian = ENDIAN_LITTLE;
		flagPool->output_bom_f = FALSE;
		break;
	case UTF_16LE_BOM:
		outputEndian = ENDIAN_LITTLE;
		flagPool->output_bom_f = TRUE;
		break;
	case UTF_32:
	case UTF_32BE_BOM:
		flagPool->output_bom_f = TRUE;
		break;
	case UTF_32LE:
		outputEndian = ENDIAN_LITTLE;
		flagPool->output_bom_f = FALSE;
		break;
	case UTF_32LE_BOM:
		outputEndian = ENDIAN_LITTLE;
		flagPool->output_bom_f = TRUE;
		break;
	}
}
/**
 * BOMが存在するかチェックし、存在すれば無視する
 */
void LibNKF::CheckBom(FILE *f) {
	int c2;
	switch (c2 = LibNKF::StdGetC(f)) {
	case 0x00:
		if ((c2 = LibNKF::StdGetC(f)) == 0x00) {
			if ((c2 = LibNKF::StdGetC(f)) == 0xFE) {
				if ((c2 = LibNKF::StdGetC(f)) == 0xFF) {
					if (!inputEncoding) {
						inputEncoding->baseEncoding->iconvName = "w_iconv32";
					}
					if (inputEncoding->baseEncoding->iconvName == "w_iconv32") {
						inputEndian = ENDIAN_BIG;
						return;
					}
					LibNKF::StdUnGetC(0xFF, f);
				} else {
					LibNKF::StdUnGetC(c2, f);
				}
				LibNKF::StdUnGetC(0xFE, f);
			} else if (c2 == 0xFF) {
				if ((c2 = LibNKF::StdGetC(f)) == 0xFE) {
					if (!inputEncoding) {
						inputEncoding->baseEncoding->iconvName = "w_iconv32";
					}
					if (inputEncoding->baseEncoding->iconvName == "w_iconv32") {
						inputEndian = ENDIAN_2143;
						return;
					}
					LibNKF::StdUnGetC(0xFF, f);
				} else {
					LibNKF::StdUnGetC(c2, f);
				}
				LibNKF::StdUnGetC(0xFF, f);
			} else {
				LibNKF::StdUnGetC(c2, f);
			}
			LibNKF::StdUnGetC(0x00, f);
		} else {
			LibNKF::StdUnGetC(c2, f);
		}
		LibNKF::StdUnGetC(0x00, f);
		break;
	case 0xEF:
		if ((c2 = LibNKF::StdGetC(f)) == 0xBB) {
			if ((c2 = LibNKF::StdGetC(f)) == 0xBF) {
				if (!inputEncoding) {
					inputEncoding->baseEncoding->iconvName = "w_iconv";
				}
				if (inputEncoding->baseEncoding->iconvName == "w_iconv") {
					return;
				}
				LibNKF::StdUnGetC(0xBF, f);
			} else
				LibNKF::StdUnGetC(c2, f);
			LibNKF::StdUnGetC(0xBB, f);
		} else
			LibNKF::StdUnGetC(c2, f);
		LibNKF::StdUnGetC(0xEF, f);
		break;
	case 0xFE:
		if ((c2 = LibNKF::StdGetC(f)) == 0xFF) {
			if ((c2 = LibNKF::StdGetC(f)) == 0x00) {
				if ((c2 = LibNKF::StdGetC(f)) == 0x00) {
					if (!inputEncoding) {
						inputEncoding->baseEncoding->iconvName = "w_iconv32";
					}
					if (inputEncoding->baseEncoding->iconvName == "w_iconv32") {
						inputEndian = ENDIAN_3412;
						return;
					}
					LibNKF::StdUnGetC(0x00, f);
				} else
					LibNKF::StdUnGetC(c2, f);
				LibNKF::StdUnGetC(0x00, f);
			} else
				LibNKF::StdUnGetC(c2, f);
			if (!inputEncoding) {
				inputEncoding->baseEncoding->iconvName = "w_iconv16";
			}
			if (inputEncoding->baseEncoding->iconvName == "w_iconv16") {
				inputEndian = ENDIAN_BIG;
				return;
			}
			LibNKF::StdUnGetC(0xFF, f);
		} else
			LibNKF::StdUnGetC(c2, f);
		LibNKF::StdUnGetC(0xFE, f);
		break;
	case 0xFF:
		if ((c2 = LibNKF::StdGetC(f)) == 0xFE) {
			if ((c2 = LibNKF::StdGetC(f)) == 0x00) {
				if ((c2 = LibNKF::StdGetC(f)) == 0x00) {
					if (!inputEncoding) {
						inputEncoding->baseEncoding->iconvName = "w_iconv32";
					}
					if (inputEncoding->baseEncoding->iconvName == "w_iconv32") {
						inputEndian = ENDIAN_LITTLE;
						return;
					}
					LibNKF::StdUnGetC(0x00, f);
				} else
					LibNKF::StdUnGetC(c2, f);
				LibNKF::StdUnGetC(0x00, f);
			} else
				LibNKF::StdUnGetC(c2, f);
			if (!inputEncoding) {
				inputEncoding->baseEncoding->iconvName = "w_iconv16";
			}
			if (inputEncoding->baseEncoding->iconvName == "w_iconv16") {
				inputEndian = ENDIAN_LITTLE;
				return;
			}
			LibNKF::StdUnGetC(0xFE, f);
		} else
			LibNKF::StdUnGetC(c2, f);
		LibNKF::StdUnGetC(0xFF, f);
		break;
	default:
		LibNKF::StdUnGetC(c2, f);
		break;
	}
}
/**
 * 使い方の表示
 */
void LibNKF::ShowUsage() {
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
	LibNKF::ShowVersion();
}
/**
 * バージョン情報の表示
 */
void LibNKF::ShowVersion() {
	fprintf(HELP_OUTPUT,
			"Network Kanji Filter Version " NKF_VERSION " (" NKF_RELEASE_DATE ") \n" COPY_RIGHT "\n");
}
/**
 * 設定情報の表示
 */
void LibNKF::ShowConfiguration() {
	fprintf(HELP_OUTPUT,
			"Summary of my nkf " NKF_VERSION " (" NKF_RELEASE_DATE ") configuration:\n"
			"  Compile-time options:\n"
			"    Compiled at:                 " __DATE__ " " __TIME__ "\n");
	fprintf(HELP_OUTPUT, "    Default output encoding:     "
#ifdef DEFAULT_CODE_LOCALE
			"LOCALE (%s)\n", nkf_enc_name(nkf_default_encoding())
#elif defined(DEFAULT_ENCIDX)
			"CONFIG (%s)\n", nkf_enc_name(nkf_default_encoding())
#else
					"NONE\n"
#endif
			);
	fprintf(HELP_OUTPUT, "    Default output end of line:  "
#if DEFAULT_NEWLINE == CR
			"CR"
#elif DEFAULT_NEWLINE == CRLF
			"CRLF"
#else
					"LF"
#endif
			"\n"
			"    Decode MIME encoded string:  "
#if MIME_DECODE_DEFAULT
			"ON"
#else
			"OFF"
#endif
			"\n"
			"    Convert JIS X 0201 Katakana: "
#if X0201_DEFAULT
			"ON"
#else
			"OFF"
#endif
			"\n"
			"    --help, --version output:    "
#if HELP_OUTPUT_HELP_OUTPUT
			"HELP_OUTPUT"
#else
			"STDOUT"
#endif
			"\n");
}

#define MAXRECOVER 20

nkf_char LibNKF::MimeBeginStrict(FILE* f) {
	nkf_char c1 = 0;
	int i, j, k;
	const unsigned char *p, *q;
	nkf_char r[MAXRECOVER]; /* recovery buffer, max mime pattern length */

	mime_decode_mode = FALSE;
	/* =? has been checked */
	j = 0;
	p = mime_pattern[j];
	r[0] = '=';
	r[1] = '?';

	for (i = 2; p[i] > SP; i++) { /* start at =? */
		if (((r[i] = c1 = LibNKF::StdGetC(f)) == EOF)
				|| nkf_toupper(c1) != p[i]) {
			/* pattern fails, try next one */
			q = p;
			while (mime_pattern[++j]) {
				p = mime_pattern[j];
				for (k = 2; k < i; k++) /* assume length(p) > i */
					if (p[k] != q[k])
						break;
				if (k == i && nkf_toupper(c1) == p[k])
					break;
			}
			p = mime_pattern[j];
			if (p)
				continue; /* found next one, continue */
			/* all fails, output from recovery buffer */
			LibNKF::StdUnGetC(c1, f);
			for (j = 0; j < i; j++) {
				outputEncoding->baseEncoding->Oconv(0, r[j], flagPool);
			}
			return c1;
		}
	}
	mime_decode_mode = p[i - 2];
	mimeIconvBack = inputEncoding->baseEncoding->iconvName;

	// Mimeそれぞれに優先すべき関数名
	std::string mimePriorityFunc[] = { "e_iconv", "s_iconv", 0, 0, 0, 0, 0,
			"w_iconv", "w_iconv", 0 };
	SetIconv(FALSE, mimePriorityFunc[j], flagPool);
	InputCode::ClrCodeScore(InputCode::FindInputcodeByFunc(mimePriorityFunc[j]),
			SCORE_iMIME);

	if (mime_decode_mode == 'B') {
		flagPool->mimebuf_f = flagPool->unbuf_f;
		if (!flagPool->unbuf_f) {
			/* do MIME integrity check */
			return MimeIntegrity(f, mime_pattern[j]);
		}
	}
	SwitchMimeGetC();
	flagPool->mimebuf_f = TRUE;
	return c1;
}

nkf_char LibNKF::MimeBegin(FILE* f) {
	nkf_char c1;
	int i, k;

	/* In NONSTRICT mode, only =? is checked. In case of failure, we  */
	/* re-read and convert again from mime_buffer.  */

	/* =? has been checked */
	k = mime_input_state.last;
	mime_input_buf(mime_input_state.last++) = '=';
	mime_input_buf(mime_input_state.last++) = '?';
	for (i = 2; i < MAXRECOVER; i++) { /* start at =? */
		/* We accept any character type even if it is breaked by new lines */
		c1 = LibNKF::StdGetC(f);
		mime_input_buf(mime_input_state.last++) = (unsigned char) c1;
		if (c1 == LF || c1 == SP || c1 == CR
				|| c1 == '-'||c1=='_'||is_alnum(c1))continue
;
		if (c1 == '=') {
			/* Failed. But this could be another MIME preemble */
			LibNKF::StdUnGetC(c1, f);
			mime_input_state.last--;
			break;
		}
		if (c1 != '?')
			break;
		else {
			/* c1=='?' */
			c1 = LibNKF::StdGetC(f);
			mime_input_buf(mime_input_state.last++) = (unsigned char) c1;
			if (!(++i < MAXRECOVER) || c1 == EOF)
				break;
			if (c1 == 'b' || c1 == 'B') {
				mime_decode_mode = 'B';
			} else if (c1 == 'q' || c1 == 'Q') {
				mime_decode_mode = 'Q';
			} else {
				break;
			}
			c1 = LibNKF::StdGetC(f);
			mime_input_buf(mime_input_state.last++) = (unsigned char) c1;
			if (!(++i < MAXRECOVER) || c1 == EOF)
				break;
			if (c1 != '?') {
				mime_decode_mode = FALSE;
			}
			break;
		}
	}
	SwitchMimeGetC();
	if (!mime_decode_mode) {
		/* false MIME premble, restart from mime_buffer */
		mime_decode_mode = 1; /* no decode, but read from the mime_buffer */
		/* Since we are in MIME mode until buffer becomes empty,    */
		/* we never go into mime_begin again for a while.           */
		return c1;
	}
	/* discard mime preemble, and goto MIME mode */
	mime_input_state.last = k;
	/* do no MIME integrity check */
	return c1; /* used only for checking EOF */
}
/**
 * Mimeの完全性をチェックする
 */
nkf_char LibNKF::MimeIntegrity(FILE* f, const unsigned char* p) {

	nkf_char c, d;
	unsigned int q;
	/*
	 * In buffered mode, read until =? or NL or buffer full
	 */
	mime_input_state.input = mime_input_state.top;
	mime_input_state.last = mime_input_state.top;

	while (*p)
		mime_input_buf(mime_input_state.input++) = *p++;
	d = 0;
	q = mime_input_state.input;
	while ((c = LibNKF::StdGetC(f)) != EOF) {
		if (((mime_input_state.input - mime_input_state.top) & MIME_BUF_MASK)
				== 0) {
			break; /* buffer full */
		}
		if (c == '=' && d == '?') {
			/* checked. skip header, start decode */
			mime_input_buf(mime_input_state.input++) = (unsigned char) c;
			/* mime_last_input = mime_input_state.input; */
			mime_input_state.input = q;
			SwitchMimeGetC();
			return 1;
		}
		if (!((c == '+' || c == '/' || c == '=' || c == '?' || is_alnum(c))))
			break;
		/* Should we check length mod 4? */mime_input_buf(mime_input_state.input++) =
				(unsigned char) c;
		d = c;
	}
	/* In case of Incomplete MIME, no MIME decode  */mime_input_buf(mime_input_state.input++) =
			(unsigned char) c;
	mime_input_state.last = mime_input_state.input; /* point undecoded buffer */
	mime_decode_mode = 1; /* no decode on mime_input_buf last in mime_getc */
	SwitchMimeGetC(); /* anyway we need buffered getc */
	return 1;
}
/**
 * 入力文字コードを設定する
 */
void LibNKF::SetInputMode(int mode) {
	inputMode = mode;
	shiftMode = 0;
	SetInputCodeName("ISO-2022-JP");
	//debug("ISO-2022-JP");
}
/**
 * 入力文字コードを設定する
 */
void LibNKF::SetInputCodeName(std::string codeName) {
	inputCodeName = codeName;
}
/**
 * Mime読み取りの際GetC関数の挙動を変更する
 */
void LibNKF::SwitchMimeGetC(void) {
//  とりあえずスタブで
//	if (i_getc != mime_getc) {
//		i_mgetc = i_getc;
//		i_getc = mime_getc;
//		i_mungetc = i_ungetc;
//		i_ungetc = mime_ungetc;
//		if (mime_f == STRICT_MIME) {
//			i_mgetc_buf = i_mgetc;
//			i_mgetc = mime_getc_buf;
//			i_mungetc_buf = i_mungetc;
//			i_mungetc = mime_ungetc_buf;
//		}
//	}
}
/**
 * Asciiコードが混じった場合のエスケープシーケンス
 */
void LibNKF::OutputAsciiEscapeSequence(int mode) {
	if (outputMode != ASCII && outputMode != ISO_8859_1) {
		OPutC(ESC);
		OPutC('(');
		OPutC(ascii_intro);
	    outputMode = mode;
	}
}


