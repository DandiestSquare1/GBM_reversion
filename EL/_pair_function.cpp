// 1.
// Add set of random indicator values to be evaluated as features. 
// Add random-guess classifier. see http://www.r-bloggers.com/computing-classification-evaluation-metrics-in-r/ 
// 2.
// Add variable interactions
// IF( X = Y, X(.1), LOG( ( 201) - X ) / (  (201) - Y), (201 - Y) ) ) <--round result to the 9th decimal place
// =IF(F3=G3,F3*0.1,ROUND(LOG((201-F3)/(201-G3),(201-G3)),9))

Var: {int RightStrength(0),}int rsLimit(0);
//RightStrength = 2;
rsLimit = 6;
//While RightStrength <= rsLimit begin


vars:
	leftStengthCounter(0)
	,rightStengthCounter(0)
	,leftPatternCounter(0)
	,rightPatternCounter(0)
	,bool calcSwingHi(True)
	,bool calcSwingLo(False)
	,plotBarNumTxt(0)
	,plotSwingHiTxt(0)
	,plotSwingLoTxt(0)
	,int BarsAgoSHiBar(0)
	,int BarsAgoSLoBar(0)
	,int SwingHiAtBarNum(0)
	,int SwingLoAtBarNum(0)
	,lastSwingHiBarNum(0)
	,lastSwingLoBarNum(0)
	,counterSH(0)
	,counterSL(0)
	,sizeOfLastSwing(0)
	,sHiBarNumForCalc(0)
   	,sLoBarNumForCalc(0)   	
   	,sHiPriceForCalc(0) 
   	,sLoPriceForCalc(0)
	,state("") 	
	//,bool debug(false) 
	,bool debugToCommentLog(True)
	,bool debugToPrintLog(True) 
	,bool debugToCSV(True);

array: swingHi[50,3](0)
	,swingLo[50,3](0);
	//Array: MyNumericArray[10,5] (0);

	//Var: BarCounter(0);	

	//if High > Highest(High,10)[1] and BarCounter < 10 then begin
	//	MyNumericArray[BarCounter,1] = High;
	//	MyNumericArray[BarCounter,2] = Low;
	//	MyNumericArray[BarCounter,3] = Open;
	//	MyNumericArray[BarCounter,4] = Close;
	//	MyNumericArray[BarCounter,5] = BarNumber;
	//	BarCounter = BarCounter + 1;
	//end;

leftStengthCounter = 1;  //LeftStrength;
rightStengthCounter = 1; //RightStrength;
leftPatternCounter = 0;
rightPatternCounter = 0;

inputs: 
	bool debug(False)
	,int LeftStrength( 8)   //Sets the required number of bars on the left side of the pivot bar.
	,int RightStrength(  25) //Sets the required number of bars on the right side of the pivot bar.
	,int leftPad(0)
	,int rightPad(0)
	,SwingHiObject(High)
	,SwingLoObject(Low)
	,bool ShowSwings(True)
	,bool ShowBarNum(False)
	,bool ShowObjVal(False)
	,bool ShowObjDiff(False)
	,bool ShowBarNumbers(False)
	,bool ReadWriteToDB(True)
	,bool UseBucketData(False)
	,int loadGroupId(1001);
	
{**********************************
 ** meta data
------ ------------------------
Pos. 1 barNumber
Pos. 2 symbol 
Pos. 3 lengthOfSeries
Pos. 4 chartType
Pos. 5 chartInterval
Pos. 6 dateBarOpened
Pos. 7 dayOfWeekBarOpened
Pos. 8 timeBarOpened
Pos. 9 timeBarOpenedhhmmss
Pos. 10 minutesBarWasOpen
Pos. 11 barOpenPrice
Pos. 12 barHighPrice
Pos. 13 barLowPrice
Pos. 14 barClosePrice
Pos. 15 minUnitPriceChange
Pos. 16 settingMaxBarsBack
**********************************}
//begin meta data
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
chartInterval = High[RightStrength] - Low[RightStrength] ; //If range bars in TradeStation use "BoxSize". If range bars in MultiCharts use "BarType"  

//Pos. 6 dateBarOpened 
strDateBarOpened = (FormatDate( "yyyy-MM-dd", ElDateToDateTime( Date[RightStrength] ) ));
dateBarOpened = strDateBarOpened ;

//Pos. 7 dayOfWeekBarOpened
dayOfWeekBarOpened = (FormatDate( "ddd", ElDateToDateTime(Date[RightStrength])));

//Pos. 8 timeBarOpened
//TimeToMinutes function calculates the number of minutes from midnight for any 24-hour (HHMM) time value
julianDateTime = DateToJulian(Date[RightStrength])+(TimeToMinutes(Time[RightStrength]) / 60 / 24 ) ; 
//timeBarOpened =(FormatTime("HH:mm:ss tt",julianDateTime));
//dateTimeBarOpened =DateTimeToString_Ms(DateTime[RightStrength]);
dateTimeBarOpened = DateTimeToString(julianDateTime );
  //DateTimeToString_Ms(DateTime[0])  will return a string value corresponding to the time and time of the current bar "5/28/2013 08:41:11.871". 

//Pos. 9 timeBarOpenedhhmmss
strHHBarOpened = FormatTime("HH",julianDateTime);
strMMBarOpened = FormatTime("mm",julianDateTime);
strSSBarOpened = FormatTime("ss",julianDateTime);
timeBarOpenedhhmmss = strHHBarOpened+":"+strMMBarOpened+":"+strSSBarOpened; //hh:mm:ss
//timeBarOpenedhhmmss = FormatTime("HH:mm:ss", ELTimetodatetime_s(Time_s[RightStrength]));

//Pos. 10 numminutesBarWasOpn 
//Time returns the time in 24-hour numeric format (HHMM format) of the closing price of the current bar
numMinutesFromMidnit = TimeToMinutes( Time[RightStrength] ); 
//numMinutesFromMidnit = SecondsFromDateTime(DateTime[RightStrength]) ;
If Date[RightStrength] = Date[RightStrength+1] Then Begin //date of current bar is the same as the date of one bar ago
	numminutesBarWasOpn = (numMinutesFromMidnit-1440) - (numMinutesFromMidnit[1]-1440);
End
Else Begin
 	numminutesBarWasOpn = (numMinutesFromMidnit+1440) - numMinutesFromMidnit[1]; 
End;

//Pos. 11 barOpenPrice
barOpenPrice = Open[RightStrength];

//Pos. 12 barHighPrice
barHighPrice = High[RightStrength];

//Pos. 13 barLowPrice
barLowPrice = Low[RightStrength];

//Pos. 14 barClosePrice
barClosePrice = Close[RightStrength];

//Pos. 15 minUnitPriceChange
minUnitPriceChange = minmove / pricescale; //PriceScale = BigPointValue / PointValue 
  {**************************************************************************************************
  ** MinMove / PriceScale calculates the minimum price move for a share or contract.
  ** MinMove / PriceScale * BigPointValue returns the profit or loss value associated with a minimum price move of a share or contract.
  ***************************************************************************************************}
  
//Pos. 16 settingMaxBarsBack
//Returns a numerical value indicating the Maximum Bars Back setting for the study. 
//All studies based on past data use a certain number of bars for their calculations. The number of bars is called Maximum number of bars a study will reference, or Maximum Bars Back. 
settingMaxBarsBack = MaxBarsBack + 1 ;

condition1 = seriesBarNumber > settingMaxBarsBack ;
	
	//plot bar numbers
	if ShowBarNumbers then begin
		plotBarNumTxt = text_new(Date[RightStrength]
				,Time[RightStrength]
				,SwingHiObject[RightStrength]+(minUnitPriceChange * 10)
				,numToStr(seriesBarNumber-RightStrength,0)
					);
		Text_SetStyle(plotBarNumTxt,2,1);
		text_setcolor(plotBarNumTxt,RGB(255,128,0)); 
	end;


var: 
	leftPadAmt(0),
	rightPadAmt(0);	

leftPadAmt = minUnitPriceChange * leftPad;			
rightPadAmt = minUnitPriceChange * rightPad;

var: bool leftHiPatternExist(FALSE),bool rightHiPatternExist(FALSE);

	if condition1 and calcSwingHi then begin
		//Start loop 1 
		while leftStengthCounter <= LeftStrength AND leftPatternCounter <= LeftStrength begin
			leftHiPatternExist = (SwingHiObject[RightStrength] >= SwingHiObject[RightStrength+leftStengthCounter]);//detect left pattern of SwingHi

			if leftHiPatternExist then
					leftPatternCounter = leftPatternCounter + 1
				else
					leftPatternCounter = 0 ;
			
			leftStengthCounter = leftStengthCounter + 1 ;
			
			//Start inner loop 
			while leftPatternCounter = LeftStrength and rightStengthCounter <= RightStrength  begin 
				rightHiPatternExist = (SwingHiObject[RightStrength] > SwingHiObject[RightStrength-rightStengthCounter])
										and (SwingHiObject[RightStrength] - Lowest(SwingLoObject,RightStrength)) >= rightPadAmt
										and (SwingHiObject[RightStrength] - Highest(SwingHiObject,RightStrength-1)) >= leftPadAmt;//detect right pattern of SwingHi

				if rightHiPatternExist then
						rightPatternCounter = rightPatternCounter + 1
					else
						rightPatternCounter = 0 ;
				
				if rightPatternCounter = RightStrength then begin 
					//Set
					SwingHiAtBarNum = seriesBarNumber-RightStrength;// <-- located swingHi to plot
					//plotMarker
					if ShowSwings then begin
					//plot1[RightStrength](SwingHiObject[RightStrength], "swingHiBar");
					//plotText
					end;
					if ShowBarNum then begin
						plotSwingHiTxt = text_new(Date[RightStrength]
								,Time[RightStrength]
								,SwingHiObject[RightStrength]
								,numToStr(seriesBarNumber-RightStrength,0)
								 );
						Text_SetStyle(plotSwingHiTxt,2,1);
						text_setcolor(plotSwingHiTxt,RGB(255,128,0)); 
						//text_setSize(plotSwingHiTxt,8);
					end;				
					if ShowObjDiff then begin
						Value1 = text_new(Date[RightStrength]
								,Time[RightStrength]
								,SwingHiObject[RightStrength]+2
								,numToStr(AbsValue(High[RightStrength]-Low[BarsAgoSLoBar+1]),2)
								 );
						Text_SetStyle(Value1 ,2,1);
						text_setcolor(Value1 ,RGB(255,128,0)); 
						//text_setSize(Value1 ,7);
					end;
					if ShowObjVal then begin
						Value2 = text_new(Date[RightStrength]
								,Time[RightStrength]
								,SwingHiObject[RightStrength]+4.5
								,numToStr(SwingHiObject[RightStrength],2)
								 );
						Text_SetStyle(Value2 ,2,1);
						text_setcolor(Value2 ,RGB(255,128,0)); 
						//text_setSize(Value2 ,8);
					end;
					lastSwingHiBarNum = seriesBarNumber-RightStrength;// <-- *** located swingHi to export to data set ***
		
					for counterSH = 0 to 49
					begin
						swingHi[50-counterSH,0] = swingHi[50-counterSH-1,0];
						swingHi[50-counterSH,1] = swingHi[50-counterSH-1,1];
						swingHi[50-counterSH,2] = swingHi[50-counterSH-1,2];
						swingHi[50-counterSH,3] = swingHi[50-counterSH-1,3];
						
					end; 
					swingHi[0,0] = lastSwingHiBarNum ; 	    //barNum
					swingHi[0,1] = SwingHiObject[RightStrength] ; //price
					swingHi[0,2] = Date[RightStrength] ; 	    //date
					swingHi[0,3] = Time[RightStrength] ; 	    //time		
					
				end; //End inner loop 
				
				rightStengthCounter = rightStengthCounter + 1 ;
				
			end; //End inner loop 

			//reset to original value 
			rightStengthCounter = 1;//RightStrength;
			rightPatternCounter = 0; 
			rightHiPatternExist = False;
			
			BarsAgoSHiBar = seriesBarNumber-SwingHiAtBarNum;	
		
		end ;//End loop 1

		//reset to original value 
		leftStengthCounter = 1;//LeftStrength;
		leftPatternCounter = 0;
		leftHiPatternExist = False;
		calcSwingHi = False;
		calcSwingLo = True;

	end;

var: bool leftLoPatternExist(FALSE),bool rightLoPatternExist(FALSE);
	if condition1 and calcSwingLo then begin
		//Start loop 1
		while leftStengthCounter <= LeftStrength AND leftPatternCounter <= LeftStrength begin
			leftLoPatternExist = (Low[RightStrength] <= Low[RightStrength+leftStengthCounter]) ;//detect left pattern of SwingLo
								//SwingLoObject
			if leftLoPatternExist then
					leftPatternCounter = leftPatternCounter + 1
				else
					leftPatternCounter = 0 ;
			
			leftStengthCounter = leftStengthCounter + 1 ;
			
			//Start inner loop 
			while leftPatternCounter = LeftStrength and rightStengthCounter <= RightStrength  begin 
				rightLoPatternExist = (SwingLoObject[RightStrength] < SwingLoObject[RightStrength-rightStengthCounter])
										and (Highest(SwingHiObject,RightStrength) - SwingLoObject[RightStrength]) >= rightPadAmt
										and (Lowest(SwingLoObject,RightStrength-1) - SwingLoObject[RightStrength]) >= leftPadAmt;//detect right pattern of SwingLo

				if rightLoPatternExist then
						rightPatternCounter = rightPatternCounter + 1
					else
						rightPatternCounter = 0 ;
				
				if rightPatternCounter = RightStrength then begin 
					//Set
					SwingLoAtBarNum = seriesBarNumber-RightStrength;// <-- located swingLo to plot
					//plotMarker
					if ShowSwings then begin
					//plot2[RightStrength](SwingLoObject[RightStrength], "swingLowBar");
					end;
					//plotText
					if ShowBarNum then begin
						plotSwingLoTxt = text_new(Date[RightStrength]
								,Time[RightStrength]
								,SwingLoObject[RightStrength]
								,numToStr(seriesBarNumber-RightStrength,0)
								 );
						text_SetStyle(plotSwingLoTxt,2,0);
						text_setcolor(plotSwingLoTxt,RGB(255,128,0));
						//text_setSize(plotSwingLoTxt,8);
					end;
					if ShowObjDiff then begin
						Value3 = text_new(Date[RightStrength]
								,Time[RightStrength]
								,SwingLoObject[RightStrength]-2
								,numToStr(AbsValue(Low[RightStrength]-High[BarsAgoSHiBar]),2)
								 );
						Text_SetStyle(Value3 ,2,0);
						text_setcolor(Value3 ,RGB(255,128,0)); 
						//text_setSize(Value3 ,7);
					end;				
					if ShowObjVal then begin
						Value4 = text_new(Date[RightStrength]
								,Time[RightStrength]
								,SwingLoObject[RightStrength]-4.5
								,numToStr(SwingLoObject[RightStrength],2)
								 );
						Text_SetStyle(Value4 ,2,0);
						text_setcolor(Value4 ,RGB(255,128,0)); 
						//text_setSize(Value4 ,8);	
					end;		
					lastSwingLoBarNum = seriesBarNumber-RightStrength;// <-- *** located swingLo to export to data set ***
		
					for counterSH = 0 to 49
					begin
						swingLo[50-counterSH,0] = swingLo[50-counterSH-1,0];
						swingLo[50-counterSH,1] = swingLo[50-counterSH-1,1];
						swingLo[50-counterSH,2] = swingLo[50-counterSH-1,2];
						swingLo[50-counterSH,3] = swingLo[50-counterSH-1,3];
						//_swingLo[1+counter] = _swingLo[0+counter];
					end; 
					swingLo[0,0] = lastSwingLoBarNum ;		    //barNum
					swingLo[0,1] = SwingLoObject[RightStrength] ; //price
					swingLo[0,2] = Date[RightStrength]  ;	    //date
					swingLo[0,3] = Time[RightStrength] ;	    //time

				end;//End inner loop 
				
				rightStengthCounter = rightStengthCounter + 1 ;

			end; //End inner loop 

			//reset to original value 
			rightStengthCounter = 1;//RightStrength;
			rightPatternCounter = 0;
			rightLoPatternExist = False; 	
			
			BarsAgoSLoBar = seriesBarNumber-SwingLoAtBarNum;

		end ;//End loop 1

		//reset to original value 
		leftStengthCounter = 1;//LeftStrength;
		leftPatternCounter = 0;
		leftLoPatternExist = False;
		calcSwingHi = True;
		calcSwingLo = False;
	end;

	if CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 then begin

		{*******************************************************
			swingLo[0,0] = lastSwingLoBarNum 	; //barNum
			swingLo[0,1] = Low[RightStrength] 	; //price
			swingLo[0,2] = Date[RightStrength] ; //date
			swingLo[0,3] = Time[RightStrength] ; //time
		*******************************************************}
		if condition1 then begin
			if swingHi[0,2] > swingLo[0,2] and //date
			swingHi[0,0] > swingLo[0,0] then begin //barNum
				//sizeOfLastSwing = AbsValue(swingHi[0,1] - swingLo[0,1]) ; //current swing high minus last swing low
				sHiBarNumForCalc =  swingHi[0,0] ;
				sLoBarNumForCalc = swingLo[0,0] ;
				sHiPriceForCalc =  swingHi[0,1] ;
				sLoPriceForCalc = swingLo[0,1] ;
			end;
			if swingLo[0,2] > swingHi[0,2] and //date
			swingLo[0,0] > swingHi[0,0] then begin //barNum
				//sizeOfLastSwing = AbsValue(swingHi[0,1] - swingLo[0,1]) ; //current swing high minus last swing low
				sHiBarNumForCalc =  swingHi[0,0] ;
				sLoBarNumForCalc = swingLo[0,0] ;
				sHiPriceForCalc =  swingHi[0,1] ;
				sLoPriceForCalc = swingLo[0,1] ;
			end;
		end;
			
		if condition1 then begin
			if //swingHi[0,2] > swingLo[0,2] and //date
			swingHi[0,0] > swingLo[0,0] and //barNum
			swingHi[0,1] > swingHi[1,1] and //price 
			swingHi[0,1] > swingLo[0,1] then begin //price
				state = "UP";
			end;
			if //swingLo[0,2] > swingHi[0,2] and //date
			swingLo[0,0] > swingHi[0,0] and //barNum
			swingLo[0,1] < swingLo[1,1] and //price 
			swingLo[0,1] < swingHi[0,1] then begin //price
				state = "DOWN";
			end;
		end;
		
		array: string stateUpDpwn[10,0]("");
		var: counterState(0);
		
		if condition1 then begin	
			for counterState = 0 to 9
			begin
				stateUpDpwn[10-counterState ,0] = stateUpDpwn[10-counterState -1,0];
				//_swingLo[1+counter] = _swingLo[0+counter];
			end; 
				stateUpDpwn[0,0] = state;
		end;	
		
				
		if debug then begin	
			Print("Bar:",numToStr(seriesBarNumber,0)
			//,"swingHiTime: ",FormatTime("HH:mm:ss", ELTimetodatetime_s(Time_s[RightStrength]))
			//," swingLoTime: ",FormatTime("HH:mm:ss", ELTimetodatetime_s(Time_s[RightStrength]))
			,", Last SwingHi [", BarsAgoSHiBar
			,"] bars ago @ bar [", SwingHiAtBarNum
			,"], Last SwingLo [", BarsAgoSLoBar
			,"] bars ago @ bar [",SwingLoAtBarNum
			//,"], sizeOfLastSwing [",sizeOfLastSwing
			,"]"
			,"state [",counterState
			,"]"
			+ NewLine);
		end;
	End;
	
{************************************************************************************************* 
**************************************************************************************************
zScore = ( x - Mean ) / SDev ;
		Mean = Average( x, Length ) ;
		SDev = StandardDev( x, Length, 2 ) ;

Normalize = (x - min[x]) / (max[x] - min[x]) //range set by min/max in sample

Denormalized = NormalizedValue * (max[x] - min[x]) + min[x]

linearlyRescale = (limit - floor) / (max[x] - min[x]) * (x - max[x]) + limit // where [x] is some lookback period and range is "hard coded" by setting [floor] and [limit] values to literals

rateOfChange = (x / x[Length] - 1 ) * 100
percentChange = (x - x[Length] ) / x[Length] ;

rsiNetChgAvg = (x - x[Length] ) / Length ;
************************************************************************************************** 
**************************************************************************************************}
{http://hightick.com/Tips.html
http://unicorn.us.com/trading/el.html}	

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
//PCTRindMEDp1_STDDEVindSMALLp3
//MaEPMAindBIGp5_PCHANindMEDp1

var: x(0),y(0);
x = PCTRindMED[RightStrength+1];//MaEPMAindBIG[RightStrength+5];	
y = STDDEVindSMALL[RightStrength+3];//PCHANindMED[RightStrength+1];
Value1 = 0.5*(x+y)*(x+y+1) +  x ;
Value11 = NormalizeData(Value1, lookBackLength, scaleLimit, scaleFloor);
Value2 = x ;
Value3 = y ;
	
Input: PlotKey(True);

If PlotKey = True Then Begin

	Plot1( Value11, "key" ) ;
End;
If PlotKey <> True Then Begin
	Plot2( Value2, "x" ) ;
	Plot3( Value3, "y" ) ;
End;
