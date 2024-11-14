# ODFValidator-Library.

**Library name:** odfvalidator-lib

**Summary:** A tool to validate Open Document Format (ODF) files.

**License:** MPLv2.0

**Author:** Ministry of Digital Affairs <odf@moda.gov.tw>

**Description:** ODFValidator is a library designed to validate Open Document Format (ODF) files.

--------------------------------------------

## File Structure

**src/**

Contains the core implementation of the ODFValidator-Library, including the main header and source files, as well as build configuration files.

**tools/**

Contains command-line tools for check if the file complies with ODF specifications.

**resources/**

Contains additional resources required for validation.

**example/**

Contains sample code for calling this library in php and C# on Windows and Linux.

--------------------------------------------

## Building and Packaging

### For GNU/Linux

Using Ubuntu 20.04 as example environment. Make sure you install required software packages first:

```
sudo apt install -y build-essential debhelper devscripts
```

#### Build the DEB package

```
debuild -uc -us -b
```

#### Build the library manually

```
./autogen.sh
./configure
make
```

You will find `odfvalidator-lib_...-1.amd64.deb` and `odfvalidator-lib-dev_...-1.amd64.deb` in the parent directory.

##### Installing development packages

To install the development packages, use the following commands:

```
sudo apt install ../odfvalidator-lib_...-1.amd64.deb`
sudo apt install ../odfvalidator-lib-dev_...-1.amd64.deb`
```

### For Windows

#### Compile the MSI package

To compile the MSI package, you need to have WiX Toolset installed. Follow these steps:

1. Install Visual Studio with the necessary components for C++ development.
2. Install WiX Toolset v3 from [WiX Toolset website](https://wixtoolset.org/).
3. Open the Visual Studio and select `odfvalidator-lib` library.
4. Change the build configuration to "x64-Release"
5. Build `odfchecker.exe`, `odfvalidator.exe`, and `ODFValidator.dll` individually.
6. Select "Build All" to compile the entire project.

You will find `ODFValidator.msi` in the `odfvalidator-lib\out\build\x64-Release` directory.
