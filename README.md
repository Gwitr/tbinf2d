## Description
This is a 2D tile-based engine that ~~supports~~ will support (somewhat) infinite worlds. It also ~~has~~ will have a real-time editor and multiplayer support.

For now it just renders a few thousand sprites.

## Building and running
### Under Windows:
```
python build.py
```
The output file is named "main.exe".

### Under Linux:
```
python3 build.py
```
The output file is named "main".

## Missing files (Windows-only)
| Missing file/folder | Description |
| --- | --- |
| ./sdl2/include | contains the SDL2 and SDL2_image headers |
| ./sdl2/lib | contains the SDL2 .a files |
| ./SDL2.dll | SDL2 dll |
| ./SDL2_image.dll | SDL2_image dll |
| ./zlib1.dll | zlib1 dll, usually bundled with SDL2_image |
| ./libpng16-16.dll | libpng dll |
