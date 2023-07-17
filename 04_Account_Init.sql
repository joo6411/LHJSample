-- ============================================================================
-- DB를 처음 설정한 후 각종 초기 설정을 위해 실행하는 쿼리입니다.
-- ============================================================================

USE [AccountDB]

declare @i int;
set @i = 0;

while @i < 100
begin
	set  @i = @i + 1;
	declare @id varchar(20);
	set @id = 'test' + RIGHT('000' + CAST(@i as varchar(3)), 3);
	insert dbo.Account values(@id, @id);
end


GO