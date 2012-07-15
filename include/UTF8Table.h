/*
 * UTF8Table.h
 *
 *  Created on: 2012/07/10
 * Contributor: Hiroyuki Nagata
 */

#ifndef UTF8TABLE_H_
#define UTF8TABLE_H_

class UTF8Table {
public:

	// constructor
	UTF8Table();

	/**
	 * UTF8_OUTPUT_ENABLE
	 */
#define sizeof_euc_to_utf8_1byte 94
#define sizeof_euc_to_utf8_2bytes 94
	static const unsigned short euc_to_utf8_1byte[];
	static const unsigned short * const euc_to_utf8_2bytes[];
	static const unsigned short * const euc_to_utf8_2bytes_ms[];
	static const unsigned short * const euc_to_utf8_2bytes_mac[];
	static const unsigned short * const x0212_to_utf8_2bytes[];

	/**
	 * UTF8_INPUT_ENABLE
	 */
#define sizeof_utf8_to_euc_C2 64
#define sizeof_utf8_to_euc_E5B8 64
#define sizeof_utf8_to_euc_2bytes 112
#define sizeof_utf8_to_euc_3bytes 16
	static const unsigned short * const utf8_to_euc_2bytes[];
	static const unsigned short * const utf8_to_euc_2bytes_ms[];
	static const unsigned short * const utf8_to_euc_2bytes_932[];
	static const unsigned short * const utf8_to_euc_2bytes_mac[];
	static const unsigned short * const * const utf8_to_euc_3bytes[];
	static const unsigned short * const * const utf8_to_euc_3bytes_ms[];
	static const unsigned short * const * const utf8_to_euc_3bytes_932[];
	static const unsigned short * const * const utf8_to_euc_3bytes_mac[];

	/**
	 * UNICODE_NORMALIZATION
	 */
#define NORMALIZATION_TABLE_LENGTH 942
#define NORMALIZATION_TABLE_NFC_LENGTH 3
#define NORMALIZATION_TABLE_NFD_LENGTH 9
	struct normalization_pair {
		const unsigned char nfc[NORMALIZATION_TABLE_NFC_LENGTH];
		const unsigned char nfd[NORMALIZATION_TABLE_NFD_LENGTH];
	};
	static const struct normalization_pair normalization_table[];

	/**
	 * SHIFTJIS_CP932
	 */
#define CP932_TABLE_BEGIN 0xFA
#define CP932_TABLE_END   0xFC
	static const unsigned short shiftjis_cp932[3][189];
#define CP932INV_TABLE_BEGIN 0xED
#define CP932INV_TABLE_END   0xEE
	static const unsigned short cp932inv[2][189];

	/**
	 * X0212_ENABLE
	 */
	static const unsigned short shiftjis_x0212[3][189];
	static const unsigned short *const x0212_shiftjis[];
};

#endif /* UTF8TABLE_H_ */
