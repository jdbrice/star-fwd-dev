

void plots( TString fin="results.root" ){

	gStyle->SetOptStat(0);
	gStyle->SetLineWidth(2);

	TCanvas *can = new TCanvas( "c", "", 1200, 900 );
	can->SetTopMargin( 0.1 );
	can->SetRightMargin( 0.1 );
	can->SetBottomMargin( 0.13 );
	can->SetLeftMargin( 0.13 );
	TFile *finput = new TFile( fin );

	// *********************************************** //
	// Efficiency vs. McEta
	TH1* hEffEta = (TH1*)finput->Get( "EffVsMcEta_4hits" );
	hEffEta->Draw( "hp" );
	hEffEta->SetLineColor(kBlue);
	hEffEta->SetLineWidth(2);
	hEffEta->SetTitle( "; #eta^{GEN}; Efficiency = (FOUND / GEN)" );
	hEffEta->GetXaxis()->SetTitleSize( 20.0 / 360 );
	hEffEta->GetYaxis()->SetTitleSize( 20.0 / 360 );
	hEffEta->GetXaxis()->SetLabelSize( 16.0 / 360 );
	hEffEta->GetYaxis()->SetLabelSize( 16.0 / 360 );
	hEffEta->GetXaxis()->SetTitleOffset(0.8);
	hEffEta->GetYaxis()->SetTitleOffset(1.0);

	can->Print( "efficiency_eta.pdf" );

	// *********************************************** //
	// Efficiency vs. McPhi
	TH1* hEffPhi = (TH1*)finput->Get( "EffVsMcPhi_4hits" );
	hEffPhi->Draw( "hp" );
	hEffPhi->SetLineColor(kBlue);
	hEffPhi->SetLineWidth(2);
	hEffPhi->SetTitle( "; #phi^{GEN}; Efficiency = (FOUND / GEN)" );
	hEffPhi->GetXaxis()->SetTitleSize( 20.0 / 360 );
	hEffPhi->GetYaxis()->SetTitleSize( 20.0 / 360 );
	hEffPhi->GetXaxis()->SetLabelSize( 16.0 / 360 );
	hEffPhi->GetYaxis()->SetLabelSize( 16.0 / 360 );
	hEffPhi->GetXaxis()->SetTitleOffset(0.8);
	hEffPhi->GetYaxis()->SetTitleOffset(1.0);

	can->Print( "efficiency_phi.pdf" );

	// *********************************************** //
	// Efficiency vs. McPt
	TH1* hEffPt = (TH1*)finput->Get( "EffVsMcPt_4hits" );
	hEffPt->Draw( "hp" );
	hEffPt->SetLineColor(kBlue);
	hEffPt->SetLineWidth(2);
	hEffPt->SetTitle( "; p_{T}^{GEN}; Efficiency = (FOUND / GEN)" );
	hEffPt->GetXaxis()->SetTitleSize( 20.0 / 360 );
	hEffPt->GetYaxis()->SetTitleSize( 20.0 / 360 );
	hEffPt->GetXaxis()->SetLabelSize( 16.0 / 360 );
	hEffPt->GetYaxis()->SetLabelSize( 16.0 / 360 );
	hEffPt->GetXaxis()->SetTitleOffset(0.8);
	hEffPt->GetYaxis()->SetTitleOffset(1.0);
	hEffPt->GetXaxis()->SetRangeUser( 0, 5.0 );

	can->Print( "efficiency_pt.pdf" );

	// *********************************************** //
	// Charge Mis-ID vs. p_T
	TH1* hAllQ = (TH1*)finput->Get( "AllQVsMcPt" );
	TH1* hWrongQ = (TH1*)finput->Get( "WrongQVsMcPt" );

	for ( int i = 1; i <52; i++ ){
		hWrongQ->SetBinContent(i, 0);
	}

	hAllQ->Sumw2();
	hWrongQ->Sumw2();
	hAllQ->Rebin(4);
	hWrongQ->Rebin(4);

	hWrongQ->Divide(hAllQ);
	hWrongQ->Draw( "hpe" );
	hWrongQ->GetXaxis()->SetRangeUser( 0, 5.0 );
	hWrongQ->GetYaxis()->SetRangeUser( 0, 0.3 );
	hWrongQ->SetLineColor(kBlue);
	hWrongQ->SetLineWidth(2);
	hWrongQ->SetTitle( "; p_{T}^{GEN}; Charge Mis-ID Rate" );
	hWrongQ->GetXaxis()->SetTitleSize( 24.0 / 360 );
	hWrongQ->GetYaxis()->SetTitleSize( 24.0 / 360 );
	hWrongQ->GetXaxis()->SetLabelSize( 16.0 / 360 );
	hWrongQ->GetYaxis()->SetLabelSize( 16.0 / 360 );
	hWrongQ->GetXaxis()->SetTitleOffset(0.8);
	hWrongQ->GetYaxis()->SetTitleOffset(1.0);
	hWrongQ->Fit("pol1", "R", "", 0, 5.0);


	can->Print( "qmis_pT.pdf" );

	// *********************************************** //
	// Inverse Momentum Resolution vs. p_T
	TH2* h2ires = (TH2*) finput->Get( "InvPtResVsTrue" );


	h2ires->RebinX(2);
	h2ires->RebinY(4);
	h2ires->FitSlicesY();
	TH1 * hSigma = (TH1*)gDirectory->Get( "InvPtResVsTrue_2" );


	hSigma->GetXaxis()->SetRangeUser( 0, 5.0 );
	hSigma->GetYaxis()->SetRangeUser( 0, 1.1 );
	hSigma->Draw( "hpe" );
	hSigma->SetLineWidth(2);
	hSigma->SetLineColor(kBlue);
	hSigma->SetTitle( "; p_{T}^{GEN}; #sigma_{1/pT}" );
	hSigma->GetXaxis()->SetTitleSize( 20.0 / 360 );
	hSigma->GetXaxis()->SetLabelSize( 16.0 / 360 );
	hSigma->GetYaxis()->SetTitleSize( 20.0 / 360 );
	hSigma->GetYaxis()->SetLabelSize( 16.0 / 360 );


	hSigma->Fit("pol1", "R", "", 0, 5.0);
	can->Print( "invptres.pdf" );


}