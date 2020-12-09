#!/bin/bash
date

mkdir -p output_all script_all log_all

rm -rf output_all/*.root
rm -rf script_all/*
rm -rf log_all/*

if [ -e submitJob.con ]; then
     rm -rf submitJob.con
fi

echo $PWD

echo "Universe     = vanilla">>submitJob.con
echo "Notification = never">>submitJob.con
echo "Initialdir   = $PWD">>submitJob.con
echo "GetEnv       = True">>submitJob.con
echo "+Job_Type    = \"cas\"">>submitJob.con
echo "">>submitJob.con
echo "">>submitJob.con

njobs=2000
ifile=1

while [ $ifile -le $njobs ] 
do
     echo $ifile

     echo "#!/bin/csh">>script_all/run_$ifile.csh
     echo "source /gpfs02/eic/shuaiy/eicTofSim/macros/detectors/EICDetector/setupEnv.csh">>script_all/run_$ifile.csh

     echo "">>script_all/run_$ifile.csh
     echo "">>script_all/run_$ifile.csh

     echo "root -l -b <<EOF ">>script_all/run_$ifile.csh
     echo -n ".x Fun4All_G4_EICDetector.C(">> script_all/run_$ifile.csh
     echo -n "500, ">> script_all/run_$ifile.csh
     echo -n "\"/sphenix/data/data02/review_2017-08-02/single_particle/spacal2d/fieldmap/G4Hits_sPHENIX_e-_eta0_8GeV-0002.root\", ">> script_all/run_$ifile.csh
     echo -n "\"G4EICDetector_${ifile}.root\", ">> script_all/run_$ifile.csh
     echo -n "\"https://www.phenix.bnl.gov/WWW/publish/phnxbld/sPHENIX/files/sPHENIX_G4Hits_sHijing_9-11fm_00000_00010.root\", ">> script_all/run_$ifile.csh
     echo -n "0, ">> script_all/run_$ifile.csh
     echo -n "\"output_all\"">> script_all/run_$ifile.csh
     echo ")">> script_all/run_$ifile.csh 
     echo ".q">> script_all/run_$ifile.csh      
     echo "EOF">> script_all/run_$ifile.csh 

     chmod 755 script_all/run_$ifile.csh

     echo "Executable       = /bin/csh">>submitJob.con
     echo "Arguments        = \"script_all/run_$ifile.csh\"">>submitJob.con
     echo "Output           = log_all/run_$ifile.out">>submitJob.con
     echo "Error            = log_all/run_$ifile.err">>submitJob.con
     echo "Log              = log_all/run_$ifile.olog">>submitJob.con
     echo  "Queue" >>submitJob.con
     echo  "     " >>submitJob.con
      
     let "ifile+=1";
done

condor_submit submitJob.con
