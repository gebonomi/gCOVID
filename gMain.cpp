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

	if(!myCard.batch) myApp = new TApplication("myApp", 0, 0);

	// < ----------------       CARD      ------------------ >

	myCard.Reset();
	myCard.ReadValues();

	// < ----------------  ROOT MANAGER   ------------------ >
	myRootManager = std::unique_ptr<gRootManager>(new gRootManager());

	// < ----------------   DATA READER   ------------------ >

	myDataReader = std::unique_ptr<gDataReader>(new gDataReader(myCard));
	Italy 		= myDataReader->GetItaly();
	World 		= myDataReader->GetWorld();
	ALL			= myDataReader->GetDataSample();

	return true;
}

void gMain::Execute() {

	// < ----------------      PLOTTER    ------------------ >

	myPlotter = std::unique_ptr<gPlotter>(new gPlotter(myCard));
	if(myCard.italy) 	myPlotter->SetDataSample(Italy);
	if(myCard.world) 	myPlotter->SetDataSample(World);
//	myPlotter->SetDataSample(ALL);
	myPlotter->CreateHistos();
	return;
}

void gMain::Finish() {
	myPlotter->Draw();

	if(!myCard.batch) myApp->Run() ;
	return;
}
