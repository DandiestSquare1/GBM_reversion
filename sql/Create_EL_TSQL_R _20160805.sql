/**********************************
	DROP TABLE #Create_EL_TSQL
	DROP TABLE #CreateCode
	DROP TABLE #Create_EL_TSQL_R
	DROP TABLE #tmp
	DROP TABLE [dbo].[barFeatures1]
	DROP TABLE [dbo].[barFeatures2]
	DROP TABLE [dbo].[barFeatures3]
	DROP TABLE [dbo].[barFeatures4]
	DROP TABLE [dbo].[barFeatures5]
	DROP TABLE [dbo].[barFeatures6]
	DROP TABLE [dbo].[barFeatures7]
	DROP TABLE [dbo].[barFeatures8]
	DROP TABLE [dbo].[barFeatures9]
	DROP TABLE [dbo].[barFeatures10]
	DROP TABLE [dbo].[barFeaturesLongTerm]
	DROP TABLE [dbo].[barFeaturesMidTerm]
	DROP TABLE [dbo].[barFeaturesNew]
	DROP TABLE [dbo].[barFeaturesShortTerm]
**********************************/

CREATE TABLE #Create_EL_TSQL (
	[Id] [bigint] IDENTITY(1,1) NOT NULL,
	[IndicatorName] VARCHAR(20)
    );

INSERT 
    INTO #Create_EL_TSQL 
    VALUES 
	('ACindBIG'),
('ACindBX'),
('ACindBXL'),
('ACindMED'),
('ACindSMALL'),
('ADXRindBIG'),
('ADXRindBX'),
('ADXRindBXL'),
('ADXRindMED'),
('ADXRindSMALL'),
('AROONindBIG'),
('AROONindBX'),
('AROONindBXL'),
('AROONindMED'),
('AROONindSMALL'),
('ATRindBIG'),
('ATRindBX'),
('ATRindBXL'),
('ATRindMED'),
('ATRindSMALL'),
('AWEOSCindBIG'),
('AWEOSCindBX'),
('AWEOSCindBXL'),
('AWEOSCindMED'),
('AWEOSCindSMALL'),
('BBDindBIG'),
('BBDindBX'),
('BBDindBXL'),
('BBDindMED'),
('BBDindSMALL'),
('CCIindBIG'),
('CCIindBX'),
('CCIindBXL'),
('CCIindMED'),
('CCIindSMALL'),
('CDMIindBIG'),
('CDMIindBX'),
('CDMIindBXL'),
('CDMIindMED'),
('CDMIindSMALL'),
('CHOPindBIG'),
('CHOPindBX'),
('CHOPindBXL'),
('CHOPindMED'),
('CHOPindSMALL'),
('CMIindBIG'),
('CMIindBX'),
('CMIindBXL'),
('CMIindMED'),
('CMIindSMALL'),
('CMOindBIG'),
('CMOindBX'),
('CMOindBXL'),
('CMOindMED'),
('CMOindSMALL'),
('CUMRSIindBIG'),
('CUMRSIindBX'),
('CUMRSIindBXL'),
('CUMRSIindMED'),
('CUMRSIindSMALL'),
('DMIStoindBIG'),
('DMIStoindBX'),
('DMIStoindBXL'),
('DMIStoindMED'),
('DMIStoindSMALL'),
('ELSindBIG'),
('ELSindBX'),
('ELSindBXL'),
('ELSindMED'),
('ELSindSMALL'),
('EMDindBIG'),
('EMDindBX'),
('EMDindBXL'),
('EMDindMED'),
('EMDindSMALL'),
('FRAMAindBIG'),
('FRAMAindBX'),
('FRAMAindBXL'),
('FRAMAindMED'),
('FRAMAindSMALL'),
('KLTNRindBIG'),
('KLTNRindBX'),
('KLTNRindBXL'),
('KLTNRindMED'),
('KLTNRindSMALL'),
('KSTindBIG'),
('KSTindBX'),
('KSTindBXL'),
('KSTindMED'),
('KSTindSMALL'),
('LINREGindBIG'),
('LINREGindBX'),
('LINREGindBXL'),
('LINREGindMED'),
('LINREGindSMALL'),
('LINRGCindBIG'),
('LINRGCindBX'),
('LINRGCindBXL'),
('LINRGCindMED'),
('LINRGCindSMALL'),
('LRSIindBIG'),
('LRSIindBX'),
('LRSIindBXL'),
('LRSIindMED'),
('LRSIindSMALL'),
('MaALMAindBIG'),
('MaALMAindBX'),
('MaALMAindBXL'),
('MaALMAindMED'),
('MaALMAindSMALL'),
('MaAMAindBIG'),
('MaAMAindBX'),
('MaAMAindBXL'),
('MaAMAindMED'),
('MaAMAindSMALL'),
('MaDEMAindBIG'),
('MaDEMAindBX'),
('MaDEMAindBXL'),
('MaDEMAindMED'),
('MaDEMAindSMALL'),
('MaEMAindBIG'),
('MaEMAindBX'),
('MaEMAindBXL'),
('MaEMAindMED'),
('MaEMAindSMALL'),
('MaEPMAindBIG'),
('MaEPMAindBX'),
('MaEPMAindBXL'),
('MaEPMAindMED'),
('MaEPMAindSMALL'),
('MaHMAindBIG'),
('MaHMAindBX'),
('MaHMAindBXL'),
('MaHMAindMED'),
('MaHMAindSMALL'),
('MaLGREindBIG'),
('MaLGREindBX'),
('MaLGREindBXL'),
('MaLGREindMED'),
('MaLGREindSMALL'),
('MaMAMAindBIG'),
('MaMAMAindBX'),
('MaMAMAindBXL'),
('MaMAMAindMED'),
('MaMAMAindSMALL'),
('MaMMAindBIG'),
('MaMMAindBX'),
('MaMMAindBXL'),
('MaMMAindMED'),
('MaMMAindSMALL'),
('MaSMAindBIG'),
('MaSMAindBX'),
('MaSMAindBXL'),
('MaSMAindMED'),
('MaSMAindSMALL'),
('MaSWMAindBIG'),
('MaSWMAindBX'),
('MaSWMAindBXL'),
('MaSWMAindMED'),
('MaSWMAindSMALL'),
('MaTEMAindBIG'),
('MaTEMAindBX'),
('MaTEMAindBXL'),
('MaTEMAindMED'),
('MaTEMAindSMALL'),
('MaTRIindBIG'),
('MaTRIindBX'),
('MaTRIindBXL'),
('MaTRIindMED'),
('MaTRIindSMALL'),
('MaVMAindBIG'),
('MaVMAindBX'),
('MaVMAindBXL'),
('MaVMAindMED'),
('MaVMAindSMALL'),
('MaWMAindBIG'),
('MaWMAindBX'),
('MaWMAindBXL'),
('MaWMAindMED'),
('MaWMAindSMALL'),
('MaZLAGindBIG'),
('MaZLAGindBX'),
('MaZLAGindBXL'),
('MaZLAGindMED'),
('MaZLAGindSMALL'),
('PCHANindBIG'),
('PCHANindBX'),
('PCHANindBXL'),
('PCHANindMED'),
('PCHANindSMALL'),
('PCHGindBIG'),
('PCHGindBX'),
('PCHGindBXL'),
('PCHGindMED'),
('PCHGindSMALL'),
('pctBindBIG'),
('pctBindBX'),
('pctBindBXL'),
('pctBindMED'),
('pctBindSMALL'),
('pctHindBIG'),
('pctHindBX'),
('pctHindBXL'),
('pctHindMED'),
('pctHindSMALL'),
('PCTRindBIG'),
('PCTRindBX'),
('PCTRindBXL'),
('PCTRindMED'),
('PCTRindSMALL'),
('POSCindBIG'),
('POSCindBX'),
('POSCindBXL'),
('POSCindMED'),
('POSCindSMALL'),
('PSARindBIG'),
('PSARindBX'),
('PSARindBXL'),
('PSARindMED'),
('PSARindSMALL'),
('RMIindBIG'),
('RMIindBX'),
('RMIindBXL'),
('RMIindMED'),
('RMIindSMALL'),
('ROCindBIG'),
('ROCindBX'),
('ROCindBXL'),
('ROCindMED'),
('ROCindSMALL'),
('RSIHULindBIG'),
('RSIHULindBX'),
('RSIHULindBXL'),
('RSIHULindMED'),
('RSIHULindSMALL'),
('RSIindBIG'),
('RSIindBX'),
('RSIindBXL'),
('RSIindMED'),
('RSIindSMALL'),
('RSQRindBIG'),
('RSQRindBX'),
('RSQRindBXL'),
('RSQRindMED'),
('RSQRindSMALL'),
('RVIindBIG'),
('RVIindBX'),
('RVIindBXL'),
('RVIindMED'),
('RVIindSMALL'),
('STDDEVindBIG'),
('STDDEVindBX'),
('STDDEVindBXL'),
('STDDEVindMED'),
('STDDEVindSMALL'),
('STDERRindBIG'),
('STDERRindBX'),
('STDERRindBXL'),
('STDERRindMED'),
('STDERRindSMALL'),
('STOFindBIG'),
('STOFindBX'),
('STOFindBXL'),
('STOFindMED'),
('STOFindSMALL'),
('StoRSIindBIG'),
('StoRSIindBX'),
('StoRSIindBXL'),
('StoRSIindMED'),
('StoRSIindSMALL'),
('STOSindBIG'),
('STOSindBX'),
('STOSindBXL'),
('STOSindMED'),
('STOSindSMALL'),
('UNIOSCindBIG'),
('UNIOSCindBX'),
('UNIOSCindBXL'),
('UNIOSCindMED'),
('UNIOSCindSMALL'),
('VHFindBIG'),
('VHFindBX'),
('VHFindBXL'),
('VHFindMED'),
('VHFindSMALL'),
('VIDYAindBIG'),
('VIDYAindBX'),
('VIDYAindBXL'),
('VIDYAindMED'),
('VIDYAindSMALL'),
('VIMAindBIG'),
('VIMAindBX'),
('VIMAindBXL'),
('VIMAindMED'),
('VIMAindSMALL');


;with CreateCode as (
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength-4],0)' as EL, ',['+IndicatorName+'[RightStrength-4]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL 
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength-3],0)' as EL, ',['+IndicatorName+'[RightStrength-3]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength-2],0)' as EL, ',['+IndicatorName+'[RightStrength-2]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength-1],0)' as EL, ',['+IndicatorName+'[RightStrength-1]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+0],0)' as EL, ',['+IndicatorName+'[RightStrength+0]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+1],0)' as EL, ',['+IndicatorName+'[RightStrength+1]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+2],0)' as EL, ',['+IndicatorName+'[RightStrength+2]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+3],0)' as EL, ',['+IndicatorName+'[RightStrength+3]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+4],0)' as EL, ',['+IndicatorName+'[RightStrength+4]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+5],0)' as EL, ',['+IndicatorName+'[RightStrength+5]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+6],0)' as EL, ',['+IndicatorName+'[RightStrength+6]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+7],0)' as EL, ',['+IndicatorName+'[RightStrength+7]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+8],0)' as EL, ',['+IndicatorName+'[RightStrength+8]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+9],0)' as EL, ',['+IndicatorName+'[RightStrength+9]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+10],0)' as EL, ',['+IndicatorName+'[RightStrength+10]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+11],0)' as EL, ',['+IndicatorName+'[RightStrength+11]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+12],0)' as EL, ',['+IndicatorName+'[RightStrength+12]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+13],0)' as EL, ',['+IndicatorName+'[RightStrength+13]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+14],0)' as EL, ',['+IndicatorName+'[RightStrength+14]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+15],0)' as EL, ',['+IndicatorName+'[RightStrength+15]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+16],0)' as EL, ',['+IndicatorName+'[RightStrength+16]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+17],0)' as EL, ',['+IndicatorName+'[RightStrength+17]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+18],0)' as EL, ',['+IndicatorName+'[RightStrength+18]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+19],0)' as EL, ',['+IndicatorName+'[RightStrength+19]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+20],0)' as EL, ',['+IndicatorName+'[RightStrength+20]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+21],0)' as EL, ',['+IndicatorName+'[RightStrength+21]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+22],0)' as EL, ',['+IndicatorName+'[RightStrength+22]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+23],0)' as EL, ',['+IndicatorName+'[RightStrength+23]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+24],0)' as EL, ',['+IndicatorName+'[RightStrength+24]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+25],0)' as EL, ',['+IndicatorName+'[RightStrength+25]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+26],0)' as EL, ',['+IndicatorName+'[RightStrength+26]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+27],0)' as EL, ',['+IndicatorName+'[RightStrength+27]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL UNION ALL
SELECT *, '+","+Numtostr('+IndicatorName+'[RightStrength+28],0)' as EL, ',['+IndicatorName+'[RightStrength+28]]] [smallint] NULL,' as [TSQL_Tbl] FROM #Create_EL_TSQL 
)
SELECT ROW_NUMBER() OVER (ORDER BY EL) AS [recId]
	,IndicatorName, EL,REPLACE([TSQL_Tbl],'NULL,','NULL') as [TSQL_Tbl]
	, REPLACE(REPLACE(c.[TSQL_Tbl],'],',']'),'[smallint] NULL,',' as ['+REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(c.[EL],'+","+Numtostr(',''),'.ind',''),'[RightStrength',''),'+','P'),'-','M'),'],0)','')+']') as [TSQL_View]
	, REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(c.[EL],'+","+Numtostr(',''),'.ind',''),'[RightStrength',''),'+','P'),'-','M'),'],0)','') as [R] 
INTO #CreateCode
FROM CreateCode c

/*************************
	SELECT * FROM [tempdb].INFORMATION_SCHEMA.COLUMNS c WHERE c.TABLE_NAME LIKE '#CreateCode%'
	SELECT * FROM tempdb.sys.columns where object_id = object_id('tempdb..#CreateCode');
*************************/

CREATE TABLE #Create_EL_TSQL_R (
	batchId INT NULL,
	recId BIGINT NULL,
	IndicatorName VARCHAR(255) NULL,
	EL VARCHAR(255) NULL,
	TSQL_Tbl VARCHAR(255) NULL,
	TSQL_View VARCHAR(255) NULL,
	R VARCHAR(255) NULL,
    );

DECLARE @id INT
	,@batchSize INT
	,@batchId INT
	,@interval INT
	,@tblSize INT;

SET @id = 1;
SET @batchSize = 1000;
SET @batchId = 1;
SET @interval = 1000;
SET @tblSize = (
		SELECT count(*)
		FROM #CreateCode
		);

WHILE @id < @tblSize + 1
BEGIN
	IF @id = @batchSize + 1
	BEGIN
		SET @batchSize = @batchSize + @interval
		SET @batchId = @batchId + 1
	END

	PRINT convert(VARCHAR(255), @batchSize) + ', ' + convert(VARCHAR(255), @id)
	
	INSERT 
    INTO #Create_EL_TSQL_R 
		SELECT @batchId, recId, IndicatorName, EL, TSQL_Tbl, TSQL_View, R FROM #CreateCode WHERE recId = @id

	IF @id = @tblSize
	BEGIN
		PRINT '/********** END ***********/'
	END

	SELECT @id = @id + 1
END

--verify distinct batchId's and record count for each group
SELECT batchId,COUNT(*) recordCount FROM #Create_EL_TSQL_R GROUP BY batchId ORDER BY batchId ASC;

--verify no dups exist for recId
SELECT recId,COUNT(*) recordCount FROM #Create_EL_TSQL_R GROUP BY recId HAVING COUNT(*) > 1 ORDER BY recId ASC;

--manually review data
--SELECT * FROM #Create_EL_TSQL_R ORDER BY batchId, recId ASC;

--create new table for each batchId
--this query creates the CREATE TABLE statement for @Tbl_Id = n
DECLARE @Tbl_Id Int, @Max_Tbl_Id Int, @XMLData XML, @ConvertedXMLData nvarchar(max),@CreateTbl nvarchar(max);
SET @Tbl_Id = (SELECT min(batchId) FROM #Create_EL_TSQL_R) ;
SET @Max_Tbl_Id = (SELECT max(batchId) FROM #Create_EL_TSQL_R) ;

WHILE @Tbl_Id < @Max_Tbl_Id + 1
BEGIN

	SET @XMLData = (
		SELECT ' ' + [TSQL_Tbl] 
		FROM #Create_EL_TSQL_R WHERE batchId = @Tbl_Id
		ORDER BY recId ASC
		FOR XML PATH('')  
		);
	
	IF @Tbl_Id = (SELECT min(batchId) FROM #Create_EL_TSQL_R)
	BEGIN
		SELECT @Tbl_Id batchId
				,format(len(convert(nvarchar(max),@XMLData)),'N0') CharLength
				,RIGHT(convert(nvarchar(max),@XMLData),len(convert(nvarchar(max),@XMLData))-1) ConcatenatedCols , format(convert(bigint,NULL),'N0') [LEN(@CreateTbl)]
		INTO #tmp;
	END
	ELSE
	BEGIN
		INSERT 
		INTO #tmp 
			SELECT @Tbl_Id batchId
					,format(len(convert(nvarchar(max),@XMLData)),'N0') CharLength
					,RIGHT(convert(nvarchar(max),@XMLData),len(convert(nvarchar(max),@XMLData))-1) ConcatenatedCols ,format(convert(bigint,NULL),'N0') [LEN(@CreateTbl)];
	END

	SET @ConvertedXMLData = (SELECT ConcatenatedCols FROM #tmp WHERE batchId = @Tbl_Id)
	--debug
	--PRINT len(@ConvertedXMLData)

	SET @CreateTbl = ('
		DECLARE @Tbl_Id Int, @Max_Tbl_Id Int, @XMLData XML, @ConvertedXMLData nvarchar(max),@CreateTbl nvarchar(max);'+CHAR(10)+
		'SET @Tbl_Id = '+CONVERT(VARCHAR(10),@Tbl_Id)+' ;'+CHAR(10)+
		'SET @Max_Tbl_Id = '+CONVERT(VARCHAR(10),@Max_Tbl_Id)+' ;'+CHAR(10)+
		'SET @ConvertedXMLData = '+CONVERT(VARCHAR(max),len(@ConvertedXMLData))+';'+CHAR(10)+	
		'CREATE TABLE [dbo].[barFeatures'+CONVERT(VARCHAR(10),@Tbl_Id)+']('+CHAR(10)+
		'	[recId] [bigint] IDENTITY(1,1) NOT NULL,
			[loadId] [int] NULL,
			[loadGroupId] [smallint] NULL,
			[RightStrength] [smallint] NULL,
			[recCreateDateTime] [datetime2](7) NULL,
			[recModiedDateTime] [datetime2](7) NULL,
			[seriesBarNumber-1] [bigint] NULL,
			[seriesBarNumber-RightStrength] [bigint] NULL,
			[lengthOfSeries] [bigint] NULL,
			[dateBarOpened] [nvarchar](11) NULL,
			[dayOfWeekBarOpened] [varchar](255) NULL,
			[dateTimeBarOpened] [varchar](255) NULL,
			[timeBarOpenedhhmmss] [varchar](255) NULL,
			[barOpenPrice] [decimal](25, 8) NULL,
			[barHighPrice] [decimal](25, 8) NULL,
			[barLowPrice] [decimal](25, 8) NULL,
			[barClosePrice] [decimal](25, 8) NULL,
			[barClass] [smallint] NULL,
			[swingRecordType] [smallint] NULL,
			[barRecordType] [smallint] NULL,
			[swingSize] [decimal](25, 7) NULL'+CHAR(10)
			+@ConvertedXMLData+CHAR(10)+
			'  CONSTRAINT [PK_barFeatures'+CONVERT(VARCHAR(10),@Tbl_Id)+'] PRIMARY KEY CLUSTERED'+CHAR(10)+
		'( '+CHAR(10)+
		'	[recId] ASC '+CHAR(10)+
		')WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY] '+CHAR(10)+
		') ON [PRIMARY] ;'+CHAR(10)+
		' ' );

	--show script to create table
	--PRINT @CreateTbl
	UPDATE tm
	SET tm.[LEN(@CreateTbl)] = format(len(@CreateTbl),'N0')
	FROM #tmp tm
	WHERE tm.batchId = @Tbl_Id

	SELECT'('+CONVERT(VARCHAR(10),@Tbl_Id)+')' ,@Max_Tbl_Id ,'EXECUTE Sp_executesql @CreateTbl', CONVERT(VARCHAR(max),@CreateTbl)
	
	EXECUTE Sp_executesql @CreateTbl

	IF  @Tbl_Id = @Max_Tbl_Id
	BEGIN
		SELECT '/******** Commands completed successfully ********/';
		SELECT c.TABLE_NAME, COUNT(*) [Columns] FROM [daytradinglogic].INFORMATION_SCHEMA.COLUMNS c WHERE c.TABLE_NAME LIKE 'barFeatures%' GROUP BY c.TABLE_NAME;
		SELECT * FROM #tmp;
		--SELECT * FROM [daytradinglogic].INFORMATION_SCHEMA.COLUMNS c WHERE c.TABLE_NAME LIKE 'barFeatures%';
	END	

	SET @Tbl_Id = @Tbl_Id + 1;
END

--run the following queries one at a time, then copy the quert results into EL IDE
	SELECT 'sql_insert_features = "INSERT INTO  [dbo].[barFeatures1] VALUES ("+queryResult//MAX(loadId)+","+Numtostr(loadGroupId,0)+","+Numtostr(RightStrength,0)//RightStrength+","+"''"+queryResultdatetime+"'' "//[recCreateDateTime]+","+"''"+queryResultdatetime+"'' "//[recModiedDateTime]+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"''"+dateBarOpened+"'' "+","+"''"+dayOfWeekBarOpened+"'' "+","+"''"+dateTimeBarOpened+"'' "+","+"''"+timeBarOpenedhhmmss+"'' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)' [//EL]UNION ALL SELECT EL as [//EL] FROM #Create_EL_TSQL_R WHERE batchId = 1 UNION ALL SELECT '+" )";  res1 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_features);' [//EL]
	SELECT 'sql_insert_features = "INSERT INTO  [dbo].[barFeatures2] VALUES ("+queryResult//MAX(loadId)+","+Numtostr(loadGroupId,0)+","+Numtostr(RightStrength,0)//RightStrength+","+"''"+queryResultdatetime+"'' "//[recCreateDateTime]+","+"''"+queryResultdatetime+"'' "//[recModiedDateTime]+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"''"+dateBarOpened+"'' "+","+"''"+dayOfWeekBarOpened+"'' "+","+"''"+dateTimeBarOpened+"'' "+","+"''"+timeBarOpenedhhmmss+"'' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)' [//EL]UNION ALL SELECT EL as [//EL] FROM #Create_EL_TSQL_R WHERE batchId = 2 UNION ALL SELECT '+" )";  res2 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_features);' [//EL]
	SELECT 'sql_insert_features = "INSERT INTO  [dbo].[barFeatures3] VALUES ("+queryResult//MAX(loadId)+","+Numtostr(loadGroupId,0)+","+Numtostr(RightStrength,0)//RightStrength+","+"''"+queryResultdatetime+"'' "//[recCreateDateTime]+","+"''"+queryResultdatetime+"'' "//[recModiedDateTime]+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"''"+dateBarOpened+"'' "+","+"''"+dayOfWeekBarOpened+"'' "+","+"''"+dateTimeBarOpened+"'' "+","+"''"+timeBarOpenedhhmmss+"'' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)' [//EL]UNION ALL SELECT EL as [//EL] FROM #Create_EL_TSQL_R WHERE batchId = 3 UNION ALL SELECT '+" )";  res3 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_features);' [//EL]
	SELECT 'sql_insert_features = "INSERT INTO  [dbo].[barFeatures4] VALUES ("+queryResult//MAX(loadId)+","+Numtostr(loadGroupId,0)+","+Numtostr(RightStrength,0)//RightStrength+","+"''"+queryResultdatetime+"'' "//[recCreateDateTime]+","+"''"+queryResultdatetime+"'' "//[recModiedDateTime]+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"''"+dateBarOpened+"'' "+","+"''"+dayOfWeekBarOpened+"'' "+","+"''"+dateTimeBarOpened+"'' "+","+"''"+timeBarOpenedhhmmss+"'' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)' [//EL]UNION ALL SELECT EL as [//EL] FROM #Create_EL_TSQL_R WHERE batchId = 4 UNION ALL SELECT '+" )";  res4 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_features);' [//EL]
	SELECT 'sql_insert_features = "INSERT INTO  [dbo].[barFeatures5] VALUES ("+queryResult//MAX(loadId)+","+Numtostr(loadGroupId,0)+","+Numtostr(RightStrength,0)//RightStrength+","+"''"+queryResultdatetime+"'' "//[recCreateDateTime]+","+"''"+queryResultdatetime+"'' "//[recModiedDateTime]+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"''"+dateBarOpened+"'' "+","+"''"+dayOfWeekBarOpened+"'' "+","+"''"+dateTimeBarOpened+"'' "+","+"''"+timeBarOpenedhhmmss+"'' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)' [//EL]UNION ALL SELECT EL as [//EL] FROM #Create_EL_TSQL_R WHERE batchId = 5 UNION ALL SELECT '+" )";  res5 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_features);' [//EL]
	SELECT 'sql_insert_features = "INSERT INTO  [dbo].[barFeatures6] VALUES ("+queryResult//MAX(loadId)+","+Numtostr(loadGroupId,0)+","+Numtostr(RightStrength,0)//RightStrength+","+"''"+queryResultdatetime+"'' "//[recCreateDateTime]+","+"''"+queryResultdatetime+"'' "//[recModiedDateTime]+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"''"+dateBarOpened+"'' "+","+"''"+dayOfWeekBarOpened+"'' "+","+"''"+dateTimeBarOpened+"'' "+","+"''"+timeBarOpenedhhmmss+"'' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)' [//EL]UNION ALL SELECT EL as [//EL] FROM #Create_EL_TSQL_R WHERE batchId = 6 UNION ALL SELECT '+" )";  res6 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_features);' [//EL]
	SELECT 'sql_insert_features = "INSERT INTO  [dbo].[barFeatures7] VALUES ("+queryResult//MAX(loadId)+","+Numtostr(loadGroupId,0)+","+Numtostr(RightStrength,0)//RightStrength+","+"''"+queryResultdatetime+"'' "//[recCreateDateTime]+","+"''"+queryResultdatetime+"'' "//[recModiedDateTime]+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"''"+dateBarOpened+"'' "+","+"''"+dayOfWeekBarOpened+"'' "+","+"''"+dateTimeBarOpened+"'' "+","+"''"+timeBarOpenedhhmmss+"'' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)' [//EL]UNION ALL SELECT EL as [//EL] FROM #Create_EL_TSQL_R WHERE batchId = 7 UNION ALL SELECT '+" )";  res7 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_features);' [//EL]
	SELECT 'sql_insert_features = "INSERT INTO  [dbo].[barFeatures8] VALUES ("+queryResult//MAX(loadId)+","+Numtostr(loadGroupId,0)+","+Numtostr(RightStrength,0)//RightStrength+","+"''"+queryResultdatetime+"'' "//[recCreateDateTime]+","+"''"+queryResultdatetime+"'' "//[recModiedDateTime]+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"''"+dateBarOpened+"'' "+","+"''"+dayOfWeekBarOpened+"'' "+","+"''"+dateTimeBarOpened+"'' "+","+"''"+timeBarOpenedhhmmss+"'' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)' [//EL]UNION ALL SELECT EL as [//EL] FROM #Create_EL_TSQL_R WHERE batchId = 8 UNION ALL SELECT '+" )";  res8 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_features);' [//EL]
	SELECT 'sql_insert_features = "INSERT INTO  [dbo].[barFeatures9] VALUES ("+queryResult//MAX(loadId)+","+Numtostr(loadGroupId,0)+","+Numtostr(RightStrength,0)//RightStrength+","+"''"+queryResultdatetime+"'' "//[recCreateDateTime]+","+"''"+queryResultdatetime+"'' "//[recModiedDateTime]+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"''"+dateBarOpened+"'' "+","+"''"+dayOfWeekBarOpened+"'' "+","+"''"+dateTimeBarOpened+"'' "+","+"''"+timeBarOpenedhhmmss+"'' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)' [//EL]UNION ALL SELECT EL as [//EL] FROM #Create_EL_TSQL_R WHERE batchId = 9 UNION ALL SELECT '+" )";  res9 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_features);' [//EL]
	SELECT 'sql_insert_features = "INSERT INTO  [dbo].[barFeatures10] VALUES ("+queryResult//MAX(loadId)+","+Numtostr(loadGroupId,0)+","+Numtostr(RightStrength,0)//RightStrength+","+"''"+queryResultdatetime+"'' "//[recCreateDateTime]+","+"''"+queryResultdatetime+"'' "//[recModiedDateTime]+","+Numtostr(seriesBarNumber-1,0)+","+Numtostr(seriesBarNumber-RightStrength,0)+","+Numtostr(lengthOfSeries,0)+","+"''"+dateBarOpened+"'' "+","+"''"+dayOfWeekBarOpened+"'' "+","+"''"+dateTimeBarOpened+"'' "+","+"''"+timeBarOpenedhhmmss+"'' "+","+Numtostr(barOpenPrice,8)+","+Numtostr(barHighPrice,8)+","+Numtostr(barLowPrice,8)+","+Numtostr(barClosePrice,8)+","+Numtostr(barClass,0)+","+Numtostr(swingRecordType,0)+","+Numtostr(barRecordType,0)+","+Numtostr(swingSize,8)' [//EL]UNION ALL SELECT EL as [//EL] FROM #Create_EL_TSQL_R WHERE batchId = 10 UNION ALL SELECT '+" )";  res10 = execsql(sql_server, sql_user, sql_password, sql_dbname, sql_insert_features);' [//EL]


