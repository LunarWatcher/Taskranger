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
* Numeric types<sup>1</sup>:
    * **Int(eger)**: whole numbers. Examples: 123, 42, 741
    * **Float**: decimal numbers. Examples: 3.14159, 21.42
    * **Unsigned**: Unsigned is a modifier combined with one of the two above. For an instance, a type can be an `unsigned int`. `Unsigned` means the number cannot be negative.
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

1: Unless otherwise noted, numbers are signed. This means they can be positive or negative. Unsigned numbers can only be positive. Integer types are usually stored as 64 bit longs, while floats are stored as doubles.

### The config file

The config file, regardless of location, is always called `.trconf`. By default, it tries locating this file in the home directory. The location of the file itself can be configured (see [`TASKRANGER_CONFIG_LOCATION`](#TASKRANGER_CONFIG_LOCATION)), but the file name is always constant.

The config file is in the JSON format, or:

```
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
```
TASKRANGER_CONFIG_LOCATION=C:/Users/Username/
TASKRANGER_CONFIG_LOCATION=/media/username/backup/.taskranger
```

## Config variables

If you haven't already, read the section on the config file. It's important to understand the syntax to understand the config.

The currently used config variables are:

### `dataDir`
Type: path

This determines where the databases are stored. Examples:

```
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
```
{
    "uda": {
        "myAttribute": {
            "type": "string",
            "label": "MA"
        },
        "mySecondAttribute": {
            "type": "strlist",
        }
    }
}
```

UDAs additionally have several attributes on their own.

#### `type`
Type: string

This is self-explanatory. What you insert here defines what type the attribute is, and defines some internal sematics. That being said, you can leave a number as a string, but you'll lose two operators in the process.

#### `label`
Type: string

The label defines what shows up in tables and other places where the label itself is used. It can be long, it can be short, it's really up to you.
