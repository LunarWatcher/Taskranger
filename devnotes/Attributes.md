# Devnotes: attributes

## Internal vs external

One major challenge with the attribute and display system as it currently stands, is that there's no way to differentiate between attributes meant for internal use, and attributes meant for the user's eyes.

There's a few workarounds, and there's definitely better ways. Option one, the one being implemented as I write this, is to use a SinkAttribute, and exclude all SinkAttributes from being listed. This is somewhat suboptimal.

Option 2 is filtering out those attributes, and keeping them internally. This makes it challenging due to the way the database is built.

Option 3 is keeping track of which attributes we ignore, and silently skipping these.

Both 2 and 3 require some code coping (as in several files using the same bit of code to filter out stuff), and don't provide input handling.

Need to get back to the design of this in general at a later time.
