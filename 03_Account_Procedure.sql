USE [AccountDB]
GO


SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- 유저 정보를 가져옵니다.
-- =============================================
CREATE PROCEDURE [dbo].[usp_Account_Select]
    @LoginID varchar(250)
AS
BEGIN
    SET NOCOUNT ON;

    SELECT [UID], LoginID, [Password]
        FROM dbo.Account(NOLOCK) WHERE LoginID = @LoginID;
    RETURN 0
END

SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- 유저 정보를 생성합니다.
-- =============================================
CREATE PROCEDURE [dbo].[usp_Account_Create]
    @LoginID varchar(250),
    @Password varchar(250)
AS
BEGIN
    SET NOCOUNT ON;

    BEGIN TRY
        BEGIN TRAN
            DECLARE @UID int
            INSERT INTO dbo.Account(LoginID, [Password])
                VALUES(@LoginID, @Password)
                SELECT @UID = @@IDENTITY;
        COMMIT TRAN
    END TRY
    BEGIN CATCH
        ROLLBACK TRAN
        -- 2627 Primary key 같은 Unique 제약 위반 오류, 2601은 Unique index 중복 오류
        IF ERROR_NUMBER() IN (2601, 2627) BEGIN
            RETURN 1 -- 이미 존재하는 계정
        END ELSE BEGIN
            THROW
        END
    END CATCH

    exec usp_Account_Select @LoginID
    RETURN 0
END
GO


SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- 유저 정보를 삭제합니다.
-- =============================================
CREATE PROCEDURE [dbo].[usp_Account_Delete]
    @UID int
AS
BEGIN
    SET NOCOUNT ON;

    DELETE dbo.Account WHERE UID = @UID;
    IF (@@ROWCOUNT = 0) BEGIN
        RETURN 1 -- 존재하지 않는 유저
    END

    RETURN 0
END
GO
