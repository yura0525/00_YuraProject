USE [KIMGAME]
GO
/****** Object:  StoredProcedure [dbo].[usp_pass]    Script Date: 10/29/2018 15:48:01 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[usp_pass]
	@UID NCHAR(10)
AS
	SELECT	* FROM dbo.USERLIST
	WHERE @UID=USERID;
RETURN 3;