Just me going through ["Crafting Interpreters"](https://craftinginterpreters.com/contents.html).
The code is pretty much the exact same as the one in the book except for some
naming conventions (`snake_case` VS `camelCase` and some other stuff).

# Setup

Write some simple Lox file like this one:


```lox
class Pair {}

fun get_pair(x, y) {
    var pair = Pair();
    pair.first = 1;
    pair.second = 2;

    return pair;
}

var p = get_pair(1, 2);
var result = p.first + p.second;
print result;
```

Then compile the binary and run it:

```bash
make && ./bin/lox file.lox
```

Should print 3 to standard output.

# Debugging

There are some flags that `make` accepts:

- `DEBUG_PRINT_CODE`: Prints the dissassembly of the Lox file.
- `DEBUG_TRACE_EXECUTION`: Prints each instruction and state of stack at runtime.
- `DEBUG_STRESS_GC`: Triggers the garbage collector on every single allocation.
- `DEBUG_LOG_GC`: Prints garbage collector execution.

Use flags like this:

```bash
make clean && make DEBUG_TRACE_EXECUTION=1 DEBUG_PRINT_CODE=1 && ./bin/lox example.lox
```

After that you should see the instructions twice, once after compilation and
then again at runtime along with the state of the stack.
