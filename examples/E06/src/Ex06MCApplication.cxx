// $Id: $
//
// Geant4 ExampleN06 adapted to Virtual Monte Carlo 
//
// Class Ex06MCApplication
// Implementation of the TVirtualMCApplication
//
// by Ivana Hrivnacova, 16.5.2005

#include <TROOT.h>
#include <TInterpreter.h>
#include <TVirtualMC.h>
#include <Riostream.h>
#include <TGeoManager.h>
#include <TVirtualGeoTrack.h>
#include <TCanvas.h>

#include "Ex06MCApplication.h"
#include "Ex03MCStack.h"
#include "Ex06DetectorConstruction.h"
#include "Ex06PrimaryGenerator.h"

ClassImp(Ex06MCApplication)

//_____________________________________________________________________________
Ex06MCApplication::Ex06MCApplication(const char *name, const char *title) 
  : TVirtualMCApplication(name,title),
    fEventNo(0),
    fGammaCounter(0),
    fVerbose(0),
    fStack(0),
    fDetConstruction(0),
    fPrimaryGenerator(0),
    fCanvas(0)
{
// Standard constructor
// ---

  // Create a user stack
  fStack = new Ex03MCStack(1000);
  
  // Create detector construction
  fDetConstruction = new Ex06DetectorConstruction();
  
  // Create a primary generator
  fPrimaryGenerator = new Ex06PrimaryGenerator(fStack);
}

//_____________________________________________________________________________
Ex06MCApplication::Ex06MCApplication()
  : TVirtualMCApplication(),
    fEventNo(0),
    fGammaCounter(0),
    fVerbose(0),
    fStack(0),
    fDetConstruction(0),
    fPrimaryGenerator(0),
    fCanvas(0)
{    
// Default constructor
// ---
}

//_____________________________________________________________________________
Ex06MCApplication::~Ex06MCApplication() 
{
// Destructor  
// ---
  
  delete fStack;
  delete fDetConstruction;
  delete fPrimaryGenerator;
  delete gMC;
  gMC = 0;
}

//
// public methods
//

//_____________________________________________________________________________
void Ex06MCApplication::InitMC(const char* setup)
{    
// Initialize MC.
// ---

  fVerbose.InitMC();

  gROOT->LoadMacro(setup);
  gInterpreter->ProcessLine("Config()");
 
  gMC->SetStack(fStack);
  gMC->Init();
  gMC->BuildPhysics(); 
}

//_____________________________________________________________________________
void Ex06MCApplication::RunMC(Int_t nofEvents)
{    
// MC run.
// ---

  fVerbose.RunMC(nofEvents);

  gMC->ProcessRun(nofEvents);

  fVerbose.FinishRun();
}

//_____________________________________________________________________________
void Ex06MCApplication::ConstructGeometry()
{    
// Construct geometry using detector contruction class
// ---

  fVerbose.ConstructGeometry();

  fDetConstruction->ConstructGeometry();  
}

//_____________________________________________________________________________
void Ex06MCApplication::ConstructOpGeometry()
{    
// Construct geometry using detector contruction class
// ---

  fVerbose.ConstructGeometry();

  fDetConstruction->ConstructOpGeometry();  
}

//_____________________________________________________________________________
void Ex06MCApplication::InitGeometry()
{    
// Initialize geometry
// ---
  
  fVerbose.InitGeometry();
}

//_____________________________________________________________________________
void Ex06MCApplication::GeneratePrimaries()
{    
// Fill the user stack (derived from TVirtualMCStack) with primary particles.
// ---
  
  fVerbose.GeneratePrimaries();

  fPrimaryGenerator->GeneratePrimaries();
}

//_____________________________________________________________________________
void Ex06MCApplication::BeginEvent()
{    
// User actions at beginning of event
// ---

  fVerbose.BeginEvent();

  fEventNo++;
  fGammaCounter = 0;
}

//_____________________________________________________________________________
void Ex06MCApplication::BeginPrimary()
{    
// User actions at beginning of a primary track
// ---

  fVerbose.BeginPrimary();
}

//_____________________________________________________________________________
void Ex06MCApplication::PreTrack()
{    
// User actions at beginning of each track
// ---

  fVerbose.PreTrack();
  
  if (gMC->TrackPid() == 50000050 ) fGammaCounter++;
}

//_____________________________________________________________________________
void Ex06MCApplication::Stepping()
{    
// User actions at each step
// ---

  fVerbose.Stepping();
}

//_____________________________________________________________________________
void Ex06MCApplication::PostTrack()
{    
// User actions after finishing of each track
// ---

  fVerbose.PostTrack();
}

//_____________________________________________________________________________
void Ex06MCApplication::FinishPrimary()
{    
// User actions after finishing of a primary track
// ---

  fVerbose.FinishPrimary();
}

//_____________________________________________________________________________
void Ex06MCApplication::FinishEvent()
{    
// User actions after finishing of an event
// ---

  fVerbose.FinishEvent();
  
  // Draw event with TGeo
  // available only with TGeant3TGeo

  if ( TString(gMC->GetName()) == "TGeant3TGeo" && 
       gGeoManager->GetListOfTracks() &&
       gGeoManager->GetTrack(0) &&
       ((TVirtualGeoTrack*)gGeoManager->GetTrack(0))->HasPoints() ) {
       
       gGeoManager->SetVisOption(0);	 
       gGeoManager->SetTopVisible();
       gGeoManager->DrawTracks("/*");  // this means all tracks
          // Drawing G3 tracks via TGeo is available only
	  // if geant3 is compile with -DCOLLECT_TRACK flag
	  // (to be activated in geant3/TGeant3/TGeant3gu.cxx)
  }    

  cout << "Number of optical photons produced in this event : "
       << fGammaCounter << endl;

  fStack->Reset();
} 

//_____________________________________________________________________________
void Ex06MCApplication::Field(const Double_t* x, Double_t* b) const
{
// Uniform magnetic field
// ---
  
   for (Int_t i=0; i<3; i++) b[i] = 0.0;
}
 