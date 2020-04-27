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
	map<string, double> rawValues; 	///< raw values from the CSV repositories
	map<string, double> rates;		///< rate increase values with respect the previous day
	map<string, double> doubles;	///< days from previous "half value"


	void 	Init();
	void 	FillValues(const string&, const string&, const string&);
	void 	SumRaw(const gDataEntry&);
	void 	EntryCorrection();
	int  	DaysFrom_1_1_2020(const string&);
	void	StandardizeDate();
	void	AddRates(const gDataEntry&);
//	void	AddDoubles();

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
