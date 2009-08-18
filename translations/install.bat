@echo off
rem  ***************************************************************************
rem  *   This file is part of the eXaro project                                *
rem  *   Copyright (C) Mohamed Tahar BADI                                      *
rem  *   mtbadi39@yahoo.fr / mtbadi39@gmail.com                                *
rem  **                   GNU General Public License Usage                    **
rem  *                                                                         *
rem  *   This library is free software: you can redistribute it and/or modify  *
rem  *   it under the terms of the GNU General Public License as published by  *
rem  *   the Free Software Foundation, either version 3 of the License, or     *
rem  *   (at your option) any later version.                                   *
rem  *   You should have received a copy of the GNU General Public License     *
rem  *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
rem  *                                                                         *
rem  **                  GNU Lesser General Public License                    **
rem  *                                                                         *
rem  *   This library is free software: you can redistribute it and/or modify  *
rem  *   it under the terms of the GNU Lesser General Public License as        * 
rem  *   published by the Free Software Foundation, either version 3 of the    *
rem  *   License, or (at your option) any later version.                       *
rem  *   You should have received a copy of the GNU Lesser General Public      *
rem  *   License along with this library.                                      * 
rem  *   If not, see <http://www.gnu.org/licenses/>.                           *
rem  *                                                                         *
rem  *   This library is distributed in the hope that it will be useful,       *
rem  *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
rem  *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
rem  *   GNU General Public License for more details.                          *
rem  ***************************************************************************

rem
rem -- This file produce or update exaro compiled translations (*.qm) files 
rem    on Windows Systems.
rem -- Qt lrelease must been on your PATH
rem -- update lines 10 and 11 to be like your eXaro installation !
rem

rem if not exist lrelease goto lrelease_not_exist
for %%f in (*.ts) do lrelease %%f
if exist C:\Progra~1\exaro-package for %%f in (*.qm) do copy /Y %%f  C:\Progra~1\exaro-package\bin\translations
if exist C:\Progra~1\exaro for %%f in (*.qm) do copy /Y %%f  C:\Progra~1\exaro\bin\translations
goto end
:lrelease_not_exist
echo.
echo ERROR :
echo -------
echo     1. [Qt lrelease] must on your PATH !
echo     2. This batch file must be in [/translations] directory.
echo.
:end
pause