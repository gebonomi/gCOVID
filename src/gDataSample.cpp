/*
 * gDataSample.cpp
 *
 *  Created on: Mar 23, 2020
 *      Author: germano
 */

#include <gDataSample.h>

gDataSample::gDataSample() {
	// TODO Auto-generated constructor stub

}

vector<gDataEntry> gDataSample::GetTerritoryData(const string& territory) {
	vector<gDataEntry> territory_data;
	map<string, vector<gDataEntry>>::iterator dit;
	dit = DataMap.find(territory);
	if(dit!=DataMap.end()) {
		territory_data = dit->second;
	} else {
		cout << "gDataSample::GetTerritoryData --> Could not find territory " << territory << " in sample " << name << endl;
	}
	return territory_data;
}

void gDataSample::Print(int option = 0) {
	cout << "---------- " << name << " ---------- " << endl;
	std::map<string, vector<gDataEntry>>::iterator it = DataMap.begin();
	while(it != DataMap.end()) {
		cout << "## " << it->first << " ##" << endl;
		vector<gDataEntry> territory_data = it->second;
		cout << "N. of entries " << territory_data.size() << endl;
		for(auto& e:territory_data) {
			e.Print(option);
		}
 		it++;
	}
}

int gDataSample::GetDayZero(const string& territory, const string& what, int howmany) {
//	///< This method returns the day (from start of the year) in which the "what" has reached the "howmany" threshold
//	///< For example when the "new_recovered" reached 1000
	int first_day = 0;
	int day = -1;
	map<string, vector<gDataEntry>>::iterator dit;
	map<string, double>::iterator eit;
	dit = DataMap.find(territory);
	if(dit!=DataMap.end()) {
		vector<gDataEntry> territory_data = dit->second; ///< Loading the data for "this territory"
		for(auto& e:territory_data) { ///< Looping on the entries (one per day)
			map<string, double> EntryMap = e.entryMap; ///< Loading the map with the values
			eit = EntryMap.find(what);
			if(eit != EntryMap.end()) { ///< "What" has been found
				if(first_day==0) first_day = e.day_of_the_year; ///< Recording "first" day of the data sample
				if(eit->second>(double)howmany) { ///< got the first occurrence of "What" > "howmany"
					day = e.day_of_the_year; ///< Recording the day of the year
					break; ///< This is done -> exit from the entries loop
				}
			}
		}
	} else {
		cout << "gDataSample::GetDayZero --> Could not find territory " << territory << endl;
		return 0;
	}
	if(day==-1&&what=="days") day = howmany; ///< Fixing the same "day" for all samples
	if(day==-1) {
		cout << "gDataSample::GetDayZero --> Could not find the day in which " << what << " is greater than " << howmany;
		cout << " for " << territory << endl;
		return first_day;
	}
	return day;
}

void gDataSample::Append(shared_ptr<gDataSample> other) {
	///< Appending data from the "other" gDataSample
	map<string, vector<gDataEntry>>::iterator it;
	map<string, vector<gDataEntry>> other_data_map = other->GetDataMap();
		for(it=other_data_map.begin(); it!=other_data_map.end(); it++) {
		DataMap.insert(std::pair<string, vector<gDataEntry>>(it->first, it->second));
	}
//	///< Appending the corresponding values of population
//	map<string, double>::iterator jt;
//	map<string, double> other_population = other->GetPopulation();
//	for(jt=other_population.begin(); jt!=other_population.end(); jt++) {
//		population.insert(std::pair<string, double>(jt->first, jt->second));
//	}
	return;
}

//double gDataSample::GetPopulation(const string& t) {
//	map<string, double>::iterator it;
//	it = population.find(t);
////	if(t.find("Cesena")!=std::string::npos) {
////		it = population.find("Forli-Cesena");
////	} else {
////		it = population.find(t);
////	}
//	if(it!=population.end()) {
//		return it->second;
//	}
//	cout << "gDataSample::GetPopulation(const string&) --> could not find population for " << t << endl;
//	return -1;
//}

gDataSample::~gDataSample() {
	// TODO Auto-generated destructor stub
}

