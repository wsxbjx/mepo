
$ontext
-----------------------------------------------------
 8760 hour system data for ERCOT2009 based capacity planning for use with StaticCapPlan family
 of models

 Sample financial & carbon data

 originally coded in GAMS by Bryan Palmintier 2010-04-08

 Ver   Date       Time  Who            What
 ---  ----------  ----- -------------- ---------------------------------
   1  2011-07-21  21:00  bpalmintier   Adapted from sys8760.inc ver 5
   2  2011-07-24  08:30  bpalmintier   Replace availability CSV with GAMS table format
   3  2011-07-26  04:30  bpalmintier   Reduced WACC to 8%
   4  2011-08-03  23:30  bpalmintier   Renamed demand8760 to ercot2009_demand
   5  2012-01-26  11:37  bpalmintier   Updated scenario dependent parameters with (S)
   6  2012-09-02  17:08  bpalmintier   Replace all $set with $setglobal (to prevent scope errors)
 -----------------------------------------------------
$offtext

*===== Scenario Dependent Parameters =====
*cost of carbon (in terms of CO2)        [$/t-CO2eq]   
 pCostCO2(S) = 0;         

*Carbon Cap in metric tons CO2(eq)
$if declared pCarbonCap
   pCarbonCap(S) = Inf;

*Scaling parameter for demand
$if declared pDemandScale
   pDemandScale(S)=1;

*renewable portfolio standard (% energy) Note: value overridden by --rps=#.# command-line option
$if declared pRPS
 pRPS(S) = 0.05;
   
*===== System Assumptions =====
*weighted average cost of capital (utility investment discount rate) [fract]
 pWACC = 0.08;       
   
*cost of non-served energy		[$/MWh]
pPriceNonServed = 1e5;

*===== Reserves =====
*Planning reserve (unused capacity) during peak period (includes op reserve)
*ERCOT uses a 12.5% planning reserve
$if declared pPlanReserve
   pPlanReserve = 0.125;

* Response time for Spinning Reserves [minutes]
$if declared pSpinResponseTime      
	pSpinResponseTime = 10.0;

*Off-line Operating reserve up (non-spin) as a fraction of load
$if declared pQuickStartLoadFract
   pQuickStartLoadFract = 0.03;

*On-line Spinning reserve as a fraction of load
$if declared pSpinReserveLoadFract
   pSpinReserveLoadFract = 0.03;

*On-line spinning reserve minimum (typically set to max plant size)
$if declared pSpinReserveMinMW
   pSpinReserveMinMW = 1350;

*Frequency regulation up as a fraction of load
$if declared pRegUpLoadFract
   pRegUpLoadFract = 0.01;

*Frequency regulation down as a fraction of load
$if declared pRegDownLoadFract
   pRegDownLoadFract = 0.01;
   
*Wind Forecast Error
$if declared pWindForecastError
   pWindForecastError=0.2;
   
*Additional reserves for Wind
* Data from (De Jonghe, et al 2011) Table III
$if declared pWindFlexUpForecast
	pWindFlexUpForecast = 0.125;
	
$if declared pWindFlexUpCapacity
	pWindFlexUpCapacity = 0.065;
	
$if declared pWindFlexDownForecast
	pWindFlexDownForecast = 0.0875;
	
$if declared pWindFlexDownCapacity
	pWindFlexDownCapacity = 0.0625;

*------- Reference data sub-include files
* -- Setup default (empty) path to csv include file. Typically this will be overridden within the model
$if NOT set data_dir    $setglobal data_dir   

$ifthen not set fuel
$include %data_dir%fuel.inc
$endif

$if not set demand $setglobal demand ercot2009_demand.inc
$include %data_dir%%demand%

$if not set gens $setglobal gens ercot2009_gens.inc
$include %data_dir%%gens%

* Read availability tables if needed
$if NOT set avail $setglobal avail ercot2009_avail.inc
$include %data_dir%ercot2009_avail.inc
