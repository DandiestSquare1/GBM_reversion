// Function returns percentChange as a double = ( ( (New-Old)/Old ) ) * 100
{/**
IF(NEW > 0 and NEW > OLD,
    IF(OLD <> 0, (NEW - OLD)/ ABS(OLD), (NEW-OLD)/(1) ),
    IF(OLD <> 0, (OLD - NEW)/ NEW, (NEW-OLD)/(1) )
)
//excel
=IF(AND(B2>0,B2>A2),
    IF(A2<>0, (B2-A2)/ABS(A2), (B2-A2)/(1) ),
    IF(A2<>0, (A2-B2)/B2, (B2-A2)/(1) )
)
**/}
inputs: double dataSeries( numericseries ), //price or indicator values
		int lookBackLength( numericsimple ),//12
		int roundPrecision(numericsimple);//5
var:
	double percent_change(0),
	double k(0),
	int n(0),
	int p(0);
k = dataSeries;
n = lookBackLength;
p = roundPrecision;

//check sign of current value
If k > 0 and k > k[n] then begin
	If k[n] <> 0 then 
		percent_change = ( k-k[n] ) / Absvalue(k[n]) else begin	
		percent_change = ( k-k[n] ) / 1;
	end;
end
else begin		
	If k[n] <> 0 then 
		percent_change = ( k[n]-k ) / k	else begin	
		percent_change = ( k-k[n] ) / 1;
	end;
end;

If percent_change <> 0 then 
	PercentChange.cpp = round(percent_change,p)
else begin	
	PercentChange.cpp = 0;
end;

//Example use: 
// PercentChange.cpp(dataSeries,lookBackLength,roundPrecision)
// PercentChange.cpp(dataSeries,12,5) where [dataSeries] is raw indicator or price value
