/*
 * FlagPool.h
 *
 *  Created on: 2012/07/11
 * Contributor: Hiroyuki Nagata
 */

#ifndef FLAGPOOL_H_
#define FLAGPOOL_H_

#define         FALSE   0
#define         TRUE    1

#define         DEFAULT_J       'B'
#define         DEFAULT_R       'B'

/* MIME ENCODE */
#define         FIXED_MIME      7
#define         STRICT_MIME     8

#define NKF_UNSPECIFIED (-TRUE)

/* MIME_DECODE_DEFAULT */
#define MIME_DECODE_DEFAULT STRICT_MIME

/* Folding */
#define FOLD_MARGIN  10
#define DEFAULT_FOLD 60

/* UCS Mapping
 * 0: Shift_JIS, eucJP-ascii
 * 1: eucJP-ms
 * 2: CP932, CP51932
 * 3: CP10001
 */
#define UCS_MAP_ASCII   0
#define UCS_MAP_MS      1
#define UCS_MAP_CP932   2
#define UCS_MAP_CP10001 3

/* byte order */
enum byte_order {
	ENDIAN_BIG = 1, ENDIAN_LITTLE = 2, ENDIAN_2143 = 3, ENDIAN_3412 = 4
};

class FlagPool {
public:

	FlagPool();

	int option_mode;

	/* all flags */

	/**
	 * バッファリングを行うかどうかのフラグ
	 */
	int unbuf_f;
	/**
	 * 文字コードが確立したかどうかを示すフラグ
	 */
	int estab_f;
	int nop_f;
	/**
	 * バイナリモードフラグ
	 */
	int binmode_f;
	/* rot14/43 mode */
	int rot_f;
	/* hira/kata henkan */
	int hira_f;
	/* convert JIx0208 alphbet to ASCII */
	int alpha_f;
	/* convert MIME B base64 or Q */
	int mime_f;
	/* mime decode is explicitly on */
	int mime_decode_f;
	/* MIME buffered input */
	int mimebuf_f;
	/* convert ESC-less broken JIS */
	int broken_f;
	/* ISO8859 through */
	int iso8859_f;
	/* base64 mode */
	int mimeout_f;
	/* convert JIS X 0201 */
	int x0201_f;
	/* replace non ISO-2022-JP with GETA */
	int iso2022jp_f;
	/* UNICODE_NORMALIZATION */
	int nfc_f;
	/* INPUT_OPTION */
	int cap_f;
	int url_f;
	/* NUMCHAR_OPTION */
	int numchar_f;
	/* CHECK_OPTION */
	int noout_f;
	int debug_f;
	/* 0: OFF, 1: ON, 2: VERBOSE */
	int guess_f;
	/* invert IBM extended characters to others */
	int cp51932_f;
	/* invert NEC-selected IBM extended characters to IBM extended characters */
	int cp932inv_f;
	int x0212_f;
	int x0213_f;
	int ms_ucs_map_f;
	int no_best_fit_chars_f;
	/* no NEC special, NEC-selected IBM extended and IBM extended characters */
	int no_cp932ext_f;
	/* OVERWRITE */
	int file_out_f;
	int overwrite_f;
	int preserve_time_f;
	int backup_f;
	char* backup_suffix;
	/* CR, LF, CRLF */
	int eolmode_f;
	/* 0: unestablished, EOF: MIXED */
	int input_eol;
	/* UTF8_OUTPUT_ENABLE */
	int output_bom_f;
	int output_endian;
	/* options */
	unsigned char kanji_intro;
	unsigned char ascii_intro;
	/* fold parameter */
	int f_line;
	/* chars in line */
	int f_prev;
	int fold_preserve_f;
	/* preserve new lines */
	int fold_f;
	int fold_len;
	int fold_margin;
};

#endif /* FLAGPOOL_H_ */
