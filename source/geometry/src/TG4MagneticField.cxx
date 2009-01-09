// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4MagneticField.cxx
/// \brief Implementation of the TG4MagneticField class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4MagneticField.h"
#include "TG4G3Units.h"
#include "TG4Globals.h"

#include <TVirtualMCApplication.h>
#include <TVirtualMC.h>

#include <G4FieldManager.hh>
#include <G4TransportationManager.hh>

//_____________________________________________________________________________
TG4MagneticField::TG4MagneticField()
  : G4MagneticField() 
{
/// Default constructor

  G4FieldManager* fieldManager
     = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  fieldManager->SetDetectorField(this);
  fieldManager->CreateChordFinder(this);
}

//_____________________________________________________________________________
TG4MagneticField::~TG4MagneticField() 
{
/// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
void TG4MagneticField::GetFieldValue(const G4double point[3], G4double* bfield) const
{
/// Return the bfield values in the given point.

  // Set units
  const G4double g3point[3] = { point[0] / TG4G3Units::Length(),
                                point[1] / TG4G3Units::Length(),
                                point[2] / TG4G3Units::Length() };

  if ( gMC->GetMagField() ) {
    gMC->GetMagField()->Field(g3point, bfield);
  }
  else {  
    static Bool_t warn = true;
    if (warn) { 
      TG4Globals::Warning(
        "TG4MagneticField", "GetFieldValue", 
        TString("Using deprecated function TVirtualMCApplication::Field().")
        + TG4Globals::Endl()
        + TString("New TVirtualMagField interface should be used instead."));
      warn = false;
    }        

    TVirtualMCApplication::Instance()->Field(g3point, bfield);
  }  
  
  // Set units
  for (G4int i=0; i<3; i++) bfield[i] = bfield[i] * TG4G3Units::Field();
}

