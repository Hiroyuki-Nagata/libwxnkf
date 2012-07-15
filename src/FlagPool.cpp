/*
 * FlagPool.cpp
 *
 *  Created on: 2012/07/11
 *      Author: learning
 */

#include "FlagPool.h"

FlagPool::FlagPool() {

	option_mode = 0;

	/* flags */
	unbuf_f = FALSE;
	estab_f = FALSE;
	nop_f = FALSE;
	binmode_f = TRUE;
	rot_f = FALSE;
	hira_f = FALSE;
	alpha_f = FALSE;
	mime_f = MIME_DECODE_DEFAULT;
	mime_decode_f = FALSE;
	mimebuf_f = FALSE;
	broken_f = FALSE;
	iso8859_f = FALSE;
	mimeout_f = FALSE;
	x0201_f = NKF_UNSPECIFIED;
	iso2022jp_f = FALSE;
	nfc_f = FALSE;
	cap_f = FALSE;
	url_f = FALSE;
	numchar_f = FALSE;
	noout_f = FALSE;
	debug_f = FALSE;
	guess_f = 0;
	cp51932_f = FALSE;
	cp932inv_f = TRUE;
	x0212_f = FALSE;
	x0213_f = FALSE;
	ms_ucs_map_f = UCS_MAP_CP932;
	no_best_fit_chars_f = FALSE;
	no_cp932ext_f = FALSE;
	file_out_f = FALSE;
	overwrite_f = FALSE;
	preserve_time_f = FALSE;
	backup_f = FALSE;
	backup_suffix = "";
	cap_f = FALSE;
	url_f = FALSE;
	eolmode_f = 0;
	input_eol = 0;
	output_bom_f = FALSE;
	output_endian = ENDIAN_BIG;
	kanji_intro = DEFAULT_J;
	ascii_intro = DEFAULT_R;
	f_line = 0;
	f_prev = 0;
	fold_preserve_f = FALSE;
	fold_f = FALSE;
	fold_len = 0;
	fold_margin = FOLD_MARGIN;
}

