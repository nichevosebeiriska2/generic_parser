
#include "parsing.h"

#include <fstream>

std::string read_file(std::string strPathToFile)
{
	std::ifstream file(strPathToFile);

	if (file.is_open())
		return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>{} };

	return "";
}

#include "BaseParserTest.h"
#include "RepeateParserTest.h"
#include "ListParserTest.h"
#include "AlternativeParserTest.h"
#include "PredicateNotTest.h"

#include "ExampleJson.h"

#include <filesystem>

std::string strjson = R"(
{ "str_val" : "\"user\":{\"name\":\"Azamat Ramazanov\",\"username\":\"azamatxo\"},\"blur_hash\":\"LjD,Tf${RoWBF*NHs.t5IWbcaxRk\",\"datetime\":\"2022-07-06T10:53:44.344Z\"}"}
)";

std::filesystem::path path_to_meta = R"(C:\Users\dmileykin\source\repos\figma_parser\build2\Debug\output\meta.json)";
std::filesystem::path path_to_scheme = R"(C:\Users\dmileykin\source\repos\figma_parser\build2\Debug\output\scheme.json)";
std::filesystem::path path_to_data = R"(C:\Users\dmileykin\source\repos\figma_parser\build2\Debug\output\data.json)";

std::filesystem::path large_file				= R"(C:\Users\dmileykin\source\repos\generic_parser\include\examples\Json\large.txt)";
std::filesystem::path extra_large_file	= R"(C:\Users\dmileykin\source\repos\generic_parser\include\examples\Json\extra_large.txt)";
std::filesystem::path formatted_64kb		= R"(C:\Users\dmileykin\source\repos\generic_parser\include\examples\Json\formatted\64kb.txt)";
std::filesystem::path formatted_128kb		= R"(C:\Users\dmileykin\source\repos\generic_parser\include\examples\Json\formatted\128kb.txt)";
std::filesystem::path formatted_256kb		= R"(C:\Users\dmileykin\source\repos\generic_parser\include\examples\Json\formatted\256kb.txt)";
std::filesystem::path formatted_512kb		= R"(C:\Users\dmileykin\source\repos\generic_parser\include\examples\Json\formatted\512kb.txt)";
std::filesystem::path formatted_1mb			= R"(C:\Users\dmileykin\source\repos\generic_parser\include\examples\Json\formatted\1mb.txt)";
std::filesystem::path formatted_5mb			= R"(C:\Users\dmileykin\source\repos\generic_parser\include\examples\Json\formatted\5mb.txt)";

#include <benchmark/benchmark.h>

#include "calculator.h"

int main(int argc, char** argv)
{
	auto [calc_status, calc_res] = ParseLexeme2("23.0  - (1 * 2 -5)*(1 + 2 + 3) ", expression);
	std::string strInput = read_file(path_to_data.generic_string());
	auto [status, res] = ParseLexeme2(strInput, value);
	//auto [status, res] = ParseLexeme2(strjson, value);

	if(status)
		std::cout<<"success"<<std::endl;
	else
		std::cout<<"error"<<std::endl;
	
	//::testing::InitGoogleTest(&argc, argv);
	//return RUN_ALL_TESTS();
    int a = 1;
}