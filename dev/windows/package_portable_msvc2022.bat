@echo off
setlocal EnableExtensions

rem Package an already-built Windows/MSVC Engauge into a portable folder and zip.
rem Usage:
rem   dev\windows\package_portable_msvc2022.bat
rem   dev\windows\package_portable_msvc2022.bat D:\out\Engauge

for %%I in ("%~dp0..\..") do set "REPO_ROOT=%%~fI"
set "DEFAULT_PACKAGE_DIR=%REPO_ROOT%\dist\Engauge"
set "PACKAGE_DIR=%DEFAULT_PACKAGE_DIR%"
if not "%~1"=="" set "PACKAGE_DIR=%~f1"

set "DIST_ROOT=%PACKAGE_DIR%"
for %%I in ("%PACKAGE_DIR%\..") do set "DIST_PARENT=%%~fI"
set "ZIP_FILE=%DIST_PARENT%\Engauge-portable-Le-Xuan-Thang.zip"
set "HELP_WORK=%DIST_PARENT%\help_build"

if not exist "%REPO_ROOT%\bin\Engauge.exe" (
  echo ERROR: "%REPO_ROOT%\bin\Engauge.exe" does not exist.
  echo Build the application first, then run this script again.
  exit /b 1
)

set "QTDIR="
for /f "delims=" %%A in ('qmake -query QT_INSTALL_PREFIX 2^>NUL') do set "QTDIR=%%A"
if not defined QTDIR (
  for /f "delims=" %%A in ('"C:\Qt\6.11.0\msvc2022_64\bin\qmake.exe" -query QT_INSTALL_PREFIX 2^>NUL') do set "QTDIR=%%A"
)
if not defined QTDIR (
  echo ERROR: Unable to determine QTDIR from qmake.
  echo Make sure Qt 6 MSVC qmake is on PATH or update this script.
  exit /b 1
)

set "FFTW_BIN=%FFTW_HOME%\bin\fftw3.dll"
if not exist "%FFTW_BIN%" set "FFTW_BIN=C:\vcpkg\installed\x64-windows\bin\fftw3.dll"
if not exist "%FFTW_BIN%" (
  echo ERROR: fftw3.dll not found.
  echo Checked:
  echo   %FFTW_HOME%\bin\fftw3.dll
  echo   C:\vcpkg\installed\x64-windows\bin\fftw3.dll
  exit /b 1
)

echo Packaging to "%PACKAGE_DIR%"

if exist "%PACKAGE_DIR%" rmdir /s /q "%PACKAGE_DIR%"
if exist "%HELP_WORK%" rmdir /s /q "%HELP_WORK%"
mkdir "%PACKAGE_DIR%"
mkdir "%PACKAGE_DIR%\platforms"
mkdir "%PACKAGE_DIR%\imageformats"
mkdir "%PACKAGE_DIR%\sqldrivers"
mkdir "%PACKAGE_DIR%\documentation"
mkdir "%PACKAGE_DIR%\translations"

copy /y "%REPO_ROOT%\bin\Engauge.exe" "%PACKAGE_DIR%\" >NUL || exit /b 1
copy /y "%REPO_ROOT%\LICENSE" "%PACKAGE_DIR%\" >NUL || exit /b 1
copy /y "%FFTW_BIN%" "%PACKAGE_DIR%\fftw3.dll" >NUL || exit /b 1

for %%F in (
  Qt6Core.dll
  Qt6Gui.dll
  Qt6Help.dll
  Qt6PrintSupport.dll
  Qt6Sql.dll
  Qt6Svg.dll
  Qt6Widgets.dll
  Qt6Xml.dll
) do (
  if exist "%QTDIR%\bin\%%F" (
    copy /y "%QTDIR%\bin\%%F" "%PACKAGE_DIR%\" >NUL || exit /b 1
  )
)

copy /y "%QTDIR%\plugins\platforms\qwindows.dll" "%PACKAGE_DIR%\platforms\" >NUL || exit /b 1
copy /y "%QTDIR%\plugins\sqldrivers\qsqlite.dll" "%PACKAGE_DIR%\sqldrivers\" >NUL || exit /b 1

for %%F in (
  qgif.dll
  qico.dll
  qjpeg.dll
  qsvg.dll
) do (
  if exist "%QTDIR%\plugins\imageformats\%%F" (
    copy /y "%QTDIR%\plugins\imageformats\%%F" "%PACKAGE_DIR%\imageformats\" >NUL || exit /b 1
  )
)

for %%F in (
  concrt140.dll
  msvcp140.dll
  msvcp140_1.dll
  msvcp140_2.dll
  vcruntime140.dll
  vcruntime140_1.dll
) do (
  if exist "%SystemRoot%\System32\%%F" (
    copy /y "%SystemRoot%\System32\%%F" "%PACKAGE_DIR%\" >NUL || exit /b 1
  )
)

mkdir "%HELP_WORK%"
xcopy "%REPO_ROOT%\help\*" "%HELP_WORK%\" /E /I /Y >NUL || exit /b 1
pushd "%HELP_WORK%"
"%QTDIR%\bin\qhelpgenerator.exe" engauge.qhp -o engauge.qch || exit /b 1
"%QTDIR%\bin\qhelpgenerator.exe" engauge.qhcp -o engauge.qhc || exit /b 1
copy /y "engauge.qch" "%PACKAGE_DIR%\documentation\" >NUL || exit /b 1
copy /y "engauge.qhc" "%PACKAGE_DIR%\documentation\" >NUL || exit /b 1
popd

for %%F in ("%REPO_ROOT%\translations\engauge_*.ts") do (
  "%QTDIR%\bin\lrelease.exe" "%%~fF" -qm "%PACKAGE_DIR%\translations\%%~nF.qm" >NUL || exit /b 1
)

>"%PACKAGE_DIR%\README-portable.txt" (
  echo Engauge Portable
  echo Maintained build by Le Xuan Thang
  echo.
  echo Run Engauge.exe from this folder.
  echo Do not separate Engauge.exe from the DLLs and plugin folders.
  echo.
  echo Included:
  echo - Qt runtime DLLs
  echo - FFTW runtime DLL
  echo - platforms\qwindows.dll
  echo - sqldrivers\qsqlite.dll
  echo - imageformats plugins
  echo - documentation\engauge.qch and engauge.qhc
  echo - translations\*.qm
)

if exist "%ZIP_FILE%" del /q "%ZIP_FILE%"
tar.exe -a -cf "%ZIP_FILE%" -C "%DIST_PARENT%" "Engauge" || exit /b 1

echo.
echo Portable package created:
echo   Folder: "%PACKAGE_DIR%"
echo   Zip:    "%ZIP_FILE%"
echo.
echo You can distribute the zip or the whole "%PACKAGE_DIR%" folder.
exit /b 0
