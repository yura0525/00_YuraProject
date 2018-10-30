CREATE PROCEDURE [dbo].[usp_Logout_1]
   @UID NCHAR(10)
AS
BEGIN

   SET NOCOUNT ON;
   UPDATE TIME
   SET LOGOUT = GETDATE()
   WHERE USERID = @UID;
END