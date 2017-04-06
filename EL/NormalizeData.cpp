// Function returns a double >= [scaleFloor] and <= [scaleLimit] based on rescaling/normalizing the input dataSeries 
//linearlyRescale = (limit - floor) / (max[x] - min[x]) * (x - max[x]) + limit // where [x] is some lookback period and range is "hard coded" by setting [floor] and [limit] values to literals

inputs: double dataSeries( numericseries ), //price or indicator values
		double lookBackLength( numericsimple ),//250
		double scaleLimit(numericsimple),//100
		double scaleFloor(numericsimple);//0

{If dataSeries <> 0 and (Highest(dataSeries,lookBackLength) - Lowest(dataSeries,lookBackLength) <> 0) then 
	NormalizeData = (scaleLimit - scaleFloor) / (Highest(dataSeries,lookBackLength) - Lowest(dataSeries,lookBackLength)) * (dataSeries - Highest(dataSeries,lookBackLength)) + scaleLimit 
Else begin	
	NormalizeData = 0;
end;}

{If AbsValue(dataSeries) < 0.0001  then 
	NormalizeData = dataSeries * 100
end
Else If
If AbsValue(dataSeries) > 9999  then 
	NormalizeData = dataSeries / 100
end
Else begin	
	NormalizeData = dataSeries;
end;}
var: double RoundValue(0);

If dataSeries-dataSeries = 0 then begin
	RoundValue = Round(dataSeries, 5);
end
else begin
	RoundValue = 0;	
end;

NormalizeData = RoundValue;

//Example use: 
// NormalizeData(dataSeries,lookBackLength,scaleLimit,scaleFloor)
// NormalizeData(dataSeries,250,100,0) where [dataSeries] is raw indicator or price value
