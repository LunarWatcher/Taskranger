# Attributes

Attributes are one of the core parts of Taskranger. Attributes are exactly what you'd think - they're pieces of data associated with the task. As can be seen in Config.md (TODO: link), there's also ways to add your own.

Taskranger comes with a bunch of attributes built-in.

As a note about this file, this is mainly intended as a reference. It only lists the attributes and how their use, if applicable, and not necessarily _how_ they can be used. This is planned as an addition to documentation later:tm:.

## Sink attributes (`tasktype`, `init`)

| Parameter | Value |
| --- | --- |
| Modifiable | No |


Sink attributes are attributes meant purely for internal use. If you're able to modify them with any part of taskranger, that's a bug.

These help provide bits of data meant for Taskranger only. `tasktype` helps to tell apart normal tasks and recurrence templates, and `init` helps make sure your recurring tasks actually generate a task when they're initially created, instead of doing nothing until the first due date is passed. `tasktype` also has a secondary function in hiding the recurrence templates where they're not necessary.

Additional sink attributes may be added in the future as needed. It's highly recommended that you don't modify sink attributes, as this may result in Taskranger breaking. The name "sink" here is meant to be a reference to how any modification of these attributes do nothing - input is silently discarded to prevent modifications on what Taskranger decides to insert.

## Recurrence (`recur`)

| Parameter | Value |
| --- | --- |
| Modifiable | Yes |
| Input type | Relative date |
| Requires | `due` |


Note: the input type will probably be changed in the future by making all dates appear as relative.

The name should explain what it does. It makes a task repeat itself. Internally, the logic creates a template task that then spawns new tasks when the due date has been passed. This is also why a due date is required.

## Due (`due`)

| Parameter  | Value |
| ---------- | ----- |
| Modifiable | Yes   |
| Input type | Date  |

Defines when the task is due. In the future, this will have an impact on urgency and possibly points (depending on settings), but until then, it'll only have a meaning for you as the consumer.
