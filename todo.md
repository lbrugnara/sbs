v0.2.0:
    [ ] Advanced support for triplets
        [ ] Use the executor to find environment's shell binaries
        [ ] Use the executor to find toolchain's executable files
        [ ] Add a preconditions property that can contain actions and is evaluated in the process of getting all the available triplets
            Notes: for env? for toolchian? both?
        [ ] A builtin action-like function to search for binaries.
        [ ] Basic support to search within the PATH
        [x] In presets, make "toolchain" property an array to allow multiple toolset combinations
        [x] In presets, make "config" property an array to allow multiple toolset combinations
    [ ] sbs run command to run actions
    [ ] Change working directory
    [ ] Defining and using variables
        [ ] Variables should only be defined/declared at the top-level? Maybe not
        [ ] Add more builtin variables like $sbs.compiler, $sbs.linker, etc
        [ ] ${env:<name>} syntax to get environment variables
            [ ] Check that "system" executor and custom executors correctly set and retrieve ENV variables
        [ ] Allow defining/extending variable in the command line
            Example: a $my.flags variable within a compile's flag property can be extended/overridden by a command line argument
        [x] Textual replacement 
    [ ] sbs init command (scaffolding): Add basic elements and common toolchains
        [ ] Init specific "identifier"? -e=my-env1,my-env2 --> my-env1{} my-env2{}
        [ ] Default win-cmd, linux-bash, osx-bash envs??
        [ ] Default clang, gcc, msvc toolchains?
    [ ] Within the same environment, a way to detect the terminal to use
        Example: running "sbs build debug" within a PowerShell instance should recognize a Windows environment prepared to run in PowerShell
    [ ] Linux support: in the latest version the Linux support is incomplete
    [ ] Update grammar file
    [x] Initial support for tiplets: collect all the combinations of environments, toolchains, and configurations available in the host machine
    [x] In presets, make "target" property an array object to run multiple targets at once
    [x] Rename "for" keyword to "if" within sections. In the current context it makes (a lot) of sense to use the conditional
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
    [ ] Add support for glob (currently we only support regexes)
        [ ] Prepend regexes with ~ to differentiate paths from it
    [ ] Make sure the objects are valid once they are resolved 
        Example: sbs build debug -s -tc=msvc -c=msvc-config -e=linux-bash --> The toolchaing 'msvc' does not exist (it is actually not valid for the requested environment)
    [ ] Passing arguments to actions and using positional variables for it ${1}, ${2}, ... ${n} where ${n} is equals to ${argc}. ${*} is a wildcard for all the arguments
        [ ] Create some common actions implemented as C functions, like reading input from the user

backlog:
    [ ] A custom/binary/command target able to run a program to generate the output of the target
        Notes: If you need to run a specific/custom step in the build process (different to compile, archive, shared, and executable) you can do it through actions, 
        but it is not part of the build per-se and it is not checked/considered as a dependency, so it is useful to have a custom target able to perform custom actions
        (it could be an script that generates a big .c file and that .c file is the dependency of a compile target)
    [ ] A CLI command to show the dependency graph
    [ ] Use a headers node within the compile target to specify .c files dependencies (without using resolve_c_file_dependencies?)
    [ ] Hash based dependencies instead of timestamp based
        [ ] Check flags and defines to invalidate the build
    [ ] -j argument for multithreading
    [ ] Scripting mode: Argument to request just the specific target without resolving dependencies?
        Example: just invoking a linker command using the shared or executable target
    [ ] Add a "show" command to view details about resources
        [ ] Every element within an SbsFile should be able to print its own representation.
    [ ] Add flags to request verbosity
        [ ] Improve logging
        [ ] Make the parser continue on errors (unknown properties, missing commas?)
    [ ] sbs setup command? A configuration block that requests input from the user to configure the local build
        Notes: It could be useful to check prerequisites and prepare the environment (like ./configure)
    [ ] Add a "skip" string array with support for regexes to exclude files from the build
    [ ] Improve error handling throughout the project
    [ ] Validate SbsFile object once it is completely parsed
    [ ] Add "extends" support for targets (Rationale behind this?)