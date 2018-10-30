USE [KIMGAME]
GO
/****** Object:  StoredProcedure [dbo].[usp_pass]    Script Date: 10/29/2018 15:52:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[usp_LoginGame]
	@UID NCHAR(10),
	@UPS NCHAR(10)
AS
BEGIN
	SET NOCOUNT ON;
	update KIMGAME.dbo.Table
	SET dbo.Table.LogIn = GETDATE()
	WHERE USERID = @UID
END
RETURN 0