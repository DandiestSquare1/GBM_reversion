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
	,int PriorSwingHiAtBarNum(0)
	,int BarsAgoPriorSHiBar(0)
	,int PriorSwingLoAtBarNum(0)
	,int BarsAgoPriorSLoBar(0)
	,int lastSwingHiBarNum(0)
	,int lastSwingLoBarNum(0)
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
	,bool debugToCSV(True)
	,int swingBarHiCount(0)
	,int swingBarLoCount(0);

//BarsAgoSHiBar = 0;
//BarsAgoSLoBar = 0;
//SwingHiAtBarNum = 0;
//SwingLoAtBarNum = 0;
swingBarHiCount = 0;
swingBarLoCount = 0;

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
	bool debug(True)
	,int LeftStrength(8)   //Sets the required number of bars on the left side of the pivot bar.
	,int RightStrength(12) //Sets the required number of bars on the right side of the pivot bar.
	,int leftPad(0)
	,int rightPad(0)
	,SwingHiObject(High)
	,SwingLoObject(Low)
	,bool ShowSwings(True)
	,bool ShowSwingBarNum(True)
	,bool ShowObjVal(False)
	,bool ShowObjDiff(False)
	,bool ShowBarNumbers(False)
	,bool ReadWriteToDB(False)
	,bool UseBucketData(False)
	,int loadGroupId(1004);
	
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


var: 
	leftPadAmt(0),
	rightPadAmt(0);	

leftPadAmt = minUnitPriceChange * leftPad;			
rightPadAmt = minUnitPriceChange * rightPad;

var: bool AllowNewSwingHi(FALSE),bool AllowNewSwingLo(FALSE);

if condition1 then begin
	if swingLo[0,0] > swingHi[0,0] or swingHi[0,0] = 0  then
					AllowNewSwingHi = TRUE
				else
					AllowNewSwingHi = FALSE ;
	
	if swingHi[0,0] > swingLo[0,0] or swingLo[0,0] = 0  then
					AllowNewSwingLo = TRUE
				else
					AllowNewSwingLo = FALSE ;
end; 

var: bool leftHiPatternExist(FALSE),bool rightHiPatternExist(FALSE);
if condition1 and calcSwingHi then begin
	while leftStengthCounter <= LeftStrength AND leftPatternCounter <= LeftStrength begin
		//detect left pattern of SwingHi
		leftHiPatternExist = (SwingHiObject[RightStrength] >= SwingHiObject[RightStrength+leftStengthCounter]);

		if leftHiPatternExist and AllowNewSwingHi then
				leftPatternCounter = leftPatternCounter + 1
			else
				leftPatternCounter = 0 ;
		
		leftStengthCounter = leftStengthCounter + 1 ;

		//detect right pattern of SwingHi
		while leftPatternCounter = LeftStrength and rightStengthCounter <= RightStrength  begin 
			rightHiPatternExist = (SwingHiObject[RightStrength] > SwingHiObject[RightStrength-rightStengthCounter])
									and (SwingHiObject[RightStrength] - Lowest(SwingLoObject,RightStrength)) >= rightPadAmt
									and (SwingHiObject[RightStrength] - Highest(SwingHiObject,RightStrength-1)) >= leftPadAmt;

			if rightHiPatternExist then
					rightPatternCounter = rightPatternCounter + 1
				else
					rightPatternCounter = 0 ;
			
			if rightPatternCounter = RightStrength then begin 

				SwingHiAtBarNum = seriesBarNumber-RightStrength;
				PriorSwingHiAtBarNum = SwingHiAtBarNum - swingHi[0,0];
				swingBarHiCount = swingLo[0,0]-swingHi[1,0];
				lastSwingHiBarNum = seriesBarNumber-RightStrength;
	
				for counterSH = 0 to 49 begin
					swingHi[50-counterSH,0] = swingHi[50-counterSH-1,0];
					swingHi[50-counterSH,1] = swingHi[50-counterSH-1,1];
					swingHi[50-counterSH,2] = swingHi[50-counterSH-1,2];
					swingHi[50-counterSH,3] = swingHi[50-counterSH-1,3];
				end; 
				swingHi[0,0] = lastSwingHiBarNum ; //barNum
				swingHi[0,1] = SwingHiObject[RightStrength] ; //price
				swingHi[0,2] = Date[RightStrength] ; //date
				swingHi[0,3] = Time[RightStrength] ; //time	
				
				if ShowSwings then begin
					plot1[RightStrength](SwingHiObject[RightStrength], "swingHiBar");
					plot2[RightStrength](SwingHiObject[RightStrength]+(minUnitPriceChange*60), "TopMargin");
				end;
				
				if swingHi[1,1] > 0 then begin
					if ShowObjDiff then begin
						Value1 = text_new(swingHi[1,2]//Date[RightStrength]
								,swingHi[1,3]//Time[RightStrength]
								,swingHi[1,1]+(minUnitPriceChange*17)//SwingHiObject[RightStrength]+(minUnitPriceChange*17)
								,"Size: "+numToStr(swingLo[0,1]-swingHi[1,1],4)+" / "+numToStr((swingLo[0,0]-swingHi[1,0]),0)+" bars"
									);
						Text_SetStyle(Value1 ,2,1);
						text_setcolor(Value1 ,RGB(255,128,0)); 
					end;
					
					if ShowObjVal then begin
						Value2 = text_new(swingHi[1,2]//Date[RightStrength]
								,swingHi[1,3]//Time[RightStrength]
								,swingHi[1,1]+(minUnitPriceChange*9)//SwingHiObject[RightStrength]+(minUnitPriceChange*9) //Price
								,"Price: "+numToStr(swingHi[1,1],4)
									);
						Text_SetStyle(Value2 ,2,1);
						text_setcolor(Value2 ,RGB(255,128,0)); 
					end;
				end;
				
				if ShowSwingBarNum then begin
					plotSwingHiTxt = text_new(swingHi[1,2]//Date[RightStrength]
							,swingHi[1,3]//Time[RightStrength]
							,swingHi[1,1]+(minUnitPriceChange*1)//SwingHiObject[RightStrength]+(minUnitPriceChange*1)
							,"BarNum: "+numToStr(swingHi[1,0],0)
								);
					Text_SetStyle(plotSwingHiTxt,2,1);
					text_setcolor(plotSwingHiTxt,RGB(255,128,0)); 
				end;
			end; 

			rightStengthCounter = rightStengthCounter + 1 ;
		end;
		
		BarsAgoSHiBar = seriesBarNumber-SwingHiAtBarNum;
		BarsAgoPriorSHiBar = seriesBarNumber-PriorSwingHiAtBarNum;
		rightStengthCounter = 1;
		rightPatternCounter = 0; 
		rightHiPatternExist = False;	
	end;
	
	leftStengthCounter = 1;
	leftPatternCounter = 0;
	leftHiPatternExist = False;
	calcSwingHi = False;
	calcSwingLo = True;

end;

var: bool leftLoPatternExist(FALSE),bool rightLoPatternExist(FALSE);
if condition1 and calcSwingLo then begin
	while leftStengthCounter <= LeftStrength AND leftPatternCounter <= LeftStrength begin
		//detect left pattern of SwingLo
		leftLoPatternExist = (Low[RightStrength] <= Low[RightStrength+leftStengthCounter]);

		if leftLoPatternExist and AllowNewSwingLo then
				leftPatternCounter = leftPatternCounter + 1
			else
				leftPatternCounter = 0 ;
		
		leftStengthCounter = leftStengthCounter + 1 ;
		
		//detect right pattern of SwingLo
		while leftPatternCounter = LeftStrength and rightStengthCounter <= RightStrength  begin 
			rightLoPatternExist = (SwingLoObject[RightStrength] < SwingLoObject[RightStrength-rightStengthCounter])
									and (Highest(SwingHiObject,RightStrength) - SwingLoObject[RightStrength]) >= rightPadAmt
									and (Lowest(SwingLoObject,RightStrength-1) - SwingLoObject[RightStrength]) >= leftPadAmt;
 
			if rightLoPatternExist then
					rightPatternCounter = rightPatternCounter + 1
				else
					rightPatternCounter = 0 ;
			
			if rightPatternCounter = RightStrength then begin 

				SwingLoAtBarNum = seriesBarNumber-RightStrength;
				PriorSwingLoAtBarNum = SwingLoAtBarNum - swingLo[0,0];
				swingBarLoCount = swingHi[0,0]-swingLo[1,0];
				lastSwingLoBarNum = seriesBarNumber-RightStrength;
									
				for counterSL = 0 to 49 begin
					swingLo[50-counterSL,0] = swingLo[50-counterSL-1,0];
					swingLo[50-counterSL,1] = swingLo[50-counterSL-1,1];
					swingLo[50-counterSL,2] = swingLo[50-counterSL-1,2];
					swingLo[50-counterSL,3] = swingLo[50-counterSL-1,3];
				end; 
				swingLo[0,0] = lastSwingLoBarNum ;  //barNum
				swingLo[0,1] = SwingLoObject[RightStrength] ; //price
				swingLo[0,2] = Date[RightStrength] ; //date
				swingLo[0,3] = Time[RightStrength] ; //time
				
				if ShowSwings then begin
					plot3[RightStrength](SwingLoObject[RightStrength], "swingLowBar");
					plot4[RightStrength](SwingLoObject[RightStrength]-(minUnitPriceChange*60), "LowMargin");
				end;
				
				
				if swingLo[1,1] > 0 then begin
					if ShowObjDiff then begin
						Value3 = text_new(swingLo[1,2]//Date[RightStrength]
								,swingLo[1,3]//Time[RightStrength]
								,swingLo[1,1]-(minUnitPriceChange*17)//SwingLoObject[RightStrength]-(minUnitPriceChange*17)
								,"Size: +"+numToStr(swingHi[0,1]-swingLo[1,1],4)+" / "+numToStr((swingHi[0,0]-swingLo[1,0]),0)+" bars"
							 	);
						Text_SetStyle(Value3 ,2,0);
						text_setcolor(Value3 ,RGB(255,128,0)); 
					end;				
	
					if ShowObjVal then begin
						Value4 = text_new(swingLo[1,2]//Date[RightStrength]
								,swingLo[1,3]//Time[RightStrength]
								,swingLo[1,1]-(minUnitPriceChange*9)//SwingLoObject[RightStrength]-(minUnitPriceChange*9)
								,"Price: "+numToStr(swingLo[1,1],4)
							 	);
						Text_SetStyle(Value4 ,2,0);
						text_setcolor(Value4 ,RGB(255,128,0)); 
					end;
				end;		

				if ShowSwingBarNum then begin
					plotSwingLoTxt = text_new(swingLo[1,2]//Date[RightStrength]
							,swingLo[1,3]//Time[RightStrength]
							,swingLo[1,1]-(minUnitPriceChange*1)//SwingLoObject[RightStrength]-(minUnitPriceChange*1)
							,"BarNum: "+numToStr(swingLo[1,0],0)
							 );
					text_SetStyle(plotSwingLoTxt,2,0);
					text_setcolor(plotSwingLoTxt,RGB(255,128,0));
				end;
			end; 

			rightStengthCounter = rightStengthCounter + 1 ;
		end; 
		
		BarsAgoSLoBar = seriesBarNumber-SwingLoAtBarNum;
		BarsAgoPriorSLoBar = seriesBarNumber-PriorSwingLoAtBarNum;
		rightStengthCounter = 1;
		rightPatternCounter = 0;
		rightLoPatternExist = False; 	
	end;
	
	leftStengthCounter = 1;
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

rateOfChange = (x / x[old] - 1 ) * 100
percentChange = ( (x - x[old]) / x[old]-1 ) * 100

rsiNetChgAvg = (x - x[Length] ) / Length ;
************************************************************************************************** 
**************************************************************************************************}

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

//Determine bar direction. barRecordType = Up(1), Down(-1) or Neutral(0)
var: int barRecordType(0);
If barClosePrice > barOpenPrice then 
	barRecordType = 1
Else If barClosePrice < barOpenPrice then 
	barRecordType = -1
Else begin	
	barRecordType = 0;
end;

var:SwingHi_Size(0),SwingHi_BarCount(0),SwingLo_Size(0),SwingLo_BarCount(0);

SwingHi_Size = swingLo[0,1]-swingHi[1,1];
SwingHi_BarCount = swingLo[0,0]-swingHi[1,0];

SwingLo_Size = swingHi[0,1]-swingLo[1,1];
SwingLo_BarCount = swingHi[0,0]-swingLo[1,0];

//SwingHi_Size = swingLo[0,1]-swingHi[1,1]
//SwingLo_Size = swingHi[0,1]-swingLo[1,1]

//SwingHi_BarCount = swingLo[0,0]-swingHi[1,0]
//SwingLo_BarCount = swingHi[0,0]-swingLo[1,0]

//lastSwingHiBarNum = seriesBarNumber-RightStrength;
//lastSwingLoBarNum = seriesBarNumber-RightStrength;

//SwingHiAtBarNum = seriesBarNumber-RightStrength;
//SwingLoAtBarNum = seriesBarNumber-RightStrength;

//BarsAgoSHiBar = seriesBarNumber-SwingHiAtBarNum
//BarsAgoSLoBar = seriesBarNumber-SwingLoAtBarNum

//seriesBarNumber-RightStrength where seriesBarNumber is CurrentBar + MaxBarsBack + 1


//classify all swings. swingRecordType = Up(1), Down(-1) or Neutral(0)
var: int swingRecordType(0);
If BarsAgoSHiBar = RightStrength + (swingLo[0,0]-swingHi[1,0]) then 
	swingRecordType = 1 
Else If BarsAgoSLoBar = RightStrength + (swingHi[0,0]-swingLo[1,0]) then 
	swingRecordType = -1
Else begin	
	swingRecordType = 0;
end;

//Determine swingSize for swings
var: double swingSize(0);
If BarsAgoSHiBar = RightStrength + (swingLo[0,0]-swingHi[1,0]) then 
	swingSize = swingLo[0,1]-swingHi[1,1]
Else If BarsAgoSLoBar = RightStrength + (swingHi[0,0]-swingLo[1,0]) then 
	swingSize = swingHi[0,1]-swingLo[1,1]
Else begin	
	swingSize = 0;
end;

//classify subset of all swings to determine which swings are > Threshold
//barClass = Up(2), Down(-2) or Neutral(0)
var: swingSizeThreshold(0) 
	,int barClass(0)
	,int BarsAgoClassHi(0)
	,int BarsAgoClassLo(0);
	swingSizeThreshold = (barHighPrice-barLowPrice)*3.0 ; // <--- classify swings greater and less than threshold. Note that this only works for fixed differnece chart types like range bars or momentum bars, not time, tick, etc.
If 		swingRecordType[0] =  1  and BarsAgoSHiBar[0] = RightStrength + (swingLo[0,0]-swingHi[1,0]) then begin //and AbsValue(swingSize) >= swingSizeThreshold
	barClass = 2;
	BarsAgoClassHi = RightStrength + (swingLo[0,0]-swingHi[1,0]); 
end
Else If swingRecordType[0] = -1  and BarsAgoSLoBar[0] = RightStrength + (swingHi[0,0]-swingLo[1,0]) then begin //and AbsValue(swingSize) >= swingSizeThreshold
	barClass = -2;
	BarsAgoClassLo = RightStrength + (swingHi[0,0]-swingLo[1,0]);
end
Else begin	
	barClass = 0;
	BarsAgoClassHi = seriesBarNumber-(swingLo[0,0]-swingHi[1,0]);
	BarsAgoClassLo = seriesBarNumber-(swingHi[0,0]-swingLo[1,0]);
end;



	



var: int num(0), plot_location(0),plot_value(0), barClassStr(""), plot_n_bars_back(0);

num = seriesBarNumber-RightStrength;
plot_location = 105;
//plot_value = BarsAgoSHiBar[0] - RightStrength;//plus or minus n //((CurrentBar + MaxBarsBack + 1)-RightStrength)-SwingHiAtBarNum;

if swingHi[0,0] > swingLo[0,0] then begin 
	plot_n_bars_back =  (swingHi[0,0]-swingHi[1,0]) +RightStrength;//SwingHi_BarCount;
end
else begin
	plot_n_bars_back = (swingLo[0,0]-swingLo[1,0]) +RightStrength;//SwingLo_BarCount;
end;

if swingHi[0,0] > swingLo[0,0] then begin 
	plot_value = SwingHi_BarCount;//BarsAgoSHiBar[0] - RightStrength;//plus or minus n //((CurrentBar + MaxBarsBack + 1)-RightStrength)-SwingHiAtBarNum;
end
else begin
	plot_value = SwingLo_BarCount;//(swingLo[0,0]-swingLo[1,0]) +RightStrength;//SwingLo_BarCount;
end;

//if swingLo[0,0] > swingHi[0,0] or swingHi[0,0] = 0  then
//if swingHi[0,0] > swingLo[0,0] or swingLo[0,0] = 0  then


{
 BarsAgoSHiBar = seriesBarNumber-SwingHiAtBarNum;
 SwingHiAtBarNum = seriesBarNumber-RightStrength;
 seriesBarNumber = CurrentBar + settingMaxBarsBack; 
 settingMaxBarsBack = MaxBarsBack + 1 ;
}	
vars: Swing_Size(0),BarCount(0);

if SwingHiAtBarNum[4] > 0 and SwingLoAtBarNum[4] > 0 then begin		
	if 1 = 0 then begin
		barClassStr = "1";
	end	
	else if seriesBarNumber - swingHi[0,0] = RightStrength + 0 then begin//BarsAgoSHiBar[0] - RightStrength +0= 0 
		barClassStr = "Hi-Entry-P0 "+NumToStr(plot_value,0)+" "+NumToStr(SwingHi_Size,4);
		Swing_Size = SwingHi_Size;
		BarCount = SwingHi_BarCount;
	end
	else if seriesBarNumber - swingHi[0,0] = RightStrength - 1 then begin//BarsAgoSHiBar[0] - (RightStrength -1)= 0 then begin
		barClassStr = "Hi-Entry-M1 "+NumToStr(plot_value,0)+" "+NumToStr(SwingHi_Size,4);
		Swing_Size = SwingHi_Size;
		BarCount = SwingHi_BarCount;
	end
	else if seriesBarNumber - swingHi[0,0] = RightStrength + 1 then begin//BarsAgoSHiBar[0] - (RightStrength +1)= 0 then begin
		barClassStr = "Hi-Entry-P1 "+NumToStr(plot_value,0)+" "+NumToStr(SwingHi_Size,4);
		Swing_Size = SwingHi_Size;
		BarCount = SwingHi_BarCount;
	end
	else if seriesBarNumber - swingHi[0,0] = RightStrength + 2 then begin//BarsAgoSHiBar[0] - (RightStrength +2)= 0 then begin
		barClassStr = "Hi-Entry-P2 "+NumToStr(plot_value,0)+" "+NumToStr(SwingHi_Size,4);
		Swing_Size = SwingHi_Size;
		BarCount = SwingHi_BarCount;
	end
	else if seriesBarNumber - swingHi[0,0] = RightStrength + 3 then begin//BarsAgoSHiBar[0] - (RightStrength +3)= 0 then begin
		barClassStr = "Hi-Entry-P3 "+NumToStr(plot_value,0)+" "+NumToStr(SwingHi_Size,4);
		Swing_Size = SwingHi_Size;
		BarCount = SwingHi_BarCount;
	end
	
	else if seriesBarNumber - swingLo[0,0] = RightStrength + 0 then begin//BarsAgoSLoBar[0] - RightStrength +0= 0 then begin
		barClassStr = "Lo-Entry-P0 "+NumToStr(plot_value,0)+" "+NumToStr(SwingLo_Size,4);
		Swing_Size = SwingLo_Size;
		BarCount = SwingLo_BarCount;
	end
	else if seriesBarNumber - swingLo[0,0] = RightStrength - 1 then begin//BarsAgoSLoBar[0] - (RightStrength -1)= 0 then begin
		barClassStr = "Lo-Entry-M1 "+NumToStr(plot_value,0)+" "+NumToStr(SwingLo_Size,4);
		Swing_Size = SwingLo_Size;
		BarCount = SwingLo_BarCount;
	end
	else if seriesBarNumber - swingLo[0,0] = RightStrength + 1 then begin//BarsAgoSLoBar[0] - (RightStrength +1)= 0 then begin
		barClassStr = "Lo-Entry-P1 "+NumToStr(plot_value,0)+" "+NumToStr(SwingLo_Size,4);
		Swing_Size = SwingLo_Size;
		BarCount = SwingLo_BarCount;
	end
	else if seriesBarNumber - swingLo[0,0] = RightStrength + 2 then begin//BarsAgoSLoBar[0] - (RightStrength +2)= 0 then begin
		barClassStr = "Lo-Entry-P2 "+NumToStr(plot_value,0)+" "+NumToStr(SwingLo_Size,4);
		Swing_Size = SwingLo_Size;
		BarCount = SwingLo_BarCount;
	end
	else if seriesBarNumber - swingLo[0,0] = RightStrength + 3 then begin//BarsAgoSLoBar[0] - (RightStrength +3)= 0 then begin
		barClassStr = "Lo-Entry-P3 "+NumToStr(plot_value,0)+" "+NumToStr(SwingLo_Size,4);
		Swing_Size = SwingLo_Size;
		BarCount = SwingLo_BarCount;
	end
	else begin
		barClassStr = "0";
		Swing_Size = 0;
		BarCount = 0;
	end;	
end;

	//plot bar numbers
	if ShowBarNumbers and swingLo[1,1] > 0 and swingHi[1,1] > 0 then begin
		if mod(num,10) = 0 then begin
			plotBarNumTxt = text_new(Date[plot_n_bars_back]
					,Time[plot_n_bars_back]
					,round(SwingHiObject[RightStrength],2)-(minUnitPriceChange * plot_location)
					,barClassStr//numToStr(plot_value,0)
						);
			Text_SetStyle(plotBarNumTxt,2,1);
			text_setcolor(plotBarNumTxt,RGB(225,28,90)); 
			end
		else If mod(num[1],10) = 0 then begin
			plotBarNumTxt = text_new(Date[plot_n_bars_back]
					,Time[plot_n_bars_back]
					,round(SwingHiObject[RightStrength],2)-(minUnitPriceChange * (plot_location+10))
					,barClassStr//numToStr(plot_value,0)
						);
			Text_SetStyle(plotBarNumTxt,2,1);
			text_setcolor(plotBarNumTxt,RGB(225,28,90)); 
			end
		else If mod(num[2],10) = 0 then begin
			plotBarNumTxt = text_new(Date[plot_n_bars_back]
					,Time[plot_n_bars_back]
					,round(SwingHiObject[RightStrength],2)-(minUnitPriceChange * (plot_location+20))
					,barClassStr//numToStr(plot_value,0)
						);
			Text_SetStyle(plotBarNumTxt,2,1);
			text_setcolor(plotBarNumTxt,RGB(225,28,90)); 
			end
		else If mod(num[3],10) = 0 then begin
			plotBarNumTxt = text_new(Date[plot_n_bars_back]
					,Time[plot_n_bars_back]
					,round(SwingHiObject[RightStrength],2)-(minUnitPriceChange * (plot_location+30))
					,barClassStr//numToStr(plot_value,0)
						);
			Text_SetStyle(plotBarNumTxt,2,1);
			text_setcolor(plotBarNumTxt,RGB(225,28,90)); 
			end
		else If mod(num[4],10) = 0 then begin
			plotBarNumTxt = text_new(Date[plot_n_bars_back]
					,Time[plot_n_bars_back]
					,round(SwingHiObject[RightStrength],2)-(minUnitPriceChange * (plot_location+40))
					,barClassStr//numToStr(plot_value,0)
						);
			Text_SetStyle(plotBarNumTxt,2,1);
			text_setcolor(plotBarNumTxt,RGB(225,28,90)); 
			end
		else If mod(num[5],10) = 0 then begin
			plotBarNumTxt = text_new(Date[plot_n_bars_back]
					,Time[plot_n_bars_back]
					,round(SwingHiObject[RightStrength],2)-(minUnitPriceChange * (plot_location+50))
					,barClassStr//numToStr(plot_value,0)
						);
			Text_SetStyle(plotBarNumTxt,2,1);
			text_setcolor(plotBarNumTxt,RGB(225,28,90)); 
			end
		else If mod(num[6],10) = 0 then begin
			plotBarNumTxt = text_new(Date[plot_n_bars_back]
					,Time[plot_n_bars_back]
					,round(SwingHiObject[RightStrength],2)-(minUnitPriceChange * (plot_location+60))
					,barClassStr//numToStr(plot_value,0)
						);
			Text_SetStyle(plotBarNumTxt,2,1);
			text_setcolor(plotBarNumTxt,RGB(225,28,90)); 
			end
		else If mod(num[7],10) = 0 then begin
			plotBarNumTxt = text_new(Date[plot_n_bars_back]
					,Time[plot_n_bars_back]
					,round(SwingHiObject[RightStrength],2)-(minUnitPriceChange * (plot_location+70))
					,barClassStr//numToStr(plot_value,0)
						);
			Text_SetStyle(plotBarNumTxt,2,1);
			text_setcolor(plotBarNumTxt,RGB(225,28,90)); 
			end
		else If mod(num[8],10) = 0 then begin
			plotBarNumTxt = text_new(Date[plot_n_bars_back]
					,Time[plot_n_bars_back]
					,round(SwingHiObject[RightStrength],2)-(minUnitPriceChange * (plot_location+80))
					,barClassStr//numToStr(plot_value,0)
						);
			Text_SetStyle(plotBarNumTxt,2,1);
			text_setcolor(plotBarNumTxt,RGB(225,28,90)); 
			end
		else If mod(num[9],10) = 0 then begin
			plotBarNumTxt = text_new(Date[plot_n_bars_back]
					,Time[plot_n_bars_back]
					,round(SwingHiObject[RightStrength],2)-(minUnitPriceChange * (plot_location+90))
					,barClassStr//numToStr(plot_value,0)
						);
			Text_SetStyle(plotBarNumTxt,2,1);
			text_setcolor(plotBarNumTxt,RGB(225,28,90)); 
			end
		else begin
			plotBarNumTxt = text_new(Date[plot_n_bars_back]
					,Time[plot_n_bars_back]
					,round(SwingHiObject[RightStrength],1)-(minUnitPriceChange * 105)
					,"[0]"
						);
			Text_SetStyle(plotBarNumTxt,2,1);
			text_setcolor(plotBarNumTxt,RGB(225,28,90)); 
		
		end;
	end;



	//*****************************************************
	//************ AC oscillator ********************* 
	//*****************************************************
	If CurrentBar > plot_n_bars_back  Then Begin
		//call indicator function ********************
		Var: double MaTEMA_SMALL(0), double MaTEMA_MED(0), double MaTEMA_BIG(0), double MaTEMA_BX(0),MaTEMA_BXL(0);
			MaTEMA_SMALL = averagefc(Close,indLengthA);
			MaTEMA_MED = averagefc(Close,indLengthB);
			MaTEMA_BIG = averagefc(Close,indLengthC);
			MaTEMA_BX = averagefc(Close,indLengthD);
			MaTEMA_BXL = averagefc(Close,indLengthE);

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
	//rateOfChange = (x / x[old] - 1 ) * 100
	//percentChange = ( (x - x[old]) / x[old]-1 ) * 100
{	If MaTEMAindSMALL[5] > 0 and MaTEMAindMED[5]  > 0 and MaTEMAindBIG[5]  > 0 and MaTEMAindBX[5]  > 0 and MaTEMAindBXL[5]  > 0 then begin
		Plot10(round((MaTEMAindSMALL[0]-MaTEMAindSMALL[5])/MaTEMAindSMALL[5]-1,5),"MaTEMASMALL");
		Plot20(round((MaTEMAindMED[0]-MaTEMAindMED[5])/MaTEMAindMED[5]-1,5),"MaTEMAindMED");
		Plot30(round((MaTEMAindBIG[0]-MaTEMAindBIG[5])/MaTEMAindBIG[5]-1,5),"MaTEMAindBIG");
		Plot40(round((MaTEMAindBX[0]-MaTEMAindBX[5])/MaTEMAindBX[5]-1,5),"MaTEMAindBX");
		Plot50(round((MaTEMAindBXL[0]-MaTEMAindBXL[5])/MaTEMAindBXL[5]-1,5),"MaTEMAindBXL");
	end;}
var: int blah(0);
blah = 0;


if Currentbar > plot_n_bars_back then begin	
	Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\EasyLanguage\blah.csv")
		,"",NumToStr(Currentbar,0)
		,",",Numtostr(seriesBarNumber,0)
		,",",Numtostr(plot_n_bars_back,0)
		,","+barClassStr
		,",",Numtostr(Swing_Size,5)
		,",",Numtostr(BarCount,0)
		,","+FormatDate( "yyyy-MM-dd", ElDateToDateTime( Date[plot_n_bars_back] ) )
		,","+FormatTime("HH",DateToJulian(Date[plot_n_bars_back])+(TimeToMinutes(Time[plot_n_bars_back]) / 60 / 24 ))+":"+FormatTime("mm",DateToJulian(Date[plot_n_bars_back])+(TimeToMinutes(Time[plot_n_bars_back]) / 60 / 24 ))+":"+FormatTime("ss",DateToJulian(Date[plot_n_bars_back])+(TimeToMinutes(Time[plot_n_bars_back]) / 60 / 24 ))
		,",",Numtostr(Open[plot_n_bars_back],8)
		,",",Numtostr(High[plot_n_bars_back],8)
		,",",Numtostr(Low[plot_n_bars_back],8)
		,",",Numtostr(Close[plot_n_bars_back],8)
		,",",NumToStr(MaTEMAindSMALL[plot_n_bars_back],5)
		,",",NumToStr(MaTEMAindMED[plot_n_bars_back],5)
		,",",NumToStr(MaTEMAindBIG[plot_n_bars_back],5)
		,",",NumToStr(MaTEMAindBX[plot_n_bars_back],5)
		,",",NumToStr(MaTEMAindBXL[plot_n_bars_back],5)
		//+ NewLine
		);
	end;

 if Currentbar > plot_n_bars_back and swingHi[1,0] > 0 and swingLo[1,0] > 0 then begin	
	Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\EasyLanguage\blah_BarCounts.csv")
		,""+barClassStr
		,",",Numtostr(seriesBarNumber,0)// seriesBarNumber = CurrentBar + settingMaxBarsBack; 
		,",",Numtostr(RightStrength,0)
		,",",Numtostr(plot_n_bars_back,0) //(swingHi[0,0]-swingHi[1,0])+RightStrength; or (swingLo[0,0]-swingLo[1,0])+RightStrength;
		,",",Numtostr(swingHi[0,0],0)
		,",",Numtostr(swingHi[1,0],0)
		,",",Numtostr(swingLo[0,0],0)
		,",",Numtostr(swingLo[1,0],0)
		,",",Numtostr(PriorSwingHiAtBarNum,0)
		,",",Numtostr(BarsAgoPriorSHiBar,0)
		,",",Numtostr(PriorSwingLoAtBarNum,0)
		,",",Numtostr(BarsAgoPriorSLoBar,0)
		,",",Numtostr(BarsAgoSHiBar,0)//BarsAgoSHiBar = seriesBarNumber-SwingHiAtBarNum;
		,",",Numtostr(BarsAgoSLoBar,0)// BarsAgoSLoBar = seriesBarNumber-SwingLoAtBarNum;
		,",",Numtostr(SwingHiAtBarNum,0)//SwingHiAtBarNum = seriesBarNumber-RightStrength;
		,",",Numtostr(SwingLoAtBarNum,0)//SwingLoAtBarNum = seriesBarNumber-RightStrength;
		,",",Numtostr(BarsAgoSHiBar[0] - RightStrength,0)
		,",",Numtostr(BarsAgoSLoBar[0] - RightStrength,0)
		,",",Numtostr(BarsAgoClassHi,0)//BarsAgoClassHi = seriesBarNumber-(swingLo[0,0]-swingHi[1,0]);
		,",",Numtostr(BarsAgoClassLo,0)//BarsAgoClassLo = seriesBarNumber-(swingHi[0,0]-swingLo[1,0]);
		,",",Numtostr(Swing_Size,5)
		,",",Numtostr(BarCount,0)//SwingHi_BarCount = swingLo[0,0]-swingHi[1,0]; or SwingLo_BarCount = swingHi[0,0]-swingLo[1,0];
		,","+FormatDate( "yyyy-MM-dd", ElDateToDateTime( Date[plot_n_bars_back] ) )
		,","+FormatTime("HH",DateToJulian(Date[plot_n_bars_back])+(TimeToMinutes(Time[plot_n_bars_back]) / 60 / 24 ))+":"+FormatTime("mm",DateToJulian(Date[plot_n_bars_back])+(TimeToMinutes(Time[plot_n_bars_back]) / 60 / 24 ))+":"+FormatTime("ss",DateToJulian(Date[plot_n_bars_back])+(TimeToMinutes(Time[plot_n_bars_back]) / 60 / 24 ))
		,",",Numtostr(Open[plot_n_bars_back],8)
		,",",Numtostr(High[plot_n_bars_back],8)
		,",",Numtostr(Low[plot_n_bars_back],8)
		,",",Numtostr(Close[plot_n_bars_back],8)
		,",",NumToStr(Currentbar,0)
		//+ NewLine
		);
	end;
	
