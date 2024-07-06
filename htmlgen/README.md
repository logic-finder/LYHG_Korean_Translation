`htmlgen` writes a html file, given a text file. This program requires 3 command line arguments to run.

- argv[1] = the path to a text file; this is the input.
- argv[2] = the directory under which the output .html file will be saved.
- argv[3] = the name of the output file.

Until the end of the input file, the programs keeps writing `<div>...</div>` to the output file. The behavior of the program depends on *generator directives*. In the below is explained how the program works.

First, the program gets a line from argv[1].

If the program have not opened a `<div>` yet, the program sees whether this line is a *pre* directive.

If so, the program writes `<div class="code-block">` and keeps writing the lines into the `<div>` until the program reads a line which is a *endpre* directive. If the program reads the *endpre* directive, the program writes `</div>`.

Otherwise, the program writes `<div class="plain-text">` and sees what kind of line it is. If this line is a *h1* directive, the program discards the current line and writes the next two lines into the `<div>` as `<h1>line</h1>`. If the line is a *h2* direvtive, the programs behaves in the similar way with the case of the *h1* directive. If the line is neither a *h1* nor *h2* directive, the program writes the current line and the next line as `<p>line</p>` into the `<div>`. After the program has written two lines inside the `<div>`, the program closes it, namely `</div>`.

These directives are expressed in the text file in the following form: `gd::directive-name`. For example: `gd::pre`.

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