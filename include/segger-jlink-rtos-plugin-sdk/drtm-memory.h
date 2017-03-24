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

#include <segger-jlink-rtos-plugin-sdk/rtos-plugin.h>
#include <stdio.h>

#if defined(__cplusplus)

#include <drtm/memory.h>

namespace segger
{
  namespace drtm
  {

    /**
     * @brief A standard allocator that allocates memory via
     * the backend allocate() function.
     */
    template<typename T, typename S>
      class allocator
      {
      public:

        // Standard types.
        using value_type = T;
        using server_api_t = S;

      public:

        allocator (const server_api_t* api) noexcept
        {
#if defined(DEBUG)
          printf ("%s(%p) @%p\n", __func__, api, this);
#endif /* defined(DEBUG) */

          api_ = api;
        }

        allocator (allocator const & a) = default;

        template<typename U>
          allocator (allocator<U, S> const & other, const server_api_t* api) noexcept
          {
            api_ = api;
          }

        allocator&
        operator= (allocator const & a) = default;

        value_type*
        allocate (std::size_t objects)
        {
#if defined(DEBUG)
          printf ("%s(%zu) %p\n", __func__, objects, this);
#endif /* defined(DEBUG) */

          if (objects > max_size ())
            {
              throw std::system_error (
                  std::error_code (EINVAL, std::system_category ()));
            }

          value_type*p = static_cast<value_type*> (api_->malloc (
              objects * sizeof(value_type)));

#if defined(DEBUG)
          printf ("%s(%zu)=%p %p\n", __func__, objects, p, this);
#endif /* defined(DEBUG) */

          return p;
        }

        void
        deallocate (value_type* p, std::size_t objects) noexcept
        {
#if defined(DEBUG)
          printf ("%s(%p,%zu) %p\n", __func__, p, objects, this);
#endif /* defined(DEBUG) */

          assert(objects <= max_size ());
          api_->free (p);
        }

        std::size_t
        max_size (void) const noexcept
        {
          return std::numeric_limits<std::size_t>::max () / sizeof(value_type);
        }

        allocator
        select_on_container_copy_construction (void) const noexcept
        {
          return allocator (api_);
        }

      private:

      private:
        const server_api_t* api_;
      };

    ;
  // Avoid formatter bug
  // ==========================================================================
  } /* namespace drtm */
} /* namespace segger */

#endif /* defined(__cplusplus) */
