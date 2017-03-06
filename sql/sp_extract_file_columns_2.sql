USE [daytradinglogic]
GO

/****** Object:  StoredProcedure [dbo].[sp_extract_file_columns_2]    Script Date: 2/28/2017 9:20:15 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO


CREATE PROCEDURE [dbo].[sp_extract_file_columns_2]
AS
SET NOCOUNT ON

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
FROM daytradinglogic.information_schema.columns 
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

SELECT 'barClass,'+STUFF((SELECT ',' 
	+ replace(replace(replace(replace(columnName,'[RightStrength',''),']',''),'+','p'),'-','m') ColumnName
	FROM #tblSchma
	WHERE OrdinalPosition > 21 
		AND RowNum BETWEEN (4096*1)+1 AND (4096*2) AND TableName NOT IN ('barFeatures0')
	ORDER BY TableName, OrdinalPosition ASC
        FOR XML PATH(''), TYPE).value('.', 'VARCHAR(MAX)'), 1, 1, '') AS listStr


GO


