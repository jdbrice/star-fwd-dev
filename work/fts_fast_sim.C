//usr/bin/env root4star -l -b -q  $0; exit $?
// that is a valid shebang to run script as executable

TH2F *hWirePulls = 0;
TH2F *hStripPulls = 0;
TH2F *hWireYX;
TH2F *hStripYX;

TGraph *gWireLocal = 0;
TGraphErrors *gStripLocal = 0;

TH2F *hGlobalYX = 0;
TH2F *hGlobalYXdig = 0;
TH2F *hGlobalYXdif = 0;
//TH3F *hGlobalYXZ = 0;
TH3F *hGlobalYXZDisk = 0;
TH3F *hErrorXYZ = 0;
TH2F *hnHitsDisk = 0;

TH2F *h_hitmap[15];

// Silicon disk segmentation
int nR = 8;   // radial divisions
int nS = 12;  // sectors
int nP = 128; // phistrips

TFile *output = 0;

void do_simple_ana(int i);

void fts_fast_sim(int n = 1000,
                  const char *inFile = "fzd/f12_testg.fzd",
                  const char *geom = "dev2021",
                  bool SiIneff = false,
                  bool graph = false) {
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

    gSystem->Load("libStFtsFastSimulatorMaker.so");

    TString qaoutname(gSystem->BaseName(inFile));
    qaoutname.ReplaceAll(".fzd", ".FastSimu.QA.root");

    // Create fast simulator and add after event maker
    if (_geom.Contains("dev2021")) {
        StFtsFastSimulatorMaker *frssim = new StFtsFastSimulatorMaker();

        frssim->setPointHits(); // X&Y combined to points
        frssim->setPixels(nR, nS, nP);
        frssim->setRaster(0.1);

        if (SiIneff)
            frssim->setInEfficiency(0.1);

        frssim->setQAFileName(qaoutname);

        // NOTE: WAS AddBefore( "0Event", frssim)
        // but changed sonce "event" was removed from chain
        cout << "Adding StFtsFastSimulatorMaker to chain" << endl;
        chain->AddMaker(frssim);
    }

    chain->Init();

    // Output filename setup
    TString outname(gSystem->BaseName(inFile));
    outname.ReplaceAll(".fzd", "");
    output = TFile::Open(Form("%s_output.root", outname.Data()), "recreate");

    //_____________________________________________________________________________
    //
    // SETUP SOME QA HISTOGRAMS
    //_____________________________________________________________________________
    hGlobalYX = new TH2F("hGlobalYX", "Global Y vs X;X [cm] Y [cm]", 151, -75.5, 75.5, 151, -75.5, 75.5);
    hGlobalYXZDisk = new TH3F("hGlobalYXZDisk", "Global Y vs X vs ZDisk;X [cm]; Y [cm]; Z [disk]", 151, -75.5, 75.5, 151, -75.5, 75.5, 20, 0, 20);
    hErrorXYZ = new TH3F("hErrorXYZ", "Error Z vs Y vs X;X [cm]; Y [cm]; Z [cm]", 120, -0.12, 0.12, 120, -0.12, 0.12, 120, -0.12, 0.12);
    hnHitsDisk = new TH2F("hnHitsDisk", "nHits Dentisity vs Disk; nHits; Disk", 100, 0, 100, 10, 0, 10);
    hGlobalYXdig = new TH2F("hGlobalYXdig", "Global Y vs X (digitized);X [cm] Y [cm]", 151, -75.5, 75.5, 151, -75.5, 75.5);
    hGlobalYXdif = new TH2F("hGlobalYXdif", "Global Y vs X (z=diff);X [cm] Y [cm]", 151, -75.5, 75.5, 151, -75.5, 75.5);

    for (int i = 0; i < 15; i++) {
        h_hitmap[i] = new TH2F(Form("h_hitmap_%d", i), "Global Y vs X; X [cm]; Y [cm]", 151, -75.5, 75.5, 151, -75.5, 75.5);
    }

    // Pulls distributions
    hWire = new TH2F("hWire", " YX", 201, -100.5, 100.5, 201, -100.5, 100.5);
    hStrip = new TH2F("hStrip", " YX", 201, -100.5, 100.5, 201, -100.5, 100.5); // 20

    hWireYX = new TH2F("hWireYX", "#Delta Y vs #Delta X", 81, -40.5, 40.5, 81, -40.5, 40.5);
    hStripYX = new TH2F("hStripYX", "#Delta Y vs #Delta X", 81, -40.5, 40.5, 81, -40.5, 40.5);

    hWirePull = new TH2F("hWirePull", "Wire pulls", 81, -4.05, 4.05, 81, -4.05, 4.05);
    hStripPull = new TH2F("hStripPull", "Strip pulls", 81, -4.05, 4.05, 81, -4.05, 4.05);

    //_____________________________________________________________________________
    //
    // MAIN EVENT LOOP
    //_____________________________________________________________________________
    for (int i = 0; i < n; i++) {

        chain->Clear();
        if (kStOK != chain->Make())
            break;

        // Simple analysis and QA histograms
        do_simple_ana(i);
    }

    output->cd();
    hGlobalYX->Write();
    hGlobalYXZDisk->Write();
    hErrorXYZ->Write();
    hnHitsDisk->Write();
    for (int j = 0; j < 15; j++) {
        h_hitmap[j]->Write();
    }
    output->Close();
}

void do_simple_ana(int i) {
    StEvent *event = (StEvent *)chain->GetDataSet("StEvent");
    if (0 == event) {
        cout << "No event found" << endl;
        return;
    }

    StRnDHitCollection *rndCollection = event->rndHitCollection();
    if (0 == rndCollection) {
        cout << "No collection found" << endl;
    }
    cout << "==== processing event: " << i << " ====" << endl;
    const StSPtrVecRnDHit &hits = rndCollection->hits();
    int nHitsOnDisk[6] = {0, 0, 0, 0, 0, 0};

    for (unsigned int ihit = 0; ihit < hits.size(); ihit++) {
        StRnDHit *hit = hits[ihit];
        if (0 == hit) {
            cout << "No hit found" << endl;
        }

        const StThreeVectorF pos = hit->position();
        float x = pos[0];          // simulated
        float x0 = hit->double0(); // true
        float xl = hit->double3(); // digitized local
        float y = pos[1];          // simulated
        float y0 = hit->double1(); // true
        float yl = hit->double4(); // digitized local
        float z = pos[2];          // simulated
        float z0 = hit->double2(); // true
        const StThreeVectorF err = hit->positionError();
        hErrorXYZ->Fill(err.x(), err.y(), err.z());
        float ex = err[0];
        float ey = err[1];
        int disk = hit->layer() - 6;
        int orientation = hit->ladder(); // 0=strips, 1=wires

        cout << "disk=" << hit->layer() << " ";
        hit->Print();

        if (disk >= 7) {
            continue;
            cout << "Error -- off by 1" << endl;
        }

        cout << "Filling Histograms" << endl;

        hGlobalYX->Fill(x, y);
        hGlobalYXZDisk->Fill(x, y, hit->layer());

        cout << "Counting Hits on disks" << endl;
        for (int ii = 0; ii < 6; ii++) {
            if (hit->layer() == (ii + 1))
                nHitsOnDisk[ii]++;
        }

        cout << "Filling Hitmap" << endl;
        for (int j = 0; j < 15; j++) {
            if (hit->layer() != j)
                continue;

            h_hitmap[j]->Fill(x, y);
        }
    }

    cout << "Filling Hits on disk" << endl;
    for (int ii = 0; ii < 6; ii++)
        hnHitsDisk->Fill(nHitsOnDisk[ii], ii + 1);

    cout << "Event ana complete" << endl;
}