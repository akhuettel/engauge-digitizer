# Engauge On Windows: A-Z Guide

This guide is for Windows users who want to either:
download link:
https://www.fftw.org/download.html

1. run a prebuilt Engauge package
2. create a portable package from a local build
3. build Engauge from source with Qt 6 and Visual Studio 2022

The steps below match the current repository state and the successful local build produced in:

- `bin/Engauge.exe`

Working screenshot for this Windows patch set:

![Software screen](../images/SoftwareScreen.png)

## 1. Short Answer

No, `Engauge.exe` alone is not enough.

On Windows, a working portable Engauge package must include:

- `Engauge.exe`
- Qt runtime DLLs
- `fftw3.dll`
- `platforms/qwindows.dll`
- `sqldrivers/qsqlite.dll`

If you want the built-in Help window to work, you also need:

- `documentation/engauge.qch`
- `documentation/engauge.qhc`

If you want translations to work, you also need:

- `translations/` with the `.qm` files

So the correct answer is:

- you can move the built application anywhere
- but you must move the whole packaged folder, not only the `.exe`

## 2. Recommended Folder Layout For End Users

For the current Windows build, a portable package should look like this:

```text
Engauge/
  Engauge.exe
  fftw3.dll
  Qt6Core.dll
  Qt6Gui.dll
  Qt6Help.dll
  Qt6PrintSupport.dll
  Qt6Sql.dll
  Qt6Widgets.dll
  Qt6Xml.dll
  platforms/
    qwindows.dll
  sqldrivers/
    qsqlite.dll
  imageformats/
    qgif.dll
    qico.dll
    qjpeg.dll
    qsvg.dll
  documentation/
    engauge.qch
    engauge.qhc
  translations/
    *.qm
```

For the build done in this repo, the minimum runtime DLLs currently copied into `bin/` are:

- `fftw3.dll`
- `Qt6Core.dll`
- `Qt6Gui.dll`
- `Qt6Help.dll`
- `Qt6PrintSupport.dll`
- `Qt6Sql.dll`
- `Qt6Widgets.dll`
- `Qt6Xml.dll`

## 3. If You Only Want To Use Engauge

If someone gives you a prepared Windows package:

1. Extract the package to any folder, for example `C:\Apps\Engauge`
2. Open that folder
3. Run `Engauge.exe`

Notes:

- Do not separate `Engauge.exe` from its DLLs and plugin folders
- Keep `platforms/qwindows.dll` beside the app folder structure
- If Help opens empty, the `documentation/engauge.qhc` file is missing
- If some UI text is not translated, the `translations/` folder is missing

## 4. Building From Source On Windows

These instructions are for developers.

### 4.1 Required Tools

Install:

- Visual Studio 2022 with MSVC x64 C++ tools
- Qt 6 `msvc2022_64`
- `qmake`
- `nmake`
- FFTW 3 for MSVC x64

Confirmed working layout for FFTW:

```text
C:\vcpkg\installed\x64-windows\
  include\
    fftw3.h
  lib\
    fftw3.lib
  bin\
    fftw3.dll
```

This repo now accepts that layout directly on Windows.

### 4.2 Verified Tool Paths

Example paths used successfully:

- `C:\Qt\6.11.0\msvc2022_64\bin\qmake.exe`
- `C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat`

### 4.3 Open The Correct Build Shell

Use a Visual Studio x64 developer environment.

Example:

```bat
call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
```

### 4.4 Set FFTW Path

Use a forward-slash path for qmake on Windows:

```bat
set FFTW_HOME=C:/vcpkg/installed/x64-windows
```

### 4.5 Generate Makefiles

From the repository root:

```bat
"C:\Qt\6.11.0\msvc2022_64\bin\qmake.exe" -cache qmake-msvc.cache engauge.pro CONFIG+=log4cpp_null
```

### 4.6 Build

```bat
nmake
```

Successful output should produce:

- `bin/Engauge.exe`

## 5. Creating A Portable Package After Build

After the build finishes, do not distribute only `bin/Engauge.exe`.

This repo now includes a packaging script:

- `dev\windows\package_portable_msvc2022.bat`

### 5.1 Recommended Command

Run it from the repository root after `nmake` succeeds:

```bat
dev\windows\package_portable_msvc2022.bat
```

Or choose a custom output folder:

```bat
dev\windows\package_portable_msvc2022.bat D:\Release\Engauge
```

### 5.2 What The Script Produces

By default it creates:

- `dist\Engauge\`
- `dist\Engauge-portable-Le-Xuan-Thang.zip`

The zip is the file you can send to end users.

### 5.3 What The Script Includes

The script packages:

- `Engauge.exe`
- `fftw3.dll`
- Qt runtime DLLs
- `platforms\qwindows.dll`
- `sqldrivers\qsqlite.dll`
- image format plugins
- `documentation\engauge.qch`
- `documentation\engauge.qhc`
- compiled translations `translations\*.qm`
- required MSVC runtime DLLs

### 5.4 Manual Packaging Fallback

If you do not want to use the script, you must manually create the same layout and copy the same files.

The application looks for help relative to `applicationDirPath()`, so `documentation\engauge.qhc` and the plugin folders must stay beside the executable in the expected structure.

## 6. Known Packaging Rules

For the current Windows configuration:

- `Engauge.exe` is portable as part of a folder package
- `Engauge.exe` is not standalone
- Help depends on `documentation/engauge.qhc`
- Translation loading depends on `translations/`
- SQLite support depends on `sqldrivers/qsqlite.dll`

## 7. Current Repo-Specific Notes

The current repo contains local Windows build adjustments:

- FFTW on MSVC accepts `fftw3.lib` from `C:/vcpkg/installed/x64-windows`
- build intermediates are written to `moc_build/`, `objs_build/`, and `rcc_build/`
- `qmake-msvc.cache` is used to work around a local `qmake` compiler-macro detection issue on this machine

If you build on another Windows machine and `qmake` works normally, you may not need the same workaround.

## 8. Recommended Distribution Strategy

If you want to share the app with non-technical Windows users, distribute one of these:

1. a zip file containing the full portable package folder
2. an installer that places the same files in a single application directory

Do not distribute:

1. only `Engauge.exe`
2. only `Engauge.exe` plus `fftw3.dll`
3. a package that omits `platforms/qwindows.dll`

## 9. Quick Checklist

Before sending the app to someone else, verify the package contains:

- `Engauge.exe`
- `fftw3.dll`
- required `Qt6*.dll` files
- `platforms/qwindows.dll`
- `sqldrivers/qsqlite.dll`
- `documentation/engauge.qhc`
- `documentation/engauge.qch`
- optional `translations/`

If all of that is present, the folder can be copied to another Windows machine and run from any location.
