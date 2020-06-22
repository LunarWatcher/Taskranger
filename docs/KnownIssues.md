# Known issues

## Newlines in the text breaks the table

... specifically, it disables the automatic word wrap, and causes weird behavior with the table. Support for newlines may be dropped entirely in a backwards compatible way.

ETA: None

## The table looks weird if the terminal isn't wide enough

This happens because no checks are done to make sure the table is narrow enough to fit inside the terminal.

This is tracked, and a fix will be implemented whenever I figure out some issues related to column size.

ETA: Soon
