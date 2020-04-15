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

void gCard::Reset() {
	isCard				= false;
	territories.clear();
	variables.clear();
	italy				= false;
	world				= false;
	day_zero_what.clear();
	day_zero_howmany	= -1;
	var_or_terr				= false;
	norm				= 0;
	verbose				= false;
	batch				= true;
	root_db_read		= false;
	root_db_create		= false;
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
        	///< Looking for territories
            found = line.find("territories");
            if(found!=std::string::npos) {
            	DecodeTerritories(line);
            }
        	///< Looking for variables
            found = line.find("variables");
            if(found!=std::string::npos) {
            	DecodeVariables(line);
            }
            ///< Looking for verbose
            found = line.find("verbose");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> verbose;		}
            ///< Looking for batch
            found = line.find("batch");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> batch;			}
            ///< Looking for day_zero_what
            found = line.find("day_0_what");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> day_zero_what;	}
            ///< Looking for day_zero_howmany
            found = line.find("day_0_howmany");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> day_zero_howmany;}
            ///< Looking for var_or_terr
            found = line.find("var_or_terr");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> var_or_terr; 			}
            ///< Looking for norm
            found = line.find("norm");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> norm; 			}
            ///< Looking for root_db_create
            found = line.find("root_db_create");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> root_db_create; }
            ///< Looking for root_db_read
            found = line.find("root_db_read");
            if(found!=std::string::npos) { istringstream iss(line); iss >> dummy >> root_db_read; }
        }
        ///< Setting other flags/fixing some inconsistency
        if(root_db_read) root_db_create = false; ///< Cannot create the ROOT DB if we are reading from it
        myfile.close();
    }  else {
        cout << "gCard::ReadValues() -> File " << filename <<  " could not be found" << endl;
        return false;
    }
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
	return true;
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

void gCard::Print() {
	if(italy) cout << "italy \t" << italy << endl;
	if(world) cout << "Showing world" << endl;
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
    cout << "var_or_terr \t" << var_or_terr << endl;
    cout << "norm \t" << norm << endl;
	cout << "verbose \t" << verbose << endl;
	cout << "batch \t" << batch << endl;
	cout << "root_db_read \t" << root_db_read << endl;
	cout << "root_db_create \t" << root_db_create << endl;
	return;
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
