This project mostly follows <https://keepachangelog.com/en/1.0.0/>, and uses [semantic versioning](https://semver.org/). (All dates are dd.mm.yyyy. Developer details in comments in this file)

<!--
This file syntax bases itself on https://keepachangelog.com/en/1.0.0/

There's a couple deviations from this standard, one of which is the dates. Dates are in the format of dd.mm.yyyy, because I said so. :D

Additionally, because meta, the version headers are H1s, while subheaders are H2. (The standard sets versions at H2 and subheaders at H3, but since I'm not gonna bother with an H1 for `# Changelog`, I'm just gonna decrease the level).

Finally, there's an additional header for grouping changes: Meta. This one covers meta changes that don't affect the project itself (like CI, changelogs, and changes to various GitHub related stuff)

This file currently cannot be automated. See #12: https://github.com/LunarWatcher/Taskranger/pull/12
Generation tools either rely on issues and PRs, which breaks with my format, or commit messages, which breaks because not all commits actually make a change worth recording. Ideas for better tools are welcome, but until then, this will be handled manually.
-->

# [Unreleased] (TBD)

# [v0.1.0] (19.06.20)

## Added
* UUID filtering
* User defined attributes
* Type restriction
* Value restriction (optional; mainly intended for UDAs)
* Add `number` as a UDA type (double under the hood)
* Operators `>=` (greatereq) and `<=` (lesseq) - names may be changed in the future (feels clunky)
* Additional testing capacity (Config, AddCommand, and UDAs are now tested)

## Fixes
* Attributes now store as the proper type
* Fix UTF-8 (theoretically; multi-byte chars are likely still broken. This is a future TODO)
* Add names to attributes (some of them were missing)
    * Add DescriptionAttribute as a separate class; lack of subclass caused missing attributes
* Fix bad logic in the `>` and `<` operators (the operators technically did the opposite of their names earlier)
* Enable `ids.operator` and `id.operator`
* Minor rewrite to the filtering system to enable multi-value filters (i.e. `ids:3,4,8,42` or `uda:3,4,621`) for built-in filters and user-defined attributes.

## Removed
* Support for wchars (redundant feature; caused more problems than good); required for the UTF-8 fix

## Changes
* `Task::loadUserDefinedAttribs()` removed and replaced with dynamic loading. Not that it was used anyway, but the API has changed
* (Internal) change the reverse iteration in DoneCommand to use a `reverse_iterator` instead of a for loop with a long long to avoid unsigned number horrors
* `TASKRANGER_CONFIG_LOCATION` can now be any path or file, as long as it ends with `.trconf`. `/home/user/MyTaskrangerConfig.trconf` is now a valid file. Mainly intended for tests

## Meta
* License section to the version command, as well as a THIRD_PARTY_LICENSES.md file containing all licenses.
* Unbreak the CI on Windows (again)

# [v0.0.3] - 07.06.2020

## Added
* Config (JSON)
* Groundwork on fixed attributes
* Internal: merge data class singletons into Environment.hpp(/.cpp)
* Better syntax-based tag searching (`+tags` searches for tags, `-tags` excludes tags)
* Better error handling
    * Some migration over to this sytem (TL;DR: throw caught strings instead of printing and aborting)
* Support for wchars

## Fixes
* Link dataDir to JSONDatabase (previously unlinked; meant dataDir had no use)

## Changes
* Switch to secure environment variables where possible

## Meta
* Add changelog

Note: v0.0.3 is the first version to have a changelog written to fit a specific system. Earlier versions won't have the same standard.

# [v0.0.2] - 28.05.2020

## Added
* Case-insensitive filters.
* Narrowing down what attributes are valid, and what aren't.
* Filter operators. These are: is (default when none specified), not, contains, greater, less. More might come later - note that not all have uses at the moment.
* A help command.
* A command to see all tasks, including completed.
* Standard: bug fixes and edge case patches, as well as increased testing.

# v0.0.1

Hello world!

Base release. Unknown release date, because the Git tag was lost to unexpected aerodynamic forces.


<!-- Links -->
[Unreleased]: //github.com/LunarWatcher/Taskranger/compare/v0.1.0...HEAD
[v0.1.0]: //github.com/LunarWatcher/Taskranger/compare/v0.0.3...v0.0.1
[v0.0.3]: //github.com/LunarWatcher/Taskranger/compare/v0.0.2...v0.0.3
[v0.0.2]: //github.com/LunarWatcher/Taskranger/releases/tag/v0.0.2
