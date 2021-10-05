# Tree-sitter + AFL = ???

Experimental harness hooking up [`afl-fuzz`](https://lcamtuf.coredump.cx/afl/) to tree-sitter grammars.

## Running

1. install `stack`
2. make sure `../tree-sitter/libtree-sitter.a` is present
3. `stack install shake`
4. `stack exec -- shake`
5. `mkdir findings`
6. `afl-fuzz -i tests -o findings _build/run @@`
