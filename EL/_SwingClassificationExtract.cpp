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
	while leftStengthCounter <= LeftStrength AND leftPatternCounter <= LeftStrength begin
		//detect left pattern of SwingHi
		leftHiPatternExist = (SwingHiObject[RightStrength] >= SwingHiObject[RightStrength+leftStengthCounter]);

		if leftHiPatternExist then
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
				swingBarHiCount = seriesBarNumber-RightStrength-swingLo[0,0];	
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
								,"Size: +"+numToStr(swingLo[0,1]-swingHi[1,1],4)+" / "+numToStr((swingLo[0,0]-swingHi[1,0]),0)+" bars"
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

		if leftLoPatternExist then
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
				swingBarLoCount = (swingHi[0,0])-(swingLo[1,0]);
				lastSwingLoBarNum = seriesBarNumber-RightStrength;

				for counterSH = 0 to 49 begin
					swingLo[50-counterSH,0] = swingLo[50-counterSH-1,0];
					swingLo[50-counterSH,1] = swingLo[50-counterSH-1,1];
					swingLo[50-counterSH,2] = swingLo[50-counterSH-1,2];
					swingLo[50-counterSH,3] = swingLo[50-counterSH-1,3];
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

	
	//*****************************************************
	//************ Weighted Close ********************* 
	//*****************************************************
	
	//*****************************************************
	//************ Williams Accumulation/Distribution  ********************* 
	//*****************************************************
	
	//*****************************************************
	//************ Elder's Force Index (EFI) ********************* 
	//*****************************************************
	//http://www.davenewberg.com/Trading/EhlersCodes.html
	//https://www.tradingview.com/stock-charts-support/index.php/Elder%27s_Force_Index_(EFI)
	//http://www.multicharts.com/support/base/?action=article&id=1256
	
	//fisher transform
	//phase / sine wave - http://www.davenewberg.com/Trading/EhlersCodes.html
	//Fractals
	//Least Squares MA
	
	
	vars:
	bool cond001(false),	bool cond021(false),	bool cond041(false),	bool cond061(false),	bool cond081(false),	bool cond101(false),	bool cond121(false),	bool cond141(false),	bool cond161(false),	bool cond181(false),
	bool cond002(false),	bool cond022(false),	bool cond042(false),	bool cond062(false),	bool cond082(false),	bool cond102(false),	bool cond122(false),	bool cond142(false),	bool cond162(false),	bool cond182(false),
	bool cond003(false),	bool cond023(false),	bool cond043(false),	bool cond063(false),	bool cond083(false),	bool cond103(false),	bool cond123(false),	bool cond143(false),	bool cond163(false),	bool cond183(false),
	bool cond004(false),	bool cond024(false),	bool cond044(false),	bool cond064(false),	bool cond084(false),	bool cond104(false),	bool cond124(false),	bool cond144(false),	bool cond164(false),	bool cond184(false),
	bool cond005(false),	bool cond025(false),	bool cond045(false),	bool cond065(false),	bool cond085(false),	bool cond105(false),	bool cond125(false),	bool cond145(false),	bool cond165(false),	bool cond185(false),
	bool cond006(false),	bool cond026(false),	bool cond046(false),	bool cond066(false),	bool cond086(false),	bool cond106(false),	bool cond126(false),	bool cond146(false),	bool cond166(false),	bool cond186(false),
	bool cond007(false),	bool cond027(false),	bool cond047(false),	bool cond067(false),	bool cond087(false),	bool cond107(false),	bool cond127(false),	bool cond147(false),	bool cond167(false),	bool cond187(false),
	bool cond008(false),	bool cond028(false),	bool cond048(false),	bool cond068(false),	bool cond088(false),	bool cond108(false),	bool cond128(false),	bool cond148(false),	bool cond168(false),	bool cond188(false),
	bool cond009(false),	bool cond029(false),	bool cond049(false),	bool cond069(false),	bool cond089(false),	bool cond109(false),	bool cond129(false),	bool cond149(false),	bool cond169(false),	bool cond189(false),
	bool cond010(false),	bool cond030(false),	bool cond050(false),	bool cond070(false),	bool cond090(false),	bool cond110(false),	bool cond130(false),	bool cond150(false),	bool cond170(false),	bool cond190(false),
	bool cond011(false),	bool cond031(false),	bool cond051(false),	bool cond071(false),	bool cond091(false),	bool cond111(false),	bool cond131(false),	bool cond151(false),	bool cond171(false),	bool cond191(false),
	bool cond012(false),	bool cond032(false),	bool cond052(false),	bool cond072(false),	bool cond092(false),	bool cond112(false),	bool cond132(false),	bool cond152(false),	bool cond172(false),	bool cond192(false),
	bool cond013(false),	bool cond033(false),	bool cond053(false),	bool cond073(false),	bool cond093(false),	bool cond113(false),	bool cond133(false),	bool cond153(false),	bool cond173(false),	bool cond193(false),
	bool cond014(false),	bool cond034(false),	bool cond054(false),	bool cond074(false),	bool cond094(false),	bool cond114(false),	bool cond134(false),	bool cond154(false),	bool cond174(false),	bool cond194(false),
	bool cond015(false),	bool cond035(false),	bool cond055(false),	bool cond075(false),	bool cond095(false),	bool cond115(false),	bool cond135(false),	bool cond155(false),	bool cond175(false),	bool cond195(false),
	bool cond016(false),	bool cond036(false),	bool cond056(false),	bool cond076(false),	bool cond096(false),	bool cond116(false),	bool cond136(false),	bool cond156(false),	bool cond176(false),	bool cond196(false),
	bool cond017(false),	bool cond037(false),	bool cond057(false),	bool cond077(false),	bool cond097(false),	bool cond117(false),	bool cond137(false),	bool cond157(false),	bool cond177(false),	bool cond197(false),
	bool cond018(false),	bool cond038(false),	bool cond058(false),	bool cond078(false),	bool cond098(false),	bool cond118(false),	bool cond138(false),	bool cond158(false),	bool cond178(false),	bool cond198(false),
	bool cond019(false),	bool cond039(false),	bool cond059(false),	bool cond079(false),	bool cond099(false),	bool cond119(false),	bool cond139(false),	bool cond159(false),	bool cond179(false),	bool cond199(false),
	bool cond020(false),	bool cond040(false),	bool cond060(false),	bool cond080(false),	bool cond100(false),	bool cond120(false),	bool cond140(false),	bool cond160(false),	bool cond180(false),	bool cond200(false),
	bool cond201(false),	bool cond241(false),	bool cond281(false),	bool cond321(false),	bool cond361(false),
	bool cond202(false),	bool cond242(false),	bool cond282(false),	bool cond322(false),	bool cond362(false),
	bool cond203(false),	bool cond243(false),	bool cond283(false),	bool cond323(false),	bool cond363(false),
	bool cond204(false),	bool cond244(false),	bool cond284(false),	bool cond324(false),	bool cond364(false),
	bool cond205(false),	bool cond245(false),	bool cond285(false),	bool cond325(false),	bool cond365(false),
	bool cond206(false),	bool cond246(false),	bool cond286(false),	bool cond326(false),	bool cond366(false),
	bool cond207(false),	bool cond247(false),	bool cond287(false),	bool cond327(false),	bool cond367(false),
	bool cond208(false),	bool cond248(false),	bool cond288(false),	bool cond328(false),	bool cond368(false),
	bool cond209(false),	bool cond249(false),	bool cond289(false),	bool cond329(false),	bool cond369(false),
	bool cond210(false),	bool cond250(false),	bool cond290(false),	bool cond330(false),	bool cond370(false),
	bool cond211(false),	bool cond251(false),	bool cond291(false),	bool cond331(false),	bool cond371(false),
	bool cond212(false),	bool cond252(false),	bool cond292(false),	bool cond332(false),	bool cond372(false),
	bool cond213(false),	bool cond253(false),	bool cond293(false),	bool cond333(false),	bool cond373(false),
	bool cond214(false),	bool cond254(false),	bool cond294(false),	bool cond334(false),	bool cond374(false),
	bool cond215(false),	bool cond255(false),	bool cond295(false),	bool cond335(false),	bool cond375(false),
	bool cond216(false),	bool cond256(false),	bool cond296(false),	bool cond336(false),	bool cond376(false),
	bool cond217(false),	bool cond257(false),	bool cond297(false),	bool cond337(false),	bool cond377(false),
	bool cond218(false),	bool cond258(false),	bool cond298(false),	bool cond338(false),	bool cond378(false),
	bool cond219(false),	bool cond259(false),	bool cond299(false),	bool cond339(false),	bool cond379(false),
	bool cond220(false),	bool cond260(false),	bool cond300(false),	bool cond340(false),	bool cond380(false),
	bool cond221(false),	bool cond261(false),	bool cond301(false),	bool cond341(false),	bool cond381(false),
	bool cond222(false),	bool cond262(false),	bool cond302(false),	bool cond342(false),	bool cond382(false),
	bool cond223(false),	bool cond263(false),	bool cond303(false),	bool cond343(false),	bool cond383(false),
	bool cond224(false),	bool cond264(false),	bool cond304(false),	bool cond344(false),	bool cond384(false),
	bool cond225(false),	bool cond265(false),	bool cond305(false),	bool cond345(false),	bool cond385(false),
	bool cond226(false),	bool cond266(false),	bool cond306(false),	bool cond346(false),	bool cond386(false),
	bool cond227(false),	bool cond267(false),	bool cond307(false),	bool cond347(false),	bool cond387(false),
	bool cond228(false),	bool cond268(false),	bool cond308(false),	bool cond348(false),	bool cond388(false),
	bool cond229(false),	bool cond269(false),	bool cond309(false),	bool cond349(false),	bool cond389(false),
	bool cond230(false),	bool cond270(false),	bool cond310(false),	bool cond350(false),	bool cond390(false),
	bool cond231(false),	bool cond271(false),	bool cond311(false),	bool cond351(false),	bool cond391(false),
	bool cond232(false),	bool cond272(false),	bool cond312(false),	bool cond352(false),	bool cond392(false),
	bool cond233(false),	bool cond273(false),	bool cond313(false),	bool cond353(false),	bool cond393(false),
	bool cond234(false),	bool cond274(false),	bool cond314(false),	bool cond354(false),	bool cond394(false),
	bool cond235(false),	bool cond275(false),	bool cond315(false),	bool cond355(false),	bool cond395(false),
	bool cond236(false),	bool cond276(false),	bool cond316(false),	bool cond356(false),	bool cond396(false),
	bool cond237(false),	bool cond277(false),	bool cond317(false),	bool cond357(false),	bool cond397(false),
	bool cond238(false),	bool cond278(false),	bool cond318(false),	bool cond358(false),	bool cond398(false),
	bool cond239(false),	bool cond279(false),	bool cond319(false),	bool cond359(false),	bool cond399(false),
	bool cond240(false),	bool cond280(false),	bool cond320(false),	bool cond360(false),	bool cond400(false);
	
	
	
	if condition1 then begin
		FileAppend("c:\csv\test.txt"
				,"Appended Text,"
				+"barNum:"+ NumToStr(seriesBarNumber, 0) 
				+",date:"+  dateBarOpened
				+",time:"+  timeBarOpenedhhmmss
				+",settingMaxBarsBack:"+  NumToStr(settingMaxBarsBack,0) 
				+","+  NumToStr(swingHi[0,0],0)//barNum
				+","+  NumToStr(swingLo[0,0],0)//barNum
				+","+  NumToStr(swingHi[0,1],0)//price
				+","+  NumToStr(swingLo[0,1],0)//price
				+","+  NumToStr(swingHi[0,2],0)//date
				+","+  NumToStr(swingLo[0,2],0)//date
				+","+  NumToStr(swingHi[0,3],0)//time
				+","+  NumToStr(swingLo[0,3],0)//time
				+","+  NumToStr(sizeOfLastSwing,0)
				+","+  NumToStr(sHiBarNumForCalc,0 )  			
				+","+  NumToStr(sLoBarNumForCalc ,0)  	
   				+","+  NumToStr(sHiPriceForCalc,0 )
   				+","+  NumToStr(sLoPriceForCalc,0)
				+","+  state
				+ NewLine);  
	end;
	

	if CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 then begin
		//MAX(High)-MIN(Low) for last 240 bars
		var:  int nBars(10),double barRange2Bars(0),double barRange3Bars(0),double barRange5Bars(0)
			,double barRange10Bars(0),double barRange20Bars(0),double barRange40Bars(0),double barRange80Bars(0)
			,double barRange120Bars(0),double barRange185Bars(0),double barRange250Bars(0);
			barRange2Bars = Highest(High,RightStrength+(nBars/2))-Lowest(Low, RightStrength+(nBars/5));
			barRange3Bars = Highest(High,RightStrength+(3))-Lowest(Low, RightStrength+(3));
			barRange5Bars = Highest(High,RightStrength+(nBars/2))-Lowest(Low, RightStrength+(nBars/2));
			barRange10Bars = Highest(High,RightStrength+nBars)-Lowest(Low, RightStrength+nBars);
			barRange20Bars = Highest(High,RightStrength+(nBars*2))-Lowest(Low, RightStrength+(nBars*2));
			barRange40Bars = Highest(High,RightStrength+(nBars*4))-Lowest(Low, RightStrength+(nBars*4));
			barRange80Bars = Highest(High,RightStrength+(nBars*8))-Lowest(Low, RightStrength+(nBars*8));
			barRange120Bars = Highest(High,RightStrength+(nBars*12))-Lowest(Low, RightStrength+(nBars*12));
			barRange185Bars = Highest(High,RightStrength+(nBars*18.5))-Lowest(Low, RightStrength+(nBars*18.5));
			barRange250Bars = Highest(High,RightStrength+(nBars*25))-Lowest(Low, RightStrength+(nBars*25));
			
		//AVG(Close)-MEDIAN(Close) for last 2-250 bars		
		Array: double MyAvgArray[250](0), double MyMedianArray[250](0);

		for counterSH = 0 to 249
		begin
			MyAvgArray[250-counterSH] = MyAvgArray[250-counterSH-1];
			MyMedianArray[250-counterSH] = MyMedianArray[250-counterSH-1];
		end; 
		MyAvgArray[1] = barClosePrice ;
		MyMedianArray[1] = barClosePrice ;
		
		var: double AVGvsMed2Bars(0), double Average2bars(0), double Median2bars(0),
			double AVGvsMed3Bars(0), double Average3bars(0), double Median3bars(0),
			double AVGvsMed5Bars(0), double Average5bars(0), double Median5bars(0),
			double AVGvsMed10Bars(0), double Average10bars(0), double Median10bars(0),
			double AVGvsMed20Bars(0), double Average20bars(0), double Median20bars(0),
			double AVGvsMed40Bars(0), double Average40bars(0), double Median40bars(0),
			double AVGvsMed80Bars(0), double Average80bars(0), double Median80bars(0),
			double AVGvsMed120Bars(0), double Average120bars(0), double Median120bars(0),
			double AVGvsMed185Bars(0), double Average185bars(0), double Median185bars(0),
			double AVGvsMed250Bars(0), double Average250bars(0), double Median250bars(0);
			
		Average2bars = AverageArray(MyAvgArray, 2);
		Average3bars = AverageArray(MyAvgArray, 3);
		Average5bars = AverageArray(MyAvgArray, 5);
		Average10bars = AverageArray(MyAvgArray, 10); //avg of last 10 bars
		Average20bars = AverageArray(MyAvgArray, 20);
		Average40bars = AverageArray(MyAvgArray, 40);
		Average80bars = AverageArray(MyAvgArray, 80);
		Average120bars = AverageArray(MyAvgArray, 120);
		Average185bars = AverageArray(MyAvgArray, 185);
		Average250bars = AverageArray(MyAvgArray, 250);
		Median2bars = MedianArray(MyMedianArray, 2);
		Median3bars = MedianArray(MyMedianArray, 3);
		Median5bars = MedianArray(MyMedianArray, 5);
		Median10bars = MedianArray(MyMedianArray, 10); //median of last 10 bars
		Median20bars = MedianArray(MyMedianArray, 20);
		Median40bars = MedianArray(MyMedianArray, 40);
		Median80bars = MedianArray(MyMedianArray, 80);
		Median120bars = MedianArray(MyMedianArray, 120);
		Median185bars = MedianArray(MyMedianArray, 185);
		Median250bars = MedianArray(MyMedianArray, 250);
		AVGvsMed2Bars =  Median2bars - Average2bars;
		AVGvsMed3Bars = Median3bars - Average3bars;
		AVGvsMed5Bars = Median5bars - Average5bars;
		AVGvsMed10Bars = Median10bars - Average10bars; //AVG(Close)-MEDIAN(Close) for last 10 bars
		AVGvsMed20Bars = Median20bars - Average20bars;
		AVGvsMed40Bars = Median40bars - Average40bars;
		AVGvsMed80Bars = Median80bars - Average80bars;
		AVGvsMed120Bars = Median120bars - Average120bars;
		AVGvsMed185Bars = Median185bars - Average185bars;
		AVGvsMed250Bars = Median250bars - Average250bars;

		//find higher swing hi's 
		var: int sHiTrendCounter(0),
			bool sHiPatternExist(False),
			int swingHiTrend(0);
			
		if swingHi[0,1] < swingHi[1,1] then begin
			swingHiTrend = 0;
		end
		Else begin
			while sHiTrendCounter <= 20  begin
				sHiPatternExist = swingHi[sHiTrendCounter,1] >= swingHi[sHiTrendCounter+1,1];//detect pattern
			
				if sHiPatternExist then begin
						swingHiTrend = sHiTrendCounter + 1;
						sHiTrendCounter = sHiTrendCounter + 1;
					end
					Else begin
						sHiTrendCounter = 21;
					End;
				
			end;
			sHiTrendCounter = 0;
			sHiPatternExist = False;	
		End;		

		//find lower swing lo's 
		var: int sLoTrendCounter(0),
			bool sLoPatternExist(False),
			int swingLoTrend(0);
			
		if swingLo[0,1] > swingLo[1,1] then begin
			swingLoTrend = 0;
		end
		Else begin
			while sLoTrendCounter <= 20  begin
				sLoPatternExist = swingLo[sLoTrendCounter,1] <= swingLo[sLoTrendCounter+1,1];//detect pattern
			
				if sLoPatternExist then begin
						swingLoTrend = sLoTrendCounter + 1;
						sLoTrendCounter = sLoTrendCounter + 1;
					end
					Else begin
						sLoTrendCounter = 21;
					End;
				
			end;
			sLoTrendCounter = 0;
			sLoPatternExist = False;
		End;
		
		//find higher swing hi's and higher swing lo's
		var: int altSHiTrendCounter(0),
			bool altSHiPatternExist(False),
			int altSwingUpTrend(0);
			
		if swingHi[0,1] < swingHi[1,1] then begin
			altSwingUpTrend = 0;
		end
		Else begin
			while altSHiTrendCounter <= 20  begin
				altSHiPatternExist = swingHi[altSHiTrendCounter,1] >= swingHi[altSHiTrendCounter+1,1] and //rising hi's
									 swingLo[altSHiTrendCounter,1] >= swingLo[altSHiTrendCounter+1,1]; //rising lo's
									 
			
				if altSHiPatternExist then begin
						altSwingUpTrend = altSHiTrendCounter + 1;
						altSHiTrendCounter = altSHiTrendCounter + 1;
					end
					Else begin
						altSHiTrendCounter = 21;
					End;
				
			end;
			altSHiTrendCounter = 0;
			altSHiPatternExist = False;	
		End;		

		//find lower swing lo's and lower swing hi's
		var: int altSLoTrendCounter(0),
			bool altSLoPatternExist(False),
			int altSwingDnTrend(0);

		if swingLo[0,1] > swingLo[1,1] then begin
			altSwingDnTrend = 0;
		end
		Else begin
			while altSLoTrendCounter <= 20  begin
				altSLoPatternExist = swingHi[altSLoTrendCounter,1] <= swingHi[altSLoTrendCounter+1,1] and //lower hi's
									 swingLo[altSLoTrendCounter,1] <= swingLo[altSLoTrendCounter+1,1]; //lower lo's
									 
			
				if altSLoPatternExist then begin
						altSwingDnTrend = altSLoTrendCounter + 1;
						altSLoTrendCounter = altSLoTrendCounter + 1;
					end
					Else begin
						altSLoTrendCounter = 21;
					End;
				
			end;
			altSLoTrendCounter = 0;
			altSLoPatternExist = False;	
		End;

		//find monotonic high bars 
		var: int monoHiCounter(0),
			bool monoHiPatternExist(False),
			int monotonicHiBars(0);
			
		if High[RightStrength] < High[1+RightStrength] then begin
			monotonicHiBars = 0;
		end
		Else begin
			while monoHiCounter <= 20  begin
				monoHiPatternExist = High[monoHiCounter+RightStrength] >= High[monoHiCounter+1+RightStrength]; //increasing monotonic bars 
			
				if monoHiPatternExist then begin
						monotonicHiBars = monoHiCounter + 1;
						monoHiCounter = monoHiCounter + 1;
					end
					Else begin
						monoHiCounter = 21;
					End;
				
			end;
			monoHiCounter = 0;
			monoHiPatternExist = False;	
		End;	

		//find monotonic low bars 
		var: int monoLoCounter(0),
			bool monoLoPatternExist(False),
			int monotonicLoBars(0);
		
		if Low[RightStrength] > Low[1+RightStrength] then begin
			monotonicLoBars = 0;
		end
		Else begin
			while monoLoCounter <= 20  begin
				monoLoPatternExist = Low[monoLoCounter+RightStrength] <= Low[monoLoCounter+1+RightStrength]; //decreasing monotonic bars 
			
				if monoLoPatternExist then begin
						monotonicLoBars = monoLoCounter + 1;
						monoLoCounter = monoLoCounter + 1;
					end
					Else begin
						monoLoCounter = 21;
					End;
				
			end;
			monoLoCounter = 0;
			monoLoPatternExist = False;	
		End;


		//categorize BarsAgoSHiBar 
		Var: double scaleBASHiB(0), double valueBucketBASHiB(0), double intervalValue.4(4);
		valueBucketBASHiB = BucketData(BarsAgoSHiBar,intervalValue.4,intervalLimit); //BarsAgoSHiBar;

		//categorize BarsAgoSLoBar 
		Var: double scaleBASLoB(0), double valueBucketBASLoB(0);
		valueBucketBASLoB = BucketData(BarsAgoSLoBar,intervalValue.4,intervalLimit);//BarsAgoSLoBar;
		
		//Determine bar direction. barRecordType = Up(1), Down(-1) or Neutral(0)
		var: int barRecordType(0);
		If barClosePrice > barOpenPrice then 
			barRecordType = 1
		Else If barClosePrice < barOpenPrice then 
			barRecordType = -1
		Else begin	
			barRecordType = 0;
		end;
		
		//classify all swings
		var: int swingRecordType(0);
		If BarsAgoSHiBar = RightStrength then 
			swingRecordType = 1
		Else If BarsAgoSLoBar = RightStrength then 
			swingRecordType = -1
		Else begin	
			swingRecordType = 0;
		end;

		//Determine swingSize for swings
		var: double swingSize(0);
		If BarsAgoSHiBar = RightStrength then 
			swingSize = High[RightStrength]-Lowest(Low, RightStrength) //Low
		Else If BarsAgoSLoBar = RightStrength then 
			swingSize = Highest(High,RightStrength)-Low[RightStrength] //High
		Else If (BarsAgoSHiBar <> RightStrength AND BarsAgoSLoBar <> RightStrength) AND (High[RightStrength]-Lowest(Low, RightStrength)) > (Highest(High,RightStrength)-Low[RightStrength]) then
			swingSize = High[RightStrength]-Lowest(Low, RightStrength)  
		Else If (BarsAgoSHiBar <> RightStrength AND BarsAgoSLoBar <> RightStrength) AND (Highest(High,RightStrength)-Low[RightStrength]) > (High[RightStrength]-Lowest(Low, RightStrength)) then
			swingSize = Highest(High,RightStrength)-Low[RightStrength] 
		Else If swingRecordType = 0 AND (High[RightStrength]-Lowest(Low, RightStrength)) >= (Highest(High,RightStrength)-Low[RightStrength]) then
			swingSize = High[RightStrength]-Lowest(Low, RightStrength)  
		Else If swingRecordType = 0 AND (Highest(High,RightStrength)-Low[RightStrength]) > (High[RightStrength]-Lowest(Low, RightStrength)) then
			swingSize = Highest(High,RightStrength)-Low[RightStrength] 
		Else begin	
			swingSize = 1234567890;
		end;
		
		//classify subset of all swings to determine which swings are "large" 
		var: swingSizeThreshold(0) 
			,int barClass(0)
			,int BarsAgoClassHi(0)
			,int BarsAgoClassLo(0);
			swingSizeThreshold = (barHighPrice-barLowPrice)*3.0 ; // <--- classify swings greater and less than threshold. Note that this only works for fixed differnece chart types like range bars or momentum bars, not time, tick, etc.
		If 		swingRecordType[0] =  1 and AbsValue(swingSize) >= swingSizeThreshold and BarsAgoSHiBar[0] = RightStrength then begin
			barClass = 2;
			BarsAgoClassHi = rightStrength; //number of bars since "large" swing high
		end
		Else If swingRecordType[0] = -1 and AbsValue(swingSize) >= swingSizeThreshold and BarsAgoSLoBar[0] = RightStrength then begin
			barClass = -2;
			BarsAgoClassLo = rightStrength; //number of bars since "large" swing low
		end
		Else If swingRecordType[0] = 1 and AbsValue(swingSize) < swingSizeThreshold and BarsAgoSHiBar[0] = RightStrength then begin
			barClass = 0; //barClass = 1 //use "barClass = 0" to reduce numbers of classes from five to three
			BarsAgoClassHi = seriesBarNumber-SwingHiAtBarNum;
		end
		Else If swingRecordType[0] = -1 and AbsValue(swingSize) < swingSizeThreshold and BarsAgoSLoBar[0] = RightStrength then begin
			barClass = 0; //barClass = -1 //use "barClass = 0" to reduce numbers of classes from five to three 
			BarsAgoClassLo = seriesBarNumber-SwingLoAtBarNum;
		end
		Else begin	
			barClass = 0;
			BarsAgoClassHi = seriesBarNumber-SwingHiAtBarNum;
			BarsAgoClassLo = seriesBarNumber-SwingLoAtBarNum;
		end;
		
		//find hour of day for each bar
		var:  hourOfDay(0);
		hourOfDay = StrToNum(LeftStr(timeBarOpenedhhmmss,2));//StrToNum(FormatTime("h",julianDateTime));
		//categorize hourOfDay 
		Var: double bucketHourOfDay(0), double intervalValueHrOfDay(4),double intervalLimitHrOfDay(23);
		bucketHourOfDay = BucketData(hourOfDay,intervalValueHrOfDay,intervalLimitHrOfDay);


		//categorize BarsAgoClassHi 
		Var: double valueBucketBAHiClass(0), double classBucketBASHiB(0), double classIntervalValue(4);
		valueBucketBAHiClass = BucketData(BarsAgoClassHi,classIntervalValue,intervalLimit); //BarsAgoSHiBar;
		
		//categorize BarsAgoClassLo 
		Var: double valueBucketBALoClass(0), double classBucketBASLoB(0);
		valueBucketBALoClass = BucketData(BarsAgoClassLo,classIntervalValue,intervalLimit);//BarsAgoSLoBar;
		
		
		var: barSize(0);
		barSize = barHighPrice-barLowPrice;

	end;


		var:
			string sql_server(""),
			string sql_dbname(""),
			string sql_user(""),
			string sql_password(""),
			string resMeta("0"), 
			string res0("0"), 
			string res1("0"), string res1v02("0"), 
			string res2("0"), string res2v02("0"),
			string res3("0"), string res3v02("0"),
			string res4("0"), string res4v02("0"), 
			string res5("0"),
			string res6("0"), 
			string res7("0"), 
			string res8("0"),
			string res9("0"), 
			string res10("0"), 
			string res11("0"),
			string res5v02("0"),
			string res6v02("0"), 
			string res7v02("0"), 
			string res8v02("0"),
			string res9v02("0"), 
			string res10v02("0"), 
			string res11v02("0"),
			
			string sql_insert_meta(""),
			string sql_insert_features(""), string sql_insert_features0(""),
			string sql_insert_feat1(""),string sql_insert_feat1v02(""), 
			string sql_insert_feat2(""),string sql_insert_feat2v02(""),
			string sql_insert_feat3(""), string sql_insert_feat3v02(""), 
			string sql_insert_feat4(""),string sql_insert_feat4v02(""),
			string sql_insert_feat5(""), string sql_insert_feat5v02(""), 
			string sql_insert_feat6(""), string sql_insert_feat6v02(""),
			string sql_insert_feat7(""), string sql_insert_feat7v02(""), 
			string sql_insert_feat8(""),string sql_insert_feat8v02(""),
			string sql_insert_feat9(""), string sql_insert_feat9v02(""), 
			string sql_insert_feat10(""),string sql_insert_feat10v02(""),
			string sql_insert_feat11(""),string sql_insert_feat11v02(""),
			Int srun_sel_0(0),
			Int srun_sel_1(0),
			Int srun_ins_0(0),
			Int srun_ins_1(0),
			Int srun(0),
			Int srun0(0),
			Int srun1(0),Int srun1v02(0),
			Int srun2(0),Int srun2v02(0),
			Int srun3(0),Int srun3v02(0),
			Int srun4(0),Int srun4v02(0),
			Int srun5(0),Int srun5v02(0),
			Int srun6(0),Int srun6v02(0),
			Int srun7(0),Int srun7v02(0),
			Int srun8(0),Int srun8v02(0),
			Int srun9(0),Int srun9v02(0),
			Int srun10(0),Int srun10v02(0),
			Int srun11(0),Int srun11v02(0),
			string sql_select_2("0"),
			string sql_select_statement("0"),
			string sql_select_datetime("0"),
			string queryResult("0"),
			string queryResultdatetime("0");
			
		//(0) set database connection params
		sql_server = "NLWDESK\MSSQL2016RC3DEV";
		sql_dbname = "daytradinglogic";
		sql_user = "sa";
		sql_password = "H5idvdkb174";
		
		srun_sel_0 = 0;
		srun_sel_1 = 0;
		srun_ins_0 = 0;
		srun_ins_1 = 0;
		srun0 = 0;
		srun1 = 0;
		srun2 = 0;
		srun3 = 0;
		srun4 = 0;
		srun5 = 0;
		srun6 = 0;
		srun7 = 0;
		srun8 = 0;
		srun9 = 0;
		srun10 = 0;
		srun11 = 0;
		srun1v02 = 0;
		srun2v02 = 0;
		srun3v02 = 0;
		srun4v02 = 0;
		srun5v02 = 0;
		srun6v02 = 0;
		srun7v02 = 0;
		srun8v02 = 0;
		srun9v02 = 0;
		srun10v02 = 0;
		srun11v02 = 0;
		
	if ReadWriteToDB and CurrentBar > MaxBarsBack +1 then begin		
		//(1) get max [loadId] from dbo.barFeaturesMetaData
		If srun_sel_0 = 0 AND MaxBarsBack > 1 AND CurrentBar < (MaxBarsBack*1.5) then begin
			sql_select_statement = "SELECT ISNULL(MAX(loadId),0)+1 FROM [dbo].[barFeaturesMetaData]";
			queryResult = execsqlsingle(sql_server, sql_user, sql_password, sql_dbname, sql_select_statement);
			srun_sel_0 = 1;
		end;
	End;

	
vars: int TblNum(0), int MaxTbls(0);
TblNum = 0;
MaxTbls = 22;

While TblNum < MaxTbls + 1 Begin
 
	if ReadWriteToDB and TblNum = 0 and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		//(2) insert meta data into dbo.barFeaturesMetaData
		If srun_ins_0 = 0 AND MaxBarsBack > 1 AND CurrentBar = MaxBarsBack+2 AND CurrentBar < (MaxBarsBack*1.5) then begin
			sql_insert_meta = "INSERT INTO [dbo].[barFeaturesMetaData] VALUES ("
				+queryResult//MAX(loadId)
				+",GETDATE()"//recCreateDateTime
				+",GETDATE()" //recModiedDateTime
				+","+Numtostr(loadGroupId,0)
				+","+"'"+dataSeriesSymbol+"' "//dataSeriesSymbol
				+","+Numtostr(chartType,0)//chartType
				+","+Numtostr(chartInterval,8)//chartInterval
				+","+Numtostr(MaxBarsBack,0) //maxBarsBack
				+","+Numtostr(barSize,8) //barSize 
				+","+Numtostr(minUnitPriceChange,8)//minUnitPriceChange
				+","+Numtostr(LeftStrength,0)//LeftStrength 
				+","+Numtostr(RightStrength,0)//RightStrength 
				+","+Numtostr(leftPad,0)
				+","+Numtostr(rightPad,0)
				+","+Numtostr(leftPadAmt,8)
				+","+Numtostr(rightPadAmt,8)
				+","+Numtostr(swingSizeThreshold,8)//swingSizeThreshold
				+","+Numtostr(intervalValue,0)//indicatorIntervalValue 
				+","+Numtostr(intervalValueLarge,0)
				+","+Numtostr(intervalLimit,0)
				+","+Numtostr(lookBackLength,0)
				+","+Numtostr(scaleLimit,0)
				+","+Numtostr(scaleFloor,0)			
				+","+Numtostr(intervalValueHrOfDay,0)//intervalValueHrOfDay 
				+","+Numtostr(intervalLimitHrOfDay,0)
				+","+Numtostr(intervalValue.4,0)//intervalValueBarsAgoSHiLoBar 
				+","+Numtostr(classIntervalValue,0)//IntervalValueBarsAgoClassHiLo
				+" )";
			resMeta = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_meta);
			srun_ins_0 = 1;
		end;
	End;
	if debug then begin	
			Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeaturesMetaData.csv")
				,",",NumToStr(swingRecordType,0)
				,",",NumToStr(barRecordType,0)
				,",",swingSize
				,"Currentbar = ",Currentbar
				,"***,",sql_insert_meta
				+ NewLine);
	end;
		
	if ReadWriteToDB and CurrentBar > MaxBarsBack +1 then begin	
		//(3) get max [recCreateDateTime] from dbo.barFeaturesMetaData
		If srun_sel_1 = 0 AND MaxBarsBack > 1 AND CurrentBar < (MaxBarsBack*1.5) then begin
			sql_select_datetime = "SELECT ISNULL(MAX(recCreateDateTime),GETDATE()) FROM [dbo].[barFeaturesMetaData]";
			queryResultdatetime = execsqlsingle(sql_server, sql_user, sql_password, sql_dbname, sql_select_datetime);
			srun_sel_1 = 1;
		end; 
	End;
	
		//(4) insert feature data into dbo.barFeatures0	
	if ReadWriteToDB and TblNum = 0 and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin			
		if  srun0 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_features0 = "INSERT INTO [dbo].[barFeatures0] VALUES ("
				+queryResult//MAX(loadId) 
				+","+Numtostr(loadGroupId,0) //+",(floor(rand() * (200 + 1)))"//random number between 0-200
				+","+Numtostr(RightStrength,0)//RightStrength
				+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
				+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
				+","+Numtostr(seriesBarNumber-1,0)
				+","+Numtostr(seriesBarNumber-RightStrength,0)
				+","+Numtostr(lengthOfSeries,0)
				+","+"'"+dateBarOpened+"' "
				+","+"'"+dayOfWeekBarOpened+"' "
				+","+"'"+dateTimeBarOpened+"' "
				+","+"'"+timeBarOpenedhhmmss+"' "
				+","+Numtostr(barOpenPrice,8)
				+","+Numtostr(barHighPrice,8)
				+","+Numtostr(barLowPrice,8)
				+","+Numtostr(barClosePrice,8)
				+","+Numtostr(barClass,0)
				+","+Numtostr(swingRecordType,0)
				+","+Numtostr(barRecordType,0)
				+","+Numtostr(swingSize,8)
				+","+Numtostr(numminutesBarWasOpn ,4)
				+","+Numtostr(DayOfWeek(Date[RightStrength]),0) //0 = Sun, 6 = Sat
				+","+Numtostr(bucketHourOfDay,0)//+","+"'"+FormatTime("htt",julianDateTime)+"' " //hourOfDay
				+","+Numtostr(hourOfDay,0)
				+","+Numtostr(swingHi[0,0],0)//barNum
				+","+Numtostr(swingLo[0,0],0)//barNum
				+","+Numtostr(swingHi[0,1],8)//price
				+","+Numtostr(swingLo[0,1],8)//price
				+","+Numtostr(swingHi[0,2],0)//date
				+","+Numtostr(swingLo[0,2],0)//date
				+","+Numtostr(swingHi[0,3],0)//time
				+","+Numtostr(swingLo[0,3],0)//time
				+","+Numtostr(sHiBarNumForCalc,0)
				+","+Numtostr(sLoBarNumForCalc,0)
				+","+Numtostr(sHiPriceForCalc,8)
				+","+Numtostr(sLoPriceForCalc,8)
				+","+Numtostr(SwingHiAtBarNum,0)
				+","+Numtostr(SwingLoAtBarNum,0)
				+","+Numtostr(swingHiTrend,0)
				+","+Numtostr(swingLoTrend,0)
				+","+Numtostr(altSwingUpTrend,0)
				+","+Numtostr(altSwingDnTrend,0)
				+","+Numtostr(monotonicHiBars,0)
				+","+Numtostr(monotonicLoBars,0)
				+","+Numtostr(BarsAgoSHiBar,0)
				+","+Numtostr(BarsAgoSLoBar,0)
				+","+Numtostr(barRange2Bars ,8)
				+","+Numtostr(barRange3Bars ,8)
				+","+Numtostr(barRange5Bars ,8)
				+","+Numtostr(barRange10Bars ,8)
				+","+Numtostr(barRange20Bars ,8)
				+","+Numtostr(barRange40Bars ,8)
				+","+Numtostr(barRange80Bars ,8)
				+","+Numtostr(barRange120Bars,8)
				+","+Numtostr(barRange185Bars,8)
				+","+Numtostr(barRange250Bars,8)
				+","+Numtostr(AVGvsMed2Bars ,8)
				+","+Numtostr(AVGvsMed3Bars ,8)
				+","+Numtostr(AVGvsMed5Bars ,8)
				+","+Numtostr(AVGvsMed10Bars ,8)
				+","+Numtostr(AVGvsMed20Bars ,8)
				+","+Numtostr(AVGvsMed40Bars ,8)
				+","+Numtostr(AVGvsMed80Bars ,8)
				+","+Numtostr(AVGvsMed120Bars ,8)
				+","+Numtostr(AVGvsMed185Bars ,8)
				+","+Numtostr(AVGvsMed250Bars ,8)
				+","+Numtostr(BarsAgoClassHi,0)
				+","+Numtostr(BarsAgoClassLo,0)
				+","+Numtostr(valueBucketBAHiClass,0)
				+","+Numtostr(valueBucketBALoClass,0)
				+","+Numtostr(valueBucketBASHiB[0],0)
				+","+Numtostr(valueBucketBASLoB[0],0)
				+" )";
			res0 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_features0);
			srun0 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures0.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_features0
					+ NewLine);
			end;
		End;
	End;
 TblNum = TblNum+1;

//(4) insert feature data into dbo.barFeatures1	
	if ReadWriteToDB and TblNum = 1  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		if  srun1 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_feat1 = "INSERT INTO  [dbo].[barFeatures1] VALUES ("
			+queryResult//MAX(loadId) 
			+","+Numtostr(loadGroupId,0) 
			+","+Numtostr(RightStrength,0)//RightStrength
			+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
			+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"'"+dateBarOpened+"' "+","+"'"+dayOfWeekBarOpened+"' "+","+"'"+dateTimeBarOpened+"' "+","+"'"+timeBarOpenedhhmmss+"' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)
+","+Numtostr(ACindBIG[RightStrength+0],9)
+","+Numtostr(ACindBIG[RightStrength+1],9)
+","+Numtostr(ACindBIG[RightStrength+10],9)
+","+Numtostr(ACindBIG[RightStrength+11],9)
+","+Numtostr(ACindBIG[RightStrength+12],9)
+","+Numtostr(ACindBIG[RightStrength+13],9)
+","+Numtostr(ACindBIG[RightStrength+14],9)
+","+Numtostr(ACindBIG[RightStrength+15],9)
+","+Numtostr(ACindBIG[RightStrength+16],9)
+","+Numtostr(ACindBIG[RightStrength+17],9)
+","+Numtostr(ACindBIG[RightStrength+18],9)
+","+Numtostr(ACindBIG[RightStrength+19],9)
+","+Numtostr(ACindBIG[RightStrength+2],9)
+","+Numtostr(ACindBIG[RightStrength+20],9)
+","+Numtostr(ACindBIG[RightStrength+21],9)
+","+Numtostr(ACindBIG[RightStrength+22],9)
+","+Numtostr(ACindBIG[RightStrength+23],9)
+","+Numtostr(ACindBIG[RightStrength+24],9)
+","+Numtostr(ACindBIG[RightStrength+25],9)
+","+Numtostr(ACindBIG[RightStrength+26],9)
+","+Numtostr(ACindBIG[RightStrength+27],9)
+","+Numtostr(ACindBIG[RightStrength+28],9)
+","+Numtostr(ACindBIG[RightStrength+3],9)
+","+Numtostr(ACindBIG[RightStrength+4],9)
+","+Numtostr(ACindBIG[RightStrength+5],9)
+","+Numtostr(ACindBIG[RightStrength+6],9)
+","+Numtostr(ACindBIG[RightStrength+7],9)
+","+Numtostr(ACindBIG[RightStrength+8],9)
+","+Numtostr(ACindBIG[RightStrength+9],9)
+","+Numtostr(ACindBIG[RightStrength-1],9)
+","+Numtostr(ACindBIG[RightStrength-2],9)
+","+Numtostr(ACindBIG[RightStrength-3],9)
+","+Numtostr(ACindBIG[RightStrength-4],9)
+","+Numtostr(ACindBX[RightStrength+0],9)
+","+Numtostr(ACindBX[RightStrength+1],9)
+","+Numtostr(ACindBX[RightStrength+10],9)
+","+Numtostr(ACindBX[RightStrength+11],9)
+","+Numtostr(ACindBX[RightStrength+12],9)
+","+Numtostr(ACindBX[RightStrength+13],9)
+","+Numtostr(ACindBX[RightStrength+14],9)
+","+Numtostr(ACindBX[RightStrength+15],9)
+","+Numtostr(ACindBX[RightStrength+16],9)
+","+Numtostr(ACindBX[RightStrength+17],9)
+","+Numtostr(ACindBX[RightStrength+18],9)
+","+Numtostr(ACindBX[RightStrength+19],9)
+","+Numtostr(ACindBX[RightStrength+2],9)
+","+Numtostr(ACindBX[RightStrength+20],9)
+","+Numtostr(ACindBX[RightStrength+21],9)
+","+Numtostr(ACindBX[RightStrength+22],9)
+","+Numtostr(ACindBX[RightStrength+23],9)
+","+Numtostr(ACindBX[RightStrength+24],9)
+","+Numtostr(ACindBX[RightStrength+25],9)
+","+Numtostr(ACindBX[RightStrength+26],9)
+","+Numtostr(ACindBX[RightStrength+27],9)
+","+Numtostr(ACindBX[RightStrength+28],9)
+","+Numtostr(ACindBX[RightStrength+3],9)
+","+Numtostr(ACindBX[RightStrength+4],9)
+","+Numtostr(ACindBX[RightStrength+5],9)
+","+Numtostr(ACindBX[RightStrength+6],9)
+","+Numtostr(ACindBX[RightStrength+7],9)
+","+Numtostr(ACindBX[RightStrength+8],9)
+","+Numtostr(ACindBX[RightStrength+9],9)
+","+Numtostr(ACindBX[RightStrength-1],9)
+","+Numtostr(ACindBX[RightStrength-2],9)
+","+Numtostr(ACindBX[RightStrength-3],9)
+","+Numtostr(ACindBX[RightStrength-4],9)
+","+Numtostr(ACindBXL[RightStrength+0],9)
+","+Numtostr(ACindBXL[RightStrength+1],9)
+","+Numtostr(ACindBXL[RightStrength+10],9)
+","+Numtostr(ACindBXL[RightStrength+11],9)
+","+Numtostr(ACindBXL[RightStrength+12],9)
+","+Numtostr(ACindBXL[RightStrength+13],9)
+","+Numtostr(ACindBXL[RightStrength+14],9)
+","+Numtostr(ACindBXL[RightStrength+15],9)
+","+Numtostr(ACindBXL[RightStrength+16],9)
+","+Numtostr(ACindBXL[RightStrength+17],9)
+","+Numtostr(ACindBXL[RightStrength+18],9)
+","+Numtostr(ACindBXL[RightStrength+19],9)
+","+Numtostr(ACindBXL[RightStrength+2],9)
+","+Numtostr(ACindBXL[RightStrength+20],9)
+","+Numtostr(ACindBXL[RightStrength+21],9)
+","+Numtostr(ACindBXL[RightStrength+22],9)
+","+Numtostr(ACindBXL[RightStrength+23],9)
+","+Numtostr(ACindBXL[RightStrength+24],9)
+","+Numtostr(ACindBXL[RightStrength+25],9)
+","+Numtostr(ACindBXL[RightStrength+26],9)
+","+Numtostr(ACindBXL[RightStrength+27],9)
+","+Numtostr(ACindBXL[RightStrength+28],9)
+","+Numtostr(ACindBXL[RightStrength+3],9)
+","+Numtostr(ACindBXL[RightStrength+4],9)
+","+Numtostr(ACindBXL[RightStrength+5],9)
+","+Numtostr(ACindBXL[RightStrength+6],9)
+","+Numtostr(ACindBXL[RightStrength+7],9)
+","+Numtostr(ACindBXL[RightStrength+8],9)
+","+Numtostr(ACindBXL[RightStrength+9],9)
+","+Numtostr(ACindBXL[RightStrength-1],9)
+","+Numtostr(ACindBXL[RightStrength-2],9)
+","+Numtostr(ACindBXL[RightStrength-3],9)
+","+Numtostr(ACindBXL[RightStrength-4],9)
+","+Numtostr(ACindMED[RightStrength+0],9)
+","+Numtostr(ACindMED[RightStrength+1],9)
+","+Numtostr(ACindMED[RightStrength+10],9)
+","+Numtostr(ACindMED[RightStrength+11],9)
+","+Numtostr(ACindMED[RightStrength+12],9)
+","+Numtostr(ACindMED[RightStrength+13],9)
+","+Numtostr(ACindMED[RightStrength+14],9)
+","+Numtostr(ACindMED[RightStrength+15],9)
+","+Numtostr(ACindMED[RightStrength+16],9)
+","+Numtostr(ACindMED[RightStrength+17],9)
+","+Numtostr(ACindMED[RightStrength+18],9)
+","+Numtostr(ACindMED[RightStrength+19],9)
+","+Numtostr(ACindMED[RightStrength+2],9)
+","+Numtostr(ACindMED[RightStrength+20],9)
+","+Numtostr(ACindMED[RightStrength+21],9)
+","+Numtostr(ACindMED[RightStrength+22],9)
+","+Numtostr(ACindMED[RightStrength+23],9)
+","+Numtostr(ACindMED[RightStrength+24],9)
+","+Numtostr(ACindMED[RightStrength+25],9)
+","+Numtostr(ACindMED[RightStrength+26],9)
+","+Numtostr(ACindMED[RightStrength+27],9)
+","+Numtostr(ACindMED[RightStrength+28],9)
+","+Numtostr(ACindMED[RightStrength+3],9)
+","+Numtostr(ACindMED[RightStrength+4],9)
+","+Numtostr(ACindMED[RightStrength+5],9)
+","+Numtostr(ACindMED[RightStrength+6],9)
+","+Numtostr(ACindMED[RightStrength+7],9)
+","+Numtostr(ACindMED[RightStrength+8],9)
+","+Numtostr(ACindMED[RightStrength+9],9)
+","+Numtostr(ACindMED[RightStrength-1],9)
+","+Numtostr(ACindMED[RightStrength-2],9)
+","+Numtostr(ACindMED[RightStrength-3],9)
+","+Numtostr(ACindMED[RightStrength-4],9)
+","+Numtostr(ACindSMALL[RightStrength+0],9)
+","+Numtostr(ACindSMALL[RightStrength+1],9)
+","+Numtostr(ACindSMALL[RightStrength+10],9)
+","+Numtostr(ACindSMALL[RightStrength+11],9)
+","+Numtostr(ACindSMALL[RightStrength+12],9)
+","+Numtostr(ACindSMALL[RightStrength+13],9)
+","+Numtostr(ACindSMALL[RightStrength+14],9)
+","+Numtostr(ACindSMALL[RightStrength+15],9)
+","+Numtostr(ACindSMALL[RightStrength+16],9)
+","+Numtostr(ACindSMALL[RightStrength+17],9)
+","+Numtostr(ACindSMALL[RightStrength+18],9)
+","+Numtostr(ACindSMALL[RightStrength+19],9)
+","+Numtostr(ACindSMALL[RightStrength+2],9)
+","+Numtostr(ACindSMALL[RightStrength+20],9)
+","+Numtostr(ACindSMALL[RightStrength+21],9)
+","+Numtostr(ACindSMALL[RightStrength+22],9)
+","+Numtostr(ACindSMALL[RightStrength+23],9)
+","+Numtostr(ACindSMALL[RightStrength+24],9)
+","+Numtostr(ACindSMALL[RightStrength+25],9)
+","+Numtostr(ACindSMALL[RightStrength+26],9)
+","+Numtostr(ACindSMALL[RightStrength+27],9)
+","+Numtostr(ACindSMALL[RightStrength+28],9)
+","+Numtostr(ACindSMALL[RightStrength+3],9)
+","+Numtostr(ACindSMALL[RightStrength+4],9)
+","+Numtostr(ACindSMALL[RightStrength+5],9)
+","+Numtostr(ACindSMALL[RightStrength+6],9)
+","+Numtostr(ACindSMALL[RightStrength+7],9)
+","+Numtostr(ACindSMALL[RightStrength+8],9)
+","+Numtostr(ACindSMALL[RightStrength+9],9)
+","+Numtostr(ACindSMALL[RightStrength-1],9)
+","+Numtostr(ACindSMALL[RightStrength-2],9)
+","+Numtostr(ACindSMALL[RightStrength-3],9)
+","+Numtostr(ACindSMALL[RightStrength-4],9)
+","+Numtostr(ADXRindBIG[RightStrength+0],9)
+","+Numtostr(ADXRindBIG[RightStrength+1],9)
+","+Numtostr(ADXRindBIG[RightStrength+10],9)
+","+Numtostr(ADXRindBIG[RightStrength+11],9)
+","+Numtostr(ADXRindBIG[RightStrength+12],9)
+","+Numtostr(ADXRindBIG[RightStrength+13],9)
+","+Numtostr(ADXRindBIG[RightStrength+14],9)
+","+Numtostr(ADXRindBIG[RightStrength+15],9)
+","+Numtostr(ADXRindBIG[RightStrength+16],9)
+","+Numtostr(ADXRindBIG[RightStrength+17],9)
+","+Numtostr(ADXRindBIG[RightStrength+18],9)
+","+Numtostr(ADXRindBIG[RightStrength+19],9)
+","+Numtostr(ADXRindBIG[RightStrength+2],9)
+","+Numtostr(ADXRindBIG[RightStrength+20],9)
+","+Numtostr(ADXRindBIG[RightStrength+21],9)
+","+Numtostr(ADXRindBIG[RightStrength+22],9)
+","+Numtostr(ADXRindBIG[RightStrength+23],9)
+","+Numtostr(ADXRindBIG[RightStrength+24],9)
+","+Numtostr(ADXRindBIG[RightStrength+25],9)
+","+Numtostr(ADXRindBIG[RightStrength+26],9)
+","+Numtostr(ADXRindBIG[RightStrength+27],9)
+","+Numtostr(ADXRindBIG[RightStrength+28],9)
+","+Numtostr(ADXRindBIG[RightStrength+3],9)
+","+Numtostr(ADXRindBIG[RightStrength+4],9)
+","+Numtostr(ADXRindBIG[RightStrength+5],9)
+","+Numtostr(ADXRindBIG[RightStrength+6],9)
+","+Numtostr(ADXRindBIG[RightStrength+7],9)
+","+Numtostr(ADXRindBIG[RightStrength+8],9)
+","+Numtostr(ADXRindBIG[RightStrength+9],9)
+","+Numtostr(ADXRindBIG[RightStrength-1],9)
+","+Numtostr(ADXRindBIG[RightStrength-2],9)
+","+Numtostr(ADXRindBIG[RightStrength-3],9)
+","+Numtostr(ADXRindBIG[RightStrength-4],9)
+","+Numtostr(ADXRindBX[RightStrength+0],9)
+","+Numtostr(ADXRindBX[RightStrength+1],9)
+","+Numtostr(ADXRindBX[RightStrength+10],9)
+","+Numtostr(ADXRindBX[RightStrength+11],9)
+","+Numtostr(ADXRindBX[RightStrength+12],9)
+","+Numtostr(ADXRindBX[RightStrength+13],9)
+","+Numtostr(ADXRindBX[RightStrength+14],9)
+","+Numtostr(ADXRindBX[RightStrength+15],9)
+","+Numtostr(ADXRindBX[RightStrength+16],9)
+","+Numtostr(ADXRindBX[RightStrength+17],9)
+","+Numtostr(ADXRindBX[RightStrength+18],9)
+","+Numtostr(ADXRindBX[RightStrength+19],9)
+","+Numtostr(ADXRindBX[RightStrength+2],9)
+","+Numtostr(ADXRindBX[RightStrength+20],9)
+","+Numtostr(ADXRindBX[RightStrength+21],9)
+","+Numtostr(ADXRindBX[RightStrength+22],9)
+","+Numtostr(ADXRindBX[RightStrength+23],9)
+","+Numtostr(ADXRindBX[RightStrength+24],9)
+","+Numtostr(ADXRindBX[RightStrength+25],9)
+","+Numtostr(ADXRindBX[RightStrength+26],9)
+","+Numtostr(ADXRindBX[RightStrength+27],9)
+","+Numtostr(ADXRindBX[RightStrength+28],9)
+","+Numtostr(ADXRindBX[RightStrength+3],9)
+","+Numtostr(ADXRindBX[RightStrength+4],9)
+","+Numtostr(ADXRindBX[RightStrength+5],9)
+","+Numtostr(ADXRindBX[RightStrength+6],9)
+","+Numtostr(ADXRindBX[RightStrength+7],9)
+","+Numtostr(ADXRindBX[RightStrength+8],9)
+","+Numtostr(ADXRindBX[RightStrength+9],9)
+","+Numtostr(ADXRindBX[RightStrength-1],9)
+","+Numtostr(ADXRindBX[RightStrength-2],9)
+","+Numtostr(ADXRindBX[RightStrength-3],9)
+","+Numtostr(ADXRindBX[RightStrength-4],9)
+","+Numtostr(ADXRindBXL[RightStrength+0],9)
+","+Numtostr(ADXRindBXL[RightStrength+1],9)
+","+Numtostr(ADXRindBXL[RightStrength+10],9)
+","+Numtostr(ADXRindBXL[RightStrength+11],9)
+","+Numtostr(ADXRindBXL[RightStrength+12],9)
+","+Numtostr(ADXRindBXL[RightStrength+13],9)
+","+Numtostr(ADXRindBXL[RightStrength+14],9)
+","+Numtostr(ADXRindBXL[RightStrength+15],9)
+","+Numtostr(ADXRindBXL[RightStrength+16],9)
+","+Numtostr(ADXRindBXL[RightStrength+17],9)
+","+Numtostr(ADXRindBXL[RightStrength+18],9)
+","+Numtostr(ADXRindBXL[RightStrength+19],9)
+","+Numtostr(ADXRindBXL[RightStrength+2],9)
+","+Numtostr(ADXRindBXL[RightStrength+20],9)
+","+Numtostr(ADXRindBXL[RightStrength+21],9)
+","+Numtostr(ADXRindBXL[RightStrength+22],9)
+","+Numtostr(ADXRindBXL[RightStrength+23],9)
+","+Numtostr(ADXRindBXL[RightStrength+24],9)
+","+Numtostr(ADXRindBXL[RightStrength+25],9)
+","+Numtostr(ADXRindBXL[RightStrength+26],9)
+","+Numtostr(ADXRindBXL[RightStrength+27],9)
+","+Numtostr(ADXRindBXL[RightStrength+28],9)
+","+Numtostr(ADXRindBXL[RightStrength+3],9)
+","+Numtostr(ADXRindBXL[RightStrength+4],9)
+","+Numtostr(ADXRindBXL[RightStrength+5],9)
+","+Numtostr(ADXRindBXL[RightStrength+6],9)
+","+Numtostr(ADXRindBXL[RightStrength+7],9)
+","+Numtostr(ADXRindBXL[RightStrength+8],9)
+","+Numtostr(ADXRindBXL[RightStrength+9],9)
+","+Numtostr(ADXRindBXL[RightStrength-1],9)
+","+Numtostr(ADXRindBXL[RightStrength-2],9)
+","+Numtostr(ADXRindBXL[RightStrength-3],9)
+","+Numtostr(ADXRindBXL[RightStrength-4],9)
+","+Numtostr(ADXRindMED[RightStrength+0],9)
+","+Numtostr(ADXRindMED[RightStrength+1],9)
+","+Numtostr(ADXRindMED[RightStrength+10],9)
+","+Numtostr(ADXRindMED[RightStrength+11],9)
+","+Numtostr(ADXRindMED[RightStrength+12],9)
+","+Numtostr(ADXRindMED[RightStrength+13],9)
+","+Numtostr(ADXRindMED[RightStrength+14],9)
+","+Numtostr(ADXRindMED[RightStrength+15],9)
+","+Numtostr(ADXRindMED[RightStrength+16],9)
+","+Numtostr(ADXRindMED[RightStrength+17],9)
+","+Numtostr(ADXRindMED[RightStrength+18],9)
+","+Numtostr(ADXRindMED[RightStrength+19],9)
+","+Numtostr(ADXRindMED[RightStrength+2],9)
+","+Numtostr(ADXRindMED[RightStrength+20],9)
+","+Numtostr(ADXRindMED[RightStrength+21],9)
+","+Numtostr(ADXRindMED[RightStrength+22],9)
+","+Numtostr(ADXRindMED[RightStrength+23],9)
+","+Numtostr(ADXRindMED[RightStrength+24],9)
+","+Numtostr(ADXRindMED[RightStrength+25],9)
+","+Numtostr(ADXRindMED[RightStrength+26],9)
+","+Numtostr(ADXRindMED[RightStrength+27],9)
+","+Numtostr(ADXRindMED[RightStrength+28],9)
+","+Numtostr(ADXRindMED[RightStrength+3],9)
+","+Numtostr(ADXRindMED[RightStrength+4],9)
+","+Numtostr(ADXRindMED[RightStrength+5],9)
+","+Numtostr(ADXRindMED[RightStrength+6],9)
+","+Numtostr(ADXRindMED[RightStrength+7],9)
+","+Numtostr(ADXRindMED[RightStrength+8],9)
+","+Numtostr(ADXRindMED[RightStrength+9],9)
+","+Numtostr(ADXRindMED[RightStrength-1],9)
+","+Numtostr(ADXRindMED[RightStrength-2],9)
+","+Numtostr(ADXRindMED[RightStrength-3],9)
+","+Numtostr(ADXRindMED[RightStrength-4],9)
+","+Numtostr(ADXRindSMALL[RightStrength+0],9)
+","+Numtostr(ADXRindSMALL[RightStrength+1],9)
+","+Numtostr(ADXRindSMALL[RightStrength+10],9)
+","+Numtostr(ADXRindSMALL[RightStrength+11],9)
+","+Numtostr(ADXRindSMALL[RightStrength+12],9)
+","+Numtostr(ADXRindSMALL[RightStrength+13],9)
+","+Numtostr(ADXRindSMALL[RightStrength+14],9)
+","+Numtostr(ADXRindSMALL[RightStrength+15],9)
+","+Numtostr(ADXRindSMALL[RightStrength+16],9)
+","+Numtostr(ADXRindSMALL[RightStrength+17],9)
+","+Numtostr(ADXRindSMALL[RightStrength+18],9)
+","+Numtostr(ADXRindSMALL[RightStrength+19],9)
+","+Numtostr(ADXRindSMALL[RightStrength+2],9)
+","+Numtostr(ADXRindSMALL[RightStrength+20],9)
+","+Numtostr(ADXRindSMALL[RightStrength+21],9)
+","+Numtostr(ADXRindSMALL[RightStrength+22],9)
+","+Numtostr(ADXRindSMALL[RightStrength+23],9)
+","+Numtostr(ADXRindSMALL[RightStrength+24],9)
+","+Numtostr(ADXRindSMALL[RightStrength+25],9)
+","+Numtostr(ADXRindSMALL[RightStrength+26],9)
+","+Numtostr(ADXRindSMALL[RightStrength+27],9)
+","+Numtostr(ADXRindSMALL[RightStrength+28],9)
+","+Numtostr(ADXRindSMALL[RightStrength+3],9)
+","+Numtostr(ADXRindSMALL[RightStrength+4],9)
+","+Numtostr(ADXRindSMALL[RightStrength+5],9)
+","+Numtostr(ADXRindSMALL[RightStrength+6],9)
+","+Numtostr(ADXRindSMALL[RightStrength+7],9)
+","+Numtostr(ADXRindSMALL[RightStrength+8],9)
+","+Numtostr(ADXRindSMALL[RightStrength+9],9)
+","+Numtostr(ADXRindSMALL[RightStrength-1],9)
+","+Numtostr(ADXRindSMALL[RightStrength-2],9)
+","+Numtostr(ADXRindSMALL[RightStrength-3],9)
+","+Numtostr(ADXRindSMALL[RightStrength-4],9)
+","+Numtostr(AROONindBIG[RightStrength+0],9)
+","+Numtostr(AROONindBIG[RightStrength+1],9)
+","+Numtostr(AROONindBIG[RightStrength+10],9)
+","+Numtostr(AROONindBIG[RightStrength+11],9)
+","+Numtostr(AROONindBIG[RightStrength+12],9)
+","+Numtostr(AROONindBIG[RightStrength+13],9)
+","+Numtostr(AROONindBIG[RightStrength+14],9)
+","+Numtostr(AROONindBIG[RightStrength+15],9)
+","+Numtostr(AROONindBIG[RightStrength+16],9)
+","+Numtostr(AROONindBIG[RightStrength+17],9)
+","+Numtostr(AROONindBIG[RightStrength+18],9)
+","+Numtostr(AROONindBIG[RightStrength+19],9)
+","+Numtostr(AROONindBIG[RightStrength+2],9)
+","+Numtostr(AROONindBIG[RightStrength+20],9)
+","+Numtostr(AROONindBIG[RightStrength+21],9)
+","+Numtostr(AROONindBIG[RightStrength+22],9)
+","+Numtostr(AROONindBIG[RightStrength+23],9)
+","+Numtostr(AROONindBIG[RightStrength+24],9)
+","+Numtostr(AROONindBIG[RightStrength+25],9)
+","+Numtostr(AROONindBIG[RightStrength+26],9)
+","+Numtostr(AROONindBIG[RightStrength+27],9)
+","+Numtostr(AROONindBIG[RightStrength+28],9)
+","+Numtostr(AROONindBIG[RightStrength+3],9)
+","+Numtostr(AROONindBIG[RightStrength+4],9)
+","+Numtostr(AROONindBIG[RightStrength+5],9)
+","+Numtostr(AROONindBIG[RightStrength+6],9)
+","+Numtostr(AROONindBIG[RightStrength+7],9)
+","+Numtostr(AROONindBIG[RightStrength+8],9)
+","+Numtostr(AROONindBIG[RightStrength+9],9)
+","+Numtostr(AROONindBIG[RightStrength-1],9)
+","+Numtostr(AROONindBIG[RightStrength-2],9)
+","+Numtostr(AROONindBIG[RightStrength-3],9)
+","+Numtostr(AROONindBIG[RightStrength-4],9)
+","+Numtostr(AROONindBX[RightStrength+0],9)
+","+Numtostr(AROONindBX[RightStrength+1],9)
+","+Numtostr(AROONindBX[RightStrength+10],9)
+","+Numtostr(AROONindBX[RightStrength+11],9)
+","+Numtostr(AROONindBX[RightStrength+12],9)
+","+Numtostr(AROONindBX[RightStrength+13],9)
+","+Numtostr(AROONindBX[RightStrength+14],9)
+","+Numtostr(AROONindBX[RightStrength+15],9)
+","+Numtostr(AROONindBX[RightStrength+16],9)
+","+Numtostr(AROONindBX[RightStrength+17],9)
+","+Numtostr(AROONindBX[RightStrength+18],9)
+","+Numtostr(AROONindBX[RightStrength+19],9)
+","+Numtostr(AROONindBX[RightStrength+2],9)
+","+Numtostr(AROONindBX[RightStrength+20],9)
+","+Numtostr(AROONindBX[RightStrength+21],9)
+","+Numtostr(AROONindBX[RightStrength+22],9)
+","+Numtostr(AROONindBX[RightStrength+23],9)
+","+Numtostr(AROONindBX[RightStrength+24],9)
+","+Numtostr(AROONindBX[RightStrength+25],9)
+","+Numtostr(AROONindBX[RightStrength+26],9)
+","+Numtostr(AROONindBX[RightStrength+27],9)
+","+Numtostr(AROONindBX[RightStrength+28],9)
+","+Numtostr(AROONindBX[RightStrength+3],9)
+","+Numtostr(AROONindBX[RightStrength+4],9)
+","+Numtostr(AROONindBX[RightStrength+5],9)
+","+Numtostr(AROONindBX[RightStrength+6],9)
+","+Numtostr(AROONindBX[RightStrength+7],9)
+","+Numtostr(AROONindBX[RightStrength+8],9)
+","+Numtostr(AROONindBX[RightStrength+9],9)
+","+Numtostr(AROONindBX[RightStrength-1],9)
+","+Numtostr(AROONindBX[RightStrength-2],9)
+","+Numtostr(AROONindBX[RightStrength-3],9)
+","+Numtostr(AROONindBX[RightStrength-4],9)
+","+Numtostr(AROONindBXL[RightStrength+0],9)
+","+Numtostr(AROONindBXL[RightStrength+1],9)
+","+Numtostr(AROONindBXL[RightStrength+10],9)
+","+Numtostr(AROONindBXL[RightStrength+11],9)
+","+Numtostr(AROONindBXL[RightStrength+12],9)
+","+Numtostr(AROONindBXL[RightStrength+13],9)
+","+Numtostr(AROONindBXL[RightStrength+14],9)
+","+Numtostr(AROONindBXL[RightStrength+15],9)
+","+Numtostr(AROONindBXL[RightStrength+16],9)
+","+Numtostr(AROONindBXL[RightStrength+17],9)
+","+Numtostr(AROONindBXL[RightStrength+18],9)
+","+Numtostr(AROONindBXL[RightStrength+19],9)
+","+Numtostr(AROONindBXL[RightStrength+2],9)
+","+Numtostr(AROONindBXL[RightStrength+20],9)
+","+Numtostr(AROONindBXL[RightStrength+21],9)
+","+Numtostr(AROONindBXL[RightStrength+22],9)
+","+Numtostr(AROONindBXL[RightStrength+23],9)
+","+Numtostr(AROONindBXL[RightStrength+24],9)
+","+Numtostr(AROONindBXL[RightStrength+25],9)
+","+Numtostr(AROONindBXL[RightStrength+26],9)
+","+Numtostr(AROONindBXL[RightStrength+27],9)
+","+Numtostr(AROONindBXL[RightStrength+28],9)
+","+Numtostr(AROONindBXL[RightStrength+3],9)
+","+Numtostr(AROONindBXL[RightStrength+4],9)
+","+Numtostr(AROONindBXL[RightStrength+5],9)
+","+Numtostr(AROONindBXL[RightStrength+6],9)
+","+Numtostr(AROONindBXL[RightStrength+7],9)
+","+Numtostr(AROONindBXL[RightStrength+8],9)
+","+Numtostr(AROONindBXL[RightStrength+9],9)
+","+Numtostr(AROONindBXL[RightStrength-1],9)
+","+Numtostr(AROONindBXL[RightStrength-2],9)
+","+Numtostr(AROONindBXL[RightStrength-3],9)
+","+Numtostr(AROONindBXL[RightStrength-4],9)
+","+Numtostr(AROONindMED[RightStrength+0],9)
+","+Numtostr(AROONindMED[RightStrength+1],9)
+","+Numtostr(AROONindMED[RightStrength+10],9)
+","+Numtostr(AROONindMED[RightStrength+11],9)
+","+Numtostr(AROONindMED[RightStrength+12],9)
+","+Numtostr(AROONindMED[RightStrength+13],9)
+","+Numtostr(AROONindMED[RightStrength+14],9)
+","+Numtostr(AROONindMED[RightStrength+15],9)
+","+Numtostr(AROONindMED[RightStrength+16],9)
+","+Numtostr(AROONindMED[RightStrength+17],9)
+","+Numtostr(AROONindMED[RightStrength+18],9)
+","+Numtostr(AROONindMED[RightStrength+19],9)
+","+Numtostr(AROONindMED[RightStrength+2],9)
+","+Numtostr(AROONindMED[RightStrength+20],9)
+","+Numtostr(AROONindMED[RightStrength+21],9)
+","+Numtostr(AROONindMED[RightStrength+22],9)
+","+Numtostr(AROONindMED[RightStrength+23],9)
+","+Numtostr(AROONindMED[RightStrength+24],9)
+","+Numtostr(AROONindMED[RightStrength+25],9)
+","+Numtostr(AROONindMED[RightStrength+26],9)
+","+Numtostr(AROONindMED[RightStrength+27],9)
+","+Numtostr(AROONindMED[RightStrength+28],9)
+","+Numtostr(AROONindMED[RightStrength+3],9)
+","+Numtostr(AROONindMED[RightStrength+4],9)
+","+Numtostr(AROONindMED[RightStrength+5],9)
+","+Numtostr(AROONindMED[RightStrength+6],9)
+","+Numtostr(AROONindMED[RightStrength+7],9)
+","+Numtostr(AROONindMED[RightStrength+8],9)
+","+Numtostr(AROONindMED[RightStrength+9],9)
+","+Numtostr(AROONindMED[RightStrength-1],9)
+","+Numtostr(AROONindMED[RightStrength-2],9)
+","+Numtostr(AROONindMED[RightStrength-3],9)
+","+Numtostr(AROONindMED[RightStrength-4],9)
+","+Numtostr(AROONindSMALL[RightStrength+0],9)
+","+Numtostr(AROONindSMALL[RightStrength+1],9)
+","+Numtostr(AROONindSMALL[RightStrength+10],9)
+","+Numtostr(AROONindSMALL[RightStrength+11],9)
+","+Numtostr(AROONindSMALL[RightStrength+12],9)
+","+Numtostr(AROONindSMALL[RightStrength+13],9)
+","+Numtostr(AROONindSMALL[RightStrength+14],9)
+","+Numtostr(AROONindSMALL[RightStrength+15],9)
+","+Numtostr(AROONindSMALL[RightStrength+16],9)
+","+Numtostr(AROONindSMALL[RightStrength+17],9)
+","+Numtostr(AROONindSMALL[RightStrength+18],9)
+","+Numtostr(AROONindSMALL[RightStrength+19],9)
+","+Numtostr(AROONindSMALL[RightStrength+2],9)
+","+Numtostr(AROONindSMALL[RightStrength+20],9)
+","+Numtostr(AROONindSMALL[RightStrength+21],9)
+","+Numtostr(AROONindSMALL[RightStrength+22],9)
+","+Numtostr(AROONindSMALL[RightStrength+23],9)
+","+Numtostr(AROONindSMALL[RightStrength+24],9)
+","+Numtostr(AROONindSMALL[RightStrength+25],9)
+","+Numtostr(AROONindSMALL[RightStrength+26],9)
+","+Numtostr(AROONindSMALL[RightStrength+27],9)
+","+Numtostr(AROONindSMALL[RightStrength+28],9)
+","+Numtostr(AROONindSMALL[RightStrength+3],9)
+","+Numtostr(AROONindSMALL[RightStrength+4],9)
+","+Numtostr(AROONindSMALL[RightStrength+5],9)
+","+Numtostr(AROONindSMALL[RightStrength+6],9)
+","+Numtostr(AROONindSMALL[RightStrength+7],9)
+","+Numtostr(AROONindSMALL[RightStrength+8],9)
+","+Numtostr(AROONindSMALL[RightStrength+9],9)
+","+Numtostr(AROONindSMALL[RightStrength-1],9)
+","+Numtostr(AROONindSMALL[RightStrength-2],9)
+","+Numtostr(AROONindSMALL[RightStrength-3],9)
+","+Numtostr(AROONindSMALL[RightStrength-4],9)
+","+Numtostr(ATRindBIG[RightStrength+0],9)
+","+Numtostr(ATRindBIG[RightStrength+1],9)
+","+Numtostr(ATRindBIG[RightStrength+10],9)
+","+Numtostr(ATRindBIG[RightStrength+11],9)
+","+Numtostr(ATRindBIG[RightStrength+12],9)
			+" )"; 
			res1 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_feat1);
			srun1 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures1.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_feat1
					+ NewLine);
			end;
		End;
	End;
	 TblNum = TblNum+1;


	//(4) insert feature data into dbo.barFeatures2	
	if ReadWriteToDB and TblNum = 2  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		if  srun2 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_feat2 = "INSERT INTO  [dbo].[barFeatures2] VALUES ("
			+queryResult//MAX(loadId) 
			+","+Numtostr(loadGroupId,0) 
			+","+Numtostr(RightStrength,0)//RightStrength
			+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
			+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"'"+dateBarOpened+"' "+","+"'"+dayOfWeekBarOpened+"' "+","+"'"+dateTimeBarOpened+"' "+","+"'"+timeBarOpenedhhmmss+"' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)
+","+Numtostr(CCIindBIG[RightStrength+18],9)
+","+Numtostr(CCIindBIG[RightStrength+19],9)
+","+Numtostr(CCIindBIG[RightStrength+2],9)
+","+Numtostr(CCIindBIG[RightStrength+20],9)
+","+Numtostr(CCIindBIG[RightStrength+21],9)
+","+Numtostr(CCIindBIG[RightStrength+22],9)
+","+Numtostr(CCIindBIG[RightStrength+23],9)
+","+Numtostr(CCIindBIG[RightStrength+24],9)
+","+Numtostr(CCIindBIG[RightStrength+25],9)
+","+Numtostr(CCIindBIG[RightStrength+26],9)
+","+Numtostr(CCIindBIG[RightStrength+27],9)
+","+Numtostr(CCIindBIG[RightStrength+28],9)
+","+Numtostr(CCIindBIG[RightStrength+3],9)
+","+Numtostr(CCIindBIG[RightStrength+4],9)
+","+Numtostr(CCIindBIG[RightStrength+5],9)
+","+Numtostr(CCIindBIG[RightStrength+6],9)
+","+Numtostr(CCIindBIG[RightStrength+7],9)
+","+Numtostr(CCIindBIG[RightStrength+8],9)
+","+Numtostr(CCIindBIG[RightStrength+9],9)
+","+Numtostr(CCIindBIG[RightStrength-1],9)
+","+Numtostr(CCIindBIG[RightStrength-2],9)
+","+Numtostr(CCIindBIG[RightStrength-3],9)
+","+Numtostr(CCIindBIG[RightStrength-4],9)
+","+Numtostr(CCIindBX[RightStrength+0],9)
+","+Numtostr(CCIindBX[RightStrength+1],9)
+","+Numtostr(CCIindBX[RightStrength+10],9)
+","+Numtostr(CCIindBX[RightStrength+11],9)
+","+Numtostr(CCIindBX[RightStrength+12],9)
+","+Numtostr(CCIindBX[RightStrength+13],9)
+","+Numtostr(CCIindBX[RightStrength+14],9)
+","+Numtostr(CCIindBX[RightStrength+15],9)
+","+Numtostr(CCIindBX[RightStrength+16],9)
+","+Numtostr(CCIindBX[RightStrength+17],9)
+","+Numtostr(CCIindBX[RightStrength+18],9)
+","+Numtostr(CCIindBX[RightStrength+19],9)
+","+Numtostr(CCIindBX[RightStrength+2],9)
+","+Numtostr(CCIindBX[RightStrength+20],9)
+","+Numtostr(CCIindBX[RightStrength+21],9)
+","+Numtostr(CCIindBX[RightStrength+22],9)
+","+Numtostr(CCIindBX[RightStrength+23],9)
+","+Numtostr(CCIindBX[RightStrength+24],9)
+","+Numtostr(CCIindBX[RightStrength+25],9)
+","+Numtostr(CCIindBX[RightStrength+26],9)
+","+Numtostr(CCIindBX[RightStrength+27],9)
+","+Numtostr(CCIindBX[RightStrength+28],9)
+","+Numtostr(CCIindBX[RightStrength+3],9)
+","+Numtostr(CCIindBX[RightStrength+4],9)
+","+Numtostr(CCIindBX[RightStrength+5],9)
+","+Numtostr(CCIindBX[RightStrength+6],9)
+","+Numtostr(CCIindBX[RightStrength+7],9)
+","+Numtostr(CCIindBX[RightStrength+8],9)
+","+Numtostr(CCIindBX[RightStrength+9],9)
+","+Numtostr(CCIindBX[RightStrength-1],9)
+","+Numtostr(CCIindBX[RightStrength-2],9)
+","+Numtostr(CCIindBX[RightStrength-3],9)
+","+Numtostr(CCIindBX[RightStrength-4],9)
+","+Numtostr(CCIindBXL[RightStrength+0],9)
+","+Numtostr(CCIindBXL[RightStrength+1],9)
+","+Numtostr(CCIindBXL[RightStrength+10],9)
+","+Numtostr(CCIindBXL[RightStrength+11],9)
+","+Numtostr(CCIindBXL[RightStrength+12],9)
+","+Numtostr(CCIindBXL[RightStrength+13],9)
+","+Numtostr(CCIindBXL[RightStrength+14],9)
+","+Numtostr(CCIindBXL[RightStrength+15],9)
+","+Numtostr(CCIindBXL[RightStrength+16],9)
+","+Numtostr(CCIindBXL[RightStrength+17],9)
+","+Numtostr(CCIindBXL[RightStrength+18],9)
+","+Numtostr(CCIindBXL[RightStrength+19],9)
+","+Numtostr(CCIindBXL[RightStrength+2],9)
+","+Numtostr(CCIindBXL[RightStrength+20],9)
+","+Numtostr(CCIindBXL[RightStrength+21],9)
+","+Numtostr(CCIindBXL[RightStrength+22],9)
+","+Numtostr(CCIindBXL[RightStrength+23],9)
+","+Numtostr(CCIindBXL[RightStrength+24],9)
+","+Numtostr(CCIindBXL[RightStrength+25],9)
+","+Numtostr(CCIindBXL[RightStrength+26],9)
+","+Numtostr(CCIindBXL[RightStrength+27],9)
+","+Numtostr(CCIindBXL[RightStrength+28],9)
+","+Numtostr(CCIindBXL[RightStrength+3],9)
+","+Numtostr(CCIindBXL[RightStrength+4],9)
+","+Numtostr(CCIindBXL[RightStrength+5],9)
+","+Numtostr(CCIindBXL[RightStrength+6],9)
+","+Numtostr(CCIindBXL[RightStrength+7],9)
+","+Numtostr(CCIindBXL[RightStrength+8],9)
+","+Numtostr(CCIindBXL[RightStrength+9],9)
+","+Numtostr(CCIindBXL[RightStrength-1],9)
+","+Numtostr(CCIindBXL[RightStrength-2],9)
+","+Numtostr(CCIindBXL[RightStrength-3],9)
+","+Numtostr(CCIindBXL[RightStrength-4],9)
+","+Numtostr(CCIindMED[RightStrength+0],9)
+","+Numtostr(CCIindMED[RightStrength+1],9)
+","+Numtostr(CCIindMED[RightStrength+10],9)
+","+Numtostr(CCIindMED[RightStrength+11],9)
+","+Numtostr(CCIindMED[RightStrength+12],9)
+","+Numtostr(CCIindMED[RightStrength+13],9)
+","+Numtostr(CCIindMED[RightStrength+14],9)
+","+Numtostr(CCIindMED[RightStrength+15],9)
+","+Numtostr(CCIindMED[RightStrength+16],9)
+","+Numtostr(CCIindMED[RightStrength+17],9)
+","+Numtostr(CCIindMED[RightStrength+18],9)
+","+Numtostr(CCIindMED[RightStrength+19],9)
+","+Numtostr(CCIindMED[RightStrength+2],9)
+","+Numtostr(CCIindMED[RightStrength+20],9)
+","+Numtostr(CCIindMED[RightStrength+21],9)
+","+Numtostr(CCIindMED[RightStrength+22],9)
+","+Numtostr(CCIindMED[RightStrength+23],9)
+","+Numtostr(CCIindMED[RightStrength+24],9)
+","+Numtostr(CCIindMED[RightStrength+25],9)
+","+Numtostr(CCIindMED[RightStrength+26],9)
+","+Numtostr(CCIindMED[RightStrength+27],9)
+","+Numtostr(CCIindMED[RightStrength+28],9)
+","+Numtostr(CCIindMED[RightStrength+3],9)
+","+Numtostr(CCIindMED[RightStrength+4],9)
+","+Numtostr(CCIindMED[RightStrength+5],9)
+","+Numtostr(CCIindMED[RightStrength+6],9)
+","+Numtostr(CCIindMED[RightStrength+7],9)
+","+Numtostr(CCIindMED[RightStrength+8],9)
+","+Numtostr(CCIindMED[RightStrength+9],9)
+","+Numtostr(CCIindMED[RightStrength-1],9)
+","+Numtostr(CCIindMED[RightStrength-2],9)
+","+Numtostr(CCIindMED[RightStrength-3],9)
+","+Numtostr(CCIindMED[RightStrength-4],9)
+","+Numtostr(CCIindSMALL[RightStrength+0],9)
+","+Numtostr(CCIindSMALL[RightStrength+1],9)
+","+Numtostr(CCIindSMALL[RightStrength+10],9)
+","+Numtostr(CCIindSMALL[RightStrength+11],9)
+","+Numtostr(CCIindSMALL[RightStrength+12],9)
+","+Numtostr(CCIindSMALL[RightStrength+13],9)
+","+Numtostr(CCIindSMALL[RightStrength+14],9)
+","+Numtostr(CCIindSMALL[RightStrength+15],9)
+","+Numtostr(CCIindSMALL[RightStrength+16],9)
+","+Numtostr(CCIindSMALL[RightStrength+17],9)
+","+Numtostr(CCIindSMALL[RightStrength+18],9)
+","+Numtostr(CCIindSMALL[RightStrength+19],9)
+","+Numtostr(CCIindSMALL[RightStrength+2],9)
+","+Numtostr(CCIindSMALL[RightStrength+20],9)
+","+Numtostr(CCIindSMALL[RightStrength+21],9)
+","+Numtostr(CCIindSMALL[RightStrength+22],9)
+","+Numtostr(CCIindSMALL[RightStrength+23],9)
+","+Numtostr(CCIindSMALL[RightStrength+24],9)
+","+Numtostr(CCIindSMALL[RightStrength+25],9)
+","+Numtostr(CCIindSMALL[RightStrength+26],9)
+","+Numtostr(CCIindSMALL[RightStrength+27],9)
+","+Numtostr(CCIindSMALL[RightStrength+28],9)
+","+Numtostr(CCIindSMALL[RightStrength+3],9)
+","+Numtostr(CCIindSMALL[RightStrength+4],9)
+","+Numtostr(CCIindSMALL[RightStrength+5],9)
+","+Numtostr(CCIindSMALL[RightStrength+6],9)
+","+Numtostr(CCIindSMALL[RightStrength+7],9)
+","+Numtostr(CCIindSMALL[RightStrength+8],9)
+","+Numtostr(CCIindSMALL[RightStrength+9],9)
+","+Numtostr(CCIindSMALL[RightStrength-1],9)
+","+Numtostr(CCIindSMALL[RightStrength-2],9)
+","+Numtostr(CCIindSMALL[RightStrength-3],9)
+","+Numtostr(CCIindSMALL[RightStrength-4],9)
+","+Numtostr(CDMIindBIG[RightStrength+0],9)
+","+Numtostr(CDMIindBIG[RightStrength+1],9)
+","+Numtostr(CDMIindBIG[RightStrength+10],9)
+","+Numtostr(CDMIindBIG[RightStrength+11],9)
+","+Numtostr(CDMIindBIG[RightStrength+12],9)
+","+Numtostr(CDMIindBIG[RightStrength+13],9)
+","+Numtostr(CDMIindBIG[RightStrength+14],9)
+","+Numtostr(CDMIindBIG[RightStrength+15],9)
+","+Numtostr(CDMIindBIG[RightStrength+16],9)
+","+Numtostr(CDMIindBIG[RightStrength+17],9)
+","+Numtostr(CDMIindBIG[RightStrength+18],9)
+","+Numtostr(CDMIindBIG[RightStrength+19],9)
+","+Numtostr(CDMIindBIG[RightStrength+2],9)
+","+Numtostr(CDMIindBIG[RightStrength+20],9)
+","+Numtostr(CDMIindBIG[RightStrength+21],9)
+","+Numtostr(CDMIindBIG[RightStrength+22],9)
+","+Numtostr(CDMIindBIG[RightStrength+23],9)
+","+Numtostr(CDMIindBIG[RightStrength+24],9)
+","+Numtostr(CDMIindBIG[RightStrength+25],9)
+","+Numtostr(CDMIindBIG[RightStrength+26],9)
+","+Numtostr(CDMIindBIG[RightStrength+27],9)
+","+Numtostr(CDMIindBIG[RightStrength+28],9)
+","+Numtostr(CDMIindBIG[RightStrength+3],9)
+","+Numtostr(CDMIindBIG[RightStrength+4],9)
+","+Numtostr(CDMIindBIG[RightStrength+5],9)
+","+Numtostr(CDMIindBIG[RightStrength+6],9)
+","+Numtostr(CDMIindBIG[RightStrength+7],9)
+","+Numtostr(CDMIindBIG[RightStrength+8],9)
+","+Numtostr(CDMIindBIG[RightStrength+9],9)
+","+Numtostr(CDMIindBIG[RightStrength-1],9)
+","+Numtostr(CDMIindBIG[RightStrength-2],9)
+","+Numtostr(CDMIindBIG[RightStrength-3],9)
+","+Numtostr(CDMIindBIG[RightStrength-4],9)
+","+Numtostr(CDMIindBX[RightStrength+0],9)
+","+Numtostr(CDMIindBX[RightStrength+1],9)
+","+Numtostr(CDMIindBX[RightStrength+10],9)
+","+Numtostr(CDMIindBX[RightStrength+11],9)
+","+Numtostr(CDMIindBX[RightStrength+12],9)
+","+Numtostr(CDMIindBX[RightStrength+13],9)
+","+Numtostr(CDMIindBX[RightStrength+14],9)
+","+Numtostr(CDMIindBX[RightStrength+15],9)
+","+Numtostr(CDMIindBX[RightStrength+16],9)
+","+Numtostr(CDMIindBX[RightStrength+17],9)
+","+Numtostr(CDMIindBX[RightStrength+18],9)
+","+Numtostr(CDMIindBX[RightStrength+19],9)
+","+Numtostr(CDMIindBX[RightStrength+2],9)
+","+Numtostr(CDMIindBX[RightStrength+20],9)
+","+Numtostr(CDMIindBX[RightStrength+21],9)
+","+Numtostr(CDMIindBX[RightStrength+22],9)
+","+Numtostr(CDMIindBX[RightStrength+23],9)
+","+Numtostr(CDMIindBX[RightStrength+24],9)
+","+Numtostr(CDMIindBX[RightStrength+25],9)
+","+Numtostr(CDMIindBX[RightStrength+26],9)
+","+Numtostr(CDMIindBX[RightStrength+27],9)
+","+Numtostr(CDMIindBX[RightStrength+28],9)
+","+Numtostr(CDMIindBX[RightStrength+3],9)
+","+Numtostr(CDMIindBX[RightStrength+4],9)
+","+Numtostr(CDMIindBX[RightStrength+5],9)
+","+Numtostr(CDMIindBX[RightStrength+6],9)
+","+Numtostr(CDMIindBX[RightStrength+7],9)
+","+Numtostr(CDMIindBX[RightStrength+8],9)
+","+Numtostr(CDMIindBX[RightStrength+9],9)
+","+Numtostr(CDMIindBX[RightStrength-1],9)
+","+Numtostr(CDMIindBX[RightStrength-2],9)
+","+Numtostr(CDMIindBX[RightStrength-3],9)
+","+Numtostr(CDMIindBX[RightStrength-4],9)
+","+Numtostr(CDMIindBXL[RightStrength+0],9)
+","+Numtostr(CDMIindBXL[RightStrength+1],9)
+","+Numtostr(CDMIindBXL[RightStrength+10],9)
+","+Numtostr(CDMIindBXL[RightStrength+11],9)
+","+Numtostr(CDMIindBXL[RightStrength+12],9)
+","+Numtostr(CDMIindBXL[RightStrength+13],9)
+","+Numtostr(CDMIindBXL[RightStrength+14],9)
+","+Numtostr(CDMIindBXL[RightStrength+15],9)
+","+Numtostr(CDMIindBXL[RightStrength+16],9)
+","+Numtostr(CDMIindBXL[RightStrength+17],9)
+","+Numtostr(CDMIindBXL[RightStrength+18],9)
+","+Numtostr(CDMIindBXL[RightStrength+19],9)
+","+Numtostr(CDMIindBXL[RightStrength+2],9)
+","+Numtostr(CDMIindBXL[RightStrength+20],9)
+","+Numtostr(CDMIindBXL[RightStrength+21],9)
+","+Numtostr(CDMIindBXL[RightStrength+22],9)
+","+Numtostr(CDMIindBXL[RightStrength+23],9)
+","+Numtostr(CDMIindBXL[RightStrength+24],9)
+","+Numtostr(CDMIindBXL[RightStrength+25],9)
+","+Numtostr(CDMIindBXL[RightStrength+26],9)
+","+Numtostr(CDMIindBXL[RightStrength+27],9)
+","+Numtostr(CDMIindBXL[RightStrength+28],9)
+","+Numtostr(CDMIindBXL[RightStrength+3],9)
+","+Numtostr(CDMIindBXL[RightStrength+4],9)
+","+Numtostr(CDMIindBXL[RightStrength+5],9)
+","+Numtostr(CDMIindBXL[RightStrength+6],9)
+","+Numtostr(CDMIindBXL[RightStrength+7],9)
+","+Numtostr(CDMIindBXL[RightStrength+8],9)
+","+Numtostr(CDMIindBXL[RightStrength+9],9)
+","+Numtostr(CDMIindBXL[RightStrength-1],9)
+","+Numtostr(CDMIindBXL[RightStrength-2],9)
+","+Numtostr(CDMIindBXL[RightStrength-3],9)
+","+Numtostr(CDMIindBXL[RightStrength-4],9)
+","+Numtostr(CDMIindMED[RightStrength+0],9)
+","+Numtostr(CDMIindMED[RightStrength+1],9)
+","+Numtostr(CDMIindMED[RightStrength+10],9)
+","+Numtostr(CDMIindMED[RightStrength+11],9)
+","+Numtostr(CDMIindMED[RightStrength+12],9)
+","+Numtostr(CDMIindMED[RightStrength+13],9)
+","+Numtostr(CDMIindMED[RightStrength+14],9)
+","+Numtostr(CDMIindMED[RightStrength+15],9)
+","+Numtostr(CDMIindMED[RightStrength+16],9)
+","+Numtostr(CDMIindMED[RightStrength+17],9)
+","+Numtostr(CDMIindMED[RightStrength+18],9)
+","+Numtostr(CDMIindMED[RightStrength+19],9)
+","+Numtostr(CDMIindMED[RightStrength+2],9)
+","+Numtostr(CDMIindMED[RightStrength+20],9)
+","+Numtostr(CDMIindMED[RightStrength+21],9)
+","+Numtostr(CDMIindMED[RightStrength+22],9)
+","+Numtostr(CDMIindMED[RightStrength+23],9)
+","+Numtostr(CDMIindMED[RightStrength+24],9)
+","+Numtostr(CDMIindMED[RightStrength+25],9)
+","+Numtostr(CDMIindMED[RightStrength+26],9)
+","+Numtostr(CDMIindMED[RightStrength+27],9)
+","+Numtostr(CDMIindMED[RightStrength+28],9)
+","+Numtostr(CDMIindMED[RightStrength+3],9)
+","+Numtostr(CDMIindMED[RightStrength+4],9)
+","+Numtostr(CDMIindMED[RightStrength+5],9)
+","+Numtostr(CDMIindMED[RightStrength+6],9)
+","+Numtostr(CDMIindMED[RightStrength+7],9)
+","+Numtostr(CDMIindMED[RightStrength+8],9)
+","+Numtostr(CDMIindMED[RightStrength+9],9)
+","+Numtostr(CDMIindMED[RightStrength-1],9)
+","+Numtostr(CDMIindMED[RightStrength-2],9)
+","+Numtostr(CDMIindMED[RightStrength-3],9)
+","+Numtostr(CDMIindMED[RightStrength-4],9)
+","+Numtostr(CDMIindSMALL[RightStrength+0],9)
+","+Numtostr(CDMIindSMALL[RightStrength+1],9)
+","+Numtostr(CDMIindSMALL[RightStrength+10],9)
+","+Numtostr(CDMIindSMALL[RightStrength+11],9)
+","+Numtostr(CDMIindSMALL[RightStrength+12],9)
+","+Numtostr(CDMIindSMALL[RightStrength+13],9)
+","+Numtostr(CDMIindSMALL[RightStrength+14],9)
+","+Numtostr(CDMIindSMALL[RightStrength+15],9)
+","+Numtostr(CDMIindSMALL[RightStrength+16],9)
+","+Numtostr(CDMIindSMALL[RightStrength+17],9)
+","+Numtostr(CDMIindSMALL[RightStrength+18],9)
+","+Numtostr(CDMIindSMALL[RightStrength+19],9)
+","+Numtostr(CDMIindSMALL[RightStrength+2],9)
+","+Numtostr(CDMIindSMALL[RightStrength+20],9)
+","+Numtostr(CDMIindSMALL[RightStrength+21],9)
+","+Numtostr(CDMIindSMALL[RightStrength+22],9)
+","+Numtostr(CDMIindSMALL[RightStrength+23],9)
+","+Numtostr(CDMIindSMALL[RightStrength+24],9)
+","+Numtostr(CDMIindSMALL[RightStrength+25],9)
+","+Numtostr(CDMIindSMALL[RightStrength+26],9)
+","+Numtostr(CDMIindSMALL[RightStrength+27],9)
+","+Numtostr(CDMIindSMALL[RightStrength+28],9)
+","+Numtostr(CDMIindSMALL[RightStrength+3],9)
+","+Numtostr(CDMIindSMALL[RightStrength+4],9)
+","+Numtostr(CDMIindSMALL[RightStrength+5],9)
+","+Numtostr(CDMIindSMALL[RightStrength+6],9)
+","+Numtostr(CDMIindSMALL[RightStrength+7],9)
+","+Numtostr(CDMIindSMALL[RightStrength+8],9)
+","+Numtostr(CDMIindSMALL[RightStrength+9],9)
+","+Numtostr(CDMIindSMALL[RightStrength-1],9)
+","+Numtostr(CDMIindSMALL[RightStrength-2],9)
+","+Numtostr(CDMIindSMALL[RightStrength-3],9)
+","+Numtostr(CDMIindSMALL[RightStrength-4],9)
+","+Numtostr(CHOPindBIG[RightStrength+0],9)
+","+Numtostr(CHOPindBIG[RightStrength+1],9)
+","+Numtostr(CHOPindBIG[RightStrength+10],9)
+","+Numtostr(CHOPindBIG[RightStrength+11],9)
+","+Numtostr(CHOPindBIG[RightStrength+12],9)
+","+Numtostr(CHOPindBIG[RightStrength+13],9)
+","+Numtostr(CHOPindBIG[RightStrength+14],9)
+","+Numtostr(CHOPindBIG[RightStrength+15],9)
+","+Numtostr(CHOPindBIG[RightStrength+16],9)
+","+Numtostr(CHOPindBIG[RightStrength+17],9)
+","+Numtostr(CHOPindBIG[RightStrength+18],9)
+","+Numtostr(CHOPindBIG[RightStrength+19],9)
+","+Numtostr(CHOPindBIG[RightStrength+2],9)
+","+Numtostr(CHOPindBIG[RightStrength+20],9)
+","+Numtostr(CHOPindBIG[RightStrength+21],9)
+","+Numtostr(CHOPindBIG[RightStrength+22],9)
+","+Numtostr(CHOPindBIG[RightStrength+23],9)
+","+Numtostr(CHOPindBIG[RightStrength+24],9)
+","+Numtostr(CHOPindBIG[RightStrength+25],9)
+","+Numtostr(CHOPindBIG[RightStrength+26],9)
+","+Numtostr(CHOPindBIG[RightStrength+27],9)
+","+Numtostr(CHOPindBIG[RightStrength+28],9)
+","+Numtostr(CHOPindBIG[RightStrength+3],9)
+","+Numtostr(CHOPindBIG[RightStrength+4],9)
+","+Numtostr(CHOPindBIG[RightStrength+5],9)
+","+Numtostr(CHOPindBIG[RightStrength+6],9)
+","+Numtostr(CHOPindBIG[RightStrength+7],9)
+","+Numtostr(CHOPindBIG[RightStrength+8],9)
+","+Numtostr(CHOPindBIG[RightStrength+9],9)
+","+Numtostr(CHOPindBIG[RightStrength-1],9)
+","+Numtostr(CHOPindBIG[RightStrength-2],9)
+","+Numtostr(CHOPindBIG[RightStrength-3],9)
+","+Numtostr(CHOPindBIG[RightStrength-4],9)
+","+Numtostr(CHOPindBX[RightStrength+0],9)
+","+Numtostr(CHOPindBX[RightStrength+1],9)
+","+Numtostr(CHOPindBX[RightStrength+10],9)
+","+Numtostr(CHOPindBX[RightStrength+11],9)
+","+Numtostr(CHOPindBX[RightStrength+12],9)
+","+Numtostr(CHOPindBX[RightStrength+13],9)
+","+Numtostr(CHOPindBX[RightStrength+14],9)
+","+Numtostr(CHOPindBX[RightStrength+15],9)
+","+Numtostr(CHOPindBX[RightStrength+16],9)
+","+Numtostr(CHOPindBX[RightStrength+17],9)
+","+Numtostr(CHOPindBX[RightStrength+18],9)
+","+Numtostr(CHOPindBX[RightStrength+19],9)
+","+Numtostr(CHOPindBX[RightStrength+2],9)
+","+Numtostr(CHOPindBX[RightStrength+20],9)
+","+Numtostr(CHOPindBX[RightStrength+21],9)
+","+Numtostr(CHOPindBX[RightStrength+22],9)
+","+Numtostr(CHOPindBX[RightStrength+23],9)
+","+Numtostr(CHOPindBX[RightStrength+24],9)
+","+Numtostr(CHOPindBX[RightStrength+25],9)
+","+Numtostr(CHOPindBX[RightStrength+26],9)
+","+Numtostr(CHOPindBX[RightStrength+27],9)
+","+Numtostr(CHOPindBX[RightStrength+28],9)
+","+Numtostr(CHOPindBX[RightStrength+3],9)
+","+Numtostr(CHOPindBX[RightStrength+4],9)
+","+Numtostr(CHOPindBX[RightStrength+5],9)
+","+Numtostr(CHOPindBX[RightStrength+6],9)
+","+Numtostr(CHOPindBX[RightStrength+7],9)
+","+Numtostr(CHOPindBX[RightStrength+8],9)
+","+Numtostr(CHOPindBX[RightStrength+9],9)
+","+Numtostr(CHOPindBX[RightStrength-1],9)
+","+Numtostr(CHOPindBX[RightStrength-2],9)
+","+Numtostr(CHOPindBX[RightStrength-3],9)
+","+Numtostr(CHOPindBX[RightStrength-4],9)
+","+Numtostr(CHOPindBXL[RightStrength+0],9)
+","+Numtostr(CHOPindBXL[RightStrength+1],9)
+","+Numtostr(CHOPindBXL[RightStrength+10],9)
+","+Numtostr(CHOPindBXL[RightStrength+11],9)
+","+Numtostr(CHOPindBXL[RightStrength+12],9)
+","+Numtostr(CHOPindBXL[RightStrength+13],9)
+","+Numtostr(CHOPindBXL[RightStrength+14],9)
+","+Numtostr(CHOPindBXL[RightStrength+15],9)
+","+Numtostr(CHOPindBXL[RightStrength+16],9)
+","+Numtostr(CHOPindBXL[RightStrength+17],9)
+","+Numtostr(CHOPindBXL[RightStrength+18],9)
+","+Numtostr(CHOPindBXL[RightStrength+19],9)
+","+Numtostr(CHOPindBXL[RightStrength+2],9)
+","+Numtostr(CHOPindBXL[RightStrength+20],9)
+","+Numtostr(CHOPindBXL[RightStrength+21],9)
+","+Numtostr(CHOPindBXL[RightStrength+22],9)
+","+Numtostr(CHOPindBXL[RightStrength+23],9)
+","+Numtostr(CHOPindBXL[RightStrength+24],9)
+","+Numtostr(CHOPindBXL[RightStrength+25],9)
+","+Numtostr(CHOPindBXL[RightStrength+26],9)
+","+Numtostr(CHOPindBXL[RightStrength+27],9)
+","+Numtostr(CHOPindBXL[RightStrength+28],9)
+","+Numtostr(CHOPindBXL[RightStrength+3],9)
+","+Numtostr(CHOPindBXL[RightStrength+4],9)
+","+Numtostr(CHOPindBXL[RightStrength+5],9)
+","+Numtostr(CHOPindBXL[RightStrength+6],9)
+","+Numtostr(CHOPindBXL[RightStrength+7],9)
+","+Numtostr(CHOPindBXL[RightStrength+8],9)
+","+Numtostr(CHOPindBXL[RightStrength+9],9)
+","+Numtostr(CHOPindBXL[RightStrength-1],9)
+","+Numtostr(CHOPindBXL[RightStrength-2],9)
+","+Numtostr(CHOPindBXL[RightStrength-3],9)
+","+Numtostr(CHOPindBXL[RightStrength-4],9)
+","+Numtostr(CHOPindMED[RightStrength+0],9)
+","+Numtostr(CHOPindMED[RightStrength+1],9)
+","+Numtostr(CHOPindMED[RightStrength+10],9)
+","+Numtostr(CHOPindMED[RightStrength+11],9)
+","+Numtostr(CHOPindMED[RightStrength+12],9)
+","+Numtostr(CHOPindMED[RightStrength+13],9)
+","+Numtostr(CHOPindMED[RightStrength+14],9)
+","+Numtostr(CHOPindMED[RightStrength+15],9)
+","+Numtostr(CHOPindMED[RightStrength+16],9)
+","+Numtostr(CHOPindMED[RightStrength+17],9)
+","+Numtostr(CHOPindMED[RightStrength+18],9)
+","+Numtostr(CHOPindMED[RightStrength+19],9)
+","+Numtostr(CHOPindMED[RightStrength+2],9)
+","+Numtostr(CHOPindMED[RightStrength+20],9)
+","+Numtostr(CHOPindMED[RightStrength+21],9)
+","+Numtostr(CHOPindMED[RightStrength+22],9)
+","+Numtostr(CHOPindMED[RightStrength+23],9)
+","+Numtostr(CHOPindMED[RightStrength+24],9)
+","+Numtostr(CHOPindMED[RightStrength+25],9)
+","+Numtostr(CHOPindMED[RightStrength+26],9)
+","+Numtostr(CHOPindMED[RightStrength+27],9)
+","+Numtostr(CHOPindMED[RightStrength+28],9)
+","+Numtostr(CHOPindMED[RightStrength+3],9)
+","+Numtostr(CHOPindMED[RightStrength+4],9)
+","+Numtostr(CHOPindMED[RightStrength+5],9)
+","+Numtostr(CHOPindMED[RightStrength+6],9)
+","+Numtostr(CHOPindMED[RightStrength+7],9)
+","+Numtostr(CHOPindMED[RightStrength+8],9)
+","+Numtostr(CHOPindMED[RightStrength+9],9)
+","+Numtostr(CHOPindMED[RightStrength-1],9)
+","+Numtostr(CHOPindMED[RightStrength-2],9)
+","+Numtostr(CHOPindMED[RightStrength-3],9)
+","+Numtostr(CHOPindMED[RightStrength-4],9)
+","+Numtostr(CHOPindSMALL[RightStrength+0],9)
+","+Numtostr(CHOPindSMALL[RightStrength+1],9)
+","+Numtostr(CHOPindSMALL[RightStrength+10],9)
+","+Numtostr(CHOPindSMALL[RightStrength+11],9)
+","+Numtostr(CHOPindSMALL[RightStrength+12],9)
+","+Numtostr(CHOPindSMALL[RightStrength+13],9)
+","+Numtostr(CHOPindSMALL[RightStrength+14],9)
+","+Numtostr(CHOPindSMALL[RightStrength+15],9)
+","+Numtostr(CHOPindSMALL[RightStrength+16],9)
+","+Numtostr(CHOPindSMALL[RightStrength+17],9)
+","+Numtostr(CHOPindSMALL[RightStrength+18],9)
+","+Numtostr(CHOPindSMALL[RightStrength+19],9)
+","+Numtostr(CHOPindSMALL[RightStrength+2],9)
+","+Numtostr(CHOPindSMALL[RightStrength+20],9)
+","+Numtostr(CHOPindSMALL[RightStrength+21],9)
+","+Numtostr(CHOPindSMALL[RightStrength+22],9)
+","+Numtostr(CHOPindSMALL[RightStrength+23],9)
+","+Numtostr(CHOPindSMALL[RightStrength+24],9)
+","+Numtostr(CHOPindSMALL[RightStrength+25],9)
+","+Numtostr(CHOPindSMALL[RightStrength+26],9)
+","+Numtostr(CHOPindSMALL[RightStrength+27],9)
+","+Numtostr(CHOPindSMALL[RightStrength+28],9)
+","+Numtostr(CHOPindSMALL[RightStrength+3],9)
+","+Numtostr(CHOPindSMALL[RightStrength+4],9)
+","+Numtostr(CHOPindSMALL[RightStrength+5],9)
+","+Numtostr(CHOPindSMALL[RightStrength+6],9)
+","+Numtostr(CHOPindSMALL[RightStrength+7],9)
+","+Numtostr(CHOPindSMALL[RightStrength+8],9)
+","+Numtostr(CHOPindSMALL[RightStrength+9],9)
+","+Numtostr(CHOPindSMALL[RightStrength-1],9)
+","+Numtostr(CHOPindSMALL[RightStrength-2],9)
+","+Numtostr(CHOPindSMALL[RightStrength-3],9)
+","+Numtostr(CHOPindSMALL[RightStrength-4],9)
+","+Numtostr(CMIindBIG[RightStrength+0],9)
+","+Numtostr(CMIindBIG[RightStrength+1],9)
+","+Numtostr(CMIindBIG[RightStrength+10],9)
+","+Numtostr(CMIindBIG[RightStrength+11],9)
+","+Numtostr(CMIindBIG[RightStrength+12],9)
+","+Numtostr(CMIindBIG[RightStrength+13],9)
+","+Numtostr(CMIindBIG[RightStrength+14],9)
+","+Numtostr(CMIindBIG[RightStrength+15],9)
+","+Numtostr(CMIindBIG[RightStrength+16],9)
+","+Numtostr(CMIindBIG[RightStrength+17],9)
+","+Numtostr(CMIindBIG[RightStrength+18],9)
+","+Numtostr(CMIindBIG[RightStrength+19],9)
+","+Numtostr(CMIindBIG[RightStrength+2],9)
+","+Numtostr(CMIindBIG[RightStrength+20],9)
+","+Numtostr(CMIindBIG[RightStrength+21],9)
			+" )"; 
			res2 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_feat2);
			srun2 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures2.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_feat2
					+ NewLine);
			end;
		End;
	End;
	 TblNum = TblNum+1;

	//(4) insert feature data into dbo.barFeatures3	
	if ReadWriteToDB and TblNum = 3  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		if  srun3 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_feat3 = "INSERT INTO  [dbo].[barFeatures3] VALUES ("
			+queryResult//MAX(loadId) 
			+","+Numtostr(loadGroupId,0) 
			+","+Numtostr(RightStrength,0)//RightStrength
			+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
			+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"'"+dateBarOpened+"' "+","+"'"+dayOfWeekBarOpened+"' "+","+"'"+dateTimeBarOpened+"' "+","+"'"+timeBarOpenedhhmmss+"' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)
+","+Numtostr(DMIStoindBIG[RightStrength+27],9)
+","+Numtostr(DMIStoindBIG[RightStrength+28],9)
+","+Numtostr(DMIStoindBIG[RightStrength+3],9)
+","+Numtostr(DMIStoindBIG[RightStrength+4],9)
+","+Numtostr(DMIStoindBIG[RightStrength+5],9)
+","+Numtostr(DMIStoindBIG[RightStrength+6],9)
+","+Numtostr(DMIStoindBIG[RightStrength+7],9)
+","+Numtostr(DMIStoindBIG[RightStrength+8],9)
+","+Numtostr(DMIStoindBIG[RightStrength+9],9)
+","+Numtostr(DMIStoindBIG[RightStrength-1],9)
+","+Numtostr(DMIStoindBIG[RightStrength-2],9)
+","+Numtostr(DMIStoindBIG[RightStrength-3],9)
+","+Numtostr(DMIStoindBIG[RightStrength-4],9)
+","+Numtostr(DMIStoindBX[RightStrength+0],9)
+","+Numtostr(DMIStoindBX[RightStrength+1],9)
+","+Numtostr(DMIStoindBX[RightStrength+10],9)
+","+Numtostr(DMIStoindBX[RightStrength+11],9)
+","+Numtostr(DMIStoindBX[RightStrength+12],9)
+","+Numtostr(DMIStoindBX[RightStrength+13],9)
+","+Numtostr(DMIStoindBX[RightStrength+14],9)
+","+Numtostr(DMIStoindBX[RightStrength+15],9)
+","+Numtostr(DMIStoindBX[RightStrength+16],9)
+","+Numtostr(DMIStoindBX[RightStrength+17],9)
+","+Numtostr(DMIStoindBX[RightStrength+18],9)
+","+Numtostr(DMIStoindBX[RightStrength+19],9)
+","+Numtostr(DMIStoindBX[RightStrength+2],9)
+","+Numtostr(DMIStoindBX[RightStrength+20],9)
+","+Numtostr(DMIStoindBX[RightStrength+21],9)
+","+Numtostr(DMIStoindBX[RightStrength+22],9)
+","+Numtostr(DMIStoindBX[RightStrength+23],9)
+","+Numtostr(DMIStoindBX[RightStrength+24],9)
+","+Numtostr(DMIStoindBX[RightStrength+25],9)
+","+Numtostr(DMIStoindBX[RightStrength+26],9)
+","+Numtostr(DMIStoindBX[RightStrength+27],9)
+","+Numtostr(DMIStoindBX[RightStrength+28],9)
+","+Numtostr(DMIStoindBX[RightStrength+3],9)
+","+Numtostr(DMIStoindBX[RightStrength+4],9)
+","+Numtostr(DMIStoindBX[RightStrength+5],9)
+","+Numtostr(DMIStoindBX[RightStrength+6],9)
+","+Numtostr(DMIStoindBX[RightStrength+7],9)
+","+Numtostr(DMIStoindBX[RightStrength+8],9)
+","+Numtostr(DMIStoindBX[RightStrength+9],9)
+","+Numtostr(DMIStoindBX[RightStrength-1],9)
+","+Numtostr(DMIStoindBX[RightStrength-2],9)
+","+Numtostr(DMIStoindBX[RightStrength-3],9)
+","+Numtostr(DMIStoindBX[RightStrength-4],9)
+","+Numtostr(DMIStoindBXL[RightStrength+0],9)
+","+Numtostr(DMIStoindBXL[RightStrength+1],9)
+","+Numtostr(DMIStoindBXL[RightStrength+10],9)
+","+Numtostr(DMIStoindBXL[RightStrength+11],9)
+","+Numtostr(DMIStoindBXL[RightStrength+12],9)
+","+Numtostr(DMIStoindBXL[RightStrength+13],9)
+","+Numtostr(DMIStoindBXL[RightStrength+14],9)
+","+Numtostr(DMIStoindBXL[RightStrength+15],9)
+","+Numtostr(DMIStoindBXL[RightStrength+16],9)
+","+Numtostr(DMIStoindBXL[RightStrength+17],9)
+","+Numtostr(DMIStoindBXL[RightStrength+18],9)
+","+Numtostr(DMIStoindBXL[RightStrength+19],9)
+","+Numtostr(DMIStoindBXL[RightStrength+2],9)
+","+Numtostr(DMIStoindBXL[RightStrength+20],9)
+","+Numtostr(DMIStoindBXL[RightStrength+21],9)
+","+Numtostr(DMIStoindBXL[RightStrength+22],9)
+","+Numtostr(DMIStoindBXL[RightStrength+23],9)
+","+Numtostr(DMIStoindBXL[RightStrength+24],9)
+","+Numtostr(DMIStoindBXL[RightStrength+25],9)
+","+Numtostr(DMIStoindBXL[RightStrength+26],9)
+","+Numtostr(DMIStoindBXL[RightStrength+27],9)
+","+Numtostr(DMIStoindBXL[RightStrength+28],9)
+","+Numtostr(DMIStoindBXL[RightStrength+3],9)
+","+Numtostr(DMIStoindBXL[RightStrength+4],9)
+","+Numtostr(DMIStoindBXL[RightStrength+5],9)
+","+Numtostr(DMIStoindBXL[RightStrength+6],9)
+","+Numtostr(DMIStoindBXL[RightStrength+7],9)
+","+Numtostr(DMIStoindBXL[RightStrength+8],9)
+","+Numtostr(DMIStoindBXL[RightStrength+9],9)
+","+Numtostr(DMIStoindBXL[RightStrength-1],9)
+","+Numtostr(DMIStoindBXL[RightStrength-2],9)
+","+Numtostr(DMIStoindBXL[RightStrength-3],9)
+","+Numtostr(DMIStoindBXL[RightStrength-4],9)
+","+Numtostr(DMIStoindMED[RightStrength+0],9)
+","+Numtostr(DMIStoindMED[RightStrength+1],9)
+","+Numtostr(DMIStoindMED[RightStrength+10],9)
+","+Numtostr(DMIStoindMED[RightStrength+11],9)
+","+Numtostr(DMIStoindMED[RightStrength+12],9)
+","+Numtostr(DMIStoindMED[RightStrength+13],9)
+","+Numtostr(DMIStoindMED[RightStrength+14],9)
+","+Numtostr(DMIStoindMED[RightStrength+15],9)
+","+Numtostr(DMIStoindMED[RightStrength+16],9)
+","+Numtostr(DMIStoindMED[RightStrength+17],9)
+","+Numtostr(DMIStoindMED[RightStrength+18],9)
+","+Numtostr(DMIStoindMED[RightStrength+19],9)
+","+Numtostr(DMIStoindMED[RightStrength+2],9)
+","+Numtostr(DMIStoindMED[RightStrength+20],9)
+","+Numtostr(DMIStoindMED[RightStrength+21],9)
+","+Numtostr(DMIStoindMED[RightStrength+22],9)
+","+Numtostr(DMIStoindMED[RightStrength+23],9)
+","+Numtostr(DMIStoindMED[RightStrength+24],9)
+","+Numtostr(DMIStoindMED[RightStrength+25],9)
+","+Numtostr(DMIStoindMED[RightStrength+26],9)
+","+Numtostr(DMIStoindMED[RightStrength+27],9)
+","+Numtostr(DMIStoindMED[RightStrength+28],9)
+","+Numtostr(DMIStoindMED[RightStrength+3],9)
+","+Numtostr(DMIStoindMED[RightStrength+4],9)
+","+Numtostr(DMIStoindMED[RightStrength+5],9)
+","+Numtostr(DMIStoindMED[RightStrength+6],9)
+","+Numtostr(DMIStoindMED[RightStrength+7],9)
+","+Numtostr(DMIStoindMED[RightStrength+8],9)
+","+Numtostr(DMIStoindMED[RightStrength+9],9)
+","+Numtostr(DMIStoindMED[RightStrength-1],9)
+","+Numtostr(DMIStoindMED[RightStrength-2],9)
+","+Numtostr(DMIStoindMED[RightStrength-3],9)
+","+Numtostr(DMIStoindMED[RightStrength-4],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+0],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+1],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+10],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+11],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+12],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+13],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+14],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+15],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+16],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+17],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+18],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+19],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+2],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+20],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+21],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+22],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+23],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+24],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+25],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+26],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+27],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+28],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+3],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+4],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+5],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+6],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+7],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+8],9)
+","+Numtostr(DMIStoindSMALL[RightStrength+9],9)
+","+Numtostr(DMIStoindSMALL[RightStrength-1],9)
+","+Numtostr(DMIStoindSMALL[RightStrength-2],9)
+","+Numtostr(DMIStoindSMALL[RightStrength-3],9)
+","+Numtostr(DMIStoindSMALL[RightStrength-4],9)
+","+Numtostr(ELSindBIG[RightStrength+0],9)
+","+Numtostr(ELSindBIG[RightStrength+1],9)
+","+Numtostr(ELSindBIG[RightStrength+10],9)
+","+Numtostr(ELSindBIG[RightStrength+11],9)
+","+Numtostr(ELSindBIG[RightStrength+12],9)
+","+Numtostr(ELSindBIG[RightStrength+13],9)
+","+Numtostr(ELSindBIG[RightStrength+14],9)
+","+Numtostr(ELSindBIG[RightStrength+15],9)
+","+Numtostr(ELSindBIG[RightStrength+16],9)
+","+Numtostr(ELSindBIG[RightStrength+17],9)
+","+Numtostr(ELSindBIG[RightStrength+18],9)
+","+Numtostr(ELSindBIG[RightStrength+19],9)
+","+Numtostr(ELSindBIG[RightStrength+2],9)
+","+Numtostr(ELSindBIG[RightStrength+20],9)
+","+Numtostr(ELSindBIG[RightStrength+21],9)
+","+Numtostr(ELSindBIG[RightStrength+22],9)
+","+Numtostr(ELSindBIG[RightStrength+23],9)
+","+Numtostr(ELSindBIG[RightStrength+24],9)
+","+Numtostr(ELSindBIG[RightStrength+25],9)
+","+Numtostr(ELSindBIG[RightStrength+26],9)
+","+Numtostr(ELSindBIG[RightStrength+27],9)
+","+Numtostr(ELSindBIG[RightStrength+28],9)
+","+Numtostr(ELSindBIG[RightStrength+3],9)
+","+Numtostr(ELSindBIG[RightStrength+4],9)
+","+Numtostr(ELSindBIG[RightStrength+5],9)
+","+Numtostr(ELSindBIG[RightStrength+6],9)
+","+Numtostr(ELSindBIG[RightStrength+7],9)
+","+Numtostr(ELSindBIG[RightStrength+8],9)
+","+Numtostr(ELSindBIG[RightStrength+9],9)
+","+Numtostr(ELSindBIG[RightStrength-1],9)
+","+Numtostr(ELSindBIG[RightStrength-2],9)
+","+Numtostr(ELSindBIG[RightStrength-3],9)
+","+Numtostr(ELSindBIG[RightStrength-4],9)
+","+Numtostr(ELSindBX[RightStrength+0],9)
+","+Numtostr(ELSindBX[RightStrength+1],9)
+","+Numtostr(ELSindBX[RightStrength+10],9)
+","+Numtostr(ELSindBX[RightStrength+11],9)
+","+Numtostr(ELSindBX[RightStrength+12],9)
+","+Numtostr(ELSindBX[RightStrength+13],9)
+","+Numtostr(ELSindBX[RightStrength+14],9)
+","+Numtostr(ELSindBX[RightStrength+15],9)
+","+Numtostr(ELSindBX[RightStrength+16],9)
+","+Numtostr(ELSindBX[RightStrength+17],9)
+","+Numtostr(ELSindBX[RightStrength+18],9)
+","+Numtostr(ELSindBX[RightStrength+19],9)
+","+Numtostr(ELSindBX[RightStrength+2],9)
+","+Numtostr(ELSindBX[RightStrength+20],9)
+","+Numtostr(ELSindBX[RightStrength+21],9)
+","+Numtostr(ELSindBX[RightStrength+22],9)
+","+Numtostr(ELSindBX[RightStrength+23],9)
+","+Numtostr(ELSindBX[RightStrength+24],9)
+","+Numtostr(ELSindBX[RightStrength+25],9)
+","+Numtostr(ELSindBX[RightStrength+26],9)
+","+Numtostr(ELSindBX[RightStrength+27],9)
+","+Numtostr(ELSindBX[RightStrength+28],9)
+","+Numtostr(ELSindBX[RightStrength+3],9)
+","+Numtostr(ELSindBX[RightStrength+4],9)
+","+Numtostr(ELSindBX[RightStrength+5],9)
+","+Numtostr(ELSindBX[RightStrength+6],9)
+","+Numtostr(ELSindBX[RightStrength+7],9)
+","+Numtostr(ELSindBX[RightStrength+8],9)
+","+Numtostr(ELSindBX[RightStrength+9],9)
+","+Numtostr(ELSindBX[RightStrength-1],9)
+","+Numtostr(ELSindBX[RightStrength-2],9)
+","+Numtostr(ELSindBX[RightStrength-3],9)
+","+Numtostr(ELSindBX[RightStrength-4],9)
+","+Numtostr(ELSindBXL[RightStrength+0],9)
+","+Numtostr(ELSindBXL[RightStrength+1],9)
+","+Numtostr(ELSindBXL[RightStrength+10],9)
+","+Numtostr(ELSindBXL[RightStrength+11],9)
+","+Numtostr(ELSindBXL[RightStrength+12],9)
+","+Numtostr(ELSindBXL[RightStrength+13],9)
+","+Numtostr(ELSindBXL[RightStrength+14],9)
+","+Numtostr(ELSindBXL[RightStrength+15],9)
+","+Numtostr(ELSindBXL[RightStrength+16],9)
+","+Numtostr(ELSindBXL[RightStrength+17],9)
+","+Numtostr(ELSindBXL[RightStrength+18],9)
+","+Numtostr(ELSindBXL[RightStrength+19],9)
+","+Numtostr(ELSindBXL[RightStrength+2],9)
+","+Numtostr(ELSindBXL[RightStrength+20],9)
+","+Numtostr(ELSindBXL[RightStrength+21],9)
+","+Numtostr(ELSindBXL[RightStrength+22],9)
+","+Numtostr(ELSindBXL[RightStrength+23],9)
+","+Numtostr(ELSindBXL[RightStrength+24],9)
+","+Numtostr(ELSindBXL[RightStrength+25],9)
+","+Numtostr(ELSindBXL[RightStrength+26],9)
+","+Numtostr(ELSindBXL[RightStrength+27],9)
+","+Numtostr(ELSindBXL[RightStrength+28],9)
+","+Numtostr(ELSindBXL[RightStrength+3],9)
+","+Numtostr(ELSindBXL[RightStrength+4],9)
+","+Numtostr(ELSindBXL[RightStrength+5],9)
+","+Numtostr(ELSindBXL[RightStrength+6],9)
+","+Numtostr(ELSindBXL[RightStrength+7],9)
+","+Numtostr(ELSindBXL[RightStrength+8],9)
+","+Numtostr(ELSindBXL[RightStrength+9],9)
+","+Numtostr(ELSindBXL[RightStrength-1],9)
+","+Numtostr(ELSindBXL[RightStrength-2],9)
+","+Numtostr(ELSindBXL[RightStrength-3],9)
+","+Numtostr(ELSindBXL[RightStrength-4],9)
+","+Numtostr(ELSindMED[RightStrength+0],9)
+","+Numtostr(ELSindMED[RightStrength+1],9)
+","+Numtostr(ELSindMED[RightStrength+10],9)
+","+Numtostr(ELSindMED[RightStrength+11],9)
+","+Numtostr(ELSindMED[RightStrength+12],9)
+","+Numtostr(ELSindMED[RightStrength+13],9)
+","+Numtostr(ELSindMED[RightStrength+14],9)
+","+Numtostr(ELSindMED[RightStrength+15],9)
+","+Numtostr(ELSindMED[RightStrength+16],9)
+","+Numtostr(ELSindMED[RightStrength+17],9)
+","+Numtostr(ELSindMED[RightStrength+18],9)
+","+Numtostr(ELSindMED[RightStrength+19],9)
+","+Numtostr(ELSindMED[RightStrength+2],9)
+","+Numtostr(ELSindMED[RightStrength+20],9)
+","+Numtostr(ELSindMED[RightStrength+21],9)
+","+Numtostr(ELSindMED[RightStrength+22],9)
+","+Numtostr(ELSindMED[RightStrength+23],9)
+","+Numtostr(ELSindMED[RightStrength+24],9)
+","+Numtostr(ELSindMED[RightStrength+25],9)
+","+Numtostr(ELSindMED[RightStrength+26],9)
+","+Numtostr(ELSindMED[RightStrength+27],9)
+","+Numtostr(ELSindMED[RightStrength+28],9)
+","+Numtostr(ELSindMED[RightStrength+3],9)
+","+Numtostr(ELSindMED[RightStrength+4],9)
+","+Numtostr(ELSindMED[RightStrength+5],9)
+","+Numtostr(ELSindMED[RightStrength+6],9)
+","+Numtostr(ELSindMED[RightStrength+7],9)
+","+Numtostr(ELSindMED[RightStrength+8],9)
+","+Numtostr(ELSindMED[RightStrength+9],9)
+","+Numtostr(ELSindMED[RightStrength-1],9)
+","+Numtostr(ELSindMED[RightStrength-2],9)
+","+Numtostr(ELSindMED[RightStrength-3],9)
+","+Numtostr(ELSindMED[RightStrength-4],9)
+","+Numtostr(ELSindSMALL[RightStrength+0],9)
+","+Numtostr(ELSindSMALL[RightStrength+1],9)
+","+Numtostr(ELSindSMALL[RightStrength+10],9)
+","+Numtostr(ELSindSMALL[RightStrength+11],9)
+","+Numtostr(ELSindSMALL[RightStrength+12],9)
+","+Numtostr(ELSindSMALL[RightStrength+13],9)
+","+Numtostr(ELSindSMALL[RightStrength+14],9)
+","+Numtostr(ELSindSMALL[RightStrength+15],9)
+","+Numtostr(ELSindSMALL[RightStrength+16],9)
+","+Numtostr(ELSindSMALL[RightStrength+17],9)
+","+Numtostr(ELSindSMALL[RightStrength+18],9)
+","+Numtostr(ELSindSMALL[RightStrength+19],9)
+","+Numtostr(ELSindSMALL[RightStrength+2],9)
+","+Numtostr(ELSindSMALL[RightStrength+20],9)
+","+Numtostr(ELSindSMALL[RightStrength+21],9)
+","+Numtostr(ELSindSMALL[RightStrength+22],9)
+","+Numtostr(ELSindSMALL[RightStrength+23],9)
+","+Numtostr(ELSindSMALL[RightStrength+24],9)
+","+Numtostr(ELSindSMALL[RightStrength+25],9)
+","+Numtostr(ELSindSMALL[RightStrength+26],9)
+","+Numtostr(ELSindSMALL[RightStrength+27],9)
+","+Numtostr(ELSindSMALL[RightStrength+28],9)
+","+Numtostr(ELSindSMALL[RightStrength+3],9)
+","+Numtostr(ELSindSMALL[RightStrength+4],9)
+","+Numtostr(ELSindSMALL[RightStrength+5],9)
+","+Numtostr(ELSindSMALL[RightStrength+6],9)
+","+Numtostr(ELSindSMALL[RightStrength+7],9)
+","+Numtostr(ELSindSMALL[RightStrength+8],9)
+","+Numtostr(ELSindSMALL[RightStrength+9],9)
+","+Numtostr(ELSindSMALL[RightStrength-1],9)
+","+Numtostr(ELSindSMALL[RightStrength-2],9)
+","+Numtostr(ELSindSMALL[RightStrength-3],9)
+","+Numtostr(ELSindSMALL[RightStrength-4],9)
+","+Numtostr(EMDindBIG[RightStrength+0],9)
+","+Numtostr(EMDindBIG[RightStrength+1],9)
+","+Numtostr(EMDindBIG[RightStrength+10],9)
+","+Numtostr(EMDindBIG[RightStrength+11],9)
+","+Numtostr(EMDindBIG[RightStrength+12],9)
+","+Numtostr(EMDindBIG[RightStrength+13],9)
+","+Numtostr(EMDindBIG[RightStrength+14],9)
+","+Numtostr(EMDindBIG[RightStrength+15],9)
+","+Numtostr(EMDindBIG[RightStrength+16],9)
+","+Numtostr(EMDindBIG[RightStrength+17],9)
+","+Numtostr(EMDindBIG[RightStrength+18],9)
+","+Numtostr(EMDindBIG[RightStrength+19],9)
+","+Numtostr(EMDindBIG[RightStrength+2],9)
+","+Numtostr(EMDindBIG[RightStrength+20],9)
+","+Numtostr(EMDindBIG[RightStrength+21],9)
+","+Numtostr(EMDindBIG[RightStrength+22],9)
+","+Numtostr(EMDindBIG[RightStrength+23],9)
+","+Numtostr(EMDindBIG[RightStrength+24],9)
+","+Numtostr(EMDindBIG[RightStrength+25],9)
+","+Numtostr(EMDindBIG[RightStrength+26],9)
+","+Numtostr(EMDindBIG[RightStrength+27],9)
+","+Numtostr(EMDindBIG[RightStrength+28],9)
+","+Numtostr(EMDindBIG[RightStrength+3],9)
+","+Numtostr(EMDindBIG[RightStrength+4],9)
+","+Numtostr(EMDindBIG[RightStrength+5],9)
+","+Numtostr(EMDindBIG[RightStrength+6],9)
+","+Numtostr(EMDindBIG[RightStrength+7],9)
+","+Numtostr(EMDindBIG[RightStrength+8],9)
+","+Numtostr(EMDindBIG[RightStrength+9],9)
+","+Numtostr(EMDindBIG[RightStrength-1],9)
+","+Numtostr(EMDindBIG[RightStrength-2],9)
+","+Numtostr(EMDindBIG[RightStrength-3],9)
+","+Numtostr(EMDindBIG[RightStrength-4],9)
+","+Numtostr(EMDindBX[RightStrength+0],9)
+","+Numtostr(EMDindBX[RightStrength+1],9)
+","+Numtostr(EMDindBX[RightStrength+10],9)
+","+Numtostr(EMDindBX[RightStrength+11],9)
+","+Numtostr(EMDindBX[RightStrength+12],9)
+","+Numtostr(EMDindBX[RightStrength+13],9)
+","+Numtostr(EMDindBX[RightStrength+14],9)
+","+Numtostr(EMDindBX[RightStrength+15],9)
+","+Numtostr(EMDindBX[RightStrength+16],9)
+","+Numtostr(EMDindBX[RightStrength+17],9)
+","+Numtostr(EMDindBX[RightStrength+18],9)
+","+Numtostr(EMDindBX[RightStrength+19],9)
+","+Numtostr(EMDindBX[RightStrength+2],9)
+","+Numtostr(EMDindBX[RightStrength+20],9)
+","+Numtostr(EMDindBX[RightStrength+21],9)
+","+Numtostr(EMDindBX[RightStrength+22],9)
+","+Numtostr(EMDindBX[RightStrength+23],9)
+","+Numtostr(EMDindBX[RightStrength+24],9)
+","+Numtostr(EMDindBX[RightStrength+25],9)
+","+Numtostr(EMDindBX[RightStrength+26],9)
+","+Numtostr(EMDindBX[RightStrength+27],9)
+","+Numtostr(EMDindBX[RightStrength+28],9)
+","+Numtostr(EMDindBX[RightStrength+3],9)
+","+Numtostr(EMDindBX[RightStrength+4],9)
+","+Numtostr(EMDindBX[RightStrength+5],9)
+","+Numtostr(EMDindBX[RightStrength+6],9)
+","+Numtostr(EMDindBX[RightStrength+7],9)
+","+Numtostr(EMDindBX[RightStrength+8],9)
+","+Numtostr(EMDindBX[RightStrength+9],9)
+","+Numtostr(EMDindBX[RightStrength-1],9)
+","+Numtostr(EMDindBX[RightStrength-2],9)
+","+Numtostr(EMDindBX[RightStrength-3],9)
+","+Numtostr(EMDindBX[RightStrength-4],9)
+","+Numtostr(EMDindBXL[RightStrength+0],9)
+","+Numtostr(EMDindBXL[RightStrength+1],9)
+","+Numtostr(EMDindBXL[RightStrength+10],9)
+","+Numtostr(EMDindBXL[RightStrength+11],9)
+","+Numtostr(EMDindBXL[RightStrength+12],9)
+","+Numtostr(EMDindBXL[RightStrength+13],9)
+","+Numtostr(EMDindBXL[RightStrength+14],9)
+","+Numtostr(EMDindBXL[RightStrength+15],9)
+","+Numtostr(EMDindBXL[RightStrength+16],9)
+","+Numtostr(EMDindBXL[RightStrength+17],9)
+","+Numtostr(EMDindBXL[RightStrength+18],9)
+","+Numtostr(EMDindBXL[RightStrength+19],9)
+","+Numtostr(EMDindBXL[RightStrength+2],9)
+","+Numtostr(EMDindBXL[RightStrength+20],9)
+","+Numtostr(EMDindBXL[RightStrength+21],9)
+","+Numtostr(EMDindBXL[RightStrength+22],9)
+","+Numtostr(EMDindBXL[RightStrength+23],9)
+","+Numtostr(EMDindBXL[RightStrength+24],9)
+","+Numtostr(EMDindBXL[RightStrength+25],9)
+","+Numtostr(EMDindBXL[RightStrength+26],9)
+","+Numtostr(EMDindBXL[RightStrength+27],9)
+","+Numtostr(EMDindBXL[RightStrength+28],9)
+","+Numtostr(EMDindBXL[RightStrength+3],9)
+","+Numtostr(EMDindBXL[RightStrength+4],9)
+","+Numtostr(EMDindBXL[RightStrength+5],9)
+","+Numtostr(EMDindBXL[RightStrength+6],9)
+","+Numtostr(EMDindBXL[RightStrength+7],9)
+","+Numtostr(EMDindBXL[RightStrength+8],9)
+","+Numtostr(EMDindBXL[RightStrength+9],9)
+","+Numtostr(EMDindBXL[RightStrength-1],9)
+","+Numtostr(EMDindBXL[RightStrength-2],9)
+","+Numtostr(EMDindBXL[RightStrength-3],9)
+","+Numtostr(EMDindBXL[RightStrength-4],9)
+","+Numtostr(EMDindMED[RightStrength+0],9)
+","+Numtostr(EMDindMED[RightStrength+1],9)
+","+Numtostr(EMDindMED[RightStrength+10],9)
+","+Numtostr(EMDindMED[RightStrength+11],9)
+","+Numtostr(EMDindMED[RightStrength+12],9)
+","+Numtostr(EMDindMED[RightStrength+13],9)
+","+Numtostr(EMDindMED[RightStrength+14],9)
+","+Numtostr(EMDindMED[RightStrength+15],9)
+","+Numtostr(EMDindMED[RightStrength+16],9)
+","+Numtostr(EMDindMED[RightStrength+17],9)
+","+Numtostr(EMDindMED[RightStrength+18],9)
+","+Numtostr(EMDindMED[RightStrength+19],9)
+","+Numtostr(EMDindMED[RightStrength+2],9)
+","+Numtostr(EMDindMED[RightStrength+20],9)
+","+Numtostr(EMDindMED[RightStrength+21],9)
+","+Numtostr(EMDindMED[RightStrength+22],9)
+","+Numtostr(EMDindMED[RightStrength+23],9)
+","+Numtostr(EMDindMED[RightStrength+24],9)
+","+Numtostr(EMDindMED[RightStrength+25],9)
+","+Numtostr(EMDindMED[RightStrength+26],9)
+","+Numtostr(EMDindMED[RightStrength+27],9)
+","+Numtostr(EMDindMED[RightStrength+28],9)
+","+Numtostr(EMDindMED[RightStrength+3],9)
+","+Numtostr(EMDindMED[RightStrength+4],9)
+","+Numtostr(EMDindMED[RightStrength+5],9)
+","+Numtostr(EMDindMED[RightStrength+6],9)
+","+Numtostr(EMDindMED[RightStrength+7],9)
+","+Numtostr(EMDindMED[RightStrength+8],9)
+","+Numtostr(EMDindMED[RightStrength+9],9)
+","+Numtostr(EMDindMED[RightStrength-1],9)
+","+Numtostr(EMDindMED[RightStrength-2],9)
+","+Numtostr(EMDindMED[RightStrength-3],9)
+","+Numtostr(EMDindMED[RightStrength-4],9)
+","+Numtostr(EMDindSMALL[RightStrength+0],9)
+","+Numtostr(EMDindSMALL[RightStrength+1],9)
+","+Numtostr(EMDindSMALL[RightStrength+10],9)
+","+Numtostr(EMDindSMALL[RightStrength+11],9)
+","+Numtostr(EMDindSMALL[RightStrength+12],9)
+","+Numtostr(EMDindSMALL[RightStrength+13],9)
+","+Numtostr(EMDindSMALL[RightStrength+14],9)
+","+Numtostr(EMDindSMALL[RightStrength+15],9)
+","+Numtostr(EMDindSMALL[RightStrength+16],9)
+","+Numtostr(EMDindSMALL[RightStrength+17],9)
+","+Numtostr(EMDindSMALL[RightStrength+18],9)
+","+Numtostr(EMDindSMALL[RightStrength+19],9)
+","+Numtostr(EMDindSMALL[RightStrength+2],9)
+","+Numtostr(EMDindSMALL[RightStrength+20],9)
+","+Numtostr(EMDindSMALL[RightStrength+21],9)
+","+Numtostr(EMDindSMALL[RightStrength+22],9)
+","+Numtostr(EMDindSMALL[RightStrength+23],9)
+","+Numtostr(EMDindSMALL[RightStrength+24],9)
+","+Numtostr(EMDindSMALL[RightStrength+25],9)
+","+Numtostr(EMDindSMALL[RightStrength+26],9)
+","+Numtostr(EMDindSMALL[RightStrength+27],9)
+","+Numtostr(EMDindSMALL[RightStrength+28],9)
+","+Numtostr(EMDindSMALL[RightStrength+3],9)
+","+Numtostr(EMDindSMALL[RightStrength+4],9)
+","+Numtostr(EMDindSMALL[RightStrength+5],9)
+","+Numtostr(EMDindSMALL[RightStrength+6],9)
+","+Numtostr(EMDindSMALL[RightStrength+7],9)
+","+Numtostr(EMDindSMALL[RightStrength+8],9)
+","+Numtostr(EMDindSMALL[RightStrength+9],9)
+","+Numtostr(EMDindSMALL[RightStrength-1],9)
+","+Numtostr(EMDindSMALL[RightStrength-2],9)
+","+Numtostr(EMDindSMALL[RightStrength-3],9)
+","+Numtostr(EMDindSMALL[RightStrength-4],9)
+","+Numtostr(FRAMAindBIG[RightStrength+0],9)
+","+Numtostr(FRAMAindBIG[RightStrength+1],9)
+","+Numtostr(FRAMAindBIG[RightStrength+10],9)
+","+Numtostr(FRAMAindBIG[RightStrength+11],9)
+","+Numtostr(FRAMAindBIG[RightStrength+12],9)
+","+Numtostr(FRAMAindBIG[RightStrength+13],9)
+","+Numtostr(FRAMAindBIG[RightStrength+14],9)
+","+Numtostr(FRAMAindBIG[RightStrength+15],9)
+","+Numtostr(FRAMAindBIG[RightStrength+16],9)
+","+Numtostr(FRAMAindBIG[RightStrength+17],9)
+","+Numtostr(FRAMAindBIG[RightStrength+18],9)
+","+Numtostr(FRAMAindBIG[RightStrength+19],9)
+","+Numtostr(FRAMAindBIG[RightStrength+2],9)
+","+Numtostr(FRAMAindBIG[RightStrength+20],9)
+","+Numtostr(FRAMAindBIG[RightStrength+21],9)
+","+Numtostr(FRAMAindBIG[RightStrength+22],9)
+","+Numtostr(FRAMAindBIG[RightStrength+23],9)
+","+Numtostr(FRAMAindBIG[RightStrength+24],9)
+","+Numtostr(FRAMAindBIG[RightStrength+25],9)
+","+Numtostr(FRAMAindBIG[RightStrength+26],9)
+","+Numtostr(FRAMAindBIG[RightStrength+27],9)
+","+Numtostr(FRAMAindBIG[RightStrength+28],9)
+","+Numtostr(FRAMAindBIG[RightStrength+3],9)
+","+Numtostr(FRAMAindBIG[RightStrength+4],9)
+","+Numtostr(FRAMAindBIG[RightStrength+5],9)
			+" )"; 
			res3 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_feat3);
			srun3 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures3.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_feat3
					+ NewLine);
			end;
		End;
	End;
	 TblNum = TblNum+1;

	//(4) insert feature data into dbo.barFeatures4	
	if ReadWriteToDB and TblNum = 4  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		if  srun4 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_feat4 = "INSERT INTO  [dbo].[barFeatures4] VALUES ("
			+queryResult//MAX(loadId) 
			+","+Numtostr(loadGroupId,0) 
			+","+Numtostr(RightStrength,0)//RightStrength
			+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
			+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"'"+dateBarOpened+"' "+","+"'"+dayOfWeekBarOpened+"' "+","+"'"+dateTimeBarOpened+"' "+","+"'"+timeBarOpenedhhmmss+"' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)
+","+Numtostr(LINREGindBIG[RightStrength-2],9)
+","+Numtostr(LINREGindBIG[RightStrength-3],9)
+","+Numtostr(LINREGindBIG[RightStrength-4],9)
+","+Numtostr(LINREGindBX[RightStrength+0],9)
+","+Numtostr(LINREGindBX[RightStrength+1],9)
+","+Numtostr(LINREGindBX[RightStrength+10],9)
+","+Numtostr(LINREGindBX[RightStrength+11],9)
+","+Numtostr(LINREGindBX[RightStrength+12],9)
+","+Numtostr(LINREGindBX[RightStrength+13],9)
+","+Numtostr(LINREGindBX[RightStrength+14],9)
+","+Numtostr(LINREGindBX[RightStrength+15],9)
+","+Numtostr(LINREGindBX[RightStrength+16],9)
+","+Numtostr(LINREGindBX[RightStrength+17],9)
+","+Numtostr(LINREGindBX[RightStrength+18],9)
+","+Numtostr(LINREGindBX[RightStrength+19],9)
+","+Numtostr(LINREGindBX[RightStrength+2],9)
+","+Numtostr(LINREGindBX[RightStrength+20],9)
+","+Numtostr(LINREGindBX[RightStrength+21],9)
+","+Numtostr(LINREGindBX[RightStrength+22],9)
+","+Numtostr(LINREGindBX[RightStrength+23],9)
+","+Numtostr(LINREGindBX[RightStrength+24],9)
+","+Numtostr(LINREGindBX[RightStrength+25],9)
+","+Numtostr(LINREGindBX[RightStrength+26],9)
+","+Numtostr(LINREGindBX[RightStrength+27],9)
+","+Numtostr(LINREGindBX[RightStrength+28],9)
+","+Numtostr(LINREGindBX[RightStrength+3],9)
+","+Numtostr(LINREGindBX[RightStrength+4],9)
+","+Numtostr(LINREGindBX[RightStrength+5],9)
+","+Numtostr(LINREGindBX[RightStrength+6],9)
+","+Numtostr(LINREGindBX[RightStrength+7],9)
+","+Numtostr(LINREGindBX[RightStrength+8],9)
+","+Numtostr(LINREGindBX[RightStrength+9],9)
+","+Numtostr(LINREGindBX[RightStrength-1],9)
+","+Numtostr(LINREGindBX[RightStrength-2],9)
+","+Numtostr(LINREGindBX[RightStrength-3],9)
+","+Numtostr(LINREGindBX[RightStrength-4],9)
+","+Numtostr(LINREGindBXL[RightStrength+0],9)
+","+Numtostr(LINREGindBXL[RightStrength+1],9)
+","+Numtostr(LINREGindBXL[RightStrength+10],9)
+","+Numtostr(LINREGindBXL[RightStrength+11],9)
+","+Numtostr(LINREGindBXL[RightStrength+12],9)
+","+Numtostr(LINREGindBXL[RightStrength+13],9)
+","+Numtostr(LINREGindBXL[RightStrength+14],9)
+","+Numtostr(LINREGindBXL[RightStrength+15],9)
+","+Numtostr(LINREGindBXL[RightStrength+16],9)
+","+Numtostr(LINREGindBXL[RightStrength+17],9)
+","+Numtostr(LINREGindBXL[RightStrength+18],9)
+","+Numtostr(LINREGindBXL[RightStrength+19],9)
+","+Numtostr(LINREGindBXL[RightStrength+2],9)
+","+Numtostr(LINREGindBXL[RightStrength+20],9)
+","+Numtostr(LINREGindBXL[RightStrength+21],9)
+","+Numtostr(LINREGindBXL[RightStrength+22],9)
+","+Numtostr(LINREGindBXL[RightStrength+23],9)
+","+Numtostr(LINREGindBXL[RightStrength+24],9)
+","+Numtostr(LINREGindBXL[RightStrength+25],9)
+","+Numtostr(LINREGindBXL[RightStrength+26],9)
+","+Numtostr(LINREGindBXL[RightStrength+27],9)
+","+Numtostr(LINREGindBXL[RightStrength+28],9)
+","+Numtostr(LINREGindBXL[RightStrength+3],9)
+","+Numtostr(LINREGindBXL[RightStrength+4],9)
+","+Numtostr(LINREGindBXL[RightStrength+5],9)
+","+Numtostr(LINREGindBXL[RightStrength+6],9)
+","+Numtostr(LINREGindBXL[RightStrength+7],9)
+","+Numtostr(LINREGindBXL[RightStrength+8],9)
+","+Numtostr(LINREGindBXL[RightStrength+9],9)
+","+Numtostr(LINREGindBXL[RightStrength-1],9)
+","+Numtostr(LINREGindBXL[RightStrength-2],9)
+","+Numtostr(LINREGindBXL[RightStrength-3],9)
+","+Numtostr(LINREGindBXL[RightStrength-4],9)
+","+Numtostr(LINREGindMED[RightStrength+0],9)
+","+Numtostr(LINREGindMED[RightStrength+1],9)
+","+Numtostr(LINREGindMED[RightStrength+10],9)
+","+Numtostr(LINREGindMED[RightStrength+11],9)
+","+Numtostr(LINREGindMED[RightStrength+12],9)
+","+Numtostr(LINREGindMED[RightStrength+13],9)
+","+Numtostr(LINREGindMED[RightStrength+14],9)
+","+Numtostr(LINREGindMED[RightStrength+15],9)
+","+Numtostr(LINREGindMED[RightStrength+16],9)
+","+Numtostr(LINREGindMED[RightStrength+17],9)
+","+Numtostr(LINREGindMED[RightStrength+18],9)
+","+Numtostr(LINREGindMED[RightStrength+19],9)
+","+Numtostr(LINREGindMED[RightStrength+2],9)
+","+Numtostr(LINREGindMED[RightStrength+20],9)
+","+Numtostr(LINREGindMED[RightStrength+21],9)
+","+Numtostr(LINREGindMED[RightStrength+22],9)
+","+Numtostr(LINREGindMED[RightStrength+23],9)
+","+Numtostr(LINREGindMED[RightStrength+24],9)
+","+Numtostr(LINREGindMED[RightStrength+25],9)
+","+Numtostr(LINREGindMED[RightStrength+26],9)
+","+Numtostr(LINREGindMED[RightStrength+27],9)
+","+Numtostr(LINREGindMED[RightStrength+28],9)
+","+Numtostr(LINREGindMED[RightStrength+3],9)
+","+Numtostr(LINREGindMED[RightStrength+4],9)
+","+Numtostr(LINREGindMED[RightStrength+5],9)
+","+Numtostr(LINREGindMED[RightStrength+6],9)
+","+Numtostr(LINREGindMED[RightStrength+7],9)
+","+Numtostr(LINREGindMED[RightStrength+8],9)
+","+Numtostr(LINREGindMED[RightStrength+9],9)
+","+Numtostr(LINREGindMED[RightStrength-1],9)
+","+Numtostr(LINREGindMED[RightStrength-2],9)
+","+Numtostr(LINREGindMED[RightStrength-3],9)
+","+Numtostr(LINREGindMED[RightStrength-4],9)
+","+Numtostr(LINREGindSMALL[RightStrength+0],9)
+","+Numtostr(LINREGindSMALL[RightStrength+1],9)
+","+Numtostr(LINREGindSMALL[RightStrength+10],9)
+","+Numtostr(LINREGindSMALL[RightStrength+11],9)
+","+Numtostr(LINREGindSMALL[RightStrength+12],9)
+","+Numtostr(LINREGindSMALL[RightStrength+13],9)
+","+Numtostr(LINREGindSMALL[RightStrength+14],9)
+","+Numtostr(LINREGindSMALL[RightStrength+15],9)
+","+Numtostr(LINREGindSMALL[RightStrength+16],9)
+","+Numtostr(LINREGindSMALL[RightStrength+17],9)
+","+Numtostr(LINREGindSMALL[RightStrength+18],9)
+","+Numtostr(LINREGindSMALL[RightStrength+19],9)
+","+Numtostr(LINREGindSMALL[RightStrength+2],9)
+","+Numtostr(LINREGindSMALL[RightStrength+20],9)
+","+Numtostr(LINREGindSMALL[RightStrength+21],9)
+","+Numtostr(LINREGindSMALL[RightStrength+22],9)
+","+Numtostr(LINREGindSMALL[RightStrength+23],9)
+","+Numtostr(LINREGindSMALL[RightStrength+24],9)
+","+Numtostr(LINREGindSMALL[RightStrength+25],9)
+","+Numtostr(LINREGindSMALL[RightStrength+26],9)
+","+Numtostr(LINREGindSMALL[RightStrength+27],9)
+","+Numtostr(LINREGindSMALL[RightStrength+28],9)
+","+Numtostr(LINREGindSMALL[RightStrength+3],9)
+","+Numtostr(LINREGindSMALL[RightStrength+4],9)
+","+Numtostr(LINREGindSMALL[RightStrength+5],9)
+","+Numtostr(LINREGindSMALL[RightStrength+6],9)
+","+Numtostr(LINREGindSMALL[RightStrength+7],9)
+","+Numtostr(LINREGindSMALL[RightStrength+8],9)
+","+Numtostr(LINREGindSMALL[RightStrength+9],9)
+","+Numtostr(LINREGindSMALL[RightStrength-1],9)
+","+Numtostr(LINREGindSMALL[RightStrength-2],9)
+","+Numtostr(LINREGindSMALL[RightStrength-3],9)
+","+Numtostr(LINREGindSMALL[RightStrength-4],9)
+","+Numtostr(LINRGCindBIG[RightStrength+0],9)
+","+Numtostr(LINRGCindBIG[RightStrength+1],9)
+","+Numtostr(LINRGCindBIG[RightStrength+10],9)
+","+Numtostr(LINRGCindBIG[RightStrength+11],9)
+","+Numtostr(LINRGCindBIG[RightStrength+12],9)
+","+Numtostr(LINRGCindBIG[RightStrength+13],9)
+","+Numtostr(LINRGCindBIG[RightStrength+14],9)
+","+Numtostr(LINRGCindBIG[RightStrength+15],9)
+","+Numtostr(LINRGCindBIG[RightStrength+16],9)
+","+Numtostr(LINRGCindBIG[RightStrength+17],9)
+","+Numtostr(LINRGCindBIG[RightStrength+18],9)
+","+Numtostr(LINRGCindBIG[RightStrength+19],9)
+","+Numtostr(LINRGCindBIG[RightStrength+2],9)
+","+Numtostr(LINRGCindBIG[RightStrength+20],9)
+","+Numtostr(LINRGCindBIG[RightStrength+21],9)
+","+Numtostr(LINRGCindBIG[RightStrength+22],9)
+","+Numtostr(LINRGCindBIG[RightStrength+23],9)
+","+Numtostr(LINRGCindBIG[RightStrength+24],9)
+","+Numtostr(LINRGCindBIG[RightStrength+25],9)
+","+Numtostr(LINRGCindBIG[RightStrength+26],9)
+","+Numtostr(LINRGCindBIG[RightStrength+27],9)
+","+Numtostr(LINRGCindBIG[RightStrength+28],9)
+","+Numtostr(LINRGCindBIG[RightStrength+3],9)
+","+Numtostr(LINRGCindBIG[RightStrength+4],9)
+","+Numtostr(LINRGCindBIG[RightStrength+5],9)
+","+Numtostr(LINRGCindBIG[RightStrength+6],9)
+","+Numtostr(LINRGCindBIG[RightStrength+7],9)
+","+Numtostr(LINRGCindBIG[RightStrength+8],9)
+","+Numtostr(LINRGCindBIG[RightStrength+9],9)
+","+Numtostr(LINRGCindBIG[RightStrength-1],9)
+","+Numtostr(LINRGCindBIG[RightStrength-2],9)
+","+Numtostr(LINRGCindBIG[RightStrength-3],9)
+","+Numtostr(LINRGCindBIG[RightStrength-4],9)
+","+Numtostr(LINRGCindBX[RightStrength+0],9)
+","+Numtostr(LINRGCindBX[RightStrength+1],9)
+","+Numtostr(LINRGCindBX[RightStrength+10],9)
+","+Numtostr(LINRGCindBX[RightStrength+11],9)
+","+Numtostr(LINRGCindBX[RightStrength+12],9)
+","+Numtostr(LINRGCindBX[RightStrength+13],9)
+","+Numtostr(LINRGCindBX[RightStrength+14],9)
+","+Numtostr(LINRGCindBX[RightStrength+15],9)
+","+Numtostr(LINRGCindBX[RightStrength+16],9)
+","+Numtostr(LINRGCindBX[RightStrength+17],9)
+","+Numtostr(LINRGCindBX[RightStrength+18],9)
+","+Numtostr(LINRGCindBX[RightStrength+19],9)
+","+Numtostr(LINRGCindBX[RightStrength+2],9)
+","+Numtostr(LINRGCindBX[RightStrength+20],9)
+","+Numtostr(LINRGCindBX[RightStrength+21],9)
+","+Numtostr(LINRGCindBX[RightStrength+22],9)
+","+Numtostr(LINRGCindBX[RightStrength+23],9)
+","+Numtostr(LINRGCindBX[RightStrength+24],9)
+","+Numtostr(LINRGCindBX[RightStrength+25],9)
+","+Numtostr(LINRGCindBX[RightStrength+26],9)
+","+Numtostr(LINRGCindBX[RightStrength+27],9)
+","+Numtostr(LINRGCindBX[RightStrength+28],9)
+","+Numtostr(LINRGCindBX[RightStrength+3],9)
+","+Numtostr(LINRGCindBX[RightStrength+4],9)
+","+Numtostr(LINRGCindBX[RightStrength+5],9)
+","+Numtostr(LINRGCindBX[RightStrength+6],9)
+","+Numtostr(LINRGCindBX[RightStrength+7],9)
+","+Numtostr(LINRGCindBX[RightStrength+8],9)
+","+Numtostr(LINRGCindBX[RightStrength+9],9)
+","+Numtostr(LINRGCindBX[RightStrength-1],9)
+","+Numtostr(LINRGCindBX[RightStrength-2],9)
+","+Numtostr(LINRGCindBX[RightStrength-3],9)
+","+Numtostr(LINRGCindBX[RightStrength-4],9)
+","+Numtostr(LINRGCindBXL[RightStrength+0],9)
+","+Numtostr(LINRGCindBXL[RightStrength+1],9)
+","+Numtostr(LINRGCindBXL[RightStrength+10],9)
+","+Numtostr(LINRGCindBXL[RightStrength+11],9)
+","+Numtostr(LINRGCindBXL[RightStrength+12],9)
+","+Numtostr(LINRGCindBXL[RightStrength+13],9)
+","+Numtostr(LINRGCindBXL[RightStrength+14],9)
+","+Numtostr(LINRGCindBXL[RightStrength+15],9)
+","+Numtostr(LINRGCindBXL[RightStrength+16],9)
+","+Numtostr(LINRGCindBXL[RightStrength+17],9)
+","+Numtostr(LINRGCindBXL[RightStrength+18],9)
+","+Numtostr(LINRGCindBXL[RightStrength+19],9)
+","+Numtostr(LINRGCindBXL[RightStrength+2],9)
+","+Numtostr(LINRGCindBXL[RightStrength+20],9)
+","+Numtostr(LINRGCindBXL[RightStrength+21],9)
+","+Numtostr(LINRGCindBXL[RightStrength+22],9)
+","+Numtostr(LINRGCindBXL[RightStrength+23],9)
+","+Numtostr(LINRGCindBXL[RightStrength+24],9)
+","+Numtostr(LINRGCindBXL[RightStrength+25],9)
+","+Numtostr(LINRGCindBXL[RightStrength+26],9)
+","+Numtostr(LINRGCindBXL[RightStrength+27],9)
+","+Numtostr(LINRGCindBXL[RightStrength+28],9)
+","+Numtostr(LINRGCindBXL[RightStrength+3],9)
+","+Numtostr(LINRGCindBXL[RightStrength+4],9)
+","+Numtostr(LINRGCindBXL[RightStrength+5],9)
+","+Numtostr(LINRGCindBXL[RightStrength+6],9)
+","+Numtostr(LINRGCindBXL[RightStrength+7],9)
+","+Numtostr(LINRGCindBXL[RightStrength+8],9)
+","+Numtostr(LINRGCindBXL[RightStrength+9],9)
+","+Numtostr(LINRGCindBXL[RightStrength-1],9)
+","+Numtostr(LINRGCindBXL[RightStrength-2],9)
+","+Numtostr(LINRGCindBXL[RightStrength-3],9)
+","+Numtostr(LINRGCindBXL[RightStrength-4],9)
+","+Numtostr(LINRGCindMED[RightStrength+0],9)
+","+Numtostr(LINRGCindMED[RightStrength+1],9)
+","+Numtostr(LINRGCindMED[RightStrength+10],9)
+","+Numtostr(LINRGCindMED[RightStrength+11],9)
+","+Numtostr(LINRGCindMED[RightStrength+12],9)
+","+Numtostr(LINRGCindMED[RightStrength+13],9)
+","+Numtostr(LINRGCindMED[RightStrength+14],9)
+","+Numtostr(LINRGCindMED[RightStrength+15],9)
+","+Numtostr(LINRGCindMED[RightStrength+16],9)
+","+Numtostr(LINRGCindMED[RightStrength+17],9)
+","+Numtostr(LINRGCindMED[RightStrength+18],9)
+","+Numtostr(LINRGCindMED[RightStrength+19],9)
+","+Numtostr(LINRGCindMED[RightStrength+2],9)
+","+Numtostr(LINRGCindMED[RightStrength+20],9)
+","+Numtostr(LINRGCindMED[RightStrength+21],9)
+","+Numtostr(LINRGCindMED[RightStrength+22],9)
+","+Numtostr(LINRGCindMED[RightStrength+23],9)
+","+Numtostr(LINRGCindMED[RightStrength+24],9)
+","+Numtostr(LINRGCindMED[RightStrength+25],9)
+","+Numtostr(LINRGCindMED[RightStrength+26],9)
+","+Numtostr(LINRGCindMED[RightStrength+27],9)
+","+Numtostr(LINRGCindMED[RightStrength+28],9)
+","+Numtostr(LINRGCindMED[RightStrength+3],9)
+","+Numtostr(LINRGCindMED[RightStrength+4],9)
+","+Numtostr(LINRGCindMED[RightStrength+5],9)
+","+Numtostr(LINRGCindMED[RightStrength+6],9)
+","+Numtostr(LINRGCindMED[RightStrength+7],9)
+","+Numtostr(LINRGCindMED[RightStrength+8],9)
+","+Numtostr(LINRGCindMED[RightStrength+9],9)
+","+Numtostr(LINRGCindMED[RightStrength-1],9)
+","+Numtostr(LINRGCindMED[RightStrength-2],9)
+","+Numtostr(LINRGCindMED[RightStrength-3],9)
+","+Numtostr(LINRGCindMED[RightStrength-4],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+0],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+1],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+10],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+11],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+12],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+13],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+14],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+15],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+16],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+17],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+18],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+19],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+2],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+20],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+21],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+22],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+23],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+24],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+25],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+26],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+27],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+28],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+3],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+4],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+5],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+6],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+7],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+8],9)
+","+Numtostr(LINRGCindSMALL[RightStrength+9],9)
+","+Numtostr(LINRGCindSMALL[RightStrength-1],9)
+","+Numtostr(LINRGCindSMALL[RightStrength-2],9)
+","+Numtostr(LINRGCindSMALL[RightStrength-3],9)
+","+Numtostr(LINRGCindSMALL[RightStrength-4],9)
+","+Numtostr(LRSIindBIG[RightStrength+0],9)
+","+Numtostr(LRSIindBIG[RightStrength+1],9)
+","+Numtostr(LRSIindBIG[RightStrength+10],9)
+","+Numtostr(LRSIindBIG[RightStrength+11],9)
+","+Numtostr(LRSIindBIG[RightStrength+12],9)
+","+Numtostr(LRSIindBIG[RightStrength+13],9)
+","+Numtostr(LRSIindBIG[RightStrength+14],9)
+","+Numtostr(LRSIindBIG[RightStrength+15],9)
+","+Numtostr(LRSIindBIG[RightStrength+16],9)
+","+Numtostr(LRSIindBIG[RightStrength+17],9)
+","+Numtostr(LRSIindBIG[RightStrength+18],9)
+","+Numtostr(LRSIindBIG[RightStrength+19],9)
+","+Numtostr(LRSIindBIG[RightStrength+2],9)
+","+Numtostr(LRSIindBIG[RightStrength+20],9)
+","+Numtostr(LRSIindBIG[RightStrength+21],9)
+","+Numtostr(LRSIindBIG[RightStrength+22],9)
+","+Numtostr(LRSIindBIG[RightStrength+23],9)
+","+Numtostr(LRSIindBIG[RightStrength+24],9)
+","+Numtostr(LRSIindBIG[RightStrength+25],9)
+","+Numtostr(LRSIindBIG[RightStrength+26],9)
+","+Numtostr(LRSIindBIG[RightStrength+27],9)
+","+Numtostr(LRSIindBIG[RightStrength+28],9)
+","+Numtostr(LRSIindBIG[RightStrength+3],9)
+","+Numtostr(LRSIindBIG[RightStrength+4],9)
+","+Numtostr(LRSIindBIG[RightStrength+5],9)
+","+Numtostr(LRSIindBIG[RightStrength+6],9)
+","+Numtostr(LRSIindBIG[RightStrength+7],9)
+","+Numtostr(LRSIindBIG[RightStrength+8],9)
+","+Numtostr(LRSIindBIG[RightStrength+9],9)
+","+Numtostr(LRSIindBIG[RightStrength-1],9)
+","+Numtostr(LRSIindBIG[RightStrength-2],9)
+","+Numtostr(LRSIindBIG[RightStrength-3],9)
+","+Numtostr(LRSIindBIG[RightStrength-4],9)
+","+Numtostr(LRSIindBX[RightStrength+0],9)
+","+Numtostr(LRSIindBX[RightStrength+1],9)
+","+Numtostr(LRSIindBX[RightStrength+10],9)
+","+Numtostr(LRSIindBX[RightStrength+11],9)
+","+Numtostr(LRSIindBX[RightStrength+12],9)
+","+Numtostr(LRSIindBX[RightStrength+13],9)
+","+Numtostr(LRSIindBX[RightStrength+14],9)
+","+Numtostr(LRSIindBX[RightStrength+15],9)
+","+Numtostr(LRSIindBX[RightStrength+16],9)
+","+Numtostr(LRSIindBX[RightStrength+17],9)
+","+Numtostr(LRSIindBX[RightStrength+18],9)
+","+Numtostr(LRSIindBX[RightStrength+19],9)
+","+Numtostr(LRSIindBX[RightStrength+2],9)
+","+Numtostr(LRSIindBX[RightStrength+20],9)
+","+Numtostr(LRSIindBX[RightStrength+21],9)
+","+Numtostr(LRSIindBX[RightStrength+22],9)
+","+Numtostr(LRSIindBX[RightStrength+23],9)
+","+Numtostr(LRSIindBX[RightStrength+24],9)
+","+Numtostr(LRSIindBX[RightStrength+25],9)
+","+Numtostr(LRSIindBX[RightStrength+26],9)
+","+Numtostr(LRSIindBX[RightStrength+27],9)
+","+Numtostr(LRSIindBX[RightStrength+28],9)
+","+Numtostr(LRSIindBX[RightStrength+3],9)
+","+Numtostr(LRSIindBX[RightStrength+4],9)
+","+Numtostr(LRSIindBX[RightStrength+5],9)
+","+Numtostr(LRSIindBX[RightStrength+6],9)
+","+Numtostr(LRSIindBX[RightStrength+7],9)
+","+Numtostr(LRSIindBX[RightStrength+8],9)
+","+Numtostr(LRSIindBX[RightStrength+9],9)
+","+Numtostr(LRSIindBX[RightStrength-1],9)
+","+Numtostr(LRSIindBX[RightStrength-2],9)
+","+Numtostr(LRSIindBX[RightStrength-3],9)
+","+Numtostr(LRSIindBX[RightStrength-4],9)
+","+Numtostr(LRSIindBXL[RightStrength+0],9)
+","+Numtostr(LRSIindBXL[RightStrength+1],9)
+","+Numtostr(LRSIindBXL[RightStrength+10],9)
+","+Numtostr(LRSIindBXL[RightStrength+11],9)
+","+Numtostr(LRSIindBXL[RightStrength+12],9)
+","+Numtostr(LRSIindBXL[RightStrength+13],9)
+","+Numtostr(LRSIindBXL[RightStrength+14],9)
+","+Numtostr(LRSIindBXL[RightStrength+15],9)
+","+Numtostr(LRSIindBXL[RightStrength+16],9)
+","+Numtostr(LRSIindBXL[RightStrength+17],9)
+","+Numtostr(LRSIindBXL[RightStrength+18],9)
+","+Numtostr(LRSIindBXL[RightStrength+19],9)
+","+Numtostr(LRSIindBXL[RightStrength+2],9)
+","+Numtostr(LRSIindBXL[RightStrength+20],9)
+","+Numtostr(LRSIindBXL[RightStrength+21],9)
+","+Numtostr(LRSIindBXL[RightStrength+22],9)
+","+Numtostr(LRSIindBXL[RightStrength+23],9)
+","+Numtostr(LRSIindBXL[RightStrength+24],9)
+","+Numtostr(LRSIindBXL[RightStrength+25],9)
+","+Numtostr(LRSIindBXL[RightStrength+26],9)
+","+Numtostr(LRSIindBXL[RightStrength+27],9)
+","+Numtostr(LRSIindBXL[RightStrength+28],9)
+","+Numtostr(LRSIindBXL[RightStrength+3],9)
+","+Numtostr(LRSIindBXL[RightStrength+4],9)
+","+Numtostr(LRSIindBXL[RightStrength+5],9)
+","+Numtostr(LRSIindBXL[RightStrength+6],9)
+","+Numtostr(LRSIindBXL[RightStrength+7],9)
+","+Numtostr(LRSIindBXL[RightStrength+8],9)
+","+Numtostr(LRSIindBXL[RightStrength+9],9)
+","+Numtostr(LRSIindBXL[RightStrength-1],9)
+","+Numtostr(LRSIindBXL[RightStrength-2],9)
+","+Numtostr(LRSIindBXL[RightStrength-3],9)
+","+Numtostr(LRSIindBXL[RightStrength-4],9)
+","+Numtostr(LRSIindMED[RightStrength+0],9)
+","+Numtostr(LRSIindMED[RightStrength+1],9)
+","+Numtostr(LRSIindMED[RightStrength+10],9)
+","+Numtostr(LRSIindMED[RightStrength+11],9)
+","+Numtostr(LRSIindMED[RightStrength+12],9)
+","+Numtostr(LRSIindMED[RightStrength+13],9)
+","+Numtostr(LRSIindMED[RightStrength+14],9)
+","+Numtostr(LRSIindMED[RightStrength+15],9)
+","+Numtostr(LRSIindMED[RightStrength+16],9)
+","+Numtostr(LRSIindMED[RightStrength+17],9)
+","+Numtostr(LRSIindMED[RightStrength+18],9)
+","+Numtostr(LRSIindMED[RightStrength+19],9)
+","+Numtostr(LRSIindMED[RightStrength+2],9)
+","+Numtostr(LRSIindMED[RightStrength+20],9)
+","+Numtostr(LRSIindMED[RightStrength+21],9)
+","+Numtostr(LRSIindMED[RightStrength+22],9)
+","+Numtostr(LRSIindMED[RightStrength+23],9)
+","+Numtostr(LRSIindMED[RightStrength+24],9)
+","+Numtostr(LRSIindMED[RightStrength+25],9)
+","+Numtostr(LRSIindMED[RightStrength+26],9)
+","+Numtostr(LRSIindMED[RightStrength+27],9)
+","+Numtostr(LRSIindMED[RightStrength+28],9)
+","+Numtostr(LRSIindMED[RightStrength+3],9)
+","+Numtostr(LRSIindMED[RightStrength+4],9)
+","+Numtostr(LRSIindMED[RightStrength+5],9)
+","+Numtostr(LRSIindMED[RightStrength+6],9)
+","+Numtostr(LRSIindMED[RightStrength+7],9)
+","+Numtostr(LRSIindMED[RightStrength+8],9)
+","+Numtostr(LRSIindMED[RightStrength+9],9)
+","+Numtostr(LRSIindMED[RightStrength-1],9)
+","+Numtostr(LRSIindMED[RightStrength-2],9)
+","+Numtostr(LRSIindMED[RightStrength-3],9)
+","+Numtostr(LRSIindMED[RightStrength-4],9)
+","+Numtostr(LRSIindSMALL[RightStrength+0],9)
+","+Numtostr(LRSIindSMALL[RightStrength+1],9)
+","+Numtostr(LRSIindSMALL[RightStrength+10],9)
+","+Numtostr(LRSIindSMALL[RightStrength+11],9)
+","+Numtostr(LRSIindSMALL[RightStrength+12],9)
+","+Numtostr(LRSIindSMALL[RightStrength+13],9)
+","+Numtostr(LRSIindSMALL[RightStrength+14],9)
+","+Numtostr(LRSIindSMALL[RightStrength+15],9)
+","+Numtostr(LRSIindSMALL[RightStrength+16],9)
+","+Numtostr(LRSIindSMALL[RightStrength+17],9)
+","+Numtostr(LRSIindSMALL[RightStrength+18],9)
+","+Numtostr(LRSIindSMALL[RightStrength+19],9)
+","+Numtostr(LRSIindSMALL[RightStrength+2],9)
+","+Numtostr(LRSIindSMALL[RightStrength+20],9)
+","+Numtostr(LRSIindSMALL[RightStrength+21],9)
+","+Numtostr(LRSIindSMALL[RightStrength+22],9)
+","+Numtostr(LRSIindSMALL[RightStrength+23],9)
+","+Numtostr(LRSIindSMALL[RightStrength+24],9)
+","+Numtostr(LRSIindSMALL[RightStrength+25],9)
+","+Numtostr(LRSIindSMALL[RightStrength+26],9)
+","+Numtostr(LRSIindSMALL[RightStrength+27],9)
+","+Numtostr(LRSIindSMALL[RightStrength+28],9)
+","+Numtostr(LRSIindSMALL[RightStrength+3],9)
+","+Numtostr(LRSIindSMALL[RightStrength+4],9)
+","+Numtostr(LRSIindSMALL[RightStrength+5],9)
+","+Numtostr(LRSIindSMALL[RightStrength+6],9)
+","+Numtostr(LRSIindSMALL[RightStrength+7],9)
+","+Numtostr(LRSIindSMALL[RightStrength+8],9)
+","+Numtostr(LRSIindSMALL[RightStrength+9],9)
+","+Numtostr(LRSIindSMALL[RightStrength-1],9)
+","+Numtostr(LRSIindSMALL[RightStrength-2],9)
+","+Numtostr(LRSIindSMALL[RightStrength-3],9)
+","+Numtostr(LRSIindSMALL[RightStrength-4],9)
+","+Numtostr(MaALMAindBIG[RightStrength+0],9)
+","+Numtostr(MaALMAindBIG[RightStrength+1],9)
+","+Numtostr(MaALMAindBIG[RightStrength+10],9)
+","+Numtostr(MaALMAindBIG[RightStrength+11],9)
+","+Numtostr(MaALMAindBIG[RightStrength+12],9)
+","+Numtostr(MaALMAindBIG[RightStrength+13],9)
+","+Numtostr(MaALMAindBIG[RightStrength+14],9)
+","+Numtostr(MaALMAindBIG[RightStrength+15],9)
+","+Numtostr(MaALMAindBIG[RightStrength+16],9)
+","+Numtostr(MaALMAindBIG[RightStrength+17],9)
+","+Numtostr(MaALMAindBIG[RightStrength+18],9)
+","+Numtostr(MaALMAindBIG[RightStrength+19],9)
+","+Numtostr(MaALMAindBIG[RightStrength+2],9)
+","+Numtostr(MaALMAindBIG[RightStrength+20],9)
+","+Numtostr(MaALMAindBIG[RightStrength+21],9)
+","+Numtostr(MaALMAindBIG[RightStrength+22],9)
+","+Numtostr(MaALMAindBIG[RightStrength+23],9)
+","+Numtostr(MaALMAindBIG[RightStrength+24],9)
+","+Numtostr(MaALMAindBIG[RightStrength+25],9)
+","+Numtostr(MaALMAindBIG[RightStrength+26],9)
+","+Numtostr(MaALMAindBIG[RightStrength+27],9)
+","+Numtostr(MaALMAindBIG[RightStrength+28],9)
+","+Numtostr(MaALMAindBIG[RightStrength+3],9)
+","+Numtostr(MaALMAindBIG[RightStrength+4],9)
+","+Numtostr(MaALMAindBIG[RightStrength+5],9)
+","+Numtostr(MaALMAindBIG[RightStrength+6],9)
+","+Numtostr(MaALMAindBIG[RightStrength+7],9)
+","+Numtostr(MaALMAindBIG[RightStrength+8],9)
+","+Numtostr(MaALMAindBIG[RightStrength+9],9)
+","+Numtostr(MaALMAindBIG[RightStrength-1],9)
+","+Numtostr(MaALMAindBIG[RightStrength-2],9)
+","+Numtostr(MaALMAindBIG[RightStrength-3],9)
+","+Numtostr(MaALMAindBIG[RightStrength-4],9)
+","+Numtostr(MaALMAindBX[RightStrength+0],9)
+","+Numtostr(MaALMAindBX[RightStrength+1],9)
			+" )"; 
			res4 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_feat4);
			srun4 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures4.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_feat4
					+ NewLine);
			end;
		End;
	End;
	 TblNum = TblNum+1;
	 

	//(4) insert feature data into dbo.barFeatures5	
	if ReadWriteToDB and TblNum = 5  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		if  srun5 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_feat5 = "INSERT INTO  [dbo].[barFeatures5] VALUES ("
			+queryResult//MAX(loadId) 
			+","+Numtostr(loadGroupId,0) 
			+","+Numtostr(RightStrength,0)//RightStrength
			+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
			+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"'"+dateBarOpened+"' "+","+"'"+dayOfWeekBarOpened+"' "+","+"'"+dateTimeBarOpened+"' "+","+"'"+timeBarOpenedhhmmss+"' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)
+","+Numtostr(MaEMAindBX[RightStrength+15],9)
+","+Numtostr(MaEMAindBX[RightStrength+16],9)
+","+Numtostr(MaEMAindBX[RightStrength+17],9)
+","+Numtostr(MaEMAindBX[RightStrength+18],9)
+","+Numtostr(MaEMAindBX[RightStrength+19],9)
+","+Numtostr(MaEMAindBX[RightStrength+2],9)
+","+Numtostr(MaEMAindBX[RightStrength+20],9)
+","+Numtostr(MaEMAindBX[RightStrength+21],9)
+","+Numtostr(MaEMAindBX[RightStrength+22],9)
+","+Numtostr(MaEMAindBX[RightStrength+23],9)
+","+Numtostr(MaEMAindBX[RightStrength+24],9)
+","+Numtostr(MaEMAindBX[RightStrength+25],9)
+","+Numtostr(MaEMAindBX[RightStrength+26],9)
+","+Numtostr(MaEMAindBX[RightStrength+27],9)
+","+Numtostr(MaEMAindBX[RightStrength+28],9)
+","+Numtostr(MaEMAindBX[RightStrength+3],9)
+","+Numtostr(MaEMAindBX[RightStrength+4],9)
+","+Numtostr(MaEMAindBX[RightStrength+5],9)
+","+Numtostr(MaEMAindBX[RightStrength+6],9)
+","+Numtostr(MaEMAindBX[RightStrength+7],9)
+","+Numtostr(MaEMAindBX[RightStrength+8],9)
+","+Numtostr(MaEMAindBX[RightStrength+9],9)
+","+Numtostr(MaEMAindBX[RightStrength-1],9)
+","+Numtostr(MaEMAindBX[RightStrength-2],9)
+","+Numtostr(MaEMAindBX[RightStrength-3],9)
+","+Numtostr(MaEMAindBX[RightStrength-4],9)
+","+Numtostr(MaEMAindBXL[RightStrength+0],9)
+","+Numtostr(MaEMAindBXL[RightStrength+1],9)
+","+Numtostr(MaEMAindBXL[RightStrength+10],9)
+","+Numtostr(MaEMAindBXL[RightStrength+11],9)
+","+Numtostr(MaEMAindBXL[RightStrength+12],9)
+","+Numtostr(MaEMAindBXL[RightStrength+13],9)
+","+Numtostr(MaEMAindBXL[RightStrength+14],9)
+","+Numtostr(MaEMAindBXL[RightStrength+15],9)
+","+Numtostr(MaEMAindBXL[RightStrength+16],9)
+","+Numtostr(MaEMAindBXL[RightStrength+17],9)
+","+Numtostr(MaEMAindBXL[RightStrength+18],9)
+","+Numtostr(MaEMAindBXL[RightStrength+19],9)
+","+Numtostr(MaEMAindBXL[RightStrength+2],9)
+","+Numtostr(MaEMAindBXL[RightStrength+20],9)
+","+Numtostr(MaEMAindBXL[RightStrength+21],9)
+","+Numtostr(MaEMAindBXL[RightStrength+22],9)
+","+Numtostr(MaEMAindBXL[RightStrength+23],9)
+","+Numtostr(MaEMAindBXL[RightStrength+24],9)
+","+Numtostr(MaEMAindBXL[RightStrength+25],9)
+","+Numtostr(MaEMAindBXL[RightStrength+26],9)
+","+Numtostr(MaEMAindBXL[RightStrength+27],9)
+","+Numtostr(MaEMAindBXL[RightStrength+28],9)
+","+Numtostr(MaEMAindBXL[RightStrength+3],9)
+","+Numtostr(MaEMAindBXL[RightStrength+4],9)
+","+Numtostr(MaEMAindBXL[RightStrength+5],9)
+","+Numtostr(MaEMAindBXL[RightStrength+6],9)
+","+Numtostr(MaEMAindBXL[RightStrength+7],9)
+","+Numtostr(MaEMAindBXL[RightStrength+8],9)
+","+Numtostr(MaEMAindBXL[RightStrength+9],9)
+","+Numtostr(MaEMAindBXL[RightStrength-1],9)
+","+Numtostr(MaEMAindBXL[RightStrength-2],9)
+","+Numtostr(MaEMAindBXL[RightStrength-3],9)
+","+Numtostr(MaEMAindBXL[RightStrength-4],9)
+","+Numtostr(MaEMAindMED[RightStrength+0],9)
+","+Numtostr(MaEMAindMED[RightStrength+1],9)
+","+Numtostr(MaEMAindMED[RightStrength+10],9)
+","+Numtostr(MaEMAindMED[RightStrength+11],9)
+","+Numtostr(MaEMAindMED[RightStrength+12],9)
+","+Numtostr(MaEMAindMED[RightStrength+13],9)
+","+Numtostr(MaEMAindMED[RightStrength+14],9)
+","+Numtostr(MaEMAindMED[RightStrength+15],9)
+","+Numtostr(MaEMAindMED[RightStrength+16],9)
+","+Numtostr(MaEMAindMED[RightStrength+17],9)
+","+Numtostr(MaEMAindMED[RightStrength+18],9)
+","+Numtostr(MaEMAindMED[RightStrength+19],9)
+","+Numtostr(MaEMAindMED[RightStrength+2],9)
+","+Numtostr(MaEMAindMED[RightStrength+20],9)
+","+Numtostr(MaEMAindMED[RightStrength+21],9)
+","+Numtostr(MaEMAindMED[RightStrength+22],9)
+","+Numtostr(MaEMAindMED[RightStrength+23],9)
+","+Numtostr(MaEMAindMED[RightStrength+24],9)
+","+Numtostr(MaEMAindMED[RightStrength+25],9)
+","+Numtostr(MaEMAindMED[RightStrength+26],9)
+","+Numtostr(MaEMAindMED[RightStrength+27],9)
+","+Numtostr(MaEMAindMED[RightStrength+28],9)
+","+Numtostr(MaEMAindMED[RightStrength+3],9)
+","+Numtostr(MaEMAindMED[RightStrength+4],9)
+","+Numtostr(MaEMAindMED[RightStrength+5],9)
+","+Numtostr(MaEMAindMED[RightStrength+6],9)
+","+Numtostr(MaEMAindMED[RightStrength+7],9)
+","+Numtostr(MaEMAindMED[RightStrength+8],9)
+","+Numtostr(MaEMAindMED[RightStrength+9],9)
+","+Numtostr(MaEMAindMED[RightStrength-1],9)
+","+Numtostr(MaEMAindMED[RightStrength-2],9)
+","+Numtostr(MaEMAindMED[RightStrength-3],9)
+","+Numtostr(MaEMAindMED[RightStrength-4],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+0],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+1],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+10],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+11],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+12],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+13],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+14],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+15],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+16],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+17],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+18],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+19],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+2],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+20],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+21],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+22],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+23],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+24],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+25],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+26],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+27],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+28],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+3],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+4],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+5],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+6],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+7],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+8],9)
+","+Numtostr(MaEMAindSMALL[RightStrength+9],9)
+","+Numtostr(MaEMAindSMALL[RightStrength-1],9)
+","+Numtostr(MaEMAindSMALL[RightStrength-2],9)
+","+Numtostr(MaEMAindSMALL[RightStrength-3],9)
+","+Numtostr(MaEMAindSMALL[RightStrength-4],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+0],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+1],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+10],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+11],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+12],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+13],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+14],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+15],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+16],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+17],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+18],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+19],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+2],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+20],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+21],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+22],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+23],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+24],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+25],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+26],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+27],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+28],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+3],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+4],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+5],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+6],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+7],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+8],9)
+","+Numtostr(MaEPMAindBIG[RightStrength+9],9)
+","+Numtostr(MaEPMAindBIG[RightStrength-1],9)
+","+Numtostr(MaEPMAindBIG[RightStrength-2],9)
+","+Numtostr(MaEPMAindBIG[RightStrength-3],9)
+","+Numtostr(MaEPMAindBIG[RightStrength-4],9)
+","+Numtostr(MaEPMAindBX[RightStrength+0],9)
+","+Numtostr(MaEPMAindBX[RightStrength+1],9)
+","+Numtostr(MaEPMAindBX[RightStrength+10],9)
+","+Numtostr(MaEPMAindBX[RightStrength+11],9)
+","+Numtostr(MaEPMAindBX[RightStrength+12],9)
+","+Numtostr(MaEPMAindBX[RightStrength+13],9)
+","+Numtostr(MaEPMAindBX[RightStrength+14],9)
+","+Numtostr(MaEPMAindBX[RightStrength+15],9)
+","+Numtostr(MaEPMAindBX[RightStrength+16],9)
+","+Numtostr(MaEPMAindBX[RightStrength+17],9)
+","+Numtostr(MaEPMAindBX[RightStrength+18],9)
+","+Numtostr(MaEPMAindBX[RightStrength+19],9)
+","+Numtostr(MaEPMAindBX[RightStrength+2],9)
+","+Numtostr(MaEPMAindBX[RightStrength+20],9)
+","+Numtostr(MaEPMAindBX[RightStrength+21],9)
+","+Numtostr(MaEPMAindBX[RightStrength+22],9)
+","+Numtostr(MaEPMAindBX[RightStrength+23],9)
+","+Numtostr(MaEPMAindBX[RightStrength+24],9)
+","+Numtostr(MaEPMAindBX[RightStrength+25],9)
+","+Numtostr(MaEPMAindBX[RightStrength+26],9)
+","+Numtostr(MaEPMAindBX[RightStrength+27],9)
+","+Numtostr(MaEPMAindBX[RightStrength+28],9)
+","+Numtostr(MaEPMAindBX[RightStrength+3],9)
+","+Numtostr(MaEPMAindBX[RightStrength+4],9)
+","+Numtostr(MaEPMAindBX[RightStrength+5],9)
+","+Numtostr(MaEPMAindBX[RightStrength+6],9)
+","+Numtostr(MaEPMAindBX[RightStrength+7],9)
+","+Numtostr(MaEPMAindBX[RightStrength+8],9)
+","+Numtostr(MaEPMAindBX[RightStrength+9],9)
+","+Numtostr(MaEPMAindBX[RightStrength-1],9)
+","+Numtostr(MaEPMAindBX[RightStrength-2],9)
+","+Numtostr(MaEPMAindBX[RightStrength-3],9)
+","+Numtostr(MaEPMAindBX[RightStrength-4],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+0],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+1],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+10],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+11],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+12],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+13],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+14],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+15],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+16],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+17],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+18],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+19],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+2],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+20],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+21],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+22],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+23],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+24],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+25],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+26],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+27],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+28],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+3],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+4],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+5],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+6],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+7],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+8],9)
+","+Numtostr(MaEPMAindBXL[RightStrength+9],9)
+","+Numtostr(MaEPMAindBXL[RightStrength-1],9)
+","+Numtostr(MaEPMAindBXL[RightStrength-2],9)
+","+Numtostr(MaEPMAindBXL[RightStrength-3],9)
+","+Numtostr(MaEPMAindBXL[RightStrength-4],9)
+","+Numtostr(MaEPMAindMED[RightStrength+0],9)
+","+Numtostr(MaEPMAindMED[RightStrength+1],9)
+","+Numtostr(MaEPMAindMED[RightStrength+10],9)
+","+Numtostr(MaEPMAindMED[RightStrength+11],9)
+","+Numtostr(MaEPMAindMED[RightStrength+12],9)
+","+Numtostr(MaEPMAindMED[RightStrength+13],9)
+","+Numtostr(MaEPMAindMED[RightStrength+14],9)
+","+Numtostr(MaEPMAindMED[RightStrength+15],9)
+","+Numtostr(MaEPMAindMED[RightStrength+16],9)
+","+Numtostr(MaEPMAindMED[RightStrength+17],9)
+","+Numtostr(MaEPMAindMED[RightStrength+18],9)
+","+Numtostr(MaEPMAindMED[RightStrength+19],9)
+","+Numtostr(MaEPMAindMED[RightStrength+2],9)
+","+Numtostr(MaEPMAindMED[RightStrength+20],9)
+","+Numtostr(MaEPMAindMED[RightStrength+21],9)
+","+Numtostr(MaEPMAindMED[RightStrength+22],9)
+","+Numtostr(MaEPMAindMED[RightStrength+23],9)
+","+Numtostr(MaEPMAindMED[RightStrength+24],9)
+","+Numtostr(MaEPMAindMED[RightStrength+25],9)
+","+Numtostr(MaEPMAindMED[RightStrength+26],9)
+","+Numtostr(MaEPMAindMED[RightStrength+27],9)
+","+Numtostr(MaEPMAindMED[RightStrength+28],9)
+","+Numtostr(MaEPMAindMED[RightStrength+3],9)
+","+Numtostr(MaEPMAindMED[RightStrength+4],9)
+","+Numtostr(MaEPMAindMED[RightStrength+5],9)
+","+Numtostr(MaEPMAindMED[RightStrength+6],9)
+","+Numtostr(MaEPMAindMED[RightStrength+7],9)
+","+Numtostr(MaEPMAindMED[RightStrength+8],9)
+","+Numtostr(MaEPMAindMED[RightStrength+9],9)
+","+Numtostr(MaEPMAindMED[RightStrength-1],9)
+","+Numtostr(MaEPMAindMED[RightStrength-2],9)
+","+Numtostr(MaEPMAindMED[RightStrength-3],9)
+","+Numtostr(MaEPMAindMED[RightStrength-4],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+0],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+1],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+10],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+11],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+12],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+13],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+14],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+15],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+16],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+17],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+18],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+19],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+2],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+20],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+21],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+22],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+23],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+24],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+25],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+26],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+27],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+28],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+3],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+4],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+5],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+6],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+7],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+8],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength+9],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength-1],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength-2],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength-3],9)
+","+Numtostr(MaEPMAindSMALL[RightStrength-4],9)
+","+Numtostr(MaHMAindBIG[RightStrength+0],9)
+","+Numtostr(MaHMAindBIG[RightStrength+1],9)
+","+Numtostr(MaHMAindBIG[RightStrength+10],9)
+","+Numtostr(MaHMAindBIG[RightStrength+11],9)
+","+Numtostr(MaHMAindBIG[RightStrength+12],9)
+","+Numtostr(MaHMAindBIG[RightStrength+13],9)
+","+Numtostr(MaHMAindBIG[RightStrength+14],9)
+","+Numtostr(MaHMAindBIG[RightStrength+15],9)
+","+Numtostr(MaHMAindBIG[RightStrength+16],9)
+","+Numtostr(MaHMAindBIG[RightStrength+17],9)
+","+Numtostr(MaHMAindBIG[RightStrength+18],9)
+","+Numtostr(MaHMAindBIG[RightStrength+19],9)
+","+Numtostr(MaHMAindBIG[RightStrength+2],9)
+","+Numtostr(MaHMAindBIG[RightStrength+20],9)
+","+Numtostr(MaHMAindBIG[RightStrength+21],9)
+","+Numtostr(MaHMAindBIG[RightStrength+22],9)
+","+Numtostr(MaHMAindBIG[RightStrength+23],9)
+","+Numtostr(MaHMAindBIG[RightStrength+24],9)
+","+Numtostr(MaHMAindBIG[RightStrength+25],9)
+","+Numtostr(MaHMAindBIG[RightStrength+26],9)
+","+Numtostr(MaHMAindBIG[RightStrength+27],9)
+","+Numtostr(MaHMAindBIG[RightStrength+28],9)
+","+Numtostr(MaHMAindBIG[RightStrength+3],9)
+","+Numtostr(MaHMAindBIG[RightStrength+4],9)
+","+Numtostr(MaHMAindBIG[RightStrength+5],9)
+","+Numtostr(MaHMAindBIG[RightStrength+6],9)
+","+Numtostr(MaHMAindBIG[RightStrength+7],9)
+","+Numtostr(MaHMAindBIG[RightStrength+8],9)
+","+Numtostr(MaHMAindBIG[RightStrength+9],9)
+","+Numtostr(MaHMAindBIG[RightStrength-1],9)
+","+Numtostr(MaHMAindBIG[RightStrength-2],9)
+","+Numtostr(MaHMAindBIG[RightStrength-3],9)
+","+Numtostr(MaHMAindBIG[RightStrength-4],9)
+","+Numtostr(MaHMAindBX[RightStrength+0],9)
+","+Numtostr(MaHMAindBX[RightStrength+1],9)
+","+Numtostr(MaHMAindBX[RightStrength+10],9)
+","+Numtostr(MaHMAindBX[RightStrength+11],9)
+","+Numtostr(MaHMAindBX[RightStrength+12],9)
+","+Numtostr(MaHMAindBX[RightStrength+13],9)
+","+Numtostr(MaHMAindBX[RightStrength+14],9)
+","+Numtostr(MaHMAindBX[RightStrength+15],9)
+","+Numtostr(MaHMAindBX[RightStrength+16],9)
+","+Numtostr(MaHMAindBX[RightStrength+17],9)
+","+Numtostr(MaHMAindBX[RightStrength+18],9)
+","+Numtostr(MaHMAindBX[RightStrength+19],9)
+","+Numtostr(MaHMAindBX[RightStrength+2],9)
+","+Numtostr(MaHMAindBX[RightStrength+20],9)
+","+Numtostr(MaHMAindBX[RightStrength+21],9)
+","+Numtostr(MaHMAindBX[RightStrength+22],9)
+","+Numtostr(MaHMAindBX[RightStrength+23],9)
+","+Numtostr(MaHMAindBX[RightStrength+24],9)
+","+Numtostr(MaHMAindBX[RightStrength+25],9)
+","+Numtostr(MaHMAindBX[RightStrength+26],9)
+","+Numtostr(MaHMAindBX[RightStrength+27],9)
+","+Numtostr(MaHMAindBX[RightStrength+28],9)
+","+Numtostr(MaHMAindBX[RightStrength+3],9)
+","+Numtostr(MaHMAindBX[RightStrength+4],9)
+","+Numtostr(MaHMAindBX[RightStrength+5],9)
+","+Numtostr(MaHMAindBX[RightStrength+6],9)
+","+Numtostr(MaHMAindBX[RightStrength+7],9)
+","+Numtostr(MaHMAindBX[RightStrength+8],9)
+","+Numtostr(MaHMAindBX[RightStrength+9],9)
+","+Numtostr(MaHMAindBX[RightStrength-1],9)
+","+Numtostr(MaHMAindBX[RightStrength-2],9)
+","+Numtostr(MaHMAindBX[RightStrength-3],9)
+","+Numtostr(MaHMAindBX[RightStrength-4],9)
+","+Numtostr(MaHMAindBXL[RightStrength+0],9)
+","+Numtostr(MaHMAindBXL[RightStrength+1],9)
+","+Numtostr(MaHMAindBXL[RightStrength+10],9)
+","+Numtostr(MaHMAindBXL[RightStrength+11],9)
+","+Numtostr(MaHMAindBXL[RightStrength+12],9)
+","+Numtostr(MaHMAindBXL[RightStrength+13],9)
+","+Numtostr(MaHMAindBXL[RightStrength+14],9)
+","+Numtostr(MaHMAindBXL[RightStrength+15],9)
+","+Numtostr(MaHMAindBXL[RightStrength+16],9)
+","+Numtostr(MaHMAindBXL[RightStrength+17],9)
+","+Numtostr(MaHMAindBXL[RightStrength+18],9)
+","+Numtostr(MaHMAindBXL[RightStrength+19],9)
+","+Numtostr(MaHMAindBXL[RightStrength+2],9)
+","+Numtostr(MaHMAindBXL[RightStrength+20],9)
+","+Numtostr(MaHMAindBXL[RightStrength+21],9)
+","+Numtostr(MaHMAindBXL[RightStrength+22],9)
+","+Numtostr(MaHMAindBXL[RightStrength+23],9)
+","+Numtostr(MaHMAindBXL[RightStrength+24],9)
+","+Numtostr(MaHMAindBXL[RightStrength+25],9)
+","+Numtostr(MaHMAindBXL[RightStrength+26],9)
+","+Numtostr(MaHMAindBXL[RightStrength+27],9)
+","+Numtostr(MaHMAindBXL[RightStrength+28],9)
+","+Numtostr(MaHMAindBXL[RightStrength+3],9)
+","+Numtostr(MaHMAindBXL[RightStrength+4],9)
+","+Numtostr(MaHMAindBXL[RightStrength+5],9)
+","+Numtostr(MaHMAindBXL[RightStrength+6],9)
+","+Numtostr(MaHMAindBXL[RightStrength+7],9)
+","+Numtostr(MaHMAindBXL[RightStrength+8],9)
+","+Numtostr(MaHMAindBXL[RightStrength+9],9)
+","+Numtostr(MaHMAindBXL[RightStrength-1],9)
+","+Numtostr(MaHMAindBXL[RightStrength-2],9)
+","+Numtostr(MaHMAindBXL[RightStrength-3],9)
+","+Numtostr(MaHMAindBXL[RightStrength-4],9)
+","+Numtostr(MaHMAindMED[RightStrength+0],9)
+","+Numtostr(MaHMAindMED[RightStrength+1],9)
+","+Numtostr(MaHMAindMED[RightStrength+10],9)
+","+Numtostr(MaHMAindMED[RightStrength+11],9)
+","+Numtostr(MaHMAindMED[RightStrength+12],9)
+","+Numtostr(MaHMAindMED[RightStrength+13],9)
+","+Numtostr(MaHMAindMED[RightStrength+14],9)
+","+Numtostr(MaHMAindMED[RightStrength+15],9)
+","+Numtostr(MaHMAindMED[RightStrength+16],9)
+","+Numtostr(MaHMAindMED[RightStrength+17],9)
+","+Numtostr(MaHMAindMED[RightStrength+18],9)
+","+Numtostr(MaHMAindMED[RightStrength+19],9)
+","+Numtostr(MaHMAindMED[RightStrength+2],9)
+","+Numtostr(MaHMAindMED[RightStrength+20],9)
+","+Numtostr(MaHMAindMED[RightStrength+21],9)
+","+Numtostr(MaHMAindMED[RightStrength+22],9)
+","+Numtostr(MaHMAindMED[RightStrength+23],9)
+","+Numtostr(MaHMAindMED[RightStrength+24],9)
+","+Numtostr(MaHMAindMED[RightStrength+25],9)
+","+Numtostr(MaHMAindMED[RightStrength+26],9)
+","+Numtostr(MaHMAindMED[RightStrength+27],9)
+","+Numtostr(MaHMAindMED[RightStrength+28],9)
+","+Numtostr(MaHMAindMED[RightStrength+3],9)
+","+Numtostr(MaHMAindMED[RightStrength+4],9)
+","+Numtostr(MaHMAindMED[RightStrength+5],9)
+","+Numtostr(MaHMAindMED[RightStrength+6],9)
+","+Numtostr(MaHMAindMED[RightStrength+7],9)
+","+Numtostr(MaHMAindMED[RightStrength+8],9)
+","+Numtostr(MaHMAindMED[RightStrength+9],9)
+","+Numtostr(MaHMAindMED[RightStrength-1],9)
+","+Numtostr(MaHMAindMED[RightStrength-2],9)
+","+Numtostr(MaHMAindMED[RightStrength-3],9)
+","+Numtostr(MaHMAindMED[RightStrength-4],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+0],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+1],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+10],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+11],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+12],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+13],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+14],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+15],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+16],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+17],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+18],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+19],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+2],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+20],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+21],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+22],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+23],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+24],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+25],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+26],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+27],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+28],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+3],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+4],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+5],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+6],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+7],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+8],9)
+","+Numtostr(MaHMAindSMALL[RightStrength+9],9)
+","+Numtostr(MaHMAindSMALL[RightStrength-1],9)
+","+Numtostr(MaHMAindSMALL[RightStrength-2],9)
+","+Numtostr(MaHMAindSMALL[RightStrength-3],9)
+","+Numtostr(MaHMAindSMALL[RightStrength-4],9)
+","+Numtostr(MaLGREindBIG[RightStrength+0],9)
+","+Numtostr(MaLGREindBIG[RightStrength+1],9)
+","+Numtostr(MaLGREindBIG[RightStrength+10],9)
+","+Numtostr(MaLGREindBIG[RightStrength+11],9)
+","+Numtostr(MaLGREindBIG[RightStrength+12],9)
+","+Numtostr(MaLGREindBIG[RightStrength+13],9)
+","+Numtostr(MaLGREindBIG[RightStrength+14],9)
+","+Numtostr(MaLGREindBIG[RightStrength+15],9)
+","+Numtostr(MaLGREindBIG[RightStrength+16],9)
+","+Numtostr(MaLGREindBIG[RightStrength+17],9)
+","+Numtostr(MaLGREindBIG[RightStrength+18],9)
+","+Numtostr(MaLGREindBIG[RightStrength+19],9)
+","+Numtostr(MaLGREindBIG[RightStrength+2],9)
+","+Numtostr(MaLGREindBIG[RightStrength+20],9)
+","+Numtostr(MaLGREindBIG[RightStrength+21],9)
+","+Numtostr(MaLGREindBIG[RightStrength+22],9)
+","+Numtostr(MaLGREindBIG[RightStrength+23],9)
+","+Numtostr(MaLGREindBIG[RightStrength+24],9)
+","+Numtostr(MaLGREindBIG[RightStrength+25],9)
+","+Numtostr(MaLGREindBIG[RightStrength+26],9)
+","+Numtostr(MaLGREindBIG[RightStrength+27],9)
+","+Numtostr(MaLGREindBIG[RightStrength+28],9)
+","+Numtostr(MaLGREindBIG[RightStrength+3],9)
+","+Numtostr(MaLGREindBIG[RightStrength+4],9)
+","+Numtostr(MaLGREindBIG[RightStrength+5],9)
+","+Numtostr(MaLGREindBIG[RightStrength+6],9)
+","+Numtostr(MaLGREindBIG[RightStrength+7],9)
+","+Numtostr(MaLGREindBIG[RightStrength+8],9)
+","+Numtostr(MaLGREindBIG[RightStrength+9],9)
+","+Numtostr(MaLGREindBIG[RightStrength-1],9)
+","+Numtostr(MaLGREindBIG[RightStrength-2],9)
+","+Numtostr(MaLGREindBIG[RightStrength-3],9)
+","+Numtostr(MaLGREindBIG[RightStrength-4],9)
+","+Numtostr(MaLGREindBX[RightStrength+0],9)
+","+Numtostr(MaLGREindBX[RightStrength+1],9)
+","+Numtostr(MaLGREindBX[RightStrength+10],9)
+","+Numtostr(MaLGREindBX[RightStrength+11],9)
+","+Numtostr(MaLGREindBX[RightStrength+12],9)
+","+Numtostr(MaLGREindBX[RightStrength+13],9)
+","+Numtostr(MaLGREindBX[RightStrength+14],9)
+","+Numtostr(MaLGREindBX[RightStrength+15],9)
+","+Numtostr(MaLGREindBX[RightStrength+16],9)
+","+Numtostr(MaLGREindBX[RightStrength+17],9)
+","+Numtostr(MaLGREindBX[RightStrength+18],9)
+","+Numtostr(MaLGREindBX[RightStrength+19],9)
			+" )"; 
			res5 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_feat5);
			srun5 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures5.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_feat5
					+ NewLine);
			end;
		End;
	End;
	 TblNum = TblNum+1;	


	//(4) insert feature data into dbo.barFeatures6	
	if ReadWriteToDB and TblNum = 6  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		if  srun6 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_feat6 = "INSERT INTO  [dbo].[barFeatures6] VALUES ("
			+queryResult//MAX(loadId) 
			+","+Numtostr(loadGroupId,0) 
			+","+Numtostr(RightStrength,0)//RightStrength
			+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
			+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"'"+dateBarOpened+"' "+","+"'"+dayOfWeekBarOpened+"' "+","+"'"+dateTimeBarOpened+"' "+","+"'"+timeBarOpenedhhmmss+"' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)
+","+Numtostr(MaSMAindBX[RightStrength+24],9)
+","+Numtostr(MaSMAindBX[RightStrength+25],9)
+","+Numtostr(MaSMAindBX[RightStrength+26],9)
+","+Numtostr(MaSMAindBX[RightStrength+27],9)
+","+Numtostr(MaSMAindBX[RightStrength+28],9)
+","+Numtostr(MaSMAindBX[RightStrength+3],9)
+","+Numtostr(MaSMAindBX[RightStrength+4],9)
+","+Numtostr(MaSMAindBX[RightStrength+5],9)
+","+Numtostr(MaSMAindBX[RightStrength+6],9)
+","+Numtostr(MaSMAindBX[RightStrength+7],9)
+","+Numtostr(MaSMAindBX[RightStrength+8],9)
+","+Numtostr(MaSMAindBX[RightStrength+9],9)
+","+Numtostr(MaSMAindBX[RightStrength-1],9)
+","+Numtostr(MaSMAindBX[RightStrength-2],9)
+","+Numtostr(MaSMAindBX[RightStrength-3],9)
+","+Numtostr(MaSMAindBX[RightStrength-4],9)
+","+Numtostr(MaSMAindBXL[RightStrength+0],9)
+","+Numtostr(MaSMAindBXL[RightStrength+1],9)
+","+Numtostr(MaSMAindBXL[RightStrength+10],9)
+","+Numtostr(MaSMAindBXL[RightStrength+11],9)
+","+Numtostr(MaSMAindBXL[RightStrength+12],9)
+","+Numtostr(MaSMAindBXL[RightStrength+13],9)
+","+Numtostr(MaSMAindBXL[RightStrength+14],9)
+","+Numtostr(MaSMAindBXL[RightStrength+15],9)
+","+Numtostr(MaSMAindBXL[RightStrength+16],9)
+","+Numtostr(MaSMAindBXL[RightStrength+17],9)
+","+Numtostr(MaSMAindBXL[RightStrength+18],9)
+","+Numtostr(MaSMAindBXL[RightStrength+19],9)
+","+Numtostr(MaSMAindBXL[RightStrength+2],9)
+","+Numtostr(MaSMAindBXL[RightStrength+20],9)
+","+Numtostr(MaSMAindBXL[RightStrength+21],9)
+","+Numtostr(MaSMAindBXL[RightStrength+22],9)
+","+Numtostr(MaSMAindBXL[RightStrength+23],9)
+","+Numtostr(MaSMAindBXL[RightStrength+24],9)
+","+Numtostr(MaSMAindBXL[RightStrength+25],9)
+","+Numtostr(MaSMAindBXL[RightStrength+26],9)
+","+Numtostr(MaSMAindBXL[RightStrength+27],9)
+","+Numtostr(MaSMAindBXL[RightStrength+28],9)
+","+Numtostr(MaSMAindBXL[RightStrength+3],9)
+","+Numtostr(MaSMAindBXL[RightStrength+4],9)
+","+Numtostr(MaSMAindBXL[RightStrength+5],9)
+","+Numtostr(MaSMAindBXL[RightStrength+6],9)
+","+Numtostr(MaSMAindBXL[RightStrength+7],9)
+","+Numtostr(MaSMAindBXL[RightStrength+8],9)
+","+Numtostr(MaSMAindBXL[RightStrength+9],9)
+","+Numtostr(MaSMAindBXL[RightStrength-1],9)
+","+Numtostr(MaSMAindBXL[RightStrength-2],9)
+","+Numtostr(MaSMAindBXL[RightStrength-3],9)
+","+Numtostr(MaSMAindBXL[RightStrength-4],9)
+","+Numtostr(MaSMAindMED[RightStrength+0],9)
+","+Numtostr(MaSMAindMED[RightStrength+1],9)
+","+Numtostr(MaSMAindMED[RightStrength+10],9)
+","+Numtostr(MaSMAindMED[RightStrength+11],9)
+","+Numtostr(MaSMAindMED[RightStrength+12],9)
+","+Numtostr(MaSMAindMED[RightStrength+13],9)
+","+Numtostr(MaSMAindMED[RightStrength+14],9)
+","+Numtostr(MaSMAindMED[RightStrength+15],9)
+","+Numtostr(MaSMAindMED[RightStrength+16],9)
+","+Numtostr(MaSMAindMED[RightStrength+17],9)
+","+Numtostr(MaSMAindMED[RightStrength+18],9)
+","+Numtostr(MaSMAindMED[RightStrength+19],9)
+","+Numtostr(MaSMAindMED[RightStrength+2],9)
+","+Numtostr(MaSMAindMED[RightStrength+20],9)
+","+Numtostr(MaSMAindMED[RightStrength+21],9)
+","+Numtostr(MaSMAindMED[RightStrength+22],9)
+","+Numtostr(MaSMAindMED[RightStrength+23],9)
+","+Numtostr(MaSMAindMED[RightStrength+24],9)
+","+Numtostr(MaSMAindMED[RightStrength+25],9)
+","+Numtostr(MaSMAindMED[RightStrength+26],9)
+","+Numtostr(MaSMAindMED[RightStrength+27],9)
+","+Numtostr(MaSMAindMED[RightStrength+28],9)
+","+Numtostr(MaSMAindMED[RightStrength+3],9)
+","+Numtostr(MaSMAindMED[RightStrength+4],9)
+","+Numtostr(MaSMAindMED[RightStrength+5],9)
+","+Numtostr(MaSMAindMED[RightStrength+6],9)
+","+Numtostr(MaSMAindMED[RightStrength+7],9)
+","+Numtostr(MaSMAindMED[RightStrength+8],9)
+","+Numtostr(MaSMAindMED[RightStrength+9],9)
+","+Numtostr(MaSMAindMED[RightStrength-1],9)
+","+Numtostr(MaSMAindMED[RightStrength-2],9)
+","+Numtostr(MaSMAindMED[RightStrength-3],9)
+","+Numtostr(MaSMAindMED[RightStrength-4],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+0],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+1],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+10],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+11],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+12],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+13],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+14],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+15],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+16],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+17],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+18],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+19],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+2],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+20],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+21],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+22],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+23],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+24],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+25],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+26],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+27],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+28],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+3],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+4],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+5],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+6],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+7],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+8],9)
+","+Numtostr(MaSMAindSMALL[RightStrength+9],9)
+","+Numtostr(MaSMAindSMALL[RightStrength-1],9)
+","+Numtostr(MaSMAindSMALL[RightStrength-2],9)
+","+Numtostr(MaSMAindSMALL[RightStrength-3],9)
+","+Numtostr(MaSMAindSMALL[RightStrength-4],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+0],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+1],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+10],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+11],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+12],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+13],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+14],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+15],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+16],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+17],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+18],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+19],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+2],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+20],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+21],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+22],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+23],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+24],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+25],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+26],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+27],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+28],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+3],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+4],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+5],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+6],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+7],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+8],9)
+","+Numtostr(MaSWMAindBIG[RightStrength+9],9)
+","+Numtostr(MaSWMAindBIG[RightStrength-1],9)
+","+Numtostr(MaSWMAindBIG[RightStrength-2],9)
+","+Numtostr(MaSWMAindBIG[RightStrength-3],9)
+","+Numtostr(MaSWMAindBIG[RightStrength-4],9)
+","+Numtostr(MaSWMAindBX[RightStrength+0],9)
+","+Numtostr(MaSWMAindBX[RightStrength+1],9)
+","+Numtostr(MaSWMAindBX[RightStrength+10],9)
+","+Numtostr(MaSWMAindBX[RightStrength+11],9)
+","+Numtostr(MaSWMAindBX[RightStrength+12],9)
+","+Numtostr(MaSWMAindBX[RightStrength+13],9)
+","+Numtostr(MaSWMAindBX[RightStrength+14],9)
+","+Numtostr(MaSWMAindBX[RightStrength+15],9)
+","+Numtostr(MaSWMAindBX[RightStrength+16],9)
+","+Numtostr(MaSWMAindBX[RightStrength+17],9)
+","+Numtostr(MaSWMAindBX[RightStrength+18],9)
+","+Numtostr(MaSWMAindBX[RightStrength+19],9)
+","+Numtostr(MaSWMAindBX[RightStrength+2],9)
+","+Numtostr(MaSWMAindBX[RightStrength+20],9)
+","+Numtostr(MaSWMAindBX[RightStrength+21],9)
+","+Numtostr(MaSWMAindBX[RightStrength+22],9)
+","+Numtostr(MaSWMAindBX[RightStrength+23],9)
+","+Numtostr(MaSWMAindBX[RightStrength+24],9)
+","+Numtostr(MaSWMAindBX[RightStrength+25],9)
+","+Numtostr(MaSWMAindBX[RightStrength+26],9)
+","+Numtostr(MaSWMAindBX[RightStrength+27],9)
+","+Numtostr(MaSWMAindBX[RightStrength+28],9)
+","+Numtostr(MaSWMAindBX[RightStrength+3],9)
+","+Numtostr(MaSWMAindBX[RightStrength+4],9)
+","+Numtostr(MaSWMAindBX[RightStrength+5],9)
+","+Numtostr(MaSWMAindBX[RightStrength+6],9)
+","+Numtostr(MaSWMAindBX[RightStrength+7],9)
+","+Numtostr(MaSWMAindBX[RightStrength+8],9)
+","+Numtostr(MaSWMAindBX[RightStrength+9],9)
+","+Numtostr(MaSWMAindBX[RightStrength-1],9)
+","+Numtostr(MaSWMAindBX[RightStrength-2],9)
+","+Numtostr(MaSWMAindBX[RightStrength-3],9)
+","+Numtostr(MaSWMAindBX[RightStrength-4],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+0],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+1],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+10],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+11],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+12],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+13],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+14],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+15],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+16],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+17],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+18],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+19],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+2],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+20],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+21],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+22],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+23],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+24],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+25],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+26],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+27],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+28],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+3],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+4],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+5],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+6],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+7],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+8],9)
+","+Numtostr(MaSWMAindBXL[RightStrength+9],9)
+","+Numtostr(MaSWMAindBXL[RightStrength-1],9)
+","+Numtostr(MaSWMAindBXL[RightStrength-2],9)
+","+Numtostr(MaSWMAindBXL[RightStrength-3],9)
+","+Numtostr(MaSWMAindBXL[RightStrength-4],9)
+","+Numtostr(MaSWMAindMED[RightStrength+0],9)
+","+Numtostr(MaSWMAindMED[RightStrength+1],9)
+","+Numtostr(MaSWMAindMED[RightStrength+10],9)
+","+Numtostr(MaSWMAindMED[RightStrength+11],9)
+","+Numtostr(MaSWMAindMED[RightStrength+12],9)
+","+Numtostr(MaSWMAindMED[RightStrength+13],9)
+","+Numtostr(MaSWMAindMED[RightStrength+14],9)
+","+Numtostr(MaSWMAindMED[RightStrength+15],9)
+","+Numtostr(MaSWMAindMED[RightStrength+16],9)
+","+Numtostr(MaSWMAindMED[RightStrength+17],9)
+","+Numtostr(MaSWMAindMED[RightStrength+18],9)
+","+Numtostr(MaSWMAindMED[RightStrength+19],9)
+","+Numtostr(MaSWMAindMED[RightStrength+2],9)
+","+Numtostr(MaSWMAindMED[RightStrength+20],9)
+","+Numtostr(MaSWMAindMED[RightStrength+21],9)
+","+Numtostr(MaSWMAindMED[RightStrength+22],9)
+","+Numtostr(MaSWMAindMED[RightStrength+23],9)
+","+Numtostr(MaSWMAindMED[RightStrength+24],9)
+","+Numtostr(MaSWMAindMED[RightStrength+25],9)
+","+Numtostr(MaSWMAindMED[RightStrength+26],9)
+","+Numtostr(MaSWMAindMED[RightStrength+27],9)
+","+Numtostr(MaSWMAindMED[RightStrength+28],9)
+","+Numtostr(MaSWMAindMED[RightStrength+3],9)
+","+Numtostr(MaSWMAindMED[RightStrength+4],9)
+","+Numtostr(MaSWMAindMED[RightStrength+5],9)
+","+Numtostr(MaSWMAindMED[RightStrength+6],9)
+","+Numtostr(MaSWMAindMED[RightStrength+7],9)
+","+Numtostr(MaSWMAindMED[RightStrength+8],9)
+","+Numtostr(MaSWMAindMED[RightStrength+9],9)
+","+Numtostr(MaSWMAindMED[RightStrength-1],9)
+","+Numtostr(MaSWMAindMED[RightStrength-2],9)
+","+Numtostr(MaSWMAindMED[RightStrength-3],9)
+","+Numtostr(MaSWMAindMED[RightStrength-4],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+0],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+1],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+10],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+11],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+12],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+13],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+14],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+15],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+16],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+17],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+18],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+19],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+2],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+20],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+21],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+22],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+23],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+24],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+25],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+26],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+27],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+28],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+3],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+4],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+5],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+6],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+7],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+8],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength+9],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength-1],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength-2],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength-3],9)
+","+Numtostr(MaSWMAindSMALL[RightStrength-4],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+0],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+1],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+10],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+11],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+12],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+13],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+14],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+15],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+16],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+17],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+18],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+19],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+2],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+20],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+21],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+22],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+23],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+24],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+25],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+26],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+27],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+28],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+3],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+4],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+5],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+6],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+7],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+8],9)
+","+Numtostr(MaTEMAindBIG[RightStrength+9],9)
+","+Numtostr(MaTEMAindBIG[RightStrength-1],9)
+","+Numtostr(MaTEMAindBIG[RightStrength-2],9)
+","+Numtostr(MaTEMAindBIG[RightStrength-3],9)
+","+Numtostr(MaTEMAindBIG[RightStrength-4],9)
+","+Numtostr(MaTEMAindBX[RightStrength+0],9)
+","+Numtostr(MaTEMAindBX[RightStrength+1],9)
+","+Numtostr(MaTEMAindBX[RightStrength+10],9)
+","+Numtostr(MaTEMAindBX[RightStrength+11],9)
+","+Numtostr(MaTEMAindBX[RightStrength+12],9)
+","+Numtostr(MaTEMAindBX[RightStrength+13],9)
+","+Numtostr(MaTEMAindBX[RightStrength+14],9)
+","+Numtostr(MaTEMAindBX[RightStrength+15],9)
+","+Numtostr(MaTEMAindBX[RightStrength+16],9)
+","+Numtostr(MaTEMAindBX[RightStrength+17],9)
+","+Numtostr(MaTEMAindBX[RightStrength+18],9)
+","+Numtostr(MaTEMAindBX[RightStrength+19],9)
+","+Numtostr(MaTEMAindBX[RightStrength+2],9)
+","+Numtostr(MaTEMAindBX[RightStrength+20],9)
+","+Numtostr(MaTEMAindBX[RightStrength+21],9)
+","+Numtostr(MaTEMAindBX[RightStrength+22],9)
+","+Numtostr(MaTEMAindBX[RightStrength+23],9)
+","+Numtostr(MaTEMAindBX[RightStrength+24],9)
+","+Numtostr(MaTEMAindBX[RightStrength+25],9)
+","+Numtostr(MaTEMAindBX[RightStrength+26],9)
+","+Numtostr(MaTEMAindBX[RightStrength+27],9)
+","+Numtostr(MaTEMAindBX[RightStrength+28],9)
+","+Numtostr(MaTEMAindBX[RightStrength+3],9)
+","+Numtostr(MaTEMAindBX[RightStrength+4],9)
+","+Numtostr(MaTEMAindBX[RightStrength+5],9)
+","+Numtostr(MaTEMAindBX[RightStrength+6],9)
+","+Numtostr(MaTEMAindBX[RightStrength+7],9)
+","+Numtostr(MaTEMAindBX[RightStrength+8],9)
+","+Numtostr(MaTEMAindBX[RightStrength+9],9)
+","+Numtostr(MaTEMAindBX[RightStrength-1],9)
+","+Numtostr(MaTEMAindBX[RightStrength-2],9)
+","+Numtostr(MaTEMAindBX[RightStrength-3],9)
+","+Numtostr(MaTEMAindBX[RightStrength-4],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+0],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+1],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+10],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+11],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+12],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+13],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+14],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+15],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+16],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+17],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+18],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+19],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+2],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+20],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+21],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+22],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+23],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+24],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+25],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+26],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+27],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+28],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+3],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+4],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+5],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+6],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+7],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+8],9)
+","+Numtostr(MaTEMAindBXL[RightStrength+9],9)
+","+Numtostr(MaTEMAindBXL[RightStrength-1],9)
+","+Numtostr(MaTEMAindBXL[RightStrength-2],9)
+","+Numtostr(MaTEMAindBXL[RightStrength-3],9)
+","+Numtostr(MaTEMAindBXL[RightStrength-4],9)
+","+Numtostr(MaTEMAindMED[RightStrength+0],9)
+","+Numtostr(MaTEMAindMED[RightStrength+1],9)
+","+Numtostr(MaTEMAindMED[RightStrength+10],9)
+","+Numtostr(MaTEMAindMED[RightStrength+11],9)
+","+Numtostr(MaTEMAindMED[RightStrength+12],9)
+","+Numtostr(MaTEMAindMED[RightStrength+13],9)
+","+Numtostr(MaTEMAindMED[RightStrength+14],9)
+","+Numtostr(MaTEMAindMED[RightStrength+15],9)
+","+Numtostr(MaTEMAindMED[RightStrength+16],9)
+","+Numtostr(MaTEMAindMED[RightStrength+17],9)
+","+Numtostr(MaTEMAindMED[RightStrength+18],9)
+","+Numtostr(MaTEMAindMED[RightStrength+19],9)
+","+Numtostr(MaTEMAindMED[RightStrength+2],9)
+","+Numtostr(MaTEMAindMED[RightStrength+20],9)
+","+Numtostr(MaTEMAindMED[RightStrength+21],9)
+","+Numtostr(MaTEMAindMED[RightStrength+22],9)
+","+Numtostr(MaTEMAindMED[RightStrength+23],9)
+","+Numtostr(MaTEMAindMED[RightStrength+24],9)
+","+Numtostr(MaTEMAindMED[RightStrength+25],9)
+","+Numtostr(MaTEMAindMED[RightStrength+26],9)
+","+Numtostr(MaTEMAindMED[RightStrength+27],9)
+","+Numtostr(MaTEMAindMED[RightStrength+28],9)
+","+Numtostr(MaTEMAindMED[RightStrength+3],9)
+","+Numtostr(MaTEMAindMED[RightStrength+4],9)
+","+Numtostr(MaTEMAindMED[RightStrength+5],9)
+","+Numtostr(MaTEMAindMED[RightStrength+6],9)
+","+Numtostr(MaTEMAindMED[RightStrength+7],9)
+","+Numtostr(MaTEMAindMED[RightStrength+8],9)
+","+Numtostr(MaTEMAindMED[RightStrength+9],9)
+","+Numtostr(MaTEMAindMED[RightStrength-1],9)
+","+Numtostr(MaTEMAindMED[RightStrength-2],9)
+","+Numtostr(MaTEMAindMED[RightStrength-3],9)
+","+Numtostr(MaTEMAindMED[RightStrength-4],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+0],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+1],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+10],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+11],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+12],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+13],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+14],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+15],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+16],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+17],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+18],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+19],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+2],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+20],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+21],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+22],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+23],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+24],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+25],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+26],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+27],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+28],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+3],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+4],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+5],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+6],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+7],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+8],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength+9],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength-1],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength-2],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength-3],9)
+","+Numtostr(MaTEMAindSMALL[RightStrength-4],9)
+","+Numtostr(MaTRIindBIG[RightStrength+0],9)
+","+Numtostr(MaTRIindBIG[RightStrength+1],9)
+","+Numtostr(MaTRIindBIG[RightStrength+10],9)
+","+Numtostr(MaTRIindBIG[RightStrength+11],9)
+","+Numtostr(MaTRIindBIG[RightStrength+12],9)
+","+Numtostr(MaTRIindBIG[RightStrength+13],9)
+","+Numtostr(MaTRIindBIG[RightStrength+14],9)
+","+Numtostr(MaTRIindBIG[RightStrength+15],9)
+","+Numtostr(MaTRIindBIG[RightStrength+16],9)
+","+Numtostr(MaTRIindBIG[RightStrength+17],9)
+","+Numtostr(MaTRIindBIG[RightStrength+18],9)
+","+Numtostr(MaTRIindBIG[RightStrength+19],9)
+","+Numtostr(MaTRIindBIG[RightStrength+2],9)
+","+Numtostr(MaTRIindBIG[RightStrength+20],9)
+","+Numtostr(MaTRIindBIG[RightStrength+21],9)
+","+Numtostr(MaTRIindBIG[RightStrength+22],9)
+","+Numtostr(MaTRIindBIG[RightStrength+23],9)
+","+Numtostr(MaTRIindBIG[RightStrength+24],9)
+","+Numtostr(MaTRIindBIG[RightStrength+25],9)
+","+Numtostr(MaTRIindBIG[RightStrength+26],9)
+","+Numtostr(MaTRIindBIG[RightStrength+27],9)
+","+Numtostr(MaTRIindBIG[RightStrength+28],9)
+","+Numtostr(MaTRIindBIG[RightStrength+3],9)
+","+Numtostr(MaTRIindBIG[RightStrength+4],9)
+","+Numtostr(MaTRIindBIG[RightStrength+5],9)
+","+Numtostr(MaTRIindBIG[RightStrength+6],9)
+","+Numtostr(MaTRIindBIG[RightStrength+7],9)
+","+Numtostr(MaTRIindBIG[RightStrength+8],9)
+","+Numtostr(MaTRIindBIG[RightStrength+9],9)
+","+Numtostr(MaTRIindBIG[RightStrength-1],9)
+","+Numtostr(MaTRIindBIG[RightStrength-2],9)
+","+Numtostr(MaTRIindBIG[RightStrength-3],9)
+","+Numtostr(MaTRIindBIG[RightStrength-4],9)
+","+Numtostr(MaTRIindBX[RightStrength+0],9)
+","+Numtostr(MaTRIindBX[RightStrength+1],9)
+","+Numtostr(MaTRIindBX[RightStrength+10],9)
+","+Numtostr(MaTRIindBX[RightStrength+11],9)
+","+Numtostr(MaTRIindBX[RightStrength+12],9)
+","+Numtostr(MaTRIindBX[RightStrength+13],9)
+","+Numtostr(MaTRIindBX[RightStrength+14],9)
+","+Numtostr(MaTRIindBX[RightStrength+15],9)
+","+Numtostr(MaTRIindBX[RightStrength+16],9)
+","+Numtostr(MaTRIindBX[RightStrength+17],9)
+","+Numtostr(MaTRIindBX[RightStrength+18],9)
+","+Numtostr(MaTRIindBX[RightStrength+19],9)
+","+Numtostr(MaTRIindBX[RightStrength+2],9)
+","+Numtostr(MaTRIindBX[RightStrength+20],9)
+","+Numtostr(MaTRIindBX[RightStrength+21],9)
+","+Numtostr(MaTRIindBX[RightStrength+22],9)
+","+Numtostr(MaTRIindBX[RightStrength+23],9)
+","+Numtostr(MaTRIindBX[RightStrength+24],9)
+","+Numtostr(MaTRIindBX[RightStrength+25],9)
+","+Numtostr(MaTRIindBX[RightStrength+26],9)
+","+Numtostr(MaTRIindBX[RightStrength+27],9)
+","+Numtostr(MaTRIindBX[RightStrength+28],9)
			+" )"; 
			res6 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_feat6);
			srun6 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures6.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_feat6
					+ NewLine);
			end;
		End;
	End;
	 TblNum = TblNum+1;

	//(4) insert feature data into dbo.barFeatures7	
	if ReadWriteToDB and TblNum = 7  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		if  srun7 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_feat7 = "INSERT INTO  [dbo].[barFeatures7] VALUES ("
			+queryResult//MAX(loadId) 
			+","+Numtostr(loadGroupId,0) 
			+","+Numtostr(RightStrength,0)//RightStrength
			+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
			+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"'"+dateBarOpened+"' "+","+"'"+dayOfWeekBarOpened+"' "+","+"'"+dateTimeBarOpened+"' "+","+"'"+timeBarOpenedhhmmss+"' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)
+","+Numtostr(MaZLAGindBX[RightStrength+8],9)
+","+Numtostr(MaZLAGindBX[RightStrength+9],9)
+","+Numtostr(MaZLAGindBX[RightStrength-1],9)
+","+Numtostr(MaZLAGindBX[RightStrength-2],9)
+","+Numtostr(MaZLAGindBX[RightStrength-3],9)
+","+Numtostr(MaZLAGindBX[RightStrength-4],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+0],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+1],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+10],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+11],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+12],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+13],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+14],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+15],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+16],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+17],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+18],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+19],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+2],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+20],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+21],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+22],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+23],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+24],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+25],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+26],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+27],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+28],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+3],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+4],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+5],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+6],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+7],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+8],9)
+","+Numtostr(MaZLAGindBXL[RightStrength+9],9)
+","+Numtostr(MaZLAGindBXL[RightStrength-1],9)
+","+Numtostr(MaZLAGindBXL[RightStrength-2],9)
+","+Numtostr(MaZLAGindBXL[RightStrength-3],9)
+","+Numtostr(MaZLAGindBXL[RightStrength-4],9)
+","+Numtostr(MaZLAGindMED[RightStrength+0],9)
+","+Numtostr(MaZLAGindMED[RightStrength+1],9)
+","+Numtostr(MaZLAGindMED[RightStrength+10],9)
+","+Numtostr(MaZLAGindMED[RightStrength+11],9)
+","+Numtostr(MaZLAGindMED[RightStrength+12],9)
+","+Numtostr(MaZLAGindMED[RightStrength+13],9)
+","+Numtostr(MaZLAGindMED[RightStrength+14],9)
+","+Numtostr(MaZLAGindMED[RightStrength+15],9)
+","+Numtostr(MaZLAGindMED[RightStrength+16],9)
+","+Numtostr(MaZLAGindMED[RightStrength+17],9)
+","+Numtostr(MaZLAGindMED[RightStrength+18],9)
+","+Numtostr(MaZLAGindMED[RightStrength+19],9)
+","+Numtostr(MaZLAGindMED[RightStrength+2],9)
+","+Numtostr(MaZLAGindMED[RightStrength+20],9)
+","+Numtostr(MaZLAGindMED[RightStrength+21],9)
+","+Numtostr(MaZLAGindMED[RightStrength+22],9)
+","+Numtostr(MaZLAGindMED[RightStrength+23],9)
+","+Numtostr(MaZLAGindMED[RightStrength+24],9)
+","+Numtostr(MaZLAGindMED[RightStrength+25],9)
+","+Numtostr(MaZLAGindMED[RightStrength+26],9)
+","+Numtostr(MaZLAGindMED[RightStrength+27],9)
+","+Numtostr(MaZLAGindMED[RightStrength+28],9)
+","+Numtostr(MaZLAGindMED[RightStrength+3],9)
+","+Numtostr(MaZLAGindMED[RightStrength+4],9)
+","+Numtostr(MaZLAGindMED[RightStrength+5],9)
+","+Numtostr(MaZLAGindMED[RightStrength+6],9)
+","+Numtostr(MaZLAGindMED[RightStrength+7],9)
+","+Numtostr(MaZLAGindMED[RightStrength+8],9)
+","+Numtostr(MaZLAGindMED[RightStrength+9],9)
+","+Numtostr(MaZLAGindMED[RightStrength-1],9)
+","+Numtostr(MaZLAGindMED[RightStrength-2],9)
+","+Numtostr(MaZLAGindMED[RightStrength-3],9)
+","+Numtostr(MaZLAGindMED[RightStrength-4],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+0],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+1],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+10],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+11],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+12],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+13],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+14],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+15],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+16],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+17],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+18],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+19],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+2],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+20],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+21],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+22],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+23],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+24],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+25],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+26],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+27],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+28],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+3],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+4],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+5],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+6],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+7],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+8],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength+9],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength-1],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength-2],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength-3],9)
+","+Numtostr(MaZLAGindSMALL[RightStrength-4],9)
+","+Numtostr(PCHANindBIG[RightStrength+0],9)
+","+Numtostr(PCHANindBIG[RightStrength+1],9)
+","+Numtostr(PCHANindBIG[RightStrength+10],9)
+","+Numtostr(PCHANindBIG[RightStrength+11],9)
+","+Numtostr(PCHANindBIG[RightStrength+12],9)
+","+Numtostr(PCHANindBIG[RightStrength+13],9)
+","+Numtostr(PCHANindBIG[RightStrength+14],9)
+","+Numtostr(PCHANindBIG[RightStrength+15],9)
+","+Numtostr(PCHANindBIG[RightStrength+16],9)
+","+Numtostr(PCHANindBIG[RightStrength+17],9)
+","+Numtostr(PCHANindBIG[RightStrength+18],9)
+","+Numtostr(PCHANindBIG[RightStrength+19],9)
+","+Numtostr(PCHANindBIG[RightStrength+2],9)
+","+Numtostr(PCHANindBIG[RightStrength+20],9)
+","+Numtostr(PCHANindBIG[RightStrength+21],9)
+","+Numtostr(PCHANindBIG[RightStrength+22],9)
+","+Numtostr(PCHANindBIG[RightStrength+23],9)
+","+Numtostr(PCHANindBIG[RightStrength+24],9)
+","+Numtostr(PCHANindBIG[RightStrength+25],9)
+","+Numtostr(PCHANindBIG[RightStrength+26],9)
+","+Numtostr(PCHANindBIG[RightStrength+27],9)
+","+Numtostr(PCHANindBIG[RightStrength+28],9)
+","+Numtostr(PCHANindBIG[RightStrength+3],9)
+","+Numtostr(PCHANindBIG[RightStrength+4],9)
+","+Numtostr(PCHANindBIG[RightStrength+5],9)
+","+Numtostr(PCHANindBIG[RightStrength+6],9)
+","+Numtostr(PCHANindBIG[RightStrength+7],9)
+","+Numtostr(PCHANindBIG[RightStrength+8],9)
+","+Numtostr(PCHANindBIG[RightStrength+9],9)
+","+Numtostr(PCHANindBIG[RightStrength-1],9)
+","+Numtostr(PCHANindBIG[RightStrength-2],9)
+","+Numtostr(PCHANindBIG[RightStrength-3],9)
+","+Numtostr(PCHANindBIG[RightStrength-4],9)
+","+Numtostr(PCHANindBX[RightStrength+0],9)
+","+Numtostr(PCHANindBX[RightStrength+1],9)
+","+Numtostr(PCHANindBX[RightStrength+10],9)
+","+Numtostr(PCHANindBX[RightStrength+11],9)
+","+Numtostr(PCHANindBX[RightStrength+12],9)
+","+Numtostr(PCHANindBX[RightStrength+13],9)
+","+Numtostr(PCHANindBX[RightStrength+14],9)
+","+Numtostr(PCHANindBX[RightStrength+15],9)
+","+Numtostr(PCHANindBX[RightStrength+16],9)
+","+Numtostr(PCHANindBX[RightStrength+17],9)
+","+Numtostr(PCHANindBX[RightStrength+18],9)
+","+Numtostr(PCHANindBX[RightStrength+19],9)
+","+Numtostr(PCHANindBX[RightStrength+2],9)
+","+Numtostr(PCHANindBX[RightStrength+20],9)
+","+Numtostr(PCHANindBX[RightStrength+21],9)
+","+Numtostr(PCHANindBX[RightStrength+22],9)
+","+Numtostr(PCHANindBX[RightStrength+23],9)
+","+Numtostr(PCHANindBX[RightStrength+24],9)
+","+Numtostr(PCHANindBX[RightStrength+25],9)
+","+Numtostr(PCHANindBX[RightStrength+26],9)
+","+Numtostr(PCHANindBX[RightStrength+27],9)
+","+Numtostr(PCHANindBX[RightStrength+28],9)
+","+Numtostr(PCHANindBX[RightStrength+3],9)
+","+Numtostr(PCHANindBX[RightStrength+4],9)
+","+Numtostr(PCHANindBX[RightStrength+5],9)
+","+Numtostr(PCHANindBX[RightStrength+6],9)
+","+Numtostr(PCHANindBX[RightStrength+7],9)
+","+Numtostr(PCHANindBX[RightStrength+8],9)
+","+Numtostr(PCHANindBX[RightStrength+9],9)
+","+Numtostr(PCHANindBX[RightStrength-1],9)
+","+Numtostr(PCHANindBX[RightStrength-2],9)
+","+Numtostr(PCHANindBX[RightStrength-3],9)
+","+Numtostr(PCHANindBX[RightStrength-4],9)
+","+Numtostr(PCHANindBXL[RightStrength+0],9)
+","+Numtostr(PCHANindBXL[RightStrength+1],9)
+","+Numtostr(PCHANindBXL[RightStrength+10],9)
+","+Numtostr(PCHANindBXL[RightStrength+11],9)
+","+Numtostr(PCHANindBXL[RightStrength+12],9)
+","+Numtostr(PCHANindBXL[RightStrength+13],9)
+","+Numtostr(PCHANindBXL[RightStrength+14],9)
+","+Numtostr(PCHANindBXL[RightStrength+15],9)
+","+Numtostr(PCHANindBXL[RightStrength+16],9)
+","+Numtostr(PCHANindBXL[RightStrength+17],9)
+","+Numtostr(PCHANindBXL[RightStrength+18],9)
+","+Numtostr(PCHANindBXL[RightStrength+19],9)
+","+Numtostr(PCHANindBXL[RightStrength+2],9)
+","+Numtostr(PCHANindBXL[RightStrength+20],9)
+","+Numtostr(PCHANindBXL[RightStrength+21],9)
+","+Numtostr(PCHANindBXL[RightStrength+22],9)
+","+Numtostr(PCHANindBXL[RightStrength+23],9)
+","+Numtostr(PCHANindBXL[RightStrength+24],9)
+","+Numtostr(PCHANindBXL[RightStrength+25],9)
+","+Numtostr(PCHANindBXL[RightStrength+26],9)
+","+Numtostr(PCHANindBXL[RightStrength+27],9)
+","+Numtostr(PCHANindBXL[RightStrength+28],9)
+","+Numtostr(PCHANindBXL[RightStrength+3],9)
+","+Numtostr(PCHANindBXL[RightStrength+4],9)
+","+Numtostr(PCHANindBXL[RightStrength+5],9)
+","+Numtostr(PCHANindBXL[RightStrength+6],9)
+","+Numtostr(PCHANindBXL[RightStrength+7],9)
+","+Numtostr(PCHANindBXL[RightStrength+8],9)
+","+Numtostr(PCHANindBXL[RightStrength+9],9)
+","+Numtostr(PCHANindBXL[RightStrength-1],9)
+","+Numtostr(PCHANindBXL[RightStrength-2],9)
+","+Numtostr(PCHANindBXL[RightStrength-3],9)
+","+Numtostr(PCHANindBXL[RightStrength-4],9)
+","+Numtostr(PCHANindMED[RightStrength+0],9)
+","+Numtostr(PCHANindMED[RightStrength+1],9)
+","+Numtostr(PCHANindMED[RightStrength+10],9)
+","+Numtostr(PCHANindMED[RightStrength+11],9)
+","+Numtostr(PCHANindMED[RightStrength+12],9)
+","+Numtostr(PCHANindMED[RightStrength+13],9)
+","+Numtostr(PCHANindMED[RightStrength+14],9)
+","+Numtostr(PCHANindMED[RightStrength+15],9)
+","+Numtostr(PCHANindMED[RightStrength+16],9)
+","+Numtostr(PCHANindMED[RightStrength+17],9)
+","+Numtostr(PCHANindMED[RightStrength+18],9)
+","+Numtostr(PCHANindMED[RightStrength+19],9)
+","+Numtostr(PCHANindMED[RightStrength+2],9)
+","+Numtostr(PCHANindMED[RightStrength+20],9)
+","+Numtostr(PCHANindMED[RightStrength+21],9)
+","+Numtostr(PCHANindMED[RightStrength+22],9)
+","+Numtostr(PCHANindMED[RightStrength+23],9)
+","+Numtostr(PCHANindMED[RightStrength+24],9)
+","+Numtostr(PCHANindMED[RightStrength+25],9)
+","+Numtostr(PCHANindMED[RightStrength+26],9)
+","+Numtostr(PCHANindMED[RightStrength+27],9)
+","+Numtostr(PCHANindMED[RightStrength+28],9)
+","+Numtostr(PCHANindMED[RightStrength+3],9)
+","+Numtostr(PCHANindMED[RightStrength+4],9)
+","+Numtostr(PCHANindMED[RightStrength+5],9)
+","+Numtostr(PCHANindMED[RightStrength+6],9)
+","+Numtostr(PCHANindMED[RightStrength+7],9)
+","+Numtostr(PCHANindMED[RightStrength+8],9)
+","+Numtostr(PCHANindMED[RightStrength+9],9)
+","+Numtostr(PCHANindMED[RightStrength-1],9)
+","+Numtostr(PCHANindMED[RightStrength-2],9)
+","+Numtostr(PCHANindMED[RightStrength-3],9)
+","+Numtostr(PCHANindMED[RightStrength-4],9)
+","+Numtostr(PCHANindSMALL[RightStrength+0],9)
+","+Numtostr(PCHANindSMALL[RightStrength+1],9)
+","+Numtostr(PCHANindSMALL[RightStrength+10],9)
+","+Numtostr(PCHANindSMALL[RightStrength+11],9)
+","+Numtostr(PCHANindSMALL[RightStrength+12],9)
+","+Numtostr(PCHANindSMALL[RightStrength+13],9)
+","+Numtostr(PCHANindSMALL[RightStrength+14],9)
+","+Numtostr(PCHANindSMALL[RightStrength+15],9)
+","+Numtostr(PCHANindSMALL[RightStrength+16],9)
+","+Numtostr(PCHANindSMALL[RightStrength+17],9)
+","+Numtostr(PCHANindSMALL[RightStrength+18],9)
+","+Numtostr(PCHANindSMALL[RightStrength+19],9)
+","+Numtostr(PCHANindSMALL[RightStrength+2],9)
+","+Numtostr(PCHANindSMALL[RightStrength+20],9)
+","+Numtostr(PCHANindSMALL[RightStrength+21],9)
+","+Numtostr(PCHANindSMALL[RightStrength+22],9)
+","+Numtostr(PCHANindSMALL[RightStrength+23],9)
+","+Numtostr(PCHANindSMALL[RightStrength+24],9)
+","+Numtostr(PCHANindSMALL[RightStrength+25],9)
+","+Numtostr(PCHANindSMALL[RightStrength+26],9)
+","+Numtostr(PCHANindSMALL[RightStrength+27],9)
+","+Numtostr(PCHANindSMALL[RightStrength+28],9)
+","+Numtostr(PCHANindSMALL[RightStrength+3],9)
+","+Numtostr(PCHANindSMALL[RightStrength+4],9)
+","+Numtostr(PCHANindSMALL[RightStrength+5],9)
+","+Numtostr(PCHANindSMALL[RightStrength+6],9)
+","+Numtostr(PCHANindSMALL[RightStrength+7],9)
+","+Numtostr(PCHANindSMALL[RightStrength+8],9)
+","+Numtostr(PCHANindSMALL[RightStrength+9],9)
+","+Numtostr(PCHANindSMALL[RightStrength-1],9)
+","+Numtostr(PCHANindSMALL[RightStrength-2],9)
+","+Numtostr(PCHANindSMALL[RightStrength-3],9)
+","+Numtostr(PCHANindSMALL[RightStrength-4],9)
+","+Numtostr(PCHGindBIG[RightStrength+0],9)
+","+Numtostr(PCHGindBIG[RightStrength+1],9)
+","+Numtostr(PCHGindBIG[RightStrength+10],9)
+","+Numtostr(PCHGindBIG[RightStrength+11],9)
+","+Numtostr(PCHGindBIG[RightStrength+12],9)
+","+Numtostr(PCHGindBIG[RightStrength+13],9)
+","+Numtostr(PCHGindBIG[RightStrength+14],9)
+","+Numtostr(PCHGindBIG[RightStrength+15],9)
+","+Numtostr(PCHGindBIG[RightStrength+16],9)
+","+Numtostr(PCHGindBIG[RightStrength+17],9)
+","+Numtostr(PCHGindBIG[RightStrength+18],9)
+","+Numtostr(PCHGindBIG[RightStrength+19],9)
+","+Numtostr(PCHGindBIG[RightStrength+2],9)
+","+Numtostr(PCHGindBIG[RightStrength+20],9)
+","+Numtostr(PCHGindBIG[RightStrength+21],9)
+","+Numtostr(PCHGindBIG[RightStrength+22],9)
+","+Numtostr(PCHGindBIG[RightStrength+23],9)
+","+Numtostr(PCHGindBIG[RightStrength+24],9)
+","+Numtostr(PCHGindBIG[RightStrength+25],9)
+","+Numtostr(PCHGindBIG[RightStrength+26],9)
+","+Numtostr(PCHGindBIG[RightStrength+27],9)
+","+Numtostr(PCHGindBIG[RightStrength+28],9)
+","+Numtostr(PCHGindBIG[RightStrength+3],9)
+","+Numtostr(PCHGindBIG[RightStrength+4],9)
+","+Numtostr(PCHGindBIG[RightStrength+5],9)
+","+Numtostr(PCHGindBIG[RightStrength+6],9)
+","+Numtostr(PCHGindBIG[RightStrength+7],9)
+","+Numtostr(PCHGindBIG[RightStrength+8],9)
+","+Numtostr(PCHGindBIG[RightStrength+9],9)
+","+Numtostr(PCHGindBIG[RightStrength-1],9)
+","+Numtostr(PCHGindBIG[RightStrength-2],9)
+","+Numtostr(PCHGindBIG[RightStrength-3],9)
+","+Numtostr(PCHGindBIG[RightStrength-4],9)
+","+Numtostr(PCHGindBX[RightStrength+0],9)
+","+Numtostr(PCHGindBX[RightStrength+1],9)
+","+Numtostr(PCHGindBX[RightStrength+10],9)
+","+Numtostr(PCHGindBX[RightStrength+11],9)
+","+Numtostr(PCHGindBX[RightStrength+12],9)
+","+Numtostr(PCHGindBX[RightStrength+13],9)
+","+Numtostr(PCHGindBX[RightStrength+14],9)
+","+Numtostr(PCHGindBX[RightStrength+15],9)
+","+Numtostr(PCHGindBX[RightStrength+16],9)
+","+Numtostr(PCHGindBX[RightStrength+17],9)
+","+Numtostr(PCHGindBX[RightStrength+18],9)
+","+Numtostr(PCHGindBX[RightStrength+19],9)
+","+Numtostr(PCHGindBX[RightStrength+2],9)
+","+Numtostr(PCHGindBX[RightStrength+20],9)
+","+Numtostr(PCHGindBX[RightStrength+21],9)
+","+Numtostr(PCHGindBX[RightStrength+22],9)
+","+Numtostr(PCHGindBX[RightStrength+23],9)
+","+Numtostr(PCHGindBX[RightStrength+24],9)
+","+Numtostr(PCHGindBX[RightStrength+25],9)
+","+Numtostr(PCHGindBX[RightStrength+26],9)
+","+Numtostr(PCHGindBX[RightStrength+27],9)
+","+Numtostr(PCHGindBX[RightStrength+28],9)
+","+Numtostr(PCHGindBX[RightStrength+3],9)
+","+Numtostr(PCHGindBX[RightStrength+4],9)
+","+Numtostr(PCHGindBX[RightStrength+5],9)
+","+Numtostr(PCHGindBX[RightStrength+6],9)
+","+Numtostr(PCHGindBX[RightStrength+7],9)
+","+Numtostr(PCHGindBX[RightStrength+8],9)
+","+Numtostr(PCHGindBX[RightStrength+9],9)
+","+Numtostr(PCHGindBX[RightStrength-1],9)
+","+Numtostr(PCHGindBX[RightStrength-2],9)
+","+Numtostr(PCHGindBX[RightStrength-3],9)
+","+Numtostr(PCHGindBX[RightStrength-4],9)
+","+Numtostr(PCHGindBXL[RightStrength+0],9)
+","+Numtostr(PCHGindBXL[RightStrength+1],9)
+","+Numtostr(PCHGindBXL[RightStrength+10],9)
+","+Numtostr(PCHGindBXL[RightStrength+11],9)
+","+Numtostr(PCHGindBXL[RightStrength+12],9)
+","+Numtostr(PCHGindBXL[RightStrength+13],9)
+","+Numtostr(PCHGindBXL[RightStrength+14],9)
+","+Numtostr(PCHGindBXL[RightStrength+15],9)
+","+Numtostr(PCHGindBXL[RightStrength+16],9)
+","+Numtostr(PCHGindBXL[RightStrength+17],9)
+","+Numtostr(PCHGindBXL[RightStrength+18],9)
+","+Numtostr(PCHGindBXL[RightStrength+19],9)
+","+Numtostr(PCHGindBXL[RightStrength+2],9)
+","+Numtostr(PCHGindBXL[RightStrength+20],9)
+","+Numtostr(PCHGindBXL[RightStrength+21],9)
+","+Numtostr(PCHGindBXL[RightStrength+22],9)
+","+Numtostr(PCHGindBXL[RightStrength+23],9)
+","+Numtostr(PCHGindBXL[RightStrength+24],9)
+","+Numtostr(PCHGindBXL[RightStrength+25],9)
+","+Numtostr(PCHGindBXL[RightStrength+26],9)
+","+Numtostr(PCHGindBXL[RightStrength+27],9)
+","+Numtostr(PCHGindBXL[RightStrength+28],9)
+","+Numtostr(PCHGindBXL[RightStrength+3],9)
+","+Numtostr(PCHGindBXL[RightStrength+4],9)
+","+Numtostr(PCHGindBXL[RightStrength+5],9)
+","+Numtostr(PCHGindBXL[RightStrength+6],9)
+","+Numtostr(PCHGindBXL[RightStrength+7],9)
+","+Numtostr(PCHGindBXL[RightStrength+8],9)
+","+Numtostr(PCHGindBXL[RightStrength+9],9)
+","+Numtostr(PCHGindBXL[RightStrength-1],9)
+","+Numtostr(PCHGindBXL[RightStrength-2],9)
+","+Numtostr(PCHGindBXL[RightStrength-3],9)
+","+Numtostr(PCHGindBXL[RightStrength-4],9)
+","+Numtostr(PCHGindMED[RightStrength+0],9)
+","+Numtostr(PCHGindMED[RightStrength+1],9)
+","+Numtostr(PCHGindMED[RightStrength+10],9)
+","+Numtostr(PCHGindMED[RightStrength+11],9)
+","+Numtostr(PCHGindMED[RightStrength+12],9)
+","+Numtostr(PCHGindMED[RightStrength+13],9)
+","+Numtostr(PCHGindMED[RightStrength+14],9)
+","+Numtostr(PCHGindMED[RightStrength+15],9)
+","+Numtostr(PCHGindMED[RightStrength+16],9)
+","+Numtostr(PCHGindMED[RightStrength+17],9)
+","+Numtostr(PCHGindMED[RightStrength+18],9)
+","+Numtostr(PCHGindMED[RightStrength+19],9)
+","+Numtostr(PCHGindMED[RightStrength+2],9)
+","+Numtostr(PCHGindMED[RightStrength+20],9)
+","+Numtostr(PCHGindMED[RightStrength+21],9)
+","+Numtostr(PCHGindMED[RightStrength+22],9)
+","+Numtostr(PCHGindMED[RightStrength+23],9)
+","+Numtostr(PCHGindMED[RightStrength+24],9)
+","+Numtostr(PCHGindMED[RightStrength+25],9)
+","+Numtostr(PCHGindMED[RightStrength+26],9)
+","+Numtostr(PCHGindMED[RightStrength+27],9)
+","+Numtostr(PCHGindMED[RightStrength+28],9)
+","+Numtostr(PCHGindMED[RightStrength+3],9)
+","+Numtostr(PCHGindMED[RightStrength+4],9)
+","+Numtostr(PCHGindMED[RightStrength+5],9)
+","+Numtostr(PCHGindMED[RightStrength+6],9)
+","+Numtostr(PCHGindMED[RightStrength+7],9)
+","+Numtostr(PCHGindMED[RightStrength+8],9)
+","+Numtostr(PCHGindMED[RightStrength+9],9)
+","+Numtostr(PCHGindMED[RightStrength-1],9)
+","+Numtostr(PCHGindMED[RightStrength-2],9)
+","+Numtostr(PCHGindMED[RightStrength-3],9)
+","+Numtostr(PCHGindMED[RightStrength-4],9)
+","+Numtostr(PCHGindSMALL[RightStrength+0],9)
+","+Numtostr(PCHGindSMALL[RightStrength+1],9)
+","+Numtostr(PCHGindSMALL[RightStrength+10],9)
+","+Numtostr(PCHGindSMALL[RightStrength+11],9)
+","+Numtostr(PCHGindSMALL[RightStrength+12],9)
+","+Numtostr(PCHGindSMALL[RightStrength+13],9)
+","+Numtostr(PCHGindSMALL[RightStrength+14],9)
+","+Numtostr(PCHGindSMALL[RightStrength+15],9)
+","+Numtostr(PCHGindSMALL[RightStrength+16],9)
+","+Numtostr(PCHGindSMALL[RightStrength+17],9)
+","+Numtostr(PCHGindSMALL[RightStrength+18],9)
+","+Numtostr(PCHGindSMALL[RightStrength+19],9)
+","+Numtostr(PCHGindSMALL[RightStrength+2],9)
+","+Numtostr(PCHGindSMALL[RightStrength+20],9)
+","+Numtostr(PCHGindSMALL[RightStrength+21],9)
+","+Numtostr(PCHGindSMALL[RightStrength+22],9)
+","+Numtostr(PCHGindSMALL[RightStrength+23],9)
+","+Numtostr(PCHGindSMALL[RightStrength+24],9)
+","+Numtostr(PCHGindSMALL[RightStrength+25],9)
+","+Numtostr(PCHGindSMALL[RightStrength+26],9)
+","+Numtostr(PCHGindSMALL[RightStrength+27],9)
+","+Numtostr(PCHGindSMALL[RightStrength+28],9)
+","+Numtostr(PCHGindSMALL[RightStrength+3],9)
+","+Numtostr(PCHGindSMALL[RightStrength+4],9)
+","+Numtostr(PCHGindSMALL[RightStrength+5],9)
+","+Numtostr(PCHGindSMALL[RightStrength+6],9)
+","+Numtostr(PCHGindSMALL[RightStrength+7],9)
+","+Numtostr(PCHGindSMALL[RightStrength+8],9)
+","+Numtostr(PCHGindSMALL[RightStrength+9],9)
+","+Numtostr(PCHGindSMALL[RightStrength-1],9)
+","+Numtostr(PCHGindSMALL[RightStrength-2],9)
+","+Numtostr(PCHGindSMALL[RightStrength-3],9)
+","+Numtostr(PCHGindSMALL[RightStrength-4],9)
+","+Numtostr(pctBindBIG[RightStrength+0],9)
+","+Numtostr(pctBindBIG[RightStrength+1],9)
+","+Numtostr(pctBindBIG[RightStrength+10],9)
+","+Numtostr(pctBindBIG[RightStrength+11],9)
+","+Numtostr(pctBindBIG[RightStrength+12],9)
+","+Numtostr(pctBindBIG[RightStrength+13],9)
+","+Numtostr(pctBindBIG[RightStrength+14],9)
+","+Numtostr(pctBindBIG[RightStrength+15],9)
+","+Numtostr(pctBindBIG[RightStrength+16],9)
+","+Numtostr(pctBindBIG[RightStrength+17],9)
+","+Numtostr(pctBindBIG[RightStrength+18],9)
+","+Numtostr(pctBindBIG[RightStrength+19],9)
+","+Numtostr(pctBindBIG[RightStrength+2],9)
+","+Numtostr(pctBindBIG[RightStrength+20],9)
+","+Numtostr(pctBindBIG[RightStrength+21],9)
+","+Numtostr(pctBindBIG[RightStrength+22],9)
+","+Numtostr(pctBindBIG[RightStrength+23],9)
+","+Numtostr(pctBindBIG[RightStrength+24],9)
+","+Numtostr(pctBindBIG[RightStrength+25],9)
+","+Numtostr(pctBindBIG[RightStrength+26],9)
+","+Numtostr(pctBindBIG[RightStrength+27],9)
+","+Numtostr(pctBindBIG[RightStrength+28],9)
+","+Numtostr(pctBindBIG[RightStrength+3],9)
+","+Numtostr(pctBindBIG[RightStrength+4],9)
+","+Numtostr(pctBindBIG[RightStrength+5],9)
+","+Numtostr(pctBindBIG[RightStrength+6],9)
+","+Numtostr(pctBindBIG[RightStrength+7],9)
+","+Numtostr(pctBindBIG[RightStrength+8],9)
+","+Numtostr(pctBindBIG[RightStrength+9],9)
+","+Numtostr(pctBindBIG[RightStrength-1],9)
+","+Numtostr(pctBindBIG[RightStrength-2],9)
+","+Numtostr(pctBindBIG[RightStrength-3],9)
+","+Numtostr(pctBindBIG[RightStrength-4],9)
+","+Numtostr(pctBindBX[RightStrength+0],9)
+","+Numtostr(pctBindBX[RightStrength+1],9)
+","+Numtostr(pctBindBX[RightStrength+10],9)
+","+Numtostr(pctBindBX[RightStrength+11],9)
+","+Numtostr(pctBindBX[RightStrength+12],9)
+","+Numtostr(pctBindBX[RightStrength+13],9)
+","+Numtostr(pctBindBX[RightStrength+14],9)
+","+Numtostr(pctBindBX[RightStrength+15],9)
+","+Numtostr(pctBindBX[RightStrength+16],9)
+","+Numtostr(pctBindBX[RightStrength+17],9)
+","+Numtostr(pctBindBX[RightStrength+18],9)
+","+Numtostr(pctBindBX[RightStrength+19],9)
+","+Numtostr(pctBindBX[RightStrength+2],9)
+","+Numtostr(pctBindBX[RightStrength+20],9)
+","+Numtostr(pctBindBX[RightStrength+21],9)
+","+Numtostr(pctBindBX[RightStrength+22],9)
+","+Numtostr(pctBindBX[RightStrength+23],9)
+","+Numtostr(pctBindBX[RightStrength+24],9)
+","+Numtostr(pctBindBX[RightStrength+25],9)
+","+Numtostr(pctBindBX[RightStrength+26],9)
+","+Numtostr(pctBindBX[RightStrength+27],9)
+","+Numtostr(pctBindBX[RightStrength+28],9)
+","+Numtostr(pctBindBX[RightStrength+3],9)
+","+Numtostr(pctBindBX[RightStrength+4],9)
+","+Numtostr(pctBindBX[RightStrength+5],9)
+","+Numtostr(pctBindBX[RightStrength+6],9)
+","+Numtostr(pctBindBX[RightStrength+7],9)
+","+Numtostr(pctBindBX[RightStrength+8],9)
+","+Numtostr(pctBindBX[RightStrength+9],9)
+","+Numtostr(pctBindBX[RightStrength-1],9)
+","+Numtostr(pctBindBX[RightStrength-2],9)
+","+Numtostr(pctBindBX[RightStrength-3],9)
			+" )"; 
			res7 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_feat7);
			srun7 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures7.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_feat7
					+ NewLine);
			end;
		End;
	End;
	 TblNum = TblNum+1;
	 

	//(4) insert feature data into dbo.barFeatures8	
	if ReadWriteToDB and TblNum = 8  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		if  srun8 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_feat8 = "INSERT INTO  [dbo].[barFeatures8] VALUES ("
			+queryResult//MAX(loadId) 
			+","+Numtostr(loadGroupId,0) 
			+","+Numtostr(RightStrength,0)//RightStrength
			+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
			+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"'"+dateBarOpened+"' "+","+"'"+dayOfWeekBarOpened+"' "+","+"'"+dateTimeBarOpened+"' "+","+"'"+timeBarOpenedhhmmss+"' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)
+","+Numtostr(ROCindBXL[RightStrength+17],9)
+","+Numtostr(ROCindBXL[RightStrength+18],9)
+","+Numtostr(ROCindBXL[RightStrength+19],9)
+","+Numtostr(ROCindBXL[RightStrength+2],9)
+","+Numtostr(ROCindBXL[RightStrength+20],9)
+","+Numtostr(ROCindBXL[RightStrength+21],9)
+","+Numtostr(ROCindBXL[RightStrength+22],9)
+","+Numtostr(ROCindBXL[RightStrength+23],9)
+","+Numtostr(ROCindBXL[RightStrength+24],9)
+","+Numtostr(ROCindBXL[RightStrength+25],9)
+","+Numtostr(ROCindBXL[RightStrength+26],9)
+","+Numtostr(ROCindBXL[RightStrength+27],9)
+","+Numtostr(ROCindBXL[RightStrength+28],9)
+","+Numtostr(ROCindBXL[RightStrength+3],9)
+","+Numtostr(ROCindBXL[RightStrength+4],9)
+","+Numtostr(ROCindBXL[RightStrength+5],9)
+","+Numtostr(ROCindBXL[RightStrength+6],9)
+","+Numtostr(ROCindBXL[RightStrength+7],9)
+","+Numtostr(ROCindBXL[RightStrength+8],9)
+","+Numtostr(ROCindBXL[RightStrength+9],9)
+","+Numtostr(ROCindBXL[RightStrength-1],9)
+","+Numtostr(ROCindBXL[RightStrength-2],9)
+","+Numtostr(ROCindBXL[RightStrength-3],9)
+","+Numtostr(ROCindBXL[RightStrength-4],9)
+","+Numtostr(ROCindMED[RightStrength+0],9)
+","+Numtostr(ROCindMED[RightStrength+1],9)
+","+Numtostr(ROCindMED[RightStrength+10],9)
+","+Numtostr(ROCindMED[RightStrength+11],9)
+","+Numtostr(ROCindMED[RightStrength+12],9)
+","+Numtostr(ROCindMED[RightStrength+13],9)
+","+Numtostr(ROCindMED[RightStrength+14],9)
+","+Numtostr(ROCindMED[RightStrength+15],9)
+","+Numtostr(ROCindMED[RightStrength+16],9)
+","+Numtostr(ROCindMED[RightStrength+17],9)
+","+Numtostr(ROCindMED[RightStrength+18],9)
+","+Numtostr(ROCindMED[RightStrength+19],9)
+","+Numtostr(ROCindMED[RightStrength+2],9)
+","+Numtostr(ROCindMED[RightStrength+20],9)
+","+Numtostr(ROCindMED[RightStrength+21],9)
+","+Numtostr(ROCindMED[RightStrength+22],9)
+","+Numtostr(ROCindMED[RightStrength+23],9)
+","+Numtostr(ROCindMED[RightStrength+24],9)
+","+Numtostr(ROCindMED[RightStrength+25],9)
+","+Numtostr(ROCindMED[RightStrength+26],9)
+","+Numtostr(ROCindMED[RightStrength+27],9)
+","+Numtostr(ROCindMED[RightStrength+28],9)
+","+Numtostr(ROCindMED[RightStrength+3],9)
+","+Numtostr(ROCindMED[RightStrength+4],9)
+","+Numtostr(ROCindMED[RightStrength+5],9)
+","+Numtostr(ROCindMED[RightStrength+6],9)
+","+Numtostr(ROCindMED[RightStrength+7],9)
+","+Numtostr(ROCindMED[RightStrength+8],9)
+","+Numtostr(ROCindMED[RightStrength+9],9)
+","+Numtostr(ROCindMED[RightStrength-1],9)
+","+Numtostr(ROCindMED[RightStrength-2],9)
+","+Numtostr(ROCindMED[RightStrength-3],9)
+","+Numtostr(ROCindMED[RightStrength-4],9)
+","+Numtostr(ROCindSMALL[RightStrength+0],9)
+","+Numtostr(ROCindSMALL[RightStrength+1],9)
+","+Numtostr(ROCindSMALL[RightStrength+10],9)
+","+Numtostr(ROCindSMALL[RightStrength+11],9)
+","+Numtostr(ROCindSMALL[RightStrength+12],9)
+","+Numtostr(ROCindSMALL[RightStrength+13],9)
+","+Numtostr(ROCindSMALL[RightStrength+14],9)
+","+Numtostr(ROCindSMALL[RightStrength+15],9)
+","+Numtostr(ROCindSMALL[RightStrength+16],9)
+","+Numtostr(ROCindSMALL[RightStrength+17],9)
+","+Numtostr(ROCindSMALL[RightStrength+18],9)
+","+Numtostr(ROCindSMALL[RightStrength+19],9)
+","+Numtostr(ROCindSMALL[RightStrength+2],9)
+","+Numtostr(ROCindSMALL[RightStrength+20],9)
+","+Numtostr(ROCindSMALL[RightStrength+21],9)
+","+Numtostr(ROCindSMALL[RightStrength+22],9)
+","+Numtostr(ROCindSMALL[RightStrength+23],9)
+","+Numtostr(ROCindSMALL[RightStrength+24],9)
+","+Numtostr(ROCindSMALL[RightStrength+25],9)
+","+Numtostr(ROCindSMALL[RightStrength+26],9)
+","+Numtostr(ROCindSMALL[RightStrength+27],9)
+","+Numtostr(ROCindSMALL[RightStrength+28],9)
+","+Numtostr(ROCindSMALL[RightStrength+3],9)
+","+Numtostr(ROCindSMALL[RightStrength+4],9)
+","+Numtostr(ROCindSMALL[RightStrength+5],9)
+","+Numtostr(ROCindSMALL[RightStrength+6],9)
+","+Numtostr(ROCindSMALL[RightStrength+7],9)
+","+Numtostr(ROCindSMALL[RightStrength+8],9)
+","+Numtostr(ROCindSMALL[RightStrength+9],9)
+","+Numtostr(ROCindSMALL[RightStrength-1],9)
+","+Numtostr(ROCindSMALL[RightStrength-2],9)
+","+Numtostr(ROCindSMALL[RightStrength-3],9)
+","+Numtostr(ROCindSMALL[RightStrength-4],9)
+","+Numtostr(RSIHULindBIG[RightStrength+0],9)
+","+Numtostr(RSIHULindBIG[RightStrength+1],9)
+","+Numtostr(RSIHULindBIG[RightStrength+10],9)
+","+Numtostr(RSIHULindBIG[RightStrength+11],9)
+","+Numtostr(RSIHULindBIG[RightStrength+12],9)
+","+Numtostr(RSIHULindBIG[RightStrength+13],9)
+","+Numtostr(RSIHULindBIG[RightStrength+14],9)
+","+Numtostr(RSIHULindBIG[RightStrength+15],9)
+","+Numtostr(RSIHULindBIG[RightStrength+16],9)
+","+Numtostr(RSIHULindBIG[RightStrength+17],9)
+","+Numtostr(RSIHULindBIG[RightStrength+18],9)
+","+Numtostr(RSIHULindBIG[RightStrength+19],9)
+","+Numtostr(RSIHULindBIG[RightStrength+2],9)
+","+Numtostr(RSIHULindBIG[RightStrength+20],9)
+","+Numtostr(RSIHULindBIG[RightStrength+21],9)
+","+Numtostr(RSIHULindBIG[RightStrength+22],9)
+","+Numtostr(RSIHULindBIG[RightStrength+23],9)
+","+Numtostr(RSIHULindBIG[RightStrength+24],9)
+","+Numtostr(RSIHULindBIG[RightStrength+25],9)
+","+Numtostr(RSIHULindBIG[RightStrength+26],9)
+","+Numtostr(RSIHULindBIG[RightStrength+27],9)
+","+Numtostr(RSIHULindBIG[RightStrength+28],9)
+","+Numtostr(RSIHULindBIG[RightStrength+3],9)
+","+Numtostr(RSIHULindBIG[RightStrength+4],9)
+","+Numtostr(RSIHULindBIG[RightStrength+5],9)
+","+Numtostr(RSIHULindBIG[RightStrength+6],9)
+","+Numtostr(RSIHULindBIG[RightStrength+7],9)
+","+Numtostr(RSIHULindBIG[RightStrength+8],9)
+","+Numtostr(RSIHULindBIG[RightStrength+9],9)
+","+Numtostr(RSIHULindBIG[RightStrength-1],9)
+","+Numtostr(RSIHULindBIG[RightStrength-2],9)
+","+Numtostr(RSIHULindBIG[RightStrength-3],9)
+","+Numtostr(RSIHULindBIG[RightStrength-4],9)
+","+Numtostr(RSIHULindBX[RightStrength+0],9)
+","+Numtostr(RSIHULindBX[RightStrength+1],9)
+","+Numtostr(RSIHULindBX[RightStrength+10],9)
+","+Numtostr(RSIHULindBX[RightStrength+11],9)
+","+Numtostr(RSIHULindBX[RightStrength+12],9)
+","+Numtostr(RSIHULindBX[RightStrength+13],9)
+","+Numtostr(RSIHULindBX[RightStrength+14],9)
+","+Numtostr(RSIHULindBX[RightStrength+15],9)
+","+Numtostr(RSIHULindBX[RightStrength+16],9)
+","+Numtostr(RSIHULindBX[RightStrength+17],9)
+","+Numtostr(RSIHULindBX[RightStrength+18],9)
+","+Numtostr(RSIHULindBX[RightStrength+19],9)
+","+Numtostr(RSIHULindBX[RightStrength+2],9)
+","+Numtostr(RSIHULindBX[RightStrength+20],9)
+","+Numtostr(RSIHULindBX[RightStrength+21],9)
+","+Numtostr(RSIHULindBX[RightStrength+22],9)
+","+Numtostr(RSIHULindBX[RightStrength+23],9)
+","+Numtostr(RSIHULindBX[RightStrength+24],9)
+","+Numtostr(RSIHULindBX[RightStrength+25],9)
+","+Numtostr(RSIHULindBX[RightStrength+26],9)
+","+Numtostr(RSIHULindBX[RightStrength+27],9)
+","+Numtostr(RSIHULindBX[RightStrength+28],9)
+","+Numtostr(RSIHULindBX[RightStrength+3],9)
+","+Numtostr(RSIHULindBX[RightStrength+4],9)
+","+Numtostr(RSIHULindBX[RightStrength+5],9)
+","+Numtostr(RSIHULindBX[RightStrength+6],9)
+","+Numtostr(RSIHULindBX[RightStrength+7],9)
+","+Numtostr(RSIHULindBX[RightStrength+8],9)
+","+Numtostr(RSIHULindBX[RightStrength+9],9)
+","+Numtostr(RSIHULindBX[RightStrength-1],9)
+","+Numtostr(RSIHULindBX[RightStrength-2],9)
+","+Numtostr(RSIHULindBX[RightStrength-3],9)
+","+Numtostr(RSIHULindBX[RightStrength-4],9)
+","+Numtostr(RSIHULindBXL[RightStrength+0],9)
+","+Numtostr(RSIHULindBXL[RightStrength+1],9)
+","+Numtostr(RSIHULindBXL[RightStrength+10],9)
+","+Numtostr(RSIHULindBXL[RightStrength+11],9)
+","+Numtostr(RSIHULindBXL[RightStrength+12],9)
+","+Numtostr(RSIHULindBXL[RightStrength+13],9)
+","+Numtostr(RSIHULindBXL[RightStrength+14],9)
+","+Numtostr(RSIHULindBXL[RightStrength+15],9)
+","+Numtostr(RSIHULindBXL[RightStrength+16],9)
+","+Numtostr(RSIHULindBXL[RightStrength+17],9)
+","+Numtostr(RSIHULindBXL[RightStrength+18],9)
+","+Numtostr(RSIHULindBXL[RightStrength+19],9)
+","+Numtostr(RSIHULindBXL[RightStrength+2],9)
+","+Numtostr(RSIHULindBXL[RightStrength+20],9)
+","+Numtostr(RSIHULindBXL[RightStrength+21],9)
+","+Numtostr(RSIHULindBXL[RightStrength+22],9)
+","+Numtostr(RSIHULindBXL[RightStrength+23],9)
+","+Numtostr(RSIHULindBXL[RightStrength+24],9)
+","+Numtostr(RSIHULindBXL[RightStrength+25],9)
+","+Numtostr(RSIHULindBXL[RightStrength+26],9)
+","+Numtostr(RSIHULindBXL[RightStrength+27],9)
+","+Numtostr(RSIHULindBXL[RightStrength+28],9)
+","+Numtostr(RSIHULindBXL[RightStrength+3],9)
+","+Numtostr(RSIHULindBXL[RightStrength+4],9)
+","+Numtostr(RSIHULindBXL[RightStrength+5],9)
+","+Numtostr(RSIHULindBXL[RightStrength+6],9)
+","+Numtostr(RSIHULindBXL[RightStrength+7],9)
+","+Numtostr(RSIHULindBXL[RightStrength+8],9)
+","+Numtostr(RSIHULindBXL[RightStrength+9],9)
+","+Numtostr(RSIHULindBXL[RightStrength-1],9)
+","+Numtostr(RSIHULindBXL[RightStrength-2],9)
+","+Numtostr(RSIHULindBXL[RightStrength-3],9)
+","+Numtostr(RSIHULindBXL[RightStrength-4],9)
+","+Numtostr(RSIHULindMED[RightStrength+0],9)
+","+Numtostr(RSIHULindMED[RightStrength+1],9)
+","+Numtostr(RSIHULindMED[RightStrength+10],9)
+","+Numtostr(RSIHULindMED[RightStrength+11],9)
+","+Numtostr(RSIHULindMED[RightStrength+12],9)
+","+Numtostr(RSIHULindMED[RightStrength+13],9)
+","+Numtostr(RSIHULindMED[RightStrength+14],9)
+","+Numtostr(RSIHULindMED[RightStrength+15],9)
+","+Numtostr(RSIHULindMED[RightStrength+16],9)
+","+Numtostr(RSIHULindMED[RightStrength+17],9)
+","+Numtostr(RSIHULindMED[RightStrength+18],9)
+","+Numtostr(RSIHULindMED[RightStrength+19],9)
+","+Numtostr(RSIHULindMED[RightStrength+2],9)
+","+Numtostr(RSIHULindMED[RightStrength+20],9)
+","+Numtostr(RSIHULindMED[RightStrength+21],9)
+","+Numtostr(RSIHULindMED[RightStrength+22],9)
+","+Numtostr(RSIHULindMED[RightStrength+23],9)
+","+Numtostr(RSIHULindMED[RightStrength+24],9)
+","+Numtostr(RSIHULindMED[RightStrength+25],9)
+","+Numtostr(RSIHULindMED[RightStrength+26],9)
+","+Numtostr(RSIHULindMED[RightStrength+27],9)
+","+Numtostr(RSIHULindMED[RightStrength+28],9)
+","+Numtostr(RSIHULindMED[RightStrength+3],9)
+","+Numtostr(RSIHULindMED[RightStrength+4],9)
+","+Numtostr(RSIHULindMED[RightStrength+5],9)
+","+Numtostr(RSIHULindMED[RightStrength+6],9)
+","+Numtostr(RSIHULindMED[RightStrength+7],9)
+","+Numtostr(RSIHULindMED[RightStrength+8],9)
+","+Numtostr(RSIHULindMED[RightStrength+9],9)
+","+Numtostr(RSIHULindMED[RightStrength-1],9)
+","+Numtostr(RSIHULindMED[RightStrength-2],9)
+","+Numtostr(RSIHULindMED[RightStrength-3],9)
+","+Numtostr(RSIHULindMED[RightStrength-4],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+0],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+1],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+10],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+11],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+12],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+13],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+14],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+15],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+16],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+17],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+18],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+19],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+2],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+20],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+21],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+22],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+23],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+24],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+25],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+26],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+27],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+28],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+3],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+4],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+5],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+6],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+7],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+8],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+9],9)
+","+Numtostr(RSIHULindSMALL[RightStrength-1],9)
+","+Numtostr(RSIHULindSMALL[RightStrength-2],9)
+","+Numtostr(RSIHULindSMALL[RightStrength-3],9)
+","+Numtostr(RSIHULindSMALL[RightStrength-4],9)
+","+Numtostr(RSIindBIG[RightStrength+0],9)
+","+Numtostr(RSIindBIG[RightStrength+1],9)
+","+Numtostr(RSIindBIG[RightStrength+10],9)
+","+Numtostr(RSIindBIG[RightStrength+11],9)
+","+Numtostr(RSIindBIG[RightStrength+12],9)
+","+Numtostr(RSIindBIG[RightStrength+13],9)
+","+Numtostr(RSIindBIG[RightStrength+14],9)
+","+Numtostr(RSIindBIG[RightStrength+15],9)
+","+Numtostr(RSIindBIG[RightStrength+16],9)
+","+Numtostr(RSIindBIG[RightStrength+17],9)
+","+Numtostr(RSIindBIG[RightStrength+18],9)
+","+Numtostr(RSIindBIG[RightStrength+19],9)
+","+Numtostr(RSIindBIG[RightStrength+2],9)
+","+Numtostr(RSIindBIG[RightStrength+20],9)
+","+Numtostr(RSIindBIG[RightStrength+21],9)
+","+Numtostr(RSIindBIG[RightStrength+22],9)
+","+Numtostr(RSIindBIG[RightStrength+23],9)
+","+Numtostr(RSIindBIG[RightStrength+24],9)
+","+Numtostr(RSIindBIG[RightStrength+25],9)
+","+Numtostr(RSIindBIG[RightStrength+26],9)
+","+Numtostr(RSIindBIG[RightStrength+27],9)
+","+Numtostr(RSIindBIG[RightStrength+28],9)
+","+Numtostr(RSIindBIG[RightStrength+3],9)
+","+Numtostr(RSIindBIG[RightStrength+4],9)
+","+Numtostr(RSIindBIG[RightStrength+5],9)
+","+Numtostr(RSIindBIG[RightStrength+6],9)
+","+Numtostr(RSIindBIG[RightStrength+7],9)
+","+Numtostr(RSIindBIG[RightStrength+8],9)
+","+Numtostr(RSIindBIG[RightStrength+9],9)
+","+Numtostr(RSIindBIG[RightStrength-1],9)
+","+Numtostr(RSIindBIG[RightStrength-2],9)
+","+Numtostr(RSIindBIG[RightStrength-3],9)
+","+Numtostr(RSIindBIG[RightStrength-4],9)
+","+Numtostr(RSIindBX[RightStrength+0],9)
+","+Numtostr(RSIindBX[RightStrength+1],9)
+","+Numtostr(RSIindBX[RightStrength+10],9)
+","+Numtostr(RSIindBX[RightStrength+11],9)
+","+Numtostr(RSIindBX[RightStrength+12],9)
+","+Numtostr(RSIindBX[RightStrength+13],9)
+","+Numtostr(RSIindBX[RightStrength+14],9)
+","+Numtostr(RSIindBX[RightStrength+15],9)
+","+Numtostr(RSIindBX[RightStrength+16],9)
+","+Numtostr(RSIindBX[RightStrength+17],9)
+","+Numtostr(RSIindBX[RightStrength+18],9)
+","+Numtostr(RSIindBX[RightStrength+19],9)
+","+Numtostr(RSIindBX[RightStrength+2],9)
+","+Numtostr(RSIindBX[RightStrength+20],9)
+","+Numtostr(RSIindBX[RightStrength+21],9)
+","+Numtostr(RSIindBX[RightStrength+22],9)
+","+Numtostr(RSIindBX[RightStrength+23],9)
+","+Numtostr(RSIindBX[RightStrength+24],9)
+","+Numtostr(RSIindBX[RightStrength+25],9)
+","+Numtostr(RSIindBX[RightStrength+26],9)
+","+Numtostr(RSIindBX[RightStrength+27],9)
+","+Numtostr(RSIindBX[RightStrength+28],9)
+","+Numtostr(RSIindBX[RightStrength+3],9)
+","+Numtostr(RSIindBX[RightStrength+4],9)
+","+Numtostr(RSIindBX[RightStrength+5],9)
+","+Numtostr(RSIindBX[RightStrength+6],9)
+","+Numtostr(RSIindBX[RightStrength+7],9)
+","+Numtostr(RSIindBX[RightStrength+8],9)
+","+Numtostr(RSIindBX[RightStrength+9],9)
+","+Numtostr(RSIindBX[RightStrength-1],9)
+","+Numtostr(RSIindBX[RightStrength-2],9)
+","+Numtostr(RSIindBX[RightStrength-3],9)
+","+Numtostr(RSIindBX[RightStrength-4],9)
+","+Numtostr(RSIindBXL[RightStrength+0],9)
+","+Numtostr(RSIindBXL[RightStrength+1],9)
+","+Numtostr(RSIindBXL[RightStrength+10],9)
+","+Numtostr(RSIindBXL[RightStrength+11],9)
+","+Numtostr(RSIindBXL[RightStrength+12],9)
+","+Numtostr(RSIindBXL[RightStrength+13],9)
+","+Numtostr(RSIindBXL[RightStrength+14],9)
+","+Numtostr(RSIindBXL[RightStrength+15],9)
+","+Numtostr(RSIindBXL[RightStrength+16],9)
+","+Numtostr(RSIindBXL[RightStrength+17],9)
+","+Numtostr(RSIindBXL[RightStrength+18],9)
+","+Numtostr(RSIindBXL[RightStrength+19],9)
+","+Numtostr(RSIindBXL[RightStrength+2],9)
+","+Numtostr(RSIindBXL[RightStrength+20],9)
+","+Numtostr(RSIindBXL[RightStrength+21],9)
+","+Numtostr(RSIindBXL[RightStrength+22],9)
+","+Numtostr(RSIindBXL[RightStrength+23],9)
+","+Numtostr(RSIindBXL[RightStrength+24],9)
+","+Numtostr(RSIindBXL[RightStrength+25],9)
+","+Numtostr(RSIindBXL[RightStrength+26],9)
+","+Numtostr(RSIindBXL[RightStrength+27],9)
+","+Numtostr(RSIindBXL[RightStrength+28],9)
+","+Numtostr(RSIindBXL[RightStrength+3],9)
+","+Numtostr(RSIindBXL[RightStrength+4],9)
+","+Numtostr(RSIindBXL[RightStrength+5],9)
+","+Numtostr(RSIindBXL[RightStrength+6],9)
+","+Numtostr(RSIindBXL[RightStrength+7],9)
+","+Numtostr(RSIindBXL[RightStrength+8],9)
+","+Numtostr(RSIindBXL[RightStrength+9],9)
+","+Numtostr(RSIindBXL[RightStrength-1],9)
+","+Numtostr(RSIindBXL[RightStrength-2],9)
+","+Numtostr(RSIindBXL[RightStrength-3],9)
+","+Numtostr(RSIindBXL[RightStrength-4],9)
+","+Numtostr(RSIindMED[RightStrength+0],9)
+","+Numtostr(RSIindMED[RightStrength+1],9)
+","+Numtostr(RSIindMED[RightStrength+10],9)
+","+Numtostr(RSIindMED[RightStrength+11],9)
+","+Numtostr(RSIindMED[RightStrength+12],9)
+","+Numtostr(RSIindMED[RightStrength+13],9)
+","+Numtostr(RSIindMED[RightStrength+14],9)
+","+Numtostr(RSIindMED[RightStrength+15],9)
+","+Numtostr(RSIindMED[RightStrength+16],9)
+","+Numtostr(RSIindMED[RightStrength+17],9)
+","+Numtostr(RSIindMED[RightStrength+18],9)
+","+Numtostr(RSIindMED[RightStrength+19],9)
+","+Numtostr(RSIindMED[RightStrength+2],9)
+","+Numtostr(RSIindMED[RightStrength+20],9)
+","+Numtostr(RSIindMED[RightStrength+21],9)
+","+Numtostr(RSIindMED[RightStrength+22],9)
+","+Numtostr(RSIindMED[RightStrength+23],9)
+","+Numtostr(RSIindMED[RightStrength+24],9)
+","+Numtostr(RSIindMED[RightStrength+25],9)
+","+Numtostr(RSIindMED[RightStrength+26],9)
+","+Numtostr(RSIindMED[RightStrength+27],9)
+","+Numtostr(RSIindMED[RightStrength+28],9)
+","+Numtostr(RSIindMED[RightStrength+3],9)
+","+Numtostr(RSIindMED[RightStrength+4],9)
+","+Numtostr(RSIindMED[RightStrength+5],9)
+","+Numtostr(RSIindMED[RightStrength+6],9)
+","+Numtostr(RSIindMED[RightStrength+7],9)
+","+Numtostr(RSIindMED[RightStrength+8],9)
+","+Numtostr(RSIindMED[RightStrength+9],9)
+","+Numtostr(RSIindMED[RightStrength-1],9)
+","+Numtostr(RSIindMED[RightStrength-2],9)
+","+Numtostr(RSIindMED[RightStrength-3],9)
+","+Numtostr(RSIindMED[RightStrength-4],9)
+","+Numtostr(RSIindSMALL[RightStrength+0],9)
+","+Numtostr(RSIindSMALL[RightStrength+1],9)
+","+Numtostr(RSIindSMALL[RightStrength+10],9)
+","+Numtostr(RSIindSMALL[RightStrength+11],9)
+","+Numtostr(RSIindSMALL[RightStrength+12],9)
+","+Numtostr(RSIindSMALL[RightStrength+13],9)
+","+Numtostr(RSIindSMALL[RightStrength+14],9)
+","+Numtostr(RSIindSMALL[RightStrength+15],9)
+","+Numtostr(RSIindSMALL[RightStrength+16],9)
+","+Numtostr(RSIindSMALL[RightStrength+17],9)
+","+Numtostr(RSIindSMALL[RightStrength+18],9)
+","+Numtostr(RSIindSMALL[RightStrength+19],9)
+","+Numtostr(RSIindSMALL[RightStrength+2],9)
+","+Numtostr(RSIindSMALL[RightStrength+20],9)
+","+Numtostr(RSIindSMALL[RightStrength+21],9)
+","+Numtostr(RSIindSMALL[RightStrength+22],9)
+","+Numtostr(RSIindSMALL[RightStrength+23],9)
+","+Numtostr(RSIindSMALL[RightStrength+24],9)
+","+Numtostr(RSIindSMALL[RightStrength+25],9)
+","+Numtostr(RSIindSMALL[RightStrength+26],9)
+","+Numtostr(RSIindSMALL[RightStrength+27],9)
+","+Numtostr(RSIindSMALL[RightStrength+28],9)
+","+Numtostr(RSIindSMALL[RightStrength+3],9)
+","+Numtostr(RSIindSMALL[RightStrength+4],9)
+","+Numtostr(RSIindSMALL[RightStrength+5],9)
+","+Numtostr(RSIindSMALL[RightStrength+6],9)
+","+Numtostr(RSIindSMALL[RightStrength+7],9)
+","+Numtostr(RSIindSMALL[RightStrength+8],9)
+","+Numtostr(RSIindSMALL[RightStrength+9],9)
+","+Numtostr(RSIindSMALL[RightStrength-1],9)
+","+Numtostr(RSIindSMALL[RightStrength-2],9)
+","+Numtostr(RSIindSMALL[RightStrength-3],9)
+","+Numtostr(RSIindSMALL[RightStrength-4],9)
+","+Numtostr(RSQRindBIG[RightStrength+0],9)
+","+Numtostr(RSQRindBIG[RightStrength+1],9)
+","+Numtostr(RSQRindBIG[RightStrength+10],9)
+","+Numtostr(RSQRindBIG[RightStrength+11],9)
+","+Numtostr(RSQRindBIG[RightStrength+12],9)
+","+Numtostr(RSQRindBIG[RightStrength+13],9)
+","+Numtostr(RSQRindBIG[RightStrength+14],9)
+","+Numtostr(RSQRindBIG[RightStrength+15],9)
+","+Numtostr(RSQRindBIG[RightStrength+16],9)
+","+Numtostr(RSQRindBIG[RightStrength+17],9)
+","+Numtostr(RSQRindBIG[RightStrength+18],9)
+","+Numtostr(RSQRindBIG[RightStrength+19],9)
+","+Numtostr(RSQRindBIG[RightStrength+2],9)
+","+Numtostr(RSQRindBIG[RightStrength+20],9)
+","+Numtostr(RSQRindBIG[RightStrength+21],9)
+","+Numtostr(RSQRindBIG[RightStrength+22],9)
+","+Numtostr(RSQRindBIG[RightStrength+23],9)
+","+Numtostr(RSQRindBIG[RightStrength+24],9)
+","+Numtostr(RSQRindBIG[RightStrength+25],9)
+","+Numtostr(RSQRindBIG[RightStrength+26],9)
+","+Numtostr(RSQRindBIG[RightStrength+27],9)
+","+Numtostr(RSQRindBIG[RightStrength+28],9)
+","+Numtostr(RSQRindBIG[RightStrength+3],9)
+","+Numtostr(RSQRindBIG[RightStrength+4],9)
+","+Numtostr(RSQRindBIG[RightStrength+5],9)
+","+Numtostr(RSQRindBIG[RightStrength+6],9)
+","+Numtostr(RSQRindBIG[RightStrength+7],9)
+","+Numtostr(RSQRindBIG[RightStrength+8],9)
+","+Numtostr(RSQRindBIG[RightStrength+9],9)
+","+Numtostr(RSQRindBIG[RightStrength-1],9)
+","+Numtostr(RSQRindBIG[RightStrength-2],9)
+","+Numtostr(RSQRindBIG[RightStrength-3],9)
+","+Numtostr(RSQRindBIG[RightStrength-4],9)
+","+Numtostr(RSQRindBX[RightStrength+0],9)
+","+Numtostr(RSQRindBX[RightStrength+1],9)
+","+Numtostr(RSQRindBX[RightStrength+10],9)
+","+Numtostr(RSQRindBX[RightStrength+11],9)
+","+Numtostr(RSQRindBX[RightStrength+12],9)
+","+Numtostr(RSQRindBX[RightStrength+13],9)
+","+Numtostr(RSQRindBX[RightStrength+14],9)
+","+Numtostr(RSQRindBX[RightStrength+15],9)
+","+Numtostr(RSQRindBX[RightStrength+16],9)
+","+Numtostr(RSQRindBX[RightStrength+17],9)
+","+Numtostr(RSQRindBX[RightStrength+18],9)
+","+Numtostr(RSQRindBX[RightStrength+19],9)
+","+Numtostr(RSQRindBX[RightStrength+2],9)
+","+Numtostr(RSQRindBX[RightStrength+20],9)
+","+Numtostr(RSQRindBX[RightStrength+21],9)
+","+Numtostr(RSQRindBX[RightStrength+22],9)
+","+Numtostr(RSQRindBX[RightStrength+23],9)
+","+Numtostr(RSQRindBX[RightStrength+24],9)
+","+Numtostr(RSQRindBX[RightStrength+25],9)
+","+Numtostr(RSQRindBX[RightStrength+26],9)
+","+Numtostr(RSQRindBX[RightStrength+27],9)
+","+Numtostr(RSQRindBX[RightStrength+28],9)
+","+Numtostr(RSQRindBX[RightStrength+3],9)
+","+Numtostr(RSQRindBX[RightStrength+4],9)
+","+Numtostr(RSQRindBX[RightStrength+5],9)
+","+Numtostr(RSQRindBX[RightStrength+6],9)
+","+Numtostr(RSQRindBX[RightStrength+7],9)
+","+Numtostr(RSQRindBX[RightStrength+8],9)
+","+Numtostr(RSQRindBX[RightStrength+9],9)
+","+Numtostr(RSQRindBX[RightStrength-1],9)
+","+Numtostr(RSQRindBX[RightStrength-2],9)
+","+Numtostr(RSQRindBX[RightStrength-3],9)
+","+Numtostr(RSQRindBX[RightStrength-4],9)
+","+Numtostr(RSQRindBXL[RightStrength+0],9)
+","+Numtostr(RSQRindBXL[RightStrength+1],9)
+","+Numtostr(RSQRindBXL[RightStrength+10],9)
+","+Numtostr(RSQRindBXL[RightStrength+11],9)
+","+Numtostr(RSQRindBXL[RightStrength+12],9)
+","+Numtostr(RSQRindBXL[RightStrength+13],9)
+","+Numtostr(RSQRindBXL[RightStrength+14],9)
+","+Numtostr(RSQRindBXL[RightStrength+15],9)
+","+Numtostr(RSQRindBXL[RightStrength+16],9)
+","+Numtostr(RSQRindBXL[RightStrength+17],9)
+","+Numtostr(RSQRindBXL[RightStrength+18],9)
+","+Numtostr(RSQRindBXL[RightStrength+19],9)
+","+Numtostr(RSQRindBXL[RightStrength+2],9)
+","+Numtostr(RSQRindBXL[RightStrength+20],9)
			+" )"; 
			res8 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_feat8);
			srun8 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures8.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_feat8
					+ NewLine);
			end;
		End;
	End;
	 TblNum = TblNum+1;
	 
	//(4) insert feature data into dbo.barFeatures9	
	if ReadWriteToDB and TblNum = 9  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		if  srun9 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_feat9 = "INSERT INTO  [dbo].[barFeatures9] VALUES ("
			+queryResult//MAX(loadId) 
			+","+Numtostr(loadGroupId,0) 
			+","+Numtostr(RightStrength,0)//RightStrength
			+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
			+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"'"+dateBarOpened+"' "+","+"'"+dayOfWeekBarOpened+"' "+","+"'"+dateTimeBarOpened+"' "+","+"'"+timeBarOpenedhhmmss+"' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)
+","+Numtostr(RSQRindBXL[RightStrength+21],9)
+","+Numtostr(RSQRindBXL[RightStrength+22],9)
+","+Numtostr(RSQRindBXL[RightStrength+23],9)
+","+Numtostr(RSQRindBXL[RightStrength+24],9)
+","+Numtostr(RSQRindBXL[RightStrength+25],9)
+","+Numtostr(RSQRindBXL[RightStrength+26],9)
+","+Numtostr(RSQRindBXL[RightStrength+27],9)
+","+Numtostr(RSQRindBXL[RightStrength+28],9)
+","+Numtostr(RSQRindBXL[RightStrength+3],9)
+","+Numtostr(RSQRindBXL[RightStrength+4],9)
+","+Numtostr(RSQRindBXL[RightStrength+5],9)
+","+Numtostr(RSQRindBXL[RightStrength+6],9)
+","+Numtostr(RSQRindBXL[RightStrength+7],9)
+","+Numtostr(RSQRindBXL[RightStrength+8],9)
+","+Numtostr(RSQRindBXL[RightStrength+9],9)
+","+Numtostr(RSQRindBXL[RightStrength-1],9)
+","+Numtostr(RSQRindBXL[RightStrength-2],9)
+","+Numtostr(RSQRindBXL[RightStrength-3],9)
+","+Numtostr(RSQRindBXL[RightStrength-4],9)
+","+Numtostr(RSQRindMED[RightStrength+0],9)
+","+Numtostr(RSQRindMED[RightStrength+1],9)
+","+Numtostr(RSQRindMED[RightStrength+10],9)
+","+Numtostr(RSQRindMED[RightStrength+11],9)
+","+Numtostr(RSQRindMED[RightStrength+12],9)
+","+Numtostr(RSQRindMED[RightStrength+13],9)
+","+Numtostr(RSQRindMED[RightStrength+14],9)
+","+Numtostr(RSQRindMED[RightStrength+15],9)
+","+Numtostr(RSQRindMED[RightStrength+16],9)
+","+Numtostr(RSQRindMED[RightStrength+17],9)
+","+Numtostr(RSQRindMED[RightStrength+18],9)
+","+Numtostr(RSQRindMED[RightStrength+19],9)
+","+Numtostr(RSQRindMED[RightStrength+2],9)
+","+Numtostr(RSQRindMED[RightStrength+20],9)
+","+Numtostr(RSQRindMED[RightStrength+21],9)
+","+Numtostr(RSQRindMED[RightStrength+22],9)
+","+Numtostr(RSQRindMED[RightStrength+23],9)
+","+Numtostr(RSQRindMED[RightStrength+24],9)
+","+Numtostr(RSQRindMED[RightStrength+25],9)
+","+Numtostr(RSQRindMED[RightStrength+26],9)
+","+Numtostr(RSQRindMED[RightStrength+27],9)
+","+Numtostr(RSQRindMED[RightStrength+28],9)
+","+Numtostr(RSQRindMED[RightStrength+3],9)
+","+Numtostr(RSQRindMED[RightStrength+4],9)
+","+Numtostr(RSQRindMED[RightStrength+5],9)
+","+Numtostr(RSQRindMED[RightStrength+6],9)
+","+Numtostr(RSQRindMED[RightStrength+7],9)
+","+Numtostr(RSQRindMED[RightStrength+8],9)
+","+Numtostr(RSQRindMED[RightStrength+9],9)
+","+Numtostr(RSQRindMED[RightStrength-1],9)
+","+Numtostr(RSQRindMED[RightStrength-2],9)
+","+Numtostr(RSQRindMED[RightStrength-3],9)
+","+Numtostr(RSQRindMED[RightStrength-4],9)
+","+Numtostr(RSQRindSMALL[RightStrength+0],9)
+","+Numtostr(RSQRindSMALL[RightStrength+1],9)
+","+Numtostr(RSQRindSMALL[RightStrength+10],9)
+","+Numtostr(RSQRindSMALL[RightStrength+11],9)
+","+Numtostr(RSQRindSMALL[RightStrength+12],9)
+","+Numtostr(RSQRindSMALL[RightStrength+13],9)
+","+Numtostr(RSQRindSMALL[RightStrength+14],9)
+","+Numtostr(RSQRindSMALL[RightStrength+15],9)
+","+Numtostr(RSQRindSMALL[RightStrength+16],9)
+","+Numtostr(RSQRindSMALL[RightStrength+17],9)
+","+Numtostr(RSQRindSMALL[RightStrength+18],9)
+","+Numtostr(RSQRindSMALL[RightStrength+19],9)
+","+Numtostr(RSQRindSMALL[RightStrength+2],9)
+","+Numtostr(RSQRindSMALL[RightStrength+20],9)
+","+Numtostr(RSQRindSMALL[RightStrength+21],9)
+","+Numtostr(RSQRindSMALL[RightStrength+22],9)
+","+Numtostr(RSQRindSMALL[RightStrength+23],9)
+","+Numtostr(RSQRindSMALL[RightStrength+24],9)
+","+Numtostr(RSQRindSMALL[RightStrength+25],9)
+","+Numtostr(RSQRindSMALL[RightStrength+26],9)
+","+Numtostr(RSQRindSMALL[RightStrength+27],9)
+","+Numtostr(RSQRindSMALL[RightStrength+28],9)
+","+Numtostr(RSQRindSMALL[RightStrength+3],9)
+","+Numtostr(RSQRindSMALL[RightStrength+4],9)
+","+Numtostr(RSQRindSMALL[RightStrength+5],9)
+","+Numtostr(RSQRindSMALL[RightStrength+6],9)
+","+Numtostr(RSQRindSMALL[RightStrength+7],9)
+","+Numtostr(RSQRindSMALL[RightStrength+8],9)
+","+Numtostr(RSQRindSMALL[RightStrength+9],9)
+","+Numtostr(RSQRindSMALL[RightStrength-1],9)
+","+Numtostr(RSQRindSMALL[RightStrength-2],9)
+","+Numtostr(RSQRindSMALL[RightStrength-3],9)
+","+Numtostr(RSQRindSMALL[RightStrength-4],9)
+","+Numtostr(RVIindBIG[RightStrength+0],9)
+","+Numtostr(RVIindBIG[RightStrength+1],9)
+","+Numtostr(RVIindBIG[RightStrength+10],9)
+","+Numtostr(RVIindBIG[RightStrength+11],9)
+","+Numtostr(RVIindBIG[RightStrength+12],9)
+","+Numtostr(RVIindBIG[RightStrength+13],9)
+","+Numtostr(RVIindBIG[RightStrength+14],9)
+","+Numtostr(RVIindBIG[RightStrength+15],9)
+","+Numtostr(RVIindBIG[RightStrength+16],9)
+","+Numtostr(RVIindBIG[RightStrength+17],9)
+","+Numtostr(RVIindBIG[RightStrength+18],9)
+","+Numtostr(RVIindBIG[RightStrength+19],9)
+","+Numtostr(RVIindBIG[RightStrength+2],9)
+","+Numtostr(RVIindBIG[RightStrength+20],9)
+","+Numtostr(RVIindBIG[RightStrength+21],9)
+","+Numtostr(RVIindBIG[RightStrength+22],9)
+","+Numtostr(RVIindBIG[RightStrength+23],9)
+","+Numtostr(RVIindBIG[RightStrength+24],9)
+","+Numtostr(RVIindBIG[RightStrength+25],9)
+","+Numtostr(RVIindBIG[RightStrength+26],9)
+","+Numtostr(RVIindBIG[RightStrength+27],9)
+","+Numtostr(RVIindBIG[RightStrength+28],9)
+","+Numtostr(RVIindBIG[RightStrength+3],9)
+","+Numtostr(RVIindBIG[RightStrength+4],9)
+","+Numtostr(RVIindBIG[RightStrength+5],9)
+","+Numtostr(RVIindBIG[RightStrength+6],9)
+","+Numtostr(RVIindBIG[RightStrength+7],9)
+","+Numtostr(RVIindBIG[RightStrength+8],9)
+","+Numtostr(RVIindBIG[RightStrength+9],9)
+","+Numtostr(RVIindBIG[RightStrength-1],9)
+","+Numtostr(RVIindBIG[RightStrength-2],9)
+","+Numtostr(RVIindBIG[RightStrength-3],9)
+","+Numtostr(RVIindBIG[RightStrength-4],9)
+","+Numtostr(RVIindBX[RightStrength+0],9)
+","+Numtostr(RVIindBX[RightStrength+1],9)
+","+Numtostr(RVIindBX[RightStrength+10],9)
+","+Numtostr(RVIindBX[RightStrength+11],9)
+","+Numtostr(RVIindBX[RightStrength+12],9)
+","+Numtostr(RVIindBX[RightStrength+13],9)
+","+Numtostr(RVIindBX[RightStrength+14],9)
+","+Numtostr(RVIindBX[RightStrength+15],9)
+","+Numtostr(RVIindBX[RightStrength+16],9)
+","+Numtostr(RVIindBX[RightStrength+17],9)
+","+Numtostr(RVIindBX[RightStrength+18],9)
+","+Numtostr(RVIindBX[RightStrength+19],9)
+","+Numtostr(RVIindBX[RightStrength+2],9)
+","+Numtostr(RVIindBX[RightStrength+20],9)
+","+Numtostr(RVIindBX[RightStrength+21],9)
+","+Numtostr(RVIindBX[RightStrength+22],9)
+","+Numtostr(RVIindBX[RightStrength+23],9)
+","+Numtostr(RVIindBX[RightStrength+24],9)
+","+Numtostr(RVIindBX[RightStrength+25],9)
+","+Numtostr(RVIindBX[RightStrength+26],9)
+","+Numtostr(RVIindBX[RightStrength+27],9)
+","+Numtostr(RVIindBX[RightStrength+28],9)
+","+Numtostr(RVIindBX[RightStrength+3],9)
+","+Numtostr(RVIindBX[RightStrength+4],9)
+","+Numtostr(RVIindBX[RightStrength+5],9)
+","+Numtostr(RVIindBX[RightStrength+6],9)
+","+Numtostr(RVIindBX[RightStrength+7],9)
+","+Numtostr(RVIindBX[RightStrength+8],9)
+","+Numtostr(RVIindBX[RightStrength+9],9)
+","+Numtostr(RVIindBX[RightStrength-1],9)
+","+Numtostr(RVIindBX[RightStrength-2],9)
+","+Numtostr(RVIindBX[RightStrength-3],9)
+","+Numtostr(RVIindBX[RightStrength-4],9)
+","+Numtostr(RVIindBXL[RightStrength+0],9)
+","+Numtostr(RVIindBXL[RightStrength+1],9)
+","+Numtostr(RVIindBXL[RightStrength+10],9)
+","+Numtostr(RVIindBXL[RightStrength+11],9)
+","+Numtostr(RVIindBXL[RightStrength+12],9)
+","+Numtostr(RVIindBXL[RightStrength+13],9)
+","+Numtostr(RVIindBXL[RightStrength+14],9)
+","+Numtostr(RVIindBXL[RightStrength+15],9)
+","+Numtostr(RVIindBXL[RightStrength+16],9)
+","+Numtostr(RVIindBXL[RightStrength+17],9)
+","+Numtostr(RVIindBXL[RightStrength+18],9)
+","+Numtostr(RVIindBXL[RightStrength+19],9)
+","+Numtostr(RVIindBXL[RightStrength+2],9)
+","+Numtostr(RVIindBXL[RightStrength+20],9)
+","+Numtostr(RVIindBXL[RightStrength+21],9)
+","+Numtostr(RVIindBXL[RightStrength+22],9)
+","+Numtostr(RVIindBXL[RightStrength+23],9)
+","+Numtostr(RVIindBXL[RightStrength+24],9)
+","+Numtostr(RVIindBXL[RightStrength+25],9)
+","+Numtostr(RVIindBXL[RightStrength+26],9)
+","+Numtostr(RVIindBXL[RightStrength+27],9)
+","+Numtostr(RVIindBXL[RightStrength+28],9)
+","+Numtostr(RVIindBXL[RightStrength+3],9)
+","+Numtostr(RVIindBXL[RightStrength+4],9)
+","+Numtostr(RVIindBXL[RightStrength+5],9)
+","+Numtostr(RVIindBXL[RightStrength+6],9)
+","+Numtostr(RVIindBXL[RightStrength+7],9)
+","+Numtostr(RVIindBXL[RightStrength+8],9)
+","+Numtostr(RVIindBXL[RightStrength+9],9)
+","+Numtostr(RVIindBXL[RightStrength-1],9)
+","+Numtostr(RVIindBXL[RightStrength-2],9)
+","+Numtostr(RVIindBXL[RightStrength-3],9)
+","+Numtostr(RVIindBXL[RightStrength-4],9)
+","+Numtostr(RVIindMED[RightStrength+0],9)
+","+Numtostr(RVIindMED[RightStrength+1],9)
+","+Numtostr(RVIindMED[RightStrength+10],9)
+","+Numtostr(RVIindMED[RightStrength+11],9)
+","+Numtostr(RVIindMED[RightStrength+12],9)
+","+Numtostr(RVIindMED[RightStrength+13],9)
+","+Numtostr(RVIindMED[RightStrength+14],9)
+","+Numtostr(RVIindMED[RightStrength+15],9)
+","+Numtostr(RVIindMED[RightStrength+16],9)
+","+Numtostr(RVIindMED[RightStrength+17],9)
+","+Numtostr(RVIindMED[RightStrength+18],9)
+","+Numtostr(RVIindMED[RightStrength+19],9)
+","+Numtostr(RVIindMED[RightStrength+2],9)
+","+Numtostr(RVIindMED[RightStrength+20],9)
+","+Numtostr(RVIindMED[RightStrength+21],9)
+","+Numtostr(RVIindMED[RightStrength+22],9)
+","+Numtostr(RVIindMED[RightStrength+23],9)
+","+Numtostr(RVIindMED[RightStrength+24],9)
+","+Numtostr(RVIindMED[RightStrength+25],9)
+","+Numtostr(RVIindMED[RightStrength+26],9)
+","+Numtostr(RVIindMED[RightStrength+27],9)
+","+Numtostr(RVIindMED[RightStrength+28],9)
+","+Numtostr(RVIindMED[RightStrength+3],9)
+","+Numtostr(RVIindMED[RightStrength+4],9)
+","+Numtostr(RVIindMED[RightStrength+5],9)
+","+Numtostr(RVIindMED[RightStrength+6],9)
+","+Numtostr(RVIindMED[RightStrength+7],9)
+","+Numtostr(RVIindMED[RightStrength+8],9)
+","+Numtostr(RVIindMED[RightStrength+9],9)
+","+Numtostr(RVIindMED[RightStrength-1],9)
+","+Numtostr(RVIindMED[RightStrength-2],9)
+","+Numtostr(RVIindMED[RightStrength-3],9)
+","+Numtostr(RVIindMED[RightStrength-4],9)
+","+Numtostr(RVIindSMALL[RightStrength+0],9)
+","+Numtostr(RVIindSMALL[RightStrength+1],9)
+","+Numtostr(RVIindSMALL[RightStrength+10],9)
+","+Numtostr(RVIindSMALL[RightStrength+11],9)
+","+Numtostr(RVIindSMALL[RightStrength+12],9)
+","+Numtostr(RVIindSMALL[RightStrength+13],9)
+","+Numtostr(RVIindSMALL[RightStrength+14],9)
+","+Numtostr(RVIindSMALL[RightStrength+15],9)
+","+Numtostr(RVIindSMALL[RightStrength+16],9)
+","+Numtostr(RVIindSMALL[RightStrength+17],9)
+","+Numtostr(RVIindSMALL[RightStrength+18],9)
+","+Numtostr(RVIindSMALL[RightStrength+19],9)
+","+Numtostr(RVIindSMALL[RightStrength+2],9)
+","+Numtostr(RVIindSMALL[RightStrength+20],9)
+","+Numtostr(RVIindSMALL[RightStrength+21],9)
+","+Numtostr(RVIindSMALL[RightStrength+22],9)
+","+Numtostr(RVIindSMALL[RightStrength+23],9)
+","+Numtostr(RVIindSMALL[RightStrength+24],9)
+","+Numtostr(RVIindSMALL[RightStrength+25],9)
+","+Numtostr(RVIindSMALL[RightStrength+26],9)
+","+Numtostr(RVIindSMALL[RightStrength+27],9)
+","+Numtostr(RVIindSMALL[RightStrength+28],9)
+","+Numtostr(RVIindSMALL[RightStrength+3],9)
+","+Numtostr(RVIindSMALL[RightStrength+4],9)
+","+Numtostr(RVIindSMALL[RightStrength+5],9)
+","+Numtostr(RVIindSMALL[RightStrength+6],9)
+","+Numtostr(RVIindSMALL[RightStrength+7],9)
+","+Numtostr(RVIindSMALL[RightStrength+8],9)
+","+Numtostr(RVIindSMALL[RightStrength+9],9)
+","+Numtostr(RVIindSMALL[RightStrength-1],9)
+","+Numtostr(RVIindSMALL[RightStrength-2],9)
+","+Numtostr(RVIindSMALL[RightStrength-3],9)
+","+Numtostr(RVIindSMALL[RightStrength-4],9)
+","+Numtostr(STDDEVindBIG[RightStrength+0],9)
+","+Numtostr(STDDEVindBIG[RightStrength+1],9)
+","+Numtostr(STDDEVindBIG[RightStrength+10],9)
+","+Numtostr(STDDEVindBIG[RightStrength+11],9)
+","+Numtostr(STDDEVindBIG[RightStrength+12],9)
+","+Numtostr(STDDEVindBIG[RightStrength+13],9)
+","+Numtostr(STDDEVindBIG[RightStrength+14],9)
+","+Numtostr(STDDEVindBIG[RightStrength+15],9)
+","+Numtostr(STDDEVindBIG[RightStrength+16],9)
+","+Numtostr(STDDEVindBIG[RightStrength+17],9)
+","+Numtostr(STDDEVindBIG[RightStrength+18],9)
+","+Numtostr(STDDEVindBIG[RightStrength+19],9)
+","+Numtostr(STDDEVindBIG[RightStrength+2],9)
+","+Numtostr(STDDEVindBIG[RightStrength+20],9)
+","+Numtostr(STDDEVindBIG[RightStrength+21],9)
+","+Numtostr(STDDEVindBIG[RightStrength+22],9)
+","+Numtostr(STDDEVindBIG[RightStrength+23],9)
+","+Numtostr(STDDEVindBIG[RightStrength+24],9)
+","+Numtostr(STDDEVindBIG[RightStrength+25],9)
+","+Numtostr(STDDEVindBIG[RightStrength+26],9)
+","+Numtostr(STDDEVindBIG[RightStrength+27],9)
+","+Numtostr(STDDEVindBIG[RightStrength+28],9)
+","+Numtostr(STDDEVindBIG[RightStrength+3],9)
+","+Numtostr(STDDEVindBIG[RightStrength+4],9)
+","+Numtostr(STDDEVindBIG[RightStrength+5],9)
+","+Numtostr(STDDEVindBIG[RightStrength+6],9)
+","+Numtostr(STDDEVindBIG[RightStrength+7],9)
+","+Numtostr(STDDEVindBIG[RightStrength+8],9)
+","+Numtostr(STDDEVindBIG[RightStrength+9],9)
+","+Numtostr(STDDEVindBIG[RightStrength-1],9)
+","+Numtostr(STDDEVindBIG[RightStrength-2],9)
+","+Numtostr(STDDEVindBIG[RightStrength-3],9)
+","+Numtostr(STDDEVindBIG[RightStrength-4],9)
+","+Numtostr(STDDEVindBX[RightStrength+0],9)
+","+Numtostr(STDDEVindBX[RightStrength+1],9)
+","+Numtostr(STDDEVindBX[RightStrength+10],9)
+","+Numtostr(STDDEVindBX[RightStrength+11],9)
+","+Numtostr(STDDEVindBX[RightStrength+12],9)
+","+Numtostr(STDDEVindBX[RightStrength+13],9)
+","+Numtostr(STDDEVindBX[RightStrength+14],9)
+","+Numtostr(STDDEVindBX[RightStrength+15],9)
+","+Numtostr(STDDEVindBX[RightStrength+16],9)
+","+Numtostr(STDDEVindBX[RightStrength+17],9)
+","+Numtostr(STDDEVindBX[RightStrength+18],9)
+","+Numtostr(STDDEVindBX[RightStrength+19],9)
+","+Numtostr(STDDEVindBX[RightStrength+2],9)
+","+Numtostr(STDDEVindBX[RightStrength+20],9)
+","+Numtostr(STDDEVindBX[RightStrength+21],9)
+","+Numtostr(STDDEVindBX[RightStrength+22],9)
+","+Numtostr(STDDEVindBX[RightStrength+23],9)
+","+Numtostr(STDDEVindBX[RightStrength+24],9)
+","+Numtostr(STDDEVindBX[RightStrength+25],9)
+","+Numtostr(STDDEVindBX[RightStrength+26],9)
+","+Numtostr(STDDEVindBX[RightStrength+27],9)
+","+Numtostr(STDDEVindBX[RightStrength+28],9)
+","+Numtostr(STDDEVindBX[RightStrength+3],9)
+","+Numtostr(STDDEVindBX[RightStrength+4],9)
+","+Numtostr(STDDEVindBX[RightStrength+5],9)
+","+Numtostr(STDDEVindBX[RightStrength+6],9)
+","+Numtostr(STDDEVindBX[RightStrength+7],9)
+","+Numtostr(STDDEVindBX[RightStrength+8],9)
+","+Numtostr(STDDEVindBX[RightStrength+9],9)
+","+Numtostr(STDDEVindBX[RightStrength-1],9)
+","+Numtostr(STDDEVindBX[RightStrength-2],9)
+","+Numtostr(STDDEVindBX[RightStrength-3],9)
+","+Numtostr(STDDEVindBX[RightStrength-4],9)
+","+Numtostr(STDDEVindBXL[RightStrength+0],9)
+","+Numtostr(STDDEVindBXL[RightStrength+1],9)
+","+Numtostr(STDDEVindBXL[RightStrength+10],9)
+","+Numtostr(STDDEVindBXL[RightStrength+11],9)
+","+Numtostr(STDDEVindBXL[RightStrength+12],9)
+","+Numtostr(STDDEVindBXL[RightStrength+13],9)
+","+Numtostr(STDDEVindBXL[RightStrength+14],9)
+","+Numtostr(STDDEVindBXL[RightStrength+15],9)
+","+Numtostr(STDDEVindBXL[RightStrength+16],9)
+","+Numtostr(STDDEVindBXL[RightStrength+17],9)
+","+Numtostr(STDDEVindBXL[RightStrength+18],9)
+","+Numtostr(STDDEVindBXL[RightStrength+19],9)
+","+Numtostr(STDDEVindBXL[RightStrength+2],9)
+","+Numtostr(STDDEVindBXL[RightStrength+20],9)
+","+Numtostr(STDDEVindBXL[RightStrength+21],9)
+","+Numtostr(STDDEVindBXL[RightStrength+22],9)
+","+Numtostr(STDDEVindBXL[RightStrength+23],9)
+","+Numtostr(STDDEVindBXL[RightStrength+24],9)
+","+Numtostr(STDDEVindBXL[RightStrength+25],9)
+","+Numtostr(STDDEVindBXL[RightStrength+26],9)
+","+Numtostr(STDDEVindBXL[RightStrength+27],9)
+","+Numtostr(STDDEVindBXL[RightStrength+28],9)
+","+Numtostr(STDDEVindBXL[RightStrength+3],9)
+","+Numtostr(STDDEVindBXL[RightStrength+4],9)
+","+Numtostr(STDDEVindBXL[RightStrength+5],9)
+","+Numtostr(STDDEVindBXL[RightStrength+6],9)
+","+Numtostr(STDDEVindBXL[RightStrength+7],9)
+","+Numtostr(STDDEVindBXL[RightStrength+8],9)
+","+Numtostr(STDDEVindBXL[RightStrength+9],9)
+","+Numtostr(STDDEVindBXL[RightStrength-1],9)
+","+Numtostr(STDDEVindBXL[RightStrength-2],9)
+","+Numtostr(STDDEVindBXL[RightStrength-3],9)
+","+Numtostr(STDDEVindBXL[RightStrength-4],9)
+","+Numtostr(STDDEVindMED[RightStrength+0],9)
+","+Numtostr(STDDEVindMED[RightStrength+1],9)
+","+Numtostr(STDDEVindMED[RightStrength+10],9)
+","+Numtostr(STDDEVindMED[RightStrength+11],9)
+","+Numtostr(STDDEVindMED[RightStrength+12],9)
+","+Numtostr(STDDEVindMED[RightStrength+13],9)
+","+Numtostr(STDDEVindMED[RightStrength+14],9)
+","+Numtostr(STDDEVindMED[RightStrength+15],9)
+","+Numtostr(STDDEVindMED[RightStrength+16],9)
+","+Numtostr(STDDEVindMED[RightStrength+17],9)
+","+Numtostr(STDDEVindMED[RightStrength+18],9)
+","+Numtostr(STDDEVindMED[RightStrength+19],9)
+","+Numtostr(STDDEVindMED[RightStrength+2],9)
+","+Numtostr(STDDEVindMED[RightStrength+20],9)
+","+Numtostr(STDDEVindMED[RightStrength+21],9)
+","+Numtostr(STDDEVindMED[RightStrength+22],9)
+","+Numtostr(STDDEVindMED[RightStrength+23],9)
+","+Numtostr(STDDEVindMED[RightStrength+24],9)
+","+Numtostr(STDDEVindMED[RightStrength+25],9)
+","+Numtostr(STDDEVindMED[RightStrength+26],9)
+","+Numtostr(STDDEVindMED[RightStrength+27],9)
+","+Numtostr(STDDEVindMED[RightStrength+28],9)
+","+Numtostr(STDDEVindMED[RightStrength+3],9)
+","+Numtostr(STDDEVindMED[RightStrength+4],9)
+","+Numtostr(STDDEVindMED[RightStrength+5],9)
+","+Numtostr(STDDEVindMED[RightStrength+6],9)
+","+Numtostr(STDDEVindMED[RightStrength+7],9)
+","+Numtostr(STDDEVindMED[RightStrength+8],9)
+","+Numtostr(STDDEVindMED[RightStrength+9],9)
+","+Numtostr(STDDEVindMED[RightStrength-1],9)
+","+Numtostr(STDDEVindMED[RightStrength-2],9)
+","+Numtostr(STDDEVindMED[RightStrength-3],9)
+","+Numtostr(STDDEVindMED[RightStrength-4],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+0],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+1],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+10],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+11],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+12],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+13],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+14],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+15],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+16],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+17],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+18],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+19],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+2],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+20],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+21],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+22],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+23],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+24],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+25],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+26],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+27],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+28],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+3],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+4],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+5],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+6],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+7],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+8],9)
+","+Numtostr(STDDEVindSMALL[RightStrength+9],9)
+","+Numtostr(STDDEVindSMALL[RightStrength-1],9)
+","+Numtostr(STDDEVindSMALL[RightStrength-2],9)
+","+Numtostr(STDDEVindSMALL[RightStrength-3],9)
+","+Numtostr(STDDEVindSMALL[RightStrength-4],9)
+","+Numtostr(STDERRindBIG[RightStrength+0],9)
+","+Numtostr(STDERRindBIG[RightStrength+1],9)
+","+Numtostr(STDERRindBIG[RightStrength+10],9)
+","+Numtostr(STDERRindBIG[RightStrength+11],9)
+","+Numtostr(STDERRindBIG[RightStrength+12],9)
+","+Numtostr(STDERRindBIG[RightStrength+13],9)
+","+Numtostr(STDERRindBIG[RightStrength+14],9)
+","+Numtostr(STDERRindBIG[RightStrength+15],9)
+","+Numtostr(STDERRindBIG[RightStrength+16],9)
+","+Numtostr(STDERRindBIG[RightStrength+17],9)
+","+Numtostr(STDERRindBIG[RightStrength+18],9)
+","+Numtostr(STDERRindBIG[RightStrength+19],9)
+","+Numtostr(STDERRindBIG[RightStrength+2],9)
+","+Numtostr(STDERRindBIG[RightStrength+20],9)
+","+Numtostr(STDERRindBIG[RightStrength+21],9)
+","+Numtostr(STDERRindBIG[RightStrength+22],9)
+","+Numtostr(STDERRindBIG[RightStrength+23],9)
+","+Numtostr(STDERRindBIG[RightStrength+24],9)
+","+Numtostr(STDERRindBIG[RightStrength+25],9)
+","+Numtostr(STDERRindBIG[RightStrength+26],9)
+","+Numtostr(STDERRindBIG[RightStrength+27],9)
+","+Numtostr(STDERRindBIG[RightStrength+28],9)
+","+Numtostr(STDERRindBIG[RightStrength+3],9)
+","+Numtostr(STDERRindBIG[RightStrength+4],9)
+","+Numtostr(STDERRindBIG[RightStrength+5],9)
+","+Numtostr(STDERRindBIG[RightStrength+6],9)
+","+Numtostr(STDERRindBIG[RightStrength+7],9)
+","+Numtostr(STDERRindBIG[RightStrength+8],9)
+","+Numtostr(STDERRindBIG[RightStrength+9],9)
+","+Numtostr(STDERRindBIG[RightStrength-1],9)
+","+Numtostr(STDERRindBIG[RightStrength-2],9)
+","+Numtostr(STDERRindBIG[RightStrength-3],9)
+","+Numtostr(STDERRindBIG[RightStrength-4],9)
+","+Numtostr(STDERRindBX[RightStrength+0],9)
+","+Numtostr(STDERRindBX[RightStrength+1],9)
+","+Numtostr(STDERRindBX[RightStrength+10],9)
+","+Numtostr(STDERRindBX[RightStrength+11],9)
+","+Numtostr(STDERRindBX[RightStrength+12],9)
+","+Numtostr(STDERRindBX[RightStrength+13],9)
+","+Numtostr(STDERRindBX[RightStrength+14],9)
+","+Numtostr(STDERRindBX[RightStrength+15],9)
+","+Numtostr(STDERRindBX[RightStrength+16],9)
+","+Numtostr(STDERRindBX[RightStrength+17],9)
+","+Numtostr(STDERRindBX[RightStrength+18],9)
+","+Numtostr(STDERRindBX[RightStrength+19],9)
+","+Numtostr(STDERRindBX[RightStrength+2],9)
+","+Numtostr(STDERRindBX[RightStrength+20],9)
+","+Numtostr(STDERRindBX[RightStrength+21],9)
+","+Numtostr(STDERRindBX[RightStrength+22],9)
+","+Numtostr(STDERRindBX[RightStrength+23],9)
+","+Numtostr(STDERRindBX[RightStrength+24],9)
+","+Numtostr(STDERRindBX[RightStrength+25],9)
+","+Numtostr(STDERRindBX[RightStrength+26],9)
+","+Numtostr(STDERRindBX[RightStrength+27],9)
+","+Numtostr(STDERRindBX[RightStrength+28],9)
+","+Numtostr(STDERRindBX[RightStrength+3],9)
+","+Numtostr(STDERRindBX[RightStrength+4],9)
+","+Numtostr(STDERRindBX[RightStrength+5],9)
+","+Numtostr(STDERRindBX[RightStrength+6],9)
+","+Numtostr(STDERRindBX[RightStrength+7],9)
+","+Numtostr(STDERRindBX[RightStrength+8],9)
+","+Numtostr(STDERRindBX[RightStrength+9],9)
+","+Numtostr(STDERRindBX[RightStrength-1],9)
+","+Numtostr(STDERRindBX[RightStrength-2],9)
+","+Numtostr(STDERRindBX[RightStrength-3],9)
+","+Numtostr(STDERRindBX[RightStrength-4],9)
+","+Numtostr(STDERRindBXL[RightStrength+0],9)
+","+Numtostr(STDERRindBXL[RightStrength+1],9)
+","+Numtostr(STDERRindBXL[RightStrength+10],9)
+","+Numtostr(STDERRindBXL[RightStrength+11],9)
+","+Numtostr(STDERRindBXL[RightStrength+12],9)
+","+Numtostr(STDERRindBXL[RightStrength+13],9)
+","+Numtostr(STDERRindBXL[RightStrength+14],9)
+","+Numtostr(STDERRindBXL[RightStrength+15],9)
+","+Numtostr(STDERRindBXL[RightStrength+16],9)
+","+Numtostr(STDERRindBXL[RightStrength+17],9)
+","+Numtostr(STDERRindBXL[RightStrength+18],9)
+","+Numtostr(STDERRindBXL[RightStrength+19],9)
+","+Numtostr(STDERRindBXL[RightStrength+2],9)
+","+Numtostr(STDERRindBXL[RightStrength+20],9)
+","+Numtostr(STDERRindBXL[RightStrength+21],9)
+","+Numtostr(STDERRindBXL[RightStrength+22],9)
+","+Numtostr(STDERRindBXL[RightStrength+23],9)
+","+Numtostr(STDERRindBXL[RightStrength+24],9)
+","+Numtostr(STDERRindBXL[RightStrength+25],9)
			+" )"; 
			res9 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_feat9);
			srun9 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures9.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_feat9
					+ NewLine);
			end;
		End;
	End;
	 TblNum = TblNum+1;
	//(4) insert feature data into dbo.barFeatures10	
	if ReadWriteToDB and TblNum = 10  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		if  srun10 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_feat10 = "INSERT INTO  [dbo].[barFeatures10] VALUES ("
			+queryResult//MAX(loadId) 
			+","+Numtostr(loadGroupId,0) 
			+","+Numtostr(RightStrength,0)//RightStrength
			+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
			+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"'"+dateBarOpened+"' "+","+"'"+dayOfWeekBarOpened+"' "+","+"'"+dateTimeBarOpened+"' "+","+"'"+timeBarOpenedhhmmss+"' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)
+","+Numtostr(STOSindBXL[RightStrength+5],9)
+","+Numtostr(STOSindBXL[RightStrength+6],9)
+","+Numtostr(STOSindBXL[RightStrength+7],9)
+","+Numtostr(STOSindBXL[RightStrength+8],9)
+","+Numtostr(STOSindBXL[RightStrength+9],9)
+","+Numtostr(STOSindBXL[RightStrength-1],9)
+","+Numtostr(STOSindBXL[RightStrength-2],9)
+","+Numtostr(STOSindBXL[RightStrength-3],9)
+","+Numtostr(STOSindBXL[RightStrength-4],9)
+","+Numtostr(STOSindMED[RightStrength+0],9)
+","+Numtostr(STOSindMED[RightStrength+1],9)
+","+Numtostr(STOSindMED[RightStrength+10],9)
+","+Numtostr(STOSindMED[RightStrength+11],9)
+","+Numtostr(STOSindMED[RightStrength+12],9)
+","+Numtostr(STOSindMED[RightStrength+13],9)
+","+Numtostr(STOSindMED[RightStrength+14],9)
+","+Numtostr(STOSindMED[RightStrength+15],9)
+","+Numtostr(STOSindMED[RightStrength+16],9)
+","+Numtostr(STOSindMED[RightStrength+17],9)
+","+Numtostr(STOSindMED[RightStrength+18],9)
+","+Numtostr(STOSindMED[RightStrength+19],9)
+","+Numtostr(STOSindMED[RightStrength+2],9)
+","+Numtostr(STOSindMED[RightStrength+20],9)
+","+Numtostr(STOSindMED[RightStrength+21],9)
+","+Numtostr(STOSindMED[RightStrength+22],9)
+","+Numtostr(STOSindMED[RightStrength+23],9)
+","+Numtostr(STOSindMED[RightStrength+24],9)
+","+Numtostr(STOSindMED[RightStrength+25],9)
+","+Numtostr(STOSindMED[RightStrength+26],9)
+","+Numtostr(STOSindMED[RightStrength+27],9)
+","+Numtostr(STOSindMED[RightStrength+28],9)
+","+Numtostr(STOSindMED[RightStrength+3],9)
+","+Numtostr(STOSindMED[RightStrength+4],9)
+","+Numtostr(STOSindMED[RightStrength+5],9)
+","+Numtostr(STOSindMED[RightStrength+6],9)
+","+Numtostr(STOSindMED[RightStrength+7],9)
+","+Numtostr(STOSindMED[RightStrength+8],9)
+","+Numtostr(STOSindMED[RightStrength+9],9)
+","+Numtostr(STOSindMED[RightStrength-1],9)
+","+Numtostr(STOSindMED[RightStrength-2],9)
+","+Numtostr(STOSindMED[RightStrength-3],9)
+","+Numtostr(STOSindMED[RightStrength-4],9)
+","+Numtostr(STOSindSMALL[RightStrength+0],9)
+","+Numtostr(STOSindSMALL[RightStrength+1],9)
+","+Numtostr(STOSindSMALL[RightStrength+10],9)
+","+Numtostr(STOSindSMALL[RightStrength+11],9)
+","+Numtostr(STOSindSMALL[RightStrength+12],9)
+","+Numtostr(STOSindSMALL[RightStrength+13],9)
+","+Numtostr(STOSindSMALL[RightStrength+14],9)
+","+Numtostr(STOSindSMALL[RightStrength+15],9)
+","+Numtostr(STOSindSMALL[RightStrength+16],9)
+","+Numtostr(STOSindSMALL[RightStrength+17],9)
+","+Numtostr(STOSindSMALL[RightStrength+18],9)
+","+Numtostr(STOSindSMALL[RightStrength+19],9)
+","+Numtostr(STOSindSMALL[RightStrength+2],9)
+","+Numtostr(STOSindSMALL[RightStrength+20],9)
+","+Numtostr(STOSindSMALL[RightStrength+21],9)
+","+Numtostr(STOSindSMALL[RightStrength+22],9)
+","+Numtostr(STOSindSMALL[RightStrength+23],9)
+","+Numtostr(STOSindSMALL[RightStrength+24],9)
+","+Numtostr(STOSindSMALL[RightStrength+25],9)
+","+Numtostr(STOSindSMALL[RightStrength+26],9)
+","+Numtostr(STOSindSMALL[RightStrength+27],9)
+","+Numtostr(STOSindSMALL[RightStrength+28],9)
+","+Numtostr(STOSindSMALL[RightStrength+3],9)
+","+Numtostr(STOSindSMALL[RightStrength+4],9)
+","+Numtostr(STOSindSMALL[RightStrength+5],9)
+","+Numtostr(STOSindSMALL[RightStrength+6],9)
+","+Numtostr(STOSindSMALL[RightStrength+7],9)
+","+Numtostr(STOSindSMALL[RightStrength+8],9)
+","+Numtostr(STOSindSMALL[RightStrength+9],9)
+","+Numtostr(STOSindSMALL[RightStrength-1],9)
+","+Numtostr(STOSindSMALL[RightStrength-2],9)
+","+Numtostr(STOSindSMALL[RightStrength-3],9)
+","+Numtostr(STOSindSMALL[RightStrength-4],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+0],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+1],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+10],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+11],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+12],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+13],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+14],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+15],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+16],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+17],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+18],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+19],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+2],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+20],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+21],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+22],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+23],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+24],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+25],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+26],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+27],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+28],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+3],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+4],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+5],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+6],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+7],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+8],9)
+","+Numtostr(UNIOSCindBIG[RightStrength+9],9)
+","+Numtostr(UNIOSCindBIG[RightStrength-1],9)
+","+Numtostr(UNIOSCindBIG[RightStrength-2],9)
+","+Numtostr(UNIOSCindBIG[RightStrength-3],9)
+","+Numtostr(UNIOSCindBIG[RightStrength-4],9)
+","+Numtostr(UNIOSCindBX[RightStrength+0],9)
+","+Numtostr(UNIOSCindBX[RightStrength+1],9)
+","+Numtostr(UNIOSCindBX[RightStrength+10],9)
+","+Numtostr(UNIOSCindBX[RightStrength+11],9)
+","+Numtostr(UNIOSCindBX[RightStrength+12],9)
+","+Numtostr(UNIOSCindBX[RightStrength+13],9)
+","+Numtostr(UNIOSCindBX[RightStrength+14],9)
+","+Numtostr(UNIOSCindBX[RightStrength+15],9)
+","+Numtostr(UNIOSCindBX[RightStrength+16],9)
+","+Numtostr(UNIOSCindBX[RightStrength+17],9)
+","+Numtostr(UNIOSCindBX[RightStrength+18],9)
+","+Numtostr(UNIOSCindBX[RightStrength+19],9)
+","+Numtostr(UNIOSCindBX[RightStrength+2],9)
+","+Numtostr(UNIOSCindBX[RightStrength+20],9)
+","+Numtostr(UNIOSCindBX[RightStrength+21],9)
+","+Numtostr(UNIOSCindBX[RightStrength+22],9)
+","+Numtostr(UNIOSCindBX[RightStrength+23],9)
+","+Numtostr(UNIOSCindBX[RightStrength+24],9)
+","+Numtostr(UNIOSCindBX[RightStrength+25],9)
+","+Numtostr(UNIOSCindBX[RightStrength+26],9)
+","+Numtostr(UNIOSCindBX[RightStrength+27],9)
+","+Numtostr(UNIOSCindBX[RightStrength+28],9)
+","+Numtostr(UNIOSCindBX[RightStrength+3],9)
+","+Numtostr(UNIOSCindBX[RightStrength+4],9)
+","+Numtostr(UNIOSCindBX[RightStrength+5],9)
+","+Numtostr(UNIOSCindBX[RightStrength+6],9)
+","+Numtostr(UNIOSCindBX[RightStrength+7],9)
+","+Numtostr(UNIOSCindBX[RightStrength+8],9)
+","+Numtostr(UNIOSCindBX[RightStrength+9],9)
+","+Numtostr(UNIOSCindBX[RightStrength-1],9)
+","+Numtostr(UNIOSCindBX[RightStrength-2],9)
+","+Numtostr(UNIOSCindBX[RightStrength-3],9)
+","+Numtostr(UNIOSCindBX[RightStrength-4],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+0],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+1],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+10],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+11],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+12],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+13],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+14],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+15],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+16],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+17],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+18],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+19],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+2],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+20],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+21],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+22],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+23],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+24],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+25],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+26],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+27],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+28],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+3],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+4],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+5],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+6],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+7],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+8],9)
+","+Numtostr(UNIOSCindBXL[RightStrength+9],9)
+","+Numtostr(UNIOSCindBXL[RightStrength-1],9)
+","+Numtostr(UNIOSCindBXL[RightStrength-2],9)
+","+Numtostr(UNIOSCindBXL[RightStrength-3],9)
+","+Numtostr(UNIOSCindBXL[RightStrength-4],9)
+","+Numtostr(UNIOSCindMED[RightStrength+0],9)
+","+Numtostr(UNIOSCindMED[RightStrength+1],9)
+","+Numtostr(UNIOSCindMED[RightStrength+10],9)
+","+Numtostr(UNIOSCindMED[RightStrength+11],9)
+","+Numtostr(UNIOSCindMED[RightStrength+12],9)
+","+Numtostr(UNIOSCindMED[RightStrength+13],9)
+","+Numtostr(UNIOSCindMED[RightStrength+14],9)
+","+Numtostr(UNIOSCindMED[RightStrength+15],9)
+","+Numtostr(UNIOSCindMED[RightStrength+16],9)
+","+Numtostr(UNIOSCindMED[RightStrength+17],9)
+","+Numtostr(UNIOSCindMED[RightStrength+18],9)
+","+Numtostr(UNIOSCindMED[RightStrength+19],9)
+","+Numtostr(UNIOSCindMED[RightStrength+2],9)
+","+Numtostr(UNIOSCindMED[RightStrength+20],9)
+","+Numtostr(UNIOSCindMED[RightStrength+21],9)
+","+Numtostr(UNIOSCindMED[RightStrength+22],9)
+","+Numtostr(UNIOSCindMED[RightStrength+23],9)
+","+Numtostr(UNIOSCindMED[RightStrength+24],9)
+","+Numtostr(UNIOSCindMED[RightStrength+25],9)
+","+Numtostr(UNIOSCindMED[RightStrength+26],9)
+","+Numtostr(UNIOSCindMED[RightStrength+27],9)
+","+Numtostr(UNIOSCindMED[RightStrength+28],9)
+","+Numtostr(UNIOSCindMED[RightStrength+3],9)
+","+Numtostr(UNIOSCindMED[RightStrength+4],9)
+","+Numtostr(UNIOSCindMED[RightStrength+5],9)
+","+Numtostr(UNIOSCindMED[RightStrength+6],9)
+","+Numtostr(UNIOSCindMED[RightStrength+7],9)
+","+Numtostr(UNIOSCindMED[RightStrength+8],9)
+","+Numtostr(UNIOSCindMED[RightStrength+9],9)
+","+Numtostr(UNIOSCindMED[RightStrength-1],9)
+","+Numtostr(UNIOSCindMED[RightStrength-2],9)
+","+Numtostr(UNIOSCindMED[RightStrength-3],9)
+","+Numtostr(UNIOSCindMED[RightStrength-4],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+0],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+1],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+10],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+11],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+12],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+13],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+14],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+15],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+16],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+17],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+18],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+19],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+2],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+20],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+21],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+22],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+23],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+24],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+25],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+26],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+27],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+28],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+3],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+4],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+5],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+6],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+7],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+8],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength+9],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength-1],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength-2],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength-3],9)
+","+Numtostr(UNIOSCindSMALL[RightStrength-4],9)
+","+Numtostr(VHFindBIG[RightStrength+0],9)
+","+Numtostr(VHFindBIG[RightStrength+1],9)
+","+Numtostr(VHFindBIG[RightStrength+10],9)
+","+Numtostr(VHFindBIG[RightStrength+11],9)
+","+Numtostr(VHFindBIG[RightStrength+12],9)
+","+Numtostr(VHFindBIG[RightStrength+13],9)
+","+Numtostr(VHFindBIG[RightStrength+14],9)
+","+Numtostr(VHFindBIG[RightStrength+15],9)
+","+Numtostr(VHFindBIG[RightStrength+16],9)
+","+Numtostr(VHFindBIG[RightStrength+17],9)
+","+Numtostr(VHFindBIG[RightStrength+18],9)
+","+Numtostr(VHFindBIG[RightStrength+19],9)
+","+Numtostr(VHFindBIG[RightStrength+2],9)
+","+Numtostr(VHFindBIG[RightStrength+20],9)
+","+Numtostr(VHFindBIG[RightStrength+21],9)
+","+Numtostr(VHFindBIG[RightStrength+22],9)
+","+Numtostr(VHFindBIG[RightStrength+23],9)
+","+Numtostr(VHFindBIG[RightStrength+24],9)
+","+Numtostr(VHFindBIG[RightStrength+25],9)
+","+Numtostr(VHFindBIG[RightStrength+26],9)
+","+Numtostr(VHFindBIG[RightStrength+27],9)
+","+Numtostr(VHFindBIG[RightStrength+28],9)
+","+Numtostr(VHFindBIG[RightStrength+3],9)
+","+Numtostr(VHFindBIG[RightStrength+4],9)
+","+Numtostr(VHFindBIG[RightStrength+5],9)
+","+Numtostr(VHFindBIG[RightStrength+6],9)
+","+Numtostr(VHFindBIG[RightStrength+7],9)
+","+Numtostr(VHFindBIG[RightStrength+8],9)
+","+Numtostr(VHFindBIG[RightStrength+9],9)
+","+Numtostr(VHFindBIG[RightStrength-1],9)
+","+Numtostr(VHFindBIG[RightStrength-2],9)
+","+Numtostr(VHFindBIG[RightStrength-3],9)
+","+Numtostr(VHFindBIG[RightStrength-4],9)
+","+Numtostr(VHFindBX[RightStrength+0],9)
+","+Numtostr(VHFindBX[RightStrength+1],9)
+","+Numtostr(VHFindBX[RightStrength+10],9)
+","+Numtostr(VHFindBX[RightStrength+11],9)
+","+Numtostr(VHFindBX[RightStrength+12],9)
+","+Numtostr(VHFindBX[RightStrength+13],9)
+","+Numtostr(VHFindBX[RightStrength+14],9)
+","+Numtostr(VHFindBX[RightStrength+15],9)
+","+Numtostr(VHFindBX[RightStrength+16],9)
+","+Numtostr(VHFindBX[RightStrength+17],9)
+","+Numtostr(VHFindBX[RightStrength+18],9)
+","+Numtostr(VHFindBX[RightStrength+19],9)
+","+Numtostr(VHFindBX[RightStrength+2],9)
+","+Numtostr(VHFindBX[RightStrength+20],9)
+","+Numtostr(VHFindBX[RightStrength+21],9)
+","+Numtostr(VHFindBX[RightStrength+22],9)
+","+Numtostr(VHFindBX[RightStrength+23],9)
+","+Numtostr(VHFindBX[RightStrength+24],9)
+","+Numtostr(VHFindBX[RightStrength+25],9)
+","+Numtostr(VHFindBX[RightStrength+26],9)
+","+Numtostr(VHFindBX[RightStrength+27],9)
+","+Numtostr(VHFindBX[RightStrength+28],9)
+","+Numtostr(VHFindBX[RightStrength+3],9)
+","+Numtostr(VHFindBX[RightStrength+4],9)
+","+Numtostr(VHFindBX[RightStrength+5],9)
+","+Numtostr(VHFindBX[RightStrength+6],9)
+","+Numtostr(VHFindBX[RightStrength+7],9)
+","+Numtostr(VHFindBX[RightStrength+8],9)
+","+Numtostr(VHFindBX[RightStrength+9],9)
+","+Numtostr(VHFindBX[RightStrength-1],9)
+","+Numtostr(VHFindBX[RightStrength-2],9)
+","+Numtostr(VHFindBX[RightStrength-3],9)
+","+Numtostr(VHFindBX[RightStrength-4],9)
+","+Numtostr(VHFindBXL[RightStrength+0],9)
+","+Numtostr(VHFindBXL[RightStrength+1],9)
+","+Numtostr(VHFindBXL[RightStrength+10],9)
+","+Numtostr(VHFindBXL[RightStrength+11],9)
+","+Numtostr(VHFindBXL[RightStrength+12],9)
+","+Numtostr(VHFindBXL[RightStrength+13],9)
+","+Numtostr(VHFindBXL[RightStrength+14],9)
+","+Numtostr(VHFindBXL[RightStrength+15],9)
+","+Numtostr(VHFindBXL[RightStrength+16],9)
+","+Numtostr(VHFindBXL[RightStrength+17],9)
+","+Numtostr(VHFindBXL[RightStrength+18],9)
+","+Numtostr(VHFindBXL[RightStrength+19],9)
+","+Numtostr(VHFindBXL[RightStrength+2],9)
+","+Numtostr(VHFindBXL[RightStrength+20],9)
+","+Numtostr(VHFindBXL[RightStrength+21],9)
+","+Numtostr(VHFindBXL[RightStrength+22],9)
+","+Numtostr(VHFindBXL[RightStrength+23],9)
+","+Numtostr(VHFindBXL[RightStrength+24],9)
+","+Numtostr(VHFindBXL[RightStrength+25],9)
+","+Numtostr(VHFindBXL[RightStrength+26],9)
+","+Numtostr(VHFindBXL[RightStrength+27],9)
+","+Numtostr(VHFindBXL[RightStrength+28],9)
+","+Numtostr(VHFindBXL[RightStrength+3],9)
+","+Numtostr(VHFindBXL[RightStrength+4],9)
+","+Numtostr(VHFindBXL[RightStrength+5],9)
+","+Numtostr(VHFindBXL[RightStrength+6],9)
+","+Numtostr(VHFindBXL[RightStrength+7],9)
+","+Numtostr(VHFindBXL[RightStrength+8],9)
+","+Numtostr(VHFindBXL[RightStrength+9],9)
+","+Numtostr(VHFindBXL[RightStrength-1],9)
+","+Numtostr(VHFindBXL[RightStrength-2],9)
+","+Numtostr(VHFindBXL[RightStrength-3],9)
+","+Numtostr(VHFindBXL[RightStrength-4],9)
+","+Numtostr(VHFindMED[RightStrength+0],9)
+","+Numtostr(VHFindMED[RightStrength+1],9)
+","+Numtostr(VHFindMED[RightStrength+10],9)
+","+Numtostr(VHFindMED[RightStrength+11],9)
+","+Numtostr(VHFindMED[RightStrength+12],9)
+","+Numtostr(VHFindMED[RightStrength+13],9)
+","+Numtostr(VHFindMED[RightStrength+14],9)
+","+Numtostr(VHFindMED[RightStrength+15],9)
+","+Numtostr(VHFindMED[RightStrength+16],9)
+","+Numtostr(VHFindMED[RightStrength+17],9)
+","+Numtostr(VHFindMED[RightStrength+18],9)
+","+Numtostr(VHFindMED[RightStrength+19],9)
+","+Numtostr(VHFindMED[RightStrength+2],9)
+","+Numtostr(VHFindMED[RightStrength+20],9)
+","+Numtostr(VHFindMED[RightStrength+21],9)
+","+Numtostr(VHFindMED[RightStrength+22],9)
+","+Numtostr(VHFindMED[RightStrength+23],9)
+","+Numtostr(VHFindMED[RightStrength+24],9)
+","+Numtostr(VHFindMED[RightStrength+25],9)
+","+Numtostr(VHFindMED[RightStrength+26],9)
+","+Numtostr(VHFindMED[RightStrength+27],9)
+","+Numtostr(VHFindMED[RightStrength+28],9)
+","+Numtostr(VHFindMED[RightStrength+3],9)
+","+Numtostr(VHFindMED[RightStrength+4],9)
+","+Numtostr(VHFindMED[RightStrength+5],9)
+","+Numtostr(VHFindMED[RightStrength+6],9)
+","+Numtostr(VHFindMED[RightStrength+7],9)
+","+Numtostr(VHFindMED[RightStrength+8],9)
+","+Numtostr(VHFindMED[RightStrength+9],9)
+","+Numtostr(VHFindMED[RightStrength-1],9)
+","+Numtostr(VHFindMED[RightStrength-2],9)
+","+Numtostr(VHFindMED[RightStrength-3],9)
+","+Numtostr(VHFindMED[RightStrength-4],9)
+","+Numtostr(VHFindSMALL[RightStrength+0],9)
+","+Numtostr(VHFindSMALL[RightStrength+1],9)
+","+Numtostr(VHFindSMALL[RightStrength+10],9)
+","+Numtostr(VHFindSMALL[RightStrength+11],9)
+","+Numtostr(VHFindSMALL[RightStrength+12],9)
+","+Numtostr(VHFindSMALL[RightStrength+13],9)
+","+Numtostr(VHFindSMALL[RightStrength+14],9)
+","+Numtostr(VHFindSMALL[RightStrength+15],9)
+","+Numtostr(VHFindSMALL[RightStrength+16],9)
+","+Numtostr(VHFindSMALL[RightStrength+17],9)
+","+Numtostr(VHFindSMALL[RightStrength+18],9)
+","+Numtostr(VHFindSMALL[RightStrength+19],9)
+","+Numtostr(VHFindSMALL[RightStrength+2],9)
+","+Numtostr(VHFindSMALL[RightStrength+20],9)
+","+Numtostr(VHFindSMALL[RightStrength+21],9)
+","+Numtostr(VHFindSMALL[RightStrength+22],9)
+","+Numtostr(VHFindSMALL[RightStrength+23],9)
+","+Numtostr(VHFindSMALL[RightStrength+24],9)
+","+Numtostr(VHFindSMALL[RightStrength+25],9)
+","+Numtostr(VHFindSMALL[RightStrength+26],9)
+","+Numtostr(VHFindSMALL[RightStrength+27],9)
+","+Numtostr(VHFindSMALL[RightStrength+28],9)
+","+Numtostr(VHFindSMALL[RightStrength+3],9)
+","+Numtostr(VHFindSMALL[RightStrength+4],9)
+","+Numtostr(VHFindSMALL[RightStrength+5],9)
+","+Numtostr(VHFindSMALL[RightStrength+6],9)
+","+Numtostr(VHFindSMALL[RightStrength+7],9)
+","+Numtostr(VHFindSMALL[RightStrength+8],9)
+","+Numtostr(VHFindSMALL[RightStrength+9],9)
+","+Numtostr(VHFindSMALL[RightStrength-1],9)
+","+Numtostr(VHFindSMALL[RightStrength-2],9)
+","+Numtostr(VHFindSMALL[RightStrength-3],9)
+","+Numtostr(VHFindSMALL[RightStrength-4],9)
+","+Numtostr(VIDYAindBIG[RightStrength+0],9)
+","+Numtostr(VIDYAindBIG[RightStrength+1],9)
+","+Numtostr(VIDYAindBIG[RightStrength+10],9)
+","+Numtostr(VIDYAindBIG[RightStrength+11],9)
+","+Numtostr(VIDYAindBIG[RightStrength+12],9)
+","+Numtostr(VIDYAindBIG[RightStrength+13],9)
+","+Numtostr(VIDYAindBIG[RightStrength+14],9)
+","+Numtostr(VIDYAindBIG[RightStrength+15],9)
+","+Numtostr(VIDYAindBIG[RightStrength+16],9)
+","+Numtostr(VIDYAindBIG[RightStrength+17],9)
+","+Numtostr(VIDYAindBIG[RightStrength+18],9)
+","+Numtostr(VIDYAindBIG[RightStrength+19],9)
+","+Numtostr(VIDYAindBIG[RightStrength+2],9)
+","+Numtostr(VIDYAindBIG[RightStrength+20],9)
+","+Numtostr(VIDYAindBIG[RightStrength+21],9)
+","+Numtostr(VIDYAindBIG[RightStrength+22],9)
+","+Numtostr(VIDYAindBIG[RightStrength+23],9)
+","+Numtostr(VIDYAindBIG[RightStrength+24],9)
+","+Numtostr(VIDYAindBIG[RightStrength+25],9)
+","+Numtostr(VIDYAindBIG[RightStrength+26],9)
+","+Numtostr(VIDYAindBIG[RightStrength+27],9)
+","+Numtostr(VIDYAindBIG[RightStrength+28],9)
+","+Numtostr(VIDYAindBIG[RightStrength+3],9)
+","+Numtostr(VIDYAindBIG[RightStrength+4],9)
+","+Numtostr(VIDYAindBIG[RightStrength+5],9)
+","+Numtostr(VIDYAindBIG[RightStrength+6],9)
+","+Numtostr(VIDYAindBIG[RightStrength+7],9)
+","+Numtostr(VIDYAindBIG[RightStrength+8],9)
+","+Numtostr(VIDYAindBIG[RightStrength+9],9)
+","+Numtostr(VIDYAindBIG[RightStrength-1],9)
+","+Numtostr(VIDYAindBIG[RightStrength-2],9)
+","+Numtostr(VIDYAindBIG[RightStrength-3],9)
+","+Numtostr(VIDYAindBIG[RightStrength-4],9)
+","+Numtostr(VIDYAindBX[RightStrength+0],9)
+","+Numtostr(VIDYAindBX[RightStrength+1],9)
+","+Numtostr(VIDYAindBX[RightStrength+10],9)
+","+Numtostr(VIDYAindBX[RightStrength+11],9)
+","+Numtostr(VIDYAindBX[RightStrength+12],9)
+","+Numtostr(VIDYAindBX[RightStrength+13],9)
+","+Numtostr(VIDYAindBX[RightStrength+14],9)
+","+Numtostr(VIDYAindBX[RightStrength+15],9)
+","+Numtostr(VIDYAindBX[RightStrength+16],9)
+","+Numtostr(VIDYAindBX[RightStrength+17],9)
+","+Numtostr(VIDYAindBX[RightStrength+18],9)
+","+Numtostr(VIDYAindBX[RightStrength+19],9)
+","+Numtostr(VIDYAindBX[RightStrength+2],9)
+","+Numtostr(VIDYAindBX[RightStrength+20],9)
+","+Numtostr(VIDYAindBX[RightStrength+21],9)
+","+Numtostr(VIDYAindBX[RightStrength+22],9)
+","+Numtostr(VIDYAindBX[RightStrength+23],9)
+","+Numtostr(VIDYAindBX[RightStrength+24],9)
+","+Numtostr(VIDYAindBX[RightStrength+25],9)
+","+Numtostr(VIDYAindBX[RightStrength+26],9)
+","+Numtostr(VIDYAindBX[RightStrength+27],9)
+","+Numtostr(VIDYAindBX[RightStrength+28],9)
+","+Numtostr(VIDYAindBX[RightStrength+3],9)
+","+Numtostr(VIDYAindBX[RightStrength+4],9)
+","+Numtostr(VIDYAindBX[RightStrength+5],9)
+","+Numtostr(VIDYAindBX[RightStrength+6],9)
+","+Numtostr(VIDYAindBX[RightStrength+7],9)
+","+Numtostr(VIDYAindBX[RightStrength+8],9)
+","+Numtostr(VIDYAindBX[RightStrength+9],9)
+","+Numtostr(VIDYAindBX[RightStrength-1],9)
+","+Numtostr(VIDYAindBX[RightStrength-2],9)
+","+Numtostr(VIDYAindBX[RightStrength-3],9)
+","+Numtostr(VIDYAindBX[RightStrength-4],9)
+","+Numtostr(VIDYAindBXL[RightStrength+0],9)
+","+Numtostr(VIDYAindBXL[RightStrength+1],9)
+","+Numtostr(VIDYAindBXL[RightStrength+10],9)
+","+Numtostr(VIDYAindBXL[RightStrength+11],9)
+","+Numtostr(VIDYAindBXL[RightStrength+12],9)
+","+Numtostr(VIDYAindBXL[RightStrength+13],9)
+","+Numtostr(VIDYAindBXL[RightStrength+14],9)
+","+Numtostr(VIDYAindBXL[RightStrength+15],9)
+","+Numtostr(VIDYAindBXL[RightStrength+16],9)
+","+Numtostr(VIDYAindBXL[RightStrength+17],9)
+","+Numtostr(VIDYAindBXL[RightStrength+18],9)
+","+Numtostr(VIDYAindBXL[RightStrength+19],9)
+","+Numtostr(VIDYAindBXL[RightStrength+2],9)
+","+Numtostr(VIDYAindBXL[RightStrength+20],9)
+","+Numtostr(VIDYAindBXL[RightStrength+21],9)
+","+Numtostr(VIDYAindBXL[RightStrength+22],9)
+","+Numtostr(VIDYAindBXL[RightStrength+23],9)
+","+Numtostr(VIDYAindBXL[RightStrength+24],9)
+","+Numtostr(VIDYAindBXL[RightStrength+25],9)
+","+Numtostr(VIDYAindBXL[RightStrength+26],9)
+","+Numtostr(VIDYAindBXL[RightStrength+27],9)
+","+Numtostr(VIDYAindBXL[RightStrength+28],9)
+","+Numtostr(VIDYAindBXL[RightStrength+3],9)
+","+Numtostr(VIDYAindBXL[RightStrength+4],9)
+","+Numtostr(VIDYAindBXL[RightStrength+5],9)
+","+Numtostr(VIDYAindBXL[RightStrength+6],9)
+","+Numtostr(VIDYAindBXL[RightStrength+7],9)
+","+Numtostr(VIDYAindBXL[RightStrength+8],9)
+","+Numtostr(VIDYAindBXL[RightStrength+9],9)
			+" )"; 
			res10 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_feat10);
			srun10 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures10.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_feat10
					+ NewLine);
			end;
		End;
	End;
	 TblNum = TblNum+1;


	//(4) insert feature data into dbo.barFeatures11	
	if ReadWriteToDB and TblNum = 11  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		if  srun11 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_feat11 = "INSERT INTO  [dbo].[barFeatures11] VALUES ("
			+queryResult//MAX(loadId) 
			+","+Numtostr(loadGroupId,0) 
			+","+Numtostr(RightStrength,0)//RightStrength
			+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
			+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"'"+dateBarOpened+"' "+","+"'"+dayOfWeekBarOpened+"' "+","+"'"+dateTimeBarOpened+"' "+","+"'"+timeBarOpenedhhmmss+"' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)
+","+Numtostr(swingHiTrend[RightStrength-4],9)
+","+Numtostr(swingHiTrend[RightStrength-3],9)
+","+Numtostr(swingHiTrend[RightStrength-2],9)
+","+Numtostr(swingHiTrend[RightStrength-1],9)
+","+Numtostr(swingHiTrend[RightStrength+0],9)
+","+Numtostr(swingHiTrend[RightStrength+1],9)
+","+Numtostr(swingHiTrend[RightStrength+2],9)
+","+Numtostr(swingHiTrend[RightStrength+3],9)
+","+Numtostr(swingHiTrend[RightStrength+4],9)
+","+Numtostr(swingHiTrend[RightStrength+5],9)
+","+Numtostr(swingHiTrend[RightStrength+6],9)
+","+Numtostr(swingHiTrend[RightStrength+7],9)
+","+Numtostr(swingHiTrend[RightStrength+8],9)
+","+Numtostr(swingHiTrend[RightStrength+9],9)
+","+Numtostr(swingHiTrend[RightStrength+10],9)
+","+Numtostr(swingHiTrend[RightStrength+11],9)
+","+Numtostr(swingHiTrend[RightStrength+12],9)
+","+Numtostr(swingHiTrend[RightStrength+13],9)
+","+Numtostr(swingHiTrend[RightStrength+14],9)
+","+Numtostr(swingHiTrend[RightStrength+15],9)
+","+Numtostr(swingHiTrend[RightStrength+16],9)
+","+Numtostr(swingHiTrend[RightStrength+17],9)
+","+Numtostr(swingHiTrend[RightStrength+18],9)
+","+Numtostr(swingHiTrend[RightStrength+19],9)
+","+Numtostr(swingHiTrend[RightStrength+20],9)
+","+Numtostr(swingHiTrend[RightStrength+21],9)
+","+Numtostr(swingHiTrend[RightStrength+22],9)
+","+Numtostr(swingHiTrend[RightStrength+23],9)
+","+Numtostr(swingHiTrend[RightStrength+24],9)
+","+Numtostr(swingHiTrend[RightStrength+25],9)
+","+Numtostr(swingHiTrend[RightStrength+26],9)
+","+Numtostr(swingHiTrend[RightStrength+27],9)
+","+Numtostr(swingHiTrend[RightStrength+28],9)
+","+Numtostr(swingLoTrend[RightStrength-4],9)
+","+Numtostr(swingLoTrend[RightStrength-3],9)
+","+Numtostr(swingLoTrend[RightStrength-2],9)
+","+Numtostr(swingLoTrend[RightStrength-1],9)
+","+Numtostr(swingLoTrend[RightStrength+0],9)
+","+Numtostr(swingLoTrend[RightStrength+1],9)
+","+Numtostr(swingLoTrend[RightStrength+2],9)
+","+Numtostr(swingLoTrend[RightStrength+3],9)
+","+Numtostr(swingLoTrend[RightStrength+4],9)
+","+Numtostr(swingLoTrend[RightStrength+5],9)
+","+Numtostr(swingLoTrend[RightStrength+6],9)
+","+Numtostr(swingLoTrend[RightStrength+7],9)
+","+Numtostr(swingLoTrend[RightStrength+8],9)
+","+Numtostr(swingLoTrend[RightStrength+9],9)
+","+Numtostr(swingLoTrend[RightStrength+10],9)
+","+Numtostr(swingLoTrend[RightStrength+11],9)
+","+Numtostr(swingLoTrend[RightStrength+12],9)
+","+Numtostr(swingLoTrend[RightStrength+13],9)
+","+Numtostr(swingLoTrend[RightStrength+14],9)
+","+Numtostr(swingLoTrend[RightStrength+15],9)
+","+Numtostr(swingLoTrend[RightStrength+16],9)
+","+Numtostr(swingLoTrend[RightStrength+17],9)
+","+Numtostr(swingLoTrend[RightStrength+18],9)
+","+Numtostr(swingLoTrend[RightStrength+19],9)
+","+Numtostr(swingLoTrend[RightStrength+20],9)
+","+Numtostr(swingLoTrend[RightStrength+21],9)
+","+Numtostr(swingLoTrend[RightStrength+22],9)
+","+Numtostr(swingLoTrend[RightStrength+23],9)
+","+Numtostr(swingLoTrend[RightStrength+24],9)
+","+Numtostr(swingLoTrend[RightStrength+25],9)
+","+Numtostr(swingLoTrend[RightStrength+26],9)
+","+Numtostr(swingLoTrend[RightStrength+27],9)
+","+Numtostr(swingLoTrend[RightStrength+28],9)
+","+Numtostr(altSwingUpTrend[RightStrength-4],9)
+","+Numtostr(altSwingUpTrend[RightStrength-3],9)
+","+Numtostr(altSwingUpTrend[RightStrength-2],9)
+","+Numtostr(altSwingUpTrend[RightStrength-1],9)
+","+Numtostr(altSwingUpTrend[RightStrength+0],9)
+","+Numtostr(altSwingUpTrend[RightStrength+1],9)
+","+Numtostr(altSwingUpTrend[RightStrength+2],9)
+","+Numtostr(altSwingUpTrend[RightStrength+3],9)
+","+Numtostr(altSwingUpTrend[RightStrength+4],9)
+","+Numtostr(altSwingUpTrend[RightStrength+5],9)
+","+Numtostr(altSwingUpTrend[RightStrength+6],9)
+","+Numtostr(altSwingUpTrend[RightStrength+7],9)
+","+Numtostr(altSwingUpTrend[RightStrength+8],9)
+","+Numtostr(altSwingUpTrend[RightStrength+9],9)
+","+Numtostr(altSwingUpTrend[RightStrength+10],9)
+","+Numtostr(altSwingUpTrend[RightStrength+11],9)
+","+Numtostr(altSwingUpTrend[RightStrength+12],9)
+","+Numtostr(altSwingUpTrend[RightStrength+13],9)
+","+Numtostr(altSwingUpTrend[RightStrength+14],9)
+","+Numtostr(altSwingUpTrend[RightStrength+15],9)
+","+Numtostr(altSwingUpTrend[RightStrength+16],9)
+","+Numtostr(altSwingUpTrend[RightStrength+17],9)
+","+Numtostr(altSwingUpTrend[RightStrength+18],9)
+","+Numtostr(altSwingUpTrend[RightStrength+19],9)
+","+Numtostr(altSwingUpTrend[RightStrength+20],9)
+","+Numtostr(altSwingUpTrend[RightStrength+21],9)
+","+Numtostr(altSwingUpTrend[RightStrength+22],9)
+","+Numtostr(altSwingUpTrend[RightStrength+23],9)
+","+Numtostr(altSwingUpTrend[RightStrength+24],9)
+","+Numtostr(altSwingUpTrend[RightStrength+25],9)
+","+Numtostr(altSwingUpTrend[RightStrength+26],9)
+","+Numtostr(altSwingUpTrend[RightStrength+27],9)
+","+Numtostr(altSwingUpTrend[RightStrength+28],9)
+","+Numtostr(altSwingDnTrend[RightStrength-4],9)
+","+Numtostr(altSwingDnTrend[RightStrength-3],9)
+","+Numtostr(altSwingDnTrend[RightStrength-2],9)
+","+Numtostr(altSwingDnTrend[RightStrength-1],9)
+","+Numtostr(altSwingDnTrend[RightStrength+0],9)
+","+Numtostr(altSwingDnTrend[RightStrength+1],9)
+","+Numtostr(altSwingDnTrend[RightStrength+2],9)
+","+Numtostr(altSwingDnTrend[RightStrength+3],9)
+","+Numtostr(altSwingDnTrend[RightStrength+4],9)
+","+Numtostr(altSwingDnTrend[RightStrength+5],9)
+","+Numtostr(altSwingDnTrend[RightStrength+6],9)
+","+Numtostr(altSwingDnTrend[RightStrength+7],9)
+","+Numtostr(altSwingDnTrend[RightStrength+8],9)
+","+Numtostr(altSwingDnTrend[RightStrength+9],9)
+","+Numtostr(altSwingDnTrend[RightStrength+10],9)
+","+Numtostr(altSwingDnTrend[RightStrength+11],9)
+","+Numtostr(altSwingDnTrend[RightStrength+12],9)
+","+Numtostr(altSwingDnTrend[RightStrength+13],9)
+","+Numtostr(altSwingDnTrend[RightStrength+14],9)
+","+Numtostr(altSwingDnTrend[RightStrength+15],9)
+","+Numtostr(altSwingDnTrend[RightStrength+16],9)
+","+Numtostr(altSwingDnTrend[RightStrength+17],9)
+","+Numtostr(altSwingDnTrend[RightStrength+18],9)
+","+Numtostr(altSwingDnTrend[RightStrength+19],9)
+","+Numtostr(altSwingDnTrend[RightStrength+20],9)
+","+Numtostr(altSwingDnTrend[RightStrength+21],9)
+","+Numtostr(altSwingDnTrend[RightStrength+22],9)
+","+Numtostr(altSwingDnTrend[RightStrength+23],9)
+","+Numtostr(altSwingDnTrend[RightStrength+24],9)
+","+Numtostr(altSwingDnTrend[RightStrength+25],9)
+","+Numtostr(altSwingDnTrend[RightStrength+26],9)
+","+Numtostr(altSwingDnTrend[RightStrength+27],9)
+","+Numtostr(altSwingDnTrend[RightStrength+28],9)
+","+Numtostr(monotonicHiBars[RightStrength-4],9)
+","+Numtostr(monotonicHiBars[RightStrength-3],9)
+","+Numtostr(monotonicHiBars[RightStrength-2],9)
+","+Numtostr(monotonicHiBars[RightStrength-1],9)
+","+Numtostr(monotonicHiBars[RightStrength+0],9)
+","+Numtostr(monotonicHiBars[RightStrength+1],9)
+","+Numtostr(monotonicHiBars[RightStrength+2],9)
+","+Numtostr(monotonicHiBars[RightStrength+3],9)
+","+Numtostr(monotonicHiBars[RightStrength+4],9)
+","+Numtostr(monotonicHiBars[RightStrength+5],9)
+","+Numtostr(monotonicHiBars[RightStrength+6],9)
+","+Numtostr(monotonicHiBars[RightStrength+7],9)
+","+Numtostr(monotonicHiBars[RightStrength+8],9)
+","+Numtostr(monotonicHiBars[RightStrength+9],9)
+","+Numtostr(monotonicHiBars[RightStrength+10],9)
+","+Numtostr(monotonicHiBars[RightStrength+11],9)
+","+Numtostr(monotonicHiBars[RightStrength+12],9)
+","+Numtostr(monotonicHiBars[RightStrength+13],9)
+","+Numtostr(monotonicHiBars[RightStrength+14],9)
+","+Numtostr(monotonicHiBars[RightStrength+15],9)
+","+Numtostr(monotonicHiBars[RightStrength+16],9)
+","+Numtostr(monotonicHiBars[RightStrength+17],9)
+","+Numtostr(monotonicHiBars[RightStrength+18],9)
+","+Numtostr(monotonicHiBars[RightStrength+19],9)
+","+Numtostr(monotonicHiBars[RightStrength+20],9)
+","+Numtostr(monotonicHiBars[RightStrength+21],9)
+","+Numtostr(monotonicHiBars[RightStrength+22],9)
+","+Numtostr(monotonicHiBars[RightStrength+23],9)
+","+Numtostr(monotonicHiBars[RightStrength+24],9)
+","+Numtostr(monotonicHiBars[RightStrength+25],9)
+","+Numtostr(monotonicHiBars[RightStrength+26],9)
+","+Numtostr(monotonicHiBars[RightStrength+27],9)
+","+Numtostr(monotonicHiBars[RightStrength+28],9)
+","+Numtostr(monotonicLoBars[RightStrength-4],9)
+","+Numtostr(monotonicLoBars[RightStrength-3],9)
+","+Numtostr(monotonicLoBars[RightStrength-2],9)
+","+Numtostr(monotonicLoBars[RightStrength-1],9)
+","+Numtostr(monotonicLoBars[RightStrength+0],9)
+","+Numtostr(monotonicLoBars[RightStrength+1],9)
+","+Numtostr(monotonicLoBars[RightStrength+2],9)
+","+Numtostr(monotonicLoBars[RightStrength+3],9)
+","+Numtostr(monotonicLoBars[RightStrength+4],9)
+","+Numtostr(monotonicLoBars[RightStrength+5],9)
+","+Numtostr(monotonicLoBars[RightStrength+6],9)
+","+Numtostr(monotonicLoBars[RightStrength+7],9)
+","+Numtostr(monotonicLoBars[RightStrength+8],9)
+","+Numtostr(monotonicLoBars[RightStrength+9],9)
+","+Numtostr(monotonicLoBars[RightStrength+10],9)
+","+Numtostr(monotonicLoBars[RightStrength+11],9)
+","+Numtostr(monotonicLoBars[RightStrength+12],9)
+","+Numtostr(monotonicLoBars[RightStrength+13],9)
+","+Numtostr(monotonicLoBars[RightStrength+14],9)
+","+Numtostr(monotonicLoBars[RightStrength+15],9)
+","+Numtostr(monotonicLoBars[RightStrength+16],9)
+","+Numtostr(monotonicLoBars[RightStrength+17],9)
+","+Numtostr(monotonicLoBars[RightStrength+18],9)
+","+Numtostr(monotonicLoBars[RightStrength+19],9)
+","+Numtostr(monotonicLoBars[RightStrength+20],9)
+","+Numtostr(monotonicLoBars[RightStrength+21],9)
+","+Numtostr(monotonicLoBars[RightStrength+22],9)
+","+Numtostr(monotonicLoBars[RightStrength+23],9)
+","+Numtostr(monotonicLoBars[RightStrength+24],9)
+","+Numtostr(monotonicLoBars[RightStrength+25],9)
+","+Numtostr(monotonicLoBars[RightStrength+26],9)
+","+Numtostr(monotonicLoBars[RightStrength+27],9)
+","+Numtostr(monotonicLoBars[RightStrength+28],9)
+","+Numtostr(barRange2Bars[RightStrength-4],9)
+","+Numtostr(barRange2Bars[RightStrength-3],9)
+","+Numtostr(barRange2Bars[RightStrength-2],9)
+","+Numtostr(barRange2Bars[RightStrength-1],9)
+","+Numtostr(barRange2Bars[RightStrength+0],9)
+","+Numtostr(barRange2Bars[RightStrength+1],9)
+","+Numtostr(barRange2Bars[RightStrength+2],9)
+","+Numtostr(barRange2Bars[RightStrength+3],9)
+","+Numtostr(barRange2Bars[RightStrength+4],9)
+","+Numtostr(barRange2Bars[RightStrength+5],9)
+","+Numtostr(barRange2Bars[RightStrength+6],9)
+","+Numtostr(barRange2Bars[RightStrength+7],9)
+","+Numtostr(barRange2Bars[RightStrength+8],9)
+","+Numtostr(barRange2Bars[RightStrength+9],9)
+","+Numtostr(barRange2Bars[RightStrength+10],9)
+","+Numtostr(barRange2Bars[RightStrength+11],9)
+","+Numtostr(barRange2Bars[RightStrength+12],9)
+","+Numtostr(barRange2Bars[RightStrength+13],9)
+","+Numtostr(barRange2Bars[RightStrength+14],9)
+","+Numtostr(barRange2Bars[RightStrength+15],9)
+","+Numtostr(barRange2Bars[RightStrength+16],9)
+","+Numtostr(barRange2Bars[RightStrength+17],9)
+","+Numtostr(barRange2Bars[RightStrength+18],9)
+","+Numtostr(barRange2Bars[RightStrength+19],9)
+","+Numtostr(barRange2Bars[RightStrength+20],9)
+","+Numtostr(barRange2Bars[RightStrength+21],9)
+","+Numtostr(barRange2Bars[RightStrength+22],9)
+","+Numtostr(barRange2Bars[RightStrength+23],9)
+","+Numtostr(barRange2Bars[RightStrength+24],9)
+","+Numtostr(barRange2Bars[RightStrength+25],9)
+","+Numtostr(barRange2Bars[RightStrength+26],9)
+","+Numtostr(barRange2Bars[RightStrength+27],9)
+","+Numtostr(barRange2Bars[RightStrength+28],9)
+","+Numtostr(barRange3Bars[RightStrength-4],9)
+","+Numtostr(barRange3Bars[RightStrength-3],9)
+","+Numtostr(barRange3Bars[RightStrength-2],9)
+","+Numtostr(barRange3Bars[RightStrength-1],9)
+","+Numtostr(barRange3Bars[RightStrength+0],9)
+","+Numtostr(barRange3Bars[RightStrength+1],9)
+","+Numtostr(barRange3Bars[RightStrength+2],9)
+","+Numtostr(barRange3Bars[RightStrength+3],9)
+","+Numtostr(barRange3Bars[RightStrength+4],9)
+","+Numtostr(barRange3Bars[RightStrength+5],9)
+","+Numtostr(barRange3Bars[RightStrength+6],9)
+","+Numtostr(barRange3Bars[RightStrength+7],9)
+","+Numtostr(barRange3Bars[RightStrength+8],9)
+","+Numtostr(barRange3Bars[RightStrength+9],9)
+","+Numtostr(barRange3Bars[RightStrength+10],9)
+","+Numtostr(barRange3Bars[RightStrength+11],9)
+","+Numtostr(barRange3Bars[RightStrength+12],9)
+","+Numtostr(barRange3Bars[RightStrength+13],9)
+","+Numtostr(barRange3Bars[RightStrength+14],9)
+","+Numtostr(barRange3Bars[RightStrength+15],9)
+","+Numtostr(barRange3Bars[RightStrength+16],9)
+","+Numtostr(barRange3Bars[RightStrength+17],9)
+","+Numtostr(barRange3Bars[RightStrength+18],9)
+","+Numtostr(barRange3Bars[RightStrength+19],9)
+","+Numtostr(barRange3Bars[RightStrength+20],9)
+","+Numtostr(barRange3Bars[RightStrength+21],9)
+","+Numtostr(barRange3Bars[RightStrength+22],9)
+","+Numtostr(barRange3Bars[RightStrength+23],9)
+","+Numtostr(barRange3Bars[RightStrength+24],9)
+","+Numtostr(barRange3Bars[RightStrength+25],9)
+","+Numtostr(barRange3Bars[RightStrength+26],9)
+","+Numtostr(barRange3Bars[RightStrength+27],9)
+","+Numtostr(barRange3Bars[RightStrength+28],9)
+","+Numtostr(barRange5Bars[RightStrength-4],9)
+","+Numtostr(barRange5Bars[RightStrength-3],9)
+","+Numtostr(barRange5Bars[RightStrength-2],9)
+","+Numtostr(barRange5Bars[RightStrength-1],9)
+","+Numtostr(barRange5Bars[RightStrength+0],9)
+","+Numtostr(barRange5Bars[RightStrength+1],9)
+","+Numtostr(barRange5Bars[RightStrength+2],9)
+","+Numtostr(barRange5Bars[RightStrength+3],9)
+","+Numtostr(barRange5Bars[RightStrength+4],9)
+","+Numtostr(barRange5Bars[RightStrength+5],9)
+","+Numtostr(barRange5Bars[RightStrength+6],9)
+","+Numtostr(barRange5Bars[RightStrength+7],9)
+","+Numtostr(barRange5Bars[RightStrength+8],9)
+","+Numtostr(barRange5Bars[RightStrength+9],9)
+","+Numtostr(barRange5Bars[RightStrength+10],9)
+","+Numtostr(barRange5Bars[RightStrength+11],9)
+","+Numtostr(barRange5Bars[RightStrength+12],9)
+","+Numtostr(barRange5Bars[RightStrength+13],9)
+","+Numtostr(barRange5Bars[RightStrength+14],9)
+","+Numtostr(barRange5Bars[RightStrength+15],9)
+","+Numtostr(barRange5Bars[RightStrength+16],9)
+","+Numtostr(barRange5Bars[RightStrength+17],9)
+","+Numtostr(barRange5Bars[RightStrength+18],9)
+","+Numtostr(barRange5Bars[RightStrength+19],9)
+","+Numtostr(barRange5Bars[RightStrength+20],9)
+","+Numtostr(barRange5Bars[RightStrength+21],9)
+","+Numtostr(barRange5Bars[RightStrength+22],9)
+","+Numtostr(barRange5Bars[RightStrength+23],9)
+","+Numtostr(barRange5Bars[RightStrength+24],9)
+","+Numtostr(barRange5Bars[RightStrength+25],9)
+","+Numtostr(barRange5Bars[RightStrength+26],9)
+","+Numtostr(barRange5Bars[RightStrength+27],9)
+","+Numtostr(barRange5Bars[RightStrength+28],9)
+","+Numtostr(barRange10Bars[RightStrength-4],9)
+","+Numtostr(barRange10Bars[RightStrength-3],9)
+","+Numtostr(barRange10Bars[RightStrength-2],9)
+","+Numtostr(barRange10Bars[RightStrength-1],9)
+","+Numtostr(barRange10Bars[RightStrength+0],9)
+","+Numtostr(barRange10Bars[RightStrength+1],9)
+","+Numtostr(barRange10Bars[RightStrength+2],9)
+","+Numtostr(barRange10Bars[RightStrength+3],9)
+","+Numtostr(barRange10Bars[RightStrength+4],9)
+","+Numtostr(barRange10Bars[RightStrength+5],9)
+","+Numtostr(barRange10Bars[RightStrength+6],9)
+","+Numtostr(barRange10Bars[RightStrength+7],9)
+","+Numtostr(barRange10Bars[RightStrength+8],9)
+","+Numtostr(barRange10Bars[RightStrength+9],9)
+","+Numtostr(barRange10Bars[RightStrength+10],9)
+","+Numtostr(barRange10Bars[RightStrength+11],9)
+","+Numtostr(barRange10Bars[RightStrength+12],9)
+","+Numtostr(barRange10Bars[RightStrength+13],9)
+","+Numtostr(barRange10Bars[RightStrength+14],9)
+","+Numtostr(barRange10Bars[RightStrength+15],9)
+","+Numtostr(barRange10Bars[RightStrength+16],9)
+","+Numtostr(barRange10Bars[RightStrength+17],9)
+","+Numtostr(barRange10Bars[RightStrength+18],9)
+","+Numtostr(barRange10Bars[RightStrength+19],9)
+","+Numtostr(barRange10Bars[RightStrength+20],9)
+","+Numtostr(barRange10Bars[RightStrength+21],9)
+","+Numtostr(barRange10Bars[RightStrength+22],9)
+","+Numtostr(barRange10Bars[RightStrength+23],9)
+","+Numtostr(barRange10Bars[RightStrength+24],9)
+","+Numtostr(barRange10Bars[RightStrength+25],9)
+","+Numtostr(barRange10Bars[RightStrength+26],9)
+","+Numtostr(barRange10Bars[RightStrength+27],9)
+","+Numtostr(barRange10Bars[RightStrength+28],9)
+","+Numtostr(barRange20Bars[RightStrength-4],9)
+","+Numtostr(barRange20Bars[RightStrength-3],9)
+","+Numtostr(barRange20Bars[RightStrength-2],9)
+","+Numtostr(barRange20Bars[RightStrength-1],9)
+","+Numtostr(barRange20Bars[RightStrength+0],9)
+","+Numtostr(barRange20Bars[RightStrength+1],9)
+","+Numtostr(barRange20Bars[RightStrength+2],9)
+","+Numtostr(barRange20Bars[RightStrength+3],9)
+","+Numtostr(barRange20Bars[RightStrength+4],9)
+","+Numtostr(barRange20Bars[RightStrength+5],9)
+","+Numtostr(barRange20Bars[RightStrength+6],9)
+","+Numtostr(barRange20Bars[RightStrength+7],9)
+","+Numtostr(barRange20Bars[RightStrength+8],9)
+","+Numtostr(barRange20Bars[RightStrength+9],9)
+","+Numtostr(barRange20Bars[RightStrength+10],9)
+","+Numtostr(barRange20Bars[RightStrength+11],9)
+","+Numtostr(barRange20Bars[RightStrength+12],9)
+","+Numtostr(barRange20Bars[RightStrength+13],9)
+","+Numtostr(barRange20Bars[RightStrength+14],9)
+","+Numtostr(barRange20Bars[RightStrength+15],9)
+","+Numtostr(barRange20Bars[RightStrength+16],9)
+","+Numtostr(barRange20Bars[RightStrength+17],9)
+","+Numtostr(barRange20Bars[RightStrength+18],9)
+","+Numtostr(barRange20Bars[RightStrength+19],9)
+","+Numtostr(barRange20Bars[RightStrength+20],9)
+","+Numtostr(barRange20Bars[RightStrength+21],9)
+","+Numtostr(barRange20Bars[RightStrength+22],9)
+","+Numtostr(barRange20Bars[RightStrength+23],9)
+","+Numtostr(barRange20Bars[RightStrength+24],9)
+","+Numtostr(barRange20Bars[RightStrength+25],9)
+","+Numtostr(barRange20Bars[RightStrength+26],9)
+","+Numtostr(barRange20Bars[RightStrength+27],9)
+","+Numtostr(barRange20Bars[RightStrength+28],9)
+","+Numtostr(barRange40Bars[RightStrength-4],9)
+","+Numtostr(barRange40Bars[RightStrength-3],9)
+","+Numtostr(barRange40Bars[RightStrength-2],9)
+","+Numtostr(barRange40Bars[RightStrength-1],9)
+","+Numtostr(barRange40Bars[RightStrength+0],9)
+","+Numtostr(barRange40Bars[RightStrength+1],9)
+","+Numtostr(barRange40Bars[RightStrength+2],9)
+","+Numtostr(barRange40Bars[RightStrength+3],9)
+","+Numtostr(barRange40Bars[RightStrength+4],9)
+","+Numtostr(barRange40Bars[RightStrength+5],9)
+","+Numtostr(barRange40Bars[RightStrength+6],9)
+","+Numtostr(barRange40Bars[RightStrength+7],9)
+","+Numtostr(barRange40Bars[RightStrength+8],9)
+","+Numtostr(barRange40Bars[RightStrength+9],9)
+","+Numtostr(barRange40Bars[RightStrength+10],9)
+","+Numtostr(barRange40Bars[RightStrength+11],9)
+","+Numtostr(barRange40Bars[RightStrength+12],9)
+","+Numtostr(barRange40Bars[RightStrength+13],9)
+","+Numtostr(barRange40Bars[RightStrength+14],9)
+","+Numtostr(barRange40Bars[RightStrength+15],9)
+","+Numtostr(barRange40Bars[RightStrength+16],9)
+","+Numtostr(barRange40Bars[RightStrength+17],9)
+","+Numtostr(barRange40Bars[RightStrength+18],9)
+","+Numtostr(barRange40Bars[RightStrength+19],9)
+","+Numtostr(barRange40Bars[RightStrength+20],9)
+","+Numtostr(barRange40Bars[RightStrength+21],9)
+","+Numtostr(barRange40Bars[RightStrength+22],9)
+","+Numtostr(barRange40Bars[RightStrength+23],9)
+","+Numtostr(barRange40Bars[RightStrength+24],9)
+","+Numtostr(barRange40Bars[RightStrength+25],9)
+","+Numtostr(barRange40Bars[RightStrength+26],9)
+","+Numtostr(barRange40Bars[RightStrength+27],9)
+","+Numtostr(barRange40Bars[RightStrength+28],9)
+","+Numtostr(barRange80Bars[RightStrength-4],9)
+","+Numtostr(barRange80Bars[RightStrength-3],9)
+","+Numtostr(barRange80Bars[RightStrength-2],9)
+","+Numtostr(barRange80Bars[RightStrength-1],9)
+","+Numtostr(barRange80Bars[RightStrength+0],9)
+","+Numtostr(barRange80Bars[RightStrength+1],9)
+","+Numtostr(barRange80Bars[RightStrength+2],9)
+","+Numtostr(barRange80Bars[RightStrength+3],9)
+","+Numtostr(barRange80Bars[RightStrength+4],9)
+","+Numtostr(barRange80Bars[RightStrength+5],9)
+","+Numtostr(barRange80Bars[RightStrength+6],9)
+","+Numtostr(barRange80Bars[RightStrength+7],9)
+","+Numtostr(barRange80Bars[RightStrength+8],9)
+","+Numtostr(barRange80Bars[RightStrength+9],9)
+","+Numtostr(barRange80Bars[RightStrength+10],9)
+","+Numtostr(barRange80Bars[RightStrength+11],9)
+","+Numtostr(barRange80Bars[RightStrength+12],9)
+","+Numtostr(barRange80Bars[RightStrength+13],9)
+","+Numtostr(barRange80Bars[RightStrength+14],9)
+","+Numtostr(barRange80Bars[RightStrength+15],9)
+","+Numtostr(barRange80Bars[RightStrength+16],9)
+","+Numtostr(barRange80Bars[RightStrength+17],9)
+","+Numtostr(barRange80Bars[RightStrength+18],9)
+","+Numtostr(barRange80Bars[RightStrength+19],9)
+","+Numtostr(barRange80Bars[RightStrength+20],9)
+","+Numtostr(barRange80Bars[RightStrength+21],9)
+","+Numtostr(barRange80Bars[RightStrength+22],9)
+","+Numtostr(barRange80Bars[RightStrength+23],9)
+","+Numtostr(barRange80Bars[RightStrength+24],9)
+","+Numtostr(barRange80Bars[RightStrength+25],9)
+","+Numtostr(barRange80Bars[RightStrength+26],9)
+","+Numtostr(barRange80Bars[RightStrength+27],9)
+","+Numtostr(barRange80Bars[RightStrength+28],9)
+","+Numtostr(barRange120Bars[RightStrength-4],9)
+","+Numtostr(barRange120Bars[RightStrength-3],9)
+","+Numtostr(barRange120Bars[RightStrength-2],9)
+","+Numtostr(barRange120Bars[RightStrength-1],9)
+","+Numtostr(barRange120Bars[RightStrength+0],9)
+","+Numtostr(barRange120Bars[RightStrength+1],9)
+","+Numtostr(barRange120Bars[RightStrength+2],9)
+","+Numtostr(barRange120Bars[RightStrength+3],9)
+","+Numtostr(barRange120Bars[RightStrength+4],9)
+","+Numtostr(barRange120Bars[RightStrength+5],9)
+","+Numtostr(barRange120Bars[RightStrength+6],9)
+","+Numtostr(barRange120Bars[RightStrength+7],9)
+","+Numtostr(barRange120Bars[RightStrength+8],9)
+","+Numtostr(barRange120Bars[RightStrength+9],9)
+","+Numtostr(barRange120Bars[RightStrength+10],9)
+","+Numtostr(barRange120Bars[RightStrength+11],9)
+","+Numtostr(barRange120Bars[RightStrength+12],9)
+","+Numtostr(barRange120Bars[RightStrength+13],9)
+","+Numtostr(barRange120Bars[RightStrength+14],9)
+","+Numtostr(barRange120Bars[RightStrength+15],9)
+","+Numtostr(barRange120Bars[RightStrength+16],9)
+","+Numtostr(barRange120Bars[RightStrength+17],9)
+","+Numtostr(barRange120Bars[RightStrength+18],9)
+","+Numtostr(barRange120Bars[RightStrength+19],9)
+","+Numtostr(barRange120Bars[RightStrength+20],9)
+","+Numtostr(barRange120Bars[RightStrength+21],9)
+","+Numtostr(barRange120Bars[RightStrength+22],9)
+","+Numtostr(barRange120Bars[RightStrength+23],9)
+","+Numtostr(barRange120Bars[RightStrength+24],9)
+","+Numtostr(barRange120Bars[RightStrength+25],9)
+","+Numtostr(barRange120Bars[RightStrength+26],9)
+","+Numtostr(barRange120Bars[RightStrength+27],9)
+","+Numtostr(barRange120Bars[RightStrength+28],9)
+","+Numtostr(barRange185Bars[RightStrength-4],9)
+","+Numtostr(barRange185Bars[RightStrength-3],9)
+","+Numtostr(barRange185Bars[RightStrength-2],9)
+","+Numtostr(barRange185Bars[RightStrength-1],9)
+","+Numtostr(barRange185Bars[RightStrength+0],9)
+","+Numtostr(barRange185Bars[RightStrength+1],9)
+","+Numtostr(barRange185Bars[RightStrength+2],9)
+","+Numtostr(barRange185Bars[RightStrength+3],9)
+","+Numtostr(barRange185Bars[RightStrength+4],9)
+","+Numtostr(barRange185Bars[RightStrength+5],9)
+","+Numtostr(barRange185Bars[RightStrength+6],9)
+","+Numtostr(barRange185Bars[RightStrength+7],9)
+","+Numtostr(barRange185Bars[RightStrength+8],9)
+","+Numtostr(barRange185Bars[RightStrength+9],9)
+","+Numtostr(barRange185Bars[RightStrength+10],9)
+","+Numtostr(barRange185Bars[RightStrength+11],9)
+","+Numtostr(barRange185Bars[RightStrength+12],9)
+","+Numtostr(barRange185Bars[RightStrength+13],9)
+","+Numtostr(barRange185Bars[RightStrength+14],9)
+","+Numtostr(barRange185Bars[RightStrength+15],9)
+","+Numtostr(barRange185Bars[RightStrength+16],9)
+","+Numtostr(barRange185Bars[RightStrength+17],9)
+","+Numtostr(barRange185Bars[RightStrength+18],9)
+","+Numtostr(barRange185Bars[RightStrength+19],9)
+","+Numtostr(barRange185Bars[RightStrength+20],9)
+","+Numtostr(barRange185Bars[RightStrength+21],9)
+","+Numtostr(barRange185Bars[RightStrength+22],9)
+","+Numtostr(barRange185Bars[RightStrength+23],9)
+","+Numtostr(barRange185Bars[RightStrength+24],9)
+","+Numtostr(barRange185Bars[RightStrength+25],9)
+","+Numtostr(barRange185Bars[RightStrength+26],9)
+","+Numtostr(barRange185Bars[RightStrength+27],9)
+","+Numtostr(barRange185Bars[RightStrength+28],9)
+","+Numtostr(barRange250Bars[RightStrength-4],9)
+","+Numtostr(barRange250Bars[RightStrength-3],9)
+","+Numtostr(barRange250Bars[RightStrength-2],9)
+","+Numtostr(barRange250Bars[RightStrength-1],9)
+","+Numtostr(barRange250Bars[RightStrength+0],9)
+","+Numtostr(barRange250Bars[RightStrength+1],9)
+","+Numtostr(barRange250Bars[RightStrength+2],9)
+","+Numtostr(barRange250Bars[RightStrength+3],9)
+","+Numtostr(barRange250Bars[RightStrength+4],9)
+","+Numtostr(barRange250Bars[RightStrength+5],9)
+","+Numtostr(barRange250Bars[RightStrength+6],9)
+","+Numtostr(barRange250Bars[RightStrength+7],9)
+","+Numtostr(barRange250Bars[RightStrength+8],9)
+","+Numtostr(barRange250Bars[RightStrength+9],9)
+","+Numtostr(barRange250Bars[RightStrength+10],9)
+","+Numtostr(barRange250Bars[RightStrength+11],9)
+","+Numtostr(barRange250Bars[RightStrength+12],9)
+","+Numtostr(barRange250Bars[RightStrength+13],9)
+","+Numtostr(barRange250Bars[RightStrength+14],9)
+","+Numtostr(barRange250Bars[RightStrength+15],9)
+","+Numtostr(barRange250Bars[RightStrength+16],9)
+","+Numtostr(barRange250Bars[RightStrength+17],9)
+","+Numtostr(barRange250Bars[RightStrength+18],9)
+","+Numtostr(barRange250Bars[RightStrength+19],9)
+","+Numtostr(barRange250Bars[RightStrength+20],9)
+","+Numtostr(barRange250Bars[RightStrength+21],9)
+","+Numtostr(barRange250Bars[RightStrength+22],9)
+","+Numtostr(barRange250Bars[RightStrength+23],9)
+","+Numtostr(barRange250Bars[RightStrength+24],9)
+","+Numtostr(barRange250Bars[RightStrength+25],9)
+","+Numtostr(barRange250Bars[RightStrength+26],9)
+","+Numtostr(barRange250Bars[RightStrength+27],9)
+","+Numtostr(barRange250Bars[RightStrength+28],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength-4],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength-3],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength-2],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength-1],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+0],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+1],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+2],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+3],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+4],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+5],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+6],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+7],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+8],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+9],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+10],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+11],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+12],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+13],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+14],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+15],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+16],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+17],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+18],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+19],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+20],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+21],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+22],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+23],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+24],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+25],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+26],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+27],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+28],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength-4],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength-3],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength-2],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength-1],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+0],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+1],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+2],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+3],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+4],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+5],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+6],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+7],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+8],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+9],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+10],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+11],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+12],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+13],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+14],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+15],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+16],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+17],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+18],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+19],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+20],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+21],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+22],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+23],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+24],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+25],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+26],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+27],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+28],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength-4],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength-3],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength-2],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength-1],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+0],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+1],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+2],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+3],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+4],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+5],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+6],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+7],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+8],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+9],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+10],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+11],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+12],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+13],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+14],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+15],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+16],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+17],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+18],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+19],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+20],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+21],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+22],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+23],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+24],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+25],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+26],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+27],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+28],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength-4],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength-3],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength-2],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength-1],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+0],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+1],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+2],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+3],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+4],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+5],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+6],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+7],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+8],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+9],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+10],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+11],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+12],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+13],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+14],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+15],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+16],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+17],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+18],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+19],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+20],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+21],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+22],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+23],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+24],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+25],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+26],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+27],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+28],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength-4],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength-3],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength-2],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength-1],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+0],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+1],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+2],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+3],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+4],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+5],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+6],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+7],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+8],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+9],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+10],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+11],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+12],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+13],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+14],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+15],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+16],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+17],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+18],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+19],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+20],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+21],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+22],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+23],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+24],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+25],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+26],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+27],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+28],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength-4],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength-3],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength-2],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength-1],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+0],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+1],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+2],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+3],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+4],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+5],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+6],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+7],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+8],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+9],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+10],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+11],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+12],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+13],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+14],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+15],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+16],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+17],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+18],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+19],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+20],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+21],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+22],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+23],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+24],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+25],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+26],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+27],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+28],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength-4],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength-3],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength-2],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength-1],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+0],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+1],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+2],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+3],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+4],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+5],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+6],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+7],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+8],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+9],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+10],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+11],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+12],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+13],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+14],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+15],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+16],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+17],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+18],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+19],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+20],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+21],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+22],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+23],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+24],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+25],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+26],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+27],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+28],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength-4],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength-3],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength-2],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength-1],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+0],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+1],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+2],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+3],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+4],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+5],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+6],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+7],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+8],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+9],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+10],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+11],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+12],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+13],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+14],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+15],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+16],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+17],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+18],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+19],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+20],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+21],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+22],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+23],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+24],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+25],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+26],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+27],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+28],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength-4],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength-3],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength-2],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength-1],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+0],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+1],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+2],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+3],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+4],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+5],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+6],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+7],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+8],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+9],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+10],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+11],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+12],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+13],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+14],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+15],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+16],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+17],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+18],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+19],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+20],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+21],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+22],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+23],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+24],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+25],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+26],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+27],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+28],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength-4],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength-3],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength-2],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength-1],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+0],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+1],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+2],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+3],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+4],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+5],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+6],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+7],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+8],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+9],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+10],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+11],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+12],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+13],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+14],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+15],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+16],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+17],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+18],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+19],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+20],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+21],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+22],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+23],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+24],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+25],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+26],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+27],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+28],9)
			+" )"; 
			res11 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_feat11);
			srun11 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures11.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_feat11
					+ NewLine);
			end;
		End;
	End;
	 TblNum = TblNum+1;
	 
	 
	 
	//(4) insert feature data into dbo.barFeatures1v02	
	if ReadWriteToDB and TblNum = 11+1  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		if  srun1v02 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_feat1v02 = "INSERT INTO  [dbo].[barFeatures1v02] VALUES ("
			+queryResult//MAX(loadId) 
			+","+Numtostr(loadGroupId,0) 
			+","+Numtostr(RightStrength,0)//RightStrength
			+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
			+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"'"+dateBarOpened+"' "+","+"'"+dayOfWeekBarOpened+"' "+","+"'"+dateTimeBarOpened+"' "+","+"'"+timeBarOpenedhhmmss+"' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)
+","+Numtostr(ATRindBIG[RightStrength+13],9)
+","+Numtostr(ATRindBIG[RightStrength+14],9)
+","+Numtostr(ATRindBIG[RightStrength+15],9)
+","+Numtostr(ATRindBIG[RightStrength+16],9)
+","+Numtostr(ATRindBIG[RightStrength+17],9)
+","+Numtostr(ATRindBIG[RightStrength+18],9)
+","+Numtostr(ATRindBIG[RightStrength+19],9)
+","+Numtostr(ATRindBIG[RightStrength+2],9)
+","+Numtostr(ATRindBIG[RightStrength+20],9)
+","+Numtostr(ATRindBIG[RightStrength+21],9)
+","+Numtostr(ATRindBIG[RightStrength+22],9)
+","+Numtostr(ATRindBIG[RightStrength+23],9)
+","+Numtostr(ATRindBIG[RightStrength+24],9)
+","+Numtostr(ATRindBIG[RightStrength+25],9)
+","+Numtostr(ATRindBIG[RightStrength+26],9)
+","+Numtostr(ATRindBIG[RightStrength+27],9)
+","+Numtostr(ATRindBIG[RightStrength+28],9)
+","+Numtostr(ATRindBIG[RightStrength+3],9)
+","+Numtostr(ATRindBIG[RightStrength+4],9)
+","+Numtostr(ATRindBIG[RightStrength+5],9)
+","+Numtostr(ATRindBIG[RightStrength+6],9)
+","+Numtostr(ATRindBIG[RightStrength+7],9)
+","+Numtostr(ATRindBIG[RightStrength+8],9)
+","+Numtostr(ATRindBIG[RightStrength+9],9)
+","+Numtostr(ATRindBIG[RightStrength-1],9)
+","+Numtostr(ATRindBIG[RightStrength-2],9)
+","+Numtostr(ATRindBIG[RightStrength-3],9)
+","+Numtostr(ATRindBIG[RightStrength-4],9)
+","+Numtostr(ATRindBX[RightStrength+0],9)
+","+Numtostr(ATRindBX[RightStrength+1],9)
+","+Numtostr(ATRindBX[RightStrength+10],9)
+","+Numtostr(ATRindBX[RightStrength+11],9)
+","+Numtostr(ATRindBX[RightStrength+12],9)
+","+Numtostr(ATRindBX[RightStrength+13],9)
+","+Numtostr(ATRindBX[RightStrength+14],9)
+","+Numtostr(ATRindBX[RightStrength+15],9)
+","+Numtostr(ATRindBX[RightStrength+16],9)
+","+Numtostr(ATRindBX[RightStrength+17],9)
+","+Numtostr(ATRindBX[RightStrength+18],9)
+","+Numtostr(ATRindBX[RightStrength+19],9)
+","+Numtostr(ATRindBX[RightStrength+2],9)
+","+Numtostr(ATRindBX[RightStrength+20],9)
+","+Numtostr(ATRindBX[RightStrength+21],9)
+","+Numtostr(ATRindBX[RightStrength+22],9)
+","+Numtostr(ATRindBX[RightStrength+23],9)
+","+Numtostr(ATRindBX[RightStrength+24],9)
+","+Numtostr(ATRindBX[RightStrength+25],9)
+","+Numtostr(ATRindBX[RightStrength+26],9)
+","+Numtostr(ATRindBX[RightStrength+27],9)
+","+Numtostr(ATRindBX[RightStrength+28],9)
+","+Numtostr(ATRindBX[RightStrength+3],9)
+","+Numtostr(ATRindBX[RightStrength+4],9)
+","+Numtostr(ATRindBX[RightStrength+5],9)
+","+Numtostr(ATRindBX[RightStrength+6],9)
+","+Numtostr(ATRindBX[RightStrength+7],9)
+","+Numtostr(ATRindBX[RightStrength+8],9)
+","+Numtostr(ATRindBX[RightStrength+9],9)
+","+Numtostr(ATRindBX[RightStrength-1],9)
+","+Numtostr(ATRindBX[RightStrength-2],9)
+","+Numtostr(ATRindBX[RightStrength-3],9)
+","+Numtostr(ATRindBX[RightStrength-4],9)
+","+Numtostr(ATRindBXL[RightStrength+0],9)
+","+Numtostr(ATRindBXL[RightStrength+1],9)
+","+Numtostr(ATRindBXL[RightStrength+10],9)
+","+Numtostr(ATRindBXL[RightStrength+11],9)
+","+Numtostr(ATRindBXL[RightStrength+12],9)
+","+Numtostr(ATRindBXL[RightStrength+13],9)
+","+Numtostr(ATRindBXL[RightStrength+14],9)
+","+Numtostr(ATRindBXL[RightStrength+15],9)
+","+Numtostr(ATRindBXL[RightStrength+16],9)
+","+Numtostr(ATRindBXL[RightStrength+17],9)
+","+Numtostr(ATRindBXL[RightStrength+18],9)
+","+Numtostr(ATRindBXL[RightStrength+19],9)
+","+Numtostr(ATRindBXL[RightStrength+2],9)
+","+Numtostr(ATRindBXL[RightStrength+20],9)
+","+Numtostr(ATRindBXL[RightStrength+21],9)
+","+Numtostr(ATRindBXL[RightStrength+22],9)
+","+Numtostr(ATRindBXL[RightStrength+23],9)
+","+Numtostr(ATRindBXL[RightStrength+24],9)
+","+Numtostr(ATRindBXL[RightStrength+25],9)
+","+Numtostr(ATRindBXL[RightStrength+26],9)
+","+Numtostr(ATRindBXL[RightStrength+27],9)
+","+Numtostr(ATRindBXL[RightStrength+28],9)
+","+Numtostr(ATRindBXL[RightStrength+3],9)
+","+Numtostr(ATRindBXL[RightStrength+4],9)
+","+Numtostr(ATRindBXL[RightStrength+5],9)
+","+Numtostr(ATRindBXL[RightStrength+6],9)
+","+Numtostr(ATRindBXL[RightStrength+7],9)
+","+Numtostr(ATRindBXL[RightStrength+8],9)
+","+Numtostr(ATRindBXL[RightStrength+9],9)
+","+Numtostr(ATRindBXL[RightStrength-1],9)
+","+Numtostr(ATRindBXL[RightStrength-2],9)
+","+Numtostr(ATRindBXL[RightStrength-3],9)
+","+Numtostr(ATRindBXL[RightStrength-4],9)
+","+Numtostr(ATRindMED[RightStrength+0],9)
+","+Numtostr(ATRindMED[RightStrength+1],9)
+","+Numtostr(ATRindMED[RightStrength+10],9)
+","+Numtostr(ATRindMED[RightStrength+11],9)
+","+Numtostr(ATRindMED[RightStrength+12],9)
+","+Numtostr(ATRindMED[RightStrength+13],9)
+","+Numtostr(ATRindMED[RightStrength+14],9)
+","+Numtostr(ATRindMED[RightStrength+15],9)
+","+Numtostr(ATRindMED[RightStrength+16],9)
+","+Numtostr(ATRindMED[RightStrength+17],9)
+","+Numtostr(ATRindMED[RightStrength+18],9)
+","+Numtostr(ATRindMED[RightStrength+19],9)
+","+Numtostr(ATRindMED[RightStrength+2],9)
+","+Numtostr(ATRindMED[RightStrength+20],9)
+","+Numtostr(ATRindMED[RightStrength+21],9)
+","+Numtostr(ATRindMED[RightStrength+22],9)
+","+Numtostr(ATRindMED[RightStrength+23],9)
+","+Numtostr(ATRindMED[RightStrength+24],9)
+","+Numtostr(ATRindMED[RightStrength+25],9)
+","+Numtostr(ATRindMED[RightStrength+26],9)
+","+Numtostr(ATRindMED[RightStrength+27],9)
+","+Numtostr(ATRindMED[RightStrength+28],9)
+","+Numtostr(ATRindMED[RightStrength+3],9)
+","+Numtostr(ATRindMED[RightStrength+4],9)
+","+Numtostr(ATRindMED[RightStrength+5],9)
+","+Numtostr(ATRindMED[RightStrength+6],9)
+","+Numtostr(ATRindMED[RightStrength+7],9)
+","+Numtostr(ATRindMED[RightStrength+8],9)
+","+Numtostr(ATRindMED[RightStrength+9],9)
+","+Numtostr(ATRindMED[RightStrength-1],9)
+","+Numtostr(ATRindMED[RightStrength-2],9)
+","+Numtostr(ATRindMED[RightStrength-3],9)
+","+Numtostr(ATRindMED[RightStrength-4],9)
+","+Numtostr(ATRindSMALL[RightStrength+0],9)
+","+Numtostr(ATRindSMALL[RightStrength+1],9)
+","+Numtostr(ATRindSMALL[RightStrength+10],9)
+","+Numtostr(ATRindSMALL[RightStrength+11],9)
+","+Numtostr(ATRindSMALL[RightStrength+12],9)
+","+Numtostr(ATRindSMALL[RightStrength+13],9)
+","+Numtostr(ATRindSMALL[RightStrength+14],9)
+","+Numtostr(ATRindSMALL[RightStrength+15],9)
+","+Numtostr(ATRindSMALL[RightStrength+16],9)
+","+Numtostr(ATRindSMALL[RightStrength+17],9)
+","+Numtostr(ATRindSMALL[RightStrength+18],9)
+","+Numtostr(ATRindSMALL[RightStrength+19],9)
+","+Numtostr(ATRindSMALL[RightStrength+2],9)
+","+Numtostr(ATRindSMALL[RightStrength+20],9)
+","+Numtostr(ATRindSMALL[RightStrength+21],9)
+","+Numtostr(ATRindSMALL[RightStrength+22],9)
+","+Numtostr(ATRindSMALL[RightStrength+23],9)
+","+Numtostr(ATRindSMALL[RightStrength+24],9)
+","+Numtostr(ATRindSMALL[RightStrength+25],9)
+","+Numtostr(ATRindSMALL[RightStrength+26],9)
+","+Numtostr(ATRindSMALL[RightStrength+27],9)
+","+Numtostr(ATRindSMALL[RightStrength+28],9)
+","+Numtostr(ATRindSMALL[RightStrength+3],9)
+","+Numtostr(ATRindSMALL[RightStrength+4],9)
+","+Numtostr(ATRindSMALL[RightStrength+5],9)
+","+Numtostr(ATRindSMALL[RightStrength+6],9)
+","+Numtostr(ATRindSMALL[RightStrength+7],9)
+","+Numtostr(ATRindSMALL[RightStrength+8],9)
+","+Numtostr(ATRindSMALL[RightStrength+9],9)
+","+Numtostr(ATRindSMALL[RightStrength-1],9)
+","+Numtostr(ATRindSMALL[RightStrength-2],9)
+","+Numtostr(ATRindSMALL[RightStrength-3],9)
+","+Numtostr(ATRindSMALL[RightStrength-4],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+0],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+1],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+10],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+11],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+12],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+13],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+14],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+15],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+16],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+17],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+18],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+19],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+2],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+20],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+21],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+22],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+23],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+24],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+25],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+26],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+27],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+28],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+3],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+4],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+5],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+6],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+7],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+8],9)
+","+Numtostr(AWEOSCindBIG[RightStrength+9],9)
+","+Numtostr(AWEOSCindBIG[RightStrength-1],9)
+","+Numtostr(AWEOSCindBIG[RightStrength-2],9)
+","+Numtostr(AWEOSCindBIG[RightStrength-3],9)
+","+Numtostr(AWEOSCindBIG[RightStrength-4],9)
+","+Numtostr(AWEOSCindBX[RightStrength+0],9)
+","+Numtostr(AWEOSCindBX[RightStrength+1],9)
+","+Numtostr(AWEOSCindBX[RightStrength+10],9)
+","+Numtostr(AWEOSCindBX[RightStrength+11],9)
+","+Numtostr(AWEOSCindBX[RightStrength+12],9)
+","+Numtostr(AWEOSCindBX[RightStrength+13],9)
+","+Numtostr(AWEOSCindBX[RightStrength+14],9)
+","+Numtostr(AWEOSCindBX[RightStrength+15],9)
+","+Numtostr(AWEOSCindBX[RightStrength+16],9)
+","+Numtostr(AWEOSCindBX[RightStrength+17],9)
+","+Numtostr(AWEOSCindBX[RightStrength+18],9)
+","+Numtostr(AWEOSCindBX[RightStrength+19],9)
+","+Numtostr(AWEOSCindBX[RightStrength+2],9)
+","+Numtostr(AWEOSCindBX[RightStrength+20],9)
+","+Numtostr(AWEOSCindBX[RightStrength+21],9)
+","+Numtostr(AWEOSCindBX[RightStrength+22],9)
+","+Numtostr(AWEOSCindBX[RightStrength+23],9)
+","+Numtostr(AWEOSCindBX[RightStrength+24],9)
+","+Numtostr(AWEOSCindBX[RightStrength+25],9)
+","+Numtostr(AWEOSCindBX[RightStrength+26],9)
+","+Numtostr(AWEOSCindBX[RightStrength+27],9)
+","+Numtostr(AWEOSCindBX[RightStrength+28],9)
+","+Numtostr(AWEOSCindBX[RightStrength+3],9)
+","+Numtostr(AWEOSCindBX[RightStrength+4],9)
+","+Numtostr(AWEOSCindBX[RightStrength+5],9)
+","+Numtostr(AWEOSCindBX[RightStrength+6],9)
+","+Numtostr(AWEOSCindBX[RightStrength+7],9)
+","+Numtostr(AWEOSCindBX[RightStrength+8],9)
+","+Numtostr(AWEOSCindBX[RightStrength+9],9)
+","+Numtostr(AWEOSCindBX[RightStrength-1],9)
+","+Numtostr(AWEOSCindBX[RightStrength-2],9)
+","+Numtostr(AWEOSCindBX[RightStrength-3],9)
+","+Numtostr(AWEOSCindBX[RightStrength-4],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+0],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+1],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+10],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+11],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+12],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+13],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+14],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+15],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+16],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+17],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+18],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+19],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+2],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+20],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+21],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+22],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+23],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+24],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+25],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+26],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+27],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+28],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+3],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+4],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+5],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+6],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+7],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+8],9)
+","+Numtostr(AWEOSCindBXL[RightStrength+9],9)
+","+Numtostr(AWEOSCindBXL[RightStrength-1],9)
+","+Numtostr(AWEOSCindBXL[RightStrength-2],9)
+","+Numtostr(AWEOSCindBXL[RightStrength-3],9)
+","+Numtostr(AWEOSCindBXL[RightStrength-4],9)
+","+Numtostr(AWEOSCindMED[RightStrength+0],9)
+","+Numtostr(AWEOSCindMED[RightStrength+1],9)
+","+Numtostr(AWEOSCindMED[RightStrength+10],9)
+","+Numtostr(AWEOSCindMED[RightStrength+11],9)
+","+Numtostr(AWEOSCindMED[RightStrength+12],9)
+","+Numtostr(AWEOSCindMED[RightStrength+13],9)
+","+Numtostr(AWEOSCindMED[RightStrength+14],9)
+","+Numtostr(AWEOSCindMED[RightStrength+15],9)
+","+Numtostr(AWEOSCindMED[RightStrength+16],9)
+","+Numtostr(AWEOSCindMED[RightStrength+17],9)
+","+Numtostr(AWEOSCindMED[RightStrength+18],9)
+","+Numtostr(AWEOSCindMED[RightStrength+19],9)
+","+Numtostr(AWEOSCindMED[RightStrength+2],9)
+","+Numtostr(AWEOSCindMED[RightStrength+20],9)
+","+Numtostr(AWEOSCindMED[RightStrength+21],9)
+","+Numtostr(AWEOSCindMED[RightStrength+22],9)
+","+Numtostr(AWEOSCindMED[RightStrength+23],9)
+","+Numtostr(AWEOSCindMED[RightStrength+24],9)
+","+Numtostr(AWEOSCindMED[RightStrength+25],9)
+","+Numtostr(AWEOSCindMED[RightStrength+26],9)
+","+Numtostr(AWEOSCindMED[RightStrength+27],9)
+","+Numtostr(AWEOSCindMED[RightStrength+28],9)
+","+Numtostr(AWEOSCindMED[RightStrength+3],9)
+","+Numtostr(AWEOSCindMED[RightStrength+4],9)
+","+Numtostr(AWEOSCindMED[RightStrength+5],9)
+","+Numtostr(AWEOSCindMED[RightStrength+6],9)
+","+Numtostr(AWEOSCindMED[RightStrength+7],9)
+","+Numtostr(AWEOSCindMED[RightStrength+8],9)
+","+Numtostr(AWEOSCindMED[RightStrength+9],9)
+","+Numtostr(AWEOSCindMED[RightStrength-1],9)
+","+Numtostr(AWEOSCindMED[RightStrength-2],9)
+","+Numtostr(AWEOSCindMED[RightStrength-3],9)
+","+Numtostr(AWEOSCindMED[RightStrength-4],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+0],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+1],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+10],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+11],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+12],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+13],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+14],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+15],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+16],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+17],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+18],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+19],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+2],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+20],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+21],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+22],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+23],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+24],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+25],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+26],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+27],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+28],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+3],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+4],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+5],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+6],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+7],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+8],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength+9],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength-1],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength-2],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength-3],9)
+","+Numtostr(AWEOSCindSMALL[RightStrength-4],9)
+","+Numtostr(BBDindBIG[RightStrength+0],9)
+","+Numtostr(BBDindBIG[RightStrength+1],9)
+","+Numtostr(BBDindBIG[RightStrength+10],9)
+","+Numtostr(BBDindBIG[RightStrength+11],9)
+","+Numtostr(BBDindBIG[RightStrength+12],9)
+","+Numtostr(BBDindBIG[RightStrength+13],9)
+","+Numtostr(BBDindBIG[RightStrength+14],9)
+","+Numtostr(BBDindBIG[RightStrength+15],9)
+","+Numtostr(BBDindBIG[RightStrength+16],9)
+","+Numtostr(BBDindBIG[RightStrength+17],9)
+","+Numtostr(BBDindBIG[RightStrength+18],9)
+","+Numtostr(BBDindBIG[RightStrength+19],9)
+","+Numtostr(BBDindBIG[RightStrength+2],9)
+","+Numtostr(BBDindBIG[RightStrength+20],9)
+","+Numtostr(BBDindBIG[RightStrength+21],9)
+","+Numtostr(BBDindBIG[RightStrength+22],9)
+","+Numtostr(BBDindBIG[RightStrength+23],9)
+","+Numtostr(BBDindBIG[RightStrength+24],9)
+","+Numtostr(BBDindBIG[RightStrength+25],9)
+","+Numtostr(BBDindBIG[RightStrength+26],9)
+","+Numtostr(BBDindBIG[RightStrength+27],9)
+","+Numtostr(BBDindBIG[RightStrength+28],9)
+","+Numtostr(BBDindBIG[RightStrength+3],9)
+","+Numtostr(BBDindBIG[RightStrength+4],9)
+","+Numtostr(BBDindBIG[RightStrength+5],9)
+","+Numtostr(BBDindBIG[RightStrength+6],9)
+","+Numtostr(BBDindBIG[RightStrength+7],9)
+","+Numtostr(BBDindBIG[RightStrength+8],9)
+","+Numtostr(BBDindBIG[RightStrength+9],9)
+","+Numtostr(BBDindBIG[RightStrength-1],9)
+","+Numtostr(BBDindBIG[RightStrength-2],9)
+","+Numtostr(BBDindBIG[RightStrength-3],9)
+","+Numtostr(BBDindBIG[RightStrength-4],9)
+","+Numtostr(BBDindBX[RightStrength+0],9)
+","+Numtostr(BBDindBX[RightStrength+1],9)
+","+Numtostr(BBDindBX[RightStrength+10],9)
+","+Numtostr(BBDindBX[RightStrength+11],9)
+","+Numtostr(BBDindBX[RightStrength+12],9)
+","+Numtostr(BBDindBX[RightStrength+13],9)
+","+Numtostr(BBDindBX[RightStrength+14],9)
+","+Numtostr(BBDindBX[RightStrength+15],9)
+","+Numtostr(BBDindBX[RightStrength+16],9)
+","+Numtostr(BBDindBX[RightStrength+17],9)
+","+Numtostr(BBDindBX[RightStrength+18],9)
+","+Numtostr(BBDindBX[RightStrength+19],9)
+","+Numtostr(BBDindBX[RightStrength+2],9)
+","+Numtostr(BBDindBX[RightStrength+20],9)
+","+Numtostr(BBDindBX[RightStrength+21],9)
+","+Numtostr(BBDindBX[RightStrength+22],9)
+","+Numtostr(BBDindBX[RightStrength+23],9)
+","+Numtostr(BBDindBX[RightStrength+24],9)
+","+Numtostr(BBDindBX[RightStrength+25],9)
+","+Numtostr(BBDindBX[RightStrength+26],9)
+","+Numtostr(BBDindBX[RightStrength+27],9)
+","+Numtostr(BBDindBX[RightStrength+28],9)
+","+Numtostr(BBDindBX[RightStrength+3],9)
+","+Numtostr(BBDindBX[RightStrength+4],9)
+","+Numtostr(BBDindBX[RightStrength+5],9)
+","+Numtostr(BBDindBX[RightStrength+6],9)
+","+Numtostr(BBDindBX[RightStrength+7],9)
+","+Numtostr(BBDindBX[RightStrength+8],9)
+","+Numtostr(BBDindBX[RightStrength+9],9)
+","+Numtostr(BBDindBX[RightStrength-1],9)
+","+Numtostr(BBDindBX[RightStrength-2],9)
+","+Numtostr(BBDindBX[RightStrength-3],9)
+","+Numtostr(BBDindBX[RightStrength-4],9)
+","+Numtostr(BBDindBXL[RightStrength+0],9)
+","+Numtostr(BBDindBXL[RightStrength+1],9)
+","+Numtostr(BBDindBXL[RightStrength+10],9)
+","+Numtostr(BBDindBXL[RightStrength+11],9)
+","+Numtostr(BBDindBXL[RightStrength+12],9)
+","+Numtostr(BBDindBXL[RightStrength+13],9)
+","+Numtostr(BBDindBXL[RightStrength+14],9)
+","+Numtostr(BBDindBXL[RightStrength+15],9)
+","+Numtostr(BBDindBXL[RightStrength+16],9)
+","+Numtostr(BBDindBXL[RightStrength+17],9)
+","+Numtostr(BBDindBXL[RightStrength+18],9)
+","+Numtostr(BBDindBXL[RightStrength+19],9)
+","+Numtostr(BBDindBXL[RightStrength+2],9)
+","+Numtostr(BBDindBXL[RightStrength+20],9)
+","+Numtostr(BBDindBXL[RightStrength+21],9)
+","+Numtostr(BBDindBXL[RightStrength+22],9)
+","+Numtostr(BBDindBXL[RightStrength+23],9)
+","+Numtostr(BBDindBXL[RightStrength+24],9)
+","+Numtostr(BBDindBXL[RightStrength+25],9)
+","+Numtostr(BBDindBXL[RightStrength+26],9)
+","+Numtostr(BBDindBXL[RightStrength+27],9)
+","+Numtostr(BBDindBXL[RightStrength+28],9)
+","+Numtostr(BBDindBXL[RightStrength+3],9)
+","+Numtostr(BBDindBXL[RightStrength+4],9)
+","+Numtostr(BBDindBXL[RightStrength+5],9)
+","+Numtostr(BBDindBXL[RightStrength+6],9)
+","+Numtostr(BBDindBXL[RightStrength+7],9)
+","+Numtostr(BBDindBXL[RightStrength+8],9)
+","+Numtostr(BBDindBXL[RightStrength+9],9)
+","+Numtostr(BBDindBXL[RightStrength-1],9)
+","+Numtostr(BBDindBXL[RightStrength-2],9)
+","+Numtostr(BBDindBXL[RightStrength-3],9)
+","+Numtostr(BBDindBXL[RightStrength-4],9)
+","+Numtostr(BBDindMED[RightStrength+0],9)
+","+Numtostr(BBDindMED[RightStrength+1],9)
+","+Numtostr(BBDindMED[RightStrength+10],9)
+","+Numtostr(BBDindMED[RightStrength+11],9)
+","+Numtostr(BBDindMED[RightStrength+12],9)
+","+Numtostr(BBDindMED[RightStrength+13],9)
+","+Numtostr(BBDindMED[RightStrength+14],9)
+","+Numtostr(BBDindMED[RightStrength+15],9)
+","+Numtostr(BBDindMED[RightStrength+16],9)
+","+Numtostr(BBDindMED[RightStrength+17],9)
+","+Numtostr(BBDindMED[RightStrength+18],9)
+","+Numtostr(BBDindMED[RightStrength+19],9)
+","+Numtostr(BBDindMED[RightStrength+2],9)
+","+Numtostr(BBDindMED[RightStrength+20],9)
+","+Numtostr(BBDindMED[RightStrength+21],9)
+","+Numtostr(BBDindMED[RightStrength+22],9)
+","+Numtostr(BBDindMED[RightStrength+23],9)
+","+Numtostr(BBDindMED[RightStrength+24],9)
+","+Numtostr(BBDindMED[RightStrength+25],9)
+","+Numtostr(BBDindMED[RightStrength+26],9)
+","+Numtostr(BBDindMED[RightStrength+27],9)
+","+Numtostr(BBDindMED[RightStrength+28],9)
+","+Numtostr(BBDindMED[RightStrength+3],9)
+","+Numtostr(BBDindMED[RightStrength+4],9)
+","+Numtostr(BBDindMED[RightStrength+5],9)
+","+Numtostr(BBDindMED[RightStrength+6],9)
+","+Numtostr(BBDindMED[RightStrength+7],9)
+","+Numtostr(BBDindMED[RightStrength+8],9)
+","+Numtostr(BBDindMED[RightStrength+9],9)
+","+Numtostr(BBDindMED[RightStrength-1],9)
+","+Numtostr(BBDindMED[RightStrength-2],9)
+","+Numtostr(BBDindMED[RightStrength-3],9)
+","+Numtostr(BBDindMED[RightStrength-4],9)
+","+Numtostr(BBDindSMALL[RightStrength+0],9)
+","+Numtostr(BBDindSMALL[RightStrength+1],9)
+","+Numtostr(BBDindSMALL[RightStrength+10],9)
+","+Numtostr(BBDindSMALL[RightStrength+11],9)
+","+Numtostr(BBDindSMALL[RightStrength+12],9)
+","+Numtostr(BBDindSMALL[RightStrength+13],9)
+","+Numtostr(BBDindSMALL[RightStrength+14],9)
+","+Numtostr(BBDindSMALL[RightStrength+15],9)
+","+Numtostr(BBDindSMALL[RightStrength+16],9)
+","+Numtostr(BBDindSMALL[RightStrength+17],9)
+","+Numtostr(BBDindSMALL[RightStrength+18],9)
+","+Numtostr(BBDindSMALL[RightStrength+19],9)
+","+Numtostr(BBDindSMALL[RightStrength+2],9)
+","+Numtostr(BBDindSMALL[RightStrength+20],9)
+","+Numtostr(BBDindSMALL[RightStrength+21],9)
+","+Numtostr(BBDindSMALL[RightStrength+22],9)
+","+Numtostr(BBDindSMALL[RightStrength+23],9)
+","+Numtostr(BBDindSMALL[RightStrength+24],9)
+","+Numtostr(BBDindSMALL[RightStrength+25],9)
+","+Numtostr(BBDindSMALL[RightStrength+26],9)
+","+Numtostr(BBDindSMALL[RightStrength+27],9)
+","+Numtostr(BBDindSMALL[RightStrength+28],9)
+","+Numtostr(BBDindSMALL[RightStrength+3],9)
+","+Numtostr(BBDindSMALL[RightStrength+4],9)
+","+Numtostr(BBDindSMALL[RightStrength+5],9)
+","+Numtostr(BBDindSMALL[RightStrength+6],9)
+","+Numtostr(BBDindSMALL[RightStrength+7],9)
+","+Numtostr(BBDindSMALL[RightStrength+8],9)
+","+Numtostr(BBDindSMALL[RightStrength+9],9)
+","+Numtostr(BBDindSMALL[RightStrength-1],9)
+","+Numtostr(BBDindSMALL[RightStrength-2],9)
+","+Numtostr(BBDindSMALL[RightStrength-3],9)
+","+Numtostr(BBDindSMALL[RightStrength-4],9)
+","+Numtostr(CCIindBIG[RightStrength+0],9)
+","+Numtostr(CCIindBIG[RightStrength+1],9)
+","+Numtostr(CCIindBIG[RightStrength+10],9)
+","+Numtostr(CCIindBIG[RightStrength+11],9)
+","+Numtostr(CCIindBIG[RightStrength+12],9)
+","+Numtostr(CCIindBIG[RightStrength+13],9)
+","+Numtostr(CCIindBIG[RightStrength+14],9)
+","+Numtostr(CCIindBIG[RightStrength+15],9)
+","+Numtostr(CCIindBIG[RightStrength+16],9)
+","+Numtostr(CCIindBIG[RightStrength+17],9)
			+" )"; 
			res1v02 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_feat1v02);
			srun1v02 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures1v02.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_feat1v02
					+ NewLine);
			end;
		End;
	End;
	 TblNum = TblNum+1;	 
	 


	//(4) insert feature data into dbo.barFeatures2v02	
	if ReadWriteToDB and TblNum = 11+2  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		if  srun2v02 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_feat2v02 = "INSERT INTO  [dbo].[barFeatures2v02] VALUES ("
			+queryResult//MAX(loadId) 
			+","+Numtostr(loadGroupId,0) 
			+","+Numtostr(RightStrength,0)//RightStrength
			+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
			+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"'"+dateBarOpened+"' "+","+"'"+dayOfWeekBarOpened+"' "+","+"'"+dateTimeBarOpened+"' "+","+"'"+timeBarOpenedhhmmss+"' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)
+","+Numtostr(CMIindBIG[RightStrength+22],9)
+","+Numtostr(CMIindBIG[RightStrength+23],9)
+","+Numtostr(CMIindBIG[RightStrength+24],9)
+","+Numtostr(CMIindBIG[RightStrength+25],9)
+","+Numtostr(CMIindBIG[RightStrength+26],9)
+","+Numtostr(CMIindBIG[RightStrength+27],9)
+","+Numtostr(CMIindBIG[RightStrength+28],9)
+","+Numtostr(CMIindBIG[RightStrength+3],9)
+","+Numtostr(CMIindBIG[RightStrength+4],9)
+","+Numtostr(CMIindBIG[RightStrength+5],9)
+","+Numtostr(CMIindBIG[RightStrength+6],9)
+","+Numtostr(CMIindBIG[RightStrength+7],9)
+","+Numtostr(CMIindBIG[RightStrength+8],9)
+","+Numtostr(CMIindBIG[RightStrength+9],9)
+","+Numtostr(CMIindBIG[RightStrength-1],9)
+","+Numtostr(CMIindBIG[RightStrength-2],9)
+","+Numtostr(CMIindBIG[RightStrength-3],9)
+","+Numtostr(CMIindBIG[RightStrength-4],9)
+","+Numtostr(CMIindBX[RightStrength+0],9)
+","+Numtostr(CMIindBX[RightStrength+1],9)
+","+Numtostr(CMIindBX[RightStrength+10],9)
+","+Numtostr(CMIindBX[RightStrength+11],9)
+","+Numtostr(CMIindBX[RightStrength+12],9)
+","+Numtostr(CMIindBX[RightStrength+13],9)
+","+Numtostr(CMIindBX[RightStrength+14],9)
+","+Numtostr(CMIindBX[RightStrength+15],9)
+","+Numtostr(CMIindBX[RightStrength+16],9)
+","+Numtostr(CMIindBX[RightStrength+17],9)
+","+Numtostr(CMIindBX[RightStrength+18],9)
+","+Numtostr(CMIindBX[RightStrength+19],9)
+","+Numtostr(CMIindBX[RightStrength+2],9)
+","+Numtostr(CMIindBX[RightStrength+20],9)
+","+Numtostr(CMIindBX[RightStrength+21],9)
+","+Numtostr(CMIindBX[RightStrength+22],9)
+","+Numtostr(CMIindBX[RightStrength+23],9)
+","+Numtostr(CMIindBX[RightStrength+24],9)
+","+Numtostr(CMIindBX[RightStrength+25],9)
+","+Numtostr(CMIindBX[RightStrength+26],9)
+","+Numtostr(CMIindBX[RightStrength+27],9)
+","+Numtostr(CMIindBX[RightStrength+28],9)
+","+Numtostr(CMIindBX[RightStrength+3],9)
+","+Numtostr(CMIindBX[RightStrength+4],9)
+","+Numtostr(CMIindBX[RightStrength+5],9)
+","+Numtostr(CMIindBX[RightStrength+6],9)
+","+Numtostr(CMIindBX[RightStrength+7],9)
+","+Numtostr(CMIindBX[RightStrength+8],9)
+","+Numtostr(CMIindBX[RightStrength+9],9)
+","+Numtostr(CMIindBX[RightStrength-1],9)
+","+Numtostr(CMIindBX[RightStrength-2],9)
+","+Numtostr(CMIindBX[RightStrength-3],9)
+","+Numtostr(CMIindBX[RightStrength-4],9)
+","+Numtostr(CMIindBXL[RightStrength+0],9)
+","+Numtostr(CMIindBXL[RightStrength+1],9)
+","+Numtostr(CMIindBXL[RightStrength+10],9)
+","+Numtostr(CMIindBXL[RightStrength+11],9)
+","+Numtostr(CMIindBXL[RightStrength+12],9)
+","+Numtostr(CMIindBXL[RightStrength+13],9)
+","+Numtostr(CMIindBXL[RightStrength+14],9)
+","+Numtostr(CMIindBXL[RightStrength+15],9)
+","+Numtostr(CMIindBXL[RightStrength+16],9)
+","+Numtostr(CMIindBXL[RightStrength+17],9)
+","+Numtostr(CMIindBXL[RightStrength+18],9)
+","+Numtostr(CMIindBXL[RightStrength+19],9)
+","+Numtostr(CMIindBXL[RightStrength+2],9)
+","+Numtostr(CMIindBXL[RightStrength+20],9)
+","+Numtostr(CMIindBXL[RightStrength+21],9)
+","+Numtostr(CMIindBXL[RightStrength+22],9)
+","+Numtostr(CMIindBXL[RightStrength+23],9)
+","+Numtostr(CMIindBXL[RightStrength+24],9)
+","+Numtostr(CMIindBXL[RightStrength+25],9)
+","+Numtostr(CMIindBXL[RightStrength+26],9)
+","+Numtostr(CMIindBXL[RightStrength+27],9)
+","+Numtostr(CMIindBXL[RightStrength+28],9)
+","+Numtostr(CMIindBXL[RightStrength+3],9)
+","+Numtostr(CMIindBXL[RightStrength+4],9)
+","+Numtostr(CMIindBXL[RightStrength+5],9)
+","+Numtostr(CMIindBXL[RightStrength+6],9)
+","+Numtostr(CMIindBXL[RightStrength+7],9)
+","+Numtostr(CMIindBXL[RightStrength+8],9)
+","+Numtostr(CMIindBXL[RightStrength+9],9)
+","+Numtostr(CMIindBXL[RightStrength-1],9)
+","+Numtostr(CMIindBXL[RightStrength-2],9)
+","+Numtostr(CMIindBXL[RightStrength-3],9)
+","+Numtostr(CMIindBXL[RightStrength-4],9)
+","+Numtostr(CMIindMED[RightStrength+0],9)
+","+Numtostr(CMIindMED[RightStrength+1],9)
+","+Numtostr(CMIindMED[RightStrength+10],9)
+","+Numtostr(CMIindMED[RightStrength+11],9)
+","+Numtostr(CMIindMED[RightStrength+12],9)
+","+Numtostr(CMIindMED[RightStrength+13],9)
+","+Numtostr(CMIindMED[RightStrength+14],9)
+","+Numtostr(CMIindMED[RightStrength+15],9)
+","+Numtostr(CMIindMED[RightStrength+16],9)
+","+Numtostr(CMIindMED[RightStrength+17],9)
+","+Numtostr(CMIindMED[RightStrength+18],9)
+","+Numtostr(CMIindMED[RightStrength+19],9)
+","+Numtostr(CMIindMED[RightStrength+2],9)
+","+Numtostr(CMIindMED[RightStrength+20],9)
+","+Numtostr(CMIindMED[RightStrength+21],9)
+","+Numtostr(CMIindMED[RightStrength+22],9)
+","+Numtostr(CMIindMED[RightStrength+23],9)
+","+Numtostr(CMIindMED[RightStrength+24],9)
+","+Numtostr(CMIindMED[RightStrength+25],9)
+","+Numtostr(CMIindMED[RightStrength+26],9)
+","+Numtostr(CMIindMED[RightStrength+27],9)
+","+Numtostr(CMIindMED[RightStrength+28],9)
+","+Numtostr(CMIindMED[RightStrength+3],9)
+","+Numtostr(CMIindMED[RightStrength+4],9)
+","+Numtostr(CMIindMED[RightStrength+5],9)
+","+Numtostr(CMIindMED[RightStrength+6],9)
+","+Numtostr(CMIindMED[RightStrength+7],9)
+","+Numtostr(CMIindMED[RightStrength+8],9)
+","+Numtostr(CMIindMED[RightStrength+9],9)
+","+Numtostr(CMIindMED[RightStrength-1],9)
+","+Numtostr(CMIindMED[RightStrength-2],9)
+","+Numtostr(CMIindMED[RightStrength-3],9)
+","+Numtostr(CMIindMED[RightStrength-4],9)
+","+Numtostr(CMIindSMALL[RightStrength+0],9)
+","+Numtostr(CMIindSMALL[RightStrength+1],9)
+","+Numtostr(CMIindSMALL[RightStrength+10],9)
+","+Numtostr(CMIindSMALL[RightStrength+11],9)
+","+Numtostr(CMIindSMALL[RightStrength+12],9)
+","+Numtostr(CMIindSMALL[RightStrength+13],9)
+","+Numtostr(CMIindSMALL[RightStrength+14],9)
+","+Numtostr(CMIindSMALL[RightStrength+15],9)
+","+Numtostr(CMIindSMALL[RightStrength+16],9)
+","+Numtostr(CMIindSMALL[RightStrength+17],9)
+","+Numtostr(CMIindSMALL[RightStrength+18],9)
+","+Numtostr(CMIindSMALL[RightStrength+19],9)
+","+Numtostr(CMIindSMALL[RightStrength+2],9)
+","+Numtostr(CMIindSMALL[RightStrength+20],9)
+","+Numtostr(CMIindSMALL[RightStrength+21],9)
+","+Numtostr(CMIindSMALL[RightStrength+22],9)
+","+Numtostr(CMIindSMALL[RightStrength+23],9)
+","+Numtostr(CMIindSMALL[RightStrength+24],9)
+","+Numtostr(CMIindSMALL[RightStrength+25],9)
+","+Numtostr(CMIindSMALL[RightStrength+26],9)
+","+Numtostr(CMIindSMALL[RightStrength+27],9)
+","+Numtostr(CMIindSMALL[RightStrength+28],9)
+","+Numtostr(CMIindSMALL[RightStrength+3],9)
+","+Numtostr(CMIindSMALL[RightStrength+4],9)
+","+Numtostr(CMIindSMALL[RightStrength+5],9)
+","+Numtostr(CMIindSMALL[RightStrength+6],9)
+","+Numtostr(CMIindSMALL[RightStrength+7],9)
+","+Numtostr(CMIindSMALL[RightStrength+8],9)
+","+Numtostr(CMIindSMALL[RightStrength+9],9)
+","+Numtostr(CMIindSMALL[RightStrength-1],9)
+","+Numtostr(CMIindSMALL[RightStrength-2],9)
+","+Numtostr(CMIindSMALL[RightStrength-3],9)
+","+Numtostr(CMIindSMALL[RightStrength-4],9)
+","+Numtostr(CMOindBIG[RightStrength+0],9)
+","+Numtostr(CMOindBIG[RightStrength+1],9)
+","+Numtostr(CMOindBIG[RightStrength+10],9)
+","+Numtostr(CMOindBIG[RightStrength+11],9)
+","+Numtostr(CMOindBIG[RightStrength+12],9)
+","+Numtostr(CMOindBIG[RightStrength+13],9)
+","+Numtostr(CMOindBIG[RightStrength+14],9)
+","+Numtostr(CMOindBIG[RightStrength+15],9)
+","+Numtostr(CMOindBIG[RightStrength+16],9)
+","+Numtostr(CMOindBIG[RightStrength+17],9)
+","+Numtostr(CMOindBIG[RightStrength+18],9)
+","+Numtostr(CMOindBIG[RightStrength+19],9)
+","+Numtostr(CMOindBIG[RightStrength+2],9)
+","+Numtostr(CMOindBIG[RightStrength+20],9)
+","+Numtostr(CMOindBIG[RightStrength+21],9)
+","+Numtostr(CMOindBIG[RightStrength+22],9)
+","+Numtostr(CMOindBIG[RightStrength+23],9)
+","+Numtostr(CMOindBIG[RightStrength+24],9)
+","+Numtostr(CMOindBIG[RightStrength+25],9)
+","+Numtostr(CMOindBIG[RightStrength+26],9)
+","+Numtostr(CMOindBIG[RightStrength+27],9)
+","+Numtostr(CMOindBIG[RightStrength+28],9)
+","+Numtostr(CMOindBIG[RightStrength+3],9)
+","+Numtostr(CMOindBIG[RightStrength+4],9)
+","+Numtostr(CMOindBIG[RightStrength+5],9)
+","+Numtostr(CMOindBIG[RightStrength+6],9)
+","+Numtostr(CMOindBIG[RightStrength+7],9)
+","+Numtostr(CMOindBIG[RightStrength+8],9)
+","+Numtostr(CMOindBIG[RightStrength+9],9)
+","+Numtostr(CMOindBIG[RightStrength-1],9)
+","+Numtostr(CMOindBIG[RightStrength-2],9)
+","+Numtostr(CMOindBIG[RightStrength-3],9)
+","+Numtostr(CMOindBIG[RightStrength-4],9)
+","+Numtostr(CMOindBX[RightStrength+0],9)
+","+Numtostr(CMOindBX[RightStrength+1],9)
+","+Numtostr(CMOindBX[RightStrength+10],9)
+","+Numtostr(CMOindBX[RightStrength+11],9)
+","+Numtostr(CMOindBX[RightStrength+12],9)
+","+Numtostr(CMOindBX[RightStrength+13],9)
+","+Numtostr(CMOindBX[RightStrength+14],9)
+","+Numtostr(CMOindBX[RightStrength+15],9)
+","+Numtostr(CMOindBX[RightStrength+16],9)
+","+Numtostr(CMOindBX[RightStrength+17],9)
+","+Numtostr(CMOindBX[RightStrength+18],9)
+","+Numtostr(CMOindBX[RightStrength+19],9)
+","+Numtostr(CMOindBX[RightStrength+2],9)
+","+Numtostr(CMOindBX[RightStrength+20],9)
+","+Numtostr(CMOindBX[RightStrength+21],9)
+","+Numtostr(CMOindBX[RightStrength+22],9)
+","+Numtostr(CMOindBX[RightStrength+23],9)
+","+Numtostr(CMOindBX[RightStrength+24],9)
+","+Numtostr(CMOindBX[RightStrength+25],9)
+","+Numtostr(CMOindBX[RightStrength+26],9)
+","+Numtostr(CMOindBX[RightStrength+27],9)
+","+Numtostr(CMOindBX[RightStrength+28],9)
+","+Numtostr(CMOindBX[RightStrength+3],9)
+","+Numtostr(CMOindBX[RightStrength+4],9)
+","+Numtostr(CMOindBX[RightStrength+5],9)
+","+Numtostr(CMOindBX[RightStrength+6],9)
+","+Numtostr(CMOindBX[RightStrength+7],9)
+","+Numtostr(CMOindBX[RightStrength+8],9)
+","+Numtostr(CMOindBX[RightStrength+9],9)
+","+Numtostr(CMOindBX[RightStrength-1],9)
+","+Numtostr(CMOindBX[RightStrength-2],9)
+","+Numtostr(CMOindBX[RightStrength-3],9)
+","+Numtostr(CMOindBX[RightStrength-4],9)
+","+Numtostr(CMOindBXL[RightStrength+0],9)
+","+Numtostr(CMOindBXL[RightStrength+1],9)
+","+Numtostr(CMOindBXL[RightStrength+10],9)
+","+Numtostr(CMOindBXL[RightStrength+11],9)
+","+Numtostr(CMOindBXL[RightStrength+12],9)
+","+Numtostr(CMOindBXL[RightStrength+13],9)
+","+Numtostr(CMOindBXL[RightStrength+14],9)
+","+Numtostr(CMOindBXL[RightStrength+15],9)
+","+Numtostr(CMOindBXL[RightStrength+16],9)
+","+Numtostr(CMOindBXL[RightStrength+17],9)
+","+Numtostr(CMOindBXL[RightStrength+18],9)
+","+Numtostr(CMOindBXL[RightStrength+19],9)
+","+Numtostr(CMOindBXL[RightStrength+2],9)
+","+Numtostr(CMOindBXL[RightStrength+20],9)
+","+Numtostr(CMOindBXL[RightStrength+21],9)
+","+Numtostr(CMOindBXL[RightStrength+22],9)
+","+Numtostr(CMOindBXL[RightStrength+23],9)
+","+Numtostr(CMOindBXL[RightStrength+24],9)
+","+Numtostr(CMOindBXL[RightStrength+25],9)
+","+Numtostr(CMOindBXL[RightStrength+26],9)
+","+Numtostr(CMOindBXL[RightStrength+27],9)
+","+Numtostr(CMOindBXL[RightStrength+28],9)
+","+Numtostr(CMOindBXL[RightStrength+3],9)
+","+Numtostr(CMOindBXL[RightStrength+4],9)
+","+Numtostr(CMOindBXL[RightStrength+5],9)
+","+Numtostr(CMOindBXL[RightStrength+6],9)
+","+Numtostr(CMOindBXL[RightStrength+7],9)
+","+Numtostr(CMOindBXL[RightStrength+8],9)
+","+Numtostr(CMOindBXL[RightStrength+9],9)
+","+Numtostr(CMOindBXL[RightStrength-1],9)
+","+Numtostr(CMOindBXL[RightStrength-2],9)
+","+Numtostr(CMOindBXL[RightStrength-3],9)
+","+Numtostr(CMOindBXL[RightStrength-4],9)
+","+Numtostr(CMOindMED[RightStrength+0],9)
+","+Numtostr(CMOindMED[RightStrength+1],9)
+","+Numtostr(CMOindMED[RightStrength+10],9)
+","+Numtostr(CMOindMED[RightStrength+11],9)
+","+Numtostr(CMOindMED[RightStrength+12],9)
+","+Numtostr(CMOindMED[RightStrength+13],9)
+","+Numtostr(CMOindMED[RightStrength+14],9)
+","+Numtostr(CMOindMED[RightStrength+15],9)
+","+Numtostr(CMOindMED[RightStrength+16],9)
+","+Numtostr(CMOindMED[RightStrength+17],9)
+","+Numtostr(CMOindMED[RightStrength+18],9)
+","+Numtostr(CMOindMED[RightStrength+19],9)
+","+Numtostr(CMOindMED[RightStrength+2],9)
+","+Numtostr(CMOindMED[RightStrength+20],9)
+","+Numtostr(CMOindMED[RightStrength+21],9)
+","+Numtostr(CMOindMED[RightStrength+22],9)
+","+Numtostr(CMOindMED[RightStrength+23],9)
+","+Numtostr(CMOindMED[RightStrength+24],9)
+","+Numtostr(CMOindMED[RightStrength+25],9)
+","+Numtostr(CMOindMED[RightStrength+26],9)
+","+Numtostr(CMOindMED[RightStrength+27],9)
+","+Numtostr(CMOindMED[RightStrength+28],9)
+","+Numtostr(CMOindMED[RightStrength+3],9)
+","+Numtostr(CMOindMED[RightStrength+4],9)
+","+Numtostr(CMOindMED[RightStrength+5],9)
+","+Numtostr(CMOindMED[RightStrength+6],9)
+","+Numtostr(CMOindMED[RightStrength+7],9)
+","+Numtostr(CMOindMED[RightStrength+8],9)
+","+Numtostr(CMOindMED[RightStrength+9],9)
+","+Numtostr(CMOindMED[RightStrength-1],9)
+","+Numtostr(CMOindMED[RightStrength-2],9)
+","+Numtostr(CMOindMED[RightStrength-3],9)
+","+Numtostr(CMOindMED[RightStrength-4],9)
+","+Numtostr(CMOindSMALL[RightStrength+0],9)
+","+Numtostr(CMOindSMALL[RightStrength+1],9)
+","+Numtostr(CMOindSMALL[RightStrength+10],9)
+","+Numtostr(CMOindSMALL[RightStrength+11],9)
+","+Numtostr(CMOindSMALL[RightStrength+12],9)
+","+Numtostr(CMOindSMALL[RightStrength+13],9)
+","+Numtostr(CMOindSMALL[RightStrength+14],9)
+","+Numtostr(CMOindSMALL[RightStrength+15],9)
+","+Numtostr(CMOindSMALL[RightStrength+16],9)
+","+Numtostr(CMOindSMALL[RightStrength+17],9)
+","+Numtostr(CMOindSMALL[RightStrength+18],9)
+","+Numtostr(CMOindSMALL[RightStrength+19],9)
+","+Numtostr(CMOindSMALL[RightStrength+2],9)
+","+Numtostr(CMOindSMALL[RightStrength+20],9)
+","+Numtostr(CMOindSMALL[RightStrength+21],9)
+","+Numtostr(CMOindSMALL[RightStrength+22],9)
+","+Numtostr(CMOindSMALL[RightStrength+23],9)
+","+Numtostr(CMOindSMALL[RightStrength+24],9)
+","+Numtostr(CMOindSMALL[RightStrength+25],9)
+","+Numtostr(CMOindSMALL[RightStrength+26],9)
+","+Numtostr(CMOindSMALL[RightStrength+27],9)
+","+Numtostr(CMOindSMALL[RightStrength+28],9)
+","+Numtostr(CMOindSMALL[RightStrength+3],9)
+","+Numtostr(CMOindSMALL[RightStrength+4],9)
+","+Numtostr(CMOindSMALL[RightStrength+5],9)
+","+Numtostr(CMOindSMALL[RightStrength+6],9)
+","+Numtostr(CMOindSMALL[RightStrength+7],9)
+","+Numtostr(CMOindSMALL[RightStrength+8],9)
+","+Numtostr(CMOindSMALL[RightStrength+9],9)
+","+Numtostr(CMOindSMALL[RightStrength-1],9)
+","+Numtostr(CMOindSMALL[RightStrength-2],9)
+","+Numtostr(CMOindSMALL[RightStrength-3],9)
+","+Numtostr(CMOindSMALL[RightStrength-4],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+0],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+1],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+10],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+11],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+12],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+13],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+14],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+15],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+16],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+17],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+18],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+19],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+2],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+20],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+21],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+22],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+23],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+24],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+25],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+26],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+27],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+28],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+3],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+4],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+5],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+6],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+7],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+8],9)
+","+Numtostr(CUMRSIindBIG[RightStrength+9],9)
+","+Numtostr(CUMRSIindBIG[RightStrength-1],9)
+","+Numtostr(CUMRSIindBIG[RightStrength-2],9)
+","+Numtostr(CUMRSIindBIG[RightStrength-3],9)
+","+Numtostr(CUMRSIindBIG[RightStrength-4],9)
+","+Numtostr(CUMRSIindBX[RightStrength+0],9)
+","+Numtostr(CUMRSIindBX[RightStrength+1],9)
+","+Numtostr(CUMRSIindBX[RightStrength+10],9)
+","+Numtostr(CUMRSIindBX[RightStrength+11],9)
+","+Numtostr(CUMRSIindBX[RightStrength+12],9)
+","+Numtostr(CUMRSIindBX[RightStrength+13],9)
+","+Numtostr(CUMRSIindBX[RightStrength+14],9)
+","+Numtostr(CUMRSIindBX[RightStrength+15],9)
+","+Numtostr(CUMRSIindBX[RightStrength+16],9)
+","+Numtostr(CUMRSIindBX[RightStrength+17],9)
+","+Numtostr(CUMRSIindBX[RightStrength+18],9)
+","+Numtostr(CUMRSIindBX[RightStrength+19],9)
+","+Numtostr(CUMRSIindBX[RightStrength+2],9)
+","+Numtostr(CUMRSIindBX[RightStrength+20],9)
+","+Numtostr(CUMRSIindBX[RightStrength+21],9)
+","+Numtostr(CUMRSIindBX[RightStrength+22],9)
+","+Numtostr(CUMRSIindBX[RightStrength+23],9)
+","+Numtostr(CUMRSIindBX[RightStrength+24],9)
+","+Numtostr(CUMRSIindBX[RightStrength+25],9)
+","+Numtostr(CUMRSIindBX[RightStrength+26],9)
+","+Numtostr(CUMRSIindBX[RightStrength+27],9)
+","+Numtostr(CUMRSIindBX[RightStrength+28],9)
+","+Numtostr(CUMRSIindBX[RightStrength+3],9)
+","+Numtostr(CUMRSIindBX[RightStrength+4],9)
+","+Numtostr(CUMRSIindBX[RightStrength+5],9)
+","+Numtostr(CUMRSIindBX[RightStrength+6],9)
+","+Numtostr(CUMRSIindBX[RightStrength+7],9)
+","+Numtostr(CUMRSIindBX[RightStrength+8],9)
+","+Numtostr(CUMRSIindBX[RightStrength+9],9)
+","+Numtostr(CUMRSIindBX[RightStrength-1],9)
+","+Numtostr(CUMRSIindBX[RightStrength-2],9)
+","+Numtostr(CUMRSIindBX[RightStrength-3],9)
+","+Numtostr(CUMRSIindBX[RightStrength-4],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+0],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+1],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+10],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+11],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+12],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+13],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+14],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+15],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+16],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+17],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+18],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+19],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+2],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+20],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+21],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+22],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+23],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+24],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+25],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+26],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+27],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+28],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+3],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+4],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+5],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+6],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+7],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+8],9)
+","+Numtostr(CUMRSIindBXL[RightStrength+9],9)
+","+Numtostr(CUMRSIindBXL[RightStrength-1],9)
+","+Numtostr(CUMRSIindBXL[RightStrength-2],9)
+","+Numtostr(CUMRSIindBXL[RightStrength-3],9)
+","+Numtostr(CUMRSIindBXL[RightStrength-4],9)
+","+Numtostr(CUMRSIindMED[RightStrength+0],9)
+","+Numtostr(CUMRSIindMED[RightStrength+1],9)
+","+Numtostr(CUMRSIindMED[RightStrength+10],9)
+","+Numtostr(CUMRSIindMED[RightStrength+11],9)
+","+Numtostr(CUMRSIindMED[RightStrength+12],9)
+","+Numtostr(CUMRSIindMED[RightStrength+13],9)
+","+Numtostr(CUMRSIindMED[RightStrength+14],9)
+","+Numtostr(CUMRSIindMED[RightStrength+15],9)
+","+Numtostr(CUMRSIindMED[RightStrength+16],9)
+","+Numtostr(CUMRSIindMED[RightStrength+17],9)
+","+Numtostr(CUMRSIindMED[RightStrength+18],9)
+","+Numtostr(CUMRSIindMED[RightStrength+19],9)
+","+Numtostr(CUMRSIindMED[RightStrength+2],9)
+","+Numtostr(CUMRSIindMED[RightStrength+20],9)
+","+Numtostr(CUMRSIindMED[RightStrength+21],9)
+","+Numtostr(CUMRSIindMED[RightStrength+22],9)
+","+Numtostr(CUMRSIindMED[RightStrength+23],9)
+","+Numtostr(CUMRSIindMED[RightStrength+24],9)
+","+Numtostr(CUMRSIindMED[RightStrength+25],9)
+","+Numtostr(CUMRSIindMED[RightStrength+26],9)
+","+Numtostr(CUMRSIindMED[RightStrength+27],9)
+","+Numtostr(CUMRSIindMED[RightStrength+28],9)
+","+Numtostr(CUMRSIindMED[RightStrength+3],9)
+","+Numtostr(CUMRSIindMED[RightStrength+4],9)
+","+Numtostr(CUMRSIindMED[RightStrength+5],9)
+","+Numtostr(CUMRSIindMED[RightStrength+6],9)
+","+Numtostr(CUMRSIindMED[RightStrength+7],9)
+","+Numtostr(CUMRSIindMED[RightStrength+8],9)
+","+Numtostr(CUMRSIindMED[RightStrength+9],9)
+","+Numtostr(CUMRSIindMED[RightStrength-1],9)
+","+Numtostr(CUMRSIindMED[RightStrength-2],9)
+","+Numtostr(CUMRSIindMED[RightStrength-3],9)
+","+Numtostr(CUMRSIindMED[RightStrength-4],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+0],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+1],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+10],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+11],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+12],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+13],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+14],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+15],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+16],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+17],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+18],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+19],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+2],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+20],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+21],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+22],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+23],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+24],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+25],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+26],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+27],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+28],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+3],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+4],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+5],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+6],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+7],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+8],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength+9],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength-1],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength-2],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength-3],9)
+","+Numtostr(CUMRSIindSMALL[RightStrength-4],9)
+","+Numtostr(DMIStoindBIG[RightStrength+0],9)
+","+Numtostr(DMIStoindBIG[RightStrength+1],9)
+","+Numtostr(DMIStoindBIG[RightStrength+10],9)
+","+Numtostr(DMIStoindBIG[RightStrength+11],9)
+","+Numtostr(DMIStoindBIG[RightStrength+12],9)
+","+Numtostr(DMIStoindBIG[RightStrength+13],9)
+","+Numtostr(DMIStoindBIG[RightStrength+14],9)
+","+Numtostr(DMIStoindBIG[RightStrength+15],9)
+","+Numtostr(DMIStoindBIG[RightStrength+16],9)
+","+Numtostr(DMIStoindBIG[RightStrength+17],9)
+","+Numtostr(DMIStoindBIG[RightStrength+18],9)
+","+Numtostr(DMIStoindBIG[RightStrength+19],9)
+","+Numtostr(DMIStoindBIG[RightStrength+2],9)
+","+Numtostr(DMIStoindBIG[RightStrength+20],9)
+","+Numtostr(DMIStoindBIG[RightStrength+21],9)
+","+Numtostr(DMIStoindBIG[RightStrength+22],9)
+","+Numtostr(DMIStoindBIG[RightStrength+23],9)
+","+Numtostr(DMIStoindBIG[RightStrength+24],9)
+","+Numtostr(DMIStoindBIG[RightStrength+25],9)
+","+Numtostr(DMIStoindBIG[RightStrength+26],9)
			+" )"; 
			res2v02 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_feat2v02);
			srun2v02 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures2v02.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_feat2v02
					+ NewLine);
			end;
		End;
	End;
	 TblNum = TblNum+1;


	//(4) insert feature data into dbo.barFeatures3v02	
	if ReadWriteToDB and TblNum = 11+3  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		if  srun3v02 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_feat3v02 = "INSERT INTO  [dbo].[barFeatures3v02] VALUES ("
			+queryResult//MAX(loadId) 
			+","+Numtostr(loadGroupId,0) 
			+","+Numtostr(RightStrength,0)//RightStrength
			+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
			+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"'"+dateBarOpened+"' "+","+"'"+dayOfWeekBarOpened+"' "+","+"'"+dateTimeBarOpened+"' "+","+"'"+timeBarOpenedhhmmss+"' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)
+","+Numtostr(FRAMAindBIG[RightStrength+6],9)
+","+Numtostr(FRAMAindBIG[RightStrength+7],9)
+","+Numtostr(FRAMAindBIG[RightStrength+8],9)
+","+Numtostr(FRAMAindBIG[RightStrength+9],9)
+","+Numtostr(FRAMAindBIG[RightStrength-1],9)
+","+Numtostr(FRAMAindBIG[RightStrength-2],9)
+","+Numtostr(FRAMAindBIG[RightStrength-3],9)
+","+Numtostr(FRAMAindBIG[RightStrength-4],9)
+","+Numtostr(FRAMAindBX[RightStrength+0],9)
+","+Numtostr(FRAMAindBX[RightStrength+1],9)
+","+Numtostr(FRAMAindBX[RightStrength+10],9)
+","+Numtostr(FRAMAindBX[RightStrength+11],9)
+","+Numtostr(FRAMAindBX[RightStrength+12],9)
+","+Numtostr(FRAMAindBX[RightStrength+13],9)
+","+Numtostr(FRAMAindBX[RightStrength+14],9)
+","+Numtostr(FRAMAindBX[RightStrength+15],9)
+","+Numtostr(FRAMAindBX[RightStrength+16],9)
+","+Numtostr(FRAMAindBX[RightStrength+17],9)
+","+Numtostr(FRAMAindBX[RightStrength+18],9)
+","+Numtostr(FRAMAindBX[RightStrength+19],9)
+","+Numtostr(FRAMAindBX[RightStrength+2],9)
+","+Numtostr(FRAMAindBX[RightStrength+20],9)
+","+Numtostr(FRAMAindBX[RightStrength+21],9)
+","+Numtostr(FRAMAindBX[RightStrength+22],9)
+","+Numtostr(FRAMAindBX[RightStrength+23],9)
+","+Numtostr(FRAMAindBX[RightStrength+24],9)
+","+Numtostr(FRAMAindBX[RightStrength+25],9)
+","+Numtostr(FRAMAindBX[RightStrength+26],9)
+","+Numtostr(FRAMAindBX[RightStrength+27],9)
+","+Numtostr(FRAMAindBX[RightStrength+28],9)
+","+Numtostr(FRAMAindBX[RightStrength+3],9)
+","+Numtostr(FRAMAindBX[RightStrength+4],9)
+","+Numtostr(FRAMAindBX[RightStrength+5],9)
+","+Numtostr(FRAMAindBX[RightStrength+6],9)
+","+Numtostr(FRAMAindBX[RightStrength+7],9)
+","+Numtostr(FRAMAindBX[RightStrength+8],9)
+","+Numtostr(FRAMAindBX[RightStrength+9],9)
+","+Numtostr(FRAMAindBX[RightStrength-1],9)
+","+Numtostr(FRAMAindBX[RightStrength-2],9)
+","+Numtostr(FRAMAindBX[RightStrength-3],9)
+","+Numtostr(FRAMAindBX[RightStrength-4],9)
+","+Numtostr(FRAMAindBXL[RightStrength+0],9)
+","+Numtostr(FRAMAindBXL[RightStrength+1],9)
+","+Numtostr(FRAMAindBXL[RightStrength+10],9)
+","+Numtostr(FRAMAindBXL[RightStrength+11],9)
+","+Numtostr(FRAMAindBXL[RightStrength+12],9)
+","+Numtostr(FRAMAindBXL[RightStrength+13],9)
+","+Numtostr(FRAMAindBXL[RightStrength+14],9)
+","+Numtostr(FRAMAindBXL[RightStrength+15],9)
+","+Numtostr(FRAMAindBXL[RightStrength+16],9)
+","+Numtostr(FRAMAindBXL[RightStrength+17],9)
+","+Numtostr(FRAMAindBXL[RightStrength+18],9)
+","+Numtostr(FRAMAindBXL[RightStrength+19],9)
+","+Numtostr(FRAMAindBXL[RightStrength+2],9)
+","+Numtostr(FRAMAindBXL[RightStrength+20],9)
+","+Numtostr(FRAMAindBXL[RightStrength+21],9)
+","+Numtostr(FRAMAindBXL[RightStrength+22],9)
+","+Numtostr(FRAMAindBXL[RightStrength+23],9)
+","+Numtostr(FRAMAindBXL[RightStrength+24],9)
+","+Numtostr(FRAMAindBXL[RightStrength+25],9)
+","+Numtostr(FRAMAindBXL[RightStrength+26],9)
+","+Numtostr(FRAMAindBXL[RightStrength+27],9)
+","+Numtostr(FRAMAindBXL[RightStrength+28],9)
+","+Numtostr(FRAMAindBXL[RightStrength+3],9)
+","+Numtostr(FRAMAindBXL[RightStrength+4],9)
+","+Numtostr(FRAMAindBXL[RightStrength+5],9)
+","+Numtostr(FRAMAindBXL[RightStrength+6],9)
+","+Numtostr(FRAMAindBXL[RightStrength+7],9)
+","+Numtostr(FRAMAindBXL[RightStrength+8],9)
+","+Numtostr(FRAMAindBXL[RightStrength+9],9)
+","+Numtostr(FRAMAindBXL[RightStrength-1],9)
+","+Numtostr(FRAMAindBXL[RightStrength-2],9)
+","+Numtostr(FRAMAindBXL[RightStrength-3],9)
+","+Numtostr(FRAMAindBXL[RightStrength-4],9)
+","+Numtostr(FRAMAindMED[RightStrength+0],9)
+","+Numtostr(FRAMAindMED[RightStrength+1],9)
+","+Numtostr(FRAMAindMED[RightStrength+10],9)
+","+Numtostr(FRAMAindMED[RightStrength+11],9)
+","+Numtostr(FRAMAindMED[RightStrength+12],9)
+","+Numtostr(FRAMAindMED[RightStrength+13],9)
+","+Numtostr(FRAMAindMED[RightStrength+14],9)
+","+Numtostr(FRAMAindMED[RightStrength+15],9)
+","+Numtostr(FRAMAindMED[RightStrength+16],9)
+","+Numtostr(FRAMAindMED[RightStrength+17],9)
+","+Numtostr(FRAMAindMED[RightStrength+18],9)
+","+Numtostr(FRAMAindMED[RightStrength+19],9)
+","+Numtostr(FRAMAindMED[RightStrength+2],9)
+","+Numtostr(FRAMAindMED[RightStrength+20],9)
+","+Numtostr(FRAMAindMED[RightStrength+21],9)
+","+Numtostr(FRAMAindMED[RightStrength+22],9)
+","+Numtostr(FRAMAindMED[RightStrength+23],9)
+","+Numtostr(FRAMAindMED[RightStrength+24],9)
+","+Numtostr(FRAMAindMED[RightStrength+25],9)
+","+Numtostr(FRAMAindMED[RightStrength+26],9)
+","+Numtostr(FRAMAindMED[RightStrength+27],9)
+","+Numtostr(FRAMAindMED[RightStrength+28],9)
+","+Numtostr(FRAMAindMED[RightStrength+3],9)
+","+Numtostr(FRAMAindMED[RightStrength+4],9)
+","+Numtostr(FRAMAindMED[RightStrength+5],9)
+","+Numtostr(FRAMAindMED[RightStrength+6],9)
+","+Numtostr(FRAMAindMED[RightStrength+7],9)
+","+Numtostr(FRAMAindMED[RightStrength+8],9)
+","+Numtostr(FRAMAindMED[RightStrength+9],9)
+","+Numtostr(FRAMAindMED[RightStrength-1],9)
+","+Numtostr(FRAMAindMED[RightStrength-2],9)
+","+Numtostr(FRAMAindMED[RightStrength-3],9)
+","+Numtostr(FRAMAindMED[RightStrength-4],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+0],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+1],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+10],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+11],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+12],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+13],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+14],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+15],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+16],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+17],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+18],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+19],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+2],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+20],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+21],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+22],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+23],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+24],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+25],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+26],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+27],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+28],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+3],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+4],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+5],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+6],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+7],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+8],9)
+","+Numtostr(FRAMAindSMALL[RightStrength+9],9)
+","+Numtostr(FRAMAindSMALL[RightStrength-1],9)
+","+Numtostr(FRAMAindSMALL[RightStrength-2],9)
+","+Numtostr(FRAMAindSMALL[RightStrength-3],9)
+","+Numtostr(FRAMAindSMALL[RightStrength-4],9)
+","+Numtostr(KLTNRindBIG[RightStrength+0],9)
+","+Numtostr(KLTNRindBIG[RightStrength+1],9)
+","+Numtostr(KLTNRindBIG[RightStrength+10],9)
+","+Numtostr(KLTNRindBIG[RightStrength+11],9)
+","+Numtostr(KLTNRindBIG[RightStrength+12],9)
+","+Numtostr(KLTNRindBIG[RightStrength+13],9)
+","+Numtostr(KLTNRindBIG[RightStrength+14],9)
+","+Numtostr(KLTNRindBIG[RightStrength+15],9)
+","+Numtostr(KLTNRindBIG[RightStrength+16],9)
+","+Numtostr(KLTNRindBIG[RightStrength+17],9)
+","+Numtostr(KLTNRindBIG[RightStrength+18],9)
+","+Numtostr(KLTNRindBIG[RightStrength+19],9)
+","+Numtostr(KLTNRindBIG[RightStrength+2],9)
+","+Numtostr(KLTNRindBIG[RightStrength+20],9)
+","+Numtostr(KLTNRindBIG[RightStrength+21],9)
+","+Numtostr(KLTNRindBIG[RightStrength+22],9)
+","+Numtostr(KLTNRindBIG[RightStrength+23],9)
+","+Numtostr(KLTNRindBIG[RightStrength+24],9)
+","+Numtostr(KLTNRindBIG[RightStrength+25],9)
+","+Numtostr(KLTNRindBIG[RightStrength+26],9)
+","+Numtostr(KLTNRindBIG[RightStrength+27],9)
+","+Numtostr(KLTNRindBIG[RightStrength+28],9)
+","+Numtostr(KLTNRindBIG[RightStrength+3],9)
+","+Numtostr(KLTNRindBIG[RightStrength+4],9)
+","+Numtostr(KLTNRindBIG[RightStrength+5],9)
+","+Numtostr(KLTNRindBIG[RightStrength+6],9)
+","+Numtostr(KLTNRindBIG[RightStrength+7],9)
+","+Numtostr(KLTNRindBIG[RightStrength+8],9)
+","+Numtostr(KLTNRindBIG[RightStrength+9],9)
+","+Numtostr(KLTNRindBIG[RightStrength-1],9)
+","+Numtostr(KLTNRindBIG[RightStrength-2],9)
+","+Numtostr(KLTNRindBIG[RightStrength-3],9)
+","+Numtostr(KLTNRindBIG[RightStrength-4],9)
+","+Numtostr(KLTNRindBX[RightStrength+0],9)
+","+Numtostr(KLTNRindBX[RightStrength+1],9)
+","+Numtostr(KLTNRindBX[RightStrength+10],9)
+","+Numtostr(KLTNRindBX[RightStrength+11],9)
+","+Numtostr(KLTNRindBX[RightStrength+12],9)
+","+Numtostr(KLTNRindBX[RightStrength+13],9)
+","+Numtostr(KLTNRindBX[RightStrength+14],9)
+","+Numtostr(KLTNRindBX[RightStrength+15],9)
+","+Numtostr(KLTNRindBX[RightStrength+16],9)
+","+Numtostr(KLTNRindBX[RightStrength+17],9)
+","+Numtostr(KLTNRindBX[RightStrength+18],9)
+","+Numtostr(KLTNRindBX[RightStrength+19],9)
+","+Numtostr(KLTNRindBX[RightStrength+2],9)
+","+Numtostr(KLTNRindBX[RightStrength+20],9)
+","+Numtostr(KLTNRindBX[RightStrength+21],9)
+","+Numtostr(KLTNRindBX[RightStrength+22],9)
+","+Numtostr(KLTNRindBX[RightStrength+23],9)
+","+Numtostr(KLTNRindBX[RightStrength+24],9)
+","+Numtostr(KLTNRindBX[RightStrength+25],9)
+","+Numtostr(KLTNRindBX[RightStrength+26],9)
+","+Numtostr(KLTNRindBX[RightStrength+27],9)
+","+Numtostr(KLTNRindBX[RightStrength+28],9)
+","+Numtostr(KLTNRindBX[RightStrength+3],9)
+","+Numtostr(KLTNRindBX[RightStrength+4],9)
+","+Numtostr(KLTNRindBX[RightStrength+5],9)
+","+Numtostr(KLTNRindBX[RightStrength+6],9)
+","+Numtostr(KLTNRindBX[RightStrength+7],9)
+","+Numtostr(KLTNRindBX[RightStrength+8],9)
+","+Numtostr(KLTNRindBX[RightStrength+9],9)
+","+Numtostr(KLTNRindBX[RightStrength-1],9)
+","+Numtostr(KLTNRindBX[RightStrength-2],9)
+","+Numtostr(KLTNRindBX[RightStrength-3],9)
+","+Numtostr(KLTNRindBX[RightStrength-4],9)
+","+Numtostr(KLTNRindBXL[RightStrength+0],9)
+","+Numtostr(KLTNRindBXL[RightStrength+1],9)
+","+Numtostr(KLTNRindBXL[RightStrength+10],9)
+","+Numtostr(KLTNRindBXL[RightStrength+11],9)
+","+Numtostr(KLTNRindBXL[RightStrength+12],9)
+","+Numtostr(KLTNRindBXL[RightStrength+13],9)
+","+Numtostr(KLTNRindBXL[RightStrength+14],9)
+","+Numtostr(KLTNRindBXL[RightStrength+15],9)
+","+Numtostr(KLTNRindBXL[RightStrength+16],9)
+","+Numtostr(KLTNRindBXL[RightStrength+17],9)
+","+Numtostr(KLTNRindBXL[RightStrength+18],9)
+","+Numtostr(KLTNRindBXL[RightStrength+19],9)
+","+Numtostr(KLTNRindBXL[RightStrength+2],9)
+","+Numtostr(KLTNRindBXL[RightStrength+20],9)
+","+Numtostr(KLTNRindBXL[RightStrength+21],9)
+","+Numtostr(KLTNRindBXL[RightStrength+22],9)
+","+Numtostr(KLTNRindBXL[RightStrength+23],9)
+","+Numtostr(KLTNRindBXL[RightStrength+24],9)
+","+Numtostr(KLTNRindBXL[RightStrength+25],9)
+","+Numtostr(KLTNRindBXL[RightStrength+26],9)
+","+Numtostr(KLTNRindBXL[RightStrength+27],9)
+","+Numtostr(KLTNRindBXL[RightStrength+28],9)
+","+Numtostr(KLTNRindBXL[RightStrength+3],9)
+","+Numtostr(KLTNRindBXL[RightStrength+4],9)
+","+Numtostr(KLTNRindBXL[RightStrength+5],9)
+","+Numtostr(KLTNRindBXL[RightStrength+6],9)
+","+Numtostr(KLTNRindBXL[RightStrength+7],9)
+","+Numtostr(KLTNRindBXL[RightStrength+8],9)
+","+Numtostr(KLTNRindBXL[RightStrength+9],9)
+","+Numtostr(KLTNRindBXL[RightStrength-1],9)
+","+Numtostr(KLTNRindBXL[RightStrength-2],9)
+","+Numtostr(KLTNRindBXL[RightStrength-3],9)
+","+Numtostr(KLTNRindBXL[RightStrength-4],9)
+","+Numtostr(KLTNRindMED[RightStrength+0],9)
+","+Numtostr(KLTNRindMED[RightStrength+1],9)
+","+Numtostr(KLTNRindMED[RightStrength+10],9)
+","+Numtostr(KLTNRindMED[RightStrength+11],9)
+","+Numtostr(KLTNRindMED[RightStrength+12],9)
+","+Numtostr(KLTNRindMED[RightStrength+13],9)
+","+Numtostr(KLTNRindMED[RightStrength+14],9)
+","+Numtostr(KLTNRindMED[RightStrength+15],9)
+","+Numtostr(KLTNRindMED[RightStrength+16],9)
+","+Numtostr(KLTNRindMED[RightStrength+17],9)
+","+Numtostr(KLTNRindMED[RightStrength+18],9)
+","+Numtostr(KLTNRindMED[RightStrength+19],9)
+","+Numtostr(KLTNRindMED[RightStrength+2],9)
+","+Numtostr(KLTNRindMED[RightStrength+20],9)
+","+Numtostr(KLTNRindMED[RightStrength+21],9)
+","+Numtostr(KLTNRindMED[RightStrength+22],9)
+","+Numtostr(KLTNRindMED[RightStrength+23],9)
+","+Numtostr(KLTNRindMED[RightStrength+24],9)
+","+Numtostr(KLTNRindMED[RightStrength+25],9)
+","+Numtostr(KLTNRindMED[RightStrength+26],9)
+","+Numtostr(KLTNRindMED[RightStrength+27],9)
+","+Numtostr(KLTNRindMED[RightStrength+28],9)
+","+Numtostr(KLTNRindMED[RightStrength+3],9)
+","+Numtostr(KLTNRindMED[RightStrength+4],9)
+","+Numtostr(KLTNRindMED[RightStrength+5],9)
+","+Numtostr(KLTNRindMED[RightStrength+6],9)
+","+Numtostr(KLTNRindMED[RightStrength+7],9)
+","+Numtostr(KLTNRindMED[RightStrength+8],9)
+","+Numtostr(KLTNRindMED[RightStrength+9],9)
+","+Numtostr(KLTNRindMED[RightStrength-1],9)
+","+Numtostr(KLTNRindMED[RightStrength-2],9)
+","+Numtostr(KLTNRindMED[RightStrength-3],9)
+","+Numtostr(KLTNRindMED[RightStrength-4],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+0],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+1],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+10],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+11],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+12],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+13],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+14],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+15],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+16],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+17],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+18],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+19],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+2],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+20],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+21],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+22],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+23],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+24],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+25],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+26],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+27],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+28],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+3],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+4],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+5],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+6],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+7],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+8],9)
+","+Numtostr(KLTNRindSMALL[RightStrength+9],9)
+","+Numtostr(KLTNRindSMALL[RightStrength-1],9)
+","+Numtostr(KLTNRindSMALL[RightStrength-2],9)
+","+Numtostr(KLTNRindSMALL[RightStrength-3],9)
+","+Numtostr(KLTNRindSMALL[RightStrength-4],9)
+","+Numtostr(KSTindBIG[RightStrength+0],9)
+","+Numtostr(KSTindBIG[RightStrength+1],9)
+","+Numtostr(KSTindBIG[RightStrength+10],9)
+","+Numtostr(KSTindBIG[RightStrength+11],9)
+","+Numtostr(KSTindBIG[RightStrength+12],9)
+","+Numtostr(KSTindBIG[RightStrength+13],9)
+","+Numtostr(KSTindBIG[RightStrength+14],9)
+","+Numtostr(KSTindBIG[RightStrength+15],9)
+","+Numtostr(KSTindBIG[RightStrength+16],9)
+","+Numtostr(KSTindBIG[RightStrength+17],9)
+","+Numtostr(KSTindBIG[RightStrength+18],9)
+","+Numtostr(KSTindBIG[RightStrength+19],9)
+","+Numtostr(KSTindBIG[RightStrength+2],9)
+","+Numtostr(KSTindBIG[RightStrength+20],9)
+","+Numtostr(KSTindBIG[RightStrength+21],9)
+","+Numtostr(KSTindBIG[RightStrength+22],9)
+","+Numtostr(KSTindBIG[RightStrength+23],9)
+","+Numtostr(KSTindBIG[RightStrength+24],9)
+","+Numtostr(KSTindBIG[RightStrength+25],9)
+","+Numtostr(KSTindBIG[RightStrength+26],9)
+","+Numtostr(KSTindBIG[RightStrength+27],9)
+","+Numtostr(KSTindBIG[RightStrength+28],9)
+","+Numtostr(KSTindBIG[RightStrength+3],9)
+","+Numtostr(KSTindBIG[RightStrength+4],9)
+","+Numtostr(KSTindBIG[RightStrength+5],9)
+","+Numtostr(KSTindBIG[RightStrength+6],9)
+","+Numtostr(KSTindBIG[RightStrength+7],9)
+","+Numtostr(KSTindBIG[RightStrength+8],9)
+","+Numtostr(KSTindBIG[RightStrength+9],9)
+","+Numtostr(KSTindBIG[RightStrength-1],9)
+","+Numtostr(KSTindBIG[RightStrength-2],9)
+","+Numtostr(KSTindBIG[RightStrength-3],9)
+","+Numtostr(KSTindBIG[RightStrength-4],9)
+","+Numtostr(KSTindBX[RightStrength+0],9)
+","+Numtostr(KSTindBX[RightStrength+1],9)
+","+Numtostr(KSTindBX[RightStrength+10],9)
+","+Numtostr(KSTindBX[RightStrength+11],9)
+","+Numtostr(KSTindBX[RightStrength+12],9)
+","+Numtostr(KSTindBX[RightStrength+13],9)
+","+Numtostr(KSTindBX[RightStrength+14],9)
+","+Numtostr(KSTindBX[RightStrength+15],9)
+","+Numtostr(KSTindBX[RightStrength+16],9)
+","+Numtostr(KSTindBX[RightStrength+17],9)
+","+Numtostr(KSTindBX[RightStrength+18],9)
+","+Numtostr(KSTindBX[RightStrength+19],9)
+","+Numtostr(KSTindBX[RightStrength+2],9)
+","+Numtostr(KSTindBX[RightStrength+20],9)
+","+Numtostr(KSTindBX[RightStrength+21],9)
+","+Numtostr(KSTindBX[RightStrength+22],9)
+","+Numtostr(KSTindBX[RightStrength+23],9)
+","+Numtostr(KSTindBX[RightStrength+24],9)
+","+Numtostr(KSTindBX[RightStrength+25],9)
+","+Numtostr(KSTindBX[RightStrength+26],9)
+","+Numtostr(KSTindBX[RightStrength+27],9)
+","+Numtostr(KSTindBX[RightStrength+28],9)
+","+Numtostr(KSTindBX[RightStrength+3],9)
+","+Numtostr(KSTindBX[RightStrength+4],9)
+","+Numtostr(KSTindBX[RightStrength+5],9)
+","+Numtostr(KSTindBX[RightStrength+6],9)
+","+Numtostr(KSTindBX[RightStrength+7],9)
+","+Numtostr(KSTindBX[RightStrength+8],9)
+","+Numtostr(KSTindBX[RightStrength+9],9)
+","+Numtostr(KSTindBX[RightStrength-1],9)
+","+Numtostr(KSTindBX[RightStrength-2],9)
+","+Numtostr(KSTindBX[RightStrength-3],9)
+","+Numtostr(KSTindBX[RightStrength-4],9)
+","+Numtostr(KSTindBXL[RightStrength+0],9)
+","+Numtostr(KSTindBXL[RightStrength+1],9)
+","+Numtostr(KSTindBXL[RightStrength+10],9)
+","+Numtostr(KSTindBXL[RightStrength+11],9)
+","+Numtostr(KSTindBXL[RightStrength+12],9)
+","+Numtostr(KSTindBXL[RightStrength+13],9)
+","+Numtostr(KSTindBXL[RightStrength+14],9)
+","+Numtostr(KSTindBXL[RightStrength+15],9)
+","+Numtostr(KSTindBXL[RightStrength+16],9)
+","+Numtostr(KSTindBXL[RightStrength+17],9)
+","+Numtostr(KSTindBXL[RightStrength+18],9)
+","+Numtostr(KSTindBXL[RightStrength+19],9)
+","+Numtostr(KSTindBXL[RightStrength+2],9)
+","+Numtostr(KSTindBXL[RightStrength+20],9)
+","+Numtostr(KSTindBXL[RightStrength+21],9)
+","+Numtostr(KSTindBXL[RightStrength+22],9)
+","+Numtostr(KSTindBXL[RightStrength+23],9)
+","+Numtostr(KSTindBXL[RightStrength+24],9)
+","+Numtostr(KSTindBXL[RightStrength+25],9)
+","+Numtostr(KSTindBXL[RightStrength+26],9)
+","+Numtostr(KSTindBXL[RightStrength+27],9)
+","+Numtostr(KSTindBXL[RightStrength+28],9)
+","+Numtostr(KSTindBXL[RightStrength+3],9)
+","+Numtostr(KSTindBXL[RightStrength+4],9)
+","+Numtostr(KSTindBXL[RightStrength+5],9)
+","+Numtostr(KSTindBXL[RightStrength+6],9)
+","+Numtostr(KSTindBXL[RightStrength+7],9)
+","+Numtostr(KSTindBXL[RightStrength+8],9)
+","+Numtostr(KSTindBXL[RightStrength+9],9)
+","+Numtostr(KSTindBXL[RightStrength-1],9)
+","+Numtostr(KSTindBXL[RightStrength-2],9)
+","+Numtostr(KSTindBXL[RightStrength-3],9)
+","+Numtostr(KSTindBXL[RightStrength-4],9)
+","+Numtostr(KSTindMED[RightStrength+0],9)
+","+Numtostr(KSTindMED[RightStrength+1],9)
+","+Numtostr(KSTindMED[RightStrength+10],9)
+","+Numtostr(KSTindMED[RightStrength+11],9)
+","+Numtostr(KSTindMED[RightStrength+12],9)
+","+Numtostr(KSTindMED[RightStrength+13],9)
+","+Numtostr(KSTindMED[RightStrength+14],9)
+","+Numtostr(KSTindMED[RightStrength+15],9)
+","+Numtostr(KSTindMED[RightStrength+16],9)
+","+Numtostr(KSTindMED[RightStrength+17],9)
+","+Numtostr(KSTindMED[RightStrength+18],9)
+","+Numtostr(KSTindMED[RightStrength+19],9)
+","+Numtostr(KSTindMED[RightStrength+2],9)
+","+Numtostr(KSTindMED[RightStrength+20],9)
+","+Numtostr(KSTindMED[RightStrength+21],9)
+","+Numtostr(KSTindMED[RightStrength+22],9)
+","+Numtostr(KSTindMED[RightStrength+23],9)
+","+Numtostr(KSTindMED[RightStrength+24],9)
+","+Numtostr(KSTindMED[RightStrength+25],9)
+","+Numtostr(KSTindMED[RightStrength+26],9)
+","+Numtostr(KSTindMED[RightStrength+27],9)
+","+Numtostr(KSTindMED[RightStrength+28],9)
+","+Numtostr(KSTindMED[RightStrength+3],9)
+","+Numtostr(KSTindMED[RightStrength+4],9)
+","+Numtostr(KSTindMED[RightStrength+5],9)
+","+Numtostr(KSTindMED[RightStrength+6],9)
+","+Numtostr(KSTindMED[RightStrength+7],9)
+","+Numtostr(KSTindMED[RightStrength+8],9)
+","+Numtostr(KSTindMED[RightStrength+9],9)
+","+Numtostr(KSTindMED[RightStrength-1],9)
+","+Numtostr(KSTindMED[RightStrength-2],9)
+","+Numtostr(KSTindMED[RightStrength-3],9)
+","+Numtostr(KSTindMED[RightStrength-4],9)
+","+Numtostr(KSTindSMALL[RightStrength+0],9)
+","+Numtostr(KSTindSMALL[RightStrength+1],9)
+","+Numtostr(KSTindSMALL[RightStrength+10],9)
+","+Numtostr(KSTindSMALL[RightStrength+11],9)
+","+Numtostr(KSTindSMALL[RightStrength+12],9)
+","+Numtostr(KSTindSMALL[RightStrength+13],9)
+","+Numtostr(KSTindSMALL[RightStrength+14],9)
+","+Numtostr(KSTindSMALL[RightStrength+15],9)
+","+Numtostr(KSTindSMALL[RightStrength+16],9)
+","+Numtostr(KSTindSMALL[RightStrength+17],9)
+","+Numtostr(KSTindSMALL[RightStrength+18],9)
+","+Numtostr(KSTindSMALL[RightStrength+19],9)
+","+Numtostr(KSTindSMALL[RightStrength+2],9)
+","+Numtostr(KSTindSMALL[RightStrength+20],9)
+","+Numtostr(KSTindSMALL[RightStrength+21],9)
+","+Numtostr(KSTindSMALL[RightStrength+22],9)
+","+Numtostr(KSTindSMALL[RightStrength+23],9)
+","+Numtostr(KSTindSMALL[RightStrength+24],9)
+","+Numtostr(KSTindSMALL[RightStrength+25],9)
+","+Numtostr(KSTindSMALL[RightStrength+26],9)
+","+Numtostr(KSTindSMALL[RightStrength+27],9)
+","+Numtostr(KSTindSMALL[RightStrength+28],9)
+","+Numtostr(KSTindSMALL[RightStrength+3],9)
+","+Numtostr(KSTindSMALL[RightStrength+4],9)
+","+Numtostr(KSTindSMALL[RightStrength+5],9)
+","+Numtostr(KSTindSMALL[RightStrength+6],9)
+","+Numtostr(KSTindSMALL[RightStrength+7],9)
+","+Numtostr(KSTindSMALL[RightStrength+8],9)
+","+Numtostr(KSTindSMALL[RightStrength+9],9)
+","+Numtostr(KSTindSMALL[RightStrength-1],9)
+","+Numtostr(KSTindSMALL[RightStrength-2],9)
+","+Numtostr(KSTindSMALL[RightStrength-3],9)
+","+Numtostr(KSTindSMALL[RightStrength-4],9)
+","+Numtostr(LINREGindBIG[RightStrength+0],9)
+","+Numtostr(LINREGindBIG[RightStrength+1],9)
+","+Numtostr(LINREGindBIG[RightStrength+10],9)
+","+Numtostr(LINREGindBIG[RightStrength+11],9)
+","+Numtostr(LINREGindBIG[RightStrength+12],9)
+","+Numtostr(LINREGindBIG[RightStrength+13],9)
+","+Numtostr(LINREGindBIG[RightStrength+14],9)
+","+Numtostr(LINREGindBIG[RightStrength+15],9)
+","+Numtostr(LINREGindBIG[RightStrength+16],9)
+","+Numtostr(LINREGindBIG[RightStrength+17],9)
+","+Numtostr(LINREGindBIG[RightStrength+18],9)
+","+Numtostr(LINREGindBIG[RightStrength+19],9)
+","+Numtostr(LINREGindBIG[RightStrength+2],9)
+","+Numtostr(LINREGindBIG[RightStrength+20],9)
+","+Numtostr(LINREGindBIG[RightStrength+21],9)
+","+Numtostr(LINREGindBIG[RightStrength+22],9)
+","+Numtostr(LINREGindBIG[RightStrength+23],9)
+","+Numtostr(LINREGindBIG[RightStrength+24],9)
+","+Numtostr(LINREGindBIG[RightStrength+25],9)
+","+Numtostr(LINREGindBIG[RightStrength+26],9)
+","+Numtostr(LINREGindBIG[RightStrength+27],9)
+","+Numtostr(LINREGindBIG[RightStrength+28],9)
+","+Numtostr(LINREGindBIG[RightStrength+3],9)
+","+Numtostr(LINREGindBIG[RightStrength+4],9)
+","+Numtostr(LINREGindBIG[RightStrength+5],9)
+","+Numtostr(LINREGindBIG[RightStrength+6],9)
+","+Numtostr(LINREGindBIG[RightStrength+7],9)
+","+Numtostr(LINREGindBIG[RightStrength+8],9)
+","+Numtostr(LINREGindBIG[RightStrength+9],9)
+","+Numtostr(LINREGindBIG[RightStrength-1],9)
			+" )"; 
			res3v02 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_feat3v02);
			srun3v02 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures3v02.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_feat3v02
					+ NewLine);
			end;
		End;
	End;
	 TblNum = TblNum+1;


	//(4) insert feature data into dbo.barFeatures4v02	
	if ReadWriteToDB and TblNum = 11+4  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		if  srun4v02 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_feat4v02 = "INSERT INTO  [dbo].[barFeatures4v02] VALUES ("
			+queryResult//MAX(loadId) 
			+","+Numtostr(loadGroupId,0) 
			+","+Numtostr(RightStrength,0)//RightStrength
			+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
			+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"'"+dateBarOpened+"' "+","+"'"+dayOfWeekBarOpened+"' "+","+"'"+dateTimeBarOpened+"' "+","+"'"+timeBarOpenedhhmmss+"' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)
+","+Numtostr(MaALMAindBX[RightStrength+10],9)
+","+Numtostr(MaALMAindBX[RightStrength+11],9)
+","+Numtostr(MaALMAindBX[RightStrength+12],9)
+","+Numtostr(MaALMAindBX[RightStrength+13],9)
+","+Numtostr(MaALMAindBX[RightStrength+14],9)
+","+Numtostr(MaALMAindBX[RightStrength+15],9)
+","+Numtostr(MaALMAindBX[RightStrength+16],9)
+","+Numtostr(MaALMAindBX[RightStrength+17],9)
+","+Numtostr(MaALMAindBX[RightStrength+18],9)
+","+Numtostr(MaALMAindBX[RightStrength+19],9)
+","+Numtostr(MaALMAindBX[RightStrength+2],9)
+","+Numtostr(MaALMAindBX[RightStrength+20],9)
+","+Numtostr(MaALMAindBX[RightStrength+21],9)
+","+Numtostr(MaALMAindBX[RightStrength+22],9)
+","+Numtostr(MaALMAindBX[RightStrength+23],9)
+","+Numtostr(MaALMAindBX[RightStrength+24],9)
+","+Numtostr(MaALMAindBX[RightStrength+25],9)
+","+Numtostr(MaALMAindBX[RightStrength+26],9)
+","+Numtostr(MaALMAindBX[RightStrength+27],9)
+","+Numtostr(MaALMAindBX[RightStrength+28],9)
+","+Numtostr(MaALMAindBX[RightStrength+3],9)
+","+Numtostr(MaALMAindBX[RightStrength+4],9)
+","+Numtostr(MaALMAindBX[RightStrength+5],9)
+","+Numtostr(MaALMAindBX[RightStrength+6],9)
+","+Numtostr(MaALMAindBX[RightStrength+7],9)
+","+Numtostr(MaALMAindBX[RightStrength+8],9)
+","+Numtostr(MaALMAindBX[RightStrength+9],9)
+","+Numtostr(MaALMAindBX[RightStrength-1],9)
+","+Numtostr(MaALMAindBX[RightStrength-2],9)
+","+Numtostr(MaALMAindBX[RightStrength-3],9)
+","+Numtostr(MaALMAindBX[RightStrength-4],9)
+","+Numtostr(MaALMAindBXL[RightStrength+0],9)
+","+Numtostr(MaALMAindBXL[RightStrength+1],9)
+","+Numtostr(MaALMAindBXL[RightStrength+10],9)
+","+Numtostr(MaALMAindBXL[RightStrength+11],9)
+","+Numtostr(MaALMAindBXL[RightStrength+12],9)
+","+Numtostr(MaALMAindBXL[RightStrength+13],9)
+","+Numtostr(MaALMAindBXL[RightStrength+14],9)
+","+Numtostr(MaALMAindBXL[RightStrength+15],9)
+","+Numtostr(MaALMAindBXL[RightStrength+16],9)
+","+Numtostr(MaALMAindBXL[RightStrength+17],9)
+","+Numtostr(MaALMAindBXL[RightStrength+18],9)
+","+Numtostr(MaALMAindBXL[RightStrength+19],9)
+","+Numtostr(MaALMAindBXL[RightStrength+2],9)
+","+Numtostr(MaALMAindBXL[RightStrength+20],9)
+","+Numtostr(MaALMAindBXL[RightStrength+21],9)
+","+Numtostr(MaALMAindBXL[RightStrength+22],9)
+","+Numtostr(MaALMAindBXL[RightStrength+23],9)
+","+Numtostr(MaALMAindBXL[RightStrength+24],9)
+","+Numtostr(MaALMAindBXL[RightStrength+25],9)
+","+Numtostr(MaALMAindBXL[RightStrength+26],9)
+","+Numtostr(MaALMAindBXL[RightStrength+27],9)
+","+Numtostr(MaALMAindBXL[RightStrength+28],9)
+","+Numtostr(MaALMAindBXL[RightStrength+3],9)
+","+Numtostr(MaALMAindBXL[RightStrength+4],9)
+","+Numtostr(MaALMAindBXL[RightStrength+5],9)
+","+Numtostr(MaALMAindBXL[RightStrength+6],9)
+","+Numtostr(MaALMAindBXL[RightStrength+7],9)
+","+Numtostr(MaALMAindBXL[RightStrength+8],9)
+","+Numtostr(MaALMAindBXL[RightStrength+9],9)
+","+Numtostr(MaALMAindBXL[RightStrength-1],9)
+","+Numtostr(MaALMAindBXL[RightStrength-2],9)
+","+Numtostr(MaALMAindBXL[RightStrength-3],9)
+","+Numtostr(MaALMAindBXL[RightStrength-4],9)
+","+Numtostr(MaALMAindMED[RightStrength+0],9)
+","+Numtostr(MaALMAindMED[RightStrength+1],9)
+","+Numtostr(MaALMAindMED[RightStrength+10],9)
+","+Numtostr(MaALMAindMED[RightStrength+11],9)
+","+Numtostr(MaALMAindMED[RightStrength+12],9)
+","+Numtostr(MaALMAindMED[RightStrength+13],9)
+","+Numtostr(MaALMAindMED[RightStrength+14],9)
+","+Numtostr(MaALMAindMED[RightStrength+15],9)
+","+Numtostr(MaALMAindMED[RightStrength+16],9)
+","+Numtostr(MaALMAindMED[RightStrength+17],9)
+","+Numtostr(MaALMAindMED[RightStrength+18],9)
+","+Numtostr(MaALMAindMED[RightStrength+19],9)
+","+Numtostr(MaALMAindMED[RightStrength+2],9)
+","+Numtostr(MaALMAindMED[RightStrength+20],9)
+","+Numtostr(MaALMAindMED[RightStrength+21],9)
+","+Numtostr(MaALMAindMED[RightStrength+22],9)
+","+Numtostr(MaALMAindMED[RightStrength+23],9)
+","+Numtostr(MaALMAindMED[RightStrength+24],9)
+","+Numtostr(MaALMAindMED[RightStrength+25],9)
+","+Numtostr(MaALMAindMED[RightStrength+26],9)
+","+Numtostr(MaALMAindMED[RightStrength+27],9)
+","+Numtostr(MaALMAindMED[RightStrength+28],9)
+","+Numtostr(MaALMAindMED[RightStrength+3],9)
+","+Numtostr(MaALMAindMED[RightStrength+4],9)
+","+Numtostr(MaALMAindMED[RightStrength+5],9)
+","+Numtostr(MaALMAindMED[RightStrength+6],9)
+","+Numtostr(MaALMAindMED[RightStrength+7],9)
+","+Numtostr(MaALMAindMED[RightStrength+8],9)
+","+Numtostr(MaALMAindMED[RightStrength+9],9)
+","+Numtostr(MaALMAindMED[RightStrength-1],9)
+","+Numtostr(MaALMAindMED[RightStrength-2],9)
+","+Numtostr(MaALMAindMED[RightStrength-3],9)
+","+Numtostr(MaALMAindMED[RightStrength-4],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+0],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+1],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+10],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+11],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+12],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+13],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+14],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+15],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+16],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+17],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+18],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+19],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+2],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+20],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+21],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+22],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+23],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+24],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+25],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+26],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+27],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+28],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+3],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+4],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+5],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+6],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+7],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+8],9)
+","+Numtostr(MaALMAindSMALL[RightStrength+9],9)
+","+Numtostr(MaALMAindSMALL[RightStrength-1],9)
+","+Numtostr(MaALMAindSMALL[RightStrength-2],9)
+","+Numtostr(MaALMAindSMALL[RightStrength-3],9)
+","+Numtostr(MaALMAindSMALL[RightStrength-4],9)
+","+Numtostr(MaAMAindBIG[RightStrength+0],9)
+","+Numtostr(MaAMAindBIG[RightStrength+1],9)
+","+Numtostr(MaAMAindBIG[RightStrength+10],9)
+","+Numtostr(MaAMAindBIG[RightStrength+11],9)
+","+Numtostr(MaAMAindBIG[RightStrength+12],9)
+","+Numtostr(MaAMAindBIG[RightStrength+13],9)
+","+Numtostr(MaAMAindBIG[RightStrength+14],9)
+","+Numtostr(MaAMAindBIG[RightStrength+15],9)
+","+Numtostr(MaAMAindBIG[RightStrength+16],9)
+","+Numtostr(MaAMAindBIG[RightStrength+17],9)
+","+Numtostr(MaAMAindBIG[RightStrength+18],9)
+","+Numtostr(MaAMAindBIG[RightStrength+19],9)
+","+Numtostr(MaAMAindBIG[RightStrength+2],9)
+","+Numtostr(MaAMAindBIG[RightStrength+20],9)
+","+Numtostr(MaAMAindBIG[RightStrength+21],9)
+","+Numtostr(MaAMAindBIG[RightStrength+22],9)
+","+Numtostr(MaAMAindBIG[RightStrength+23],9)
+","+Numtostr(MaAMAindBIG[RightStrength+24],9)
+","+Numtostr(MaAMAindBIG[RightStrength+25],9)
+","+Numtostr(MaAMAindBIG[RightStrength+26],9)
+","+Numtostr(MaAMAindBIG[RightStrength+27],9)
+","+Numtostr(MaAMAindBIG[RightStrength+28],9)
+","+Numtostr(MaAMAindBIG[RightStrength+3],9)
+","+Numtostr(MaAMAindBIG[RightStrength+4],9)
+","+Numtostr(MaAMAindBIG[RightStrength+5],9)
+","+Numtostr(MaAMAindBIG[RightStrength+6],9)
+","+Numtostr(MaAMAindBIG[RightStrength+7],9)
+","+Numtostr(MaAMAindBIG[RightStrength+8],9)
+","+Numtostr(MaAMAindBIG[RightStrength+9],9)
+","+Numtostr(MaAMAindBIG[RightStrength-1],9)
+","+Numtostr(MaAMAindBIG[RightStrength-2],9)
+","+Numtostr(MaAMAindBIG[RightStrength-3],9)
+","+Numtostr(MaAMAindBIG[RightStrength-4],9)
+","+Numtostr(MaAMAindBX[RightStrength+0],9)
+","+Numtostr(MaAMAindBX[RightStrength+1],9)
+","+Numtostr(MaAMAindBX[RightStrength+10],9)
+","+Numtostr(MaAMAindBX[RightStrength+11],9)
+","+Numtostr(MaAMAindBX[RightStrength+12],9)
+","+Numtostr(MaAMAindBX[RightStrength+13],9)
+","+Numtostr(MaAMAindBX[RightStrength+14],9)
+","+Numtostr(MaAMAindBX[RightStrength+15],9)
+","+Numtostr(MaAMAindBX[RightStrength+16],9)
+","+Numtostr(MaAMAindBX[RightStrength+17],9)
+","+Numtostr(MaAMAindBX[RightStrength+18],9)
+","+Numtostr(MaAMAindBX[RightStrength+19],9)
+","+Numtostr(MaAMAindBX[RightStrength+2],9)
+","+Numtostr(MaAMAindBX[RightStrength+20],9)
+","+Numtostr(MaAMAindBX[RightStrength+21],9)
+","+Numtostr(MaAMAindBX[RightStrength+22],9)
+","+Numtostr(MaAMAindBX[RightStrength+23],9)
+","+Numtostr(MaAMAindBX[RightStrength+24],9)
+","+Numtostr(MaAMAindBX[RightStrength+25],9)
+","+Numtostr(MaAMAindBX[RightStrength+26],9)
+","+Numtostr(MaAMAindBX[RightStrength+27],9)
+","+Numtostr(MaAMAindBX[RightStrength+28],9)
+","+Numtostr(MaAMAindBX[RightStrength+3],9)
+","+Numtostr(MaAMAindBX[RightStrength+4],9)
+","+Numtostr(MaAMAindBX[RightStrength+5],9)
+","+Numtostr(MaAMAindBX[RightStrength+6],9)
+","+Numtostr(MaAMAindBX[RightStrength+7],9)
+","+Numtostr(MaAMAindBX[RightStrength+8],9)
+","+Numtostr(MaAMAindBX[RightStrength+9],9)
+","+Numtostr(MaAMAindBX[RightStrength-1],9)
+","+Numtostr(MaAMAindBX[RightStrength-2],9)
+","+Numtostr(MaAMAindBX[RightStrength-3],9)
+","+Numtostr(MaAMAindBX[RightStrength-4],9)
+","+Numtostr(MaAMAindBXL[RightStrength+0],9)
+","+Numtostr(MaAMAindBXL[RightStrength+1],9)
+","+Numtostr(MaAMAindBXL[RightStrength+10],9)
+","+Numtostr(MaAMAindBXL[RightStrength+11],9)
+","+Numtostr(MaAMAindBXL[RightStrength+12],9)
+","+Numtostr(MaAMAindBXL[RightStrength+13],9)
+","+Numtostr(MaAMAindBXL[RightStrength+14],9)
+","+Numtostr(MaAMAindBXL[RightStrength+15],9)
+","+Numtostr(MaAMAindBXL[RightStrength+16],9)
+","+Numtostr(MaAMAindBXL[RightStrength+17],9)
+","+Numtostr(MaAMAindBXL[RightStrength+18],9)
+","+Numtostr(MaAMAindBXL[RightStrength+19],9)
+","+Numtostr(MaAMAindBXL[RightStrength+2],9)
+","+Numtostr(MaAMAindBXL[RightStrength+20],9)
+","+Numtostr(MaAMAindBXL[RightStrength+21],9)
+","+Numtostr(MaAMAindBXL[RightStrength+22],9)
+","+Numtostr(MaAMAindBXL[RightStrength+23],9)
+","+Numtostr(MaAMAindBXL[RightStrength+24],9)
+","+Numtostr(MaAMAindBXL[RightStrength+25],9)
+","+Numtostr(MaAMAindBXL[RightStrength+26],9)
+","+Numtostr(MaAMAindBXL[RightStrength+27],9)
+","+Numtostr(MaAMAindBXL[RightStrength+28],9)
+","+Numtostr(MaAMAindBXL[RightStrength+3],9)
+","+Numtostr(MaAMAindBXL[RightStrength+4],9)
+","+Numtostr(MaAMAindBXL[RightStrength+5],9)
+","+Numtostr(MaAMAindBXL[RightStrength+6],9)
+","+Numtostr(MaAMAindBXL[RightStrength+7],9)
+","+Numtostr(MaAMAindBXL[RightStrength+8],9)
+","+Numtostr(MaAMAindBXL[RightStrength+9],9)
+","+Numtostr(MaAMAindBXL[RightStrength-1],9)
+","+Numtostr(MaAMAindBXL[RightStrength-2],9)
+","+Numtostr(MaAMAindBXL[RightStrength-3],9)
+","+Numtostr(MaAMAindBXL[RightStrength-4],9)
+","+Numtostr(MaAMAindMED[RightStrength+0],9)
+","+Numtostr(MaAMAindMED[RightStrength+1],9)
+","+Numtostr(MaAMAindMED[RightStrength+10],9)
+","+Numtostr(MaAMAindMED[RightStrength+11],9)
+","+Numtostr(MaAMAindMED[RightStrength+12],9)
+","+Numtostr(MaAMAindMED[RightStrength+13],9)
+","+Numtostr(MaAMAindMED[RightStrength+14],9)
+","+Numtostr(MaAMAindMED[RightStrength+15],9)
+","+Numtostr(MaAMAindMED[RightStrength+16],9)
+","+Numtostr(MaAMAindMED[RightStrength+17],9)
+","+Numtostr(MaAMAindMED[RightStrength+18],9)
+","+Numtostr(MaAMAindMED[RightStrength+19],9)
+","+Numtostr(MaAMAindMED[RightStrength+2],9)
+","+Numtostr(MaAMAindMED[RightStrength+20],9)
+","+Numtostr(MaAMAindMED[RightStrength+21],9)
+","+Numtostr(MaAMAindMED[RightStrength+22],9)
+","+Numtostr(MaAMAindMED[RightStrength+23],9)
+","+Numtostr(MaAMAindMED[RightStrength+24],9)
+","+Numtostr(MaAMAindMED[RightStrength+25],9)
+","+Numtostr(MaAMAindMED[RightStrength+26],9)
+","+Numtostr(MaAMAindMED[RightStrength+27],9)
+","+Numtostr(MaAMAindMED[RightStrength+28],9)
+","+Numtostr(MaAMAindMED[RightStrength+3],9)
+","+Numtostr(MaAMAindMED[RightStrength+4],9)
+","+Numtostr(MaAMAindMED[RightStrength+5],9)
+","+Numtostr(MaAMAindMED[RightStrength+6],9)
+","+Numtostr(MaAMAindMED[RightStrength+7],9)
+","+Numtostr(MaAMAindMED[RightStrength+8],9)
+","+Numtostr(MaAMAindMED[RightStrength+9],9)
+","+Numtostr(MaAMAindMED[RightStrength-1],9)
+","+Numtostr(MaAMAindMED[RightStrength-2],9)
+","+Numtostr(MaAMAindMED[RightStrength-3],9)
+","+Numtostr(MaAMAindMED[RightStrength-4],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+0],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+1],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+10],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+11],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+12],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+13],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+14],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+15],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+16],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+17],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+18],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+19],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+2],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+20],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+21],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+22],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+23],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+24],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+25],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+26],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+27],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+28],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+3],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+4],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+5],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+6],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+7],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+8],9)
+","+Numtostr(MaAMAindSMALL[RightStrength+9],9)
+","+Numtostr(MaAMAindSMALL[RightStrength-1],9)
+","+Numtostr(MaAMAindSMALL[RightStrength-2],9)
+","+Numtostr(MaAMAindSMALL[RightStrength-3],9)
+","+Numtostr(MaAMAindSMALL[RightStrength-4],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+0],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+1],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+10],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+11],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+12],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+13],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+14],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+15],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+16],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+17],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+18],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+19],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+2],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+20],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+21],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+22],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+23],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+24],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+25],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+26],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+27],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+28],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+3],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+4],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+5],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+6],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+7],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+8],9)
+","+Numtostr(MaDEMAindBIG[RightStrength+9],9)
+","+Numtostr(MaDEMAindBIG[RightStrength-1],9)
+","+Numtostr(MaDEMAindBIG[RightStrength-2],9)
+","+Numtostr(MaDEMAindBIG[RightStrength-3],9)
+","+Numtostr(MaDEMAindBIG[RightStrength-4],9)
+","+Numtostr(MaDEMAindBX[RightStrength+0],9)
+","+Numtostr(MaDEMAindBX[RightStrength+1],9)
+","+Numtostr(MaDEMAindBX[RightStrength+10],9)
+","+Numtostr(MaDEMAindBX[RightStrength+11],9)
+","+Numtostr(MaDEMAindBX[RightStrength+12],9)
+","+Numtostr(MaDEMAindBX[RightStrength+13],9)
+","+Numtostr(MaDEMAindBX[RightStrength+14],9)
+","+Numtostr(MaDEMAindBX[RightStrength+15],9)
+","+Numtostr(MaDEMAindBX[RightStrength+16],9)
+","+Numtostr(MaDEMAindBX[RightStrength+17],9)
+","+Numtostr(MaDEMAindBX[RightStrength+18],9)
+","+Numtostr(MaDEMAindBX[RightStrength+19],9)
+","+Numtostr(MaDEMAindBX[RightStrength+2],9)
+","+Numtostr(MaDEMAindBX[RightStrength+20],9)
+","+Numtostr(MaDEMAindBX[RightStrength+21],9)
+","+Numtostr(MaDEMAindBX[RightStrength+22],9)
+","+Numtostr(MaDEMAindBX[RightStrength+23],9)
+","+Numtostr(MaDEMAindBX[RightStrength+24],9)
+","+Numtostr(MaDEMAindBX[RightStrength+25],9)
+","+Numtostr(MaDEMAindBX[RightStrength+26],9)
+","+Numtostr(MaDEMAindBX[RightStrength+27],9)
+","+Numtostr(MaDEMAindBX[RightStrength+28],9)
+","+Numtostr(MaDEMAindBX[RightStrength+3],9)
+","+Numtostr(MaDEMAindBX[RightStrength+4],9)
+","+Numtostr(MaDEMAindBX[RightStrength+5],9)
+","+Numtostr(MaDEMAindBX[RightStrength+6],9)
+","+Numtostr(MaDEMAindBX[RightStrength+7],9)
+","+Numtostr(MaDEMAindBX[RightStrength+8],9)
+","+Numtostr(MaDEMAindBX[RightStrength+9],9)
+","+Numtostr(MaDEMAindBX[RightStrength-1],9)
+","+Numtostr(MaDEMAindBX[RightStrength-2],9)
+","+Numtostr(MaDEMAindBX[RightStrength-3],9)
+","+Numtostr(MaDEMAindBX[RightStrength-4],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+0],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+1],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+10],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+11],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+12],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+13],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+14],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+15],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+16],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+17],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+18],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+19],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+2],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+20],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+21],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+22],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+23],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+24],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+25],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+26],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+27],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+28],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+3],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+4],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+5],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+6],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+7],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+8],9)
+","+Numtostr(MaDEMAindBXL[RightStrength+9],9)
+","+Numtostr(MaDEMAindBXL[RightStrength-1],9)
+","+Numtostr(MaDEMAindBXL[RightStrength-2],9)
+","+Numtostr(MaDEMAindBXL[RightStrength-3],9)
+","+Numtostr(MaDEMAindBXL[RightStrength-4],9)
+","+Numtostr(MaDEMAindMED[RightStrength+0],9)
+","+Numtostr(MaDEMAindMED[RightStrength+1],9)
+","+Numtostr(MaDEMAindMED[RightStrength+10],9)
+","+Numtostr(MaDEMAindMED[RightStrength+11],9)
+","+Numtostr(MaDEMAindMED[RightStrength+12],9)
+","+Numtostr(MaDEMAindMED[RightStrength+13],9)
+","+Numtostr(MaDEMAindMED[RightStrength+14],9)
+","+Numtostr(MaDEMAindMED[RightStrength+15],9)
+","+Numtostr(MaDEMAindMED[RightStrength+16],9)
+","+Numtostr(MaDEMAindMED[RightStrength+17],9)
+","+Numtostr(MaDEMAindMED[RightStrength+18],9)
+","+Numtostr(MaDEMAindMED[RightStrength+19],9)
+","+Numtostr(MaDEMAindMED[RightStrength+2],9)
+","+Numtostr(MaDEMAindMED[RightStrength+20],9)
+","+Numtostr(MaDEMAindMED[RightStrength+21],9)
+","+Numtostr(MaDEMAindMED[RightStrength+22],9)
+","+Numtostr(MaDEMAindMED[RightStrength+23],9)
+","+Numtostr(MaDEMAindMED[RightStrength+24],9)
+","+Numtostr(MaDEMAindMED[RightStrength+25],9)
+","+Numtostr(MaDEMAindMED[RightStrength+26],9)
+","+Numtostr(MaDEMAindMED[RightStrength+27],9)
+","+Numtostr(MaDEMAindMED[RightStrength+28],9)
+","+Numtostr(MaDEMAindMED[RightStrength+3],9)
+","+Numtostr(MaDEMAindMED[RightStrength+4],9)
+","+Numtostr(MaDEMAindMED[RightStrength+5],9)
+","+Numtostr(MaDEMAindMED[RightStrength+6],9)
+","+Numtostr(MaDEMAindMED[RightStrength+7],9)
+","+Numtostr(MaDEMAindMED[RightStrength+8],9)
+","+Numtostr(MaDEMAindMED[RightStrength+9],9)
+","+Numtostr(MaDEMAindMED[RightStrength-1],9)
+","+Numtostr(MaDEMAindMED[RightStrength-2],9)
+","+Numtostr(MaDEMAindMED[RightStrength-3],9)
+","+Numtostr(MaDEMAindMED[RightStrength-4],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+0],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+1],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+10],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+11],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+12],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+13],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+14],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+15],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+16],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+17],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+18],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+19],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+2],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+20],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+21],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+22],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+23],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+24],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+25],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+26],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+27],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+28],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+3],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+4],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+5],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+6],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+7],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+8],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength+9],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength-1],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength-2],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength-3],9)
+","+Numtostr(MaDEMAindSMALL[RightStrength-4],9)
+","+Numtostr(MaEMAindBIG[RightStrength+0],9)
+","+Numtostr(MaEMAindBIG[RightStrength+1],9)
+","+Numtostr(MaEMAindBIG[RightStrength+10],9)
+","+Numtostr(MaEMAindBIG[RightStrength+11],9)
+","+Numtostr(MaEMAindBIG[RightStrength+12],9)
+","+Numtostr(MaEMAindBIG[RightStrength+13],9)
+","+Numtostr(MaEMAindBIG[RightStrength+14],9)
+","+Numtostr(MaEMAindBIG[RightStrength+15],9)
+","+Numtostr(MaEMAindBIG[RightStrength+16],9)
+","+Numtostr(MaEMAindBIG[RightStrength+17],9)
+","+Numtostr(MaEMAindBIG[RightStrength+18],9)
+","+Numtostr(MaEMAindBIG[RightStrength+19],9)
+","+Numtostr(MaEMAindBIG[RightStrength+2],9)
+","+Numtostr(MaEMAindBIG[RightStrength+20],9)
+","+Numtostr(MaEMAindBIG[RightStrength+21],9)
+","+Numtostr(MaEMAindBIG[RightStrength+22],9)
+","+Numtostr(MaEMAindBIG[RightStrength+23],9)
+","+Numtostr(MaEMAindBIG[RightStrength+24],9)
+","+Numtostr(MaEMAindBIG[RightStrength+25],9)
+","+Numtostr(MaEMAindBIG[RightStrength+26],9)
+","+Numtostr(MaEMAindBIG[RightStrength+27],9)
+","+Numtostr(MaEMAindBIG[RightStrength+28],9)
+","+Numtostr(MaEMAindBIG[RightStrength+3],9)
+","+Numtostr(MaEMAindBIG[RightStrength+4],9)
+","+Numtostr(MaEMAindBIG[RightStrength+5],9)
+","+Numtostr(MaEMAindBIG[RightStrength+6],9)
+","+Numtostr(MaEMAindBIG[RightStrength+7],9)
+","+Numtostr(MaEMAindBIG[RightStrength+8],9)
+","+Numtostr(MaEMAindBIG[RightStrength+9],9)
+","+Numtostr(MaEMAindBIG[RightStrength-1],9)
+","+Numtostr(MaEMAindBIG[RightStrength-2],9)
+","+Numtostr(MaEMAindBIG[RightStrength-3],9)
+","+Numtostr(MaEMAindBIG[RightStrength-4],9)
+","+Numtostr(MaEMAindBX[RightStrength+0],9)
+","+Numtostr(MaEMAindBX[RightStrength+1],9)
+","+Numtostr(MaEMAindBX[RightStrength+10],9)
+","+Numtostr(MaEMAindBX[RightStrength+11],9)
+","+Numtostr(MaEMAindBX[RightStrength+12],9)
+","+Numtostr(MaEMAindBX[RightStrength+13],9)
+","+Numtostr(MaEMAindBX[RightStrength+14],9)
			+" )"; 
			res4v02 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_feat4v02);
			srun4v02 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures4v02.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_feat4v02
					+ NewLine);
			end;
		End;
	End;
	 TblNum = TblNum+1;

	//(4) insert feature data into dbo.barFeatures5v02	
	if ReadWriteToDB and TblNum = 11+5  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		if  srun5v02 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_feat5v02 = "INSERT INTO  [dbo].[barFeatures5v02] VALUES ("
			+queryResult//MAX(loadId) 
			+","+Numtostr(loadGroupId,0) 
			+","+Numtostr(RightStrength,0)//RightStrength
			+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
			+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"'"+dateBarOpened+"' "+","+"'"+dayOfWeekBarOpened+"' "+","+"'"+dateTimeBarOpened+"' "+","+"'"+timeBarOpenedhhmmss+"' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)
+","+Numtostr(MaLGREindBX[RightStrength+2],9)
+","+Numtostr(MaLGREindBX[RightStrength+20],9)
+","+Numtostr(MaLGREindBX[RightStrength+21],9)
+","+Numtostr(MaLGREindBX[RightStrength+22],9)
+","+Numtostr(MaLGREindBX[RightStrength+23],9)
+","+Numtostr(MaLGREindBX[RightStrength+24],9)
+","+Numtostr(MaLGREindBX[RightStrength+25],9)
+","+Numtostr(MaLGREindBX[RightStrength+26],9)
+","+Numtostr(MaLGREindBX[RightStrength+27],9)
+","+Numtostr(MaLGREindBX[RightStrength+28],9)
+","+Numtostr(MaLGREindBX[RightStrength+3],9)
+","+Numtostr(MaLGREindBX[RightStrength+4],9)
+","+Numtostr(MaLGREindBX[RightStrength+5],9)
+","+Numtostr(MaLGREindBX[RightStrength+6],9)
+","+Numtostr(MaLGREindBX[RightStrength+7],9)
+","+Numtostr(MaLGREindBX[RightStrength+8],9)
+","+Numtostr(MaLGREindBX[RightStrength+9],9)
+","+Numtostr(MaLGREindBX[RightStrength-1],9)
+","+Numtostr(MaLGREindBX[RightStrength-2],9)
+","+Numtostr(MaLGREindBX[RightStrength-3],9)
+","+Numtostr(MaLGREindBX[RightStrength-4],9)
+","+Numtostr(MaLGREindBXL[RightStrength+0],9)
+","+Numtostr(MaLGREindBXL[RightStrength+1],9)
+","+Numtostr(MaLGREindBXL[RightStrength+10],9)
+","+Numtostr(MaLGREindBXL[RightStrength+11],9)
+","+Numtostr(MaLGREindBXL[RightStrength+12],9)
+","+Numtostr(MaLGREindBXL[RightStrength+13],9)
+","+Numtostr(MaLGREindBXL[RightStrength+14],9)
+","+Numtostr(MaLGREindBXL[RightStrength+15],9)
+","+Numtostr(MaLGREindBXL[RightStrength+16],9)
+","+Numtostr(MaLGREindBXL[RightStrength+17],9)
+","+Numtostr(MaLGREindBXL[RightStrength+18],9)
+","+Numtostr(MaLGREindBXL[RightStrength+19],9)
+","+Numtostr(MaLGREindBXL[RightStrength+2],9)
+","+Numtostr(MaLGREindBXL[RightStrength+20],9)
+","+Numtostr(MaLGREindBXL[RightStrength+21],9)
+","+Numtostr(MaLGREindBXL[RightStrength+22],9)
+","+Numtostr(MaLGREindBXL[RightStrength+23],9)
+","+Numtostr(MaLGREindBXL[RightStrength+24],9)
+","+Numtostr(MaLGREindBXL[RightStrength+25],9)
+","+Numtostr(MaLGREindBXL[RightStrength+26],9)
+","+Numtostr(MaLGREindBXL[RightStrength+27],9)
+","+Numtostr(MaLGREindBXL[RightStrength+28],9)
+","+Numtostr(MaLGREindBXL[RightStrength+3],9)
+","+Numtostr(MaLGREindBXL[RightStrength+4],9)
+","+Numtostr(MaLGREindBXL[RightStrength+5],9)
+","+Numtostr(MaLGREindBXL[RightStrength+6],9)
+","+Numtostr(MaLGREindBXL[RightStrength+7],9)
+","+Numtostr(MaLGREindBXL[RightStrength+8],9)
+","+Numtostr(MaLGREindBXL[RightStrength+9],9)
+","+Numtostr(MaLGREindBXL[RightStrength-1],9)
+","+Numtostr(MaLGREindBXL[RightStrength-2],9)
+","+Numtostr(MaLGREindBXL[RightStrength-3],9)
+","+Numtostr(MaLGREindBXL[RightStrength-4],9)
+","+Numtostr(MaLGREindMED[RightStrength+0],9)
+","+Numtostr(MaLGREindMED[RightStrength+1],9)
+","+Numtostr(MaLGREindMED[RightStrength+10],9)
+","+Numtostr(MaLGREindMED[RightStrength+11],9)
+","+Numtostr(MaLGREindMED[RightStrength+12],9)
+","+Numtostr(MaLGREindMED[RightStrength+13],9)
+","+Numtostr(MaLGREindMED[RightStrength+14],9)
+","+Numtostr(MaLGREindMED[RightStrength+15],9)
+","+Numtostr(MaLGREindMED[RightStrength+16],9)
+","+Numtostr(MaLGREindMED[RightStrength+17],9)
+","+Numtostr(MaLGREindMED[RightStrength+18],9)
+","+Numtostr(MaLGREindMED[RightStrength+19],9)
+","+Numtostr(MaLGREindMED[RightStrength+2],9)
+","+Numtostr(MaLGREindMED[RightStrength+20],9)
+","+Numtostr(MaLGREindMED[RightStrength+21],9)
+","+Numtostr(MaLGREindMED[RightStrength+22],9)
+","+Numtostr(MaLGREindMED[RightStrength+23],9)
+","+Numtostr(MaLGREindMED[RightStrength+24],9)
+","+Numtostr(MaLGREindMED[RightStrength+25],9)
+","+Numtostr(MaLGREindMED[RightStrength+26],9)
+","+Numtostr(MaLGREindMED[RightStrength+27],9)
+","+Numtostr(MaLGREindMED[RightStrength+28],9)
+","+Numtostr(MaLGREindMED[RightStrength+3],9)
+","+Numtostr(MaLGREindMED[RightStrength+4],9)
+","+Numtostr(MaLGREindMED[RightStrength+5],9)
+","+Numtostr(MaLGREindMED[RightStrength+6],9)
+","+Numtostr(MaLGREindMED[RightStrength+7],9)
+","+Numtostr(MaLGREindMED[RightStrength+8],9)
+","+Numtostr(MaLGREindMED[RightStrength+9],9)
+","+Numtostr(MaLGREindMED[RightStrength-1],9)
+","+Numtostr(MaLGREindMED[RightStrength-2],9)
+","+Numtostr(MaLGREindMED[RightStrength-3],9)
+","+Numtostr(MaLGREindMED[RightStrength-4],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+0],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+1],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+10],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+11],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+12],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+13],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+14],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+15],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+16],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+17],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+18],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+19],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+2],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+20],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+21],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+22],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+23],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+24],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+25],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+26],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+27],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+28],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+3],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+4],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+5],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+6],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+7],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+8],9)
+","+Numtostr(MaLGREindSMALL[RightStrength+9],9)
+","+Numtostr(MaLGREindSMALL[RightStrength-1],9)
+","+Numtostr(MaLGREindSMALL[RightStrength-2],9)
+","+Numtostr(MaLGREindSMALL[RightStrength-3],9)
+","+Numtostr(MaLGREindSMALL[RightStrength-4],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+0],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+1],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+10],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+11],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+12],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+13],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+14],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+15],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+16],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+17],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+18],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+19],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+2],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+20],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+21],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+22],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+23],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+24],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+25],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+26],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+27],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+28],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+3],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+4],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+5],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+6],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+7],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+8],9)
+","+Numtostr(MaMAMAindBIG[RightStrength+9],9)
+","+Numtostr(MaMAMAindBIG[RightStrength-1],9)
+","+Numtostr(MaMAMAindBIG[RightStrength-2],9)
+","+Numtostr(MaMAMAindBIG[RightStrength-3],9)
+","+Numtostr(MaMAMAindBIG[RightStrength-4],9)
+","+Numtostr(MaMAMAindBX[RightStrength+0],9)
+","+Numtostr(MaMAMAindBX[RightStrength+1],9)
+","+Numtostr(MaMAMAindBX[RightStrength+10],9)
+","+Numtostr(MaMAMAindBX[RightStrength+11],9)
+","+Numtostr(MaMAMAindBX[RightStrength+12],9)
+","+Numtostr(MaMAMAindBX[RightStrength+13],9)
+","+Numtostr(MaMAMAindBX[RightStrength+14],9)
+","+Numtostr(MaMAMAindBX[RightStrength+15],9)
+","+Numtostr(MaMAMAindBX[RightStrength+16],9)
+","+Numtostr(MaMAMAindBX[RightStrength+17],9)
+","+Numtostr(MaMAMAindBX[RightStrength+18],9)
+","+Numtostr(MaMAMAindBX[RightStrength+19],9)
+","+Numtostr(MaMAMAindBX[RightStrength+2],9)
+","+Numtostr(MaMAMAindBX[RightStrength+20],9)
+","+Numtostr(MaMAMAindBX[RightStrength+21],9)
+","+Numtostr(MaMAMAindBX[RightStrength+22],9)
+","+Numtostr(MaMAMAindBX[RightStrength+23],9)
+","+Numtostr(MaMAMAindBX[RightStrength+24],9)
+","+Numtostr(MaMAMAindBX[RightStrength+25],9)
+","+Numtostr(MaMAMAindBX[RightStrength+26],9)
+","+Numtostr(MaMAMAindBX[RightStrength+27],9)
+","+Numtostr(MaMAMAindBX[RightStrength+28],9)
+","+Numtostr(MaMAMAindBX[RightStrength+3],9)
+","+Numtostr(MaMAMAindBX[RightStrength+4],9)
+","+Numtostr(MaMAMAindBX[RightStrength+5],9)
+","+Numtostr(MaMAMAindBX[RightStrength+6],9)
+","+Numtostr(MaMAMAindBX[RightStrength+7],9)
+","+Numtostr(MaMAMAindBX[RightStrength+8],9)
+","+Numtostr(MaMAMAindBX[RightStrength+9],9)
+","+Numtostr(MaMAMAindBX[RightStrength-1],9)
+","+Numtostr(MaMAMAindBX[RightStrength-2],9)
+","+Numtostr(MaMAMAindBX[RightStrength-3],9)
+","+Numtostr(MaMAMAindBX[RightStrength-4],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+0],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+1],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+10],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+11],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+12],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+13],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+14],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+15],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+16],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+17],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+18],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+19],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+2],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+20],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+21],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+22],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+23],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+24],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+25],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+26],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+27],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+28],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+3],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+4],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+5],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+6],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+7],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+8],9)
+","+Numtostr(MaMAMAindBXL[RightStrength+9],9)
+","+Numtostr(MaMAMAindBXL[RightStrength-1],9)
+","+Numtostr(MaMAMAindBXL[RightStrength-2],9)
+","+Numtostr(MaMAMAindBXL[RightStrength-3],9)
+","+Numtostr(MaMAMAindBXL[RightStrength-4],9)
+","+Numtostr(MaMAMAindMED[RightStrength+0],9)
+","+Numtostr(MaMAMAindMED[RightStrength+1],9)
+","+Numtostr(MaMAMAindMED[RightStrength+10],9)
+","+Numtostr(MaMAMAindMED[RightStrength+11],9)
+","+Numtostr(MaMAMAindMED[RightStrength+12],9)
+","+Numtostr(MaMAMAindMED[RightStrength+13],9)
+","+Numtostr(MaMAMAindMED[RightStrength+14],9)
+","+Numtostr(MaMAMAindMED[RightStrength+15],9)
+","+Numtostr(MaMAMAindMED[RightStrength+16],9)
+","+Numtostr(MaMAMAindMED[RightStrength+17],9)
+","+Numtostr(MaMAMAindMED[RightStrength+18],9)
+","+Numtostr(MaMAMAindMED[RightStrength+19],9)
+","+Numtostr(MaMAMAindMED[RightStrength+2],9)
+","+Numtostr(MaMAMAindMED[RightStrength+20],9)
+","+Numtostr(MaMAMAindMED[RightStrength+21],9)
+","+Numtostr(MaMAMAindMED[RightStrength+22],9)
+","+Numtostr(MaMAMAindMED[RightStrength+23],9)
+","+Numtostr(MaMAMAindMED[RightStrength+24],9)
+","+Numtostr(MaMAMAindMED[RightStrength+25],9)
+","+Numtostr(MaMAMAindMED[RightStrength+26],9)
+","+Numtostr(MaMAMAindMED[RightStrength+27],9)
+","+Numtostr(MaMAMAindMED[RightStrength+28],9)
+","+Numtostr(MaMAMAindMED[RightStrength+3],9)
+","+Numtostr(MaMAMAindMED[RightStrength+4],9)
+","+Numtostr(MaMAMAindMED[RightStrength+5],9)
+","+Numtostr(MaMAMAindMED[RightStrength+6],9)
+","+Numtostr(MaMAMAindMED[RightStrength+7],9)
+","+Numtostr(MaMAMAindMED[RightStrength+8],9)
+","+Numtostr(MaMAMAindMED[RightStrength+9],9)
+","+Numtostr(MaMAMAindMED[RightStrength-1],9)
+","+Numtostr(MaMAMAindMED[RightStrength-2],9)
+","+Numtostr(MaMAMAindMED[RightStrength-3],9)
+","+Numtostr(MaMAMAindMED[RightStrength-4],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+0],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+1],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+10],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+11],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+12],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+13],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+14],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+15],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+16],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+17],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+18],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+19],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+2],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+20],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+21],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+22],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+23],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+24],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+25],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+26],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+27],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+28],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+3],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+4],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+5],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+6],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+7],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+8],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength+9],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength-1],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength-2],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength-3],9)
+","+Numtostr(MaMAMAindSMALL[RightStrength-4],9)
+","+Numtostr(MaMMAindBIG[RightStrength+0],9)
+","+Numtostr(MaMMAindBIG[RightStrength+1],9)
+","+Numtostr(MaMMAindBIG[RightStrength+10],9)
+","+Numtostr(MaMMAindBIG[RightStrength+11],9)
+","+Numtostr(MaMMAindBIG[RightStrength+12],9)
+","+Numtostr(MaMMAindBIG[RightStrength+13],9)
+","+Numtostr(MaMMAindBIG[RightStrength+14],9)
+","+Numtostr(MaMMAindBIG[RightStrength+15],9)
+","+Numtostr(MaMMAindBIG[RightStrength+16],9)
+","+Numtostr(MaMMAindBIG[RightStrength+17],9)
+","+Numtostr(MaMMAindBIG[RightStrength+18],9)
+","+Numtostr(MaMMAindBIG[RightStrength+19],9)
+","+Numtostr(MaMMAindBIG[RightStrength+2],9)
+","+Numtostr(MaMMAindBIG[RightStrength+20],9)
+","+Numtostr(MaMMAindBIG[RightStrength+21],9)
+","+Numtostr(MaMMAindBIG[RightStrength+22],9)
+","+Numtostr(MaMMAindBIG[RightStrength+23],9)
+","+Numtostr(MaMMAindBIG[RightStrength+24],9)
+","+Numtostr(MaMMAindBIG[RightStrength+25],9)
+","+Numtostr(MaMMAindBIG[RightStrength+26],9)
+","+Numtostr(MaMMAindBIG[RightStrength+27],9)
+","+Numtostr(MaMMAindBIG[RightStrength+28],9)
+","+Numtostr(MaMMAindBIG[RightStrength+3],9)
+","+Numtostr(MaMMAindBIG[RightStrength+4],9)
+","+Numtostr(MaMMAindBIG[RightStrength+5],9)
+","+Numtostr(MaMMAindBIG[RightStrength+6],9)
+","+Numtostr(MaMMAindBIG[RightStrength+7],9)
+","+Numtostr(MaMMAindBIG[RightStrength+8],9)
+","+Numtostr(MaMMAindBIG[RightStrength+9],9)
+","+Numtostr(MaMMAindBIG[RightStrength-1],9)
+","+Numtostr(MaMMAindBIG[RightStrength-2],9)
+","+Numtostr(MaMMAindBIG[RightStrength-3],9)
+","+Numtostr(MaMMAindBIG[RightStrength-4],9)
+","+Numtostr(MaMMAindBX[RightStrength+0],9)
+","+Numtostr(MaMMAindBX[RightStrength+1],9)
+","+Numtostr(MaMMAindBX[RightStrength+10],9)
+","+Numtostr(MaMMAindBX[RightStrength+11],9)
+","+Numtostr(MaMMAindBX[RightStrength+12],9)
+","+Numtostr(MaMMAindBX[RightStrength+13],9)
+","+Numtostr(MaMMAindBX[RightStrength+14],9)
+","+Numtostr(MaMMAindBX[RightStrength+15],9)
+","+Numtostr(MaMMAindBX[RightStrength+16],9)
+","+Numtostr(MaMMAindBX[RightStrength+17],9)
+","+Numtostr(MaMMAindBX[RightStrength+18],9)
+","+Numtostr(MaMMAindBX[RightStrength+19],9)
+","+Numtostr(MaMMAindBX[RightStrength+2],9)
+","+Numtostr(MaMMAindBX[RightStrength+20],9)
+","+Numtostr(MaMMAindBX[RightStrength+21],9)
+","+Numtostr(MaMMAindBX[RightStrength+22],9)
+","+Numtostr(MaMMAindBX[RightStrength+23],9)
+","+Numtostr(MaMMAindBX[RightStrength+24],9)
+","+Numtostr(MaMMAindBX[RightStrength+25],9)
+","+Numtostr(MaMMAindBX[RightStrength+26],9)
+","+Numtostr(MaMMAindBX[RightStrength+27],9)
+","+Numtostr(MaMMAindBX[RightStrength+28],9)
+","+Numtostr(MaMMAindBX[RightStrength+3],9)
+","+Numtostr(MaMMAindBX[RightStrength+4],9)
+","+Numtostr(MaMMAindBX[RightStrength+5],9)
+","+Numtostr(MaMMAindBX[RightStrength+6],9)
+","+Numtostr(MaMMAindBX[RightStrength+7],9)
+","+Numtostr(MaMMAindBX[RightStrength+8],9)
+","+Numtostr(MaMMAindBX[RightStrength+9],9)
+","+Numtostr(MaMMAindBX[RightStrength-1],9)
+","+Numtostr(MaMMAindBX[RightStrength-2],9)
+","+Numtostr(MaMMAindBX[RightStrength-3],9)
+","+Numtostr(MaMMAindBX[RightStrength-4],9)
+","+Numtostr(MaMMAindBXL[RightStrength+0],9)
+","+Numtostr(MaMMAindBXL[RightStrength+1],9)
+","+Numtostr(MaMMAindBXL[RightStrength+10],9)
+","+Numtostr(MaMMAindBXL[RightStrength+11],9)
+","+Numtostr(MaMMAindBXL[RightStrength+12],9)
+","+Numtostr(MaMMAindBXL[RightStrength+13],9)
+","+Numtostr(MaMMAindBXL[RightStrength+14],9)
+","+Numtostr(MaMMAindBXL[RightStrength+15],9)
+","+Numtostr(MaMMAindBXL[RightStrength+16],9)
+","+Numtostr(MaMMAindBXL[RightStrength+17],9)
+","+Numtostr(MaMMAindBXL[RightStrength+18],9)
+","+Numtostr(MaMMAindBXL[RightStrength+19],9)
+","+Numtostr(MaMMAindBXL[RightStrength+2],9)
+","+Numtostr(MaMMAindBXL[RightStrength+20],9)
+","+Numtostr(MaMMAindBXL[RightStrength+21],9)
+","+Numtostr(MaMMAindBXL[RightStrength+22],9)
+","+Numtostr(MaMMAindBXL[RightStrength+23],9)
+","+Numtostr(MaMMAindBXL[RightStrength+24],9)
+","+Numtostr(MaMMAindBXL[RightStrength+25],9)
+","+Numtostr(MaMMAindBXL[RightStrength+26],9)
+","+Numtostr(MaMMAindBXL[RightStrength+27],9)
+","+Numtostr(MaMMAindBXL[RightStrength+28],9)
+","+Numtostr(MaMMAindBXL[RightStrength+3],9)
+","+Numtostr(MaMMAindBXL[RightStrength+4],9)
+","+Numtostr(MaMMAindBXL[RightStrength+5],9)
+","+Numtostr(MaMMAindBXL[RightStrength+6],9)
+","+Numtostr(MaMMAindBXL[RightStrength+7],9)
+","+Numtostr(MaMMAindBXL[RightStrength+8],9)
+","+Numtostr(MaMMAindBXL[RightStrength+9],9)
+","+Numtostr(MaMMAindBXL[RightStrength-1],9)
+","+Numtostr(MaMMAindBXL[RightStrength-2],9)
+","+Numtostr(MaMMAindBXL[RightStrength-3],9)
+","+Numtostr(MaMMAindBXL[RightStrength-4],9)
+","+Numtostr(MaMMAindMED[RightStrength+0],9)
+","+Numtostr(MaMMAindMED[RightStrength+1],9)
+","+Numtostr(MaMMAindMED[RightStrength+10],9)
+","+Numtostr(MaMMAindMED[RightStrength+11],9)
+","+Numtostr(MaMMAindMED[RightStrength+12],9)
+","+Numtostr(MaMMAindMED[RightStrength+13],9)
+","+Numtostr(MaMMAindMED[RightStrength+14],9)
+","+Numtostr(MaMMAindMED[RightStrength+15],9)
+","+Numtostr(MaMMAindMED[RightStrength+16],9)
+","+Numtostr(MaMMAindMED[RightStrength+17],9)
+","+Numtostr(MaMMAindMED[RightStrength+18],9)
+","+Numtostr(MaMMAindMED[RightStrength+19],9)
+","+Numtostr(MaMMAindMED[RightStrength+2],9)
+","+Numtostr(MaMMAindMED[RightStrength+20],9)
+","+Numtostr(MaMMAindMED[RightStrength+21],9)
+","+Numtostr(MaMMAindMED[RightStrength+22],9)
+","+Numtostr(MaMMAindMED[RightStrength+23],9)
+","+Numtostr(MaMMAindMED[RightStrength+24],9)
+","+Numtostr(MaMMAindMED[RightStrength+25],9)
+","+Numtostr(MaMMAindMED[RightStrength+26],9)
+","+Numtostr(MaMMAindMED[RightStrength+27],9)
+","+Numtostr(MaMMAindMED[RightStrength+28],9)
+","+Numtostr(MaMMAindMED[RightStrength+3],9)
+","+Numtostr(MaMMAindMED[RightStrength+4],9)
+","+Numtostr(MaMMAindMED[RightStrength+5],9)
+","+Numtostr(MaMMAindMED[RightStrength+6],9)
+","+Numtostr(MaMMAindMED[RightStrength+7],9)
+","+Numtostr(MaMMAindMED[RightStrength+8],9)
+","+Numtostr(MaMMAindMED[RightStrength+9],9)
+","+Numtostr(MaMMAindMED[RightStrength-1],9)
+","+Numtostr(MaMMAindMED[RightStrength-2],9)
+","+Numtostr(MaMMAindMED[RightStrength-3],9)
+","+Numtostr(MaMMAindMED[RightStrength-4],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+0],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+1],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+10],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+11],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+12],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+13],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+14],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+15],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+16],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+17],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+18],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+19],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+2],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+20],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+21],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+22],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+23],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+24],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+25],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+26],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+27],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+28],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+3],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+4],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+5],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+6],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+7],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+8],9)
+","+Numtostr(MaMMAindSMALL[RightStrength+9],9)
+","+Numtostr(MaMMAindSMALL[RightStrength-1],9)
+","+Numtostr(MaMMAindSMALL[RightStrength-2],9)
+","+Numtostr(MaMMAindSMALL[RightStrength-3],9)
+","+Numtostr(MaMMAindSMALL[RightStrength-4],9)
+","+Numtostr(MaSMAindBIG[RightStrength+0],9)
+","+Numtostr(MaSMAindBIG[RightStrength+1],9)
+","+Numtostr(MaSMAindBIG[RightStrength+10],9)
+","+Numtostr(MaSMAindBIG[RightStrength+11],9)
+","+Numtostr(MaSMAindBIG[RightStrength+12],9)
+","+Numtostr(MaSMAindBIG[RightStrength+13],9)
+","+Numtostr(MaSMAindBIG[RightStrength+14],9)
+","+Numtostr(MaSMAindBIG[RightStrength+15],9)
+","+Numtostr(MaSMAindBIG[RightStrength+16],9)
+","+Numtostr(MaSMAindBIG[RightStrength+17],9)
+","+Numtostr(MaSMAindBIG[RightStrength+18],9)
+","+Numtostr(MaSMAindBIG[RightStrength+19],9)
+","+Numtostr(MaSMAindBIG[RightStrength+2],9)
+","+Numtostr(MaSMAindBIG[RightStrength+20],9)
+","+Numtostr(MaSMAindBIG[RightStrength+21],9)
+","+Numtostr(MaSMAindBIG[RightStrength+22],9)
+","+Numtostr(MaSMAindBIG[RightStrength+23],9)
+","+Numtostr(MaSMAindBIG[RightStrength+24],9)
+","+Numtostr(MaSMAindBIG[RightStrength+25],9)
+","+Numtostr(MaSMAindBIG[RightStrength+26],9)
+","+Numtostr(MaSMAindBIG[RightStrength+27],9)
+","+Numtostr(MaSMAindBIG[RightStrength+28],9)
+","+Numtostr(MaSMAindBIG[RightStrength+3],9)
+","+Numtostr(MaSMAindBIG[RightStrength+4],9)
+","+Numtostr(MaSMAindBIG[RightStrength+5],9)
+","+Numtostr(MaSMAindBIG[RightStrength+6],9)
+","+Numtostr(MaSMAindBIG[RightStrength+7],9)
+","+Numtostr(MaSMAindBIG[RightStrength+8],9)
+","+Numtostr(MaSMAindBIG[RightStrength+9],9)
+","+Numtostr(MaSMAindBIG[RightStrength-1],9)
+","+Numtostr(MaSMAindBIG[RightStrength-2],9)
+","+Numtostr(MaSMAindBIG[RightStrength-3],9)
+","+Numtostr(MaSMAindBIG[RightStrength-4],9)
+","+Numtostr(MaSMAindBX[RightStrength+0],9)
+","+Numtostr(MaSMAindBX[RightStrength+1],9)
+","+Numtostr(MaSMAindBX[RightStrength+10],9)
+","+Numtostr(MaSMAindBX[RightStrength+11],9)
+","+Numtostr(MaSMAindBX[RightStrength+12],9)
+","+Numtostr(MaSMAindBX[RightStrength+13],9)
+","+Numtostr(MaSMAindBX[RightStrength+14],9)
+","+Numtostr(MaSMAindBX[RightStrength+15],9)
+","+Numtostr(MaSMAindBX[RightStrength+16],9)
+","+Numtostr(MaSMAindBX[RightStrength+17],9)
+","+Numtostr(MaSMAindBX[RightStrength+18],9)
+","+Numtostr(MaSMAindBX[RightStrength+19],9)
+","+Numtostr(MaSMAindBX[RightStrength+2],9)
+","+Numtostr(MaSMAindBX[RightStrength+20],9)
+","+Numtostr(MaSMAindBX[RightStrength+21],9)
+","+Numtostr(MaSMAindBX[RightStrength+22],9)
+","+Numtostr(MaSMAindBX[RightStrength+23],9)
			+" )"; 
			res5v02 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_feat5v02);
			srun5v02 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures5v02.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_feat5v02
					+ NewLine);
			end;
		End;
	End;
	 TblNum = TblNum+1;

	//(4) insert feature data into dbo.barFeatures6v02	
	if ReadWriteToDB and TblNum = 11+6  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		if  srun6v02 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_feat6v02 = "INSERT INTO  [dbo].[barFeatures6v02] VALUES ("
			+queryResult//MAX(loadId) 
			+","+Numtostr(loadGroupId,0) 
			+","+Numtostr(RightStrength,0)//RightStrength
			+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
			+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"'"+dateBarOpened+"' "+","+"'"+dayOfWeekBarOpened+"' "+","+"'"+dateTimeBarOpened+"' "+","+"'"+timeBarOpenedhhmmss+"' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)
+","+Numtostr(MaTRIindBX[RightStrength+3],9)
+","+Numtostr(MaTRIindBX[RightStrength+4],9)
+","+Numtostr(MaTRIindBX[RightStrength+5],9)
+","+Numtostr(MaTRIindBX[RightStrength+6],9)
+","+Numtostr(MaTRIindBX[RightStrength+7],9)
+","+Numtostr(MaTRIindBX[RightStrength+8],9)
+","+Numtostr(MaTRIindBX[RightStrength+9],9)
+","+Numtostr(MaTRIindBX[RightStrength-1],9)
+","+Numtostr(MaTRIindBX[RightStrength-2],9)
+","+Numtostr(MaTRIindBX[RightStrength-3],9)
+","+Numtostr(MaTRIindBX[RightStrength-4],9)
+","+Numtostr(MaTRIindBXL[RightStrength+0],9)
+","+Numtostr(MaTRIindBXL[RightStrength+1],9)
+","+Numtostr(MaTRIindBXL[RightStrength+10],9)
+","+Numtostr(MaTRIindBXL[RightStrength+11],9)
+","+Numtostr(MaTRIindBXL[RightStrength+12],9)
+","+Numtostr(MaTRIindBXL[RightStrength+13],9)
+","+Numtostr(MaTRIindBXL[RightStrength+14],9)
+","+Numtostr(MaTRIindBXL[RightStrength+15],9)
+","+Numtostr(MaTRIindBXL[RightStrength+16],9)
+","+Numtostr(MaTRIindBXL[RightStrength+17],9)
+","+Numtostr(MaTRIindBXL[RightStrength+18],9)
+","+Numtostr(MaTRIindBXL[RightStrength+19],9)
+","+Numtostr(MaTRIindBXL[RightStrength+2],9)
+","+Numtostr(MaTRIindBXL[RightStrength+20],9)
+","+Numtostr(MaTRIindBXL[RightStrength+21],9)
+","+Numtostr(MaTRIindBXL[RightStrength+22],9)
+","+Numtostr(MaTRIindBXL[RightStrength+23],9)
+","+Numtostr(MaTRIindBXL[RightStrength+24],9)
+","+Numtostr(MaTRIindBXL[RightStrength+25],9)
+","+Numtostr(MaTRIindBXL[RightStrength+26],9)
+","+Numtostr(MaTRIindBXL[RightStrength+27],9)
+","+Numtostr(MaTRIindBXL[RightStrength+28],9)
+","+Numtostr(MaTRIindBXL[RightStrength+3],9)
+","+Numtostr(MaTRIindBXL[RightStrength+4],9)
+","+Numtostr(MaTRIindBXL[RightStrength+5],9)
+","+Numtostr(MaTRIindBXL[RightStrength+6],9)
+","+Numtostr(MaTRIindBXL[RightStrength+7],9)
+","+Numtostr(MaTRIindBXL[RightStrength+8],9)
+","+Numtostr(MaTRIindBXL[RightStrength+9],9)
+","+Numtostr(MaTRIindBXL[RightStrength-1],9)
+","+Numtostr(MaTRIindBXL[RightStrength-2],9)
+","+Numtostr(MaTRIindBXL[RightStrength-3],9)
+","+Numtostr(MaTRIindBXL[RightStrength-4],9)
+","+Numtostr(MaTRIindMED[RightStrength+0],9)
+","+Numtostr(MaTRIindMED[RightStrength+1],9)
+","+Numtostr(MaTRIindMED[RightStrength+10],9)
+","+Numtostr(MaTRIindMED[RightStrength+11],9)
+","+Numtostr(MaTRIindMED[RightStrength+12],9)
+","+Numtostr(MaTRIindMED[RightStrength+13],9)
+","+Numtostr(MaTRIindMED[RightStrength+14],9)
+","+Numtostr(MaTRIindMED[RightStrength+15],9)
+","+Numtostr(MaTRIindMED[RightStrength+16],9)
+","+Numtostr(MaTRIindMED[RightStrength+17],9)
+","+Numtostr(MaTRIindMED[RightStrength+18],9)
+","+Numtostr(MaTRIindMED[RightStrength+19],9)
+","+Numtostr(MaTRIindMED[RightStrength+2],9)
+","+Numtostr(MaTRIindMED[RightStrength+20],9)
+","+Numtostr(MaTRIindMED[RightStrength+21],9)
+","+Numtostr(MaTRIindMED[RightStrength+22],9)
+","+Numtostr(MaTRIindMED[RightStrength+23],9)
+","+Numtostr(MaTRIindMED[RightStrength+24],9)
+","+Numtostr(MaTRIindMED[RightStrength+25],9)
+","+Numtostr(MaTRIindMED[RightStrength+26],9)
+","+Numtostr(MaTRIindMED[RightStrength+27],9)
+","+Numtostr(MaTRIindMED[RightStrength+28],9)
+","+Numtostr(MaTRIindMED[RightStrength+3],9)
+","+Numtostr(MaTRIindMED[RightStrength+4],9)
+","+Numtostr(MaTRIindMED[RightStrength+5],9)
+","+Numtostr(MaTRIindMED[RightStrength+6],9)
+","+Numtostr(MaTRIindMED[RightStrength+7],9)
+","+Numtostr(MaTRIindMED[RightStrength+8],9)
+","+Numtostr(MaTRIindMED[RightStrength+9],9)
+","+Numtostr(MaTRIindMED[RightStrength-1],9)
+","+Numtostr(MaTRIindMED[RightStrength-2],9)
+","+Numtostr(MaTRIindMED[RightStrength-3],9)
+","+Numtostr(MaTRIindMED[RightStrength-4],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+0],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+1],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+10],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+11],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+12],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+13],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+14],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+15],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+16],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+17],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+18],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+19],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+2],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+20],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+21],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+22],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+23],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+24],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+25],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+26],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+27],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+28],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+3],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+4],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+5],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+6],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+7],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+8],9)
+","+Numtostr(MaTRIindSMALL[RightStrength+9],9)
+","+Numtostr(MaTRIindSMALL[RightStrength-1],9)
+","+Numtostr(MaTRIindSMALL[RightStrength-2],9)
+","+Numtostr(MaTRIindSMALL[RightStrength-3],9)
+","+Numtostr(MaTRIindSMALL[RightStrength-4],9)
+","+Numtostr(MaVMAindBIG[RightStrength+0],9)
+","+Numtostr(MaVMAindBIG[RightStrength+1],9)
+","+Numtostr(MaVMAindBIG[RightStrength+10],9)
+","+Numtostr(MaVMAindBIG[RightStrength+11],9)
+","+Numtostr(MaVMAindBIG[RightStrength+12],9)
+","+Numtostr(MaVMAindBIG[RightStrength+13],9)
+","+Numtostr(MaVMAindBIG[RightStrength+14],9)
+","+Numtostr(MaVMAindBIG[RightStrength+15],9)
+","+Numtostr(MaVMAindBIG[RightStrength+16],9)
+","+Numtostr(MaVMAindBIG[RightStrength+17],9)
+","+Numtostr(MaVMAindBIG[RightStrength+18],9)
+","+Numtostr(MaVMAindBIG[RightStrength+19],9)
+","+Numtostr(MaVMAindBIG[RightStrength+2],9)
+","+Numtostr(MaVMAindBIG[RightStrength+20],9)
+","+Numtostr(MaVMAindBIG[RightStrength+21],9)
+","+Numtostr(MaVMAindBIG[RightStrength+22],9)
+","+Numtostr(MaVMAindBIG[RightStrength+23],9)
+","+Numtostr(MaVMAindBIG[RightStrength+24],9)
+","+Numtostr(MaVMAindBIG[RightStrength+25],9)
+","+Numtostr(MaVMAindBIG[RightStrength+26],9)
+","+Numtostr(MaVMAindBIG[RightStrength+27],9)
+","+Numtostr(MaVMAindBIG[RightStrength+28],9)
+","+Numtostr(MaVMAindBIG[RightStrength+3],9)
+","+Numtostr(MaVMAindBIG[RightStrength+4],9)
+","+Numtostr(MaVMAindBIG[RightStrength+5],9)
+","+Numtostr(MaVMAindBIG[RightStrength+6],9)
+","+Numtostr(MaVMAindBIG[RightStrength+7],9)
+","+Numtostr(MaVMAindBIG[RightStrength+8],9)
+","+Numtostr(MaVMAindBIG[RightStrength+9],9)
+","+Numtostr(MaVMAindBIG[RightStrength-1],9)
+","+Numtostr(MaVMAindBIG[RightStrength-2],9)
+","+Numtostr(MaVMAindBIG[RightStrength-3],9)
+","+Numtostr(MaVMAindBIG[RightStrength-4],9)
+","+Numtostr(MaVMAindBX[RightStrength+0],9)
+","+Numtostr(MaVMAindBX[RightStrength+1],9)
+","+Numtostr(MaVMAindBX[RightStrength+10],9)
+","+Numtostr(MaVMAindBX[RightStrength+11],9)
+","+Numtostr(MaVMAindBX[RightStrength+12],9)
+","+Numtostr(MaVMAindBX[RightStrength+13],9)
+","+Numtostr(MaVMAindBX[RightStrength+14],9)
+","+Numtostr(MaVMAindBX[RightStrength+15],9)
+","+Numtostr(MaVMAindBX[RightStrength+16],9)
+","+Numtostr(MaVMAindBX[RightStrength+17],9)
+","+Numtostr(MaVMAindBX[RightStrength+18],9)
+","+Numtostr(MaVMAindBX[RightStrength+19],9)
+","+Numtostr(MaVMAindBX[RightStrength+2],9)
+","+Numtostr(MaVMAindBX[RightStrength+20],9)
+","+Numtostr(MaVMAindBX[RightStrength+21],9)
+","+Numtostr(MaVMAindBX[RightStrength+22],9)
+","+Numtostr(MaVMAindBX[RightStrength+23],9)
+","+Numtostr(MaVMAindBX[RightStrength+24],9)
+","+Numtostr(MaVMAindBX[RightStrength+25],9)
+","+Numtostr(MaVMAindBX[RightStrength+26],9)
+","+Numtostr(MaVMAindBX[RightStrength+27],9)
+","+Numtostr(MaVMAindBX[RightStrength+28],9)
+","+Numtostr(MaVMAindBX[RightStrength+3],9)
+","+Numtostr(MaVMAindBX[RightStrength+4],9)
+","+Numtostr(MaVMAindBX[RightStrength+5],9)
+","+Numtostr(MaVMAindBX[RightStrength+6],9)
+","+Numtostr(MaVMAindBX[RightStrength+7],9)
+","+Numtostr(MaVMAindBX[RightStrength+8],9)
+","+Numtostr(MaVMAindBX[RightStrength+9],9)
+","+Numtostr(MaVMAindBX[RightStrength-1],9)
+","+Numtostr(MaVMAindBX[RightStrength-2],9)
+","+Numtostr(MaVMAindBX[RightStrength-3],9)
+","+Numtostr(MaVMAindBX[RightStrength-4],9)
+","+Numtostr(MaVMAindBXL[RightStrength+0],9)
+","+Numtostr(MaVMAindBXL[RightStrength+1],9)
+","+Numtostr(MaVMAindBXL[RightStrength+10],9)
+","+Numtostr(MaVMAindBXL[RightStrength+11],9)
+","+Numtostr(MaVMAindBXL[RightStrength+12],9)
+","+Numtostr(MaVMAindBXL[RightStrength+13],9)
+","+Numtostr(MaVMAindBXL[RightStrength+14],9)
+","+Numtostr(MaVMAindBXL[RightStrength+15],9)
+","+Numtostr(MaVMAindBXL[RightStrength+16],9)
+","+Numtostr(MaVMAindBXL[RightStrength+17],9)
+","+Numtostr(MaVMAindBXL[RightStrength+18],9)
+","+Numtostr(MaVMAindBXL[RightStrength+19],9)
+","+Numtostr(MaVMAindBXL[RightStrength+2],9)
+","+Numtostr(MaVMAindBXL[RightStrength+20],9)
+","+Numtostr(MaVMAindBXL[RightStrength+21],9)
+","+Numtostr(MaVMAindBXL[RightStrength+22],9)
+","+Numtostr(MaVMAindBXL[RightStrength+23],9)
+","+Numtostr(MaVMAindBXL[RightStrength+24],9)
+","+Numtostr(MaVMAindBXL[RightStrength+25],9)
+","+Numtostr(MaVMAindBXL[RightStrength+26],9)
+","+Numtostr(MaVMAindBXL[RightStrength+27],9)
+","+Numtostr(MaVMAindBXL[RightStrength+28],9)
+","+Numtostr(MaVMAindBXL[RightStrength+3],9)
+","+Numtostr(MaVMAindBXL[RightStrength+4],9)
+","+Numtostr(MaVMAindBXL[RightStrength+5],9)
+","+Numtostr(MaVMAindBXL[RightStrength+6],9)
+","+Numtostr(MaVMAindBXL[RightStrength+7],9)
+","+Numtostr(MaVMAindBXL[RightStrength+8],9)
+","+Numtostr(MaVMAindBXL[RightStrength+9],9)
+","+Numtostr(MaVMAindBXL[RightStrength-1],9)
+","+Numtostr(MaVMAindBXL[RightStrength-2],9)
+","+Numtostr(MaVMAindBXL[RightStrength-3],9)
+","+Numtostr(MaVMAindBXL[RightStrength-4],9)
+","+Numtostr(MaVMAindMED[RightStrength+0],9)
+","+Numtostr(MaVMAindMED[RightStrength+1],9)
+","+Numtostr(MaVMAindMED[RightStrength+10],9)
+","+Numtostr(MaVMAindMED[RightStrength+11],9)
+","+Numtostr(MaVMAindMED[RightStrength+12],9)
+","+Numtostr(MaVMAindMED[RightStrength+13],9)
+","+Numtostr(MaVMAindMED[RightStrength+14],9)
+","+Numtostr(MaVMAindMED[RightStrength+15],9)
+","+Numtostr(MaVMAindMED[RightStrength+16],9)
+","+Numtostr(MaVMAindMED[RightStrength+17],9)
+","+Numtostr(MaVMAindMED[RightStrength+18],9)
+","+Numtostr(MaVMAindMED[RightStrength+19],9)
+","+Numtostr(MaVMAindMED[RightStrength+2],9)
+","+Numtostr(MaVMAindMED[RightStrength+20],9)
+","+Numtostr(MaVMAindMED[RightStrength+21],9)
+","+Numtostr(MaVMAindMED[RightStrength+22],9)
+","+Numtostr(MaVMAindMED[RightStrength+23],9)
+","+Numtostr(MaVMAindMED[RightStrength+24],9)
+","+Numtostr(MaVMAindMED[RightStrength+25],9)
+","+Numtostr(MaVMAindMED[RightStrength+26],9)
+","+Numtostr(MaVMAindMED[RightStrength+27],9)
+","+Numtostr(MaVMAindMED[RightStrength+28],9)
+","+Numtostr(MaVMAindMED[RightStrength+3],9)
+","+Numtostr(MaVMAindMED[RightStrength+4],9)
+","+Numtostr(MaVMAindMED[RightStrength+5],9)
+","+Numtostr(MaVMAindMED[RightStrength+6],9)
+","+Numtostr(MaVMAindMED[RightStrength+7],9)
+","+Numtostr(MaVMAindMED[RightStrength+8],9)
+","+Numtostr(MaVMAindMED[RightStrength+9],9)
+","+Numtostr(MaVMAindMED[RightStrength-1],9)
+","+Numtostr(MaVMAindMED[RightStrength-2],9)
+","+Numtostr(MaVMAindMED[RightStrength-3],9)
+","+Numtostr(MaVMAindMED[RightStrength-4],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+0],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+1],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+10],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+11],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+12],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+13],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+14],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+15],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+16],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+17],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+18],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+19],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+2],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+20],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+21],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+22],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+23],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+24],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+25],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+26],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+27],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+28],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+3],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+4],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+5],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+6],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+7],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+8],9)
+","+Numtostr(MaVMAindSMALL[RightStrength+9],9)
+","+Numtostr(MaVMAindSMALL[RightStrength-1],9)
+","+Numtostr(MaVMAindSMALL[RightStrength-2],9)
+","+Numtostr(MaVMAindSMALL[RightStrength-3],9)
+","+Numtostr(MaVMAindSMALL[RightStrength-4],9)
+","+Numtostr(MaWMAindBIG[RightStrength+0],9)
+","+Numtostr(MaWMAindBIG[RightStrength+1],9)
+","+Numtostr(MaWMAindBIG[RightStrength+10],9)
+","+Numtostr(MaWMAindBIG[RightStrength+11],9)
+","+Numtostr(MaWMAindBIG[RightStrength+12],9)
+","+Numtostr(MaWMAindBIG[RightStrength+13],9)
+","+Numtostr(MaWMAindBIG[RightStrength+14],9)
+","+Numtostr(MaWMAindBIG[RightStrength+15],9)
+","+Numtostr(MaWMAindBIG[RightStrength+16],9)
+","+Numtostr(MaWMAindBIG[RightStrength+17],9)
+","+Numtostr(MaWMAindBIG[RightStrength+18],9)
+","+Numtostr(MaWMAindBIG[RightStrength+19],9)
+","+Numtostr(MaWMAindBIG[RightStrength+2],9)
+","+Numtostr(MaWMAindBIG[RightStrength+20],9)
+","+Numtostr(MaWMAindBIG[RightStrength+21],9)
+","+Numtostr(MaWMAindBIG[RightStrength+22],9)
+","+Numtostr(MaWMAindBIG[RightStrength+23],9)
+","+Numtostr(MaWMAindBIG[RightStrength+24],9)
+","+Numtostr(MaWMAindBIG[RightStrength+25],9)
+","+Numtostr(MaWMAindBIG[RightStrength+26],9)
+","+Numtostr(MaWMAindBIG[RightStrength+27],9)
+","+Numtostr(MaWMAindBIG[RightStrength+28],9)
+","+Numtostr(MaWMAindBIG[RightStrength+3],9)
+","+Numtostr(MaWMAindBIG[RightStrength+4],9)
+","+Numtostr(MaWMAindBIG[RightStrength+5],9)
+","+Numtostr(MaWMAindBIG[RightStrength+6],9)
+","+Numtostr(MaWMAindBIG[RightStrength+7],9)
+","+Numtostr(MaWMAindBIG[RightStrength+8],9)
+","+Numtostr(MaWMAindBIG[RightStrength+9],9)
+","+Numtostr(MaWMAindBIG[RightStrength-1],9)
+","+Numtostr(MaWMAindBIG[RightStrength-2],9)
+","+Numtostr(MaWMAindBIG[RightStrength-3],9)
+","+Numtostr(MaWMAindBIG[RightStrength-4],9)
+","+Numtostr(MaWMAindBX[RightStrength+0],9)
+","+Numtostr(MaWMAindBX[RightStrength+1],9)
+","+Numtostr(MaWMAindBX[RightStrength+10],9)
+","+Numtostr(MaWMAindBX[RightStrength+11],9)
+","+Numtostr(MaWMAindBX[RightStrength+12],9)
+","+Numtostr(MaWMAindBX[RightStrength+13],9)
+","+Numtostr(MaWMAindBX[RightStrength+14],9)
+","+Numtostr(MaWMAindBX[RightStrength+15],9)
+","+Numtostr(MaWMAindBX[RightStrength+16],9)
+","+Numtostr(MaWMAindBX[RightStrength+17],9)
+","+Numtostr(MaWMAindBX[RightStrength+18],9)
+","+Numtostr(MaWMAindBX[RightStrength+19],9)
+","+Numtostr(MaWMAindBX[RightStrength+2],9)
+","+Numtostr(MaWMAindBX[RightStrength+20],9)
+","+Numtostr(MaWMAindBX[RightStrength+21],9)
+","+Numtostr(MaWMAindBX[RightStrength+22],9)
+","+Numtostr(MaWMAindBX[RightStrength+23],9)
+","+Numtostr(MaWMAindBX[RightStrength+24],9)
+","+Numtostr(MaWMAindBX[RightStrength+25],9)
+","+Numtostr(MaWMAindBX[RightStrength+26],9)
+","+Numtostr(MaWMAindBX[RightStrength+27],9)
+","+Numtostr(MaWMAindBX[RightStrength+28],9)
+","+Numtostr(MaWMAindBX[RightStrength+3],9)
+","+Numtostr(MaWMAindBX[RightStrength+4],9)
+","+Numtostr(MaWMAindBX[RightStrength+5],9)
+","+Numtostr(MaWMAindBX[RightStrength+6],9)
+","+Numtostr(MaWMAindBX[RightStrength+7],9)
+","+Numtostr(MaWMAindBX[RightStrength+8],9)
+","+Numtostr(MaWMAindBX[RightStrength+9],9)
+","+Numtostr(MaWMAindBX[RightStrength-1],9)
+","+Numtostr(MaWMAindBX[RightStrength-2],9)
+","+Numtostr(MaWMAindBX[RightStrength-3],9)
+","+Numtostr(MaWMAindBX[RightStrength-4],9)
+","+Numtostr(MaWMAindBXL[RightStrength+0],9)
+","+Numtostr(MaWMAindBXL[RightStrength+1],9)
+","+Numtostr(MaWMAindBXL[RightStrength+10],9)
+","+Numtostr(MaWMAindBXL[RightStrength+11],9)
+","+Numtostr(MaWMAindBXL[RightStrength+12],9)
+","+Numtostr(MaWMAindBXL[RightStrength+13],9)
+","+Numtostr(MaWMAindBXL[RightStrength+14],9)
+","+Numtostr(MaWMAindBXL[RightStrength+15],9)
+","+Numtostr(MaWMAindBXL[RightStrength+16],9)
+","+Numtostr(MaWMAindBXL[RightStrength+17],9)
+","+Numtostr(MaWMAindBXL[RightStrength+18],9)
+","+Numtostr(MaWMAindBXL[RightStrength+19],9)
+","+Numtostr(MaWMAindBXL[RightStrength+2],9)
+","+Numtostr(MaWMAindBXL[RightStrength+20],9)
+","+Numtostr(MaWMAindBXL[RightStrength+21],9)
+","+Numtostr(MaWMAindBXL[RightStrength+22],9)
+","+Numtostr(MaWMAindBXL[RightStrength+23],9)
+","+Numtostr(MaWMAindBXL[RightStrength+24],9)
+","+Numtostr(MaWMAindBXL[RightStrength+25],9)
+","+Numtostr(MaWMAindBXL[RightStrength+26],9)
+","+Numtostr(MaWMAindBXL[RightStrength+27],9)
+","+Numtostr(MaWMAindBXL[RightStrength+28],9)
+","+Numtostr(MaWMAindBXL[RightStrength+3],9)
+","+Numtostr(MaWMAindBXL[RightStrength+4],9)
+","+Numtostr(MaWMAindBXL[RightStrength+5],9)
+","+Numtostr(MaWMAindBXL[RightStrength+6],9)
+","+Numtostr(MaWMAindBXL[RightStrength+7],9)
+","+Numtostr(MaWMAindBXL[RightStrength+8],9)
+","+Numtostr(MaWMAindBXL[RightStrength+9],9)
+","+Numtostr(MaWMAindBXL[RightStrength-1],9)
+","+Numtostr(MaWMAindBXL[RightStrength-2],9)
+","+Numtostr(MaWMAindBXL[RightStrength-3],9)
+","+Numtostr(MaWMAindBXL[RightStrength-4],9)
+","+Numtostr(MaWMAindMED[RightStrength+0],9)
+","+Numtostr(MaWMAindMED[RightStrength+1],9)
+","+Numtostr(MaWMAindMED[RightStrength+10],9)
+","+Numtostr(MaWMAindMED[RightStrength+11],9)
+","+Numtostr(MaWMAindMED[RightStrength+12],9)
+","+Numtostr(MaWMAindMED[RightStrength+13],9)
+","+Numtostr(MaWMAindMED[RightStrength+14],9)
+","+Numtostr(MaWMAindMED[RightStrength+15],9)
+","+Numtostr(MaWMAindMED[RightStrength+16],9)
+","+Numtostr(MaWMAindMED[RightStrength+17],9)
+","+Numtostr(MaWMAindMED[RightStrength+18],9)
+","+Numtostr(MaWMAindMED[RightStrength+19],9)
+","+Numtostr(MaWMAindMED[RightStrength+2],9)
+","+Numtostr(MaWMAindMED[RightStrength+20],9)
+","+Numtostr(MaWMAindMED[RightStrength+21],9)
+","+Numtostr(MaWMAindMED[RightStrength+22],9)
+","+Numtostr(MaWMAindMED[RightStrength+23],9)
+","+Numtostr(MaWMAindMED[RightStrength+24],9)
+","+Numtostr(MaWMAindMED[RightStrength+25],9)
+","+Numtostr(MaWMAindMED[RightStrength+26],9)
+","+Numtostr(MaWMAindMED[RightStrength+27],9)
+","+Numtostr(MaWMAindMED[RightStrength+28],9)
+","+Numtostr(MaWMAindMED[RightStrength+3],9)
+","+Numtostr(MaWMAindMED[RightStrength+4],9)
+","+Numtostr(MaWMAindMED[RightStrength+5],9)
+","+Numtostr(MaWMAindMED[RightStrength+6],9)
+","+Numtostr(MaWMAindMED[RightStrength+7],9)
+","+Numtostr(MaWMAindMED[RightStrength+8],9)
+","+Numtostr(MaWMAindMED[RightStrength+9],9)
+","+Numtostr(MaWMAindMED[RightStrength-1],9)
+","+Numtostr(MaWMAindMED[RightStrength-2],9)
+","+Numtostr(MaWMAindMED[RightStrength-3],9)
+","+Numtostr(MaWMAindMED[RightStrength-4],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+0],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+1],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+10],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+11],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+12],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+13],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+14],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+15],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+16],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+17],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+18],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+19],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+2],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+20],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+21],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+22],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+23],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+24],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+25],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+26],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+27],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+28],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+3],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+4],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+5],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+6],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+7],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+8],9)
+","+Numtostr(MaWMAindSMALL[RightStrength+9],9)
+","+Numtostr(MaWMAindSMALL[RightStrength-1],9)
+","+Numtostr(MaWMAindSMALL[RightStrength-2],9)
+","+Numtostr(MaWMAindSMALL[RightStrength-3],9)
+","+Numtostr(MaWMAindSMALL[RightStrength-4],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+0],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+1],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+10],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+11],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+12],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+13],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+14],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+15],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+16],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+17],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+18],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+19],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+2],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+20],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+21],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+22],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+23],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+24],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+25],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+26],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+27],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+28],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+3],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+4],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+5],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+6],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+7],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+8],9)
+","+Numtostr(MaZLAGindBIG[RightStrength+9],9)
+","+Numtostr(MaZLAGindBIG[RightStrength-1],9)
+","+Numtostr(MaZLAGindBIG[RightStrength-2],9)
+","+Numtostr(MaZLAGindBIG[RightStrength-3],9)
+","+Numtostr(MaZLAGindBIG[RightStrength-4],9)
+","+Numtostr(MaZLAGindBX[RightStrength+0],9)
+","+Numtostr(MaZLAGindBX[RightStrength+1],9)
+","+Numtostr(MaZLAGindBX[RightStrength+10],9)
+","+Numtostr(MaZLAGindBX[RightStrength+11],9)
+","+Numtostr(MaZLAGindBX[RightStrength+12],9)
+","+Numtostr(MaZLAGindBX[RightStrength+13],9)
+","+Numtostr(MaZLAGindBX[RightStrength+14],9)
+","+Numtostr(MaZLAGindBX[RightStrength+15],9)
+","+Numtostr(MaZLAGindBX[RightStrength+16],9)
+","+Numtostr(MaZLAGindBX[RightStrength+17],9)
+","+Numtostr(MaZLAGindBX[RightStrength+18],9)
+","+Numtostr(MaZLAGindBX[RightStrength+19],9)
+","+Numtostr(MaZLAGindBX[RightStrength+2],9)
+","+Numtostr(MaZLAGindBX[RightStrength+20],9)
+","+Numtostr(MaZLAGindBX[RightStrength+21],9)
+","+Numtostr(MaZLAGindBX[RightStrength+22],9)
+","+Numtostr(MaZLAGindBX[RightStrength+23],9)
+","+Numtostr(MaZLAGindBX[RightStrength+24],9)
+","+Numtostr(MaZLAGindBX[RightStrength+25],9)
+","+Numtostr(MaZLAGindBX[RightStrength+26],9)
+","+Numtostr(MaZLAGindBX[RightStrength+27],9)
+","+Numtostr(MaZLAGindBX[RightStrength+28],9)
+","+Numtostr(MaZLAGindBX[RightStrength+3],9)
+","+Numtostr(MaZLAGindBX[RightStrength+4],9)
+","+Numtostr(MaZLAGindBX[RightStrength+5],9)
+","+Numtostr(MaZLAGindBX[RightStrength+6],9)
+","+Numtostr(MaZLAGindBX[RightStrength+7],9)
			+" )"; 
			res6v02 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_feat6v02);
			srun6v02 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures6v02.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_feat6v02
					+ NewLine);
			end;
		End;
	End;
	 TblNum = TblNum+1;

	//(4) insert feature data into dbo.barFeatures7v02	
	if ReadWriteToDB and TblNum = 11+7  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		if  srun7v02 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_feat7v02 = "INSERT INTO  [dbo].[barFeatures7v02] VALUES ("
			+queryResult//MAX(loadId) 
			+","+Numtostr(loadGroupId,0) 
			+","+Numtostr(RightStrength,0)//RightStrength
			+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
			+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"'"+dateBarOpened+"' "+","+"'"+dayOfWeekBarOpened+"' "+","+"'"+dateTimeBarOpened+"' "+","+"'"+timeBarOpenedhhmmss+"' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)
+","+Numtostr(pctBindBX[RightStrength-4],9)
+","+Numtostr(pctBindBXL[RightStrength+0],9)
+","+Numtostr(pctBindBXL[RightStrength+1],9)
+","+Numtostr(pctBindBXL[RightStrength+10],9)
+","+Numtostr(pctBindBXL[RightStrength+11],9)
+","+Numtostr(pctBindBXL[RightStrength+12],9)
+","+Numtostr(pctBindBXL[RightStrength+13],9)
+","+Numtostr(pctBindBXL[RightStrength+14],9)
+","+Numtostr(pctBindBXL[RightStrength+15],9)
+","+Numtostr(pctBindBXL[RightStrength+16],9)
+","+Numtostr(pctBindBXL[RightStrength+17],9)
+","+Numtostr(pctBindBXL[RightStrength+18],9)
+","+Numtostr(pctBindBXL[RightStrength+19],9)
+","+Numtostr(pctBindBXL[RightStrength+2],9)
+","+Numtostr(pctBindBXL[RightStrength+20],9)
+","+Numtostr(pctBindBXL[RightStrength+21],9)
+","+Numtostr(pctBindBXL[RightStrength+22],9)
+","+Numtostr(pctBindBXL[RightStrength+23],9)
+","+Numtostr(pctBindBXL[RightStrength+24],9)
+","+Numtostr(pctBindBXL[RightStrength+25],9)
+","+Numtostr(pctBindBXL[RightStrength+26],9)
+","+Numtostr(pctBindBXL[RightStrength+27],9)
+","+Numtostr(pctBindBXL[RightStrength+28],9)
+","+Numtostr(pctBindBXL[RightStrength+3],9)
+","+Numtostr(pctBindBXL[RightStrength+4],9)
+","+Numtostr(pctBindBXL[RightStrength+5],9)
+","+Numtostr(pctBindBXL[RightStrength+6],9)
+","+Numtostr(pctBindBXL[RightStrength+7],9)
+","+Numtostr(pctBindBXL[RightStrength+8],9)
+","+Numtostr(pctBindBXL[RightStrength+9],9)
+","+Numtostr(pctBindBXL[RightStrength-1],9)
+","+Numtostr(pctBindBXL[RightStrength-2],9)
+","+Numtostr(pctBindBXL[RightStrength-3],9)
+","+Numtostr(pctBindBXL[RightStrength-4],9)
+","+Numtostr(pctBindMED[RightStrength+0],9)
+","+Numtostr(pctBindMED[RightStrength+1],9)
+","+Numtostr(pctBindMED[RightStrength+10],9)
+","+Numtostr(pctBindMED[RightStrength+11],9)
+","+Numtostr(pctBindMED[RightStrength+12],9)
+","+Numtostr(pctBindMED[RightStrength+13],9)
+","+Numtostr(pctBindMED[RightStrength+14],9)
+","+Numtostr(pctBindMED[RightStrength+15],9)
+","+Numtostr(pctBindMED[RightStrength+16],9)
+","+Numtostr(pctBindMED[RightStrength+17],9)
+","+Numtostr(pctBindMED[RightStrength+18],9)
+","+Numtostr(pctBindMED[RightStrength+19],9)
+","+Numtostr(pctBindMED[RightStrength+2],9)
+","+Numtostr(pctBindMED[RightStrength+20],9)
+","+Numtostr(pctBindMED[RightStrength+21],9)
+","+Numtostr(pctBindMED[RightStrength+22],9)
+","+Numtostr(pctBindMED[RightStrength+23],9)
+","+Numtostr(pctBindMED[RightStrength+24],9)
+","+Numtostr(pctBindMED[RightStrength+25],9)
+","+Numtostr(pctBindMED[RightStrength+26],9)
+","+Numtostr(pctBindMED[RightStrength+27],9)
+","+Numtostr(pctBindMED[RightStrength+28],9)
+","+Numtostr(pctBindMED[RightStrength+3],9)
+","+Numtostr(pctBindMED[RightStrength+4],9)
+","+Numtostr(pctBindMED[RightStrength+5],9)
+","+Numtostr(pctBindMED[RightStrength+6],9)
+","+Numtostr(pctBindMED[RightStrength+7],9)
+","+Numtostr(pctBindMED[RightStrength+8],9)
+","+Numtostr(pctBindMED[RightStrength+9],9)
+","+Numtostr(pctBindMED[RightStrength-1],9)
+","+Numtostr(pctBindMED[RightStrength-2],9)
+","+Numtostr(pctBindMED[RightStrength-3],9)
+","+Numtostr(pctBindMED[RightStrength-4],9)
+","+Numtostr(pctBindSMALL[RightStrength+0],9)
+","+Numtostr(pctBindSMALL[RightStrength+1],9)
+","+Numtostr(pctBindSMALL[RightStrength+10],9)
+","+Numtostr(pctBindSMALL[RightStrength+11],9)
+","+Numtostr(pctBindSMALL[RightStrength+12],9)
+","+Numtostr(pctBindSMALL[RightStrength+13],9)
+","+Numtostr(pctBindSMALL[RightStrength+14],9)
+","+Numtostr(pctBindSMALL[RightStrength+15],9)
+","+Numtostr(pctBindSMALL[RightStrength+16],9)
+","+Numtostr(pctBindSMALL[RightStrength+17],9)
+","+Numtostr(pctBindSMALL[RightStrength+18],9)
+","+Numtostr(pctBindSMALL[RightStrength+19],9)
+","+Numtostr(pctBindSMALL[RightStrength+2],9)
+","+Numtostr(pctBindSMALL[RightStrength+20],9)
+","+Numtostr(pctBindSMALL[RightStrength+21],9)
+","+Numtostr(pctBindSMALL[RightStrength+22],9)
+","+Numtostr(pctBindSMALL[RightStrength+23],9)
+","+Numtostr(pctBindSMALL[RightStrength+24],9)
+","+Numtostr(pctBindSMALL[RightStrength+25],9)
+","+Numtostr(pctBindSMALL[RightStrength+26],9)
+","+Numtostr(pctBindSMALL[RightStrength+27],9)
+","+Numtostr(pctBindSMALL[RightStrength+28],9)
+","+Numtostr(pctBindSMALL[RightStrength+3],9)
+","+Numtostr(pctBindSMALL[RightStrength+4],9)
+","+Numtostr(pctBindSMALL[RightStrength+5],9)
+","+Numtostr(pctBindSMALL[RightStrength+6],9)
+","+Numtostr(pctBindSMALL[RightStrength+7],9)
+","+Numtostr(pctBindSMALL[RightStrength+8],9)
+","+Numtostr(pctBindSMALL[RightStrength+9],9)
+","+Numtostr(pctBindSMALL[RightStrength-1],9)
+","+Numtostr(pctBindSMALL[RightStrength-2],9)
+","+Numtostr(pctBindSMALL[RightStrength-3],9)
+","+Numtostr(pctBindSMALL[RightStrength-4],9)
+","+Numtostr(pctHindBIG[RightStrength+0],9)
+","+Numtostr(pctHindBIG[RightStrength+1],9)
+","+Numtostr(pctHindBIG[RightStrength+10],9)
+","+Numtostr(pctHindBIG[RightStrength+11],9)
+","+Numtostr(pctHindBIG[RightStrength+12],9)
+","+Numtostr(pctHindBIG[RightStrength+13],9)
+","+Numtostr(pctHindBIG[RightStrength+14],9)
+","+Numtostr(pctHindBIG[RightStrength+15],9)
+","+Numtostr(pctHindBIG[RightStrength+16],9)
+","+Numtostr(pctHindBIG[RightStrength+17],9)
+","+Numtostr(pctHindBIG[RightStrength+18],9)
+","+Numtostr(pctHindBIG[RightStrength+19],9)
+","+Numtostr(pctHindBIG[RightStrength+2],9)
+","+Numtostr(pctHindBIG[RightStrength+20],9)
+","+Numtostr(pctHindBIG[RightStrength+21],9)
+","+Numtostr(pctHindBIG[RightStrength+22],9)
+","+Numtostr(pctHindBIG[RightStrength+23],9)
+","+Numtostr(pctHindBIG[RightStrength+24],9)
+","+Numtostr(pctHindBIG[RightStrength+25],9)
+","+Numtostr(pctHindBIG[RightStrength+26],9)
+","+Numtostr(pctHindBIG[RightStrength+27],9)
+","+Numtostr(pctHindBIG[RightStrength+28],9)
+","+Numtostr(pctHindBIG[RightStrength+3],9)
+","+Numtostr(pctHindBIG[RightStrength+4],9)
+","+Numtostr(pctHindBIG[RightStrength+5],9)
+","+Numtostr(pctHindBIG[RightStrength+6],9)
+","+Numtostr(pctHindBIG[RightStrength+7],9)
+","+Numtostr(pctHindBIG[RightStrength+8],9)
+","+Numtostr(pctHindBIG[RightStrength+9],9)
+","+Numtostr(pctHindBIG[RightStrength-1],9)
+","+Numtostr(pctHindBIG[RightStrength-2],9)
+","+Numtostr(pctHindBIG[RightStrength-3],9)
+","+Numtostr(pctHindBIG[RightStrength-4],9)
+","+Numtostr(pctHindBX[RightStrength+0],9)
+","+Numtostr(pctHindBX[RightStrength+1],9)
+","+Numtostr(pctHindBX[RightStrength+10],9)
+","+Numtostr(pctHindBX[RightStrength+11],9)
+","+Numtostr(pctHindBX[RightStrength+12],9)
+","+Numtostr(pctHindBX[RightStrength+13],9)
+","+Numtostr(pctHindBX[RightStrength+14],9)
+","+Numtostr(pctHindBX[RightStrength+15],9)
+","+Numtostr(pctHindBX[RightStrength+16],9)
+","+Numtostr(pctHindBX[RightStrength+17],9)
+","+Numtostr(pctHindBX[RightStrength+18],9)
+","+Numtostr(pctHindBX[RightStrength+19],9)
+","+Numtostr(pctHindBX[RightStrength+2],9)
+","+Numtostr(pctHindBX[RightStrength+20],9)
+","+Numtostr(pctHindBX[RightStrength+21],9)
+","+Numtostr(pctHindBX[RightStrength+22],9)
+","+Numtostr(pctHindBX[RightStrength+23],9)
+","+Numtostr(pctHindBX[RightStrength+24],9)
+","+Numtostr(pctHindBX[RightStrength+25],9)
+","+Numtostr(pctHindBX[RightStrength+26],9)
+","+Numtostr(pctHindBX[RightStrength+27],9)
+","+Numtostr(pctHindBX[RightStrength+28],9)
+","+Numtostr(pctHindBX[RightStrength+3],9)
+","+Numtostr(pctHindBX[RightStrength+4],9)
+","+Numtostr(pctHindBX[RightStrength+5],9)
+","+Numtostr(pctHindBX[RightStrength+6],9)
+","+Numtostr(pctHindBX[RightStrength+7],9)
+","+Numtostr(pctHindBX[RightStrength+8],9)
+","+Numtostr(pctHindBX[RightStrength+9],9)
+","+Numtostr(pctHindBX[RightStrength-1],9)
+","+Numtostr(pctHindBX[RightStrength-2],9)
+","+Numtostr(pctHindBX[RightStrength-3],9)
+","+Numtostr(pctHindBX[RightStrength-4],9)
+","+Numtostr(pctHindBXL[RightStrength+0],9)
+","+Numtostr(pctHindBXL[RightStrength+1],9)
+","+Numtostr(pctHindBXL[RightStrength+10],9)
+","+Numtostr(pctHindBXL[RightStrength+11],9)
+","+Numtostr(pctHindBXL[RightStrength+12],9)
+","+Numtostr(pctHindBXL[RightStrength+13],9)
+","+Numtostr(pctHindBXL[RightStrength+14],9)
+","+Numtostr(pctHindBXL[RightStrength+15],9)
+","+Numtostr(pctHindBXL[RightStrength+16],9)
+","+Numtostr(pctHindBXL[RightStrength+17],9)
+","+Numtostr(pctHindBXL[RightStrength+18],9)
+","+Numtostr(pctHindBXL[RightStrength+19],9)
+","+Numtostr(pctHindBXL[RightStrength+2],9)
+","+Numtostr(pctHindBXL[RightStrength+20],9)
+","+Numtostr(pctHindBXL[RightStrength+21],9)
+","+Numtostr(pctHindBXL[RightStrength+22],9)
+","+Numtostr(pctHindBXL[RightStrength+23],9)
+","+Numtostr(pctHindBXL[RightStrength+24],9)
+","+Numtostr(pctHindBXL[RightStrength+25],9)
+","+Numtostr(pctHindBXL[RightStrength+26],9)
+","+Numtostr(pctHindBXL[RightStrength+27],9)
+","+Numtostr(pctHindBXL[RightStrength+28],9)
+","+Numtostr(pctHindBXL[RightStrength+3],9)
+","+Numtostr(pctHindBXL[RightStrength+4],9)
+","+Numtostr(pctHindBXL[RightStrength+5],9)
+","+Numtostr(pctHindBXL[RightStrength+6],9)
+","+Numtostr(pctHindBXL[RightStrength+7],9)
+","+Numtostr(pctHindBXL[RightStrength+8],9)
+","+Numtostr(pctHindBXL[RightStrength+9],9)
+","+Numtostr(pctHindBXL[RightStrength-1],9)
+","+Numtostr(pctHindBXL[RightStrength-2],9)
+","+Numtostr(pctHindBXL[RightStrength-3],9)
+","+Numtostr(pctHindBXL[RightStrength-4],9)
+","+Numtostr(pctHindMED[RightStrength+0],9)
+","+Numtostr(pctHindMED[RightStrength+1],9)
+","+Numtostr(pctHindMED[RightStrength+10],9)
+","+Numtostr(pctHindMED[RightStrength+11],9)
+","+Numtostr(pctHindMED[RightStrength+12],9)
+","+Numtostr(pctHindMED[RightStrength+13],9)
+","+Numtostr(pctHindMED[RightStrength+14],9)
+","+Numtostr(pctHindMED[RightStrength+15],9)
+","+Numtostr(pctHindMED[RightStrength+16],9)
+","+Numtostr(pctHindMED[RightStrength+17],9)
+","+Numtostr(pctHindMED[RightStrength+18],9)
+","+Numtostr(pctHindMED[RightStrength+19],9)
+","+Numtostr(pctHindMED[RightStrength+2],9)
+","+Numtostr(pctHindMED[RightStrength+20],9)
+","+Numtostr(pctHindMED[RightStrength+21],9)
+","+Numtostr(pctHindMED[RightStrength+22],9)
+","+Numtostr(pctHindMED[RightStrength+23],9)
+","+Numtostr(pctHindMED[RightStrength+24],9)
+","+Numtostr(pctHindMED[RightStrength+25],9)
+","+Numtostr(pctHindMED[RightStrength+26],9)
+","+Numtostr(pctHindMED[RightStrength+27],9)
+","+Numtostr(pctHindMED[RightStrength+28],9)
+","+Numtostr(pctHindMED[RightStrength+3],9)
+","+Numtostr(pctHindMED[RightStrength+4],9)
+","+Numtostr(pctHindMED[RightStrength+5],9)
+","+Numtostr(pctHindMED[RightStrength+6],9)
+","+Numtostr(pctHindMED[RightStrength+7],9)
+","+Numtostr(pctHindMED[RightStrength+8],9)
+","+Numtostr(pctHindMED[RightStrength+9],9)
+","+Numtostr(pctHindMED[RightStrength-1],9)
+","+Numtostr(pctHindMED[RightStrength-2],9)
+","+Numtostr(pctHindMED[RightStrength-3],9)
+","+Numtostr(pctHindMED[RightStrength-4],9)
+","+Numtostr(pctHindSMALL[RightStrength+0],9)
+","+Numtostr(pctHindSMALL[RightStrength+1],9)
+","+Numtostr(pctHindSMALL[RightStrength+10],9)
+","+Numtostr(pctHindSMALL[RightStrength+11],9)
+","+Numtostr(pctHindSMALL[RightStrength+12],9)
+","+Numtostr(pctHindSMALL[RightStrength+13],9)
+","+Numtostr(pctHindSMALL[RightStrength+14],9)
+","+Numtostr(pctHindSMALL[RightStrength+15],9)
+","+Numtostr(pctHindSMALL[RightStrength+16],9)
+","+Numtostr(pctHindSMALL[RightStrength+17],9)
+","+Numtostr(pctHindSMALL[RightStrength+18],9)
+","+Numtostr(pctHindSMALL[RightStrength+19],9)
+","+Numtostr(pctHindSMALL[RightStrength+2],9)
+","+Numtostr(pctHindSMALL[RightStrength+20],9)
+","+Numtostr(pctHindSMALL[RightStrength+21],9)
+","+Numtostr(pctHindSMALL[RightStrength+22],9)
+","+Numtostr(pctHindSMALL[RightStrength+23],9)
+","+Numtostr(pctHindSMALL[RightStrength+24],9)
+","+Numtostr(pctHindSMALL[RightStrength+25],9)
+","+Numtostr(pctHindSMALL[RightStrength+26],9)
+","+Numtostr(pctHindSMALL[RightStrength+27],9)
+","+Numtostr(pctHindSMALL[RightStrength+28],9)
+","+Numtostr(pctHindSMALL[RightStrength+3],9)
+","+Numtostr(pctHindSMALL[RightStrength+4],9)
+","+Numtostr(pctHindSMALL[RightStrength+5],9)
+","+Numtostr(pctHindSMALL[RightStrength+6],9)
+","+Numtostr(pctHindSMALL[RightStrength+7],9)
+","+Numtostr(pctHindSMALL[RightStrength+8],9)
+","+Numtostr(pctHindSMALL[RightStrength+9],9)
+","+Numtostr(pctHindSMALL[RightStrength-1],9)
+","+Numtostr(pctHindSMALL[RightStrength-2],9)
+","+Numtostr(pctHindSMALL[RightStrength-3],9)
+","+Numtostr(pctHindSMALL[RightStrength-4],9)
+","+Numtostr(PCTRindBIG[RightStrength+0],9)
+","+Numtostr(PCTRindBIG[RightStrength+1],9)
+","+Numtostr(PCTRindBIG[RightStrength+10],9)
+","+Numtostr(PCTRindBIG[RightStrength+11],9)
+","+Numtostr(PCTRindBIG[RightStrength+12],9)
+","+Numtostr(PCTRindBIG[RightStrength+13],9)
+","+Numtostr(PCTRindBIG[RightStrength+14],9)
+","+Numtostr(PCTRindBIG[RightStrength+15],9)
+","+Numtostr(PCTRindBIG[RightStrength+16],9)
+","+Numtostr(PCTRindBIG[RightStrength+17],9)
+","+Numtostr(PCTRindBIG[RightStrength+18],9)
+","+Numtostr(PCTRindBIG[RightStrength+19],9)
+","+Numtostr(PCTRindBIG[RightStrength+2],9)
+","+Numtostr(PCTRindBIG[RightStrength+20],9)
+","+Numtostr(PCTRindBIG[RightStrength+21],9)
+","+Numtostr(PCTRindBIG[RightStrength+22],9)
+","+Numtostr(PCTRindBIG[RightStrength+23],9)
+","+Numtostr(PCTRindBIG[RightStrength+24],9)
+","+Numtostr(PCTRindBIG[RightStrength+25],9)
+","+Numtostr(PCTRindBIG[RightStrength+26],9)
+","+Numtostr(PCTRindBIG[RightStrength+27],9)
+","+Numtostr(PCTRindBIG[RightStrength+28],9)
+","+Numtostr(PCTRindBIG[RightStrength+3],9)
+","+Numtostr(PCTRindBIG[RightStrength+4],9)
+","+Numtostr(PCTRindBIG[RightStrength+5],9)
+","+Numtostr(PCTRindBIG[RightStrength+6],9)
+","+Numtostr(PCTRindBIG[RightStrength+7],9)
+","+Numtostr(PCTRindBIG[RightStrength+8],9)
+","+Numtostr(PCTRindBIG[RightStrength+9],9)
+","+Numtostr(PCTRindBIG[RightStrength-1],9)
+","+Numtostr(PCTRindBIG[RightStrength-2],9)
+","+Numtostr(PCTRindBIG[RightStrength-3],9)
+","+Numtostr(PCTRindBIG[RightStrength-4],9)
+","+Numtostr(PCTRindBX[RightStrength+0],9)
+","+Numtostr(PCTRindBX[RightStrength+1],9)
+","+Numtostr(PCTRindBX[RightStrength+10],9)
+","+Numtostr(PCTRindBX[RightStrength+11],9)
+","+Numtostr(PCTRindBX[RightStrength+12],9)
+","+Numtostr(PCTRindBX[RightStrength+13],9)
+","+Numtostr(PCTRindBX[RightStrength+14],9)
+","+Numtostr(PCTRindBX[RightStrength+15],9)
+","+Numtostr(PCTRindBX[RightStrength+16],9)
+","+Numtostr(PCTRindBX[RightStrength+17],9)
+","+Numtostr(PCTRindBX[RightStrength+18],9)
+","+Numtostr(PCTRindBX[RightStrength+19],9)
+","+Numtostr(PCTRindBX[RightStrength+2],9)
+","+Numtostr(PCTRindBX[RightStrength+20],9)
+","+Numtostr(PCTRindBX[RightStrength+21],9)
+","+Numtostr(PCTRindBX[RightStrength+22],9)
+","+Numtostr(PCTRindBX[RightStrength+23],9)
+","+Numtostr(PCTRindBX[RightStrength+24],9)
+","+Numtostr(PCTRindBX[RightStrength+25],9)
+","+Numtostr(PCTRindBX[RightStrength+26],9)
+","+Numtostr(PCTRindBX[RightStrength+27],9)
+","+Numtostr(PCTRindBX[RightStrength+28],9)
+","+Numtostr(PCTRindBX[RightStrength+3],9)
+","+Numtostr(PCTRindBX[RightStrength+4],9)
+","+Numtostr(PCTRindBX[RightStrength+5],9)
+","+Numtostr(PCTRindBX[RightStrength+6],9)
+","+Numtostr(PCTRindBX[RightStrength+7],9)
+","+Numtostr(PCTRindBX[RightStrength+8],9)
+","+Numtostr(PCTRindBX[RightStrength+9],9)
+","+Numtostr(PCTRindBX[RightStrength-1],9)
+","+Numtostr(PCTRindBX[RightStrength-2],9)
+","+Numtostr(PCTRindBX[RightStrength-3],9)
+","+Numtostr(PCTRindBX[RightStrength-4],9)
+","+Numtostr(PCTRindBXL[RightStrength+0],9)
+","+Numtostr(PCTRindBXL[RightStrength+1],9)
+","+Numtostr(PCTRindBXL[RightStrength+10],9)
+","+Numtostr(PCTRindBXL[RightStrength+11],9)
+","+Numtostr(PCTRindBXL[RightStrength+12],9)
+","+Numtostr(PCTRindBXL[RightStrength+13],9)
+","+Numtostr(PCTRindBXL[RightStrength+14],9)
+","+Numtostr(PCTRindBXL[RightStrength+15],9)
+","+Numtostr(PCTRindBXL[RightStrength+16],9)
+","+Numtostr(PCTRindBXL[RightStrength+17],9)
+","+Numtostr(PCTRindBXL[RightStrength+18],9)
+","+Numtostr(PCTRindBXL[RightStrength+19],9)
+","+Numtostr(PCTRindBXL[RightStrength+2],9)
+","+Numtostr(PCTRindBXL[RightStrength+20],9)
+","+Numtostr(PCTRindBXL[RightStrength+21],9)
+","+Numtostr(PCTRindBXL[RightStrength+22],9)
+","+Numtostr(PCTRindBXL[RightStrength+23],9)
+","+Numtostr(PCTRindBXL[RightStrength+24],9)
+","+Numtostr(PCTRindBXL[RightStrength+25],9)
+","+Numtostr(PCTRindBXL[RightStrength+26],9)
+","+Numtostr(PCTRindBXL[RightStrength+27],9)
+","+Numtostr(PCTRindBXL[RightStrength+28],9)
+","+Numtostr(PCTRindBXL[RightStrength+3],9)
+","+Numtostr(PCTRindBXL[RightStrength+4],9)
+","+Numtostr(PCTRindBXL[RightStrength+5],9)
+","+Numtostr(PCTRindBXL[RightStrength+6],9)
+","+Numtostr(PCTRindBXL[RightStrength+7],9)
+","+Numtostr(PCTRindBXL[RightStrength+8],9)
+","+Numtostr(PCTRindBXL[RightStrength+9],9)
+","+Numtostr(PCTRindBXL[RightStrength-1],9)
+","+Numtostr(PCTRindBXL[RightStrength-2],9)
+","+Numtostr(PCTRindBXL[RightStrength-3],9)
+","+Numtostr(PCTRindBXL[RightStrength-4],9)
+","+Numtostr(PCTRindMED[RightStrength+0],9)
+","+Numtostr(PCTRindMED[RightStrength+1],9)
+","+Numtostr(PCTRindMED[RightStrength+10],9)
+","+Numtostr(PCTRindMED[RightStrength+11],9)
+","+Numtostr(PCTRindMED[RightStrength+12],9)
+","+Numtostr(PCTRindMED[RightStrength+13],9)
+","+Numtostr(PCTRindMED[RightStrength+14],9)
+","+Numtostr(PCTRindMED[RightStrength+15],9)
+","+Numtostr(PCTRindMED[RightStrength+16],9)
+","+Numtostr(PCTRindMED[RightStrength+17],9)
+","+Numtostr(PCTRindMED[RightStrength+18],9)
+","+Numtostr(PCTRindMED[RightStrength+19],9)
+","+Numtostr(PCTRindMED[RightStrength+2],9)
+","+Numtostr(PCTRindMED[RightStrength+20],9)
+","+Numtostr(PCTRindMED[RightStrength+21],9)
+","+Numtostr(PCTRindMED[RightStrength+22],9)
+","+Numtostr(PCTRindMED[RightStrength+23],9)
+","+Numtostr(PCTRindMED[RightStrength+24],9)
+","+Numtostr(PCTRindMED[RightStrength+25],9)
+","+Numtostr(PCTRindMED[RightStrength+26],9)
+","+Numtostr(PCTRindMED[RightStrength+27],9)
+","+Numtostr(PCTRindMED[RightStrength+28],9)
+","+Numtostr(PCTRindMED[RightStrength+3],9)
+","+Numtostr(PCTRindMED[RightStrength+4],9)
+","+Numtostr(PCTRindMED[RightStrength+5],9)
+","+Numtostr(PCTRindMED[RightStrength+6],9)
+","+Numtostr(PCTRindMED[RightStrength+7],9)
+","+Numtostr(PCTRindMED[RightStrength+8],9)
+","+Numtostr(PCTRindMED[RightStrength+9],9)
+","+Numtostr(PCTRindMED[RightStrength-1],9)
+","+Numtostr(PCTRindMED[RightStrength-2],9)
+","+Numtostr(PCTRindMED[RightStrength-3],9)
+","+Numtostr(PCTRindMED[RightStrength-4],9)
+","+Numtostr(PCTRindSMALL[RightStrength+0],9)
+","+Numtostr(PCTRindSMALL[RightStrength+1],9)
+","+Numtostr(PCTRindSMALL[RightStrength+10],9)
+","+Numtostr(PCTRindSMALL[RightStrength+11],9)
+","+Numtostr(PCTRindSMALL[RightStrength+12],9)
+","+Numtostr(PCTRindSMALL[RightStrength+13],9)
+","+Numtostr(PCTRindSMALL[RightStrength+14],9)
+","+Numtostr(PCTRindSMALL[RightStrength+15],9)
+","+Numtostr(PCTRindSMALL[RightStrength+16],9)
+","+Numtostr(PCTRindSMALL[RightStrength+17],9)
+","+Numtostr(PCTRindSMALL[RightStrength+18],9)
+","+Numtostr(PCTRindSMALL[RightStrength+19],9)
+","+Numtostr(PCTRindSMALL[RightStrength+2],9)
+","+Numtostr(PCTRindSMALL[RightStrength+20],9)
+","+Numtostr(PCTRindSMALL[RightStrength+21],9)
+","+Numtostr(PCTRindSMALL[RightStrength+22],9)
+","+Numtostr(PCTRindSMALL[RightStrength+23],9)
+","+Numtostr(PCTRindSMALL[RightStrength+24],9)
+","+Numtostr(PCTRindSMALL[RightStrength+25],9)
+","+Numtostr(PCTRindSMALL[RightStrength+26],9)
+","+Numtostr(PCTRindSMALL[RightStrength+27],9)
+","+Numtostr(PCTRindSMALL[RightStrength+28],9)
+","+Numtostr(PCTRindSMALL[RightStrength+3],9)
+","+Numtostr(PCTRindSMALL[RightStrength+4],9)
+","+Numtostr(PCTRindSMALL[RightStrength+5],9)
+","+Numtostr(PCTRindSMALL[RightStrength+6],9)
+","+Numtostr(PCTRindSMALL[RightStrength+7],9)
+","+Numtostr(PCTRindSMALL[RightStrength+8],9)
+","+Numtostr(PCTRindSMALL[RightStrength+9],9)
+","+Numtostr(PCTRindSMALL[RightStrength-1],9)
+","+Numtostr(PCTRindSMALL[RightStrength-2],9)
+","+Numtostr(PCTRindSMALL[RightStrength-3],9)
+","+Numtostr(PCTRindSMALL[RightStrength-4],9)
+","+Numtostr(POSCindBIG[RightStrength+0],9)
+","+Numtostr(POSCindBIG[RightStrength+1],9)
+","+Numtostr(POSCindBIG[RightStrength+10],9)
+","+Numtostr(POSCindBIG[RightStrength+11],9)
+","+Numtostr(POSCindBIG[RightStrength+12],9)
+","+Numtostr(POSCindBIG[RightStrength+13],9)
+","+Numtostr(POSCindBIG[RightStrength+14],9)
+","+Numtostr(POSCindBIG[RightStrength+15],9)
+","+Numtostr(POSCindBIG[RightStrength+16],9)
+","+Numtostr(POSCindBIG[RightStrength+17],9)
+","+Numtostr(POSCindBIG[RightStrength+18],9)
+","+Numtostr(POSCindBIG[RightStrength+19],9)
+","+Numtostr(POSCindBIG[RightStrength+2],9)
+","+Numtostr(POSCindBIG[RightStrength+20],9)
+","+Numtostr(POSCindBIG[RightStrength+21],9)
+","+Numtostr(POSCindBIG[RightStrength+22],9)
+","+Numtostr(POSCindBIG[RightStrength+23],9)
+","+Numtostr(POSCindBIG[RightStrength+24],9)
+","+Numtostr(POSCindBIG[RightStrength+25],9)
+","+Numtostr(POSCindBIG[RightStrength+26],9)
+","+Numtostr(POSCindBIG[RightStrength+27],9)
+","+Numtostr(POSCindBIG[RightStrength+28],9)
+","+Numtostr(POSCindBIG[RightStrength+3],9)
+","+Numtostr(POSCindBIG[RightStrength+4],9)
+","+Numtostr(POSCindBIG[RightStrength+5],9)
+","+Numtostr(POSCindBIG[RightStrength+6],9)
+","+Numtostr(POSCindBIG[RightStrength+7],9)
+","+Numtostr(POSCindBIG[RightStrength+8],9)
+","+Numtostr(POSCindBIG[RightStrength+9],9)
+","+Numtostr(POSCindBIG[RightStrength-1],9)
+","+Numtostr(POSCindBIG[RightStrength-2],9)
+","+Numtostr(POSCindBIG[RightStrength-3],9)
+","+Numtostr(POSCindBIG[RightStrength-4],9)
+","+Numtostr(POSCindBX[RightStrength+0],9)
+","+Numtostr(POSCindBX[RightStrength+1],9)
+","+Numtostr(POSCindBX[RightStrength+10],9)
+","+Numtostr(POSCindBX[RightStrength+11],9)
+","+Numtostr(POSCindBX[RightStrength+12],9)
+","+Numtostr(POSCindBX[RightStrength+13],9)
+","+Numtostr(POSCindBX[RightStrength+14],9)
+","+Numtostr(POSCindBX[RightStrength+15],9)
+","+Numtostr(POSCindBX[RightStrength+16],9)
+","+Numtostr(POSCindBX[RightStrength+17],9)
+","+Numtostr(POSCindBX[RightStrength+18],9)
+","+Numtostr(POSCindBX[RightStrength+19],9)
+","+Numtostr(POSCindBX[RightStrength+2],9)
+","+Numtostr(POSCindBX[RightStrength+20],9)
+","+Numtostr(POSCindBX[RightStrength+21],9)
+","+Numtostr(POSCindBX[RightStrength+22],9)
+","+Numtostr(POSCindBX[RightStrength+23],9)
+","+Numtostr(POSCindBX[RightStrength+24],9)
+","+Numtostr(POSCindBX[RightStrength+25],9)
+","+Numtostr(POSCindBX[RightStrength+26],9)
+","+Numtostr(POSCindBX[RightStrength+27],9)
+","+Numtostr(POSCindBX[RightStrength+28],9)
+","+Numtostr(POSCindBX[RightStrength+3],9)
+","+Numtostr(POSCindBX[RightStrength+4],9)
+","+Numtostr(POSCindBX[RightStrength+5],9)
+","+Numtostr(POSCindBX[RightStrength+6],9)
+","+Numtostr(POSCindBX[RightStrength+7],9)
+","+Numtostr(POSCindBX[RightStrength+8],9)
+","+Numtostr(POSCindBX[RightStrength+9],9)
+","+Numtostr(POSCindBX[RightStrength-1],9)
+","+Numtostr(POSCindBX[RightStrength-2],9)
+","+Numtostr(POSCindBX[RightStrength-3],9)
+","+Numtostr(POSCindBX[RightStrength-4],9)
+","+Numtostr(POSCindBXL[RightStrength+0],9)
+","+Numtostr(POSCindBXL[RightStrength+1],9)
+","+Numtostr(POSCindBXL[RightStrength+10],9)
+","+Numtostr(POSCindBXL[RightStrength+11],9)
			+" )"; 
			res7v02 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_feat7v02);
			srun7v02 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures7v02.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_feat7v02
					+ NewLine);
			end;
		End;
	End;
	 TblNum = TblNum+1;
	 
	 
	//(4) insert feature data into dbo.barFeatures8v02	
	if ReadWriteToDB and TblNum = 11+8  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		if  srun8v02 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_feat8v02 = "INSERT INTO  [dbo].[barFeatures8v02] VALUES ("
			+queryResult//MAX(loadId) 
			+","+Numtostr(loadGroupId,0) 
			+","+Numtostr(RightStrength,0)//RightStrength
			+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
			+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"'"+dateBarOpened+"' "+","+"'"+dayOfWeekBarOpened+"' "+","+"'"+dateTimeBarOpened+"' "+","+"'"+timeBarOpenedhhmmss+"' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)
+","+Numtostr(ROCindBXL[RightStrength+17],9)
+","+Numtostr(ROCindBXL[RightStrength+18],9)
+","+Numtostr(ROCindBXL[RightStrength+19],9)
+","+Numtostr(ROCindBXL[RightStrength+2],9)
+","+Numtostr(ROCindBXL[RightStrength+20],9)
+","+Numtostr(ROCindBXL[RightStrength+21],9)
+","+Numtostr(ROCindBXL[RightStrength+22],9)
+","+Numtostr(ROCindBXL[RightStrength+23],9)
+","+Numtostr(ROCindBXL[RightStrength+24],9)
+","+Numtostr(ROCindBXL[RightStrength+25],9)
+","+Numtostr(ROCindBXL[RightStrength+26],9)
+","+Numtostr(ROCindBXL[RightStrength+27],9)
+","+Numtostr(ROCindBXL[RightStrength+28],9)
+","+Numtostr(ROCindBXL[RightStrength+3],9)
+","+Numtostr(ROCindBXL[RightStrength+4],9)
+","+Numtostr(ROCindBXL[RightStrength+5],9)
+","+Numtostr(ROCindBXL[RightStrength+6],9)
+","+Numtostr(ROCindBXL[RightStrength+7],9)
+","+Numtostr(ROCindBXL[RightStrength+8],9)
+","+Numtostr(ROCindBXL[RightStrength+9],9)
+","+Numtostr(ROCindBXL[RightStrength-1],9)
+","+Numtostr(ROCindBXL[RightStrength-2],9)
+","+Numtostr(ROCindBXL[RightStrength-3],9)
+","+Numtostr(ROCindBXL[RightStrength-4],9)
+","+Numtostr(ROCindMED[RightStrength+0],9)
+","+Numtostr(ROCindMED[RightStrength+1],9)
+","+Numtostr(ROCindMED[RightStrength+10],9)
+","+Numtostr(ROCindMED[RightStrength+11],9)
+","+Numtostr(ROCindMED[RightStrength+12],9)
+","+Numtostr(ROCindMED[RightStrength+13],9)
+","+Numtostr(ROCindMED[RightStrength+14],9)
+","+Numtostr(ROCindMED[RightStrength+15],9)
+","+Numtostr(ROCindMED[RightStrength+16],9)
+","+Numtostr(ROCindMED[RightStrength+17],9)
+","+Numtostr(ROCindMED[RightStrength+18],9)
+","+Numtostr(ROCindMED[RightStrength+19],9)
+","+Numtostr(ROCindMED[RightStrength+2],9)
+","+Numtostr(ROCindMED[RightStrength+20],9)
+","+Numtostr(ROCindMED[RightStrength+21],9)
+","+Numtostr(ROCindMED[RightStrength+22],9)
+","+Numtostr(ROCindMED[RightStrength+23],9)
+","+Numtostr(ROCindMED[RightStrength+24],9)
+","+Numtostr(ROCindMED[RightStrength+25],9)
+","+Numtostr(ROCindMED[RightStrength+26],9)
+","+Numtostr(ROCindMED[RightStrength+27],9)
+","+Numtostr(ROCindMED[RightStrength+28],9)
+","+Numtostr(ROCindMED[RightStrength+3],9)
+","+Numtostr(ROCindMED[RightStrength+4],9)
+","+Numtostr(ROCindMED[RightStrength+5],9)
+","+Numtostr(ROCindMED[RightStrength+6],9)
+","+Numtostr(ROCindMED[RightStrength+7],9)
+","+Numtostr(ROCindMED[RightStrength+8],9)
+","+Numtostr(ROCindMED[RightStrength+9],9)
+","+Numtostr(ROCindMED[RightStrength-1],9)
+","+Numtostr(ROCindMED[RightStrength-2],9)
+","+Numtostr(ROCindMED[RightStrength-3],9)
+","+Numtostr(ROCindMED[RightStrength-4],9)
+","+Numtostr(ROCindSMALL[RightStrength+0],9)
+","+Numtostr(ROCindSMALL[RightStrength+1],9)
+","+Numtostr(ROCindSMALL[RightStrength+10],9)
+","+Numtostr(ROCindSMALL[RightStrength+11],9)
+","+Numtostr(ROCindSMALL[RightStrength+12],9)
+","+Numtostr(ROCindSMALL[RightStrength+13],9)
+","+Numtostr(ROCindSMALL[RightStrength+14],9)
+","+Numtostr(ROCindSMALL[RightStrength+15],9)
+","+Numtostr(ROCindSMALL[RightStrength+16],9)
+","+Numtostr(ROCindSMALL[RightStrength+17],9)
+","+Numtostr(ROCindSMALL[RightStrength+18],9)
+","+Numtostr(ROCindSMALL[RightStrength+19],9)
+","+Numtostr(ROCindSMALL[RightStrength+2],9)
+","+Numtostr(ROCindSMALL[RightStrength+20],9)
+","+Numtostr(ROCindSMALL[RightStrength+21],9)
+","+Numtostr(ROCindSMALL[RightStrength+22],9)
+","+Numtostr(ROCindSMALL[RightStrength+23],9)
+","+Numtostr(ROCindSMALL[RightStrength+24],9)
+","+Numtostr(ROCindSMALL[RightStrength+25],9)
+","+Numtostr(ROCindSMALL[RightStrength+26],9)
+","+Numtostr(ROCindSMALL[RightStrength+27],9)
+","+Numtostr(ROCindSMALL[RightStrength+28],9)
+","+Numtostr(ROCindSMALL[RightStrength+3],9)
+","+Numtostr(ROCindSMALL[RightStrength+4],9)
+","+Numtostr(ROCindSMALL[RightStrength+5],9)
+","+Numtostr(ROCindSMALL[RightStrength+6],9)
+","+Numtostr(ROCindSMALL[RightStrength+7],9)
+","+Numtostr(ROCindSMALL[RightStrength+8],9)
+","+Numtostr(ROCindSMALL[RightStrength+9],9)
+","+Numtostr(ROCindSMALL[RightStrength-1],9)
+","+Numtostr(ROCindSMALL[RightStrength-2],9)
+","+Numtostr(ROCindSMALL[RightStrength-3],9)
+","+Numtostr(ROCindSMALL[RightStrength-4],9)
+","+Numtostr(RSIHULindBIG[RightStrength+0],9)
+","+Numtostr(RSIHULindBIG[RightStrength+1],9)
+","+Numtostr(RSIHULindBIG[RightStrength+10],9)
+","+Numtostr(RSIHULindBIG[RightStrength+11],9)
+","+Numtostr(RSIHULindBIG[RightStrength+12],9)
+","+Numtostr(RSIHULindBIG[RightStrength+13],9)
+","+Numtostr(RSIHULindBIG[RightStrength+14],9)
+","+Numtostr(RSIHULindBIG[RightStrength+15],9)
+","+Numtostr(RSIHULindBIG[RightStrength+16],9)
+","+Numtostr(RSIHULindBIG[RightStrength+17],9)
+","+Numtostr(RSIHULindBIG[RightStrength+18],9)
+","+Numtostr(RSIHULindBIG[RightStrength+19],9)
+","+Numtostr(RSIHULindBIG[RightStrength+2],9)
+","+Numtostr(RSIHULindBIG[RightStrength+20],9)
+","+Numtostr(RSIHULindBIG[RightStrength+21],9)
+","+Numtostr(RSIHULindBIG[RightStrength+22],9)
+","+Numtostr(RSIHULindBIG[RightStrength+23],9)
+","+Numtostr(RSIHULindBIG[RightStrength+24],9)
+","+Numtostr(RSIHULindBIG[RightStrength+25],9)
+","+Numtostr(RSIHULindBIG[RightStrength+26],9)
+","+Numtostr(RSIHULindBIG[RightStrength+27],9)
+","+Numtostr(RSIHULindBIG[RightStrength+28],9)
+","+Numtostr(RSIHULindBIG[RightStrength+3],9)
+","+Numtostr(RSIHULindBIG[RightStrength+4],9)
+","+Numtostr(RSIHULindBIG[RightStrength+5],9)
+","+Numtostr(RSIHULindBIG[RightStrength+6],9)
+","+Numtostr(RSIHULindBIG[RightStrength+7],9)
+","+Numtostr(RSIHULindBIG[RightStrength+8],9)
+","+Numtostr(RSIHULindBIG[RightStrength+9],9)
+","+Numtostr(RSIHULindBIG[RightStrength-1],9)
+","+Numtostr(RSIHULindBIG[RightStrength-2],9)
+","+Numtostr(RSIHULindBIG[RightStrength-3],9)
+","+Numtostr(RSIHULindBIG[RightStrength-4],9)
+","+Numtostr(RSIHULindBX[RightStrength+0],9)
+","+Numtostr(RSIHULindBX[RightStrength+1],9)
+","+Numtostr(RSIHULindBX[RightStrength+10],9)
+","+Numtostr(RSIHULindBX[RightStrength+11],9)
+","+Numtostr(RSIHULindBX[RightStrength+12],9)
+","+Numtostr(RSIHULindBX[RightStrength+13],9)
+","+Numtostr(RSIHULindBX[RightStrength+14],9)
+","+Numtostr(RSIHULindBX[RightStrength+15],9)
+","+Numtostr(RSIHULindBX[RightStrength+16],9)
+","+Numtostr(RSIHULindBX[RightStrength+17],9)
+","+Numtostr(RSIHULindBX[RightStrength+18],9)
+","+Numtostr(RSIHULindBX[RightStrength+19],9)
+","+Numtostr(RSIHULindBX[RightStrength+2],9)
+","+Numtostr(RSIHULindBX[RightStrength+20],9)
+","+Numtostr(RSIHULindBX[RightStrength+21],9)
+","+Numtostr(RSIHULindBX[RightStrength+22],9)
+","+Numtostr(RSIHULindBX[RightStrength+23],9)
+","+Numtostr(RSIHULindBX[RightStrength+24],9)
+","+Numtostr(RSIHULindBX[RightStrength+25],9)
+","+Numtostr(RSIHULindBX[RightStrength+26],9)
+","+Numtostr(RSIHULindBX[RightStrength+27],9)
+","+Numtostr(RSIHULindBX[RightStrength+28],9)
+","+Numtostr(RSIHULindBX[RightStrength+3],9)
+","+Numtostr(RSIHULindBX[RightStrength+4],9)
+","+Numtostr(RSIHULindBX[RightStrength+5],9)
+","+Numtostr(RSIHULindBX[RightStrength+6],9)
+","+Numtostr(RSIHULindBX[RightStrength+7],9)
+","+Numtostr(RSIHULindBX[RightStrength+8],9)
+","+Numtostr(RSIHULindBX[RightStrength+9],9)
+","+Numtostr(RSIHULindBX[RightStrength-1],9)
+","+Numtostr(RSIHULindBX[RightStrength-2],9)
+","+Numtostr(RSIHULindBX[RightStrength-3],9)
+","+Numtostr(RSIHULindBX[RightStrength-4],9)
+","+Numtostr(RSIHULindBXL[RightStrength+0],9)
+","+Numtostr(RSIHULindBXL[RightStrength+1],9)
+","+Numtostr(RSIHULindBXL[RightStrength+10],9)
+","+Numtostr(RSIHULindBXL[RightStrength+11],9)
+","+Numtostr(RSIHULindBXL[RightStrength+12],9)
+","+Numtostr(RSIHULindBXL[RightStrength+13],9)
+","+Numtostr(RSIHULindBXL[RightStrength+14],9)
+","+Numtostr(RSIHULindBXL[RightStrength+15],9)
+","+Numtostr(RSIHULindBXL[RightStrength+16],9)
+","+Numtostr(RSIHULindBXL[RightStrength+17],9)
+","+Numtostr(RSIHULindBXL[RightStrength+18],9)
+","+Numtostr(RSIHULindBXL[RightStrength+19],9)
+","+Numtostr(RSIHULindBXL[RightStrength+2],9)
+","+Numtostr(RSIHULindBXL[RightStrength+20],9)
+","+Numtostr(RSIHULindBXL[RightStrength+21],9)
+","+Numtostr(RSIHULindBXL[RightStrength+22],9)
+","+Numtostr(RSIHULindBXL[RightStrength+23],9)
+","+Numtostr(RSIHULindBXL[RightStrength+24],9)
+","+Numtostr(RSIHULindBXL[RightStrength+25],9)
+","+Numtostr(RSIHULindBXL[RightStrength+26],9)
+","+Numtostr(RSIHULindBXL[RightStrength+27],9)
+","+Numtostr(RSIHULindBXL[RightStrength+28],9)
+","+Numtostr(RSIHULindBXL[RightStrength+3],9)
+","+Numtostr(RSIHULindBXL[RightStrength+4],9)
+","+Numtostr(RSIHULindBXL[RightStrength+5],9)
+","+Numtostr(RSIHULindBXL[RightStrength+6],9)
+","+Numtostr(RSIHULindBXL[RightStrength+7],9)
+","+Numtostr(RSIHULindBXL[RightStrength+8],9)
+","+Numtostr(RSIHULindBXL[RightStrength+9],9)
+","+Numtostr(RSIHULindBXL[RightStrength-1],9)
+","+Numtostr(RSIHULindBXL[RightStrength-2],9)
+","+Numtostr(RSIHULindBXL[RightStrength-3],9)
+","+Numtostr(RSIHULindBXL[RightStrength-4],9)
+","+Numtostr(RSIHULindMED[RightStrength+0],9)
+","+Numtostr(RSIHULindMED[RightStrength+1],9)
+","+Numtostr(RSIHULindMED[RightStrength+10],9)
+","+Numtostr(RSIHULindMED[RightStrength+11],9)
+","+Numtostr(RSIHULindMED[RightStrength+12],9)
+","+Numtostr(RSIHULindMED[RightStrength+13],9)
+","+Numtostr(RSIHULindMED[RightStrength+14],9)
+","+Numtostr(RSIHULindMED[RightStrength+15],9)
+","+Numtostr(RSIHULindMED[RightStrength+16],9)
+","+Numtostr(RSIHULindMED[RightStrength+17],9)
+","+Numtostr(RSIHULindMED[RightStrength+18],9)
+","+Numtostr(RSIHULindMED[RightStrength+19],9)
+","+Numtostr(RSIHULindMED[RightStrength+2],9)
+","+Numtostr(RSIHULindMED[RightStrength+20],9)
+","+Numtostr(RSIHULindMED[RightStrength+21],9)
+","+Numtostr(RSIHULindMED[RightStrength+22],9)
+","+Numtostr(RSIHULindMED[RightStrength+23],9)
+","+Numtostr(RSIHULindMED[RightStrength+24],9)
+","+Numtostr(RSIHULindMED[RightStrength+25],9)
+","+Numtostr(RSIHULindMED[RightStrength+26],9)
+","+Numtostr(RSIHULindMED[RightStrength+27],9)
+","+Numtostr(RSIHULindMED[RightStrength+28],9)
+","+Numtostr(RSIHULindMED[RightStrength+3],9)
+","+Numtostr(RSIHULindMED[RightStrength+4],9)
+","+Numtostr(RSIHULindMED[RightStrength+5],9)
+","+Numtostr(RSIHULindMED[RightStrength+6],9)
+","+Numtostr(RSIHULindMED[RightStrength+7],9)
+","+Numtostr(RSIHULindMED[RightStrength+8],9)
+","+Numtostr(RSIHULindMED[RightStrength+9],9)
+","+Numtostr(RSIHULindMED[RightStrength-1],9)
+","+Numtostr(RSIHULindMED[RightStrength-2],9)
+","+Numtostr(RSIHULindMED[RightStrength-3],9)
+","+Numtostr(RSIHULindMED[RightStrength-4],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+0],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+1],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+10],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+11],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+12],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+13],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+14],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+15],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+16],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+17],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+18],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+19],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+2],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+20],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+21],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+22],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+23],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+24],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+25],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+26],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+27],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+28],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+3],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+4],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+5],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+6],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+7],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+8],9)
+","+Numtostr(RSIHULindSMALL[RightStrength+9],9)
+","+Numtostr(RSIHULindSMALL[RightStrength-1],9)
+","+Numtostr(RSIHULindSMALL[RightStrength-2],9)
+","+Numtostr(RSIHULindSMALL[RightStrength-3],9)
+","+Numtostr(RSIHULindSMALL[RightStrength-4],9)
+","+Numtostr(RSIindBIG[RightStrength+0],9)
+","+Numtostr(RSIindBIG[RightStrength+1],9)
+","+Numtostr(RSIindBIG[RightStrength+10],9)
+","+Numtostr(RSIindBIG[RightStrength+11],9)
+","+Numtostr(RSIindBIG[RightStrength+12],9)
+","+Numtostr(RSIindBIG[RightStrength+13],9)
+","+Numtostr(RSIindBIG[RightStrength+14],9)
+","+Numtostr(RSIindBIG[RightStrength+15],9)
+","+Numtostr(RSIindBIG[RightStrength+16],9)
+","+Numtostr(RSIindBIG[RightStrength+17],9)
+","+Numtostr(RSIindBIG[RightStrength+18],9)
+","+Numtostr(RSIindBIG[RightStrength+19],9)
+","+Numtostr(RSIindBIG[RightStrength+2],9)
+","+Numtostr(RSIindBIG[RightStrength+20],9)
+","+Numtostr(RSIindBIG[RightStrength+21],9)
+","+Numtostr(RSIindBIG[RightStrength+22],9)
+","+Numtostr(RSIindBIG[RightStrength+23],9)
+","+Numtostr(RSIindBIG[RightStrength+24],9)
+","+Numtostr(RSIindBIG[RightStrength+25],9)
+","+Numtostr(RSIindBIG[RightStrength+26],9)
+","+Numtostr(RSIindBIG[RightStrength+27],9)
+","+Numtostr(RSIindBIG[RightStrength+28],9)
+","+Numtostr(RSIindBIG[RightStrength+3],9)
+","+Numtostr(RSIindBIG[RightStrength+4],9)
+","+Numtostr(RSIindBIG[RightStrength+5],9)
+","+Numtostr(RSIindBIG[RightStrength+6],9)
+","+Numtostr(RSIindBIG[RightStrength+7],9)
+","+Numtostr(RSIindBIG[RightStrength+8],9)
+","+Numtostr(RSIindBIG[RightStrength+9],9)
+","+Numtostr(RSIindBIG[RightStrength-1],9)
+","+Numtostr(RSIindBIG[RightStrength-2],9)
+","+Numtostr(RSIindBIG[RightStrength-3],9)
+","+Numtostr(RSIindBIG[RightStrength-4],9)
+","+Numtostr(RSIindBX[RightStrength+0],9)
+","+Numtostr(RSIindBX[RightStrength+1],9)
+","+Numtostr(RSIindBX[RightStrength+10],9)
+","+Numtostr(RSIindBX[RightStrength+11],9)
+","+Numtostr(RSIindBX[RightStrength+12],9)
+","+Numtostr(RSIindBX[RightStrength+13],9)
+","+Numtostr(RSIindBX[RightStrength+14],9)
+","+Numtostr(RSIindBX[RightStrength+15],9)
+","+Numtostr(RSIindBX[RightStrength+16],9)
+","+Numtostr(RSIindBX[RightStrength+17],9)
+","+Numtostr(RSIindBX[RightStrength+18],9)
+","+Numtostr(RSIindBX[RightStrength+19],9)
+","+Numtostr(RSIindBX[RightStrength+2],9)
+","+Numtostr(RSIindBX[RightStrength+20],9)
+","+Numtostr(RSIindBX[RightStrength+21],9)
+","+Numtostr(RSIindBX[RightStrength+22],9)
+","+Numtostr(RSIindBX[RightStrength+23],9)
+","+Numtostr(RSIindBX[RightStrength+24],9)
+","+Numtostr(RSIindBX[RightStrength+25],9)
+","+Numtostr(RSIindBX[RightStrength+26],9)
+","+Numtostr(RSIindBX[RightStrength+27],9)
+","+Numtostr(RSIindBX[RightStrength+28],9)
+","+Numtostr(RSIindBX[RightStrength+3],9)
+","+Numtostr(RSIindBX[RightStrength+4],9)
+","+Numtostr(RSIindBX[RightStrength+5],9)
+","+Numtostr(RSIindBX[RightStrength+6],9)
+","+Numtostr(RSIindBX[RightStrength+7],9)
+","+Numtostr(RSIindBX[RightStrength+8],9)
+","+Numtostr(RSIindBX[RightStrength+9],9)
+","+Numtostr(RSIindBX[RightStrength-1],9)
+","+Numtostr(RSIindBX[RightStrength-2],9)
+","+Numtostr(RSIindBX[RightStrength-3],9)
+","+Numtostr(RSIindBX[RightStrength-4],9)
+","+Numtostr(RSIindBXL[RightStrength+0],9)
+","+Numtostr(RSIindBXL[RightStrength+1],9)
+","+Numtostr(RSIindBXL[RightStrength+10],9)
+","+Numtostr(RSIindBXL[RightStrength+11],9)
+","+Numtostr(RSIindBXL[RightStrength+12],9)
+","+Numtostr(RSIindBXL[RightStrength+13],9)
+","+Numtostr(RSIindBXL[RightStrength+14],9)
+","+Numtostr(RSIindBXL[RightStrength+15],9)
+","+Numtostr(RSIindBXL[RightStrength+16],9)
+","+Numtostr(RSIindBXL[RightStrength+17],9)
+","+Numtostr(RSIindBXL[RightStrength+18],9)
+","+Numtostr(RSIindBXL[RightStrength+19],9)
+","+Numtostr(RSIindBXL[RightStrength+2],9)
+","+Numtostr(RSIindBXL[RightStrength+20],9)
+","+Numtostr(RSIindBXL[RightStrength+21],9)
+","+Numtostr(RSIindBXL[RightStrength+22],9)
+","+Numtostr(RSIindBXL[RightStrength+23],9)
+","+Numtostr(RSIindBXL[RightStrength+24],9)
+","+Numtostr(RSIindBXL[RightStrength+25],9)
+","+Numtostr(RSIindBXL[RightStrength+26],9)
+","+Numtostr(RSIindBXL[RightStrength+27],9)
+","+Numtostr(RSIindBXL[RightStrength+28],9)
+","+Numtostr(RSIindBXL[RightStrength+3],9)
+","+Numtostr(RSIindBXL[RightStrength+4],9)
+","+Numtostr(RSIindBXL[RightStrength+5],9)
+","+Numtostr(RSIindBXL[RightStrength+6],9)
+","+Numtostr(RSIindBXL[RightStrength+7],9)
+","+Numtostr(RSIindBXL[RightStrength+8],9)
+","+Numtostr(RSIindBXL[RightStrength+9],9)
+","+Numtostr(RSIindBXL[RightStrength-1],9)
+","+Numtostr(RSIindBXL[RightStrength-2],9)
+","+Numtostr(RSIindBXL[RightStrength-3],9)
+","+Numtostr(RSIindBXL[RightStrength-4],9)
+","+Numtostr(RSIindMED[RightStrength+0],9)
+","+Numtostr(RSIindMED[RightStrength+1],9)
+","+Numtostr(RSIindMED[RightStrength+10],9)
+","+Numtostr(RSIindMED[RightStrength+11],9)
+","+Numtostr(RSIindMED[RightStrength+12],9)
+","+Numtostr(RSIindMED[RightStrength+13],9)
+","+Numtostr(RSIindMED[RightStrength+14],9)
+","+Numtostr(RSIindMED[RightStrength+15],9)
+","+Numtostr(RSIindMED[RightStrength+16],9)
+","+Numtostr(RSIindMED[RightStrength+17],9)
+","+Numtostr(RSIindMED[RightStrength+18],9)
+","+Numtostr(RSIindMED[RightStrength+19],9)
+","+Numtostr(RSIindMED[RightStrength+2],9)
+","+Numtostr(RSIindMED[RightStrength+20],9)
+","+Numtostr(RSIindMED[RightStrength+21],9)
+","+Numtostr(RSIindMED[RightStrength+22],9)
+","+Numtostr(RSIindMED[RightStrength+23],9)
+","+Numtostr(RSIindMED[RightStrength+24],9)
+","+Numtostr(RSIindMED[RightStrength+25],9)
+","+Numtostr(RSIindMED[RightStrength+26],9)
+","+Numtostr(RSIindMED[RightStrength+27],9)
+","+Numtostr(RSIindMED[RightStrength+28],9)
+","+Numtostr(RSIindMED[RightStrength+3],9)
+","+Numtostr(RSIindMED[RightStrength+4],9)
+","+Numtostr(RSIindMED[RightStrength+5],9)
+","+Numtostr(RSIindMED[RightStrength+6],9)
+","+Numtostr(RSIindMED[RightStrength+7],9)
+","+Numtostr(RSIindMED[RightStrength+8],9)
+","+Numtostr(RSIindMED[RightStrength+9],9)
+","+Numtostr(RSIindMED[RightStrength-1],9)
+","+Numtostr(RSIindMED[RightStrength-2],9)
+","+Numtostr(RSIindMED[RightStrength-3],9)
+","+Numtostr(RSIindMED[RightStrength-4],9)
+","+Numtostr(RSIindSMALL[RightStrength+0],9)
+","+Numtostr(RSIindSMALL[RightStrength+1],9)
+","+Numtostr(RSIindSMALL[RightStrength+10],9)
+","+Numtostr(RSIindSMALL[RightStrength+11],9)
+","+Numtostr(RSIindSMALL[RightStrength+12],9)
+","+Numtostr(RSIindSMALL[RightStrength+13],9)
+","+Numtostr(RSIindSMALL[RightStrength+14],9)
+","+Numtostr(RSIindSMALL[RightStrength+15],9)
+","+Numtostr(RSIindSMALL[RightStrength+16],9)
+","+Numtostr(RSIindSMALL[RightStrength+17],9)
+","+Numtostr(RSIindSMALL[RightStrength+18],9)
+","+Numtostr(RSIindSMALL[RightStrength+19],9)
+","+Numtostr(RSIindSMALL[RightStrength+2],9)
+","+Numtostr(RSIindSMALL[RightStrength+20],9)
+","+Numtostr(RSIindSMALL[RightStrength+21],9)
+","+Numtostr(RSIindSMALL[RightStrength+22],9)
+","+Numtostr(RSIindSMALL[RightStrength+23],9)
+","+Numtostr(RSIindSMALL[RightStrength+24],9)
+","+Numtostr(RSIindSMALL[RightStrength+25],9)
+","+Numtostr(RSIindSMALL[RightStrength+26],9)
+","+Numtostr(RSIindSMALL[RightStrength+27],9)
+","+Numtostr(RSIindSMALL[RightStrength+28],9)
+","+Numtostr(RSIindSMALL[RightStrength+3],9)
+","+Numtostr(RSIindSMALL[RightStrength+4],9)
+","+Numtostr(RSIindSMALL[RightStrength+5],9)
+","+Numtostr(RSIindSMALL[RightStrength+6],9)
+","+Numtostr(RSIindSMALL[RightStrength+7],9)
+","+Numtostr(RSIindSMALL[RightStrength+8],9)
+","+Numtostr(RSIindSMALL[RightStrength+9],9)
+","+Numtostr(RSIindSMALL[RightStrength-1],9)
+","+Numtostr(RSIindSMALL[RightStrength-2],9)
+","+Numtostr(RSIindSMALL[RightStrength-3],9)
+","+Numtostr(RSIindSMALL[RightStrength-4],9)
+","+Numtostr(RSQRindBIG[RightStrength+0],9)
+","+Numtostr(RSQRindBIG[RightStrength+1],9)
+","+Numtostr(RSQRindBIG[RightStrength+10],9)
+","+Numtostr(RSQRindBIG[RightStrength+11],9)
+","+Numtostr(RSQRindBIG[RightStrength+12],9)
+","+Numtostr(RSQRindBIG[RightStrength+13],9)
+","+Numtostr(RSQRindBIG[RightStrength+14],9)
+","+Numtostr(RSQRindBIG[RightStrength+15],9)
+","+Numtostr(RSQRindBIG[RightStrength+16],9)
+","+Numtostr(RSQRindBIG[RightStrength+17],9)
+","+Numtostr(RSQRindBIG[RightStrength+18],9)
+","+Numtostr(RSQRindBIG[RightStrength+19],9)
+","+Numtostr(RSQRindBIG[RightStrength+2],9)
+","+Numtostr(RSQRindBIG[RightStrength+20],9)
+","+Numtostr(RSQRindBIG[RightStrength+21],9)
+","+Numtostr(RSQRindBIG[RightStrength+22],9)
+","+Numtostr(RSQRindBIG[RightStrength+23],9)
+","+Numtostr(RSQRindBIG[RightStrength+24],9)
+","+Numtostr(RSQRindBIG[RightStrength+25],9)
+","+Numtostr(RSQRindBIG[RightStrength+26],9)
+","+Numtostr(RSQRindBIG[RightStrength+27],9)
+","+Numtostr(RSQRindBIG[RightStrength+28],9)
+","+Numtostr(RSQRindBIG[RightStrength+3],9)
+","+Numtostr(RSQRindBIG[RightStrength+4],9)
+","+Numtostr(RSQRindBIG[RightStrength+5],9)
+","+Numtostr(RSQRindBIG[RightStrength+6],9)
+","+Numtostr(RSQRindBIG[RightStrength+7],9)
+","+Numtostr(RSQRindBIG[RightStrength+8],9)
+","+Numtostr(RSQRindBIG[RightStrength+9],9)
+","+Numtostr(RSQRindBIG[RightStrength-1],9)
+","+Numtostr(RSQRindBIG[RightStrength-2],9)
+","+Numtostr(RSQRindBIG[RightStrength-3],9)
+","+Numtostr(RSQRindBIG[RightStrength-4],9)
+","+Numtostr(RSQRindBX[RightStrength+0],9)
+","+Numtostr(RSQRindBX[RightStrength+1],9)
+","+Numtostr(RSQRindBX[RightStrength+10],9)
+","+Numtostr(RSQRindBX[RightStrength+11],9)
+","+Numtostr(RSQRindBX[RightStrength+12],9)
+","+Numtostr(RSQRindBX[RightStrength+13],9)
+","+Numtostr(RSQRindBX[RightStrength+14],9)
+","+Numtostr(RSQRindBX[RightStrength+15],9)
+","+Numtostr(RSQRindBX[RightStrength+16],9)
+","+Numtostr(RSQRindBX[RightStrength+17],9)
+","+Numtostr(RSQRindBX[RightStrength+18],9)
+","+Numtostr(RSQRindBX[RightStrength+19],9)
+","+Numtostr(RSQRindBX[RightStrength+2],9)
+","+Numtostr(RSQRindBX[RightStrength+20],9)
+","+Numtostr(RSQRindBX[RightStrength+21],9)
+","+Numtostr(RSQRindBX[RightStrength+22],9)
+","+Numtostr(RSQRindBX[RightStrength+23],9)
+","+Numtostr(RSQRindBX[RightStrength+24],9)
+","+Numtostr(RSQRindBX[RightStrength+25],9)
+","+Numtostr(RSQRindBX[RightStrength+26],9)
+","+Numtostr(RSQRindBX[RightStrength+27],9)
+","+Numtostr(RSQRindBX[RightStrength+28],9)
+","+Numtostr(RSQRindBX[RightStrength+3],9)
+","+Numtostr(RSQRindBX[RightStrength+4],9)
+","+Numtostr(RSQRindBX[RightStrength+5],9)
+","+Numtostr(RSQRindBX[RightStrength+6],9)
+","+Numtostr(RSQRindBX[RightStrength+7],9)
+","+Numtostr(RSQRindBX[RightStrength+8],9)
+","+Numtostr(RSQRindBX[RightStrength+9],9)
+","+Numtostr(RSQRindBX[RightStrength-1],9)
+","+Numtostr(RSQRindBX[RightStrength-2],9)
+","+Numtostr(RSQRindBX[RightStrength-3],9)
+","+Numtostr(RSQRindBX[RightStrength-4],9)
+","+Numtostr(RSQRindBXL[RightStrength+0],9)
+","+Numtostr(RSQRindBXL[RightStrength+1],9)
+","+Numtostr(RSQRindBXL[RightStrength+10],9)
+","+Numtostr(RSQRindBXL[RightStrength+11],9)
+","+Numtostr(RSQRindBXL[RightStrength+12],9)
+","+Numtostr(RSQRindBXL[RightStrength+13],9)
+","+Numtostr(RSQRindBXL[RightStrength+14],9)
+","+Numtostr(RSQRindBXL[RightStrength+15],9)
+","+Numtostr(RSQRindBXL[RightStrength+16],9)
+","+Numtostr(RSQRindBXL[RightStrength+17],9)
+","+Numtostr(RSQRindBXL[RightStrength+18],9)
+","+Numtostr(RSQRindBXL[RightStrength+19],9)
+","+Numtostr(RSQRindBXL[RightStrength+2],9)
+","+Numtostr(RSQRindBXL[RightStrength+20],9)
			+" )"; 
			res8v02 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_feat8v02);
			srun8v02 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures8v02.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_feat8v02
					+ NewLine);
			end;
		End;
	End;
	 TblNum = TblNum+1;
	 

	//(4) insert feature data into dbo.barFeatures9v02	
	if ReadWriteToDB and TblNum = 11+9  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		if  srun9v02 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_feat9v02 = "INSERT INTO  [dbo].[barFeatures9v02] VALUES ("
			+queryResult//MAX(loadId) 
			+","+Numtostr(loadGroupId,0) 
			+","+Numtostr(RightStrength,0)//RightStrength
			+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
			+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"'"+dateBarOpened+"' "+","+"'"+dayOfWeekBarOpened+"' "+","+"'"+dateTimeBarOpened+"' "+","+"'"+timeBarOpenedhhmmss+"' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)
+","+Numtostr(STDERRindBXL[RightStrength+26],9)
+","+Numtostr(STDERRindBXL[RightStrength+27],9)
+","+Numtostr(STDERRindBXL[RightStrength+28],9)
+","+Numtostr(STDERRindBXL[RightStrength+3],9)
+","+Numtostr(STDERRindBXL[RightStrength+4],9)
+","+Numtostr(STDERRindBXL[RightStrength+5],9)
+","+Numtostr(STDERRindBXL[RightStrength+6],9)
+","+Numtostr(STDERRindBXL[RightStrength+7],9)
+","+Numtostr(STDERRindBXL[RightStrength+8],9)
+","+Numtostr(STDERRindBXL[RightStrength+9],9)
+","+Numtostr(STDERRindBXL[RightStrength-1],9)
+","+Numtostr(STDERRindBXL[RightStrength-2],9)
+","+Numtostr(STDERRindBXL[RightStrength-3],9)
+","+Numtostr(STDERRindBXL[RightStrength-4],9)
+","+Numtostr(STDERRindMED[RightStrength+0],9)
+","+Numtostr(STDERRindMED[RightStrength+1],9)
+","+Numtostr(STDERRindMED[RightStrength+10],9)
+","+Numtostr(STDERRindMED[RightStrength+11],9)
+","+Numtostr(STDERRindMED[RightStrength+12],9)
+","+Numtostr(STDERRindMED[RightStrength+13],9)
+","+Numtostr(STDERRindMED[RightStrength+14],9)
+","+Numtostr(STDERRindMED[RightStrength+15],9)
+","+Numtostr(STDERRindMED[RightStrength+16],9)
+","+Numtostr(STDERRindMED[RightStrength+17],9)
+","+Numtostr(STDERRindMED[RightStrength+18],9)
+","+Numtostr(STDERRindMED[RightStrength+19],9)
+","+Numtostr(STDERRindMED[RightStrength+2],9)
+","+Numtostr(STDERRindMED[RightStrength+20],9)
+","+Numtostr(STDERRindMED[RightStrength+21],9)
+","+Numtostr(STDERRindMED[RightStrength+22],9)
+","+Numtostr(STDERRindMED[RightStrength+23],9)
+","+Numtostr(STDERRindMED[RightStrength+24],9)
+","+Numtostr(STDERRindMED[RightStrength+25],9)
+","+Numtostr(STDERRindMED[RightStrength+26],9)
+","+Numtostr(STDERRindMED[RightStrength+27],9)
+","+Numtostr(STDERRindMED[RightStrength+28],9)
+","+Numtostr(STDERRindMED[RightStrength+3],9)
+","+Numtostr(STDERRindMED[RightStrength+4],9)
+","+Numtostr(STDERRindMED[RightStrength+5],9)
+","+Numtostr(STDERRindMED[RightStrength+6],9)
+","+Numtostr(STDERRindMED[RightStrength+7],9)
+","+Numtostr(STDERRindMED[RightStrength+8],9)
+","+Numtostr(STDERRindMED[RightStrength+9],9)
+","+Numtostr(STDERRindMED[RightStrength-1],9)
+","+Numtostr(STDERRindMED[RightStrength-2],9)
+","+Numtostr(STDERRindMED[RightStrength-3],9)
+","+Numtostr(STDERRindMED[RightStrength-4],9)
+","+Numtostr(STDERRindSMALL[RightStrength+0],9)
+","+Numtostr(STDERRindSMALL[RightStrength+1],9)
+","+Numtostr(STDERRindSMALL[RightStrength+10],9)
+","+Numtostr(STDERRindSMALL[RightStrength+11],9)
+","+Numtostr(STDERRindSMALL[RightStrength+12],9)
+","+Numtostr(STDERRindSMALL[RightStrength+13],9)
+","+Numtostr(STDERRindSMALL[RightStrength+14],9)
+","+Numtostr(STDERRindSMALL[RightStrength+15],9)
+","+Numtostr(STDERRindSMALL[RightStrength+16],9)
+","+Numtostr(STDERRindSMALL[RightStrength+17],9)
+","+Numtostr(STDERRindSMALL[RightStrength+18],9)
+","+Numtostr(STDERRindSMALL[RightStrength+19],9)
+","+Numtostr(STDERRindSMALL[RightStrength+2],9)
+","+Numtostr(STDERRindSMALL[RightStrength+20],9)
+","+Numtostr(STDERRindSMALL[RightStrength+21],9)
+","+Numtostr(STDERRindSMALL[RightStrength+22],9)
+","+Numtostr(STDERRindSMALL[RightStrength+23],9)
+","+Numtostr(STDERRindSMALL[RightStrength+24],9)
+","+Numtostr(STDERRindSMALL[RightStrength+25],9)
+","+Numtostr(STDERRindSMALL[RightStrength+26],9)
+","+Numtostr(STDERRindSMALL[RightStrength+27],9)
+","+Numtostr(STDERRindSMALL[RightStrength+28],9)
+","+Numtostr(STDERRindSMALL[RightStrength+3],9)
+","+Numtostr(STDERRindSMALL[RightStrength+4],9)
+","+Numtostr(STDERRindSMALL[RightStrength+5],9)
+","+Numtostr(STDERRindSMALL[RightStrength+6],9)
+","+Numtostr(STDERRindSMALL[RightStrength+7],9)
+","+Numtostr(STDERRindSMALL[RightStrength+8],9)
+","+Numtostr(STDERRindSMALL[RightStrength+9],9)
+","+Numtostr(STDERRindSMALL[RightStrength-1],9)
+","+Numtostr(STDERRindSMALL[RightStrength-2],9)
+","+Numtostr(STDERRindSMALL[RightStrength-3],9)
+","+Numtostr(STDERRindSMALL[RightStrength-4],9)
+","+Numtostr(STOFindBIG[RightStrength+0],9)
+","+Numtostr(STOFindBIG[RightStrength+1],9)
+","+Numtostr(STOFindBIG[RightStrength+10],9)
+","+Numtostr(STOFindBIG[RightStrength+11],9)
+","+Numtostr(STOFindBIG[RightStrength+12],9)
+","+Numtostr(STOFindBIG[RightStrength+13],9)
+","+Numtostr(STOFindBIG[RightStrength+14],9)
+","+Numtostr(STOFindBIG[RightStrength+15],9)
+","+Numtostr(STOFindBIG[RightStrength+16],9)
+","+Numtostr(STOFindBIG[RightStrength+17],9)
+","+Numtostr(STOFindBIG[RightStrength+18],9)
+","+Numtostr(STOFindBIG[RightStrength+19],9)
+","+Numtostr(STOFindBIG[RightStrength+2],9)
+","+Numtostr(STOFindBIG[RightStrength+20],9)
+","+Numtostr(STOFindBIG[RightStrength+21],9)
+","+Numtostr(STOFindBIG[RightStrength+22],9)
+","+Numtostr(STOFindBIG[RightStrength+23],9)
+","+Numtostr(STOFindBIG[RightStrength+24],9)
+","+Numtostr(STOFindBIG[RightStrength+25],9)
+","+Numtostr(STOFindBIG[RightStrength+26],9)
+","+Numtostr(STOFindBIG[RightStrength+27],9)
+","+Numtostr(STOFindBIG[RightStrength+28],9)
+","+Numtostr(STOFindBIG[RightStrength+3],9)
+","+Numtostr(STOFindBIG[RightStrength+4],9)
+","+Numtostr(STOFindBIG[RightStrength+5],9)
+","+Numtostr(STOFindBIG[RightStrength+6],9)
+","+Numtostr(STOFindBIG[RightStrength+7],9)
+","+Numtostr(STOFindBIG[RightStrength+8],9)
+","+Numtostr(STOFindBIG[RightStrength+9],9)
+","+Numtostr(STOFindBIG[RightStrength-1],9)
+","+Numtostr(STOFindBIG[RightStrength-2],9)
+","+Numtostr(STOFindBIG[RightStrength-3],9)
+","+Numtostr(STOFindBIG[RightStrength-4],9)
+","+Numtostr(STOFindBX[RightStrength+0],9)
+","+Numtostr(STOFindBX[RightStrength+1],9)
+","+Numtostr(STOFindBX[RightStrength+10],9)
+","+Numtostr(STOFindBX[RightStrength+11],9)
+","+Numtostr(STOFindBX[RightStrength+12],9)
+","+Numtostr(STOFindBX[RightStrength+13],9)
+","+Numtostr(STOFindBX[RightStrength+14],9)
+","+Numtostr(STOFindBX[RightStrength+15],9)
+","+Numtostr(STOFindBX[RightStrength+16],9)
+","+Numtostr(STOFindBX[RightStrength+17],9)
+","+Numtostr(STOFindBX[RightStrength+18],9)
+","+Numtostr(STOFindBX[RightStrength+19],9)
+","+Numtostr(STOFindBX[RightStrength+2],9)
+","+Numtostr(STOFindBX[RightStrength+20],9)
+","+Numtostr(STOFindBX[RightStrength+21],9)
+","+Numtostr(STOFindBX[RightStrength+22],9)
+","+Numtostr(STOFindBX[RightStrength+23],9)
+","+Numtostr(STOFindBX[RightStrength+24],9)
+","+Numtostr(STOFindBX[RightStrength+25],9)
+","+Numtostr(STOFindBX[RightStrength+26],9)
+","+Numtostr(STOFindBX[RightStrength+27],9)
+","+Numtostr(STOFindBX[RightStrength+28],9)
+","+Numtostr(STOFindBX[RightStrength+3],9)
+","+Numtostr(STOFindBX[RightStrength+4],9)
+","+Numtostr(STOFindBX[RightStrength+5],9)
+","+Numtostr(STOFindBX[RightStrength+6],9)
+","+Numtostr(STOFindBX[RightStrength+7],9)
+","+Numtostr(STOFindBX[RightStrength+8],9)
+","+Numtostr(STOFindBX[RightStrength+9],9)
+","+Numtostr(STOFindBX[RightStrength-1],9)
+","+Numtostr(STOFindBX[RightStrength-2],9)
+","+Numtostr(STOFindBX[RightStrength-3],9)
+","+Numtostr(STOFindBX[RightStrength-4],9)
+","+Numtostr(STOFindBXL[RightStrength+0],9)
+","+Numtostr(STOFindBXL[RightStrength+1],9)
+","+Numtostr(STOFindBXL[RightStrength+10],9)
+","+Numtostr(STOFindBXL[RightStrength+11],9)
+","+Numtostr(STOFindBXL[RightStrength+12],9)
+","+Numtostr(STOFindBXL[RightStrength+13],9)
+","+Numtostr(STOFindBXL[RightStrength+14],9)
+","+Numtostr(STOFindBXL[RightStrength+15],9)
+","+Numtostr(STOFindBXL[RightStrength+16],9)
+","+Numtostr(STOFindBXL[RightStrength+17],9)
+","+Numtostr(STOFindBXL[RightStrength+18],9)
+","+Numtostr(STOFindBXL[RightStrength+19],9)
+","+Numtostr(STOFindBXL[RightStrength+2],9)
+","+Numtostr(STOFindBXL[RightStrength+20],9)
+","+Numtostr(STOFindBXL[RightStrength+21],9)
+","+Numtostr(STOFindBXL[RightStrength+22],9)
+","+Numtostr(STOFindBXL[RightStrength+23],9)
+","+Numtostr(STOFindBXL[RightStrength+24],9)
+","+Numtostr(STOFindBXL[RightStrength+25],9)
+","+Numtostr(STOFindBXL[RightStrength+26],9)
+","+Numtostr(STOFindBXL[RightStrength+27],9)
+","+Numtostr(STOFindBXL[RightStrength+28],9)
+","+Numtostr(STOFindBXL[RightStrength+3],9)
+","+Numtostr(STOFindBXL[RightStrength+4],9)
+","+Numtostr(STOFindBXL[RightStrength+5],9)
+","+Numtostr(STOFindBXL[RightStrength+6],9)
+","+Numtostr(STOFindBXL[RightStrength+7],9)
+","+Numtostr(STOFindBXL[RightStrength+8],9)
+","+Numtostr(STOFindBXL[RightStrength+9],9)
+","+Numtostr(STOFindBXL[RightStrength-1],9)
+","+Numtostr(STOFindBXL[RightStrength-2],9)
+","+Numtostr(STOFindBXL[RightStrength-3],9)
+","+Numtostr(STOFindBXL[RightStrength-4],9)
+","+Numtostr(STOFindMED[RightStrength+0],9)
+","+Numtostr(STOFindMED[RightStrength+1],9)
+","+Numtostr(STOFindMED[RightStrength+10],9)
+","+Numtostr(STOFindMED[RightStrength+11],9)
+","+Numtostr(STOFindMED[RightStrength+12],9)
+","+Numtostr(STOFindMED[RightStrength+13],9)
+","+Numtostr(STOFindMED[RightStrength+14],9)
+","+Numtostr(STOFindMED[RightStrength+15],9)
+","+Numtostr(STOFindMED[RightStrength+16],9)
+","+Numtostr(STOFindMED[RightStrength+17],9)
+","+Numtostr(STOFindMED[RightStrength+18],9)
+","+Numtostr(STOFindMED[RightStrength+19],9)
+","+Numtostr(STOFindMED[RightStrength+2],9)
+","+Numtostr(STOFindMED[RightStrength+20],9)
+","+Numtostr(STOFindMED[RightStrength+21],9)
+","+Numtostr(STOFindMED[RightStrength+22],9)
+","+Numtostr(STOFindMED[RightStrength+23],9)
+","+Numtostr(STOFindMED[RightStrength+24],9)
+","+Numtostr(STOFindMED[RightStrength+25],9)
+","+Numtostr(STOFindMED[RightStrength+26],9)
+","+Numtostr(STOFindMED[RightStrength+27],9)
+","+Numtostr(STOFindMED[RightStrength+28],9)
+","+Numtostr(STOFindMED[RightStrength+3],9)
+","+Numtostr(STOFindMED[RightStrength+4],9)
+","+Numtostr(STOFindMED[RightStrength+5],9)
+","+Numtostr(STOFindMED[RightStrength+6],9)
+","+Numtostr(STOFindMED[RightStrength+7],9)
+","+Numtostr(STOFindMED[RightStrength+8],9)
+","+Numtostr(STOFindMED[RightStrength+9],9)
+","+Numtostr(STOFindMED[RightStrength-1],9)
+","+Numtostr(STOFindMED[RightStrength-2],9)
+","+Numtostr(STOFindMED[RightStrength-3],9)
+","+Numtostr(STOFindMED[RightStrength-4],9)
+","+Numtostr(STOFindSMALL[RightStrength+0],9)
+","+Numtostr(STOFindSMALL[RightStrength+1],9)
+","+Numtostr(STOFindSMALL[RightStrength+10],9)
+","+Numtostr(STOFindSMALL[RightStrength+11],9)
+","+Numtostr(STOFindSMALL[RightStrength+12],9)
+","+Numtostr(STOFindSMALL[RightStrength+13],9)
+","+Numtostr(STOFindSMALL[RightStrength+14],9)
+","+Numtostr(STOFindSMALL[RightStrength+15],9)
+","+Numtostr(STOFindSMALL[RightStrength+16],9)
+","+Numtostr(STOFindSMALL[RightStrength+17],9)
+","+Numtostr(STOFindSMALL[RightStrength+18],9)
+","+Numtostr(STOFindSMALL[RightStrength+19],9)
+","+Numtostr(STOFindSMALL[RightStrength+2],9)
+","+Numtostr(STOFindSMALL[RightStrength+20],9)
+","+Numtostr(STOFindSMALL[RightStrength+21],9)
+","+Numtostr(STOFindSMALL[RightStrength+22],9)
+","+Numtostr(STOFindSMALL[RightStrength+23],9)
+","+Numtostr(STOFindSMALL[RightStrength+24],9)
+","+Numtostr(STOFindSMALL[RightStrength+25],9)
+","+Numtostr(STOFindSMALL[RightStrength+26],9)
+","+Numtostr(STOFindSMALL[RightStrength+27],9)
+","+Numtostr(STOFindSMALL[RightStrength+28],9)
+","+Numtostr(STOFindSMALL[RightStrength+3],9)
+","+Numtostr(STOFindSMALL[RightStrength+4],9)
+","+Numtostr(STOFindSMALL[RightStrength+5],9)
+","+Numtostr(STOFindSMALL[RightStrength+6],9)
+","+Numtostr(STOFindSMALL[RightStrength+7],9)
+","+Numtostr(STOFindSMALL[RightStrength+8],9)
+","+Numtostr(STOFindSMALL[RightStrength+9],9)
+","+Numtostr(STOFindSMALL[RightStrength-1],9)
+","+Numtostr(STOFindSMALL[RightStrength-2],9)
+","+Numtostr(STOFindSMALL[RightStrength-3],9)
+","+Numtostr(STOFindSMALL[RightStrength-4],9)
+","+Numtostr(StoRSIindBIG[RightStrength+0],9)
+","+Numtostr(StoRSIindBIG[RightStrength+1],9)
+","+Numtostr(StoRSIindBIG[RightStrength+10],9)
+","+Numtostr(StoRSIindBIG[RightStrength+11],9)
+","+Numtostr(StoRSIindBIG[RightStrength+12],9)
+","+Numtostr(StoRSIindBIG[RightStrength+13],9)
+","+Numtostr(StoRSIindBIG[RightStrength+14],9)
+","+Numtostr(StoRSIindBIG[RightStrength+15],9)
+","+Numtostr(StoRSIindBIG[RightStrength+16],9)
+","+Numtostr(StoRSIindBIG[RightStrength+17],9)
+","+Numtostr(StoRSIindBIG[RightStrength+18],9)
+","+Numtostr(StoRSIindBIG[RightStrength+19],9)
+","+Numtostr(StoRSIindBIG[RightStrength+2],9)
+","+Numtostr(StoRSIindBIG[RightStrength+20],9)
+","+Numtostr(StoRSIindBIG[RightStrength+21],9)
+","+Numtostr(StoRSIindBIG[RightStrength+22],9)
+","+Numtostr(StoRSIindBIG[RightStrength+23],9)
+","+Numtostr(StoRSIindBIG[RightStrength+24],9)
+","+Numtostr(StoRSIindBIG[RightStrength+25],9)
+","+Numtostr(StoRSIindBIG[RightStrength+26],9)
+","+Numtostr(StoRSIindBIG[RightStrength+27],9)
+","+Numtostr(StoRSIindBIG[RightStrength+28],9)
+","+Numtostr(StoRSIindBIG[RightStrength+3],9)
+","+Numtostr(StoRSIindBIG[RightStrength+4],9)
+","+Numtostr(StoRSIindBIG[RightStrength+5],9)
+","+Numtostr(StoRSIindBIG[RightStrength+6],9)
+","+Numtostr(StoRSIindBIG[RightStrength+7],9)
+","+Numtostr(StoRSIindBIG[RightStrength+8],9)
+","+Numtostr(StoRSIindBIG[RightStrength+9],9)
+","+Numtostr(StoRSIindBIG[RightStrength-1],9)
+","+Numtostr(StoRSIindBIG[RightStrength-2],9)
+","+Numtostr(StoRSIindBIG[RightStrength-3],9)
+","+Numtostr(StoRSIindBIG[RightStrength-4],9)
+","+Numtostr(StoRSIindBX[RightStrength+0],9)
+","+Numtostr(StoRSIindBX[RightStrength+1],9)
+","+Numtostr(StoRSIindBX[RightStrength+10],9)
+","+Numtostr(StoRSIindBX[RightStrength+11],9)
+","+Numtostr(StoRSIindBX[RightStrength+12],9)
+","+Numtostr(StoRSIindBX[RightStrength+13],9)
+","+Numtostr(StoRSIindBX[RightStrength+14],9)
+","+Numtostr(StoRSIindBX[RightStrength+15],9)
+","+Numtostr(StoRSIindBX[RightStrength+16],9)
+","+Numtostr(StoRSIindBX[RightStrength+17],9)
+","+Numtostr(StoRSIindBX[RightStrength+18],9)
+","+Numtostr(StoRSIindBX[RightStrength+19],9)
+","+Numtostr(StoRSIindBX[RightStrength+2],9)
+","+Numtostr(StoRSIindBX[RightStrength+20],9)
+","+Numtostr(StoRSIindBX[RightStrength+21],9)
+","+Numtostr(StoRSIindBX[RightStrength+22],9)
+","+Numtostr(StoRSIindBX[RightStrength+23],9)
+","+Numtostr(StoRSIindBX[RightStrength+24],9)
+","+Numtostr(StoRSIindBX[RightStrength+25],9)
+","+Numtostr(StoRSIindBX[RightStrength+26],9)
+","+Numtostr(StoRSIindBX[RightStrength+27],9)
+","+Numtostr(StoRSIindBX[RightStrength+28],9)
+","+Numtostr(StoRSIindBX[RightStrength+3],9)
+","+Numtostr(StoRSIindBX[RightStrength+4],9)
+","+Numtostr(StoRSIindBX[RightStrength+5],9)
+","+Numtostr(StoRSIindBX[RightStrength+6],9)
+","+Numtostr(StoRSIindBX[RightStrength+7],9)
+","+Numtostr(StoRSIindBX[RightStrength+8],9)
+","+Numtostr(StoRSIindBX[RightStrength+9],9)
+","+Numtostr(StoRSIindBX[RightStrength-1],9)
+","+Numtostr(StoRSIindBX[RightStrength-2],9)
+","+Numtostr(StoRSIindBX[RightStrength-3],9)
+","+Numtostr(StoRSIindBX[RightStrength-4],9)
+","+Numtostr(StoRSIindBXL[RightStrength+0],9)
+","+Numtostr(StoRSIindBXL[RightStrength+1],9)
+","+Numtostr(StoRSIindBXL[RightStrength+10],9)
+","+Numtostr(StoRSIindBXL[RightStrength+11],9)
+","+Numtostr(StoRSIindBXL[RightStrength+12],9)
+","+Numtostr(StoRSIindBXL[RightStrength+13],9)
+","+Numtostr(StoRSIindBXL[RightStrength+14],9)
+","+Numtostr(StoRSIindBXL[RightStrength+15],9)
+","+Numtostr(StoRSIindBXL[RightStrength+16],9)
+","+Numtostr(StoRSIindBXL[RightStrength+17],9)
+","+Numtostr(StoRSIindBXL[RightStrength+18],9)
+","+Numtostr(StoRSIindBXL[RightStrength+19],9)
+","+Numtostr(StoRSIindBXL[RightStrength+2],9)
+","+Numtostr(StoRSIindBXL[RightStrength+20],9)
+","+Numtostr(StoRSIindBXL[RightStrength+21],9)
+","+Numtostr(StoRSIindBXL[RightStrength+22],9)
+","+Numtostr(StoRSIindBXL[RightStrength+23],9)
+","+Numtostr(StoRSIindBXL[RightStrength+24],9)
+","+Numtostr(StoRSIindBXL[RightStrength+25],9)
+","+Numtostr(StoRSIindBXL[RightStrength+26],9)
+","+Numtostr(StoRSIindBXL[RightStrength+27],9)
+","+Numtostr(StoRSIindBXL[RightStrength+28],9)
+","+Numtostr(StoRSIindBXL[RightStrength+3],9)
+","+Numtostr(StoRSIindBXL[RightStrength+4],9)
+","+Numtostr(StoRSIindBXL[RightStrength+5],9)
+","+Numtostr(StoRSIindBXL[RightStrength+6],9)
+","+Numtostr(StoRSIindBXL[RightStrength+7],9)
+","+Numtostr(StoRSIindBXL[RightStrength+8],9)
+","+Numtostr(StoRSIindBXL[RightStrength+9],9)
+","+Numtostr(StoRSIindBXL[RightStrength-1],9)
+","+Numtostr(StoRSIindBXL[RightStrength-2],9)
+","+Numtostr(StoRSIindBXL[RightStrength-3],9)
+","+Numtostr(StoRSIindBXL[RightStrength-4],9)
+","+Numtostr(StoRSIindMED[RightStrength+0],9)
+","+Numtostr(StoRSIindMED[RightStrength+1],9)
+","+Numtostr(StoRSIindMED[RightStrength+10],9)
+","+Numtostr(StoRSIindMED[RightStrength+11],9)
+","+Numtostr(StoRSIindMED[RightStrength+12],9)
+","+Numtostr(StoRSIindMED[RightStrength+13],9)
+","+Numtostr(StoRSIindMED[RightStrength+14],9)
+","+Numtostr(StoRSIindMED[RightStrength+15],9)
+","+Numtostr(StoRSIindMED[RightStrength+16],9)
+","+Numtostr(StoRSIindMED[RightStrength+17],9)
+","+Numtostr(StoRSIindMED[RightStrength+18],9)
+","+Numtostr(StoRSIindMED[RightStrength+19],9)
+","+Numtostr(StoRSIindMED[RightStrength+2],9)
+","+Numtostr(StoRSIindMED[RightStrength+20],9)
+","+Numtostr(StoRSIindMED[RightStrength+21],9)
+","+Numtostr(StoRSIindMED[RightStrength+22],9)
+","+Numtostr(StoRSIindMED[RightStrength+23],9)
+","+Numtostr(StoRSIindMED[RightStrength+24],9)
+","+Numtostr(StoRSIindMED[RightStrength+25],9)
+","+Numtostr(StoRSIindMED[RightStrength+26],9)
+","+Numtostr(StoRSIindMED[RightStrength+27],9)
+","+Numtostr(StoRSIindMED[RightStrength+28],9)
+","+Numtostr(StoRSIindMED[RightStrength+3],9)
+","+Numtostr(StoRSIindMED[RightStrength+4],9)
+","+Numtostr(StoRSIindMED[RightStrength+5],9)
+","+Numtostr(StoRSIindMED[RightStrength+6],9)
+","+Numtostr(StoRSIindMED[RightStrength+7],9)
+","+Numtostr(StoRSIindMED[RightStrength+8],9)
+","+Numtostr(StoRSIindMED[RightStrength+9],9)
+","+Numtostr(StoRSIindMED[RightStrength-1],9)
+","+Numtostr(StoRSIindMED[RightStrength-2],9)
+","+Numtostr(StoRSIindMED[RightStrength-3],9)
+","+Numtostr(StoRSIindMED[RightStrength-4],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+0],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+1],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+10],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+11],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+12],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+13],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+14],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+15],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+16],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+17],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+18],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+19],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+2],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+20],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+21],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+22],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+23],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+24],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+25],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+26],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+27],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+28],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+3],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+4],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+5],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+6],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+7],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+8],9)
+","+Numtostr(StoRSIindSMALL[RightStrength+9],9)
+","+Numtostr(StoRSIindSMALL[RightStrength-1],9)
+","+Numtostr(StoRSIindSMALL[RightStrength-2],9)
+","+Numtostr(StoRSIindSMALL[RightStrength-3],9)
+","+Numtostr(StoRSIindSMALL[RightStrength-4],9)
+","+Numtostr(STOSindBIG[RightStrength+0],9)
+","+Numtostr(STOSindBIG[RightStrength+1],9)
+","+Numtostr(STOSindBIG[RightStrength+10],9)
+","+Numtostr(STOSindBIG[RightStrength+11],9)
+","+Numtostr(STOSindBIG[RightStrength+12],9)
+","+Numtostr(STOSindBIG[RightStrength+13],9)
+","+Numtostr(STOSindBIG[RightStrength+14],9)
+","+Numtostr(STOSindBIG[RightStrength+15],9)
+","+Numtostr(STOSindBIG[RightStrength+16],9)
+","+Numtostr(STOSindBIG[RightStrength+17],9)
+","+Numtostr(STOSindBIG[RightStrength+18],9)
+","+Numtostr(STOSindBIG[RightStrength+19],9)
+","+Numtostr(STOSindBIG[RightStrength+2],9)
+","+Numtostr(STOSindBIG[RightStrength+20],9)
+","+Numtostr(STOSindBIG[RightStrength+21],9)
+","+Numtostr(STOSindBIG[RightStrength+22],9)
+","+Numtostr(STOSindBIG[RightStrength+23],9)
+","+Numtostr(STOSindBIG[RightStrength+24],9)
+","+Numtostr(STOSindBIG[RightStrength+25],9)
+","+Numtostr(STOSindBIG[RightStrength+26],9)
+","+Numtostr(STOSindBIG[RightStrength+27],9)
+","+Numtostr(STOSindBIG[RightStrength+28],9)
+","+Numtostr(STOSindBIG[RightStrength+3],9)
+","+Numtostr(STOSindBIG[RightStrength+4],9)
+","+Numtostr(STOSindBIG[RightStrength+5],9)
+","+Numtostr(STOSindBIG[RightStrength+6],9)
+","+Numtostr(STOSindBIG[RightStrength+7],9)
+","+Numtostr(STOSindBIG[RightStrength+8],9)
+","+Numtostr(STOSindBIG[RightStrength+9],9)
+","+Numtostr(STOSindBIG[RightStrength-1],9)
+","+Numtostr(STOSindBIG[RightStrength-2],9)
+","+Numtostr(STOSindBIG[RightStrength-3],9)
+","+Numtostr(STOSindBIG[RightStrength-4],9)
+","+Numtostr(STOSindBX[RightStrength+0],9)
+","+Numtostr(STOSindBX[RightStrength+1],9)
+","+Numtostr(STOSindBX[RightStrength+10],9)
+","+Numtostr(STOSindBX[RightStrength+11],9)
+","+Numtostr(STOSindBX[RightStrength+12],9)
+","+Numtostr(STOSindBX[RightStrength+13],9)
+","+Numtostr(STOSindBX[RightStrength+14],9)
+","+Numtostr(STOSindBX[RightStrength+15],9)
+","+Numtostr(STOSindBX[RightStrength+16],9)
+","+Numtostr(STOSindBX[RightStrength+17],9)
+","+Numtostr(STOSindBX[RightStrength+18],9)
+","+Numtostr(STOSindBX[RightStrength+19],9)
+","+Numtostr(STOSindBX[RightStrength+2],9)
+","+Numtostr(STOSindBX[RightStrength+20],9)
+","+Numtostr(STOSindBX[RightStrength+21],9)
+","+Numtostr(STOSindBX[RightStrength+22],9)
+","+Numtostr(STOSindBX[RightStrength+23],9)
+","+Numtostr(STOSindBX[RightStrength+24],9)
+","+Numtostr(STOSindBX[RightStrength+25],9)
+","+Numtostr(STOSindBX[RightStrength+26],9)
+","+Numtostr(STOSindBX[RightStrength+27],9)
+","+Numtostr(STOSindBX[RightStrength+28],9)
+","+Numtostr(STOSindBX[RightStrength+3],9)
+","+Numtostr(STOSindBX[RightStrength+4],9)
+","+Numtostr(STOSindBX[RightStrength+5],9)
+","+Numtostr(STOSindBX[RightStrength+6],9)
+","+Numtostr(STOSindBX[RightStrength+7],9)
+","+Numtostr(STOSindBX[RightStrength+8],9)
+","+Numtostr(STOSindBX[RightStrength+9],9)
+","+Numtostr(STOSindBX[RightStrength-1],9)
+","+Numtostr(STOSindBX[RightStrength-2],9)
+","+Numtostr(STOSindBX[RightStrength-3],9)
+","+Numtostr(STOSindBX[RightStrength-4],9)
+","+Numtostr(STOSindBXL[RightStrength+0],9)
+","+Numtostr(STOSindBXL[RightStrength+1],9)
+","+Numtostr(STOSindBXL[RightStrength+10],9)
+","+Numtostr(STOSindBXL[RightStrength+11],9)
+","+Numtostr(STOSindBXL[RightStrength+12],9)
+","+Numtostr(STOSindBXL[RightStrength+13],9)
+","+Numtostr(STOSindBXL[RightStrength+14],9)
+","+Numtostr(STOSindBXL[RightStrength+15],9)
+","+Numtostr(STOSindBXL[RightStrength+16],9)
+","+Numtostr(STOSindBXL[RightStrength+17],9)
+","+Numtostr(STOSindBXL[RightStrength+18],9)
+","+Numtostr(STOSindBXL[RightStrength+19],9)
+","+Numtostr(STOSindBXL[RightStrength+2],9)
+","+Numtostr(STOSindBXL[RightStrength+20],9)
+","+Numtostr(STOSindBXL[RightStrength+21],9)
+","+Numtostr(STOSindBXL[RightStrength+22],9)
+","+Numtostr(STOSindBXL[RightStrength+23],9)
+","+Numtostr(STOSindBXL[RightStrength+24],9)
+","+Numtostr(STOSindBXL[RightStrength+25],9)
+","+Numtostr(STOSindBXL[RightStrength+26],9)
+","+Numtostr(STOSindBXL[RightStrength+27],9)
+","+Numtostr(STOSindBXL[RightStrength+28],9)
+","+Numtostr(STOSindBXL[RightStrength+3],9)
+","+Numtostr(STOSindBXL[RightStrength+4],9)
			+" )"; 
			res9v02 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_feat9v02);
			srun9v02 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures9v02.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_feat9v02
					+ NewLine);
			end;
		End;
	End;
	 TblNum = TblNum+1;
	 

	//(4) insert feature data into dbo.barFeatures10v02	
	if ReadWriteToDB and TblNum = 11+10  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		if  srun10v02 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_feat10v02 = "INSERT INTO  [dbo].[barFeatures10v02] VALUES ("
			+queryResult//MAX(loadId) 
			+","+Numtostr(loadGroupId,0) 
			+","+Numtostr(RightStrength,0)//RightStrength
			+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
			+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"'"+dateBarOpened+"' "+","+"'"+dayOfWeekBarOpened+"' "+","+"'"+dateTimeBarOpened+"' "+","+"'"+timeBarOpenedhhmmss+"' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)
+","+Numtostr(VIDYAindBXL[RightStrength-1],9)
+","+Numtostr(VIDYAindBXL[RightStrength-2],9)
+","+Numtostr(VIDYAindBXL[RightStrength-3],9)
+","+Numtostr(VIDYAindBXL[RightStrength-4],9)
+","+Numtostr(VIDYAindMED[RightStrength+0],9)
+","+Numtostr(VIDYAindMED[RightStrength+1],9)
+","+Numtostr(VIDYAindMED[RightStrength+10],9)
+","+Numtostr(VIDYAindMED[RightStrength+11],9)
+","+Numtostr(VIDYAindMED[RightStrength+12],9)
+","+Numtostr(VIDYAindMED[RightStrength+13],9)
+","+Numtostr(VIDYAindMED[RightStrength+14],9)
+","+Numtostr(VIDYAindMED[RightStrength+15],9)
+","+Numtostr(VIDYAindMED[RightStrength+16],9)
+","+Numtostr(VIDYAindMED[RightStrength+17],9)
+","+Numtostr(VIDYAindMED[RightStrength+18],9)
+","+Numtostr(VIDYAindMED[RightStrength+19],9)
+","+Numtostr(VIDYAindMED[RightStrength+2],9)
+","+Numtostr(VIDYAindMED[RightStrength+20],9)
+","+Numtostr(VIDYAindMED[RightStrength+21],9)
+","+Numtostr(VIDYAindMED[RightStrength+22],9)
+","+Numtostr(VIDYAindMED[RightStrength+23],9)
+","+Numtostr(VIDYAindMED[RightStrength+24],9)
+","+Numtostr(VIDYAindMED[RightStrength+25],9)
+","+Numtostr(VIDYAindMED[RightStrength+26],9)
+","+Numtostr(VIDYAindMED[RightStrength+27],9)
+","+Numtostr(VIDYAindMED[RightStrength+28],9)
+","+Numtostr(VIDYAindMED[RightStrength+3],9)
+","+Numtostr(VIDYAindMED[RightStrength+4],9)
+","+Numtostr(VIDYAindMED[RightStrength+5],9)
+","+Numtostr(VIDYAindMED[RightStrength+6],9)
+","+Numtostr(VIDYAindMED[RightStrength+7],9)
+","+Numtostr(VIDYAindMED[RightStrength+8],9)
+","+Numtostr(VIDYAindMED[RightStrength+9],9)
+","+Numtostr(VIDYAindMED[RightStrength-1],9)
+","+Numtostr(VIDYAindMED[RightStrength-2],9)
+","+Numtostr(VIDYAindMED[RightStrength-3],9)
+","+Numtostr(VIDYAindMED[RightStrength-4],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+0],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+1],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+10],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+11],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+12],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+13],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+14],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+15],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+16],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+17],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+18],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+19],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+2],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+20],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+21],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+22],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+23],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+24],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+25],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+26],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+27],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+28],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+3],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+4],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+5],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+6],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+7],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+8],9)
+","+Numtostr(VIDYAindSMALL[RightStrength+9],9)
+","+Numtostr(VIDYAindSMALL[RightStrength-1],9)
+","+Numtostr(VIDYAindSMALL[RightStrength-2],9)
+","+Numtostr(VIDYAindSMALL[RightStrength-3],9)
+","+Numtostr(VIDYAindSMALL[RightStrength-4],9)
+","+Numtostr(VIMAindBIG[RightStrength+0],9)
+","+Numtostr(VIMAindBIG[RightStrength+1],9)
+","+Numtostr(VIMAindBIG[RightStrength+10],9)
+","+Numtostr(VIMAindBIG[RightStrength+11],9)
+","+Numtostr(VIMAindBIG[RightStrength+12],9)
+","+Numtostr(VIMAindBIG[RightStrength+13],9)
+","+Numtostr(VIMAindBIG[RightStrength+14],9)
+","+Numtostr(VIMAindBIG[RightStrength+15],9)
+","+Numtostr(VIMAindBIG[RightStrength+16],9)
+","+Numtostr(VIMAindBIG[RightStrength+17],9)
+","+Numtostr(VIMAindBIG[RightStrength+18],9)
+","+Numtostr(VIMAindBIG[RightStrength+19],9)
+","+Numtostr(VIMAindBIG[RightStrength+2],9)
+","+Numtostr(VIMAindBIG[RightStrength+20],9)
+","+Numtostr(VIMAindBIG[RightStrength+21],9)
+","+Numtostr(VIMAindBIG[RightStrength+22],9)
+","+Numtostr(VIMAindBIG[RightStrength+23],9)
+","+Numtostr(VIMAindBIG[RightStrength+24],9)
+","+Numtostr(VIMAindBIG[RightStrength+25],9)
+","+Numtostr(VIMAindBIG[RightStrength+26],9)
+","+Numtostr(VIMAindBIG[RightStrength+27],9)
+","+Numtostr(VIMAindBIG[RightStrength+28],9)
+","+Numtostr(VIMAindBIG[RightStrength+3],9)
+","+Numtostr(VIMAindBIG[RightStrength+4],9)
+","+Numtostr(VIMAindBIG[RightStrength+5],9)
+","+Numtostr(VIMAindBIG[RightStrength+6],9)
+","+Numtostr(VIMAindBIG[RightStrength+7],9)
+","+Numtostr(VIMAindBIG[RightStrength+8],9)
+","+Numtostr(VIMAindBIG[RightStrength+9],9)
+","+Numtostr(VIMAindBIG[RightStrength-1],9)
+","+Numtostr(VIMAindBIG[RightStrength-2],9)
+","+Numtostr(VIMAindBIG[RightStrength-3],9)
+","+Numtostr(VIMAindBIG[RightStrength-4],9)
+","+Numtostr(VIMAindBX[RightStrength+0],9)
+","+Numtostr(VIMAindBX[RightStrength+1],9)
+","+Numtostr(VIMAindBX[RightStrength+10],9)
+","+Numtostr(VIMAindBX[RightStrength+11],9)
+","+Numtostr(VIMAindBX[RightStrength+12],9)
+","+Numtostr(VIMAindBX[RightStrength+13],9)
+","+Numtostr(VIMAindBX[RightStrength+14],9)
+","+Numtostr(VIMAindBX[RightStrength+15],9)
+","+Numtostr(VIMAindBX[RightStrength+16],9)
+","+Numtostr(VIMAindBX[RightStrength+17],9)
+","+Numtostr(VIMAindBX[RightStrength+18],9)
+","+Numtostr(VIMAindBX[RightStrength+19],9)
+","+Numtostr(VIMAindBX[RightStrength+2],9)
+","+Numtostr(VIMAindBX[RightStrength+20],9)
+","+Numtostr(VIMAindBX[RightStrength+21],9)
+","+Numtostr(VIMAindBX[RightStrength+22],9)
+","+Numtostr(VIMAindBX[RightStrength+23],9)
+","+Numtostr(VIMAindBX[RightStrength+24],9)
+","+Numtostr(VIMAindBX[RightStrength+25],9)
+","+Numtostr(VIMAindBX[RightStrength+26],9)
+","+Numtostr(VIMAindBX[RightStrength+27],9)
+","+Numtostr(VIMAindBX[RightStrength+28],9)
+","+Numtostr(VIMAindBX[RightStrength+3],9)
+","+Numtostr(VIMAindBX[RightStrength+4],9)
+","+Numtostr(VIMAindBX[RightStrength+5],9)
+","+Numtostr(VIMAindBX[RightStrength+6],9)
+","+Numtostr(VIMAindBX[RightStrength+7],9)
+","+Numtostr(VIMAindBX[RightStrength+8],9)
+","+Numtostr(VIMAindBX[RightStrength+9],9)
+","+Numtostr(VIMAindBX[RightStrength-1],9)
+","+Numtostr(VIMAindBX[RightStrength-2],9)
+","+Numtostr(VIMAindBX[RightStrength-3],9)
+","+Numtostr(VIMAindBX[RightStrength-4],9)
+","+Numtostr(VIMAindBXL[RightStrength+0],9)
+","+Numtostr(VIMAindBXL[RightStrength+1],9)
+","+Numtostr(VIMAindBXL[RightStrength+10],9)
+","+Numtostr(VIMAindBXL[RightStrength+11],9)
+","+Numtostr(VIMAindBXL[RightStrength+12],9)
+","+Numtostr(VIMAindBXL[RightStrength+13],9)
+","+Numtostr(VIMAindBXL[RightStrength+14],9)
+","+Numtostr(VIMAindBXL[RightStrength+15],9)
+","+Numtostr(VIMAindBXL[RightStrength+16],9)
+","+Numtostr(VIMAindBXL[RightStrength+17],9)
+","+Numtostr(VIMAindBXL[RightStrength+18],9)
+","+Numtostr(VIMAindBXL[RightStrength+19],9)
+","+Numtostr(VIMAindBXL[RightStrength+2],9)
+","+Numtostr(VIMAindBXL[RightStrength+20],9)
+","+Numtostr(VIMAindBXL[RightStrength+21],9)
+","+Numtostr(VIMAindBXL[RightStrength+22],9)
+","+Numtostr(VIMAindBXL[RightStrength+23],9)
+","+Numtostr(VIMAindBXL[RightStrength+24],9)
+","+Numtostr(VIMAindBXL[RightStrength+25],9)
+","+Numtostr(VIMAindBXL[RightStrength+26],9)
+","+Numtostr(VIMAindBXL[RightStrength+27],9)
+","+Numtostr(VIMAindBXL[RightStrength+28],9)
+","+Numtostr(VIMAindBXL[RightStrength+3],9)
+","+Numtostr(VIMAindBXL[RightStrength+4],9)
+","+Numtostr(VIMAindBXL[RightStrength+5],9)
+","+Numtostr(VIMAindBXL[RightStrength+6],9)
+","+Numtostr(VIMAindBXL[RightStrength+7],9)
+","+Numtostr(VIMAindBXL[RightStrength+8],9)
+","+Numtostr(VIMAindBXL[RightStrength+9],9)
+","+Numtostr(VIMAindBXL[RightStrength-1],9)
+","+Numtostr(VIMAindBXL[RightStrength-2],9)
+","+Numtostr(VIMAindBXL[RightStrength-3],9)
+","+Numtostr(VIMAindBXL[RightStrength-4],9)
+","+Numtostr(VIMAindMED[RightStrength+0],9)
+","+Numtostr(VIMAindMED[RightStrength+1],9)
+","+Numtostr(VIMAindMED[RightStrength+10],9)
+","+Numtostr(VIMAindMED[RightStrength+11],9)
+","+Numtostr(VIMAindMED[RightStrength+12],9)
+","+Numtostr(VIMAindMED[RightStrength+13],9)
+","+Numtostr(VIMAindMED[RightStrength+14],9)
+","+Numtostr(VIMAindMED[RightStrength+15],9)
+","+Numtostr(VIMAindMED[RightStrength+16],9)
+","+Numtostr(VIMAindMED[RightStrength+17],9)
+","+Numtostr(VIMAindMED[RightStrength+18],9)
+","+Numtostr(VIMAindMED[RightStrength+19],9)
+","+Numtostr(VIMAindMED[RightStrength+2],9)
+","+Numtostr(VIMAindMED[RightStrength+20],9)
+","+Numtostr(VIMAindMED[RightStrength+21],9)
+","+Numtostr(VIMAindMED[RightStrength+22],9)
+","+Numtostr(VIMAindMED[RightStrength+23],9)
+","+Numtostr(VIMAindMED[RightStrength+24],9)
+","+Numtostr(VIMAindMED[RightStrength+25],9)
+","+Numtostr(VIMAindMED[RightStrength+26],9)
+","+Numtostr(VIMAindMED[RightStrength+27],9)
+","+Numtostr(VIMAindMED[RightStrength+28],9)
+","+Numtostr(VIMAindMED[RightStrength+3],9)
+","+Numtostr(VIMAindMED[RightStrength+4],9)
+","+Numtostr(VIMAindMED[RightStrength+5],9)
+","+Numtostr(VIMAindMED[RightStrength+6],9)
+","+Numtostr(VIMAindMED[RightStrength+7],9)
+","+Numtostr(VIMAindMED[RightStrength+8],9)
+","+Numtostr(VIMAindMED[RightStrength+9],9)
+","+Numtostr(VIMAindMED[RightStrength-1],9)
+","+Numtostr(VIMAindMED[RightStrength-2],9)
+","+Numtostr(VIMAindMED[RightStrength-3],9)
+","+Numtostr(VIMAindMED[RightStrength-4],9)
+","+Numtostr(VIMAindSMALL[RightStrength+0],9)
+","+Numtostr(VIMAindSMALL[RightStrength+1],9)
+","+Numtostr(VIMAindSMALL[RightStrength+10],9)
+","+Numtostr(VIMAindSMALL[RightStrength+11],9)
+","+Numtostr(VIMAindSMALL[RightStrength+12],9)
+","+Numtostr(VIMAindSMALL[RightStrength+13],9)
+","+Numtostr(VIMAindSMALL[RightStrength+14],9)
+","+Numtostr(VIMAindSMALL[RightStrength+15],9)
+","+Numtostr(VIMAindSMALL[RightStrength+16],9)
+","+Numtostr(VIMAindSMALL[RightStrength+17],9)
+","+Numtostr(VIMAindSMALL[RightStrength+18],9)
+","+Numtostr(VIMAindSMALL[RightStrength+19],9)
+","+Numtostr(VIMAindSMALL[RightStrength+2],9)
+","+Numtostr(VIMAindSMALL[RightStrength+20],9)
+","+Numtostr(VIMAindSMALL[RightStrength+21],9)
+","+Numtostr(VIMAindSMALL[RightStrength+22],9)
+","+Numtostr(VIMAindSMALL[RightStrength+23],9)
+","+Numtostr(VIMAindSMALL[RightStrength+24],9)
+","+Numtostr(VIMAindSMALL[RightStrength+25],9)
+","+Numtostr(VIMAindSMALL[RightStrength+26],9)
+","+Numtostr(VIMAindSMALL[RightStrength+27],9)
+","+Numtostr(VIMAindSMALL[RightStrength+28],9)
+","+Numtostr(VIMAindSMALL[RightStrength+3],9)
+","+Numtostr(VIMAindSMALL[RightStrength+4],9)
+","+Numtostr(VIMAindSMALL[RightStrength+5],9)
+","+Numtostr(VIMAindSMALL[RightStrength+6],9)
+","+Numtostr(VIMAindSMALL[RightStrength+7],9)
+","+Numtostr(VIMAindSMALL[RightStrength+8],9)
+","+Numtostr(VIMAindSMALL[RightStrength+9],9)
+","+Numtostr(VIMAindSMALL[RightStrength-1],9)
+","+Numtostr(VIMAindSMALL[RightStrength-2],9)
+","+Numtostr(VIMAindSMALL[RightStrength-3],9)
+","+Numtostr(VIMAindSMALL[RightStrength-4],9)
			+" )"; 
			res10v02 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_feat10v02);
			srun10v02 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures10v02.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_feat10v02
					+ NewLine);
			end;
		End;
	End;
	 TblNum = TblNum+1;


	//(4) insert feature data into dbo.barFeatures11v02	
	if ReadWriteToDB and TblNum = 11+11  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	
		if  srun11v02 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin
			sql_insert_feat11v02 = "INSERT INTO  [dbo].[barFeatures11v02] VALUES ("
			+queryResult//MAX(loadId) 
			+","+Numtostr(loadGroupId,0) 
			+","+Numtostr(RightStrength,0)//RightStrength
			+","+"'"+queryResultdatetime+"' "//[recCreateDateTime]
			+","+"'"+queryResultdatetime+"' "//[recModiedDateTime]
			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"'"+dateBarOpened+"' "+","+"'"+dayOfWeekBarOpened+"' "+","+"'"+dateTimeBarOpened+"' "+","+"'"+timeBarOpenedhhmmss+"' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)
+","+Numtostr(barRange250Bars[RightStrength+1],9)
+","+Numtostr(barRange250Bars[RightStrength+2],9)
+","+Numtostr(barRange250Bars[RightStrength+3],9)
+","+Numtostr(barRange250Bars[RightStrength+4],9)
+","+Numtostr(barRange250Bars[RightStrength+5],9)
+","+Numtostr(barRange250Bars[RightStrength+6],9)
+","+Numtostr(barRange250Bars[RightStrength+7],9)
+","+Numtostr(barRange250Bars[RightStrength+8],9)
+","+Numtostr(barRange250Bars[RightStrength+9],9)
+","+Numtostr(barRange250Bars[RightStrength+10],9)
+","+Numtostr(barRange250Bars[RightStrength+11],9)
+","+Numtostr(barRange250Bars[RightStrength+12],9)
+","+Numtostr(barRange250Bars[RightStrength+13],9)
+","+Numtostr(barRange250Bars[RightStrength+14],9)
+","+Numtostr(barRange250Bars[RightStrength+15],9)
+","+Numtostr(barRange250Bars[RightStrength+16],9)
+","+Numtostr(barRange250Bars[RightStrength+17],9)
+","+Numtostr(barRange250Bars[RightStrength+18],9)
+","+Numtostr(barRange250Bars[RightStrength+19],9)
+","+Numtostr(barRange250Bars[RightStrength+20],9)
+","+Numtostr(barRange250Bars[RightStrength+21],9)
+","+Numtostr(barRange250Bars[RightStrength+22],9)
+","+Numtostr(barRange250Bars[RightStrength+23],9)
+","+Numtostr(barRange250Bars[RightStrength+24],9)
+","+Numtostr(barRange250Bars[RightStrength+25],9)
+","+Numtostr(barRange250Bars[RightStrength+26],9)
+","+Numtostr(barRange250Bars[RightStrength+27],9)
+","+Numtostr(barRange250Bars[RightStrength+28],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength-4],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength-3],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength-2],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength-1],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+0],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+1],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+2],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+3],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+4],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+5],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+6],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+7],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+8],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+9],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+10],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+11],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+12],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+13],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+14],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+15],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+16],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+17],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+18],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+19],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+20],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+21],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+22],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+23],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+24],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+25],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+26],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+27],9)
+","+Numtostr(AVGvsMed2Bars[RightStrength+28],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength-4],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength-3],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength-2],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength-1],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+0],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+1],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+2],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+3],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+4],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+5],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+6],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+7],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+8],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+9],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+10],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+11],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+12],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+13],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+14],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+15],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+16],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+17],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+18],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+19],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+20],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+21],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+22],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+23],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+24],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+25],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+26],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+27],9)
+","+Numtostr(AVGvsMed3Bars[RightStrength+28],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength-4],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength-3],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength-2],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength-1],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+0],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+1],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+2],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+3],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+4],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+5],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+6],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+7],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+8],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+9],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+10],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+11],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+12],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+13],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+14],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+15],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+16],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+17],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+18],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+19],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+20],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+21],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+22],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+23],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+24],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+25],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+26],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+27],9)
+","+Numtostr(AVGvsMed5Bars[RightStrength+28],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength-4],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength-3],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength-2],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength-1],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+0],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+1],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+2],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+3],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+4],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+5],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+6],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+7],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+8],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+9],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+10],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+11],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+12],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+13],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+14],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+15],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+16],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+17],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+18],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+19],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+20],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+21],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+22],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+23],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+24],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+25],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+26],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+27],9)
+","+Numtostr(AVGvsMed10Bars[RightStrength+28],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength-4],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength-3],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength-2],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength-1],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+0],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+1],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+2],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+3],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+4],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+5],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+6],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+7],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+8],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+9],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+10],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+11],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+12],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+13],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+14],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+15],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+16],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+17],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+18],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+19],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+20],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+21],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+22],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+23],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+24],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+25],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+26],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+27],9)
+","+Numtostr(AVGvsMed20Bars[RightStrength+28],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength-4],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength-3],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength-2],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength-1],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+0],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+1],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+2],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+3],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+4],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+5],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+6],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+7],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+8],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+9],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+10],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+11],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+12],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+13],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+14],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+15],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+16],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+17],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+18],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+19],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+20],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+21],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+22],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+23],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+24],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+25],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+26],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+27],9)
+","+Numtostr(AVGvsMed40Bars[RightStrength+28],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength-4],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength-3],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength-2],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength-1],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+0],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+1],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+2],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+3],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+4],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+5],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+6],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+7],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+8],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+9],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+10],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+11],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+12],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+13],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+14],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+15],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+16],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+17],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+18],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+19],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+20],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+21],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+22],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+23],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+24],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+25],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+26],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+27],9)
+","+Numtostr(AVGvsMed80Bars[RightStrength+28],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength-4],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength-3],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength-2],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength-1],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+0],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+1],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+2],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+3],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+4],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+5],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+6],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+7],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+8],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+9],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+10],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+11],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+12],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+13],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+14],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+15],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+16],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+17],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+18],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+19],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+20],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+21],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+22],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+23],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+24],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+25],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+26],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+27],9)
+","+Numtostr(AVGvsMed120Bars[RightStrength+28],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength-4],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength-3],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength-2],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength-1],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+0],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+1],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+2],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+3],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+4],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+5],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+6],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+7],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+8],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+9],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+10],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+11],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+12],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+13],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+14],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+15],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+16],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+17],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+18],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+19],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+20],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+21],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+22],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+23],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+24],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+25],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+26],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+27],9)
+","+Numtostr(AVGvsMed185Bars[RightStrength+28],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength-4],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength-3],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength-2],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength-1],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+0],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+1],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+2],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+3],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+4],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+5],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+6],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+7],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+8],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+9],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+10],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+11],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+12],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+13],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+14],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+15],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+16],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+17],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+18],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+19],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+20],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+21],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+22],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+23],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+24],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+25],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+26],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+27],9)
+","+Numtostr(AVGvsMed250Bars[RightStrength+28],9)
			+" )"; 
			res11v02 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_feat11v02);
			srun11v02 = 1;
			if debug then begin	
				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures11v02.csv")
					,",",NumToStr(swingRecordType,0)
					,",",NumToStr(barRecordType,0)
					,",",swingSize
					,"Currentbar = ",Currentbar
					,"***,",sql_insert_feat11v02
					+ NewLine);
			end;
		End;
	End;
	 TblNum = TblNum+1;


{	if 1 > 2 then begin	
			Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\swingData_ES_test.csv")
				,",",NumToStr(swingRecordType,0)
				,",",NumToStr(barRecordType,0)
				,",",swingSize
				,"Currentbar = ",Currentbar
				,resMeta," | ",sql_insert_meta
				,res0," | ",sql_insert_features0
				+ NewLine);
	end;}

 TblNum = TblNum+1;
End;
