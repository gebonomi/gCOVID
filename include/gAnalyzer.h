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
	void ShowResults();
	virtual ~gAnalyzer();

private:
	gCard myCard;
	shared_ptr<gDataSample> DataSample;
	vector<gDataEntry>		RankingRaw; 		///< One entry per territory only
	vector<gDataEntry>		RankingRate;		///< All entries of all territories
	vector<gDataEntry>		RankingDouble;		///< All entries of all territories
	vector<gDataEntry>		RankingDayZero;		///< One entry per territory only (the one of "day zero" - see gCOVID.card)

	void 					ThroughData();
	void 					OrderRawBy();
	void 					OrderRateBy();
	void 					OrderDoubleBy();
	void					ShowRankingRaw();
	void					ShowRankingRate();
	void					ShowRankingDouble();

	static bool SortByDay(const gDataEntry& a, const gDataEntry& b) {
	    return (a.day_of_the_year < b.day_of_the_year);
	}

};

#endif /* INCLUDE_GANALYZER_H_ */
