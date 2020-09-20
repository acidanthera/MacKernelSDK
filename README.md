MacKernelSDK
============

This project lets your kernel extensions (kexts) target a broad range of XNU kernels when using the latest Xcode version right from the interface. In addition it also includes several headers only available in other projects or older Xcode releases.

## Quick start

To start using **MacKernelSDK** do the following steps:

1. Clone MacKernelSDK to your your project directory:

    ```sh
    git clone https://github.com/acidanthera/MacKernelSDK
    ```

2. Add custom `KERNEL_EXTENSION_HEADER_SEARCH_PATHS` and `KERNEL_FRAMEWORK_HEADERS` values in project configuration in Xcode. To do this choose **Add User-defined Setting**.

    ```
    KERNEL_EXTENSION_HEADER_SEARCH_PATHS = $(PROJECT_DIR)/MacKernelSDK/Headers
    KERNEL_FRAMEWORK_HEADERS = $(PROJECT_DIR)/MacKernelSDK/Headers
    ```

    ![xcode-config](Docs/xcode-config.png)

3. Link to custom `libkmod.a` library by adding it to your kernel extension **Link Binary with Libraries**.

    ![xcode-link](Docs/xcode-link.png)

4. Optionally add `/MacKernelSDK` to `.gitignore`.

## Targeting `i386`

To compile for 32-bit you will need to add a set of flags in your project:

1. Other C Flags (`OTHER_CFLAGS`):

    - `-nostdlib`
    - `-Wno-stdlibcxx-not-found`
    - `-target i386-apple-macos10.6`
    - `-fallow-unsupported`

2. Other Linker Flags (`OTHER_LDFLAGS`):

    - `-static`
    - `-target i386-apple-macos10.6`

3. C++ Standard Library (`CLANG_CXX_LIBRARY`) to **Compiler Default** or empty.

## Extensions and modifications

- Added extra headers from `xnu-6153.141.1`:
    - Cryptography (`corecrypto`, `libkern/crypto`)
    - CPU PM (`i386/cpu_topology.h` and `i386/pmCPU.h`)
    - MAC Framework (`security/_label.h`, `security/mac_framework.h`, `security/mac_policy.h`)
- Added kmod targeting earlier macOS kernels:
    - 10.8 (`Library/x86_64/libkmod.a`)
- Fixed `string.h` to avoid using new checked interfaces before 10.13
- Fixed `IOService.h` compatibility with versions before 10.9
