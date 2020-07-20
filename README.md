# Simple Build System

> **WARNING**: The project is in a experimental stage. It can build projects (it builds itself, and builds other projects of my authority), but it is not well tested, documentation is conspicuous by its absence, and it is far away of being production ready piece of software. Use under your own risk.

**sbs** goals is to be a portable build system that works in multiple platforms without too much effort. In order to do that it uses the following objects that are the building blocks of the build receipt:

- Environments
- Actions
- Toolchains
- Configurations
- Targets

These objects create an abstraction over different parts of the build process that may change between different platforms to simplify the build process and to ease the integration of new platforms or compilers.

## Environments

An environment contains information about the context where the build will happen.

```bash
# The name of the environment must be unique
env <name> { 
    # The type is "system" by default, but can be "cmd", "bash", or "powershell".
    type: "",
    # The terminal property allows the user to use a particular executable
    terminal: "",
    # A comma-separated list of strings that represent shell arguments (argv)
    args: [ ],
    # A comma-separated list of strings that represent environment variable with the format "<key>=<value>" (putenv)
    variables: [],
    # An environment can execute actions before doing anything, and after executing the build process
    actions: {
        # before and after are arrays of command strings (like `echo Hi`), or action names to be executed
        before: [],
        after: []
    }
}
```

By default **sbs** uses the `system` function to run the build commands, and therefore the most basic environment can be defined as:

```
env my-env {}
```

## Actions

An action is something that can be executed at certain points in the build process:

```bash
    action <name> {
        # Command strings are shell commands that can be executed
        `echo Hi`,
        # Actions can be compound with other actions referencing them by their names
        some-action-name
    }
```

They key idea of actions is to be able to define executable tasks that run in all the supported systems, and for that there is a `for` keyword that can be used to specify when the command strings or other actions must be run within the action being defined:

```bash
# The clean action deletes the obj and build folders within the project's directory
action clean {
    # If the environment is win-cmd at the moment of executing the clean action, these command strings will be run
    for win-cmd {
        `if exist .\obj (rd /s /q .\obj)`,
        `if exist .\build (rd /s /q .\build)`
    },

    # Otherwise, if the environment is linux or osx, this command string will be run
    for linux-bash, osx-bash {
        `rm -fr obj/ build/`
    }

    # Command strings or action names can be present outside of the for clauses, and in that case, those command will be
    # run for all the environments
}
```

## Toolchains

A toolchain is an object that contains information about a particular compiler, including the compiler itself, the linker, and the archiver. The following example shows 2 toolchains:

```bash
# Toolchain name is the toolchain identifier
toolchain clang {

    # Information about the compiler usage
    compiler: {
        bin: "clang",
        include_dir_flag: "-I",
        define_flag: "-D"
    }    

    # Windows specifics
    for win-cmd {
        # We use LLVM's archiver
        archiver: {
            bin: "llvm-lib.exe"
        },
        # We use MS' linker
        linker: {
            bin: "link.exe",
            lib_dir_flag: "/LIBPATH:",
            lib_flag: "/DEFAULTLIB:"
        }
    },

    # Linux specifics
    for linux-bash {
        archiver: {
            bin: "ar"
        },
        linker: {
            bin: "lld-link",
            lib_dir_flag: "-L",
            lib_flag: "-l"
        }
    }
}

toolchain gcc {

    compiler: {
        bin: "gcc",
        include_dir_flag: "-I",
        define_flag: "-D"
    }    

    # Linux specifics
    for linux-bash {
        archiver: {
            bin: "ar"
        },
        linker: {
            bin: "gcc",
            lib_dir_flag: "-L",
            lib_flag: "-l"
        }
    }
}
```

These examples are pretty self-explanatory, and the truth is these toolchains won't vary that much between projects.

## Configurations

The configurations are the set of flags to be passed to the toolchains while building the project, here is a trimmed example:

```bash
# The base configuration contains all the flags shared between debug and release configurations
config clang-config {
    # The compile object contains all the information to work with a toolchain's compiler. Being at the "root" of the
    # config declaration, it is shared by all the environments
    compile: {
        # The default object file extension
        extension: ".o",
        # Flags are a list of strings
        flags: [
            "-std=c99",
            "-Wall",
            "-Werror",
            "-Wextra",
            "-pedantic",
            # ${sbs.input_file} is a special variable that contains the source filename
            "-c ${sbs.input_file}",
            # The ${sbs.output_file} variable contains the output name for the compiled object
            "-o ${sbs.output_file}"
        ]
    },    

    # The for clause con be used to use specific overrides for a specific environment, like in this
    # case for the linux-bash environment
    for linux-bash {
        compile: {
            flags: [ "-fPIC" ]
        },
        # In this case, the archive defines all the information to work with the toolchain's archiver
        archive: {
            extension: ".a",
            flags: [ "rcs" ]
        },
        # The shared object is used to build a shared library, this information is used by the toolchain's linker
        shared: {
            extension: ".so",
            flags: [ 
                "-shared",
                "-o ${sbs.output_file}"
            ]
        },
        # Similar to the shared object, the executable object is used by the toolchain's linker to create an executable
        executable: {
            extension: "",
            flags: [ "-o ${sbs.output_file}" ]
        }
    },

    # Specific overrides for win-cmd environment
    for win-cmd {
        archive: {
            extension: ".lib",
            flags: [
                "/NOLOGO",
                "/OUT:${sbs.output_file}"
            ]
        },
        shared: {
            extension: ".dll",
            flags: [
                "/NOLOGO",
                "/DLL",
                "/OUT:${sbs.output_file}"
            ]
        },
        executable: {
            extension: ".exe",
            flags: [
                "/OUT:${sbs.output_file}",
                "/DEFAULTLIB:libcmt.lib",
                "/NOLOGO",
                "/SUBSYSTEM:CONSOLE"
            ]
        }
    }
}

# A configuration can extend from another one to add/override values
config clang-debug extends clang-config {
    compile: {
        # Arrays appends the extended values to the base ones, in this case, flags
        # will include the clang-config flags AND the flags from the clan-debug config
        flags: [
            "-ggdb",
            "-O0",
        ]

        # If the extension property is enabled here, it overrides the clang-config's value
        # extension: ".obj"
    }

    for win-cmd {
        shared: {
            flags: [
                "/DEBUG:FULL"
            ]
        },
        executable: {
            flags: [
                "/DEBUG:FULL"
            ]
        }
    }
}

# The release configuration adds specifc flags to get optimized code
config clang-release extends clang-config {
    compile: {
        flags: [
            "-O3"
        ]
    }
}
```

## Targets

Targets objects contains information about the parts of the project needed to build a certain piece of it. There are 4 types of targets:

- compile
- archive
- shared
- executable


### Compile

A compile target is used to compile one or many source files to one or many object files. It has the following format:

```bash
# The name of the target
compile <name> {
    # A comma-separated list of strings that represents directory path that are used to search for includes
    includes: [],
    # The output directory where to put the generated object files (the structure within the output directory includes
    # information about the environment, the toolchain, and the configuration)
    output_dir: "obj/",
    # The source files (parts) used to generate the output files
    sources: [
        "src/file1.c",
        "src/file2.c",
        # ...
        "src/fileN.c"
    ],
    # Similar to the environment actions, but they are executed before building the target and after building it
    actions: {
        before: [],
        after: []
    },

    # A for clause can be used to override/append values for specific environments
    for <env-name>, <env-name2> {
        # The same structure above can be used within a for clause
    }
}
```

### Archive

The archive target creates an archive of files, particularly used to create static libraries. The format of the archive target is as follows:

```bash
# The name of the target
archive <name> {
    # The name of the library
    output_name: "libname",
    # The output directory
    output_dir: "build/",
    # Similar to the environment actions, but they are executed before building the target and after building it
    actions: {
        before: [],
        after: []
    },
    # List of objects to be included within the output file
    objects: [
        # It can be a string to a valid file
        "file.o"
        # It can also be the output of a compile target that will be executed
        compile-target-objects
    ]

    # A for clause can be used to override/append values for specific environments
    for <env-name>, <env-name2> {
        # The same structure above can be used within a for clause
    }
}
```

### Shared

The shared is used to create shared libraries. Its format is similar to the archive target, the difference is that one uses the toolchain archiver's configuration while the other uses the toolchain linker's configuration:

```bash
# The name of the target
shared <name> {
    # The name of the library
    output_name: "libname",
    # The output directory
    output_dir: "build/",
    # Similar to the environment actions, but they are executed before building the target and after building it
    actions: {
        before: [],
        after: []
    },
    # List of objects to be included within the output file
    objects: [
        # It can be a string to a valid file
        "file.o"
        # It can also be the output of a compile target that will be executed
        compile-target-objects
    ]

    # A for clause can be used to override/append values for specific environments
    for <env-name>, <env-name2> {
        # The same structure above can be used within a for clause
    }
}
```

### Executable

The executable target is used to generate... executable files! Its format is:

```bash
# The name of the executable target
executable <name> {
    # The name of the executable
    output_name: "executable",
    # The output dir
    output_dir: "build/",
    # Similar to the environment actions, but they are executed before building the target and after building it
    actions: {
        before: [],
        after: []
    },
    # List of objects needed to create the executable
    objects: [
        # It can be a compile target name
        compile-target-objects,
        # It can also be an archive file that is the output of an archive target
        archive-target-file,
        # It can be a string that represents an object file
        "some-obj-file.o"
    ]
    # The libraries object allows the user to specify shared libraries
    libraries: [
        # It can be a system library (math library)
        { name: "m" }
        # It also accepts the path to a directory along with the library name
        { path "path/to/lib/directory", name: "mylib" },
        
    ]

    # A for clause can be used to override/append values for specific environments
    for <env-name>, <env-name2> {
        # The same structure above can be used within a for clause
    }
}
```

## Presets

Presets are a way to organize the build process. A preset is a "shortcut" for building a target:

```bash
# The preset name
preset <preset name> {
    # The following are the objects to use for the build
    env: <environment>,
    toolchain: <toolchain>,
    config: <config>,
    target: <target>,
    # Similar to the environment actions, but they are executed before running the preset
    actions: {
        before: [],
        after: []
    }
}
```

All the objects within a preset can be overridden when calling the build system from the command line.

# Usage

The sbs CLI currently has 3 commands:

```
init            Initializes the sbs system in the current directory
list            Prints the list of user defined elements for a resource
build           Starts the build process
```

### init

The `init` subcommand creates a `.sbs` directory containing a file called `build.sbs` within the project's directory. (The file is empty by now, the idea is to provide boilerplate for common environments, toolchains, and actions). All the objects can be defined in the `build.sbs` file or they can be splitted in multiple files, then with an `include` statement the `build.sbs` file can reference the new files:

```bash
include [
    "actions.sbs",
    "configs.sbs",
    "envs.sbs",
    "targets.sbs",
    "toolchains.sbs",
]
```

### list

The `list` command shows the different types of objects defined in the configuration file

```
sbs list envs            Shows the list of available environments
sbs list toolchains      Shows the list of available toolchains
sbs list configs         Shows the list of the available configurations
sbs list targets         Shows all the targets available to be built
sbs list presets         Shows the list of available presets
```

### build

This is the most important command, it builds a target, using the provided toolchain and configuration, within the provided environment:

```
--env=<name>, -e=<name>             Environment to be used in the build
                                    process

--toolchain=<name>, -tc=<name>      Toolchain to build the target

--config=<name>, -c=<name>          Configuration used to build the
                                    target

--target=<name>, -t=<name>          Target's name to be built
```

As an example, the following command builds the target "my-executable", using the "clang" toolchain with the "clang-debug" configuration, under the "linux-bash" environment:

```bash
sbs build --env=linux-bash --toolchain=clang --config=clang-debug --target=my-executable
```

Presets come in to simplify all this process. If we create a preset called "linux-clang-debug", we can set all those values within the preset, that way we just need to provide the preset's name instead of all the arguments:

```bash
preset linux-clang-debug {
    env: linux-bash,
    toolchain: clang,
    config: clang-debug,
    target: my-executable,
}
```

Now the target can be built by just running:

```bash
sbs build linux-clang-debug
```

# Building sbs with sbs

## Prerequisites

### For Windows

- Clang
- Visual Studio Developer Command Prompt: You will need the Microsoft C++ (MSVC) compiler toolset for the particular architecture you want to compile for. For more information check this Microsoft page: [Build C/C++ code on the command line](https://docs.microsoft.com/en-us/cpp/build/building-on-the-command-line?view=vs-2017).

### For Linux

- GCC or Clang

## Obtaining fllib
First you need to get the latest release version of the **fllib** static library [from here](https://github.com/lbrugnara/fllib/releases) based on your platform. 

**sbs** expects the **fllib** static library to be present in the following directories:

- For Windows:
```
lib/fllib/win/clang-release/libfl.lib
```

- For Linux:
```
lib/fllib/linux/clang-release/libfl.a
```

## Getting the latest sbs executable
The latest release version of **sbs** should be [here](https://github.com/lbrugnara/sbs/releases)

### Checking the available presets

If you run `sbs list presets` you should get the following:

```
sbs build system (0.1.0)
List of presets in the build file .sbs/build.sbs
clean
copy-fllib
install
linux-gcc-debug
linux-gcc-release
win-clang-debug
win-clang-release
```

Now, based on your platform run `sbs build linux-gcc-release` or `sbs build win-clang-release`.

If you want to compile with Clang under linux, you can run:

```bash
sbs build linux-gcc-release -tc=clang
```