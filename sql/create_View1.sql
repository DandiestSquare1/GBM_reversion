USE daytradinglogic
GO

DROP TABLE #tblSchma
--get indicator columns
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
WHERE table_name LIKE 'barFeatures%' AND table_name NOT IN ('barFeatures','barFeaturesMetaData','barFeaturesXML0')
)
SELECT 
	GETDATE() [TimeStamp]
	,ROW_NUMBER()
			OVER (
				PARTITION BY DatabaseName
				ORDER BY TableName,OrdinalPosition ASC
				) RowNum,* 
INTO #tblSchma 
FROM tblSchma 


SELECT TableName, max(OrdinalPosition) maxOrdinalPosition, min(RowNum) minRowNum, max(RowNum) maxRowNum
FROM #tblSchma
GROUP BY TableName
ORDER BY TableName ASC


SELECT TableName, DataType,format(COUNT(*),'N0') recordCount
FROM #tblSchma
WHERE OrdinalPosition > 21
GROUP BY TableName,DataType
ORDER BY TableName,DataType ASC

SELECT 'SELECT [recCreateDateTime], min(recId), max(recId) FROM '+TableName+' GROUP BY [recCreateDateTime] UNION ALL'
FROM #tblSchma
WHERE columnname = 'recCreateDateTime'
ORDER BY TableName ASC


--get transformed column names for view
--extract_file_1.sql
WITH extract_file_1 as (SELECT '' TableName,'' ColumnName, 0 OrdinalPosition,'SELECT zero.[barClass]' [column names for view] UNION ALL
SELECT TableName, ColumnName,OrdinalPosition, ','+right(tablename,6)+'.['+columnName+']] as '+replace(replace(replace(replace(columnName,'[RightStrength',''),']',''),'+','p'),'-','m')+'' [column names for view]
FROM #tblSchma
WHERE OrdinalPosition > 21 
	AND RowNum BETWEEN 22 AND (4096*1) AND TableName NOT IN ('barFeatures0') AND ColumnName NOT LIKE '%bars%')
SELECT * FROM extract_file_1 ORDER BY TableName, OrdinalPosition ASC

--extract_file_2.sql
WITH extract_file_2 as (SELECT '' TableName,'' ColumnName, 0 OrdinalPosition,'SELECT zero.[barClass]' [column names for view] UNION ALL
SELECT TableName, ColumnName,OrdinalPosition, ','+right(tablename,6)+'.['+columnName+']] as '+replace(replace(replace(replace(columnName,'[RightStrength',''),']',''),'+','p'),'-','m')+'' [column names for view]
FROM #tblSchma
WHERE OrdinalPosition > 21 
	AND RowNum BETWEEN (4096*1)+1 AND (4096*2) AND TableName NOT IN ('barFeatures0'))
SELECT * FROM extract_file_2 ORDER BY TableName, OrdinalPosition ASC

--extract_file_3.sql
WITH extract_file_3 as (SELECT '' TableName,'' ColumnName, 0 OrdinalPosition,'SELECT zero.[barClass]' [column names for view] UNION ALL
SELECT TableName, ColumnName,OrdinalPosition, ','+right(tablename,6)+'.['+columnName+']] as '+replace(replace(replace(replace(columnName,'[RightStrength',''),']',''),'+','p'),'-','m')+'' [column names for view]
FROM #tblSchma
WHERE OrdinalPosition > 21 
	AND RowNum BETWEEN (4096*2)+1 AND (4096*3) AND TableName NOT IN ('barFeatures0') )
SELECT * FROM extract_file_3 ORDER BY TableName, OrdinalPosition ASC

--get join logic
SELECT *, CASE 
				WHEN tablename = 'barFeatures0' THEN 'FROM DayTradingLogic.dbo.barFeatures0 zero'
				WHEN columnName LIKE 'dateTimeBarOpened%' AND tablename <> 'barFeatures0' THEN 'JOIN DayTradingLogic.dbo.'+tablename+' '+right(tablename,6)+' ON '+right(tablename,6)+'.'+columnName+' = zero.dateTimeBarOpened0 AND '+right(tablename,6)+'.recId = zero.recId'
				WHEN columnName LIKE 'barOpenPrice%' AND tablename <> 'barFeatures0' THEN '    AND '+right(tablename,6)+'.'+columnName+' = zero.barOpenPrice0'
				WHEN columnName LIKE 'barHighPrice%' AND tablename <> 'barFeatures0' THEN '    AND '+right(tablename,6)+'.'+columnName+' = zero.barHighPrice0'
				WHEN columnName LIKE 'barLowPrice%' AND tablename <> 'barFeatures0' THEN '    AND '+right(tablename,6)+'.'+columnName+' = zero.barLowPrice0'
				WHEN columnName LIKE 'barClosePrice%' AND tablename <> 'barFeatures0' THEN '    AND '+right(tablename,6)+'.'+columnName+' = zero.barClosePrice0'
			end joinStatements
FROM #tblSchma
WHERE OrdinalPosition IN (12,14,15,16,17)
 AND tablename LIKE 'bar%' AND tablename NOT IN ('barFeatures','barFeaturesMetaData')
ORDER BY tablename ASC

--get column names in comma delimited list --extract_file_1.sql
SELECT STUFF((SELECT ',' 
	+ replace(replace(replace(replace(columnName,'[RightStrength',''),']',''),'+','p'),'-','m') ColumnName
	FROM #tblSchma
	WHERE OrdinalPosition > 21 
		AND RowNum BETWEEN 22 AND (4096*1) AND TableName NOT IN ('barFeatures0') AND ColumnName NOT LIKE '%bars%'
	ORDER BY TableName, OrdinalPosition ASC
        FOR XML PATH(''), TYPE).value('.', 'VARCHAR(MAX)'), 1, 1, '') AS listStr
--get column names in comma delimited list --extract_file_2.sql
SELECT STUFF((SELECT ',' 
	+ replace(replace(replace(replace(columnName,'[RightStrength',''),']',''),'+','p'),'-','m') ColumnName
	FROM #tblSchma
	WHERE OrdinalPosition > 21 
		AND RowNum BETWEEN (4096*1)+1 AND (4096*2) AND TableName NOT IN ('barFeatures0')
	ORDER BY TableName, OrdinalPosition ASC
        FOR XML PATH(''), TYPE).value('.', 'VARCHAR(MAX)'), 1, 1, '') AS listStr
--get column names in comma delimited list --extract_file_3.sql
SELECT STUFF((SELECT ',' 
	+ replace(replace(replace(replace(columnName,'[RightStrength',''),']',''),'+','p'),'-','m') ColumnName
	FROM #tblSchma
	WHERE OrdinalPosition > 21 
		AND RowNum BETWEEN (4096*2)+1 AND (4096*3) AND TableName NOT IN ('barFeatures0')
	ORDER BY TableName, OrdinalPosition ASC
        FOR XML PATH(''), TYPE).value('.', 'VARCHAR(MAX)'), 1, 1, '') AS listStr


--export data with column names to txt files
--https://sqlandme.com/tag/execute-bcp-using-tsql/
EXEC master.dbo.sp_configure 'show advanced options', 1
RECONFIGURE
EXEC master.dbo.sp_configure 'xp_cmdshell', 1
RECONFIGURE 

DECLARE @FilePath NVARCHAR(1255),@FileExtension NVARCHAR(1255),@FileDateTime NVARCHAR(1255),@HeaderFileName NVARCHAR(1255),@DataFileName NVARCHAR(1255),@GetHeaderSQLCommand NVARCHAR(1255)
		,@GetHeaderBCPCommand NVARCHAR(1255),@GetDataSQLCommand NVARCHAR(1255),@GetDataBCPCommand NVARCHAR(1255),@MergeFilesBCPCommand NVARCHAR(1255)
SET @FilePath = 'C:\sql\'
SET @FileExtension = '.txt'
SET @FileDateTime = REPLACE(REPLACE(CONVERT(VARCHAR,GETDATE(),120),' ','_'),':','')
--save header file
SELECT 'save header file'
SET @HeaderFileName = 'sp_extract_columns_1'
SET @HeaderFileName = @HeaderFileName +'_'+ @FileDateTime + @FileExtension
SET @GetHeaderSQLCommand = 'EXEC DayTradingLogic.dbo.sp_extract_file_columns_1'
SET @GetHeaderBCPCommand = 'bcp "'+@GetHeaderSQLCommand+' " queryout'
SET @GetHeaderBCPCommand = @GetHeaderBCPCommand +' '+ @FilePath + @HeaderFileName + ' -c -t, -T -S '+ @@servername
print @GetHeaderBCPCommand
exec master..xp_cmdshell @GetHeaderBCPCommand
--save data file
SELECT '--save data file'
SET @DataFileName = 'sp_extract_data_1'
SET @DataFileName = @DataFileName +'_'+ @FileDateTime + @FileExtension
SET @GetDataSQLCommand = 'EXEC DayTradingLogic.dbo.sp_extract_file_1'
SET @GetDataBCPCommand = 'bcp "'+@GetDataSQLCommand+' " queryout'
SET @GetDataBCPCommand = @GetDataBCPCommand +' '+ @FilePath + @DataFileName + ' -c -t, -T -S '+ @@servername
print @GetDataBCPCommand
exec master..xp_cmdshell @GetDataBCPCommand
--merge header to data file
SELECT '--merge header to data file'
SET @MergeFilesBCPCommand = 'copy ' + @FilePath + @HeaderFileName +' + '+ @FilePath + @DataFileName +' '+ @FilePath + 'data_extract_1' +'_'+ @FileDateTime + @FileExtension
print @MergeFilesBCPCommand
exec master..xp_cmdshell @MergeFilesBCPCommand


exec dbo.sp_file_extract '1'

DECLARE @FilePath NVARCHAR(1255),@FileExtension NVARCHAR(1255),@FileDateTime NVARCHAR(1255),@HeaderFileName NVARCHAR(1255),@DataFileName NVARCHAR(1255),@GetHeaderSQLCommand NVARCHAR(1255)
		,@GetHeaderBCPCommand NVARCHAR(1255),@GetDataSQLCommand NVARCHAR(1255),@GetDataBCPCommand NVARCHAR(1255),@MergeFilesBCPCommand NVARCHAR(1255)
SET @FilePath = 'C:\sql\'
SET @FileExtension = '.txt'
SET @FileDateTime = REPLACE(REPLACE(CONVERT(VARCHAR,GETDATE(),120),' ','_'),':','')
--save header file
SELECT 'save header file'
SET @HeaderFileName = 'sp_extract_columns_2'
SET @HeaderFileName = @HeaderFileName +'_'+ @FileDateTime + @FileExtension
SET @GetHeaderSQLCommand = 'EXEC DayTradingLogic.dbo.sp_extract_file_columns_2'
SET @GetHeaderBCPCommand = 'bcp "'+@GetHeaderSQLCommand+' " queryout'
SET @GetHeaderBCPCommand = @GetHeaderBCPCommand +' '+ @FilePath + @HeaderFileName + ' -c -t, -T -S '+ @@servername
print @GetHeaderBCPCommand
exec master..xp_cmdshell @GetHeaderBCPCommand
--save data file
SELECT '--save data file'
SET @DataFileName = 'sp_extract_data_2'
SET @DataFileName = @DataFileName +'_'+ @FileDateTime + @FileExtension
SET @GetDataSQLCommand = 'EXEC DayTradingLogic.dbo.sp_extract_file_2'
SET @GetDataBCPCommand = 'bcp "'+@GetDataSQLCommand+' " queryout'
SET @GetDataBCPCommand = @GetDataBCPCommand +' '+ @FilePath + @DataFileName + ' -c -t, -T -S '+ @@servername
print @GetDataBCPCommand
exec master..xp_cmdshell @GetDataBCPCommand
--merge header to data file
SELECT '--merge header to data file'
SET @MergeFilesBCPCommand = 'copy ' + @FilePath + @HeaderFileName +' + '+ @FilePath + @DataFileName +' '+ @FilePath + 'data_extract_2' +'_'+ @FileDateTime + @FileExtension
print @MergeFilesBCPCommand
exec master..xp_cmdshell @MergeFilesBCPCommand



DECLARE @FilePath NVARCHAR(1255),@FileExtension NVARCHAR(1255),@FileDateTime NVARCHAR(1255),@HeaderFileName NVARCHAR(1255),@DataFileName NVARCHAR(1255),@GetHeaderSQLCommand NVARCHAR(1255)
		,@GetHeaderBCPCommand NVARCHAR(1255),@GetDataSQLCommand NVARCHAR(1255),@GetDataBCPCommand NVARCHAR(1255),@MergeFilesBCPCommand NVARCHAR(1255)
SET @FilePath = 'C:\sql\'
SET @FileExtension = '.txt'
SET @FileDateTime = REPLACE(REPLACE(CONVERT(VARCHAR,GETDATE(),120),' ','_'),':','')
--save header file
SELECT 'save header file'
SET @HeaderFileName = 'sp_extract_columns_3'
SET @HeaderFileName = @HeaderFileName +'_'+ @FileDateTime + @FileExtension
SET @GetHeaderSQLCommand = 'EXEC DayTradingLogic.dbo.sp_extract_file_columns_3'
SET @GetHeaderBCPCommand = 'bcp "'+@GetHeaderSQLCommand+' " queryout'
SET @GetHeaderBCPCommand = @GetHeaderBCPCommand +' '+ @FilePath + @HeaderFileName + ' -c -t, -T -S '+ @@servername
print @GetHeaderBCPCommand
exec master..xp_cmdshell @GetHeaderBCPCommand
--save data file
SELECT '--save data file'
SET @DataFileName = 'sp_extract_data_3'
SET @DataFileName = @DataFileName +'_'+ @FileDateTime + @FileExtension
SET @GetDataSQLCommand = 'EXEC DayTradingLogic.dbo.sp_extract_file_3'
SET @GetDataBCPCommand = 'bcp "'+@GetDataSQLCommand+' " queryout'
SET @GetDataBCPCommand = @GetDataBCPCommand +' '+ @FilePath + @DataFileName + ' -c -t, -T -S '+ @@servername
print @GetDataBCPCommand
exec master..xp_cmdshell @GetDataBCPCommand
--merge header to data file
SELECT '--merge header to data file'
SET @MergeFilesBCPCommand = 'copy ' + @FilePath + @HeaderFileName +' + '+ @FilePath + @DataFileName +' '+ @FilePath + 'data_extract_3' +'_'+ @FileDateTime + @FileExtension
print @MergeFilesBCPCommand
exec master..xp_cmdshell @MergeFilesBCPCommand



SELECT ColumnName, COUNT(*)
FROM #tblSchma
WHERE OrdinalPosition > 21 
	AND RowNum BETWEEN 22 AND 3000 --AND TableName NOT IN ('barFeatures0','barFeatures11') --AND ColumnName NOT LIKE '%bars%'
GROUP BY ColumnName
HAVING COUNT(*) > 1
ORDER BY COUNT(*) DESC

SELECT TableName, COUNT(*)
FROM #tblSchma
WHERE OrdinalPosition > 21 
	AND RowNum BETWEEN 22 AND 3000 AND ColumnName IN (
		SELECT ColumnName	FROM #tblSchma
		WHERE OrdinalPosition > 21 AND RowNum BETWEEN 22 AND 3000 
		GROUP BY ColumnName
		HAVING COUNT(*) > 1
	)
GROUP BY TableName


--DROP TABLE #swing
WITH vars as 
	(
		SELECT swingSizeThreshold = 0.0001
			,loadIdFilter = 3
			,loadGroupId = 1003
			/*,ROW_NUMBER() 
				OVER (
					PARTITION BY f.dateTimeBarOpened,f.barOpenPrice	,f.barHighPrice	,f.barLowPrice	,f.barClosePrice --or partition by f.[seriesBarNumber-1] and ORDER by f.[seriesBarNumber-1] desc
					ORDER BY f.SwingSize DESC
					) rownumber*/
		--FROM [daytradinglogic].dbo.barFeatures0 
		)
SELECT 
	CASE 
		WHEN barFeatures0.swingRecordType = -1 and abs(barFeatures0.SwingSize) >= swingSizeThreshold then -2
		WHEN barFeatures0.swingRecordType = 1 and abs(barFeatures0.SwingSize) >= swingSizeThreshold  then 2
		WHEN barFeatures0.swingRecordType = -1 and abs(barFeatures0.SwingSize) < swingSizeThreshold  then 0
		WHEN barFeatures0.swingRecordType = 1 and abs(barFeatures0.SwingSize) < swingSizeThreshold  then 0
		ELSE 0
	END [ClassNum]
	,CASE 
		WHEN barFeatures0.swingRecordType = -1 and abs(barFeatures0.SwingSize) >= swingSizeThreshold then 'SwingLo' 
		WHEN barFeatures0.swingRecordType = 1 and abs(barFeatures0.SwingSize) >= swingSizeThreshold  then 'SwingHi'
		WHEN barFeatures0.swingRecordType = -1 and abs(barFeatures0.SwingSize) < swingSizeThreshold  then 'SwingNULL'  
		WHEN barFeatures0.swingRecordType = 1 and abs(barFeatures0.SwingSize) < swingSizeThreshold  then 'SwingNULL'  
		ELSE 'SwingNULL'
	END as [ClassChar]
/*INTO #swing
FROM [daytradinglogic].[dbo].barFeatures0
JOIN vars
	on loadId0 = loadIdFilter 
SELECT ClassChar, COUNT(*) RecordCount FROM #swing GROUP BY ClassChar
*/
	,CASE 
		WHEN barFeatures0.swingRecordType = -1 and abs(barFeatures0.SwingSize) >= swingSizeThreshold then barFeatures0.SwingSize
		WHEN barFeatures0.swingRecordType = 1 and abs(barFeatures0.SwingSize) >= swingSizeThreshold  then barFeatures0.SwingSize
		WHEN barFeatures0.swingRecordType = -1 and abs(barFeatures0.SwingSize) < swingSizeThreshold  then barFeatures0.SwingSize
		WHEN barFeatures0.swingRecordType = 1 and abs(barFeatures0.SwingSize) < swingSizeThreshold  then barFeatures0.SwingSize
		ELSE 0
	END [swingSize]
--	,loadId0 loadId
	,loadGroupId0 loadGroupId
/*	,dateTimeBarOpened0 dateTimeBarOpened
	,barOpenPrice0 barOpenPrice
	,barHighPrice0 barHighPrice
	,barLowPrice0 barLowPrice
	,barClosePrice0 barClosePrice
	,[numMinutesBarWasOpn ]
	,[dayOfWeekInd]  */
--	,[bucketHourOfDay]
/*	,[hourOfDay]
,[VIMAindSMALL[RightStrength+4]]]	 as 	VIMAindSMALLp4
,[VIMAindSMALL[RightStrength+5]]]	 as 	VIMAindSMALLp5
,[VIMAindSMALL[RightStrength+6]]]	 as 	VIMAindSMALLp6
,[VIMAindSMALL[RightStrength-1]]]	 as 	VIMAindSMALLm1
,[VIMAindSMALL[RightStrength-2]]]	 as 	VIMAindSMALLm2
,[VIMAindSMALL[RightStrength-3]]]	 as 	VIMAindSMALLm3
,[VIMAindSMALL[RightStrength-4]]]	 as 	VIMAindSMALLm4
*/






with a as(SELECT cast(CASE 
	WHEN [barClass] = -2 THEN '-2'
	WHEN [barClass] = 0 THEN '0'
	WHEN [barClass] = 2 THEN '2'
		ELSE [barClass] 
	END as nvarchar(10) ) as [barClass]
	,*
FROM [daytradinglogic].[dbo].[vw_barFeatures])
SELECT case [swingRecordTypeNchar]
	when '-2' then 'SwingLo' 
	when '0' then 'SwingNULL' 
	when '2' then 'SwingHi' end as [swingRecordType]
	,case [swingRecordTypeNchar]
	when '-2' then SwingSize 
	when '0' then 0 
	when '2' then SwingSize end as SwingSize







