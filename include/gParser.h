/*
 * gParser.h
 *
 *  Created on: Mar 27, 2020
 *      Author: germano
 */

#ifndef INCLUDE_GPARSER_H_
#define INCLUDE_GPARSER_H_
#include "myCommon.h"
#include "gCard.h"
#include "gDataEntry.h"

class gParser {
public:
	gParser();
	gParser(gCard&);
	virtual ~gParser();
	vector<string> LegendToTitles(const string&, int&, int&);
	vector<string> LineToValues(const string&);
	map<string, vector<gDataEntry>> LinesToEntries(const string&, vector<string>&);

private:
	string RemoveNewLine(const string&);
	string EmptyStringToZero(const string&);
	string CleanLine(const string&);

};

#endif /* INCLUDE_GPARSER_H_ */
