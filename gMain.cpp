/*
 * gMain.cpp
 *
 *  Created on: Mar 20, 2020
 *      Author: germano
 */

#include "gMain.h"

gMain::gMain() {
	// TODO Auto-generated constructor stub
//	myRootManager 	= nullptr;
	myCard.Init();
}

gMain::~gMain() {
	// TODO Auto-generated destructor stub
}

bool gMain::Init() {
	if(!myCard.ReadValues()) {
		return false;
	}
	if(myCard.verbose) cout << "gMain::Init --> " << endl;
//	// < ----------------  ROOT MANAGER   ------------------ >
//
//	myRootManager = std::unique_ptr<gRootManager>(new gRootManager());

	// < ----------------   DATA READER   ------------------ >

	myDataReader = std::unique_ptr<gDataReader>(new gDataReader(myCard));
	myDataReader->ReadData();
	DataSample= myDataReader->GetDataSample();

	return true;
}

void gMain::Execute() {

	if(myCard.verbose) cout << "gMain::Execute --> " << endl;
	// < ----------------     ANALYZER    ------------------ >
	if(myCard.analyzer_flag) {
		if(myCard.verbose) cout << "gMain::Execute --> Analyzer" << endl;
		myAnalyzer = std::unique_ptr<gAnalyzer>(new gAnalyzer(myCard));
		myAnalyzer->SetDataSample(DataSample);
		myAnalyzer->Analyze();
	}

	// < ----------------      PLOTTER    ------------------ >
	if(myCard.plotter_flag) {
		if(myCard.verbose) cout << "gMain::Execute --> Plotter" << endl;
		myApp = new TApplication("myApp", 0, 0);
		myPlotter = std::unique_ptr<gPlotter>(new gPlotter(myCard));
		myPlotter->SetDataSample(DataSample);
		myPlotter->CreateHistos();
	}

	return;
}

void gMain::Finish() {

	if(myCard.verbose) cout << "gMain::Execute --> Finish" << endl;
	// < ----------------     ANALYZER    ------------------ >
	if(myCard.analyzer_flag) {
		myAnalyzer->ShowResults();
	}

	// < ----------------      PLOTTER    ------------------ >
	if(myCard.plotter_flag) {
		myPlotter->Draw();
		myApp->Run();
	}

	return;
}
