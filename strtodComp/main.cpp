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

using namespace std;

static int iterCount = 100;

template <typename F>
void benchmark(vector<string> &lines, F&& lambda) {
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
}

int main(int argc, const char * argv[]) {
	ifstream canada("./canada.txt");
	string line;
	vector<string> lines;
	while (getline(canada, line)) {
		lines.push_back(line);
	}
	for (int j = 0; j < 3; j++) {
		cout << endl;
		benchmark(lines, [](string &line) -> double {
			double next = 0;
			const char *str = line.c_str();
			bool foundMinus = str[0] == '-';
			__unused bool unused = parse_number((const uint8_t *)str, 0, foundMinus, &next);
			return next;
		});
		usleep(500000);
    	({
    		double sum = 0;
    		auto start = chrono::high_resolution_clock::now();
    		for (int i = 0; i < iterCount; i++) {
    			for (auto &line: lines) {
    				double next = 0;
    				const char *str = line.c_str();
    				bool foundMinus = str[0] == '-';
    				__unused bool unused = parse_number((const uint8_t *)str, 0, foundMinus, &next);
    				// cout << next << endl;
    				/*if (next != strtod(str, NULL)) {
    					abort();
    				}*/
    				sum += next;
    			}
    		}
    		auto end = chrono::high_resolution_clock::now();
    		auto diff = chrono::duration_cast<chrono::microseconds>(end - start);
    		cout << diff.count() << ", " << sum << endl;
    		;
    	});
		/*({
			double sum = 0;
			auto start = chrono::high_resolution_clock::now();
			for (int i = 0; i < iterCount; i++) {
				for (auto &line: lines) {
					double next = strtod(line.c_str(), NULL);
					sum += next;
				}
			}
			auto end = chrono::high_resolution_clock::now();
			auto diff = chrono::duration_cast<chrono::microseconds>(end - start);
			cout << diff.count() << ", " << sum << endl;
			;
		});
    	({
        	double sum = 0;
        	auto start = chrono::high_resolution_clock::now();
    		for (int i = 0; i < iterCount; i++) {
            	for (auto &line: lines) {
            		double next = 0;
            		__unused bool unused = absl::SimpleAtod(line, &next);
            		sum += next;
            	}
    		}
        	auto end = chrono::high_resolution_clock::now();
        	auto diff = chrono::duration_cast<chrono::microseconds>(end - start);
        	cout << diff.count() << ", " << sum << endl;
    		;
    	});*/
	}
	return 0;
}
