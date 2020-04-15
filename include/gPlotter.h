/*
 * gPlotter.h
 *
 *  Created on: Mar 27, 2020
 *      Author: germano
 */

#ifndef INCLUDE_GPLOTTER_H_
#define INCLUDE_GPLOTTER_H_
#include "myRoot.h"
#include "gDataSample.h"
#include "gCard.h"

class gPlotter {
public:
	gPlotter();
	gPlotter(gCard&);
	void SetDataSample(shared_ptr<gDataSample> D) {
		DataSample = D;
//		population = DataSample->GetPopulation();
	};
	void CreateHistos();
	void Draw();

	virtual ~gPlotter();

private:
	gCard myCard;
	shared_ptr<gDataSample> DataSample;
//	map<string, double> population;
	vector<string> territories; ///< Territories to show
	vector<string> variables; ///< Categories to show
	map<string, vector<shared_ptr<TH2D>>> HistoMap;
	map<string, vector<shared_ptr<TGraphErrors>>> GraphMap;

	shared_ptr<TGraphErrors> ToGraph(const string&, const string&, vector<double>&, vector<double>&, vector<double>&, vector<double>&);
	shared_ptr<TH2D> ToHistoForGraph(const string&, const string&, vector<double>&, vector<double>&, vector<double>&, vector<double>&);
	void SetPalette();
//	void SetPopulation();
	map<string, int> palette;
	void Normalize(vector<shared_ptr<TH2D>>&, vector<shared_ptr<TGraphErrors>>&);
};

#endif /* INCLUDE_GPLOTTER_H_ */
