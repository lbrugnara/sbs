v0.2.0:
    [ ] Rename "for" keyword to "if". In the current context it makes (a lot) of sense to use the conditional
    [ ] Make "toolchain" an array to allow multiple toolset combinations
    [ ] Make "target" in presets an array object to run multiple targets at once
    [ ] sbs run command to run presets
    [ ] Change working directory
    [ ] Defining and using variables. 
        [x] Textual replacement 
        [ ] ${env:<name>} syntax to get environment variables
            [ ] Check that "system" executor and custom executors correctly set and retrieve ENV variables
    [ ] sbs init command (scaffolding): Add basic elements and common toolchains
        [ ] Init specific "identifier"? -e=my-env1,my-env2 --> my-env1{} my-env2{}
        [ ] Default win-cmd, linux-bash, osx-bash envs??
        [ ] Default clang, gcc, msvc toolchains?
    [x] Scripting mode
        [x] Basic support
    [x] Expression evaluator to run "for" declarations
    [x] Update "for" syntax to support "env(env1, env2)", "arch(arch1, arch2)"...
    [x] Use realpath for files...
    [x] Detect host OS and architecture (basic)

v0.2.x:
    [ ] String interpolation for variables and probably a ternary if or something like that
        Examples: 
            - "/path/to/libsomething.${sbs.env == "win-cmd" ? "lib" : "a"}" <-- The "win-cmd" must be a string because we don't have context for it
            - "/path/to/libsomething.${env(win-cmd) ? "lib" : "a"}" <-- win-cmd is expected to be an env (this one could work right away)
    [ ] Detect installed toolchains: search for the binaries using the executor?
    [ ] Add support for glob (currently we only support regexes)
        [ ] Prepend regexes with ~ to differentiate paths from it
    [ ] Make sure the objects are valid once they are resolved 
        Example: sbs build debug -s -tc=msvc -c=msvc-config -e=linux-bash --> The toolchaing 'msvc' does not exist (it is actually not valid for the requested environment)
    [ ] Passing arguments to actions and using positional variables for it ${1}, ${2}, ... ${n} where ${n} is equals to ${argc}. ${*} is a wildcard for all the arguments
        [ ] Create some common actions implemented as C functions, like reading input from the user

backlog:
    [ ] Check flags and defines to invalidate the build
        Maybe this can be done with a hash of the command IF the source timestamp is lesser or equals to the obj timestamp
    [ ] -j argument for multithreading
    [ ] Scripting mode: Argument to request just the specific target without resolving dependencies?
        Example: just invoking a linker command using the shared or executable target
    [ ] Add a "show" command to view details about resources
        [ ] Every element within an SbsFile should be able to print its own representation.
    [ ] Add flags to request verbosity
        [ ] Improve logging
        [ ] Make the parser continue on errors (unknown properties, missing commas?)
    [ ] sbs setup command? A configuration block that requests input from the user to configure the local build
    [ ] Add a "skip" string array with support for regexes to exclude files from the build
    [ ] Improve error handling throughout the project
    [ ] Validate SbsFile object once it is completely parsed
    [ ] Add "extends" support for targets (Rationale behind this?)