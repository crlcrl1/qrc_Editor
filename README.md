# qrc Editor

## Why

I want to edit Qt resource file in an external editor rather than QtCreator, so I write this application.

## How to Build

You can either open the `CMakeLists.txt` in your IDE such as QtCreator, or use commandline tools to
build it:
``` shell
cmake -DCMAKE_PREFIX_PATH=/qt/install/path/youQtVersion/compilerYouWantToUse <Other CMake Options>
```

## How To Use

Since it is a GUI application, so it's easy to use. It can be also used as an external tool for IDEs.

Here is an example to use it in Neovim:
```lua
vim.cmd [[
    autocmd BufReadPre *.qrc silent execute '!/path/to/qrc_Editor.exe "%"'
]]

vim.cmd [[
    autocmd BufReadPost *.qrc bw!
]]
```

## License

MIT
