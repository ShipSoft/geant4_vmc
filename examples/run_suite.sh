#!/bin/sh 
#------------------------------------------------
# The Virtual Monte Carlo examples
# Copyright (C) 2007 - 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: root-vmc@cern.ch
#-------------------------------------------------

#
# Test all VMC examples and regenerate output files
#
# by I. Hrivnacova, IPN Orsay

CURDIR=`pwd`

for EXAMPLE in E01 E02 E03 E06 A01 ExGarfield Gflash TR
do
  cd $CURDIR/$EXAMPLE

  # run G3 - cannot be run with geometry defined via root 
  # echo "... Running example $EXAMPLE with G3" 
  # root.exe -q "run_g3.C" >& run_g3.out
  
  # run G3 + TGeo navigation
  echo "... Running example $EXAMPLE with G3 + TGeo" 
  root.exe -q -b load_g3.C run_g3.C\(\"g3tgeoConfig.C\"\) >& run_g3tgeo.out
  
  # run G4
  echo "... Running example $EXAMPLE with G4" 
  root.exe -q -b load_g4.C run_g4.C  >& run_g4.out

  # run G4 + TGeo navigation
  echo "... Running example $EXAMPLE with G4 + TGeo navigation" 
  root.exe -q -b load_g4.C run_g4.C\(\"g4tgeoConfig.C\"\)  >& run_g4tgeo.out

  # configuration available only in E03, A01 example
  if [ "$EXAMPLE" = "E03" -o "$EXAMPLE" = "A01" ]; then 

    # run G4 + geometry via G4
    echo "... Running example $EXAMPLE with G4; geometry via G4" 
    root.exe -q -b load_g4.C run_g4.C\(\"g4Config1.C\"\)  >& run_g4pl.out
  fi
  
  # configuration available only in E03 example
  if [ "$EXAMPLE" = "E03" ]; then 
    # run G4 + User physics list
    echo "... Running example $EXAMPLE with G4; user PL" 
    root.exe -q -b load_g4.C run_g4.C\(\"g4Config2.C\"\)  >& run_g4pl.out
  fi

done
        
cd $CURDIR
