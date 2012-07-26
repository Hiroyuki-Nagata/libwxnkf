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
	const unsigned short euc_to_utf8_1byte[];
	const unsigned short * const euc_to_utf8_2bytes[];
	const unsigned short * const euc_to_utf8_2bytes_ms[];
	const unsigned short * const euc_to_utf8_2bytes_mac[];
	const unsigned short * const x0212_to_utf8_2bytes[];

	/**
	 * UTF8_INPUT_ENABLE
	 */
#define sizeof_utf8_to_euc_C2 64
#define sizeof_utf8_to_euc_E5B8 64
#define sizeof_utf8_to_euc_2bytes 112
#define sizeof_utf8_to_euc_3bytes 16
	const unsigned short * const utf8_to_euc_2bytes[];
	const unsigned short * const utf8_to_euc_2bytes_ms[];
	const unsigned short * const utf8_to_euc_2bytes_932[];
	const unsigned short * const utf8_to_euc_2bytes_mac[];
	const unsigned short * const * const utf8_to_euc_3bytes[];
	const unsigned short * const * const utf8_to_euc_3bytes_ms[];
	const unsigned short * const * const utf8_to_euc_3bytes_932[];
	const unsigned short * const * const utf8_to_euc_3bytes_mac[];

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
	const struct normalization_pair normalization_table[];

	/**
	 * SHIFTJIS_CP932
	 */
#define CP932_TABLE_BEGIN 0xFA
#define CP932_TABLE_END   0xFC
	const unsigned short shiftjis_cp932[3][189];
#define CP932INV_TABLE_BEGIN 0xED
#define CP932INV_TABLE_END   0xEE
	const unsigned short cp932inv[2][189];

	/**
	 * X0212_ENABLE
	 */
	const unsigned short shiftjis_x0212[3][189];
	const unsigned short *const x0212_shiftjis[];
};

#endif /* UTF8TABLE_H_ */
