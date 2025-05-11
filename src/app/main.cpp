/*!
 * @file main.cpp
 *
 * @brief Source Lines Of Code (SLOC) para programas C/C++.
 *
 * @author José Carlos da Paz Silva (carlos.paz.707@ufrn.edu.br)
 * @author Leandro Andrade (leandro.andrade.401@ufrn.edu.br)
 *
 * @version 0.1
 * @date 2025-05-07
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <iomanip>   // `std::setw`
#include <iostream>  // `std::cout`
#include <sstream>   // `std::ostringstream`

#include "../common/aliases.hpp"
#include "../core/filter/field_option.hpp"
#include "../core/filter/filter.hpp"
#include "../core/options/running_options.hpp"
#include "../core/sloc/file_info.hpp"
#include "../core/sloc/sloc.hpp"
#include "../core/sort/sort.hpp"

const char* help_message = R"(Welcome to sloc cpp, version 1.0, (c) DIMAp/UFRN.

NAME
 sloc - single line of code counter


SYNOPSIS
 sloc [-h | --help] [-r] [(-s | -S) f|t|c|b|s|a] <file | directory>


EXAMPLES
 sloc main.cpp sloc.cpp
  Counts loc, comments, blanks of the source files 'main.cpp' and 'sloc.cpp'.

 sloc source
  Counts loc, comments, blanks of all C/C++ source files inside 'source'.

 sloc -r -s c source
  Counts loc, comments, blanks of all C/C++ source files recursively inside 'source'
  and sort the result in ascending order by # of comment lines.


DESCRIPTION
 Sloc counts the individual number **lines of code** (LOC), comments, and blank
lines found in a list of files or directories passed as the last argument
(after options).
 After the counting process is concluded the program prints out to the standard
output a table summarizing the information gathered, by each source file and/or
directory provided.
 It is possible to inform which fields sloc should use to sort the data by, as
well as if the data should be presented in ascending/descending numeric order.


OPTIONS
-h | --help                         Display this information.

-r                                  Look for files recursively in the directory provided

-s f|t|c|d|b|s|a                    Sort table in ASCENDING order by (f)ilename, (t) filetype,
                                    (c)omments, (d)oc comments, (b)lank lines, (s)loc, or (a)ll.
                                    Default is to show files in ordem of appearance.

-S f|t|c|d|b|s|a                    Sort table in DESCENDING order by (f)ilename, (t) filetype,
                                    (c)omments, (d)oc comments, (b)lank lines, (s)loc, or (a)ll.
                                    Default is to show files in ordem of appearance.
)";

void reset_stream(std::ostringstream& ss)
{
  ss.str("");
  ss.clear();
}

void usage(str_view msg = "")
{
  if (not msg.empty())
    std::cout << "\033[31mError: " << msg << ".\033[0m\n\n";
  std::cout << help_message;
  exit(EXIT_SUCCESS);
}

str get_language_name(LangType type)
{
  static const umap<LangType, str> lang_names{
    { LangType::C, "C" }, { LangType::H, "C/C++ header" }, { LangType::CPP, "C++" }, { LangType::HPP, "C++ header" }
  };

  return lang_names.at(type);
}

str get_option_name(FieldOption field)
{
  static const umap<FieldOption, str> fields_names{ { FieldOption::NONE, "NONE" },
                                                    { FieldOption::FILENAME, "FILENAME" },
                                                    { FieldOption::FILETYPE, "FILETYPE" },
                                                    { FieldOption::COMMENTS, "COMMENTS" },
                                                    { FieldOption::DOC_COMENTS, "DOC_COMENTS" },
                                                    { FieldOption::BLANK_LINES, "BLANK_LINES" },
                                                    { FieldOption::SLOC, "SLOC" },
                                                    { FieldOption::ALL, "ALL" } };

  return fields_names.at(field);
}

str format_percentage(const std::size_t& value, std::size_t& total)
{
  oss stream{};  //!< Stream de saída para construção da string formatada.

  // [!] Calcula a porcentagem (evita divisão por zero).
  double percentage{ total == 0 ? 0.0 : (static_cast<double>(value) * 100.0 / total) };
  // [!] Formata a saída como: "valor (porcentagem%)".
  stream << value << " (" << std::fixed << std::setprecision(1) << percentage << "%)";

  // [!] Retorna a string formatada.
  return stream.str();
}

void print_results_header(const std::size_t& max_filename_len, oss& table)
{
  table << "┌";
  for (size_t i{ 0 }; i < max_filename_len + 94; ++i)
  {
    table << "─";
  }
  table << "┐\n";

  table << "│ ";
  table << std::left << std::setw(max_filename_len + 2) << "Filename";
  table << std::setw(16) << "Language";
  table << std::setw(16) << "Comments";
  table << std::setw(16) << "Doc Comments";
  table << std::setw(16) << "Blank";
  table << std::setw(16) << "Code";
  table << std::setw(10) << "# of lines";
  table << " │\n";

  table << "├";
  for (size_t i{ 0 }; i < max_filename_len + 94; ++i)
  {
    table << "─";
  }
  table << "┤\n";

  std::cout << table.str();
  reset_stream(table);
}

void print_results_body(const RunningOptions& run_options, const std::size_t& max_filename_len, oss& table)
{
  for (const auto& file : run_options.sources)
  {
    size_t total_lines{ file.n_blank_lines + file.n_doc_comments + file.n_loc + file.n_reg_comments + 2 };

    table << "│ ";
    table << std::left << std::setw(max_filename_len + 2) << file.m_filename;
    table << std::setw(16) << get_language_name(file.m_type);
    table << std::setw(16) << format_percentage(file.n_reg_comments, total_lines);
    table << std::setw(16) << format_percentage(file.n_doc_comments, total_lines);
    table << std::setw(16) << format_percentage(file.n_blank_lines, total_lines);
    table << std::setw(16) << format_percentage(file.n_loc, total_lines);
    table << std::setw(10) << file.n_lines;
    table << " │\n";
  }

  std::cout << table.str();
  reset_stream(table);
}

void print_results_footer(const std::size_t& max_filename_len, std::ostringstream& table, FileInfo& sum_file)
{

  table << "├";
  for (size_t i{ 0 }; i < max_filename_len + 94; ++i)
  {
    table << "─";
  }
  table << "┤\n";

  table << "│ ";
  table << std::left << std::setw(max_filename_len + 2 + 16) << "SUM";
  table << std::setw(16) << sum_file.n_reg_comments;
  table << std::setw(16) << sum_file.n_doc_comments;
  table << std::setw(16) << sum_file.n_blank_lines;
  table << std::setw(16) << sum_file.n_loc;
  table << std::setw(10) << sum_file.n_lines;
  table << " │\n";

  table << "└";
  for (size_t i{ 0 }; i < max_filename_len + 94; ++i)
  {
    table << "─";
  }
  table << "┘\n";

  std::cout << table.str();
  reset_stream(table);
}

void print_results(const RunningOptions& run_options)
{
  // [!] Arquivo acumulador para totais gerais.
  FileInfo sum_file{};

  // [!] Calcula o comprimento máximo de nome de arquivo (começa com tamanho do cabeçalho).
  std::size_t max_filename_len{ str("Filename").size() + 2 };  // [!] +2 para margem.

  // [!] Stream para construção da tabela de saída.
  std::ostringstream table{};

  // [!] 1. Pré-processamento: Calcula totais e tamanhos.
  for (const auto& file : run_options.sources)
  {
    sum_file += file;                                                       // [!] Acumula estatísticas totais.
    max_filename_len = std::max(max_filename_len, file.m_filename.size());  // [!] Atualiza tamanho máximo.
  }

  // [!] 2. Cabeçalho geral.
  table << " Files processed: " << run_options.sources.size() << "\n";

  // [!] Se houver ordenação aplicada, mostra critério.
  if (run_options.sort_field != FieldOption::NONE)
  {
    table << " Sorting: " << (run_options.ascending ? "ASC" : "DESC") << " by " << get_option_name(run_options.sort_field) << '\n';
  }

  // HEADER {{{

  print_results_header(max_filename_len, table);  // [!] Printa o cabeçalho da tabela.
  // }}}

  // BODY {{{
  print_results_body(run_options, max_filename_len, table);  // [!] Printa o corpo da tabela.
  // }}}

  // FOOTER {{{
  print_results_footer(max_filename_len, table, sum_file);  // [!] Printa o rodapé da tabela.
  // }}}
}

void handle_sort_option(int argc, char* argv[], int& index, RunningOptions& run_options, const umap<char, FieldOption>& sort_map, oss& error_msg)
{
  // [!] Checa se existe um argumento após a flag de ordenação (-s ou -S).
  if (index + 1 >= argc)
  {
    // [!] Constrói mensagem de erro caso falte o argumento.
    error_msg << "Missing value for " << argv[index] << " option";
    // [!] Chama a função de ajuda com a mensagem de erro.
    usage(error_msg.str());
  }

  // [!] Determina se a ordenação é ascendente (-s) ou descendente (-S).
  run_options.ascending = (str(argv[index]) == "-s");
  // [!] Avança para o próximo argumento e obtém a string de campos de ordenação.
  const str sort_fields{ argv[++index] };

  // Processa cada caractere da string de campos de ordenação.
  for (const char field : sort_fields)
  {
    // [!] Verifica se o caractere está no mapa de campos válidos.
    auto it{ sort_map.find(field) };
    // [!] Se encontrou o campo no mapa...
    if (it != sort_map.end())
    {
      // [!] Armazena o campo de ordenação nas opções.
      run_options.sort_field = it->second;
    }
  }

  if (run_options.sort_field == FieldOption::NONE)
  {
    // [!] Constrói mensagem de erro para campo inválido.
    error_msg << "No valid sort field has been entered";
    // [!] Chama a função de ajuda com a mensagem de erro.
    usage(error_msg.str());
  }
}

RunningOptions parse_arguments(int argc, char* argv[])
{
  if (argc <= 1)  // [!] Chamada de programa sem argumentos.
    usage();

  RunningOptions run_options{};  //!< Encapsula as opções passadas por linha de comando.
  vec<str> input_sources{};      //!< Armazena arquivos e diretórios que o usuário quer processar.
  oss error_msg{};               //!< Monta mensagens de erro.

  //!< Mapa para ajudar a converter rapidamente a entrada do usuário para os enums que controlam como os resultados serão ordenados.
  umap<char, FieldOption> sort_map{ { 'f', FieldOption::FILENAME },    { 't', FieldOption::FILETYPE },    { 'c', FieldOption::COMMENTS },
                                    { 'd', FieldOption::DOC_COMENTS }, { 'b', FieldOption::BLANK_LINES }, { 's', FieldOption::SLOC },
                                    { 'a', FieldOption::ALL } };

  for (int i{ 1 }; i < argc; ++i)  // [!] O argumento 'argv[0]' é o nome do programa.
  {
    str arg{ argv[i] };  // [!] Recupera o argumento atual.

    if (arg == "-h" or arg == "--help")  // [!] Checa se opção de ajuda foi passada.
    {
      usage();  // [!] Mostra somente mensagem de ajuda.
    }
    else if (arg == "-r")  // [!] Checa se a opção de análise recursiva foi passada.
    {
      run_options.recursive = true;  // [!] Habilita a análise recursiva.
    }
    else if (arg == "-s" or arg == "-S")  // [!] Checa se opção de ordenação foi passada.
    {
      // [!] Lida com opções de ordenação.
      handle_sort_option(argc, argv, i, run_options, sort_map, error_msg);
    }
    else if (not arg.empty() and arg.at(0) == '-')  // [!] Checa se argumento é uma opção inválida.
    {
      error_msg << "Unknown option: " << arg;  // [!] Contrói mensagem de opção desconhecida.
      usage(error_msg.str());
    }
    else
    {
      // [!] Se o argumento não for alguma opção válida, campos de ordenação ou opção inválida, ele será um arquivo ou diretório.
      input_sources.push_back(arg);
    }

    reset_stream(error_msg);  // [!] Reinicializa stream.
  }

  // [!] Checa se não foram passados algum arquivo ou diretório.
  if (input_sources.empty())
  {
    usage("No input files or directories provided");
  }

  // [!] Coleta todos os arquivos válidos a partir dos caminhos fornecidos.
  run_options.sources = Filter::filter(input_sources, run_options.recursive);

  return run_options;
}

int main(int argc, char* argv[])
{
  std::cout << " Welcome to sloc cpp, version 1.0, (c) DIMAp/UFRN.\n\n";

  // #1 Analisar argumentos da linha de comando
  RunningOptions run_options{ parse_arguments(argc, argv) };

  /* [!]
   * Verifica se pelo menos um input do usuário foi considerado como arquivo válido.
   * Evita chamadas desnecessárias aos métodos principais do programa.
   */
  if (not run_options.sources.empty())  // [!] Verifica se algum arquivo foi validado. Evita chamadas des
  {
    // #2 Analisar cada arquivo.
    Sloc sloc_counter{};
    for (auto& file : run_options.sources)
    {
      sloc_counter.analyze_file(file);
    }

    // #3 Ordenar os arquivos se necessário.
    if (run_options.sort_field != FieldOption::NONE)
    {
      Sort::sortSloc(run_options.sources, run_options.sort_field, run_options);
    }

    // #4 Imprimir os resultados.
    print_results(run_options);
  }

  return EXIT_SUCCESS;
}
