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

## Added
* UUID filtering
* User defined attributes
* Type restriction
* Value restriction (optional; mainly intended for UDAs)

## Fixes
* Attributes now store as the proper type
* Fix UTF-8 (theoretically; multi-byte chars are likely still broken. This is a future TODO)
* Add names to attributes (some of them were missing)
    * Add DescriptionAttribute as a separate class; lack of subclass caused missing attributes

## Changes
* `Task::loadUserDefinedAttribs()` removed and replaced with dynamic loading. Not that it was used anyway, but the API has changed
* (Internal) change the reverse iteration in DoneCommand to use a `reverse_iterator` instead of a for loop with a long long to avoid unsigned number horrors

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
[Unreleased]: //github.com/LunarWatcher/Taskranger/compare/v0.0.3...HEAD
[v0.0.3]: //github.com/LunarWatcher/Taskranger/compare/v0.0.2...v0.0.3
[v0.0.2]: //github.com/LunarWatcher/Taskranger/releases/tag/v0.0.2
