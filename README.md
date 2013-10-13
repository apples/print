# README

`print()` is a fully type-safe, templated version of `printf()`.
It supports printing to any stream,
or even any object that supports insertion (`<<`).

## Why?

Traditionally, `printf()` does not do any kind of type checking,
and it uses C-style variadic functions.
Modern compilers support `printf()` as an intrinsic,
so they can do safety checks on the given arguments.
However, this only works for static format strings,
and cannot work on format strings that are, for example,
read from a file during runtime.
Also, `printf()` can only support basic datatypes,
and cannot be overloaded for user-defined types.

## How?

Using `print()` is very simple.
The basic version is defined as follows:

```C++
template <typename... Ts>
void print (char const*, Ts&&...);
```

The first argument is the format string,
which currently must be a `char const*`.
The remaining arguments are either items to be printed or format parameters.

The format string's specification is practically identical to `printf()`.
For example:

```C++
print ("Integer: %$; Float: %$;\n", 7, 7.5);
```

The `%$` is the field specifier.
It is analogous to `%d` or `%f` when using `printf()`,
except it automatically detects the proper datatype.

`print()` even supports most of the format flags used by `printf()`.

```C++
print ("Width   10: %10$;\n", 7);
print ("Width   ??: %*$;\n", 10, 7);
print ("Left-align: %-10$;\n", 7);
```

The complete field format specification is as follows:

    %[flags][width][.[pflags]precision]$

`flags` can be any combination of the following:

| Flag | Effect
|:-----|:--
| `-`  | Left-align
| `+`  | Show `+` for positive numbers.
| `0`  | Pad the field with zeroes instead of spaces.

`width` can either be a number to set as the field width,
or it can be `*` to use the current parameter as the width.

`pflags` affect floating-point numbers, and can be the following:

| Flag | Effect
|:-----|:--
| `e`  | Use scientific notation
| `f`  | Use fixed decimal point

`precision` works just like `width`,
but instead sets the precision for floating-point numbers.
If the `f` flag is used, `precision` will be the number of digits
after the decimal point.

## Using Streams

By default, `print()` will print to `std::cout`.
However, there is a second form of `print()`:

```C++
template <typename S, typename... Ts>
void print (S&&, char const*, Ts&&...);
```

It's the same as the first version,
except the first parameter is the stream to use.
The given stream does not have to be derived from `std::ostream`,
but `print()` uses stream sytax (`<<` and manipulators),
so be aware of possible incompatabilities.

## License

Print is licensed under the unmodified zlib/libpng liscense. Full text can be
found [here](http://opensource.org/licenses/zlib-license.php).

For those unfamiliar, this basically means that you can do whatever you want
with the source code as long as you don't claim ownership or remove the
license.

## Contact

Jeramy Harrison

Email: `dbralir@gmail.com`
