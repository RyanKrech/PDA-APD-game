@ECHO OFF
ECHO Compile in progress.....
gcc -o game_xsi0 game_xsi0.c

IF ERRORLEVEL 1 (
    ECHO Compilation failed. Here are the errors:
    ECHO.
    type errors.log
    ECHO.
) ELSE (
    ECHO Compilation done
    ECHO Start running
    game_xsi0
)

PAUSE
