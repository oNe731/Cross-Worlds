

//// xcopy /�ɼ� .������������ �ִ� ���			.����������



xcopy /y/s .\Engine\Public\*.*				.\Reference\Headers\

xcopy /y .\Engine\Bin\Debug\				.\Reference\Librarys\Debug\
xcopy /y .\Engine\Bin\Release\				.\Reference\Librarys\Release\

xcopy /y .\Engine\ThirdPartyLib\Debug\*.dll					.\Client\Bin\Debug\
xcopy /y .\Engine\ThirdPartyLib\Release\*.dll					.\Client\Bin\Release\

xcopy /y .\Engine\Bin\Debug\*.dll					.\Client\Bin\Debug\
xcopy /y .\Engine\Bin\Release\*.dll					.\Client\Bin\Release\


xcopy /y .\Engine\ThirdPartyLib\Debug\*.*			.\Reference\Librarys\Debug\
xcopy /y .\Engine\ThirdPartyLib\Release\*.*			.\Reference\Librarys\Release\







xcopy /y .\Engine\Bin\ShaderFiles\*.*		.\Client\Bin\ShaderFiles\

