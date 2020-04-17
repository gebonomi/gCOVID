/*
 * gRootDB.cpp
 *
 *  Created on: Mar 27, 2020
 *      Author: germano
 */

#include <gRootDB.h>

gRootDB::gRootDB() {
	// TODO Auto-generated constructor stub
}

gRootDB::gRootDB(const string& filename, const string& flag) {
	///< Opening the Root file
	if(!OpenRootFile(filename,flag)) cout << "gRootDB::gRootDB ---> creator failed " << endl;
}

void gRootDB::Write() {
	ROOT_DB_FILE->cd();
	if(!SetBranchesWrite()) {
		cout << "gRootDB::Write --> could not define branches " << endl;
		return;
	}
	FillWrite();
    tData->Write();
   	return;
}

void gRootDB::Read(const vector<string>& territories) {
	ROOT_DB_FILE->cd();
	SetBranchesRead();
	if(!tData) {
		cout << "gRootDB::Read --> could not open TTree tData" << endl;
	}
	FillRead(territories);
	return;
}

void gRootDB::FillRead(const vector<string>& territories) {
    DataSample = shared_ptr<gDataSample>(new gDataSample);
	DataSample->SetName("ALL");

	map<string, vector<gDataEntry>> DataSampleMap;
	map<string, vector<gDataEntry>>::iterator it;
	unsigned int nentries = tData->GetEntriesFast();
	for(auto i=0; i<nentries; i++) {
		tData->GetEntry(i);
		sample 		= *sample_ptr;
		territory 	= *territory_ptr;
		date 		= *date_ptr;
		///< Load only the territories that are required
		gDataEntry this_entry = FillEntry();
		///< Filling the Map for the sample
		it = DataSampleMap.find(territory);
		if(it!=DataSampleMap.end()) { ///< this territory s already in the map
			it->second.push_back(this_entry);
		} else { ///< new territory -> insert
			vector<gDataEntry> this_vector; this_vector.push_back(this_entry);
			DataSampleMap.insert(std::pair<string, vector<gDataEntry>>(territory,this_vector));
		}
	}
	DataSample->SetDataMap(DataSampleMap);
	return;
}
void gRootDB::FillWrite() {
	map<string, vector<gDataEntry>>::iterator it;
	map<string, vector<gDataEntry>> DataMap = DataSample->GetDataMap();
	for(it=DataMap.begin(); it!=DataMap.end(); it++) {
		territory = it->first;
		vector<gDataEntry> entries = it->second;
		for(unsigned int i=0; i<entries.size(); i++) {
			entry 			        = entries[i];
			sample 					= entry.sample;
			population 				= entry.population;
			date 			        = entry.date;
			day_of_the_year         = entry.day_of_the_year;
            confirmed               = entry.entryMap.at("confirmed");
            new_confirmed           = entry.entryMap.at("new_confirmed");
            actives                 = entry.entryMap.at("actives");
            new_actives             = entry.entryMap.at("new_actives");
            recovered               = entry.entryMap.at("recovered");
            new_recovered           = entry.entryMap.at("new_recovered");
            deceased                = entry.entryMap.at("deceased");
            new_deceased            = entry.entryMap.at("new_deceased");
            tests                   = entry.entryMap.at("tests");
            new_tests               = entry.entryMap.at("new_tests");
            hospitalized_symptoms   = entry.entryMap.at("hospitalized_symptoms");
            hospitalized_intensive  = entry.entryMap.at("hospitalized_intensive");
            hostpitalized_total     = entry.entryMap.at("hostpitalized_total");
            home_isolation          = entry.entryMap.at("home_isolation");
			tData->Fill();
		}
	}
	return;
}

gDataEntry gRootDB::FillEntry() {
	gDataEntry entry;
	entry.sample							   = sample					  ;
	entry.territory                            = territory                ;
	entry.population                           = population               ;
	entry.date                                 = date                     ;
	entry.day_of_the_year                      = day_of_the_year          ;
    entry.entryMap.at("confirmed")             = confirmed                ;
    entry.entryMap.at("new_confirmed")         = new_confirmed            ;
    entry.entryMap.at("actives")               = actives                  ;
    entry.entryMap.at("new_actives")           = new_actives              ;
    entry.entryMap.at("recovered")             = recovered                ;
    entry.entryMap.at("new_recovered")         = new_recovered            ;
    entry.entryMap.at("deceased")              = deceased                 ;
    entry.entryMap.at("new_deceased")          = new_deceased             ;
    entry.entryMap.at("tests")                 = tests                    ;
    entry.entryMap.at("new_tests")             = new_tests                ;
    entry.entryMap.at("hospitalized_symptoms") = hospitalized_symptoms    ;
    entry.entryMap.at("hospitalized_intensive")= hospitalized_intensive   ;
    entry.entryMap.at("hostpitalized_total")   = hostpitalized_total      ;
    entry.entryMap.at("home_isolation")        = home_isolation           ;
    return entry;
}

bool gRootDB::SetBranchesRead() {
	tData = std::unique_ptr<TTree>(dynamic_cast<TTree*>(ROOT_DB_FILE->Get("Data")));
    tData->SetMakeClass(1);
    tData->SetBranchAddress("sample", &sample_ptr);
    tData->SetBranchAddress("territory", &territory_ptr);
    tData->SetBranchAddress("date", &date_ptr);
    tData->SetBranchAddress("day_of_the_year", &day_of_the_year);
    tData->SetBranchAddress("population", &population);
    tData->SetBranchAddress("actives", &actives);
    tData->SetBranchAddress("confirmed", &confirmed);
    tData->SetBranchAddress("deceased", &deceased);
    tData->SetBranchAddress("home_isolation", &home_isolation);
    tData->SetBranchAddress("hospitalized_intensive", &hospitalized_intensive);
    tData->SetBranchAddress("hospitalized_symptoms", &hospitalized_symptoms);
    tData->SetBranchAddress("hostpitalized_total", &hostpitalized_total);
    tData->SetBranchAddress("new_actives", &new_actives);
    tData->SetBranchAddress("new_confirmed", &new_confirmed);
    tData->SetBranchAddress("new_deceased", &new_deceased);
    tData->SetBranchAddress("new_recovered", &new_recovered);
    tData->SetBranchAddress("new_tests", &new_tests);
    tData->SetBranchAddress("recovered", &recovered);
    tData->SetBranchAddress("tests", &tests);
    return true;
}

bool gRootDB::SetBranchesWrite() {
	if(!ROOT_DB_FILE) {
		cout << "gRootDB::SetBranchesWrite --> ROOT_DB_FILE has not been opened " << endl;
		return false;
	}
	///< Defining the output TTrees
	tData 		= unique_ptr<TTree>(new TTree("Data", "ALL [World, Italy] Covid Data"));
	///< tData
	tData->Branch("sample"			, &sample);
	tData->Branch("territory"		, &territory);
	tData->Branch("date"				, &date);
    tData->Branch("day_of_the_year"	, &day_of_the_year	, "day_of_the_year/I");
    tData->Branch("population"		, &population		, "population/D");
    map<string, double>::iterator it;
    for(it=entry.entryMap.begin(); it!=entry.entryMap.end(); it++) {
    	ostringstream type; type << it->first << "/D";
        if(it->first == "confirmed") {
        	tData->Branch(it->first.c_str(), &confirmed, type.str().c_str());
        } else if(it->first == "new_confirmed") {
        	tData->Branch(it->first.c_str(), &new_confirmed, type.str().c_str());
        } else if(it->first == "actives") {
        	tData->Branch(it->first.c_str(), &actives, type.str().c_str());
        } else if(it->first == "new_actives") {
        	tData->Branch(it->first.c_str(), &new_actives, type.str().c_str());
        } else if(it->first == "recovered") {
        	tData->Branch(it->first.c_str(), &recovered, type.str().c_str());
        } else if(it->first == "new_recovered") {
        	tData->Branch(it->first.c_str(), &new_recovered, type.str().c_str());
        } else if(it->first == "deceased") {
        	tData->Branch(it->first.c_str(), &deceased, type.str().c_str());
        } else if(it->first == "new_deceased") {
        	tData->Branch(it->first.c_str(), &new_deceased, type.str().c_str());
        } else if(it->first == "tests") {
        	tData->Branch(it->first.c_str(), &tests, type.str().c_str());
        } else if(it->first == "new_tests") {
        	tData->Branch(it->first.c_str(), &new_tests, type.str().c_str());
        } else if(it->first == "hospitalized_symptoms") {
        	tData->Branch(it->first.c_str(), &hospitalized_symptoms, type.str().c_str());
        } else if(it->first == "hospitalized_intensive") {
        	tData->Branch(it->first.c_str(), &hospitalized_intensive, type.str().c_str());
        } else if(it->first == "hostpitalized_total") {
        	tData->Branch(it->first.c_str(), &hostpitalized_total, type.str().c_str());
        } else if(it->first == "home_isolation") {
        	tData->Branch(it->first.c_str(), &home_isolation, type.str().c_str());
        } else {
            cout << "gRootDB::SetBranchesWrite --> could not set branch for variable " << it->first << endl;
        }
    }
    return true;
}

bool gRootDB::OpenRootFile(const string& filename, const string& flag) {
	ROOT_DB_FILE = std::unique_ptr<TFile>(new TFile(filename.c_str(),flag.c_str()));
	if(!ROOT_DB_FILE) {
		cout << "gRootDB::OpenRootFile ---> could not find " << filename << "root file" << endl;
		return false;
	}
	return true;
}

void gRootDB::Close() {
	if(ROOT_DB_FILE) ROOT_DB_FILE->Close();
}

gRootDB::~gRootDB() {
	// TODO Auto-generated destructor stub
}

