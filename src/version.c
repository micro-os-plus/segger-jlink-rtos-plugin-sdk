/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2017 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * The plug-in client API definitions (the `RTOS_*` functions) and the
 * GDB server API definitions are compatible with the SEGGER GDBServer
 * RTOS Plug-in SDK API definitions.
 *
 * All IP rights, title and interest in the GDBServer RTOS Plug-in SDK
 * are and shall at all times remain with SEGGER.
 *
 * Copyright (c) 2004-2016 SEGGER Microcontroller GmbH & Co. KG
 * Internet: www.segger.com        Support:  support@segger.com
 */

#include <segger-jlink-rtos-plugin-sdk/version.h>

typedef struct version_s
{
  int major;
  int minor;
  int patch;
} version_t;

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#endif /* defined(__clang__) */

version_t ilg_segger_jlink_rtos_plugin_sdk_version =
  {
  //
      XPACK_ILG_SEGGER_JLINK_RTOS_PLUGIN_SDK_VERSION_MAJOR,
      XPACK_ILG_SEGGER_JLINK_RTOS_PLUGIN_SDK_VERSION_MINOR,
      XPACK_ILG_SEGGER_JLINK_RTOS_PLUGIN_SDK_VERSION_PATCH
  /**/
  };

#pragma GCC diagnostic pop
