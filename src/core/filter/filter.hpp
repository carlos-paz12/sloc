/**
 * @file filter.hpp
 *
 * @brief Classe para filtragem de arquivos.
 *
 * @author José Carlos da Paz Silva (carlos.paz.707@ufrn.edu.br)
 * @author Leandro Andrade (leandro.andrade.401@ufrn.edu.br)
 *
 * @version 0.1
 *
 * @date 2025-05-10
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef FILTER_HPP
#define FILTER_HPP

#include <algorithm>   // to `std::find`
#include <filesystem>  // to `std::filesystem::*`
#include <iomanip>     // to `std::quoted`
#include <iostream>    // to `std::cout`

#include "../common/aliases.hpp"       // to `unmap`, `str`, `vec`, `size_t`
#include "../core/sloc/file_info.hpp"  // to `FileInfo`
#include "../core/sloc/lang_type.hpp"  // to `LangType`

namespace fs = std::filesystem;  // Alias para facilitar uso de filesystem.

/*!
 * @brief Classe responsável por filtrar arquivos de acordo com suas extensões.
 *
 * Esta classe contém métodos para verificar se um arquivo é válido, se já foi adicionado
 * à lista de arquivos filtrados e para filtrar arquivos em um diretório.
 */
class Filter
{
private:
  //!< Mapa que relaciona extensões suportadas aos seus respectivos tipos de linguagem.
  const static inline umap<str, LangType> supported_extensions = {
    { ".c", LangType::C },
    { ".cpp", LangType::CPP },
    { ".hpp", LangType::HPP },
    { ".h", LangType::H },
  };

  /**
   * @brief  metodo que verifica se a extensão do arquivo é válida.
   *
   * @param file_extension  Extensão do arquivo a ser verificada.
   * @return true  se a extensão do arquivo for válida.
   * @return false caso o contrário.
   */
  static bool is_valid_file(const str& file_extension)
  {
    // [!] Usa `find` para verificar de `file_extension` faz parte das extensões suportadas.
    return supported_extensions.find(file_extension) != supported_extensions.end();
  }

  /**
   * @brief  metodo que verifica se o arquivo já foi adicionado à lista de arquivos filtrados.
   *
   * @param file  Arquivo a ser verificado.
   * @param filtered_files  Lista de arquivos filtrados.
   * @return true  se o arquivo já foi adicionado à lista.
   * @return false caso contrário.
   */
  static bool was_pushed(const FileInfo& file, const vec<FileInfo>& filtered_files)
  {
    // [!] Usa `std::find` para verificar se arquivo já foi adicionado na lista dos filtrados.
    return std::find(filtered_files.cbegin(), filtered_files.cend(), file) != filtered_files.end();
  }

  /**
   * @brief  metodo que tenta adicionar um arquivo à lista de arquivos filtrados.
   *
   * @param file  Arquivo a ser adicionado.
   * @param filtered_files  Lista de arquivos filtrados.
   * @return true  se o arquivo foi adicionado à lista.
   * @return false caso contrário.
   */
  static bool try_push_file(const fs::path& file, vec<FileInfo>& filtered_files)
  {
    /* [!]
     * Tenta adicionar um novo arquivo na lista.
     * Teremos sucesso quando o arquivo for válido e não já ter sido adicionado na lista (evitar duplicatas).
     */
    str file_extension{ file.extension() };  // [!] Recupera a extensão do arquivo.

    if (is_valid_file(file_extension))  // [!] Verifica se ele é válido.
    {
      // [!] Instancia um novo objeto `FileInfo` com as informações iniciais do arquivo.
      FileInfo file_info(file, supported_extensions.at(file_extension));

      if (not was_pushed(file_info, filtered_files))  // [!] Verifica se esse arquivo já foi adicionado na lista.
      {
        // [!] Se ele é duplicado, adiciona na lista.
        filtered_files.push_back(file_info);
        return true;
      }
    }
    return false;
  }

  /**
   * @brief  metodo que filtra arquivos em um diretório.
   *
   * @tparam IteratorType  Tipo do iterador a ser usado (recursivo ou não).
   * @param dir_root  Diretório raiz a ser filtrado.
   * @param filtered_files  Lista de arquivos filtrados.
   * @return size_t  Número de arquivos adicionados à lista.
   */
  template <typename IteratorType>
  static size_t filter_files_in_directory(const fs::path& dir_root, vec<FileInfo>& filtered_files)
  {
    size_t n_files_pushed{};  //!< Armazena quantos arquivos do diretório `dir_root` foram adicionados na lista.

    for (const auto& entry : IteratorType(dir_root))
    {
      // [!] Se `entry` for um arquivo e tiver sido adicionado na lista, incrementa o contador.
      if (fs::is_regular_file(entry))
      {
        n_files_pushed += try_push_file(entry, filtered_files) ? 1 : 0;
      }
    }

    return n_files_pushed;
  }

public:
  /**
   * @brief  metodo que filtra arquivos a partir de uma lista de entradas.
   *
   * @details  Este método verifica se as entradas são arquivos ou diretórios e filtra os arquivos válidos.
   * Se uma entrada for um diretório, ele filtra os arquivos dentro dele (recursivamente ou não).
   * Se uma entrada for um arquivo, ele tenta adicioná-lo diretamente à lista de arquivos filtrados.
   * Se uma entrada não for um arquivo ou diretório válido, uma mensagem de erro é exibida.
   * Se uma entrada não existir, uma mensagem de erro é exibida.
   *
   * @param input_sources  Lista de entradas (arquivos ou diretórios) a serem filtradas.
   * @param recursive  Se `true`, filtra arquivos recursivamente em diretórios.

   * @return vec<FileInfo>  Lista de arquivos filtrados.
   */
  static vec<FileInfo> filter(const vec<str>& input_sources, const bool& recursive)
  {
    vec<FileInfo> filtered_files{};  //!< Vetor de arquivos filtrados.

    for (const auto& input : input_sources)
    {
      if (fs::exists(input))  //[!] Verifica se o input do usuário representa um caminho real do sistema de arquivos.
      {
        fs::path entry(input);  //!< Variável para arquivo/diretório.

        if (fs::is_directory(entry))
        {
          size_t pusheds{ 0 };  //!< Arquivos totais que foram adicionados do diretório.

          // [!] Itera sobre o diretório recursivamente (ou não) e conta os arquivos adicionados.
          pusheds = recursive ? filter_files_in_directory<fs::recursive_directory_iterator>(entry, filtered_files)
                              : filter_files_in_directory<fs::directory_iterator>(entry, filtered_files);

          if (pusheds == 0)  // [!] Exibe mensagem de alerta caso o diretório não tenha arquivos válidos.
          {
            std::cout << entry << ": Sorry, no supported source files found in directory.\n";
          }
        }
        // [!] Se `entry` não for um diretório, mas for um arquivo, tenta adicionar direto na lista.
        else if (fs::is_regular_file(entry))
        {
          str entry_extension{ entry.extension() };  // [!] Recupera a extensão do arquivo.
          if (is_valid_file(entry_extension))
          {
            // [!] Como não é um diretório, tenta adicionar na lista
            try_push_file(entry, filtered_files);
          }
          else
          {
            // [!] Se não for válido, exibe uma mensagem de alerta.
            if (not entry_extension.empty())
            {
              std::cout << entry << ": Sorry, " << std::quoted(entry_extension) << " files are not supported at this time.\n";
            }
            else
            {
              std::cout << entry << ": Sorry, this type of files are not supported at this time.\n";
            }
          }
        }
        else
        {
          // [!] Alerta ao usuário que a entrada não é arquivo nem diretório.
          std::cout << std::quoted(input) << ": Sorry, this isn't a file or directory.\n";
        }
      }
      else
      {
        // [!] Alerta ao usuário que entrada não existe.
        std::cout << std::quoted(input) << ": Sorry, no such file or directory.\n";
      }
    }

    return filtered_files;
  }
};

#endif  //!< FILTER_HPP
