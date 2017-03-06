--http://stackoverflow.com/questions/3628846/insert-xml-into-sql-server-2008-database
--https://www.youtube.com/watch?v=tg3GD2_V4_I
USE daytradinglogic
GO 

DROP TABLE barFeaturesXML0

create table barFeaturesXML0 (
	fileId bigint,
	timeSeries XML,
	createdTime DATETIME2(7)
)

SELECT * FROM barFeaturesXML0

create table MyTbl (
	fileId bigint,
	filemeta XML,
	createdTime DATETIME
)

SELECT * FROM MyTbl
select * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = 'MyTbl'

INSERT INTO MyTbl
  (fileId, filemeta,createdTime) 
VALUES 
  (2
	,N'<?xml version=''1.0'' encoding=''utf-16'' standalone=''yes''?><!--This is a test XML file--><filemeta filetype="Audio"><Comments /><AlbumTitle /><TrackNumber />
	<ArtistName /><Year /><Genre /><TrackTitle /></filemeta>'
	,GETDATE()
  );

