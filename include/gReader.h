/*
 * gDataReader.h
 *
 *  Created on: Mar 20, 2020
 *      Author: germano
 * This class reads, holds and stores the whole data
 */

#ifndef INCLUDE_GREADER_H_
#define INCLUDE_GREADER_H_
#include "myCommon.h"
#include "gCard.h"
#include "gParser.h"
#include "gDataEntry.h"
#include "gDataSample.h"
#include <gRootDB.h>

class gDataReader {
public:
	gDataReader();
	gDataReader(gCard&);
	shared_ptr<gDataSample>		GetItaly() 		{return Italy;};
	shared_ptr<gDataSample> 	GetWorld() 		{return World;};
	shared_ptr<gDataSample> 	GetDataSample() {return ALL;};

	virtual ~gDataReader();

private:
	string italy_repo_file;
	string regioni_repo_file;
	string province_repo_file;
	string world_repo_dir;
	string population_data_file;
	string root_dir;
	string root_db_file;

	gCard myCard;
	gParser myParser;
	unique_ptr<gRootDB> myRootDB;

	map<string, double> pop;

	shared_ptr<gDataSample> Italy;
	shared_ptr<gDataSample> World;
	shared_ptr<gDataSample> ALL;

	void Init();
	void FillDataFromCSV();
	void FillDataFromRootDB();
	void WriteToRootDB();
	void Sort(vector<gDataEntry>&);
	vector<gDataEntry> GroupEntries(vector<gDataEntry>&);
	void CompleteInfo(vector<gDataEntry>&);
	map<string, vector<gDataEntry>> ArrangeByTerritory(vector<gDataEntry>&);

	static bool CompareByDay (const gDataEntry& A, const gDataEntry& B) {
		return A.day_of_the_year < B.day_of_the_year;
	};
	shared_ptr<gDataSample> ReadFromItalyCSV(const string&);
	shared_ptr<gDataSample> ReadFromWorldCSV(const string&);
	map<string, double> ReadPopulation();
	string FileToLines(const string&, vector<string>&);

};

#endif /* INCLUDE_GREADER_H_ */
