USE [KIMGAME]
GO
/****** Object:  StoredProcedure [dbo].[usp_pass]    Script Date: 10/26/2018 18:39:53 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[usp_pass]
	@UID NCHAR(10)
AS
	SELECT	* FROM dbo.USERLIST
	WHERE @UID=USERID;
RETURN 2