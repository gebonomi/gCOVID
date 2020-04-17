/*
 * gAnalyzer.cpp
 *
 *  Created on: Apr 16, 2020
 *      Author: germano
 */

#include <gAnalyzer.h>

gAnalyzer::gAnalyzer() {
	// TODO Auto-generated constructor stub
}

gAnalyzer::gAnalyzer(const gCard& card) {
	myCard = card;
}

void gAnalyzer::Analyze() {
 	if(DataSample->isEmpty()) {
 		cout << "gAnalyzer::Analyze --> DataSample is empty " << endl;
 		return;
 	}

 	vector<gDataEntry> y = Rank(myCard.rank);
 	 	cout << "Ranking territories per ---> "  << myCard.rank;
 	 	if(myCard.norm) cout << " per milion inhabitants ";
 	 	cout << " <--- (in brackets values per 1.000.000 inhabitants)" << endl;
 	cout << left << setw(3) << setfill(' ') << "#" << "- ";
 	cout << left  << setw(11) << setfill(' ')  << "Territory";
 	cout << left  << setw(10)  << setfill(' ')  << "date";
	   ///< Confirmed
 		cout << left << setw(17) << setfill(' ')  << "  confirmed";
//	   ///< Deceased
	   cout << right << setw(10) << setfill(' ')  << "deceased";
//	   ///< Actives
	   cout << right << setw(17) << setfill(' ')  << "actives";
//	   ///< new_confirmed
	   cout << right << setw(7) << setfill(' ')  << "newC";
//	   ///< new_deceased
	   cout << right << setw(6) << setfill(' ')  << "newD";
//	   ///< new_actives
	   cout << right << setw(6) << setfill(' ')  << "newA";
	   cout << endl;

 	int i=0;
 	for(auto& x:y) {
 		if(i>=myCard.nstop) break;
 		if(myCard.sample==x.sample||myCard.sample=="ALL") {
 			cout << left << setw(3) << setfill(' ') << i+1 << "- "; x.Print(2);
 			i++;
 		}
 	}
	return;
}

vector<gDataEntry> gAnalyzer::Rank(const string& variable) {
	vector<gDataEntry> entries;
	map<string, gDataEntry> HigherEntryMap = HigherEntry(variable);
	//< Now order the "Ranking"
	map<string, gDataEntry>::iterator it;
	for(it=HigherEntryMap.begin(); it!=HigherEntryMap.end(); it++) {
		entries.push_back(it->second);
	}
	OrderBy(entries, variable);
	return entries;
}

void gAnalyzer::OrderBy(vector<gDataEntry>& entries, const string& variable) {
	for(unsigned int i=0; i<entries.size()-1; i++) {
		for(unsigned int j=i+1; j<entries.size(); j++) {
			double v1 = entries[i].entryMap.at(variable);
			if(myCard.norm==1&&entries[i].population!=0.) v1 /= entries[i].population;
			double v2 = entries[j].entryMap.at(variable);
			if(myCard.norm==1&&entries[j].population!=0.) v2 /= entries[j].population;
			if(v2>v1) iter_swap(entries.begin() + i, entries.begin() + j);
		}
	}
}

map<string, gDataEntry> gAnalyzer::HigherEntry(const string& variable) {
	map<string, gDataEntry> HigherEntryMap;
//	map<string, vector<gDataEntry>>::iterator it;
	string t, v;
	///< For each territory find the gDataEntry with the highest value of the variable
	map<string, vector<gDataEntry>> dataMap = DataSample->GetDataMap();
	for(auto it = dataMap.begin(); it!=dataMap.end(); it++) {
		vector<gDataEntry> entries = it->second;
		for(auto i=0; i<entries.size(); i++) {
			t = entries[i].territory;
			auto jt = HigherEntryMap.find(t);
			if(jt!=HigherEntryMap.end()) {
				double v1 = jt->second.entryMap.at(variable);
				double v2 = entries[i].entryMap.at(variable);
				if(myCard.norm==1&&jt->second.population!=0.) v1 /= jt->second.population;
				if(myCard.norm==1&&entries[i].population!=0.) v2 /= entries[i].population;
				if(v2>v1) jt->second = entries[i];
			} else { ///< New territory
				if(isnan(entries[i].entryMap.at(myCard.rank))) continue; ///< Not listing if data are not available
				if(entries[i].population<myCard.pop_limit) continue;
				HigherEntryMap.insert(std::pair<string, gDataEntry>(t, entries[i]));
			}
		}
	}
	return HigherEntryMap;
}

gAnalyzer::~gAnalyzer() {
	// TODO Auto-generated destructor stub
}

