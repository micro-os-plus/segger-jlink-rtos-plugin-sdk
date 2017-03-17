# File templates

The template files use the [Shopify liquid](https://shopify.github.io/liquid/) template language.

## `version.liquid.h`

This file is used as input to generate the `include/segger-jlink-rtos-plugin-sdk/version.h` file, based on the xPack version in `package.json`.

Until the `xpm` tool will be available to fully process the liquid tags and filters, this file is processed by the separate shell script `scripts/version.sh`, invoked by `npm`.

