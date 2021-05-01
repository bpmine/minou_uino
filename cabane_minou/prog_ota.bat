SET PT="C:\Users\Utilisateur\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\2.7.4\tools"
SET IP="192.168.1.44"
REM SET IP="192.168.1.4"
SET OUT="C:\Users\UTILIS~1\AppData\Local\Temp\arduino_build_727388\cabane_minou.ino.bin"
SET HOST="192.168.1.48"

python "%PT%\espota.py" -i %IP% -p 8266 --auth 123 -f %OUT% -r -d

pause