# CHIP-8 EMULATOR

This is a small project that aims to create a CHIP-8 emulator as an educational project in order to learn more about emulation.

## Side note:

If you run into diagnostic errors from your LSP when using this project, if you are using `clangd` make sure that you have copied over a `/build` folder in the root directory. This will provide context for the linker to the lsp and will provide information on where the headers files are located.

Inside the `/build` folder, should include a `compile_commands.json` file inside.

You can generate your own using `bear` by doing the following:

```
$ bear -- make
```

Which will generate the file, then you can simply `cp` it over to your `build/` folder.

```
$ cp compile_commands.json ./build/compile_commands.json
```
