/*
 * Util.cpp
 *
 *  Created on: 2012/07/12
 *      Author: learning
 */

#include "Util.h"

/**
 * 文字コード名からNKFEncodingクラスを判別し、インスタンスを返す
 */
NKFNativeEncoding* Util::NKFEncFind(const char* name) {
	int idx = -1;
	idx = NKFEncFindIndex(name);
	if (idx < 0)
		return 0;
	return NKFEncFromIndex(idx);
}

/**
 * IDに対応するNKFEncodingクラスを返却する
 */
NKFNativeEncoding* Util::NKFEncFromIndex(int idx) {
	if (idx < 0 || NKF_ENCODING_TABLE_SIZE <= idx) {
		return 0;
	}

	NKFNativeEncoding* enc;
	enc->id = idx;
	enc->name = &nkf_encoding_table[idx];

	return enc;
}

NKFEncoding* Util::NKFDefaultEncoding() {
	NKFNativeEncoding* enc;
	enc->id = 0;

#ifdef DEFAULT_CODE_LOCALE
	enc = NKFLocaleEncoding();
#elif defined(DEFAULT_ENCIDX)
	enc = Util::NKFEncFromIndex(DEFAULT_ENCIDX);
#endif
	if (!enc)
		NKFNativeEncoding* enc;

	return enc;
}

/**
 * 文字コード名から文字コードに対応するIDを取得する
 */
int Util::NKFEncFindIndex(const char* name) {
	int i;
	if (name[0] == 'X' && *(name + 1) == '-')
		name += 2;
	for (i = 0; encoding_name_to_id_table[i].id >= 0; i++) {
		if (nkf_str_caseeql(encoding_name_to_id_table[i].name, name)) {
			return encoding_name_to_id_table[i].id;
		}
	}
	return -1;
}

NKFEncoding* Util::NKFLocaleEncoding() {
	NKFNativeEncoding* enc;
	enc->id = 0;
	const char *encname = Util::NKFLocaleCharmap();
	if (encname)
		// encnameに何らかの文字列が設定されていた場合
		enc = Util::NKFEncFind(encname);
	return enc;
}

