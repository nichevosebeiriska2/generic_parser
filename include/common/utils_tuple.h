#pragma once

#include <tuple>

namespace tuple_utils
{

	namespace implementation// hide implementation from user inside 'implementation' namespace
	{
		template <typename T>
		struct is_std_tuple : std::false_type
		{
		};

		template <typename... Ts>
		struct is_std_tuple<std::tuple<Ts...>> : std::true_type
		{
		};

		template <typename T>
		constexpr bool is_std_tuple_v = is_std_tuple<std::decay_t<T>>::value;


		template<typename T>
		auto TupleDeleteParamsOfRequiredType_Impl(auto && ... args)
		{
			constexpr auto lambda_condiditional = [&](auto&& arg)
			{
				if constexpr(std::is_same_v<std::remove_cvref_t<decltype(arg)>, T>)
					return std::tuple{};
				else
					return std::tuple{arg};
			};

			return std::tuple_cat(lambda_condiditional(std::forward<decltype(args)>(args)) ...);
		}
	}


	template <typename T>
	constexpr auto as_tuple(T &&t)
	{
		if constexpr(implementation::is_std_tuple_v<std::decay_t<T>>)
			return std::forward<T>(t);
		else
			return std::make_tuple(std::forward<T>(t));
	}

	template<typename ... Args>
	constexpr auto concat_as_tuple(Args ... args)
	{
		return std::tuple_cat(as_tuple(args) ...);
	}

	template<typename TRequiredToDelete, typename ... T>
	auto TupleDeleteParamsOfRequiredType(std::tuple<T...> &tup_results)
	{
		return std::apply([&](auto ... args){ return implementation::TupleDeleteParamsOfRequiredType_Impl<TRequiredToDelete>(std::forward<decltype(args)>(args)...); }, tup_results);
	}

	template<typename TRequiredToDelete, typename ... T>
	auto TupleDeleteParamsOfRequiredType(std::tuple<T...> && tup_results)
	{
		return std::apply([&](auto ... args)
		{
			return implementation::TupleDeleteParamsOfRequiredType_Impl<TRequiredToDelete>(std::forward<decltype(args)>(args)...);
		}, tup_results);
	}

}