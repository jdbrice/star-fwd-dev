# star-fwd-dev

Development environment for the official STAR Forward tracking package (as part of StRoot)

## Setup
Clone the repo:
```sh
git clone https://github.com/jdbrice/star-fwd-dev.git
cd star-fwd-dev
```

Now, make sure the submodules are initialized:
```sh
git submodule update --init --recursive
```

## Modifying and building
The tracking code lives in `star-sw/StRoot/StFwdTrackMaker/` so you can edit the code here.
The Fst fast simulator is in `star-sw/StRoot/StFstFastSimulatorMaker/`.
After making changes you can rebuild the code with:
```sh
./build 1 # full build (needed first time for StFtsFastSimulatorMaker)
# OR
./build # After first time, faster for updating code
```

NOTE a few things:
1) You should always re-build the code after starting a new docker container to be sure it is up-to-date
2) The first 
If compilation succeeds then the code is updated.

## Running the Docker container
Launch a (docker) development environment with:
```sh
./dev.sh
```
This will drop you into a docker container with the current path of `/tmp/work/` where work is you local `work` 

NOTE: if you are far from BNL or just want to make running faster and don't need STAR DB you can use:
```sh
./dev.sh --network none
```
This will disable the network inside the container, making the STAR DB access return instantly. It may speed up the running of the code a lot if you have a poor connection to BNL. 


## Setup the STAR environment and ROOT 
Before running the code, setup the STAR environment variables:
```sh
source star_env
```
  

## Running the Forward Tracking code with the FST Fast Simulator
Once inside the docker container session:
```sh
./fst_study.C
```
This will run the fast sim and StFwdTrackMaker on the given `.fzd` and `config.xml` file. You can gain additional control over the parameters by running it as a root macro:
```sh
root4star -b -q -l 'fst_study.C( ... )'
```


## Running the Forward Tracking code from GEANT hits
Now you can run the code with:
```sh
root4star -b -q -l simple.C
```
OR you can use the script as:
```sh
./run.sh <n_events> <fzd_file> <xml_config>
```
  
This will produce an output called `results.root` with histograms etc.

## Running the Fts Fast Simulator Maker (Standalone)
For the default parameters simply use the script: 
```sh
./fts_fast_sim.C
```
you can also specify the number of events and `FZD` file:
```sh
root4star -b -q -l 'fts_fast_sim( n_events, "fzd_file.fzd" )'
```
This produces an output file with the name `<input_fzd>_output.root` with the QA histograms.


## Modifying the Geometry
NOTE: As of 05/05/2020 - we know that the high detail Si Geometry causes GenFit to fail/run very very slowly. This can be worked around by using simpler / removing Fst geometry from GenFit tracking step. To do this rebuild the geometry before building code (above steps). 

```sh
./geom-build.sh
```
This takes `~10` minutes and rebuilds the Geometry from scratch to change the Fst geometry. This only needs to be done once when you start the container.
If the z-location of the disks are changed in the simulation stage then the config needs to be updated as well to reflect the z location of Si disks since current work-around is to remove Si geometry for track fitting.  Stgc z locations are picked up from the geometry, so you do not need to set these manually.

  
## Forward Tracking Configuration
The forward tracking code is driven by a configuration file `work/config.xml` :
You can modify this file to change the configuration of the ForwardTrack maker.


Example bare minimum configuration for Track Fitting using perfect (MC) track finding:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<config>
    <Output url="results_fitting_mcfinding.root" />
    <!-- WITHOUT a <TrackFinder> node, use MC (perfect) TrackFinding -->
    
    <!-- Track Fitter config -->
    <!-- Dont refit with Si hits, use Mc Seed - i.e. momentum seed from MC truth -->
    <TrackFitter refitSi="false" mcSeed="true"> 
        <Vertex sigmaXY="0.05" sigmaZ="0.05" includeInFit="true" /> <!-- Set the PV resolution and niclude it in the track fit -->

        <!-- for MC sTGC hits only only -->
        <Hits sigmaXY="0.01" useFCM="true" />
    </TrackFitter>
</config>
```


Example configuration and some information about the various parts below:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<config>

    <!-- The below line is still needed, do not change for now -->
    <Input url="SimHitsTuple_1track_0p2_5.root" type="sim_mc" max-events="200" first-event="0" si="false" />

    <!-- This is the output file for the QA ROOT file produced by the tracking code -->
    <Output url="results.root" />

    <!-- CONFIGURATION for Track finding -->
    <TrackFinder nIterations="1">
        <Iteration> <!-- Options for first iteration -->
            <SegmentBuilder>
                <Criteria name="Crit2_RZRatio" min="0.9" max="1.11" />
                <Criteria name="Crit2_DeltaPhi" min="0" max="10" /> 
                <Criteria name="Crit2_DeltaRho" min="-5" max="20"/>
                <Criteria name="Crit2_StraightTrackRatio" min="0.9" max="1.1"/>
            </SegmentBuilder>

            <ThreeHitSegments>
                <Criteria name="Crit3_3DAngle" min="0" max="90" />
                <Criteria name="Crit3_PT" min="0" max="100" />
                <Criteria name="Crit3_ChangeRZRatio" min="0" max="1.01" />
                <Criteria name="Crit3_2DAngle" min="0" max="2" />
            </ThreeHitSegments>
        </Iteration>

        <!-- These are used if not defined inside <Iteration> -->
        <ThreeHitSegments>
            <Criteria name="Crit3_2DAngle" min="0" max="50" />
        </ThreeHitSegments>

        <Connector distance="1"/>

        <SubsetNN active="true" min-hits-on-track="4" >
            <Omega>0.99</Omega>
            <StableThreshold>0.001</StableThreshold>
        </SubsetNN> 

        <HitRemover active="true">
        </HitRemover>

    </TrackFinder>

    <!-- CONFIGURATION for Track Fitting -->
    <TrackFitter constB="true" display="false" noMaterialEffects="true" >
        <Geometry stgc="" si="144.48788, 167.98288, 191.47788" /> <!-- Override z location of either detector. Must provide all locations for a give system (4 for STGC, 3 for Si) -->
        <Vertex sigmaXY="0.01" sigmaZ="5" includeInFit="true" />
        
        <!-- for MC only -->
        <Hits sigmaXY="0.01" useFCM="true" />
    </TrackFitter>


</config>

```

In the following I use the syntax `node.node:attribute`, e.g. the `sigmaXY` attribute of the `Vertex` Node inside `TrackFitter` would be `TrackFitter.Vertex:sigmaXY`

**Input** - not needed for version running in StRoot - it reads data from GEANT tables. In future we will read from hit collections

**Output:url** - specifies the output file for QA histograms written by tracking code. Once the code is fully integrated we will/should do QA on the tracks in StEvent/StMuDst.


### Track Finding
Purpose: control the track finding parameters used by the CA procedure. All of these are implicitly prefixed by `TrackFinder` (i.e. inside the `TrackFinder` Node):  
  
**:nIterations** - determines the number of iterations to run. An iteration is puctuated by track fitting and then removing hits that belong to accepted tracks.  
  
**Iteration[1..N]** - Specify parameters for a given iteration, if they are not specified here, then the default will be used as defined in `TrackFinder`  
**Iteration.SegmentBuilder** - parameters for the 2-hit segment builder. This is the first step in the CA track building process  
**Iteration.ThreeHitSegments** - parameters for the 3-hit segment builder  
  
**Connector:distance** - determines whether or not tracks can skip layers. For now we require all 4 sTGC hits, so no skipping is allowed. Should not be changed for normal studies.  
  
**SubsetNN** - controls the Hopfield Neural Network used to select the best subset of tracks with highest quality that do not share hits.  
**SubsetNN:active** - turn it on and off  
**SubsetNN:min-hits-on-track** - specify the minimum number of hits required for a track seed.  
**SubsetNN.Omega** - parameter controls weight of quality  
**SubsetNN:StableThreshold** - controls how much noise is considered stable, changing may help speed up convergence  
  
**HitRemover:active** - controls whether or not hits are removed after each iteration  
  
### Track Fitting
Track fitting is controlled by parameters in `TrackFitter` Node:  
**:constB** - use const B-field vs. real STAR B-field  
**:display** - can be used to display events in GenFit Event Display, not going to work well on RCF/docker so may be removed in future  
**:noMaterialEffects** - turn on/off material effects in Kalman Filter.  
  
**.Geometry:si** - optional list of z locations for Si detector planes (must be length 3 or not used)   
**.Geometry:stgc** - optional list of z locations for stgc detector planes (must be length 4 or not used)   

**.Vertex:sigmaXY** - specifies the XY resolution of the primary vertex  
**.Vertex:sigmaZ** - resolution of the Z coordinate of primary vertex  
**.Vertex:includeInFit** - include / exclude PV in the track fitting  
  
**.Hits:sigmaXY** - the xy resolution for the sTGC hits (used when calculating cov mat), only used if **.Hits:useFCM** = `true`  




## Generating simulated hits
The above code expects simulated hits available in the file `testg.fzd`. 
If you need to generate simulation input, you can use the project here: https://github.com/jdbrice/star-fwd-sim  


## Output histograms and data files
The tracking code does basic MC association and QA plotting to provide immediate feedback. Here is a (non-exhaustive) list of the most important histograms and what they mean:

- "FitDuration" : Time taken for the track fitting step only
- "FitStatus" : Shows the number of track seeds found, the number of successful/failed sTGC track fits, the number of possible/successful/failed refits with Si
- "AllQuality" : The "quality" of all track seeds. Quality = # correct hits / #mc hits. Example, if a track has 4 hits with 3 from one MC track and one from another, then the quality is 3/4. If a track has 4 hits with 2 from one MC track and 2 from another then the quality is 2/4. If the track has 4 hits where each is from a different MC track then the quality is 1/4.
- "DurationPerEvent" : The total time taken for the event including seed finding and track fitting
- "EffVsMcEta" : (same for Pt, Eta, Phi) This is Acceptance x Efficiency for tracks with perfect quality (4/4).
- "EffVsMcEtaAllQ" : (same for Pt, Eta, Phi) This is Acceptance x Efficiency for tracks with any quality.
- "EffVsMcEta_4hits" : (same for Pt, Eta, Phi) This is Efficiency only (Acceptance factored out) for tracks with perfect quality
- "EffVsMcEta_4hitsAllQ" : (same for Pt, Eta, Phi) This is Efficiency only (Acceptance factored out) for tracks with any quality
- "EffVsMcPtEtaPhi..." : same as above but in 3D
- "QMatrix" : 2D histogram of the reco charge vs. gen (MC level) charge.
- "NQMatrix" : 2D histogram of the reco charge vs. gen (MC level) charge. Normalized to 1.0
- "PtRes" : 1D histogram of pT resolution averaged over all (pT, eta, phi).
- "PtResVsTrue" : 2D histogram of pT resolution vs. Mc pT averaged over ( eta, phi).
- "InvPtRes" : 1D histogram of inverse pT resolution averaged over all (pT, eta, phi).
- "InvPtResVsTrue" : 2D histogram of inverse pT resolution vs. Mc pT averaged over ( eta, phi).
- "WrongQVsMcPt" : The number of tracks with a wrong charge vs. Mc Pt

## Running on RCF

You can run the tracking code on rcf now (05/11/2020).
1. checkout this code : 
```sh
git clone https://github.com/jdbrice/star-fwd-dev.git
cd star-fwd-dev/work
```
2. `starver dev`
3. `bash`
4. `source rcf_env.sh`
5. `./run`




