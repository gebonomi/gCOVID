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
 	if(myCard.verbose) cout << "gAnalyzer::Analyze --> ThroughData " << endl;
 	ThroughData();
 	if(myCard.verbose) cout << "gAnalyzer::Analyze --> Ordering " << endl;
	if(myCard.rank_raw_flag) 	OrderRawBy();
	if(myCard.rank_rate_flag) 	OrderRateBy();
	if(myCard.rank_double_flag) OrderDoubleBy();
 	return;
}

void gAnalyzer::ShowResults() {
 	if(myCard.verbose) cout << "gAnalyzer::ShowResults --> " << endl;
	if(myCard.rank_raw_flag) 	ShowRankingRaw();
	if(myCard.rank_rate_flag) 	ShowRankingRate();
	if(myCard.rank_double_flag) ShowRankingDouble();
	return;
}

void gAnalyzer::ShowRankingRaw() {
	if(RankingRaw.size()==0) {
		cout << "gAnalyzer::ShowRankingRaw --> RankingRaw is empty" << endl;
		return;
	}
	cout << "<-----------------------------------------------------------------------------------------> " << endl;
	cout << "Ranking territories per ---> "  << myCard.rank_raw_what;
	if(myCard.rank_raw_norm) cout << " per milion inhabitants ";
	cout << " <--- (in brackets values per 1.000.000 inhabitants)" << endl;
	cout << left << setw(3) << setfill(' ') << "#" << "- ";
	cout << left  << setw(11) << setfill(' ')  << "Territory";
	cout << left  << setw(10)  << setfill(' ')  << "date";
	///< Confirmed
	cout << left << setw(17) << setfill(' ')  << "  confirmed";
	cout << right << setw(10) << setfill(' ')  << "deceased";
	cout << right << setw(17) << setfill(' ')  << "actives";
	cout << right << setw(7) << setfill(' ')  << "newC";
	cout << right << setw(6) << setfill(' ')  << "newD";
	cout << right << setw(6) << setfill(' ')  << "newA";
	cout << right << setw(8) << setfill(' ')  << "pop (M)";
	cout << endl;

	int i=0;
	for(auto& entry:RankingRaw) {
		if(i>=myCard.rank_n_stop) break;
		if(myCard.sample==entry.sample||myCard.sample=="ALL") {
			cout << left << setw(3) << setfill(' ') << i+1 << "- "; entry.Print(10);
			i++;
		}
	}
	cout << endl;
	return;
}

void gAnalyzer::ShowRankingRate() {
	if(RankingRate.size()==0) {
		cout << "gAnalyzer::ShowRankingRate --> RankingRate.size() is " << RankingRate.size() << endl;
		return;
	}
	cout << "<-----------------------------------------------------------------------------------------> " << endl;
	cout << "Ranking territories per rate increase of --> "  << myCard.rank_rate_what << " <-- (rates are between \"brackets\")" << endl;
	cout << left << setw(3) << setfill(' ') << "#" << "- ";
	cout << left  << setw(11) << setfill(' ')  << "Territory";
	cout << left  << setw(9)  << setfill(' ')  << "date";
	///< Confirmed
	cout << left << setw(16) << setfill(' ')  << "  confirmed";
	cout << right << setw(11) << setfill(' ') << "deceased";
	cout << right << setw(15) << setfill(' ')  << "newC";
	cout << right << setw(5) << setfill(' ')  << "newD";
	cout << right << setw(10) << setfill(' ')  << "pop (M)";
	cout << endl;

	int i=0;
	for(auto& entry:RankingRate) {
		if(i>=myCard.rank_n_stop) break;
		if(myCard.sample==entry.sample||myCard.sample=="ALL") {
			cout << left << setw(3) << setfill(' ') << i+1 << "- "; entry.Print(20);
			i++;
		}
	}
	cout << endl;
	return;
}

void gAnalyzer::ShowRankingDouble() {
	if(RankingDouble.size()==0) {
		cout << "gAnalyzer::ShowRankingDouble --> RankingDouble is empty" << endl;
		return;
	}
	cout << "<-----------------------------------------------------------------------------------------> " << endl;
	cout << "Ranking territories per 1./doubling time of --> "  << myCard.rank_double_what << " <-- (1./doubling are between \"brackets\")" << endl;
	cout << left << setw(3) << setfill(' ') << "#" << "- ";
	cout << left  << setw(11) << setfill(' ')  << "Territory";
	cout << left  << setw(9)  << setfill(' ')  << "date";
	///< Confirmed
	cout << left << setw(16) << setfill(' ')  << "  confirmed";
	cout << right << setw(10) << setfill(' ') << "deceased";
	cout << right << setw(14) << setfill(' ')  << "newC";
	cout << right << setw(5) << setfill(' ')  << "newD";
	cout << right << setw(10) << setfill(' ')  << "pop (M)";
	cout << endl;

	int i=0;
	for(auto& entry:RankingDouble) {
		if(i>=myCard.rank_n_stop) break;
		if(myCard.sample==entry.sample||myCard.sample=="ALL") {
			cout << left << setw(3) << setfill(' ') << i+1 << "- "; entry.Print(30);
			i++;
		}
	}
	cout << endl;
	return;
}


void gAnalyzer::OrderRawBy() {
 	if(myCard.verbose) cout << "gAnalyzer::OrderRawBy --> " << endl;
	///< This method order the vector of RankingRaw with respects to the "myCard.rank_raw_what"
 	if(RankingRaw.size()<2) {
 		cout << "gAnalyzer::OrderRawBy --> RankingRaw.size() == " << RankingRaw.size() << endl;
 		return;
 	}
	for(unsigned int i=0; i<RankingRaw.size()-1; i++) {
		for(unsigned int j=i+1; j<RankingRaw.size(); j++) {
			double v1 = RankingRaw[i].rawValues.at(myCard.rank_raw_what);
			if(myCard.rank_raw_norm==1&&RankingRaw[i].population!=0.) v1 /= RankingRaw[i].population;
			double v2 = RankingRaw[j].rawValues.at(myCard.rank_raw_what);
			if(myCard.rank_raw_norm==1&&RankingRaw[j].population!=0.) v2 /= RankingRaw[j].population;
			if(v2>v1) iter_swap(RankingRaw.begin() + i, RankingRaw.begin() + j);
		}
	}
}

void gAnalyzer::OrderRateBy() {
 	if(myCard.verbose) cout << "gAnalyzer::OrderRateBy --> " << endl;
	///< This method order the vector of RankingRate with respects to the "myCard.rank_rate_what"
 	if(RankingRate.size()<2) {
 		cout << "gAnalyzer::OrderRateBy --> RankingRate.size() == " << RankingRate.size() << endl;
 		return;
 	}
 	for(unsigned int i=0; i<RankingRate.size()-1; i++) {
		for(unsigned int j=i+1; j<RankingRate.size(); j++) {
			double v1 = RankingRate[i].rates.at(myCard.rank_rate_what);
			double v2 = RankingRate[j].rates.at(myCard.rank_rate_what);
			if(v2>v1) iter_swap(RankingRate.begin() + i, RankingRate.begin() + j);
		}
	}
 	return;
}

void gAnalyzer::OrderDoubleBy() {
 	if(myCard.verbose) cout << "gAnalyzer::OrderDoubleBy --> " << endl;
	///< This method order the vector of RankingDouble with respects to the "myCard.rank_double_what"
 	if(RankingDouble.size()<2) {
 		cout << "gAnalyzer::OrderDoubleBy --> RankingDouble.size() == " << RankingDouble.size() << endl;
 		return;
 	}
	for(unsigned int i=0; i<RankingDouble.size()-1; i++) {
		for(unsigned int j=i+1; j<RankingDouble.size(); j++) {
			double v1 = RankingDouble[i].doubles.at(myCard.rank_double_what);
			double v2 = RankingDouble[j].doubles.at(myCard.rank_double_what);
			if(v2>v1) iter_swap(RankingDouble.begin() + i, RankingDouble.begin() + j);
		}
	}
}

void gAnalyzer::ThroughData() {
	///< This method create a map (territory, highest entry).
	///< For each territory it extracts the entry with the higher value of the "myCard.rank_raw_what"
	map<string, gDataEntry> HigherEntryMap;
//	map<string, vector<gDataEntry>>::iterator it;
	string t, v;
	///< For each territory find the gDataEntry with the highest value of the myCard.rank_raw_what
	map<string, vector<gDataEntry>> dataMap = DataSample->GetDataMap();
	for(auto it = dataMap.begin(); it!=dataMap.end(); it++) {
		vector<gDataEntry> entries = it->second;
		for(auto i=0; i<entries.size(); i++) {
			if(entries[i].day_of_the_year<myCard.day_min+1 || entries[i].day_of_the_year>myCard.day_max+1) continue; ///< Filtering by date
			///< ----------------------------------------------------------------
//			///< Adding entries to RankingRate and RankingDouble
			if(entries[i].rawValues.at(myCard.rank_rate_what)>myCard.rank_rate_lim) {
				if(entries[i].rates.at(myCard.rank_rate_what)>myCard.rank_rate_thrs) RankingRate.push_back(entries[i]);
			}
			if(entries[i].rawValues.at(myCard.rank_double_what)>myCard.rank_double_lim) {
				if(entries[i].doubles.at(myCard.rank_double_what)>myCard.rank_double_thrs) RankingDouble.push_back(entries[i]);
			}
			///< ----------------------------------------------------------------
			t = entries[i].territory;
			auto jt = HigherEntryMap.find(t);
			if(jt!=HigherEntryMap.end()) {
				double v1 = jt->second.rawValues.at(myCard.rank_raw_what);
				double v2 = entries[i].rawValues.at(myCard.rank_raw_what);
				if(myCard.rank_raw_norm==1&&jt->second.population!=0.) v1 /= jt->second.population;
				if(myCard.rank_raw_norm==1&&entries[i].population!=0.) v2 /= entries[i].population;
				if(v2>v1) jt->second = entries[i];
			} else { ///< New territory
				if(isnan(entries[i].rawValues.at(myCard.rank_raw_what))) continue; ///< Not listing if data are not available
				if(entries[i].population<myCard.pop_limit) continue;
				HigherEntryMap.insert(std::pair<string, gDataEntry>(t, entries[i]));
			}
		}
	}
	for(auto it=HigherEntryMap.begin(); it!=HigherEntryMap.end(); it++) RankingRaw.push_back(it->second);
	return;
}

gAnalyzer::~gAnalyzer() {
	// TODO Auto-generated destructor stub
}

