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
#include "InputCodeList.h"
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

/**
 * Mime名のパターン
 */
static const unsigned char *mime_pattern[] = {
    (const unsigned char *)"\075?EUC-JP?B?",
    (const unsigned char *)"\075?SHIFT_JIS?B?",
    (const unsigned char *)"\075?ISO-8859-1?Q?",
    (const unsigned char *)"\075?ISO-8859-1?B?",
    (const unsigned char *)"\075?ISO-2022-JP?B?",
    (const unsigned char *)"\075?ISO-2022-JP?B?",
    (const unsigned char *)"\075?ISO-2022-JP?Q?",
#if defined(UTF8_INPUT_ENABLE)
    (const unsigned char *)"\075?UTF-8?B?",
    (const unsigned char *)"\075?UTF-8?Q?",
#endif
    (const unsigned char *)"\075?US-ASCII?Q?",
    NULL
};

/* MIME preprocessor fifo */

#define MIME_BUF_SIZE   (1024)    /* 2^n ring buffer */
#define MIME_BUF_MASK   (MIME_BUF_SIZE-1)
#define mime_input_buf(n)        mime_input_state.buf[(n)&MIME_BUF_MASK]
static struct {
    unsigned char buf[MIME_BUF_SIZE];
    unsigned int  top;
    unsigned int  last;  /* decoded */
    unsigned int  input; /* undecoded */
} mime_input_state;

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

	// UTF16Utilはフレンドクラス指定する
	friend class UTF16Util;

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
	/**
	 * 文字コード変換された文字列の出力先
	 */
	wstring oConvStr;
	/**
	 * 入力された文字コード・出力する文字コード
	 * このオブジェクトはLibNKFクラスにそれぞれ一つしか存在しない
	 */
	static NKFEncoding* inputEncoding;
	static NKFEncoding* outputEncoding;
	/**
	 * 入力された文字コード名称を保存する
	 * 入力文字コードの推測のために使われる
	 * NULL: unestablished, "": BINARY
	 */
	static std::string inputCodeName;

private:
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
	static int inputMode;
	static int outputMode;
	/**
	 *
	 */
	static int shiftMode;
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
	 * 入力文字コードを設定する
	 */
	void SetInputMode(int mode);
	/**
	 * 入力文字コードを設定する
	 */
	void SetInputCodeName(std::string codeName);
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
	 *
	 */
	nkf_char MimeBeginStrict(FILE *f);
	/**
	 * 判定したMime名を保存する文字列
	 */
	std::string mimeIconvBack;
	/**
	 *
	 */
	nkf_char MimeBegin(FILE *f);
	/**
	 * Mimeの完全性をチェックする
	 */
	nkf_char MimeIntegrity(FILE *f, const unsigned char *p);
	/**
	 * 入力する文字コードとその処理を決定する
	 */
	static void SetIconv(nkf_char f, std::string name);
	/**
	 * SetIconvの検証のための文字列
	 */
	static std::string iconvForCheck;
	/**
	 * ??
	 */
	static unsigned char prefix_table[256];
	/**
	 * Mime読み取りの際GetC関数の挙動を変更する
	 */
	static void SwitchMimeGetC(void);
};

#endif /* LIBNKF_H_ */
