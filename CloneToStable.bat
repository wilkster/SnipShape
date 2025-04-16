@echo off
cd ..\SnipShapeStable
:: 1st archive what is there into a new zip
set ver=%DATE:~10,4%%DATE:~4,2%%DATE:~7,2%
del zip\%ver%-SnipShape.zip
zip -r -u zip\%ver%-SnipShape.zip *.ahk2
zip -r -u zip\%ver%-SnipShape.zip lib\*.ahk2
zip -r -u zip\%ver%-SnipShape.zip lib\shapes\*.ahk2
:: Copy the files over now
cd ..\SnipShape
xcopy *.ahk2 ..\SnipShapeStable\ /S /Y /D
xcopy *.cur ..\SnipShapeStable\ /S /Y /D
xcopy *.ico ..\SnipShapeStable\ /S /Y /D
xcopy *.png ..\SnipShapeStable\ /S /Y /D
timeout 3