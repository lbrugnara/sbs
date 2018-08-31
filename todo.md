[ ] include statement to allow splitting the build.sbs file into smaller files
[ ] Improve error handling in the parser
[ ] Add dependency resolvers (detect .h)
[ ] sbs init command (scaffolding)
[ ] sbs config command: In order to create an specific .sbs/.config file with specific variables/configurations for a particular user. The idea is to have a configuration block that makes the particular user inputs required variables
[ ] Validate SbsFile object once it is completely parsed
    [ ] Check that all the requested objects exist
    [ ] Check for and extends clauses to make sure they are valid
[ ] Defining and using variables (at least textual replacements)
[ ] Passing arguments to actions and using positional variables for it ${1}, ${2}, ... ${n} where ${n} is equals to ${argc}. ${*} is a wildcard for all the arguments