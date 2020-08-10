//usr/bin/env root4star -l -b -q  $0; exit $?
// that is a valid shebang to run script as executable

TFile *output = 0;

void fast_track(   int n = 100,
                  const char *inFile = "fzd/shenghui.fzd",
                  std::string configFile = "config/fast_sim_default.xml",
                  const char *geom = "dev2021") {
    TString _geom = geom;

    bool SiIneff = false;

    TString _chain;

    // NOTE "event" does not work in CMAKE StRoot wo network, it includes detDb - root problem. Swap to StEvent instead
    _chain = Form("fzin %s StEvent evout geantout ReverseField agml usexgeom bigbig", _geom.Data());

    gROOT->LoadMacro("bfc.C");
    bfc(-1, _chain, inFile);

    // StarMagField::setConstBz(true);

    gSystem->Load("libMathMore.so");
    gSystem->Load("libStSiSimulatorMaker.so");
    gSystem->Load("libStFTTSimulatorMaker.so");

    gSystem->Load("libgenfit2.so");
    gSystem->Load("libKiTrack.so");
    gSystem->Load("libStEventUtilities.so");
    gSystem->Load("libStgMaker.so");

    StFTTSimulatorMaker *fttSim = new StFTTSimulatorMaker();
    cout << "Adding StFTTSimulatorMaker to chain" << endl;
    chain->AddMaker(fttSim);

    TString qaoutname(gSystem->BaseName(inFile));
    qaoutname.ReplaceAll(".fzd", ".FastSimu.QA.root");

    // Create fast simulator and add after event maker
    StSiSimulatorMaker *frssim = new StSiSimulatorMaker();

    frssim->setPointHits(); // X&Y combined to points
    frssim->setPixels(8, 12, 128);
    frssim->setRaster(0.0);

    if (SiIneff)
        frssim->setInEfficiency(0.1);

    frssim->setQAFileName(qaoutname);

    // NOTE: WAS AddBefore( "0Event", frssim)
    // but changed sonce "event" was removed from chain
    cout << "Adding StSiSimulatorMaker to chain" << endl;
    chain->AddMaker(frssim);

    StgMaker *gmk = new StgMaker();
    gmk->SetConfigFile( configFile );
    gmk->GenerateTree( false );
    chain->AddAfter( "fsiSim", gmk );

    chain->Init();

    // Output filename setup
    // TString outname(gSystem->BaseName(inFile));
    // outname.ReplaceAll(".fzd", "");
    // output = TFile::Open(Form("%s_output.root", outname.Data()), "recreate");

    //_____________________________________________________________________________
    //
    // MAIN EVENT LOOP
    //_____________________________________________________________________________
    for (int i = 0; i < n; i++) {

        chain->Clear();
        if (kStOK != chain->Make())
            break;
    }

    // output->cd();
    // output->Close();
}
