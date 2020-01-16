// $Id: $

/*!
 * \file Fun4All_G4_Readback.C
 * \brief 
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision:   $
 * \date $Date: $
 */

#if ROOT_VERSION_CODE >= ROOT_VERSION(6, 00, 0)

//#include <anatutorial/AnaTutorial.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllNoSyncDstInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/SubsysReco.h>
#include <phhepmc/Fun4AllHepMCInputManager.h>

//R__LOAD_LIBRARY(libanatutorial.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4dst.so)

#endif

using namespace std;

int Fun4All_G4_Readback(
    const int nEvents = 50,
    const char *inputFile = "G4sPHENIX.root",
    const char *outputFile = "G4sPHENIX_Readback.root")
{
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4dst.so");

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(01);

  //--------------
  // IO management
  //--------------
  // Hits file
  Fun4AllInputManager *hitsin = new Fun4AllDstInputManager("DSTin");
  hitsin->fileopen(inputFile);
  se->registerInputManager(hitsin);

  //  cout << "start book my analysis" << endl;
  //  AnaTutorial *anaTutorial = new AnaTutorial("anaTutorial", string(outputFile) + "_anaTutorial.root");
  //  anaTutorial->setMinJetPt(10.);
  //  anaTutorial->Verbosity(100);
  //  anaTutorial->analyzeTracks(true);
  //  anaTutorial->analyzeClusters(false);
  //  anaTutorial->analyzeJets(false);
  //  anaTutorial->analyzeTruth(false);
  //  se->registerSubsystem(anaTutorial);
  //  cout << "register subsystem" << endl;
  //-----------------
  // Event processing
  //-----------------
  if (nEvents < 0)
  {
    return 0;
  }

  se->run(nEvents);
  // se->run(0);

  //-----
  // Exit
  //-----

  se->End();
  std::cout << "All done" << std::endl;
  delete se;
  gSystem->Exit(0);
  return 0;
}
