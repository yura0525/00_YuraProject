﻿CREATE TABLE [dbo].[Table]
(
	[Id] INT NOT NULL PRIMARY KEY, 
    [USERID] NCHAR(10) NOT NULL, 
    [USERPS] NCHAR(10) NOT NULL, 
    [ACCOUNTTIME] SMALLDATETIME NOT NULL DEFAULT getdate()
)
