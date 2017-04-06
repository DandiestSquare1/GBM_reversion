inputs: 
	bool debug(True)
	,int LeftStrength(8)   //Sets the required number of bars on the left side of the pivot bar.
	,int RightStrength(4) //Sets the required number of bars on the right side of the pivot bar.
	,int leftPad(0)
	,int rightPad(0)
	,SwingHiObject(High)
	,SwingLoObject(Low)
	,bool ShowSwings(False)
	,bool ShowBarNum(False)
	,bool ShowObjVal(False)
	,bool ShowObjDiff(False)
	,bool ShowBarNumbers(False)
	,bool ReadWriteToDB(True)
	,bool UseBucketData(False)
	,int loadGroupId(1004);

vars: 
	 int seriesBarNumber(0)
	,string dataSeriesSymbol("") //symbolName
	,int lengthOfSeries(0)
	,int chartType(0) //BarType where 0 = TickBar, 1 = Minute, 2 = Daily, 3 = Weekly, 4 = Monthly, 5 = Point & Figure, 6 = (reserved), 7 = (reserved), 8 = Kagi, 9 = Kase, 10 = Line Break, 11 = Momentum, 12 = Range, 13 = Renko, 14 = Second
	,chartInterval(0) 
	,string dateBarOpened("")
	,string dayOfWeekBarOpened("")
	,string dateTimeBarOpened("") 
	,string timeBarOpenedhhmmss("")
	,minutesBarWasOpen(0)
	,double barOpenPrice(0)
	,double barHighPrice(0)
	,double barLowPrice(0)
	,double barClosePrice(0)
	,double minUnitPriceChange(0) //PriceScale = BigPointValue/PointValue 
 	,int settingMaxBarsBack(0)

	,string strtimeBarOpened("")
	,string strHHBarOpened("")
	,string strMMBarOpened("")
	,string strSSBarOpened("")
	,string strDateBarOpened("")
	,string strDateYYBarOpened("")
	,string strDateMMBarOpened("")
	,string strDateDDBarOpened("")
	,string strDateDowBarOpened("")
	,julianDateTime(0)
	,numMinutesFromMidnit(0)
	,numminutesBarWasOpn(0);


//Pos. 1 barNumber
//Returns the number of the current bar. 
//Each bar, subsequent to the initial number of bars specified by the Maximum Bars Back setting, is assigned a sequential number; the initial bars specified by the setting are not numbered. 
//For example, if Maximum Bars Back is set to 20, the 21st bar will be assigned a number of 1. 
seriesBarNumber = CurrentBar + settingMaxBarsBack; //or CurrentBar + MaxBarsBack;

//Pos. 2 symbol
//Returns a string expression containing the name of the symbol that the study is applied to
dataSeriesSymbol = SymbolName ;
 
//Pos. 3 lengthOfSeries
//Returns a numerical value indicating the actual number of bars of a data series on chart
//If the series is not updating in real-time (no new bars appear), the keyword returns the same value on each calculation. If the chart updates in real-time the keyword returns the number of bars available by the moment script references the keyword. It means that the keyword possibly can return different values even during calculation within the same bar. 
lengthOfSeries = seriesBarNumber ;//Symbol_Length ; 
 
//Pos. 4 chartType
//Returns a numerical value, indicating the resolution units of the data series that the study is applied to. 
chartType = BarType ;
{***********************************************************************************************************
	values returned for each type of resolution units: 
	  0 = Ticks (Ticks & Contracts)			8 = Kagi
	  1 = Intra-Day (Seconds, Minutes, & Hours)	9 = (Reserved for future use)
	  2 = Days						10 = Line Break
	  3 = Weeks						11 = (Reserved for future use)
	  4 = Months, Quarters, & Years			12 = (Reserved for future use)
	  5 = Points, Changes, Point & Figure		13 = Renko
	  6 = (Reserved for future use)			256 = Heikin Ashi
	  7 = (Reserved for future use)	  
***********************************************************************************************************}

//Pos. 5 chartInterval
//Returns a numerical value, indicating the number of resolution units (bar interval) of the data series that the study is applied to. 
//Returns the number of Ticks, Contracts, Points, Changes, Seconds, Minutes, Hours, Days, Weeks, Months, Quarters, or Years, depending on the chart resolution; a value of 5 will be returned for a 5-second as well as for a 5-tick chart. 
chartInterval = High[0] - Low[0] ; //If range bars in TradeStation use "BoxSize". If range bars in MultiCharts use "BarType"  

//Pos. 6 dateBarOpened 
strDateBarOpened = (FormatDate( "yyyy-MM-dd", ElDateToDateTime( Date[0] ) ));
dateBarOpened = strDateBarOpened ;

//Pos. 7 dayOfWeekBarOpened
dayOfWeekBarOpened = (FormatDate( "ddd", ElDateToDateTime(Date[0])));

//Pos. 8 timeBarOpened
//TimeToMinutes function calculates the number of minutes from midnight for any 24-hour (HHMM) time value
julianDateTime = DateToJulian(Date[0])+(TimeToMinutes(Time[0]) / 60 / 24 ) ; 
//timeBarOpened =(FormatTime("HH:mm:ss tt",julianDateTime));
//dateTimeBarOpened =DateTimeToString_Ms(DateTime[RightStrength]);
dateTimeBarOpened = DateTimeToString(julianDateTime );
  //DateTimeToString_Ms(DateTime[0])  will return a string value corresponding to the time and time of the current bar "5/28/2013 08:41:11.871". 

//Pos. 11 barOpenPrice
barOpenPrice = Open[0];

//Pos. 12 barHighPrice
barHighPrice = High[0];

//Pos. 13 barLowPrice
barLowPrice = Low[0];

//Pos. 14 barClosePrice
barClosePrice = Close[0];

	
vars: 	int intervalValue(4),
		int intervalValueLarge(20),
		int intervalLimit(1000),
		int lookBackLength(350),
		int scaleLimit(1000),
		int scaleFloor(0),
		int i_Len(0),
		double indLengthA(0),
		double indLengthB(0),
		double indLengthC(0),
		double indLengthD(0),
		double indLengthE(0),
		double i_Multiple(0);
i_Len = 4;
i_Multiple = 2;
indLengthA = ROUND(i_Len, 0) + MOD(ROUND(i_Len, 0), 2);
indLengthB = ROUND(indLengthA*0.75 + indLengthA*0.5, 0)+MOD(ROUND(indLengthA*0.75 + indLengthA*0.5, 0), 2);
indLengthC = ROUND(indLengthA*2.25 + indLengthA*0.5, 0)+MOD(ROUND(indLengthA*2.25 + indLengthA*0.5, 0), 2);
indLengthD = ROUND(indLengthA*4.75 + indLengthA*0.5, 0)+MOD(ROUND(indLengthA*4.75 + indLengthA*0.5, 0), 2);
indLengthE = ROUND(indLengthA*7.75 + indLengthA*0.5, 0)+MOD(ROUND(indLengthA*7.75 + indLengthA*0.5, 0), 2);



	//*****************************************************
	//************ AC oscillator ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double AC_SMALL(0), double AC_MED(0), double AC_BIG(0), double AC_BX(0),AC_BXL(0);
			AC_SMALL = fn_AC_Oscillator((high+low)/2,indLengthA,indLengthA*3,indLengthA*2,indLengthA*2,indLengthA);
			AC_MED = fn_AC_Oscillator((high+low)/2,indLengthB,indLengthB*3,indLengthB*2,indLengthB*2,indLengthB);
			AC_BIG = fn_AC_Oscillator((high+low)/2,indLengthC,indLengthC*3,indLengthC*2,indLengthC*2,indLengthC);
			AC_BX = fn_AC_Oscillator((high+low)/2,indLengthD,indLengthD*3,indLengthD*2,indLengthD*2,indLengthD);
			AC_BXL = fn_AC_Oscillator((high+low)/2,indLengthE,indLengthE*3,indLengthE*2,indLengthE*2,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double AC_SMALL_NORM(0),double AC_MED_NORM(0),double AC_BIG_NORM(0),double AC_BX_NORM(0),double AC_BXL_NORM(0);
			AC_SMALL_NORM = NormalizeData(AC_SMALL, lookBackLength, scaleLimit, scaleFloor);
			AC_MED_NORM = NormalizeData(AC_MED, lookBackLength, scaleLimit, scaleFloor);
			AC_BIG_NORM = NormalizeData(AC_BIG, lookBackLength, scaleLimit, scaleFloor);
			AC_BX_NORM = NormalizeData(AC_BX, lookBackLength, scaleLimit, scaleFloor);
			AC_BXL_NORM = NormalizeData(AC_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double ACindSMALL (0),double ACindMED (0),double ACindBIG (0),double ACindBX (0),double ACindBXL (0);
			If UseBucketData Then Begin
				ACindSMALL = BucketData(AC_SMALL_NORM,intervalValue,intervalLimit);
				ACindMED = BucketData(AC_MED_NORM,intervalValue,intervalLimit);
				ACindBIG = BucketData(AC_BIG_NORM,intervalValue,intervalLimit);
				ACindBX = BucketData(AC_BX_NORM,intervalValue,intervalLimit);
				ACindBXL = BucketData(AC_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				ACindSMALL = AC_SMALL_NORM*100;
				ACindMED = AC_MED_NORM*100;
				ACindBIG = AC_BIG_NORM*100;
				ACindBX = AC_BX_NORM*100;
				ACindBXL = AC_BXL_NORM*100;
	End;
	

	//*****************************************************
	//************ Accumulation Swing Index (ASI) ********************* 
	//*****************************************************
	//Plot1( AccumSwingIndex, "ASX" ) ;  //AccumSwingIndex = AccumSwingIndex[1] + SwingIndex ;
	{
	variables:
		asi.value( 0 ) ,asi.diff(0);
		
	asi.value = AccumSwingIndex ;
	
	if CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 then begin
		//scale and normalize features, (indicator results) ********************
		Var: double n.asi.Percent(0);
		n.asi.Percent = NormalizeData(asi.value,lookBackLength,scaleLimit,scaleFloor);
		//categorize features, (indicator results)*****************************
		Var: double asi.indRange (0), double asi.indRangeLarge (0);
		asi.indRange = BucketData(n.asi.Percent,intervalValue,intervalLimit);
		//asi.indRangeLarge = BucketData(n.asi.Percent,intervalValueLarge,intervalLimit);
	end;}
	
	{
	Plot1(50,"50");
	Plot2(asi.indRange,"indRange");
	Plot3(n.asi.Percent);
	}

	//*****************************************************
	//************ Accumulation Distribution (ADL) ********************* 
	//*****************************************************
	//https://www.tradingview.com/stock-charts-support/index.php/Accumulation_Distribution_(ADL)

	
	//*****************************************************
	//************ Alligator ********************* 
	//*****************************************************
	{inputs: jawLen(13), lipsLen(8), teethLen(5);
	
	vars:jaw(0), lips(0), teeth(0);
	
	jaw=SmoothedAverage(close, jawLen)[lipsLen];
	lips=SmoothedAverage(close, lipsLen)[teethLen];
	teeth=SmoothedAverage(close, teethLen)[teethLen*.6];}
	
	{
	plot1(jaw, "jaw");
	plot2(lips, "lips");
	plot3(teeth, "teeth");
	}

	//*****************************************************
	//************ Aroon Oscillator ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double AROON_SMALL(0), double AROON_MED(0), double AROON_BIG(0), double AROON_BX(0),AROON_BXL(0);
			AROON_SMALL = fn_Aroon_Oscillator(High,Low,indLengthA,indLengthB);
			AROON_MED = fn_Aroon_Oscillator(High,Low,indLengthB,indLengthC);
			AROON_BIG = fn_Aroon_Oscillator(High,Low,indLengthC,indLengthD);
			AROON_BX = fn_Aroon_Oscillator(High,Low,indLengthD,indLengthE);
			AROON_BXL = fn_Aroon_Oscillator(High,Low,indLengthE,indLengthC+indLengthD);

		//scale and normalize features, (indicator results) ********************
		Var: double AROON_SMALL_NORM(0),double AROON_MED_NORM(0),double AROON_BIG_NORM(0),double AROON_BX_NORM(0),double AROON_BXL_NORM(0);
			AROON_SMALL_NORM = NormalizeData(AROON_SMALL, lookBackLength, scaleLimit, scaleFloor);
			AROON_MED_NORM = NormalizeData(AROON_MED, lookBackLength, scaleLimit, scaleFloor);
			AROON_BIG_NORM = NormalizeData(AROON_BIG, lookBackLength, scaleLimit, scaleFloor);
			AROON_BX_NORM = NormalizeData(AROON_BX, lookBackLength, scaleLimit, scaleFloor);
			AROON_BXL_NORM = NormalizeData(AROON_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double AROONindSMALL (0),double AROONindMED (0),double AROONindBIG (0),double AROONindBX (0),double AROONindBXL (0);
			If UseBucketData Then Begin
				AROONindSMALL = BucketData(AROON_SMALL_NORM,intervalValue,intervalLimit);
				AROONindMED = BucketData(AROON_MED_NORM,intervalValue,intervalLimit);
				AROONindBIG = BucketData(AROON_BIG_NORM,intervalValue,intervalLimit);
				AROONindBX = BucketData(AROON_BX_NORM,intervalValue,intervalLimit);
				AROONindBXL = BucketData(AROON_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				AROONindSMALL = AROON_SMALL_NORM*10;
				AROONindMED = AROON_MED_NORM*10;
				AROONindBIG = AROON_BIG_NORM*10;
				AROONindBX = AROON_BX_NORM*10;
				AROONindBXL = AROON_BXL_NORM*10;
	End;

 
	//*****************************************************
	//************ Average Directional Index (ADX) ********************* 
	//*****************************************************
	{inputs: 
		adx.Length( 14 ) ;
	
	variables: 
		adx.oDMIPlus( 0 ), 
		adx.oDMIMinus( 0 ), 
		adx.oDMI( 0 ), 
		adx.oADX( 0 ), 
		adx.oADXR( 0 ), 
		adx.oVolty( 0 ),
		adx.ADXvalue(0);
	
	adx.ADXvalue = DirMovement( H, L, C, adx.Length, adx.oDMIPlus, adx.oDMIMinus, adx.oDMI, adx.oADX, adx.oADXR, 
 	adx.oVolty ) ;}
	{
	Plot1( adx.oADX, "ADX" ) ;
	}
	
	//*****************************************************
	//************ Average Directional Index Rating (ADXR) ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double ADXR_SMALL(0), double ADXR_MED(0), double ADXR_BIG(0), double ADXR_BX(0),ADXR_BXL(0);
			ADXR_SMALL = fn_ADXR(indLengthA);
			ADXR_MED = fn_ADXR(indLengthB);
			ADXR_BIG = fn_ADXR(indLengthC);
			ADXR_BX = fn_ADXR(indLengthD);
			ADXR_BXL = fn_ADXR(indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double ADXR_SMALL_NORM(0),double ADXR_MED_NORM(0),double ADXR_BIG_NORM(0),double ADXR_BX_NORM(0),double ADXR_BXL_NORM(0);
			ADXR_SMALL_NORM = NormalizeData(ADXR_SMALL, lookBackLength, scaleLimit, scaleFloor);
			ADXR_MED_NORM = NormalizeData(ADXR_MED, lookBackLength, scaleLimit, scaleFloor);
			ADXR_BIG_NORM = NormalizeData(ADXR_BIG, lookBackLength, scaleLimit, scaleFloor);
			ADXR_BX_NORM = NormalizeData(ADXR_BX, lookBackLength, scaleLimit, scaleFloor);
			ADXR_BXL_NORM = NormalizeData(ADXR_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double ADXRindSMALL (0),double ADXRindMED (0),double ADXRindBIG (0),double ADXRindBX (0),double ADXRindBXL (0);
			If UseBucketData Then Begin
				ADXRindSMALL = BucketData(ADXR_SMALL_NORM,intervalValue,intervalLimit);
				ADXRindMED = BucketData(ADXR_MED_NORM,intervalValue,intervalLimit);
				ADXRindBIG = BucketData(ADXR_BIG_NORM,intervalValue,intervalLimit);
				ADXRindBX = BucketData(ADXR_BX_NORM,intervalValue,intervalLimit);
				ADXRindBXL = BucketData(ADXR_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				ADXRindSMALL = ADXR_SMALL_NORM;
				ADXRindMED = ADXR_MED_NORM;
				ADXRindBIG = ADXR_BIG_NORM;
				ADXRindBX = ADXR_BX_NORM;
				ADXRindBXL = ADXR_BXL_NORM;
	End;

	//*****************************************************
	//************ Average True Range (ATR) ***************
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double ATR_SMALL(0), double ATR_MED(0), double ATR_BIG(0), double ATR_BX(0),ATR_BXL(0);
			ATR_SMALL = fn_Average_True_Range(Close,High,Low,indLengthA);
			ATR_MED = fn_Average_True_Range(Close,High,Low,indLengthB);
			ATR_BIG = fn_Average_True_Range(Close,High,Low,indLengthC);
			ATR_BX = fn_Average_True_Range(Close,High,Low,indLengthD);
			ATR_BXL = fn_Average_True_Range(Close,High,Low,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double ATR_SMALL_NORM(0),double ATR_MED_NORM(0),double ATR_BIG_NORM(0),double ATR_BX_NORM(0),double ATR_BXL_NORM(0);
			ATR_SMALL_NORM = NormalizeData(ATR_SMALL, lookBackLength, scaleLimit, scaleFloor);
			ATR_MED_NORM = NormalizeData(ATR_MED, lookBackLength, scaleLimit, scaleFloor);
			ATR_BIG_NORM = NormalizeData(ATR_BIG, lookBackLength, scaleLimit, scaleFloor);
			ATR_BX_NORM = NormalizeData(ATR_BX, lookBackLength, scaleLimit, scaleFloor);
			ATR_BXL_NORM = NormalizeData(ATR_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double ATRindSMALL (0),double ATRindMED (0),double ATRindBIG (0),double ATRindBX (0),double ATRindBXL (0);
			If UseBucketData Then Begin
				ATRindSMALL = BucketData(ATR_SMALL_NORM,intervalValue,intervalLimit);
				ATRindMED = BucketData(ATR_MED_NORM,intervalValue,intervalLimit);
				ATRindBIG = BucketData(ATR_BIG_NORM,intervalValue,intervalLimit);
				ATRindBX = BucketData(ATR_BX_NORM,intervalValue,intervalLimit);
				ATRindBXL = BucketData(ATR_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				ATRindSMALL = ATR_SMALL_NORM*10;
				ATRindMED = ATR_MED_NORM*10;
				ATRindBIG = ATR_BIG_NORM*10;
				ATRindBX = ATR_BX_NORM*10;
				ATRindBXL = ATR_BXL_NORM*10;
	End;

	//*****************************************************
	//************ Awesome Oscillator ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double AWEOSC_SMALL(0), double AWEOSC_MED(0), double AWEOSC_BIG(0), double AWEOSC_BX(0),AWEOSC_BXL(0);
			AWEOSC_SMALL = fn_Awesome_Oscillator(High,Low,indLengthA,indLengthB);
			AWEOSC_MED = fn_Awesome_Oscillator(High,Low,indLengthB,indLengthC);
			AWEOSC_BIG = fn_Awesome_Oscillator(High,Low,indLengthC,indLengthD);
			AWEOSC_BX = fn_Awesome_Oscillator(High,Low,indLengthD,indLengthE);
			AWEOSC_BXL = fn_Awesome_Oscillator(High,Low,indLengthE,indLengthC+indLengthD);

		//scale and normalize features, (indicator results) ********************
		Var: double AWEOSC_SMALL_NORM(0),double AWEOSC_MED_NORM(0),double AWEOSC_BIG_NORM(0),double AWEOSC_BX_NORM(0),double AWEOSC_BXL_NORM(0);
			AWEOSC_SMALL_NORM = NormalizeData(AWEOSC_SMALL, lookBackLength, scaleLimit, scaleFloor);
			AWEOSC_MED_NORM = NormalizeData(AWEOSC_MED, lookBackLength, scaleLimit, scaleFloor);
			AWEOSC_BIG_NORM = NormalizeData(AWEOSC_BIG, lookBackLength, scaleLimit, scaleFloor);
			AWEOSC_BX_NORM = NormalizeData(AWEOSC_BX, lookBackLength, scaleLimit, scaleFloor);
			AWEOSC_BXL_NORM = NormalizeData(AWEOSC_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double AWEOSCindSMALL (0),double AWEOSCindMED (0),double AWEOSCindBIG (0),double AWEOSCindBX (0),double AWEOSCindBXL (0);
			If UseBucketData Then Begin
				AWEOSCindSMALL = BucketData(AWEOSC_SMALL_NORM,intervalValue,intervalLimit);
				AWEOSCindMED = BucketData(AWEOSC_MED_NORM,intervalValue,intervalLimit);
				AWEOSCindBIG = BucketData(AWEOSC_BIG_NORM,intervalValue,intervalLimit);
				AWEOSCindBX = BucketData(AWEOSC_BX_NORM,intervalValue,intervalLimit);
				AWEOSCindBXL = BucketData(AWEOSC_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				AWEOSCindSMALL = AWEOSC_SMALL_NORM*100;
				AWEOSCindMED = AWEOSC_MED_NORM*100;
				AWEOSCindBIG = AWEOSC_BIG_NORM*100;
				AWEOSCindBX = AWEOSC_BX_NORM*100;
				AWEOSCindBXL = AWEOSC_BXL_NORM*100;
	End;

	//*****************************************************
	//************ Bollinger Band %B Osc ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double pctB_SMALL(0), double pctB_MED(0), double pctB_BIG(0), double pctB_BX(0),pctB_BXL(0);
			pctB_SMALL = fn_Bollinger_Band_B_Osc(Close,indLengthA,2,-2);
			pctB_MED = fn_Bollinger_Band_B_Osc(Close,indLengthB,2,-2);
			pctB_BIG = fn_Bollinger_Band_B_Osc(Close,indLengthC,2,-2);
			pctB_BX = fn_Bollinger_Band_B_Osc(Close,indLengthD,2,-2);
			pctB_BXL = fn_Bollinger_Band_B_Osc(Close,indLengthE,2,-2);

		//scale and normalize features, (indicator results) ********************
		Var: double pctB_SMALL_NORM(0),double pctB_MED_NORM(0),double pctB_BIG_NORM(0),double pctB_BX_NORM(0),double pctB_BXL_NORM(0);
			pctB_SMALL_NORM = NormalizeData(pctB_SMALL, lookBackLength, scaleLimit, scaleFloor);
			pctB_MED_NORM = NormalizeData(pctB_MED, lookBackLength, scaleLimit, scaleFloor);
			pctB_BIG_NORM = NormalizeData(pctB_BIG, lookBackLength, scaleLimit, scaleFloor);
			pctB_BX_NORM = NormalizeData(pctB_BX, lookBackLength, scaleLimit, scaleFloor);
			pctB_BXL_NORM = NormalizeData(pctB_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double pctBindSMALL (0),double pctBindMED (0),double pctBindBIG (0),double pctBindBX (0),double pctBindBXL (0);
			If UseBucketData Then Begin
				pctBindSMALL = BucketData(pctB_SMALL_NORM,intervalValue,intervalLimit);
				pctBindMED = BucketData(pctB_MED_NORM,intervalValue,intervalLimit);
				pctBindBIG = BucketData(pctB_BIG_NORM,intervalValue,intervalLimit);
				pctBindBX = BucketData(pctB_BX_NORM,intervalValue,intervalLimit);
				pctBindBXL = BucketData(pctB_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				pctBindSMALL = pctB_SMALL_NORM;
				pctBindMED = pctB_MED_NORM;
				pctBindBIG = pctB_BIG_NORM;
				pctBindBX = pctB_BX_NORM;
				pctBindBXL = pctB_BXL_NORM;
	End;

	//*****************************************************
	//************ Bollinger Band (Hull) %B Osc ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double pctH_SMALL(0), double pctH_MED(0), double pctH_BIG(0), double pctH_BX(0),pctH_BXL(0);
			pctH_SMALL = fn_Bollinger_Band_B_Hull_Osc(Close,indLengthA,2,-2);
			pctH_MED = fn_Bollinger_Band_B_Hull_Osc(Close,indLengthB,2,-2);
			pctH_BIG = fn_Bollinger_Band_B_Hull_Osc(Close,indLengthC,2,-2);
			pctH_BX = fn_Bollinger_Band_B_Hull_Osc(Close,indLengthD,2,-2);
			pctH_BXL = fn_Bollinger_Band_B_Hull_Osc(Close,indLengthE,2,-2);

		//scale and normalize features, (indicator results) ********************
		Var: double pctH_SMALL_NORM(0),double pctH_MED_NORM(0),double pctH_BIG_NORM(0),double pctH_BX_NORM(0),double pctH_BXL_NORM(0);
			pctH_SMALL_NORM = NormalizeData(pctH_SMALL, lookBackLength, scaleLimit, scaleFloor);
			pctH_MED_NORM = NormalizeData(pctH_MED, lookBackLength, scaleLimit, scaleFloor);
			pctH_BIG_NORM = NormalizeData(pctH_BIG, lookBackLength, scaleLimit, scaleFloor);
			pctH_BX_NORM = NormalizeData(pctH_BX, lookBackLength, scaleLimit, scaleFloor);
			pctH_BXL_NORM = NormalizeData(pctH_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double pctHindSMALL (0),double pctHindMED (0),double pctHindBIG (0),double pctHindBX (0),double pctHindBXL (0);
			If UseBucketData Then Begin
				pctHindSMALL = BucketData(pctH_SMALL_NORM,intervalValue,intervalLimit);
				pctHindMED = BucketData(pctH_MED_NORM,intervalValue,intervalLimit);
				pctHindBIG = BucketData(pctH_BIG_NORM,intervalValue,intervalLimit);
				pctHindBX = BucketData(pctH_BX_NORM,intervalValue,intervalLimit);
				pctHindBXL = BucketData(pctH_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				pctHindSMALL = pctH_SMALL_NORM;
				pctHindMED = pctH_MED_NORM;
				pctHindBIG = pctH_BIG_NORM;
				pctHindBX = pctH_BX_NORM;
				pctHindBXL = pctH_BXL_NORM;
	End;
	
	
	//*****************************************************
	//************ Bollinger Bands ********************* 
	//*****************************************************
{	inputs:
		double bb.Price(  Close ),
		double bb.UBand(  Close ), 
		double bb.LBand(  Close ), 
		int bb.Length(  9 ), 
		double bb.NumDevsUp(  2 ), 
		double bb.NumDevsDn(  -2 ), 
		int bb.Displace(  0 ); 
	
	variables:
	
		double bb.Avg( 0 ),
		double bb.SDev( 0 ),
		double bb.LowerBand( 0 ),
		double bb.UpperBand( 0 );
		
	if CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 then begin
	bb.Avg = AverageFC( bb.Price, bb.Length ) ;
	bb.SDev = StandardDev( bb.Price, bb.Length, 1 ) ;
	bb.UpperBand = bb.Avg + bb.NumDevsUp * bb.SDev ;
	bb.LowerBand = bb.Avg + bb.NumDevsDn * bb.SDev ;
	
	if bb.Displace >= 0 or CurrentBar > AbsValue( bb.Displace ) then 
		begin
	
		//Plot1[Displace]( UpperBand, "UpperBand" ) ;
		//Plot2[Displace]( LowerBand, "LowerBand" ) ;
		//Plot3[Displace]( Avg, "MidLine" ) ;
	end;
	end;}
	
	//*****************************************************
	//************ Bollinger Band Width indicator ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double BBD_SMALL(0), double BBD_MED(0), double BBD_BIG(0), double BBD_BX(0),BBD_BXL(0);
			BBD_SMALL = fn_Bollinger_Band_Width_Ind(Close,indLengthA,2,-2);
			BBD_MED = fn_Bollinger_Band_Width_Ind(Close,indLengthB,2,-2);
			BBD_BIG = fn_Bollinger_Band_Width_Ind(Close,indLengthC,2,-2);
			BBD_BX = fn_Bollinger_Band_Width_Ind(Close,indLengthD,2,-2);
			BBD_BXL = fn_Bollinger_Band_Width_Ind(Close,indLengthE,2,-2);

		//scale and normalize features, (indicator results) ********************
		Var: double BBD_SMALL_NORM(0),double BBD_MED_NORM(0),double BBD_BIG_NORM(0),double BBD_BX_NORM(0),double BBD_BXL_NORM(0);
			BBD_SMALL_NORM = NormalizeData(BBD_SMALL, lookBackLength, scaleLimit, scaleFloor);
			BBD_MED_NORM = NormalizeData(BBD_MED, lookBackLength, scaleLimit, scaleFloor);
			BBD_BIG_NORM = NormalizeData(BBD_BIG, lookBackLength, scaleLimit, scaleFloor);
			BBD_BX_NORM = NormalizeData(BBD_BX, lookBackLength, scaleLimit, scaleFloor);
			BBD_BXL_NORM = NormalizeData(BBD_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double BBDindSMALL (0),double BBDindMED (0),double BBDindBIG (0),double BBDindBX (0),double BBDindBXL (0);
			If UseBucketData Then Begin
				BBDindSMALL = BucketData(BBD_SMALL_NORM,intervalValue,intervalLimit);
				BBDindMED = BucketData(BBD_MED_NORM,intervalValue,intervalLimit);
				BBDindBIG = BucketData(BBD_BIG_NORM,intervalValue,intervalLimit);
				BBDindBX = BucketData(BBD_BX_NORM,intervalValue,intervalLimit);
				BBDindBXL = BucketData(BBD_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				BBDindSMALL = BBD_SMALL_NORM*100;
				BBDindMED = BBD_MED_NORM*100;
				BBDindBIG = BBD_BIG_NORM*100;
				BBDindBX = BBD_BX_NORM*100;
				BBDindBXL = BBD_BXL_NORM*100;
	End;
	
	//*****************************************************
	//************ Chaikin Money Flow (CMF) ********************* 
	//*****************************************************
	{Inputs:  cmf.length(21);
	{ UpVolColor(green), DownVolColor(Red),
	low10color(white), uptencolor(white), topcolor(blue),
	I had to bracket out these inputs as TS4 says input names too long.}
	Variable: cmf.AccDist(0),  cmf.MF(0), cmf.totalsum(0) ;
	
	If high - low <> 0 and volume <> 0 then
	cmf.AccDist = ((close-low) - (high - close)) / (high - low) * volume;
	
	cmf.Totalsum = summation(volume, cmf.length);
	
	If cmf.Totalsum <>0 then
  	cmf.MF = summation(cmf.AccDist, cmf.Length) / cmf.totalsum ;}
	
	{
	Plot1 (cmf.Mf, "money Flow");
	Plot2 (0, "0 line");
	}
	
	//*****************************************************
	//************ Chaikin Osc ********************* 
	//*****************************************************
	{inputs:
		co.FastLength( 3 ),
		co.SlowLength( 10 );
	
	var: co.ChaikinOscVal(0);
	
	co.ChaikinOscVal = ChaikinOsc( volume, co.FastLength, co.SlowLength ) ;}
	{
	Plot1( co.ChaikinOscVal, "ChaikinOsc" ) ;
	Plot2( 0 , "ZeroLine" ) ;
	}
	
	//*****************************************************
	//************ Chaikin Volatility ********************* 
	//*****************************************************
	
	//*****************************************************
	//************ Chande Momentum Oscillator (CMO) ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double CMO_SMALL(0), double CMO_MED(0), double CMO_BIG(0), double CMO_BX(0),CMO_BXL(0);
			CMO_SMALL = fn_CMO(Close,indLengthA);
			CMO_MED = fn_CMO(Close,indLengthB);
			CMO_BIG = fn_CMO(Close,indLengthC);
			CMO_BX = fn_CMO(Close,indLengthD);
			CMO_BXL = fn_CMO(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double CMO_SMALL_NORM(0),double CMO_MED_NORM(0),double CMO_BIG_NORM(0),double CMO_BX_NORM(0),double CMO_BXL_NORM(0);
			CMO_SMALL_NORM = NormalizeData(CMO_SMALL, lookBackLength, scaleLimit, scaleFloor);
			CMO_MED_NORM = NormalizeData(CMO_MED, lookBackLength, scaleLimit, scaleFloor);
			CMO_BIG_NORM = NormalizeData(CMO_BIG, lookBackLength, scaleLimit, scaleFloor);
			CMO_BX_NORM = NormalizeData(CMO_BX, lookBackLength, scaleLimit, scaleFloor);
			CMO_BXL_NORM = NormalizeData(CMO_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double CMOindSMALL (0),double CMOindMED (0),double CMOindBIG (0),double CMOindBX (0),double CMOindBXL (0);
			If UseBucketData Then Begin
				CMOindSMALL = BucketData(CMO_SMALL_NORM,intervalValue,intervalLimit);
				CMOindMED = BucketData(CMO_MED_NORM,intervalValue,intervalLimit);
				CMOindBIG = BucketData(CMO_BIG_NORM,intervalValue,intervalLimit);
				CMOindBX = BucketData(CMO_BX_NORM,intervalValue,intervalLimit);
				CMOindBXL = BucketData(CMO_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				CMOindSMALL = CMO_SMALL_NORM;
				CMOindMED = CMO_MED_NORM;
				CMOindBIG = CMO_BIG_NORM;
				CMOindBX = CMO_BX_NORM;
				CMOindBXL = CMO_BXL_NORM;
	End;
	
	
	//*****************************************************
	//************ Chande DMI (CDMI) ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double CDMI_SMALL(0), double CDMI_MED(0), double CDMI_BIG(0), double CDMI_BX(0),CDMI_BXL(0);
			CDMI_SMALL = fn_CDMI(indLengthA);
			CDMI_MED = fn_CDMI(indLengthB);
			CDMI_BIG = fn_CDMI(indLengthC);
			CDMI_BX = fn_CDMI(indLengthD);
			CDMI_BXL = fn_CDMI(indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double CDMI_SMALL_NORM(0),double CDMI_MED_NORM(0),double CDMI_BIG_NORM(0),double CDMI_BX_NORM(0),double CDMI_BXL_NORM(0);
			CDMI_SMALL_NORM = NormalizeData(CDMI_SMALL, lookBackLength, scaleLimit, scaleFloor);
			CDMI_MED_NORM = NormalizeData(CDMI_MED, lookBackLength, scaleLimit, scaleFloor);
			CDMI_BIG_NORM = NormalizeData(CDMI_BIG, lookBackLength, scaleLimit, scaleFloor);
			CDMI_BX_NORM = NormalizeData(CDMI_BX, lookBackLength, scaleLimit, scaleFloor);
			CDMI_BXL_NORM = NormalizeData(CDMI_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double CDMIindSMALL (0),double CDMIindMED (0),double CDMIindBIG (0),double CDMIindBX (0),double CDMIindBXL (0);
			If UseBucketData Then Begin
				CDMIindSMALL = BucketData(CDMI_SMALL_NORM,intervalValue,intervalLimit);
				CDMIindMED = BucketData(CDMI_MED_NORM,intervalValue,intervalLimit);
				CDMIindBIG = BucketData(CDMI_BIG_NORM,intervalValue,intervalLimit);
				CDMIindBX = BucketData(CDMI_BX_NORM,intervalValue,intervalLimit);
				CDMIindBXL = BucketData(CDMI_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				CDMIindSMALL = CDMI_SMALL_NORM;
				CDMIindMED = CDMI_MED_NORM;
				CDMIindBIG = CDMI_BIG_NORM;
				CDMIindBX = CDMI_BX_NORM;
				CDMIindBXL = CDMI_BXL_NORM;
	End;
	
	//*****************************************************
	//************ DMI Stochastic ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double DMISto_SMALL(0), double DMISto_MED(0), double DMISto_BIG(0), double DMISto_BX(0),DMISto_BXL(0);
			DMISto_SMALL = fn_DMI_Stochastic(indLengthA, 3, 3);
			DMISto_MED = fn_DMI_Stochastic(indLengthB, 3, 3);
			DMISto_BIG = fn_DMI_Stochastic(indLengthC, 3, 3);
			DMISto_BX = fn_DMI_Stochastic(indLengthD, 3, 3);
			DMISto_BXL = fn_DMI_Stochastic(indLengthE, 3, 3);

		//scale and normalize features, (indicator results) ********************
		Var: double DMISto_SMALL_NORM(0),double DMISto_MED_NORM(0),double DMISto_BIG_NORM(0),double DMISto_BX_NORM(0),double DMISto_BXL_NORM(0);
			DMISto_SMALL_NORM = NormalizeData(DMISto_SMALL, lookBackLength, scaleLimit, scaleFloor);
			DMISto_MED_NORM = NormalizeData(DMISto_MED, lookBackLength, scaleLimit, scaleFloor);
			DMISto_BIG_NORM = NormalizeData(DMISto_BIG, lookBackLength, scaleLimit, scaleFloor);
			DMISto_BX_NORM = NormalizeData(DMISto_BX, lookBackLength, scaleLimit, scaleFloor);
			DMISto_BXL_NORM = NormalizeData(DMISto_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double DMIStoindSMALL (0),double DMIStoindMED (0),double DMIStoindBIG (0),double DMIStoindBX (0),double DMIStoindBXL (0);
			If UseBucketData Then Begin
				DMIStoindSMALL = BucketData(DMISto_SMALL_NORM,intervalValue,intervalLimit);
				DMIStoindMED = BucketData(DMISto_MED_NORM,intervalValue,intervalLimit);
				DMIStoindBIG = BucketData(DMISto_BIG_NORM,intervalValue,intervalLimit);
				DMIStoindBX = BucketData(DMISto_BX_NORM,intervalValue,intervalLimit);
				DMIStoindBXL = BucketData(DMISto_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				DMIStoindSMALL = DMISto_SMALL_NORM;
				DMIStoindMED = DMISto_MED_NORM;
				DMIStoindBIG = DMISto_BIG_NORM;
				DMIStoindBX = DMISto_BX_NORM;
				DMIStoindBXL = DMISto_BXL_NORM;
	End;


	//*****************************************************
	//************ Relative Momentum Index (RMI) ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double RMI_SMALL(0), double RMI_MED(0), double RMI_BIG(0), double RMI_BX(0),RMI_BXL(0);
			RMI_SMALL = fn_RMI(Close,indLengthA);
			RMI_MED = fn_RMI(Close,indLengthB);
			RMI_BIG = fn_RMI(Close,indLengthC);
			RMI_BX = fn_RMI(Close,indLengthD);
			RMI_BXL = fn_RMI(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double RMI_SMALL_NORM(0),double RMI_MED_NORM(0),double RMI_BIG_NORM(0),double RMI_BX_NORM(0),double RMI_BXL_NORM(0);
			RMI_SMALL_NORM = NormalizeData(RMI_SMALL, lookBackLength, scaleLimit, scaleFloor);
			RMI_MED_NORM = NormalizeData(RMI_MED, lookBackLength, scaleLimit, scaleFloor);
			RMI_BIG_NORM = NormalizeData(RMI_BIG, lookBackLength, scaleLimit, scaleFloor);
			RMI_BX_NORM = NormalizeData(RMI_BX, lookBackLength, scaleLimit, scaleFloor);
			RMI_BXL_NORM = NormalizeData(RMI_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double RMIindSMALL (0),double RMIindMED (0),double RMIindBIG (0),double RMIindBX (0),double RMIindBXL (0);
			If UseBucketData Then Begin
				RMIindSMALL = BucketData(RMI_SMALL_NORM,intervalValue,intervalLimit);
				RMIindMED = BucketData(RMI_MED_NORM,intervalValue,intervalLimit);
				RMIindBIG = BucketData(RMI_BIG_NORM,intervalValue,intervalLimit);
				RMIindBX = BucketData(RMI_BX_NORM,intervalValue,intervalLimit);
				RMIindBXL = BucketData(RMI_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				RMIindSMALL = RMI_SMALL_NORM;
				RMIindMED = RMI_MED_NORM;
				RMIindBIG = RMI_BIG_NORM;
				RMIindBX = RMI_BX_NORM;
				RMIindBXL = RMI_BXL_NORM;
	End;
		
	//*****************************************************
	//************ Commodity Channel Index (CCI) ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double CCI_SMALL(0), double CCI_MED(0), double CCI_BIG(0), double CCI_BX(0),CCI_BXL(0);
			CCI_SMALL = fn_CCI(Close,High,Low,indLengthA,0.015);
			CCI_MED = fn_CCI(Close,High,Low,indLengthB,0.015);
			CCI_BIG = fn_CCI(Close,High,Low,indLengthC,0.015);
			CCI_BX = fn_CCI(Close,High,Low,indLengthD,0.015);
			CCI_BXL = fn_CCI(Close,High,Low,indLengthE,0.015);

		//scale and normalize features, (indicator results) ********************
		Var: double CCI_SMALL_NORM(0),double CCI_MED_NORM(0),double CCI_BIG_NORM(0),double CCI_BX_NORM(0),double CCI_BXL_NORM(0);
			CCI_SMALL_NORM = NormalizeData(CCI_SMALL, lookBackLength, scaleLimit, scaleFloor);
			CCI_MED_NORM = NormalizeData(CCI_MED, lookBackLength, scaleLimit, scaleFloor);
			CCI_BIG_NORM = NormalizeData(CCI_BIG, lookBackLength, scaleLimit, scaleFloor);
			CCI_BX_NORM = NormalizeData(CCI_BX, lookBackLength, scaleLimit, scaleFloor);
			CCI_BXL_NORM = NormalizeData(CCI_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double CCIindSMALL (0),double CCIindMED (0),double CCIindBIG (0),double CCIindBX (0),double CCIindBXL (0);
			If UseBucketData Then Begin
				CCIindSMALL = BucketData(CCI_SMALL_NORM,intervalValue,intervalLimit);
				CCIindMED = BucketData(CCI_MED_NORM,intervalValue,intervalLimit);
				CCIindBIG = BucketData(CCI_BIG_NORM,intervalValue,intervalLimit);
				CCIindBX = BucketData(CCI_BX_NORM,intervalValue,intervalLimit);
				CCIindBXL = BucketData(CCI_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				CCIindSMALL = CCI_SMALL_NORM*100;
				CCIindMED = CCI_MED_NORM*100;
				CCIindBIG = CCI_BIG_NORM*100;
				CCIindBX = CCI_BX_NORM*100;
				CCIindBXL = CCI_BXL_NORM*100;
	End;

	//*****************************************************
	//************ Commodity Selection Index (CSI) ********************* 
	//*****************************************************
	//fn_CSI(,,)
	
	//*****************************************************
	//************ Choppiness Index (CHOP) ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double CHOP_SMALL(0), double CHOP_MED(0), double CHOP_BIG(0), double CHOP_BX(0),CHOP_BXL(0);
			CHOP_SMALL = fn_CHOP(Close,indLengthA);
			CHOP_MED = fn_CHOP(Close,indLengthB);
			CHOP_BIG = fn_CHOP(Close,indLengthC);
			CHOP_BX = fn_CHOP(Close,indLengthD);
			CHOP_BXL = fn_CHOP(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double CHOP_SMALL_NORM(0),double CHOP_MED_NORM(0),double CHOP_BIG_NORM(0),double CHOP_BX_NORM(0),double CHOP_BXL_NORM(0);
			CHOP_SMALL_NORM = NormalizeData(CHOP_SMALL, lookBackLength, scaleLimit, scaleFloor);
			CHOP_MED_NORM = NormalizeData(CHOP_MED, lookBackLength, scaleLimit, scaleFloor);
			CHOP_BIG_NORM = NormalizeData(CHOP_BIG, lookBackLength, scaleLimit, scaleFloor);
			CHOP_BX_NORM = NormalizeData(CHOP_BX, lookBackLength, scaleLimit, scaleFloor);
			CHOP_BXL_NORM = NormalizeData(CHOP_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double CHOPindSMALL (0),double CHOPindMED (0),double CHOPindBIG (0),double CHOPindBX (0),double CHOPindBXL (0);
			If UseBucketData Then Begin
				CHOPindSMALL = BucketData(CHOP_SMALL_NORM,intervalValue,intervalLimit);
				CHOPindMED = BucketData(CHOP_MED_NORM,intervalValue,intervalLimit);
				CHOPindBIG = BucketData(CHOP_BIG_NORM,intervalValue,intervalLimit);
				CHOPindBX = BucketData(CHOP_BX_NORM,intervalValue,intervalLimit);
				CHOPindBXL = BucketData(CHOP_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				CHOPindSMALL = CHOP_SMALL_NORM;
				CHOPindMED = CHOP_MED_NORM;
				CHOPindBIG = CHOP_BIG_NORM;
				CHOPindBX = CHOP_BX_NORM;
				CHOPindBXL = CHOP_BXL_NORM;
	End;	
	
	
	//*****************************************************
	//************ Choppy Market Index Function by George Pruitt (fn_CMI) ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double CMI_SMALL(0), double CMI_MED(0), double CMI_BIG(0), double CMI_BX(0),CMI_BXL(0);
			CMI_SMALL = fn_CMI(Close,High,Low,indLengthA);
			CMI_MED = fn_CMI(Close,High,Low,indLengthB);
			CMI_BIG = fn_CMI(Close,High,Low,indLengthC);
			CMI_BX = fn_CMI(Close,High,Low,indLengthD);
			CMI_BXL = fn_CMI(Close,High,Low,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double CMI_SMALL_NORM(0),double CMI_MED_NORM(0),double CMI_BIG_NORM(0),double CMI_BX_NORM(0),double CMI_BXL_NORM(0);
			CMI_SMALL_NORM = NormalizeData(CMI_SMALL, lookBackLength, scaleLimit, scaleFloor);
			CMI_MED_NORM = NormalizeData(CMI_MED, lookBackLength, scaleLimit, scaleFloor);
			CMI_BIG_NORM = NormalizeData(CMI_BIG, lookBackLength, scaleLimit, scaleFloor);
			CMI_BX_NORM = NormalizeData(CMI_BX, lookBackLength, scaleLimit, scaleFloor);
			CMI_BXL_NORM = NormalizeData(CMI_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double CMIindSMALL (0),double CMIindMED (0),double CMIindBIG (0),double CMIindBX (0),double CMIindBXL (0);
			If UseBucketData Then Begin
				CMIindSMALL = BucketData(CMI_SMALL_NORM,intervalValue,intervalLimit);
				CMIindMED = BucketData(CMI_MED_NORM,intervalValue,intervalLimit);
				CMIindBIG = BucketData(CMI_BIG_NORM,intervalValue,intervalLimit);
				CMIindBX = BucketData(CMI_BX_NORM,intervalValue,intervalLimit);
				CMIindBXL = BucketData(CMI_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				CMIindSMALL = CMI_SMALL_NORM;
				CMIindMED = CMI_MED_NORM;
				CMIindBIG = CMI_BIG_NORM;
				CMIindBX = CMI_BX_NORM;
				CMIindBXL = CMI_BXL_NORM;
	End;	
	
	//*****************************************************
	//****** Ehlers Universal Oscillator **********************
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double UNIOSC_SMALL(0), double UNIOSC_MED(0), double UNIOSC_BIG(0), double UNIOSC_BX(0),UNIOSC_BXL(0);
			UNIOSC_SMALL = fn_UO(indLengthA);
			UNIOSC_MED = fn_UO(indLengthB);
			UNIOSC_BIG = fn_UO(indLengthC);
			UNIOSC_BX = fn_UO(indLengthD);
			UNIOSC_BXL = fn_UO(indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double UNIOSC_SMALL_NORM(0),double UNIOSC_MED_NORM(0),double UNIOSC_BIG_NORM(0),double UNIOSC_BX_NORM(0),double UNIOSC_BXL_NORM(0);
			UNIOSC_SMALL_NORM = NormalizeData(UNIOSC_SMALL, lookBackLength, scaleLimit, scaleFloor);
			UNIOSC_MED_NORM = NormalizeData(UNIOSC_MED, lookBackLength, scaleLimit, scaleFloor);
			UNIOSC_BIG_NORM = NormalizeData(UNIOSC_BIG, lookBackLength, scaleLimit, scaleFloor);
			UNIOSC_BX_NORM = NormalizeData(UNIOSC_BX, lookBackLength, scaleLimit, scaleFloor);
			UNIOSC_BXL_NORM = NormalizeData(UNIOSC_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double UNIOSCindSMALL (0),double UNIOSCindMED (0),double UNIOSCindBIG (0),double UNIOSCindBX (0),double UNIOSCindBXL (0);
			If UseBucketData Then Begin
				UNIOSCindSMALL = BucketData(UNIOSC_SMALL_NORM,intervalValue,intervalLimit);
				UNIOSCindMED = BucketData(UNIOSC_MED_NORM,intervalValue,intervalLimit);
				UNIOSCindBIG = BucketData(UNIOSC_BIG_NORM,intervalValue,intervalLimit);
				UNIOSCindBX = BucketData(UNIOSC_BX_NORM,intervalValue,intervalLimit);
				UNIOSCindBXL = BucketData(UNIOSC_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				UNIOSCindSMALL = UNIOSC_SMALL_NORM;
				UNIOSCindMED = UNIOSC_MED_NORM;
				UNIOSCindBIG = UNIOSC_BIG_NORM;
				UNIOSCindBX = UNIOSC_BX_NORM;
				UNIOSCindBXL = UNIOSC_BXL_NORM;
	End;
	
	//*****************************************************
	//************ Know Sure Thing (KST) ********************* 
	//*****************************************************
	//https://www.tradingview.com/stock-charts-support/index.php/Know_Sure_Thing_(KST)
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double KST_SMALL(0), double KST_MED(0), double KST_BIG(0), double KST_BX(0),KST_BXL(0);
			KST_SMALL = fn_KST(Close,indLengthA);
			KST_MED = fn_KST(Close,indLengthB);
			KST_BIG = fn_KST(Close,indLengthC);
			KST_BX = fn_KST(Close,indLengthD);
			KST_BXL = fn_KST(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double KST_SMALL_NORM(0),double KST_MED_NORM(0),double KST_BIG_NORM(0),double KST_BX_NORM(0),double KST_BXL_NORM(0);
			KST_SMALL_NORM = NormalizeData(KST_SMALL, lookBackLength, scaleLimit, scaleFloor);
			KST_MED_NORM = NormalizeData(KST_MED, lookBackLength, scaleLimit, scaleFloor);
			KST_BIG_NORM = NormalizeData(KST_BIG, lookBackLength, scaleLimit, scaleFloor);
			KST_BX_NORM = NormalizeData(KST_BX, lookBackLength, scaleLimit, scaleFloor);
			KST_BXL_NORM = NormalizeData(KST_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double KSTindSMALL (0),double KSTindMED (0),double KSTindBIG (0),double KSTindBX (0),double KSTindBXL (0);
			If UseBucketData Then Begin
				KSTindSMALL = BucketData(KST_SMALL_NORM,intervalValue,intervalLimit);
				KSTindMED = BucketData(KST_MED_NORM,intervalValue,intervalLimit);
				KSTindBIG = BucketData(KST_BIG_NORM,intervalValue,intervalLimit);
				KSTindBX = BucketData(KST_BX_NORM,intervalValue,intervalLimit);
				KSTindBXL = BucketData(KST_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				KSTindSMALL = KST_SMALL_NORM;
				KSTindMED = KST_MED_NORM;
				KSTindBIG = KST_BIG_NORM;
				KSTindBX = KST_BX_NORM;
				KSTindBXL = KST_BXL_NORM;
	End;


	//*****************************************************
	//************ Parabolic SAR (SAR) ********************* 
	//*****************************************************
	//https://www.tradingview.com/stock-charts-support/index.php/Know_Sure_Thing_(KST)
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double PSAR_SMALL(0), double PSAR_MED(0), double PSAR_BIG(0), double PSAR_BX(0),PSAR_BXL(0);
			PSAR_SMALL = fn_PSAR(indLengthA*.01,indLengthA*.1);
			PSAR_MED = fn_PSAR(indLengthB*.01,indLengthA*.1);
			PSAR_BIG = fn_PSAR(indLengthC*.01,indLengthA*.1);
			PSAR_BX = fn_PSAR(indLengthD*.01,indLengthA*.1);
			PSAR_BXL = fn_PSAR(indLengthE*.01,indLengthA*.1);

		//scale and normalize features, (indicator results) ********************
		Var: double PSAR_SMALL_NORM(0),double PSAR_MED_NORM(0),double PSAR_BIG_NORM(0),double PSAR_BX_NORM(0),double PSAR_BXL_NORM(0);
			PSAR_SMALL_NORM = NormalizeData(PSAR_SMALL, lookBackLength, scaleLimit, scaleFloor);
			PSAR_MED_NORM = NormalizeData(PSAR_MED, lookBackLength, scaleLimit, scaleFloor);
			PSAR_BIG_NORM = NormalizeData(PSAR_BIG, lookBackLength, scaleLimit, scaleFloor);
			PSAR_BX_NORM = NormalizeData(PSAR_BX, lookBackLength, scaleLimit, scaleFloor);
			PSAR_BXL_NORM = NormalizeData(PSAR_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double PSARindSMALL (0),double PSARindMED (0),double PSARindBIG (0),double PSARindBX (0),double PSARindBXL (0);
			If UseBucketData Then Begin
				PSARindSMALL = BucketData(PSAR_SMALL_NORM,intervalValue,intervalLimit);
				PSARindMED = BucketData(PSAR_MED_NORM,intervalValue,intervalLimit);
				PSARindBIG = BucketData(PSAR_BIG_NORM,intervalValue,intervalLimit);
				PSARindBX = BucketData(PSAR_BX_NORM,intervalValue,intervalLimit);
				PSARindBXL = BucketData(PSAR_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				PSARindSMALL = PSAR_SMALL_NORM;
				PSARindMED = PSAR_MED_NORM;
				PSARindBIG = PSAR_BIG_NORM;
				PSARindBX = PSAR_BX_NORM;
				PSARindBXL = PSAR_BXL_NORM;
	End;
	
		
	//*****************************************************
	//************ De-trended Price ********************* 
	//*****************************************************
	
	//*****************************************************
	//************ Detrended Price Oscillator (DPO) ********************* 
	//*****************************************************
	
	//*****************************************************
	//************ Dynamic Momentum Index (DMI) ********************* 
	//*****************************************************
	
	//*****************************************************
	//************ Ease of Movement (EMV) ********************* 
	//*****************************************************

	//*****************************************************
	//************ Envelope ********************* 
	//*****************************************************
	
	//*****************************************************
	//************ Forecast Oscillator ********************* 
	//*****************************************************
	
	//*****************************************************
	//************ Fractal/Squat indicator ********************* 
	//*****************************************************
	
	//*****************************************************
	//************ Inertia ********************* 
	//*****************************************************
	
	//*****************************************************
	//************ Intraday Momentum Index (IMI) ********************* 
	//*****************************************************
	
	//*****************************************************
	//************ Kairi *********************
	//*****************************************************
	
	//*****************************************************
	//************ Keltner Channel ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double KLTNR_SMALL(0), double KLTNR_MED(0), double KLTNR_BIG(0), double KLTNR_BX(0),KLTNR_BXL(0);
			KLTNR_SMALL = fn_Keltner_Channel(Close,indLengthA,1.5);
			KLTNR_MED = fn_Keltner_Channel(Close,indLengthB,1.5);
			KLTNR_BIG = fn_Keltner_Channel(Close,indLengthC,1.5);
			KLTNR_BX = fn_Keltner_Channel(Close,indLengthD,1.5);
			KLTNR_BXL = fn_Keltner_Channel(Close,indLengthE,1.5);

		//scale and normalize features, (indicator results) ********************
		Var: double KLTNR_SMALL_NORM(0),double KLTNR_MED_NORM(0),double KLTNR_BIG_NORM(0),double KLTNR_BX_NORM(0),double KLTNR_BXL_NORM(0);
			KLTNR_SMALL_NORM = NormalizeData(KLTNR_SMALL, lookBackLength, scaleLimit, scaleFloor);
			KLTNR_MED_NORM = NormalizeData(KLTNR_MED, lookBackLength, scaleLimit, scaleFloor);
			KLTNR_BIG_NORM = NormalizeData(KLTNR_BIG, lookBackLength, scaleLimit, scaleFloor);
			KLTNR_BX_NORM = NormalizeData(KLTNR_BX, lookBackLength, scaleLimit, scaleFloor);
			KLTNR_BXL_NORM = NormalizeData(KLTNR_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double KLTNRindSMALL (0),double KLTNRindMED (0),double KLTNRindBIG (0),double KLTNRindBX (0),double KLTNRindBXL (0);
			If UseBucketData Then Begin
				KLTNRindSMALL = BucketData(KLTNR_SMALL_NORM,intervalValue,intervalLimit);
				KLTNRindMED = BucketData(KLTNR_MED_NORM,intervalValue,intervalLimit);
				KLTNRindBIG = BucketData(KLTNR_BIG_NORM,intervalValue,intervalLimit);
				KLTNRindBX = BucketData(KLTNR_BX_NORM,intervalValue,intervalLimit);
				KLTNRindBXL = BucketData(KLTNR_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				KLTNRindSMALL = KLTNR_SMALL_NORM;
				KLTNRindMED = KLTNR_MED_NORM;
				KLTNRindBIG = KLTNR_BIG_NORM;
				KLTNRindBX = KLTNR_BX_NORM;
				KLTNRindBXL = KLTNR_BXL_NORM;
	End;
	
	//*****************************************************
	//************ Klinger Oscillator (KO) ********************* 
	//*****************************************************
	
	//*****************************************************
	//************ Linear Regression *********************
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double LINRGC_SMALL(0), double LINRGC_MED(0), double LINRGC_BIG(0), double LINRGC_BX(0),LINRGC_BXL(0);
			LINRGC_SMALL = fn_LinRegCurve(Close,indLengthA);
			LINRGC_MED = fn_LinRegCurve(Close,indLengthB);
			LINRGC_BIG = fn_LinRegCurve(Close,indLengthC);
			LINRGC_BX = fn_LinRegCurve(Close,indLengthD);
			LINRGC_BXL = fn_LinRegCurve(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double LINRGC_SMALL_NORM(0),double LINRGC_MED_NORM(0),double LINRGC_BIG_NORM(0),double LINRGC_BX_NORM(0),double LINRGC_BXL_NORM(0);
			LINRGC_SMALL_NORM = NormalizeData(LINRGC_SMALL, lookBackLength, scaleLimit, scaleFloor);
			LINRGC_MED_NORM = NormalizeData(LINRGC_MED, lookBackLength, scaleLimit, scaleFloor);
			LINRGC_BIG_NORM = NormalizeData(LINRGC_BIG, lookBackLength, scaleLimit, scaleFloor);
			LINRGC_BX_NORM = NormalizeData(LINRGC_BX, lookBackLength, scaleLimit, scaleFloor);
			LINRGC_BXL_NORM = NormalizeData(LINRGC_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double LINRGCindSMALL (0),double LINRGCindMED (0),double LINRGCindBIG (0),double LINRGCindBX (0),double LINRGCindBXL (0);
			If UseBucketData Then Begin
				LINRGCindSMALL = BucketData(LINRGC_SMALL_NORM,intervalValue,intervalLimit);
				LINRGCindMED = BucketData(LINRGC_MED_NORM,intervalValue,intervalLimit);
				LINRGCindBIG = BucketData(LINRGC_BIG_NORM,intervalValue,intervalLimit);
				LINRGCindBX = BucketData(LINRGC_BX_NORM,intervalValue,intervalLimit);
				LINRGCindBXL = BucketData(LINRGC_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				LINRGCindSMALL = LINRGC_SMALL_NORM;
				LINRGCindMED = LINRGC_MED_NORM;
				LINRGCindBIG = LINRGC_BIG_NORM;
				LINRGCindBX = LINRGC_BX_NORM;
				LINRGCindBXL = LINRGC_BXL_NORM;
	End;

 	
 	//*****************************************************
	//************ Linear Regression Angle Osc ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double LINREG_SMALL(0), double LINREG_MED(0), double LINREG_BIG(0), double LINREG_BX(0),LINREG_BXL(0);
			LINREG_SMALL = fn_Lin_Reg_Angle_Osc(Close,indLengthA,0,-1);
			LINREG_MED = fn_Lin_Reg_Angle_Osc(Close,indLengthB,0,-1);
			LINREG_BIG = fn_Lin_Reg_Angle_Osc(Close,indLengthC,0,-1);
			LINREG_BX = fn_Lin_Reg_Angle_Osc(Close,indLengthD,0,-1);
			LINREG_BXL = fn_Lin_Reg_Angle_Osc(Close,indLengthE,0,-1);

		//scale and normalize features, (indicator results) ********************
		Var: double LINREG_SMALL_NORM(0),double LINREG_MED_NORM(0),double LINREG_BIG_NORM(0),double LINREG_BX_NORM(0),double LINREG_BXL_NORM(0);
			LINREG_SMALL_NORM = NormalizeData(LINREG_SMALL, lookBackLength, scaleLimit, scaleFloor);
			LINREG_MED_NORM = NormalizeData(LINREG_MED, lookBackLength, scaleLimit, scaleFloor);
			LINREG_BIG_NORM = NormalizeData(LINREG_BIG, lookBackLength, scaleLimit, scaleFloor);
			LINREG_BX_NORM = NormalizeData(LINREG_BX, lookBackLength, scaleLimit, scaleFloor);
			LINREG_BXL_NORM = NormalizeData(LINREG_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double LINREGindSMALL (0),double LINREGindMED (0),double LINREGindBIG (0),double LINREGindBX (0),double LINREGindBXL (0);
			If UseBucketData Then Begin
				LINREGindSMALL = BucketData(LINREG_SMALL_NORM,intervalValue,intervalLimit);
				LINREGindMED = BucketData(LINREG_MED_NORM,intervalValue,intervalLimit);
				LINREGindBIG = BucketData(LINREG_BIG_NORM,intervalValue,intervalLimit);
				LINREGindBX = BucketData(LINREG_BX_NORM,intervalValue,intervalLimit);
				LINREGindBXL = BucketData(LINREG_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				LINREGindSMALL = LINREG_SMALL_NORM;
				LINREGindMED = LINREG_MED_NORM;
				LINREGindBIG = LINREG_BIG_NORM;
				LINREGindBX = LINREG_BX_NORM;
				LINREGindBXL = LINREG_BXL_NORM;
	End;
	
	//*****************************************************
	//************ Linear Regression Slope ********************* 
	//*****************************************************
	
	//*****************************************************
	//************ Mass Index ********************* 
	//*****************************************************
	
	//*****************************************************
	//************ Median Price ********************* 
	//*****************************************************
	
	//*****************************************************
	//************ Mov Avg Simple (% Price above/below simple moving avg) *********************
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double MaSMA_SMALL(0), double MaSMA_MED(0), double MaSMA_BIG(0), double MaSMA_BX(0),MaSMA_BXL(0);
			MaSMA_SMALL = fn_Mov_Avg_Simple(Close,indLengthA);
			MaSMA_MED = fn_Mov_Avg_Simple(Close,indLengthB);
			MaSMA_BIG = fn_Mov_Avg_Simple(Close,indLengthC);
			MaSMA_BX = fn_Mov_Avg_Simple(Close,indLengthD);
			MaSMA_BXL = fn_Mov_Avg_Simple(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double MaSMA_SMALL_NORM(0),double MaSMA_MED_NORM(0),double MaSMA_BIG_NORM(0),double MaSMA_BX_NORM(0),double MaSMA_BXL_NORM(0);
			MaSMA_SMALL_NORM = NormalizeData(MaSMA_SMALL, lookBackLength, scaleLimit, scaleFloor);
			MaSMA_MED_NORM = NormalizeData(MaSMA_MED, lookBackLength, scaleLimit, scaleFloor);
			MaSMA_BIG_NORM = NormalizeData(MaSMA_BIG, lookBackLength, scaleLimit, scaleFloor);
			MaSMA_BX_NORM = NormalizeData(MaSMA_BX, lookBackLength, scaleLimit, scaleFloor);
			MaSMA_BXL_NORM = NormalizeData(MaSMA_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double MaSMAindSMALL (0),double MaSMAindMED (0),double MaSMAindBIG (0),double MaSMAindBX (0),double MaSMAindBXL (0);
			If UseBucketData Then Begin
				MaSMAindSMALL = BucketData(MaSMA_SMALL_NORM,intervalValue,intervalLimit);
				MaSMAindMED = BucketData(MaSMA_MED_NORM,intervalValue,intervalLimit);
				MaSMAindBIG = BucketData(MaSMA_BIG_NORM,intervalValue,intervalLimit);
				MaSMAindBX = BucketData(MaSMA_BX_NORM,intervalValue,intervalLimit);
				MaSMAindBXL = BucketData(MaSMA_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				MaSMAindSMALL = MaSMA_SMALL_NORM;
				MaSMAindMED = MaSMA_MED_NORM;
				MaSMAindBIG = MaSMA_BIG_NORM;
				MaSMAindBX = MaSMA_BX_NORM;
				MaSMAindBXL = MaSMA_BXL_NORM;
	End;
	
	//*****************************************************
	//************ MESA Adaptive Moving Average (MAMA) ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double MaMAMA_SMALL(0), double MaMAMA_MED(0), double MaMAMA_BIG(0), double MaMAMA_BX(0),MaMAMA_BXL(0);
			MaMAMA_SMALL = fn_Mov_MAMA((High+Low)/2,indLengthA*.1,indLengthA*.1);
			MaMAMA_MED = fn_Mov_MAMA((High+Low)/2,indLengthB*.1,indLengthA*.1);
			MaMAMA_BIG = fn_Mov_MAMA((High+Low)/2,indLengthC*.1,indLengthA*.1);
			MaMAMA_BX = fn_Mov_MAMA((High+Low)/2,indLengthD*.1,indLengthA*.1);
			MaMAMA_BXL = fn_Mov_MAMA((High+Low)/2,indLengthE*.1,indLengthA*.1);

		//scale and normalize features, (indicator results) ********************
		Var: double MaMAMA_SMALL_NORM(0),double MaMAMA_MED_NORM(0),double MaMAMA_BIG_NORM(0),double MaMAMA_BX_NORM(0),double MaMAMA_BXL_NORM(0);
			MaMAMA_SMALL_NORM = NormalizeData(MaMAMA_SMALL, lookBackLength, scaleLimit, scaleFloor);
			MaMAMA_MED_NORM = NormalizeData(MaMAMA_MED, lookBackLength, scaleLimit, scaleFloor);
			MaMAMA_BIG_NORM = NormalizeData(MaMAMA_BIG, lookBackLength, scaleLimit, scaleFloor);
			MaMAMA_BX_NORM = NormalizeData(MaMAMA_BX, lookBackLength, scaleLimit, scaleFloor);
			MaMAMA_BXL_NORM = NormalizeData(MaMAMA_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double MaMAMAindSMALL (0),double MaMAMAindMED (0),double MaMAMAindBIG (0),double MaMAMAindBX (0),double MaMAMAindBXL (0);
			If UseBucketData Then Begin
				MaMAMAindSMALL = BucketData(MaMAMA_SMALL_NORM,intervalValue,intervalLimit);
				MaMAMAindMED = BucketData(MaMAMA_MED_NORM,intervalValue,intervalLimit);
				MaMAMAindBIG = BucketData(MaMAMA_BIG_NORM,intervalValue,intervalLimit);
				MaMAMAindBX = BucketData(MaMAMA_BX_NORM,intervalValue,intervalLimit);
				MaMAMAindBXL = BucketData(MaMAMA_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				MaMAMAindSMALL = MaMAMA_SMALL_NORM;
				MaMAMAindMED = MaMAMA_MED_NORM;
				MaMAMAindBIG = MaMAMA_BIG_NORM;
				MaMAMAindBX = MaMAMA_BX_NORM;
				MaMAMAindBXL = MaMAMA_BXL_NORM;
	End;


	//*****************************************************
	//************ Modified Moving Average (MMA) ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double MaMMA_SMALL(0), double MaMMA_MED(0), double MaMMA_BIG(0), double MaMMA_BX(0),MaMMA_BXL(0);
			MaMMA_SMALL = fn_MMA(Close,indLengthA);
			MaMMA_MED = fn_MMA(Close,indLengthB);
			MaMMA_BIG = fn_MMA(Close,indLengthC);
			MaMMA_BX = fn_MMA(Close,indLengthD);
			MaMMA_BXL = fn_MMA(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double MaMMA_SMALL_NORM(0),double MaMMA_MED_NORM(0),double MaMMA_BIG_NORM(0),double MaMMA_BX_NORM(0),double MaMMA_BXL_NORM(0);
			MaMMA_SMALL_NORM = NormalizeData(MaMMA_SMALL, lookBackLength, scaleLimit, scaleFloor);
			MaMMA_MED_NORM = NormalizeData(MaMMA_MED, lookBackLength, scaleLimit, scaleFloor);
			MaMMA_BIG_NORM = NormalizeData(MaMMA_BIG, lookBackLength, scaleLimit, scaleFloor);
			MaMMA_BX_NORM = NormalizeData(MaMMA_BX, lookBackLength, scaleLimit, scaleFloor);
			MaMMA_BXL_NORM = NormalizeData(MaMMA_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double MaMMAindSMALL (0),double MaMMAindMED (0),double MaMMAindBIG (0),double MaMMAindBX (0),double MaMMAindBXL (0);
			If UseBucketData Then Begin
				MaMMAindSMALL = BucketData(MaMMA_SMALL_NORM,intervalValue,intervalLimit);
				MaMMAindMED = BucketData(MaMMA_MED_NORM,intervalValue,intervalLimit);
				MaMMAindBIG = BucketData(MaMMA_BIG_NORM,intervalValue,intervalLimit);
				MaMMAindBX = BucketData(MaMMA_BX_NORM,intervalValue,intervalLimit);
				MaMMAindBXL = BucketData(MaMMA_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				MaMMAindSMALL = MaMMA_SMALL_NORM;
				MaMMAindMED = MaMMA_MED_NORM;
				MaMMAindBIG = MaMMA_BIG_NORM;
				MaMMAindBX = MaMMA_BX_NORM;
				MaMMAindBXL = MaMMA_BXL_NORM;
	End;

	//*****************************************************
	//************ Momentum ********************* 
	//*****************************************************
	
	//*****************************************************
	//************ Mov Avg Adaptive ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double MaAMA_SMALL(0), double MaAMA_MED(0), double MaAMA_BIG(0), double MaAMA_BX(0),MaAMA_BXL(0);
			MaAMA_SMALL = fn_Mov_AMA(Close,indLengthA,indLengthB,indLengthC);
			MaAMA_MED = fn_Mov_AMA(Close,indLengthB,indLengthB,indLengthC);
			MaAMA_BIG = fn_Mov_AMA(Close,indLengthC,indLengthB,indLengthC);
			MaAMA_BX = fn_Mov_AMA(Close,indLengthD,indLengthB,indLengthC);
			MaAMA_BXL = fn_Mov_AMA(Close,indLengthE,indLengthB,indLengthC);

		//scale and normalize features, (indicator results) ********************
		Var: double MaAMA_SMALL_NORM(0),double MaAMA_MED_NORM(0),double MaAMA_BIG_NORM(0),double MaAMA_BX_NORM(0),double MaAMA_BXL_NORM(0);
			MaAMA_SMALL_NORM = NormalizeData(MaAMA_SMALL, lookBackLength, scaleLimit, scaleFloor);
			MaAMA_MED_NORM = NormalizeData(MaAMA_MED, lookBackLength, scaleLimit, scaleFloor);
			MaAMA_BIG_NORM = NormalizeData(MaAMA_BIG, lookBackLength, scaleLimit, scaleFloor);
			MaAMA_BX_NORM = NormalizeData(MaAMA_BX, lookBackLength, scaleLimit, scaleFloor);
			MaAMA_BXL_NORM = NormalizeData(MaAMA_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double MaAMAindSMALL (0),double MaAMAindMED (0),double MaAMAindBIG (0),double MaAMAindBX (0),double MaAMAindBXL (0);
			If UseBucketData Then Begin
				MaAMAindSMALL = BucketData(MaAMA_SMALL_NORM,intervalValue,intervalLimit);
				MaAMAindMED = BucketData(MaAMA_MED_NORM,intervalValue,intervalLimit);
				MaAMAindBIG = BucketData(MaAMA_BIG_NORM,intervalValue,intervalLimit);
				MaAMAindBX = BucketData(MaAMA_BX_NORM,intervalValue,intervalLimit);
				MaAMAindBXL = BucketData(MaAMA_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				MaAMAindSMALL = MaAMA_SMALL_NORM;
				MaAMAindMED = MaAMA_MED_NORM;
				MaAMAindBIG = MaAMA_BIG_NORM;
				MaAMAindBX = MaAMA_BX_NORM;
				MaAMAindBXL = MaAMA_BXL_NORM;
	End;
	
	//*****************************************************
	//************ Mov Avg Exponential *********************
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double MaEMA_SMALL(0), double MaEMA_MED(0), double MaEMA_BIG(0), double MaEMA_BX(0),MaEMA_BXL(0);
			MaEMA_SMALL = fn_Mov_Exp(Close,indLengthA);
			MaEMA_MED = fn_Mov_Exp(Close,indLengthB);
			MaEMA_BIG = fn_Mov_Exp(Close,indLengthC);
			MaEMA_BX = fn_Mov_Exp(Close,indLengthD);
			MaEMA_BXL = fn_Mov_Exp(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double MaEMA_SMALL_NORM(0),double MaEMA_MED_NORM(0),double MaEMA_BIG_NORM(0),double MaEMA_BX_NORM(0),double MaEMA_BXL_NORM(0);
			MaEMA_SMALL_NORM = NormalizeData(MaEMA_SMALL, lookBackLength, scaleLimit, scaleFloor);
			MaEMA_MED_NORM = NormalizeData(MaEMA_MED, lookBackLength, scaleLimit, scaleFloor);
			MaEMA_BIG_NORM = NormalizeData(MaEMA_BIG, lookBackLength, scaleLimit, scaleFloor);
			MaEMA_BX_NORM = NormalizeData(MaEMA_BX, lookBackLength, scaleLimit, scaleFloor);
			MaEMA_BXL_NORM = NormalizeData(MaEMA_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double MaEMAindSMALL (0),double MaEMAindMED (0),double MaEMAindBIG (0),double MaEMAindBX (0),double MaEMAindBXL (0);
			If UseBucketData Then Begin
				MaEMAindSMALL = BucketData(MaEMA_SMALL_NORM,intervalValue,intervalLimit);
				MaEMAindMED = BucketData(MaEMA_MED_NORM,intervalValue,intervalLimit);
				MaEMAindBIG = BucketData(MaEMA_BIG_NORM,intervalValue,intervalLimit);
				MaEMAindBX = BucketData(MaEMA_BX_NORM,intervalValue,intervalLimit);
				MaEMAindBXL = BucketData(MaEMA_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				MaEMAindSMALL = MaEMA_SMALL_NORM;
				MaEMAindMED = MaEMA_MED_NORM;
				MaEMAindBIG = MaEMA_BIG_NORM;
				MaEMAindBX = MaEMA_BX_NORM;
				MaEMAindBXL = MaEMA_BXL_NORM;
	End;

	//*****************************************************
	//************ Mov Avg Hull (HMA) ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double MaHMA_SMALL(0), double MaHMA_MED(0), double MaHMA_BIG(0), double MaHMA_BX(0),MaHMA_BXL(0);
			MaHMA_SMALL = fn_Mov_HULL(Close,indLengthA);
			MaHMA_MED = fn_Mov_HULL(Close,indLengthB);
			MaHMA_BIG = fn_Mov_HULL(Close,indLengthC);
			MaHMA_BX = fn_Mov_HULL(Close,indLengthD);
			MaHMA_BXL = fn_Mov_HULL(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double MaHMA_SMALL_NORM(0),double MaHMA_MED_NORM(0),double MaHMA_BIG_NORM(0),double MaHMA_BX_NORM(0),double MaHMA_BXL_NORM(0);
			MaHMA_SMALL_NORM = NormalizeData(MaHMA_SMALL, lookBackLength, scaleLimit, scaleFloor);
			MaHMA_MED_NORM = NormalizeData(MaHMA_MED, lookBackLength, scaleLimit, scaleFloor);
			MaHMA_BIG_NORM = NormalizeData(MaHMA_BIG, lookBackLength, scaleLimit, scaleFloor);
			MaHMA_BX_NORM = NormalizeData(MaHMA_BX, lookBackLength, scaleLimit, scaleFloor);
			MaHMA_BXL_NORM = NormalizeData(MaHMA_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double MaHMAindSMALL (0),double MaHMAindMED (0),double MaHMAindBIG (0),double MaHMAindBX (0),double MaHMAindBXL (0);
			If UseBucketData Then Begin
				MaHMAindSMALL = BucketData(MaHMA_SMALL_NORM,intervalValue,intervalLimit);
				MaHMAindMED = BucketData(MaHMA_MED_NORM,intervalValue,intervalLimit);
				MaHMAindBIG = BucketData(MaHMA_BIG_NORM,intervalValue,intervalLimit);
				MaHMAindBX = BucketData(MaHMA_BX_NORM,intervalValue,intervalLimit);
				MaHMAindBXL = BucketData(MaHMA_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				MaHMAindSMALL = MaHMA_SMALL_NORM;
				MaHMAindMED = MaHMA_MED_NORM;
				MaHMAindBIG = MaHMA_BIG_NORM;
				MaHMAindBX = MaHMA_BX_NORM;
				MaHMAindBXL = MaHMA_BXL_NORM;
	End;


	//*****************************************************
	//************ Mov Avg Triangular ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double MaTRI_SMALL(0), double MaTRI_MED(0), double MaTRI_BIG(0), double MaTRI_BX(0),MaTRI_BXL(0);
			MaTRI_SMALL = fn_Mov_Tri(Close,indLengthA);
			MaTRI_MED = fn_Mov_Tri(Close,indLengthB);
			MaTRI_BIG = fn_Mov_Tri(Close,indLengthC);
			MaTRI_BX = fn_Mov_Tri(Close,indLengthD);
			MaTRI_BXL = fn_Mov_Tri(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double MaTRI_SMALL_NORM(0),double MaTRI_MED_NORM(0),double MaTRI_BIG_NORM(0),double MaTRI_BX_NORM(0),double MaTRI_BXL_NORM(0);
			MaTRI_SMALL_NORM = NormalizeData(MaTRI_SMALL, lookBackLength, scaleLimit, scaleFloor);
			MaTRI_MED_NORM = NormalizeData(MaTRI_MED, lookBackLength, scaleLimit, scaleFloor);
			MaTRI_BIG_NORM = NormalizeData(MaTRI_BIG, lookBackLength, scaleLimit, scaleFloor);
			MaTRI_BX_NORM = NormalizeData(MaTRI_BX, lookBackLength, scaleLimit, scaleFloor);
			MaTRI_BXL_NORM = NormalizeData(MaTRI_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double MaTRIindSMALL (0),double MaTRIindMED (0),double MaTRIindBIG (0),double MaTRIindBX (0),double MaTRIindBXL (0);
			If UseBucketData Then Begin
				MaTRIindSMALL = BucketData(MaTRI_SMALL_NORM,intervalValue,intervalLimit);
				MaTRIindMED = BucketData(MaTRI_MED_NORM,intervalValue,intervalLimit);
				MaTRIindBIG = BucketData(MaTRI_BIG_NORM,intervalValue,intervalLimit);
				MaTRIindBX = BucketData(MaTRI_BX_NORM,intervalValue,intervalLimit);
				MaTRIindBXL = BucketData(MaTRI_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				MaTRIindSMALL = MaTRI_SMALL_NORM;
				MaTRIindMED = MaTRI_MED_NORM;
				MaTRIindBIG = MaTRI_BIG_NORM;
				MaTRIindBX = MaTRI_BX_NORM;
				MaTRIindBXL = MaTRI_BXL_NORM;
	End;

	//*****************************************************
	//************ Mov Avg Double Exponential Moving Average (DEMA) ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double MaDEMA_SMALL(0), double MaDEMA_MED(0), double MaDEMA_BIG(0), double MaDEMA_BX(0),MaDEMA_BXL(0);
			MaDEMA_SMALL = fn_Mov_DEMA(Close,indLengthA);
			MaDEMA_MED = fn_Mov_DEMA(Close,indLengthB);
			MaDEMA_BIG = fn_Mov_DEMA(Close,indLengthC);
			MaDEMA_BX = fn_Mov_DEMA(Close,indLengthD);
			MaDEMA_BXL = fn_Mov_DEMA(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double MaDEMA_SMALL_NORM(0),double MaDEMA_MED_NORM(0),double MaDEMA_BIG_NORM(0),double MaDEMA_BX_NORM(0),double MaDEMA_BXL_NORM(0);
			MaDEMA_SMALL_NORM = NormalizeData(MaDEMA_SMALL, lookBackLength, scaleLimit, scaleFloor);
			MaDEMA_MED_NORM = NormalizeData(MaDEMA_MED, lookBackLength, scaleLimit, scaleFloor);
			MaDEMA_BIG_NORM = NormalizeData(MaDEMA_BIG, lookBackLength, scaleLimit, scaleFloor);
			MaDEMA_BX_NORM = NormalizeData(MaDEMA_BX, lookBackLength, scaleLimit, scaleFloor);
			MaDEMA_BXL_NORM = NormalizeData(MaDEMA_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double MaDEMAindSMALL (0),double MaDEMAindMED (0),double MaDEMAindBIG (0),double MaDEMAindBX (0),double MaDEMAindBXL (0);
			If UseBucketData Then Begin
				MaDEMAindSMALL = BucketData(MaDEMA_SMALL_NORM,intervalValue,intervalLimit);
				MaDEMAindMED = BucketData(MaDEMA_MED_NORM,intervalValue,intervalLimit);
				MaDEMAindBIG = BucketData(MaDEMA_BIG_NORM,intervalValue,intervalLimit);
				MaDEMAindBX = BucketData(MaDEMA_BX_NORM,intervalValue,intervalLimit);
				MaDEMAindBXL = BucketData(MaDEMA_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				MaDEMAindSMALL = MaDEMA_SMALL_NORM;
				MaDEMAindMED = MaDEMA_MED_NORM;
				MaDEMAindBIG = MaDEMA_BIG_NORM;
				MaDEMAindBX = MaDEMA_BX_NORM;
				MaDEMAindBXL = MaDEMA_BXL_NORM;
	End;
	
	//*****************************************************
	//************ Mov Avg Triple Exponential Moving Average (TEMA) ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double MaTEMA_SMALL(0), double MaTEMA_MED(0), double MaTEMA_BIG(0), double MaTEMA_BX(0),MaTEMA_BXL(0);
			MaTEMA_SMALL = fn_Mov_TEMA(Close,indLengthA);
			MaTEMA_MED = fn_Mov_TEMA(Close,indLengthB);
			MaTEMA_BIG = fn_Mov_TEMA(Close,indLengthC);
			MaTEMA_BX = fn_Mov_TEMA(Close,indLengthD);
			MaTEMA_BXL = fn_Mov_TEMA(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double MaTEMA_SMALL_NORM(0),double MaTEMA_MED_NORM(0),double MaTEMA_BIG_NORM(0),double MaTEMA_BX_NORM(0),double MaTEMA_BXL_NORM(0);
			MaTEMA_SMALL_NORM = NormalizeData(MaTEMA_SMALL, lookBackLength, scaleLimit, scaleFloor);
			MaTEMA_MED_NORM = NormalizeData(MaTEMA_MED, lookBackLength, scaleLimit, scaleFloor);
			MaTEMA_BIG_NORM = NormalizeData(MaTEMA_BIG, lookBackLength, scaleLimit, scaleFloor);
			MaTEMA_BX_NORM = NormalizeData(MaTEMA_BX, lookBackLength, scaleLimit, scaleFloor);
			MaTEMA_BXL_NORM = NormalizeData(MaTEMA_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double MaTEMAindSMALL (0),double MaTEMAindMED (0),double MaTEMAindBIG (0),double MaTEMAindBX (0),double MaTEMAindBXL (0);
			If UseBucketData Then Begin
				MaTEMAindSMALL = BucketData(MaTEMA_SMALL_NORM,intervalValue,intervalLimit);
				MaTEMAindMED = BucketData(MaTEMA_MED_NORM,intervalValue,intervalLimit);
				MaTEMAindBIG = BucketData(MaTEMA_BIG_NORM,intervalValue,intervalLimit);
				MaTEMAindBX = BucketData(MaTEMA_BX_NORM,intervalValue,intervalLimit);
				MaTEMAindBXL = BucketData(MaTEMA_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				MaTEMAindSMALL = MaTEMA_SMALL_NORM;
				MaTEMAindMED = MaTEMA_MED_NORM;
				MaTEMAindBIG = MaTEMA_BIG_NORM;
				MaTEMAindBX = MaTEMA_BX_NORM;
				MaTEMAindBXL = MaTEMA_BXL_NORM;
	End;

	//*****************************************************
	//************ Empirical_Mode_Decomposition ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double EMD_SMALL(0), double EMD_MED(0), double EMD_BIG(0), double EMD_BX(0),EMD_BXL(0);
			EMD_SMALL = fn_Empirical_Mode_Decomposition((0.5 * ( High + Low )),indLengthA,indLengthB,0.5,0.5);
			EMD_MED = fn_Empirical_Mode_Decomposition((0.5 * ( High + Low )),indLengthB,indLengthC,0.5,0.5);
			EMD_BIG = fn_Empirical_Mode_Decomposition((0.5 * ( High + Low )),indLengthC,indLengthD,0.5,0.5);
			EMD_BX = fn_Empirical_Mode_Decomposition((0.5 * ( High + Low )),indLengthD,indLengthE,0.5,0.5);
			EMD_BXL = fn_Empirical_Mode_Decomposition((0.5 * ( High + Low )),indLengthE,indLengthC+indLengthD,0.5,0.5);

		//scale and normalize features, (indicator results) ********************
		Var: double EMD_SMALL_NORM(0),double EMD_MED_NORM(0),double EMD_BIG_NORM(0),double EMD_BX_NORM(0),double EMD_BXL_NORM(0);
			EMD_SMALL_NORM = NormalizeData(EMD_SMALL, lookBackLength, scaleLimit, scaleFloor);
			EMD_MED_NORM = NormalizeData(EMD_MED, lookBackLength, scaleLimit, scaleFloor);
			EMD_BIG_NORM = NormalizeData(EMD_BIG, lookBackLength, scaleLimit, scaleFloor);
			EMD_BX_NORM = NormalizeData(EMD_BX, lookBackLength, scaleLimit, scaleFloor);
			EMD_BXL_NORM = NormalizeData(EMD_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double EMDindSMALL (0),double EMDindMED (0),double EMDindBIG (0),double EMDindBX (0),double EMDindBXL (0);
			If UseBucketData Then Begin
				EMDindSMALL = BucketData(EMD_SMALL_NORM,intervalValue,intervalLimit);
				EMDindMED = BucketData(EMD_MED_NORM,intervalValue,intervalLimit);
				EMDindBIG = BucketData(EMD_BIG_NORM,intervalValue,intervalLimit);
				EMDindBX = BucketData(EMD_BX_NORM,intervalValue,intervalLimit);
				EMDindBXL = BucketData(EMD_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				EMDindSMALL = EMD_SMALL_NORM;
				EMDindMED = EMD_MED_NORM;
				EMDindBIG = EMD_BIG_NORM;
				EMDindBX = EMD_BX_NORM;
				EMDindBXL = EMD_BXL_NORM;
	End;

	//*****************************************************
	//************ Mov Avg_Endpoint Moving Average (EPMA) ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double MaEPMA_SMALL(0), double MaEPMA_MED(0), double MaEPMA_BIG(0), double MaEPMA_BX(0),MaEPMA_BXL(0);
			MaEPMA_SMALL = fn_Mov_EPMA(Close,indLengthA);
			MaEPMA_MED = fn_Mov_EPMA(Close,indLengthB);
			MaEPMA_BIG = fn_Mov_EPMA(Close,indLengthC);
			MaEPMA_BX = fn_Mov_EPMA(Close,indLengthD);
			MaEPMA_BXL = fn_Mov_EPMA(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double MaEPMA_SMALL_NORM(0),double MaEPMA_MED_NORM(0),double MaEPMA_BIG_NORM(0),double MaEPMA_BX_NORM(0),double MaEPMA_BXL_NORM(0);
			MaEPMA_SMALL_NORM = NormalizeData(MaEPMA_SMALL, lookBackLength, scaleLimit, scaleFloor);
			MaEPMA_MED_NORM = NormalizeData(MaEPMA_MED, lookBackLength, scaleLimit, scaleFloor);
			MaEPMA_BIG_NORM = NormalizeData(MaEPMA_BIG, lookBackLength, scaleLimit, scaleFloor);
			MaEPMA_BX_NORM = NormalizeData(MaEPMA_BX, lookBackLength, scaleLimit, scaleFloor);
			MaEPMA_BXL_NORM = NormalizeData(MaEPMA_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double MaEPMAindSMALL (0),double MaEPMAindMED (0),double MaEPMAindBIG (0),double MaEPMAindBX (0),double MaEPMAindBXL (0);
			If UseBucketData Then Begin
				MaEPMAindSMALL = BucketData(MaEPMA_SMALL_NORM,intervalValue,intervalLimit);
				MaEPMAindMED = BucketData(MaEPMA_MED_NORM,intervalValue,intervalLimit);
				MaEPMAindBIG = BucketData(MaEPMA_BIG_NORM,intervalValue,intervalLimit);
				MaEPMAindBX = BucketData(MaEPMA_BX_NORM,intervalValue,intervalLimit);
				MaEPMAindBXL = BucketData(MaEPMA_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				MaEPMAindSMALL = MaEPMA_SMALL_NORM;
				MaEPMAindMED = MaEPMA_MED_NORM;
				MaEPMAindBIG = MaEPMA_BIG_NORM;
				MaEPMAindBX = MaEPMA_BX_NORM;
				MaEPMAindBXL = MaEPMA_BXL_NORM;
	End;
	
	//*****************************************************
	//************ Mov Avg Fractal Adaptive Moving Average (FRAMA) ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double FRAMA_SMALL(0), double FRAMA_MED(0), double FRAMA_BIG(0), double FRAMA_BX(0),FRAMA_BXL(0);
			FRAMA_SMALL = fn_Mov_FRAMA((High+Low)/2,indLengthA);
			FRAMA_MED = fn_Mov_FRAMA((High+Low)/2,indLengthB);
			FRAMA_BIG = fn_Mov_FRAMA((High+Low)/2,indLengthC);
			FRAMA_BX = fn_Mov_FRAMA((High+Low)/2,indLengthD);
			FRAMA_BXL = fn_Mov_FRAMA((High+Low)/2,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double FRAMA_SMALL_NORM(0),double FRAMA_MED_NORM(0),double FRAMA_BIG_NORM(0),double FRAMA_BX_NORM(0),double FRAMA_BXL_NORM(0);
			FRAMA_SMALL_NORM = NormalizeData(FRAMA_SMALL, lookBackLength, scaleLimit, scaleFloor);
			FRAMA_MED_NORM = NormalizeData(FRAMA_MED, lookBackLength, scaleLimit, scaleFloor);
			FRAMA_BIG_NORM = NormalizeData(FRAMA_BIG, lookBackLength, scaleLimit, scaleFloor);
			FRAMA_BX_NORM = NormalizeData(FRAMA_BX, lookBackLength, scaleLimit, scaleFloor);
			FRAMA_BXL_NORM = NormalizeData(FRAMA_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double FRAMAindSMALL (0),double FRAMAindMED (0),double FRAMAindBIG (0),double FRAMAindBX (0),double FRAMAindBXL (0);
			If UseBucketData Then Begin
				FRAMAindSMALL = BucketData(FRAMA_SMALL_NORM,intervalValue,intervalLimit);
				FRAMAindMED = BucketData(FRAMA_MED_NORM,intervalValue,intervalLimit);
				FRAMAindBIG = BucketData(FRAMA_BIG_NORM,intervalValue,intervalLimit);
				FRAMAindBX = BucketData(FRAMA_BX_NORM,intervalValue,intervalLimit);
				FRAMAindBXL = BucketData(FRAMA_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				FRAMAindSMALL = FRAMA_SMALL_NORM;
				FRAMAindMED = FRAMA_MED_NORM;
				FRAMAindBIG = FRAMA_BIG_NORM;
				FRAMAindBX = FRAMA_BX_NORM;
				FRAMAindBXL = FRAMA_BXL_NORM;
	End;
	
	
	
	//*****************************************************
	//************ Mov Avg Variable Index Dynamic Average (VIDYA) ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double VIDYA_SMALL(0), double VIDYA_MED(0), double VIDYA_BIG(0), double VIDYA_BX(0),VIDYA_BXL(0);
			VIDYA_SMALL = fn_VIDYA(Close,indLengthA,indLengthB);
			VIDYA_MED = fn_VIDYA(Close,indLengthB,indLengthC);
			VIDYA_BIG = fn_VIDYA(Close,indLengthC,indLengthD);
			VIDYA_BX = fn_VIDYA(Close,indLengthD,indLengthE);
			VIDYA_BXL = fn_VIDYA(Close,indLengthE,indLengthC+indLengthD);

		//scale and normalize features, (indicator results) ********************
		Var: double VIDYA_SMALL_NORM(0),double VIDYA_MED_NORM(0),double VIDYA_BIG_NORM(0),double VIDYA_BX_NORM(0),double VIDYA_BXL_NORM(0);
			VIDYA_SMALL_NORM = NormalizeData(VIDYA_SMALL, lookBackLength, scaleLimit, scaleFloor);
			VIDYA_MED_NORM = NormalizeData(VIDYA_MED, lookBackLength, scaleLimit, scaleFloor);
			VIDYA_BIG_NORM = NormalizeData(VIDYA_BIG, lookBackLength, scaleLimit, scaleFloor);
			VIDYA_BX_NORM = NormalizeData(VIDYA_BX, lookBackLength, scaleLimit, scaleFloor);
			VIDYA_BXL_NORM = NormalizeData(VIDYA_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double VIDYAindSMALL (0),double VIDYAindMED (0),double VIDYAindBIG (0),double VIDYAindBX (0),double VIDYAindBXL (0);
			If UseBucketData Then Begin
				VIDYAindSMALL = BucketData(VIDYA_SMALL_NORM,intervalValue,intervalLimit);
				VIDYAindMED = BucketData(VIDYA_MED_NORM,intervalValue,intervalLimit);
				VIDYAindBIG = BucketData(VIDYA_BIG_NORM,intervalValue,intervalLimit);
				VIDYAindBX = BucketData(VIDYA_BX_NORM,intervalValue,intervalLimit);
				VIDYAindBXL = BucketData(VIDYA_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				VIDYAindSMALL = VIDYA_SMALL_NORM;
				VIDYAindMED = VIDYA_MED_NORM;
				VIDYAindBIG = VIDYA_BIG_NORM;
				VIDYAindBX = VIDYA_BX_NORM;
				VIDYAindBXL = VIDYA_BXL_NORM;
	End;

	//*****************************************************
	//************ Mov Avg Variable Interval Moving Average (VIMA) ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double VIMA_SMALL(0), double VIMA_MED(0), double VIMA_BIG(0), double VIMA_BX(0),VIMA_BXL(0);
			VIMA_SMALL = fn_Mov_VIMA(Close,indLengthA,indLengthB,indLengthA,indLengthB);
			VIMA_MED = fn_Mov_VIMA(Close,indLengthB,indLengthC,indLengthA,indLengthC);
			VIMA_BIG = fn_Mov_VIMA(Close,indLengthC,indLengthD,indLengthA,indLengthD);
			VIMA_BX = fn_Mov_VIMA(Close,indLengthD,indLengthE,indLengthA,indLengthE);
			VIMA_BXL = fn_Mov_VIMA(Close,indLengthE,indLengthC+indLengthD,indLengthA,indLengthC+indLengthD);

		//scale and normalize features, (indicator results) ********************
		Var: double VIMA_SMALL_NORM(0),double VIMA_MED_NORM(0),double VIMA_BIG_NORM(0),double VIMA_BX_NORM(0),double VIMA_BXL_NORM(0);
			VIMA_SMALL_NORM = NormalizeData(VIMA_SMALL, lookBackLength, scaleLimit, scaleFloor);
			VIMA_MED_NORM = NormalizeData(VIMA_MED, lookBackLength, scaleLimit, scaleFloor);
			VIMA_BIG_NORM = NormalizeData(VIMA_BIG, lookBackLength, scaleLimit, scaleFloor);
			VIMA_BX_NORM = NormalizeData(VIMA_BX, lookBackLength, scaleLimit, scaleFloor);
			VIMA_BXL_NORM = NormalizeData(VIMA_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double VIMAindSMALL (0),double VIMAindMED (0),double VIMAindBIG (0),double VIMAindBX (0),double VIMAindBXL (0);
			If UseBucketData Then Begin
				VIMAindSMALL = BucketData(VIMA_SMALL_NORM,intervalValue,intervalLimit);
				VIMAindMED = BucketData(VIMA_MED_NORM,intervalValue,intervalLimit);
				VIMAindBIG = BucketData(VIMA_BIG_NORM,intervalValue,intervalLimit);
				VIMAindBX = BucketData(VIMA_BX_NORM,intervalValue,intervalLimit);
				VIMAindBXL = BucketData(VIMA_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				VIMAindSMALL = VIMA_SMALL_NORM;
				VIMAindMED = VIMA_MED_NORM;
				VIMAindBIG = VIMA_BIG_NORM;
				VIMAindBX = VIMA_BX_NORM;
				VIMAindBXL = VIMA_BXL_NORM;
	End;
	
	//************************************************************************
	//************ Mov Avg_Variable Moving Average (VMA) ********************* 
	//************************************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double MaVMA_SMALL(0), double MaVMA_MED(0), double MaVMA_BIG(0), double MaVMA_BX(0),MaVMA_BXL(0);
			MaVMA_SMALL = fn_Mov_VMA(Close,indLengthA,indLengthB);
			MaVMA_MED = fn_Mov_VMA(Close,indLengthB,indLengthC);
			MaVMA_BIG = fn_Mov_VMA(Close,indLengthC,indLengthD);
			MaVMA_BX = fn_Mov_VMA(Close,indLengthD,indLengthE);
			MaVMA_BXL = fn_Mov_VMA(Close,indLengthE,indLengthC+indLengthD);

		//scale and normalize features, (indicator results) ********************
		Var: double MaVMA_SMALL_NORM(0),double MaVMA_MED_NORM(0),double MaVMA_BIG_NORM(0),double MaVMA_BX_NORM(0),double MaVMA_BXL_NORM(0);
			MaVMA_SMALL_NORM = NormalizeData(MaVMA_SMALL, lookBackLength, scaleLimit, scaleFloor);
			MaVMA_MED_NORM = NormalizeData(MaVMA_MED, lookBackLength, scaleLimit, scaleFloor);
			MaVMA_BIG_NORM = NormalizeData(MaVMA_BIG, lookBackLength, scaleLimit, scaleFloor);
			MaVMA_BX_NORM = NormalizeData(MaVMA_BX, lookBackLength, scaleLimit, scaleFloor);
			MaVMA_BXL_NORM = NormalizeData(MaVMA_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double MaVMAindSMALL (0),double MaVMAindMED (0),double MaVMAindBIG (0),double MaVMAindBX (0),double MaVMAindBXL (0);
			If UseBucketData Then Begin
				MaVMAindSMALL = BucketData(MaVMA_SMALL_NORM,intervalValue,intervalLimit);
				MaVMAindMED = BucketData(MaVMA_MED_NORM,intervalValue,intervalLimit);
				MaVMAindBIG = BucketData(MaVMA_BIG_NORM,intervalValue,intervalLimit);
				MaVMAindBX = BucketData(MaVMA_BX_NORM,intervalValue,intervalLimit);
				MaVMAindBXL = BucketData(MaVMA_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				MaVMAindSMALL = MaVMA_SMALL_NORM;
				MaVMAindMED = MaVMA_MED_NORM;
				MaVMAindBIG = MaVMA_BIG_NORM;
				MaVMAindBX = MaVMA_BX_NORM;
				MaVMAindBXL = MaVMA_BXL_NORM;
	End;
	
	//*****************************************************
	//************ Mov Avg_Weighted Moving Average (WMA) ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double MaWMA_SMALL(0), double MaWMA_MED(0), double MaWMA_BIG(0), double MaWMA_BX(0),MaWMA_BXL(0);
			MaWMA_SMALL = fn_Mov_WMA(Close,indLengthA);
			MaWMA_MED = fn_Mov_WMA(Close,indLengthB);
			MaWMA_BIG = fn_Mov_WMA(Close,indLengthC);
			MaWMA_BX = fn_Mov_WMA(Close,indLengthD);
			MaWMA_BXL = fn_Mov_WMA(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double MaWMA_SMALL_NORM(0),double MaWMA_MED_NORM(0),double MaWMA_BIG_NORM(0),double MaWMA_BX_NORM(0),double MaWMA_BXL_NORM(0);
			MaWMA_SMALL_NORM = NormalizeData(MaWMA_SMALL, lookBackLength, scaleLimit, scaleFloor);
			MaWMA_MED_NORM = NormalizeData(MaWMA_MED, lookBackLength, scaleLimit, scaleFloor);
			MaWMA_BIG_NORM = NormalizeData(MaWMA_BIG, lookBackLength, scaleLimit, scaleFloor);
			MaWMA_BX_NORM = NormalizeData(MaWMA_BX, lookBackLength, scaleLimit, scaleFloor);
			MaWMA_BXL_NORM = NormalizeData(MaWMA_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double MaWMAindSMALL (0),double MaWMAindMED (0),double MaWMAindBIG (0),double MaWMAindBX (0),double MaWMAindBXL (0);
			If UseBucketData Then Begin
				MaWMAindSMALL = BucketData(MaWMA_SMALL_NORM,intervalValue,intervalLimit);
				MaWMAindMED = BucketData(MaWMA_MED_NORM,intervalValue,intervalLimit);
				MaWMAindBIG = BucketData(MaWMA_BIG_NORM,intervalValue,intervalLimit);
				MaWMAindBX = BucketData(MaWMA_BX_NORM,intervalValue,intervalLimit);
				MaWMAindBXL = BucketData(MaWMA_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				MaWMAindSMALL = MaWMA_SMALL_NORM;
				MaWMAindMED = MaWMA_MED_NORM;
				MaWMAindBIG = MaWMA_BIG_NORM;
				MaWMAindBX = MaWMA_BX_NORM;
				MaWMAindBXL = MaWMA_BXL_NORM;
	End;
	

	//*****************************************************
	//************ Mov Avg ALMA(Arnaud Legoux Moving Average) ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double MaALMA_SMALL(0), double MaALMA_MED(0), double MaALMA_BIG(0), double MaALMA_BX(0),MaALMA_BXL(0);
			MaALMA_SMALL = fn_Arnaud_Legoux_Mov_Avg(Close,indLengthA,indLengthA,0.85);
			MaALMA_MED = fn_Arnaud_Legoux_Mov_Avg(Close,indLengthB,indLengthA,0.85);
			MaALMA_BIG = fn_Arnaud_Legoux_Mov_Avg(Close,indLengthC,indLengthA,0.85);
			MaALMA_BX = fn_Arnaud_Legoux_Mov_Avg(Close,indLengthD,indLengthA,0.85);
			MaALMA_BXL = fn_Arnaud_Legoux_Mov_Avg(Close,indLengthE,indLengthA,0.85);

		//scale and normalize features, (indicator results) ********************
		Var: double MaALMA_SMALL_NORM(0),double MaALMA_MED_NORM(0),double MaALMA_BIG_NORM(0),double MaALMA_BX_NORM(0),double MaALMA_BXL_NORM(0);
			MaALMA_SMALL_NORM = NormalizeData(MaALMA_SMALL, lookBackLength, scaleLimit, scaleFloor);
			MaALMA_MED_NORM = NormalizeData(MaALMA_MED, lookBackLength, scaleLimit, scaleFloor);
			MaALMA_BIG_NORM = NormalizeData(MaALMA_BIG, lookBackLength, scaleLimit, scaleFloor);
			MaALMA_BX_NORM = NormalizeData(MaALMA_BX, lookBackLength, scaleLimit, scaleFloor);
			MaALMA_BXL_NORM = NormalizeData(MaALMA_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double MaALMAindSMALL (0),double MaALMAindMED (0),double MaALMAindBIG (0),double MaALMAindBX (0),double MaALMAindBXL (0);
			If UseBucketData Then Begin
				MaALMAindSMALL = BucketData(MaALMA_SMALL_NORM,intervalValue,intervalLimit);
				MaALMAindMED = BucketData(MaALMA_MED_NORM,intervalValue,intervalLimit);
				MaALMAindBIG = BucketData(MaALMA_BIG_NORM,intervalValue,intervalLimit);
				MaALMAindBX = BucketData(MaALMA_BX_NORM,intervalValue,intervalLimit);
				MaALMAindBXL = BucketData(MaALMA_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				MaALMAindSMALL = MaALMA_SMALL_NORM;
				MaALMAindMED = MaALMA_MED_NORM;
				MaALMAindBIG = MaALMA_BIG_NORM;
				MaALMAindBX = MaALMA_BX_NORM;
				MaALMAindBXL = MaALMA_BXL_NORM;
	End;


	//*****************************************************
	//************ Mov Avg Nonlinear Laguerre MA ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double MaLGRE_SMALL(0), double MaLGRE_MED(0), double MaLGRE_BIG(0), double MaLGRE_BX(0),MaLGRE_BXL(0);
			MaLGRE_SMALL = fn_Mov_Laguerre((H+L)/2,indLengthA);
			MaLGRE_MED = fn_Mov_Laguerre((H+L)/2,indLengthB);
			MaLGRE_BIG = fn_Mov_Laguerre((H+L)/2,indLengthC);
			MaLGRE_BX = fn_Mov_Laguerre((H+L)/2,indLengthD);
			MaLGRE_BXL = fn_Mov_Laguerre((H+L)/2,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double MaLGRE_SMALL_NORM(0),double MaLGRE_MED_NORM(0),double MaLGRE_BIG_NORM(0),double MaLGRE_BX_NORM(0),double MaLGRE_BXL_NORM(0);
			MaLGRE_SMALL_NORM = NormalizeData(MaLGRE_SMALL, lookBackLength, scaleLimit, scaleFloor);
			MaLGRE_MED_NORM = NormalizeData(MaLGRE_MED, lookBackLength, scaleLimit, scaleFloor);
			MaLGRE_BIG_NORM = NormalizeData(MaLGRE_BIG, lookBackLength, scaleLimit, scaleFloor);
			MaLGRE_BX_NORM = NormalizeData(MaLGRE_BX, lookBackLength, scaleLimit, scaleFloor);
			MaLGRE_BXL_NORM = NormalizeData(MaLGRE_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double MaLGREindSMALL (0),double MaLGREindMED (0),double MaLGREindBIG (0),double MaLGREindBX (0),double MaLGREindBXL (0);
			If UseBucketData Then Begin
				MaLGREindSMALL = BucketData(MaLGRE_SMALL_NORM,intervalValue,intervalLimit);
				MaLGREindMED = BucketData(MaLGRE_MED_NORM,intervalValue,intervalLimit);
				MaLGREindBIG = BucketData(MaLGRE_BIG_NORM,intervalValue,intervalLimit);
				MaLGREindBX = BucketData(MaLGRE_BX_NORM,intervalValue,intervalLimit);
				MaLGREindBXL = BucketData(MaLGRE_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				MaLGREindSMALL = MaLGRE_SMALL_NORM;
				MaLGREindMED = MaLGRE_MED_NORM;
				MaLGREindBIG = MaLGRE_BIG_NORM;
				MaLGREindBX = MaLGRE_BX_NORM;
				MaLGREindBXL = MaLGRE_BXL_NORM;
	End;
	
	//*****************************************************
	//************ Mov Avg_Zero Lag Exponential Moving Average  ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double MaZLAG_SMALL(0), double MaZLAG_MED(0), double MaZLAG_BIG(0), double MaZLAG_BX(0),MaZLAG_BXL(0);
			MaZLAG_SMALL = fn_Mov_Zero_Lag(Close,indLengthA);
			MaZLAG_MED = fn_Mov_Zero_Lag(Close,indLengthB);
			MaZLAG_BIG = fn_Mov_Zero_Lag(Close,indLengthC);
			MaZLAG_BX = fn_Mov_Zero_Lag(Close,indLengthD);
			MaZLAG_BXL = fn_Mov_Zero_Lag(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double MaZLAG_SMALL_NORM(0),double MaZLAG_MED_NORM(0),double MaZLAG_BIG_NORM(0),double MaZLAG_BX_NORM(0),double MaZLAG_BXL_NORM(0);
			MaZLAG_SMALL_NORM = NormalizeData(MaZLAG_SMALL, lookBackLength, scaleLimit, scaleFloor);
			MaZLAG_MED_NORM = NormalizeData(MaZLAG_MED, lookBackLength, scaleLimit, scaleFloor);
			MaZLAG_BIG_NORM = NormalizeData(MaZLAG_BIG, lookBackLength, scaleLimit, scaleFloor);
			MaZLAG_BX_NORM = NormalizeData(MaZLAG_BX, lookBackLength, scaleLimit, scaleFloor);
			MaZLAG_BXL_NORM = NormalizeData(MaZLAG_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double MaZLAGindSMALL (0),double MaZLAGindMED (0),double MaZLAGindBIG (0),double MaZLAGindBX (0),double MaZLAGindBXL (0);
			If UseBucketData Then Begin
				MaZLAGindSMALL = BucketData(MaZLAG_SMALL_NORM,intervalValue,intervalLimit);
				MaZLAGindMED = BucketData(MaZLAG_MED_NORM,intervalValue,intervalLimit);
				MaZLAGindBIG = BucketData(MaZLAG_BIG_NORM,intervalValue,intervalLimit);
				MaZLAGindBX = BucketData(MaZLAG_BX_NORM,intervalValue,intervalLimit);
				MaZLAGindBXL = BucketData(MaZLAG_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				MaZLAGindSMALL = MaZLAG_SMALL_NORM;
				MaZLAGindMED = MaZLAG_MED_NORM;
				MaZLAGindBIG = MaZLAG_BIG_NORM;
				MaZLAGindBX = MaZLAG_BX_NORM;
				MaZLAGindBXL = MaZLAG_BXL_NORM;
	End;
	

	//*****************************************************
	//************ Moving Average Convergence/Divergence (MACD) ********************* 
	//*****************************************************
	

	//*****************************************************
	//************ Percent Change ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double PCHG_SMALL(0), double PCHG_MED(0), double PCHG_BIG(0), double PCHG_BX(0),PCHG_BXL(0);
			PCHG_SMALL = fn_Percent_Change(Close,indLengthA);
			PCHG_MED = fn_Percent_Change(Close,indLengthB);
			PCHG_BIG = fn_Percent_Change(Close,indLengthC);
			PCHG_BX = fn_Percent_Change(Close,indLengthD);
			PCHG_BXL = fn_Percent_Change(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double PCHG_SMALL_NORM(0),double PCHG_MED_NORM(0),double PCHG_BIG_NORM(0),double PCHG_BX_NORM(0),double PCHG_BXL_NORM(0);
			PCHG_SMALL_NORM = NormalizeData(PCHG_SMALL, lookBackLength, scaleLimit, scaleFloor);
			PCHG_MED_NORM = NormalizeData(PCHG_MED, lookBackLength, scaleLimit, scaleFloor);
			PCHG_BIG_NORM = NormalizeData(PCHG_BIG, lookBackLength, scaleLimit, scaleFloor);
			PCHG_BX_NORM = NormalizeData(PCHG_BX, lookBackLength, scaleLimit, scaleFloor);
			PCHG_BXL_NORM = NormalizeData(PCHG_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double PCHGindSMALL (0),double PCHGindMED (0),double PCHGindBIG (0),double PCHGindBX (0),double PCHGindBXL (0);
			If UseBucketData Then Begin
				PCHGindSMALL = BucketData(PCHG_SMALL_NORM,intervalValue,intervalLimit);
				PCHGindMED = BucketData(PCHG_MED_NORM,intervalValue,intervalLimit);
				PCHGindBIG = BucketData(PCHG_BIG_NORM,intervalValue,intervalLimit);
				PCHGindBX = BucketData(PCHG_BX_NORM,intervalValue,intervalLimit);
				PCHGindBXL = BucketData(PCHG_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				PCHGindSMALL = PCHG_SMALL_NORM;
				PCHGindMED = PCHG_MED_NORM;
				PCHGindBIG = PCHG_BIG_NORM;
				PCHGindBX = PCHG_BX_NORM;
				PCHGindBXL = PCHG_BXL_NORM;
	End;
	
	//***************************************************
	//************ Percent of Resistance (PCR) ********************* 
	//***************************************************
	
	//************************************************
	//************ Percent R ********************* 
	//************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double PCTR_SMALL(0), double PCTR_MED(0), double PCTR_BIG(0), double PCTR_BX(0),PCTR_BXL(0);
			PCTR_SMALL = fn_PercentR(Close,High,Low,indLengthA);
			PCTR_MED = fn_PercentR(Close,High,Low,indLengthB);
			PCTR_BIG = fn_PercentR(Close,High,Low,indLengthC);
			PCTR_BX = fn_PercentR(Close,High,Low,indLengthD);
			PCTR_BXL = fn_PercentR(Close,High,Low,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double PCTR_SMALL_NORM(0),double PCTR_MED_NORM(0),double PCTR_BIG_NORM(0),double PCTR_BX_NORM(0),double PCTR_BXL_NORM(0);
			PCTR_SMALL_NORM = NormalizeData(PCTR_SMALL, lookBackLength, scaleLimit, scaleFloor);
			PCTR_MED_NORM = NormalizeData(PCTR_MED, lookBackLength, scaleLimit, scaleFloor);
			PCTR_BIG_NORM = NormalizeData(PCTR_BIG, lookBackLength, scaleLimit, scaleFloor);
			PCTR_BX_NORM = NormalizeData(PCTR_BX, lookBackLength, scaleLimit, scaleFloor);
			PCTR_BXL_NORM = NormalizeData(PCTR_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double PCTRindSMALL (0),double PCTRindMED (0),double PCTRindBIG (0),double PCTRindBX (0),double PCTRindBXL (0);
			If UseBucketData Then Begin
				PCTRindSMALL = BucketData(PCTR_SMALL_NORM,intervalValue,intervalLimit);
				PCTRindMED = BucketData(PCTR_MED_NORM,intervalValue,intervalLimit);
				PCTRindBIG = BucketData(PCTR_BIG_NORM,intervalValue,intervalLimit);
				PCTRindBX = BucketData(PCTR_BX_NORM,intervalValue,intervalLimit);
				PCTRindBXL = BucketData(PCTR_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				PCTRindSMALL = PCTR_SMALL_NORM;
				PCTRindMED = PCTR_MED_NORM;
				PCTRindBIG = PCTR_BIG_NORM;
				PCTRindBX = PCTR_BX_NORM;
				PCTRindBXL = PCTR_BXL_NORM;
	End;	
	
	
	//***************************************************
	//************ Price Channel ********************* 
	//***************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double PCHAN_SMALL(0), double PCHAN_MED(0), double PCHAN_BIG(0), double PCHAN_BX(0),PCHAN_BXL(0);
			PCHAN_SMALL = fn_Price_Channel(Close,High,Low,indLengthA);
			PCHAN_MED = fn_Price_Channel(Close,High,Low,indLengthB);
			PCHAN_BIG = fn_Price_Channel(Close,High,Low,indLengthC);
			PCHAN_BX = fn_Price_Channel(Close,High,Low,indLengthD);
			PCHAN_BXL = fn_Price_Channel(Close,High,Low,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double PCHAN_SMALL_NORM(0),double PCHAN_MED_NORM(0),double PCHAN_BIG_NORM(0),double PCHAN_BX_NORM(0),double PCHAN_BXL_NORM(0);
			PCHAN_SMALL_NORM = NormalizeData(PCHAN_SMALL, lookBackLength, scaleLimit, scaleFloor);
			PCHAN_MED_NORM = NormalizeData(PCHAN_MED, lookBackLength, scaleLimit, scaleFloor);
			PCHAN_BIG_NORM = NormalizeData(PCHAN_BIG, lookBackLength, scaleLimit, scaleFloor);
			PCHAN_BX_NORM = NormalizeData(PCHAN_BX, lookBackLength, scaleLimit, scaleFloor);
			PCHAN_BXL_NORM = NormalizeData(PCHAN_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double PCHANindSMALL (0),double PCHANindMED (0),double PCHANindBIG (0),double PCHANindBX (0),double PCHANindBXL (0);
			If UseBucketData Then Begin
				PCHANindSMALL = BucketData(PCHAN_SMALL_NORM,intervalValue,intervalLimit);
				PCHANindMED = BucketData(PCHAN_MED_NORM,intervalValue,intervalLimit);
				PCHANindBIG = BucketData(PCHAN_BIG_NORM,intervalValue,intervalLimit);
				PCHANindBX = BucketData(PCHAN_BX_NORM,intervalValue,intervalLimit);
				PCHANindBXL = BucketData(PCHAN_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				PCHANindSMALL = PCHAN_SMALL_NORM;
				PCHANindMED = PCHAN_MED_NORM;
				PCHANindBIG = PCHAN_BIG_NORM;
				PCHANindBX = PCHAN_BX_NORM;
				PCHANindBXL = PCHAN_BXL_NORM;
	End;
	
	
	//***************************************************
	//************ Price Oscillator *********************
	//***************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double POSC_SMALL(0), double POSC_MED(0), double POSC_BIG(0), double POSC_BX(0),POSC_BXL(0);
			POSC_SMALL = fn_PriceOscillator(Close,indLengthA,indLengthB);
			POSC_MED = fn_PriceOscillator(Close,indLengthB,indLengthB);
			POSC_BIG = fn_PriceOscillator(Close,indLengthC,indLengthB);
			POSC_BX = fn_PriceOscillator(Close,indLengthD,indLengthB);
			POSC_BXL = fn_PriceOscillator(Close,indLengthE,indLengthB);

		//scale and normalize features, (indicator results) ********************
		Var: double POSC_SMALL_NORM(0),double POSC_MED_NORM(0),double POSC_BIG_NORM(0),double POSC_BX_NORM(0),double POSC_BXL_NORM(0);
			POSC_SMALL_NORM = NormalizeData(POSC_SMALL, lookBackLength, scaleLimit, scaleFloor);
			POSC_MED_NORM = NormalizeData(POSC_MED, lookBackLength, scaleLimit, scaleFloor);
			POSC_BIG_NORM = NormalizeData(POSC_BIG, lookBackLength, scaleLimit, scaleFloor);
			POSC_BX_NORM = NormalizeData(POSC_BX, lookBackLength, scaleLimit, scaleFloor);
			POSC_BXL_NORM = NormalizeData(POSC_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double POSCindSMALL (0),double POSCindMED (0),double POSCindBIG (0),double POSCindBX (0),double POSCindBXL (0);
			If UseBucketData Then Begin
				POSCindSMALL = BucketData(POSC_SMALL_NORM,intervalValue,intervalLimit);
				POSCindMED = BucketData(POSC_MED_NORM,intervalValue,intervalLimit);
				POSCindBIG = BucketData(POSC_BIG_NORM,intervalValue,intervalLimit);
				POSCindBX = BucketData(POSC_BX_NORM,intervalValue,intervalLimit);
				POSCindBXL = BucketData(POSC_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				POSCindSMALL = POSC_SMALL_NORM;
				POSCindMED = POSC_MED_NORM;
				POSCindBIG = POSC_BIG_NORM;
				POSCindBX = POSC_BX_NORM;
				POSCindBXL = POSC_BXL_NORM;
	End;
	
	//***************************************************
	//************ Profitunity Oscillator ********************* 
	//***************************************************
		
	//***************************************************
	//************ Rate of Change ********************* 
	//***************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double ROC_SMALL(0), double ROC_MED(0), double ROC_BIG(0), double ROC_BX(0),ROC_BXL(0);
			ROC_SMALL = fn_RateOfChange(Close,indLengthA);
			ROC_MED = fn_RateOfChange(Close,indLengthB);
			ROC_BIG = fn_RateOfChange(Close,indLengthC);
			ROC_BX = fn_RateOfChange(Close,indLengthD);
			ROC_BXL = fn_RateOfChange(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double ROC_SMALL_NORM(0),double ROC_MED_NORM(0),double ROC_BIG_NORM(0),double ROC_BX_NORM(0),double ROC_BXL_NORM(0);
			ROC_SMALL_NORM = NormalizeData(ROC_SMALL, lookBackLength, scaleLimit, scaleFloor);
			ROC_MED_NORM = NormalizeData(ROC_MED, lookBackLength, scaleLimit, scaleFloor);
			ROC_BIG_NORM = NormalizeData(ROC_BIG, lookBackLength, scaleLimit, scaleFloor);
			ROC_BX_NORM = NormalizeData(ROC_BX, lookBackLength, scaleLimit, scaleFloor);
			ROC_BXL_NORM = NormalizeData(ROC_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double ROCindSMALL (0),double ROCindMED (0),double ROCindBIG (0),double ROCindBX (0),double ROCindBXL (0);
			If UseBucketData Then Begin
				ROCindSMALL = BucketData(ROC_SMALL_NORM,intervalValue,intervalLimit);
				ROCindMED = BucketData(ROC_MED_NORM,intervalValue,intervalLimit);
				ROCindBIG = BucketData(ROC_BIG_NORM,intervalValue,intervalLimit);
				ROCindBX = BucketData(ROC_BX_NORM,intervalValue,intervalLimit);
				ROCindBXL = BucketData(ROC_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				ROCindSMALL = ROC_SMALL_NORM;
				ROCindMED = ROC_MED_NORM;
				ROCindBIG = ROC_BIG_NORM;
				ROCindBX = ROC_BX_NORM;
				ROCindBXL = ROC_BXL_NORM;
	End;


	//****************************************************************
	//************ Relative Strength Index (RSI) ********************* 
	//****************************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double RSI_SMALL(0), double RSI_MED(0), double RSI_BIG(0), double RSI_BX(0),RSI_BXL(0);
			RSI_SMALL = fn_RSI(Close,indLengthA);
			RSI_MED = fn_RSI(Close,indLengthB);
			RSI_BIG = fn_RSI(Close,indLengthC);
			RSI_BX = fn_RSI(Close,indLengthD);
			RSI_BXL = fn_RSI(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double RSI_SMALL_NORM(0),double RSI_MED_NORM(0),double RSI_BIG_NORM(0),double RSI_BX_NORM(0),double RSI_BXL_NORM(0);
			RSI_SMALL_NORM = NormalizeData(RSI_SMALL, lookBackLength, scaleLimit, scaleFloor);
			RSI_MED_NORM = NormalizeData(RSI_MED, lookBackLength, scaleLimit, scaleFloor);
			RSI_BIG_NORM = NormalizeData(RSI_BIG, lookBackLength, scaleLimit, scaleFloor);
			RSI_BX_NORM = NormalizeData(RSI_BX, lookBackLength, scaleLimit, scaleFloor);
			RSI_BXL_NORM = NormalizeData(RSI_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double RSIindSMALL (0),double RSIindMED (0),double RSIindBIG (0),double RSIindBX (0),double RSIindBXL (0);
			If UseBucketData Then Begin
				RSIindSMALL = BucketData(RSI_SMALL_NORM,intervalValue,intervalLimit);
				RSIindMED = BucketData(RSI_MED_NORM,intervalValue,intervalLimit);
				RSIindBIG = BucketData(RSI_BIG_NORM,intervalValue,intervalLimit);
				RSIindBX = BucketData(RSI_BX_NORM,intervalValue,intervalLimit);
				RSIindBXL = BucketData(RSI_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				RSIindSMALL = RSI_SMALL_NORM;
				RSIindMED = RSI_MED_NORM;
				RSIindBIG = RSI_BIG_NORM;
				RSIindBX = RSI_BX_NORM;
				RSIindBXL = RSI_BXL_NORM;
	End;
	
	
	//****************************************************************
	//************* Cummulative RSI **********************************
	//****************************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double CUMRSI_SMALL(0), double CUMRSI_MED(0), double CUMRSI_BIG(0), double CUMRSI_BX(0),CUMRSI_BXL(0);
			CUMRSI_SMALL = fn_Cummulative_RSI(Close,indLengthA);
			CUMRSI_MED = fn_Cummulative_RSI(Close,indLengthB);
			CUMRSI_BIG = fn_Cummulative_RSI(Close,indLengthC);
			CUMRSI_BX = fn_Cummulative_RSI(Close,indLengthD);
			CUMRSI_BXL = fn_Cummulative_RSI(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double CUMRSI_SMALL_NORM(0),double CUMRSI_MED_NORM(0),double CUMRSI_BIG_NORM(0),double CUMRSI_BX_NORM(0),double CUMRSI_BXL_NORM(0);
			CUMRSI_SMALL_NORM = NormalizeData(CUMRSI_SMALL, lookBackLength, scaleLimit, scaleFloor);
			CUMRSI_MED_NORM = NormalizeData(CUMRSI_MED, lookBackLength, scaleLimit, scaleFloor);
			CUMRSI_BIG_NORM = NormalizeData(CUMRSI_BIG, lookBackLength, scaleLimit, scaleFloor);
			CUMRSI_BX_NORM = NormalizeData(CUMRSI_BX, lookBackLength, scaleLimit, scaleFloor);
			CUMRSI_BXL_NORM = NormalizeData(CUMRSI_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double CUMRSIindSMALL (0),double CUMRSIindMED (0),double CUMRSIindBIG (0),double CUMRSIindBX (0),double CUMRSIindBXL (0);
			If UseBucketData Then Begin
				CUMRSIindSMALL = BucketData(CUMRSI_SMALL_NORM,intervalValue,intervalLimit);
				CUMRSIindMED = BucketData(CUMRSI_MED_NORM,intervalValue,intervalLimit);
				CUMRSIindBIG = BucketData(CUMRSI_BIG_NORM,intervalValue,intervalLimit);
				CUMRSIindBX = BucketData(CUMRSI_BX_NORM,intervalValue,intervalLimit);
				CUMRSIindBXL = BucketData(CUMRSI_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				CUMRSIindSMALL = CUMRSI_SMALL_NORM;
				CUMRSIindMED = CUMRSI_MED_NORM;
				CUMRSIindBIG = CUMRSI_BIG_NORM;
				CUMRSIindBX = CUMRSI_BX_NORM;
				CUMRSIindBXL = CUMRSI_BXL_NORM;
	End;
	
	//****************************************************************
	//************* Hull RSI **********************************
	//****************************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double RSIHUL_SMALL(0), double RSIHUL_MED(0), double RSIHUL_BIG(0), double RSIHUL_BX(0),RSIHUL_BXL(0);
			RSIHUL_SMALL = fn_Hull_RSI(Close,indLengthA);
			RSIHUL_MED = fn_Hull_RSI(Close,indLengthB);
			RSIHUL_BIG = fn_Hull_RSI(Close,indLengthC);
			RSIHUL_BX = fn_Hull_RSI(Close,indLengthD);
			RSIHUL_BXL = fn_Hull_RSI(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double RSIHUL_SMALL_NORM(0),double RSIHUL_MED_NORM(0),double RSIHUL_BIG_NORM(0),double RSIHUL_BX_NORM(0),double RSIHUL_BXL_NORM(0);
			RSIHUL_SMALL_NORM = NormalizeData(RSIHUL_SMALL, lookBackLength, scaleLimit, scaleFloor);
			RSIHUL_MED_NORM = NormalizeData(RSIHUL_MED, lookBackLength, scaleLimit, scaleFloor);
			RSIHUL_BIG_NORM = NormalizeData(RSIHUL_BIG, lookBackLength, scaleLimit, scaleFloor);
			RSIHUL_BX_NORM = NormalizeData(RSIHUL_BX, lookBackLength, scaleLimit, scaleFloor);
			RSIHUL_BXL_NORM = NormalizeData(RSIHUL_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double RSIHULindSMALL (0),double RSIHULindMED (0),double RSIHULindBIG (0),double RSIHULindBX (0),double RSIHULindBXL (0);
			If UseBucketData Then Begin
				RSIHULindSMALL = BucketData(RSIHUL_SMALL_NORM,intervalValue,intervalLimit);
				RSIHULindMED = BucketData(RSIHUL_MED_NORM,intervalValue,intervalLimit);
				RSIHULindBIG = BucketData(RSIHUL_BIG_NORM,intervalValue,intervalLimit);
				RSIHULindBX = BucketData(RSIHUL_BX_NORM,intervalValue,intervalLimit);
				RSIHULindBXL = BucketData(RSIHUL_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				RSIHULindSMALL = RSIHUL_SMALL_NORM;
				RSIHULindMED = RSIHUL_MED_NORM;
				RSIHULindBIG = RSIHUL_BIG_NORM;
				RSIHULindBX = RSIHUL_BX_NORM;
				RSIHULindBXL = RSIHUL_BXL_NORM;
	End;

	
	//*****************************************************
	//************ Relative Vigor Index (RVI) ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double RVI_SMALL(0), double RVI_MED(0), double RVI_BIG(0), double RVI_BX(0),RVI_BXL(0);
			RVI_SMALL = fn_RVI(indLengthA);
			RVI_MED = fn_RVI(indLengthB);
			RVI_BIG = fn_RVI(indLengthC);
			RVI_BX = fn_RVI(indLengthD);
			RVI_BXL = fn_RVI(indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double RVI_SMALL_NORM(0),double RVI_MED_NORM(0),double RVI_BIG_NORM(0),double RVI_BX_NORM(0),double RVI_BXL_NORM(0);
			RVI_SMALL_NORM = NormalizeData(RVI_SMALL, lookBackLength, scaleLimit, scaleFloor);
			RVI_MED_NORM = NormalizeData(RVI_MED, lookBackLength, scaleLimit, scaleFloor);
			RVI_BIG_NORM = NormalizeData(RVI_BIG, lookBackLength, scaleLimit, scaleFloor);
			RVI_BX_NORM = NormalizeData(RVI_BX, lookBackLength, scaleLimit, scaleFloor);
			RVI_BXL_NORM = NormalizeData(RVI_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double RVIindSMALL (0),double RVIindMED (0),double RVIindBIG (0),double RVIindBX (0),double RVIindBXL (0);
			If UseBucketData Then Begin
				RVIindSMALL = BucketData(RVI_SMALL_NORM,intervalValue,intervalLimit);
				RVIindMED = BucketData(RVI_MED_NORM,intervalValue,intervalLimit);
				RVIindBIG = BucketData(RVI_BIG_NORM,intervalValue,intervalLimit);
				RVIindBX = BucketData(RVI_BX_NORM,intervalValue,intervalLimit);
				RVIindBXL = BucketData(RVI_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				RVIindSMALL = RVI_SMALL_NORM;
				RVIindMED = RVI_MED_NORM;
				RVIindBIG = RVI_BIG_NORM;
				RVIindBX = RVI_BX_NORM;
				RVIindBXL = RVI_BXL_NORM;
	End;
	
	
	//*****************************************************
	//************ R-Squared indicator ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double RSQR_SMALL(0), double RSQR_MED(0), double RSQR_BIG(0), double RSQR_BX(0),RSQR_BXL(0);
			RSQR_SMALL = fn_RSquare(BarNumber, Close,indLengthA);
			RSQR_MED = fn_RSquare(BarNumber, Close,indLengthB);
			RSQR_BIG = fn_RSquare(BarNumber, Close,indLengthC);
			RSQR_BX = fn_RSquare(BarNumber, Close,indLengthD);
			RSQR_BXL = fn_RSquare(BarNumber, Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double RSQR_SMALL_NORM(0),double RSQR_MED_NORM(0),double RSQR_BIG_NORM(0),double RSQR_BX_NORM(0),double RSQR_BXL_NORM(0);
			RSQR_SMALL_NORM = NormalizeData(RSQR_SMALL, lookBackLength, scaleLimit, scaleFloor);
			RSQR_MED_NORM = NormalizeData(RSQR_MED, lookBackLength, scaleLimit, scaleFloor);
			RSQR_BIG_NORM = NormalizeData(RSQR_BIG, lookBackLength, scaleLimit, scaleFloor);
			RSQR_BX_NORM = NormalizeData(RSQR_BX, lookBackLength, scaleLimit, scaleFloor);
			RSQR_BXL_NORM = NormalizeData(RSQR_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double RSQRindSMALL (0),double RSQRindMED (0),double RSQRindBIG (0),double RSQRindBX (0),double RSQRindBXL (0);
			If UseBucketData Then Begin
				RSQRindSMALL = BucketData(RSQR_SMALL_NORM,intervalValue,intervalLimit);
				RSQRindMED = BucketData(RSQR_MED_NORM,intervalValue,intervalLimit);
				RSQRindBIG = BucketData(RSQR_BIG_NORM,intervalValue,intervalLimit);
				RSQRindBX = BucketData(RSQR_BX_NORM,intervalValue,intervalLimit);
				RSQRindBXL = BucketData(RSQR_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				RSQRindSMALL = RSQR_SMALL_NORM;
				RSQRindMED = RSQR_MED_NORM;
				RSQRindBIG = RSQR_BIG_NORM;
				RSQRindBX = RSQR_BX_NORM;
				RSQRindBXL = RSQR_BXL_NORM;
	End;

	//*****************************************************
	//************ Sine Weighted Moving Average ***********
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double MaSWMA_SMALL(0), double MaSWMA_MED(0), double MaSWMA_BIG(0), double MaSWMA_BX(0),MaSWMA_BXL(0);
			MaSWMA_SMALL = fn_Sine_Weighted_Moving_Avg(Close,indLengthA);
			MaSWMA_MED = fn_Sine_Weighted_Moving_Avg(Close,indLengthB);
			MaSWMA_BIG = fn_Sine_Weighted_Moving_Avg(Close,indLengthC);
			MaSWMA_BX = fn_Sine_Weighted_Moving_Avg(Close,indLengthD);
			MaSWMA_BXL = fn_Sine_Weighted_Moving_Avg(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double MaSWMA_SMALL_NORM(0),double MaSWMA_MED_NORM(0),double MaSWMA_BIG_NORM(0),double MaSWMA_BX_NORM(0),double MaSWMA_BXL_NORM(0);
			MaSWMA_SMALL_NORM = NormalizeData(MaSWMA_SMALL, lookBackLength, scaleLimit, scaleFloor);
			MaSWMA_MED_NORM = NormalizeData(MaSWMA_MED, lookBackLength, scaleLimit, scaleFloor);
			MaSWMA_BIG_NORM = NormalizeData(MaSWMA_BIG, lookBackLength, scaleLimit, scaleFloor);
			MaSWMA_BX_NORM = NormalizeData(MaSWMA_BX, lookBackLength, scaleLimit, scaleFloor);
			MaSWMA_BXL_NORM = NormalizeData(MaSWMA_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double MaSWMAindSMALL (0),double MaSWMAindMED (0),double MaSWMAindBIG (0),double MaSWMAindBX (0),double MaSWMAindBXL (0);
			If UseBucketData Then Begin
				MaSWMAindSMALL = BucketData(MaSWMA_SMALL_NORM,intervalValue,intervalLimit);
				MaSWMAindMED = BucketData(MaSWMA_MED_NORM,intervalValue,intervalLimit);
				MaSWMAindBIG = BucketData(MaSWMA_BIG_NORM,intervalValue,intervalLimit);
				MaSWMAindBX = BucketData(MaSWMA_BX_NORM,intervalValue,intervalLimit);
				MaSWMAindBXL = BucketData(MaSWMA_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				MaSWMAindSMALL = MaSWMA_SMALL_NORM;
				MaSWMAindMED = MaSWMA_MED_NORM;
				MaSWMAindBIG = MaSWMA_BIG_NORM;
				MaSWMAindBX = MaSWMA_BX_NORM;
				MaSWMAindBXL = MaSWMA_BXL_NORM;
	End;


	//*****************************************************
	//************ Standard Deviation ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double STDDEV_SMALL(0), double STDDEV_MED(0), double STDDEV_BIG(0), double STDDEV_BX(0),STDDEV_BXL(0);
			STDDEV_SMALL = fn_StdDev(Close,indLengthA);
			STDDEV_MED = fn_StdDev(Close,indLengthB);
			STDDEV_BIG = fn_StdDev(Close,indLengthC);
			STDDEV_BX = fn_StdDev(Close,indLengthD);
			STDDEV_BXL = fn_StdDev(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double STDDEV_SMALL_NORM(0),double STDDEV_MED_NORM(0),double STDDEV_BIG_NORM(0),double STDDEV_BX_NORM(0),double STDDEV_BXL_NORM(0);
			STDDEV_SMALL_NORM = NormalizeData(STDDEV_SMALL, lookBackLength, scaleLimit, scaleFloor);
			STDDEV_MED_NORM = NormalizeData(STDDEV_MED, lookBackLength, scaleLimit, scaleFloor);
			STDDEV_BIG_NORM = NormalizeData(STDDEV_BIG, lookBackLength, scaleLimit, scaleFloor);
			STDDEV_BX_NORM = NormalizeData(STDDEV_BX, lookBackLength, scaleLimit, scaleFloor);
			STDDEV_BXL_NORM = NormalizeData(STDDEV_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double STDDEVindSMALL (0),double STDDEVindMED (0),double STDDEVindBIG (0),double STDDEVindBX (0),double STDDEVindBXL (0);
			If UseBucketData Then Begin
				STDDEVindSMALL = BucketData(STDDEV_SMALL_NORM,intervalValue,intervalLimit);
				STDDEVindMED = BucketData(STDDEV_MED_NORM,intervalValue,intervalLimit);
				STDDEVindBIG = BucketData(STDDEV_BIG_NORM,intervalValue,intervalLimit);
				STDDEVindBX = BucketData(STDDEV_BX_NORM,intervalValue,intervalLimit);
				STDDEVindBXL = BucketData(STDDEV_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				STDDEVindSMALL = STDDEV_SMALL_NORM;
				STDDEVindMED = STDDEV_MED_NORM;
				STDDEVindBIG = STDDEV_BIG_NORM;
				STDDEVindBX = STDDEV_BX_NORM;
				STDDEVindBXL = STDDEV_BXL_NORM;
	End;
	
	
	//*****************************************************
	//************ Standard Error Bands ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double STDERR_SMALL(0), double STDERR_MED(0), double STDERR_BIG(0), double STDERR_BX(0),STDERR_BXL(0);
			STDERR_SMALL = fn_StdError(Close,indLengthA);
			STDERR_MED = fn_StdError(Close,indLengthB);
			STDERR_BIG = fn_StdError(Close,indLengthC);
			STDERR_BX = fn_StdError(Close,indLengthD);
			STDERR_BXL = fn_StdError(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double STDERR_SMALL_NORM(0),double STDERR_MED_NORM(0),double STDERR_BIG_NORM(0),double STDERR_BX_NORM(0),double STDERR_BXL_NORM(0);
			STDERR_SMALL_NORM = NormalizeData(STDERR_SMALL, lookBackLength, scaleLimit, scaleFloor);
			STDERR_MED_NORM = NormalizeData(STDERR_MED, lookBackLength, scaleLimit, scaleFloor);
			STDERR_BIG_NORM = NormalizeData(STDERR_BIG, lookBackLength, scaleLimit, scaleFloor);
			STDERR_BX_NORM = NormalizeData(STDERR_BX, lookBackLength, scaleLimit, scaleFloor);
			STDERR_BXL_NORM = NormalizeData(STDERR_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double STDERRindSMALL (0),double STDERRindMED (0),double STDERRindBIG (0),double STDERRindBX (0),double STDERRindBXL (0);
			If UseBucketData Then Begin
				STDERRindSMALL = BucketData(STDERR_SMALL_NORM,intervalValue,intervalLimit);
				STDERRindMED = BucketData(STDERR_MED_NORM,intervalValue,intervalLimit);
				STDERRindBIG = BucketData(STDERR_BIG_NORM,intervalValue,intervalLimit);
				STDERRindBX = BucketData(STDERR_BX_NORM,intervalValue,intervalLimit);
				STDERRindBXL = BucketData(STDERR_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				STDERRindSMALL = STDERR_SMALL_NORM;
				STDERRindMED = STDERR_MED_NORM;
				STDERRindBIG = STDERR_BIG_NORM;
				STDERRindBX = STDERR_BX_NORM;
				STDERRindBXL = STDERR_BXL_NORM;
	End;


	//*****************************************************
	//************ Stochastics Fast ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double STOF_SMALL(0), double STOF_MED(0), double STOF_BIG(0), double STOF_BX(0),STOF_BXL(0);
			STOF_SMALL = fn_Stochastics_Fast(Close, High, Low,indLengthA,(indLengthA/2), (indLengthA/2),1);
			STOF_MED = fn_Stochastics_Fast(Close, High, Low,indLengthB,(indLengthB/2), (indLengthB/2),1);
			STOF_BIG = fn_Stochastics_Fast(Close, High, Low,indLengthC,(indLengthC/2), (indLengthC/2),1);
			STOF_BX = fn_Stochastics_Fast(Close, High, Low,indLengthD,(indLengthD/2), (indLengthD/2),1);
			STOF_BXL = fn_Stochastics_Fast(Close, High, Low,indLengthE,(indLengthE/2), (indLengthE/2),1);

		//scale and normalize features, (indicator results) ********************
		Var: double STOF_SMALL_NORM(0),double STOF_MED_NORM(0),double STOF_BIG_NORM(0),double STOF_BX_NORM(0),double STOF_BXL_NORM(0);
			STOF_SMALL_NORM = NormalizeData(STOF_SMALL, lookBackLength, scaleLimit, scaleFloor);
			STOF_MED_NORM = NormalizeData(STOF_MED, lookBackLength, scaleLimit, scaleFloor);
			STOF_BIG_NORM = NormalizeData(STOF_BIG, lookBackLength, scaleLimit, scaleFloor);
			STOF_BX_NORM = NormalizeData(STOF_BX, lookBackLength, scaleLimit, scaleFloor);
			STOF_BXL_NORM = NormalizeData(STOF_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double STOFindSMALL (0),double STOFindMED (0),double STOFindBIG (0),double STOFindBX (0),double STOFindBXL (0);
			If UseBucketData Then Begin
				STOFindSMALL = BucketData(STOF_SMALL_NORM,intervalValue,intervalLimit);
				STOFindMED = BucketData(STOF_MED_NORM,intervalValue,intervalLimit);
				STOFindBIG = BucketData(STOF_BIG_NORM,intervalValue,intervalLimit);
				STOFindBX = BucketData(STOF_BX_NORM,intervalValue,intervalLimit);
				STOFindBXL = BucketData(STOF_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				STOFindSMALL = STOF_SMALL_NORM;
				STOFindMED = STOF_MED_NORM;
				STOFindBIG = STOF_BIG_NORM;
				STOFindBX = STOF_BX_NORM;
				STOFindBXL = STOF_BXL_NORM;
	End;

	
	//*****************************************************
	//************ Stochastics Slow ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double STOS_SMALL(0), double STOS_MED(0), double STOS_BIG(0), double STOS_BX(0),STOS_BXL(0);
			STOS_SMALL = fn_Stochastics_Slow(Close, High, Low,indLengthA,(indLengthA/2), (indLengthA/2),1);
			STOS_MED = fn_Stochastics_Slow(Close, High, Low,indLengthB,(indLengthB/2), (indLengthB/2),1);
			STOS_BIG = fn_Stochastics_Slow(Close, High, Low,indLengthC,(indLengthC/2), (indLengthC/2),1);
			STOS_BX = fn_Stochastics_Slow(Close, High, Low,indLengthD,(indLengthD/2), (indLengthD/2),1);
			STOS_BXL = fn_Stochastics_Slow(Close, High, Low,indLengthE,(indLengthE/2), (indLengthE/2),1);

		//scale and normalize features, (indicator results) ********************
		Var: double STOS_SMALL_NORM(0),double STOS_MED_NORM(0),double STOS_BIG_NORM(0),double STOS_BX_NORM(0),double STOS_BXL_NORM(0);
			STOS_SMALL_NORM = NormalizeData(STOS_SMALL, lookBackLength, scaleLimit, scaleFloor);
			STOS_MED_NORM = NormalizeData(STOS_MED, lookBackLength, scaleLimit, scaleFloor);
			STOS_BIG_NORM = NormalizeData(STOS_BIG, lookBackLength, scaleLimit, scaleFloor);
			STOS_BX_NORM = NormalizeData(STOS_BX, lookBackLength, scaleLimit, scaleFloor);
			STOS_BXL_NORM = NormalizeData(STOS_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double STOSindSMALL (0),double STOSindMED (0),double STOSindBIG (0),double STOSindBX (0),double STOSindBXL (0);
			If UseBucketData Then Begin
				STOSindSMALL = BucketData(STOS_SMALL_NORM,intervalValue,intervalLimit);
				STOSindMED = BucketData(STOS_MED_NORM,intervalValue,intervalLimit);
				STOSindBIG = BucketData(STOS_BIG_NORM,intervalValue,intervalLimit);
				STOSindBX = BucketData(STOS_BX_NORM,intervalValue,intervalLimit);
				STOSindBXL = BucketData(STOS_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				STOSindSMALL = STOS_SMALL_NORM;
				STOSindMED = STOS_MED_NORM;
				STOSindBIG = STOS_BIG_NORM;
				STOSindBX = STOS_BX_NORM;
				STOSindBXL = STOS_BXL_NORM;
	End;


	//*****************************************************
	//************ Stochastic Momentum Index (SMI) ********************* 
	//*****************************************************
	
	//*****************************************************
	//************ Stochastic Ehlers  ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double ELS_SMALL(0), double ELS_MED(0), double ELS_BIG(0), double ELS_BX(0),ELS_BXL(0);
			ELS_SMALL = fn_ELS(Close,indLengthA, .3);
			ELS_MED = fn_ELS(Close,indLengthB, .3);
			ELS_BIG = fn_ELS(Close,indLengthC, .3);
			ELS_BX = fn_ELS(Close,indLengthD, .3);
			ELS_BXL = fn_ELS(Close,indLengthE, .3);

		//scale and normalize features, (indicator results) ********************
		Var: double ELS_SMALL_NORM(0),double ELS_MED_NORM(0),double ELS_BIG_NORM(0),double ELS_BX_NORM(0),double ELS_BXL_NORM(0);
			ELS_SMALL_NORM = NormalizeData(ELS_SMALL, lookBackLength, scaleLimit, scaleFloor);
			ELS_MED_NORM = NormalizeData(ELS_MED, lookBackLength, scaleLimit, scaleFloor);
			ELS_BIG_NORM = NormalizeData(ELS_BIG, lookBackLength, scaleLimit, scaleFloor);
			ELS_BX_NORM = NormalizeData(ELS_BX, lookBackLength, scaleLimit, scaleFloor);
			ELS_BXL_NORM = NormalizeData(ELS_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double ELSindSMALL (0),double ELSindMED (0),double ELSindBIG (0),double ELSindBX (0),double ELSindBXL (0);
			If UseBucketData Then Begin
				ELSindSMALL = BucketData(ELS_SMALL_NORM,intervalValue,intervalLimit);
				ELSindMED = BucketData(ELS_MED_NORM,intervalValue,intervalLimit);
				ELSindBIG = BucketData(ELS_BIG_NORM,intervalValue,intervalLimit);
				ELSindBX = BucketData(ELS_BX_NORM,intervalValue,intervalLimit);
				ELSindBXL = BucketData(ELS_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				ELSindSMALL = ELS_SMALL_NORM;
				ELSindMED = ELS_MED_NORM;
				ELSindBIG = ELS_BIG_NORM;
				ELSindBX = ELS_BX_NORM;
				ELSindBXL = ELS_BXL_NORM;
	End;


	//*****************************************************
	//************ Laguerre RSI from John Ehlers ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double LRSI_SMALL(0), double LRSI_MED(0), double LRSI_BIG(0), double LRSI_BX(0),LRSI_BXL(0);
			LRSI_SMALL = fn_Ehlers_Laguerre_RSI(Close,indLengthA*.1);
			LRSI_MED = fn_Ehlers_Laguerre_RSI(Close,indLengthB*.1);
			LRSI_BIG = fn_Ehlers_Laguerre_RSI(Close,indLengthC*.1);
			LRSI_BX = fn_Ehlers_Laguerre_RSI(Close,indLengthD*.1);
			LRSI_BXL = fn_Ehlers_Laguerre_RSI(Close,indLengthE*.1);

		//scale and normalize features, (indicator results) ********************
		Var: double LRSI_SMALL_NORM(0),double LRSI_MED_NORM(0),double LRSI_BIG_NORM(0),double LRSI_BX_NORM(0),double LRSI_BXL_NORM(0);
			LRSI_SMALL_NORM = NormalizeData(LRSI_SMALL, lookBackLength, scaleLimit, scaleFloor);
			LRSI_MED_NORM = NormalizeData(LRSI_MED, lookBackLength, scaleLimit, scaleFloor);
			LRSI_BIG_NORM = NormalizeData(LRSI_BIG, lookBackLength, scaleLimit, scaleFloor);
			LRSI_BX_NORM = NormalizeData(LRSI_BX, lookBackLength, scaleLimit, scaleFloor);
			LRSI_BXL_NORM = NormalizeData(LRSI_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double LRSIindSMALL (0),double LRSIindMED (0),double LRSIindBIG (0),double LRSIindBX (0),double LRSIindBXL (0);
			If UseBucketData Then Begin
				LRSIindSMALL = BucketData(LRSI_SMALL_NORM,intervalValue,intervalLimit);
				LRSIindMED = BucketData(LRSI_MED_NORM,intervalValue,intervalLimit);
				LRSIindBIG = BucketData(LRSI_BIG_NORM,intervalValue,intervalLimit);
				LRSIindBX = BucketData(LRSI_BX_NORM,intervalValue,intervalLimit);
				LRSIindBXL = BucketData(LRSI_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				LRSIindSMALL = LRSI_SMALL_NORM;
				LRSIindMED = LRSI_MED_NORM;
				LRSIindBIG = LRSI_BIG_NORM;
				LRSIindBX = LRSI_BX_NORM;
				LRSIindBXL = LRSI_BXL_NORM;
	End;

		
	//*****************************************************
	//************ Stochastic RSI (StochRSI) ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double StoRSI_SMALL(0), double StoRSI_MED(0), double StoRSI_BIG(0), double StoRSI_BX(0),StoRSI_BXL(0);
			StoRSI_SMALL = fn_Ehlers_Sto_RSI(MedianPrice,indLengthA, indLengthA, indLengthA, indLengthA);
			StoRSI_MED = fn_Ehlers_Sto_RSI(MedianPrice,indLengthB, indLengthB, indLengthB, indLengthB);
			StoRSI_BIG = fn_Ehlers_Sto_RSI(MedianPrice,indLengthC, indLengthC, indLengthC, indLengthC);
			StoRSI_BX = fn_Ehlers_Sto_RSI(MedianPrice,indLengthD, indLengthD, indLengthD, indLengthD);
			StoRSI_BXL = fn_Ehlers_Sto_RSI(MedianPrice,indLengthE, indLengthE, indLengthE, indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double StoRSI_SMALL_NORM(0),double StoRSI_MED_NORM(0),double StoRSI_BIG_NORM(0),double StoRSI_BX_NORM(0),double StoRSI_BXL_NORM(0);
			StoRSI_SMALL_NORM = NormalizeData(StoRSI_SMALL, lookBackLength, scaleLimit, scaleFloor);
			StoRSI_MED_NORM = NormalizeData(StoRSI_MED, lookBackLength, scaleLimit, scaleFloor);
			StoRSI_BIG_NORM = NormalizeData(StoRSI_BIG, lookBackLength, scaleLimit, scaleFloor);
			StoRSI_BX_NORM = NormalizeData(StoRSI_BX, lookBackLength, scaleLimit, scaleFloor);
			StoRSI_BXL_NORM = NormalizeData(StoRSI_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double StoRSIindSMALL (0),double StoRSIindMED (0),double StoRSIindBIG (0),double StoRSIindBX (0),double StoRSIindBXL (0);
			If UseBucketData Then Begin
				StoRSIindSMALL = BucketData(StoRSI_SMALL_NORM,intervalValue,intervalLimit);
				StoRSIindMED = BucketData(StoRSI_MED_NORM,intervalValue,intervalLimit);
				StoRSIindBIG = BucketData(StoRSI_BIG_NORM,intervalValue,intervalLimit);
				StoRSIindBX = BucketData(StoRSI_BX_NORM,intervalValue,intervalLimit);
				StoRSIindBXL = BucketData(StoRSI_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				StoRSIindSMALL = StoRSI_SMALL_NORM;
				StoRSIindMED = StoRSI_MED_NORM;
				StoRSIindBIG = StoRSI_BIG_NORM;
				StoRSIindBX = StoRSI_BX_NORM;
				StoRSIindBXL = StoRSI_BXL_NORM;
	End;


	//*****************************************************
	//************ Stoller Average Range Channels (STARC) Bands ********************* 
	//*****************************************************
	
	//*****************************************************
	//************ Swing Index ********************* 
	//*****************************************************
	
	//*****************************************************
	//************ TRIX ********************* 
	//*****************************************************
	
	//*****************************************************
	//************ Typical Price ********************* 
	//*****************************************************
	
	//*****************************************************
	//************ Vertical Horizontal Filter (VHF) ********************* 
	//*****************************************************
	If CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 Then Begin
		//call indicator function ********************
		Var: double VHF_SMALL(0), double VHF_MED(0), double VHF_BIG(0), double VHF_BX(0),VHF_BXL(0);
			VHF_SMALL = fn_VHF(Close,indLengthA);
			VHF_MED = fn_VHF(Close,indLengthB);
			VHF_BIG = fn_VHF(Close,indLengthC);
			VHF_BX = fn_VHF(Close,indLengthD);
			VHF_BXL = fn_VHF(Close,indLengthE);

		//scale and normalize features, (indicator results) ********************
		Var: double VHF_SMALL_NORM(0),double VHF_MED_NORM(0),double VHF_BIG_NORM(0),double VHF_BX_NORM(0),double VHF_BXL_NORM(0);
			VHF_SMALL_NORM = NormalizeData(VHF_SMALL, lookBackLength, scaleLimit, scaleFloor);
			VHF_MED_NORM = NormalizeData(VHF_MED, lookBackLength, scaleLimit, scaleFloor);
			VHF_BIG_NORM = NormalizeData(VHF_BIG, lookBackLength, scaleLimit, scaleFloor);
			VHF_BX_NORM = NormalizeData(VHF_BX, lookBackLength, scaleLimit, scaleFloor);
			VHF_BXL_NORM = NormalizeData(VHF_BXL, lookBackLength, scaleLimit, scaleFloor);

		//categorize features, (indicator results)*****************************
		Var: double VHFindSMALL (0),double VHFindMED (0),double VHFindBIG (0),double VHFindBX (0),double VHFindBXL (0);
			If UseBucketData Then Begin
				VHFindSMALL = BucketData(VHF_SMALL_NORM,intervalValue,intervalLimit);
				VHFindMED = BucketData(VHF_MED_NORM,intervalValue,intervalLimit);
				VHFindBIG = BucketData(VHF_BIG_NORM,intervalValue,intervalLimit);
				VHFindBX = BucketData(VHF_BX_NORM,intervalValue,intervalLimit);
				VHFindBXL = BucketData(VHF_BXL_NORM,intervalValue,intervalLimit);
			End
			Else 
				VHFindSMALL = VHF_SMALL_NORM;
				VHFindMED = VHF_MED_NORM;
				VHFindBIG = VHF_BIG_NORM;
				VHFindBX = VHF_BX_NORM;
				VHFindBXL = VHF_BXL_NORM;
	End;

	
[IntrabarOrderGeneration=false]
///////////////////////////////////////////////////////////////
Input:
	PredictiveResponse( 3 );
///////////////////////////////////////////////////////////////
Input:
	UseEntryFilter( False);
///////////////////////////////////////////////////////////////
Input:
	ExitCondition( 1 ); //if set to '0' then this input will be ignored and the strategy will nothave an exit
///////////////////////////////////////////////////////////////
Inputs: 
	DailyDollarStop(0), //if set to '0' then this input will be ignored
	WeeklyDollarStop(0), //if set to '0' then this input will be ignored
	UseSounds(True),  //if set to 'False' then no sounds will play
	EntrySound("c:\sounds\REVERSION_Signal-Alert.wav"), //only used if UseSounds input is set to "True"
	ExitSound("c:\sounds\REVERSION_Signal-Alert.wav"); //only used if UseSounds input is set to "True"
///////////////////////////////////////////////////////////////
Inputs: 
	StrategyStartTime(0000), //only used if UseStartStopTimes input is set to "True"
	StrategyStopTime(0000), //only used if UseStartStopTimes input is set to "True"
	LongTradesOnly(False), //if set to 'True' then Strategy will only take long trades
	ShortTradesOnly(False); //if set to 'True' then Strategy will only take short trades
///////////////////////////////////////////////////////////////
Inputs: 
	TrailingStopDollarAmount( 0 ) ; //if set to '0' then this input will be ignored
If TrailingStopDollarAmount > 0 then begin
SetDollarTrailing( TrailingStopDollarAmount ) ;
end;
///////////////////////////////////////////////////////////////
Inputs: 
	ProfitTargetDollarAmount( 0 ) ; //if set to '0' then this input will be ignored
If ProfitTargetDollarAmount > 0 then begin
SetProfitTarget( ProfitTargetDollarAmount ) ;
end;
//////////////////////////////////////////////////////////////
Inputs: 
	StopLossDollarAmount( 0 ) ; //if set to '0' then this input will be ignored
If StopLossDollarAmount > 0 then begin
SetStopLoss( StopLossDollarAmount ) ;
end;
///////////////////////////////////////////////////////////////
	
	
Var: 
	TradeOvernight(0),
	MP(0),
	DailyNetProfit(0),
	PrevDayNetProfit(0),
	DayNetProfitOK(True),
	DayStopPoints(0),
	WeeklyNetProfit(0),
	PrevWeekNetProfit(0),
	WeekNetProfitOK(True),
	WeekStopPoints(0), 
	Bool1(True), 
	EntryTimeOK(False),
	EntrySoundOK(False),
	ExitSoundOK(False);


SetStopPosition;
MP = MarketPosition;

EntrySoundOK = False;
ExitSoundOK = False;

If DailyDollarStop > 0 and WeeklyDollarStop > 0 then begin
///////////////////////////////////////////////////////////////////
//Daily/Weekly Loss Calcs
if date <> date[1] then begin
	PrevDayNetProfit = NetProfit;
	DayNetProfitOK = True;
	if DayOfWeek(date) = 0 then begin
		PrevWeekNetProfit = NetProfit;
		WeekNetProfitOK = True;
	end;
end;

if MP = 0 then begin
	DailyNetProfit = NetProfit - PrevDayNetProfit;
	WeeklyNetProfit = NetProfit - PrevWeekNetProfit;
end
else if MP = 1 then begin
	DailyNetProfit = NetProfit - PrevDayNetProfit + CurrentContracts*BigPointValue*(Close - Entryprice);
	WeeklyNetProfit = NetProfit - PrevWeekNetProfit + CurrentContracts*BigPointValue*(Close - Entryprice);
end
else if MP = -1 then begin
	DailyNetProfit = NetProfit - PrevDayNetProfit + CurrentContracts*BigPointValue*(Entryprice - Close);
	WeeklyNetProfit = NetProfit - PrevWeekNetProfit + CurrentContracts*BigPointValue*(Entryprice - Close);
end;

//Daily Stop Loss
if MP <> 0 then
	DayStopPoints = (DailyNetProfit + DailyDollarStop) / (CurrentContracts*BigPointValue);

if DailyNetProfit <= -DailyDollarStop then
	DayNetProfitOK = False;

if MP <> 0 then begin
	sell ("LX DayLoss") next bar at Close - DayStopPoints Stop;	
	buytocover ("SX DayLoss") next bar at Close + DayStopPoints Stop;
end;	

//Weekly Stop Loss
if MP <> 0 then
	WeekStopPoints = (WeeklyNetProfit + WeeklyDollarStop) / (CurrentContracts*BigPointValue);

if WeeklyNetProfit <= -WeeklyDollarStop then
	WeekNetProfitOK = False;

if MP <> 0 then begin
	sell ("LX WkLoss") next bar at Close - WeekStopPoints Stop;//market;	
	buytocover ("SX WkLoss") next bar at Close + WeekStopPoints Stop;//market;
end;

if LastBarOnChart and ((DayNetProfitOK = False and DayNetProfitOK[1] = True) or (WeekNetProfitOK = False and WeekNetProfitOK[1] = True)) then
	ExitSoundOK = (PlaySound(ExitSound));
///////////////////////////////////////////////////////////////////
End;

variables: 
	OKToTradeBasedonTime(False);

OKToTradeBasedonTime = False;

If StrategyStartTime = 0 and StrategyStopTime = 0 then begin 
	OKToTradeBasedonTime = True;
end
	else begin
		If Time > StrategyStartTime and Time < StrategyStopTime then 
			OKToTradeBasedonTime = True;
	end;
///////////////////////////////////////////////////////////////
variables: 
//1. Variables
bool SwingHiRule_1(False),
bool SwingHiRule_2(False),
bool SwingHiRule_3(False),
bool SwingHiRule_4(False),
bool SwingHiRule_5(False),
bool SwingHiRule_6(False),
bool SwingHiRule_7(False),
bool SwingHiRule_8(False),
bool SwingHiRule_9(False),
bool SwingHiRule_10(False),
bool SwingHiRule_11(False),
bool SwingHiRule_12(False),
bool SwingHiRule_13(False),
bool SwingHiRule_14(False),
bool SwingHiRule_15(False),
bool SwingHiRule_16(False),
bool SwingHiRule_17(False),
bool SwingHiRule_18(False),
bool SwingHiRule_19(False),
bool SwingHiRule_20(False),
bool SwingHiRule_21(False),
bool SwingHiRule_22(False),
bool SwingHiRule_23(False),
bool SwingHiRule_24(False),
bool SwingHiRule_25(False),
bool SwingHiRule_26(False),
bool SwingHiRule_27(False),
bool SwingHiRule_28(False),
bool SwingHiRule_29(False),
bool SwingHiRule_30(False),
bool SwingHiRule_31(False),
bool SwingHiRule_32(False),
bool SwingHiRule_33(False),
bool SwingHiRule_34(False),
bool SwingHiRule_35(False),
bool SwingHiRule_36(False);



//2. Conditions

SwingHiRule_1 = PCTRindBX[RightStrength+0]<=89.223335 AND FRAMAindMED[RightStrength+0]>0.00102 AND MaHMAindBX[RightStrength-4]<=-0.001685;
//BBDindMED[RightStrength-3]>0.3545 and FRAMAindSMALL[RightStrength+0]>0.002155;
{SwingHiRule_2 = LINRGCindBIG[RightStrength-4]<=-0.001345 AND MaLGREindBIG[RightStrength+7]<=0.00778 AND MaMAMAindMED[RightStrength-1]<=0.000525;
SwingHiRule_3 = VIDYAindBX[RightStrength+10]>0.522805 AND MaHMAindBX[RightStrength-4]<=-0.001645 AND MaLGREindBIG[RightStrength+7]<=0.00739 AND MaMAMAindSMALL[RightStrength+0]>0.000615;
SwingHiRule_4 = EMDindSMALL[RightStrength+1]<=266.894085 AND MaHMAindBX[RightStrength-4]<=-0.00193;
SwingHiRule_5 = CMIindMED[RightStrength+13]>1.666665 AND DMIStoindBX[RightStrength-3]<=14.21093 AND MaLGREindBIG[RightStrength+7]<=0.00778;
SwingHiRule_6 = CMIindBIG[RightStrength+21]<=98.27538 AND RSQRindSMALL[RightStrength+16]<=0.99957 AND LINRGCindBIG[RightStrength-4]<=-0.001335;
SwingHiRule_7 = CMIindMED[RightStrength+13]>1.666665 AND DMIStoindBX[RightStrength-3]<=14.232165 AND MaLGREindBIG[RightStrength+7]<=0.00778;
SwingHiRule_8 = RSQRindSMALL[RightStrength+16]<=0.99957 AND LINRGCindBIG[RightStrength-4]<=-0.001335 AND MaLGREindBIG[RightStrength+8]<=0.00761;
SwingHiRule_9 = MaZLAGindMED[RightStrength+0]<=0.000165 AND RSQRindSMALL[RightStrength+16]<=0.99957 AND LINRGCindBIG[RightStrength-4]<=-0.001335;
SwingHiRule_10 = CMIindBXL[RightStrength-1]<=86.07229 AND MaZLAGindMED[RightStrength+0]<=0.000165 AND LINRGCindBIG[RightStrength-4]<=-0.001335;
SwingHiRule_11 = AROONindSMALL[RightStrength-3]<=416.66665 AND CDMIindBIG[RightStrength-1]<=56.32677 AND LINRGCindBIG[RightStrength-4]<=-0.001295;
SwingHiRule_12 = RSQRindSMALL[RightStrength+16]<=0.99957 AND LINRGCindBIG[RightStrength-4]<=-0.001335 AND MaLGREindBIG[RightStrength+7]<=0.00778;
SwingHiRule_13 = CMIindMED[RightStrength+13]>1.666665 AND RSIindBXL[RightStrength+23]>33.510635 AND DMIStoindBX[RightStrength-3]<=14.21093;
SwingHiRule_14 = MaZLAGindMED[RightStrength+0]<=0.000165 AND RSQRindSMALL[RightStrength+16]<=0.99957 AND LINRGCindBIG[RightStrength-4]<=-0.001355;
SwingHiRule_15 = RSQRindSMALL[RightStrength+16]<=0.999755 AND MaHMAindBX[RightStrength-4]<=-0.00193 AND MaLGREindBIG[RightStrength+7]<=0.00778;
SwingHiRule_16 = VIDYAindBX[RightStrength+0]>0.524905 AND CDMIindBIG[RightStrength+15]<=81.019395 AND MaHMAindBX[RightStrength-4]<=-0.001645;
SwingHiRule_17 = VHFindSMALL[RightStrength+21]>-0.75964 AND MaHMAindBX[RightStrength-4]<=-0.00193 AND MaMAMAindSMALL[RightStrength+0]>0.00055;
SwingHiRule_18 = MaZLAGindMED[RightStrength+0]<=0.000165 AND RSQRindSMALL[RightStrength+16]<=0.99957 AND LINRGCindBIG[RightStrength-4]<=-0.001335;
SwingHiRule_19 = VIDYAindBX[RightStrength+6]>0.521435 AND RSIHULindSMALL[RightStrength+28]<=98.99003 AND MaHMAindBXL[RightStrength+1]>0.003805;
SwingHiRule_20 = RSQRindSMALL[RightStrength+16]<=0.999755 AND MaHMAindBX[RightStrength-4]<=-0.00193 AND MaLGREindBXL[RightStrength+7]<=0.007635;
SwingHiRule_21 = VIDYAindBX[RightStrength+6]>0.521435 AND DMIStoindBX[RightStrength-3]<=15.46268 AND MaMAMAindBXL[RightStrength+7]>-0.029625;
SwingHiRule_22 = CMIindBIG[RightStrength+21]<=98.27538 AND MaZLAGindMED[RightStrength+0]<=0.000165 AND RSQRindSMALL[RightStrength+16]<=0.99957 AND LINRGCindBIG[RightStrength-4]<=-0.001335;
SwingHiRule_23 = RSQRindSMALL[RightStrength+16]<=0.99957 AND LINRGCindBIG[RightStrength-4]<=-0.001335;
SwingHiRule_24 = CCIindBXL[RightStrength+1]<=25396.9705 AND RSIHULindSMALL[RightStrength+28]<=98.99003 AND MaHMAindBXL[RightStrength+1]>0.003805;
SwingHiRule_25 = AROONindSMALL[RightStrength-4]<=-250 AND VIDYAindBX[RightStrength+6]>0.521435 AND MaHMAindBXL[RightStrength+1]>0.00381;
SwingHiRule_26 = VHFindMED[RightStrength+25]<=-0.17722 AND VIDYAindBX[RightStrength+6]>0.52171 AND MaZLAGindBXL[RightStrength+1]>0.004455;
SwingHiRule_27 = RSIindBXL[RightStrength+23]>33.510635 AND LINREGindSMALL[RightStrength-2]<=-0.1175;
SwingHiRule_28 = VIDYAindBX[RightStrength+6]>0.521435 AND EMDindMED[RightStrength-2]>19.033815 AND LINRGCindBXL[RightStrength+1]>0.004245;
SwingHiRule_29 = VIDYAindBX[RightStrength+6]>0.521435 AND pctBindMED[RightStrength-1]>2.327105 AND MaHMAindBXL[RightStrength+1]>0.004365;
SwingHiRule_30 = VIDYAindBX[RightStrength+6]>0.52171 AND EMDindMED[RightStrength-1]>33.916045 AND LINRGCindBXL[RightStrength+1]>0.004215;
SwingHiRule_31 = CCIindBXL[RightStrength+1]<=25396.9705 AND EMDindMED[RightStrength-2]>19.033815 AND LINRGCindBXL[RightStrength+1]>0.004245;
SwingHiRule_32 = PCTRindBX[RightStrength+0]<=89.223335 AND FRAMAindMED[RightStrength+0]>0.00102 AND MaHMAindBX[RightStrength-4]<=-0.001685;
SwingHiRule_33 = VIDYAindBX[RightStrength+10]>0.522305 AND LINRGCindMED[RightStrength+14]<=0.00241 AND MaHMAindBXL[RightStrength+1]>0.004365;
SwingHiRule_34 = AROONindMED[RightStrength-4]<=625 AND VIDYAindBX[RightStrength+6]>0.521435 AND LINRGCindBXL[RightStrength+1]>0.004245;
SwingHiRule_35 = CDMIindMED[RightStrength+23]>-64.62346 AND MaZLAGindSMALL[RightStrength+0]<=-0.001335;
SwingHiRule_36 = CCIindBIG[RightStrength+0]>7634.908 AND LINRGCindSMALL[RightStrength+6]>-0.001305 AND LRSIindSMALL[RightStrength-4]<=0.56333;
}


if 1<2 {condition1} then begin
If (DayNetProfitOK or DailyDollarStop = 0) and (WeekNetProfitOK or WeeklyDollarStop = 0) and OKToTradeBasedonTime then Begin
	// Sell Case
	If LongTradesOnly = False then begin
		var: shortEntryLocation(0), bool ShortFilter(False);		
		shortEntryLocation = low[0]-.0001;//open; //swingHi[0,1]
		
		//is current high less than low 4 bars ago and is current close less than current open?
		ShortFilter = high[0] < low[RightStrength] and close[0] <= open[0] ;
			//and high[RightStrength] > high[RightStrength+1] and high[RightStrength] > high[RightStrength-1]
			//and high[RightStrength] > high[RightStrength+2] and high[RightStrength] > high[RightStrength-2]
			//and high[RightStrength] > high[RightStrength+3] and high[RightStrength] > high[RightStrength-3];
			//and high[RightStrength] > high[RightStrength+4] and high[RightStrength] > high[RightStrength-4] ;
//3. Trade Expressions
  If SwingHiRule_1 AND ShortFilter then begin   SellShort ("SwingHiRule_1") Next Bar at shortEntryLocation stop;  End;//next bar at market;  End;//Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_2 AND ShortFilter then begin   SellShort ("SwingHiRule_2") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_3 AND ShortFilter then begin   SellShort ("SwingHiRule_3") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_4 AND ShortFilter then begin   SellShort ("SwingHiRule_4") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_5 AND ShortFilter then begin   SellShort ("SwingHiRule_5") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_6 AND ShortFilter then begin   SellShort ("SwingHiRule_6") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_7 AND ShortFilter then begin   SellShort ("SwingHiRule_7") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_8 AND ShortFilter then begin   SellShort ("SwingHiRule_8") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_9 AND ShortFilter then begin   SellShort ("SwingHiRule_9") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_10 AND ShortFilter then begin   SellShort ("SwingHiRule_10") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_11 AND ShortFilter then begin   SellShort ("SwingHiRule_11") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_12 AND ShortFilter then begin   SellShort ("SwingHiRule_12") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_13 AND ShortFilter then begin   SellShort ("SwingHiRule_13") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_14 AND ShortFilter then begin   SellShort ("SwingHiRule_14") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_15 AND ShortFilter then begin   SellShort ("SwingHiRule_15") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_16 AND ShortFilter then begin   SellShort ("SwingHiRule_16") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_17 AND ShortFilter then begin   SellShort ("SwingHiRule_17") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_18 AND ShortFilter then begin   SellShort ("SwingHiRule_18") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_19 AND ShortFilter then begin   SellShort ("SwingHiRule_19") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_20 AND ShortFilter then begin   SellShort ("SwingHiRule_20") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_21 AND ShortFilter then begin   SellShort ("SwingHiRule_21") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_22 AND ShortFilter then begin   SellShort ("SwingHiRule_22") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_23 AND ShortFilter then begin   SellShort ("SwingHiRule_23") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_24 AND ShortFilter then begin   SellShort ("SwingHiRule_24") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_25 AND ShortFilter then begin   SellShort ("SwingHiRule_25") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_26 AND ShortFilter then begin   SellShort ("SwingHiRule_26") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_27 AND ShortFilter then begin   SellShort ("SwingHiRule_27") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_28 AND ShortFilter then begin   SellShort ("SwingHiRule_28") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_29 AND ShortFilter then begin   SellShort ("SwingHiRule_29") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_30 AND ShortFilter then begin   SellShort ("SwingHiRule_30") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_31 AND ShortFilter then begin   SellShort ("SwingHiRule_31") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_32 AND ShortFilter then begin   SellShort ("SwingHiRule_32") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_33 AND ShortFilter then begin   SellShort ("SwingHiRule_33") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_34 AND ShortFilter then begin   SellShort ("SwingHiRule_34") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_35 AND ShortFilter then begin   SellShort ("SwingHiRule_35") Next Bar at shortEntryLocation stop;  End;
  //If SwingHiRule_36 AND ShortFilter then begin   SellShort ("SwingHiRule_36") Next Bar at shortEntryLocation stop;  End; 

	End;
	// Buy Case 
	If ShortTradesOnly = False then begin
		var: longEntryLocation(0);		longEntryLocation = open;//high[0]+.0001; //swingHi[0,1]
//3. Trade Expressions
{  If SwingNULLRule_1 then begin   Buy ("SwingNULLRul1") Next Bar at longEntryLocation limit;  End;
  If SwingNULLRule_2 then begin   Buy ("SwingNULLRul2") Next Bar at longEntryLocation limit;  End;
  If SwingNULLRule_3 then begin   Buy ("SwingNULLRul3") Next Bar at longEntryLocation limit;  End;
  If SwingNULLRule_4 then begin   Buy ("SwingNULLRul4") Next Bar at longEntryLocation limit;  End;
  If SwingNULLRule_5 then begin   Buy ("SwingNULLRul5") Next Bar at longEntryLocation limit;  End;
  If SwingNULLRule_6 then begin   Buy ("SwingNULLRul6") Next Bar at longEntryLocation limit;  End;
  If SwingNULLRule_7 then begin   Buy ("SwingNULLRul7") Next Bar at longEntryLocation limit;  End;
  If SwingNULLRule_8 then begin   Buy ("SwingNULLRul8") Next Bar at longEntryLocation limit;  End;
  If SwingNULLRule_9 then begin   Buy ("SwingNULLRul9") Next Bar at longEntryLocation limit;  End;
  If SwingNULLRule_10 then begin   Buy ("SwingNULLRul10") Next Bar at longEntryLocation limit;  End;
  If SwingNULLRule_11 then begin   Buy ("SwingNULLRul11") Next Bar at longEntryLocation limit;  End;
  If SwingNULLRule_12 then begin   Buy ("SwingNULLRul12") Next Bar at longEntryLocation limit;  End;
 }


	End;
End;
end;

if 1 > 0 then begin	
	Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\EasyLanguage\condition_SwingHiRule_1.csv")
		,"",NumToStr(Currentbar,0)
		,",'"+dateTimeBarOpened+"'"
		,",",Numtostr(barOpenPrice,8)
		,",",Numtostr(barHighPrice,8)
		,",",Numtostr(barLowPrice,8)
		,",",Numtostr(barClosePrice,8)
		,",AROONindMED[RightStrength-3]>-291.66665=,",NumToStr(AROONindMED[RightStrength-3],5)
		,",DMIStoindSMALL[RightStrength-3]<=19.409605=,",NumToStr(DMIStoindSMALL[RightStrength-3],5)
		,",MaALMAindBIG[RightStrength+2]<=0.000675=,",NumToStr(MaALMAindBIG[RightStrength+2],5)
		,",SwingHiRule_1,",SwingHiRule_1
		,",SwingHiRule_2,",SwingHiRule_2
		,",SwingHiRule_3,",SwingHiRule_3
		,",SwingHiRule_4,",SwingHiRule_4
		,",SwingHiRule_5,",SwingHiRule_5
		,",SwingHiRule_6,",SwingHiRule_6
		,",SwingHiRule_7,",SwingHiRule_7
		,",SwingHiRule_8,",SwingHiRule_8
		,",SwingHiRule_9,",SwingHiRule_9
		,",SwingHiRule_10,",SwingHiRule_10
		,",SwingHiRule_11,",SwingHiRule_11
		,",SwingHiRule_12,",SwingHiRule_12
		//+ NewLine
		);
	end;

{
Plot1 ( ACindBIG[RightStrength+0], "ACiBIG+0" ) ;
Plot2 ( AROONindMED[RightStrength-3], "AROMED-3" ) ;
Plot3 ( AROONindSMALL[RightStrength-2], "AROALL-2" ) ;
Plot4 ( BBDindBIG[RightStrength-1], "BBDBIG-1" ) ;
Plot5 ( BBDindBIG[RightStrength-2], "BBDBIG-2" ) ;
Plot6 ( BBDindMED[RightStrength-3], "BBDMED-3" ) ;
Plot7 ( CCIindBIG[RightStrength+0], "CCIBIG+0" ) ;
Plot8 ( CMIindBIG[RightStrength+9], "CMIBIG+9" ) ;
Plot9 ( CMIindBXL[RightStrength+5], "CMIBXL+5" ) ;
Plot10 ( CMIindMED[RightStrength+13], "CMIMED13" ) ;
Plot11 ( CMIindMED[RightStrength+17], "CMIMED17" ) ;
Plot12 ( DMIStoindBX[RightStrength-3], "DMIdBX-3" ) ;
Plot13 ( DMIStoindSMALL[RightStrength-3], "DMIALL-3" ) ;
Plot14 ( FRAMAindBXL[RightStrength+26], "FRABXL26" ) ;
Plot15 ( FRAMAindMED[RightStrength+0], "FRAMED+0" ) ;
Plot16 ( LINREGindMED[RightStrength-4], "LINMED-4" ) ;
Plot17 ( MaLGREindBIG[RightStrength+7], "MaLBIG+7" ) ;
Plot18 ( MaLGREindSMALL[RightStrength+0], "MaLALL+0" ) ;
Plot19 ( MaMAMAindSMALL[RightStrength+1], "MaMALL+1" ) ;
Plot20 ( pctBindSMALL[RightStrength-1], "pctALL-1" ) ;
Plot21 ( PCTRindMED[RightStrength+1], "PCTMED+1" ) ;
Plot22 ( RSIindSMALL[RightStrength+2], "RSIALL+2" ) ;
Plot23 ( VIDYAindBX[RightStrength+4], "VIDdBX+4" ) ;
}
///////////////////////////////////////////////////////////////
//Indicator Exit section

//MESA Adaptive Moving Average (MAMA)
//https://drive.google.com/drive/u/0/folders/0B4WuHPHd7tSacm50QWxrR2pWaGs

Inputs: Price((H+L)/2);
vars:FastLimit(0), SlowLimit(0);
FastLimit = 1.2 ;
SlowLimit = .005 ;
Vars: Smooth(0),
	Detrender(0),
	I1(0),
	Q1(0),
	jI(0),
	jQ(0),
	I2(0),
	Q2(0),
	Re(0),
	Im(0),
	Period(0),
	SmoothPeriod(0),
	Phase(0),
	DeltaPhase(0),
	alpha(0),
	MAMA(0),
	FAMA(0);
If CurrentBar > 5 then begin
	Smooth = (4*Price + 3*Price[1] + 2*Price[2] + Price[3]) / 10;
	Detrender = (.0962*Smooth + .5769*Smooth[2] - .5769*Smooth[4] - .0962*Smooth[6])*(.075*Period[1] + .54);
	{Compute InPhase and Quadrature components}
	Q1 = (.0962*Detrender + .5769*Detrender[2] - .5769*Detrender[4] - .0962*Detrender[6])*(.075*Period[1] + .54);
	I1 = Detrender[3];
	{Advance the phase of I1 and Q1 by 90 degrees}
	jI = (.0962*I1 + .5769*I1[2] - .5769*I1[4] - .0962*I1[6])*(.075*Period[1] + .54);
	jQ = (.0962*Q1 + .5769*Q1[2] - .5769*Q1[4] - .0962*Q1[6])*(.075*Period[1] + .54);
	{Phasor addition for 3 bar averaging)}
	I2 = I1 - jQ;
	Q2 = Q1 + jI;
	{Smooth the I and Q components before applying the discriminator}
	I2 = .2*I2 + .8*I2[1];
	Q2 = .2*Q2 + .8*Q2[1];
	{Homodyne Discriminator}
	Re = I2*I2[1] + Q2*Q2[1];
	Im = I2*Q2[1] - Q2*I2[1];
	Re = .2*Re + .8*Re[1];
	Im = .2*Im + .8*Im[1];
	If Im <> 0 and Re <> 0 then Period = 360/ArcTangent(Im/Re);
	If Period > 1.5*Period[1] then Period = 1.5*Period[1];
	If Period < .67*Period[1] then Period = .67*Period[1];
	If Period < 6 then Period = 6;
	If Period > 50 then Period = 50;
	Period = .2*Period + .8*Period[1];
	SmoothPeriod = .33*Period + .67*SmoothPeriod[1];
	If I1 <> 0 then Phase = (ArcTangent(Q1 / I1));
	DeltaPhase = Phase[1] - Phase;
	If DeltaPhase < 1 then DeltaPhase = 1;
	alpha = FastLimit / DeltaPhase;
	If alpha < SlowLimit then alpha = SlowLimit;
	MAMA = alpha*Price + (1 - alpha)*MAMA[1];
	FAMA = .5*alpha*MAMA + (1 - .5*alpha)*FAMA[1];
	//Plot1(MAMA, "MAMA");
	//Plot2(FAMA, "FAMA");
End;


If ExitCondition > 0 then begin
{if MP = 1 and PercentChg < 0 then begin
			sell ("Exit1 . ") next bar at market;
end;
if MP = -1 and PercentChg > 0 then begin
			buytocover ("Exit1. ") next bar at market;			
end;}
	If ExitCondition = 1 then begin
		if MP = 1 and MAMA[0] < FAMA[0] and MAMA[1] > FAMA[1] then begin
			sell ("Exit1 .") next bar at market;
			if MP = 1 and UseSounds and LastBarOnChart then
				ExitSoundOK = (PlaySound(ExitSound));
		end;
		if MP = -1 and MAMA[0] > FAMA[0] and MAMA[1] < FAMA[1] then begin
			buytocover ("Exit1.") next bar at market;
			if MP = -1 and UseSounds and LastBarOnChart then
				ExitSoundOK = (PlaySound(ExitSound));
		end;
	End;
{	If ExitCondition = 2 then begin
		if MP = 1 and aaMACDDiff[0] > 0 and aaMACDDiff[1] > 0 and aaMACDDiff[2] < 0 then begin
			sell ("Exit2 .") next bar at market;
			if MP = 1 and UseSounds and LastBarOnChart then
				ExitSoundOK = (PlaySound(ExitSound));
		end;
		if MP = -1 and aaMACDDiff[0] < 0 and aaMACDDiff[1] < 0 and aaMACDDiff[2] > 0 then begin
			buytocover ("Exit2.") next bar at market;
			if MP = -1 and UseSounds and LastBarOnChart then
				ExitSoundOK = (PlaySound(ExitSound));
		end;
	End;
	If ExitCondition = 3 then begin
		if MP = 1 and aaMACDDiff[0] > 0 and aaMACDDiff[1] > 0 and aaMACDDiff[2] > 0 and aaMACDDiff[3] < 0 then begin
			sell ("Exit3 .") next bar at market;
			if MP = 1 and UseSounds and LastBarOnChart then
				ExitSoundOK = (PlaySound(ExitSound));
		end;
		if MP = -1 and aaMACDDiff[0] < 0 and aaMACDDiff[1] < 0 and aaMACDDiff[2] < 0 and aaMACDDiff[3] > 0 then begin
			buytocover ("Exit3.") next bar at market;
			if MP = -1 and UseSounds and LastBarOnChart then
				ExitSoundOK = (PlaySound(ExitSound));
		end;
	End;
	If ExitCondition = 4 then begin
		if MP = 1 and aaMACDDiff[0] > 0 and aaMACDDiff[1] > 0 and aaMACDDiff[2] > 0 and aaMACDDiff[3] > 0 and aaMACDDiff[4] < 0 then begin
			sell ("Exit4 .") next bar at market;
			if MP = 1 and UseSounds and LastBarOnChart then
				ExitSoundOK = (PlaySound(ExitSound));
		end;
		if MP = -1 and aaMACDDiff[0] < 0 and aaMACDDiff[1] < 0 and aaMACDDiff[2] < 0 and aaMACDDiff[3] < 0 and aaMACDDiff[4] > 0 then begin
			buytocover ("Exit4.") next bar at market;
			if MP = -1 and UseSounds and LastBarOnChart then
				ExitSoundOK = (PlaySound(ExitSound));
		end;
	End;
	If ExitCondition = 5 then begin
		if MP = 1 and aaMACDDiff[0] > 0 and aaMACDDiff[1] > 0 and aaMACDDiff[2] > 0 and aaMACDDiff[3] > 0 and aaMACDDiff[4] > 0 and aaMACDDiff[5] < 0 then begin
			sell ("Exit5 .") next bar at market;
			if MP = 1 and UseSounds and LastBarOnChart then
				ExitSoundOK = (PlaySound(ExitSound));
		end;
		if MP = -1 and aaMACDDiff[0] < 0 and aaMACDDiff[1] < 0 and aaMACDDiff[2] < 0 and aaMACDDiff[3] < 0 and aaMACDDiff[4] < 0 and aaMACDDiff[5] > 0 then begin
			buytocover ("Exit5.") next bar at market;
			if MP = -1 and UseSounds and LastBarOnChart then
				ExitSoundOK = (PlaySound(ExitSound));
		end;
	End;
	If ExitCondition > 5 then begin
		if MP = 1 and aaMACDDiff[0] > 0 and aaMACDDiff[1] > 0 and aaMACDDiff[2] > 0 and aaMACDDiff[3] > 0 and aaMACDDiff[4] > 0 and aaMACDDiff[5] < 0 then begin
			sell ("Exit6 .") next bar at market;
			if MP = 1 and UseSounds and LastBarOnChart then
				ExitSoundOK = (PlaySound(ExitSound));
		end;
		if MP = -1 and aaMACDDiff[0] < 0 and aaMACDDiff[1] < 0 and aaMACDDiff[2] < 0 and aaMACDDiff[3] < 0 and aaMACDDiff[4] < 0 and aaMACDDiff[5] > 0 then begin
			buytocover ("Exit6.") next bar at market;
			if MP = -1 and UseSounds and LastBarOnChart then
				ExitSoundOK = (PlaySound(ExitSound));
		end;
	End;}

End;
///////////////////////////////////////////////////////////////
//Time Exit Section
If StrategyStopTime > 0 and Time > StrategyStopTime then begin
		sell ("ExitTime..") next bar at market;
		buytocover ("ExitTime ..") next bar at market;
End;
///////////////////////////////////////////////////////////////	
//end exit section
