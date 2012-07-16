/*
 * LibNKF.h
 *	libnkfの中心となるクラス
 *  Created on: 2012/07/10
 * Contributor: Hiroyuki Nagata
 */

#ifndef LIBNKF_H_
#define LIBNKF_H_

#include <iostream>
#include <string>
#include <sys/stat.h>
#include "NKFNativeEncodings.h"
#include "NKFEncoding.h"
#include "UTF8Table.h"
#include "FlagPool.h"
#include "UTF16Util.h"
#include "GuessConv.h"
#include "Util.h"

using namespace std;

/* MIME_DECODE_DEFAULT */
#define MIME_DECODE_DEFAULT STRICT_MIME

/* X0201_DEFAULT */
#define X0201_DEFAULT TRUE

/* DEFAULT_NEWLINE */
#if DEFAULT_NEWLINE == 0x0D0A
#elif DEFAULT_NEWLINE == 0x0D
#else
#define DEFAULT_NEWLINE 0x0A
#endif
/* HELP_OUTPUT */
#ifdef HELP_OUTPUT_STDERR
#define HELP_OUTPUT stderr
#else
#define HELP_OUTPUT stdout
#endif

/* Compatibility definitions */
#ifdef nkf_char
#elif defined(INT_IS_SHORT)
typedef long nkf_char;
#define NKF_INT32_C(n)   (n##L)
#else
typedef int nkf_char;
#define NKF_INT32_C(n)   (n)
#endif

#if ( (defined(_MSC_VER) && defined(__386__) && !defined(__LINUX__)) || defined(__MINGW32__) || defined(__MSDOS__) || defined(__WINDOWS__) || defined(__DOS__) )
#if ( (defined(__Win32__) || defined(_WIN32)) && !defined(__WIN32__))
#define __WIN32__
#endif
#endif

#ifdef _MSC_VER
#define mktemp _mktemp
#endif

/* open file mode setting */
#ifdef __WIN32__
#define setbinmode(fp) setmode(fileno(fp), O_BINARY)
#else /* UNIX */
#define setbinmode(fp) (void)(fp)
#endif

#ifdef _IOFBF /* SysV and MSDOS, Windows */
#define       setvbuffer(fp, buf, size)       setvbuf(fp, buf, _IOFBF, size)
#else /* BSD */
#define       setvbuffer(fp, buf, size)       setbuffer(fp, buf, size)
#endif

#define         FALSE   0
#define         TRUE    1

/* state of output_mode and input_mode

 c2           0 means ASCII
 JIS_X_0201_1976_K
 ISO_8859_1
 JIS_X_0208
 EOF      all termination
 c1           32bit data

 */

/* MIME ENCODE */

#define         FIXED_MIME      7
#define         STRICT_MIME     8

/* all long option */
static const struct {
	const char *name;
	const char *alias;
} long_option[] = { { "ic=", "" }, { "oc=", "" }, { "base64", "jMB" }, { "euc",
		"e" }, { "euc-input", "E" }, { "fj", "jm" }, { "help", "" }, { "jis",
		"j" }, { "jis-input", "J" }, { "mac", "sLm" }, { "mime", "jM" }, {
		"mime-input", "m" }, { "msdos", "sLw" }, { "sjis", "s" }, {
		"sjis-input", "S" }, { "unix", "eLu" }, { "version", "v" }, { "windows",
		"sLw" }, { "hiragana", "h1" }, { "katakana", "h2" }, {
		"katakana-hiragana", "h3" }, { "guess=", "" }, { "guess", "g2" }, {
		"cp932", "" }, { "no-cp932", "" }, { "x0212", "" }, { "utf8", "w" }, {
		"utf16", "w16" }, { "ms-ucs-map", "" }, { "fb-skip", "" }, { "fb-html",
		"" }, { "fb-xml", "" }, { "fb-perl", "" }, { "fb-java", "" }, {
		"fb-subchar", "" }, { "fb-subchar=", "" }, { "utf8-input", "W" }, {
		"utf16-input", "W16" }, { "no-cp932ext", "" },
		{ "no-best-fit-chars", "" }, { "utf8mac-input", "" },
		{ "overwrite", "" }, { "overwrite=", "" }, { "in-place", "" }, {
				"in-place=", "" }, { "cap-input", "" }, { "url-input", "" }, {
				"numchar-input", "" }, { "no-output", "" }, { "debug", "" }, {
				"cp932inv", "" }, { "prefix=", "" }, };

/*
 * エンコード名とIDのテーブル
 */
struct {
	const char *name;
	const int id;
} encoding_name_to_id_table[] = { { "US-ASCII", ASCII }, { "ASCII", ASCII },
		{ "646", ASCII }, { "ROMAN8", ASCII },
		{ "ISO-2022-JP", ISO_2022_JP }, { "ISO2022JP-CP932", CP50220 }, {
				"CP50220", CP50220 }, { "CP50221", CP50221 }, {
				"CSISO2022JP", CP50221 }, { "CP50222", CP50222 }, {
				"ISO-2022-JP-1", ISO_2022_JP_1 }, { "ISO-2022-JP-3",
				ISO_2022_JP_3 }, { "ISO-2022-JP-2004", ISO_2022_JP_2004 }, {
				"SHIFT_JIS", SHIFT_JIS }, { "SJIS", SHIFT_JIS }, {
				"MS_Kanji", SHIFT_JIS }, { "PCK", SHIFT_JIS }, {
				"WINDOWS-31J", WINDOWS_31J },
		{ "CSWINDOWS31J", WINDOWS_31J }, { "CP932", WINDOWS_31J }, {
				"MS932", WINDOWS_31J }, { "CP10001", CP10001 }, { "EUCJP",
				EUC_JP }, { "EUC-JP", EUC_JP }, { "EUCJP-NKF", EUCJP_NKF },
		{ "CP51932", CP51932 }, { "EUC-JP-MS", EUCJP_MS }, { "EUCJP-MS",
				EUCJP_MS }, { "EUCJPMS", EUCJP_MS }, { "EUC-JP-ASCII",
				EUCJP_ASCII }, { "EUCJP-ASCII", EUCJP_ASCII }, {
				"SHIFT_JISX0213", SHIFT_JISX0213 }, { "SHIFT_JIS-2004",
				SHIFT_JIS_2004 }, { "EUC-JISX0213", EUC_JISX0213 }, {
				"EUC-JIS-2004", EUC_JIS_2004 }, { "UTF-8", UTF_8 }, {
				"UTF-8N", UTF_8N }, { "UTF-8-BOM", UTF_8_BOM }, {
				"UTF8-MAC", UTF8_MAC }, { "UTF-8-MAC", UTF8_MAC }, {
				"UTF-16", UTF_16 }, { "UTF-16BE", UTF_16BE }, {
				"UTF-16BE-BOM", UTF_16BE_BOM }, { "UTF-16LE", UTF_16LE }, {
				"UTF-16LE-BOM", UTF_16LE_BOM }, { "UTF-32", UTF_32 }, {
				"UTF-32BE", UTF_32BE }, { "UTF-32BE-BOM", UTF_32BE_BOM }, {
				"UTF-32LE", UTF_32LE }, { "UTF-32LE-BOM", UTF_32LE_BOM }, {
				"BINARY", BINARY }, { NULL, -1 } };

#if defined(DEFAULT_CODE_JIS)
#define	    DEFAULT_ENCIDX ISO_2022_JP
#elif defined(DEFAULT_CODE_SJIS)
#define	    DEFAULT_ENCIDX SHIFT_JIS
#elif defined(DEFAULT_CODE_WINDOWS_31J)
#define	    DEFAULT_ENCIDX WINDOWS_31J
#elif defined(DEFAULT_CODE_EUC)
#define	    DEFAULT_ENCIDX EUC_JP
#elif defined(DEFAULT_CODE_UTF8)
#define	    DEFAULT_ENCIDX UTF_8
#endif

#define		is_alnum(c)  \
    (('a'<=c && c<='z')||('A'<= c && c<='Z')||('0'<=c && c<='9'))

/* I don't trust portablity of toupper */
#define nkf_toupper(c)  (('a'<=c && c<='z')?(c-('a'-'A')):c)
#define nkf_isoctal(c)  ('0'<=c && c<='7')
#define nkf_isdigit(c)  ('0'<=c && c<='9')
#define nkf_isxdigit(c)  (nkf_isdigit(c) || ('a'<=c && c<='f') || ('A'<=c && c <= 'F'))
#define nkf_isblank(c) (c == SP || c == TAB)
#define nkf_isspace(c) (nkf_isblank(c) || c == CR || c == LF)
#define nkf_isalpha(c) (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
#define nkf_isalnum(c) (nkf_isdigit(c) || nkf_isalpha(c))
#define nkf_isprint(c) (SP<=c && c<='~')
#define nkf_isgraph(c) ('!'<=c && c<='~')
#define hex2bin(c) (('0'<=c&&c<='9') ? (c-'0') : \
		    ('A'<=c&&c<='F') ? (c-'A'+10) : \
		    ('a'<=c&&c<='f') ? (c-'a'+10) : 0)
#define bin2hex(c) ("0123456789ABCDEF"[c&15])
#define is_eucg3(c2) (((unsigned short)c2 >> 8) == SS3)
#define nkf_noescape_mime(c) ((c == CR) || (c == LF) || \
			      ((c > SP) && (c < DEL) && (c != '?') && (c != '=') && (c != '_') \
			       && (c != '(') && (c != ')') && (c != '.') && (c != 0x22)))

#define is_ibmext_in_sjis(c2) (CP932_TABLE_BEGIN <= c2 && c2 <= CP932_TABLE_END)
#define nkf_byte_jisx0201_katakana_p(c) (SP <= c && c <= 0x5F)

#define nkf_char_euc3_new(c) ((c) | PREFIX_EUCG3)
#define nkf_char_unicode_new(c) ((c) | CLASS_UNICODE)
#define nkf_char_unicode_p(c) ((c & CLASS_MASK) == CLASS_UNICODE)
#define nkf_char_unicode_bmp_p(c) ((c & VALUE_MASK) <= UNICODE_BMP_MAX)
#define nkf_char_unicode_value_p(c) ((c & VALUE_MASK) <= UNICODE_MAX)

/* NKFEncodingに関するマクロ */
#define nkf_enc_name(enc) (enc)->name	// ←　単にクラスのポインタから名前をとればいいのでは？
#define nkf_enc_to_index(enc) (enc)->id // ←　単にクラスのポインタからidをとればいいのでは？
#define nkf_enc_to_base_encoding(enc) (enc)->base_encoding // ←　単にクラスのポインタからbaseEncodingをとればいいのでは？
#define nkf_enc_to_iconv(enc) nkf_enc_to_base_encoding(enc)->iconv //
#define nkf_enc_to_oconv(enc) nkf_enc_to_base_encoding(enc)->oconv //
#define nkf_enc_asciicompat(enc) ( nkf_enc_to_base_encoding(enc) == &NkfEncodingASCII || nkf_enc_to_base_encoding(enc) == &NkfEncodingISO_2022_JP)
#define nkf_enc_unicode_p(enc) (nkf_enc_to_base_encoding(enc) == &NkfEncodingUTF_8 || nkf_enc_to_base_encoding(enc) == &NkfEncodingUTF_16 || nkf_enc_to_base_encoding(enc) == &NkfEncodingUTF_32)
#define nkf_enc_cp5022x_p(enc) (nkf_enc_to_index(enc) == CP50220 ||	nkf_enc_to_index(enc) == CP50221 ||	nkf_enc_to_index(enc) == CP50222)

#define         HOLD_SIZE       1024
#if defined(INT_IS_SHORT)
#define         IOBUF_SIZE      2048
#else
#define         IOBUF_SIZE      16384
#endif

#define         DEFAULT_J       'B'
#define         DEFAULT_R       'B'

#define         GETA1   0x22
#define         GETA2   0x2e

/**
 * KanjiConvertメソッド中のswitch文用マクロ
 */
#define NEXT continue        /* no output, get next */
#define SKIP c2=0;continue   /* no output, get next */
#define MORE c2=c1;continue  /* need one more byte */
#define SEND (void)0         /* output c1 and c2, get next */
#define LAST break           /* end of loop, go closing  */

class LibNKF {

public:
	/**
	 * すべてのフラグを集合させたクラス
	 */
	FlagPool* flagPool;
	/**
	 * コンストラクタ
	 */
	LibNKF();
	/**
	 * デストラクタ
	 */
	~LibNKF();
	/**
	 * nkf用にオプションをセットする
	 */
	int SetOption(const string option);
	/**
	 * 文字コードを変換する処理のラッパーで、外部に見せるメソッド
	 */
	int Convert(const wstring src, wstring dst);
	/**
	 * ファイルポインタから1バイトnkf用のデータを読み取って返す
	 */
	static nkf_char StdGetC(FILE *f) {
		return getc(f);
	};
	/**
	 * ファイルポインタではなく引数に指定したnkf_charを返す
	 */
	static nkf_char StdUnGetC(nkf_char c, FILE *f) {
		return c;
	};
private:
	/**
	 * 文字コード変換された文字列の出力先
	 */
	wstring oConvStr;
	/**
	 * 入力された文字コード・出力する文字コード
	 */
	static NKFEncoding* inputEncoding;
	static NKFEncoding* outputEncoding;
	/**
	 * MIME mode B base64, Q hex
	 */
	static int mime_decode_mode;
	/**
	 * 0, -1, 'Q', 'B', 1, 2
	 */
	static int mimeout_mode;
	/**
	 * 入力されるバイトの並び・出力するバイトの並び
	 */
	static int inputEndian;
	static int outputEndian;
	/*
	 * 入出力の文字コード設定
	 */
	static int outputMode;
	static int inputMode;
	/**
	 * 文字コードの種類判別
	 */
	NKFEncoding* enc;
	/**
	 * LibNKFの初期化処理を行う
	 */
	void ReInit();
	/**
	 * このクラスの主要メソッド：
	 * ファイルポインタを引数に文字コードを変換する
	 */
	int KanjiConvert(FILE *f);
	/**
	 * 設定されたフラグから文字コード変換に使うメソッドを決める
	 */
	int ModuleConnection();
	/**
	 * 入力された文字コードを元にフラグを設定する
	 */
	void SetInputEncoding(NKFEncoding *enc);
	/**
	 * 出力する文字コード用にフラグを設定する
	 */
	void SetOutputEncoding(NKFEncoding *enc);
	/**
	 * 使い方の表示
	 */
	static void ShowUsage();
	/**
	 * バージョン情報の表示
	 */
	static void ShowVersion();
	/**
	 * 設定情報の表示
	 */
	static void ShowConfiguration();
	/**
	 * BOMが存在するかチェックし、存在すれば無視する
	 */
	void CheckBom(FILE *f);
	/**
	 * ??
	 */
	static unsigned char prefix_table[256];
	/*
	 *
	 */
	struct inputCode inputCodeList[] = {
	    {"EUC-JP",    0, 0, 0, {0, 0, 0}, e_status, e_iconv, 0},
	    {"Shift_JIS", 0, 0, 0, {0, 0, 0}, s_status, s_iconv, 0},
	    {"UTF-8",     0, 0, 0, {0, 0, 0}, w_status, w_iconv, 0},
	    {"UTF-16",    0, 0, 0, {0, 0, 0}, NULL, w_iconv16, 0},
	    {"UTF-32",    0, 0, 0, {0, 0, 0}, NULL, w_iconv32, 0},
	    {NULL,        0, 0, 0, {0, 0, 0}, NULL, NULL, 0}
	};
};

#endif /* LIBNKF_H_ */
