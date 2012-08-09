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
}
/**
 * destructor
 */
wxNKF::~wxNKF() {
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

