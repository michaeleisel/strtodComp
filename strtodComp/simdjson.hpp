//
//  simdjson.hpp
//  strtodComp
//
//  Created by Michael Eisel on 2/26/20.
//  Copyright © 2020 Michael Eisel. All rights reserved.
//

#ifndef simdjson_hpp
#define simdjson_hpp

#include <stdio.h>
#include <cstdint>

using namespace std;

bool parse_number(const uint8_t *const buf,
									   const uint32_t offset,
				  bool found_minus, double *outDouble);
#endif /* simdjson_hpp */
