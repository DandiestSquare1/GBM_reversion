--http://www.sql-ex.com/help/select20.php
USE daytradinglogic
GO 

DECLARE @TblName VARCHAR(100) = 'barFeatures3' --select * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = @TblName
DECLARE @DBName VARCHAR(100) = (SELECT DISTINCT TABLE_CATALOG FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = @TblName)
DECLARE @SchemaName VARCHAR(100) = (SELECT DISTINCT TABLE_SCHEMA FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = @TblName)
DECLARE @QuotedIdentifier char(1) = '''' -- single quote
DECLARE @ListDelimeter char(27) = 'AS VARCHAR(67))+'',''+CAST(' --use '+' to soncatenate all columns into a string

SELECT 'SELECT CAST(' + REPLACE((
			SELECT '['+COLUMN_NAME+']]' AS 'data()'
			FROM INFORMATION_SCHEMA.COLUMNS
			WHERE TABLE_NAME = @TblName
				AND COLUMN_NAME NOT IN ('code') 
				AND ORDINAL_POSITION > 21 AND  ORDINAL_POSITION < 61 
			ORDER BY ORDINAL_POSITION
			FOR XML PATH('')
			), ' ', @ListDelimeter) + 'AS VARCHAR(67)) FROM '+@DBName+'.'+@SchemaName+'.'+@TblName;


SELECT CAST([DMIStoindBIG[RightStrength+27]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBIG[RightStrength+28]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBIG[RightStrength+3]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBIG[RightStrength+4]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBIG[RightStrength+5]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBIG[RightStrength+6]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBIG[RightStrength+7]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBIG[RightStrength+8]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBIG[RightStrength+9]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBIG[RightStrength-1]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBIG[RightStrength-2]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBIG[RightStrength-3]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBIG[RightStrength-4]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+0]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+1]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+10]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+11]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+12]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+13]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+14]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+15]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+16]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+17]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+18]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+19]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+2]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+20]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+21]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+22]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+23]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+24]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+25]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+26]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+27]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+28]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+3]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+4]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+5]]]AS VARCHAR(67))+','+CAST(  [DMIStoindBX[RightStrength+6]]]AS VARCHAR(67)) as DataFrombarFeatures3
INTO #barFeatures3
FROM daytradinglogic.dbo.barFeatures3

SELECT * FROM #barFeatures3
SELECT * FROM tempdb.sys.columns c WHERE c.[object_id] = object_Id('tempdb.dbo.#barFeatures3')
SELECT * FROM tempdb.INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME LIKE '#barFeatures3%'

USE daytradinglogic
GO 

--http://stackoverflow.com/a/13761470
DECLARE @s VARCHAR(8000)

SELECT @s =  ISNULL(@s+'+'',''+ ','') + 'convert(varchar(67),['+c.name+'])'   
FROM  sys.all_columns c join sys.tables  t 
            ON  c.object_id = t.object_id
WHERE t.name = 'barFeatures3'

SELECT '<text>' + @s + '</text>'


--https://social.msdn.microsoft.com/Forums/sqlserver/en-US/61685438-7969-4a5e-9626-e8a4af54eb08/concatenate-all-the-column-values-in-one-row?forum=transactsql
USE [tempdb]
GO

SET NOCOUNT ON;
GO

--Step 1 - Use Select into to create a #temp table

IF OBJECT_ID('tempdb.dbo.#t') IS NOT NULL
BEGIN
	DROP TABLE #t;
END
GO

--Create the temp table via select into
SELECT TOP 1 * INTO #t
FROM [daytradinglogic].[dbo].barFeatures3
GO

SELECT * FROM  #t

--Now Begin Solution
DECLARE @ObjName VARCHAR(100),
		@Cols VARCHAR(8000),
		@sql VARCHAR(8000)

SET @ObjName = 'tempdb.dbo.#t'
SELECT @Cols = 
	STUFF(
	(
	SELECT '+''|''+CAST([' + c.name + ']] AS VARCHAR(67))' + CHAR(13)
	FROM tempdb.sys.columns c
	WHERE c.[object_id] = object_Id(@ObjName)
	FOR XML PATH(''),TYPE
	).value('.','VARCHAR(MAX)')
	,1,4,'')
	
SET @sql = N'SELECT ' + @Cols + ' FROM ' + @ObjName
PRINT @sql
SELECT @sql
EXEC sp_executesql @sql


