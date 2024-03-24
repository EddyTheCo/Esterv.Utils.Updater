# Esterv.Utils.Updater 

[TOC]

This repo implements a C++ class and QML Module that takes care of downloading and installing application updates. 
The latter can be used to do in-app updates.
The methods rely on the [Qt Installer Framework ](https://doc.qt.io/qtinstallerframework/)(QtIFW) and that the application also install the 'maintenancetool' provided by the QtIFW. 

## Dependencies

The repo depends on [Qt](https://doc.qt.io/) libraries.

## Installing the library 

### From source code
```
git clone https://github.com/EddyTheCo/QUpdater.git

mkdir build
cd build
qt-cmake -G Ninja -DCMAKE_INSTALL_PREFIX=installDir -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLES=OFF -DQTDEPLOY=OFF -DUSE_QML=OFF -DBUILD_DOCS=OFF ../QUpdater

cmake --build . 

cmake --install . 
```
where `installDir` is the installation path, `QTDEPLOY` install the examples and Qt dependencies using the 
[cmake-deployment-api](https://www.qt.io/blog/cmake-deployment-api). Setting the `USE_QML` variable produce or not the QML module.
One can choose to build the examples and the documentation with the `BUILD_EXAMPLES` and `BUILD_DOCS` variables.

### From GitHub releases
Download the releases from this repo. 

## Adding the libraries to your CMake project 

```CMake
include(FetchContent)
FetchContent_Declare(
	QtUpdater	
	GIT_REPOSITORY https://github.com/EddyTheCo/QUpdater.git
	GIT_TAG vMAJOR.MINOR.PATCH 
	FIND_PACKAGE_ARGS MAJOR.MINOR CONFIG  
	)
FetchContent_MakeAvailable(QtUpdater)

target_link_libraries(<target> <PRIVATE|PUBLIC|INTERFACE> QtUpdater::qupdater)
```
If you want to use the QML module also add
```
$<$<STREQUAL:$<TARGET_PROPERTY:QtUpdater::qupdater,TYPE>,STATIC_LIBRARY>:QtUpdater::updaterplugin>
```

## API reference

You can read the [API reference](https://eddytheco.github.io/QUpdater/), or generate it yourself like
```
cmake -DBUILD_DOCS=ON ../
cmake --build . --target doxygen_docs
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
