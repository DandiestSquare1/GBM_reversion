/**
truncate table [daytradinglogic].[dbo].[barFeaturesMetaData]
truncate table [daytradinglogic].[dbo].[barFeatures0]
truncate table [daytradinglogic].[dbo].[barFeatures1]
truncate table [daytradinglogic].[dbo].[barFeatures1v02]
truncate table [daytradinglogic].[dbo].[barFeatures2]
truncate table [daytradinglogic].[dbo].[barFeatures2v02]
truncate table [daytradinglogic].[dbo].[barFeatures3]
truncate table [daytradinglogic].[dbo].[barFeatures4]
truncate table [daytradinglogic].[dbo].[barFeatures4v02]
truncate table [daytradinglogic].[dbo].[barFeatures5]
truncate table [daytradinglogic].[dbo].[barFeatures6]
truncate table [daytradinglogic].[dbo].[barFeatures7]
truncate table [daytradinglogic].[dbo].[barFeatures8]
truncate table [daytradinglogic].[dbo].[barFeatures9]
truncate table [daytradinglogic].[dbo].[barFeatures10]
truncate table [daytradinglogic].[dbo].[barFeatures11]
**/

SELECT format(COUNT(*),'N0') FROM [daytradinglogic].[dbo].[barFeaturesMetaData]
SELECT * FROM [daytradinglogic].[dbo].[barFeaturesMetaData]

SELECT format(COUNT(*),'N0') RecordCount FROM [daytradinglogic].[dbo].barFeatures0
SELECT loadId0,loadGroupId0,[barClass],format(COUNT(*),'N0') RecordCount ,Min(convert(date,dateTimeBarOpened0)) StartDate, MAX(convert(date,dateTimeBarOpened0)) EndDate ,DATEDIFF(MONTH, Min(convert(date,dateTimeBarOpened0)), MAX(convert(date,dateTimeBarOpened0))) +
    CASE 
        WHEN DAY(Min(convert(date,dateTimeBarOpened0))) < DAY(MAX(convert(date,dateTimeBarOpened0)))
        THEN 1 
        ELSE 0 
    END Months
FROM [daytradinglogic].[dbo].barFeatures0 GROUP BY loadGroupId0,[barClass],loadId0
SELECT [barClass],format(COUNT(*),'N0') RecordCount FROM [daytradinglogic].[dbo].barFeatures0 GROUP BY [barClass]
SELECT * FROM [daytradinglogic].[dbo].barFeatures0


SELECT format(COUNT(*),'N0') RecordCount FROM [daytradinglogic].[dbo].barFeatures11
SELECT loadId11,loadGroupId11,[barClass],format(COUNT(*),'N0') RecordCount ,Min(convert(date,dateTimeBarOpened11)) StartDate, MAX(convert(date,dateTimeBarOpened11)) EndDate ,DATEDIFF(MONTH, Min(convert(date,dateTimeBarOpened11)), MAX(convert(date,dateTimeBarOpened11))) +
    CASE 
        WHEN DAY(Min(convert(date,dateTimeBarOpened11))) < DAY(MAX(convert(date,dateTimeBarOpened11)))
        THEN 1 
        ELSE 0 
    END Months
FROM [daytradinglogic].[dbo].barFeatures11 GROUP BY loadGroupId11,[barClass],loadId11


SELECT format(COUNT(*),'N0') FROM [daytradinglogic].[dbo].barFeatures1
SELECT TOP 1 * FROM [daytradinglogic].[dbo].barFeatures1
SELECT Min(dateTimeBarOpened1), MAX(dateTimeBarOpened1) FROM [daytradinglogic].[dbo].barFeatures1
SELECT [barClass],format(COUNT(*),'N0') RecordCount FROM [daytradinglogic].[dbo].barFeatures1 GROUP BY [barClass]
SELECT format(COUNT(*),'N0') FROM [daytradinglogic].[dbo].barFeatures3

SELECT [barClass],format(COUNT(*),'N0') RecordCount FROM [daytradinglogic].[dbo].barFeatures3 GROUP BY [barClass]
SELECT swingSize,format(COUNT(*),'N0') RecordCount FROM [daytradinglogic].[dbo].barFeatures3 GROUP BY swingSize ORDER BY swingSize ASC
SELECT swingSize,format(COUNT(*),'N0') RecordCount FROM [daytradinglogic].[dbo].barFeatures3 GROUP BY swingSize ORDER BY cOUNT(*) DESC


DELETE FROM [daytradinglogic].[dbo].barFeatures1 WHERE loadId1 = 3

SELECT format(COUNT(*),'N0') FROM [daytradinglogic].[dbo].barFeatures2
SELECT * FROM [daytradinglogic].[dbo].barFeatures2

--INSERT INTO [dbo].[barFeatures0] ([loadId]) VALUES (6)
--INSERT INTO [dbo].[barFeatures1] ([loadId]) VALUES (6)
--Chart start and end date: 2/7/2011 through 9/5/2016

--last date: 05/25/2016
--first date: 10/07/2009

declare @groupId int; set @groupId = 1004;

SELECT 'barFeatures0' Tbl, loadGroupId0, loadId0 ,format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened0)) StartDate, MAX(convert(date,dateTimeBarOpened0)) EndDate 
    ,DATEDIFF(MONTH, Min(convert(date,dateTimeBarOpened0)), MAX(convert(date,dateTimeBarOpened0))) +
    CASE 
        WHEN DAY(Min(convert(date,dateTimeBarOpened0))) < DAY(MAX(convert(date,dateTimeBarOpened0)))
        THEN 1 
        ELSE 0 
    END Months
FROM [daytradinglogic].[dbo].barFeatures0 
--WHERE loadGroupId0 = @groupId
GROUP BY loadGroupId0,loadId0
ORDER BY loadGroupId0,loadId0 ASC

--SELECT COUNT(*) FROM [daytradinglogic].[dbo].barFeaturesXML0
--SELECT * FROM [daytradinglogic].[dbo].barFeaturesXML0

SELECT * FROM [daytradinglogic].[dbo].barFeaturesMetaData
SELECT 'barFeatures0' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened0)) StartDate, MAX(convert(date,dateTimeBarOpened0)) EndDate FROM [daytradinglogic].[dbo].barFeatures0 WHERE loadGroupId0 = @groupId UNION ALL
SELECT 'barFeatures1' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened1)) StartDate, MAX(convert(date,dateTimeBarOpened1)) EndDate FROM [daytradinglogic].[dbo].barFeatures1 WHERE loadGroupId1 = @groupId  UNION ALL
SELECT 'barFeatures2' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened2)) StartDate, MAX(convert(date,dateTimeBarOpened2)) EndDate FROM [daytradinglogic].[dbo].barFeatures2 WHERE loadGroupId2 = @groupId  UNION ALL
SELECT 'barFeatures3' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened3)) StartDate, MAX(convert(date,dateTimeBarOpened3)) EndDate FROM [daytradinglogic].[dbo].barFeatures3  WHERE loadGroupId3 = @groupId UNION ALL
SELECT 'barFeatures4' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened4)) StartDate, MAX(convert(date,dateTimeBarOpened4)) EndDate FROM [daytradinglogic].[dbo].barFeatures4 WHERE loadGroupId4 = @groupId  UNION ALL
SELECT 'barFeatures5' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened5)) StartDate, MAX(convert(date,dateTimeBarOpened5)) EndDate FROM [daytradinglogic].[dbo].barFeatures5  WHERE loadGroupId5 = @groupId UNION ALL
SELECT 'barFeatures6' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened6)) StartDate, MAX(convert(date,dateTimeBarOpened6)) EndDate FROM [daytradinglogic].[dbo].barFeatures6  WHERE loadGroupId6 = @groupId UNION ALL
SELECT 'barFeatures7' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened7)) StartDate, MAX(convert(date,dateTimeBarOpened7)) EndDate FROM [daytradinglogic].[dbo].barFeatures7 WHERE loadGroupId7 = @groupId  UNION ALL
SELECT 'barFeatures8' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened8)) StartDate, MAX(convert(date,dateTimeBarOpened8)) EndDate FROM [daytradinglogic].[dbo].barFeatures8 WHERE loadGroupId8 = @groupId  UNION ALL
SELECT 'barFeatures9' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened9)) StartDate, MAX(convert(date,dateTimeBarOpened9)) EndDate FROM [daytradinglogic].[dbo].barFeatures9 WHERE loadGroupId9 = @groupId  UNION ALL
SELECT 'barFeatures10' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened10)) StartDate, MAX(convert(date,dateTimeBarOpened10)) EndDate FROM [daytradinglogic].[dbo].barFeatures10  WHERE loadGroupId10 = @groupId UNION ALL
SELECT 'barFeatures11' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened11)) StartDate, MAX(convert(date,dateTimeBarOpened11)) EndDate FROM [daytradinglogic].[dbo].barFeatures11  WHERE loadGroupId11 = 1004 UNION ALL
SELECT 'barFeatures1v02' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened1v02)) StartDate, MAX(convert(date,dateTimeBarOpened1v02)) EndDate FROM [daytradinglogic].[dbo].barFeatures1v02 WHERE loadGroupId1v02 = @groupId  UNION ALL
SELECT 'barFeatures2v02' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened2v02)) StartDate, MAX(convert(date,dateTimeBarOpened2v02)) EndDate FROM [daytradinglogic].[dbo].barFeatures2v02 WHERE loadGroupId2v02 = @groupId  UNION ALL
SELECT 'barFeatures3v02' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened3v02)) StartDate, MAX(convert(date,dateTimeBarOpened3v02)) EndDate FROM [daytradinglogic].[dbo].barFeatures3v02  WHERE loadGroupId3v02 = @groupId UNION ALL
SELECT 'barFeatures4v02' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened4v02)) StartDate, MAX(convert(date,dateTimeBarOpened4v02)) EndDate FROM [daytradinglogic].[dbo].barFeatures4v02 WHERE loadGroupId4v02 = @groupId  UNION ALL
SELECT 'barFeatures5v02' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened5v02)) StartDate, MAX(convert(date,dateTimeBarOpened5v02)) EndDate FROM [daytradinglogic].[dbo].barFeatures5v02  WHERE loadGroupId5v02 = @groupId UNION ALL
SELECT 'barFeatures6v02' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened6v02)) StartDate, MAX(convert(date,dateTimeBarOpened6v02)) EndDate FROM [daytradinglogic].[dbo].barFeatures6v02  WHERE loadGroupId6v02 = @groupId UNION ALL
SELECT 'barFeatures7v02' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened7v02)) StartDate, MAX(convert(date,dateTimeBarOpened7v02)) EndDate FROM [daytradinglogic].[dbo].barFeatures7v02 WHERE loadGroupId7v02 = @groupId  UNION ALL
SELECT 'barFeatures8v02' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened8v02)) StartDate, MAX(convert(date,dateTimeBarOpened8v02)) EndDate FROM [daytradinglogic].[dbo].barFeatures8v02 WHERE loadGroupId8v02 = @groupId  UNION ALL
SELECT 'barFeatures9v02' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened9v02)) StartDate, MAX(convert(date,dateTimeBarOpened9v02)) EndDate FROM [daytradinglogic].[dbo].barFeatures9v02 WHERE loadGroupId9v02 = @groupId  UNION ALL
SELECT 'barFeatures10v02' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened10v02)) StartDate, MAX(convert(date,dateTimeBarOpened10v02)) EndDate FROM [daytradinglogic].[dbo].barFeatures10v02  WHERE loadGroupId10v02 = @groupId UNION ALL
SELECT 'barFeatures11v02' Tbl,  format(COUNT(*),'N0') RecordCount, Min(convert(date,dateTimeBarOpened11v02)) StartDate, MAX(convert(date,dateTimeBarOpened11v02)) EndDate FROM [daytradinglogic].[dbo].barFeatures11v02  WHERE loadGroupId11v02 = 1004 


SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures0 GROUP BY [recCreateDateTime] UNION ALL
SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures1 GROUP BY [recCreateDateTime] UNION ALL
SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures10 GROUP BY [recCreateDateTime] UNION ALL
SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures10v02 GROUP BY [recCreateDateTime] UNION ALL
SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures11 GROUP BY [recCreateDateTime] UNION ALL
SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures11v02 GROUP BY [recCreateDateTime] UNION ALL
SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures1v02 GROUP BY [recCreateDateTime] UNION ALL
SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures2 GROUP BY [recCreateDateTime] UNION ALL
SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures2v02 GROUP BY [recCreateDateTime] UNION ALL
SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures3 GROUP BY [recCreateDateTime] UNION ALL
SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures3v02 GROUP BY [recCreateDateTime] UNION ALL
SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures4 GROUP BY [recCreateDateTime] UNION ALL
SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures4v02 GROUP BY [recCreateDateTime] UNION ALL
SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures5 GROUP BY [recCreateDateTime] UNION ALL
SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures5v02 GROUP BY [recCreateDateTime] UNION ALL
SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures6 GROUP BY [recCreateDateTime] UNION ALL
SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures6v02 GROUP BY [recCreateDateTime] UNION ALL
SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures7 GROUP BY [recCreateDateTime] UNION ALL
SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures7v02 GROUP BY [recCreateDateTime] UNION ALL
SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures8 GROUP BY [recCreateDateTime] UNION ALL
SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures8v02 GROUP BY [recCreateDateTime] UNION ALL
SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures9 GROUP BY [recCreateDateTime] UNION ALL
SELECT [recCreateDateTime], min(recId), max(recId) FROM barFeatures9v02 GROUP BY [recCreateDateTime] 


--Returns the logarithm of a number using a specified base.
LogXY(baseX,num) 
--Returns the absolute value of num.
AbsValue(num) 
--Returns the location of String2 within String1.
InStr(String1, String2) 
--Returns a string expression containing a specified number of characters from the middle of String.
MidStr(String, Pos, Size) 
--Returns a string expression containing character from the left side of String.
LeftStr(String, sSize)
--Returns a string expression containing a specified number of characters from the right side of String.
RightStr(String, sSize) ; 
--Counts and returns number of characters in the specified string expression.
StrLen("Str")
--Returns the highest integer less than the specified (Num). Floor(4.5) returns a value of 4. Floor(-1.72) returns a value of -2.
Floor(Num) 
--Returns the lowest integer greater than the specified Num.
Ceiling(Num)
--Returns the number raised to the specified power.
Power(Num, Pow)
 
--get value of just decimal
select num
	,len(ABS(num) - FLOOR(ABS(num)))-2 as [decimal places] 
	,ABS(num) - FLOOR(ABS(num)) as [decimal value]
	,num-ABS(num) - FLOOR(ABS(num)) [int value]
	,CEILING(num * power(10.0000,(scale)))/power(10.0000,(scale))
	,FLOOR(num * power(10.0000,(scale)))/power(10.0000,(scale))
	--,power(10.000,(-scale))
	--, CASE WHEN num + 0.0005 >= 0.001 THEN 1 END
from (
    select 910080.7119 as num, 3.000 as scale
) a

--The process is to figure out significant places, multiply by a number that will move all those 
--places before the decimal, make it an integer and divide is back, e.g. ( sorta pseudo code )
n = 3.1415926
places = 3

m = 10^places // 1000
n = n * m // 3141.5926
n = (int)n  // 3141
n = n/m  // 3.141

;with s1 as(select n
				,places
				,m = n*power(10.0000,(places-0))
				,n_int = n-(ABS(n) - FLOOR(ABS(n)))
	from (
		select 91.2616726 as n, 2.000 as places
	) a)
select *
	--,CASE WHEN m+0.5 >= m+1 THEN m/power(10.0000,(places)) END
	,(m-(ABS(m) - CEILING(ABS(m)))) / power(10.0000,(places)) as CEILING
	,(m-(ABS(m) - FLOOR(ABS(m)))) / power(10.0000,(places)) as FLOOR
	,m+5
	,m+10
from s1


--Rounding up, as noted, is just a 0.5 away.
n = n * m // 3141.5926
n = n + 0.5 // 3142.0926


SELECT 0.7189-0.72

--get number of decimal places
SELECT num, LEN(SUBSTRING(num, dot_pos,LEN(num))) as [decimal places]
FROM
(
 SELECT '-910080.0007189' as num
      , CHARINDEX('.','-910080.0007189')+1 as dot_pos 
) a


--https://www.codeproject.com/Articles/58289/C-Round-Function
int round(double number)
{
    return (number >= 0) ? (int)(number + 0.5) : (int)(number - 0.5);
}


SELECT format(power(10.,2),'N0')

--DELETE --SELECT * FROM 
[dbo].barFeatures0 WHERE loadGroupId0 = 1003
SELECT * FROM 
[dbo].barFeatures1 WHERE loadGroupId1 = 1003

;with a as (
SELECT ELName = ',['+c.COLUMN_NAME+']]',' as '[as],rColName = replace(replace(replace(replace(replace(c.COLUMN_NAME,']',''),'[',''),'RightStrength',''),'+','p'),'-','m')
	,',format(round((sum(['+c.COLUMN_NAME+']])-avg(['+c.COLUMN_NAME+']]))/nullif(stdev(['+c.COLUMN_NAME+']]),0),2),''N0'') as ZScore_'+replace(replace(replace(replace(replace(c.COLUMN_NAME,']',''),'[',''),'RightStrength',''),'+','p'),'-','m')+'' ZScore
	,* 
FROM daytradinglogic.INFORMATION_SCHEMA.COLUMNS c
WHERE c.TABLE_NAME like 'barFeatures%'
	AND c.TABLE_NAME NOT IN ('barFeaturesMetaData','barFeatures')
	AND c.COLUMN_NAME NOT IN (
	'recId'	,'loadId', 'loadGroupId' ,'RightStrength'
	,'recCreateDateTime' ,'recModiedDateTime' ,'seriesBarNumber-1' ,'seriesBarNumber-RightStrength'
	,'lengthOfSeries' ,'dateBarOpened' ,'dayOfWeekBarOpened' ,'dateTimeBarOpened'
	,'timeBarOpenedhhmmss' ,'barOpenPrice' ,'barHighPrice' ,'barLowPrice'
	,'barClosePrice' ,'barClass' ,'swingRecordType' ,'barRecordType','numMinutesBarWasOpn '
	,'swingSize','[swingHi[1,0]//barNum]' ,'[swingLo[1,0]//barNum]'
	,'[swingHi[1,1]//price]' ,'[swingLo[1,1]//price]' ,'[swingHi[1,2]//date]' ,'[swingLo[1,2]//date]'
	,'[swingHi[1,3]//time]' ,'[swingLo[1,3]//time]' ,'sHiBarNumForCalc' ,'sLoBarNumForCalc'
	,'sHiPriceForCalc' ,'sLoPriceForCalc' ,'SwingHiAtBarNum' ,'SwingLoAtBarNum'
	,'swingHiTrend' ,'swingLoTrend' ,'altSwingUpTrend' ,'altSwingDnTrend'
	,'valueBucketBAHiClass' ,'valueBucketBALoClass' ,'valueBucketBASHiB[0]' ,'valueBucketBASLoB[0]','BarsAgoSHiBar','BarsAgoSLoBar','BarsAgoClassHi','BarsAgoClassLo'
	) AND c.COLUMN_NAME NOT LIKE 'loadId%' AND c.COLUMN_NAME NOT LIKE 'loadGroupId%'	AND c.COLUMN_NAME NOT LIKE 'dateTimeBarOpened%' AND c.COLUMN_NAME NOT LIKE 'barOpenPrice%'
	AND c.COLUMN_NAME NOT LIKE 'barHighPrice%' AND c.COLUMN_NAME NOT LIKE 'barLowPrice%'	AND c.COLUMN_NAME NOT LIKE 'barClosePrice%'
	AND c.COLUMN_NAME NOT LIKE '%monotonic%' AND c.COLUMN_NAME NOT LIKE 'barRange%Bars' AND c.COLUMN_NAME NOT LIKE 'AVGvsMed%Bars' 	AND c.COLUMN_NAME NOT LIKE '%day%' 
--	AND c.COLUMN_NAME NOT LIKE '%+4]' AND c.COLUMN_NAME NOT LIKE '%5]' 
	AND c.COLUMN_NAME NOT LIKE '%7]' AND c.COLUMN_NAME NOT LIKE '%7]' AND c.COLUMN_NAME NOT LIKE '%8]' AND c.COLUMN_NAME NOT LIKE '%9]'	AND c.COLUMN_NAME NOT LIKE '%10]' 
	AND c.COLUMN_NAME NOT LIKE '%11]' AND c.COLUMN_NAME NOT LIKE '%12]' AND c.COLUMN_NAME NOT LIKE '%13]' AND c.COLUMN_NAME NOT LIKE '%14]' AND c.COLUMN_NAME NOT LIKE '%15]'
	AND c.COLUMN_NAME NOT LIKE '%16]' AND c.COLUMN_NAME NOT LIKE '%17]' AND c.COLUMN_NAME NOT LIKE '%18]' AND c.COLUMN_NAME NOT LIKE '%19]' AND c.COLUMN_NAME NOT LIKE '%20]' 
	AND c.COLUMN_NAME NOT LIKE '%21]' AND c.COLUMN_NAME NOT LIKE '%22]' AND c.COLUMN_NAME NOT LIKE '%23]' AND c.COLUMN_NAME NOT LIKE '%24]'AND c.COLUMN_NAME NOT LIKE '%25]' 
	AND c.COLUMN_NAME NOT LIKE '%26]' AND c.COLUMN_NAME NOT LIKE '%27]'	AND c.COLUMN_NAME NOT LIKE '%28]' 
	)
SELECT * FROM a ORDER BY COLUMN_NAME ASC;

exec [SearchAllTables2] 'barFeatures1','= "0"' -- double quotes are automatically escaped to single quotes...

SELECT * FROM daytradinglogic.INFORMATION_SCHEMA.COLUMNS c
WHERE c.TABLE_NAME like 'barFeatures%' AND c.COLUMN_NAME LIKE 'loadId%' AND c.TABLE_NAME NOT IN ('barFeaturesMetaData','barFeatures')

SELECT * FROM daytradinglogic.INFORMATION_SCHEMA.COLUMNS c
WHERE c.TABLE_NAME like 'barFeatures%' AND c.COLUMN_NAME LIKE 'loadGroupId%' AND c.TABLE_NAME NOT IN ('barFeaturesMetaData','barFeatures')

SELECT * FROM daytradinglogic.INFORMATION_SCHEMA.COLUMNS c
WHERE c.TABLE_NAME like 'barFeatures%' AND c.COLUMN_NAME LIKE 'dateTimeBarOpened%' AND c.TABLE_NAME NOT IN ('barFeaturesMetaData','barFeatures')

SELECT * FROM daytradinglogic.INFORMATION_SCHEMA.COLUMNS c
WHERE c.TABLE_NAME like 'barFeatures%' AND c.COLUMN_NAME LIKE 'barOpenPrice%' AND c.TABLE_NAME NOT IN ('barFeaturesMetaData','barFeatures')

SELECT * FROM daytradinglogic.INFORMATION_SCHEMA.COLUMNS c
WHERE c.TABLE_NAME like 'barFeatures%' AND c.COLUMN_NAME LIKE 'barHighPrice%' AND c.TABLE_NAME NOT IN ('barFeaturesMetaData','barFeatures')

SELECT * FROM daytradinglogic.INFORMATION_SCHEMA.COLUMNS c
WHERE c.TABLE_NAME like 'barFeatures%' AND c.COLUMN_NAME LIKE 'barLowPrice%' AND c.TABLE_NAME NOT IN ('barFeaturesMetaData','barFeatures')

SELECT * FROM daytradinglogic.INFORMATION_SCHEMA.COLUMNS c
WHERE c.TABLE_NAME like 'barFeatures%' AND c.COLUMN_NAME LIKE 'barClosePrice%' AND c.TABLE_NAME NOT IN ('barFeaturesMetaData','barFeatures')

SELECT * FROM [daytradinglogic].[dbo].barFeatures0
SELECT * FROM [daytradinglogic].[dbo].barFeatures1
SELECT * FROM [daytradinglogic].[dbo].barFeatures2
SELECT * FROM [daytradinglogic].[dbo].barFeatures3
SELECT * FROM [daytradinglogic].[dbo].barFeatures4
SELECT * FROM [daytradinglogic].[dbo].barFeatures5
SELECT * FROM [daytradinglogic].[dbo].barFeatures6
SELECT * FROM [daytradinglogic].[dbo].barFeatures7
SELECT * FROM [daytradinglogic].[dbo].barFeatures8
SELECT * FROM [daytradinglogic].[dbo].barFeatures9
SELECT * FROM [daytradinglogic].[dbo].barFeatures10
SELECT * FROM [daytradinglogic].[dbo].barFeatures11


