#pragma once

#include <tuple>
#include <vector>
#include <type_traits>


//template<typename T>
//using TParsingResult = std::tuple<bool, T, int>;
//
//using TParsingResultOmited = std::tuple<bool, int>;
//
// template<typename T>
// using TParsingResultVector = std::tuple<bool, std::vector<T>, int>;
//
//using TScanningResult = std::pair<bool, int>;

template<typename T>
concept ConceptCharType = std::is_same_v<T, char> || std::is_same_v<T, wchar_t>;

