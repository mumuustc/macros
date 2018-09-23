#include <string>

using namespace std;

void Fun4All_TPCFEE(int nEvents = 10000000, const char *input_file = "data/tpcfee_00002658-0000.evt",
                    const char *output_file = "data/beam_00000406.root")
{
  gSystem->Load("libfun4all");
  gSystem->Load("libtpcdaq.so");

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(Fun4AllServer::VERBOSITY_SOME);

  recoConsts *rc = recoConsts::instance();
  // rc->set_IntFlag("RUNNUMBER",0);

  // ------------------- Run info -> RUN node -------------------
  TPCFEETestRecov1 *tpcfee = new TPCFEETestRecov1(string(input_file) + string("_TPCFEETestRecov1.root"));
//  tpcfee->Verbosity(TPCFEETestRecov1::VERBOSITY_SOME);
//  tpcfee->Verbosity(TPCFEETestRecov1::VERBOSITY_MORE);

  tpcfee->setNPreSample(5);
  tpcfee->setNPostSample(7);

  se->registerSubsystem(tpcfee);
  //

  Fun4AllInputManager *in = new Fun4AllPrdfInputManager("PRDFin");
  in->fileopen(input_file);
  se->registerInputManager(in);

  se->run(nEvents);

  se->End();
}
