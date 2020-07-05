# Taskranger configuration

Configuration is generally split in two: environment variables (little used), and text config.

The environment variables are very rarely used, and will generally only be used when there's no other alternatives. If this doesn't make sense, look at the examples.

The text config on the other hand, is intended to be significantly more comprehensive, and aims to coverall the features. Eventually, this will also contain user defined attributes. This early, hwoever, the list of used config is very short.

## Preface: types and other labels

The various entries on both environment variables and types will usually contain a type. This section of this document covers the meanings of these, in case it's unclear what it actually means.

* **Path**: Directory path. DO NOT confuse with a file path. Variables expecting a path expect a path to a directory, not a file.
    Examples:
    * `/home/user/.taskranger`
    * `/media/username/drive/`
    * `D:/config`
* **File path**: Contains a path to a specific file.
    Examples:
    * `/home/user/file.txt`
    * `C:/data/file.txt`
* Numeric types:
    * `number`: stored as a double; supports integers and decimal numbers.
    * `ullong`: unsigned long long. Mainly used for IDs. Only supports positive numbers.
* **String**: Plain text. Nothing fancy, just text.
* **Map**: This isn't a type used for the field itself, but it's used to store subtypes. For an instance, in this:
    ```
    "mapKey": {
        "key": "value"
    }
    ```
    `mapKey` in the docs would be referred to as a map. This means it doesn't hold one or more values on its own, but holds subvalues.
* **Vector** or **list**: contains several values. This is always annotated with additional types. A `String list` for an instance means exactly what you'd expect, a list of strings. Lists in JSON are annotated with square brackets:
    ```
    "thisIsAStringList": ["one value", "two values", "three values", "and so on"]
    ```
* **Date format**: Not to be confused with a date. Date format is a format used to parse dates. For the syntax, [see this page](https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/classicu_1_1SimpleDateFormat.html#details). Also see [`dates`](#dates) for configuration and the Dates file for usage.



### The config file

The config file, regardless of location, is always called `.trconf`. By default, it tries locating this file in the home directory. The location of the file itself can be configured (see [`TASKRANGER_CONFIG_LOCATION`](#TASKRANGER_CONFIG_LOCATION)), but the file name is always constant.

The config file is in the JSON format, or:

```json
{
    "config": "value",
    "scopes": {
        "are": {
            "maps": "because it's pretty and works with formatting",
            "multivalues": "and helps deal with multiple values. This is especially important for UDAs"
        }
    }
}
```

Note that scoped variables are often referred to by a shorthand. In the example above, the `maps` key could be referred to as `scopes.are.maps`. Keep this in mind, as this notation is some times used to demonstrate certain config.

There's a lot more to JSON than what's covered here. If you're unfamiliar with JSON, here's some resources:

* https://www.w3schools.com/js/js_json_syntax.asp
* https://www.json.org/json-en.html
* https://en.wikipedia.org/wiki/JSON

Taskranger uses [nlohmann/json](https://github.com/nlohmann/json) as its JSON parser. As far as I know, it follows the JSON standard, so there shouldn't be any specific quirks to the syntax or anything like that. Plain 'ol JSON, basically.

## Environment variables

The currently used environment variables are:

### `TASKRANGER_CONFIG_LOCATION`
Type: path

This is a variable that you hopefully won't need. This specifies the directory of `.trconf`.

Examples (note: no specific operating system is used in these examples. Use common sense when setting the variable):
```sh
TASKRANGER_CONFIG_LOCATION=C:/Users/Username/
TASKRANGER_CONFIG_LOCATION=/media/username/backup/.taskranger
```

## Config variables

If you haven't already, read the section on the config file. It's important to understand the syntax to understand the config.

The currently used config variables are:

### `dataDir`
Type: path

This determines where the databases are stored. Examples:

```json
"dataDir": "~/.taskranger"
"dataDir": "/mnt/ExternalDrive/taskranger"
"dataDir": "D:/tasks/"
```

By switching between data directories, you can also split up tasks any way you want. Note that there currently isn't a way to specify where to store tasks on a per-command basis, but this will be implemented eventuallyBy switching between data directories, you can also split up tasks any way you want. Note that there currently isn't a way to specify where to store tasks on a per-command basis, but this will be implemented eventually.

### `uda`
Type: map

This map contains user defined attributes. [Development status: currently not entirely supported]

Every single user defined attribute _has_ to have a type. Without it, it's considered invalid. TODO: explain what types are available. And maybe get around to making a slightly better system?

Being a map, several attributes can be declared in the UDA. Here's a sample:
```json
{
    "uda": {
        "myAttribute": {
            "type": "string",
            "label": "MA"
        },
        "mySecondAttribute": {
            "type": "strlist"
        }
    }
}
```

UDAs additionally have several attributes on their own.

#### `type`
Type: string

This is self-explanatory. What you insert here defines what type the attribute is, and defines some internal sematics. That being said, you can leave a number as a string, but you'll lose two operators in the process.

Supported types:

* `string` - stores the value as text
* `number` - stores the value as a number (supports decimals and negative numbers)
* `ullong` - stores the value as an unsigned whole number
* `strlist` - stores multiple strings in a list
* `date` - stores the value as a timestamp

#### `label`
Type: string

The label defines what shows up in tables and other places where the label itself is used. It can be long, it can be short, it's really up to you.

#### `values`
type: array

Defines the allowed values for the attribute. For an instance:

```json
{
    "uda": {
        "difficulty": {
            "type": "string",
            "values": ["easy", "medium", "hard", "insane"]
        }
    }
}
```

... will only allow those four values in the attribute. That means:

```
taskranger add Some task difficulty:"invalid attribute value"
```

is not allowed.

### `dates`
Type: map

This map contains date formats, both custom ones and the one required for the program to function. All of its key-value pairs are date formats, and yes, mostly anything goes for the names:

```json
{
    "dates": {
        "default": "dd.MM.y HH:mm:ss",
        "klingon": "dd.MM",
        "asidethedefaultkeyalmostanythinggoes": "y",
        "idontusearchbtw": "L HH:mm:ss"
    }
}
```

```
taskranger add "What year is it?" due.asidethedefaultalmostanythinggoes:2020
```

The syntax used in the last command is covered in docs/Dates.md.

#### `default`
Type: Date format

This is used as the output format, as well as the format to use for parsing when none is specified. It's highly recommended that this date format contains the day, month, year, and time of day, as it'll also be used as the output format.
