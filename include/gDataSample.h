/*
 * gDataSample.h
 *
 *  Created on: Mar 23, 2020
 *      Author: germano
 */

#ifndef INCLUDE_GDATASAMPLE_H_
#define INCLUDE_GDATASAMPLE_H_
#include "myCommon.h"
#include "gCard.h"
#include "gDataEntry.h"

class gDataSample {
public:
	gDataSample();

	void SetDataMap(map<string, vector<gDataEntry>> M) {DataMap = M;};
	void SetName(const string& s) 					{name = s;};
//	void SetPopulation(map<string, double> m) 		{population = m;};
	void Append(shared_ptr<gDataSample>);

	map<string, vector<gDataEntry>>		GetDataMap() {return DataMap;};
	vector<gDataEntry>					GetTerritoryData(const string&);
	string 								GetName()	{return name;};
//	map<string, double> 				GetPopulation() {return population;};
//	double								GetPopulation(const string&);
	int									GetDayZero(const string&, const string&, int);

	void Print(int);

	virtual ~gDataSample();

private:
	string name;
	map<string, vector<gDataEntry>> DataMap;

};

#endif /* INCLUDE_GDATASAMPLE_H_ */
