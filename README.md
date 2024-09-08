# Esterv.Utils.Updater 

[TOC]

This repo implements a C++ class and QML Module that takes care of downloading and installing application updates. 
The latter can be used to do in-app updates.
The methods rely on the [Qt Installer Framework ](https://doc.qt.io/qtinstallerframework/)(QtIFW) and that the application also installs the 'maintenancetool' provided by the QtIFW. 

## Dependencies

The repo depends on [Qt](https://doc.qt.io/) libraries.

## Configure, build, test, package ...
 
The project uses [CMake presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html) as a way to share CMake configurations.
Refer to [cmake](https://cmake.org/cmake/help/latest/manual/cmake.1.html), [ctest](https://cmake.org/cmake/help/latest/manual/ctest.1.html) and [cpack](https://cmake.org/cmake/help/latest/manual/cpack.1.html) documentation for more information on the use of presets.

## Adding the libraries to your CMake project 

```CMake
include(FetchContent)
FetchContent_Declare(
	EstervUpdater
	GIT_REPOSITORY https://github.com/EddyTheCo/Esterv.Utils.Updater.git
	GIT_TAG vMAJOR.MINOR.PATCH
	FIND_PACKAGE_ARGS MAJOR.MINOR CONFIG
	)
FetchContent_MakeAvailable(EstervUpdater)
target_link_libraries(<target> <PRIVATE|PUBLIC|INTERFACE> Esterv::updater)
```

If you want to use the QML module also add
```
$<$<STREQUAL:$<TARGET_PROPERTY:Esterv::updater,TYPE>,STATIC_LIBRARY>:Esterv::updaterplugin>
```

## API reference

You can read the [API reference](https://eddytheco.github.io/Esterv.Utils.Updater/) here, or generate it yourself like

```
cmake --workflow --preset default-documentation
```

## Using the QML modules

One needs to  make available to the QML engine the different modules by setting the [QML import path](https://doc.qt.io/qt-6/qtqml-syntax-imports.html#qml-import-path).

1. In your main function `engine.addImportPath("qrc:/esterVtech.com/imports");` to use the resource file. 

2. Set the environment variable like `export QML_IMPORT_PATH=installDir/CMAKE_INSTALL_LIBDIR`  where `CMAKE_INSTALL_LIBDIR` is where `Esterv` folder was created.

## Examples

The [examples](examples) folder shows the use of the different classes and QML types.


## Contributing

We appreciate any contribution!


You can open an issue or request a feature.
You can open a PR to the `develop` branch and the CI/CD will take care of the rest.
Make sure to acknowledge your work, and ideas when contributing.
