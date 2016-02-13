md ..\..\Lib\Win32Debug
md ..\..\Lib\Win32Release
md ..\..\Lib\Win32Profile
md ..\..\Lib\x64Debug
md ..\..\Lib\x64Release
md ..\..\Lib\x64Profile

echo f | xcopy /D /y SDL\VisualC\Bin\SDL2\Desktop_2013\Win32\Debug\SDL2.dll ..\MSVC\SDL2.dll

echo f | xcopy /D /y SDL\VisualC\Bin\SDL2\Desktop_2013\Win32\Debug\SDL2.lib ..\..\Lib\Win32Debug\SDL2.lib
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2\Desktop_2013\Win32\Debug\SDL2.pdb ..\..\Lib\Win32Debug\SDL2.pdb
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2\Desktop_2013\Win32\Release\SDL2.lib ..\..\Lib\Win32Release\SDL2.lib
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2\Desktop_2013\Win32\Release\SDL2.pdb ..\..\Lib\Win32Release\SDL2.pdb
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2\Desktop_2013\Win32\Profile\SDL2.lib ..\..\Lib\Win32Profile\SDL2.lib
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2\Desktop_2013\Win32\Profile\SDL2.pdb ..\..\Lib\Win32Profile\SDL2.pdb

echo f | xcopy /D /y SDL\VisualC\Bin\SDL2main\Desktop_2013\Win32\Debug\SDL2main.lib ..\..\Lib\Win32Debug\SDL2main.lib
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2main\Desktop_2013\Win32\Debug\SDL2main.pdb ..\..\Lib\Win32Debug\SDL2main.pdb
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2main\Desktop_2013\Win32\Release\SDL2main.lib ..\..\Lib\Win32Release\SDL2main.lib
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2main\Desktop_2013\Win32\Release\SDL2main.pdb ..\..\Lib\Win32Release\SDL2main.pdb
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2main\Desktop_2013\Win32\Profile\SDL2main.lib ..\..\Lib\Win32Profile\SDL2main.lib
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2main\Desktop_2013\Win32\Profile\SDL2main.pdb ..\..\Lib\Win32Profile\SDL2main.pdb

echo f | xcopy /D /y SDL\VisualC\Bin\SDL2test\Desktop_2013\Win32\Debug\SDL2test.lib ..\..\Lib\Win32Debug\SDL2test.lib
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2test\Desktop_2013\Win32\Debug\SDL2test.pdb ..\..\Lib\Win32Debug\SDL2test.pdb
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2test\Desktop_2013\Win32\Release\SDL2test.lib ..\..\Lib\Win32Release\SDL2test.lib
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2test\Desktop_2013\Win32\Release\SDL2test.pdb ..\..\Lib\Win32Release\SDL2test.pdb
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2test\Desktop_2013\Win32\Profile\SDL2test.lib ..\..\Lib\Win32Profile\SDL2test.lib
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2test\Desktop_2013\Win32\Profile\SDL2test.pdb ..\..\Lib\Win32Profile\SDL2test.pdb

echo f | xcopy /D /y SDL\VisualC\Bin\SDL2\Desktop_2013\x64\Debug\SDL2.lib ..\..\Lib\x64Debug\SDL2.lib
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2\Desktop_2013\x64\Debug\SDL2.pdb ..\..\Lib\x64Debug\SDL2.pdb
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2\Desktop_2013\x64\Release\SDL2.lib ..\..\Lib\x64Release\SDL2.lib
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2\Desktop_2013\x64\Release\SDL2.pdb ..\..\Lib\x64Release\SDL2.pdb
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2\Desktop_2013\x64\Profile\SDL2.lib ..\..\Lib\x64Profile\SDL2.lib
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2\Desktop_2013\x64\Profile\SDL2.pdb ..\..\Lib\x64Profile\SDL2.pdb

echo f | xcopy /D /y SDL\VisualC\Bin\SDL2main\Desktop_2013\x64\Debug\SDL2main.lib ..\..\Lib\x64Debug\SDL2main.lib
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2main\Desktop_2013\x64\Debug\SDL2main.pdb ..\..\Lib\x64Debug\SDL2main.pdb
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2main\Desktop_2013\x64\Release\SDL2main.lib ..\..\Lib\x64Release\SDL2main.lib
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2main\Desktop_2013\x64\Release\SDL2main.pdb ..\..\Lib\x64Release\SDL2main.pdb
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2main\Desktop_2013\x64\Profile\SDL2main.lib ..\..\Lib\x64Profile\SDL2main.lib
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2main\Desktop_2013\x64\Profile\SDL2main.pdb ..\..\Lib\x64Profile\SDL2main.pdb

echo f | xcopy /D /y SDL\VisualC\Bin\SDL2test\Desktop_2013\x64\Debug\SDL2test.lib ..\..\Lib\x64Debug\SDL2test.lib
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2test\Desktop_2013\x64\Debug\SDL2test.pdb ..\..\Lib\x64Debug\SDL2test.pdb
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2test\Desktop_2013\x64\Release\SDL2test.lib ..\..\Lib\x64Release\SDL2test.lib
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2test\Desktop_2013\x64\Release\SDL2test.pdb ..\..\Lib\x64Release\SDL2test.pdb
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2test\Desktop_2013\x64\Profile\SDL2test.lib ..\..\Lib\x64Profile\SDL2test.lib
echo f | xcopy /D /y SDL\VisualC\Bin\SDL2test\Desktop_2013\x64\Profile\SDL2test.pdb ..\..\Lib\x64Profile\SDL2test.pdb




echo f | xcopy /D /y GLEW\bin\glew_shared\Desktop_2013\Win32\Debug\glew32d.dll ..\..\Lib\Win32Debug\glew32d.dll
echo f | xcopy /D /y GLEW\bin\glew_shared\Desktop_2013\Win32\Debug\glew32d.pdb ..\..\Lib\Win32Debug\glew32d.pdb
echo f | xcopy /D /y GLEW\bin\glew_shared\Desktop_2013\Win32\Release\glew32d.dll ..\..\Lib\Win32Release\glew32d.dll
echo f | xcopy /D /y GLEW\bin\glew_shared\Desktop_2013\Win32\Release\glew32d.pdb ..\..\Lib\Win32Release\glew32d.pdb
echo f | xcopy /D /y GLEW\bin\glew_shared\Desktop_2013\Win32\Profile\glew32d.dll ..\..\Lib\Win32Profile\glew32d.dll
echo f | xcopy /D /y GLEW\bin\glew_shared\Desktop_2013\Win32\Profile\glew32d.pdb ..\..\Lib\Win32Profile\glew32d.pdb

echo f | xcopy /D /y GLEW\bin\glew_static\Desktop_2013\Win32\Debug\glew32sd.lib ..\..\Lib\Win32Debug\glew32sd.lib
echo f | xcopy /D /y GLEW\bin\glew_static\Desktop_2013\Win32\Debug\glew32sd.pdb ..\..\Lib\Win32Debug\glew32sd.pdb
echo f | xcopy /D /y GLEW\bin\glew_static\Desktop_2013\Win32\Release\glew32sd.lib ..\..\Lib\Win32Release\glew32sd.lib
echo f | xcopy /D /y GLEW\bin\glew_static\Desktop_2013\Win32\Release\glew32sd.pdb ..\..\Lib\Win32Release\glew32sd.pdb
echo f | xcopy /D /y GLEW\bin\glew_static\Desktop_2013\Win32\Profile\glew32sd.lib ..\..\Lib\Win32Profile\glew32sd.lib
echo f | xcopy /D /y GLEW\bin\glew_static\Desktop_2013\Win32\Profile\glew32sd.pdb ..\..\Lib\Win32Profile\glew32sd.pdb


echo f | xcopy /D /y GLEW\bin\glew_shared\Desktop_2013\x64\Debug\glew32d.dll ..\..\Lib\x64Debug\glew32d.dll
echo f | xcopy /D /y GLEW\bin\glew_shared\Desktop_2013\x64\Debug\glew32d.pdb ..\..\Lib\x64Debug\glew32d.pdb
echo f | xcopy /D /y GLEW\bin\glew_shared\Desktop_2013\x64\Release\glew32d.dll ..\..\Lib\x64Release\glew32d.dll
echo f | xcopy /D /y GLEW\bin\glew_shared\Desktop_2013\x64\Release\glew32d.pdb ..\..\Lib\x64Release\glew32d.pdb
echo f | xcopy /D /y GLEW\bin\glew_shared\Desktop_2013\x64\Profile\glew32d.dll ..\..\Lib\x64Profile\glew32d.dll
echo f | xcopy /D /y GLEW\bin\glew_shared\Desktop_2013\x64\Profile\glew32d.pdb ..\..\Lib\x64Profile\glew32d.pdb

echo f | xcopy /D /y GLEW\bin\glew_static\Desktop_2013\x64\Debug\glew32sd.lib ..\..\Lib\x64Debug\glew32sd.lib
echo f | xcopy /D /y GLEW\bin\glew_static\Desktop_2013\x64\Debug\glew32sd.pdb ..\..\Lib\x64Debug\glew32sd.pdb
echo f | xcopy /D /y GLEW\bin\glew_static\Desktop_2013\x64\Release\glew32sd.lib ..\..\Lib\x64Release\glew32sd.lib
echo f | xcopy /D /y GLEW\bin\glew_static\Desktop_2013\x64\Release\glew32sd.pdb ..\..\Lib\x64Release\glew32sd.pdb
echo f | xcopy /D /y GLEW\bin\glew_static\Desktop_2013\x64\Profile\glew32sd.lib ..\..\Lib\x64Profile\glew32sd.lib
echo f | xcopy /D /y GLEW\bin\glew_static\Desktop_2013\x64\Profile\glew32sd.pdb ..\..\Lib\x64Profile\glew32sd.pdb
