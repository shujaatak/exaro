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
rem -- This file produce or update eXaro translations files on Windows Systems.
rem -- Add your translation file exaro_xx_YY.ts
rem			xx : Language id
rem			YY : Country id
rem -- Qt lupdate must on your PATH
rem -- use Qt Linguist to translate your exaro_xx_YY.ts file
rem


rem if not exist lupdate goto lupdate_not_exist
lupdate -no-obsolete -recursive -locations relative .. -ts exaro_it_IT.ts exaro_ro_RO.ts exaro_ru_RU.ts exaro_ru_UA.ts exaro_ar_DZ.ts exaro_fr_FR.ts
goto end
:lupdate_not_exist
echo.
echo ERROR :
echo -------
echo     1. [Qt lupdate] must on your PATH !
echo     2. This batch file must be in [/translations] directory.
echo.
:end
pause