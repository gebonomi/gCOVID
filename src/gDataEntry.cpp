/*
 * gDataEntry.cpp
 *
 *  Created on: Mar 20, 2020
 *      Author: germano
 */

#include <gDataEntry.h>

gDataEntry::gDataEntry() {
	// TODO Auto-generated constructor stub
	Init();
}

gDataEntry::~gDataEntry() {
	// TODO Auto-generated destructor stub
}

void gDataEntry::Init() {
	sample					= "";
	territory 				= "";
	population				= std::numeric_limits<double>::quiet_NaN();
	date 					= "";
	day_of_the_year         = -1; ///< 0-365 [2020 has 366 days]
    rawValues.insert(std::pair<string, double>("confirmed",		        std::numeric_limits<double>::quiet_NaN()));
    rawValues.insert(std::pair<string, double>("new_confirmed",	        std::numeric_limits<double>::quiet_NaN()));
    rawValues.insert(std::pair<string, double>("actives",		        std::numeric_limits<double>::quiet_NaN()));
    rawValues.insert(std::pair<string, double>("new_actives",	        std::numeric_limits<double>::quiet_NaN()));
    rawValues.insert(std::pair<string, double>("recovered",		        std::numeric_limits<double>::quiet_NaN()));
    rawValues.insert(std::pair<string, double>("new_recovered",	        std::numeric_limits<double>::quiet_NaN()));
    rawValues.insert(std::pair<string, double>("deceased",		        std::numeric_limits<double>::quiet_NaN()));
    rawValues.insert(std::pair<string, double>("new_deceased",	        std::numeric_limits<double>::quiet_NaN()));
    rawValues.insert(std::pair<string, double>("tests",			        std::numeric_limits<double>::quiet_NaN()));
    rawValues.insert(std::pair<string, double>("new_tests",		        std::numeric_limits<double>::quiet_NaN()));
    rawValues.insert(std::pair<string, double>("hospitalized_symptoms",	std::numeric_limits<double>::quiet_NaN()));
    rawValues.insert(std::pair<string, double>("hospitalized_intensive",std::numeric_limits<double>::quiet_NaN()));
    rawValues.insert(std::pair<string, double>("hostpitalized_total",	std::numeric_limits<double>::quiet_NaN()));
    rawValues.insert(std::pair<string, double>("home_isolation",	    std::numeric_limits<double>::quiet_NaN()));

    rates.insert(std::pair<string, double>("confirmed",		        	std::numeric_limits<double>::quiet_NaN()));
    rates.insert(std::pair<string, double>("deceased",		        	std::numeric_limits<double>::quiet_NaN()));

    doubles.insert(std::pair<string, double>("confirmed",		        std::numeric_limits<double>::quiet_NaN()));
    doubles.insert(std::pair<string, double>("deceased",		        std::numeric_limits<double>::quiet_NaN()));

    //	confirmed; 			///< This is the cumulative of the positive cases
	//	new_confirmed; 		///< This is the daily (tested) positive cases
	//	actives;   			///< This is the cumulative of the active cases (confirmed - deceased - recovered)
	//	new_actives;		///< This is the daily increase in active cases (new_confirmed - new_deceased - new_recovered)
	//	recovered;			///< This is the cumulative of the recovered cases
	//	new_recovered;		///< This is the daily recovered cases
	//	deceased;			///< This is the cumulative of the deceased cases
	//	new_deceased;		///< This is the daily deceased cases
	//	tests;				///< This is the cumulative of the performed tests
	//	new_tests;			///< This is the daily new tests
}

void gDataEntry::FillValues(const string& name, const string& legend, const string& line) {
	if(name=="Italy") 		FillValuesItaly(legend, line);
	if(name=="Regioni") 	FillValuesRegioni(legend, line);
	if(name=="Province") 	FillValuesProvince(legend, line);
}

void gDataEntry::FillValuesItaly(const string& legend, const string& line) {
	Init();
	sample = "italy";
	string dummy;
    istringstream s;
	istringstream iss_line(line);
	getline(iss_line, date, ',');           ///< data
    day_of_the_year = DaysFrom_1_1_2020(date);
	getline(iss_line, territory, ',');         ///< stato,
	getline(iss_line, dummy, ',');          ///< ricoverati_con_sintomi,
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("hospitalized_symptoms");
	getline(iss_line, dummy, ',');          ///< terapia_intensiva,
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("hospitalized_intensive");
	getline(iss_line, dummy, ',');          ///< totale_ospedalizzati,
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("hostpitalized_total");
	getline(iss_line, dummy, ',');          ///< isolamento_domiciliare,
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("home_isolation");
	getline(iss_line, dummy, ',');          ///< totale_positivi	,
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("actives");
    getline(iss_line, dummy, ',');          ///< variazione_totale_positivi,
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("new_actives");
    getline(iss_line, dummy, ',');          ///< nuovi_positivi,
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("new_confirmed");
    getline(iss_line, dummy, ',');          ///< dimessi_guariti,
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("recovered");
	getline(iss_line, dummy, ',');          ///< deceduti,
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("deceased");
	getline(iss_line, dummy, ',');          ///< totale_casi,
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("confirmed");
	getline(iss_line, dummy, ',');          ///< tamponi
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("tests");
        getline(iss_line, dummy, ',');  ///< casi_testati
    return;
}

void gDataEntry::FillValuesRegioni(const string& legend, const string& line) {
	Init();
	sample = "regioni";
	string dummy;
    istringstream s;
	istringstream iss_line(line);
	getline(iss_line, date, ',');		///< data,
    day_of_the_year = DaysFrom_1_1_2020(date);
	getline(iss_line, dummy, ',');		///< stato,
	getline(iss_line, dummy, ',');		///< codice regione
	getline(iss_line, territory, ',');		///< denominazione_regione,
	getline(iss_line, dummy, ',');		///< lat,
	getline(iss_line, dummy, ',');		///< long,
	getline(iss_line, dummy, ',');		///< ricoverati_con_sintomi,
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("hospitalized_symptoms");
	getline(iss_line, dummy, ',');		///< terapia_intensiva,
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("hospitalized_intensive");
	getline(iss_line, dummy, ',');		///< totale_ospedalizzati,
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("hostpitalized_total");
	getline(iss_line, dummy, ',');		///< isolamento_domiciliare,
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("home_isolation");
    getline(iss_line, dummy, ',');          ///< totale_positivi	,
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("actives");
    getline(iss_line, dummy, ',');          ///< variazione_totale_positivi,
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("new_actives");
    getline(iss_line, dummy, ',');          ///< nuovi_positivi,
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("new_confirmed");
    getline(iss_line, dummy, ',');          ///< dimessi_guariti,
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("recovered");
    getline(iss_line, dummy, ',');          ///< deceduti,
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("deceased");
    getline(iss_line, dummy, ',');          ///< totale_casi,
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("confirmed");
    getline(iss_line, dummy, ',');          ///< tamponi
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("tests");
    return;
}

void gDataEntry::FillValuesProvince(const string& legend, const string& line) {
	Init();
	sample = "province";
	string dummy;
    istringstream s;
	istringstream iss_line(line);
	getline(iss_line, date, ',');		///< data,
    day_of_the_year = DaysFrom_1_1_2020(date);
	getline(iss_line, dummy, ',');		///< stato,
	getline(iss_line, dummy, ',');		///< codice regione
	getline(iss_line, dummy, ',');		///< denominazione_regione,
	getline(iss_line, dummy, ',');		///< codice_provincia,
	getline(iss_line, territory, ',');		///< denominazione_provincia,
	getline(iss_line, dummy, ',');		///< sigla_provincia,
	getline(iss_line, dummy, ',');		///< lat,
	getline(iss_line, dummy, ',');		///< long,
	getline(iss_line, dummy, ',');      ///< totale_casi,
        s.clear(); s.str(dummy.c_str());
        s >> rawValues.at("confirmed");
    if(territory.find("Cesena")!=std::string::npos) { ///< Correcting for accent
    	territory = "Forli-Cesena";
    }
    return;
}

void gDataEntry::Print(int option = 0) {
	map<string, double>::iterator it;
    double confirmed                   = rawValues.at("confirmed");
    double new_confirmed               = rawValues.at("new_confirmed");
    double actives                     = rawValues.at("actives");
    double new_actives                 = rawValues.at("new_actives");
    double recovered                   = rawValues.at("recovered");
//    double new_recovered               = rawValues.at("new_recovered");
    double deceased                    = rawValues.at("deceased");
    double new_deceased                = rawValues.at("new_deceased");
    double tests                       = rawValues.at("tests");
//    double new_tests                   = rawValues.at("new_tests");
//    double hospitalized_symptoms       = rawValues.at("hospitalized_symptoms");
//    double hospitalized_intensive      = rawValues.at("hospitalized_intensive");
//    double hostpitalized_total         = rawValues.at("hostpitalized_total");
//    double home_isolation              = rawValues.at("home_isolation");

	switch(option) {
	   case 0: ///< short - default
		   cout << sample  << ": ";
           cout << territory << " (" << date << ") ";
           cout << "confirmed " << rawValues.at("confirmed") << endl;
			break;
       case 1: ///< short with data in the DB
           cout << territory;
           cout << " (" << date << "): ";
//           cout << " (day " << setw(3) << day_of_the_year << "):";
           cout << " confirmed " 	<< confirmed;
           cout << " (" 			<< (confirmed/population)*1.e6 << ")";
           cout << " deceased " 	<< deceased;
           cout << " (" 			<< (deceased/population)*1.e6 << ")";
           cout << " recovered " 	<< recovered;
           cout << " (" 			<< (recovered/population)*1.e6 << ")";
           if(rawValues.at("tests")>0) cout << " tests " << tests;
           cout << endl;
           break;
       case 10: ///< Optimized for gAnalyzer (show raw rank)
    	   cout << left  << setw(11) << setfill(' ')  << territory.substr(0,10);
    	   cout << left  << setw(9)  << setfill(' ')  << date;
    	   ///< Confirmed
    	   cout << right << setw(10) << setfill(' ')  << fixed << setprecision(0) << confirmed
        			<< left  << " ("
    				<< right << setw(6)  <<  setfill(' ') << (confirmed/population)*1.e6 << ")";
    	   ///< Deceased
    	   cout << right << setw(8) << setfill(' ')  << deceased
        			<< left  << " ("
    				<< right << setw(5)  <<  setfill(' ') << (deceased/population)*1.e6 << ")";
    	   ///< Actives
    	   cout << right << setw(9) << setfill(' ')  << actives;
    	   ///< new_confirmed
    	   cout << right << setw(7) << setfill(' ')  << new_confirmed;
    	   ///< new_deceased
    	   cout << right << setw(5) << setfill(' ')  << new_deceased;
    	   ///< new_actives
    	   cout << right << setw(6) << setfill(' ')  << new_actives;
    	   ///< pop (in milions)
    	   cout << right << setw(8) << setfill(' ')  << setprecision(1) << population/1.e6;
    	   cout << endl;
    	   break;
       case 20: ///< Optimized for gAnalyzer (show rate rank)
    	   cout << left  << setw(11) << setfill(' ')  << territory.substr(0,10);
    	   cout << left  << setw(9)  << setfill(' ')  << date;
    	   ///< Confirmed
    	   cout << right << setw(10) << setfill(' ')  << fixed << setprecision(0) << confirmed
        			<< left  << " ("
    				<< right << setw(4)  <<  setfill(' ') << fixed << setprecision(1) << rates.at("confirmed")*100. << "\%)";
    	   ///< Deceased
    	   cout << right << setw(8) << setfill(' ')  << fixed << setprecision(0) << deceased
        			<< left  << " ("
    				<< right << setw(4)  <<  setfill(' ') << fixed << setprecision(1) << rates.at("deceased")*100. << "%)";
    	   ///< new_confirmed
    	   cout << right << setw(7) << setfill(' ')  << fixed << setprecision(0) << new_confirmed;
    	   ///< new_deceased
    	   cout << right << setw(5) << setfill(' ')  << new_deceased;
    	   ///< pop (in milions)
    	   cout << right << setw(8) << setfill(' ')  << setprecision(1) << population/1.e6;
    	   cout << endl;
    	   break;
       case 30: ///< Optimized for gAnalyzer (show 1./doubling time rank)
    	   cout << left  << setw(11) << setfill(' ')  << territory.substr(0,10);
    	   cout << left  << setw(9)  << setfill(' ')  << date;
    	   ///< Confirmed
    	   cout << right << setw(10) << setfill(' ')  << fixed << setprecision(0) << confirmed
        			<< left  << " ("
    				<< right << setw(4)  <<  setfill(' ') << fixed << setprecision(2) << doubles.at("confirmed") << ")";
    	   ///< Deceased
    	   cout << right << setw(8) << setfill(' ')  << fixed << setprecision(0) << deceased
        			<< left  << " ("
    				<< right << setw(4)  <<  setfill(' ') << fixed << setprecision(2) << doubles.at("deceased") << ")";
    	   ///< new_confirmed
    	   cout << right << setw(7) << setfill(' ')  << fixed << setprecision(0) << new_confirmed;
    	   ///< new_deceased
    	   cout << right << setw(5) << setfill(' ')  << new_deceased;
    	   ///< pop (in milions)
    	   cout << right << setw(8) << setfill(' ')  << setprecision(1) << population/1.e6;
    	   cout << endl;
    	   break;
       case 99: ///< Only "day zero"
    	   cout << left  << setw(11) << setfill(' ')  << territory.substr(0,10);
    	   cout << left  << setw(9)  << setfill(' ')  << day_of_the_year << " (" << date << ")";
    	   cout << " -> confirmed " << confirmed << " -> deceased " << deceased;
    	   cout << endl;
    	   break;
	}
}

void gDataEntry::StandardizeDate() {
	time_t entry_time = DateStringToTimeT(date);
	tm *ltm = localtime(&entry_time);
	ostringstream s;
	s 	<< 1900 + ltm->tm_year << "/"
		<< std::setw(2) << std::setfill('0') << 1 + ltm->tm_mon << "/"
		<< std::setw(2) << std::setfill('0') << ltm->tm_mday;
	date = s.str();
	return;
}

///< Prinvate utilities of gDataEntry
time_t gDataEntry::DateStringToTimeT(const string& date) {
	///< It constructs the time_t info from the date string
	///< Some manipulations are needed for the World database where the dates are written in different formats:
	///< for example 1/31/20 17:00, or 1/31/2020 17:00, or 2020-02-11T16:43:06 ... (what a mess!)
	time_t now = time(nullptr);
    tm *timeinfo = localtime(&now);
    size_t ini = date.find("/");
    if(ini == string::npos) { ///< The date DO NOT cointais the "/"
    	timeinfo->tm_year  = atoi(date.substr(0, 4).c_str())-1900;
    	timeinfo->tm_mon   = atoi(date.substr(5, 2).c_str())-1;
    	timeinfo->tm_mday  = atoi(date.substr(8, 2).c_str())  ;
    	timeinfo->tm_hour  = atoi(date.substr(11,2).c_str())  ;
    	timeinfo->tm_min   = atoi(date.substr(14,2).c_str())  ;
    	timeinfo->tm_sec   = atoi(date.substr(17,2).c_str())  ;
    	timeinfo->tm_isdst = -1; // A value of -1 causes mgTime to check whether DST is in effect or not.
    } else { ///< The date CONTAINS the "/" (e.g. 1/31/2020)
    	timeinfo->tm_mon   = atoi(date.substr(0, ini).c_str())-1;
    	size_t end = date.find("/",ini+1); ///< finding next "/"
    	timeinfo->tm_mday  = atoi(date.substr(ini+1,end-ini).c_str())  ;
    	size_t space = date.find(" "); ///< Finding the " " between day and hours (e.g. 1/31/20 17:00)
    	timeinfo->tm_year  = atoi(date.substr(end+1, space-end-1).c_str()) ;
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

int gDataEntry::DaysFrom_1_1_2020(const string& date) {
	time_t entry_time = DateStringToTimeT(date);
	time_t year_start = DateStringToTimeT("2020-01-01 00:00:00");

	double diff_s = difftime(entry_time,year_start);
	return (int)(diff_s/86400.);
}

string gDataEntry::CleanString(const string& s) {
	string clean_s = s;
	if(s.size()==0) clean_s = "0"; ///< Fix empty strings --> set no info such as "0"
	///< Remove carriage return (newline)
	size_t found = clean_s.find("\r");
	if(found != string::npos) {
		clean_s.erase(found,clean_s.size()-found);
	}
	return clean_s;
}
string gDataEntry::CleanLine(const string& line) {
	///< Some entries in the world database are such "Korea, South" instead of Korea South
	///< This method removes both the " and the ,
	string clean_line(line.c_str());
	///< Find the double quotes, if any
	vector<size_t> to_remove;
	vector<size_t> where_double_quotes;
	size_t pos = line.find("\""); 		// Get the first occurrence
	while( pos != std::string::npos) 	// Repeat till end is reached
	{
		where_double_quotes.push_back(pos);
		to_remove.push_back(pos);
		pos = line.find("\"", pos+1); ///< Get the next occurrence
	}
	if(where_double_quotes.size()==0) return clean_line;
	if ((where_double_quotes.size() % 2) != 0) {
		cout << "gDataEntry::CleanLine ---> ERROR - even number of double quotes (\") have been found in \n " << line << endl;
	}
	vector<size_t> where_commas; ///< Where commas between double quotes
	for(unsigned int i=0; i<where_double_quotes.size()-1; i+=2) {
		size_t com = line.find(",", where_double_quotes[i]); // Get the first occurrence
		while( com != std::string::npos && com<where_double_quotes[i+1]) 	// Repeat till end is reached
		{
			where_commas.push_back(com);
			to_remove.push_back(com);
			com = line.find(",", com+1); ///< Get the next occurrence
		}
	}
	std::sort(to_remove.rbegin(),to_remove.rend()); ///< reverse order to remove last occurences first (to avoid string resize)

	for(unsigned int i=0; i<to_remove.size(); i++) {
		clean_line.erase(to_remove[i],1);
	}
	return clean_line;
}

void gDataEntry::EntryCorrection() {
	if(territory=="Korea South") territory = "South Korea";
	if(territory=="Mainland China") territory = "China";
	if(territory=="United Kingdom") territory = "UK";
	if(territory=="Viet Nam") territory = "Vietnam";
	return;
}

void gDataEntry::SumRaw(const gDataEntry& other) {
	///< Checking that we are indeed adding entries of the same territory and date
    if(territory!=other.territory) {
        cout << "gDataEntry::SumRaw -> trying to sum entries of different territories [" << territory << ", " << other.territory << "]" << endl;
    }
    if(day_of_the_year!=other.day_of_the_year) {
        cout << "gDataEntry::SumRaw -> trying to sum entries of different days [" << date << ", " << other.date << "]" << endl;
    }
    map<string, double>::iterator it;
    for(it=rawValues.begin(); it!=rawValues.end(); it++) {
        if( isnan(it->second) || isnan(other.rawValues.at(it->first)) ) continue;
        it->second += other.rawValues.at(it->first);
    }
    return;
}

void gDataEntry::AddRates(const gDataEntry& other) {
	///< Adding the info about the increase/decrease rates
	///< Checking that we are indeed adding entries of the same territory and date
    if(territory!=other.territory) {
        cout << "gDataEntry::AddRates -> trying to sum entries of different territories [" << territory << ", " << other.territory << "]" << endl;
    }
    if(day_of_the_year!=other.day_of_the_year) {
        cout << "gDataEntry::AddRates -> trying to sum entries of different days [" << date << ", " << other.date << "]" << endl;
    }
    map<string, double>::iterator it;
    for(it=rates.begin(); it!=rates.end(); it++) {
        if( isnan(other.rates.at(it->first)) ) continue;
        it->second += other.rates.at(it->first);
    }
    return;
}
