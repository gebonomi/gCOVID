/*
 * gCard.cpp
 *
 *  Created on: Mar 20, 2020
 *      Author: germano
 */

#include <gCard.h>

gCard::gCard() {
	// TODO Auto-generated constructor stub
	SetProvince();
}

gCard::~gCard() {
	// TODO Auto-generated destructor stub
}

void gCard::Init() {
	///< general
	isCard				= false;
	verbose				= false;
	root_db_read		= false;
	root_db_create		= false;
	///< plotter
	plotter_flag		= false;
	plotter_norm		= false;
	territories.clear();
	variables.clear();
	italy				= false;
	world				= false;
	day_zero_what.clear();
	day_zero_howmany	= -1;
	var_or_terr			= false;
	show_raw			= false;
	show_rates			= false;
	show_doubling		= false;
	///< analyzer
	analyzer_flag		= false;
	pop_limit			= 0.;
	day_min				= 0.;
	day_max				= 10000;
	sample.clear();
	rank_n_stop			= 0;

	rank_raw_flag		= false;
	rank_raw_what.clear();
	rank_raw_norm		= false;

	rank_rate_flag		= false;
	rank_rate_what.clear();
	rank_rate_lim		= 0;
	rank_rate_thrs		= 0.;

	rank_double_flag	= false;
	rank_double_what.clear();
	rank_double_lim		= 0;
	rank_double_thrs	= 0.;

	return;
}

bool gCard::ReadValues() {
    string line, dummy;
    ifstream myfile (filename.c_str());
    if (myfile.is_open()) {
        size_t found;
        while ( getline (myfile,line) ) {
        	///< Skipping comment lines
        	if(line.find("#")!=std::string::npos) continue;

            found = line.find("verbose");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> verbose;		}

            found = line.find("root_db_read");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> root_db_read; 	}

            ///< -------------------- PLOTTER ------------------------------------------------------
            ///< Plotter general
            found = line.find("plotter_flag");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> plotter_flag;		}

            found = line.find("plotter_norm");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> plotter_norm; 	}

            found = line.find("territories");			///< Looking for territories
            if(found!=std::string::npos) {
            	DecodeTerritories(line);
            }

            found = line.find("variables");				///< Looking for variables
            if(found!=std::string::npos) {
            	DecodeVariables(line);
            }

            found = line.find("day_0_what");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> day_zero_what;	}

            found = line.find("day_0_howmany");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> day_zero_howmany;}

            found = line.find("var_or_terr");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> var_or_terr; 	}

            found = line.find("show_rates");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> show_rates; 	}

            found = line.find("show_doubling");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> show_doubling; 	}

            found = line.find("show_raw");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> show_raw; 	}

            ///< -------------------- ANALYZER ------------------------------------------------------
            ///< Analyzer general
            found = line.find("analyzer");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> analyzer_flag; 		}

            found = line.find("sample");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> sample; 		}

            found = line.find("pop_limit");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> pop_limit; 		}

            string date_min;
            found = line.find("date_min");
            if(found!=std::string::npos) {
            	istringstream iss(line); iss >> dummy >> date_min;
                day_min = DaysFrom_1_1_2020(date_min);
            }

            string date_max;
            found = line.find("date_max");
            if(found!=std::string::npos) {
            	istringstream iss(line); iss >> dummy >> date_max;
                day_max = DaysFrom_1_1_2020(date_max);
            }

            ///< Analyzer ranking
            found = line.find("rank_n_stop");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> rank_n_stop; 		}

            found = line.find("rank_raw_flag");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> rank_raw_flag; 		}

            found = line.find("rank_raw_what");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> rank_raw_what; 		}

            found = line.find("rank_raw_norm");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> rank_raw_norm; 		}

            found = line.find("rank_rate_flag");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> rank_rate_flag; 	}

            found = line.find("rank_rate_what");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> rank_rate_what; 	}

            found = line.find("rank_rate_lim");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> rank_rate_lim; 		}

            found = line.find("rank_rate_thrs");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> rank_rate_thrs; 	}

            found = line.find("rank_double_flag");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> rank_double_flag; 	}

            found = line.find("rank_double_what");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> rank_double_what; 	}

            found = line.find("rank_double_lim");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> rank_double_lim; 	}

            found = line.find("rank_double_thrs");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> rank_double_thrs; 	}

        }
        myfile.close();
    }  else {
        cout << "gCard::ReadValues() -> File " << filename <<  " could not be found" << endl;
        return false;
    }
//    Print();
    return SanityCheck();
}

void gCard::DecodeTerritories(const string& line) {
	string s, dummy;
	size_t first_space = line.find(" ");
	s = line.substr(first_space+1,line.size()-first_space); ///< Removing "territories"
	///< Check for the ":"
	size_t columns = s.find(":");
	if(columns!=std::string::npos) {
		string sample = s.substr(0,columns); ///< Removing first letter (to search for) to avoid confusion with upper/lower case in gCOVID.card
		if(sample.find("orld") != std::string::npos) 	world = true;
		if(sample.find("taly") != std::string::npos) 	italy = true;
		if(sample.find("egioni") != std::string::npos) 	italy = true;
		if(sample.find("rovince") != std::string::npos) italy = true;
	} else {
		cout << "gCard::DecodeTerritories --> Could not find : in territories field" << endl;
		return;
	}
	string values = s.substr(columns+1, s.size()-columns-1);
	istringstream iss_values(values);
	size_t pos=0,next_pos=0;
	while( pos != std::string::npos) {
		next_pos = values.find(",", pos+1);
		if(next_pos!=std::string::npos) {
			territories.push_back(values.substr(pos, next_pos-pos));
		} else {
			territories.push_back(values.substr(pos, values.size()-pos));
			break;
		}
		pos = next_pos+1;
	}
	return;
}

void gCard::DecodeVariables(const string& line) {
	string values, dummy;
	istringstream iss(line); iss >> dummy >> values; // dummy == "variables"
	istringstream iss_values(values);
	while( getline(iss_values, dummy, ',') ) {
		variables.push_back(dummy);
	}
	return;
}

bool gCard::SanityCheck() {
	///< Setting other flags/fixing some inconsistency
    root_db_create = !root_db_read; ///< If not reading from the ROOT DB, read from CSV and create the ROOT DB
    ///< Check variables
    gDataEntry entry;
    map<string, double> rawValues = entry.rawValues;

    for(auto& v:variables) {
    	bool ok = false;
    	for(auto it=rawValues.begin(); it!=rawValues.end(); it++) {
    		if(v==it->first) ok = true;
    	}
    	if(!ok) {
    		cout << "gCard::SanityCheck --> variable " << v << " is not an available one" << endl;
    		return false;
    	}
    }
    ///< Overiding variables based on available data from different samples (for the PLOTTER)
    bool check_province = false;
    for(auto& t:territories) {
        for(auto& p:province) {
            if(t==p) {
                check_province = true;
                break;
            }
        }
    }
    if(check_province) { ///< Overriding because "province" has only the confirmed
		variables.clear();
		variables.push_back("confirmed");
		variables.push_back("new_confirmed");
	}
	if(world) { ///< Overriding because "world" doesn't have some categories
		vector<string>::iterator it = variables.begin();
		for( ; it!=variables.end(); ) {
			if(*it=="tests" || *it =="new_tests" || *it == "hospitalized_symptoms" ||
			   *it =="hospitalized_intensive" || *it=="hostpitalized_total" || *it=="home_isolation") {
					it = variables.erase(it);
			} else {
				it++;
			}
		}
	}
    ///< Overiding variables based on available data from different samples (for the ANALYZER)
	if(sample=="province") {
		if(rank_raw_what!="confirmed"||rank_raw_what!="new_confirmed") rank_raw_what = "confirmed";
	}
	return true;
}
void gCard::Print() {
    if(territories.size()>0) {
        cout << "  territories: \t| ";
        for(auto& t:territories) cout << t << " | ";
        cout << endl;
    }
    if(variables.size()>0) {
        cout << "  variables: \t| ";
        for(auto& t:variables) cout << t << " | ";
        cout << endl;
    }
	cout << "General flags --------------- " << endl;
	cout << " verbose \t " << verbose << endl;
	cout << " root_db_read \t " << root_db_read << endl;
	cout << " root_db_create \t " << root_db_create << endl;
	cout << " italy \t " << italy << endl;
	cout << " world \t " << world << endl;
	cout << "Plotter flags --------------- " << endl;
	cout << "plotter_flag \t" << plotter_flag << endl;
    cout << "day_zero_what \t" << day_zero_what << endl;
	cout << "day_zero_howmany \t" << day_zero_howmany << endl;
	cout << "var_or_terr \t" << var_or_terr << endl;
	cout << "plotter_norm \t" << plotter_norm << endl;
	cout << "show_raw \t" << show_raw << endl;
	cout << "show_rates \t" << show_rates << endl;
	cout << "show_doubling \t" << show_doubling << endl;
	cout << "Analyzer flags --------------- " << endl;
	cout << "analyzer_flag \t " << analyzer_flag << endl;
	cout << "sample \t " << sample << endl;
	cout << "pop_limit \t " << pop_limit << endl;
	cout << "day_min \t " << day_min << endl;
	cout << "day_max \t " << day_max << endl;
	cout << "rank_n_stop \t " << rank_n_stop << endl;
	cout << "rank_raw_flag \t " << rank_raw_flag << endl;
	cout << "rank_raw_what \t " << rank_raw_what << endl;
	cout << "rank_raw_norm \t " << rank_raw_norm << endl;
	cout << "rank_rate_flag \t " << rank_rate_flag << endl;
	cout << "rank_rate_what \t " << rank_rate_what << endl;
	cout << "rank_rate_lim \t " << rank_rate_lim << endl;
	cout << "rank_double_flag \t " << rank_double_flag << endl;
	cout << "rank_double_what \t " << rank_double_what << endl;
	cout << "rank_double_lim \t " << rank_double_lim << endl;
	return;
}

time_t gCard::DateStringToTimeT(const string& date) {
	///< It constructs the time_t info from the date string
	time_t now = time(nullptr);
    tm *timeinfo = localtime(&now);
    size_t first = date.find("/");
    if(first == string::npos) { ///< The date DO NOT cointais the "-"
    	cout << "gCard::DateStringToTimeT --> expecting / as date separator -> change format in " << filename << endl;
    } else { ///< The date CONTAINS the "/" (e.g. 1/31/2020)
    	size_t second = date.find("/",first+1); ///< finding next "/"
    	timeinfo->tm_mday   = atoi(date.substr(0, first).c_str());
    	timeinfo->tm_mon  	= atoi(date.substr(first+1,second-first).c_str()) -1;
    	timeinfo->tm_year  	= atoi(date.substr(second+1, date.size()-second-1).c_str()) ;
    	if(timeinfo->tm_year>2000) {
    		timeinfo->tm_year = timeinfo->tm_year - 1900; ///< e.g. 2020 -> 120 (timeinfo is from 1900)
    	} else {
    		timeinfo->tm_year = timeinfo->tm_year + 100; ///< e.g. 20 -> 120 (timeinfo is from 1900)
    	}
    	timeinfo->tm_hour  = 8  ; ///< setting 8 (hours are not important
    	timeinfo->tm_min   = 0  ; ///< setting to 0 (minutes are not important)
    	timeinfo->tm_sec   = 0  ; ///< setting to 0 (seconds are not important)
    	timeinfo->tm_isdst = -1;  ///< A value of -1 causes mgTime to check whether DST is in effect or not.
    }
    return mktime(timeinfo);
}

int gCard::DaysFrom_1_1_2020(const string& date) {
	time_t entry_time = DateStringToTimeT(date);
	time_t year_start = DateStringToTimeT("01/01/2020");

	double diff_s = difftime(entry_time,year_start);
	return (int)(diff_s/86400.);
}


void gCard::SetProvince() {
    province.push_back("Agrigento");
    province.push_back("Alessandria");
    province.push_back("Ancona");
    province.push_back("Aosta");
    province.push_back("Arezzo");
    province.push_back("Ascoli Piceno");
    province.push_back("Asti");
    province.push_back("Avellino");
    province.push_back("Bari");
    province.push_back("Barletta-Andria-Trani");
    province.push_back("Belluno");
    province.push_back("Benevento");
    province.push_back("Bergamo");
    province.push_back("Biella");
    province.push_back("Bologna");
    province.push_back("Bolzano");
    province.push_back("Brescia");
    province.push_back("Brindisi");
    province.push_back("Cagliari");
    province.push_back("Caltanissetta");
    province.push_back("Campobasso");
    province.push_back("Caserta");
    province.push_back("Catania");
    province.push_back("Catanzaro");
    province.push_back("Chieti");
    province.push_back("Como");
    province.push_back("Cosenza");
    province.push_back("Cremona");
    province.push_back("Crotone");
    province.push_back("Cuneo");
    province.push_back("Enna");
    province.push_back("Fermo");
    province.push_back("Ferrara");
    province.push_back("Firenze");
    province.push_back("Foggia");
    province.push_back("Forli-Cesena");
    province.push_back("Frosinone");
    province.push_back("Genova");
    province.push_back("Gorizia");
    province.push_back("Grosseto");
    province.push_back("Imperia");
    province.push_back("Isernia");
    province.push_back("L'Aquila");
    province.push_back("La Spezia");
    province.push_back("Latina");
    province.push_back("Lecce");
    province.push_back("Lecco");
    province.push_back("Livorno");
    province.push_back("Lodi");
    province.push_back("Lucca");
    province.push_back("Macerata");
    province.push_back("Mantova");
    province.push_back("Massa Carrara");
    province.push_back("Matera");
    province.push_back("Messina");
    province.push_back("Milano");
    province.push_back("Modena");
    province.push_back("Monza e della Brianza");
    province.push_back("Napoli");
    province.push_back("Novara");
    province.push_back("Nuoro");
    province.push_back("Oristano");
    province.push_back("Padova");
    province.push_back("Palermo");
    province.push_back("Parma");
    province.push_back("Pavia");
    province.push_back("Perugia");
    province.push_back("Pesaro e Urbino");
    province.push_back("Pescara");
    province.push_back("Piacenza");
    province.push_back("Pisa");
    province.push_back("Pistoia");
    province.push_back("Pordenone");
    province.push_back("Potenza");
    province.push_back("Prato");
    province.push_back("Ragusa");
    province.push_back("Ravenna");
    province.push_back("Reggio di Calabria");
    province.push_back("Reggio nell'Emilia");
    province.push_back("Rieti");
    province.push_back("Rimini");
    province.push_back("Roma");
    province.push_back("Rovigo");
    province.push_back("Salerno");
    province.push_back("Sassari");
    province.push_back("Savona");
    province.push_back("Siena");
    province.push_back("Siracusa");
    province.push_back("Sondrio");
    province.push_back("Sud Sardegna");
    province.push_back("Taranto");
    province.push_back("Teramo");
    province.push_back("Terni");
    province.push_back("Torino");
    province.push_back("Trapani");
    province.push_back("Trento");
    province.push_back("Treviso");
    province.push_back("Trieste");
    province.push_back("Udine");
    province.push_back("Varese");
    province.push_back("Venezia");
    province.push_back("Verbano-Cusio-Ossola");
    province.push_back("Vercelli");
    province.push_back("Verona");
    province.push_back("Vibo Valentia");
    province.push_back("Vicenza");
    province.push_back("Viterbo");
	return;
}
