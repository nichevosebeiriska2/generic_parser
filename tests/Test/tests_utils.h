#pragma once

namespace {
	constexpr auto result_getter = [](auto pair_parsed_result) {return pair_parsed_result.second; };
	constexpr auto status_getter = [](auto pair_parsed_result) {return pair_parsed_result.first; };
	auto lambda_identity = [](auto arg) { return arg; };
};