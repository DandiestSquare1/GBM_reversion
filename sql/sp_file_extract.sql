USE [daytradinglogic]
GO

/****** Object:  StoredProcedure [dbo].[sp_file_extract]    Script Date: 2/28/2017 9:19:23 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE  PROCEDURE [dbo].[sp_file_extract] 
	 @ExtractNum NVARCHAR(1) 
AS
SET NOCOUNT ON

DECLARE @sp_StartTime datetime,@executionTimeInMilliseconds int, @sp_EndTime datetime
SET @sp_StartTime  = getdate()
-- your SQL statements
--WAITFOR DELAY '02:00' --Two hours
--WAITFOR DELAY '00:02' --Two minutes
--WAITFOR DELAY '00:00:02' --Two seconds
--WAITFOR DELAY '00:00:00.200' --Two tenths of a seconds
	SET @sp_EndTime  = getdate()
	SET @executionTimeInMilliseconds = datediff(ms, @sp_StartTime , getdate())
	SELECT @sp_StartTime [sp start time],@sp_EndTime [sp end time],@executionTimeInMilliseconds [execution time in Milliseconds] 

--EXEC master.dbo.sp_configure 'show advanced options', 1
--RECONFIGURE
--EXEC master.dbo.sp_configure 'xp_cmdshell', 1
--RECONFIGURE 

DECLARE @FilePath NVARCHAR(1255),@FileExtension NVARCHAR(1255),@FileDateTime NVARCHAR(1255),@HeaderFileName NVARCHAR(1255),@DataFileName NVARCHAR(1255),@GetHeaderSQLCommand NVARCHAR(1255)
		,@GetHeaderBCPCommand NVARCHAR(1255),@GetDataSQLCommand NVARCHAR(1255),@GetDataBCPCommand NVARCHAR(1255),@MergeFilesBCPCommand NVARCHAR(1255)
SET @FilePath = 'C:\sql\'
SET @FileExtension = '.txt'
SET @FileDateTime = REPLACE(REPLACE(CONVERT(VARCHAR,GETDATE(),120),' ','_'),':','')
--save header file
SELECT 'save header file'
SET @HeaderFileName = 'sp_extract_columns_'+@ExtractNum
SET @HeaderFileName = @HeaderFileName +'_'+ @FileDateTime + @FileExtension
SET @GetHeaderSQLCommand = 'EXEC DayTradingLogic.dbo.sp_extract_file_columns_'+@ExtractNum
SET @GetHeaderBCPCommand = 'bcp "'+@GetHeaderSQLCommand+' " queryout'
SET @GetHeaderBCPCommand = @GetHeaderBCPCommand +' '+ @FilePath + @HeaderFileName + ' -c -t, -T -S '+ @@servername

	SET @sp_EndTime  = getdate()
	SET @executionTimeInMilliseconds = datediff(ms, @sp_StartTime , getdate())
	SELECT @sp_StartTime [sp start time],@sp_EndTime [sp end time],@executionTimeInMilliseconds [execution time in Milliseconds] 

--save data file
SELECT '--save data file'
SET @DataFileName = 'sp_extract_data_'+@ExtractNum
SET @DataFileName = @DataFileName +'_'+ @FileDateTime + @FileExtension
SET @GetDataSQLCommand = 'EXEC DayTradingLogic.dbo.sp_extract_file_'+@ExtractNum
SET @GetDataBCPCommand = 'bcp "'+@GetDataSQLCommand+' " queryout'
SET @GetDataBCPCommand = @GetDataBCPCommand +' '+ @FilePath + @DataFileName + ' -c -t, -T -S '+ @@servername

	SET @sp_EndTime  = getdate()
	SET @executionTimeInMilliseconds = datediff(ms, @sp_StartTime , getdate())
	SELECT @sp_StartTime [sp start time],@sp_EndTime [sp end time],@executionTimeInMilliseconds [execution time in Milliseconds] 

--merge header to data file
SELECT '--merge header to data file'
SET @MergeFilesBCPCommand = 'copy ' + @FilePath + @HeaderFileName +' + '+ @FilePath + @DataFileName +' '+ @FilePath + 'data_extract_'+@ExtractNum +'_'+ @FileDateTime + @FileExtension

	SET @sp_EndTime  = getdate()
	SET @executionTimeInMilliseconds = datediff(ms, @sp_StartTime , getdate())
	SELECT @sp_StartTime [sp start time],@sp_EndTime [sp end time],@executionTimeInMilliseconds [execution time in Milliseconds] 

print @GetHeaderBCPCommand
print @GetDataBCPCommand
print @MergeFilesBCPCommand

exec master..xp_cmdshell @GetHeaderBCPCommand
	SET @sp_EndTime  = getdate()
	SET @executionTimeInMilliseconds = datediff(ms, @sp_StartTime , getdate())
	SELECT @sp_StartTime [sp start time],@sp_EndTime [sp end time],@executionTimeInMilliseconds [execution time in Milliseconds] 

exec master..xp_cmdshell @GetDataBCPCommand
	SET @sp_EndTime  = getdate()
	SET @executionTimeInMilliseconds = datediff(ms, @sp_StartTime , getdate())
	SELECT @sp_StartTime [sp start time],@sp_EndTime [sp end time],@executionTimeInMilliseconds [execution time in Milliseconds] 

exec master..xp_cmdshell @MergeFilesBCPCommand
	SET @sp_EndTime  = getdate()
	SET @executionTimeInMilliseconds = datediff(ms, @sp_StartTime , getdate())
	SELECT @sp_StartTime [sp start time],@sp_EndTime [sp end time],@executionTimeInMilliseconds [execution time in Milliseconds] 


GO


