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
     * @brief
     * A memory manager that allocates memory via
     * the backend allocate() function.
     */
    template<typename T>
      class memory_resource : public ::drtm::memory_resource
      {
      public:
        using server_api_t = T;

      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a named memory manager object instance.
         */
        memory_resource (const server_api_t* api)
        {
          api_ = api;
#if defined(DEBUG)
          printf ("%s(%p) @%p\n", __func__, api, this);
#endif
        }

        /**
         * @cond ignore
         */

        memory_resource (const memory_resource&) = delete;
        memory_resource (memory_resource&&) = delete;
        memory_resource&
        operator= (const memory_resource&) = delete;
        memory_resource&
        operator= (memory_resource&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Destruct the memory manager object instance.
         */
        virtual
        ~memory_resource ()
        {
#if defined(DEBUG)
          printf ("%s() @%p\n", __func__, this);
#endif
        }

        /**
         * @}
         */

      protected:

        /**
         * @name Private Member Functions
         * @{
         */

        /**
         * @brief Implementation of the memory allocator.
         * @param bytes Number of bytes to allocate.
         * @param alignment Alignment constraint (power of 2).
         * @return Pointer to newly allocated block, or `nullptr`.
         */
        virtual void*
        do_allocate (std::size_t bytes, std::size_t align) override
        {
#if defined(DEBUG)
          printf ("%s(%zu,%zu) @%p\n", __func__, bytes, align, this);
#endif
          // Ignore alignment for now.
          void* mem = api_->allocate (bytes);
#if defined(DEBUG_)
          printf ("%s(%zu,%zu)=%p @%p\n", __func__, bytes, align, mem,
              this);
#endif

          return mem;
        }

        /**
         * @brief Implementation of the memory deallocator.
         * @param addr Address of a previously allocated block to free.
         * @param bytes Number of bytes to deallocate (may be 0 if unknown).
         * @param alignment Alignment constraint (power of 2).
         * @par Returns
         *  Nothing.
         */
        virtual void
        do_deallocate (void* addr, std::size_t bytes, std::size_t alignment) noexcept override
        {
#if defined(DEBUG)
          printf ("%s(%p,%zu,%zu) @%p\n", __func__, addr, bytes, alignment,
                  this);
#endif
          // Ignore size and alignment for now.
          api_->deallocate (addr);
        }

        /**
         * @brief Implementation of the equality comparator.
         * @param other Reference to another `memory_resource`.
         * @retval true The `memory_resource` objects are equal.
         * @retval false The `memory_resource` objects are not equal.
         */
        virtual bool
        do_is_equal (::drtm::memory_resource const &other) const noexcept override
        {
          // Actually there is only one such allocator, the backend is unique.
          return true;
        }

        /**
         * @}
         */

      private:
        const server_api_t* api_;
      };

  // template<typename T>
  //  const typename memory_resource<T>::server_api_t* memory_resource<T>::api_;

  } /* namespace drtm */
} /* namespace segger */

#endif /* defined(__cplusplus) */
