# Filtering tasks

Some commands take a filter as an optional input. A filter is exactly what you'd expect - it searches the tasks for tasks matching a query. The query itself follows a specific syntax, which is what this document covers.

## Syntax

### General filtering syntax
The syntax is fairly straight-forward, and tries to follow the general pattern of the input:

```
taskranger next attributeToFilter:valuesToFilter
```

The system can filter any and all defined attributes, which means built-in and user-defined attributes that are still defined. If the UDA was added, used to make a task, and then removed, it can't be filtered. It won't be removed from the task, hwoever, to avoid data loss.

Filtering a single value is allowed:

```
taskranger next attribute:arguments,are,comma,separated
```

Under the hood, this is equivalent to using `attribute.is`. `.is` indicates an operator. This is a topic I'll be getting back to in a little bit. The point, however, is that it looks for exact matches on the attribute. Given:

```
taskranger next ids:1,2,6,3
```

... you'll get task 1, 2, 3, and 6. For most attributes, this is the basic syntax. Keep in mind that additional filters are equivalent to an and operation: if you define a filter for two attributes (say IDs and project), you'll only get a subset of the original tasks. To put it like this, assuming there's at least 6 tasks and all the IDs resolve, this:

```
taskranger next ids:1,2,3,6
```
will return 4 tasks with the IDs listed in the command. However:

```
taskranger next ids:1,2,3,6 project:@MyProject
```

will only return the tasks that have the ID 1, 2, 3, or 6, but also is assigned to the project `@MyProject`. It will not return 4 tasks along with any tasks that happen to use the @MyProject project, which would be an or operation instead of an and. <!-- unclear? -->

### Filtering projects and tags

Tags and projects can follow the standard syntax (see [general filtering syntax](#general-filtering-syntax)), but they each have a second variant that's legal.

Tags can be filtered with `+tag` or `-tag`. `+tag` is equivalent to `tags:+tag`, where as `-tag` is equivalent to `tags.not:+tag` - again, we'll get back to operators in a bit, but it means excluding tags.

Here's an example:
```
taskranger next +MyTag -MyOtherTag
```

This will include tasks with the `+MyTag` tag, provided they don't also have the `+MyOtherTag` tag.

Projects only have one operator out of the box: `@Project`. Using:
```
taskranger next @MyProject
```
... you'll get all the tasks in the `@MyProject` project. To exclude, with the not operator, the general filtering syntax has to be used:
```
taskranger next project.not:@MyProject
```

## Filtering operators

Operators is where the power of filters presents itself. There's a few operators, but before I list these, keep in mind that not all types are supported. Different attributes have different types, and by extension, different support for operators.

The syntax is fairly straight-forward:

```
taskranger next attribute.operator:values
```

The function itself is identical to the general filtering syntax; the only difference is the `.operator`, which defines how the values are treated. By default, when no operator is specified, Taskranger assumes you mean `is`. The

The list of supported operators follows a specific syntax: `name` (binary equivalent, if applicable): supported in types

The name is what you'd type after the `.` to specify which operator you're using.

* `is` (==): number, ullong, string, strlist (note: acts as `contains` with strlist for technical reasons)
* `not` (!=): number, ullong, string, strlist (note: acts as `not contains` with strlist for technical reasons)
* `greater` (&gt;): number, ullong
*   Less excludes the first item. `ids.greater:1` will include 2, 3, 4, and higher, but excludes 1. If you want to capture 1, use `greatereq`.
* `less` (&lt;): number, ullong
    Less excludes the first item. `ids.less:5` will include 1, 2, 3, and 4, but not 5. If you want to capture 5, use `lesseq`.
* `greatereq` (&gt;=): number, ullong
* `lesseq` (&lt;=): number, ullong
* `contains`: string (note: case-insensitive)
    Looks for one or more provided substrings within the attribute

Again, note that `is` is implicit, and doesn't need to be explicitly typed. Additionally, the binary operators themselves aren't supported for use. This may become an option in a later version of Taskranger.
