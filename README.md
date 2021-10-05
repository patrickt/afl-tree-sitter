# Tree-sitter + AFL = ???

Experimental harness hooking up [`afl-fuzz`](https://lcamtuf.coredump.cx/afl/) to tree-sitter grammars.

## Running

1. `git submodule update --init`
2. install `stack`
3. make sure `../tree-sitter/libtree-sitter.a` is present
4. `stack install shake`
5. `stack exec -- shake`
6. `mkdir findings`
7. `afl-fuzz -i tests -o findings -x dict/json.dict _build/run @@`
