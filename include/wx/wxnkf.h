/*
 * wxnkf.h
 *
 *  Created on: 2012/08/09
 * Contributor: Hiroyuki Nagata
 */

#ifndef WXNKF_H_
#define WXNKF_H_

#include <wx/wx.h>
#include <wx/stream.h>
#include <wx/filesys.h>
#include <wx/datstrm.h>
#include <wx/wfstream.h>
#include "flagset.h"


class wxNKF {
public:
	/**
	 * constructor
	 */
	wxNKF();
	/**
	 * destructor
	 */
	~wxNKF();
	/**
	 * convert charcter code in file, with option
	 */
	int Convert(const wxString inputFilePath, const wxString outputFilePath,
			const wxString option);
private:
	/**
	 * set of nkf flag
	 */
	std::bitset<nkf_flag_num> nkfFlags;
	/**
	 * SetOption : setting and judge options
	 *
	 * return values:
	 *    0: success
	 *   -1: ArgumentError
	 */
	int SetOption(const wxString option);
	/**
	 * main method of this class
	 * convert charcode
	 */
	int KanjiConvert(wxInputStream* in, wxDataOutputStream* out);
};

#endif /* WXNKF_H_ */
