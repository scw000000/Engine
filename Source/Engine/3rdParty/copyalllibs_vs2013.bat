md ..\..\Lib\Win32Debug
md ..\..\Lib\Win32Release
md ..\..\Lib\Win32Profile
md ..\..\Lib\x64Debug
md ..\..\Lib\x64Release
md ..\..\Lib\x64Profile

echo f | xcopy /D /y DXUT11\Core\Bin\Desktop_2013\Win32\Debug\dxut.lib ..\..\Lib\Win32Debug\dxut.lib
echo f | xcopy /D /y DXUT11\Core\Bin\Desktop_2013\Win32\Debug\dxut.pdb ..\..\Lib\Win32Debug\dxut.pdb
echo f | xcopy /D /y DXUT11\Core\Bin\Desktop_2013\Win32\Release\dxut.lib ..\..\Lib\Win32Release\dxut.lib
echo f | xcopy /D /y DXUT11\Core\Bin\Desktop_2013\Win32\Release\dxut.pdb ..\..\Lib\Win32Release\dxut.pdb
echo f | xcopy /D /y DXUT11\Core\Bin\Desktop_2013\Win32\Profile\dxut.lib ..\..\Lib\Win32Profile\dxut.lib
echo f | xcopy /D /y DXUT11\Core\Bin\Desktop_2013\Win32\Profile\dxut.pdb ..\..\Lib\Win32Profile\dxut.pdb

echo f | xcopy /D /y DXUT11\Optional\Bin\Desktop_2013\Win32\Debug\dxutopt.lib ..\..\Lib\Win32Debug\dxutopt.lib
echo f | xcopy /D /y DXUT11\Optional\Bin\Desktop_2013\Win32\Debug\dxutopt.pdb ..\..\Lib\Win32Debug\dxutopt.pdb
echo f | xcopy /D /y DXUT11\Optional\Bin\Desktop_2013\Win32\Release\dxutopt.lib ..\..\Lib\Win32Release\dxutopt.lib
echo f | xcopy /D /y DXUT11\Optional\Bin\Desktop_2013\Win32\Release\dxutopt.pdb ..\..\Lib\Win32Release\dxutopt.pdb
echo f | xcopy /D /y DXUT11\Optional\Bin\Desktop_2013\Win32\Profile\dxutopt.lib ..\..\Lib\Win32Profile\dxutopt.lib
echo f | xcopy /D /y DXUT11\Optional\Bin\Desktop_2013\Win32\Profile\dxutopt.pdb ..\..\Lib\Win32Profile\dxutopt.pdb

echo f | xcopy /D /y DXUT11\Core\Bin\Desktop_2013\x64\Debug\dxut.lib ..\..\Lib\x64Debug\dxut.lib
echo f | xcopy /D /y DXUT11\Core\Bin\Desktop_2013\x64\Debug\dxut.pdb ..\..\Lib\x64Debug\dxut.pdb
echo f | xcopy /D /y DXUT11\Core\Bin\Desktop_2013\x64\Release\dxut.lib ..\..\Lib\x64Release\dxut.lib
echo f | xcopy /D /y DXUT11\Core\Bin\Desktop_2013\x64\Release\dxut.pdb ..\..\Lib\x64Release\dxut.pdb
echo f | xcopy /D /y DXUT11\Core\Bin\Desktop_2013\x64\Profile\dxut.lib ..\..\Lib\x64Profile\dxut.lib
echo f | xcopy /D /y DXUT11\Core\Bin\Desktop_2013\x64\Profile\dxut.pdb ..\..\Lib\x64Profile\dxut.pdb

echo f | xcopy /D /y DXUT11\Optional\Bin\Desktop_2013\x64\Debug\dxutopt.lib ..\..\Lib\x64Debug\dxutopt.lib
echo f | xcopy /D /y DXUT11\Optional\Bin\Desktop_2013\x64\Debug\dxutopt.pdb ..\..\Lib\x64Debug\dxutopt.pdb
echo f | xcopy /D /y DXUT11\Optional\Bin\Desktop_2013\x64\Release\dxutopt.lib ..\..\Lib\x64Release\dxutopt.lib
echo f | xcopy /D /y DXUT11\Optional\Bin\Desktop_2013\x64\Release\dxutopt.pdb ..\..\Lib\x64Release\dxutopt.pdb
echo f | xcopy /D /y DXUT11\Optional\Bin\Desktop_2013\x64\Profile\dxutopt.lib ..\..\Lib\x64Profile\dxutopt.lib
echo f | xcopy /D /y DXUT11\Optional\Bin\Desktop_2013\x64\Profile\dxutopt.pdb ..\..\Lib\x64Profile\dxutopt.pdb


