# star-fwd-dev

Development environment for the STAR Forward tracking package (inside StRoot)

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

## Running the forward tracking code
Launch a (docker) development environment with:
```sh
./dev.sh
```
This will drop you into a docker container with the current path of `/tmp/work/` where work is you local `work` 
NOTE: if you are far from BNL or just want to make running faster you can use:
```sh
./dev.sh --network none
```
This will disable the network inside the container, making the STAR DB access return instantly. It may speed up the running of the code a lot if you have a poor connection to BNL. 

If there exists a star-cvs folder (not part of repo, clone it yourself) then this will mount the geometry into place for build.


directory mounted into the container. Before running the code, setup the STAR environment variables:
```sh
source star_env
```
  

Now you can run the code with:
```sh
root4star -b -q -l simple.C
```
  
This will produce an output called `results.root` with histograms etc.

## Modifying and building
The code lives in `star-sw/StRoot/StgMaker/` so you can edit the code here.  
After making changes you can rebuild the code with:
```sh
./build
```
If compilation succeeds then the code is updated.

## Modifying the Geometry
NOTE: As of 05/05/2020 - we know that the high detail Si Geometry causes GenFit to fail/run very very slowly. This can be worked around by using simpler / removing Fst geometry from GenFit tracking step. To do this rebuild the geometry before building code (above steps). 

```sh
./geom-build.sh
```
This takes `~10` minutes and rebuilds the Geometry from scratch to change the Fst geometry. This only needs to be done once when you start the container.
If the z-location of the disks are changed then the config needs to be updated as well. (will be fixed soon to read from geometry).

  
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
  
**.Vertex:sigmaXY** - specifies the XY resolution of the primary vertex  
**.Vertex:sigmaZ** - resolution of the Z coordinate of primary vertex  
**.Vertex:includeInFit** - include / exclude PV in the track fitting  
  
**.Hits:sigmaXY** - the xy resolution for the sTGC hits (used when calculating cov mat), only used if **.Hits:useFCM** = `true`  




## Generating simulated hits
The above code expects simulated hits available in the file `testg.fzd`. 
If you need to generate simulation input, you can use the project here: https://github.com/jdbrice/star-fwd-sim  




