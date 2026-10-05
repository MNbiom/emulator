WIP (explanations in gui should be good enought mostly)
# MNPU2 emulator
This program simulates MNPU2 - 1.25Hz CPU built in minecraft https://youtu.be/GCmohTZukFM

ISA: https://docs.google.com/spreadsheets/d/1aOvMMqjjtbJwQ4dTB1HlWPE_uM2PamwpnOvkguVXVjU/edit?usp=sharing

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

## Usage

### Code editor
Here you input assembler code described in [ISA](https://docs.google.com/spreadsheets/d/1aOvMMqjjtbJwQ4dTB1HlWPE_uM2PamwpnOvkguVXVjU/edit?usp=sharing).
- Comments
  - Anything after ";" will be ignored.
    ```asm
    ; comment
    ADD R1 ; another comment
    ```
- Using labels:
  - To create label you type your label name with ":" at the end:
  ```asm
  label_name:
  ```
  - In code you use the label that you created, by just typing it as if it was value,
    also to get page of the label, you add "_page" at the end of label name, so:
    ```asm
    SWP
    label_name_page
    label_name
    ```
- Writing to another page
  To start writing in next page you have write "]next_page]". After this line, you will start writing from the begining of the next page.
  ```asm
  ; page 0
  ADD R1
  SWP
  1
  0
  
  ]next_page]
  
  ; page 1
  AST R2
  ```
- Defining constants
  (spaces matter)
  ```asm
  $ const_name = 123
  ```
