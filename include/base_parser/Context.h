#pragma once

#include "common.h"

#include <type_traits>

template<ConceptCharType CharType>
class Skipper
{
	std::basic_string_view<CharType> m_skip_symbols;

public:
	Skipper(std::basic_string_view<CharType> symbols)
		: m_skip_symbols{ symbols }
	{}

	Skipper(const CharType* symbols)
		: m_skip_symbols{ symbols }
	{};

	void Skip(constCharPtrRef<CharType> ptr_string)
	{
		if constexpr(std::is_same_v<char, CharType>)
			ptr_string += std::strspn(ptr_string, m_skip_symbols.data());

		else if constexpr(std::is_same_v<wchar_t, CharType>)
			ptr_string += std::wcsspn(ptr_string, m_skip_symbols.data());

		else
			static_assert(false, "Skipper::Skip() - forbidden char type");
	}
};


template<ConceptCharType CharType>
Skipper(std::basic_string_view<CharType> symbols) -> Skipper<CharType>;

template<ConceptCharType CharType>
Skipper(const CharType* symbols) -> Skipper<CharType>;


template<typename SkipperType = tag_skipper_non, bool omited = false, bool case_sensetive = true>
class Context
{
public:
	using _skipper_type = SkipperType;
	constexpr static bool _omited{omited};
	constexpr static bool _case_sensetive{case_sensetive};
	constexpr static bool _has_skipper = !is_skipper_non_type_v<SkipperType>;
	SkipperType _skipper;

public:

	Context() = default;

	template<typename TSkipper>
	Context(TSkipper &&skipper)
		: _skipper{std::forward<TSkipper>(skipper)}
	{
	}

	Context(Context<SkipperType> &&other)
		: _skipper{other._skipper}
	{
	}

	Context(const Context<SkipperType> &other)
		: _skipper{other._skipper}
	{
	}

	constexpr static bool HasSkipper()
	{
		return _has_skipper;
	}

	template<ConceptCharType CharType>
	constexpr void UseSkipper(constCharPtrRef<CharType> ptr_string, constCharPtrRef<CharType> ptr_string_end)
	{
		if constexpr(HasSkipper())
			_skipper.Skip(ptr_string);
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

template<typename T>
struct is_context : std::false_type {};

template<typename TSkipper,bool omited, bool case_sensetive>
struct is_context<Context<TSkipper, omited, case_sensetive>> : std::true_type {};

template <typename T>
concept ConceptContext = is_context<std::remove_cvref_t<T>>::value;


template<ConceptCharType CharType, typename TChildValue>
class CActionContext
{
	std::reference_wrapper<TChildValue> _val;

	constCharPtr<CharType> ptr_begin;
	constCharPtr<CharType> ptr_end;

public:

	constexpr CActionContext(TChildValue& val) noexcept
		: _val{val}
	{}

	TChildValue& GetValue() { return _val.get(); }

	void SetBegin(constCharPtr<CharType> ptr) { ptr_begin = ptr; }
	void SetEnd(constCharPtr<CharType> ptr) { ptr_end = ptr; }
	auto Begin() const { return ptr_begin; }
	auto End() const { return ptr_end; }
};

