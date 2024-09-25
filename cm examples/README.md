<html><body>
<div align="center">
<h1>I'm going to take you to a quick tour on the Color Mapping DSL</h1>
</div>
<span>Purposes:</span>
<ul>
<li>Defining custom color schemes for terminal emulators & CLIs</li>
<li>Currently used as a style changer for this very project</li>
</ul>
<div align="center">
<h1>Syntax</h1>
</div>
<span>In the file <a href="/example1.cm">example1.cm</a> the first line is being this:</span>
<code>Define custom color mapping 1.0;</code>
<span>which is the CM's entry point.</span><br>
<span>On the third line and so on, you can see some <em>random numbers</em>. Let me clarify:</span>
<ul>

* First argument (<code>\<int\></code>) is a value, ranging from 1 to 8 in this example (could be higher or lower).
* Second argument (<code>\<string\></code>) is a sequence of numbers separated by semicolons, that correspond to an ANSI escape code.
* * Second argument splits into another 3:
* * * Foreground (<code>\<int\></code>),
* * * Background (<code>\<int\></code>),
* * * Text style (<code>\<int\></code>).<br>
</ul>
<span>There are also comments out here (also there's a bug where you can only put comments on empty lines):

```
// skipped
3 34;43;1 // parsed
// ...
```
</div><hr>
<div align="center"><p>Learn more: https://en.wikipedia.org/wiki/ANSI_escape_code#Colors</p></div>
</body></html>