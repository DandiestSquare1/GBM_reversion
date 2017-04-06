// Function returns a double rounded to [interval] >= 0 and <= [intervalLimit]

inputs: double dataSeries( numericseries ), //price or indicator values
	double interval(numericseries),//5 or 20, must be less than [intervalLimit]
	double intervalLimit(numericseries);//100

var: double counter(0);
counter = interval;

If dataSeries < interval Then Begin//case 1
	BucketData = 0;
End;
While counter < intervalLimit begin//case 2
	if dataSeries >= counter and dataSeries < (counter + interval) then begin
		BucketData = counter ;
		counter = intervalLimit ;//"max-out" counter to exit loop
	end
	else begin
		counter = (counter + interval) ;
	end;
end;
counter = interval;//reset counter
If dataSeries >= intervalLimit Then Begin//case 3
	BucketData = intervalLimit;
End;
//Example use: 
// BucketData(dataSeries,interval,intervalLimit)
// BucketData(dataSeries,10,0) where [dataSeries] is raw indicator or price value
