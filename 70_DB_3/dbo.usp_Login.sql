CREATE PROCEDURE [dbo].[usp_Login]
   @ID NCHAR(10),
   @PS NCHAR(10)
AS
   UPDATE [dbo].[TIME]
   SET LOGIN = GETDATE()
   WHERE USERID = @ID;
RETURN 0