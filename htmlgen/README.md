# Introduction
`htmlgen` writes a html file based on a given text file.

# Build
Executing `make` from this directory will build `htmlgen`, the executable.

# Usage
Executing `make gen name=input.txt` will search the `input.txt` from the `../raw` directory and will produce a `input.html` to the `../generated` directory.

# Mechanism
Basically, this program keeps writing `<div>...</div>` to the output file, until the end of the input file.
## Generator Directive
The way this program interprets the input file depends on *generator directives* which appear in the input file. Currently there are four in them:
- `gd::pre` specifies the beginning of a `pre` tag, namely `<pre>`.
- `gd::endpre` specifies the end of the `pre` tag, namely `</pre>`.
- `gd::h1` specifies the next two lines are of `<h1>`.
- `gd::h2` specifies the next two lines are of `<h2>`.

## Explanation
In the below is explained how the program works.

First, the program gets a line from `argv[1]`. If this line is empty, then discarded; the program goes to the next line.

If the program have not opened a `<div>` yet, the program sees whether this line is a *pre* directive.

If so, the program writes `<div class="code-block">` and keeps writing the lines into the `<div>` until the program reads a line which is a *endpre* directive. If the program reads the *endpre* directive, the program writes `</div>`.

Otherwise, the program writes `<div class="plain-text">` and sees what kind of line it is. If this line is a *h1* directive, the program discards the current line and writes the next two lines into this `<div>` as `<h1>line</h1>`. If the line is a *h2* direvtive, the programs behaves in the similar way as the case of the *h1* directive. If the line is neither a *h1* nor *h2* directive, the program writes the current line and the next line as `<p>line</p>` into this `<div>`. After the program has written two lines inside this `<div>`, the program closes it, namely with `</div>`.

## Example
The following is an example of input.txt and output.html.

```
// input.txt               // output.html

gd::h2                     <div class="plain-text">
example sentence 1            <h2>example sentence 1</h2>
example sentence 2            <h2>example sentence 2</h2>
                           </div>
example sentence 3         <div class="plain-text">
example sentence 4            <p>example sentence 3</p>
                              <p>example sentence 4</p>
                           </div>
gd::pre                    <div class="code-block">
example sentence 5            <pre>
example sentence 6         example sentence 5
example sentence 7         example sentence 6
gd::endpre                 example sentence 7</pre>
                           </div>
```