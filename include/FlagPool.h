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

	static int option_mode;

	/* all flags */

	/**
	 * バッファリングを行うかどうかのフラグ
	 */
	static int unbuf_f;
	/**
	 * 文字コードが確立したかどうかを示すフラグ
	 */
	static int estab_f;
	static int nop_f;
	/**
	 * バイナリモードフラグ
	 */
	static int binmode_f;
	/* rot14/43 mode */
	static int rot_f;
	/* hira/kata henkan */
	static int hira_f;
	/* convert JIx0208 alphbet to ASCII */
	static int alpha_f;
	/* convert MIME B base64 or Q */
	static int mime_f;
	/* mime decode is explicitly on */
	static int mime_decode_f;
	/* MIME buffered input */
	static int mimebuf_f;
	/* convert ESC-less broken JIS */
	static int broken_f;
	/* ISO8859 through */
	static int iso8859_f;
	/* base64 mode */
	static int mimeout_f;
	/* convert JIS X 0201 */
	static int x0201_f;
	/* replace non ISO-2022-JP with GETA */
	static int iso2022jp_f;
	/* UNICODE_NORMALIZATION */
	static int nfc_f;
	/* INPUT_OPTION */
	static int cap_f;
	static int url_f;
	/* NUMCHAR_OPTION */
	static int numchar_f;
	/* CHECK_OPTION */
	static int noout_f;
	static int debug_f;
	/* 0: OFF, 1: ON, 2: VERBOSE */
	static int guess_f;
	/* invert IBM extended characters to others */
	static int cp51932_f;
	/* invert NEC-selected IBM extended characters to IBM extended characters */
	static int cp932inv_f;
	static int x0212_f;
	static int x0213_f;
	static int ms_ucs_map_f;
	static int no_best_fit_chars_f;
	/* no NEC special, NEC-selected IBM extended and IBM extended characters */
	static int no_cp932ext_f;
	/* OVERWRITE */
	static int file_out_f;
	static int overwrite_f;
	static int preserve_time_f;
	static int backup_f;
	static char* backup_suffix;
	/* CR, LF, CRLF */
	static int eolmode_f;
	/* 0: unestablished, EOF: MIXED */
	static int input_eol;
	/* UTF8_OUTPUT_ENABLE */
	static int output_bom_f;
	static int output_endian;
	/* options */
	static unsigned char kanji_intro;
	static unsigned char ascii_intro;
	/* fold parameter */
	static int f_line;
	/* chars in line */
	static int f_prev;
	static int fold_preserve_f;
	/* preserve new lines */
	static int fold_f;
	static int fold_len;
	static int fold_margin;
};

#endif /* FLAGPOOL_H_ */
