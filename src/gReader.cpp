/*
 * gDataReader.cpp
 *
 *  Created on: Mar 20, 2020
 *      Author: germano
 */

#include <gReader.h>

gDataReader::gDataReader() {
	// TODO Auto-generated constructor stub
}

gDataReader::gDataReader(gCard& Card) {
	myCard 		= Card;
	Init();
}

void gDataReader::ReadData() {

	if(myCard.root_db_read) {
		FillDataFromRootDB();
	} else {
		FillDataFromCSV();
	}

	if(myCard.root_db_create) WriteToRootDB();
}


gDataReader::~gDataReader() {
	// TODO Auto-generated destructor stub
}

void gDataReader::Init() {
	///< Setting path to DB (IN and OUT)
	italy_repo_file 	 = "DATA/ITALIA/dati-andamento-nazionale/dpc-covid19-ita-andamento-nazionale.csv";
	regioni_repo_file 	 = "DATA/ITALIA/dati-regioni/dpc-covid19-ita-regioni.csv";
	province_repo_file 	 = "DATA/ITALIA/dati-province/dpc-covid19-ita-province.csv";
	world_repo_dir 		 = "DATA/WORLD/csse_covid_19_data/csse_covid_19_time_series";
	population_data_file = "DATA/population_by_territory.csv";
	root_dir 			 = "./DATA";
	root_db_file 		 = "COVID_DB.root";
	return;
}

void gDataReader::FillDataFromCSV() {
	///< Reading population data
	pop = ReadPopulation();
	///< Reading values from the CSV repositories
	shared_ptr<gDataSample> World 		= ReadFromWorldCSV(world_repo_dir);
	shared_ptr<gDataSample> Italy 		= ReadFromItalyCSV(italy_repo_file);
	shared_ptr<gDataSample> Regioni 	= ReadFromItalyCSV(regioni_repo_file);
	shared_ptr<gDataSample> Province 	= ReadFromItalyCSV(province_repo_file);
	///< Merging all
	DataSample = shared_ptr<gDataSample>(new gDataSample);
	DataSample->Append(World);
	DataSample->Append(Italy);
	DataSample->Append(Regioni);
	DataSample->Append(Province);
	return;
}

void gDataReader::FillDataFromRootDB() {
	///< if needed open the ROOT DB
	ostringstream s; s << root_dir << "/" << root_db_file;
	myRootDB = std::unique_ptr<gRootDB>(new gRootDB(s.str(), "READ"));
	if(!myRootDB) {
		cout << "gDataReader::FillDataFromRootDB ---> Reading from " << root_dir << "/" << root_db_file << " failed" << endl;
		return;
	}
	myRootDB->Read(myCard.territories);
	DataSample = myRootDB->GetData();
	myRootDB->Close();
	return;
}

void gDataReader::WriteToRootDB() {
	///< if needed open the ROOT DB
	ostringstream s; s << root_dir << "/" << root_db_file;
	myRootDB = std::unique_ptr<gRootDB>(new gRootDB(s.str(), "RECREATE"));
	if(!myRootDB) {
		cout << "gDataReader::WriteToRootDB ---> Writing to " << root_dir << "/" << root_db_file << " failed" << endl;
		return;
	}

	myRootDB->SetData(DataSample);
	myRootDB->Write();
	myRootDB->Close();
	return;
}

shared_ptr<gDataSample> gDataReader::ReadFromItalyCSV(const string& repo_file) {
	string name; ///< Getting the name of the sample from the repository name
	if(repo_file==italy_repo_file) 		name = "Italy";
	if(repo_file==regioni_repo_file) 	name = "Regioni";
	if(repo_file==province_repo_file)	name = "Province";
	///< Loop in the repo_dir to get the files with the data (one per day)
	string legend, line, s;
    ///< Reading the data from the csv files (the "decoding" is done by gDataEntry
    vector<gDataEntry> local_data; ///< Loading all the entries, for all the territories, for all the days
    local_data.clear();
    gDataEntry this_entry;
//    for(auto& p: fs::directory_iterator(repo_dir.c_str())) { ///< Looping on the files in the directory (one per day)
//    	ostringstream filename; filename << p.path();
//        found = filename.str().find("-2020"); ///< Filtering to get only the files with the daily reports
//        if(found==std::string::npos) continue;
    ifstream myfile (repo_file.c_str());
    if (myfile.is_open()) {
		getline(myfile, legend); ///< Reading legend line
		while (getline(myfile, line)) { ///< Reading lines with values
			this_entry.FillValues(name, legend, line); ///< Parsing is done in the gDataEntry class
			if(this_entry.territory.find("aggiornamento")!=std::string::npos) continue; ///< Removing spurious lines
			local_data.push_back(this_entry);
		}
	}
//    }
    ///<  Reordering the data by date and adding some info
    ///< returning a vector of string with the different territories (nations/regions/provinces)
    map<string, vector<gDataEntry>> D = ArrangeByTerritory(local_data);
    shared_ptr<gDataSample> dataSample(new gDataSample);
    dataSample->SetName(name);
    dataSample->SetDataMap(D);
    return dataSample;
}

shared_ptr<gDataSample> gDataReader::ReadFromWorldCSV(const string& repo_dir) { ///< READING THE TIME SERIES
	string name; ///< Getting the name of the sample from the repository name
	if(repo_dir==world_repo_dir) 	name = "World";
	ostringstream file_confirmed; file_confirmed << repo_dir << "/" << "time_series_covid19_confirmed_global.csv";
	ostringstream file_deceased; file_deceased << repo_dir << "/" << "time_series_covid19_deaths_global.csv";
	ostringstream file_recovered; file_recovered << repo_dir << "/" << "time_series_covid19_recovered_global.csv";
	vector<string> lines_confirmed, lines_deceased, lines_recovered;
	///< Loop in the repo_dir to get the files with the data (one per day)
	string legend_confirmed, legend_recovered, legend_deceased, line, s;
    legend_confirmed = FileToLines(file_confirmed.str(), lines_confirmed);
    legend_deceased = FileToLines(file_deceased.str(), lines_deceased);
    legend_recovered = FileToLines(file_recovered.str(), lines_recovered);

    map<string, vector<gDataEntry>> map_total;
    map<string, vector<gDataEntry>> map_confirmed = myParser.LinesToEntries(legend_confirmed, lines_confirmed);
    map<string, vector<gDataEntry>> map_deceased  = myParser.LinesToEntries(legend_deceased, lines_deceased);
    map<string, vector<gDataEntry>> map_recovered = myParser.LinesToEntries(legend_recovered, lines_recovered);

	map<string, vector<gDataEntry>>::iterator it;
    for(it = map_confirmed.begin(); it != map_confirmed.end(); ++it) { ///< Looping on the confirmed ones [and adding here the other into]
    	string sample = "world";
    	string territory = it->first;
    	vector<gDataEntry> entries;

    	vector<gDataEntry> confirmed 	= it->second;
    	vector<gDataEntry> deceased 	= map_deceased.at(territory);
    	vector<gDataEntry> recovered 	= map_recovered.at(territory);

    	for(auto& c:confirmed) {
    		gDataEntry thisEntry = c;
    		thisEntry.sample = sample;
    		for(auto& d:deceased) {
        		if(thisEntry.territory!=d.territory) continue;
        		if(thisEntry.day_of_the_year!=d.day_of_the_year) continue;
        		thisEntry.entryMap.at("deceased") = d.entryMap.at("confirmed");
        	}
        	for(auto& r:recovered) {
        		if(thisEntry.territory!=r.territory) continue;
        		if(thisEntry.day_of_the_year!=r.day_of_the_year) continue;
        		thisEntry.entryMap.at("recovered") = r.entryMap.at("confirmed");
        	}
        	thisEntry.EntryCorrection();		///< Correcting some territories names
        	territory = thisEntry.territory; 	///< Correcting some territories names
        	entries.push_back(thisEntry);
    	}
    	CompleteInfo(entries); ///< Add some pivot/reference points/times
    	map_total.insert(std::pair<string, vector<gDataEntry>>(territory, entries));
    }
    shared_ptr<gDataSample> D(new gDataSample);
    D->SetName(name);
    D->SetDataMap(map_total);
    return D;
}

map<string, vector<gDataEntry>> gDataReader::ArrangeByTerritory(vector<gDataEntry>& local_data) {
	///< Here all the entries of a sample are reorganized, ordered and completed
	///< - 1. the territories are found and stored (in territories)
	///< - 2. for each territory the entries are grouped and stored in a local vector<gDataEntry> (this_territory_data)
	///< - 3. this_territory_data is sorted by time (days) and some reference points added
	///< - 4. this_territory_data is copied into a vector<shared_ptr<gDataEntry>> (this_territory_ptr_data)
	///< - 5. the map with the "territory name" and the "territory data" is created
	vector<string> territories;
	for(auto& e:local_data) {
		bool new_territory = true;
		for(auto& s:territories) {
			if(s==e.territory) { ///< "This territory" already in the list
				new_territory = false;
				break;
			}
		}
		if(new_territory) territories.push_back(e.territory);
	}

	map<string, vector<gDataEntry> > D;
	for(auto& s: territories) { ///< Loop on the territories
    	vector<gDataEntry> this_territory_data;
    	for(auto& e:local_data) { ///< Loop on all the dataEntries
    		if(e.territory != s) continue; ///< not "this territory"
    	    this_territory_data.push_back(e); ///< Adding all the entries to "this territory"
    	}
    	vector<gDataEntry> grouped_data = GroupEntries(this_territory_data); ///< If there are more entries from the same territory -> group them together
    	Sort(grouped_data); ///< Sort by day (time ordered)
    	CompleteInfo(grouped_data); ///< Add some pivot/reference points/times
    	D.insert(std::pair<string, vector<gDataEntry>>(s, grouped_data));
    	this_territory_data.clear(); grouped_data.clear();
    }
	return D;
}

vector<gDataEntry> gDataReader::GroupEntries(vector<gDataEntry>& data) {
	vector<gDataEntry> grouped;
	vector<bool> added;
	for(unsigned int i=0; i<data.size(); i++) {
		added.push_back(0);
	}
	for(unsigned int i=0; i<data.size(); i++) {
		if(added[i]) continue; ///< Not considering entries already added
		gDataEntry entry = data[i];
		for(unsigned int j=i+1; j<data.size(); j++) {
			if(added[j]) continue; ///< Not considering entries already added
			if(data[j].day_of_the_year==entry.day_of_the_year) { ///< same day -> more than 1 entry
				entry.Sum(data[j]);
				added[j] = "true";
			}
		}
		grouped.push_back(entry);
	}
	return grouped;
}

void gDataReader::Sort(vector<gDataEntry>& data) {
	///< Put the data in order of date (day from the start of the year)
	for(unsigned int i=0; i<data.size(); i++) {
		for(unsigned int j=i+1; j<data.size(); j++) {
			if(CompareByDay(data[j],data[i])) { ///< if data[j].day_of_year<data[i].day_of_the_year
				gDataEntry tmp = data[i];
				data[i] = data[j];
				data[j] = tmp;
//				gDataEntry tmp; tmp.Replace(data[i]);
//				tmp.Replace(data[i]);
//				data[i].Replace(data[j]);
//				data[j].Replace(tmp);
			}
		}
	}
	return;
}

void gDataReader::CompleteInfo(vector<gDataEntry>& data) {
	for(unsigned int i=0; i<data.size(); i++) { ///< Adding information
		///< Completing the info
		map<string, double>::iterator it;
		it = pop.find(data[i].territory);
		if(it!=pop.end()) {
			data[i].population = it->second;
		} else {
			cout << "gDataReader::CompleteInfo --> population for " <<  data[i].territory << " has not been found/added" << endl;
		}

		double actives 	 = data[i].entryMap.at("actives");
		double confirmed = data[i].entryMap.at("confirmed");
		double recovered = data[i].entryMap.at("recovered");
		double deceased  = data[i].entryMap.at("deceased");
		double tests	 = data[i].entryMap.at("tests");

		if( isnan(actives) && !isnan(confirmed) && !isnan(recovered) && !isnan(deceased)) {
			data[i].entryMap.at("actives") = data[i].entryMap.at("confirmed") - (data[i].entryMap.at("recovered") + data[i].entryMap.at("deceased"));
		}
		if(data[i].entryMap.at("actives")<0) {
			cout << "gDataReader::CompleteInfo --> actives < 0 [" << endl;
			data[i].Print(1);
		}
		actives 	 = data[i].entryMap.at("actives");
		if(i>0) {
			if( isnan(data[i].entryMap.at("new_confirmed")) && !isnan(confirmed) )
				data[i].entryMap.at("new_confirmed") 	= data[i].entryMap.at("confirmed") 	- data[i-1].entryMap.at("confirmed");
			if( isnan(data[i].entryMap.at("new_actives")) && !isnan(actives) )
				data[i].entryMap.at("new_actives") 	= data[i].entryMap.at("actives") 	- data[i-1].entryMap.at("actives");
			if( isnan(data[i].entryMap.at("new_recovered")) && !isnan(recovered) )
				data[i].entryMap.at("new_recovered")= data[i].entryMap.at("recovered") 	- data[i-1].entryMap.at("recovered");
			if( isnan(data[i].entryMap.at("new_deceased")) && !isnan(deceased) )
				data[i].entryMap.at("new_deceased") = data[i].entryMap.at("deceased") 	- data[i-1].entryMap.at("deceased");
			if( isnan(data[i].entryMap.at("new_tests")) && !isnan(tests) )
				data[i].entryMap.at("new_tests") 	= data[i].entryMap.at("tests") 		- data[i-1].entryMap.at("tests");
		}
		data[i].StandardizeDate();
	}
	return;
}

map<string, double> gDataReader::ReadPopulation() {
	map<string, double> Pop;
    string line, code, territory;
    double v;
    ifstream myfile (population_data_file.c_str());
    if (myfile.is_open()) {
        getline (myfile,line); ///< Get legend line
        while ( getline (myfile,line) ) {
        	size_t code_pos = line.find(";");
        	code = line.substr(0, code_pos);
        	size_t terr_pos = line.find(";",code_pos+1);
        	territory = line.substr(code_pos+1, terr_pos-code_pos-1);
        	istringstream iss(line.substr(terr_pos+1,line.size()-terr_pos));
        	iss >> v;
            Pop.insert(std::pair<string, double>(territory,v));
        }
        myfile.close();
    }  else {
        cout << "gDataReader::ReadPopulation -> File " << population_data_file <<  " could not be found" << endl;
    }
    return Pop;
}

string gDataReader::FileToLines(const string& filename, vector<string>& lines) {
	string line;
	string legend;
	ifstream myfile (filename.c_str());
	if (myfile.is_open()) {
		getline (myfile, legend);								///< Reading legend line
		while ( getline (myfile,line) ) { ///< Reading lines with values
			lines.push_back(line);
		}
		myfile.close();
	} else {
		cout << "gDataReader::FileToLines --> file " << filename << " has not been found " << endl;
	}
	return legend;
}

