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
 * convert charcter code with option
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
 * main method of this class
 * convert charcode
 */
int wxNKF::KanjiConvert(wxInputStream* in, wxDataOutputStream* out) {
	while (!in->Eof()) {
		out->Write8(in->GetC());
	}
}

