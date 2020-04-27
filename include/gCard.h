/*
 * gCard.h
 *
 *  Created on: Mar 20, 2020
 *      Author: germano
 * This class read and store the information of the gCOVID.card (external file to customize program running)
*/

#ifndef INCLUDE_GCARD_H_
#define INCLUDE_GCARD_H_

#include "myCommon.h"
#include "gDataEntry.h"

class gCard {
public:
	gCard();
	void Init();
	bool ReadValues();
	void Print();
	virtual ~gCard();


	bool   			isCard;
	///< General flags
	bool   			verbose;
	bool   			root_db_read;
	bool   			root_db_create;
	bool   			italy;
	bool   			world;
	///< Plotter flags
	bool   			plotter_flag;
	vector<string> 	territories;
	vector<string> 	variables;
	string 			day_zero_what;
	int	   			day_zero_howmany;
	bool   			var_or_terr;
	bool    		plotter_norm;
	bool			show_raw;
	bool			show_rates;
	bool			show_doubling;
	///< Analyzer flags
	bool			analyzer_flag;
	string 			sample;
	double			pop_limit;
	int				day_min;
	int				day_max;
	int				rank_n_stop;

	bool			rank_raw_flag;
	string 			rank_raw_what;
	bool			rank_raw_norm;

	bool			rank_rate_flag;
	string 			rank_rate_what;
	int				rank_rate_lim;
	double			rank_rate_thrs;

	bool			rank_double_flag;
	string 			rank_double_what;
	int				rank_double_lim;
	double			rank_double_thrs;

private:
	string filename = "gCOVID.card";
	vector<string> province;

	bool SanityCheck();
	void DecodeTerritories(const string&);
	void DecodeVariables(const string&);
	void SetProvince();
	time_t 	DateStringToTimeT(const string&);
	int  	DaysFrom_1_1_2020(const string&);
};

#endif /* INCLUDE_GCARD_H_ */
