/*
 * NKFEncoding.h
 *
 *  Created on: 2012/07/16
 *      Author: learning
 */

#ifndef NKFENCODING_H_
#define NKFENCODING_H_

class NKFEncoding {
public:
	NKFEncoding();
    int id;
    std::string name;
    NKFNativeEncoding* baseEncoding;
};

#endif /* NKFENCODING_H_ */
