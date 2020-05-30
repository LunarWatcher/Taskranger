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

### The config file

The config file, regardless of location, is always called `.trconf`. By default, it tries locating this file in the home directory. The location of the file itself can be configured (see [`TASKRANGER_CONFIG_LOCATION`](#TASKRANGER_CONFIG_LOCATION)), but the file name is always constant.

The syntax is very simple:

```
# Commented lines start with a # sign
# Aside comments, this file can contain variable assignment:
key=value
# Due to implementation practices, there cannot be a space between the key or value and equal sign.
# The key syntax allows for dots in the names themselves:
some.have.scopes=/home
```

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
dataDir=~/.taskranger
dataDir=/mnt/ExternalDrive/taskranger
dataDir=D:/tasks/
```

By switching between data directories, you can also split up tasks any way you want. Note that there currently isn't a way to specify where to store tasks on a per-command basis, but this will be implemented eventuallyBy switching between data directories, you can also split up tasks any way you want. Note that there currently isn't a way to specify where to store tasks on a per-command basis, but this will be implemented eventually
