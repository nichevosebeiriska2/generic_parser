#pragma once

#include "common.h"

#include <type_traits>

template<typename SkipperType = tag_skipper_non, bool omited = false, bool case_sensetive = true>
class Context
{
	static_assert(is_skipper_non_type_v<SkipperType> || ConceptParser<SkipperType>, "Context::SkipperType must satisfy requirements of ConceptParser<SkipperType> "
								"or be equal to tag_skipper_non in case context intended to ignore skipper");

public:
	using _skipper_type = SkipperType;
	constexpr static bool _omited{omited};
	constexpr static bool _case_sensetive{case_sensetive};

	SkipperType _skipper;

public:

	//Context(){};

	template<typename TSkipper>
	Context(TSkipper &&skipper)
		: _skipper{std::forward<TSkipper>(skipper)}
	{
	}

	Context(Context<SkipperType> &&other)
		: _skipper{other._skipper}
	{
	}

	Context(Context<SkipperType> &other)
		: _skipper{other._skipper}
	{
	}

	constexpr static bool HasSkipper()
	{
		return !is_skipper_non_type_v<SkipperType>;
	}

	template<ConceptCharType CharType>
	constexpr void UseSkipper(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end)
	{
		if constexpr(HasSkipper())
		{
			::UseSkipper(ptr_string, ptr_string_end, _skipper);
		}
	}

	constexpr static bool IsOmitedStatic()
	{
		return _omited;
	}

	constexpr bool IsOmited()
	{
		return _omited;
	}
};

template<typename TContext>
constexpr auto OmitedFromThis(TContext &&other)
{
	return Context<typename std::remove_cvref_t<TContext>::_skipper_type
								, true
								, std::remove_cvref_t<decltype(other)>::_case_sensetive>(other);
}

template<typename TContext>
constexpr auto CaseSensetiveFromThis(TContext &&other)
{
	return Context<typename std::remove_cvref_t<TContext>::_skipper_type
								, std::remove_cvref_t<decltype(other)>::_omited
								, true>(other);
}

template<typename TContext>
constexpr auto CaseInsensetiveFromThis(TContext &&other)
{
	return Context<typename std::remove_cvref_t<TContext>::_skipper_type
								, std::remove_cvref_t<decltype(other)>::_omited
								, false>(other);
}

template<typename TSkipper>
Context(TSkipper &&skipper) -> Context<TSkipper>;

template<typename TSkipper>
Context(TSkipper &skipper) -> Context<TSkipper>;

template<typename TSkipper>
Context(Context<TSkipper> &&other) -> Context<TSkipper>;

template<typename TSkipper>
Context(Context<TSkipper> &other) -> Context<TSkipper>;


template<typename TAttribute, typename TValue>
class CParsingResultContext
{
	TAttribute &_attr;
	TValue &_val;
public:

	CParsingResultContext(TAttribute & attr, TValue &val)
		: _attr{attr}
		, _val{val}
	{
	}
};