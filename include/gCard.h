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

class gCard {
public:
	gCard();
	void Reset();
	bool ReadValues();
	void Print();
	virtual ~gCard();


	bool   isCard;
	vector<string> territories;
	vector<string> variables;
	bool   italy;
	bool   world;
	string day_zero_what;
	int	   day_zero_howmany;
	bool   var_or_terr;
	int    norm;
	bool   verbose;
	bool   batch;
	bool   root_db_read;
	bool   root_db_create;

private:
	string filename = "gCOVID.card";
	void DecodeTerritories(const string&);
	void DecodeVariables(const string&);
	void SetProvince();
	vector<string> province;
};

#endif /* INCLUDE_GCARD_H_ */
