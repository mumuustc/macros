int Fun4All_PythiaHepMC(
    const int nEvents = 100,
    const char *inputFile = "phpythia8_MB.cfg",
    const char *outputFile = "phpythia8_MB")
{
  gSystem->Load("libfun4all.so");
  gSystem->Load("libPHPythia8.so");

  //---------------
  // Fun4All server
  //---------------
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);
  // just if we set some flags somewhere in this macro
  recoConsts *rc = recoConsts::instance();

  //-----------------
  // Event generation
  //-----------------

  PHPythia8 *pythia8 = new PHPythia8();
  // see coresoftware/generators/PHPythia8 for example config
  pythia8->set_config_file(inputFile);

  pythia8->print_config();

  se->registerSubsystem(pythia8);

  //-----------------
  // output
  //-----------------
  Fun4AllHepMCOutputManager *out1 = new Fun4AllHepMCOutputManager("HepMCOUT", string(outputFile) + string("_hepmc.dat.gz"));
  out1->set_embedding_id(1);  //! output the pythia sub-event
  se->registerOutputManager(out1);

  //-----------------
  // Event processing
  //-----------------
  se->run(nEvents);

  //-----
  // Exit
  //-----
  gSystem->Exec("ps -o sid,ppid,pid,user,comm,vsize,rssize,time");
  se->End();
  std::cout << "All done" << std::endl;
  delete se;
  gSystem->Exit(0);
}
