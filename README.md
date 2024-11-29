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

### For RedHat-based Linux distributions

You can use [the Docker Compose configuation file](docker-compose.yaml) to build the RPM packages for the RedHat-based Linux distributions.

Currently only Rocky Linux 8 is tested, for other distribution versions YMMV.

#### Prerequisites

* Docker Engine
* The build host must have Internet access.

#### Launch a Docker container as the build environment

1. Launch your preferred text terminal emulator application.
1. Change the working directory to the project directory.
1. Run the following command to launch a Docker container for building the product:

    ```bash
    docker compose -f docker-compose.packaging.yaml up -d
    ```

   **NOTE:**

   * Depending on your system and Docker Engine installation you may need to run this (and all the following docker commands) _as the administrator user(root)_.
   * You can stop and destroy the container by running the following command:

        ```bash
        docker compose -f docker-compose.packaging.yaml down --timeout 0
        ```

1. Run the following command to aquire a login shell in the container:

    ```bash
    docker exec -it odfvalidator-lib-rhpkg bash --login
    ```

1. (OPTIONAL) Run the following command to update all packages in the container to apply any bug fix that may be available after the base container image is built:

    ```bash
    dnf update -y
    ```

1. Run the following command to change the working directory to the project working tree mounted inside the container:

    ```bash
    cd /project
    ```

#### Install the build dependency packages

Run the following commands to install the build dependency packages:

```bash
build_dep_pkgs=(
    # Dependencies of auto{clean,gen}.sh
    autoconf
    automake
    findutils
    libtool

    # Dependencies of the build configuration program
    diffutils
    file

    # Dependencies of the Makefiles
    make

    # C/C++ compiler
    gcc
    gcc-c++

    # Dependencies for building the RPM packages
    rpm-build
)
dnf install -y "${build_dep_pkgs[@]}"
```

#### Generate the build configuration program

Run the following command to generate the build configuration program using GNU Autotools:

```bash
./autogen.sh
```

#### Configure the build

Run the following command to configure the build:

```bash
./configure
```

**NOTE:** This step also generates the RPM package specification file from [its template](odfvalidator.spec.in).

#### Generate the source distribution tarball

Run the following command to generate the source distribution tarball:

```bash
make dist
```

#### Build the RPM packages

Run the following command to build the RPM packages(replace `X.Y.Z` with the actual version string):

```bash
rpmbuild -tb odfvalidator-X.Y.Z.tar.gz
```

The built RPM packages should now be available under the ~/rpmbuild/RPMS directory:

```txt
/root/rpmbuild/RPMS
└── x86_64
    ├── ODFValidator-1.0.1-1.el8.x86_64.rpm
    ├── ODFValidator-debuginfo-1.0.1-1.el8.x86_64.rpm
    ├── ODFValidator-debugsource-1.0.1-1.el8.x86_64.rpm
    └── ODFValidator-devel-1.0.1-1.el8.x86_64.rpm

1 directory, 4 files
```

Run the following command to fetch the built RPM packages to the project work tree:

```bash
cp ~/rpmbuild/RPMS/*/*.rpm /project
```

### For Debian-based Linux distributions

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

## References

The following material are referenced during the development of this project:

* [3.4. What is a SPEC file | Red Hat Product Documentation](https://docs.redhat.com/fr/documentation/red_hat_enterprise_linux/9/html/packaging_and_distributing_software/assembly_what-a-spec-file-is_packaging-software#ref_spec-file-preamble-items_assembly_what-a-spec-file-is)
  Explains the format of the `BuildRequires` RPM spec file preamble item.
