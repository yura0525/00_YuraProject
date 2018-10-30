CREATE PROCEDURE [dbo].[usp_AccountCreate_1]
   @UID NCHAR(10),
   @UPS NCHAR(10)
AS
   SET NOCOUNT ON
   BEGIN TRY
      BEGIN TRAN
         insert into TIME(USERID, USERPS) values(@UID, @UPS);

         EXECUTE usp_Login @UID, @UPS;
      COMMIT TRAN      
      RETURN 0;
   END TRY

   BEGIN CATCH
      ROLLBACK -- TRAN ���н� �ѹ�
      DECLARE @ret NCHAR(10)
      SELECT @ret = USERID
      FROM TIME
      WHERE USERID = @UID

      --�̹� �����ϴ� ID -> 1
      IF(@ret <> N'') -- NULL�̾ƴϴ�.
         BEGIN
            RETURN 1;
         END
      RETURN 2;
   END CATCH