int Fun4All_G4_pileup(const int nEvents=1,const int jobid=-1,const int active_pileup=0)
{  
  //===============
  // Input options
  //===============
  const bool readhits    =false;
  const bool readhepmc   =true; 
  const bool runpythia8  =false;
  const bool runpythia6  =false;
  const bool do_embedding=false;
  const bool particles   =true  && !readhits;
  const bool usegun      =false && !readhits;
  const bool upsilons    =false && !readhits;
  const bool pileup_evt  =active_pileup;
  const int  n_TPC_layers=40;

  if(jobid>1999){
    cout<<"Error: should be jobid<2000"<<endl;
    return;
  }
  else if(jobid<0){
    const char *inputFile =Form("/sphenix/user/yorito/INTT/macros/macros/g4simulations/test_samples/input_00000.txt");
    const char *pileupFile=Form("/sphenix/user/yorito/INTT/macros/macros/g4simulations/test_samples/pileup_00000.txt");
    const char *outputFile=Form("testout.root");
  }
  else{
    const char *inputFile =Form("/sphenix/user/yorito/INTT/macros/macros/g4simulations/pileup_input/hijing_%05i.txt.bz2",jobid);
    const char *pileupFile=Form("/sphenix/user/yorito/INTT/macros/macros/g4simulations/pileup_input/pileup_%05i.txt",    jobid);
    const char *outputFile=Form("output_%05i.root",jobid);
  }
  cout<<"Input : "<<inputFile <<endl
      <<"Pileup: "<<pileupFile<<endl    
      <<"Output: "<<outputFile<<endl;
  if(pileup_evt) cout<<"event Pile-up: ON" <<endl;
  else           cout<<"event Pile-up: OFF"<<endl;

  //======================
  // What to run
  //======================
  // BBC
  bool do_bbc            =false;
  // Beam pipe
  bool do_pipe           =true;
  // INTT
  bool do_svtx           =true;
  bool do_svtx_cell      =do_svtx && true;
  bool do_svtx_track     =do_svtx_cell && true;
  bool do_svtx_eval      =do_svtx_track && true;
  // Pre-Shower
  bool do_preshower      =false;
  // CEMC
  bool do_cemc           =false;
  bool do_cemc_cell      =do_cemc && true;
  bool do_cemc_twr       =do_cemc_cell && true;
  bool do_cemc_cluster   =do_cemc_twr && true;
  bool do_cemc_eval      =do_cemc_cluster && true;
  // HCAL (inner)
  bool do_hcalin         =false;
  bool do_hcalin_cell    =do_hcalin && true;
  bool do_hcalin_twr     =do_hcalin_cell && true;
  bool do_hcalin_cluster =do_hcalin_twr && true;
  bool do_hcalin_eval    =do_hcalin_cluster && true;
  // Magnet
  bool do_magnet         =false;
  // HCAL (outer)
  bool do_hcalout        =false;
  bool do_hcalout_cell   =do_hcalout && true;
  bool do_hcalout_twr    =do_hcalout_cell && true;
  bool do_hcalout_cluster=do_hcalout_twr && true;
  bool do_hcalout_eval   =do_hcalout_cluster && true;
  // Global Vertex
  bool do_global         =false;
  bool do_global_fastsim =false;
  // Calorimeter trigger
  bool do_calotrigger    =true && do_cemc_twr && do_hcalin_twr && do_hcalout_twr;
  // Jet reconstruction
  bool do_jet_reco       =false;
  bool do_jet_eval       =do_jet_reco && true;
  bool do_HIjetreco      =false && do_jet_reco && do_cemc_twr && do_hcalin_twr && do_hcalout_twr;
  // DST Output
  bool do_dst_compress   =false;
  bool do_DSTReader      =false;

  //---------------
  // Load libraries
  //---------------
  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libphhepmc.so");
  gSystem->Load("libg4testbench.so");
  gSystem->Load("libg4hough.so");
  gSystem->Load("libcemc.so");
  gSystem->Load("libg4eval.so");

  // establish the geometry and reconstruction setup
  gROOT->LoadMacro("G4Setup_pileup.C");
  G4Init(do_svtx,do_preshower,do_cemc,do_hcalin,do_magnet,do_hcalout,do_pipe,n_TPC_layers);

  int absorberactive=1; // set to 1 to make all absorbers active volumes
  //  const string magfield = "1.5";
  const string magfield="/phenix/upgrades/decadal/fieldmaps/sPHENIX.2d.root";
  const float magfield_rescale=1.4/1.5; // scale the map to a 1.4 T field

  //---------------
  // Fun4All server
  //---------------
  Fun4AllServer *se=Fun4AllServer::instance();
  se->Verbosity(0);
  recoConsts    *rc=recoConsts::instance();
  // By default every random number generator uses
  // PHRandomSeed() which reads /dev/urandom to get its seed
  // if the RANDOMSEED flag is set its value is taken as seed
  // You ca neither set this to a random value using PHRandomSeed()
  // which will make all seeds identical (not sure what the point of
  // this would be:
  //  rc->set_IntFlag("RANDOMSEED",PHRandomSeed());
  // or set it to a fixed value so you can debug your code
  //  rc->set_IntFlag("RANDOMSEED", 12345);

  //-----------------
  // Event generation
  //-----------------
  if(readhits){
    if(do_embedding){
      cout <<"Do not support read hits and embed background at the same time."<<endl;
      exit(1);
    }
  }
  else if(readhepmc){
    gSystem->Load("libg4dst.so");
    HepMCNodeReader *hr=new HepMCNodeReader();
    hr->VertexPosition(0.,0.,0.);
    hr->SmearVertex(0.,0.,5.);
    se->registerSubsystem(hr);
  }
  else if(runpythia8){
    gSystem->Load("libPHPythia8.so");
    PHPythia8 *pythia8=new PHPythia8();
    // see coresoftware/generators/PHPythia8 for example config
    pythia8->set_config_file("phpythia8.cfg"); 
    se->registerSubsystem(pythia8);
    HepMCNodeReader *hr=new HepMCNodeReader();
    se->registerSubsystem(hr);
  }
  else if(runpythia6){
    gSystem->Load("libPHPythia6.so");
    PHPythia6 *pythia6=new PHPythia6();
    pythia6->set_config_file("phpythia6.cfg");
    se->registerSubsystem(pythia6);
    HepMCNodeReader *hr=new HepMCNodeReader();
    se->registerSubsystem(hr);
  }
  // Embedded particles or simple particle generator
  if(particles){      
    PHG4SimpleEventGenerator *gen=new PHG4SimpleEventGenerator();
    gen->add_particles("pi-",10); // mu+,e+,proton,pi+,Upsilon
    gen->add_particles("pi+",10); // mu+,e+,proton,pi+,Upsilon
    if(readhepmc || do_embedding){
      gen->set_reuse_existing_vertex(true);
      gen->set_existing_vertex_offset_vector(0.,0.,0.);
    }
    else{
      gen->set_vertex_distribution_function(PHG4SimpleEventGenerator::Uniform,
					    PHG4SimpleEventGenerator::Uniform,
					    PHG4SimpleEventGenerator::Uniform);
      gen->set_vertex_distribution_mean(0.,0.,0.);
      gen->set_vertex_distribution_width(0.,0.,5.);
    }
    gen->set_vertex_size_function(PHG4SimpleEventGenerator::Uniform);
    gen->set_vertex_size_parameters(0.,0.);
    gen->set_eta_range(-1.5,1.5);
    gen->set_phi_range(-1.*TMath::Pi(),1.*TMath::Pi());
    gen->set_pt_range(0.1,40.0);
    gen->Embed(1);
    gen->Verbosity(0);
    se->registerSubsystem(gen);
  }
  if(usegun){
    PHG4ParticleGun *gun=new PHG4ParticleGun();
    // gun->set_name("anti_proton");
    gun->set_name("geantino");
    gun->set_vtx(0,0,0);
    gun->set_mom(10,0,0.01);
    // gun->AddParticle("geantino",1.7776,-0.4335,0.);
    // gun->AddParticle("geantino",1.7709,-0.4598,0.);
    // gun->AddParticle("geantino",2.5621,0.60964,0.);
    // gun->AddParticle("geantino",1.8121,0.253,0.);
    // se->registerSubsystem(gun);
    PHG4ParticleGenerator *pgen=new PHG4ParticleGenerator();
    pgen->set_name("geantino");
    pgen->set_z_range(0,0);
    pgen->set_eta_range(0.01,0.01);
    pgen->set_mom_range(10,10);
    pgen->set_phi_range(5.3/180.*TMath::Pi(),5.7/180.*TMath::Pi());
    se->registerSubsystem(pgen);
  }
  if(upsilons){
    PHG4ParticleGeneratorVectorMeson *vgen=new PHG4ParticleGeneratorVectorMeson();
    vgen->add_decay_particles("e+","e-",0); // i=decay id
    if(readhepmc || do_embedding || particles) vgen->set_reuse_existing_vertex(true);
    else      	                               vgen->set_vtx_zrange(-10.,+10.);
    // Note: this rapidity range completely fills the acceptance of eta = +/- 1 unit
    vgen->set_rapidity_range(-1.,+1.);
    vgen->set_pt_range(0.,10.);    
    int istate=1;    
    if     (istate==1){ // Upsilon(1S)
      vgen->set_mass(9.46);
      vgen->set_width(54.02e-6);
    }
    else if(istate==2){	// Upsilon(2S)
      vgen->set_mass(10.0233);
      vgen->set_width(31.98e-6);
    }
    else{ // Upsilon(3S)
      vgen->set_mass(10.3552);
      vgen->set_width(20.32e-6);
    }
    vgen->Verbosity(0);
    vgen->Embed(2);
    se->registerSubsystem(vgen);
    cout << "Upsilon generator for istate = " << istate << " created and registered "  << endl;	  
  }      

  //---------------------
  // Detector description
  //---------------------
  if(!readhits){
    G4Setup(absorberactive, magfield, TPythia6Decayer::kAll,
	    do_svtx,do_preshower,do_cemc,do_hcalin,do_magnet,do_hcalout,do_pipe,magfield_rescale);
  }

  //---------
  // BBC Reco
  //---------
  if(do_bbc){
    gROOT->LoadMacro("G4_Bbc.C");
    BbcInit();
    Bbc_Reco();
  }

  //------------------
  // Detector Division
  //------------------
  if(do_svtx_cell)    Svtx_Cells();
  if(do_cemc_cell)    CEMC_Cells();
  if(do_hcalin_cell)  HCALInner_Cells();
  if(do_hcalout_cell) HCALOuter_Cells();

  //-----------------------------
  // CEMC towering and clustering
  //-----------------------------
  if(do_cemc_twr)     CEMC_Towers();
  if(do_cemc_cluster) CEMC_Clusters();
  
  //-----------------------------
  // HCAL towering and clustering
  //-----------------------------
  if(do_hcalin_twr)      HCALInner_Towers();
  if(do_hcalin_cluster)  HCALInner_Clusters();
  if(do_hcalout_twr)     HCALOuter_Towers();
  if(do_hcalout_cluster) HCALOuter_Clusters();
  if(do_dst_compress)    ShowerCompress();

  //--------------
  // SVTX tracking
  //--------------
  if(do_svtx_track) Svtx_Reco();

  //-----------------
  // Global Vertexing
  //-----------------
  if(do_global){
    gROOT->LoadMacro("G4_Global.C");
    Global_Reco();
  }
  else if(do_global_fastsim){
    gROOT->LoadMacro("G4_Global.C");
    Global_FastSim();
  }  

  //-----------------
  // Calo Trigger Simulation
  //-----------------
  if(do_calotrigger){
    gROOT->LoadMacro("G4_CaloTrigger.C");
    CaloTrigger_Sim();
  }

  //---------
  // Jet reco
  //---------
  if(do_jet_reco){
    gROOT->LoadMacro("G4_Jets.C");
    Jet_Reco();
  }
  if(do_HIjetreco){
    gROOT->LoadMacro("G4_HIJetReco.C");
    HIJetReco();
  }

  //----------------------
  // Simulation evaluation
  //----------------------
  cout<<"Evaluator output to file "<<outputFile<<endl;
  if (do_svtx_eval)    Svtx_Eval(outputFile);
  if (do_cemc_eval)    CEMC_Eval(string(outputFile) + "_g4cemc_eval.root");
  if (do_hcalin_eval)  HCALInner_Eval(string(outputFile) + "_g4hcalin_eval.root");
  if (do_hcalout_eval) HCALOuter_Eval(string(outputFile) + "_g4hcalout_val.root");
  if (do_jet_eval)     Jet_Eval(string(outputFile) + "_g4jet_eval.root");

  //-------------- 
  // IO management
  //--------------
  if(readhits){
    // Hits file
    Fun4AllInputManager *hitsin=new Fun4AllDstInputManager("DSTin");
    hitsin->fileopen(inputFile);
    se->registerInputManager(hitsin);
  }
  if(do_embedding){
    if(embed_input_file==NULL){
      cout<<"Missing embed_input_file! Exit";
      exit(3);
    }
    Fun4AllDstInputManager *in1=new Fun4AllNoSyncDstInputManager("DSTinEmbed");
    // in1->AddFile(embed_input_file); // if one use a single input file
    in1->AddListFile(embed_input_file); // RecommendedL: if one use a text list of many input files
    se->registerInputManager(in1);
  }
  if(readhepmc){
    Fun4AllInputManager *in=new Fun4AllHepMCInputManager("HEPMCIN");
    se->registerInputManager(in);
    se->fileopen(in->Name().c_str(),inputFile);
    // pile-up events
    if(pileup_evt){
      Fun4AllHepMCInputManager::VTXFUNC gaus   =Fun4AllHepMCInputManager::Gaus;
      Fun4AllHepMCPileupInputManager    *pileup=new Fun4AllHepMCPileupInputManager("PILEUPIN");
      pileup->set_vertex_distribution_function(gaus,gaus,gaus);
      pileup->set_vertex_distribution_mean(0.,0.,0.);   // cm
      pileup->set_vertex_distribution_width(0.,0.,20.); // cm    
      pileup->set_time_window(-17500.,+17500.); // ns
      pileup->set_collision_rate(50); // kHz
      se->registerInputManager(pileup);
      // for(int ifile=0;ifile<10;++ifile) pileup->AddFile(pileupFile);
      se->fileopen(pileup->Name().c_str(),pileupFile);
    }
  }
  else{
    // for single particle generators we just need something which drives
    // the event loop, the Dummy Input Mgr does just that
    Fun4AllInputManager *in=new Fun4AllDummyInputManager( "JADE");
    se->registerInputManager(in);
  }

  if(do_DSTReader){
    // Convert DST to human command readable TTree for quick poke around the outputs
    gROOT->LoadMacro("G4_DSTReader.C");
    G4DSTreader(outputFile,     // char
		absorberactive, // int
		do_svtx,        // bool
		do_preshower,   // bool
		do_cemc,        // bool
		do_hcalin,      // bool
		do_magnet,      // bool
		do_hcalout,     // bool
		do_cemc_twr,    // bool
		do_hcalin_twr,  // bool
		do_magnet,      // bool
		do_hcalout_twr  // bool
		);
  }
  
  // Fun4AllDstOutputManager *out=new Fun4AllDstOutputManager("DSTOUT",outputFile);
  // if(do_dst_compress) DstCompress(out);
  // se->registerOutputManager(out);

  //-----------------
  // Event processing
  //-----------------
  if(nEvents<0) return;
  if(nEvents==0 && !readhits && !readhepmc){
    cout<<"using 0 for number of events is a bad idea when using particle generators"<<endl;
    cout<<"it will run forever, so I just return without running anything"<<endl;
    return;
  }  
  se->run(nEvents);

  //-----
  // Exit
  //-----
  se->End();
  std::cout<<"All done"<<std::endl;
  delete se;
  gSystem->Exit(0);
}
