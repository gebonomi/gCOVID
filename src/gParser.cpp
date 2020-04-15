/*
 * gParser.cpp
 *
 *  Created on: Mar 27, 2020
 *      Author: germano
 */

#include <gParser.h>

gParser::gParser() {
	// TODO Auto-generated constructor stub
}

gParser::~gParser() {
	// TODO Auto-generated destructor stub
}


map<string, vector<gDataEntry>> gParser::LinesToEntries(const string& legend, vector<string>& lines) {
	int where_territory = -1, where_first_date = -1;
	///< ##########
	///< Find the "titles" in the legend, the position of the "territory", the position of the first date
	///< ##########
	vector<string> titles = LegendToTitles(legend, where_territory, where_first_date); ///< Decoding "legend"

	///< ##########
	///< Creating the map<territory, vector<gDataEntry>> -> one entry per day
	///< ##########
	vector<gDataEntry> tmp;
	map<string, vector<gDataEntry>> D;
	map<string, vector<gDataEntry>>::iterator it;
	for(auto& line:lines) { ///< Reading lines with values
		vector<string> values = LineToValues(line);
		istringstream iss;
		string territory = values[where_territory];
		///< Check if more than one entry for this territory (e.g. Washington, US - New York, US ...)
		///< In case remove it and save the data in a tmp vector --> then add to the new entry
		tmp.clear();
		it = D.find(territory);
		if(it != D.end()) { ///< Add all the lines with the territory values into a tmp vector and remove the "line" from the map
			tmp = it->second;
			D.erase(it);
		}
		vector<gDataEntry> entries;
		for(unsigned iv=where_first_date; iv<values.size(); iv++) {
			gDataEntry thisEntry;
			thisEntry.territory = territory;
			thisEntry.date		= titles[iv];
			thisEntry.day_of_the_year = thisEntry.DaysFromYearStart(thisEntry.date);
			iss.clear(); iss.str(values[iv]);
	        iss >> thisEntry.entryMap.at("confirmed");
	        for(auto& t:tmp) {
	        	if(thisEntry.date==t.date) thisEntry.Sum(t);
	        }
	        entries.push_back(thisEntry);
		}
		D.insert(std::pair<string, vector<gDataEntry>>(territory, entries));
	}
	return D;
}


string gParser::RemoveNewLine(const string& s) {
	string clean_s = s;
	///< Remove carriage return (newline)
	size_t found = clean_s.find("\r");
	if(found != string::npos) {
		clean_s.erase(found,clean_s.size()-found);
	}
	return clean_s;
}

string gParser::EmptyStringToZero(const string& s) {
	string clean_s = s;
	if(s.size()==0) clean_s = "0"; ///< Fix empty strings --> set no info such as "0"
	return clean_s;
}

string gParser::CleanLine(const string& line) {
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

vector<string> gParser::LegendToTitles(const string& line, int& where_territory, int& where_first_date) {
	vector<string> titles = LineToValues(line);
	for(unsigned int it=0; it<titles.size(); it++) {
		if(titles[it].find("Country") != std::string::npos) where_territory = it;
		if(titles[it].find("20") !=std::string::npos && where_first_date==-1) where_first_date = it;
	}
	if(where_territory==-1||where_first_date==-1) cout << "gParser::LegendToTitles ---> SOMETHING is FISHY " << endl;
	return titles;
}

vector<string> gParser::LineToValues(const string& line) {
	vector<string> values;
	string s1 = CleanLine(line); ///< Remove double quotes and commas between double quotes (",") [e.g. "Korea, South"]
	string s2 = RemoveNewLine(s1);
	istringstream iss_line(s2);
	string dummy;
	while( getline(iss_line, dummy, ',') ) {
		values.push_back(EmptyStringToZero(dummy));
	}
	return values;
}
