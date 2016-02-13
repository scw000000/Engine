set origdir=%~dp0

set vardir=C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC

chdir /d %vardir%

call vcvarsall.bat

chdir /d %origdir%

devenv SDL\VisualC\SDL.sln /build Debug /project SDL2 /projectconfig "Debug|Win32"
devenv SDL\VisualC\SDL.sln /build Debug /project SDL2main /projectconfig "Debug|Win32"
devenv SDL\VisualC\SDL.sln /build Debug /project SDL2test /projectconfig "Debug|Win32"

devenv GLEW\build\vc12\glew.sln /build Debug /project glew_shared /projectconfig "Debug|Win32"
devenv GLEW\build\vc12\glew.sln /build Debug /project glew_static /projectconfig "Debug|Win32"


call copyalllibs_vs2013.bat

pause




