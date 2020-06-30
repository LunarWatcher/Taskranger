# Known issues

## Newlines in the text breaks the table

... specifically, it disables the automatic word wrap, and causes weird behavior with the table. Support for newlines may be dropped entirely in a backwards compatible way.

ETA: None

## The table overflows if there's too much content

Happens because tabulate has no built-in size checks.

ETA: Before v1.0.0

## The program will break on devices where time_t is 32 bit in 2038
Wontfix. If you're still running 32 bit when the 2038 problem starts getting even closer, you should really update your hardware -- Taskranger breaking will be the tiniest problem
