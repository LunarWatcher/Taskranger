# Date and time

Dates can be extremely tricky for a program like this. Some dates are relative, some dates are absolute. Unfortunately, there's no easy way to deal with this. Using multiple formats isn't an option, because if they're iterated while they're being tested, Taskranger could accidentally parse a date as a time of day. That really doesn't work.

Just specifying one time format is horrible. Doing this would involve manually overriding it each time you want a different date, and that can get ugly fast. Typing out the entire timezone format every time, especially given some implementation details, is not feasible either. As a result, there's a few systems in place to preserve your sanity.

Before continuing to the two major date systems, there's another thing to keep in mind; when the date lacks certain components, Taskranger will automatically infer these based on the current time, or just relative to midnight.

If minutes, or seconds are missing, it'll be set to 0. If the hours are missing, it'll be set to 23:59. If the year is missing, for obvious reasons, Taskranger will assume you mean the current year. If you don't specify a day, it'll assume you mean today. If you don't specify a month, it'll assume you mean the current month. Note that for relative times, these are more commonly inferred. `<N>days` will assume by 23:59 in 2 days. It can work out the rest, being a relative date, but certain assumptions have to be made.

## Custom date formats

Under the hood, Taskranger runs a library, [initially written by Howard Hinnant](https://github.com/HowardHinnant/date/tree/v3.0.0), to deal with dates. This library has since made it into the C++ standard (starting with C++20), but Taskranger will continue to use the standalone library until using C++20 is a viable option.

Boring tech details aside, the point is that all the formatting specifiers can be found [here](https://en.cppreference.com/w/cpp/chrono/parse). If you're planning on using custom date formats, that list is going to be useful.

This is where Taskranger itself comes in; writing out date formats isn't a good option, it's not always a good idea using relative dates, and making assumptions will backfire. Taskranger instead lets you specify date constants in your config, and use those with the date attribute.

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
        "default": "%d.%m.%Y %H:%M:%S",
        "tod": "%H:%M:%S",
        "day": "%d.%m",
        "date": "%d.%m &H:%M"
    }
}
```

### Output format

There can only be one output format defined. This is mainly because there's not a need for anything else.

While date formats can be configured in any way you want, there's one key that's the exception; `default`.

`dates.default` is both the one used for output, as well as input when no specific format has been specified.

## Relative time

In addition to using date formats, it's also possible to use relative dates. Note that these will take priority over date formats whereever possible.

The general idea is that you'll be able to write `dateAttribute:2days`, and Taskranger will automagically calculate the date.
