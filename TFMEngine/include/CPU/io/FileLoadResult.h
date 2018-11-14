#ifndef __RENDERLIB_CPU_FILELOADRESULT_H__
#define __RENDERLIB_CPU_FILELOADRESULT_H__

#include <memory>

namespace RenderLib
{
  namespace CPU
  {
    namespace IO
    {
      template <class T> class LoadedParameter
      {
      private:
		/// <summary>
		/// The value this loaded parameters hold (if exists)
		/// </summary>
		T value;


		/// <summary>
		/// Flag that tells wether this parameter has been modified
		/// during load
		/// </summary>
		bool exists;

      public:
		/// <summary>
		/// Initializes a new instance of the <see cref="LoadedParameter"/> class.
		/// </summary>
		LoadedParameter() : exists(false)
        {
        }


		/// <summary>
		/// Sets the specified new value, modify the <see cref="exists"/> flag.
		/// </summary>
		/// <param name="newVal">The new value to be set.</param>
		void
        set(const T & newVal)
        {
          value  = newVal;
          exists = true;
        }


		/// <summary>
		/// Returns a reference to the current value. Does not check
		/// for existance
		/// </summary>
		/// <returns>A const reference to the current value</returns>
		const T &
        get()
        {
          return value;
        }

		/// <summary>
		/// Operator()s that returns a const reference to the current value.
		/// Does not check for existance
		/// </summary>
		/// <returns></returns>
		const T &
        operator()()
        {
          return value;
        }


		/// <summary>
		/// Returns wether this parameter exists (has been modified during loading of a file)
		/// </summary>
		/// <returns>True if the parameter exists, false otherwise</returns>
		const bool &
        paramExists()
        {
          return exists;
        }
      };

      class AbstractLoadResult
      {
      public:
		/// <summary>
		/// Gets the size (in bytes) of the loaded data
		/// </summary>
		/// <returns></returns>
		virtual size_t
        getResultSizeBytes()
            = 0;
      };

      typedef std::unique_ptr<AbstractLoadResult> AbstractLoadResultPtr;
    } // namespace IO
  } // namespace CPU
} // namespace RenderLib

#endif
