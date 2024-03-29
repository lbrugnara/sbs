v0.2.0:
    [x] Add a `flags` key to the targets so that they can add target specific flags (e.g.: /DEF: files for windows)
    [ ] sbs info or sbs host command that shows information about the host OS
    [ ] Environments
        [ ] Create a "default" environment for the current os-arch that uses "system" if there is no environment defined in the build file
        [ ] Fix and improve the shell drivers (currently "system" is the only reliable way of using sbs)
            [ ] Within the same environment, a way to detect the terminal to use
                Example: running "sbs build debug" within a PowerShell instance should recognize a Windows environment prepared to run in PowerShell
            [x] Check directory separator in the case of a win-bash combination
                Note: An environment with a for clause like $sbs.host.os == win can be combined with an os property with
                      value $sbs.linux and the paths should be correctly changed to use '/' as directory separator
        [x] Add for clauses to the environments
        [x] Environments review the API and the properties
            [x] Rename env properties from type, terminal, and args to shell_type, shell_command, and shell_args
            [x] Expose builtin variables (like $sbs.shell)
    [ ] Advanced support for triplets
        [ ] Use the executor to find toolchain's executable files
        [ ] Add a preconditions property that can contain actions and is evaluated in the process of getting all the available triplets
            Notes: for env? for toolchian? both?
        [x] In presets, make "toolchain" property an array to allow multiple toolset combinations
        [x] In presets, make "config" property an array to allow multiple toolset combinations
    [ ] Add builtin sbs binaries and wrap them in actions
        [ ] sbs-mkdir
        [ ] sbs-whereis
        [ ] sbs-pkg-config like command?
    [ ] sbs run command to run actions
    [ ] Script mode
        [ ] Add sbs-mkdir commands to the output scripts
        [x] Don't create directories in the host
        [x] Use host default triplet if -e|--env is not provided
    [ ] Conditional statements and variables: support different data type
    [ ] Defining and using variables
        [ ] Allow defining variables within sections
        [ ] Check that "system" executor and custom executors correctly set and retrieve ENV variables
        [ ] Allow defining/extending variable in the command line
            Example: a $my.flags variable within a compile's flag property can be extended/overridden by a command line argument
        [x] Add $sbs.version.major, $sbs.version.minor, $sbs.version.patch, and $sbs.version.string
        [x] Add more builtin variables: sbs.host.os and sbs.host.arch
        [x] Add constant-like variables: sbs.linux, sbs.win, sbs.x86, sbs.x86_64
        [x] Define variables at the top-level scope
        [x] Add builtin variables: $sbs.compiler, $sbs.archiver, $sbs.linker
        [x] $env.<name> syntax to get environment variables
        [x] Textual replacement 
    [ ] String interpolation for variables
        [x] Add support for actions
        [ ] Add support for environments
        [ ] Add support for presets
        [ ] Add support for targets
        [ ] Add support for toolchains
        [x] Add support for the configuration section (compile, archive, shared, and executable)
        [x] Initial support for string interpolation (compile flags support interpolated strings)
    [ ] String interpolation for expressions
        Example: "/path/to/lib.{$sbs.host.os == win ? "lib" : "a"}"
        [ ] Escape with double brace: 
            Example 1: "The variable {{$sbs.env}} is not interpolated" -> The variable {$sbs.env} is not interpolated
            Example 2: "The expression {{$sbs.env == win-cmd}} is not interpolated" -> The expression {$sbs.env == win-cmd} is not interpolated
        [x] Fix bug with escaped characters (\" \` and \\)
    [x] Eval context: create the API to avoid directly using fl_hashtable_* functions
        Notes: There is a remaining usage of the context->variables member that should be removed once the "String interpolation for variables" is implemented
    [ ] All types: Add specific inline functions to interop with FlHashtables and FlArrays
    [ ] sbs init command (scaffolding): Add basic elements and common toolchains
        [ ] Init specific "identifier"? -e=my-env1,my-env2 --> my-env1{} my-env2{}
        [ ] Default win-cmd, linux-bash, osx-bash envs??
        [ ] Default clang, gcc, msvc toolchains?    
    [ ] Update grammar file
    [x] Add version command (-v|--version) to get information about the program version
    [x] Add the flag --arch|-a to provide an specific architecture(x86, x86_64)
    [x] Configuration: Add support for nested ifs and if-else support
    [x] Fix build command when preset name is not provided
    [x] Scripting language: Add support for the ternary operator
    [x] --working-dir|-cwd flag accepted by the main program along with the build subcommand
        Example: `sbs -cwd=../ build debug` is equals to `sbs build debug -cwd=../`. The advantage is that a command like
        `sbs -cwd=../ list presets` works out-of-the-box
    [x] Add builtin $sbs.bin variable
    [x] Linux support: in the latest version the Linux support is incomplete
    [x] Allow changing the working directory for the build command
    [x] Check flags and defines to invalidate the build
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
        Examples: 
            - "/path/to/libsomething.${sbs.env == "win-cmd" ? "lib" : "a"}" <-- The "win-cmd" must be a string because we don't have context for it
            - "/path/to/libsomething.${env(win-cmd) ? "lib" : "a"}" <-- win-cmd is expected to be an env (this one could work right away)
    [ ] Add support for glob (currently we only support regexes)
        [ ] Prepend regexes with ~ to differentiate paths from it
    [ ] Make sure the objects are valid once they are resolved 
        Example: sbs build debug -s -tc=msvc -c=msvc-config -e=linux-bash --> The toolchaing 'msvc' does not exist (it is actually not valid for the requested environment)
    [ ] Passing arguments to actions and using positional variables for it ${1}, ${2}, ... ${n} where ${n} is equals to ${argc}. ${*} is a wildcard for all the arguments
        [ ] Create some common actions implemented as C functions, like reading input from the user
    [ ] Value sources: allow different sources:
        - Plain strings (currently supported)
        - Command strings
        - Regexes (currently supported but it could be great to implement the ~"" syntax for it)
        - Identifiers (currently supported -refer to other targets-)
    [ ] Add a "sources" property to targets archive, shared, and executable to directly compile sources??
    [ ] sbs list command:
        [ ] Targets: show the type of target next to the name

backlog:
    [ ] Add a way to process an string to change it, particularly for paths:
        Examples:
            - `.\build\${triplet}\tests.exe` to `.\build\{$triplet | sbs.to-windows-path}\tests.exe`
            - `./build/${triplet}/tests.exe` to `./build/{$triplet | sbs.to-linux-path}/tests.exe`
    [ ] Add string command blocks (```) to avoid using the '\''\n' combination to continue the line
        Example:
        ```
        <first command> && <second command>
        && <third command>
        ```
        Instead of
        `<first command> && <second command> \
        && <third command>`
    [ ] Add "multi presets" or a new concept related to it. Now this can be done through actions
        Example `sbs build win-debug` preset could run other presets like:
            - `sbs build debug -e=win-cmd-x64 -tc=clang`    
            - `sbs build debug-coverage -e=win-cmd-x64 -tc=clang`
            - `sbs build debug-sanitize -e=win-cmd-x64 -tc=clang`
            - `sbs build debug -e=win-cmd-x86 -tc=clang`    
            - `sbs build debug-coverage -e=win-cmd-x86 -tc=clang`
            - `sbs build debug-sanitize -e=win-cmd-x86 -tc=clang`
            - `sbs build debug -e=win-wsl-x64-tc=gcc`    
            - `sbs build debug-coverage -e=win-wsl-x64-tc=gcc`
            - `sbs build debug-sanitize -e=win-wsl-x86-tc=gcc`
    [ ] Find a workaround for the limitation of Windows' cmd to run commands larger than 8192 characters
    [ ] A custom/binary/command target able to run a program to generate the output of the target
        Notes: If you need to run a specific/custom step in the build process (different to compile, archive, shared, and executable) you can do it through actions, 
        but it is not part of the build per-se and it is not checked/considered as a dependency, so it is useful to have a custom target able to perform custom actions
        (it could be an script that generates a big .c file and that .c file is the dependency of a compile target)
    [ ] A CLI command to show the dependency graph
    [ ] Use a headers node within the compile target to specify .c files dependencies (without using resolve_c_file_dependencies?)
    [ ] Hash based dependencies instead of timestamp based
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
    [ ] Build targets: a lot of code to reuse and/or encapsulate between the different targets
    [ ] TESTS TESTS TESTS