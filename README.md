# MNPU2 emulator

## Building
To build, first make sure that you have installed in MSYS2 UCRT64 following things:
```shell
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
pacman -S --needed mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
pacman -Syu
pacman -Su
```
Once you have these installed, create build folder:
```shell
git clone https://github.com/MNbiom/emulator.git
cd emulator
md build
```
Then open project folder in Visual Studio Code and press ctrl+shift+B to build debug executable (build/debug/main.exe).
To debug press F5.
To build release executable (build/release/main.exe), run "cmake build release" task.
