/*
 * gDataEntry.h
 *
 *  Created on: Mar 20, 2020
 *      Author: germano
 */

#ifndef INCLUDE_GDATAENTRY_H_
#define INCLUDE_GDATAENTRY_H_
#include "myCommon.h"

class gDataEntry {
public:
	gDataEntry();
	gDataEntry(const string&);
	string sample;
	string territory;
	double population;
	string date;
	int	day_of_the_year; ///< 0-365 [2020 has 366 days]
	map<string, double> entryMap;

	void 	Init();
	void 	FillValues(const string&, const string&, const string&);
	void 	Replace(const gDataEntry&);
	void 	Sum(const gDataEntry&);
	void 	EntryCorrection();
	int  	DaysFromYearStart(const string&);
	void	StandardizeDate();

	void 	Print(int);
	virtual ~gDataEntry();

private:
	time_t 	DateStringToTimeT(const string&);
	void 	FillValuesItaly(const string&, const string&);
	void 	FillValuesRegioni(const string&, const string&);
	void 	FillValuesProvince(const string&, const string&);
	string 	CleanLine(const string&);
	string 	CleanString(const string&);


};

#endif /* INCLUDE_GDATAENTRY_H_ */
