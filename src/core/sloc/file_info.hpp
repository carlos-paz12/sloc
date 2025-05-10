/**
 * @file file_info.hpp
 *
 * @brief Define a estrutura FileInfo para armazenar métricas de arquivos de código fonte.
 *
 * @author José Carlos da Paz Silva (carlos.paz.707@ufrn.edu.br)
 * @author Leandro Andrade (leandro.andrade.401@ufrn.edu.br)
 *
 * @version 0.1
 * @date 2025-05-09
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef FILE_INFO_HPP
#define FILE_INFO_HPP

#include "aliases.hpp"
#include "lang_type.hpp"

/// @brief Tipo inteiro para contagem de linhas.
using count_t = unsigned long;

/**
 * @struct FileInfo
 *
 * @brief Armazena informações de arquivo coletadas durante a análise
 *
 * @details Esta struct mantém detalhes sobre um arquivo sendo analisado pela ferramenta SLOC, incluindo nome, tipo de linguagem e estatísticas de
 *          linhas.
 */
struct FileInfo
{
public:
  str m_filename;               //!< Nome do arquivo (string)
  LangType m_type;              //!< Tipo de linguagem (enum: C, C++, header, etc.)
  count_t n_loc{ 0 };           //!< Contador de linhas de código (LOC)
  count_t n_reg_comments{ 0 };  //!< Contador de linhas de comentários regulares
  count_t n_doc_comments{ 0 };  //!< Contador de linhas de comentários de documentação
  count_t n_blank_lines{ 0 };   //!< Contador de linhas em branco
  count_t n_lines{ 0 };         //!< Contador do total de linhas no arquivo

  /**
   * @brief Construtor de FileInfo
   *
   * Inicializa as informações do arquivo com valores padrão opcionais.
   *
   * @param fn Nome do arquivo (string)
   * @param t Tipo de linguagem (LangType)
   */
  FileInfo(str fn = "", LangType ty = LangType::UNDEF) : m_filename{ std::move(fn) }, m_type{ ty } { /* empty*/ }

  bool operator==(const FileInfo& other) const { return m_filename == other.m_filename and m_type == other.m_type; }

  bool operator<(const FileInfo& other) const { return m_filename < other.m_filename; }

  FileInfo& operator+=(const FileInfo& other)
  {
    n_blank_lines += other.n_blank_lines;
    n_reg_comments += other.n_reg_comments;
    n_doc_comments += other.n_doc_comments;
    n_loc += other.n_loc;
    n_lines += other.n_lines;

    return *this;
  }
};

#endif  //!< FILE_INFO_HPP
