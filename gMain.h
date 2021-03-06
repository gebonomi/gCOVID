/*
 * gMain.h
 *
 *  Created on: Mar 20, 2020
 *      Author: germano
 */

#ifndef GMAIN_H_
#define GMAIN_H_
#include "gCard.h"
#include <gReader.h>
#include <gPlotter.h>
#include <gAnalyzer.h>
#include <gRootManager.h>


class gMain {
public:
	gMain();
	virtual ~gMain();

	bool Init();
	void Execute();
	void Finish();

	gCard GetCard() {return myCard;};

private:
	gCard myCard;
	unique_ptr<gDataReader> 	myDataReader;
	unique_ptr<gPlotter> 		myPlotter;
	unique_ptr<gAnalyzer>		myAnalyzer;
	unique_ptr<gRootManager> 	myRootManager;
    TApplication* myApp;

	shared_ptr<gDataSample> DataSample;
};

#endif /* GMAIN_H_ */
