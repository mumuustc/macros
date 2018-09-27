/*
 * DrawTPCFEE.C
 *
 *  Created on: Sep 22, 2018
 *      Author: jinhuang
 */

#include <TFile.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TString.h>
#include <TTree.h>
#include <cassert>
#include <cmath>
#include "SaveCanvas.C"
#include "sPhenixStyle.C"

TFile *_file0 = NULL;
TString description;

void DrawTPCFEE(
    //	                    const char *infile = "data/tpcfee_00002658-0000.evt_TPCFEETestRecov1.root"  // button source
    //	                        const char *infile = "data/tpcfee_00002681-0000.evt_TPCFEETestRecov1.root"  // check pad 13,9
        const char *infile = "data/tpcfee_00002697-0000.evt_TPCFEETestRecov1.root"  // check pad 13,9
//    const char *infile = "data/tpcfee_00002702-0000.evt_TPCFEETestRecov1.root"  // check pad 13,9
                                                                                //    const TString infile = "data/tpcfee_00002658-0000.evt_TPCFEETestRecov1_DefaultMap.root"  //
)
{
  gSystem->Load("libtpcdaq.so");

  SetsPhenixStyle();
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1111);
  TVirtualFitter::SetDefaultFitter("Minuit2");

  _file0 = new TFile(infile);
  assert(_file0->IsOpen());
  //

  const double nEvent = hNormalization->GetBinContent(1);

  EventCheck();
  ChannelCheck();
}

void EventCheck()
{
  assert(_file0);

  const double nEvent = hNormalization->GetBinContent(1);

  TCanvas *c1 = new TCanvas("EventCheck", "EventCheck", 1900, 960);
  c1->Divide(4, 2);
  int idx = 1;
  TPad *p;
  p = (TPad *) c1->cd(idx++);
  c1->Update();
  p->SetLogy();
  //  p->SetLeftMargin(.2);
  //  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);

  eventT->Draw("nClusters>>hnClusters(10,-.5,9.5)");
  //  hsampa_address->Scale(1. / nEvent);
  hnClusters->SetTitle(";Number of clusters;event count");

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  //  p->SetLogz();
  //  p->SetLeftMargin(.2);
  //  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);

  eventT->Draw("bx_counter>>hbx_counter(1000,0,1000000)");
  //  hsampa_address->Scale(1. / nEvent);
  hbx_counter->SetTitle(";bx_counter;event count");

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  p->SetLogz();
  //  p->SetLeftMargin(.2);
  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);

  eventT->Draw("Clusters.avg_pady:Clusters.avg_padx>>hAllClusterPad(100,-.5,49.5,24,-.5,11.5)", "nClusters>=1", "colz");
  hAllClusterPad->SetTitle(";All Cluster <Pad X>;All Cluster <Pad Y>");

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  p->SetLogy();
  //  p->SetLeftMargin(.2);
  //  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);

  eventT->Draw("Clusters.peak>>hAllClusterPeak(180,0,1800)", "nClusters>=1");
  hAllClusterPeak->Scale(1. / nEvent);
  hAllClusterPeak->SetTitle(";All Cluster Energy [adc];cluster count/event/adc bin");

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  p->SetLogz();
  //  p->SetLeftMargin(.2);
  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);

  eventT->Draw("Clusters[0].avg_pady:Clusters[0].avg_padx>>hSingleClusterPad(100,-.5,49.5,24,-.5,11.5)", "nClusters==1", "colz");
  hSingleClusterPad->SetTitle(";Single Cluster <Pad X>;Single Cluster <Pad Y>");

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  p->SetLogy();
  //  p->SetLeftMargin(.2);
  //  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);

  eventT->Draw("Clusters[0].peak>>hSingleClusterPeak(180,0,1800)", "nClusters==1");
  hSingleClusterPeak->Scale(1. / nEvent);
  hSingleClusterPeak->SetTitle(";Single Cluster Energy [adc];Single cluster count/event/adc bin");

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  p->SetLogz();
  //  p->SetLeftMargin(.2);
  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);

  eventT->Draw("Clusters[0].avg_pady:Clusters[0].avg_padx>>hSingleClusterPad1(100,-.5,49.5,24,-.5,11.5)", "nClusters==1 && Clusters[0].size_pad_x == 1 && Clusters[0].size_pad_y == 1", "colz");
  hSingleClusterPad1->SetTitle(";Single 1-pad Cluster <Pad X> | Padx<=11;Single Cluster <Pad Y>");

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  p->SetLogy();
  //  p->SetLeftMargin(.2);
  //  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);

  eventT->Draw("Clusters[0].peak>>hSingleClusterPeak1(180,0,1800)", "nClusters==1 && Clusters[0].size_pad_x == 1 && Clusters[0].size_pad_y == 1");
  hSingleClusterPeak1->Scale(1. / nEvent);
  hSingleClusterPeak1->SetTitle(";Single 1-pad Cluster Energy [adc];Single cluster count/event/adc bin");
  //  p = (TPad *) c1->cd(idx++);
  //  c1->Update();
  //  p->SetLogz();
  //  //  p->SetLeftMargin(.2);
  //  p->SetRightMargin(.15);
  //  //  p->SetTopMargin(.15);
  //
  //  //  p->DrawFrame(-200,-200,200,200);
  //
  //  eventT->Draw("Clusters[0].avg_pady:Clusters[0].avg_padx>>hSingleClusterPad11(100,-.5,49.5,24,-.5,11.5)", "nClusters==1 && Clusters[0].avg_padx<=11", "colz");
  //  hSingleClusterPad11->SetTitle(";Single Cluster <Pad X> | Padx<=11;Single Cluster <Pad Y>");
  //
  //  p = (TPad *) c1->cd(idx++);
  //  c1->Update();
  //  p->SetLogy();
  //  //  p->SetLeftMargin(.2);
  //  //  p->SetRightMargin(.15);
  //  //  p->SetTopMargin(.15);
  //
  //  //  p->DrawFrame(-200,-200,200,200);
  //
  //  eventT->Draw("Clusters[0].peak>>hSingleClusterPeak11(180,0,1800)", "nClusters==1 && Clusters[0].avg_padx<=11");
  //  hSingleClusterPeak11->Scale(1. / nEvent);
  //  hSingleClusterPeak11->SetTitle(";Single Cluster Energy [adc] | Padx<=11;Single cluster count/event/adc bin");

  SaveCanvas(c1, TString(_file0->GetName()) + TString(c1->GetName()), kTRUE);

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  TCanvas *c1 = new TCanvas("EventCheck_MultiClusterEvent", "EventCheck_MultiClusterEvent", 1900, 960);
  c1->Divide(4, 2);
  int idx = 1;
  TPad *p;

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  p->SetLogz();
  //  p->SetLeftMargin(.2);
  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);

  eventT->Draw("Sum$(Clusters[].avg_pady * Clusters[].peak)/Sum$(Clusters[].peak):Sum$(Clusters[].avg_padx * Clusters[].peak)/Sum$(Clusters[].peak)>>hSumClusterPad(100,-.5,49.5,24,-.5,11.5)", "nClusters>=2", "colz");
  hSumClusterPad->SetTitle(";Energy Weighted Multi-Cluster <Pad X>;Energy Weighted Multi-Cluster <Pad Y>");

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  p->SetLogy();
  //  p->SetLeftMargin(.2);
  //  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);

  eventT->Draw("Sum$(Clusters[].peak)>>hSumlusterPeak(180,0,1800)", "nClusters>=2");
  hSumlusterPeak->Scale(1. / nEvent);
  hSumlusterPeak->SetTitle(";Multi-cluster Sum Cluster Energy [adc];Multi-cluster event/all event/adc bin");

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  p->SetLogz();
  //  p->SetLeftMargin(.2);
  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);

  eventT->Draw("Clusters[0].avg_pady:Clusters[0].avg_padx>>hLeadClusterPad(100,-.5,49.5,24,-.5,11.5)", "nClusters>=2", "colz");
  hLeadClusterPad->SetTitle(";Lead Cluster <Pad X>;Lead Cluster <Pad Y>");

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  p->SetLogy();
  //  p->SetLeftMargin(.2);
  //  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);

  eventT->Draw("Clusters[0].peak>>hLeadClusterPeak(180,0,1800)", "nClusters>=2");
  hLeadClusterPeak->Scale(1. / nEvent);
  hLeadClusterPeak->SetTitle(";Lead Cluster Energy [adc];cluster count/event/adc bin");

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  p->SetLogz();
  //  p->SetLeftMargin(.2);
  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);

  eventT->Draw("Clusters.avg_pady:Clusters.avg_padx>>hNonLeadClusterPad(100,-.5,49.5,24,-.5,11.5)", "nClusters>=2 && Iteration$>=2", "colz");
  hNonLeadClusterPad->SetTitle(";Secondary+ Cluster <Pad X>;Secondary+ Cluster <Pad Y>");

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  p->SetLogy();
  //  p->SetLeftMargin(.2);
  //  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);

  eventT->Draw("Clusters.peak>>hNonLeadClusterPeak(180,0,1800)", "nClusters>=2 && Iteration$>=2");
  hNonLeadClusterPeak->Scale(1. / nEvent);
  hNonLeadClusterPeak->SetTitle(";Secondary+ Cluster Energy [adc];cluster count/event/adc bin");

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  p->SetLogy();
  //  p->SetLeftMargin(.2);
  //  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);
  eventT->Draw("Clusters[].min_sample +  Clusters[].peak_sample - (Clusters[0].min_sample + Clusters[0].peak_sample)>>htimediff(600,-30,30)", "nClusters>=2 && Iteration$>=1");
  htimediff->Scale(1. / nEvent);
  htimediff->SetTitle(";2nd+ - Primary Cluster Time [ADC Sample];Secondary clusters/all event/bin");

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  p->SetLogy();
  //  p->SetLeftMargin(.2);
  //  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);
  eventT->Draw("Clusters[].avg_padx - Clusters[0].avg_padx>>hpadxdiff(180,-30,30)", "nClusters>=2 && Iteration$>=1");
  hpadxdiff->Scale(1. / nEvent);
  hpadxdiff->SetTitle(";2nd+ - Primary Cluster <X> [pad x];Secondary clusters/all event/bin");

  SaveCanvas(c1, TString(_file0->GetName()) + TString(c1->GetName()), kTRUE);
}

void ChannelCheck()
{
  assert(_file0);

  const double nEvent = hNormalization->GetBinContent(1);

  TCanvas *c1 = new TCanvas("ChannelCheck", "ChannelCheck", 1900, 960);
  c1->Divide(4, 2);
  int idx = 1;
  TPad *p;

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  //  p->SetLogz();
  //  p->SetLeftMargin(.2);
  //  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);

  chanT->Draw("sampa_address>>hsampa_address(8,-.5,7.5)");
  hsampa_address->Scale(1. / nEvent);
  hsampa_address->SetTitle(";sampa_address;channel count/event");

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  //  p->SetLogz();
  //  p->SetLeftMargin(.2);
  //  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);

  chanT->Draw("sampa_channel>>hsampa_channel(50,-10.5,39.5)");
  hsampa_channel->Scale(1. / nEvent);
  hsampa_channel->SetTitle(";sampa_channel;channel count/event");

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  //  p->SetLogz();
  //  p->SetLeftMargin(.2);
  //  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);

  chanT->Draw("fee_channel>>hfee_channel(256,-.5,255.5)");
  hfee_channel->Scale(1. / nEvent);
  hfee_channel->SetTitle(";fee_channel;channel count/event");

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  //  p->SetLogz();
  //  p->SetLeftMargin(.2);
  //  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);

  chanT->Draw("size>>hsize(1000,0,1000)");
  hsize->Scale(1. / nEvent);
  hsize->SetTitle(";size;channel count/event");

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  //  p->SetLogz();
  //  p->SetLeftMargin(.2);
  //  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);

  chanT->Draw("packet_type>>hpacket_type(10,-.5,9.5)");
  hpacket_type->Scale(1. / nEvent);
  hpacket_type->SetTitle(";packet_type;channel count/event");

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  p->SetLogz();
  //  p->SetLeftMargin(.2);
  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);

  chanT->Draw("adc:Iteration$>>hpersistent(150,-.5,149.5,1100,-.5,1099.5)", "", "colz");
  hpersistent->SetTitle(";ADC Sample ID;adc");

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  p->SetLogy();
  p->SetLogz();
  //  p->SetLeftMargin(.2);
  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);

  chanT->Draw("pedestal:fee_channel>>h_pedestalfee_channel(256,-.5,255.5,1100,-.5,1099.5)", "", "colz");
  h_pedestalfee_channel->SetTitle(";fee_channel;Median sample as pedestal [adc]");

  p = (TPad *) c1->cd(idx++);
  c1->Update();
  p->SetLogz();
  //  p->SetLeftMargin(.2);
  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  //  p->DrawFrame(-200,-200,200,200);

  chanT->Draw("max - pedestal:fee_channel>>h_maxfee_channel(256,-.5,255.5,1100,.5,1100.5)", "", "colz");
  h_maxfee_channel->SetTitle(";fee_channel;max sample - pedestal [adc]");

  SaveCanvas(c1, TString(_file0->GetName()) + TString(c1->GetName()), kTRUE);

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  TCanvas *c1 = new TCanvas("ChannelCheck_ChannelMap", "ChannelCheck_ChannelMap", 1000, 560);
  //  c1->Divide(4, 2);
  int idx = 1;
  TPad *p;
  p = (TPad *) c1->cd(idx++);
  c1->Update();
  p->SetLogz();
  //  p->SetLeftMargin(.2);
  p->SetRightMargin(.15);
  //  p->SetTopMargin(.15);

  chanT->Draw("pad_y:pad_x>>hpadMap(60,-10.5,49.5,15,.5,15.5)", "max - pedestal", "colz");
  TH2 *hpadMap = (TH2 *) gDirectory->FindObject("hpadMap");
  hpadMap->Scale(1. / nEvent);
  hpadMap->SetTitle(";pad X;pad Y");
  //  TPaletteAxis *pal = (TPaletteAxis *) (hpadMap->GetListOfFunctions()->FindObject( "palette"  ));
  //  pal->GetAxis()->SetTitle("(max - pedestal)/event [adc]");

  SaveCanvas(c1, TString(_file0->GetName()) + TString(c1->GetName()), kTRUE);
}
