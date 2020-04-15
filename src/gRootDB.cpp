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
    tALL->Write();
    tWorld->Write();
    tItaly->Write();
   	return;
}

void gRootDB::Read(const vector<string>& territories) {
	ROOT_DB_FILE->cd();
	SetBranchesRead();
	if(!tALL) {
		cout << "gRootDB::Read --> could not open TTree tALL" << endl;
	}
	FillRead(territories);
	return;
}

void gRootDB::FillRead(const vector<string>& territories) {
	World = shared_ptr<gDataSample>(new gDataSample);
    Italy = shared_ptr<gDataSample>(new gDataSample);
    ALL = shared_ptr<gDataSample>(new gDataSample);
	World->SetName("World");
	Italy->SetName("Italy");
	ALL->SetName("ALL");

	map<string, vector<gDataEntry>> WorldMap;
	map<string, vector<gDataEntry>> ItalyMap;
	map<string, vector<gDataEntry>> ALLMap;
	map<string, vector<gDataEntry>>::iterator it;
	unsigned int nentries = tALL->GetEntriesFast();
	for(auto i=0; i<nentries; i++) {
		tALL->GetEntry(i);
		sample 		= *sample_ptr;
		territory 	= *territory_ptr;
		date 		= *date_ptr;
		///< Load only the territories that are required
		bool ok = false;
		for(auto& t:territories) {
			if(territory==t) ok = true;
		}
		if(!ok) continue;
		gDataEntry this_entry = FillEntry();
		///< Filling the Map for the sample
		if(sample=="world") {
			it = WorldMap.find(territory);
			if(it!=WorldMap.end()) { ///< this territory s already in the map
				it->second.push_back(this_entry);
			} else { ///< new territory -> insert
				vector<gDataEntry> this_vector; this_vector.push_back(this_entry);
				WorldMap.insert(std::pair<string, vector<gDataEntry>>(territory,this_vector));
			}
		}
		if(sample=="italy") {
			it = ItalyMap.find(territory);
			if(it!=ItalyMap.end()) { ///< this territory s already in the map
				it->second.push_back(this_entry);
			} else { ///< new territory -> insert
				vector<gDataEntry> this_vector; this_vector.push_back(this_entry);
				ItalyMap.insert(std::pair<string, vector<gDataEntry>>(territory,this_vector));
			}
		}
		it = ALLMap.find(territory);
		if(it!=ALLMap.end()) { ///< this territory s already in the map
			it->second.push_back(this_entry);
		} else { ///< new territory -> insert
			vector<gDataEntry> this_vector; this_vector.push_back(this_entry);
			ALLMap.insert(std::pair<string, vector<gDataEntry>>(territory,this_vector));
		}
	}
	World->SetDataMap(WorldMap);
	Italy->SetDataMap(ItalyMap);
	ALL->SetDataMap(ALLMap);
	return;
}
void gRootDB::FillWrite() {
	map<string, vector<gDataEntry>>::iterator it;
	map<string, vector<gDataEntry>> DataMap = World->GetDataMap();
	sample = "world";
	for(it=DataMap.begin(); it!=DataMap.end(); it++) {
		territory = it->first;
		vector<gDataEntry> entries = it->second;
		for(unsigned int i=0; i<entries.size(); i++) {
			entry 			        = entries[i];
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
            tWorld->Fill();
			tALL->Fill();
		}
	}
	DataMap = Italy->GetDataMap();
	sample = "italy";
	for(it=DataMap.begin(); it!=DataMap.end(); it++) {
		territory = it->first;
		vector<gDataEntry> entries = it->second;
		for(unsigned int i=0; i<entries.size(); i++) {
			entry 			        = entries[i];
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
            tItaly->Fill();
			tALL->Fill();
		}
	}
	return;
}

gDataEntry gRootDB::FillEntry() {
	gDataEntry entry;
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
	tALL = std::unique_ptr<TTree>(dynamic_cast<TTree*>(ROOT_DB_FILE->Get("ALL")));
    tALL->SetMakeClass(1);
    tALL->SetBranchAddress("sample", &sample_ptr);
    tALL->SetBranchAddress("territory", &territory_ptr);
    tALL->SetBranchAddress("date", &date_ptr);
    tALL->SetBranchAddress("day_of_the_year", &day_of_the_year);
    tALL->SetBranchAddress("population", &population);
    tALL->SetBranchAddress("actives", &actives);
    tALL->SetBranchAddress("confirmed", &confirmed);
    tALL->SetBranchAddress("deceased", &deceased);
    tALL->SetBranchAddress("home_isolation", &home_isolation);
    tALL->SetBranchAddress("hospitalized_intensive", &hospitalized_intensive);
    tALL->SetBranchAddress("hospitalized_symptoms", &hospitalized_symptoms);
    tALL->SetBranchAddress("hostpitalized_total", &hostpitalized_total);
    tALL->SetBranchAddress("new_actives", &new_actives);
    tALL->SetBranchAddress("new_confirmed", &new_confirmed);
    tALL->SetBranchAddress("new_deceased", &new_deceased);
    tALL->SetBranchAddress("new_recovered", &new_recovered);
    tALL->SetBranchAddress("new_tests", &new_tests);
    tALL->SetBranchAddress("recovered", &recovered);
    tALL->SetBranchAddress("tests", &tests);
    return true;
}

bool gRootDB::SetBranchesWrite() {
	if(!ROOT_DB_FILE) {
		cout << "gRootDB::SetBranchesWrite --> ROOT_DB_FILE has not been opened " << endl;
		return false;
	}
	///< Defining the output TTrees
	tALL 		= unique_ptr<TTree>(new TTree("ALL", "ALL Covid Data"));
	tWorld 		= unique_ptr<TTree>(new TTree("World", "World Covid Data"));
	tItaly 		= unique_ptr<TTree>(new TTree("Italy", "Italian Covid Data"));
	///< tALL
	tALL->Branch("sample"			, &sample);
	tALL->Branch("territory"		, &territory);
	tALL->Branch("date"				, &date);
    tALL->Branch("day_of_the_year"	, &day_of_the_year	, "day_of_the_year/I");
    tALL->Branch("population"		, &population		, "population/D");
	///< tWorld
	tWorld->Branch("sample"			, "string"			, &sample);
	tWorld->Branch("territory"		, "string"			, &territory);
	tWorld->Branch("date"			, "string"			, &date);
    tWorld->Branch("day_of_the_year", &day_of_the_year	, "day_of_the_year/I");
    tWorld->Branch("population"		, &population		, "population/D");
	///< tItaly
	tItaly->Branch("sample"			, "string"			, &sample);
	tItaly->Branch("territory"		, "string"			, &territory);
	tItaly->Branch("date"			, "string"			, &date);
    tItaly->Branch("day_of_the_year", &day_of_the_year	, "day_of_the_year/I");
    tItaly->Branch("population"		, &population		, "population/D");
    map<string, double>::iterator it;
    for(it=entry.entryMap.begin(); it!=entry.entryMap.end(); it++) {
    	ostringstream type; type << it->first << "/D";
        if(it->first == "confirmed") {
        	tALL->Branch(it->first.c_str(), &confirmed, type.str().c_str());
        	tWorld->Branch(it->first.c_str(), &confirmed, type.str().c_str());
        	tItaly->Branch(it->first.c_str(), &confirmed, type.str().c_str());
        } else if(it->first == "new_confirmed") {
        	tALL->Branch(it->first.c_str(), &new_confirmed, type.str().c_str());
        	tWorld->Branch(it->first.c_str(), &new_confirmed, type.str().c_str());
        	tItaly->Branch(it->first.c_str(), &new_confirmed, type.str().c_str());
        } else if(it->first == "actives") {
        	tALL->Branch(it->first.c_str(), &actives, type.str().c_str());
        	tWorld->Branch(it->first.c_str(), &actives, type.str().c_str());
        	tItaly->Branch(it->first.c_str(), &actives, type.str().c_str());
        } else if(it->first == "new_actives") {
        	tALL->Branch(it->first.c_str(), &new_actives, type.str().c_str());
        	tWorld->Branch(it->first.c_str(), &new_actives, type.str().c_str());
        	tItaly->Branch(it->first.c_str(), &new_actives, type.str().c_str());
        } else if(it->first == "recovered") {
        	tALL->Branch(it->first.c_str(), &recovered, type.str().c_str());
        	tWorld->Branch(it->first.c_str(), &recovered, type.str().c_str());
        	tItaly->Branch(it->first.c_str(), &recovered, type.str().c_str());
        } else if(it->first == "new_recovered") {
        	tALL->Branch(it->first.c_str(), &new_recovered, type.str().c_str());
        	tWorld->Branch(it->first.c_str(), &new_recovered, type.str().c_str());
        	tItaly->Branch(it->first.c_str(), &new_recovered, type.str().c_str());
        } else if(it->first == "deceased") {
        	tALL->Branch(it->first.c_str(), &deceased, type.str().c_str());
        	tWorld->Branch(it->first.c_str(), &deceased, type.str().c_str());
        	tItaly->Branch(it->first.c_str(), &deceased, type.str().c_str());
        } else if(it->first == "new_deceased") {
        	tALL->Branch(it->first.c_str(), &new_deceased, type.str().c_str());
        	tWorld->Branch(it->first.c_str(), &new_deceased, type.str().c_str());
        	tItaly->Branch(it->first.c_str(), &new_deceased, type.str().c_str());
        } else if(it->first == "tests") {
        	tALL->Branch(it->first.c_str(), &tests, type.str().c_str());
        	tWorld->Branch(it->first.c_str(), &tests, type.str().c_str());
        	tItaly->Branch(it->first.c_str(), &tests, type.str().c_str());
        } else if(it->first == "new_tests") {
        	tALL->Branch(it->first.c_str(), &new_tests, type.str().c_str());
        	tWorld->Branch(it->first.c_str(), &new_tests, type.str().c_str());
        	tItaly->Branch(it->first.c_str(), &new_tests, type.str().c_str());
        } else if(it->first == "hospitalized_symptoms") {
        	tALL->Branch(it->first.c_str(), &hospitalized_symptoms, type.str().c_str());
        	tWorld->Branch(it->first.c_str(), &hospitalized_symptoms, type.str().c_str());
        	tItaly->Branch(it->first.c_str(), &hospitalized_symptoms, type.str().c_str());
        } else if(it->first == "hospitalized_intensive") {
        	tALL->Branch(it->first.c_str(), &hospitalized_intensive, type.str().c_str());
        	tWorld->Branch(it->first.c_str(), &hospitalized_intensive, type.str().c_str());
        	tItaly->Branch(it->first.c_str(), &hospitalized_intensive, type.str().c_str());
        } else if(it->first == "hostpitalized_total") {
        	tALL->Branch(it->first.c_str(), &hostpitalized_total, type.str().c_str());
        	tWorld->Branch(it->first.c_str(), &hostpitalized_total, type.str().c_str());
        	tItaly->Branch(it->first.c_str(), &hostpitalized_total, type.str().c_str());
        } else if(it->first == "home_isolation") {
        	tALL->Branch(it->first.c_str(), &home_isolation, type.str().c_str());
        	tWorld->Branch(it->first.c_str(), &home_isolation, type.str().c_str());
        	tItaly->Branch(it->first.c_str(), &home_isolation, type.str().c_str());
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

