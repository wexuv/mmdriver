#ifndef _BASE64_H_
#define _BASE64_H_

#include <string>  
#include <iostream>  
#include <sstream>  

namespace driver
{
	bool Base64Encode(const std::string& input, std::string& output);
	
	bool Base64Decode(const std::string& input, std::string& output);

}

#endif //_BASE64_H_