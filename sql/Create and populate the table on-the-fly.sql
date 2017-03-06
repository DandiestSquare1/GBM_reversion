--http://mitchelsellers.com/blogs/2008/09/12/creating-random-sql-server-test-data.aspx
/*** version 1 ***/
DECLARE @RowCount INT
DECLARE @RowString VARCHAR(10)
DECLARE @Random INT
DECLARE @Upper INT
DECLARE @Lower INT
DECLARE @InsertDate DATETIME

SET @Lower = - 730
SET @Upper = - 1
SET @RowCount = 0

WHILE @RowCount < 3000000
BEGIN
	SET @RowString = CAST(@RowCount AS VARCHAR(10))

	SELECT @Random = ROUND(((@Upper - @Lower - 1) * RAND() + @Lower), 0)

	SET @InsertDate = DATEADD(dd, @Random, GETDATE())

	INSERT INTO TestTableSize (
		MyKeyField
		,MyDate1
		,MyDate2
		,MyDate3
		,MyDate4
		,MyDate5
		)
	VALUES (
		REPLICATE('0', 10 - DATALENGTH(@RowString)) + @RowString
		,@InsertDate
		,DATEADD(dd, 1, @InsertDate)
		,DATEADD(dd, 2, @InsertDate)
		,DATEADD(dd, 3, @InsertDate)
		,DATEADD(dd, 4, @InsertDate)
		)

	SET @RowCount = @RowCount + 1
END

/*** version 2 ***/
--===== If the test table already exists, drop it to make reruns.
-- in SSMS easier.
IF OBJECT_ID('dbo.TestTableSize', 'U') IS NOT NULL
	DROP TABLE dbo.TestTableSize;
GO

--===== Environment presets
-- Suppress the auto-display of
rowcounts TO increase performance.

SET NOCOUNT ON;

--===== Create the test table.
CREATE TABLE dbo.TestTableSize (
	MyKeyField VARCHAR(10) NOT NULL
	,MyDate1 DATETIME NOT NULL
	,MyDate2 DATETIME NOT NULL
	,MyDate3 DATETIME NOT NULL
	,MyDate4 DATETIME NOT NULL
	,MyDate5 DATETIME NOT NULL
	);

--===== Local variables
DECLARE @RowCount INT
	,@RowString VARCHAR(10)
	,@Random INT
	,@Upper INT
	,@Lower INT
	,@InsertDate DATETIME;

--===== Presets
SELECT @Lower = - 730
	,@Upper = - 1
	,@RowCount = 0;

--===== Populate the table using a transaction to substantially
-- increase performance of the
WHILE LOOP.
	BEGIN TRANSACTION

WHILE @RowCount < 3000000
BEGIN
	--===== Single
	SELECT instead OF multiple SETs TO increase performance

	SELECT @RowString = CAST(@RowCount AS VARCHAR(10))
		,@Random = ROUND(((@Upper - @Lower - 1) * RAND() + @Lower), 0)
		,@InsertDate = DATEADD(dd, @Random, GETDATE());

	INSERT INTO dbo.TestTableSize (
		MyKeyField
		,MyDate1
		,MyDate2
		,MyDate3
		,MyDate4
		,MyDate5
		)
	SELECT
		--Used SELECT instead of values to add clarity to which formula is which column
		MyKeyField = REPLICATE('0', 10 - DATALENGTH(@RowString)) + @RowString
		,MyDate1 = @InsertDate
		,MyDate2 = DATEADD(dd, 1, @InsertDate)
		,MyDate3 = DATEADD(dd, 2, @InsertDate)
		,MyDate4 = DATEADD(dd, 3, @InsertDate)
		,MyDate5 = DATEADD(dd, 4, @InsertDate);

	SELECT @RowCount = @RowCount + 1;
END;

COMMIT;

/*** version 3 ***/
--===== If the test table already exists, drop it to make reruns.
-- in SSMS easier.
IF OBJECT_ID('dbo.TestTableSize', 'U') IS NOT NULL
	DROP TABLE dbo.TestTableSize;
GO

--===== Local Variables
DECLARE @RequiredRows INT
	,@LoDate DATETIME --Inclusive (Temporally "Closed")
	,@HiDate DATETIME --Exclusive (Temporally "Open")
	,@Days INT;

--===== Presets
SELECT @RequiredRows = 3000000
	,@LoDate = DATEADD(yy, - 2, CAST(GETDATE() AS DATE)) --Inclusive (Temporally "Closed")
	,@HiDate = CAST(GETDATE() AS DATE) --Exclusive (Temporally "Open")
	,@Days = DATEDIFF(dd, @LoDate, @HiDate);

--===== Create and populate the table on-the-fly.
-- Note that the resulting test table has all the correct datatypes and NULLability.
SELECT TOP (@RequiredRows) --The ISNULLs make NOT NULL columns
	MyKeyField = ISNULL(RIGHT(ROW_NUMBER() OVER (
				ORDER BY (
						SELECT NULL
						)
				) + 10000000000, 10), '')
	,MyDate1 = ISNULL(RAND(CHECKSUM(NEWID())) * @Days + @LoDate, 0)
	,MyDate2 = ISNULL(RAND(CHECKSUM(NEWID())) * @Days + @LoDate, 0)
	,MyDate3 = ISNULL(RAND(CHECKSUM(NEWID())) * @Days + @LoDate, 0)
	,MyDate4 = ISNULL(RAND(CHECKSUM(NEWID())) * @Days + @LoDate, 0)
	,MyDate5 = ISNULL(RAND(CHECKSUM(NEWID())) * @Days + @LoDate, 0)
INTO dbo.TestTableSize
FROM sys.all_columns ac1
CROSS JOIN sys.all_columns ac2;
