//
//  main.cpp
//  strtodComp
//
//  Created by Michael Eisel on 2/26/20.
//  Copyright © 2020 Michael Eisel. All rights reserved.
//

#include <iostream>
#include "absl/strings/numbers.h"
#include <fstream>
#include <vector>
#include <chrono>
#include "simdjson.hpp"
#include <unistd.h>
#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/qi.hpp>
#include <folly/Conv.h>
#include <double-conversion/strtod.h>
//#include <folly/Conv.h

using namespace std;

static int iterCount = 100;

template <typename F>
void benchmark(const char *name, vector<string> &lines, F&& lambda) {
	cout << name << endl;
	double sum = 0;
	auto start = chrono::high_resolution_clock::now();
	for (int i = 0; i < iterCount; i++) {
		for (auto &line: lines) {
        	sum += lambda(line);
		}
	}
	auto end = chrono::high_resolution_clock::now();
	auto diff = chrono::duration_cast<chrono::microseconds>(end - start);
	cout << diff.count() << ", " << sum << endl;
	usleep(50000);
}

int main(int argc, const char * argv[]) {
	ifstream canada("./canada.txt");
	string line;
	double_conversion::StringToDoubleConverter converter(0, 0, 0, "INF", "NAN");
	vector<string> lines;
	while (getline(canada, line)) {
		lines.push_back(line);
	}
	for (int j = 0; j < 3; j++) {
		cout << endl;
		benchmark("simdjson", lines, [](string &line) -> double {
			double next = 0;
			const char *str = line.c_str();
			bool foundMinus = str[0] == '-';
			__unused bool unused = parse_number((const uint8_t *)str, 0, foundMinus, &next);
			return next;
		});
		benchmark("abseil", lines, [](string &line) -> double {
			double next = 0;
			__unused bool unused = absl::SimpleAtod(line, &next);
			return next;
		});
		/*benchmark("boost", lines, [](string &line) -> double {
			return boost::lexical_cast<double>(line.c_str(), line.length());
		});*/
		benchmark("boost spirit", lines, [](string &line) -> double {
			using boost::spirit::qi::double_;
			double d;
			boost::spirit::qi::parse(line.c_str(), line.c_str() + line.length(), double_, d);
			return d;
		});
		benchmark("folly", lines, [](string &line) -> double {
			return folly::to<double>(line.c_str());
		});
		benchmark("folly 2", lines, [](string &line) -> double {
			return folly::tryTo<double>(line.c_str()).value();
		});
		benchmark("double-conversion", lines, [&](string &line) -> double {
			int unused = 0;
			return converter.StringToDouble(line.c_str(), (int)line.length(), &unused);
		});
	}
	return 0;
}

folly::ConversionError folly::makeConversionError(folly::ConversionCode, folly::Range<char const*>) {
	abort();
}
