USE daytradinglogic
GO

DROP TABLE #tblSchma

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
--WHERE DataType NOT LIKE '%var%' --AND NOT LIKE '%dat%'
WHERE OrdinalPosition >= 22
ORDER BY TableName, OrdinalPosition ASC

SELECT TableName,MAX(OrdinalPosition) FROM #tblSchma
GROUP BY TableName
ORDER BY TableName 

SELECT * FROM #tblSchma 

;with LastColumnName as(SELECT TableName,MAX(OrdinalPosition) MaxOrdinalPosition FROM #tblSchma GROUP BY TableName)
SELECT a.*, b.ColumnName 
FROM LastColumnName a
JOIN #tblSchma b
	ON a.TableName = b.TableName AND a.MaxOrdinalPosition = b.OrdinalPosition
ORDER BY a.TableName


SELECT TableName, DataType, COUNT(*) Count
FROM #tblSchma 
WHERE TableName <> 'barFeatures0'
GROUP BY TableName, DataType
ORDER BY TableName, DataType

DROP TABLE #TestTable

CREATE TABLE #TestTable (
	MyField [decimal](16, 6) NULL
	);

INSERT INTO  #TestTable VALUES (
	-941.17647
) 
GO



SELECT 'DROP TABLE '+c.TABLE_NAME
,*
FROM daytradinglogic.INFORMATION_SCHEMA.TABLES c
ORDER BY c.TABLE_NAME ASC

SELECT c.TABLE_NAME, format(COUNT(*),'N0')
FROM daytradinglogic.INFORMATION_SCHEMA.COLUMNS c
WHERE c.TABLE_NAME like 'barFeatures%'
GROUP BY c.TABLE_NAME
ORDER BY c.TABLE_NAME ASC


DROP TABLE #tblSchma

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
ORDER BY TableName ,DataType ASC



SELECT TableName, columnname,DataType,OrdinalPosition
	,'CREATE UNIQUE CLUSTERED INDEX [index_'+TableName+'_'+convert(nvarchar(3),floor(rand() * (200 + 1)))+'] ON [dbo].[' +TableName+'] '
		+' ('+columnname+',[barClass],recId ASC) '
		+'  WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]'
FROM #tblSchma
WHERE --OrdinalPosition = 21 and 
ColumnName LIKE '%dateTimeBarOpened%' and 
tablename LIKE 'bar%' AND tablename NOT IN ('barFeatures','barFeaturesMetaData')
ORDER BY tablename ASC

CREATE UNIQUE CLUSTERED INDEX [index_barFeatures9v02_156] ON [dbo].[barFeatures9v02] ( [dateTimeBarOpened9v02] ) WITH (FILLFACTOR=100, ONLINE=?, SORT_IN_TEMPDB=?);
CREATE CLUSTERED INDEX [ClusteredIndex-20170211-162401] ON [dbo].[barFeatures0] ( [dateTimeBarOpened0], [barClass], [loadGroupId0], [recCreateDateTime] ) WITH (FILLFACTOR=100, ONLINE=?, SORT_IN_TEMPDB=?);


CREATE UNIQUE CLUSTERED INDEX [index_barFeatures0_68] ON [dbo].[barFeatures0]  (dateTimeBarOpened0,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
CREATE UNIQUE CLUSTERED INDEX [index_barFeatures1_68] ON [dbo].[barFeatures1]  (dateTimeBarOpened1,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
CREATE UNIQUE CLUSTERED INDEX [index_barFeatures10_68] ON [dbo].[barFeatures10]  (dateTimeBarOpened10,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
CREATE UNIQUE CLUSTERED INDEX [index_barFeatures10v02_68] ON [dbo].[barFeatures10v02]  (dateTimeBarOpened10v02,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
CREATE UNIQUE CLUSTERED INDEX [index_barFeatures11_68] ON [dbo].[barFeatures11]  (dateTimeBarOpened11,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
CREATE UNIQUE CLUSTERED INDEX [index_barFeatures11v02_68] ON [dbo].[barFeatures11v02]  (dateTimeBarOpened11v02,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
CREATE UNIQUE CLUSTERED INDEX [index_barFeatures1v02_68] ON [dbo].[barFeatures1v02]  (dateTimeBarOpened1v02,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
CREATE UNIQUE CLUSTERED INDEX [index_barFeatures2_68] ON [dbo].[barFeatures2]  (dateTimeBarOpened2,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
CREATE UNIQUE CLUSTERED INDEX [index_barFeatures2v02_68] ON [dbo].[barFeatures2v02]  (dateTimeBarOpened2v02,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
CREATE UNIQUE CLUSTERED INDEX [index_barFeatures3_68] ON [dbo].[barFeatures3]  (dateTimeBarOpened3,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
CREATE UNIQUE CLUSTERED INDEX [index_barFeatures3v02_68] ON [dbo].[barFeatures3v02]  (dateTimeBarOpened3v02,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
CREATE UNIQUE CLUSTERED INDEX [index_barFeatures4_68] ON [dbo].[barFeatures4]  (dateTimeBarOpened4,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
CREATE UNIQUE CLUSTERED INDEX [index_barFeatures4v02_68] ON [dbo].[barFeatures4v02]  (dateTimeBarOpened4v02,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
CREATE UNIQUE CLUSTERED INDEX [index_barFeatures5_68] ON [dbo].[barFeatures5]  (dateTimeBarOpened5,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
CREATE UNIQUE CLUSTERED INDEX [index_barFeatures5v02_68] ON [dbo].[barFeatures5v02]  (dateTimeBarOpened5v02,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
CREATE UNIQUE CLUSTERED INDEX [index_barFeatures6_68] ON [dbo].[barFeatures6]  (dateTimeBarOpened6,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
CREATE UNIQUE CLUSTERED INDEX [index_barFeatures6v02_68] ON [dbo].[barFeatures6v02]  (dateTimeBarOpened6v02,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
CREATE UNIQUE CLUSTERED INDEX [index_barFeatures7_68] ON [dbo].[barFeatures7]  (dateTimeBarOpened7,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
CREATE UNIQUE CLUSTERED INDEX [index_barFeatures7v02_68] ON [dbo].[barFeatures7v02]  (dateTimeBarOpened7v02,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
CREATE UNIQUE CLUSTERED INDEX [index_barFeatures8_68] ON [dbo].[barFeatures8]  (dateTimeBarOpened8,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
CREATE UNIQUE CLUSTERED INDEX [index_barFeatures8v02_68] ON [dbo].[barFeatures8v02]  (dateTimeBarOpened8v02,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
CREATE UNIQUE CLUSTERED INDEX [index_barFeatures9_68] ON [dbo].[barFeatures9]  (dateTimeBarOpened9,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
CREATE UNIQUE CLUSTERED INDEX [index_barFeatures9v02_68] ON [dbo].[barFeatures9v02]  (dateTimeBarOpened9v02,[barClass],recId ASC)   WITH (SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]



SELECT *, CASE 
				WHEN columnName LIKE 'dateTimeBarOpened%' THEN 'JOIN '+tablename+' ON '+columnName+' = dateTimeBarOpened0'
				WHEN columnName LIKE 'barOpenPrice%' THEN '    AND '+columnName+' = barOpenPrice0'
				WHEN columnName LIKE 'barHighPrice%' THEN '    AND '+columnName+' = barHighPrice0'
				WHEN columnName LIKE 'barLowPrice%' THEN '    AND '+columnName+' = barLowPrice0'
				WHEN columnName LIKE 'barClosePrice%' THEN '    AND '+columnName+' = barClosePrice0'
			end joinStatements
FROM #tblSchma
WHERE OrdinalPosition IN (12,14,15,16,17)
 AND tablename LIKE 'bar%' AND tablename NOT IN ('barFeatures','barFeaturesMetaData')
 --AND columnName LIKE '%dateTimeBarOpened%'
ORDER BY tablename ASC

dateTimeBarOpened0
barOpenPrice0
barHighPrice0
barLowPrice0
barClosePrice0