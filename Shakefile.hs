{-# LANGUAGE BlockArguments #-}

import Development.Shake
import Development.Shake.Command
import Development.Shake.FilePath
import Development.Shake.Util

main :: IO ()
main = shakeArgs shakeOptions {shakeFiles = "_build"} do
  want ["_build/run" <.> exe]

  phony "clean" do
    putInfo "Removing files in _build"
    removeFilesAfter "_build" ["//*"]

  "_build/run" <.> exe %> \out -> do
    cs <- getDirectoryFiles "" ["*.c", "vendor/tree-sitter-json/src/*.c"]
    let os = ["_build" </> c -<.> "o" | c <- cs]
    need os
    cmd_ "afl-clang -o" [out] "-I ../tree-sitter/lib/include ../tree-sitter/libtree-sitter.a" os

  "_build//*.o" %> \out -> do
    let c = dropDirectory1 $ out -<.> "c"
    let m = out -<.> "m"
    cmd_ "afl-clang -c" [c] "-o" [out] "-MMD -MF" [m]
    neededMakefileDependencies m
