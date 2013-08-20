#include "MHashEngine.h"
#include <sstream>
using namespace RCF;

MHashEngine::MHashEngine() {
	_out = NULL;
	_thread = mhash_init(MHASH_MD5);
	_outsize = mhash_get_block_size(MHASH_MD5);
}

MHashEngine::~MHashEngine() {
	_out = NULL;
	mhash_deinit(_thread, NULL);
}

std::string MHashEngine::getHash(std::string in) {
	mhash(_thread, in.c_str(), in.length());
	_out = static_cast<unsigned char*>(mhash_end(_thread));
	std::stringstream hexoutss;
	hexoutss.str("");
	for(int i = 0; i < _outsize; i++) {
		hexoutss << std::hex << static_cast<unsigned int>(_out[i]);
	}
	_out = NULL;
	_thread = mhash_init(MHASH_MD5);
	return hexoutss.str();
}
