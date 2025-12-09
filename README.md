# Case Study

The case-study is implemented based on an arch linux system. As a result the code is designed and tested for linux only and no other operation systems.

## Build the application

There are several ways to build the target application:

1. `cmake-presets` can be used from the command line. The easiest way is to use the workflow presets:

     ```bash
    cmake --workflow <cmake-preset>
    ```

3. CLion can be used for building.
   In CLion, you can use the CMake presets (Settings -> Build, Execution, Deployment -> CMake).
   The “Debug” preset is added by CLion and can be removed. Otherwise, the targets can be enabled or disabled as needed.
   Additional CMake or environment variables must be added via the CMake preset files.

It depends on your personal flavour how to build the examples. You can use the command line but for developing purposes
configuring an IDE of your choice is recommended.

### GIT hooks

The [pre-commit](https://archlinux.org/packages/extra/any/pre-commit/) tool is used for running git hooks on every
commit.
Several pre-commit hooks are enabled in the .pre-commit-config.yaml.
For repositories that have already been cloned, the hook must be installed separately for each repository and submodule:

```bash
pre-commit install
```

To avoid having to install the pre-commit hook separately for each new git clone, it is possible to install it system-wide.
You can do this by first creating a global git template directory:

```bash
git config --global init.templateDir ~/.config/git/git-templates
```

and then attach the pre-commit hook to it:

```bash
pre-commit init-templatedir ~/.config/git/git-templates
```

Hooks can also be run manually:

```bash
pre-commit run --all-files
```

To perform a commit without executing the commit-hook, pass the `--no-verify` commit option.
