USE [KIMGAME]
GO
/****** Object:  StoredProcedure [dbo].[usp_pass]    Script Date: 10/29/2018 15:54:31 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[usp_pass]
	@UID NCHAR(10)
AS
BEGIN
	SELECT	* FROM dbo.USERLIST
	WHERE @UID=USERID;
	END
RETURN 0