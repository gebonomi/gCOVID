/*
 * main.cpp
 *
 *  Created on: Mar 19, 2020
 *      Author: germano
 */
#include <iostream>
#include "TApplication.h"
#include "TCanvas.h"
#include "TGraph.h"
#include <vector>
#include "gMain.h"

using namespace std;

int main() {

	gMain *g = new gMain();

	if(!g->Init()) {
		cout << "Exiting prematurely" << endl;
		return 1;
	}

	g->Execute();

	g->Finish();

	return 0;
}





