/*
This macro shows how to compute jet energy scale.
root -l examples/Example4.C'("delphes_output.root", "plots.root")'
*/

#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#else
class ExRootTreeReader;
class ExRootResult;
#endif

class ExRootResult;
class ExRootTreeReader;

//------------------------------------------------------------------------------

void AnalyseEvents(ExRootTreeReader *treeReader, const char *outputFile_det, const char *outputFile_part, double topmass)
{
  TClonesArray *branchGenJet = treeReader->UseBranch("GenJet");
  TClonesArray *branchParticle = treeReader->UseBranch("Particle");
  TClonesArray *branchElectron = treeReader->UseBranch("Electron");
  TClonesArray *branchPhoton = treeReader->UseBranch("Photon");
  TClonesArray *branchMuon = treeReader->UseBranch("Muon");
  TClonesArray *branchEFlowTrack = treeReader->UseBranch("EFlowTrack");
  TClonesArray *branchEFlowPhoton = treeReader->UseBranch("EFlowPhoton");
  TClonesArray *branchEFlowNeutralHadron = treeReader->UseBranch("EFlowNeutralHadron");
  TClonesArray *branchJet = treeReader->UseBranch("Jet");
  TClonesArray *branchFatJet = treeReader->UseBranch("FatJet");
  TClonesArray *branchTruthFatJet = treeReader->UseBranch("TruthFatJet");
  TClonesArray *branchMissingET = treeReader->UseBranch("MissingET");

  MissingET *met;

  Long64_t allEntries = treeReader->GetEntries();
  ofstream myfile_det;
  ofstream myfile_part;

  cout << "** Chain contains " << allEntries << " events" << endl;

  Jet *jet, *genjet;
  GenParticle *muparticle;
  GenParticle *genparticle;
  GenParticle *motherparticle;
  Jet *particle;
  Jet *particle2;
  TObject *object;
  Muon *muon;

  Track *track;
  Tower *tower;

  TLorentzVector jetMomentum, genJetMomentum, bestGenJetMomentum;

  Float_t deltaR;
  Float_t pt, eta;
  Long64_t entry;

  Int_t i, j;

  myfile_det.open (outputFile_det);//example_detector.txt");
  myfile_part.open (outputFile_part);//example_particle.txt");

  // Loop over all events
  for(entry = 0; entry < allEntries; ++entry)
  {
    if (entry > 10000) break;
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);

    if(entry%500 == 0) cout << "Event number: "<< entry <<endl;

    //std::cout << entry << std::endl;
    for(i = 0; i < branchJet->GetEntriesFast(); ++i){
      jet = (Jet*) branchJet->At(i);
      jetMomentum = jet->P4();
      myfile_det << i << " " << jet->PT << " " << jet->Eta << " " << jet->Phi << " " << jetMomentum.M() << " " << jet->BTag << " " << topmass << " ";
      //std::cout << "  D " << i << " " << jetMomentum.Pt() << " " << jet->BTag << std::endl;
    }

    for(i = 0; i < branchMuon->GetEntriesFast(); ++i)
      {
	muon = (Muon*) branchMuon->At(i);
	//muparticle = (GenParticle*) muon->Particle.GetObject();
	myfile_det << i << " " << muon->PT << " " << muon->Eta << " " << muon->Phi << " " << 0.1 << " " << 13 << " " << topmass << " ";
	//std::cout << "muon ! " << muon->PT << std::endl;
      }

    if(branchMissingET->GetEntriesFast() > 0)
      {
	met = (MissingET*) branchMissingET->At(0);
	myfile_det <<  " 0 " << met->MET << " 0 " << met->Eta << " " << met->Phi << " 0 14 "<< " " << topmass << " "; 
      }
    

    myfile_det << std::endl;  

    // Loop over all hard partons in event                                                                                                                                                   
    for(j = 0; j < branchParticle->GetEntriesFast(); ++j)
      {
        genparticle = (GenParticle*) branchParticle->At(j);
        genJetMomentum = genparticle->P4();
        if(genJetMomentum.Px() == 0 && genJetMomentum.Py() == 0) continue;
	if (abs(genparticle->PID)!=13 && abs(genparticle->PID)!=14) continue;
	motherparticle = (GenParticle*) branchParticle->At(genparticle->M1);
	if (motherparticle->PID!=24) continue;
	//std::cout << "truth muon ! " << genparticle->PT << std::endl;
	if (abs(genparticle->PID)==13) myfile_part << i << " " << genparticle->PT << " " << genparticle->Eta << " " << genparticle->Phi << " " << 0.1 << " " << 13 << " " << topmass << " ";
	if (abs(genparticle->PID)==14) myfile_part << i << " " << genparticle->PT << " " << " 0. " << " " << genparticle->Phi << " " << 0. << " " << 14 << " " << topmass << " ";
      }

    for(i = 0; i < branchGenJet->GetEntriesFast(); ++i){
      jet = (Jet*) branchGenJet->At(i);
      jetMomentum = jet->P4();
      myfile_part << i << " " << jet->PT << " " << jet->Eta << " " << jet->Phi << " " << jetMomentum.M() << " " << jet->BTag << " " << topmass << " ";
      //std::cout << "  G " << i << " " << jetMomentum.Pt() << " " << jet->BTag << std::endl;
    }

    myfile_part << std::endl;  

    /*
    // Loop over all reconstructed jets in event
    for(i = 0; i < branchFatJet->GetEntriesFast(); ++i)
      {

	if (i > 0) continue; //let's just take the leading jet.
	
	jet = (Jet*) branchFatJet->At(i);
	jetMomentum = jet->P4();
	
	//std::cout << "squirrel " << jetMomentum.Pt() << " " << jet->Tau[1] << std::endl;
	//std::cout << i << " squirrel " << jetMomentum.Pt() << " " << std::endl;

	deltaR = 999;
	int whichjet = 0;
	
	// Loop over all hard partons in event
	for(j = 0; j < branchTruthFatJet->GetEntriesFast(); ++j)
	  {
	    particle = (Jet*) branchTruthFatJet->At(j);
	    
	    genJetMomentum = particle->P4();
	    
	    // take the closest parton candidate
	    if(genJetMomentum.DeltaR(jetMomentum) < deltaR)
	      {
		deltaR = genJetMomentum.DeltaR(jetMomentum);
		bestGenJetMomentum = genJetMomentum;
		particle2 = (Jet*) branchTruthFatJet->At(j);
		whichjet = j;
	      }
	  }
	
	if(deltaR < 0.3)
	  {
	    pt  = jetMomentum.Pt();
	    eta = TMath::Abs(jetMomentum.Eta());
	    //myfile << bestGenJetMomentum.M() << " " << jetMomentum.M() << std::endl;
	    
	    //For the part part.
	    myfile_part << whichjet << " " << particle2->PT << " " << particle2->Eta << " " << particle2->Phi << " " << jetMomentum.M() << " ";
	    myfile_part << particle2->Tau[0] << " " << particle2->Tau[1] << " " << particle2->Tau[2] << " " << particle2->Tau[3] << " " << particle2->Tau[4] << " ";
	    myfile_part << particle2->SoftDroppedP4.Pt() << " " << particle2->SoftDroppedP4.M() << " " << particle2->TrimmedP4.Pt() << " " << particle2->TrimmedP4.M() << " " << particle2->NSubJetsTrimmed << " ";
	    //std::cout << "    " << particle2->Constituents.GetEntriesFast() << " " << particle2->PT << " " << jet->PT << " " <<  std::endl;
	    for(int j2 = 0; j2 < particle2->Constituents.GetEntriesFast(); ++j2)
	      {
		object = particle2->Constituents.At(j2);
		if(object == 0){

                }
		if(object->IsA() == GenParticle::Class())
		  {
		    subparticle = (GenParticle*) object;
		    myfile_part << subparticle->PT << " " << subparticle->Eta << " " << subparticle->Phi << " " << subparticle->PID << " ";
		  }
	      }
	    myfile_part << std::endl;

	    //Now for the det part.
	    myfile_det << i << " " << jet->PT << " " << jet->Eta << " " << jet->Phi << " " << bestGenJetMomentum.M() << " ";
	    myfile_det << jet->Tau[0] << " " << jet->Tau[1] << " " << jet->Tau[2] << " " << jet->Tau[3] << " " << jet->Tau[4] << " ";
	    myfile_det << jet->SoftDroppedP4.Pt() << " " << jet->SoftDroppedP4.M() << " " << jet->TrimmedP4.Pt() << " " << jet->TrimmedP4.M() << " " << jet->NSubJetsTrimmed << " "; 
	    for(int j2 = 0; j2 < jet->Constituents.GetEntriesFast(); ++j2)
	      {
		object = jet->Constituents.At(j2);
		if(object == 0){
		  
		}
		else if(object->IsA() == Track::Class()){
		  track = (Track*) object;
		  if (abs(track->PID)==11){
		    myfile_det << track->PT << " " << track->Eta << " " << track->Phi << " 11 ";  
		  }
		  else if (abs(track->PID)==13){
		    myfile_det << track->PT << " " << track->Eta << " " << track->Phi << " 13 ";
		  }
		  else{
		    myfile_det << track->PT << " " << track->Eta << " " << track->Phi << " 211 ";
		  }
		}
		else if(object->IsA() == Tower::Class()){
		  tower = (Tower*) object;
		  if (tower->Eem/(tower->Eem+tower->Ehad) == 1){
		    myfile_det << tower->ET << " " << tower->Eta << " " << tower->Phi << " 22 ";
		  }
		  else{
		    myfile_det << tower->ET << " " << tower->Eta << " " << tower->Phi << " 2112 ";
		  }
		}
	      }
	    myfile_det << std::endl;
	  }
      }
    */
  }
}
//------------------------------------------------------------------------------

void topmass(const char *inputFile, const char *outputFile_det, const char *outputFile_part, double topmass)
{
  gSystem->Load("libDelphes");

  TChain *chain = new TChain("Delphes");
  chain->Add(inputFile);

  ExRootTreeReader *treeReader = new ExRootTreeReader(chain);
  ExRootResult *result = new ExRootResult();

  AnalyseEvents(treeReader,outputFile_det,outputFile_part, topmass);

  cout << "** Exiting..." << endl;

  delete result;
  delete treeReader;
  delete chain;
}

//------------------------------------------------------------------------------
