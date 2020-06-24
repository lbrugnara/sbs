[ ] Check flags and defines to invalidate the build
[ ] Improve error handling throughout the project
[ ] sbs init command (scaffolding): Add basic elements and common toolchains
[ ] sbs setup command? A configuration block that requests input from the user to configure the local build
[ ] Validate SbsFile object once it is completely parsed
    [ ] Check that all the requested objects exist
    [ ] Check for and extends clauses to make sure they are valid
[ ] Defining and using variables. (We have some minor textual replacements, it would be great to ad a ${env:<name>} syntax to get environment variables)
[ ] Passing arguments to actions and using positional variables for it ${1}, ${2}, ... ${n} where ${n} is equals to ${argc}. ${*} is a wildcard for all the arguments
    [ ] Create some common actions implemented as C functions, like reading input from the user
[ ] Add "extends" support for targets