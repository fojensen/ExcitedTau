#include "labelCMS.h"
#include <iostream>
#include <TLegend.h>
#include <TCut.h>
#include <TLine.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TH1D.h>

TH1D* runPoint(const TString tag, const bool baselinecuts=false)
{
   TChain chain_ss("ssNTupleMaker/tree");
   const int n_ss_files = chain_ss.Add("./mcsamples/"+tag+".root");
   TChain chain_os("osNTupleMaker/tree");
   const int n_os_files = chain_os.Add("./mcsamples/"+tag+".root"); 
   std::cout << "n_ss_files: " << n_ss_files << std::endl;
   std::cout << "n_os_files: " << n_os_files << std::endl;
   
   TCut baseline = "nLeptons>=2";
   if (baselinecuts) {
      baseline = baseline && TCut("nPhotons>0 && BTags[2]==0 && m_ll>=91.1876");
      baseline = baseline && TCut("dr_ll>=0.4 && dr_lead>=0.4 && dr_sublead>=0.4");
   }

   TH1D * h_ss = new TH1D("h_ss_"+tag, ";charge sum;mc events", 7, -3.5, 3.5);
   h_ss->SetLineWidth(2);
   h_ss->SetStats(0);
   TH1D * h_os = (TH1D*)h_ss->Clone("h_os_"+tag);
   std::cout << "begin projecting from same-sign chain..." << std::endl;
   const int n_ss_project = chain_ss.Project(h_ss->GetName(), "sumQ", baseline);
   std::cout << "begin projecting from opposite-sign chain..." << std::endl;
   const int n_os_project = chain_os.Project(h_os->GetName(), "sumQ", baseline);
   std::cout << "n_ss_project: " << n_ss_project << std::endl;
   std::cout << "n_os_project: " << n_os_project << std::endl;

   const double n_ssm2 = h_ss->GetBinContent(2);
   const double n_os = h_os->GetBinContent(4);
   const double n_ssp2 = h_ss->GetBinContent(6);

   TH1D * h_num = new TH1D("h_num_"+tag, "", 3, -0.5, 2.5);
   h_num->SetLineWidth(2);
   h_num->SetStats(0);
   h_num->Sumw2();
   TH1D * h_denom = (TH1D*)h_num->Clone("h_denom_"+tag);
   h_num->SetBinContent(1, n_os); h_num->SetBinError(1, sqrt(n_os));
   h_num->SetBinContent(2, n_os); h_num->SetBinError(2, sqrt(n_os));
   h_num->SetBinContent(3, n_os); h_num->SetBinError(3, sqrt(n_os));
   h_denom->SetBinContent(1, n_ssm2);        h_denom->SetBinError(1, sqrt(n_ssm2));
   h_denom->SetBinContent(2, n_ssp2);        h_denom->SetBinError(2, sqrt(n_ssp2));
   h_denom->SetBinContent(3, n_ssm2+n_ssp2); h_denom->SetBinError(3, sqrt(n_ssm2+n_ssp2));

   TH1D * r = (TH1D*)h_num->Clone("r_"+tag);
   r->Divide(h_denom);

   const char *labels[3] = {"N(+-) / N(++)", "N(+-) / N(--)", "N(+-) / (N(++)+N(--))"};
   for (int i = 0; i < 3; ++i) {
      r->GetXaxis()->SetBinLabel(i+1, labels[i]);
   }
   r->GetXaxis()->SetTitle("scaling factor");
   r->SetTitle(tag);

   TCanvas * canvas = new TCanvas("canvas_"+tag, tag, 800, 400);
   canvas->Divide(2, 1);

   TPad *p = (TPad*)canvas->cd(1);
   h_os->SetStats(0);
   h_os->SetLineColor(2);
   h_os->Draw("HIST, E");
   h_ss->SetLineColor(3);
   h_ss->Draw("HIST, E, SAME");

   TLegend * leg = new TLegend(0.25, 0.75, 0.75, 0.85);
   leg->SetBorderSize(0);
   leg->SetNColumns(2);
   leg->AddEntry(h_ss, "ss", "L");
   leg->AddEntry(h_os, "os", "L");
   leg->Draw();

   h_os->SetMinimum(1.);
   h_os->SetMaximum(1.e6);
   p->SetLogy();

   canvas->cd(2);
   r->SetMarkerStyle(20);
   r->Draw("PE");

   char savetag[100];
   if (baselinecuts) {
      sprintf(savetag, "./plots/chargesums.%s.baselinecuts.pdf", tag.Data());
   } else {
     sprintf(savetag, "./plots/chargesums.%s.taupaironly.pdf", tag.Data());
   }
   canvas->SaveAs(savetag);
   labelSim();

   return r;
}

void osss(const bool baselinecuts=true)
{
   const int n = 5;

   TString tags[n];
   tags[0] = "QCD_HT50to100";
   tags[1] = "QCD_HT100to200";
   tags[2] = "QCD_HT200to300";
   tags[3] = "QCD_HT300to500";
   tags[4] = "QCD_HT500to700";
   //tags[5] = "QCD_HT700to1000";
   //tags[6] = "QCD_HT1000to1500";
   //tags[7] = "QCD_HT1500to2000";
   //tags[8] = "QCD_HT2000toInf";
   TH1D *h[n];
   for (int i = 0; i < n; ++i) {
      h[i] = runPoint(tags[i], baselinecuts);
   }
   
   TCanvas * c = new TCanvas("c_"+tags, tags, 400, 400);

   TLegend * leg = new TLegend(0.25, 0.75, 0.75, 0.85);
   leg->SetBorderSize(0);
   leg->SetNColumns(2);

   for (int i = 0; i < n; ++i) {
      h[i]->SetLineColor(i+2);
      h[i]->SetMarkerColor(i+2);      
      h[i]->Draw("PE, SAME");
      leg->AddEntry(h[i], tags[i], "L");
   }
   h[0]->SetTitle("");
   h[0]->SetMinimum(0.);
   h[0]->SetMaximum(4.);
   leg->Draw();

   TLine * l = new TLine(-0.5, 1., 2.5, 1.);
   l->SetLineStyle(2);
   l->SetLineColor(2);
   l->Draw();

   char savetag[100];
   if (baselinecuts) { 
      sprintf(savetag, "./plots/rsummary.baselinecuts.pdf");
   } else {
     sprintf(savetag, "./plots/rsummary.taupaironly.pdf");
   }
   labelSim();
   c->SaveAs(savetag);
}
