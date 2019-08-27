#include "labelCMS.h"
#include "addOverflow.h"
#include <iostream>
#include <TLegend.h>
#include <TLine.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TCut.h>

void runPoint(TH1D * h, const TString var) //pbr!
{
   TChain chain_ss("ssNTupleMaker/tree");             TChain chain_os("osNTupleMaker/tree");
   chain_ss.Add("./mcsamples/QCD_HT50to100.root");    chain_os.Add("./mcsamples/QCD_HT50to100.root");
   chain_ss.Add("./mcsamples/QCD_HT100to200.root");   chain_os.Add("./mcsamples/QCD_HT100to200.root");
   chain_ss.Add("./mcsamples/QCD_HT200to300.root");   chain_os.Add("./mcsamples/QCD_HT200to300.root");
   chain_ss.Add("./mcsamples/QCD_HT300to500.root");   chain_os.Add("./mcsamples/QCD_HT300to500.root");
   chain_ss.Add("./mcsamples/QCD_HT500to700.root");   chain_os.Add("./mcsamples/QCD_HT500to700.root");
   //chain_ss.Add("./mcsamples/QCD_HT700to1000.root");  chain_os.Add("./mcsamples/QCD_HT700to1000.root");
   //chain_ss.Add("./mcsamples/QCD_HT1000to1500.root"); chain_os.Add("./mcsamples/QCD_HT1000to1500.root");
   //chain_ss.Add("./mcsamples/QCD_HT1500to2000.root"); chain_os.Add("./mcsamples/QCD_HT1500to2000.root");
   //chain_ss.Add("./mcsamples/QCD_HT2000toInf.root");  chain_os.Add("./mcsamples/QCD_HT2000toInf.root");

   TCut baseline = "nLeptons>=2"; 
   //baseline = baseline && TCut("nPhotons>0 && BTags[2]==0 && m_ll>=91.1876 && dr_ll>=0.4 && dr_lead>=0.4 && dr_sublead>=0.4");

   TH1D * h_os = (TH1D*)h->Clone("h_os");
   h_os->Sumw2();
   h_os->SetLineWidth(2);
   h_os->SetStats(0);
   TH1D * h_ssm2 = (TH1D*)h_os->Clone("h_ssm2");
   TH1D * h_ssp2 = (TH1D*)h_os->Clone("h_ssp2");

   std::cout << "now project from ssm2" << std::endl;
   char buffssm2[100];
   sprintf(buffssm2, "xsWeight * (%s && sumQ==-2)", TString(baseline).Data());
   chain_ss.Project(h_ssm2->GetName(), var, buffssm2);
   addOverflow(h_ssm2);

   std::cout << "now project from ssp2" << std::endl;
   char buffssp2[100];
   sprintf(buffssp2, "xsWeight * (%s && sumQ==2)", TString(baseline).Data());
   chain_ss.Project(h_ssp2->GetName(), var, buffssp2);
   addOverflow(h_ssp2);

   std::cout << "now project from os" << std::endl;
   char buffos[100];
   sprintf(buffos, "xsWeight * (%s && sumQ==0)", TString(baseline).Data());
   chain_os.Project(h_os->GetName(), var, buffos);
   addOverflow(h_os);

   TH1D * r_ssp2 = (TH1D*)h_os->Clone("r_ssp2");
   r_ssp2->Divide(h_ssp2);
   
   TH1D * r_ssm2 = (TH1D*)h_os->Clone("r_ssm2");
   r_ssm2->Divide(h_ssm2);

   TH1D * h_ss = (TH1D*)h_ssm2->Clone("h_ss");
   h_ss->Add(h_ssp2); 
   TH1D * r_ss = (TH1D*)h_os->Clone("r_ss");
   r_ss->Divide(h_ss);

   TCanvas * canvas = new TCanvas("canvas_"+var, var, 400, 400);
   
   r_ss->GetYaxis()->SetTitle("scaling ratio");
   r_ss->SetMinimum(0.);
   r_ss->SetMaximum(5.);

   r_ss->SetLineColor(2);
   r_ss->SetMarkerColor(2);
   r_ss->SetMarkerStyle(20);
   r_ss->Draw("PE");

   r_ssm2->SetLineColor(3);
   r_ssm2->SetMarkerColor(3);
   r_ssm2->SetMarkerStyle(20);
   r_ssm2->Draw("PE, SAME");

   r_ssp2->SetLineColor(4);
   r_ssp2->SetMarkerColor(4);
   r_ssp2->SetMarkerStyle(20);
   r_ssp2->Draw("PE, SAME");
  
   TLine * l = new TLine(h->GetBinLowEdge(1), 1., h->GetBinLowEdge(h->GetNbinsX()+1), 1.);
   l->SetLineStyle(2);
   l->SetLineColor(2);
   l->Draw();
 
   TLegend * leg = new TLegend(0.25, 0.75, 0.75, 0.85);
   leg->SetBorderSize(0);
   leg->SetHeader("q_{1}+q_{2}=");
   leg->SetNColumns(3);
   leg->AddEntry(h_ssm2, "-2", "M");
   leg->AddEntry(h_ssp2, "+2", "M");
   leg->AddEntry(h_ss, "-2 or +2", "M");
   leg->Draw();

   labelSim();
   canvas->SaveAs("./plots/r_"+var+".pdf");
}

void qcdStudy()
{
   /*TH1D * h_ptlead = new TH1D("h_ptlead", ";leading #tau_{had} p_{T} [GeV];events / 100 GeV", 5, 0., 500.);
   runPoint(h_ptlead, "pt_lead");

   TH1D * h_etalead = new TH1D("h_etalead", ";leading #tau_{had} }#eta|;events / 0.5", 5, 0., 2.5);
   runPoint(h_etalead, "std::abs(eta_lead)");

   TH1D * h_ptsublead = new TH1D("h_ptsublead", ";subleading #tau_{had} p_{T} [GeV];events / 100 GeV", 5, 0., 500.);
   runPoint(h_ptsublead, "pt_sublead");
   
   TH1D * h_etasublead = new TH1D("h_etasublead", ";subleading #tau_{had} |#eta|;events / 0.5", 5, 0., 2.5);
   runPoint(h_etasublead, "std::abs(eta_sublead)");
   
   TH1D * h_ptll = new TH1D("h_ptll", ";#tau_{had}#tau_{had} p_{T} [GeV];events / 100 GeV", 5, 0., 500.);
   runPoint(h_ptll, "pt_ll");

   TH1D * h_etall = new TH1D("h_etall", ";#tau_{had}#tau_{had} |#eta|;events / 0.5", 5, 0., 2.5);
   runPoint(h_etall, "std::abs(eta_ll");

   TH1D * h_mll = new TH1D("h_mll", ";#tau_{had}#tau_{had} mass [GeV];events / 100 GeV", 5, 0., 500.);
   runPoint(h_mll, "m_ll");*/

   TH1D * h_collmin = new TH1D("h_collmin", ";min(#tau_{#nu}, #photon) mass [GeV];events / 50 GeV", 10, 0., 500.);
   runPoint(h_collmin, "collinear_m_min");

   TH1D * h_collmax = new TH1D("h_collmax", ";max(#tau_{#nu}, #photon) mass [GeV];events / 50 GeV", 10, 0., 500.);
   runPoint(h_collmax, "collinear_m_max");
}

