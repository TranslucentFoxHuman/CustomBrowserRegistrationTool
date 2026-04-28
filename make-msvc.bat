@ECHO OFF
REM Build script for CBRT

if "%1"=="clean" (
    del *.exe *.obj *.res
    exit /b
)

rc /fo resources.res resources.rc
cl /O2 cbrt.c resources.res advapi32.lib /MD