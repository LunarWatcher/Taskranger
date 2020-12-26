# Date and time

Dates can be extremely tricky for a program like this. Some dates are relative, some dates are absolute. Unfortunately, there's no easy way to deal with this. Using multiple formats isn't an option, because if they're iterated while they're being tested, Taskranger could accidentally parse a date as a time of day. That really doesn't work.

Just specifying one time format is horrible. Doing this would involve manually overriding it each time you want a different date, and that can get ugly fast. Typing out the entire timezone format every time, especially given some implementation details, is not feasible either. As a result, there's a few systems in place to preserve your sanity.

Before continuing to the two major date systems, there's another thing to keep in mind; when the date lacks certain components, Taskranger will automatically infer these based on the current time, or just relative to midnight.

If minutes, or seconds are missing, it'll be set to 0. If the hours are missing, it'll be set to 23:59. If the year is missing, for obvious reasons, Taskranger will assume you mean the current year. If you don't specify a day, it'll assume you mean today. If you don't specify a month, it'll assume you mean the current month. Note that for relative times, these are more commonly inferred. `<N>days` will assume by 23:59 in 2 days. It can work out the rest, being a relative date, but certain assumptions have to be made.

## Custom date formats

Under the hood, Taskranger uses [ICU](https://github.com/unicode-org/icu) to deal with dates.

Boring tech details aside, the point is that all the formatting specifiers can be found [here](https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/classicu_1_1SimpleDateFormat.html#details). If you're planning on using custom date formats, that list is going to be useful.

This is where Taskranger itself comes in; writing out date formats isn't a good option, it's not always a good idea using relative dates, and making assumptions will backfire. Let's assume we brute-force date formats, and accidentally parse `03.05.07` as a date instead of a time stamp. Different users prefer different time formats, and this may lead to ambiguity that pretty much makes brute-forcing it undesirable. Taskranger instead lets you specify date constants in your config, and use those with the date attribute.

The usage is simple:

```
taskranger add This is an example task with a date field specified dateAttribute.dateFormat:22.06.2020
```

The key here is `dateAttribute.dateFormat`:

`dateAttribute` refers to any attribute that's a date attribute. Aside your user-defined attributes, `due` and `recur` are two examples.

`dateFormat` is the interesting one -- this can be anything as long as you define it in your `.trconf`. Specifically, everything after that `.`, up to the end of the attribute name or a new `.` (this restriction is to prevent operators from breaking) will be looked up in the `dates` key. More on the details behind this can be found in the config documentation.

The point here is that you can also name your custom formats whatever you want. They can also overwrite default date formats.

That being said, this:

```
taskranger add Example due:"22.06.2020 22:14:26"
```

will look for a date format named `default`. If you don't specify a default date format, it'll use a format the developer picked based on her own preferences -- it's also fully customizable, so you're free to change it if you don't like it. **TL;DR:** for output, or for input when no other format is specified, it'll look for a key in the dates map called "default" to use.

### Default date formats

```json
{
    "dates",  {
        "default": "dd.MM.y HH:mm:ss",
        "zoned": "dd.MM.y HH:mm:ss'T'z",
        "tod": "HH:mm:ss",
        "day": "dd.MM",
        "date": "dd.MM HH:mm"
    }
}
```

Quick list in case you  didn't read the documentation:

`dd` is the day. `MM` is the month. `y`, is the year. `HH` is the hours, `mm` is the minutes (NOTE: do not confuse `mm` for `MM`. They represent two very different values. Yes, it is confusing). `'T'` means the letter `T`, literally. `z` means the timezone (i.e. `+0200`).

### Output format

There can only be one output format defined. This is mainly because there's not a need for anything else. Note that the output format only affects the absolute time; the relative time in commands like `info` are not affected by the default output format.

While date formats can be configured in any way you want, there's one key that's the exception; `default`.

`dates.default` is both the one used for output, as well as input when no specific format has been specified.

## Relative time

In addition to using date formats, it's also possible to use relative dates. Note that these will take priority over date formats whereever possible, because design choices.

Basically:

```
dateattribute:number<unit without brackets>
```

Where number is the number of the unit. the supported units are:

| Unit | Meaning |
| ---- | ------- |
| d    | Day     |
| m    | Month   |
| y    | Year    |
| w    | Week    |
| h    | Hour    |
