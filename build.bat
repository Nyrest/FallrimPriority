@ REM Check MSBuild available in Path
@ WHERE MSBuild >nul 2>nul
@ IF %ERRORLEVEL% NEQ 0 goto MSBuildNotFound

@ REM MSBuild is Available
@ REM Build the solution
:Build
@ MSBuild /p:Configuration="Release" /p:Platform="x86" /m
@ IF %ERRORLEVEL% NEQ 0 goto BuildError
@ MSBuild /p:Configuration="Release" /p:Platform="x64" /m
@ IF %ERRORLEVEL% NEQ 0 goto BuildError
@ exit /b

@ REM ========================
@ REM Build Error
@ REM Throw an exception
:BuildError
@ echo Fatal Build Error 1>&2
@ echo Aborting... 1>&2
@ exit /b 1

@ REM ========================
@ REM MSBuild is not Available
@ REM Throw an exception
:MSBuildNotFound
@ echo MSBuild is not found. 1>&2
@ echo Make sure it's installed and added to path. 1>&2
@ exit /b 1
