
SELECT 'DROP TABLE '+c.TABLE_NAME
,*
FROM daytradinglogic.INFORMATION_SCHEMA.TABLES c
ORDER BY c.TABLE_NAME ASC

SELECT c.TABLE_NAME, format(COUNT(*),'N0')
FROM daytradinglogic.INFORMATION_SCHEMA.COLUMNS c
WHERE c.TABLE_NAME like 'barFeatures%'
GROUP BY c.TABLE_NAME
ORDER BY c.TABLE_NAME ASC

;with tblSchma as (SELECT CASE @@SERVERNAME WHEN 'W08-VM-SQL-S-2' then 'DWMSSQLStaging1' WHEN 'W08-VM-SQL-3' THEN 'DWMSSQLPROD1' ELSE @@SERVERNAME END ServerName,TABLE_CATALOG DatabaseName,table_schema TableSchema,TABLE_NAME TableName,COLUMN_NAME ColumnName,ORDINAL_POSITION OrdinalPosition
	,CASE
		WHEN isnull(datetime_PRECISION,0) <> 0 
			then data_type+'('+cast(datetime_PRECISION as varchar)+')'
		WHEN isnull(NUMERIC_PRECISION,0) <> 0 and isnull(NUMERIC_SCALE,0) <> 0
			then data_type+'('+cast(NUMERIC_PRECISION as varchar)+','+cast(NUMERIC_SCALE as varchar)+')'
		WHEN isnull(CHARACTER_MAXIMUM_LENGTH,0) <> 0 
			then data_type+'('+cast(CHARACTER_MAXIMUM_LENGTH as varchar)+')'
		else data_type
	end as DataType
	,IS_NULLABLE Nullable
FROM information_schema.columns 
WHERE table_name LIKE 'bar%' AND table_name NOT IN ('barFeatures','barFeaturesMetaData')
)
SELECT GETDATE() [TimeStamp],* 
INTO #tblSchma 
FROM tblSchma 

SELECT TableName, format(COUNT(*),'N0') recordCount
FROM #tblSchma
GROUP BY TableName
ORDER BY TableName ASC

SELECT * FROM #tblSchma
WHERE OrdinalPosition = 21
ORDER BY TableName ASC

SELECT tableName, '+","+Numtostr('+ColumnName+',9)'
FROM #tblSchma
WHERE OrdinalPosition = 22
ORDER BY TableName ASC

SELECT TableName, DataType,format(COUNT(*),'N0') recordCount
FROM #tblSchma
WHERE OrdinalPosition > 21
GROUP BY TableName,DataType
ORDER BY TableName ,DataType ASC

DELETE a 
FROM #tblSchma a
WHERE a.TableName = 'barFeaturesXML0'

select 
+'	//(4) insert feature data into dbo.barFeatures1	'+char(13)
+'	if ReadWriteToDB and TblNum = 1  and StrToNum(queryResult) > 0 and CurrentBar > MaxBarsBack +1 then begin	'+char(13)		
+'		if  srun1 = 0 AND condition1 AND MaxBarsBack > 1 AND CurrentBar > (MaxBarsBack*1.5) then begin'+char(13)
+'			sql_insert_features1 = "INSERT INTO  [dbo].[barFeatures1] VALUES ("'+char(13)
+'			+queryResult//MAX(loadId) '+char(13)
+'			+","+Numtostr(loadGroupId,0) '+char(13)
+'			+","+Numtostr(RightStrength,0)//RightStrength'+char(13)
+'			+","+"''"+queryResultdatetime+"'' "//[recCreateDateTime]'+char(13)
+'			+","+"''"+queryResultdatetime+"'' "//[recModiedDateTime]'+char(13)
+'			+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"''"+dateBarOpened+"'' "+","+"''"+dayOfWeekBarOpened+"'' "+","+"''"+dateTimeBarOpened+"'' "+","+"''"+timeBarOpenedhhmmss+"'' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)'+char(13)
 as startingStatement
,

+'			+" )"; '+char(13)
+'			res1 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_features1);'+char(13)
+'			srun1 = 1;'+char(13)
+'			if debug then begin	'+char(13)
+'				Print(File("C:\Users\Neal\Documents\www.DAYTRADINGLOGIC.com\_neal\swing\csv\barFeatures1.csv")'+char(13)
+'					,",",NumToStr(swingRecordType,0)'+char(13)
+'					,",",NumToStr(barRecordType,0)'+char(13)
+'					,",",swingSize'+char(13)
+'					,"Currentbar = ",Currentbar'+char(13)
+'					,"***,",sql_insert_features1'+char(13)
+'					+ NewLine);'+char(13)
+'			end;'+char(13)
+'		End;'+char(13)
+'	End;'+char(13)
+'	 TblNum = TblNum+1;'+char(13)
 as endingStatement
 INTO #el


 DECLARE @TblNum nvarchar(5)
 SET @TblNum = 11
SELECT REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(startingStatement,
	'into dbo.barFeatures1'		,'into dbo.barFeatures'+@TblNum+'v02'),
	'TblNum = 1'				,'TblNum = 11+'+@TblNum),
	'srun1 = 0'					,'srun'+@TblNum+'v02 = 0'),
	'sql_insert_features1 ='	,'sql_insert_feat'+@TblNum+'v02 ='),
	'[barFeatures1]'			,'[barFeatures'+@TblNum+'v02]') startingStatement 
 INTO #startingStatement 
 FROM #el

SELECT REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(endingStatement,
	'res1 = execsql'					,'res'+@TblNum+'v02 = execsql'),
	'sql_dbname, sql_insert_features1'	,'sql_dbname, sql_insert_feat'+@TblNum+'v02'),
	'srun1 = 1;'						,'srun'+@TblNum+'v02 = 1;'),
	'barFeatures1.csv'					,'barFeatures'+@TblNum+'v02.csv'),
	',"***,",sql_insert_features1'		,',"***,",sql_insert_feat'+@TblNum+'v02') endingStatement
INTO #endingStatement
FROM #el

DECLARE @startingStatement nvarchar(max)
SET @startingStatement = (SELECT * FROM #startingStatement)
SET @startingStatement = @startingStatement+''+char(13)+(SELECT * FROM #endingStatement)
PRINT @startingStatement

SELECT tableName, '+","+Numtostr('+ColumnName+',9)'
FROM #tblSchma
WHERE OrdinalPosition >= 1 AND TableName = 'barFeatures11v02'--1
ORDER BY OrdinalPosition ASC


DROP TABLE #startingStatement
DROP TABLE #endingStatement

