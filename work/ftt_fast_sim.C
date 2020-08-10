//usr/bin/env root4star -l -b -q  $0; exit $?
// that is a valid shebang to run script as executable

TFile *output = 0;

void stgc_fast_sim(int n = 1000,
                  const char *inFile = "fzd/f12_testg.fzd",
                  const char *geom = "dev2021" ) {
    TString _geom = geom;

    TString _chain;

    // NOTE "event" does not work in CMAKE StRoot wo network, it includes detDb - root problem. Swap to StEvent instead
    _chain = Form("fzin %s StEvent evout geantout ReverseField agml usexgeom bigbig", _geom.Data());

    gROOT->LoadMacro("bfc.C");
    bfc(-1, _chain, inFile);

    // StarMagField::setConstBz(true);

    gSystem->Load("libMathMore.so");

    // Needed for StarRandom
    // gSystem->Load( "StarGeneratorUtil" );

    gSystem->Load("libStFTTSimulatorMaker.so");

    TString qaoutname(gSystem->BaseName(inFile));
    qaoutname.ReplaceAll(".fzd", ".FastSimu.QA.root");

    // Create fast simulator and add after event maker
    
    StFTTSimulatorMaker *fttSim = new StFTTSimulatorMaker();

    // fttSim->setQAFileName(qaoutname);

    // NOTE: WAS AddBefore( "0Event", fttSim)
    // but changed sonce "event" was removed from chain
    cout << "Adding StFTTSimulatorMaker to chain" << endl;
    chain->AddMaker(fttSim);
    chain->Init();

    // Output filename setup
    TString outname(gSystem->BaseName(inFile));
    outname.ReplaceAll(".fzd", "");
    output = TFile::Open(Form("%s_output.root", outname.Data()), "recreate");

    //_____________________________________________________________________________
    //
    // MAIN EVENT LOOP
    //_____________________________________________________________________________
    for (int i = 0; i < n; i++) {

        chain->Clear();
        if (kStOK != chain->Make())
            break;
    }

    output->cd();
    output->Close();
}
