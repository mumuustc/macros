// this macro sets the field loads a precanned configuration for the
// Svx geometry adds an all absorbing layer around it and registers the
// Truth subsystem (always last)
// the pre-canned configurations all return their outer radius with
// an overlap protection added (so they can serve as the radius of an
// immediate following layer)

double  no_overlapp = 0.0001; // added to radii to avoid overlapping volumes
bool overlapcheck = false; // set to true if you want to check for overlaps

void G4Init()
{
  // load detector macros and execute Init() function
  gROOT->LoadMacro("G4_tpc_plus_vtx.C");
  SvtxInit();
}

int G4Setup(const int absorberactive = 0, const string & field = "1.5")
{

  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  //---------------------
  // Detector description
  //---------------------

  PHG4Reco* g4Reco = new PHG4Reco();
  g4Reco->set_rapidity_coverage(1.1); // according to drawings


  // setting the magnetic field
  double fieldstrength;
  istringstream stringline(field);
  stringline >> fieldstrength;
  if (stringline.fail()) // conversion to double fails -> we have a string
    {
      if (field.find("sPHENIX.root") != string::npos)
        {
          g4Reco->set_field_map(field, 1);
        }
      else
        {
          g4Reco->set_field_map(field, 2);
        }
    }
  else
    {
      g4Reco->set_field(fieldstrength); // use const soleniodal field
    }


  double radius = 0.; // 0 is just an initialization

  // the Svtx function returns the outer radius of it with overlap protection
  // the radius argument of the Svtx function is not being used
  radius = Svtx(g4Reco, radius, absorberactive);

  // swallow all particles coming out of the back of this setup
  // and keep track of them to see the leakage and prevent curling muons from reentering
  // the detector when using a constant solenoidal magnetic field in the world volume
  PHG4CylinderSubsystem *blackhole = new PHG4CylinderSubsystem("BH", 1);
  blackhole->SetRadius(radius + 10); // add 10 cm in radius
  blackhole->SetLengthViaRapidityCoverage(false);
  blackhole->SetLength(g4Reco->GetWorldSizeZ() - no_overlapp); // make it cover the world in length
  blackhole->BlackHole();
  blackhole->SetThickness(0.1); // it needs some thickness
  blackhole->SetActive(); // always store what leaks out
  blackhole->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(blackhole);

  // the truth subsystem which keeps track of the truth info has to be
  // registered last
  PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
  g4Reco->registerSubsystem(truth);
  se->registerSubsystem( g4Reco );
}
