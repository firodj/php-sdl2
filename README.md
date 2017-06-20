## Compile SDL2 PHP on Windows:

### Build SDL2 Library

Open `C:\SDL2\VisualC\SDL.sln` using **Microsoft Visual Studio**
then Build Relase x64.

### Build Extension

Please read resource:

* http://blog.benoitblanchon.fr/build-php-extension-on-windows/
* https://wiki.php.net/internals/windows/stepbystepbuild

Step:

1. Install **Visual Studio 2015**.
2. Open **VS2015 x64 Native Tools Command Prompt**.
3. The PHP binary tools SDK is located in `C:\php-sdk`
4. Create build directory once (skip if already exists):

   ```
   $ cd c:\php-sdk\
   $ bin\phpsdk_buildtree.bat phpdev
   ```

   Copy `C:\php-sdk\phpdev\vc9` to `C:\php-sdk\phpdev\vc14`.

5. The PHP source code is located in `C:\php-sdk\phpdev\vc14\x64\php-7.1.4-src`
6. Put libraries on which PHP depends in `C:\php-sdk\phpdev\vc14\x64\deps`
6. Clone this repo in `C:\php-exts\php-sdl2`
7. Setup environment:

   ```
   $ cd c:\php-sdk\
   $ bin\phpsdk_setvars.bat
   ```

7. Add the path of headers, libraries, and runtime into `INCLUDE`, `LIB`, and `PATH` environment:

   ```
   $ set INCLUDE=%INCLUDE%;C:\SDL2\include
   $ set LIB=%LIB%;C:\SDL2\VisualC\x64\Release
   $ set PATH=%PATH%;C:\SDL2\VisualC\x64\Release
   ```

7. Build config as **phpize**:

   ```
   $ cd C:\php-sdk\phpdev\vc14\x64\php-7.1.4-src
   $ buildconf.bat --add-modules-dir=C:\php-exts
   ```

   You should see the output like:
   ```
   Rebuilding configure.js
   Adding C:\php-exts to the module search path
   ```

8. Configure:

   ```
   $ configure.bat --disable-all --enable-cli --enable-sdl2
   ```

   You should see the output like:
   ```
   Enabled extensions:
   -----------------------
   | Extension  | Mode   |
   -----------------------
   | sdl2       | shared |
   ```

9. Make:

   ```
   $ nmake php_sdl2.dll
   ```

The generated file will be in `.\x64\Relase_TS` for Win64.

### Testing:

```
$ cd C:\php-sdk\phpdev\vc14\x64\php-7.1.4-src
$ echo extension_dir=C:\php-sdk\phpdev\vc14\x64\php-7.1.4-src\x64\Release_TS >> x64\Release_TS\php.ini
$ echo extension=php_sdl2.dll >> x64\Release_TS\php.ini
$ nmake test TESTS=C:\php-exts\php-sdl2\tests
```

## Compile SDL2 PHP on Mac:

### Install SDL2 Library

```
$ brew install sdl2
```

### Build Extension

```
$ phpize --clean
$ phpize
$ ./configure
$ make
```

### Test:
```
$ make test
```

### Install:
```
$ make install
$ echo "extension=sdl2.so" > /usr/local/etc/php/7.1/conf.d/ext-sdl2.ini
$ brew services restart php71
```

