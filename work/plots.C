
void common_style( TH1 * h, int col = -1 ){
	h->SetLineWidth(2);
	if ( col >= 0 )
		h->SetLineColor(col);
}

void master_style( TH1 * h ){
	common_style( h );
	h->GetXaxis()->SetTitleSize( 20.0 / 360 );
	h->GetYaxis()->SetTitleSize( 20.0 / 360 );
	h->GetXaxis()->SetLabelSize( 16.0 / 360 );
	h->GetYaxis()->SetLabelSize( 16.0 / 360 );
	h->GetXaxis()->SetTitleOffset(0.8);
	h->GetYaxis()->SetTitleOffset(1.0);
}

void draw_legend( TH1 * h0, TH1 * h1, TH1 * h2, bool show_allq = true ){
	TLegend * tl = new TLegend( 0.5, 0.2, 0.9, 0.5 );
	tl->SetBorderSize(0);
	tl->SetFillStyle(0);
	// tl->AddEntry( h0, "Efficiency x Acceptance", "l" );
	tl->AddEntry( h1, "Efficiency (Quality = 4/4)", "l" );
	// tl->AddEntry( h2, "Efficiency (Quality >= 3/4)", "l" );
	if ( show_allq )
		tl->AddEntry( h2, "Efficiency (Quality >= 3/4)", "l" );
	tl->Draw("same");
}


void efficiency_plots( TString fin="results.root", bool show_allq = true ){
	gStyle->SetOptStat(0);
	gStyle->SetLineWidth(2);

	TCanvas *can = new TCanvas( "c", "", 1200, 900 );
	can->SetTopMargin( 0.1 );
	can->SetRightMargin( 0.1 );
	can->SetBottomMargin( 0.13 );
	can->SetLeftMargin( 0.13 );
	TFile *finput = new TFile( fin );

	// *********************************************** //
	// Duration per event
	TH1 * hDurEvent = (TH1*)finput->Get("DurationPerEvent");
	common_style( hDurEvent, kBlack );
	master_style( hDurEvent );
	hDurEvent->GetXaxis()->SetRangeUser( 0, 500 );
	hDurEvent->Draw();
	gPad->SetLogy(1);

	can->Print( "duration_per_event.pdf" );


	// *********************************************** //
	// All Quality Plot
	TH1 * AllQ = (TH1*)finput->Get("AllQuality");
	common_style( AllQ, kBlack );
	
	
	gStyle->SetPaintTextFormat("0.3f");
	TH1 * hFrame = new TH1F( "hFrame", "", 1, 0, 1.5 );
	hFrame->Draw();
	hFrame->GetYaxis()->SetRangeUser( 5e-5, 10 );
	master_style( hFrame );
	hFrame->SetTitle( "; Quality ( # Correct Hits Found / Total )" );

	AllQ->Scale( 1.0 / AllQ->Integral() );
	AllQ->Draw( "same hist" );
	
	AllQ->Draw( "same text" );
	AllQ->SetMarkerSize( 2 );

	gPad->Update();
	gPad->SetLogy(1);

	can->Print( "all_quality.pdf" );

	gPad->SetLogy(0);

	// *********************************************** //
	// Efficiency vs. McEta
	TH1* hEffAccEtaAllQ = (TH1*)finput->Get( "EffVsMcEtaAllQ" );
	TH1* hEffEta = (TH1*)finput->Get( "EffVsMcEta_4hits" );
	TH1* hEffEtaAllQ = (TH1*)finput->Get( "EffVsMcEta_4hits_AllQ" );
	hEffEta->Draw( "hp" );
	hEffEta->GetXaxis()->SetRangeUser( 2.50, 4.0 );
	hEffEta->GetYaxis()->SetRangeUser( 0, 1.1 );
	common_style( hEffEta, kBlue );
	master_style( hEffEta );
	hEffEta->SetTitle( "; #eta^{GEN}; Efficiency = (FOUND / GEN)" );
	
	common_style( hEffEtaAllQ, kRed );
	if ( show_allq )
		hEffEtaAllQ->Draw("same hp");

	common_style( hEffAccEtaAllQ, kBlack );
	// hEffAccEtaAllQ->Draw("same hp");

	draw_legend( hEffAccEtaAllQ, hEffEta, hEffEtaAllQ, show_allq );

	can->Print( "efficiency_eta.pdf" );

	// *********************************************** //
	// Efficiency vs. McPhi
	TH1* hEffAccPhiAllQ = (TH1*)finput->Get( "EffVsMcPhiAllQ" );
	TH1* hEffPhiAllQ = (TH1*)finput->Get( "EffVsMcPhi_4hits_AllQ" );
	TH1* hEffPhi = (TH1*)finput->Get( "EffVsMcPhi_4hits" );
	hEffPhi->Draw( "hp" );
	common_style(hEffPhi, kBlue);
	hEffPhi->SetTitle( "; #phi^{GEN}; Efficiency = (FOUND / GEN)" );
	master_style(hEffPhi);

	common_style( hEffPhiAllQ, kRed );
	if ( show_allq )
		hEffPhiAllQ->Draw("same hp");

	common_style( hEffAccPhiAllQ, kBlack );
	// hEffAccPhiAllQ->Draw("same hp");
	draw_legend( hEffAccPhiAllQ, hEffPhi, hEffPhiAllQ, show_allq );

	can->Print( "efficiency_phi.pdf" );

	// *********************************************** //
	// Efficiency vs. McPt
	TH1* hEffAccPtAllQ = (TH1*)finput->Get( "EffVsMcPtAllQ" );
	TH1* hEffPt = (TH1*)finput->Get( "EffVsMcPt_4hits" );
	TH1* hEffPtAllQ = (TH1*)finput->Get( "EffVsMcPt_4hits_AllQ" );
	hEffPt->Draw( "hp" );
	hEffPt->GetXaxis()->SetRangeUser( 0, 5.0 );
	common_style( hEffPt, kBlue );
	master_style( hEffPt );
	hEffPt->SetTitle( "; p_{T}^{GEN}; Efficiency = (FOUND / GEN)" );
	
	common_style( hEffPtAllQ, kRed );
	if ( show_allq )
		hEffPtAllQ->Draw("same hp");

	common_style( hEffAccPtAllQ, kBlack );
	// hEffAccPtAllQ->Draw("same hp");
	draw_legend( hEffAccPtAllQ, hEffPt, hEffPtAllQ, show_allq );

	can->Print( "efficiency_pt.pdf" );
}

void vertex_plots( TString fin="results_si_refit_2mm_25cm_vertex2.root" ){
	// gStyle->SetOptStat(11111);
	gStyle->SetLineWidth(2);

	TCanvas *can = new TCanvas( "c", "", 1200, 900 );
	can->SetTopMargin( 0.1 );
	can->SetRightMargin( 0.1 );
	can->SetBottomMargin( 0.13 );
	can->SetLeftMargin( 0.13 );
	TFile *finput = new TFile( fin );

	// *********************************************** //
	// Position Z
	TH1 * hPosZ = (TH1*)finput->Get("Fit/VertexProjPosZ");
	common_style( hPosZ, kBlack );
	master_style( hPosZ );
	hPosZ->GetXaxis()->SetRangeUser( -100, 100 );
	hPosZ->Draw();
	gPad->SetLogy(1);

	can->Print( "vertex_pos_z.pdf" );

	TH1 * hSigmaZ = (TH1*)finput->Get("Fit/VertexProjSigmaZ");
	common_style( hSigmaZ, kBlack );
	master_style( hSigmaZ );
	hSigmaZ->GetXaxis()->SetRangeUser( -100, 100 );
	hSigmaZ->Draw();
	gPad->SetLogy(1);

	can->Print( "vertex_sigma_z.pdf" );

	gPad->SetLogy(0);

	TH1 * hPosXY = (TH1*)finput->Get("Fit/VertexProjPosXY");
	common_style( hPosXY, kBlack );
	master_style( hPosXY );
	hPosXY->GetXaxis()->SetRangeUser( -100, 100 );
	hPosXY->Draw("colz");
	// gPad->SetLogy(1);

	can->Print( "vertex_pos_xy.pdf" );

	TH1 * hSigmaXY = (TH1*)finput->Get("Fit/VertexProjSigmaXY");
	common_style( hSigmaXY, kBlack );
	master_style( hSigmaXY );
	hSigmaXY->GetXaxis()->SetRangeUser( -10, 10 );
	hSigmaXY->GetYaxis()->SetRangeUser( -10, 10 );
	hSigmaXY->Draw("colz");
	// gPad->SetLogy(1);

	can->Print( "vertex_sigma_xy.pdf" );
}


void fitting_plots( TString fin="results.root" ){

	gStyle->SetOptStat(0);
	gStyle->SetLineWidth(2);

	TCanvas *can = new TCanvas( "c", "", 1200, 900 );
	can->SetTopMargin( 0.1 );
	can->SetRightMargin( 0.1 );
	can->SetBottomMargin( 0.13 );
	can->SetLeftMargin( 0.13 );
	

	// efficiency_plots( fin );

	TFile *finput = new TFile( fin );

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
	
	TF1 * fpol1Q = new TF1( "fpol1Q", "pol1" );
	hWrongQ->Fit(fpol1Q, "R", "", 0, 5.0);
	TLatex tl;
	tl.DrawLatexNDC( 0.2, 0.7, TString::Format("%%WrongQ = %d%% + %d%% x pT", (int)(100*fpol1Q->GetParameter(0)), (int)(100*fpol1Q->GetParameter(1))) );

	can->Print( "qmis_pT.pdf" );

	// *********************************************** //
	// Inverse Momentum Resolution vs. p_T
	TH2* h2ires = (TH2*) finput->Get( "InvPtResVsTrue" );


	h2ires->RebinX(2);
	// h2ires->RebinY(4);
	TF1 * fgy = new TF1( "fgy", "gaus" );
	fgy->SetRange( -0.5, 0.5 );
	h2ires->FitSlicesY( fgy );
	TH1 * hSigma = (TH1*)gDirectory->Get( "InvPtResVsTrue_2" );

	hSigma-> SetBinContent( hSigma->FindBin( 0.1 ), 0 );
	hSigma-> SetBinError( hSigma->FindBin( 0.1 ), 0 );

	hSigma->GetXaxis()->SetRangeUser( 0, 5.0 );
	hSigma->GetYaxis()->SetRangeUser( 0, 1.1 );
	hSigma->Draw( "hpe" );
	hSigma->SetLineWidth(2);
	hSigma->SetLineColor(kBlue);
	hSigma->SetTitle( "; p_{T}^{GEN}; (1/p_{T}^{MC} - 1/p_{T}^{RC}) / (1/p_{T}^{MC})" );
	hSigma->GetXaxis()->SetTitleSize( 20.0 / 360 );
	hSigma->GetXaxis()->SetLabelSize( 16.0 / 360 );
	hSigma->GetYaxis()->SetTitleSize( 20.0 / 360 );
	hSigma->GetYaxis()->SetLabelSize( 16.0 / 360 );

	TF1 * fpol1 = new TF1( "fpol1", "pol1" );
	hSigma->Fit(fpol1, "R", "", 0, 5.0);
	// TLatex tl;
	tl.DrawLatexNDC( 0.2, 0.7, TString::Format("#sigma_{1/pT} / (1/p_{T}) = %d%% + %d%% x (1/pT)", (int)(100*fpol1->GetParameter(0)), (int)(100*fpol1->GetParameter(1))) );

	can->Print( "invptres.pdf" );


	// *********************************************** //
	// Momentum Resolution vs. p_T
	TH2* h2res = (TH2*) finput->Get( "PtResVsTrue" );


	h2res->RebinX(2);
	// h2res->RebinY(4);
	TF1 * fgyRes = new TF1( "fgyRes", "gaus" );
	fgyRes->SetRange( -0.5, 0.5 );
	h2res->FitSlicesY( fgyRes );
	TH1 * hSigmaRes = (TH1*)gDirectory->Get( "PtResVsTrue_2" );

	hSigmaRes-> SetBinContent( hSigmaRes->FindBin( 0.0 ), 0 );
	hSigmaRes->GetXaxis()->SetRangeUser( 0, 5.0 );
	hSigmaRes->GetYaxis()->SetRangeUser( 0, 1.1 );
	hSigmaRes->Draw( "hpe" );
	hSigmaRes->SetLineWidth(2);
	hSigmaRes->SetLineColor(kBlue);
	hSigmaRes->SetTitle( "; p_{T}^{GEN}; (p_{T}^{MC} - p_{T}^{RC}) / p_{T}^{MC}" );
	hSigmaRes->GetXaxis()->SetTitleSize( 20.0 / 360 );
	hSigmaRes->GetXaxis()->SetLabelSize( 16.0 / 360 );
	hSigmaRes->GetYaxis()->SetTitleSize( 20.0 / 360 );
	hSigmaRes->GetYaxis()->SetLabelSize( 16.0 / 360 );

	TF1 * fpol1Res = new TF1( "fpol1Res", "pol1" );
	hSigmaRes->Fit(fpol1Res, "R", "", 0, 5.0);
	
	tl.DrawLatexNDC( 0.2, 0.7, TString::Format("#sigma_{pT} / p_{T} = %d%% + %d%% x pT", (int)(100*fpol1Res->GetParameter(0)), (int)(100*fpol1Res->GetParameter(1))) );

	can->Print( "ptres.pdf" );

	can->SetBottomMargin( 0.15 );


	// *********************************************** //
	// Momentum Resolution 
	TH1* h1res = (TH1*) finput->Get( "PtRes" );
	h1res->RebinX(2);

	gStyle->SetOptFit(111);
	
	TF1 * f1gRes = new TF1( "f1gRes", "gaus" );

	h1res->Fit( f1gRes, "R", "", -0.5, 0.5 );

	
	// hSigmaRes->GetXaxis()->SetRangeUser( 0, 5.0 );
	// hSigmaRes->GetYaxis()->SetRangeUser( 0, 1.1 );
	h1res->Draw( "hpe" );
	h1res->SetLineWidth(2);
	h1res->SetLineColor(kBlue);
	h1res->SetTitle( "; (p_{T}^{MC} - p_{T}^{RC}) / p_{T}^{MC}" );
	h1res->GetXaxis()->SetTitleSize( 20.0 / 360 );
	h1res->GetXaxis()->SetLabelSize( 16.0 / 360 );
	h1res->GetYaxis()->SetTitleSize( 20.0 / 360 );
	h1res->GetYaxis()->SetLabelSize( 16.0 / 360 );

	can->Print( "ptres1d.pdf" );

	/***********************************************/ //
	// Inverse Momentum Resolution 
	h1res = (TH1*) finput->Get( "InvPtRes" );
	h1res->RebinX(8);
	
	// TF1 * f1gRes = new TF1( "f1gRes", "gaus" );

	h1res->Fit( f1gRes, "R", "", -0.5, 0.5 );

	
	// hSigmaRes->GetXaxis()->SetRangeUser( 0, 5.0 );
	// hSigmaRes->GetYaxis()->SetRangeUser( 0, 1.1 );
	h1res->Draw( "hpe" );
	h1res->SetLineWidth(2);
	h1res->SetLineColor(kBlue);
	h1res->SetTitle( "; ((1/p_{T}^{MC}) - (1/p_{T}^{RC})) / (1/p_{T}^{MC})" );
	h1res->GetXaxis()->SetTitleSize( 20.0 / 360 );
	h1res->GetXaxis()->SetLabelSize( 16.0 / 360 );
	h1res->GetYaxis()->SetTitleSize( 20.0 / 360 );
	h1res->GetYaxis()->SetLabelSize( 16.0 / 360 );

	can->Print( "invptres1d.pdf" );

	/***********************************************/ //
	// Inverse Momentum Resolution 
	TH2 * NQMat = (TH2*) finput->Get( "NQMatrix" );
	// NQMat->RebinX(2);
	
	// TF1 * f1gRes = new TF1( "f1gRes", "gaus" );

	// NQMat->Fit( f1gRes, "R", "", -0.5, 0.5 );

	
	// hSigmaRes->GetXaxis()->SetRangeUser( 0, 5.0 );
	// hSigmaRes->GetYaxis()->SetRangeUser( 0, 1.1 );
	gStyle->SetPaintTextFormat("0.3f");
	NQMat->Draw( "colz" );
	NQMat->SetMarkerSize(2);
	NQMat->Draw( "same text" );
	NQMat->SetLineWidth(2);
	NQMat->SetLineColor(kBlue);
	// NQMat->SetTitle( "; (p_{T}^{MC} - p_{T}^{RC}) / p_{T}^{MC}" );
	NQMat->GetXaxis()->SetTitleSize( 20.0 / 360 );
	NQMat->GetXaxis()->SetLabelSize( 16.0 / 360 );
	NQMat->GetYaxis()->SetTitleSize( 20.0 / 360 );
	NQMat->GetYaxis()->SetLabelSize( 16.0 / 360 );

	can->Print( "q_mat.pdf" );


}