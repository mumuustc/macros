#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6, 00, 0)
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllNoSyncDstInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/SubsysReco.h>
#include <g4detectors/PHG4BlockSubsystem.h>
#include <g4detectors/PHG4DetectorSubsystem.h>
#include <g4eval/PHG4DSTReader.h>
#include <g4main/HepMCNodeReader.h>
#include <g4main/PHG4ParticleGenerator.h>
#include <g4main/PHG4ParticleGeneratorBase.h>
#include <g4main/PHG4ParticleGeneratorVectorMeson.h>
#include <g4main/PHG4ParticleGun.h>
#include <g4main/PHG4Reco.h>
#include <g4main/PHG4SimpleEventGenerator.h>
#include <g4main/PHG4TruthSubsystem.h>
#include <phhepmc/Fun4AllHepMCInputManager.h>
#include <phool/recoConsts.h>
#include <phpythia6/PHPythia6.h>
#include <phpythia8/PHPythia8.h>
#include <phsartre/PHSartre.h>
#include <phsartre/PHSartreParticleTrigger.h>
#include <synradana/ReadSynRadFiles.h>

#include <fun4all/Fun4AllServer.h>
#include <g4eval/PHG4DSTReader.h>
#include <g4main/HepMCNodeReader.h>
R__LOAD_LIBRARY(libg4eval.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4testbench.so)
R__LOAD_LIBRARY(libPHPythia6.so)
R__LOAD_LIBRARY(libPHPythia8.so)
R__LOAD_LIBRARY(libPHSartre.so)
R__LOAD_LIBRARY(libg4eval.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4testbench.so)
R__LOAD_LIBRARY(libg4detectors.so)

#include <synradana/SynRadAna.h>
#include "DisplayOn.C"
R__LOAD_LIBRARY(libSynRadAna.so)
#endif

using namespace std;

int Fun4All_G4_EICDetector(
        const int nEvents = 10000000,
//    const int nEvents = 1000,
    //                           const char * inputFile = "data/SynRad Example Particle log.csv",
    const char *inputFile = "data/Particle log facet 18952 +4.5m.csv",
    //                           const char * inputFile = "data/23April2020_incidentFlux200- -200 cm.csv",
    const char *outputFile = "G4EICDetector.root")
{
  ///////////////////////////////////////////
  // Make the Server
  //////////////////////////////////////////
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);  // enable some blabbering

  recoConsts *rc = recoConsts::instance();
  // uncomment and change number (or not)if you want to use a fixed seed
  //  rc->set_IntFlag("RANDOMSEED", 12345);

  //===============
  // Input options
  //===============

  //  //  // this module is needed to read the EICTree style records into our G4 sims
  //  ReadSynRadFiles *eicr = new ReadSynRadFiles();
  //  eicr->OpenInputFile(inputFile);
  //  eicr->SetEntryPerEvent(1);
  //  //  eicr->Verbosity(1);
  //  se->registerSubsystem(eicr);
  //
  //  // read-in HepMC events to Geant4 if there is any
  //  HepMCNodeReader *hr = new HepMCNodeReader();
  //  se->registerSubsystem(hr);

  //  //  //   particle gun
  //    PHG4ParticleGun *gun = new PHG4ParticleGun("PGUN");
  //    //  gun->set_name("anti_proton");
  //    gun->set_name("gamma");
  //    //  gun->set_name("proton");
  //    gun->set_vtx(0, 0, 450);
  //    gun->set_mom(0, 0, -100);
  //    se->registerSubsystem(gun);

  PHG4SimpleEventGenerator *gen = new PHG4SimpleEventGenerator();
  gen->add_particles("gamma", 1);  // mu+,e+,proton,pi+,Upsilon
                                   //gen->add_particles("pi+",100); // 100 pion option

  gen->set_vertex_distribution_function(PHG4SimpleEventGenerator::Uniform,
                                        PHG4SimpleEventGenerator::Uniform,
                                        PHG4SimpleEventGenerator::Uniform);
  gen->set_vertex_distribution_mean(0.0, 0.0, 450.0);
  gen->set_vertex_distribution_width(0.1, 0.1, 0.0);

  gen->set_eta_range(-100, -100);
  gen->set_phi_range(-1.0 * TMath::Pi(), 1.0 * TMath::Pi());
  //gen->set_pt_range(0.1, 50.0);
  gen->set_p_range(0e-6, 100e-6);
  gen->Embed(1);
  gen->Verbosity(0);
  se->registerSubsystem(gen);

  // Fun4All G4 module
  PHG4Reco *g4Reco = new PHG4Reco();

  g4Reco->SetWorldMaterial("G4_Galactic");
  // no magnetic field
  g4Reco->set_field(0);
  // size of the world - every detector has to fit in here
  g4Reco->SetWorldSizeX(500);
  g4Reco->SetWorldSizeY(500);
  g4Reco->SetWorldSizeZ(2000);
  // shape of our world - it is a box
  g4Reco->SetWorldShape("G4BOX");
  // this is what our world is filled with
  g4Reco->SetWorldMaterial("G4_Galactic");
  // Geant4 Physics list to use
  g4Reco->SetPhysicsList("FTFP_BERT");
    g4Reco->SetPhysicsList("FTFP_BERT_HP");

  g4Reco->ApplyCommand("/control/verbose 2");
  g4Reco->ApplyCommand("/run/verbose 2");

  // our block "detector", size is in cm
  double xsize = 200.;
  double ysize = 200.;
  double zsize = 300e-4;

  for (int i = 0; i < 1; ++i)
  {
    PHG4BlockSubsystem *box = new PHG4BlockSubsystem("SVTX", i);
    box->SuperDetector("SVTX");
    box->set_double_param("size_x", xsize);
    box->set_double_param("size_y", ysize);
    box->set_double_param("size_z", zsize);
    box->set_double_param("place_z", -i * 3 * zsize);  // shift box so we do not create particles in its center and shift by 10 so we can see the track of the incoming particle
    box->set_string_param("material", "G4_Si");        // material of box
    box->SetActive();                                  // it is an active volume - save G4Hits                           // it is an active volume - save G4Hits
    box->OverlapCheck(true);
    g4Reco->registerSubsystem(box);
  }
  {
    PHG4BlockSubsystem *box = new PHG4BlockSubsystem("PIPE");
    box->SuperDetector("PIPE");
    box->set_double_param("size_x", xsize);
    box->set_double_param("size_y", ysize);
    box->set_double_param("size_z", 762e-4);
    box->set_double_param("place_z", 0.1);       // shift box so we do not create particles in its center and shift by 10 so we can see the track of the incoming particle
    box->set_string_param("material", "G4_Be");  // material of box
    box->SetActive();                            // it is an active volume - save G4Hits
    box->OverlapCheck(true);
    g4Reco->registerSubsystem(box);

  }
  {

    PHG4BlockSubsystem *box = new PHG4BlockSubsystem("PIPE-Au");
    box->SuperDetector("PIPE");
    box->set_double_param("size_x", xsize);
    box->set_double_param("size_y", ysize);
    box->set_double_param("size_z", 2e-4);
    box->set_double_param("place_z", 0.1+2e-4);       // shift box so we do not create particles in its center and shift by 10 so we can see the track of the incoming particle
    box->set_string_param("material", "G4_Au");  // material of box
    box->SetActive();                            // it is an active volume - save G4Hits
    box->OverlapCheck(true);
    g4Reco->registerSubsystem(box);
  }
  PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
  g4Reco->registerSubsystem(truth);

  se->registerSubsystem(g4Reco);

  {
    // save a comprehensive  evaluation file
    PHG4DSTReader *ana = new PHG4DSTReader(
        string(outputFile) + string("_DSTReader.root"));
    ana->set_save_particle(true);
    ana->set_load_all_particle(true);
    ana->set_load_active_particle(true);
    ana->set_save_vertex(true);
    ana->AddNode("SVTX");
    ana->AddNode("PIPE");
    se->registerSubsystem(ana);
  }
  // for single particle generators we just need something which drives
  // the event loop, the Dummy Input Mgr does just that
  Fun4AllInputManager *in = new Fun4AllDummyInputManager("JADE");
  se->registerInputManager(in);

  Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", outputFile);
  se->registerOutputManager(out);

//  {
//    SynRadAna *ana = new SynRadAna();
//    ana->AddNode("SVTX");
//    ana->AddNode("PIPE");
//    //    ana->Verbosity(2);
//    se->registerSubsystem(ana);
//  }

  //-----------------
  // Event processing
  //-----------------
  if (nEvents < 0)
  {
    return 0;
  }
  // if we run the particle generator and use 0 it'll run forever
  if (nEvents == 0)
  {
    cout << "using 0 for number of events is a bad idea when using particle generators" << endl;
    cout << "it will run forever, so I just return without running anything" << endl;
    return 0;
  }

  se->run(nEvents);

  //-----
  // Exit
  //-----

  se->End();
  std::cout << "All done" << std::endl;
  delete se;
  gSystem->Exit(0);
  return 0;
}

// This function is only used to test if we can load this as root6 macro
// without running into unresolved libraries and include files
void RunFFALoadTest() {}
