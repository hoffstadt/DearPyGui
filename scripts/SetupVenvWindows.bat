cd /d "%~dp0\.."
if not exist venv py -3.12 -m venv venv
if %errorlevel% neq 0 exit /b %errorlevel%
call venv\Scripts\activate.bat
if %errorlevel% neq 0 exit /b %errorlevel%
py -m pip install --upgrade pip