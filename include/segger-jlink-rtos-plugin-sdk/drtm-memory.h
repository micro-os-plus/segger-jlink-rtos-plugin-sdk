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

          value_type*p = static_cast<value_type*> (api_->allocate (
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
          api_->deallocate (p);
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

    // ========================================================================

    /**
     * @brief A memory manager that allocates memory via
     * the backend allocate() function.
     *
     * Useful when the application requires a polymorphic allocator;
     * otherwise use the simple allocator.
     */
    template<typename S>
      class memory_resource : public ::drtm::memory_resource
      {
      public:

        using server_api_t = S;

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

  // ==========================================================================
  } /* namespace drtm */
} /* namespace segger */

#endif /* defined(__cplusplus) */
