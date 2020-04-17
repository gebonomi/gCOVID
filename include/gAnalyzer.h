/*
 * gAnalyzer.h
 *
 *  Created on: Apr 16, 2020
 *      Author: germano
 */

#ifndef INCLUDE_GANALYZER_H_
#define INCLUDE_GANALYZER_H_
#include "myCommon.h"
#include "gDataSample.h"
#include "gCard.h"

class gAnalyzer {

public:
	gAnalyzer();
	gAnalyzer(const gCard&);
	void SetDataSample(shared_ptr<gDataSample> sample) {DataSample = sample;};
	void Analyze();
	virtual ~gAnalyzer();

private:
	gCard myCard;
	shared_ptr<gDataSample> DataSample;
	map<string, gDataEntry> HigherEntry(const string&);
	vector<gDataEntry> Rank(const string&);
	void OrderBy(vector<gDataEntry>&, const string&);
	static std::ostream& bold_on(std::ostream& os) 	{return os << "\e[1m";};
	static std::ostream& bold_off(std::ostream& os) 	{return os << "\e[0m";}
};

#endif /* INCLUDE_GANALYZER_H_ */
