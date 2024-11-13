# Sample Code Usage

This document provides instructions on how to use the sample code provided in the `odfvalidator-lib` repository.

## Prerequisites

- Ensure you have installed the `odfvalidator-lib` library.
- Ensure you have the necessary runtime environment and compilers installed for PHP and .NET.
- Ensure you have the FFI extension installed for PHP.

### Installing PHP FFI Extension

PHP version 7.4 or higher is required to use the FFI extension.

1. Open your `php.ini` file (usually located in your PHP installation directory).
2. Uncomment or add the following line:
   ```ini
   extension=ffi
3. Restart your web server or PHP service to apply the changes.

## PHP Sample Code

### Windows

1. Launch Command Prompt.
2. Navigate to the example directory:
   ```sh
   cd c:\Program Files\ODFValidator-Library\example\PHP
   ```
3. Run the sample code:
   ```sh
   php testDll.php <file-path>
   ```

### Linux

1. Install the `odfvalidator-lib-dev` package.
2. Launch a treminal.
3. Copy sample code to your home directory, change owner and grant executable permission.
   ```sh
   cp /usr/share/odfvalidator-lib/example/PHP/testSo.php ~
   sudo chown {your username}:{your user group} testSo.php
   chmod u+x testSo.php
   ```
4. Run the sample code:
   ```sh
   php ~/testSo.php <file-path>
   ```

## C# Sample Code

### Windows

1. Launch Command Prompt.
2. Navigate to the example directory:
   ```sh
   cd c:\Program Files\ODFValidator-Library\example\C-Sharp\TestDll
   ```
3. Build the `.csproj` file:
   ```sh
   dotnet build TestDll.csproj
   ```
4. Run the sample code:
   ```sh
   dotnet run <file-path>
   ```

### Linux

1. Launch a terminal.
2. Copy sample code to your home directory and change owner.
   ```sh
   cp -R /usr/share/odfvalidator-lib/example/C-Sharp/TestSo ~
   sudo chown -R {your username}:{your user group} TestSo/
   ```
3. Build the `.csproj` file:
   ```sh
   cd TestSo/
   dotnet build TestSo.csproj
   ```
4. Run the sample code:
   ```sh
   dotnet run <file-path>
   ```
