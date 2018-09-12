#ifndef __RENDERLIB_ENGINEEXCEPTION_H__
#define __RENDERLIB_ENGINEEXCEPTION_H__

#include <stdexcept>

namespace RenderLib
{
	class EngineException : public std::runtime_error
	{
	public:
		EngineException(const char * const message) throw();
		virtual const char * what() const throw ();
	};
}

#endif