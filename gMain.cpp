/*
 * gMain.cpp
 *
 *  Created on: Mar 20, 2020
 *      Author: germano
 */

#include "gMain.h"

gMain::gMain() {
	// TODO Auto-generated constructor stub
	myDataReader 	= nullptr;
	myRootManager 	= nullptr;
	myPlotter 		= nullptr;
	myCard.Reset();
}

gMain::~gMain() {
	// TODO Auto-generated destructor stub
}

bool gMain::Init() {
	if(!myCard.ReadValues()) {
		return false;
	}

	// < ---------------- ROOT INTERACTIVE App ------------------ >

	if(!myCard.batch) myApp = new TApplication("myApp", 0, 0);

	// < ----------------  ROOT MANAGER   ------------------ >

	myRootManager = std::unique_ptr<gRootManager>(new gRootManager());

	// < ----------------   DATA READER   ------------------ >

	myDataReader = std::unique_ptr<gDataReader>(new gDataReader(myCard));

	myDataReader->ReadData();

	DataSample= myDataReader->GetDataSample();

	return true;
}

void gMain::Execute() {

	// < ----------------     ANALYZER    ------------------ >

	myAnalyzer = std::unique_ptr<gAnalyzer>(new gAnalyzer(myCard));
	myAnalyzer->SetDataSample(DataSample);
	myAnalyzer->Analyze();

	// < ----------------      PLOTTER    ------------------ >

	myPlotter = std::unique_ptr<gPlotter>(new gPlotter(myCard));
	myPlotter->SetDataSample(DataSample);
	myPlotter->CreateHistos();

	return;
}

void gMain::Finish() {

	myPlotter->Draw();

	if(!myCard.batch) myApp->Run() ;
	return;
}
