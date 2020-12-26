# Contributing to Taskranger

## Code of Conduct

This project is govern by a [Code of Conduct](https://github.com/LunarWatcher/Taskranger/blob/master/.github/CODE_OF_CONDUCT.md). By participating, you're expected to uphold this Code of Conduct.

## I have a question!

For the time being, questions should be directed to [GitHub discussions][2]. [The issue tracker][1] can also be used, but it's preferred to use GitHub discussions. Issues should primarily be reserved for bugs, or questions where there's a _very high_ probability it's triggered by a bug.

Use the question template to get started if you pick issues. Either way, don't forget to include all relevant information. If you're having trouble with a specific feature, it's usually a good idea to try to reproduce it just from the details in your question - if you can't reproduce it with the steps you have in your issue/discussion, how are others meant to? A Discord server may be set up in the future, but for now, use GitHub discussions.

## I found the issue templates to be unclear

If any part of the issue template you used feels weird or is unclear, feel free to explain that at the end of your issue. The issue templates are a work in progress, and will be improved whenever necessary.

## I want to submit a pull request

[Pull requests](http://help.github.com/pull-requests/) are always welcome! Note that there are a couple things that must be included:

* A description of the changes.
* If applicable: issues it fixes or works towards fixing.

Additionally, code must follow the project code style. Running the [pre-commit hooks](https://pre-commit.com/) are highly recommended before pushing. Note that the code will be tested for compliance when pushed. To get started:

```
pip3 install -r requirements.txt
pre-commit install
```

Pre-commit will also clean up common problems when you try to commit - you'll have to re-add files and commit again if the hooks error out.

## Code style

**Naming conventions:**
* Variable and function names are camelCase.
* Class names and namespaces used to emulate a class-like scope must start with an upper-case letter.

**Formatting and general style:**
* Spaces, not tabs.
* Aside those listed here, [.clang-format](https://github.com/LunarWatcher/Taskranger/blob/master/.clang-format) defines general style conventions that it can fix.

[1]: https://github.com/LunarWatcher/Taskranger/issues
[2]: https://github.com/LunarWatcher/Taskranger/discussions
