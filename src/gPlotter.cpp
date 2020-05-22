/*
 * gPlotter.cpp
 *
 *  Created on: Mar 27, 2020
 *      Author: germano
 */

#include <gPlotter.h>


gPlotter::gPlotter() {
	// TODO Auto-generated constructor stub
	cout << "gPlotter::gPlotter --> use other constructor \"gPlotter::gPlotter(gCard& card)\" " << endl;
}

gPlotter::gPlotter(gCard& card) {
	// TODO Auto-generated constructor stub
	myCard = card;
	territories = myCard.territories;
	variables = myCard.variables;
	SetPalette();
}

void gPlotter::AddRawValuesHistos() {
	map<string, vector<gDataEntry>> dataMap = DataSample->GetDataMap();

	map<string, double>::iterator eit;
	vector<string> first;
	vector<string> second;
	if(myCard.var_or_terr) {
		first  = myCard.variables;		///< a -> variable
		second = myCard.territories;	///< b -> territory
	} else {
		first  = myCard.territories;	///< a -> territory
		second = myCard.variables;		///< b -> variable
	}
	string t;
	string v;
	///< Raw variables
	for(auto& a:first) {
		vector<shared_ptr<TH2D>> these_histos;
		vector<shared_ptr<TGraphErrors>> these_graphs;
		for(auto& b:second) {
			if(myCard.var_or_terr) { v = a; t = b; } else { t = a; v = b;}
			vector<gDataEntry> entries 	= DataSample->GetTerritoryData(t);
			if(entries.size()==0) continue;
			int day_zero 				= DataSample->GetDayZero(t, myCard.day_zero_what, myCard.day_zero_howmany);
			double scale = 1.;
			vector<double> x, ex, y, ey;
			scale = 1.;
			for(unsigned int i=0; i<entries.size(); i++) {
				gDataEntry e = entries[i];
				if(myCard.plotter_norm==1) {
					scale = 1.e6/e.population;
					if(isnan(scale)) {
						cout << "gPlotter::AddRawValuesHistos --> population not found for " << e.territory << " in date " << e.date << endl;
						scale = 1.;
					}
				}
				eit = e.rawValues.find(v);
				x.push_back(e.day_of_the_year-day_zero); ex.push_back(0.);
				y.push_back(eit->second*scale);
				double err = sqrt(eit->second)*scale;
				ey.push_back(err);
			}
			if(x.size()==0) {
				cout << "gPlotter::AddRawValuesHistos --> Could not find data for the \"" << v << "\"" << endl;
				continue;
			}
			ostringstream name, title;
			name << "h_" << t << "_" << v;
			if(myCard.var_or_terr) title << v << " [days since " << myCard.day_zero_what << " > " << myCard.day_zero_howmany << "]";
			if(!myCard.var_or_terr) title << t << " [days since " << myCard.day_zero_what << " > " << myCard.day_zero_howmany << "]";
			these_histos.push_back(ToHistoForGraph(name.str(), title.str(), x, ex, y, ey)); ///< Variable and conditions
			these_graphs.push_back(ToGraph(t, v, x, ex, y, ey)); ///< territory as name, variable as title
		}
		if(these_histos.size()==0 || these_graphs.size()==0) {
			cout << "gPlotter::AddRawValuesHistos --> these_histos.size()==0 || these_graphs.size()==0 ) for [" << a << "]" << endl;
			continue;
		}
		HistoMap.insert(std::pair<string, vector<shared_ptr<TH2D>>>(a,these_histos));
		GraphMap.insert(std::pair<string, vector<shared_ptr<TGraphErrors>>>(a,these_graphs));
	}
	return;
}

void gPlotter::AddRatesHistos() {
	if(!myCard.var_or_terr) {
		cout << "gPlotter::AddRatesHistos --> showing rate histos only per variable. Set var_or_terr to 1 in gCOVID.card" << endl;
	}
	map<string, vector<gDataEntry>> dataMap = DataSample->GetDataMap();
	map<string, double>::iterator eit;

	///< Rates variables
	vector<string> variables;
	gDataEntry tmp;
	for(auto it=tmp.rates.begin(); it!=tmp.rates.end(); it++) { variables.push_back(it->first); }
	///< Rate variables
	for(auto& v:variables) {
		vector<shared_ptr<TH2D>> these_histos;
		vector<shared_ptr<TGraphErrors>> these_graphs;
		for(auto& t:myCard.territories) {
			vector<gDataEntry> entries 	= DataSample->GetTerritoryData(t);
			if(entries.size()==0) continue;
			int day_zero = DataSample->GetDayZero(t, myCard.day_zero_what, myCard.day_zero_howmany);
			vector<double> x, ex, y, ey;
			for(auto& e:entries) {
//				cout << e.territory << " (" << e.date << "): [" << e.day_of_the_year-day_zero << ", " << eit->second << "]" << endl;
				eit = e.rates.find(v);
				if(eit==e.rates.end()) continue;
				if(isnan(eit->second)) continue;
				x.push_back(e.day_of_the_year-day_zero); ex.push_back(0.);
				y.push_back(eit->second*100);
				ey.push_back(0.);
			}
			if(x.size()==0) {
//				cout << "gPlotter::AddRatesHistos --> Could not find data for rates.at(\"" << v << "\")" << endl;
				continue;
			}
			ostringstream name, title;
			name << "h_" << t << "_rates_" << v;
			title << v << " (rate) [days since " << myCard.day_zero_what << " > " << myCard.day_zero_howmany << "]";
			these_histos.push_back(ToHistoForGraph(name.str(), title.str(), x, ex, y, ey)); ///< Variable and conditions
			these_graphs.push_back(ToGraph(t, v, x, ex, y, ey)); ///< territory as name, variable as title
		}
		if(these_histos.size()==0 || these_graphs.size()==0) {
			cout << "gPlotter::AddRatesHistos --> these_histos.size()==0 || these_graphs.size()==0) for rates[" << v << "]" << endl;
			continue;
		}
		ostringstream s;
		s << v << "_rate";
		HistoMap.insert(std::pair<string, vector<shared_ptr<TH2D>>>(s.str(),these_histos));
		GraphMap.insert(std::pair<string, vector<shared_ptr<TGraphErrors>>>(s.str(),these_graphs));
	}
	return;
}

void gPlotter::AddDoublingHistos() {
	if(!myCard.var_or_terr) {
		cout << "gPlotter::AddDoublingHistos --> showing doubling histos only per variable. Set var_or_terr to 1 in gCOVID.card" << endl;
	}
	map<string, vector<gDataEntry>> dataMap = DataSample->GetDataMap();
	map<string, double>::iterator eit;

	///< Rates variables
	vector<string> variables;
	gDataEntry tmp;
	for(auto it=tmp.doubles.begin(); it!=tmp.doubles.end(); it++) { variables.push_back(it->first); }
	///< Rate variables
	for(auto& v:variables) {
		vector<shared_ptr<TH2D>> these_histos;
		vector<shared_ptr<TGraphErrors>> these_graphs;
		for(auto& t:myCard.territories) {
			vector<gDataEntry> entries 	= DataSample->GetTerritoryData(t);
			if(entries.size()==0) continue;
			int day_zero = DataSample->GetDayZero(t, myCard.day_zero_what, myCard.day_zero_howmany);
			vector<double> x, ex, y, ey;
			for(auto& e:entries) {
//				cout << e.territory << " (" << e.date << "): [" << e.day_of_the_year-day_zero << ", " << eit->second << "]" << endl;
				eit = e.doubles.find(v);
				if(eit==e.doubles.end()) continue;
				if(isnan(eit->second)) continue;
				x.push_back(e.day_of_the_year-day_zero); ex.push_back(0.);
				y.push_back(eit->second);
				ey.push_back(0.);
			}
			if(x.size()==0) {
//				cout << "gPlotter::AddRatesHistos --> Could not find data for doubles.at(\"" << v << "\")" << endl;
				continue;
			}
			ostringstream name, title;
			name << "h_" << t << "_doubles_" << v;
			title << v << " (1./doubling time) [days since " << myCard.day_zero_what << " > " << myCard.day_zero_howmany << "]";
			these_histos.push_back(ToHistoForGraph(name.str(), title.str(), x, ex, y, ey)); ///< Variable and conditions
			these_graphs.push_back(ToGraph(t, v, x, ex, y, ey)); ///< territory as name, variable as title
		}
		if(these_histos.size()==0 || these_graphs.size()==0) {
			cout << "gPlotter::AddDoublingHistos --> these_histos.size()==0 || these_graphs.size()==0) for doubles[" << v << "]" << endl;
			continue;
		}
		ostringstream s;
		s << v << "_doubles";
		HistoMap.insert(std::pair<string, vector<shared_ptr<TH2D>>>(s.str(),these_histos));
		GraphMap.insert(std::pair<string, vector<shared_ptr<TGraphErrors>>>(s.str(),these_graphs));
	}
	return;
}


void gPlotter::Draw() {
	///< This method present the plots per variable (e.g. confirmed, actives, etc.)
	///< superimposing the distributions of the different territories
	///< 1 canvas per variable
	if(HistoMap.size()==0) {
		cout << "gPlotter::Draw() --> Nothing to draw " << endl;
		return;
	}
	ostringstream name;
	map<string, vector<shared_ptr<TH2D>>>::iterator 		hit;
	map<string, vector<shared_ptr<TGraphErrors>>>::iterator git;
	vector<TCanvas*> Cs;
	vector<TLegend*> Ls;
	for(hit = HistoMap.begin(); hit != HistoMap.end(); hit++) { ///< Loop on the histograms
		string s = hit->first;
		git = GraphMap.find(s); /// Loading the corresponding graph
		Cs.push_back(new TCanvas(s.c_str(),s.c_str(),1200,800));
		Ls.push_back(new TLegend(0.15,0.69,0.45,0.89));
		vector<shared_ptr<TH2D>> 		 hs = hit->second;
		vector<shared_ptr<TGraphErrors>> gs = git->second;
		Normalize(hs,gs);
		int cind = 0;
		for (unsigned int i = 0; i < hs.size(); i++) {
			Cs.at(Cs.size()-1)->cd(++cind);
			hs.at(i)->Draw("SAME");
			gs.at(i)->Draw("P,SAME");
			if(!myCard.var_or_terr) Ls.at(Cs.size()-1)->AddEntry(gs.at(i).get(),gs.at(i)->GetTitle(),"lfp");
			if(myCard.var_or_terr) Ls.at(Cs.size()-1)->AddEntry(gs.at(i).get(),gs.at(i)->GetName(),"lfp");
		}
	}
	for(auto it=0; it< Cs.size(); it++) {
		Cs[it]->cd(); Ls[it]->Draw();
		Cs[it]->Modified(); Cs[it]->Update();
	}
	return;
	return;
}


shared_ptr<TGraphErrors> gPlotter::ToGraph(const string& territory, const string& variable, vector<double>& xin, vector<double>& exin, vector<double>& yin, vector<double>& eyin) {
	///< This methods create a TGraphError starting from vector of doubles
	const Int_t n = (Int_t)xin.size();
    double x[n], ex[n], y[n], ey[n];
    for(Int_t i=0; i<n; i++) {
        x[i]  = xin[i];
        ex[i] = exin[i];
        y[i]  = yin[i];
        ey[i] = eyin[i];
    }
    shared_ptr<TGraphErrors> g = std::shared_ptr<TGraphErrors>(new TGraphErrors(n, x, y, ex, ey));
    g->SetMarkerStyle(20);
    ///< Setting "territory" as name and "variable" as title
    g->SetNameTitle(territory.c_str(), variable.c_str());
	if(myCard.var_or_terr) {
		g->SetMarkerColor(palette.find(territory)->second);
		g->SetLineColor(palette.find(territory)->second);
	} else {
		g->SetMarkerColor(palette.find(variable)->second);
		g->SetLineColor(palette.find(variable)->second);
	}
	g->SetLineWidth(3);
    return g;
}

shared_ptr<TH2D> gPlotter::ToHistoForGraph(const string &name, const string& title, vector<double>& x, vector<double>& ex, vector<double>& y, vector<double>& ey) {
	///< This methods create an empty histogram (to host the display a TGraphErrors::Draw) with the appropriate limits
	Int_t nBins = 1000;
    double xMin=std::numeric_limits<double>::max();
    double xMax=std::numeric_limits<double>::min();
    double exMax=std::numeric_limits<double>::min();
    double xRange;
    for(auto& v:x) {
    	if(v<xMin) xMin = v;
    	if(v>xMax) xMax = v;
    }
    for(auto& v:ex) {
    	if(v>exMax) exMax = v;
    }
    xMax += exMax;
    if(xMin<0) xMin = 0;
    xRange = xMax - xMin;
    xMin = xMin-xRange/5.; xMax = xMax+xRange/5.; ///< Enlarging a little bit the histogram
    if(xMin>0&&xRange/3.>xMin) xMin = 0.;

    double yMin=std::numeric_limits<double>::max();
    double yMax=std::numeric_limits<double>::min();
    double eyMax=std::numeric_limits<double>::min();
    double yRange;
    for(auto& v:y) {
    	if(v<yMin) yMin = v;
    	if(v>yMax) yMax = v;
    }
    for(auto& v:ey) {
    	if(v>eyMax) eyMax = v;
    }
    yMin -= eyMax;
    yMax += eyMax;
    yRange = yMax - yMin;
    if(yMin!=0.) yMin = yMin-yRange/10.;

    if(title.find("new_actives")==std::string::npos) yMin = 0.;
    yMax = yMax+yRange/5.; ///< Enlarging a little bit the histogram
    string xtitle = "day";
    string ytitle = "#";
	if(myCard.plotter_norm==1) ytitle = "# (per million inhabitants)";
	if(name.find("rate")!=std::string::npos) { ///< Fixing some parameters for the rate histos
		ytitle = "\%";
		if(title.find("confirmed")!=std::string::npos) 		yMax = 60;
		if(title.find("deceased")!=std::string::npos) 		yMax = 60;
	}
	if(name.find("double")!=std::string::npos) { ///< Fixing some parameters for the rate histos
		ytitle = "1./days";
	}
    shared_ptr<TH2D> h(new TH2D(name.c_str(), title.c_str(), nBins, xMin, xMax, nBins, yMin, yMax));
    h->SetXTitle(xtitle.c_str()); h->SetYTitle(ytitle.c_str());
	h->SetStats(kFALSE);
	return h;
}

void gPlotter::Normalize(vector<shared_ptr<TH2D>>& hs, vector<shared_ptr<TGraphErrors>>& gs) {
	if(hs.size()==1) return; ///< No need to normalize
	if(hs.size()!=gs.size()) {
		cout << "gPlotter::Normalize --> hs.size()!=gs.size" << endl;
		return;
	}
    double xMin=std::numeric_limits<double>::max();
    double xMax=std::numeric_limits<double>::min();
    double yMin=std::numeric_limits<double>::max();
    double yMax=std::numeric_limits<double>::min();
	for(unsigned int i=0; i<hs.size(); i++) {
		if(hs[i]->GetXaxis()->GetXmin()<xMin) xMin = hs[i]->GetXaxis()->GetXmin();
		if(hs[i]->GetXaxis()->GetXmax()>xMax) xMax = hs[i]->GetXaxis()->GetXmax();
		if(hs[i]->GetYaxis()->GetXmin()<yMin) yMin = hs[i]->GetYaxis()->GetXmin();
		if(hs[i]->GetYaxis()->GetXmax()>yMax) yMax = hs[i]->GetYaxis()->GetXmax();
	}
	for(unsigned int i=0; i<hs.size(); i++) { ///< Setting for all the same "x" range
		hs[i]->GetXaxis()->SetLimits(xMin,xMax);
		hs[i]->GetYaxis()->SetLimits(yMin,yMax);
	}
	return;
}

void gPlotter::SetPalette() {
	vector<int> colors;
	colors.push_back(46);
	colors.push_back(38);
	colors.push_back(29);
	colors.push_back(14);
	colors.push_back(2);
	colors.push_back(8);
	colors.push_back(9);
	int ind = 0;
	for(auto& t:territories) { ///< Associate a color to each territory
		///< Fixing some colors
		if(t=="Italy") {palette.insert(std::pair<string, int>(t, 8)); continue;}
		if(t=="Spain") {palette.insert(std::pair<string, int>(t, 2)); continue;}
		if(t=="US") {palette.insert(std::pair<string, int>(t, 9)); continue;}
		if(t=="France") {palette.insert(std::pair<string, int>(t, 4)); continue;}
		if(t=="Germany") {palette.insert(std::pair<string, int>(t, kYellow+2)); continue;}

		if(t=="Lombardia") {palette.insert(std::pair<string, int>(t, 8)); continue;}

		if(t=="Brescia") {palette.insert(std::pair<string, int>(t, 9)); continue;}
		if(t=="Milano") {palette.insert(std::pair<string, int>(t, 2)); continue;}
		if(t=="Bergamo") {palette.insert(std::pair<string, int>(t, kBlack)); continue;}
		if(t=="Roma") {palette.insert(std::pair<string, int>(t, kYellow+2)); continue;}

		if(ind>=colors.size()) ind = 0;
		palette.insert(std::pair<string, int>(t, colors[ind++]));
	}

	for(auto& v:variables) {
		if(v=="recovered") 		{palette.insert(std::pair<string, int>(v, kGreen+3)); continue;}
		if(v=="new_recovered") 	{palette.insert(std::pair<string, int>(v, kGreen+1)); continue;}
		if(v=="deceased") 		{palette.insert(std::pair<string, int>(v, kRed)); continue;}
		if(v=="new_deceased") 	{palette.insert(std::pair<string, int>(v, kRed+2)); continue;}
		if(v=="confirmed") 		{palette.insert(std::pair<string, int>(v, kBlue)); continue;}
		if(v=="new_confirmed") 	{palette.insert(std::pair<string, int>(v, kAzure-3)); continue;}
		if(v=="actives") 		{palette.insert(std::pair<string, int>(v, kYellow+2)); continue;}
		if(v=="new_actives") 	{palette.insert(std::pair<string, int>(v, kYellow-5)); continue;}
		if(v=="tests") 			{palette.insert(std::pair<string, int>(v, kOrange+7)); continue;}
		if(v=="new_tests") 		{palette.insert(std::pair<string, int>(v, kOrange-3)); continue;}
		if(ind>=colors.size()) ind = 0;
		palette.insert(std::pair<string, int>(v, colors[ind++]));
	}
	return;
}

gPlotter::~gPlotter() {
	// TODO Auto-generated destructor stub
}
