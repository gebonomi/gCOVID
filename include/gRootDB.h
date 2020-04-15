/*
 * gRootDB.h
 *
 *  Created on: Mar 27, 2020
 *      Author: germano
 */

#ifndef INCLUDE_GROOTDB_H_
#define INCLUDE_GROOTDB_H_
#include "myRoot.h"
#include "gDataSample.h"

class gRootDB {
public:
	gRootDB();
	gRootDB(const string&, const string&);
	void Write();
	void Read(const vector<string>&);

	shared_ptr<gDataSample> GetData() {return DataSample;};

	void SetData(shared_ptr<gDataSample> D) {DataSample = D;};

	void Close();

	virtual ~gRootDB();

private:
	unique_ptr<TFile> ROOT_DB_FILE;
	unique_ptr<TTree> tData;

	shared_ptr<gDataSample> DataSample;
	///< Variables to book the brances and fill the data
	gDataEntry entry;
	string sample, territory, date;
	string *sample_ptr=nullptr;
	string *territory_ptr=nullptr;
	string *date_ptr=nullptr;
	int day_of_the_year;
    double confirmed;
    double new_confirmed;
    double actives;
    double new_actives;
    double recovered;
    double new_recovered;
    double deceased;
    double new_deceased;
    double tests;
    double new_tests;
    double hospitalized_symptoms;
    double hospitalized_intensive;
    double hostpitalized_total;
    double home_isolation;
    double population;

	bool OpenRootFile(const string&, const string&);
	bool SetBranchesWrite();
	void FillWrite();

	bool SetBranchesRead();
	void FillRead(const vector<string>&);
	gDataEntry FillEntry();
};

#endif /* INCLUDE_GROOTDB_H_ */
