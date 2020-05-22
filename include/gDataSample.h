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
	void Append(shared_ptr<gDataSample>);
	void AddRates();
	void AddDoubles();

	map<string, vector<gDataEntry>>		GetDataMap() {return DataMap;};
	vector<gDataEntry>					GetTerritoryData(const string&);
	string 								GetName()	{return name;};
	int									GetDayZero(const string&, const string&, int);
	gDataEntry							GetDayZeroEntry(const string&, const string&, int);

	void Print(int);

	bool isEmpty() {
		if(DataMap.size()==0) return true;
		return false;
	}

	virtual ~gDataSample();

private:
	string name;
	map<string, vector<gDataEntry>> DataMap;

};

#endif /* INCLUDE_GDATASAMPLE_H_ */
