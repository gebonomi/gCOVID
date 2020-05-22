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
	gCard myCard;
	myCard.ReadValues();
	while(it != DataMap.end()) {
//		for(auto t:myCard.territories) {
//			if(t==it->first) {
				cout << "## " << it->first << " ##" << endl;
				vector<gDataEntry> territory_data = it->second;
				cout << "N. of entries " << territory_data.size() << endl;
				for(auto& e:territory_data) {
					e.Print(option);
				}
//			}
//		}
 		it++;
	}
}

int gDataSample::GetDayZero(const string& territory, const string& what, int howmany) {
//	///< This method returns the day (from start of the year) in which the "what" has reached the "howmany" threshold
//	///< For example when the "new_recovered" reached 1000
	int day = -1;
	if(what=="days") return howmany; ///< Fixing the same "day" for all samples
	gDataEntry day_zero_entry = GetDayZeroEntry(territory, what, howmany);
	day = day_zero_entry.day_of_the_year;
	if(day==-1) {
		cout << "gDataSample::GetDayZero --> Could not find the day in which " << what << " is greater than " << howmany;
		cout << " for " << territory << endl;
		return 0; ///< 0 = January 1st 2020
	}
	return day;
}

gDataEntry gDataSample::GetDayZeroEntry(const string& territory, const string& what, int howmany) {
//	///< This method returns the day (from start of the year) in which the "what" has reached the "howmany" threshold
//	///< For example when the "new_recovered" reached 1000
	gDataEntry day_zero_entry;
	map<string, vector<gDataEntry>>::iterator dit;
	map<string, double>::iterator eit;
	dit = DataMap.find(territory);
	if(dit!=DataMap.end()) {
		vector<gDataEntry> territory_data = dit->second; ///< Loading the data for "this territory"
		for(auto& e:territory_data) { ///< Looping on the entries (one per day)
			map<string, double> rawValues = e.rawValues; ///< Loading the map with the values
			eit = rawValues.find(what);
			if(eit != rawValues.end()) { ///< "What" has been found
				if(eit->second>(double)howmany) { ///< got the first occurrence of "What" > "howmany"
					day_zero_entry = e; ///< Recording the entry
					break; ///< This is done -> exit from the entries loop
				}
			}
		}
	} else {
		cout << "gDataSample::GetDayZero --> Could not find territory " << territory << endl;
		return day_zero_entry;
	}
	return day_zero_entry;
}

void gDataSample::Append(shared_ptr<gDataSample> other) {
	///< Appending data from the "other" gDataSample
	map<string, vector<gDataEntry>>::iterator it;
	map<string, vector<gDataEntry>> other_data_map = other->GetDataMap();
	string s;
	for(it=other_data_map.begin(); it!=other_data_map.end(); it++) {
		s = it->first;
		///< Patch to add "ITA" territory of sample "Italy" to DataSample "Regioni" with sample name "regioni"
		if(name=="Regioni"&&other->GetName()=="Italy") {
			for(unsigned int i=0; i<it->second.size(); i++) {
				it->second.at(i).sample = "regioni";
				it->second.at(i).territory = "Italia";
				s = "Italia";
			}
		}
		DataMap.insert(std::pair<string, vector<gDataEntry>>(s, it->second));
	}
	return;
}

void gDataSample::AddRates() {
	///< This method fills the information about the rates (with respect the previous day)
	map<string, vector<gDataEntry>>::iterator it;
	double rate = 0.;
	for(it=DataMap.begin(); it!=DataMap.end(); it++) {
		vector<gDataEntry> entries = it->second;
		for(unsigned int i=entries.size()-1; i>0; i--) {
			gDataEntry this_entry, prev_entry;
			this_entry = entries[i];
			prev_entry = entries[i-1];
			if( (this_entry.day_of_the_year-prev_entry.day_of_the_year) != 1) continue; ///< Previous day is not present
			///< confirmed
			if( (prev_entry.rawValues.at("confirmed") > 0) && (this_entry.rawValues.at("confirmed") >= 2.) ) {
				rate = ( this_entry.rawValues.at("confirmed") - prev_entry.rawValues.at("confirmed") ) / ( prev_entry.rawValues.at("confirmed") );
				entries[i].rates.at("confirmed") = rate;
			}

//			///< new_confirmed
//			if( (prev_entry.rawValues.at("new_confirmed") > 0) && (this_entry.rawValues.at("new_confirmed") >= 0.) ) {
//				rate = this_entry.rawValues.at("new_confirmed") / ( prev_entry.rawValues.at("new_confirmed") );
//				entries[i].rates.at("new_confirmed") = rate;
//			}
			///< deceased
			if( (prev_entry.rawValues.at("deceased") > 0) && (this_entry.rawValues.at("deceased") >= 2.) ) {
				rate = ( this_entry.rawValues.at("deceased") - prev_entry.rawValues.at("deceased") ) / ( prev_entry.rawValues.at("deceased") );
				entries[i].rates.at("deceased") = rate;
			}
//			///< new_deceased
//			if( (prev_entry.rawValues.at("new_deceased") > 0) && (this_entry.rawValues.at("new_deceased") >= 0.) ) {
//				rate = this_entry.rawValues.at("new_deceased") / ( prev_entry.rawValues.at("new_deceased") );
//				entries[i].rates.at("new_deceased") = rate;
//			}
		}
		it->second = entries;
	}
	return;
}

void gDataSample::AddDoubles() {
	///< This method fills the information about the doubling times (days from the previous "half value"
	map<string, vector<gDataEntry>>::iterator it;
	double this_confirmed, this_deceased;
	double prev_confirmed, prev_deceased;
	for(it=DataMap.begin(); it!=DataMap.end(); it++) {
		vector<gDataEntry> entries = it->second;
		for(unsigned int i=entries.size()-1; i>1; i--) {
			this_confirmed = entries[i].rawValues.at("confirmed");
			this_deceased = entries[i].rawValues.at("deceased");
			for(unsigned int j=i-1;j>0; j--) {
				if( (entries[i].day_of_the_year-entries[j].day_of_the_year) < 0 ) continue; ///< Looking only backward
				prev_confirmed = entries[j].rawValues.at("confirmed");
				prev_deceased = entries[j].rawValues.at("deceased");
				///< Finding the "first value" lower that half (and setting a threshold at 100.)
				if( prev_confirmed<(this_confirmed/2.) && this_confirmed>=2. && isnan(entries[i].doubles.at("confirmed")) ) {
					entries[i].doubles.at("confirmed") = 1./((double)entries[i].day_of_the_year-(double)entries[j].day_of_the_year);
				}
				if( prev_deceased<(this_deceased/2.) && this_deceased>=2. && isnan(entries[i].doubles.at("deceased")) ) {
					entries[i].doubles.at("deceased") = 1./((double)entries[i].day_of_the_year-(double)entries[j].day_of_the_year);
				}
				///< Both have been filled -> break
				if( !isnan(entries[i].doubles.at("confirmed")) && !isnan(entries[i].doubles.at("deceased")) ) break;
			}
		}
		it->second = entries;
	}
	return;
}

gDataSample::~gDataSample() {
	// TODO Auto-generated destructor stub
}

